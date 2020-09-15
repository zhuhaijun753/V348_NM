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
#include "Rte_GlowTtMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(GlowTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8 l_glow_status_u8 = cTT_TURN_OFF;

#define TURN_ON_GLOW	((uint8)1)
//#define TURN_OFF_GLOW	((uint8)0)
//#define TURN_OFF_GLOW_3	((uint8)3)
//#define KEEP_LAST_GLOW	((Uint8)2)
/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
//static uint8 l_last_glow_status_u8;
static void fttmdl_N356glow_process(void);
//static void fttmdl_Hdc_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_glow_status_u8 = cTT_TURN_OFF;
	//l_last_glow_status_u8 = cTT_TURN_OFF;
	Rte_Call_rpCS_GlowTtMdl_TIMdlUpdateTtStatus_Operation(cTTGLOW,l_glow_status_u8);
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
	l_glow_status_u8 = cTT_TURN_OFF;
	//l_last_glow_status_u8 = cTT_TURN_OFF;
	Rte_Call_rpCS_GlowTtMdl_TIMdlUpdateTtStatus_Operation(cTTGLOW,l_glow_status_u8);
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
	l_glow_status_u8 = cTT_TURN_OFF;
	//l_last_glow_status_u8 = cTT_TURN_OFF;
	Rte_Call_rpCS_GlowTtMdl_TIMdlUpdateTtStatus_Operation(cTTGLOW,l_glow_status_u8);
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
	l_glow_status_u8 = cTT_TURN_OFF;
	//l_last_glow_status_u8 = cTT_TURN_OFF;
	Rte_Call_rpCS_GlowTtMdl_TIMdlUpdateTtStatus_Operation(cTTGLOW,l_glow_status_u8);
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

	boolean IsEngineCfg = FALSE;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Engine,&IsEngineCfg);
	
	if(FALSE == IsEngineCfg) //Diesel
	{
		//fttmdl_N356glow_process();
	}
	else
	{
		//NULL	Gasoline
	}
	
	fttmdl_N356glow_process();
	
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
--Hdct indicator--
input signal =>
CAN signal (0x140):
	1. ESC_HDC_DisplayMessageReq

		|	Input								|Output
		|	ESC_HDC_DisplayMessageReq			|Indicator
---------------------------------------------------------
		|	0									|turn off
		|	1									|turn on
---------------------------------------------------------
ps:
*/
/*********************************************************************************
		
	Function Name	  :  fttmdl_Hdct_process (void)
	
	Description 	  : Hdct Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
static void fttmdl_N356glow_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;
	uint8 fl_glow_sig_value_U8;
	
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_DCM_GlowPlugLmpReq(&fl_glow_sig_value_U8);
	
	if ( (RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR) )
	{
		l_glow_status_u8 = cTT_TURN_OFF;
	}
	else
	{
		if(TURN_ON_GLOW == fl_glow_sig_value_U8)
		{
			l_glow_status_u8 = cTT_TURN_ON;
		}
		else
		{
			l_glow_status_u8 = cTT_TURN_OFF;

		}

	}	
	Rte_Call_rpCS_GlowTtMdl_TIMdlUpdateTtStatus_Operation(cTTGLOW,l_glow_status_u8);
#else
	
		static unsigned int u32TimeCnt = 0;
		if(u32TimeCnt < 50)
		{
			l_glow_status_u8 = cTT_TURN_ON;
			Rte_Call_rpCS_GlowTtMdl_TIMdlUpdateTtStatus_Operation(cTTGLOW,l_glow_status_u8);
	
		}
		else if(u32TimeCnt < 100)
		{
			l_glow_status_u8 = cTT_TURN_OFF;
			Rte_Call_rpCS_GlowTtMdl_TIMdlUpdateTtStatus_Operation(cTTGLOW,l_glow_status_u8);
		}
		else
		{
			u32TimeCnt = 0;
		}
		u32TimeCnt++;
#endif

}

