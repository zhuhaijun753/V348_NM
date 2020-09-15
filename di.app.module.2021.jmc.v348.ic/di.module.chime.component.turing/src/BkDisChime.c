/******************************************************************************
              CONFIDENTIAL VISTEON CORPORATION

 This is an unpublished work of authorship, which contains trade secrets,
 created in 2012. Visteon Corporation owns all rights to this work and
 intends to maintain it in confidence to preserve its trade secret status.
 Visteon Corporation reserves the right, under the copyright laws of the
 United States or those of any other country that may have  jurisdiction,
 to protect this work as an unpublished work, in the event of an
 inadvertent or deliberate unauthorized publication. Visteon Corporation
 also reserves its rights under all copyright laws to protect this work as
 a published work, when appropriate. Those having access to this work may
 not copy it, use it, modify it or disclose the information contained in
 it without the written authorization of Visteon Corporation
******************************************************************************/
/*****************************************************************************

File Name        :  BkDisChime.c
Module Short Name:  BkDisChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define BkDisChime_C

#include "Rte_BkDisChime.h"
#include "CmpLib.h"
#include "ChimeLogical.h"


CMPLIB_INSTANCE(BkDisChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
// static uint8    l_BkDisChimeState_1HZ_U8 = FALSE;
// static uint8    l_BkDisChimeState_2HZ_U8 = FALSE;
// static uint8    l_BkDisChimeState_4HZ_U8 = FALSE;
// static uint8    l_BkDisChimeState_LONG_U8 = FALSE;
// static uint8    l_IGN_statePre_U8 = FALSE;
// static boolean  l_BkDis_flag = TRUE;
// static uint8    l_ProveoutCounter_U8 = FALSE;

/*****************************************************************************
*                                 Manifest Constants                         *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/

/*****************************************************************************
*                                 Macro Definitions                          *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/
// #define RearSysTemWorkSts_Disable_OFF   0x00
// #define RearSysTemWorkSts_Enable_OFF    0x01
// #define RearSysTemWorkSts_Active_ON     0x02
// #define RearSysTemWorkSts_Failed_OFF    0x03

// #define BD_ChimeCmdSoundStatus_NoWarning_OFF                0x00
// #define BD_ChimeCmdSoundStatus_FrontSystemWarning_OFF       0x01
// #define BD_ChimeCmdSoundStatus_RearSystemWarning_ON         0x02
// #define BD_ChimeCmdSoundStatus_FrontRearSystemWarning_ON    0x03

// #define BD_ChimeCmdSoundCadence_Tone1  0x01
// #define BD_ChimeCmdSoundCadence_Tone2  0x02
// #define BD_ChimeCmdSoundCadence_Tone3  0x03
// #define BD_ChimeCmdSoundCadence_Tone4  0x04
// #define BD_ChimeCmdSoundCadence_Tone5  0x05

/*                                 Type Declarations                          *
******************************************************************************/


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/



/*****************************************************************************
*                   Functions                                                *
******************************************************************************/
// static void f_BkDisinitialState(void);
// static boolean f_BkDis_input_verify(void);
// static void f_BkDisChime_Process(void);
// static boolean BkDis_PowerOn_Self_Test_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	// f_BkDisinitialState();
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_LONG,l_BkDisChimeState_LONG_U8);   
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_1HZ,l_BkDisChimeState_1HZ_U8);
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_2HZ,l_BkDisChimeState_2HZ_U8);
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_4HZ,l_BkDisChimeState_4HZ_U8);
	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional de-initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
	// f_BkDisinitialState();
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_LONG,l_BkDisChimeState_LONG_U8);   
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_1HZ,l_BkDisChimeState_1HZ_U8);
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_2HZ,l_BkDisChimeState_2HZ_U8);
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_4HZ,l_BkDisChimeState_4HZ_U8);
	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional activation state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation( void )
{
	// f_BkDisinitialState();
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_LONG,l_BkDisChimeState_LONG_U8);   
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_1HZ,l_BkDisChimeState_1HZ_U8);
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_2HZ,l_BkDisChimeState_2HZ_U8);
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_4HZ,l_BkDisChimeState_4HZ_U8);
	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional de-activation state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation( void )
{
	// f_BkDisinitialState();
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_LONG,l_BkDisChimeState_LONG_U8);   
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_1HZ,l_BkDisChimeState_1HZ_U8);
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_2HZ,l_BkDisChimeState_2HZ_U8);
    // Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_4HZ,l_BkDisChimeState_4HZ_U8);
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
#if 0
	eIgnState fl_IGN_state;
	EBatteryState fl_Battery_state;
    boolean	f_ret = FALSE;

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	if((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage))
	{
		f_BkDisinitialState();
	}
	else
	{
        f_ret = f_BkDis_input_verify();
        if(f_ret == TRUE)
        {
            if(fl_IGN_state == eIGN_RUN)
            {
                if(l_IGN_statePre_U8 == eIGN_OFF)
                {
                    l_BkDis_flag = TRUE; 
                    l_ProveoutCounter_U8 = 0;
                }
                else if((l_IGN_statePre_U8 == eIGN_RUN) && (l_BkDis_flag == FALSE))
                {				
                    f_BkDisChime_Process();
                }
            
                BkDis_PowerOn_Self_Test_Action();
            }
            else
            {
                f_BkDisinitialState();
            }
        }
        else
        {
            f_BkDisinitialState();
        }

        l_IGN_statePre_U8 = fl_IGN_state;
	}

    Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_LONG,l_BkDisChimeState_LONG_U8);   
    Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_1HZ,l_BkDisChimeState_1HZ_U8);
    Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_2HZ,l_BkDisChimeState_2HZ_U8);
    Rte_Call_rpCS_BkDisChime_UpdateStatus_Operation(CHIME_REQID_BK_DIS_4HZ,l_BkDisChimeState_4HZ_U8);
