//---------------------------------------------------------------------------------------------------------------------
//
// VISTEON CORPORATION CONFIDENTIAL
// ________________________________
//
// [2015] Visteon Corporation
// All Rights Reserved.
//
// NOTICE: This is an unpublished work of authorship, which contains trade secrets.
// Visteon Corporation owns all rights to this work and intends to maintain it in confidence to preserve
// its trade secret status. Visteon Corporation reserves the right, under the copyright laws of the United States
// or those of any other country that may have jurisdiction, to protect this work as an unpublished work,
// in the event of an inadvertent or deliberate unauthorized publication. Visteon Corporation also reserves its rights
// under all copyright laws to protect this work as a published work, when appropriate.
// Those having access to this work may not copy it, use it, modify it, or disclose the information contained in it
// without the written authorization of Visteon Corporation.
//
//---------------------------------------------------------------------------------------------------------------------
#include "Rte.h"
#include "Rte_type.h"
#include "Rte_APBTtMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"
#include "TmExt_Cfg.h"
#include "TmExt_Timer.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(APBTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#if 0
#define APB_STATUS_0				  ((uint8)0x00)
#define APB_STATUS_1				  ((uint8)0x01)
#define APB_STATUS_2				  ((uint8)0x02)
#define APB_STATUS_3			   	  ((uint8)0x03)

#define VEHSEPLGT_VALUE 		 	  ((uint8)30)
#define APB_TIME_2S    			 	  ((uint8)0x42)
#define APB_TIME_3S    			 	  ((uint8)0x64)
#define APB_TIME_10S    		 	  ((uint16)0x14D)
#define APB_TIME_30S			 	  ((uint16)0x14D) // ((uint16)0x3E7)
#define cTT_APB_TIMER_3000_IN_MS  	  ((uint16)3000)
#define HW_Invalid					  ((uint16)0xFFF)
#define DATA_COUNT_END_FLAG		 	  ((uint8)0x0)
#define cTT_APBTT_PROVEOUT_TIME_IN_MS ((uint16)10000)
#endif
static boolean fl_APB_TT_IGN_OFF_flag = FALSE;
#define FUNCTION_APB_TIME_30MS       (30)  
#define FUNCTION_APB_TIME_2000MS     (2000 / FUNCTION_APB_TIME_30MS)  
#define FUNCTION_APB_TIME_6500MS     (6500 / FUNCTION_APB_TIME_30MS)   
#define FUNCTION_APB_TIME_10000MS    (10000 / FUNCTION_APB_TIME_30MS)  
#define FUNCTION_APB_TIME_30000MS    (30000 / FUNCTION_APB_TIME_30MS)   

#define FUNCTION_APB_VALUE_0		 (0)
#define FUNCTION_APB_VALUE_1		 (1)
#define FUNCTION_APB_VALUE_2		 (2)
#define FUNCTION_APB_VALUE_3		 (3)

#define SPEED_VALUE_3KM 		     (53)
#define SPEED_VALUE_INVALID		     (4095)

#define VEHICLE_Brake_Mechanical     (0)
#define VEHICLE_Brake_Electronic     (1)

#define APB_STATUS_VALUE_1   (1)
#define APB_STATUS_VALUE_2   (2)
#define APB_STATUS_VALUE_3   (3)
#define APB_STATUS_VALUE_4   (4)
#define APB_STATUS_VALUE_5   (5)
#define APB_STATUS_VALUE_6   (6)
#define APB_STATUS_VALUE_7   (7)
#define APB_STATUS_VALUE_8   (8)
#define APB_STATUS_VALUE_9   (9)
#define APB_STATUS_VALUE_10  (10)
#define APB_STATUS_VALUE_11  (11)
#define APB_STATUS_VALUE_12  (12)
#define APB_STATUS_VALUE_13  (13)
#define APB_STATUS_VALUE_14  (14)
#define APB_STATUS_VALUE_15  (15)
//#define APB_STATUS_VALUE_16  (16)

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
#if 0
static uint8  fl_IGN_Last_state 		   = eIGN_OFF;
static uint8  fl_APB_tt_sig_value_U8 	   = cTT_TURN_OFF;
static uint8  fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
static uint8  fl_APB_time_2S_u8			   = APB_TIME_2S;
static uint8  fl_APB_time_3S_u16		   = APB_TIME_3S;
static uint16 fl_APB_time_10S_u16		   = APB_TIME_10S;
static uint16 fl_APB_time_30S_u16		   = APB_TIME_30S;
static boolean fl_IGN_ON_Wait_3s_flag	   = FALSE;
static boolean fl_wait_reset_flag		   = FALSE;
static uint8 fl_ESP_EPBErrorStatus_Last_u8 = APB_STATUS_0;
static uint8 fl_ESP_EPBStatus_Last_u8      = APB_STATUS_0;
static uint16 fl_VehSpdLgt_Last_u16		   = 0;
#endif

static boolean	l_10000MS_Red1_Flag = TRUE;
static uint16	l_Proveout_Counter_10000MS_Red1_U16 = 0;

static boolean	l_10000MS_Red2_Flag = TRUE;
static uint16	l_Proveout_Counter_10000MS_Red2_U16 = 0;

static boolean	l_30000MS_Red3_Flag = TRUE;
static uint16	l_Proveout_Counter_30000MS_Red3_U16 = 0;

static boolean	l_30000MS_Red4_Flag = TRUE;
static uint16	l_Proveout_Counter_30000MS_Red4_U16 = 0;

static boolean	l_30000MS_Red5_Flag = TRUE;
static uint16	l_Proveout_Counter_30000MS_Red5_U16 = 0;

static boolean	l_30000MS_Red6_Flag = TRUE;
static uint16	l_Proveout_Counter_30000MS_Red6_U16 = 0;

static boolean	l_30000MS_Yellow1_Flag = TRUE;
static uint16	l_Proveout_Counter_30000MS_Yellow1_U16 = 0;

static boolean	l_30000MS_Yellow2_Flag = TRUE;
static uint16	l_Proveout_Counter_30000MS_Yellow2_U16 = 0;

static boolean	l_30000MS_Yellow3_Flag = TRUE;
static uint16	l_Proveout_Counter_30000MS_Yellow3_U16 = 0;

static boolean	l_30000MS_Yellow4_Flag = TRUE;
static uint16	l_Proveout_Counter_30000MS_Yellow4_U16 = 0;

static boolean	l_30000MS_Yellow5_Flag = TRUE;
static uint16	l_Proveout_Counter_30000MS_Yellow5_U16 = 0;

static boolean	l_30000MS_Yellow6_Flag = TRUE;
static uint16	l_Proveout_Counter_30000MS_Yellow6_U16 = 0;

static boolean	l_30000MS_Yellow7_Flag = TRUE;
static uint16	l_Proveout_Counter_30000MS_Yellow7_U16 = 0;

static boolean  l_Work_On_Flag = TRUE;
static uint8    l_Proveout_Counter_Work_On_U8 = 0;

static boolean  l_Action_On_Flag = TRUE;
static uint8    l_Proveout_Counter_Action_On_U8 = 0;



#if 0
static void fttmdl_wait_timer_reset_process(void);
static void fttmdl_APBTt_process(void);
static void fttmdl_APBTt_Scenario2_process(void);
static void fttmdl_APBTt_Init_Variable(void);
#endif



