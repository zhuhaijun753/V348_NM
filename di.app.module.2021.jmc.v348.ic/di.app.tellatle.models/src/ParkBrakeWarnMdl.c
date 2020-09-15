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
#include "Rte_ParkBrakeWarnMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(ParkBrakeWarnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */
//#define cTIMER_1500MS   	     (uint16)1500u


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
//static uint8 TtParkBrakeWarnStatus;

//static void fttmdl_ParkBrakeWarn_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	//TtParkBrakeWarnStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_ParkBrakeWarnMd_TIMdlUpdateTtStatus_Operation(cTTParkingBrakeWarning,TtParkBrakeWarnStatus);
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
	//TtParkBrakeWarnStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_ParkBrakeWarnMd_TIMdlUpdateTtStatus_Operation(cTTParkingBrakeWarning,TtParkBrakeWarnStatus);
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
	//TtParkBrakeWarnStatus = cTT_TURN_OFF;
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
	//TtParkBrakeWarnStatus = cTT_TURN_OFF;
	//Rte_Call_rpCS_ParkBrakeWarnMd_TIMdlUpdateTtStatus_Operation(cTTParkingBrakeWarning,TtParkBrakeWarnStatus);
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
	//fttmdl_ParkBrakeWarn_process();

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
	1. GW_ESC_EBDAlarmsig (0xA0)
	2. GW_BrakeFluidLevelAlarm(0xA0)
		|	Input					|	Input						|Output     
		|	GW_ESC_EBDAlarmsig 		|	GW_ESC_EBDAlarmsig 			|Red Indicator
------------------------------------------------------------------------------
		|	0						|	0							|turn off   
		|	x						|	1							|turn on    
		|	1						|	x							|turn on
ps:GW_Brake Fluid Level alarm debounce time is 1.5s,
which means the status of signal shall not change within 1.5s 
then the status can be regarded as certain state
*/

/*********************************************************************************
		
	Function Name	  :  fttmdl_ParkBrakeWarn_process (void)
	
	Description 	  : ParkBrakeWarn Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
// static void fttmdl_ParkBrakeWarn_process(void)
// {
	// static Boolean TimerFlag = FALSE;
	// Boolean Timer_status = FALSE;
	// uint8 u8MsgNR;
	// uint8 u8MsgTout;	
	// uint8 u8EBDAlarmSigValue;
	// uint8 u8BrakeFliudLevelAlarm;
	// static uint8 u8CertainBrakeFliudLevelAlarm = 0;

	// Rte_Read_rpSR_ParkBrakeWarnMd_CANMSG_ESC2_ComIn_NR(&u8MsgNR);
	// Rte_Read_rpSR_ParkBrakeWarnMd_CANMSG_ESC2_ComIn_Tout(&u8MsgTout);
	// Rte_Read_rpSR_ParkBrakeWarnMd_CANMSG_ESC2_ComIn_EBDAlarmSig(&u8EBDAlarmSigValue);
	// Rte_Read_rpSR_ParkBrakeWarnMd_CANMSG_ESC2_ComIn_Brake_fluidleveAlarm(&u8BrakeFliudLevelAlarm);

	// if ((RTE_E_TIMEOUT == u8MsgTout)||(RTE_E_NEVER_RECEIVED == u8MsgNR))
	// {
		// TtParkBrakeWarnStatus = cTT_TURN_ON;

	// }
	// else
	// {
		// /*GW_Brake Fluid Level alarm debounce time is 1.5s, 
		// which means the status of signal shall not change within 1.5s 
		// then the status can be regarded as certain state.*/
		// if(u8BrakeFliudLevelAlarm != u8CertainBrakeFliudLevelAlarm)
		// {
			// if(FALSE == TimerFlag)
			// {
				// Rte_Call_rptimer_TmExt_Timer_Start(eParkBrakeWarnTimer);
				// TimerFlag = TRUE;
			// }
			// else
			// {
				// Rte_Call_rptimer_TmExt_Timer_IsElapsed(eParkBrakeWarnTimer,cTIMER_1500MS,&Timer_status);
				// if(FALSE != Timer_status)
				// {
					// Rte_Call_rptimer_TmExt_Timer_Stop(eParkBrakeWarnTimer);
					// TimerFlag = FALSE;
					// Timer_status = FALSE;
					// u8CertainBrakeFliudLevelAlarm = u8BrakeFliudLevelAlarm;
				// }
			// }

		// }	
		// else
		// {	
			// if(FALSE != TimerFlag)
			// {
				// Rte_Call_rptimer_TmExt_Timer_Stop(eParkBrakeWarnTimer);
				// TimerFlag = FALSE;
			// }
		// }

		// /*Use BrakeFliudLevelAlarm certain state*/
		// if((1 == u8EBDAlarmSigValue) || (1 == u8CertainBrakeFliudLevelAlarm) )
		// {
			// TtParkBrakeWarnStatus = cTT_TURN_ON;
		// }
		// else
		// {
			// TtParkBrakeWarnStatus = cTT_TURN_OFF;
		// }
	// }
	
	// //Rte_Call_rpCS_ParkBrakeWarnMd_TIMdlUpdateTtStatus_Operation(cTTParkingBrakeWarning,TtParkBrakeWarnStatus);

// }