#endif
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Diagnostic state
///
/// @return E_OK:			Diagnostic has completed. Go to DiagReturn state.
///<br> 	E_NOT_READY:	Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:			Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br> 	E_NOT_READY:	Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
	return E_OK;
}

#if 0
static void f_BkDisinitialState(void)
{
    l_BkDisChimeState_1HZ_U8 = FALSE;
    l_BkDisChimeState_2HZ_U8 = FALSE;
    l_BkDisChimeState_4HZ_U8 = FALSE;
    l_BkDisChimeState_LONG_U8 = FALSE;
    l_ProveoutCounter_U8 = 0;
}

static boolean f_BkDis_input_verify(void)
{
    boolean	f_ret = TRUE;
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_Tout(&fl_CAN_msg_status_U8);	
	if((fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_msg_status_U8 == RTE_E_TIMEOUT))
	{
		f_ret = FALSE;
	}
	
	return f_ret;
}

static boolean BkDis_PowerOn_Self_Test_Action(void)
{
    if(l_ProveoutCounter_U8++ > 2)
    {
        l_BkDis_flag = FALSE;
    }
    else
    {
        l_BkDis_flag = TRUE;
    }

    return l_BkDis_flag;
}

static void f_BkDisChime_Process(void)
{
    uint8   fl_signal_RearSysTemWorkSt_U8 = 0;
    uint8   fl_signal_ChimeCmdSoundSt_U8 = 0;
    uint8   fl_signal_ChimeCmdSoundCadence_U8 = 0;
    uint8   fl_animationFlag = TRUE;

    Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_RearSysTemWorkSts(&fl_signal_RearSysTemWorkSt_U8);
    Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundStatus(&fl_signal_ChimeCmdSoundSt_U8);
    Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundCadence(&fl_signal_ChimeCmdSoundCadence_U8);
    Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);

    if(fl_animationFlag == FALSE)
    {
        if(fl_signal_RearSysTemWorkSt_U8 == RearSysTemWorkSts_Active_ON)
        {
            if( (fl_signal_ChimeCmdSoundSt_U8 == BD_ChimeCmdSoundStatus_RearSystemWarning_ON) || (fl_signal_ChimeCmdSoundSt_U8 == BD_ChimeCmdSoundStatus_FrontRearSystemWarning_ON))
            {
                switch(fl_signal_ChimeCmdSoundCadence_U8)
                {
                    case BD_ChimeCmdSoundCadence_Tone1:
                        l_BkDisChimeState_LONG_U8 = TRUE;
                        l_BkDisChimeState_1HZ_U8 = FALSE;
                        l_BkDisChimeState_2HZ_U8 = FALSE;
                        l_BkDisChimeState_4HZ_U8 = FALSE;
                        break;
                    case BD_ChimeCmdSoundCadence_Tone2:
                        l_BkDisChimeState_LONG_U8 = FALSE;
                        l_BkDisChimeState_1HZ_U8 = FALSE;
                        l_BkDisChimeState_2HZ_U8 = FALSE;
                        l_BkDisChimeState_4HZ_U8 = TRUE;
                        break;
                    case BD_ChimeCmdSoundCadence_Tone3:
                        l_BkDisChimeState_LONG_U8 = FALSE;
                        l_BkDisChimeState_1HZ_U8 = FALSE;
                        l_BkDisChimeState_2HZ_U8 = FALSE;
                        l_BkDisChimeState_4HZ_U8 = TRUE;
                        break;
                    case BD_ChimeCmdSoundCadence_Tone4:
                        l_BkDisChimeState_LONG_U8 = FALSE;
                        l_BkDisChimeState_1HZ_U8 = FALSE;
                        l_BkDisChimeState_2HZ_U8 = TRUE;
                        l_BkDisChimeState_4HZ_U8 = FALSE;
                        break;
                    case BD_ChimeCmdSoundCadence_Tone5:
                        l_BkDisChimeState_LONG_U8 = FALSE;
                        l_BkDisChimeState_1HZ_U8 = TRUE;
                        l_BkDisChimeState_2HZ_U8 = FALSE;
                        l_BkDisChimeState_4HZ_U8 = FALSE;
                        break;
                    default:
                        l_BkDisChimeState_LONG_U8 = FALSE;
                        l_BkDisChimeState_1HZ_U8 = FALSE;
                        l_BkDisChimeState_2HZ_U8 = FALSE;
                        l_BkDisChimeState_4HZ_U8 = FALSE;
                        break;
                }
            }
            else
            {
                f_BkDisinitialState();
            }
        }
        else
        {
            f_BkDisinitialState();
        }
    }
    else
    {
        f_BkDisinitialState();
    }
    
}
#endif

/*End of File*/
/*****************************************************************************
*   for each change to this file, be sure to record:                         *
*      1.  who made the change and when the change was made                  *
*      2.  why the change was made and the intended result                   *
*   Following block needs to be repeated for each change
******************************************************************************
*   Note: In the traceability column we need to trace back to the Design Doc.*
*   For the initial version it is traced to the Design Document section.     *
*   For further changes it shall trace to the source of the change which may *
*   be SPSS/SCR/Defect details(Defect may be Testing/validation defect)/Any  *
*   other reason                                                            *
******************************************************************************/