static void fttmdl_N356_APB_ParkBrake_process(void);
static void fttmdl_N356_EPB_process(void);
static void fttmdl_N356_EPB_Red_Work_On_process(void);
static void fttmdl_N356_EPB_Ylw_Work_On_process(void);
static void fttmdl_N356_EPB_Action_On_process(void);

static boolean f_Action_PowerOn_Self_10s_Red1_process(void);
static boolean f_Action_PowerOn_Self_10s_Red2_process(void);
static boolean f_Action_PowerOn_Self_30s_Red3_process(void);		
static boolean f_Action_PowerOn_Self_30s_Red4_process(void);		
static boolean f_Action_PowerOn_Self_30s_Red5_process(void);		
static boolean f_Action_PowerOn_Self_30s_Red6_process(void);		

static boolean f_Action_PowerOn_Self_30s_Yellow1_process(void);
static boolean f_Action_PowerOn_Self_30s_Yellow2_process(void);
static boolean f_Action_PowerOn_Self_30s_Yellow3_process(void);
static boolean f_Action_PowerOn_Self_30s_Yellow4_process(void);
static boolean f_Action_PowerOn_Self_30s_Yellow5_process(void);
static boolean f_Action_PowerOn_Self_30s_Yellow6_process(void);
static boolean f_Action_PowerOn_Self_30s_Yellow7_process(void);

static void fttmdl_N356_EPB_Action_On_Init(void);
static boolean f_Work_PowerOn_Self_7s_process(void);
static boolean f_PowerOn_Self_2s_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	//fttmdl_APBTt_Init_Variable();
	//Rte_Call_rpTimer_TmExt_Timer_Start(eAPBWarnTimer);
	Rte_Write_ppSR_APB_tt_state_value(FALSE);
	Rte_Write_ppSR_APB_Message1_tt_state_value(FALSE);
	Rte_Write_ppSR_APB_Message2_tt_state_value(FALSE);
	
	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_OFF);
	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,cTT_TURN_OFF);

	fttmdl_N356_EPB_Action_On_Init();
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
	//fttmdl_APBTt_Init_Variable();
	Rte_Write_ppSR_APB_tt_state_value(FALSE);
	Rte_Write_ppSR_APB_Message1_tt_state_value(FALSE);
	Rte_Write_ppSR_APB_Message2_tt_state_value(FALSE);
	
	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_OFF);
	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,cTT_TURN_OFF);

	fttmdl_N356_EPB_Action_On_Init();
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
	//fttmdl_APBTt_Init_Variable();
	Rte_Write_ppSR_APB_tt_state_value(FALSE);
	Rte_Write_ppSR_APB_Message1_tt_state_value(FALSE);
	Rte_Write_ppSR_APB_Message2_tt_state_value(FALSE);
	
	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_OFF);
	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,cTT_TURN_OFF);
	fttmdl_N356_EPB_Action_On_Init();
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
	//fttmdl_APBTt_Init_Variable();
	Rte_Write_ppSR_APB_tt_state_value(FALSE);
	Rte_Write_ppSR_APB_Message1_tt_state_value(FALSE);
	Rte_Write_ppSR_APB_Message2_tt_state_value(FALSE);
	
	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_OFF);
	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,cTT_TURN_OFF);
	fttmdl_N356_EPB_Action_On_Init();
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
	uint8 IsEngineCfg_Brake_Type = 0;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_BRAKE_TYPE,&IsEngineCfg_Brake_Type);	
	
	
	switch(IsEngineCfg_Brake_Type)
	{		
		case VEHICLE_Brake_Mechanical:
			fttmdl_N356_APB_ParkBrake_process();
			break;
		case VEHICLE_Brake_Electronic:
			fttmdl_N356_EPB_process();
			break;
		default:
			break;
			
	}
	
	//fttmdl_APBTt_process();

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

#if 0
static void fttmdl_APBTt_Init_Variable(void)
{
	fl_IGN_Last_state 	          = eIGN_OFF;
	fl_APB_tt_sig_value_U8 		  = cTT_TURN_OFF;
	fl_APB_fault_tt_sig_value_u8  = cTT_TURN_OFF;
	fl_APB_time_2S_u8			  = APB_TIME_2S;
	fl_APB_time_3S_u16			  = APB_TIME_3S;
	fl_APB_time_10S_u16 		  = APB_TIME_10S;
	fl_APB_time_30S_u16			  = APB_TIME_30S;
	fl_ESP_EPBErrorStatus_Last_u8 = APB_STATUS_0;
    fl_ESP_EPBStatus_Last_u8      = APB_STATUS_0;
	fl_IGN_ON_Wait_3s_flag 		  = FALSE;
	fl_wait_reset_flag 			  = FALSE;
	fl_VehSpdLgt_Last_u16		  = 0;
    fl_APB_TT_IGN_OFF_flag	      = FALSE;

	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTPARK_LAMPS,fl_APB_tt_sig_value_U8);
	// Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,fl_APB_tt_sig_value_U8);
	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,fl_APB_fault_tt_sig_value_u8);
}

