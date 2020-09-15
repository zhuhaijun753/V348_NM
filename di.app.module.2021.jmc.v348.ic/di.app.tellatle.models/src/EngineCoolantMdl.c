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
#include "Rte_EngineCoolantMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(EngineCoolantMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

#define NVM_ENG_COOLANT_TT_ON_THRESHOLD  ((uint16)11400)
#define NVM_ENG_COOLANT_TT_OFF_THRESHOLD ((uint16)11000)
#define ENG_COOLANT_TT_INVALID			 ((uint8)0xff)
#define ENG_COOLANT_TT_INVALID_TIMEOUT   ((uint16)2000)
/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 l_CoolantTempHot_status_red_u8 = cTT_TURN_OFF;
static uint8 l_CoolantTempHot_status_white_u8 = cTT_TURN_OFF;



static void fttmdl_N356CoolantTempHot_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_CoolantTempHot_status_red_u8 = cTT_TURN_OFF;
	l_CoolantTempHot_status_white_u8 = cTT_TURN_OFF;

	Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT,l_CoolantTempHot_status_red_u8);
	Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT_WHITE,l_CoolantTempHot_status_white_u8);
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
	l_CoolantTempHot_status_red_u8 = cTT_TURN_OFF;
	l_CoolantTempHot_status_white_u8 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT,l_CoolantTempHot_status_red_u8);
	Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT_WHITE,l_CoolantTempHot_status_white_u8);
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
	l_CoolantTempHot_status_red_u8 = cTT_TURN_OFF;
	l_CoolantTempHot_status_white_u8 = cTT_TURN_OFF;

	Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT,l_CoolantTempHot_status_red_u8);
	Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT_WHITE,l_CoolantTempHot_status_white_u8);
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
	l_CoolantTempHot_status_red_u8 = cTT_TURN_OFF;
	l_CoolantTempHot_status_white_u8 = cTT_TURN_OFF;

	Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT,l_CoolantTempHot_status_red_u8);
	Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT_WHITE,l_CoolantTempHot_status_white_u8);
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
	fttmdl_N356CoolantTempHot_process();

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
--Engine coolant temperature high warning indicator--
input signal =>
CAN signal (0x300):
	1. EMS_EnginecoolanttempHighWarning
	

		|	Input										|Output
		|	EMS_EnginecoolanttempHighWarning			|Indicator
-------------------------------------------------------------------
IGN ON	|	0											|turn off
		|	1											|turn on
-------------------------------------------------------------------
IGN OFF	|	NC											|turn off
*/
/*********************************************************************************
		
	Function Name	  :  fttmdl_N356CoolantTempHot_process (void)
	
	Description 	  : Engine coolant temperature high warning Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
static void fttmdl_N356CoolantTempHot_process(void)
{
#ifndef TT_ONOFF_TEST

	Boolean fl_high_temp_status_BOOL = FALSE;
	eIgnState fl_IGN_state = FALSE;


	Rte_Read_rpSR_WaterTempHigh_STATUS(&fl_high_temp_status_BOOL);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

	if (FALSE != fl_high_temp_status_BOOL)
	{
		l_CoolantTempHot_status_red_u8 = cTT_TURN_ON;
		l_CoolantTempHot_status_white_u8 = cTT_TURN_OFF;
	}
	else
	{
		l_CoolantTempHot_status_red_u8 = cTT_TURN_OFF;
		l_CoolantTempHot_status_white_u8 = cTT_TURN_ON;
	}
	
	Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT_WHITE,l_CoolantTempHot_status_white_u8);
	Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT,l_CoolantTempHot_status_red_u8);
	

#else

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_CoolantTempHot_status_red_u8 = cTT_TURN_OFF;
		l_CoolantTempHot_status_white_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT,l_CoolantTempHot_status_red_u8);
		Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT_WHITE,l_CoolantTempHot_status_white_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_CoolantTempHot_status_red_u8 = cTT_TURN_OFF;
		l_CoolantTempHot_status_white_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT,l_CoolantTempHot_status_red_u8);
		Rte_Call_rpCS_EngineCoolantMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT_WHITE,l_CoolantTempHot_status_white_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
#endif
}



