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
#include "Rte_BackFogMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(BackFogMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

#define REARFOG_SIG_OFF ((uint8)1)
#define REARFOG_SIG_ON ((uint8)2)

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static uint8 l_rfog_status_u8 = cTT_TURN_OFF;

static void fttmdl_N356rfog_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_rfog_status_u8 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_BackFogMdl_TIMdlUpdateTtStatus_Operation(cTTREAR_FOG,l_rfog_status_u8);
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
	l_rfog_status_u8 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_BackFogMdl_TIMdlUpdateTtStatus_Operation(cTTREAR_FOG,l_rfog_status_u8);	
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
	l_rfog_status_u8 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_BackFogMdl_TIMdlUpdateTtStatus_Operation(cTTREAR_FOG,l_rfog_status_u8);
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
	l_rfog_status_u8 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_BackFogMdl_TIMdlUpdateTtStatus_Operation(cTTREAR_FOG,l_rfog_status_u8);
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
	fttmdl_N356rfog_process();
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
		
	Function Name	  :  fttmdl_N356rfog_process (void)
	
	Description 	  : FrontFog Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
extern uint8 Com_ReceiveSignal(uint16 SignalId, void * SignalDataPtr);
static void fttmdl_N356rfog_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;
	uint8 fl_rfog_sig_value_U8 = cTT_TURN_OFF;
	
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpSts(&fl_rfog_sig_value_U8);
	
	if ( (RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR) )
	{
		l_rfog_status_u8 = cTT_TURN_OFF;
	}
	else
	{
		if(REARFOG_SIG_ON == fl_rfog_sig_value_U8)
		{
			l_rfog_status_u8 = cTT_TURN_ON;
		}
		else
		{
			l_rfog_status_u8 = cTT_TURN_OFF;
		}
	}
	
	Rte_Call_rpCS_BackFogMdl_TIMdlUpdateTtStatus_Operation(cTTREAR_FOG,l_rfog_status_u8);

#else

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_rfog_status_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_BackFogMdl_TIMdlUpdateTtStatus_Operation(cTTREAR_FOG,l_rfog_status_u8);
	}
	else if(u32TimeCnt < 100)
	{
		l_rfog_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_BackFogMdl_TIMdlUpdateTtStatus_Operation(cTTREAR_FOG,l_rfog_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
	
#endif
}


