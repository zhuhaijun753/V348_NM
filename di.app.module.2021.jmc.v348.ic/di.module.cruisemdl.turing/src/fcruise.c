/*============================================================================
**
**                     CONFIDENTIAL VISTEON CORPORATION
**
** This is an unpublished work of authorship, which contains trade secrets,
** created in 2003. Visteon Corporation owns all rights to this work and
** intends to maintain it in confidence to preserve its trade secret status.
** Visteon Corporation reserves the right, under the copyright laws of the
** United States or those of any other country that may have jurisdiction,
** to protect this work as an unpublished work, in the event of an
** inadvertent or deliberate unauthorized publication. Visteon Corporation
** also reserves its rights under all copyright laws to protect this work as
** a published work, when appropriate. Those having access to this work may
** not copy it, use it, modify it or disclose the information contained in
** it without the written authorization of Visteon Corporation.
**
**============================================================================
**
** Name:           fcruise.c
**
** Description:    Implementation module for Cruise Speed
**
** Organization:   Driver Information Software Section,
**                 DI Core Engineering Department
**
**============================================================================
**Compiler Name    : Metrowerks 
******************************************************************************/

#define FCRUISE_C

/*============================================================================
** I N C L U D E   F I L E S
**==========================================================================*/
//#include "fcruise.h"
//#include "fstmgr_ps.h"
//#include "sched.h"
//#include "fwarning.h"

#include "Rte.h"
#include "Rte_type.h"
#include "Rte_CruiseMdl.h"
#include "CmpLib.h"
#include "fcruise.h"



/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(CruiseMdl)

/*============================================================================
** I N T E R N A L   M A C R O   A N D   T Y P E   D E F I N I T I O N S
**==========================================================================*/
#define CALLED_PERIOD				  ((UINT32)50) //function called period 50ms
#define cTIMER_1SEC  				  (UINT16)1000u
#define SPEED_MAX_CAN_MESSAGE_VALUE   (UINT32)30000u
#define SPEED_MIN_CAN_MESSAGE_VALUE   (UINT32)200u

#define CRUISE_OFF                    (UINT16)0x08
#define CRUISE_PREPARE                (UINT16)0x04
#define CRUISE_ACTIVE                 (UINT16)0x02
#define CRUISE_ACCELERATION_OVERRIDE  (UINT16)0x01

#define CRUISE_MIN_SPEED 4000
#define CRUISE_MAX_SPEED 17500

#define CRUISE_SPEED_LONG_PRESS_STEP 500
#define CRUISE_SPEED_SHORT_PRESS_STEP 100

#define CRUISE_SPEED_BASE 100
#define CRUISE_SPEED_FACTOR 103
#define CRUISE_SPEED_SHIFT 250

#define CRUISE_MAXIMUM_SPEED 30000

#define ALERET                (UINT8)0
#define NO_CRUISE_CONTROL     (UINT8)1
#define CRUISE_CONTROL_ACTIVE (UINT8)2
#define ACCELERATION_OVERRIDE (UINT8)3
#define DEC_CONSIGNE          (UINT8)4
#define FIGEAGE_REGULATION    (UINT8)5
#define FIGEAGE_ACCELERATION  (UINT8)6
#define DECROISSANCE_SECURITE (UINT8)7

typedef BOOLEAN (*const pPMCndFunc)(void);
typedef struct cruise_state_tbl_struct
{
    UINT16     CurrCruiseState;
    pPMCndFunc PMCndFunc_p;
    UINT16     NextCruiseState;
} CRUISE_STATE_TBL_S;

typedef struct Cruise_state_trans_struct
{
    UINT16 l_current_cruise_state_U16;
    UINT16 l_next_cruise_state_U16;
    void(*cruise_state_transition_func_FP)(void);
}CRUISE_STATE_TRANS_S;

/*============================================================================
** I N T E R N A L   F U N C T I O N   P R O T O T Y P E S
**==========================================================================*/
static void fcruise_common_init (void);
static void execute_cruise_state_transitions(void);
static void check_cruise_state_transitions(void);

static void Cruise_ProcessOff2Off(void);
static void Cruise_ProcessOff2Prepare(void);
static void Cruise_ProcessPrepare2Off(void);
static void Cruise_ProcessPrepare2Active(void);
static void Cruise_ProcessActive2Off(void);
static void Cruise_ProcessActive2Prepare(void);
static void Cruise_ProcessActive2Override(void);
static void Cruise_ProcessOverride2Off(void);
static void Cruise_ProcessOverride2Prepare(void);
static void Cruise_ProcessOverride2Active(void);

static BOOLEAN transOff2Off(void);
static BOOLEAN transOff2Prepare(void);
static BOOLEAN transPrepare2Off(void);
static BOOLEAN transPrepare2Active(void);
static BOOLEAN transActive2Off(void);
static BOOLEAN transActive2Prepare(void);
static BOOLEAN transActive2Override(void);
static BOOLEAN transOverride2Off(void);
static BOOLEAN transOverride2Prepare(void);
static BOOLEAN transOverride2Active(void);
static void reset_All_Flags(void);

static void fcruise_IDTT(void);
static void fcruise_timedtask(void);
static BOOLEAN IsSpeedInvalid(void);


static UINT8 l_IGN_State; 
static UINT32 l_last_cruise_active_speed;
static UINT8 l_EMS_CruiseCtrlSts;
static UINT32 l_IC_TargetCruis;
static UINT32 l_IC_TargetCruis_Display;
static BOOLEAN l_IC_CruiseSwitch_BOOL;
static UINT16 l_curr_CruiseState;
static UINT16 l_next_CruiseState;
static UINT32 l_minus_button_long_press_timer;
static UINT32 l_plus_button_long_press_timer;

static FCRUISE_TITLE_T l_cruise_telltale_state;
static FCRUISE_SPEED_T l_cruise_speed_state;
static BOOLEAN l_cruise_sts_missing_BOOL;
static BOOLEAN l_currSpeed_invalid_BOOL;
static BOOLEAN l_currSpeed_missing_BOOL;

static BOOLEAN l_cruise_has_been_active_before_BOOL;

static UI_CRUISE_CONTROL_STATUS_T l_cruise_button_state;
static UI_CRUISE_CONTROL_OPTION_T l_cruise_option_button_state;

