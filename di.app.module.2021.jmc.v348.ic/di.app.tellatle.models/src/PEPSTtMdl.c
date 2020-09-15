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
#include "Rte_PEPSTtMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(PEPSTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8 l_peps_status_u8 = cTT_TURN_OFF; 

#define TURN_PEPS_VALUE_1  ((uint8)0x01)

#define VEHICLE_1_IMMO  (1)
#define VEHICLE_2_PEPS  (2)

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static void fttmdl_N356PEPS_process(void);
static void fttmdl_IMMO_326_KeyLowBattery_process(void);
static void fttmdl_PEPS_325_process(void);




//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_peps_status_u8 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_PEPSTtMdl_TIMdlUpdateTtStatus_Operation(cTTPEPS_FAULT,l_peps_status_u8);
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
	l_peps_status_u8 = cTT_TURN_OFF;

	Rte_Call_rpCS_PEPSTtMdl_TIMdlUpdateTtStatus_Operation(cTTPEPS_FAULT,l_peps_status_u8);
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
	l_peps_status_u8 = cTT_TURN_OFF;

	Rte_Call_rpCS_PEPSTtMdl_TIMdlUpdateTtStatus_Operation(cTTPEPS_FAULT,l_peps_status_u8);
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
	l_peps_status_u8 = cTT_TURN_OFF;

	Rte_Call_rpCS_PEPSTtMdl_TIMdlUpdateTtStatus_Operation(cTTPEPS_FAULT,l_peps_status_u8);
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
	fttmdl_N356PEPS_process();
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


static void fttmdl_IMMO_326_KeyLowBattery_process(void)
{
	uint8 u8MsgNR   = 0;
	uint8 u8MsgTout = 0;
	uint8 fl_RF_keyLowBatteryWaring_U8 = 0;

	Rte_Read_rpSR_CANMSG_RF_0x326_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_RF_0x326_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_RF_0x326_ComIn_RF_KeyLowBatteryWarning(&fl_RF_keyLowBatteryWaring_U8);
	if((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
	{
		l_peps_status_u8 = cTT_TURN_OFF;
	}
	else
	{
		if(TURN_PEPS_VALUE_1 == fl_RF_keyLowBatteryWaring_U8)
		{
			l_peps_status_u8 = cTT_TURN_ON;
		}
		else
		{
			l_peps_status_u8 = cTT_TURN_OFF;
		}
	}
}


static void fttmdl_PEPS_325_process(void)
{
	uint8 u8MsgNR   = 0;
	uint8 u8MsgTout = 0;
	
	uint8 fl_KeyReminderWarning_U8  = 0;
	uint8 fl_KeyOutWarning1_U8	   = 0;
	uint8 fl_NoKeyFoundWarning_U8  = 0;

	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyReminderWarning(&fl_KeyReminderWarning_U8);
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyOutWarning1(&fl_KeyOutWarning1_U8);
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NoKeyFoundWarning(&fl_NoKeyFoundWarning_U8);

	if((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
	{
		l_peps_status_u8 = cTT_TURN_OFF;
	}
	else
	{
		if((TURN_PEPS_VALUE_1 == fl_KeyReminderWarning_U8)
		|| (TURN_PEPS_VALUE_1 == fl_KeyOutWarning1_U8)
		|| (TURN_PEPS_VALUE_1 == fl_NoKeyFoundWarning_U8))
		{
			l_peps_status_u8 = cTT_TURN_ON;
		}
		else
		{
			l_peps_status_u8 = cTT_TURN_OFF;
		}
	}
}


static void fttmdl_N356PEPS_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 IsEngineCfg_Key_Battery = 0;
	
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_PEPS,&IsEngineCfg_Key_Battery);

	switch(IsEngineCfg_Key_Battery)
	{
		case VEHICLE_1_IMMO:
			fttmdl_IMMO_326_KeyLowBattery_process();
		break;
		case VEHICLE_2_PEPS:
			fttmdl_PEPS_325_process();
		break;
		default:
			//
		break;
	}

	Rte_Call_rpCS_PEPSTtMdl_TIMdlUpdateTtStatus_Operation(cTTPEPS_FAULT,l_peps_status_u8);
	

#else

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_peps_status_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_PEPSTtMdl_TIMdlUpdateTtStatus_Operation(cTTPEPS_FAULT,l_peps_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_peps_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_PEPSTtMdl_TIMdlUpdateTtStatus_Operation(cTTPEPS_FAULT,l_peps_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
	
#endif
}

