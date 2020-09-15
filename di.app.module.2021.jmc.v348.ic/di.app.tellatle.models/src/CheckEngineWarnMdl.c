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
#include "Rte_CheckEngineWarnMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(CheckEngineWarnMdl)

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
//static uint8 TtCheckEngineWarnStatus;

//static void fttmdl_CheckEngineWarn_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	//TtCheckEngineWarnStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_CheckEngineWarnMdl_TIMdlUpdateTtStatus_Operation(cTTCHK_ENG,TtCheckEngineWarnStatus);
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
	//TtCheckEngineWarnStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_CheckEngineWarnMdl_TIMdlUpdateTtStatus_Operation(cTTCHK_ENG,TtCheckEngineWarnStatus);
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
	//TtCheckEngineWarnStatus = cTT_TURN_OFF;
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
	//TtCheckEngineWarnStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_CheckEngineWarnMdl_TIMdlUpdateTtStatus_Operation(cTTCHK_ENG,TtCheckEngineWarnStatus);
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
	//fttmdl_CheckEngineWarn_process();

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
--CheckEngineWarn indicator--
input signal =>
CAN signal (0x0E0&0x93):
	1. GW_EMS_EngMILTelltaleSts(0x0E0)
	2. TCU_TransmissionMILReq(0x93)
		|	Input					|	Input					|Output
		|	GW_EMS_EngMILTelltaleSts|	TCU_TransmissionMILReq	|Indicator
----------------------------------------------------------------------------
		|	0,3						|	0						|turn off
		|	0,3						|	1						|turn on
		|	1						|	x						|turn on
		|	2						|	x						|Flash @ 1HZ
----------------------------------------------------------------------------
ps:
*/
/*********************************************************************************
		
	Function Name	  :  fttmdl_CheckEngineWarn_process (void)
	
	Description 	  : CheckEngineWarn Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
// static void fttmdl_CheckEngineWarn_process(void)
// {

	// uint8 u8MsgEmsNR = 0;
	// uint8 u8MsgEmsTout = 0;	
	// uint8 u8MsgTcu1NR = 0;
	// uint8 u8MsgTcu1Tout = 0;	
	// uint8 u8MilTelltaleSt = 0;
	// uint8 u8MilReq = 0;

	// Rte_Read_rpSR_CANMSG_GWS5_EMS_ComIn_NR(&u8MsgEmsNR);
	// Rte_Read_rpSR_CANMSG_GWS5_EMS_ComIn_Tout(&u8MsgEmsTout);
	// Rte_Read_rpSR_CANMSG_GW_TCU1_ComIn_NR(&u8MsgTcu1NR);
	// Rte_Read_rpSR_CANMSG_GW_TCU1_ComIn_Tout(&u8MsgTcu1Tout);
	// Rte_Read_rpSR_CANMSG_GWS5_EMS_ComIn_EngMILTelltaleSts(&u8MilTelltaleSt);
	// Rte_Read_rpSR_CANMSG_GW_TCU1_ComIn_TCU_TransmissionMILReq(&u8MilReq);

	// if(((RTE_E_TIMEOUT == u8MsgEmsTout)||(RTE_E_NEVER_RECEIVED == u8MsgEmsNR))&&\
		// ((RTE_E_TIMEOUT == u8MsgTcu1Tout)||(RTE_E_NEVER_RECEIVED == u8MsgTcu1NR)))
	// {
		// TtCheckEngineWarnStatus = cTT_TURN_OFF;
	// }
	// else
	// {
		// if ((RTE_E_TIMEOUT == u8MsgEmsTout)||(RTE_E_NEVER_RECEIVED == u8MsgEmsNR))
		// {
			// u8MilTelltaleSt = 0;// Default/Degrd Value
		// }
		
		// if ((RTE_E_TIMEOUT == u8MsgTcu1Tout)||(RTE_E_NEVER_RECEIVED == u8MsgTcu1NR))
		// {
			// u8MilReq = 0;// Default/Degrd Value
		// }

		// if((0 == u8MilTelltaleSt) || (3 == u8MilTelltaleSt))
		// {
			// if(1 == u8MilReq)
			// {
				// TtCheckEngineWarnStatus = cTT_TURN_ON;
			// }
			// else
			// {
				// TtCheckEngineWarnStatus = cTT_TURN_OFF;
			// }
		// }
		// else if (1 == u8MilTelltaleSt)
		// {
			// TtCheckEngineWarnStatus = cTT_TURN_ON;
		// }
		// else if(2 == u8MilTelltaleSt)
		// {
			// TtCheckEngineWarnStatus = cTT_FLASH_1HZ;
		// }
		// else
		// {
			// TtCheckEngineWarnStatus = cTT_TURN_OFF;
		// }
	// }
	// //Rte_Call_rpCS_CheckEngineWarnMdl_TIMdlUpdateTtStatus_Operation(cTTCHK_ENG,TtCheckEngineWarnStatus);

// }