static void fttmdl_wait_timer_reset_process(void)
{
	uint8 fl_ESP_EPBErrorStatus_u8 = 0;
	uint8 fl_ESP_EPBStatus_u8	   = 0;

    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus(&fl_ESP_EPBErrorStatus_u8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus(&fl_ESP_EPBStatus_u8);

	if(fl_ESP_EPBErrorStatus_Last_u8 != fl_ESP_EPBErrorStatus_u8)
	{
		fl_APB_time_10S_u16 = APB_TIME_10S;
	}

	if(fl_ESP_EPBStatus_Last_u8 != fl_ESP_EPBStatus_u8)
	{
		fl_APB_time_30S_u16 = APB_TIME_30S;
	}

	fl_ESP_EPBErrorStatus_Last_u8 = fl_ESP_EPBErrorStatus_u8;
	fl_ESP_EPBStatus_Last_u8 	  = fl_ESP_EPBStatus_u8;
}

static void fttmdl_APBTt_Scenario2_process(void)
{
	// Here fl_APB_tt_sig_value_U8 is 'Scenario 1' the state
	// will not appear 'Scenario 2'
	if(cTT_TURN_ON != fl_APB_tt_sig_value_U8)
	{
		fl_APB_tt_sig_value_U8 = cTT_TURN_OFF;
	}
	else
	{
		/* Keep the previous value */
	}

	fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
}


static void fttmdl_APBTt_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8_0x243_MsgNR   = 0;
	uint8 u8_0x243_MsgTout = 0;
	uint8 u8_0x221_MsgNR   = 0;
	uint8 u8_0x221_MsgTout = 0;

	uint8 Timer_status 				= 0;
	uint16 fl_VehSpdLgt_u16	  	    = 0;
	uint8  fl_ESP_EPBErrorStatus_u8 = 0;
	uint8  fl_ESP_EPBStatus_u8	    = 0;
	uint8  fl_EPBWarningMessage1_u8 = 0;
	uint8  fl_EPBWarningMessage2_u8 = 0;
	uint8  fl_EPBWarningMessage3_u8 = 0;

	uint8  fl_ign_substate = eIGN_OFF;

	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(&u8_0x243_MsgNR);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(&u8_0x243_MsgTout);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8_0x221_MsgNR);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8_0x221_MsgTout);

	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_VehSpdLgt_u16);
	//Rte_Read_rpSR_SpdMdl_VehicleIndicatedspeed_value(&fl_VehSpdLgt_u16);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus(&fl_ESP_EPBErrorStatus_u8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus(&fl_ESP_EPBStatus_u8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage1(&fl_EPBWarningMessage1_u8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage2(&fl_EPBWarningMessage2_u8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage3(&fl_EPBWarningMessage3_u8);

	/*  Read input missing status by CAN message ID  xxx */
	/*  Read input never received status by CAN message ID   xxx*/
	/* If CAN signal is Missing > timeout,TURN OFF TT */
	if(RTE_E_TIMEOUT == u8_0x243_MsgTout)
	{
		fl_APB_tt_sig_value_U8 		 = cTT_TURN_OFF;
		fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;

	}
	/* If CAN signal is Missing <= timeout, Retain previous */
	else if((RTE_E_NEVER_RECEIVED == u8_0x243_MsgNR) || (RTE_E_NEVER_RECEIVED == u8_0x221_MsgNR))
	{
		fl_APB_tt_sig_value_U8 		 = cTT_TURN_OFF;
		fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
	}
	else
	{
		#if 0
		if((eIGN_OFF == fl_IGN_Last_state) && (eIGN_RUN == fl_ign_substate))
		{
			Rte_Call_rpTimer_TmExt_Timer_Start(eAPBProveOutTimer);
			fl_IGN_ON_Wait_3s_flag = FALSE;
		}

		Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eAPBProveOutTimer, cTT_APB_TIMER_3000_IN_MS,&Timer_status);
		if((((uint8)1u) == Timer_status) && (FALSE == fl_IGN_ON_Wait_3s_flag))
		{
			//Rte_Call_rpTimer_TmExt_Timer_Stop(eAPBProveOutTimer);
			fl_IGN_ON_Wait_3s_flag = TRUE;
		}
		#else

		if(eIGN_OFF == fl_ign_substate)
		{
			fl_IGN_ON_Wait_3s_flag = FALSE;
			fl_APB_time_3S_u16	= APB_TIME_3S;
		}
		else if(fl_APB_time_3S_u16 > DATA_COUNT_END_FLAG)
		{
			fl_APB_time_3S_u16--;
		}
		else
		{
			fl_IGN_ON_Wait_3s_flag = TRUE;
		}

		#endif

		if(RTE_E_TIMEOUT == u8_0x221_MsgTout)
		{
			fl_APB_time_2S_u8 = APB_TIME_2S;
			fl_VehSpdLgt_u16 = 0;
		}
		else if(HW_Invalid == fl_VehSpdLgt_u16)
		{
			if(fl_APB_time_2S_u8 > DATA_COUNT_END_FLAG)
			{
				fl_VehSpdLgt_u16 = fl_VehSpdLgt_Last_u16;
				fl_APB_time_2S_u8--;
			}
			else
			{
				fl_VehSpdLgt_u16 = 0;
			}
		}
		else
		{
			fl_APB_time_2S_u8 = APB_TIME_2S;
			fl_VehSpdLgt_Last_u16 = fl_VehSpdLgt_u16;
		}

		if(((APB_STATUS_1 == fl_EPBWarningMessage1_u8)
		|| (APB_STATUS_1 == fl_EPBWarningMessage2_u8)
		|| (APB_STATUS_1 == fl_EPBWarningMessage3_u8))
		&& (TRUE == fl_IGN_ON_Wait_3s_flag))
		{
			fl_APB_tt_sig_value_U8 = cTT_TURN_ON;
		}
		else
		{
			fl_APB_tt_sig_value_U8 = cTT_TURN_OFF;
		}

		fttmdl_wait_timer_reset_process();

		switch(fl_ESP_EPBErrorStatus_u8) // ESP_EPBErrorStatus
		{
			case APB_STATUS_1:
				if(fl_VehSpdLgt_u16 <= VEHSEPLGT_VALUE) // < 3km/h
				{
					if((APB_STATUS_3 == fl_ESP_EPBStatus_u8)
					|| (APB_STATUS_2 == fl_ESP_EPBStatus_u8))
					{
						fl_APB_tt_sig_value_U8		 = cTT_FLASH_2HZ;
						fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
					}
					else if(APB_STATUS_1 == fl_ESP_EPBStatus_u8)
					{
						if(eIGN_RUN == fl_ign_substate)
						{
							fl_APB_tt_sig_value_U8		 = cTT_TURN_ON;
							fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;

							fl_APB_time_10S_u16 = APB_TIME_10S;
						}
						else
						{
							if(fl_APB_time_10S_u16 > DATA_COUNT_END_FLAG)
							{
								/* Turn on with 10 s then turn OFF */
								fl_APB_tt_sig_value_U8		 = cTT_TURN_ON;
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;

								fl_APB_time_10S_u16--;
							}
							else
							{
								//fl_APB_tt_sig_value_U8		 = cTT_TURN_OFF;
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
							}
						}
					}
					else
					{
						fttmdl_APBTt_Scenario2_process();
					}
				}
				else  // > 3km/h
				{
					if((APB_STATUS_3 == fl_ESP_EPBStatus_u8)
					|| (APB_STATUS_2 == fl_ESP_EPBStatus_u8))
					{
						fl_APB_tt_sig_value_U8 		 = cTT_FLASH_2HZ;
						fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
					}
					else
					{
						fttmdl_APBTt_Scenario2_process();
					}
				}
				break;
			case APB_STATUS_3:
				if(eIGN_RUN == fl_ign_substate)
				{
					if(APB_STATUS_0 == fl_ESP_EPBStatus_u8)
					{
						//fl_APB_tt_sig_value_U8		 = cTT_TURN_OFF;
						fl_APB_fault_tt_sig_value_u8 = cTT_FLASH_2HZ;
					}
					else if(APB_STATUS_1 == fl_ESP_EPBStatus_u8)
					{
						fl_APB_tt_sig_value_U8 		 = cTT_TURN_ON;
						fl_APB_fault_tt_sig_value_u8 = cTT_FLASH_2HZ;
					}
					else if((APB_STATUS_3 == fl_ESP_EPBStatus_u8)
						 || (APB_STATUS_2 == fl_ESP_EPBStatus_u8))
					{
						fl_APB_tt_sig_value_U8 		 = cTT_FLASH_2HZ;
						fl_APB_fault_tt_sig_value_u8 = cTT_FLASH_2HZ;
					}
					else
					{
						fttmdl_APBTt_Scenario2_process();
					}
				}
				else
				{
					fttmdl_APBTt_Scenario2_process();
				}
				break;
			case APB_STATUS_2:
				if(fl_VehSpdLgt_u16 <= VEHSEPLGT_VALUE) // 3km/h
				{
					if(TRUE == fl_wait_reset_flag)
					{
						fl_APB_time_30S_u16 = APB_TIME_30S;
						fl_wait_reset_flag = FALSE;
					}

					if(APB_STATUS_0 == fl_ESP_EPBStatus_u8)
					{
						if(eIGN_RUN == fl_ign_substate)
						{
							fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;

							fl_APB_time_30S_u16 = APB_TIME_30S;
						}
						else
						{
							if(fl_APB_time_30S_u16 > DATA_COUNT_END_FLAG)
							{
								/* Turn on with 30s  then oFF */
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;

								fl_APB_time_30S_u16--;
							}
							else
							{
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
							}
						}

						//fl_APB_tt_sig_value_U8	= cTT_TURN_OFF;

					}
					else if((APB_STATUS_3 == fl_ESP_EPBStatus_u8)
						 || (APB_STATUS_2 == fl_ESP_EPBStatus_u8))
					{
						if(eIGN_RUN == fl_ign_substate)
						{
							fl_APB_tt_sig_value_U8		 = cTT_FLASH_2HZ;
							fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;

							fl_APB_time_30S_u16 = APB_TIME_30S;
						}
						else
						{
							if(fl_APB_time_30S_u16 > DATA_COUNT_END_FLAG)
							{
								/* Turn on with 30s  then oFF */
								fl_APB_tt_sig_value_U8		 = cTT_FLASH_2HZ;
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;

								fl_APB_time_30S_u16--;
							}
							else
							{
								//fl_APB_tt_sig_value_U8 		 = cTT_TURN_OFF;
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
							}
						}
					}
					else if(APB_STATUS_1 == fl_ESP_EPBStatus_u8)
					{
						if(eIGN_RUN == fl_ign_substate)
						{
							fl_APB_tt_sig_value_U8		 = cTT_TURN_ON;
							fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;

							fl_APB_time_30S_u16 = APB_TIME_30S;
							fl_APB_time_10S_u16 = APB_TIME_10S;
						}
						else
						{
							if(fl_APB_time_30S_u16 > DATA_COUNT_END_FLAG)
							{
								/* Turn on with 30s  then oFF */
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;

								fl_APB_time_30S_u16--;
							}
							else
							{
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
							}

							if(fl_APB_time_10S_u16 > DATA_COUNT_END_FLAG)
							{
								/* Turn on with 10s  then oFF */
								fl_APB_tt_sig_value_U8 = cTT_TURN_ON;

								fl_APB_time_10S_u16--;
							}
							else
							{
								//fl_APB_tt_sig_value_U8 = cTT_TURN_OFF;
							}
						}
					}
					else
					{
						fttmdl_APBTt_Scenario2_process();
					}
				}
				else // > 3km/h
				{
					if(FALSE == fl_wait_reset_flag)
					{
						fl_APB_time_30S_u16 = APB_TIME_30S;
						fl_wait_reset_flag = TRUE;
					}

					if(APB_STATUS_0 == fl_ESP_EPBStatus_u8)
					{
						if(eIGN_RUN == fl_ign_substate)
						{
							fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;

							fl_APB_time_30S_u16 = APB_TIME_30S;
						}
						else
						{
							if(fl_APB_time_30S_u16 > DATA_COUNT_END_FLAG)
							{
								/* Turn on with 30s  then oFF */
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;

								fl_APB_time_30S_u16--;
							}
							else
							{
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
							}
						}

						//fl_APB_tt_sig_value_U8 = cTT_TURN_OFF;
					}
					else if((APB_STATUS_3 == fl_ESP_EPBStatus_u8)
						 || (APB_STATUS_2 == fl_ESP_EPBStatus_u8))
					{
						if(eIGN_RUN == fl_ign_substate)
						{
							fl_APB_tt_sig_value_U8		 = cTT_FLASH_2HZ;
							fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;

							fl_APB_time_30S_u16 = APB_TIME_30S;
						}
						else
						{
							if(fl_APB_time_30S_u16 > DATA_COUNT_END_FLAG)
							{
								/* Turn on with 30s  then oFF */
								fl_APB_tt_sig_value_U8		 = cTT_FLASH_2HZ;
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;

								fl_APB_time_30S_u16--;
							}
							else
							{
								//fl_APB_tt_sig_value_U8 		 = cTT_TURN_OFF;
								fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
							}
						}
					}
					else
					{
						fttmdl_APBTt_Scenario2_process();
					}
				}
				break;
			default:
				fttmdl_APBTt_Scenario2_process();
				break;
		}
	}


	if(eIGN_OFF == fl_ign_substate)
	{
		if((eIGN_RUN == fl_IGN_Last_state)
		&& (FALSE == fl_APB_TT_IGN_OFF_flag))
		{
			Rte_Call_rpTimer_TmExt_Timer_Start(eAPBWarnTimer);
		}
		else if(TRUE == fl_APB_TT_IGN_OFF_flag)
		{
			fl_APB_tt_sig_value_U8		 = cTT_TURN_OFF;
			fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
		}
	}
	else
	{
		Rte_Call_rpTimer_TmExt_Timer_Stop(eAPBWarnTimer);
		fl_APB_TT_IGN_OFF_flag = FALSE;
	}

	Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eAPBWarnTimer, cTT_APBTT_PROVEOUT_TIME_IN_MS,&Timer_status);
	if((FALSE == fl_APB_TT_IGN_OFF_flag)
	&& (((uint8)1u) == Timer_status))
	{
		/* Proveout Timer is Expired */
		fl_APB_TT_IGN_OFF_flag		   = TRUE;
		Rte_Call_rpTimer_TmExt_Timer_Stop(eAPBWarnTimer);

		fl_APB_tt_sig_value_U8 		 = cTT_TURN_OFF;
		fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
	}

	fl_IGN_Last_state = fl_ign_substate;

	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,fl_APB_tt_sig_value_U8);
	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,fl_APB_fault_tt_sig_value_u8);

