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
#include "Rte_LowFuelMdl.h"
#include "WarningCtrl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(LowFuelMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 l_lfuel_status_yellow_u8 	  = cTT_TURN_OFF;
static uint8 l_lfuel_status_white_u8 	  = cTT_TURN_OFF;

static uint8 LowFuelWarningStatus = UI_WARNING_STATUS_OFF;



static void fttmdl_N356LowFuel_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_lfuel_status_yellow_u8	 = cTT_TURN_OFF;
	LowFuelWarningStatus = UI_WARNING_STATUS_OFF;
	l_lfuel_status_white_u8 	 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_FUEL_WARNING_ID,LowFuelWarningStatus);
	Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL,l_lfuel_status_yellow_u8);
	Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL_WHITE,l_lfuel_status_white_u8);
	Rte_Write_ppSR_TILowFule_tt_state_value(l_lfuel_status_yellow_u8);
	
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
  l_lfuel_status_yellow_u8	  = cTT_TURN_OFF;
  LowFuelWarningStatus = UI_WARNING_STATUS_OFF;
  l_lfuel_status_white_u8 	  = cTT_TURN_OFF;
  
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_FUEL_WARNING_ID,LowFuelWarningStatus);
  Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL,l_lfuel_status_yellow_u8);
  Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL_WHITE,l_lfuel_status_white_u8);
  Rte_Write_ppSR_TILowFule_tt_state_value(l_lfuel_status_yellow_u8);
  
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
	l_lfuel_status_yellow_u8	 = cTT_TURN_OFF;
	LowFuelWarningStatus = UI_WARNING_STATUS_OFF;
	l_lfuel_status_white_u8 	 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_FUEL_WARNING_ID,LowFuelWarningStatus);
	Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL,l_lfuel_status_yellow_u8);
	Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL_WHITE,l_lfuel_status_white_u8);
	Rte_Write_ppSR_TILowFule_tt_state_value(l_lfuel_status_yellow_u8);
	
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
	l_lfuel_status_yellow_u8	 = cTT_TURN_OFF;
	LowFuelWarningStatus = UI_WARNING_STATUS_OFF;
	l_lfuel_status_white_u8 	 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_FUEL_WARNING_ID,LowFuelWarningStatus);
	Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL,l_lfuel_status_yellow_u8);
	Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL_WHITE,l_lfuel_status_white_u8);
	Rte_Write_ppSR_TILowFule_tt_state_value(l_lfuel_status_yellow_u8);
	
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
	fttmdl_N356LowFuel_process();

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
--TurnLeft indicator--
input signal =>
CAN signal (0x23A):
	1. BCM_LeftligthSt

		|	Input					|Output
		|	BCM_LeftligthSt			|Indicator
------------------------------------------------
		|	0						|turn off
		|	1						|turn on
------------------------------------------------
ps:
1.Following message shall be sent to CAN when corresponding network awaken
*/

/*********************************************************************************
		
	Function Name	  :  fttmdl_LowFuel_process (void)
	
	Description 	  : Low Oil Fuel Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
static void fttmdl_N356LowFuel_process(void)
{
//#ifndef TT_ONOFF_TEST
	
	Boolean fl_low_fuel_status_BOOL 			  = FALSE;
	Boolean fl_FuelShortCircuitTimout_status_BOOL = FALSE;
	Boolean fl_FuelOpenCircuitTimout_status_BOOL  = FALSE;
	
	uint8 fl_ign_substate = eIGN_OFF;
		
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);

	if( eIGN_RUN == fl_ign_substate )
	{
		Rte_Read_rpSR_FuelShortCircuitTimeout_STATUS(&fl_FuelShortCircuitTimout_status_BOOL);
		Rte_Read_rpSR_FuelOpenCircuitTimeout_STATUS(&fl_FuelOpenCircuitTimout_status_BOOL);
		/*TURE is off == if （<3||>300）&& >60s*/
		
		
		if(TRUE == fl_FuelShortCircuitTimout_status_BOOL)
		{
			/*Check short circut timeout*/		
			l_lfuel_status_yellow_u8 	 = cTT_TURN_OFF;
			l_lfuel_status_white_u8 	 = cTT_TURN_ON;
			LowFuelWarningStatus = UI_WARNING_STATUS_OFF;
		}
		else if(TRUE == fl_FuelOpenCircuitTimout_status_BOOL)
		{
			/*Check open circut timeout*/
			l_lfuel_status_yellow_u8	 = cTT_TURN_OFF;
			l_lfuel_status_white_u8 	 = cTT_TURN_ON;
			LowFuelWarningStatus = UI_WARNING_STATUS_OFF;
		}
		else
		{
			Rte_Read_rpSR_LowFuel_STATUS(&fl_low_fuel_status_BOOL);

			
			if (FALSE != fl_low_fuel_status_BOOL)
			{
				l_lfuel_status_yellow_u8	 = cTT_TURN_ON;
				l_lfuel_status_white_u8 	 = cTT_TURN_OFF;
				LowFuelWarningStatus = UI_WARNING_STATUS_ON;
			}
			else
			{
				l_lfuel_status_yellow_u8	 = cTT_TURN_OFF;
				l_lfuel_status_white_u8 	 = cTT_TURN_ON;
				LowFuelWarningStatus = UI_WARNING_STATUS_OFF;
			}
		}			
	}
	else
	{
		l_lfuel_status_yellow_u8	 = cTT_TURN_OFF;
		l_lfuel_status_white_u8 	 = cTT_TURN_OFF;
		LowFuelWarningStatus = UI_WARNING_STATUS_OFF;
	}
	
	Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL,l_lfuel_status_yellow_u8);
	Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL_WHITE,l_lfuel_status_white_u8);
	Rte_Write_ppSR_TILowFule_tt_state_value(l_lfuel_status_yellow_u8);
	Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_FUEL_WARNING_ID,LowFuelWarningStatus);


#if 0

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_lfuel_status_yellow_u8 = cTT_TURN_ON;
		Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL,l_lfuel_status_yellow_u8);
	}
	else if(u32TimeCnt < 100)
	{
		l_lfuel_status_yellow_u8 = cTT_TURN_OFF;
		Rte_Call_rpiCS_LowFuelMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL,l_lfuel_status_yellow_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
	
#endif
}