static const CRUISE_STATE_TBL_S cruise_tbl_array_SA[] =
{
   { CRUISE_OFF,                    transOff2Off,          CRUISE_OFF },
   { CRUISE_OFF,                    transOff2Prepare,      CRUISE_PREPARE },
   { CRUISE_PREPARE,                transPrepare2Off,      CRUISE_OFF },
   { CRUISE_PREPARE,                transPrepare2Active,   CRUISE_ACTIVE },
   { CRUISE_ACTIVE,                 transActive2Off,       CRUISE_OFF },
   { CRUISE_ACTIVE,                 transActive2Prepare,   CRUISE_PREPARE },
   { CRUISE_ACTIVE,                 transActive2Override,  CRUISE_ACCELERATION_OVERRIDE },
   { CRUISE_ACCELERATION_OVERRIDE,  transOverride2Off,     CRUISE_OFF },
   { CRUISE_ACCELERATION_OVERRIDE,  transOverride2Prepare, CRUISE_PREPARE },
   { CRUISE_ACCELERATION_OVERRIDE,  transOverride2Active,  CRUISE_ACTIVE }
};

static const CRUISE_STATE_TRANS_S cruise_state_transition_array_SA[] = 
{
    {CRUISE_OFF,                   CRUISE_OFF ,                   &Cruise_ProcessOff2Off},\
    {CRUISE_OFF,                   CRUISE_PREPARE,                &Cruise_ProcessOff2Prepare},\
    {CRUISE_PREPARE,               CRUISE_OFF,                    &Cruise_ProcessPrepare2Off},\
    {CRUISE_PREPARE,               CRUISE_ACTIVE,                 &Cruise_ProcessPrepare2Active},\
    {CRUISE_ACTIVE,                CRUISE_OFF,                    &Cruise_ProcessActive2Off},\
    {CRUISE_ACTIVE,                CRUISE_PREPARE,                &Cruise_ProcessActive2Prepare},\
    {CRUISE_ACTIVE,                CRUISE_ACCELERATION_OVERRIDE,  &Cruise_ProcessActive2Override},\
    {CRUISE_ACCELERATION_OVERRIDE, CRUISE_OFF,                    &Cruise_ProcessOverride2Off},\
    {CRUISE_ACCELERATION_OVERRIDE, CRUISE_PREPARE,                &Cruise_ProcessOverride2Prepare},\
    {CRUISE_ACCELERATION_OVERRIDE, CRUISE_ACTIVE,                 &Cruise_ProcessOverride2Active},\
    {0, 0, ((void (*)(void))0) }
};

/*============================================================================
** M E M O R Y   A L L O C A T I O N
**==========================================================================*/
/*============================================================================
** E N T R Y   P O I N T S  /  D A T A   A C C E S S   S E R V I C E S
**==========================================================================*/


