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

#include "Rte_ModMdl.h"
#include "CmpLib.h"

boolean dem_IGN_ON_5s = FALSE;
static void Modmdl_dem_process(eIgnState Status);

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(ModMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------

static Std_ReturnType CmpInit( void )
{
	dem_IGN_ON_5s = FALSE;
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
	 dem_IGN_ON_5s = FALSE;
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
	 dem_IGN_ON_5s = FALSE;
	(void)Rte_Write_ppIgnState_IGNState(eIGN_OFF);/*Lhbb*/
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
	 dem_IGN_ON_5s = FALSE;
	(void)Rte_Write_ppIgnState_IGNState(eIGN_OFF);/*Lhbb*/
	return  E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	boolean pinstatus;
	eIgnState IgnSts;

	pinstatus = FALSE;

	(void)Rte_Call_rpIoHwAbGet_Impl_GetPin(eIOPinInId_MDI_IGN_INPUT, &pinstatus);

	if(TRUE == pinstatus)
    {
    	IgnSts = eIGN_RUN;
    }
    else
    {
    	IgnSts = eIGN_OFF;
    }

	Modmdl_dem_process(IgnSts);
   	(void)Rte_Write_ppIgnState_IGNState(IgnSts); /*Lhbb*/

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

static void Modmdl_dem_process(eIgnState Status)
{
	boolean fl_timerStarted = FALSE;
	boolean fl_timerExp = FALSE;

    if (Status == eIGN_RUN)
     {
        if (dem_IGN_ON_5s == FALSE) /*Lhbb if not true,start timer*/
        {
	   	    Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerModMdlDemWait,&fl_timerStarted);
			if (fl_timerStarted == FALSE)
			{
			 Rte_Call_rptimer_TmExt_Timer_Start(eTimerModMdlDemWait);
			}

			Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerModMdlDemWait,3000,&fl_timerExp);
			if (fl_timerExp == TRUE)
			{
			  dem_IGN_ON_5s = TRUE;
			  Rte_Call_rptimer_TmExt_Timer_Stop(eTimerModMdlDemWait); 
			}
        }
     }
	 else
     {
		dem_IGN_ON_5s = FALSE;
		Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerModMdlDemWait,&fl_timerStarted);/*ign shake in 5s*/
		if (fl_timerStarted == TRUE)
		{
			Rte_Call_rptimer_TmExt_Timer_Stop(eTimerModMdlDemWait);
		}
     }
	 Rte_Write_tiSR_IgnState_Dem_state(dem_IGN_ON_5s);
}

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================
    
