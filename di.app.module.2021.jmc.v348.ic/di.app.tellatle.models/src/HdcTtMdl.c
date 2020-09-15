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
#include "Rte_HdcTtMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(HdcTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#if 0
#define Active_HDC_0_OFF ((uint8)0)
#define Active_HDC_1_ON  ((uint8)1)
#define Active_HDC_2_ON  ((uint8)2)
#define Active_HDC_3_ON  ((uint8)3)

#define STATE_RESET_0			   (0)
#define STATE_FLAG_3			   ((uint16)300)
#define INVALID_OVER_SPEED_FLAG_2S ((uint16)2000/30)
#define VEHICLE_SPEED_INVALID	   ((uint16)0xFFF)
#define OVERSPEED_MIN			   ((uint16)9000)
#define OVERSPEED_MAX			   ((uint16)16000)
#define OVERSPEED_VALUE			   ((uint16)12000)

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8  l_HDC_status_u8 			= Active_HDC_0_OFF;

static uint8  l_OverspeedState_U8 		= cTT_TURN_OFF;
static uint8  l_OverspeedState_U8_last  = cTT_TURN_OFF;
static uint16 l_Speed_Invalid_count 	= STATE_RESET_0;
static uint16 l_Voerspeed_Set_Value 	= OVERSPEED_VALUE;
static uint16 l_Voerspeed_Set_Value_ret = OVERSPEED_VALUE;
static uint16 l_Voerspeed_New_Set_Value = OVERSPEED_VALUE;
static uint16 fl_ABS_VehSpdLgt_u16 		= STATE_RESET_0;
static uint32 fl_SpdMdl_u32 			= STATE_RESET_0;
static uint32 l_Speed_Last_u16 			= STATE_RESET_0;

static void Overspeed_Init_Variable(void);
static void Overspeed_set_Value(void);
static void Vehicle_Speed_Invalid(void);
static void fttmdl_N356HDC_process(void);
static void Fttmdl_CX756_OverSpeed_Process(void);
#endif
static uint8  l_OverspeedState_U8 		= cTT_TURN_OFF;
static uint16 fl_ABS_VehSpdLgt_u16      = 0;
static uint16 fl_last_ABS_VehSpdLgt_u16 = 0;

#define OverSpd_INVALID_SPEED_4095      (4095)
#define OverSpd_SPEED_VALUE_NOT         (4113)
#define OverSpd_SPEED_VALUE_80KM        (1422)
#define OverSpd_SPEED_VALUE_100KM       (1777)
#define OverSpd_SPEED_VALUE_120KM       (2133)

static void fttmdl_N356_OverSpeed_process(void);


//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	//l_HDC_status_u8 = cTT_TURN_OFF;
	//Overspeed_Init_Variable();

	//Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(cTTHDC,l_HDC_status_u8);
	
	Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(CTT_OVERSPEED,cTT_TURN_OFF);
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
	//l_HDC_status_u8 = cTT_TURN_OFF;
	//Overspeed_Init_Variable();	
	//Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(cTTHDC,l_HDC_status_u8);

	Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(CTT_OVERSPEED,cTT_TURN_OFF);
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
	//l_HDC_status_u8 = cTT_TURN_OFF;
	//Overspeed_Init_Variable();	
	//Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(cTTHDC,l_HDC_status_u8);

	Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(CTT_OVERSPEED,cTT_TURN_OFF);
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
	//l_HDC_status_u8 = cTT_TURN_OFF;
	//Overspeed_Init_Variable();
	//Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(cTTHDC,l_HDC_status_u8);

	Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(CTT_OVERSPEED,cTT_TURN_OFF);
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
	//fttmdl_N356HDC_process();
	//Fttmdl_CX756_OverSpeed_Process();
	fttmdl_N356_OverSpeed_process();
	
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

