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
#include "Rte_ABSTtMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(ABSTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8 l_abs_status_u8 = cTT_TURN_OFF; 
#define TURN_OFF_ABSTT ((uint8)0)
#define TURN_ON_ABSTT  ((uint8)1)

#define FUNCTION_ABS_TIME_30MS                (30)           
#define FUNCTION_ABS_TIME_6500MS              (6500 / FUNCTION_ABS_TIME_30MS)       
/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static boolean  l_ABS_Flag = TRUE;
static uint8    l_Proveout_Counter_U8 = FALSE;

static void fttmdl_N356ABS_process(void);
static boolean f_ABS_PowerOn_Self_7s_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_abs_status_u8 = cTT_TURN_OFF;
	l_ABS_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_rpCS_ABSTtMdl_TIMdlUpdateTtStatus_Operation(cTTABS_SYSTEM_FAILURE,l_abs_status_u8);
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
	l_abs_status_u8 = cTT_TURN_OFF;
	l_ABS_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_rpCS_ABSTtMdl_TIMdlUpdateTtStatus_Operation(cTTABS_SYSTEM_FAILURE,l_abs_status_u8);
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
	l_abs_status_u8 = cTT_TURN_OFF;
	l_ABS_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_rpCS_ABSTtMdl_TIMdlUpdateTtStatus_Operation(cTTABS_SYSTEM_FAILURE,l_abs_status_u8);
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
	l_abs_status_u8 = cTT_TURN_OFF;
	l_ABS_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_rpCS_ABSTtMdl_TIMdlUpdateTtStatus_Operation(cTTABS_SYSTEM_FAILURE,l_abs_status_u8);
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
	uint8 IsEngineCfg_Abs = FALSE;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ABS,&IsEngineCfg_Abs);

	if(TRUE == IsEngineCfg_Abs)
	{
		fttmdl_N356ABS_process();
	}
	else
	{
		Rte_Call_rpCS_ABSTtMdl_TIMdlUpdateTtStatus_Operation(cTTABS_SYSTEM_FAILURE,FALSE);
	}

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

static void fttmdl_N356ABS_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;

	uint8 fl_abs_states   = TURN_OFF_ABSTT;
	uint8 fl_ign_substate = eIGN_OFF;
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_AbsFlgFlt(&fl_abs_states);


	if( eIGN_RUN == fl_ign_substate )
	{
		if(RTE_E_TIMEOUT == u8MsgTout)
		{
			l_abs_status_u8 = cTT_TURN_ON;
		}
		else if (RTE_E_NEVER_RECEIVED == u8MsgNR)
		{
			f_ABS_PowerOn_Self_7s_Action();
			
			if(FALSE == l_ABS_Flag)
			{
				l_abs_status_u8 = cTT_TURN_ON;
			}
			else
			{
				l_abs_status_u8 = cTT_TURN_OFF;
			}
		}
		else
		{
			if(TURN_ON_ABSTT == fl_abs_states)
			{
				l_abs_status_u8 = cTT_TURN_ON;
			}
			else
			{
				l_abs_status_u8 = cTT_TURN_OFF;
			}
		}
	}
	else
	{
		l_abs_status_u8 = cTT_TURN_OFF;
	
		l_ABS_Flag = TRUE;
		l_Proveout_Counter_U8 = 0;
	}
	
	Rte_Call_rpCS_ABSTtMdl_TIMdlUpdateTtStatus_Operation(cTTABS_SYSTEM_FAILURE,l_abs_status_u8);
	
#else

	static unsigned int u32TimeCnt = 0;
	u32TimeCnt++;
	if(u32TimeCnt < 50)
	{
		l_abs_status_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_ABSTtMdl_TIMdlUpdateTtStatus_Operation(cTTABS_SYSTEM_FAILURE,l_abs_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_abs_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_ABSTtMdl_TIMdlUpdateTtStatus_Operation(cTTABS_SYSTEM_FAILURE,l_abs_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	
#endif
}

static boolean f_ABS_PowerOn_Self_7s_Action(void)
{
    if(l_Proveout_Counter_U8 >= FUNCTION_ABS_TIME_6500MS)
    {
        l_ABS_Flag = FALSE;
    }
    else
    {
        l_ABS_Flag = TRUE;
		l_Proveout_Counter_U8++;
    }
	
    return l_ABS_Flag;
}

