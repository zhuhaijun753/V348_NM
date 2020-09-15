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

File Name        :  OverSpdChime.c
Module Short Name:  OverSpdChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define OverSpdCHIME_C

#include "Rte_OverSpdChime.h"
#include "CmpLib.h"
#include "ChimeLogical.h"


CMPLIB_INSTANCE(OverSpdChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
static boolean l_OverSpdChimeState_U8       = FALSE;
static boolean l_OverSpdChimeState_last_U8  = FALSE;

static boolean  l_Overspeed_Flag       = TRUE;
static uint8    l_ProveoutCounter_U8   = FALSE;


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
#define OverSpd_Invalid_SPEED           (4095)
#define OverSpd_SPEED_VALUE_NOT         (4113)
#define OverSpd_SPEED_VALUE_80KM        (1422)
#define OverSpd_SPEED_VALUE_100KM       (1777)
#define OverSpd_SPEED_VALUE_120KM       (2133)

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
static void f_OverSpdChime_InitialState(void);
static void f_OverSpdChime_Process(void);
static boolean f_OverSpdChime_input_verify(void);
static boolean Overspeed_PowerOn_Self_Test_Action(void);


//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_OverSpdChime_InitialState();
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
	f_OverSpdChime_InitialState();
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
	f_OverSpdChime_InitialState();
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
	f_OverSpdChime_InitialState();
	return E_OK;
}

static boolean Overspeed_PowerOn_Self_Test_Action(void)
{
	  if(l_ProveoutCounter_U8 >= 30)
    {
        l_Overspeed_Flag = FALSE;
    }
    else
    {
        l_Overspeed_Flag = TRUE;
	      l_ProveoutCounter_U8++;
    }

    return l_Overspeed_Flag;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	eIgnState fl_IGN_state = eIGN_OFF;
	EBatteryState fl_Battery_state = eBatteryState_Normal;
    boolean fl_animationFlag = TRUE;
	
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);

	f_OverSpdChime_InitialState();
	if((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage))
	{
		f_OverSpdChime_InitialState();
	}
	else
	{
		if(eIGN_RUN == fl_IGN_state)
		{
			if(FALSE == fl_animationFlag)
			{
				Overspeed_PowerOn_Self_Test_Action();
				if(FALSE == l_Overspeed_Flag)
				{
					f_OverSpdChime_Process();
				}
				else
				{
					f_OverSpdChime_InitialState();
				}
			}
		}
		else
		{
			l_Overspeed_Flag	   = TRUE;
			l_ProveoutCounter_U8   = FALSE;
			f_OverSpdChime_InitialState();
		}
	}
	
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

static void f_OverSpdChime_InitialState(void)
{
	l_OverSpdChimeState_U8 = FALSE;

	Rte_Call_rpCS_OverSpdChime_UpdateStatus_Operation(CHIME_REQID_Over_Spd, l_OverSpdChimeState_U8);
}

static boolean f_OverSpdChime_input_verify(void)
{
	boolean	f_ret = TRUE;
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_CAN_msg_status_U8);
		
	if(fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_ret = FALSE;
	}
	
	return f_ret;
}

static void f_OverSpdChime_Process(void)
{
	
	boolean	f_ret = FALSE;
	uint16	fl_Spd_Value_U16 = 0;
	uint8 IsEngineCfg_OverSpeed_Chime = 0;
	uint16 fl_ABS_VehSpdLgt_Normal_u16 = 0;
	uint16 fl_ABS_VehSpdLgt_Wanring_u16 = 0;
	uint16 fl_ABS_VehSpdLgt_Safe_u16 = 0;
	
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_OVER_SPEED,&IsEngineCfg_OverSpeed_Chime);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_Spd_Value_U16);
	
	switch(IsEngineCfg_OverSpeed_Chime)
	{
		case 0:
			fl_ABS_VehSpdLgt_Normal_u16 = OverSpd_SPEED_VALUE_NOT;
		break;
		case 1:
			fl_ABS_VehSpdLgt_Normal_u16 = OverSpd_SPEED_VALUE_80KM;
		break;
		case 2:
			fl_ABS_VehSpdLgt_Normal_u16 = OverSpd_SPEED_VALUE_100KM;
		break;	
		case 3:
			fl_ABS_VehSpdLgt_Normal_u16 = OverSpd_SPEED_VALUE_120KM;
		break;	
		default:
		   	//
		break;
	}
	
	fl_ABS_VehSpdLgt_Wanring_u16 = fl_ABS_VehSpdLgt_Normal_u16 -18;
	fl_ABS_VehSpdLgt_Safe_u16 = fl_ABS_VehSpdLgt_Normal_u16 -35;
	
	f_ret = f_OverSpdChime_input_verify();
	if(FALSE == f_ret)
	{
		f_OverSpdChime_InitialState();
	}
	else
	{
		if(fl_Spd_Value_U16 != OverSpd_Invalid_SPEED)
		{
			if(fl_Spd_Value_U16 > fl_ABS_VehSpdLgt_Wanring_u16)
			{
				l_OverSpdChimeState_U8 = TRUE;
				l_OverSpdChimeState_last_U8 = l_OverSpdChimeState_U8;
			}
			else if(fl_Spd_Value_U16 <= fl_ABS_VehSpdLgt_Safe_u16)
			{
				l_OverSpdChimeState_U8 = FALSE;
				l_OverSpdChimeState_last_U8 = l_OverSpdChimeState_U8;
			}
			else
			{
				l_OverSpdChimeState_U8 = l_OverSpdChimeState_last_U8;
			}
		}
		else
		{

			l_OverSpdChimeState_U8 = FALSE;
		}
	}
		
	Rte_Call_rpCS_OverSpdChime_UpdateStatus_Operation(CHIME_REQID_Over_Spd, l_OverSpdChimeState_U8);
}

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