#else

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		fl_APB_tt_sig_value_U8 = cTT_TURN_ON;
		Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,fl_APB_tt_sig_value_U8);

	}
	else if(u32TimeCnt < 100)
	{
		fl_APB_tt_sig_value_U8 = cTT_TURN_OFF;
		Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,fl_APB_tt_sig_value_U8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;

	static unsigned int u32TimeCnt_fault = 0;
	if(u32TimeCnt_fault < 50)
	{
		fl_APB_fault_tt_sig_value_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,fl_APB_fault_tt_sig_value_u8);

	}
	else if(u32TimeCnt_fault < 100)
	{
		fl_APB_fault_tt_sig_value_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,fl_APB_fault_tt_sig_value_u8);
	}
	else
	{
		u32TimeCnt_fault = 0;
	}
	u32TimeCnt_fault++;

#endif
}
#endif

#if 1
static void fttmdl_N356_APB_ParkBrake_process(void)
{
	uint8 u8Msg_Apb_NR313   = 0;
	uint8 u8Msg_Apb_Tout313 = 0;
	uint8 fl_Mechanical_Hand_brake = 0;
	uint8 fl_ign_substate = eIGN_OFF;
	uint8 fl_ParkBrake_Red_status_u8 = cTT_TURN_OFF; //function-36 apb red
	
	//function-36 apb_red
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_NR(&u8Msg_Apb_NR313);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_Tout(&u8Msg_Apb_Tout313);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_Mechanical_Hand_Brake(&fl_Mechanical_Hand_brake);
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	
	if(eIGN_RUN == fl_ign_substate)
	{	
		if(RTE_E_TIMEOUT == u8Msg_Apb_Tout313)
		{
			fl_ParkBrake_Red_status_u8 = cTT_TURN_ON;
		}
		else if(RTE_E_NEVER_RECEIVED == u8Msg_Apb_NR313)
		{
			f_Work_PowerOn_Self_7s_process();
			if(FALSE == l_Work_On_Flag)
			{
				fl_ParkBrake_Red_status_u8 = cTT_TURN_ON;
			}
		}
		else
		{
			if(FUNCTION_APB_VALUE_2 == fl_Mechanical_Hand_brake) 
			{
				fl_ParkBrake_Red_status_u8 = cTT_TURN_ON;
			}
			else
			{
				fl_ParkBrake_Red_status_u8 = cTT_TURN_OFF;
			}
		}
	}
	else
	{
		l_Work_On_Flag = TRUE;
		l_Proveout_Counter_Work_On_U8 = 0;
		
		fl_ParkBrake_Red_status_u8 = cTT_TURN_OFF;
	}

	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,fl_ParkBrake_Red_status_u8);
	
}

