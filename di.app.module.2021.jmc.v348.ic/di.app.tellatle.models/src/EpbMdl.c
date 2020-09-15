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
#include "Rte_EpbMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(EpbMdl)

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
//static uint8 TtEpbRedStatus;
//static uint8 TtEpbGreenStatus;

//static void fttmdl_EPB_process(void);
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	//TtEpbRedStatus = cTT_TURN_OFF;
	//TtEpbGreenStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_EpbMdl_TIMdlUpdateTtStatus_Operation(cTTEpbGreen,TtEpbGreenStatus);
	//Rte_Call_rpCS_EpbMdl_TIMdlUpdateTtStatus_Operation(cTTEpbRed,TtEpbRedStatus);
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
	//TtEpbRedStatus = cTT_TURN_OFF;
	//TtEpbGreenStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_EpbMdl_TIMdlUpdateTtStatus_Operation(cTTEpbGreen,TtEpbGreenStatus);
	//Rte_Call_rpCS_EpbMdl_TIMdlUpdateTtStatus_Operation(cTTEpbRed,TtEpbRedStatus);
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
	//TtEpbRedStatus = cTT_TURN_OFF;
	//TtEpbGreenStatus = cTT_TURN_OFF;
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
	//TtEpbRedStatus = cTT_TURN_OFF;
	//TtEpbGreenStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_EpbMdl_TIMdlUpdateTtStatus_Operation(cTTEpbGreen,TtEpbGreenStatus);
	//Rte_Call_rpCS_EpbMdl_TIMdlUpdateTtStatus_Operation(cTTEpbRed,TtEpbRedStatus);
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
	//fttmdl_EPB_process();

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
--EPB Warning indicator--
input signal =>
CAN signal :
	1. EPB_EPBWarningIndicationReq (0x31A)
		|	Input							|Output     
		|	EPB_EPBWarningIndicationReq 	|Red Indicator
-----------------------------------------------------------
IGN ON	|	0								|turn off   
		|	1								|turn on    
		|	2								|flash (2Hz)
		|	3								|flash (4Hz)   
-----------------------------------------------------------
IGN OFF	|	1								|turn on
		|	Not1							|turn off

*/
/*********************************************************************************
		
	Function Name	  :  fttmdl_EPB_process (void)
	
	Description 	  :  Electrical Parking Brake Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
// static void fttmdl_EPB_process(void)
// {
		// uint8 u8MsgNR = 0;
		// uint8 u8MsgTout = 0;	
		// uint8 u8EpbSystemStIndicationreq = 0;
		// eIgnState eIgnStutus = eIGN_OFF;
		
		// Rte_Read_rpSR_CANMSG_GW_PT1_ComIn_NR(&u8MsgNR);
		// Rte_Read_rpSR_CANMSG_GW_PT1_ComIn_NR(&u8MsgTout);
		// Rte_Read_rpSR_CANMSG_GW_PT1_ComIn_GW_EPB_EPBSystemStIndicationReq(&u8EpbSystemStIndicationreq);
		
		// Rte_Read_rpIgnState_IGNState(&eIgnStutus);

		// if ((RTE_E_TIMEOUT == u8MsgTout)||(RTE_E_NEVER_RECEIVED == u8MsgNR))
		// {
			// TtEpbRedStatus = cTT_TURN_OFF;
			// TtEpbGreenStatus = cTT_TURN_OFF;
		// }
		// else
		// {
			// if(eIGN_OFF != eIgnStutus)						// {
				// if(0 == u8EpbSystemStIndicationreq)
				// {
					// TtEpbRedStatus = cTT_TURN_OFF;
					// TtEpbGreenStatus = cTT_TURN_OFF;

				// }
				// else if(1 == u8EpbSystemStIndicationreq)
				// {
					// TtEpbRedStatus = cTT_TURN_ON;
					// TtEpbGreenStatus = cTT_TURN_OFF;

				// }
				// else if(2 == u8EpbSystemStIndicationreq)
				// {
					// TtEpbRedStatus = cTT_FLASH_2HZ;
					// TtEpbGreenStatus = cTT_TURN_OFF;

				// }
				// else if(3 == u8EpbSystemStIndicationreq)
				// {
					// TtEpbRedStatus = cTT_FLASH_4HZ;
					// TtEpbGreenStatus = cTT_TURN_OFF;

				// }
				// else
				// {
					// TtEpbRedStatus = cTT_TURN_OFF;
					// TtEpbGreenStatus = cTT_TURN_OFF;

				// }
			// }
			// else
			// {
				// if(1 == u8EpbSystemStIndicationreq)
				// {
					// TtEpbRedStatus = cTT_TURN_ON;
					// TtEpbGreenStatus = cTT_TURN_OFF;

				// }
				// else
				// {
					// TtEpbRedStatus = cTT_TURN_OFF;
					// TtEpbGreenStatus = cTT_TURN_OFF;
				// }

			// }
		// }
		// //Rte_Call_rpCS_EpbMdl_TIMdlUpdateTtStatus_Operation(cTTEpbGreen,TtEpbGreenStatus);
		// //Rte_Call_rpCS_EpbMdl_TIMdlUpdateTtStatus_Operation(cTTEpbRed,TtEpbRedStatus);

// }