//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	fcruise_common_init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
	fcruise_common_init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation( void )
{
	fcruise_common_init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation( void )
{
	fcruise_common_init();
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
/****************************************************************************
Function Name     	: CmpActive

Description         : This function process the oilpressure TT ON/OFF for
					  ISG /NON ISG Variant.

Invocation          : called in cmpActive.

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static Std_ReturnType CmpActive( void )
{
	boolean IsCfg = FALSE;
	Rte_Read_tiSR_GdtCtrl_CC_Cfg(&IsCfg);
	if(FALSE != IsCfg)
	{
		fcruise_timedtask();
	}
	else
	{
		fcruise_common_init();
	}

	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Diagnostic state
///
/// @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}

/*============================================================================
** Function Name     : fcruise_common_init
**
** Description       : Clears the Warning State hold variable.
**        
** Invocation        : Invoked by the Scheduler during cold Init
**
** Parameters        : None.
**
** Return Type       : None.
**
** Critical Section  : None.
**
**==========================================================================*/
static void fcruise_common_init (void)
{
    l_IGN_State = (UINT8)(eIGN_OFF);
    l_last_cruise_active_speed = (UINT32)0;
    l_EMS_CruiseCtrlSts = (UINT8)0;
    l_IC_TargetCruis = (UINT32)0;
    l_IC_TargetCruis_Display = (UINT32)0;
    l_IC_CruiseSwitch_BOOL = FALSE;
    l_curr_CruiseState = (UINT16)(CRUISE_OFF);
    l_next_CruiseState = (UINT16)(CRUISE_OFF);
    l_minus_button_long_press_timer = (UINT32)0;
    l_plus_button_long_press_timer = (UINT32)0;
    l_cruise_telltale_state = CRUISE_ICON_HIDE;
    l_cruise_speed_state = CRUISE_SPEED_NOT_DISPLAY;
    l_cruise_sts_missing_BOOL = FALSE;
    l_currSpeed_invalid_BOOL = FALSE;
    l_currSpeed_missing_BOOL = FALSE;
    l_cruise_has_been_active_before_BOOL = FALSE;
    l_cruise_button_state = UI_CRUCON_OFF;
    l_cruise_option_button_state = UI_CRUCON_OPTION;
    
}


/*============================================================================
**
** Function Name:       void fcruise_IDTT(void)
**
** Visibility:          This function is called  every 50 ms.
**                      it is called as if it were an interrupt
**
** Description:         This function will add or subtract the cruise speed when SET/- or RES/+ was long pressed.
**
** Invocation:          This function is called  every 50 ms
**
** Inputs/Outputs:      none
**
** Critical Section:    none
**==========================================================================*/
static void fcruise_IDTT(void)
{
    if (CRUISE_ACTIVE == l_curr_CruiseState)
    {
        if (UI_CRUCON_SUB_LONG == l_cruise_option_button_state)
        {
            /*2129-Set IC_CruiseSwitch=1,biased IC_TargetCruis decrease by 5 km/h*/
            l_IC_CruiseSwitch_BOOL = TRUE;

            l_minus_button_long_press_timer += CALLED_PERIOD;
            if ((UINT32)1000 <= l_minus_button_long_press_timer)
            {
                l_minus_button_long_press_timer = (UINT32)0;
                if ((l_IC_TargetCruis_Display - (UINT32)CRUISE_MIN_SPEED) >= CRUISE_SPEED_BASE)
                {
                    if ((l_IC_TargetCruis_Display % (UINT32)CRUISE_SPEED_LONG_PRESS_STEP) != (UINT32)0)
                    {
                        l_IC_TargetCruis_Display = ((l_IC_TargetCruis_Display + (UINT32)CRUISE_SPEED_LONG_PRESS_STEP) / (UINT32)CRUISE_SPEED_LONG_PRESS_STEP) * (UINT32)CRUISE_SPEED_LONG_PRESS_STEP;
                    }
                    l_IC_TargetCruis_Display -= (UINT32)CRUISE_SPEED_LONG_PRESS_STEP;
                    l_IC_TargetCruis = (l_IC_TargetCruis_Display - CRUISE_SPEED_SHIFT)*CRUISE_SPEED_BASE/CRUISE_SPEED_FACTOR;
                    l_last_cruise_active_speed = l_IC_TargetCruis_Display;
                    //fw_set_warning_status(W_LOWEST_CRUISE_SPEED_HAS_RACHED, WARNING_NOT_DISPLAY);
                }
                else
                {
                    /*When biased IC_TargetCruis<=40km/h, the target cruise speed shall NOT decrease anymore and cluster shall pop up warning message "Has reached lowest cruising speed"*/
                    //fw_set_warning_status(W_LOWEST_CRUISE_SPEED_HAS_RACHED, WARNING_DISPLAY);
                }
            }

            /*Cruise control telltale shall be displayed with green color*/
            l_cruise_telltale_state = CRUISE_ICON_GREEN;
            /*Cruise speed shall be displayed with biased IC_TargetCruis value*/
        }
        else
        {
            l_minus_button_long_press_timer = (UINT32)0;
        }

        if (UI_CRUCON_ADD_LONG == l_cruise_option_button_state)
        {
            /* Set IC_CruiseSwitch=1,biased IC_TargetCruis increase by 1km/h*/
            l_IC_CruiseSwitch_BOOL = TRUE;

            l_plus_button_long_press_timer += CALLED_PERIOD;

            if ((UINT32)1000 <= l_plus_button_long_press_timer)
            {
                l_plus_button_long_press_timer = (UINT32)0;

                if (l_IC_TargetCruis_Display < (UINT32)CRUISE_MAX_SPEED)
                {
                    if ((l_IC_TargetCruis_Display % (UINT32)CRUISE_SPEED_LONG_PRESS_STEP) != (UINT32)0)
                    {
                        l_IC_TargetCruis_Display = (l_IC_TargetCruis_Display / (UINT32)CRUISE_SPEED_LONG_PRESS_STEP) * (UINT32)CRUISE_SPEED_LONG_PRESS_STEP;
                    }
                    l_IC_TargetCruis_Display += (UINT32)CRUISE_SPEED_LONG_PRESS_STEP;
                    l_IC_TargetCruis = (l_IC_TargetCruis_Display - CRUISE_SPEED_SHIFT)*CRUISE_SPEED_BASE/CRUISE_SPEED_FACTOR;
                    l_last_cruise_active_speed = l_IC_TargetCruis_Display;
                    //fw_set_warning_status(W_HIGHEST_CRUISE_SPEED_HAS_RACHED, WARNING_NOT_DISPLAY);
                }
                else
                {
                    /*When biased IC_TargetCruis>=175km/h, the target cruise speed shall NOT increase anymore and cluster shall pop up warning message "Has reached highest cruising speed"*/
                    //fw_set_warning_status(W_HIGHEST_CRUISE_SPEED_HAS_RACHED, WARNING_DISPLAY);
                }
            }

            /*Cruise control telltale shall be displayed with green color*/
            l_cruise_telltale_state = CRUISE_ICON_GREEN;
            /*Cruise speed shall be displayed with biased IC_TargetCruis value*/
        }
        else
        {
            l_plus_button_long_press_timer = (UINT32)0;
        }
    }
    else
    {
        l_minus_button_long_press_timer = (UINT32)0;
        l_plus_button_long_press_timer = (UINT32)0;
    }
}

/*============================================================================
**
** Function Name:      void fcruise_KSRRobin(void)
**
** Visibility:         global via ficc.h
**
** Description:        Round robin for Cruise speed, Unit and Icon Indicator display feature
**
** Invocation:         This function is called from DI KERNEL Round Robin List
**
** Inputs/Outputs:     None
**
** Critical Section:   None
**
**==========================================================================*/
static void fcruise_timedtask(void)
{

	UINT8 u8Pt2MsgNr = 0;
	UINT8 u8Pt2MsgTout = 0;	
	UINT8 u8CruiseCtrlSts = 0;	

    l_cruise_button_state = get_cruise_control_status();
	l_currSpeed_invalid_BOOL = IsSpeedInvalid();	
	Rte_Read_rpSR_CANMSG_GW_PT2_ComIn_NR(&u8Pt2MsgNr);
	Rte_Read_rpSR_CANMSG_GW_PT2_ComIn_Tout(&u8Pt2MsgTout);
	Rte_Read_rpSR_CANMSG_GW_PT2_ComIn_EMS_CruiseCtrlSts(&u8CruiseCtrlSts);

	if((RTE_E_NEVER_RECEIVED == u8Pt2MsgNr)||(RTE_E_TIMEOUT == u8Pt2MsgTout))
	{
		l_cruise_sts_missing_BOOL = TRUE;
	}
	else
	{
		l_cruise_sts_missing_BOOL = FALSE;
	}

	l_EMS_CruiseCtrlSts = u8CruiseCtrlSts;

    Rte_Read_rpIgnState_IGNState(&l_IGN_State);
    check_cruise_state_transitions();

    if (CRUISE_OFF == l_curr_CruiseState)
    {
        if (l_cruise_button_state != UI_CRUCON_ON)
        {
            l_IC_CruiseSwitch_BOOL = FALSE;
            l_IC_TargetCruis_Display = l_IC_TargetCruis;
            /*Cruise control telltale shall NOT be displayed*/
            l_cruise_telltale_state = CRUISE_ICON_HIDE;
            /*Cruise control speed shall NOT be displayed*/
            l_cruise_speed_state = CRUISE_SPEED_NOT_DISPLAY;
        }
        //fw_set_warning_status(W_CRUISE_CONTROL_NOT_ENABLED, WARNING_NOT_DISPLAY);
        //fw_set_warning_status(W_CRUISE_SPEED_TOO_LOW, WARNING_NOT_DISPLAY);
        //fw_set_warning_status(W_CRUISE_SPEED_TOO_HIGH, WARNING_NOT_DISPLAY);
        //fw_set_warning_status(W_LOWEST_CRUISE_SPEED_HAS_RACHED, WARNING_NOT_DISPLAY);
        //fw_set_warning_status(W_HIGHEST_CRUISE_SPEED_HAS_RACHED, WARNING_NOT_DISPLAY);
    }
    else if (CRUISE_PREPARE == l_curr_CruiseState)
    {
        /*2113-In cruise control prepare state, IC needs judge whether cruise control
        enters Cruise Control Active state before this moment,if cruise control enters
        Cruise Active status before this moment, the cruise speed of active state shall
        be displayed, otherwise, IC should not display the cruise speed.*/
        
        if ((TRUE == l_IC_CruiseSwitch_BOOL) && ((ALERET == l_EMS_CruiseCtrlSts) || (DECROISSANCE_SECURITE == l_EMS_CruiseCtrlSts)))
        {
            /*2120-Warning message "Cruise control is not enabled" shall be poped up*/
            //fw_set_warning_status(W_CRUISE_CONTROL_NOT_ENABLED, WARNING_DISPLAY);
            /*Set IC_CruiseSwitch=0,IC_TargetCruis =Random value*/
            l_IC_CruiseSwitch_BOOL = FALSE;
            /*Cruise control telltale shall be displayed with gray color*/
            l_cruise_telltale_state = CRUISE_ICON_GRAY;

            if (TRUE == l_cruise_has_been_active_before_BOOL)
            {
                l_IC_TargetCruis_Display = l_last_cruise_active_speed;
                l_cruise_speed_state = CRUISE_SPEED_DISPLAY;
            }
            else
            {
                l_cruise_speed_state = CRUISE_SPEED_NOT_DISPLAY;
            }
        }
        else
        {
            //fw_set_warning_status(W_CRUISE_CONTROL_NOT_ENABLED, WARNING_NOT_DISPLAY);
        }

        //fw_set_warning_status(W_LOWEST_CRUISE_SPEED_HAS_RACHED, WARNING_NOT_DISPLAY);
        //fw_set_warning_status(W_HIGHEST_CRUISE_SPEED_HAS_RACHED, WARNING_NOT_DISPLAY);
    }
    else if (CRUISE_ACTIVE == l_curr_CruiseState)
    {
        l_last_cruise_active_speed = l_IC_TargetCruis_Display;
        //fw_set_warning_status(W_CRUISE_CONTROL_NOT_ENABLED, WARNING_NOT_DISPLAY);
        //fw_set_warning_status(W_CRUISE_SPEED_TOO_LOW, WARNING_NOT_DISPLAY);
        //fw_set_warning_status(W_CRUISE_SPEED_TOO_HIGH, WARNING_NOT_DISPLAY);
    }
    else
    {
        //fw_set_warning_status(W_CRUISE_CONTROL_NOT_ENABLED, WARNING_NOT_DISPLAY);
        //fw_set_warning_status(W_CRUISE_SPEED_TOO_LOW, WARNING_NOT_DISPLAY);
        //fw_set_warning_status(W_CRUISE_SPEED_TOO_HIGH, WARNING_NOT_DISPLAY);
        //fw_set_warning_status(W_LOWEST_CRUISE_SPEED_HAS_RACHED, WARNING_NOT_DISPLAY);
        //fw_set_warning_status(W_HIGHEST_CRUISE_SPEED_HAS_RACHED, WARNING_NOT_DISPLAY);
    }

	fcruise_IDTT();
	Rte_Write_ppSR_CANMSG_IC_ComOut_IC_CruiseSwitch(l_IC_CruiseSwitch_BOOL);
	Rte_Write_ppSR_CANMSG_IC_ComOut_IC_TargetCruiseVehS(l_IC_TargetCruis);
	Rte_Write_ppSR_WriteCruise_Flash(l_cruise_speed_state);
	Rte_Write_ppSR_WriteCruise_Icon(l_cruise_telltale_state);
	Rte_Write_ppSR_WriteCruise_Speed(l_IC_TargetCruis_Display);
	
	
}

static BOOLEAN IsSpeedInvalid(void)
{
	//UINT8 u8Esc2MsgNr = 0;
	//UINT8 u8Esc2MsgTout = 0;
	//UINT8 u8VehSpdValidFlag = 0;
	//UINT16 u8VehSpd = 0;
	//BOOLEAN bIsTimerStartSt = FALSE;
	//BOOLEAN bIsElapsedSt = FALSE;
	BOOLEAN bIsInvalid = FALSE;
	//static BOOLEAN bIsTimeOut = FALSE;
#if 0
	Rte_Read_rpR_CANMSG_GW_ESC2_ComIn_NR(&u8Esc2MsgNr);
	Rte_Read_rpR_CANMSG_GW_ESC2_ComIn_Tout(&u8Esc2MsgTout);
	Rte_Read_rpR_CANMSG_GW_ESC2_ComIn_VehSpdValidFlag(&u8VehSpdValidFlag);
	Rte_Read_rpR_CANMSG_GW_ESC2_ComIn_ESC_VehSpd(&u8VehSpd);
	if((RTE_E_NEVER_RECEIVED == u8Esc2MsgNr)||(RTE_E_TIMEOUT == u8Esc2MsgTout))
	{
		l_currSpeed_missing_BOOL = TRUE;
		u8VehSpdValidFlag = 1;
	}
	else
	{
		l_currSpeed_missing_BOOL = FALSE;
	}

	if(0 != u8VehSpdValidFlag)
	{
		u8VehSpd = 0;
	}

    if((u8VehSpd > SPEED_MAX_CAN_MESSAGE_VALUE) ||(u8VehSpd < SPEED_MIN_CAN_MESSAGE_VALUE) )
    {
    	if(TRUE == bIsTimeOut)
    	{
			bIsInvalid = TRUE;
		}
		else
		{
	        Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerCruiseMdl,&bIsTimerStartSt);
	        if(FALSE == bIsTimerStartSt)
	        {
				Rte_Call_rptimer_TmExt_Timer_Start(eTimerCruiseMdl);
				bIsInvalid = FALSE;
				bIsTimeOut = FALSE;
	        }
	        else
	        {
	        	Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerCruiseMdl,cTIMER_1SEC,&bIsElapsedSt);
				if(TRUE == bIsElapsedSt)
				{
					bIsInvalid = TRUE;
					bIsTimeOut = TRUE;
				}
				else
				{
					bIsInvalid = FALSE;
					bIsTimeOut = FALSE;
				}
	        }
		}
    }
    else
    {

	 	Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerCruiseMdl,&bIsTimerStartSt);
		if(TRUE == bIsTimerStartSt)
        {
			Rte_Call_rptimer_TmExt_Timer_Stop(eTimerCruiseMdl);
        }
		bIsInvalid = FALSE;
		bIsTimeOut = FALSE;
    }