static void fttmdl_N356_EPB_process(void)
{
	uint8 fl_ign_substate = eIGN_OFF;
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);

	if(eIGN_RUN == fl_ign_substate)
	{	
		l_Action_On_Flag = TRUE;
		l_Proveout_Counter_Action_On_U8 = 0;
		
		f_Work_PowerOn_Self_7s_process();
		fttmdl_N356_EPB_Red_Work_On_process();
		fttmdl_N356_EPB_Ylw_Work_On_process();
		
		fttmdl_N356_EPB_Action_On_Init();
	}
	else
	{
		l_Work_On_Flag = TRUE;
		l_Proveout_Counter_Work_On_U8 = 0;
		
		Rte_Write_ppSR_APB_tt_state_value(FALSE);
		Rte_Write_ppSR_APB_Message1_tt_state_value(FALSE);
		Rte_Write_ppSR_APB_Message2_tt_state_value(FALSE);
		
		fttmdl_N356_EPB_Action_On_process();
	}

	
}


static void fttmdl_N356_EPB_Red_Work_On_process(void)
{
	uint8 l_epb_status_U8 = FALSE;
	uint8 l_epb_Message1_status_U8 = FALSE;
	uint8 l_epb_Message2_status_U8 = FALSE;
	
	uint8 fl_Epb_Red_status_1_u8 = cTT_TURN_OFF; //function-37-A epb_red
	uint8 fl_Epb_Red_status_2_u8 = cTT_TURN_OFF; //function-37-B epb_red
	
	uint8 u8_0x243_MsgNR   = 0;
	uint8 u8_0x243_MsgTout = 0;
	uint8 u8_0x221_MsgNR   = 0;
	uint8 u8_0x221_MsgTout = 0;
	
	uint8  fl_EPBWarningMessage1_u8 = 0;
	uint8  fl_EPBWarningMessage2_u8 = 0;
	uint8  fl_ESP_EPBStatus_u8 = 0;
	uint16 fl_VehSpdLgt_u16 = 0;
	uint8  fl_ESP_EPBErrorStatus_u8 = 0;
	
	uint8 fl_Epb_Red_status_1_Flag = FALSE;
	uint8 fl_Epb_Red_status_2_Flag = FALSE;

	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8_0x221_MsgNR);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8_0x221_MsgTout);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_VehSpdLgt_u16);
	
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(&u8_0x243_MsgNR);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(&u8_0x243_MsgTout);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage1(&fl_EPBWarningMessage1_u8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage2(&fl_EPBWarningMessage2_u8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus(&fl_ESP_EPBErrorStatus_u8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus(&fl_ESP_EPBStatus_u8);

	if((RTE_E_TIMEOUT == u8_0x221_MsgTout) || (RTE_E_NEVER_RECEIVED == u8_0x221_MsgNR) || (SPEED_VALUE_INVALID == fl_VehSpdLgt_u16))
	{
		fl_VehSpdLgt_u16 = 0;
	}
		
	if(RTE_E_TIMEOUT == u8_0x243_MsgTout)
	{
		Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_ON);
	}
	else if(RTE_E_NEVER_RECEIVED == u8_0x243_MsgNR)
	{
		
		if(FALSE == l_Work_On_Flag)
		{
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_ON);
		}
		else
		{
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_OFF);
		}
	}
	else
	{
		//function-37-A  epd_red fl_EPBWarningMessage1_u8 = 1
		if(FUNCTION_APB_VALUE_1 == fl_EPBWarningMessage1_u8)
		{
			fl_Epb_Red_status_1_u8 = cTT_TURN_ON;
			fl_Epb_Red_status_1_Flag = TRUE;
			l_epb_Message1_status_U8 = APB_STATUS_VALUE_14;
		}

		
		//function-37-A  epd_red fl_EPBWarningMessage2_u8 = 1
		if(FUNCTION_APB_VALUE_1 == fl_EPBWarningMessage2_u8)
		{
			fl_Epb_Red_status_1_u8 = cTT_TURN_ON;
			fl_Epb_Red_status_1_Flag = TRUE;
			l_epb_Message2_status_U8 = APB_STATUS_VALUE_15;
		}
		
		/**********************************************************************************************************************************************/

		//function-37-B red For ON  have 3 exam 
		if((FUNCTION_APB_VALUE_1 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_1 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_u8 = cTT_TURN_ON;
			fl_Epb_Red_status_2_Flag = TRUE;	
			l_epb_status_U8 = APB_STATUS_VALUE_1;
		}
		
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_1 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_u8 = cTT_TURN_ON;
			fl_Epb_Red_status_2_Flag = TRUE;
			l_epb_status_U8 = APB_STATUS_VALUE_2;
		}
		
		if((FUNCTION_APB_VALUE_3 == fl_ESP_EPBErrorStatus_u8) && (FUNCTION_APB_VALUE_1 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_u8 = cTT_TURN_ON;
			fl_Epb_Red_status_2_Flag = TRUE;
			l_epb_status_U8 = APB_STATUS_VALUE_3;
		}

		//function-37-B red For 2HZ  have 10 exam 
		
		// fl_ESP_EPBErrorStatus_u8 = 1 or 2
		if((FUNCTION_APB_VALUE_1 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
			fl_Epb_Red_status_2_Flag = TRUE;
			l_epb_status_U8 = APB_STATUS_VALUE_4;
		}
		
		if((FUNCTION_APB_VALUE_1 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
			fl_Epb_Red_status_2_Flag = TRUE;
			l_epb_status_U8 = APB_STATUS_VALUE_5;
		}		
		
		if((FUNCTION_APB_VALUE_1 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 > SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
			fl_Epb_Red_status_2_Flag = TRUE;
			l_epb_status_U8 = APB_STATUS_VALUE_6;
		}		
		
		if((FUNCTION_APB_VALUE_1 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 > SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
			fl_Epb_Red_status_2_Flag = TRUE;
			l_epb_status_U8 = APB_STATUS_VALUE_7;
		}	
		
		// fl_ESP_EPBErrorStatus_u8 =  2
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
			fl_Epb_Red_status_2_Flag = TRUE;
			l_epb_status_U8 = APB_STATUS_VALUE_8;
		}
		
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
			fl_Epb_Red_status_2_Flag = TRUE;
			l_epb_status_U8 = APB_STATUS_VALUE_9;
		}
		
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 > SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
			fl_Epb_Red_status_2_Flag = TRUE;
			l_epb_status_U8 = APB_STATUS_VALUE_10;
		}
		
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 > SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
			fl_Epb_Red_status_2_Flag = TRUE;	
			l_epb_status_U8 = APB_STATUS_VALUE_11;
		}

 		// fl_ESP_EPBErrorStatus_u8 = 3
		if((FUNCTION_APB_VALUE_3 == fl_ESP_EPBErrorStatus_u8) && (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8))
		{		
				fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;	
				fl_Epb_Red_status_2_Flag = TRUE;
				l_epb_status_U8 = APB_STATUS_VALUE_12;
		}
		
		if((FUNCTION_APB_VALUE_3 == fl_ESP_EPBErrorStatus_u8) && (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
		{			
				fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;	
				fl_Epb_Red_status_2_Flag = TRUE;
				l_epb_status_U8 = APB_STATUS_VALUE_13;
		}

		//compare epb red 37-A < 37-B
		if(TRUE == fl_Epb_Red_status_2_Flag)
		{
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,fl_Epb_Red_status_2_u8);
		}
		else if(TRUE == fl_Epb_Red_status_1_Flag)
		{	
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,fl_Epb_Red_status_1_u8);
		}
		else
		{
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_OFF);
		}
		
	}

	Rte_Write_ppSR_APB_tt_state_value(l_epb_status_U8);
	Rte_Write_ppSR_APB_Message1_tt_state_value(l_epb_Message1_status_U8);
	Rte_Write_ppSR_APB_Message2_tt_state_value(l_epb_Message2_status_U8);
	
}

