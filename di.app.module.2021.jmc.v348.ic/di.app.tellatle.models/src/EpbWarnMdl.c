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
#include "Rte_EpbWarnMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"
#include "TmExt_Cfg.h"
#include "TmExt_Timer.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(EpbWarnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define NO_ERROR_EPB ((uint8)1)
#define ERROR_EPB ((uint8)2)
#define DIAG_EPB ((uint8)3)
/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 l_EPB_status_u8;
static uint8 l_last_EPB_status_u8;

static void fttmdl_N356EPB_process(void);
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_EPB_status_u8 = cTT_TURN_OFF;
	l_last_EPB_status_u8 = cTT_TURN_OFF;
	//Rte_Call_rpCS_EpbWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPB,l_EPB_status_u8);
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
	l_EPB_status_u8 = cTT_TURN_OFF;
	l_last_EPB_status_u8 = cTT_TURN_OFF;
	//Rte_Call_rpCS_EpbWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPB,l_EPB_status_u8);
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
	l_EPB_status_u8 = cTT_TURN_OFF;
	l_last_EPB_status_u8 = cTT_TURN_OFF;
	//Rte_Call_rpCS_EpbWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPB,l_EPB_status_u8);
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
	l_EPB_status_u8 = cTT_TURN_OFF;
	l_last_EPB_status_u8 = cTT_TURN_OFF;
	//Rte_Call_rpCS_EpbWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPB,l_EPB_status_u8);
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
	//fttmdl_N356EPB_process();
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
--EpbWarn indicator--
input signal =>
CAN signal (0x140):
	1. GW_EPB_EPBSystemStIndicationReq

		|	Input									|Output
		|	GW_EPB_EPBSystemStIndicationReq			|Indicator
-----------------------------------------------------------------
		|	0										|turn off
		|	1										|turn on
		|	2										|Flash @ 2HZ
		|	3										|Flash @ 4HZ
-----------------------------------------------------------------
ps:
*/
/*********************************************************************************
		
	Function Name	  :  fttmdl_N356EPB_process (void)
	
	Description 	  : EpbWarn Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
#if 0
static void fttmdl_N356EPB_process(void)
{
#ifndef TT_ONOFF_TEST

		uint8 u8MsgNR = 0;
		uint8 u8MsgTout = 0;
		uint8 fl_EPB_sig_value_U8;
//		uint8 	fl_ign_substate;
//		static uint8 l_ign_substate = eIGN_OFF;

//		static Boolean TimerFlag = FALSE;
//		Boolean TimerElapsedStatus = FALSE;
//		Boolean Timer_status = FALSE;
		
		Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(&u8MsgNR);
		Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(&u8MsgTout);
		Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus(&fl_EPB_sig_value_U8);
	
//		Rte_Read_rpIgnState_IGNState(&fl_ign_substate);//获取IG ON 或OFF状态
	
		if(RTE_E_TIMEOUT == u8MsgTout)
		{
			l_EPB_status_u8 = cTT_TURN_ON;
		}
		else if(RTE_E_NEVER_RECEIVED == u8MsgNR)
		{
			l_EPB_status_u8 = cTT_TURN_OFF;
		}
		else
		{
			if(ERROR_EPB == fl_EPB_sig_value_U8)
			{
				l_EPB_status_u8 = cTT_TURN_ON;
				l_last_EPB_status_u8 = l_EPB_status_u8;
			}
			else if(DIAG_EPB == fl_EPB_sig_value_U8)
			{
				l_EPB_status_u8 = cTT_FLASH_2HZ;
				l_last_EPB_status_u8 = l_EPB_status_u8;
			}
			else if(NO_ERROR_EPB == fl_EPB_sig_value_U8)
			{
				l_EPB_status_u8 = cTT_TURN_OFF;
				l_last_EPB_status_u8 = l_EPB_status_u8;
			}
			else
			{
				//keep last status
				l_EPB_status_u8 = l_last_EPB_status_u8;
			}
			
		}	

		/*SysRS_05_Telltales_2089:
		Once IGN OFF, the function is workable with in 10s, during the period, cluster shall keep detect input message 
		missing behavior.*/
//		if( fl_ign_substate == eIGN_OFF)
//		{
//			if( l_ign_substate != IGN_OFF )
//			{
//				Rte_Call_rptimer_TmExt_Timer_Start(eEPBWarnTimer);
//			}
//			Rte_Call_rptimer_TmExt_Timer_IsElapsed(eEPBWarnTimer,TT_TURNOFF_TIME_10S,&Timer_status);
//			if(((uint8)1u) ==Timer_status)
//			{
//				Rte_Call_rptimer_TmExt_Timer_Stop(eEPBWarnTimer);
//				l_EPB_status_u8 = cTT_TURN_OFF;
//			}
//		}
//		
//		l_ign_substate = fl_ign_substate;
	
		//Rte_Call_rpCS_EpbWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPB,l_EPB_status_u8);//
		
#else
		
			static unsigned int u32TimeCnt = 0;
			if(u32TimeCnt < 50)
			{
				l_EPB_status_u8 = cTT_TURN_ON;
				//Rte_Call_rpCS_EpbWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPB,l_EPB_status_u8);
		
			}
			else if(u32TimeCnt < 100)
			{
				l_EPB_status_u8 = cTT_TURN_OFF;
				//Rte_Call_rpCS_EpbWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPB,l_EPB_status_u8);
			}
			else
			{
				u32TimeCnt = 0;
			}
			u32TimeCnt++;
#endif

}
#endif

