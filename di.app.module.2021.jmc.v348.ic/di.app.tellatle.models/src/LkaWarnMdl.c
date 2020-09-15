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
#include "Rte_LkaWarnMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(LkaWarnMdl)

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
// static uint8 TtLkaWarnGreenStatus;
// static uint8 TtLkaWarnYellowStatus;


// static void fttmdl_LkaWarn_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	//TtLkaWarnGreenStatus = cTT_TURN_OFF;
	//TtLkaWarnYellowStatus = cTT_TURN_OFF;
	//Rte_Call_rpiCS_LkaWarnMdl_TIMdlUpdateTtStatus_Operation(cTTLKAWarningGreen,TtLkaWarnGreenStatus);
	//Rte_Call_rpiCS_LkaWarnMdl_TIMdlUpdateTtStatus_Operation(cTTLKAWarningYellow,TtLkaWarnYellowStatus);
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
	//TtLkaWarnGreenStatus = cTT_TURN_OFF;
	//TtLkaWarnYellowStatus = cTT_TURN_OFF;
	//Rte_Call_rpiCS_LkaWarnMdl_TIMdlUpdateTtStatus_Operation(cTTLKAWarningGreen,TtLkaWarnGreenStatus);
	//Rte_Call_rpiCS_LkaWarnMdl_TIMdlUpdateTtStatus_Operation(cTTLKAWarningYellow,TtLkaWarnYellowStatus);

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
	//TtLkaWarnGreenStatus = cTT_TURN_OFF;
	//TtLkaWarnYellowStatus = cTT_TURN_OFF;
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
	//TtLkaWarnGreenStatus = cTT_TURN_OFF;
	//TtLkaWarnYellowStatus = cTT_TURN_OFF;
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
	// Rte_Read_tiSR_GdtCtrl_LDW_LKA_Cfg(&IsCfg);
	// if(FALSE != IsCfg)
	// {
		// fttmdl_LkaWarn_process();
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
--LkaWarn indicator--
input signal =>
CAN signal (0x541):
	1. GW_LKA_Stauts_Display

		|	Input					|Output
		|	GW_LKA_Stauts_Display	|Indicator
------------------------------------------------
		|	0\3						|turn off
		|	1						|on with Green
		|	2						|On With Yellow
------------------------------------------------
ps:
1.The Turn Right Indicator and Turn Left Indicator shall not be displayed in Power Down Transition  mode 
2.The Turn Right Indicator and Turn Left Indicator shall not be displayed in Sleep mode
3.key pos ON/OFF
*/
/*********************************************************************************
		
	Function Name	  :  fttmdl_LkaWarn_process (void)
	
	Description 	  : LkaWarn Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
// static void fttmdl_LkaWarn_process(void)
// {
	// uint8 u8MsgNR = 0;
	// uint8 u8MsgTout = 0;	
	// uint8 u8LkaStDisplay = 0;

	// Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_NR(&u8MsgNR);
	// Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_Tout(&u8MsgTout);
	// Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_LKA_Status_Display(&u8LkaStDisplay);
	// if ((RTE_E_TIMEOUT == u8MsgTout)||(RTE_E_NEVER_RECEIVED == u8MsgNR))
	// {
		// TtLkaWarnGreenStatus = cTT_TURN_OFF;
		// TtLkaWarnYellowStatus = cTT_TURN_OFF;
	// }
	// else
	// {
		// if(1 == u8LkaStDisplay)
		// {
			// TtLkaWarnGreenStatus = cTT_TURN_ON;
			// TtLkaWarnYellowStatus = cTT_TURN_OFF;
		// }
		// else if(2 == u8LkaStDisplay)
		// {
			// TtLkaWarnGreenStatus = cTT_TURN_OFF;
			// TtLkaWarnYellowStatus = cTT_TURN_ON;
		// }
		// else
		// {
			// TtLkaWarnGreenStatus = cTT_TURN_OFF;
			// TtLkaWarnYellowStatus = cTT_TURN_OFF;
		// }
	// }
	// //Rte_Call_rpiCS_LkaWarnMdl_TIMdlUpdateTtStatus_Operation(cTTLKAWarningGreen,TtLkaWarnGreenStatus);
	// //Rte_Call_rpiCS_LkaWarnMdl_TIMdlUpdateTtStatus_Operation(cTTLKAWarningYellow,TtLkaWarnYellowStatus);

// }