static void fttmdl_N356_EPB_Ylw_Work_On_process(void)
{
	uint8 fl_Epb_Yellow_status_3_u8 = cTT_TURN_OFF; //function-37-B apd_yellow
	
	uint8 u8_0x243_MsgNR   = 0;
	uint8 u8_0x243_MsgTout = 0;
	uint8 u8_0x221_MsgNR   = 0;
	uint8 u8_0x221_MsgTout = 0;
	
	uint8  fl_ESP_EPBStatus_u8 = 0;
	uint16 fl_VehSpdLgt_u16 = 0;
	uint8  fl_ESP_EPBErrorStatus_u8 = 0;
	
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8_0x221_MsgNR);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8_0x221_MsgTout);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_VehSpdLgt_u16);
	
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(&u8_0x243_MsgNR);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(&u8_0x243_MsgTout);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus(&fl_ESP_EPBErrorStatus_u8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus(&fl_ESP_EPBStatus_u8);

	if((RTE_E_TIMEOUT == u8_0x221_MsgTout) || (RTE_E_NEVER_RECEIVED == u8_0x221_MsgNR) || (SPEED_VALUE_INVALID == fl_VehSpdLgt_u16))
	{
		fl_VehSpdLgt_u16 = 0;
	}
	
	if(RTE_E_TIMEOUT == u8_0x243_MsgTout)
	{
		fl_Epb_Yellow_status_3_u8 = cTT_TURN_ON;
	}
	else if(RTE_E_NEVER_RECEIVED == u8_0x243_MsgNR)
	{		
		if(FALSE == l_Work_On_Flag)
		{
			fl_Epb_Yellow_status_3_u8 = cTT_TURN_ON;
		}
		else
		{
			fl_Epb_Yellow_status_3_u8 = cTT_TURN_OFF;
		}
	}
	else
	{
	
		if(FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8)
		{
 			if(fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM)
			{
				if((FUNCTION_APB_VALUE_0 == fl_ESP_EPBStatus_u8) || (FUNCTION_APB_VALUE_1 == fl_ESP_EPBStatus_u8)
				|| (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8) || (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
				{				
					fl_Epb_Yellow_status_3_u8 = cTT_TURN_ON;
				}
			}
			else
			{
				if((FUNCTION_APB_VALUE_0 == fl_ESP_EPBStatus_u8) || (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8)
				|| (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
				{				
					fl_Epb_Yellow_status_3_u8 = cTT_TURN_ON;
				}
			}

		}
		else if(FUNCTION_APB_VALUE_3 == fl_ESP_EPBErrorStatus_u8)
		{
			if((FUNCTION_APB_VALUE_0 == fl_ESP_EPBStatus_u8) || (FUNCTION_APB_VALUE_1 == fl_ESP_EPBStatus_u8)
			|| (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8) || (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
			{
				fl_Epb_Yellow_status_3_u8 = cTT_FLASH_2HZ;
			}
			
		}
		else
		{
			//
		}
	}
	
	Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,fl_Epb_Yellow_status_3_u8);
	
}

static void fttmdl_N356_EPB_Action_On_process(void)
{
	
	uint8 u8_0x243_MsgNR   = 0;
	uint8 u8_0x243_MsgTout = 0;
	uint8 u8_0x221_MsgNR   = 0;
	uint8 u8_0x221_MsgTout = 0;

	uint8  fl_EPBWarningMessage1_u8 = 0;
	uint8  fl_EPBWarningMessage2_u8 = 0;
	uint8  fl_ESP_EPBStatus_u8 = 0;
	uint16 fl_VehSpdLgt_u16 = 0;
	uint8  fl_ESP_EPBErrorStatus_u8 = 0;

	uint8 fl_Epb_Red_status_1_u8 = cTT_TURN_OFF; //function-37-A epb_red
	uint8 fl_Epb_Red_status_2_u8 = cTT_TURN_OFF; //function-37-B epb_red
	uint8 fl_Epb_Yellow_status_3_u8 = cTT_TURN_OFF; //function-37-B apd_yellow
	uint8 fl_Epb_Red_status_1_Flag = FALSE;
	uint8 fl_Epb_Red_status_2_Flag = FALSE;

	boolean ret_Yellow = TRUE;
	boolean ret_Red = TRUE;
	
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(&u8_0x243_MsgNR);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(&u8_0x243_MsgTout);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8_0x221_MsgNR);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8_0x221_MsgTout);

	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage1(&fl_EPBWarningMessage1_u8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage2(&fl_EPBWarningMessage2_u8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus(&fl_ESP_EPBErrorStatus_u8);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_VehSpdLgt_u16);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus(&fl_ESP_EPBStatus_u8);

	if((RTE_E_TIMEOUT == u8_0x221_MsgTout) || (RTE_E_NEVER_RECEIVED == u8_0x221_MsgNR) || (SPEED_VALUE_INVALID == fl_VehSpdLgt_u16))
	{
		fl_VehSpdLgt_u16 = 0;
	}

	if(RTE_E_TIMEOUT == u8_0x243_MsgTout)
	{
		Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_ON);
		Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,cTT_TURN_ON);

		fttmdl_N356_EPB_Action_On_Init();
	}
	else if(RTE_E_NEVER_RECEIVED == u8_0x243_MsgNR)
	{
		f_PowerOn_Self_2s_process();
	
		if(FALSE == l_Action_On_Flag)
		{
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_ON);
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,cTT_TURN_ON);
		}
		else
		{
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_OFF);
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,cTT_TURN_OFF);
		}

		fttmdl_N356_EPB_Action_On_Init();
	}
	else
	{
		//function-37-A  epd_red
		if((FUNCTION_APB_VALUE_1 == fl_EPBWarningMessage1_u8) || (FUNCTION_APB_VALUE_1 == fl_EPBWarningMessage2_u8))
		{
			fl_Epb_Red_status_1_u8 = cTT_TURN_ON;
			fl_Epb_Red_status_1_Flag = TRUE;
		}
		else
		{
			fl_Epb_Red_status_1_u8 = cTT_TURN_OFF;
		}
		
		/*************************************************************************************************************************************************/
		//function-37-B epd_red

			//red 2HZ on ALL
		if((FUNCTION_APB_VALUE_1 == fl_ESP_EPBErrorStatus_u8) && (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_Flag = TRUE;
			fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
		}
		else if((FUNCTION_APB_VALUE_1 == fl_ESP_EPBErrorStatus_u8) && (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
		{
			fl_Epb_Red_status_2_Flag = TRUE;
			fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
		}
		else
		{
			fl_Epb_Red_status_2_u8 = cTT_TURN_OFF;
		}
		
		//apb-1 red ON 10s then off
		if((FUNCTION_APB_VALUE_1 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_1 == fl_ESP_EPBStatus_u8))
		{
			ret_Red = f_Action_PowerOn_Self_10s_Red1_process();
			if(ret_Red == FALSE)
			{
				fl_Epb_Red_status_2_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Red_status_2_u8 = cTT_TURN_ON;
				fl_Epb_Red_status_2_Flag = TRUE;
			}
		}
		else
		{
			l_Proveout_Counter_10000MS_Red1_U16 = 0;
		}
		
		//apb-2 red ON 10s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_1 == fl_ESP_EPBStatus_u8))
		{
			ret_Red = f_Action_PowerOn_Self_10s_Red2_process();
			if(ret_Red == FALSE)
			{
				fl_Epb_Red_status_2_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Red_status_2_u8 = cTT_TURN_ON;
				fl_Epb_Red_status_2_Flag = TRUE;
			}
		}
		else
		{
			l_Proveout_Counter_10000MS_Red2_U16 = 0;
		}
				
		//apb-3 red 2HZ 30s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8))
		{
			ret_Red = f_Action_PowerOn_Self_30s_Red3_process();
			if(ret_Red == FALSE)
			{
				fl_Epb_Red_status_2_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
				fl_Epb_Red_status_2_Flag = TRUE;
			}
		}
		else
		{
			l_Proveout_Counter_30000MS_Red3_U16 = 0;
		}

		//apb-4 red 2HZ 30s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
		{
			ret_Red = f_Action_PowerOn_Self_30s_Red4_process();
			if(ret_Red == FALSE)
			{
				fl_Epb_Red_status_2_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
				fl_Epb_Red_status_2_Flag = TRUE;
			}
		}
		else
		{
			l_Proveout_Counter_30000MS_Red4_U16 = 0;
		}

		//apb-5 red 2HZ 30s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 > SPEED_VALUE_3KM)  && (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8))
		{
			ret_Red = f_Action_PowerOn_Self_30s_Red5_process();
			if(ret_Red == FALSE)
			{
				fl_Epb_Red_status_2_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
				fl_Epb_Red_status_2_Flag = TRUE;
			}				
		}
		else
		{
			l_Proveout_Counter_30000MS_Red5_U16 = 0;
		}

		//apb-6 red 2HZ 30s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 > SPEED_VALUE_3KM)  && (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
		{
			ret_Red = f_Action_PowerOn_Self_30s_Red6_process();
			if(ret_Red == FALSE)
			{
				fl_Epb_Red_status_2_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Red_status_2_u8 = cTT_FLASH_2HZ;
				fl_Epb_Red_status_2_Flag = TRUE;
			}
		}
		else
		{
			l_Proveout_Counter_30000MS_Red6_U16 = 0;
		}

		
		//compare epb red 37-A and 37-B 
		if(TRUE == fl_Epb_Red_status_2_Flag)
		{
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,fl_Epb_Red_status_2_u8);
		}
		else if(TRUE == fl_Epb_Red_status_1_Flag)
		{
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,fl_Epb_Red_status_1_u8);
		}
		else
		{
			Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_OFF);
		}



		/*************************************************************************************************************************************************/
		//function-37-B epd_yellow

		//apb-1 yellow ON 30s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_0 == fl_ESP_EPBStatus_u8))
		{
			ret_Yellow = f_Action_PowerOn_Self_30s_Yellow1_process();
			if(ret_Yellow == FALSE)
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_ON;
			}
		}
		else
		{
			l_Proveout_Counter_30000MS_Yellow1_U16 = 0;
		}

		//apb-2 yellow ON 30s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_1 == fl_ESP_EPBStatus_u8))
		{
			ret_Yellow = f_Action_PowerOn_Self_30s_Yellow2_process();
			if(ret_Yellow == FALSE)
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_ON;
			}
		}
		else
		{
			l_Proveout_Counter_30000MS_Yellow2_U16 = 0;
		}

		//apb-3 yellow ON 30s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8))
		{
			ret_Yellow = f_Action_PowerOn_Self_30s_Yellow3_process();
			if(ret_Yellow == FALSE)
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_ON;
			}
		}
		else
		{
			l_Proveout_Counter_30000MS_Yellow3_U16 = 0;
		}
		
		//apb-4 yellow ON 30s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 <= SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
		{
			ret_Yellow = f_Action_PowerOn_Self_30s_Yellow4_process();
			if(ret_Yellow == FALSE)
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_ON;
			}
		}
		else
		{
			l_Proveout_Counter_30000MS_Yellow4_U16 = 0;
		}

		//apb-5 yellow ON 30s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 > SPEED_VALUE_3KM)  && (FUNCTION_APB_VALUE_0 == fl_ESP_EPBStatus_u8))
		{
			ret_Yellow = f_Action_PowerOn_Self_30s_Yellow5_process();
			if(ret_Yellow == FALSE)
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_ON;
			}
		}
		else
		{
			l_Proveout_Counter_30000MS_Yellow5_U16 = 0;
		}

		//apb-6 yellow ON 30s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 > SPEED_VALUE_3KM)  && (FUNCTION_APB_VALUE_2 == fl_ESP_EPBStatus_u8))
		{
			ret_Yellow = f_Action_PowerOn_Self_30s_Yellow6_process();
			if(ret_Yellow == FALSE)
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_ON;
			}
		}
		else
		{
			l_Proveout_Counter_30000MS_Yellow6_U16 = 0;
		}

		//apb-7 yellow ON 30s then off
		if((FUNCTION_APB_VALUE_2 == fl_ESP_EPBErrorStatus_u8) && (fl_VehSpdLgt_u16 > SPEED_VALUE_3KM) && (FUNCTION_APB_VALUE_3 == fl_ESP_EPBStatus_u8))
		{
			ret_Yellow = f_Action_PowerOn_Self_30s_Yellow7_process();
			if(ret_Yellow == FALSE)
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_OFF;
			}
			else
			{
				fl_Epb_Yellow_status_3_u8 = cTT_TURN_ON;
			}
		}
		else
		{
			l_Proveout_Counter_30000MS_Yellow7_U16 = 0;
		}
			
		Rte_Call_rpCS_APBTtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,fl_Epb_Yellow_status_3_u8);
	}
}