#endif
	return bIsInvalid;
}



/*****************************************************************************
Function Name    : CCruiseMdl_Event
Description      : To handle the trip switch user event and take required action
Invocation       : LSH when the trip switch event occurs
Parameters       : p_button_event_U8 - Button Event
Return Value     : None
Critical Section : None
*****************************************************************************/
void CCruiseMdl_Event(UInt8 p_button_event_U8)
{
	cruise_event_handler(p_button_event_U8);
}

/****************************************************************************
Function Name     : processButton

Description       : process the operations when the corresponding button was pressed

Invocation       : Invoked by Required modules

Parameters        : buttonType, shows which button was pressed and also shows long press or 
                           short press.

Return Value      :None

Critical Section  :None

******************************************************************************/
void processButton(UI_CRUISE_CONTROL_OPTION_T buttonType)
{
    l_cruise_option_button_state = buttonType;
    switch(l_curr_CruiseState)
    {
    case CRUISE_OFF:
        break;
    case CRUISE_PREPARE:
        {
            UINT32 fl_current_indicator_speed;
            Rte_Read_tiSR_TISpdMdl_value(&fl_current_indicator_speed);
            if ((fl_current_indicator_speed >= (UINT32)CRUISE_MIN_SPEED) && (fl_current_indicator_speed <= (UINT32)CRUISE_MAX_SPEED))
            {
                if ((UI_CRUCON_SUB_SHORT == l_cruise_option_button_state) || (UI_CRUCON_SUB_LONG == l_cruise_option_button_state))
                {
                    l_IC_CruiseSwitch_BOOL = TRUE;
                    /*2117-IC_TargetCruis= current speed Vraw*/
                    Rte_Read_rpSR_TISpdMdl_Real_Value(&l_IC_TargetCruis);
                    l_IC_TargetCruis_Display = l_IC_TargetCruis * CRUISE_SPEED_FACTOR / CRUISE_SPEED_BASE + CRUISE_SPEED_SHIFT;
                    /*Cruise control telltale shall be displayed with gray color*/
                    l_cruise_telltale_state = CRUISE_ICON_GRAY;
                    l_cruise_speed_state = CRUISE_SPEED_DISPLAY;
                    //fw_set_warning_status(W_CRUISE_SPEED_TOO_LOW, WARNING_NOT_DISPLAY);
                    //fw_set_warning_status(W_CRUISE_SPEED_TOO_HIGH, WARNING_NOT_DISPLAY);
                }
                else if((UI_CRUCON_ADD_SHORT == l_cruise_option_button_state) || (UI_CRUCON_ADD_LONG == l_cruise_option_button_state))
                {
                    if (TRUE == l_cruise_has_been_active_before_BOOL)
                    {
                        /*2119-IC_CruiseSwitch=1,IC_TargetCruis= target cruise speed in last cruise active state*/
                        l_IC_CruiseSwitch_BOOL = TRUE;
                        l_IC_TargetCruis_Display = l_last_cruise_active_speed;
                        l_IC_TargetCruis = (l_IC_TargetCruis_Display - CRUISE_SPEED_SHIFT) * CRUISE_SPEED_BASE / CRUISE_SPEED_FACTOR;
                        /* Cruise Speed shall be displayed target cruise speed in last cruise active state*/
                        l_cruise_speed_state = CRUISE_SPEED_DISPLAY;
                    }
                    else
                    {
                        /*Set IC_CruiseSwitch=0,IC_TargetCruis= Random value*/
                        l_IC_CruiseSwitch_BOOL = FALSE;
                        /*Cruise Speed shall NOT be displayed*/
                        l_cruise_speed_state = CRUISE_SPEED_NOT_DISPLAY;
                    }
                    /*Cruise control telltale shall be displayed with gray color*/
                    l_cruise_telltale_state = CRUISE_ICON_GRAY;
                    //fw_set_warning_status(W_CRUISE_SPEED_TOO_LOW, WARNING_NOT_DISPLAY);
                    //fw_set_warning_status(W_CRUISE_SPEED_TOO_HIGH, WARNING_NOT_DISPLAY);
                }
            }
            else
            {
                if ((UI_CRUCON_SUB_SHORT == l_cruise_option_button_state)
                || (UI_CRUCON_SUB_LONG == l_cruise_option_button_state)
                || (UI_CRUCON_ADD_SHORT == l_cruise_option_button_state)
                || (UI_CRUCON_ADD_LONG == l_cruise_option_button_state))
                {
                    /*2116/2118-
                    1. Warning message "cruise speed is too low/high" shall be poped up;
                    2. Send IC_CruiseSwitch=0,IC_TargetCruiseVehSpd =Random value;
                    3. Cruise control telltale shall be displayed with gray color.
                    */
                    if(fl_current_indicator_speed < (UINT32)CRUISE_MIN_SPEED)
                    {
                        //fw_set_warning_status(W_CRUISE_SPEED_TOO_LOW, WARNING_DISPLAY);
                    }
                    else
                    {
                        //fw_set_warning_status(W_CRUISE_SPEED_TOO_LOW, WARNING_NOT_DISPLAY);
                    }

                    if (fl_current_indicator_speed > (UINT32)CRUISE_MAX_SPEED)
                    {
                        //fw_set_warning_status(W_CRUISE_SPEED_TOO_HIGH, WARNING_DISPLAY);
                    }
                    else
                    {
                        //fw_set_warning_status(W_CRUISE_SPEED_TOO_HIGH, WARNING_NOT_DISPLAY);
                    }
                    l_cruise_telltale_state = CRUISE_ICON_GRAY;
                    l_IC_CruiseSwitch_BOOL = FALSE;
                }
            }
            break;
        }
    case CRUISE_ACTIVE:
        {
            if (UI_CRUCON_CANCEL == l_cruise_option_button_state)
            {
                /*2127-Send IC_CruiseSwitch=0, IC_TargetCruiseVehSpd = random value*/
                l_IC_CruiseSwitch_BOOL = FALSE;
            }

            if (UI_CRUCON_SUB_SHORT == l_cruise_option_button_state)
            {
                /*2128-Set IC_CruiseSwitch=1,biased IC_TargetCruis decrease by 1km/h*/
                l_IC_CruiseSwitch_BOOL = TRUE;
                if ((l_IC_TargetCruis_Display - (UINT32)CRUISE_MIN_SPEED) >= (UINT32)CRUISE_SPEED_SHORT_PRESS_STEP)
                {
                    l_IC_TargetCruis_Display -= (UINT32)CRUISE_SPEED_SHORT_PRESS_STEP;
                    l_last_cruise_active_speed = l_IC_TargetCruis_Display;
                    l_IC_TargetCruis = (l_IC_TargetCruis_Display - CRUISE_SPEED_SHIFT) * CRUISE_SPEED_BASE / CRUISE_SPEED_FACTOR;
                    //fw_set_warning_status(W_LOWEST_CRUISE_SPEED_HAS_RACHED, WARNING_NOT_DISPLAY);
                }
                else
                {
                    /*cluster shall pop up warning message "Has reached lowest cruising speed"*/
                    //fw_set_warning_status(W_LOWEST_CRUISE_SPEED_HAS_RACHED, WARNING_DISPLAY);
                }
                /*Cruise control telltale shall be displayed with green color*/
                l_cruise_telltale_state = CRUISE_ICON_GREEN;
                /*Cruise speed shall be displayed with biased IC_TargetCruis value*/
                l_cruise_speed_state = CRUISE_SPEED_DISPLAY;
            }
            else if (UI_CRUCON_ADD_SHORT == l_cruise_option_button_state)
            {
                /*2130-Set IC_CruiseSwitch=1,biased IC_TargetCruis increase by 1km/h*/
                l_IC_CruiseSwitch_BOOL = TRUE;

                if (l_IC_TargetCruis_Display < (UINT32)CRUISE_MAX_SPEED)
                {
                    l_IC_TargetCruis_Display += (UINT32)CRUISE_SPEED_SHORT_PRESS_STEP;
                    l_last_cruise_active_speed = l_IC_TargetCruis_Display;
                    l_IC_TargetCruis = (l_IC_TargetCruis_Display - CRUISE_SPEED_SHIFT) * CRUISE_SPEED_BASE / CRUISE_SPEED_FACTOR;
                    //fw_set_warning_status(W_LOWEST_CRUISE_SPEED_HAS_RACHED, WARNING_NOT_DISPLAY);
                }
                else
                {
                    /*When biased IC_TargetCruis>=175km/h, the target cruise speed shall NOT increase anymore and cluster shall pop up warning message "Has reached highest cruising speed"*/
                    //fw_set_warning_status(W_LOWEST_CRUISE_SPEED_HAS_RACHED, WARNING_DISPLAY);
                }
                /*Cruise control telltale shall be displayed with green color*/
                l_cruise_telltale_state = CRUISE_ICON_GREEN;
                /*Cruise speed shall be displayed with biased IC_TargetCruis value*/
                l_cruise_speed_state = CRUISE_SPEED_DISPLAY;            }
            else
            {
            }

            break;
        }
    case CRUISE_ACCELERATION_OVERRIDE:
        {
            if ((UI_CRUCON_SUB_SHORT == l_cruise_option_button_state)
                || (UI_CRUCON_SUB_LONG == l_cruise_option_button_state))
            {
                /*2137-Set IC_CruiseSwitch=1,IC_TargetCruis = current speed VSpeedo*/
                l_IC_CruiseSwitch_BOOL = TRUE;
                /*Cruise control telltale shall be displayed with green color*/
                l_cruise_telltale_state = CRUISE_ICON_GREEN;
                /*Cruise speed shall be flashed @1Hz with biased IC_TargetCruis values*/
                l_cruise_speed_state = CRUISE_SPEED_FLASH_1HZ;
				Rte_Read_rpSR_TISpdMdl_Real_Value(&l_IC_TargetCruis);
                l_IC_TargetCruis_Display = l_IC_TargetCruis * CRUISE_SPEED_FACTOR / CRUISE_SPEED_BASE + CRUISE_SPEED_SHIFT;
            }
            break;
        }
    default:
        break;
    }

}

