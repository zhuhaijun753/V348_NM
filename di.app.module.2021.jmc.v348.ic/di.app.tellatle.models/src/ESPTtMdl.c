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
#include "Rte_ESPTtMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"
#include "TmExt_Cfg.h"
#include "TmExt_Timer.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(ESPTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

#define TURN_OFF_ESPTT ((uint8)0)
#define TURN_ON_ESPTT  ((uint8)1)

#define FUNCTION_ESP_TIME_30MS                (30)           
#define FUNCTION_ESP_TIME_6500MS              (6500 / FUNCTION_ESP_TIME_30MS)      

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static uint8 l_esp_status_u8 = cTT_TURN_OFF;

static boolean  l_ESP_Flag = TRUE;
static uint8    l_Proveout_Counter_U8 = FALSE;

static void fttmdl_N356ESP_process(void);
static boolean f_ESP_PowerOn_Self_7s_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_esp_status_u8	= cTT_TURN_OFF;
	
	Rte_Call_rpCS_ESPTtMdl_TIMdlUpdateTtStatus_Operation(cTTESP,l_esp_status_u8);

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
	l_esp_status_u8	= cTT_TURN_OFF;
	
	Rte_Call_rpCS_ESPTtMdl_TIMdlUpdateTtStatus_Operation(cTTESP,l_esp_status_u8);

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
	l_esp_status_u8	= cTT_TURN_OFF;
	
	Rte_Call_rpCS_ESPTtMdl_TIMdlUpdateTtStatus_Operation(cTTESP,l_esp_status_u8);

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
	l_esp_status_u8	= cTT_TURN_OFF;
	
	Rte_Call_rpCS_ESPTtMdl_TIMdlUpdateTtStatus_Operation(cTTESP,l_esp_status_u8);

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
	
	uint8 IsEngineCfg_Esp = FALSE;	
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ESP,&IsEngineCfg_Esp);

	if(TRUE == IsEngineCfg_Esp)
	{
		fttmdl_N356ESP_process();
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


static void fttmdl_N356ESP_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR243 = 0;
	uint8 u8MsgTout243 = 0;
	
	uint8 fl_ign_substate		  	 = eIGN_OFF;
	uint8 fl_TcsFlgFlt_sig_val_U8	 = TURN_OFF_ESPTT;
	uint8 fl_VdcFlgFlt_sig_val_U8 	 = TURN_OFF_ESPTT;
	uint8 fl_ESP_InfoLamp_sig_val_U8 = TURN_OFF_ESPTT;
	uint8 l_esp_off_status_u8       = 0;

	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(&u8MsgNR243);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(&u8MsgTout243);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ESPInfoLamp(&fl_ESP_InfoLamp_sig_val_U8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_VdcFlgFlt(&fl_VdcFlgFlt_sig_val_U8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_TcsFlgFlt(&fl_TcsFlgFlt_sig_val_U8);
	Rte_Read_ESPTtMdl_rpSR_ESPOFF_tt_ESPOFF_state_value_l_esp_off_status_u8(&l_esp_off_status_u8);

	if( eIGN_RUN == fl_ign_substate )
	{	
		if(RTE_E_TIMEOUT == u8MsgTout243)
		{
			l_esp_status_u8 = cTT_TURN_ON;
		}
		else if (RTE_E_NEVER_RECEIVED == u8MsgNR243)
		{
			f_ESP_PowerOn_Self_7s_Action();
			
			if(FALSE == l_ESP_Flag)
			{
				l_esp_status_u8 = cTT_TURN_ON;
			}
		}
		else
		{	
			if(1 == l_esp_off_status_u8)
			{
				l_esp_status_u8 = cTT_TURN_OFF;
			}
			else 
			{
				if((TURN_ON_ESPTT == fl_VdcFlgFlt_sig_val_U8) || (TURN_ON_ESPTT == fl_TcsFlgFlt_sig_val_U8))
				{
					l_esp_status_u8 = cTT_TURN_ON;
				}
				else if( TURN_ON_ESPTT == fl_ESP_InfoLamp_sig_val_U8)
				{
					l_esp_status_u8 = cTT_FLASH_1HZ;
				}
				else
				{
					l_esp_status_u8 = cTT_TURN_OFF;
				}
			}
		}
	}
	else
	{
		l_esp_status_u8 = cTT_TURN_OFF;
	
		l_ESP_Flag = TRUE;
		l_Proveout_Counter_U8 = FALSE;
	}
	
	Rte_Call_rpCS_ESPTtMdl_TIMdlUpdateTtStatus_Operation(cTTESP,l_esp_status_u8);
	
#else

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_esp_status_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_ESPTtMdl_TIMdlUpdateTtStatus_Operation(cTTESP,l_esp_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_esp_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_ESPTtMdl_TIMdlUpdateTtStatus_Operation(cTTESP,l_esp_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
	
#endif
}

static boolean f_ESP_PowerOn_Self_7s_Action(void)
{
    if(l_Proveout_Counter_U8 >= FUNCTION_ESP_TIME_6500MS)
    {
        l_ESP_Flag = FALSE;
    }
    else
    {
        l_ESP_Flag = TRUE;
		l_Proveout_Counter_U8++;
    }
	
    return l_ESP_Flag;
}