static boolean f_Action_PowerOn_Self_10s_Red1_process(void)
{

    if(l_Proveout_Counter_10000MS_Red1_U16 > FUNCTION_APB_TIME_10000MS)
    {
        l_10000MS_Red1_Flag = FALSE;
    }
    else
    {
        l_10000MS_Red1_Flag = TRUE;
		l_Proveout_Counter_10000MS_Red1_U16++;
    }
	
    return l_10000MS_Red1_Flag;
}

static boolean f_Action_PowerOn_Self_10s_Red2_process(void)
{

    if(l_Proveout_Counter_10000MS_Red2_U16 > FUNCTION_APB_TIME_10000MS)
    {
        l_10000MS_Red2_Flag = FALSE;
    }
    else
    {
        l_10000MS_Red2_Flag = TRUE;
		l_Proveout_Counter_10000MS_Red2_U16++;
    }
	
    return l_10000MS_Red2_Flag;
}


static boolean f_Action_PowerOn_Self_30s_Red3_process(void)
{

    if(l_Proveout_Counter_30000MS_Red3_U16 > FUNCTION_APB_TIME_30000MS)
    {
        l_30000MS_Red3_Flag = FALSE;
    }
    else
    {
        l_30000MS_Red3_Flag = TRUE;
		l_Proveout_Counter_30000MS_Red3_U16++;
    }
	
    return l_30000MS_Red3_Flag;
}