/****************************************************************************
Function Name     : execute_cruise_state_transitions

Description       : executes the cruise control state tansition functions

Invocation       : void check_cruise_state_transitions(void)

Parameters        : None.

Return Value      :None.

Critical Section  :None

******************************************************************************/
void execute_cruise_state_transitions(void)
{
    CRUISE_STATE_TRANS_S const * lp_ptr_SP;

    UINT16 l_pre_CruiseState = (UINT16)0;

    l_pre_CruiseState = l_curr_CruiseState;
    l_curr_CruiseState = l_next_CruiseState;

    for (lp_ptr_SP = cruise_state_transition_array_SA; lp_ptr_SP->cruise_state_transition_func_FP  != ((void (*)(void))0); lp_ptr_SP++)
    {
        if ( ( (UINT8)0 != ((lp_ptr_SP->l_current_cruise_state_U16) & l_pre_CruiseState) ) && \
            ( (UINT8)0 != ((lp_ptr_SP->l_next_cruise_state_U16) & l_curr_CruiseState) )  )
        {
            (lp_ptr_SP->cruise_state_transition_func_FP)();
        }
        else
        {

        }
    }
}

/****************************************************************************
Function Name     :  check_cruise_state_transitions

Description       :  Check for any cruise control state transitions and
                     calls corresponding transitions functions 

Invocation       :   void fcruise_KSRRobin(void)

Parameters        : None

Return Value      : None

Critical Section  : None

******************************************************************************/
void check_cruise_state_transitions(void)
{
    UINT32 i;
    for (i = (UINT32)0; i < sizeof(cruise_tbl_array_SA) / sizeof(CRUISE_STATE_TBL_S); i++)
    {
        if (cruise_tbl_array_SA[i].CurrCruiseState == l_curr_CruiseState)
        {
            if (TRUE == (cruise_tbl_array_SA[i].PMCndFunc_p)())
            {
                l_next_CruiseState = cruise_tbl_array_SA[i].NextCruiseState;
                break;
            }
        }
    }

    if (l_curr_CruiseState != l_next_CruiseState)
    {
        execute_cruise_state_transitions();
    }
}

