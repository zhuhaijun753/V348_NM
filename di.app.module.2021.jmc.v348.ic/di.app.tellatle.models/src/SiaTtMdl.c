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
#include "Rte_SiaTtMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(SiaTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
//#define cTIMER_3SEC  (uint16)3000u


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
//static uint8 TtSiaStatus;
//static void fttmdl_SiaTt_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	//TtSiaStatus = cTT_TURN_OFF;
	//Rte_Call_rpiCS_TurnRightMdl_TIMdlUpdateTtStatus_Operation(cTTTURNRIGHT,TtSiaStatus);
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
	//TtSiaStatus = cTT_TURN_OFF;
	//Rte_Call_rpiCS_TurnRightMdl_TIMdlUpdateTtStatus_Operation(cTTTURNRIGHT,TtSiaStatus);
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
	//TtSiaStatus = cTT_TURN_OFF;
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
	//TtSiaStatus = cTT_TURN_OFF;
	//Rte_Call_rpiCS_TurnRightMdl_TIMdlUpdateTtStatus_Operation(cTTTURNRIGHT,TtSiaStatus);
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
	//fttmdl_SiaTt_process();

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

/*********************************************************************************
		
	Function Name	  :  fttmdl_SiaTt_process (void)
	
	Description 	  : Sia Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
// static void fttmdl_SiaTt_process(void)
// {
	// uint8 SiaTTStSigVal = 0;
	// boolean AnimationRunning = FALSE;	
	// static boolean TimerRunningFlag = FALSE;
	// static boolean TimerElapsedStatus = FALSE;
	// boolean Timer_status = FALSE;
	
	// Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&AnimationRunning);
	// Rte_Read_tiSR_Sia_to_TT_st(&SiaTTStSigVal);
	// if(FALSE != AnimationRunning)
	// {
		// TimerRunningFlag = FALSE;
		// TimerElapsedStatus = FALSE;
		// Timer_status = FALSE;
		// Rte_Call_rptimer_TmExt_Timer_Stop(eSiaTTTimer);
		// TtSiaStatus = cTT_TURN_OFF;
	// }
	// else
	// {
		// if(1 == SiaTTStSigVal)
		// {
			// if(TimerElapsedStatus == FALSE)/* 3s is not elapsed */
			// {
				// if(FALSE != TimerRunningFlag) /*if start timer*/
				// {
					// Rte_Call_rptimer_TmExt_Timer_IsElapsed(eSiaTTTimer,cTIMER_3SEC,&Timer_status);
					// if( FALSE != Timer_status)
					// {
						// /*3s is elapsed */
						// Rte_Call_rptimer_TmExt_Timer_Stop(eSiaTTTimer);
						// TimerElapsedStatus = TRUE;
						// TtSiaStatus = cTT_TURN_ON;
					// }
					// else
					// {
						// /*3s is not elapsed */
						// TimerElapsedStatus = FALSE;
						// TtSiaStatus = cTT_FLASH_1HZ;
					// }
				// }
				// else /*if not start timer,start timer*/
				// {
					// TimerRunningFlag = TRUE;
					// TimerElapsedStatus = FALSE;
					// Timer_status = FALSE;
					// Rte_Call_rptimer_TmExt_Timer_Start(eSiaTTTimer);
					// TtSiaStatus = cTT_FLASH_1HZ;
				// }
			// }
			// else/* 3s is elapsed */
			// {
				// TtSiaStatus = cTT_TURN_ON;
			// }
		// }
		// else
		// {
			// TimerRunningFlag = FALSE;
			// TimerElapsedStatus = FALSE;
			// Timer_status = FALSE;
			// Rte_Call_rptimer_TmExt_Timer_Stop(eSiaTTTimer);
			// TtSiaStatus = cTT_TURN_OFF;
		// }
	// }
	
	// //Rte_Call_rpCS_TIMdlUpdateTtStatus_Operation(cTTSIA,TtSiaStatus);

// }