static boolean f_Action_PowerOn_Self_30s_Red4_process(void)
{

    if(l_Proveout_Counter_30000MS_Red4_U16 > FUNCTION_APB_TIME_30000MS)
    {
        l_30000MS_Red4_Flag = FALSE;
    }
    else
    {
        l_30000MS_Red4_Flag = TRUE;
		l_Proveout_Counter_30000MS_Red4_U16++;
    }
	
    return l_30000MS_Red4_Flag;
}

static boolean f_Action_PowerOn_Self_30s_Red5_process(void)
{

    if(l_Proveout_Counter_30000MS_Red5_U16 > FUNCTION_APB_TIME_30000MS)
    {
        l_30000MS_Red5_Flag = FALSE;
    }
    else
    {
        l_30000MS_Red5_Flag = TRUE;
		l_Proveout_Counter_30000MS_Red5_U16++;
    }
	
    return l_30000MS_Red5_Flag;
}

static boolean f_Action_PowerOn_Self_30s_Red6_process(void)
{

    if(l_Proveout_Counter_30000MS_Red6_U16 > FUNCTION_APB_TIME_30000MS)
    {
        l_30000MS_Red6_Flag = FALSE;
    }
    else
    {
        l_30000MS_Red6_Flag = TRUE;
		l_Proveout_Counter_30000MS_Red6_U16++;
    }
	
    return l_30000MS_Red6_Flag;
}

static boolean f_Action_PowerOn_Self_30s_Yellow1_process(void)
{

    if(l_Proveout_Counter_30000MS_Yellow1_U16 > FUNCTION_APB_TIME_30000MS)
    {
        l_30000MS_Yellow1_Flag = FALSE;
    }
    else
    {
        l_30000MS_Yellow1_Flag = TRUE;
		l_Proveout_Counter_30000MS_Yellow1_U16++;
    }
	
    return l_30000MS_Yellow1_Flag;
}

static boolean f_Action_PowerOn_Self_30s_Yellow2_process(void)
{

    if(l_Proveout_Counter_30000MS_Yellow2_U16 > FUNCTION_APB_TIME_30000MS)
    {
        l_30000MS_Yellow2_Flag = FALSE;
    }
    else
    {
        l_30000MS_Yellow2_Flag = TRUE;
		l_Proveout_Counter_30000MS_Yellow2_U16++;
    }
	
    return l_30000MS_Yellow2_Flag;
}

static boolean f_Action_PowerOn_Self_30s_Yellow3_process(void)
{

    if(l_Proveout_Counter_30000MS_Yellow3_U16 > FUNCTION_APB_TIME_30000MS)
    {
        l_30000MS_Yellow3_Flag = FALSE;
    }
    else
    {
        l_30000MS_Yellow3_Flag = TRUE;
		l_Proveout_Counter_30000MS_Yellow3_U16++;
    }
	
    return l_30000MS_Yellow3_Flag;
}

static boolean f_Action_PowerOn_Self_30s_Yellow4_process(void)
{

    if(l_Proveout_Counter_30000MS_Yellow4_U16 > FUNCTION_APB_TIME_30000MS)
    {
        l_30000MS_Yellow4_Flag = FALSE;
    }
    else
    {
        l_30000MS_Yellow4_Flag = TRUE;
		l_Proveout_Counter_30000MS_Yellow4_U16++;
    }
	
    return l_30000MS_Yellow4_Flag;
}

static boolean f_Action_PowerOn_Self_30s_Yellow5_process(void)
{

    if(l_Proveout_Counter_30000MS_Yellow5_U16 > FUNCTION_APB_TIME_30000MS)
    {
        l_30000MS_Yellow5_Flag = FALSE;
    }
    else
    {
        l_30000MS_Yellow5_Flag = TRUE;
		l_Proveout_Counter_30000MS_Yellow5_U16++;
    }
	
    return l_30000MS_Yellow5_Flag;
}

static boolean f_Action_PowerOn_Self_30s_Yellow6_process(void)
{

    if(l_Proveout_Counter_30000MS_Yellow6_U16 > FUNCTION_APB_TIME_30000MS)
    {
        l_30000MS_Yellow6_Flag = FALSE;
    }
    else
    {
        l_30000MS_Yellow6_Flag = TRUE;
		l_Proveout_Counter_30000MS_Yellow6_U16++;
    }
	
    return l_30000MS_Yellow6_Flag;
}

static boolean f_Action_PowerOn_Self_30s_Yellow7_process(void)
{

    if(l_Proveout_Counter_30000MS_Yellow7_U16 > FUNCTION_APB_TIME_30000MS)
    {
        l_30000MS_Yellow7_Flag = FALSE;
    }
    else
    {
        l_30000MS_Yellow7_Flag = TRUE;
		l_Proveout_Counter_30000MS_Yellow7_U16++;
    }
	
    return l_30000MS_Yellow7_Flag;
}

static void fttmdl_N356_EPB_Action_On_Init(void)
{
	// red init
	l_10000MS_Red1_Flag = TRUE;
	l_Proveout_Counter_10000MS_Red1_U16 = 0;
	
	l_10000MS_Red2_Flag = TRUE;
	l_Proveout_Counter_10000MS_Red2_U16 = 0;
	
	l_30000MS_Red3_Flag = TRUE;
	l_Proveout_Counter_30000MS_Red3_U16 = 0;
	
	l_30000MS_Red4_Flag = TRUE;
	l_Proveout_Counter_30000MS_Red4_U16 = 0;
	
	l_30000MS_Red5_Flag = TRUE;
	l_Proveout_Counter_30000MS_Red5_U16 = 0;
	
	l_30000MS_Red6_Flag = TRUE;
	l_Proveout_Counter_30000MS_Red6_U16 = 0;

	// yellow init
	l_30000MS_Yellow1_Flag = TRUE;
	l_Proveout_Counter_30000MS_Yellow1_U16 = 0;
	
	l_30000MS_Yellow2_Flag = TRUE;
	l_Proveout_Counter_30000MS_Yellow2_U16 = 0;
	
	l_30000MS_Yellow3_Flag = TRUE;
	l_Proveout_Counter_30000MS_Yellow3_U16 = 0;
	
	l_30000MS_Yellow4_Flag = TRUE;
	l_Proveout_Counter_30000MS_Yellow4_U16 = 0;
	
	l_30000MS_Yellow5_Flag = TRUE;
	l_Proveout_Counter_30000MS_Yellow5_U16 = 0;
	
	l_30000MS_Yellow6_Flag = TRUE;
	l_Proveout_Counter_30000MS_Yellow6_U16 = 0;
	
	l_30000MS_Yellow7_Flag = TRUE;
	l_Proveout_Counter_30000MS_Yellow7_U16 = 0;
	
}


static boolean f_Work_PowerOn_Self_7s_process(void)
{
    if(l_Proveout_Counter_Work_On_U8 >= FUNCTION_APB_TIME_6500MS)
    {
        l_Work_On_Flag = FALSE;
    }
    else
    {
        l_Work_On_Flag = TRUE;
		l_Proveout_Counter_Work_On_U8++;
    }
	
    return l_Work_On_Flag;
}

static boolean f_PowerOn_Self_2s_process(void)
{
    if(l_Proveout_Counter_Action_On_U8 >= FUNCTION_APB_TIME_2000MS)
    {
        l_Action_On_Flag = FALSE;
    }
    else
    {
        l_Action_On_Flag = TRUE;
		l_Proveout_Counter_Action_On_U8++;
    }
	
    return l_Action_On_Flag;
}

#endif