/****************************************************************************
Function Name     : Cruise_ProcessOff2Off

Description       : process the operations when the cruise mode transfer from Off to Off

Invocation       : Invoked by Required modules

Parameters        : None.

Return Value      :None

Critical Section  :None

******************************************************************************/
void Cruise_ProcessOff2Off(void)
{
    /*Cruise control telltale shall NOT be displayed */
    /*Cruise control speed shall NOT be displayed*/
    reset_All_Flags();
}

/****************************************************************************
Function Name     : Cruise_ProcessOff2Prepare

Description       : process the operations when the cruise mode transfer from Off to Prepare

Invocation       : Invoked by Required modules

Parameters        : None.

Return Value      :None

Critical Section  :None

******************************************************************************/
void Cruise_ProcessOff2Prepare(void)
{
    l_IC_CruiseSwitch_BOOL = FALSE;
    l_IC_TargetCruis = (UINT32)0;
    l_IC_TargetCruis_Display = l_IC_TargetCruis;
    /*Cruise control telltale shall be displayed with gray color*/
    l_cruise_telltale_state = CRUISE_ICON_GRAY;
    /*Cruise speed shall NOT be displayed*/
    l_cruise_speed_state = CRUISE_SPEED_NOT_DISPLAY;
}

/****************************************************************************
Function Name     : Cruise_ProcessPrepare2Off

Description       : process the operations when the cruise mode transfer from Prepare to Off

Invocation       : Invoked by Required modules

Parameters        : None.

Return Value      :None

Critical Section  :None

******************************************************************************/
void Cruise_ProcessPrepare2Off(void)
{
    reset_All_Flags();
}

/****************************************************************************
Function Name     : Cruise_ProcessPrepare2Active

Description       : process the operations when the cruise mode transfer from Prepare to Active

Invocation       : Invoked by Required modules

Parameters        : None.

Return Value      :None

Critical Section  :None

******************************************************************************/
void Cruise_ProcessPrepare2Active(void)
{
    /*Cruise control telltale shall be displayed with green color*/
    l_cruise_telltale_state = CRUISE_ICON_GREEN;
    /*Cruise speed shall be displayed with biased IC_TargetCruis value*/
    l_cruise_speed_state = CRUISE_SPEED_DISPLAY;
    l_cruise_has_been_active_before_BOOL = TRUE;
    l_last_cruise_active_speed = l_IC_TargetCruis_Display;
}

