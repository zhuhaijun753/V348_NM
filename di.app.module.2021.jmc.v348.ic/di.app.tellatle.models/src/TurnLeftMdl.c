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
#include "Rte_TurnLeftMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(TurnLeftMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
//static uint8 TtLeftStatus;

//static void fttmdl_TurnLeft_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	//TtLeftStatus = cTT_TURN_OFF;
	//Rte_Call_rpiCS_rpIgnState_TIMdlUpdateTtStatus_Operation(cTTTURNLEFT,TtLeftStatus);
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
	//TtLeftStatus = cTT_TURN_OFF;
	//Rte_Call_rpiCS_rpIgnState_TIMdlUpdateTtStatus_Operation(cTTTURNLEFT,TtLeftStatus);
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
	//TtLeftStatus = cTT_TURN_OFF;
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
	//TtLeftStatus = cTT_TURN_OFF;
	//Rte_Call_rpiCS_rpIgnState_TIMdlUpdateTtStatus_Operation(cTTTURNLEFT,TtLeftStatus);
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
	//fttmdl_TurnLeft_process();

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
1.The Turn Right Indicator and Turn Left Indicator shall not be displayed in Power Down Transition  mode 
2.The Turn Right Indicator and Turn Left Indicator shall not be displayed in Sleep mode
3.key pos ON/OFF
*/
/*********************************************************************************
		
	Function Name	  :  fttmdl_TurnLeft_process (void)
	
	Description 	  : TurnLeft Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
// static void fttmdl_TurnLeft_process(void)
// {
	// uint8 u8MsgNR;
	// uint8 u8MsgTout;
	// uint8 u8TureLeftSt;
	
	// u8MsgNR = 0;
	// u8MsgTout = 0;
	// u8TureLeftSt = 0;
		
	// Rte_Read_rpSR_CANMSG_IBCM2_ComIn_NR(&u8MsgNR);
	// Rte_Read_rpSR_CANMSG_IBCM2_ComIn_Tout(&u8MsgTout);
	// Rte_Read_rpSR_CANMSG_IBCM2_ComIn_BCM_LetfligthSt(&u8TureLeftSt);
	
	// if (RTE_E_TIMEOUT == u8MsgTout )
	// {
		// TtLeftStatus = cTT_TURN_OFF;
	// }
	// else if (RTE_E_NEVER_RECEIVED == u8MsgNR)
	// {
		// TtLeftStatus = cTT_TURN_OFF;
	// }
	// else
	// {
		// if(1 == u8TureLeftSt)
		// {
			// TtLeftStatus = cTT_TURN_ON;
		// }
		// else
		// {
			// TtLeftStatus = cTT_TURN_OFF;
		// }
	// }
	
	// //Rte_Call_rpiCS_rpIgnState_TIMdlUpdateTtStatus_Operation(cTTTURNLEFT,TtLeftStatus);

// }


