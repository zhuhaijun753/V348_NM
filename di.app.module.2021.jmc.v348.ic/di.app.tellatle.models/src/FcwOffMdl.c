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
#include "Rte_FcwOffMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(FcwOffMdl)

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
//static uint8 TtFcwOffStatus;

//static void fttmdl_FcwOff_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	//TtFcwOffStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_FcwOffMdlTIMdl_UpdateTtStatus_Operation(cTTFCWOFF,TtFcwOffStatus);
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
	//TtFcwOffStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_FcwOffMdlTIMdl_UpdateTtStatus_Operation(cTTFCWOFF,TtFcwOffStatus);
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
	//TtFcwOffStatus = cTT_TURN_OFF;
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
	//TtFcwOffStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_FcwOffMdlTIMdl_UpdateTtStatus_Operation(cTTFCWOFF,TtFcwOffStatus);
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
	// boolean IsCfg = FALSE;
	// Rte_Read_rptiSR_GdtCtrl_AEB_FCW_ACC_Cfg(&IsCfg);
	// if(FALSE != IsCfg)
	// {
		// fttmdl_FcwOff_process();
	// }
	// else
	// {
		// /*do nothing*/
	// }

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
--FcwOff indicator--
input signal =>
CAN signal (0x4F7):
	1. GW_AEB_Switch_Status

		|	Input					|Output
		|	GW_AEB_Switch_Status	|Indicator
------------------------------------------------
		|	0\1						|turn off
	on	|	2\3						|turn on
------------------------------------------------
	off	|	NC						|turn off
ps:
*/
/*********************************************************************************
		
	Function Name	  :  fttmdl_FcwOff_process (void)
	
	Description 	  : FcwOff Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
// static void fttmdl_FcwOff_process(void)
// {
	
		// uint8 u8MsgNR = 0;
		// uint8 u8MsgTout = 0;	
		// uint8 u8AebSwitchSt = 0;	
		
		// Rte_Read_rpSR_CANMSG_GW_RADA2_ComIn_NR(&u8MsgNR);
		// Rte_Read_rpSR_CANMSG_GW_RADA2_ComIn_Tout(&u8MsgTout);
		// Rte_Read_rpSR_CANMSG_GW_RADA2_ComIn_GW_AEB_Switch_Status(&u8AebSwitchSt);
		
		// if ((RTE_E_TIMEOUT == u8MsgTout)||(RTE_E_NEVER_RECEIVED == u8MsgNR))
		// {
			// TtFcwOffStatus = cTT_TURN_ON;
		// }
		// else
		// {
			// if((2 == u8AebSwitchSt) || (3 == u8AebSwitchSt))
			// {
				// TtFcwOffStatus = cTT_TURN_ON;
			// }
			// else
			// {
				// TtFcwOffStatus = cTT_TURN_OFF;
			// }
		// }
	
		// //Rte_Call_rpCS_FcwOffMdlTIMdl_UpdateTtStatus_Operation(cTTFCWOFF,TtFcwOffStatus);
	
// }