/****************************************************************************
Function Name     : Cruise_ProcessActive2Off

Description       : process the operations when the cruise mode transfer from Active to Off

Invocation       : Invoked by Required modules

Parameters        : None.

Return Value      :None

Critical Section  :None

******************************************************************************/
void Cruise_ProcessActive2Off(void)
{
    /*Set IC_CruiseSwitch=0,IC_TargetCruis =Random value*/
    /*Cruise control telltale shall NOT be displayed*/
    /*Cruise speed shall NOT be displayed*/
    reset_All_Flags();
}

/****************************************************************************
Function Name     : Cruise_ProcessActive2Prepare

Description       : process the operations when the cruise mode transfer from Active to Prepare

Invocation       : Invoked by Required modules

Parameters        : None.

Return Value      :None

Critical Section  :None

******************************************************************************/
void Cruise_ProcessActive2Prepare(void)
{
    /*2123/2125-Send IC_CruiseSwitch=0,IC_TargetCruiseVehSpd =Random value*/
    l_IC_CruiseSwitch_BOOL = FALSE;
    /*Cruise control telltale shall be displayed with gray color*/
    l_cruise_telltale_state = CRUISE_ICON_GRAY;
    /*The latest Cruise speed shall be displayed */
    l_cruise_speed_state = CRUISE_SPEED_DISPLAY;
}

/****************************************************************************
Function Name     : Cruise_ProcessActive2Override

Description       : process the operations when the cruise mode transfer from Active to Override

Invocation       : Invoked by Required modules

Parameters        : None.

Return Value      :None

Critical Section  :None

******************************************************************************/
void Cruise_ProcessActive2Override(void)
{
    /*2124-Cruise control telltale shall be displayed with green color*/
    l_cruise_telltale_state = CRUISE_ICON_GREEN;
    /*Cruise speed shall display biased IC_TargetCruis value and flash @1Hz*/
    l_cruise_speed_state = CRUISE_SPEED_FLASH_1HZ;
}

/****************************************************************************
Function Name     : Cruise_ProcessOverride2Off

Description       : process the operations when the cruise mode transfer from Override to Off

Invocation       : Invoked by Required modules

Parameters        : None.

Return Value      :None

Critical Section  :None

******************************************************************************/
void Cruise_ProcessOverride2Off(void)
{
    /*2126-Set IC_CruiseSwitch=0,IC_TargetCruis =Random value*/
    /*Cruise control telltale shall NOT be displayed*/
    /*Cruise speed shall NOT be displayed*/
    reset_All_Flags();
}

/****************************************************************************
Function Name     : Cruise_ProcessOverride2Prepare

Description       : process the operations when the cruise mode transfer from Override to Prepare

Invocation       : Invoked by Required modules

Parameters        : None.

Return Value      :None

Critical Section  :None

******************************************************************************/
void Cruise_ProcessOverride2Prepare(void)
{
    /*2133-Set IC_CruiseSwitch=0,IC_TargetCruis =Random value*/
    l_IC_CruiseSwitch_BOOL = FALSE;
    /*Cruise control telltale shall be displayed with gray Color*/
    l_cruise_telltale_state = CRUISE_ICON_GRAY;
    /*The latest Cruise speed shall be displayed*/
    l_cruise_speed_state = CRUISE_SPEED_DISPLAY;
}

/****************************************************************************
Function Name     : Cruise_ProcessOverride2Active

Description       : process the operations when the cruise mode transfer from Override to Active

Invocation       : Invoked by Required modules

Parameters        : None.

Return Value      :None

Critical Section  :None

******************************************************************************/
void Cruise_ProcessOverride2Active(void)
{
    /*2134-Cruise control telltale shall be displayed with green color*/
    l_cruise_telltale_state = CRUISE_ICON_GREEN;
    /*Cruise speed shall be displayed with biased IC_TargetCruis value*/
    l_cruise_speed_state = CRUISE_SPEED_DISPLAY;
    l_cruise_has_been_active_before_BOOL = TRUE;
    l_last_cruise_active_speed = l_IC_TargetCruis_Display;
}

/****************************************************************************
Function Name     : transOff2Off

Description       : This function process the transition from the Off to Off
Invocation        : check_cruise_state_transitions

Parameters        : None

Return Value      : if the transition condition is satisfied

Critical Section  : None

******************************************************************************/
BOOLEAN transOff2Off(void)
{
    BOOLEAN bRet = FALSE;
    if (((UINT8)eIGN_OFF == l_IGN_State)
        || (TRUE == l_cruise_sts_missing_BOOL)
        || (TRUE == l_currSpeed_invalid_BOOL)
        || (TRUE == l_currSpeed_missing_BOOL))
    {
        bRet = TRUE;
    }
    return bRet;
}

/****************************************************************************
Function Name     : transOff2Prepare

Description       : This function process the transition from the Off to Prepare
Invocation        : check_cruise_state_transitions

Parameters        : None

Return Value      : if the transition condition is satisfied

Critical Section  : None

******************************************************************************/
BOOLEAN transOff2Prepare(void)
{
    BOOLEAN bRet = FALSE;
    if (UI_CRUCON_ON == l_cruise_button_state)
    {
        bRet = TRUE;
    }
    return bRet;
}

/****************************************************************************
Function Name     : transPrepare2Off

Description       : This function process the transition from the Prepare to Off
Invocation        : check_cruise_state_transitions

Parameters        : None

Return Value      : if the transition condition is satisfied

Critical Section  : None

******************************************************************************/
BOOLEAN transPrepare2Off(void)
{
    BOOLEAN bRet = FALSE;
    if (((UINT8)eIGN_OFF == l_IGN_State)
        || (UI_CRUCON_OFF == l_cruise_button_state)
        || (TRUE == l_cruise_sts_missing_BOOL)
        || (TRUE == l_currSpeed_invalid_BOOL)
        || (TRUE == l_currSpeed_missing_BOOL))
    {
        bRet = TRUE;
    }
    return bRet;
}

/****************************************************************************
Function Name     : transPrepare2Active

Description       : This function process the transition from the Prepare to Active
Invocation        : check_cruise_state_transitions

Parameters        : None

Return Value      : if the transition condition is satisfied

Critical Section  : None

******************************************************************************/
BOOLEAN transPrepare2Active(void)
{
    BOOLEAN bRet = FALSE;
    if ((TRUE == l_IC_CruiseSwitch_BOOL) && ((CRUISE_CONTROL_ACTIVE == l_EMS_CruiseCtrlSts) || (FIGEAGE_REGULATION == l_EMS_CruiseCtrlSts)))
    {
        bRet = TRUE;
    }
    return bRet;
}