/*
--Hdct indicator--
input signal =>
CAN signal (0x140):
	1. ESC_HDC_DisplayMessageReq

		|	Input								|Output
		|	ESC_HDC_DisplayMessageReq			|Indicator
---------------------------------------------------------
		|	0									|turn off
		|	1									|turn on
---------------------------------------------------------
ps:
*/
/*********************************************************************************
		
	Function Name	  :  fttmdl_N356glow_process (void)
	
	Description 	  : Hdct Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
#if 0
static void fttmdl_N356HDC_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;
	uint8 fl_HDC_sig_value_U8 = 0;
	
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_HDCActive(&fl_HDC_sig_value_U8);
	
	if ((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
	{
		l_HDC_status_u8 = cTT_TURN_OFF;
	}
	else
	{
		if((Active_HDC_1_ON == fl_HDC_sig_value_U8) || (Active_HDC_2_ON == fl_HDC_sig_value_U8))
		{
			l_HDC_status_u8 = cTT_TURN_ON;
		}
		else
		{
			l_HDC_status_u8 = cTT_TURN_OFF;
		}
	}	
	Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(cTTHDC,l_HDC_status_u8);
#else
	
		static unsigned int u32TimeCnt = 0;
		if(u32TimeCnt < 50)
		{
			l_HDC_status_u8 = cTT_TURN_ON;
			Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(cTTHDC,l_HDC_status_u8);
	
		}
		else if(u32TimeCnt < 100)
		{
			l_HDC_status_u8 = cTT_TURN_OFF;
			Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(cTTHDC,l_HDC_status_u8);
		}
		else
		{
			u32TimeCnt = 0;
		}
		u32TimeCnt++;
#endif

}
#endif

static void fttmdl_N356_OverSpeed_process(void)
{
	uint8  u8Msg_0x221_NR = 0;
	uint8  u8Msg_0x221_Tout = 0;
	uint8 fl_ign_substate	  = eIGN_OFF;

	uint8 IsEngineCfg_OverSpeed = 0;
	uint16 fl_ABS_VehSpdLgt_Normal_u16 = 0;
	uint16 fl_ABS_VehSpdLgt_Wanring_u16 = 0;
	uint16 fl_ABS_VehSpdLgt_Safe_u16 = 0;
	
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_OVER_SPEED,&IsEngineCfg_OverSpeed);
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8Msg_0x221_NR);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8Msg_0x221_Tout);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_ABS_VehSpdLgt_u16);
	
	switch(IsEngineCfg_OverSpeed)
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
	
	if(eIGN_RUN == fl_ign_substate)
	{
		if((RTE_E_NEVER_RECEIVED == u8Msg_0x221_NR)|| (RTE_E_TIMEOUT == u8Msg_0x221_Tout))
		{
			l_OverspeedState_U8 = cTT_TURN_OFF;
		}
		else
		{
			if(OverSpd_INVALID_SPEED_4095 != fl_ABS_VehSpdLgt_u16)
			{
				if(fl_ABS_VehSpdLgt_u16 > fl_ABS_VehSpdLgt_Wanring_u16 )
				{
					l_OverspeedState_U8 = cTT_TURN_ON;
				}
				else if(fl_ABS_VehSpdLgt_u16 <= fl_ABS_VehSpdLgt_Safe_u16)
				{
					l_OverspeedState_U8 = cTT_TURN_OFF;
				}
				else
				{
					/*keep last state*/
				}
			}
			else
			{
				l_OverspeedState_U8 = cTT_TURN_OFF;
			}
		}
	}

	Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(CTT_OVERSPEED,l_OverspeedState_U8);
}
#if 0
static void Fttmdl_CX756_OverSpeed_Process(void)
{
#ifndef TT_ONOFF_TEST

	uint8  u8Msg_0x221_NR = 0;
	uint8  u8Msg_0x221_Tout = 0;

	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8Msg_0x221_NR);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8Msg_0x221_Tout);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_ABS_VehSpdLgt_u16);
	Rte_Read_rpSR_SpdMdl_VehicleIndicatedspeed_value(&fl_SpdMdl_u32);

	if((RTE_E_NEVER_RECEIVED == u8Msg_0x221_NR)|| (RTE_E_TIMEOUT == u8Msg_0x221_Tout))
	{
		fl_ABS_VehSpdLgt_u16 = 0;
		l_Speed_Invalid_count = STATE_RESET_0;
		l_OverspeedState_U8 = cTT_TURN_OFF;
	}
	else
	{
		Vehicle_Speed_Invalid();
		
		Overspeed_set_Value();
		
		if(l_Voerspeed_Set_Value <= fl_SpdMdl_u32)
		{
			l_OverspeedState_U8 = cTT_TURN_ON;
		}
		else if(l_Voerspeed_Set_Value_ret <= fl_SpdMdl_u32)
		{
			if(cTT_TURN_ON == l_OverspeedState_U8_last)
			{
				l_OverspeedState_U8 = cTT_TURN_ON;
			}
			else
			{
				l_OverspeedState_U8 = cTT_TURN_OFF;
			}
		}
		else
		{
			l_OverspeedState_U8 = cTT_TURN_OFF;
		}
	}

	l_OverspeedState_U8_last = l_OverspeedState_U8;

	Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(CTT_OVERSPEED,l_OverspeedState_U8);

