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
#include "Rte_AVHTtMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(AVHTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8 fl_AVH_status_u8 = cTT_TURN_OFF;

#define STANDBY_AVH 		((uint8)1)

#define AVH_STATUS_VALUE_1   (1)
#define VEHICLE_Brake_AVH_Electronic     (1)

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static void fttmdl_N356_AVH_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	Rte_Call_rpCS_AVHTtMdl_TIMdlUpdateTtStatus_Operation(cTTAVH,cTT_TURN_OFF);
	Rte_Write_ppSR_AVH_tt_state_value(FALSE);
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
	Rte_Call_rpCS_AVHTtMdl_TIMdlUpdateTtStatus_Operation(cTTAVH,cTT_TURN_OFF);
	Rte_Write_ppSR_AVH_tt_state_value(FALSE);
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
	Rte_Call_rpCS_AVHTtMdl_TIMdlUpdateTtStatus_Operation(cTTAVH,cTT_TURN_OFF);
	Rte_Write_ppSR_AVH_tt_state_value(FALSE);
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
	Rte_Call_rpCS_AVHTtMdl_TIMdlUpdateTtStatus_Operation(cTTAVH,cTT_TURN_OFF);
	Rte_Write_ppSR_AVH_tt_state_value(FALSE);
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
	uint8 IsEngineCfg_Brake_Type = FALSE;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_BRAKE_TYPE,&IsEngineCfg_Brake_Type);	

	
	if(VEHICLE_Brake_AVH_Electronic == IsEngineCfg_Brake_Type)
	{
		fttmdl_N356_AVH_process();
	}
	else
	{
		//
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

static void fttmdl_N356_AVH_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 l_avh_status_U8 = FALSE;

	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;
	uint8 fl_AVH_sig_value_U8 = 0;
	uint8 fl_ign_substate = cTT_TURN_OFF;
	
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhActive(&fl_AVH_sig_value_U8);
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
		
	if(eIGN_RUN == fl_ign_substate)
	{
		if ( (RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR) )
		{
			fl_AVH_status_u8 = cTT_TURN_OFF;
			l_avh_status_U8 = FALSE;
		}
		else
		{	
			if(STANDBY_AVH == fl_AVH_sig_value_U8)
			{
				fl_AVH_status_u8 = cTT_TURN_ON;
				l_avh_status_U8 = AVH_STATUS_VALUE_1;
			}
			else
			{
				fl_AVH_status_u8 = cTT_TURN_OFF;
				l_avh_status_U8 = FALSE;
			}
		}	
	}
	else
	{
		fl_AVH_status_u8 = cTT_TURN_OFF;
		l_avh_status_U8 = FALSE;
	}
	
	Rte_Write_ppSR_AVH_tt_state_value(l_avh_status_U8);
	Rte_Call_rpCS_AVHTtMdl_TIMdlUpdateTtStatus_Operation(cTTAVH,fl_AVH_status_u8);
	
#else
	
		static unsigned int u32TimeCnt = 0;
		if(u32TimeCnt < 50)
		{
			fl_AVH_status_u8 = cTT_TURN_ON;
			Rte_Call_rpCS_AVHTtMdl_TIMdlUpdateTtStatus_Operation(cTTAVH,fl_AVH_status_u8);
	
		}
		else if(u32TimeCnt < 100)
		{
			fl_AVH_status_u8 = cTT_TURN_OFF;
			Rte_Call_rpCS_AVHTtMdl_TIMdlUpdateTtStatus_Operation(cTTAVH,fl_AVH_status_u8);
		}
		else
		{
			u32TimeCnt = 0;
		}
		u32TimeCnt++;
#endif

}