/****************************************************************************
Function Name     : transActive2Off

Description       : This function process the transition from the Active to Off
Invocation        : check_cruise_state_transitions

Parameters        : None

Return Value      : if the transition condition is satisfied

Critical Section  : None

******************************************************************************/
BOOLEAN transActive2Off(void)
{
    BOOLEAN bRet = FALSE;
    if (((DEC_CONSIGNE == l_EMS_CruiseCtrlSts) && (UI_CRUCON_OFF == l_cruise_button_state))
        || ((UINT8)eIGN_OFF == l_IGN_State)
        || (TRUE == l_cruise_sts_missing_BOOL)
        || (TRUE == l_currSpeed_invalid_BOOL)
        || (TRUE == l_currSpeed_missing_BOOL))
    {
        bRet = TRUE;
    }
    return bRet;
}

/****************************************************************************
Function Name     : transActive2Prepare

Description       : This function process the transition from the Active to Prepare
Invocation        : check_cruise_state_transitions

Parameters        : None

Return Value      : if the transition condition is satisfied

Critical Section  : None

******************************************************************************/
BOOLEAN transActive2Prepare(void)
{
    BOOLEAN bRet = FALSE;
    if (    (ALERET == l_EMS_CruiseCtrlSts)
        || (NO_CRUISE_CONTROL == l_EMS_CruiseCtrlSts)
        || (DECROISSANCE_SECURITE == l_EMS_CruiseCtrlSts)
        || ((DEC_CONSIGNE == l_EMS_CruiseCtrlSts) && (UI_CRUCON_ON == l_cruise_button_state)))
    {
        bRet = TRUE;
    }
    return bRet;
}

/****************************************************************************
Function Name     : transActive2Override

Description       : This function process the transition from the Active to Override
Invocation        : check_cruise_state_transitions

Parameters        : None

Return Value      : if the transition condition is satisfied

Critical Section  : None

******************************************************************************/
BOOLEAN transActive2Override(void)
{
    BOOLEAN bRet = FALSE;
    if ((ACCELERATION_OVERRIDE == l_EMS_CruiseCtrlSts) || (FIGEAGE_ACCELERATION == l_EMS_CruiseCtrlSts))
    {
        bRet = TRUE;
    }
    return bRet;
}

/****************************************************************************
Function Name     : transOverride2Off

Description       : This function process the transition from the Override to Off
Invocation        : check_cruise_state_transitions

Parameters        : None

Return Value      : if the transition condition is satisfied

Critical Section  : None

******************************************************************************/
BOOLEAN transOverride2Off(void)
{
    BOOLEAN bRet = FALSE;
    if (((DEC_CONSIGNE == l_EMS_CruiseCtrlSts) && (UI_CRUCON_OFF == l_cruise_button_state))
        || ((UINT8)eIGN_OFF == l_IGN_State)
        || (TRUE == l_cruise_sts_missing_BOOL)
        || (TRUE == l_currSpeed_invalid_BOOL)
        || (TRUE == l_currSpeed_missing_BOOL))
    {
        bRet = TRUE;
    }
    return bRet;
}

/****************************************************************************
Function Name     : transOverride2Prepare

Description       : This function process the transition from the Override to Prepare
Invocation        : check_cruise_state_transitions

Parameters        : None

Return Value      : if the transition condition is satisfied

Critical Section  : None

******************************************************************************/
BOOLEAN transOverride2Prepare(void)
{
    BOOLEAN bRet = FALSE;
    if ((ALERET == l_EMS_CruiseCtrlSts) || (NO_CRUISE_CONTROL == l_EMS_CruiseCtrlSts) || (DECROISSANCE_SECURITE == l_EMS_CruiseCtrlSts)
        || ((DEC_CONSIGNE == l_EMS_CruiseCtrlSts) && (UI_CRUCON_ON== l_cruise_button_state)))
    {
        bRet = TRUE;
    }
    return bRet;
}

/****************************************************************************
Function Name     : transOverride2Active

Description       : This function process the transition from the Override to Active
Invocation        : check_cruise_state_transitions

Parameters        : None

Return Value      : if the transition condition is satisfied

Critical Section  : None

******************************************************************************/
BOOLEAN transOverride2Active(void)
{
    BOOLEAN bRet = FALSE;
    if ((CRUISE_CONTROL_ACTIVE == l_EMS_CruiseCtrlSts) || (FIGEAGE_REGULATION == l_EMS_CruiseCtrlSts))
    {
        bRet = TRUE;
    }
    return bRet;
}

/****************************************************************************
Function Name     : void fcruise_trans_Outof_Normal(void)

Description       : This function will clear the Cruise Control Speed, Unit and Icon Indicator display on 
                    transition out of Normal state.

Invocation        : local

Parameters        : none

Return Value      : none

Critical Section  : none

******************************************************************************/
void fcruise_trans_Outof_Normal(void)
{
	fcruise_common_init();
}

/****************************************************************************
Function Name     : reset_All_Flags

Description       : reset all the member variables in this file

Invocation       : Invoked by Required modules

Parameters        : None.

Return Value      :None

Critical Section  :None

******************************************************************************/
void reset_All_Flags(void)
{
    l_IGN_State = (UINT8)(eIGN_OFF);
    l_last_cruise_active_speed = (UINT32)0;
    l_EMS_CruiseCtrlSts = (UINT8)0;
    l_IC_TargetCruis = (UINT32)0;
    l_IC_TargetCruis_Display = (UINT32)0;
    l_IC_CruiseSwitch_BOOL = FALSE;
    l_curr_CruiseState = (UINT16)(CRUISE_OFF);
    l_next_CruiseState = (UINT16)(CRUISE_OFF);
    l_minus_button_long_press_timer = (UINT32)0;
    l_plus_button_long_press_timer = (UINT32)0;
    l_cruise_telltale_state = CRUISE_ICON_HIDE;
    l_cruise_speed_state = CRUISE_SPEED_NOT_DISPLAY;
    l_cruise_sts_missing_BOOL = FALSE;
    l_currSpeed_invalid_BOOL = FALSE;
    l_currSpeed_missing_BOOL = FALSE;
    l_cruise_has_been_active_before_BOOL = FALSE;
    l_cruise_button_state = UI_CRUCON_OFF;
    l_cruise_option_button_state = UI_CRUCON_OPTION;

    button_cruise_off();
}
/*============================================================================
** C L E A R C A S E    R E V I S I O N    N O T E S
**============================================================================
**
** For each change to this file, be sure to record:
** 1.  Who made the change and when the change was made.
** 2.  Why the change was made and the intended result.
**
** CLEARCASE Rev #     Date         By
** CLEARCASE Rev X.X   mm/dd/yy     CDSID
**
**============================================================================*/