#else
		
	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_OverspeedState_U8 = cTT_TURN_ON;
		Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(cTTHDC,l_OverspeedState_U8);
		
	}
	else if(u32TimeCnt < 100)
	{
		l_OverspeedState_U8 = cTT_TURN_OFF;
		Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(cTTHDC,l_OverspeedState_U8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;

#endif
}


static void Overspeed_Init_Variable(void)
{
	l_Speed_Invalid_count 	  = STATE_RESET_0;
	l_Speed_Last_u16 		  = STATE_RESET_0;
	l_Voerspeed_Set_Value 	  = OVERSPEED_VALUE;
	l_Voerspeed_Set_Value_ret = l_Voerspeed_Set_Value - STATE_FLAG_3;
	l_OverspeedState_U8 	  = cTT_TURN_OFF;
	l_OverspeedState_U8_last  = cTT_TURN_OFF;
	l_Voerspeed_New_Set_Value = OVERSPEED_VALUE;
	fl_ABS_VehSpdLgt_u16 	  = STATE_RESET_0;
	fl_SpdMdl_u32 			  = STATE_RESET_0;
	
	Rte_Call_rpCS_HdcTtMdl_TIMdlUpdateTtStatus_Operation(CTT_OVERSPEED,l_OverspeedState_U8);

}

static void Overspeed_set_Value(void)
{
	//Rte_Call_rpCS_Value(l_Voerspeed_New_Set_Value);

	if((OVERSPEED_MIN <= l_Voerspeed_New_Set_Value) 
	&& (OVERSPEED_MAX >= l_Voerspeed_New_Set_Value))
	{
		l_Voerspeed_Set_Value = l_Voerspeed_New_Set_Value;
		l_Voerspeed_Set_Value_ret = l_Voerspeed_Set_Value - STATE_FLAG_3;
	}
}

static void Vehicle_Speed_Invalid(void)
{
	if(VEHICLE_SPEED_INVALID == fl_ABS_VehSpdLgt_u16)
	{
		if(INVALID_OVER_SPEED_FLAG_2S > l_Speed_Invalid_count)
		{
			l_Speed_Invalid_count++;
			fl_SpdMdl_u32 = l_Speed_Last_u16;
		}
		else
		{
			fl_SpdMdl_u32    = 0;
			l_Speed_Last_u16 = fl_SpdMdl_u32;
		}
	}
	else
	{
		l_Speed_Invalid_count = STATE_RESET_0;
		l_Speed_Last_u16	  = fl_SpdMdl_u32;
	}
}
#endif

