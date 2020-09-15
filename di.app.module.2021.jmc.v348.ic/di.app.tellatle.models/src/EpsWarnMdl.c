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
#include "Rte_EpsWarnMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(EpsWarnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8 l_epsred_status_u8 = cTT_TURN_OFF;
static uint8 l_epsylw_status_u8 = cTT_TURN_OFF;

#define TURN_OFF_EPS_RED  ((uint8)0)
#define TURN_ON_EPS_RED   ((uint8)1)

#define TURN_OFF_EPS_YLW  ((uint8)0)
#define TURN_ON_EPS_YLW   ((uint8)1)

#define FUNCTION_EPS_TIME_30MS                (30)           
#define FUNCTION_EPS_TIME_6500MS              (6500 / FUNCTION_EPS_TIME_30MS)     

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static boolean  l_EPS_Flag = TRUE;
static uint8    l_Proveout_Counter_U8 = FALSE;

static void fttmdl_N356eps_process(void);
static void fttmdl_EPSWARN_Init_Variable(void);
static boolean f_EPS_PowerOn_Self_7s_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	fttmdl_EPSWARN_Init_Variable();

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
	fttmdl_EPSWARN_Init_Variable();

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
	fttmdl_EPSWARN_Init_Variable();

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
	fttmdl_EPSWARN_Init_Variable();

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
	uint8 IsEngineCfg_Eps = FALSE;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_EPS,&IsEngineCfg_Eps);

	if(TRUE == IsEngineCfg_Eps)
	{
		fttmdl_N356eps_process();
	}
	else
	{
		//
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

/*
--EpsWarn indicator--
input signal =>
CAN signal (0x140):
	1. BCM_LeftligthSt

		|	Input					|Output
		|	GW_EPS_FaultWarning		|Indicator
------------------------------------------------
		|	1/2						|turn on
		|	0/3						|turn off
------------------------------------------------
ps:
*/
/*********************************************************************************
		
	Function Name	  :  fttmdl_N356eps_process (void)
	
	Description 	  : EpsWarn Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/

static void fttmdl_EPSWARN_Init_Variable(void)
{
	l_epsred_status_u8 = cTT_TURN_OFF;
	Rte_Call_rpiCS_EpsWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_RED,l_epsred_status_u8);
	l_epsylw_status_u8 = cTT_TURN_OFF;
	Rte_Call_rpiCS_EpsWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_YLW,l_epsylw_status_u8);
}

static void fttmdl_N356eps_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR 	= 0;
	uint8 u8MsgTout = 0;
	
	uint8 fl_ign_substate = eIGN_OFF;
	uint8 fl_epsred_sig_value_U8 = 0;
	uint8 fl_epsylw_sig_value_U8 = 0;
	
	Rte_Read_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_Tout(&u8MsgTout);
	
	Rte_Read_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_SafeLampRed(&fl_epsred_sig_value_U8);
	Rte_Read_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_WarningLampYellow(&fl_epsylw_sig_value_U8);
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	
	if(eIGN_RUN == fl_ign_substate)
	{
		if ( RTE_E_TIMEOUT == u8MsgTout )
		{
			l_epsred_status_u8 = cTT_TURN_OFF;
			l_epsylw_status_u8 = cTT_TURN_ON;
		}
		else if( RTE_E_NEVER_RECEIVED == u8MsgNR )
		{
			f_EPS_PowerOn_Self_7s_Action();
			
			if(FALSE == l_EPS_Flag)
			{
				l_epsred_status_u8 = cTT_TURN_OFF;
				l_epsylw_status_u8 = cTT_TURN_ON;
			}

		}
		else
		{
			
			if(TURN_ON_EPS_RED == fl_epsred_sig_value_U8)
			{
				l_epsylw_status_u8 = cTT_TURN_OFF;
				l_epsred_status_u8 = cTT_TURN_ON;//red
			}
			else if(TURN_ON_EPS_YLW == fl_epsylw_sig_value_U8)
			{
				l_epsylw_status_u8 = cTT_TURN_ON;//yellow
				l_epsred_status_u8 = cTT_TURN_OFF;
			}
			else
			{
				l_epsylw_status_u8 = cTT_TURN_OFF;
				l_epsred_status_u8 = cTT_TURN_OFF;
			}	
		}
	}
	else 
	{
		l_epsylw_status_u8 = cTT_TURN_OFF;
		l_epsred_status_u8 = cTT_TURN_OFF;
		
		l_EPS_Flag = TRUE;
		l_Proveout_Counter_U8 = FALSE;
	}
	
	Rte_Call_rpiCS_EpsWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_RED,l_epsred_status_u8);
	Rte_Call_rpiCS_EpsWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_YLW,l_epsylw_status_u8);

#else

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_epsred_status_u8 = cTT_TURN_ON;
		Rte_Call_rpiCS_EpsWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_RED,l_epsred_status_u8);
		l_epsylw_status_u8 = cTT_TURN_ON;
		Rte_Call_rpiCS_EpsWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_YLW,l_epsylw_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_epsred_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpiCS_EpsWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_RED,l_epsred_status_u8);
		l_epsylw_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpiCS_EpsWarnMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_YLW,l_epsylw_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
#endif
}


static boolean f_EPS_PowerOn_Self_7s_Action(void)
{
    if(l_Proveout_Counter_U8 >= FUNCTION_EPS_TIME_6500MS)
    {
        l_EPS_Flag = FALSE;
    }
    else
    {
        l_EPS_Flag = TRUE;
		l_Proveout_Counter_U8++;
    }
	
    return l_EPS_Flag;
}


