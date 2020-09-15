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

File Name        :  NotInPChime.c
Module Short Name:  NotInPChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define NotInPChime_C

#include "Rte_NotInPChime.h"
#include "CmpLib.h"
#include "ChimeLogical.h"

CMPLIB_INSTANCE(NotInPChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/


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
// static uint8  l_invalid_cnt = 0;
static uint8  l_NotInPChimeState_U8 = FALSE;
static uint8 fl_count_2s = 0;

#define T_GEAR_OFF 			   0xd
#define T_GEAR_INVALID		   0xf
#define T_Invalid_value	       0xff
#define VehsepLgt_Value 	   500U
#define VEHICLE_TACH_INVALID   (0x7FFF)
// #define Engine_Speed		   ((uint32)45000)

/*                                 Type Declarations                          *
******************************************************************************/


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/

//static uint8  l_NotInPChime_Last_State_U8 = FALSE;
static eIgnState fl_IGN_Last_state = eIGN_OFF;


/*****************************************************************************
*                   Functions                                                *
******************************************************************************/
static void f_NotInPChime_initial(void);
static void f_NotInPChime_Process(void);
static boolean f_NotInPChime_input_verity(void);



//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_NotInPChime_initial();
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
	f_NotInPChime_initial();
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
	f_NotInPChime_initial();
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
	f_NotInPChime_initial();
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	boolean f_ret = FALSE;
	EBatteryState fl_Battery_state = eBatteryState_Normal;
	eIgnState fl_IGN_state = eIGN_OFF;
	boolean IsEngineCfg = FALSE;
	boolean fl_PEPS_IMMO = FALSE;
	uint8 fl_animationFlag = TRUE;

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AT_MT,&IsEngineCfg);  /* TRUE:AT,   FALSE:MT   */
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_IMMO,&fl_PEPS_IMMO);  /* TRUE:PEPS, FALSE:IMMO */
	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);

	if ((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage) || (fl_Battery_state == eBatteryState_OverVoltagePhase1)) 
	{
		f_NotInPChime_initial();
	}
	else
	{
		f_ret = f_NotInPChime_input_verity();
		if(f_ret == TRUE)  /* && (IsEngineCfg != FALSE) && (fl_PEPS_IMMO != FALSE)*/ /* AT && PEPS */
		{	
			if((FALSE == fl_animationFlag) && (fl_IGN_state == eIGN_RUN))
			{
				f_NotInPChime_Process();
			}
			else
			{
				f_NotInPChime_initial();
			}
		}
		else
		{ 
			f_NotInPChime_initial();
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

static void f_NotInPChime_initial(void) 
{
	l_NotInPChimeState_U8 = FALSE;
	//l_NotInPChime_Last_State_U8 = FALSE;
	// l_invalid_cnt = 0;
	fl_count_2s = 0;
	Rte_Call_rpCS_NotInPChime_UpdateStatus_Operation(CHIME_REQID_NOT_IN_P, FALSE);
}

static void f_NotInPChime_Process(void)
{
	uint8 fl_TCU_DisplayGear_signal_U8 = 0;
	uint8 fl_DoorStsFrntLe_U8 = 0;
	uint32 fl_VehSpdLgt_u8 = 0;
	//uint32 fl_PowerMode_u8 = 0;
	uint32 fl_No_In_P_VehSpdLgt_u8 = 0;
	//uint32 fl_No_In_P_EngSpeed_u8 = 0;
	eIgnState fl_IGN_state = eIGN_OFF;

	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear(&fl_TCU_DisplayGear_signal_U8);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&fl_DoorStsFrntLe_U8);	
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_No_In_P_VehSpdLgt_u8);
	Rte_Read_rpSR_SpdMdl_VehicleIndicatedspeed_value(&fl_VehSpdLgt_u8);
	//Rte_Read_rpSR_TITachMdl_value(&fl_No_In_P_EngSpeed_u8);
	//Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_PowerMode(&fl_PowerMode_u8);

	if(fl_IGN_state == eIGN_OFF)
	{
		fl_VehSpdLgt_u8 = 0;
	}

	if((fl_IGN_state == eIGN_OFF) && (fl_IGN_Last_state == eIGN_RUN))
	{
		f_NotInPChime_initial();
	}

	/* A Warning: Condition not satisfied,cancel immediately */	
	if((fl_DoorStsFrntLe_U8 == 2) 
	&& (fl_TCU_DisplayGear_signal_U8 != T_GEAR_OFF) 
	&& ((fl_VehSpdLgt_u8 < VehsepLgt_Value) || (fl_No_In_P_VehSpdLgt_u8 == 4095)))
	{
		if(fl_No_In_P_VehSpdLgt_u8 == 4095)
		{
			if(fl_count_2s >= 20)
			{
				fl_count_2s = 20;
				l_NotInPChimeState_U8 = FALSE;
			}
			else
			{
				fl_count_2s++;
			}
		}
		else
		{
			fl_count_2s = 0;
			l_NotInPChimeState_U8 = TRUE;
		}
		
	}
	else
	{
		l_NotInPChimeState_U8 = FALSE;
	}

#if 0
	/* B Warning: Condition not satisfied,cancel after 3s */
	if((fl_TCU_DisplayGear_signal_U8 != T_GEAR_OFF) 
	&& (fl_VehSpdLgt_u8 < VehsepLgt_Value)
	&& ((fl_PowerMode_u8 == 1) || (fl_PowerMode_u8 == 0)))
	{
		l_invalid_cnt = 0;
		
		l_NotInPChimeState_U8 = TRUE;
		l_NotInPChime_Last_State_U8 = l_NotInPChimeState_U8;
	}
	else
	{
		if(l_invalid_cnt >= 20)
		{
			l_invalid_cnt = 20;
			l_NotInPChimeState_U8 = FALSE;
		}
		else
		{
			l_invalid_cnt++;
			l_NotInPChimeState_U8 = l_NotInPChime_Last_State_U8;
		}
	}
#endif

	fl_IGN_Last_state = fl_IGN_state;
	Rte_Call_rpCS_NotInPChime_UpdateStatus_Operation(CHIME_REQID_NOT_IN_P, l_NotInPChimeState_U8);
}

static boolean f_NotInPChime_input_verity(void)
{
	boolean f_ret = TRUE;

	uint8 	fl_CAN_123_msg_status_U8 = 0;
	uint8	fl_CAN_123_nvr_status_U8 = 0;
	uint8 	fl_CAN_310_msg_status_U8 = 0;
	uint8	fl_CAN_310_nvr_status_U8 = 0;
	uint8   fl_CAN_221_nvr_status_U8 = 0;
	uint8   fl_CAN_221_msg_status_U8 = 0;
	//uint8   fl_CAN_325_nvr_status_U8 = 0;
	//uint8   fl_CAN_325_msg_status_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_NR(&fl_CAN_123_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_Tout(&fl_CAN_123_msg_status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&fl_CAN_310_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&fl_CAN_310_msg_status_U8);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_CAN_221_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_CAN_221_msg_status_U8);
	//Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_NR(&fl_CAN_325_nvr_status_U8);
	//Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_Tout(&fl_CAN_325_msg_status_U8);
	
	if((fl_CAN_123_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_123_msg_status_U8 == RTE_E_TIMEOUT)
	|| (fl_CAN_310_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_310_msg_status_U8 == RTE_E_TIMEOUT)
	|| (fl_CAN_221_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_221_msg_status_U8 == RTE_E_TIMEOUT))
	//|| (fl_CAN_325_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_325_msg_status_U8 == RTE_E_TIMEOUT))
	{
		f_ret = FALSE;
	}

	return f_ret;
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

