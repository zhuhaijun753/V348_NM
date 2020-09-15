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
#include "Rte_AirbagMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(AirbagMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8   l_airbag_status_u8 		= cTT_TURN_OFF;
static uint8   fl_Airbag_IGN_Last_state = eIGN_OFF;

#define AIRBAG_SIG_TURN_OFF			 ((uint8)0x00)
#define AIRBAG_SIG_FALSH_1HZ		 ((uint8)0x01)
#define AIRBAG_SIG_TURN_ON			 ((uint8)0x02)
#define AIRBAG_SIG_KEEP				 ((uint8)0x03)

#define FUNCTION_AIRBAG_TIME_30MS    (30)           
#define FUNCTION_AIRBAG_TIME_6500MS  (6500 / FUNCTION_AIRBAG_TIME_30MS)

#define AIRBAG_wait_start			 (0x14D)
#define CTT_AIRBAG_TIMER_10000_IN_MS (10000/30)	 // Airbag 10S(10000ms) timer 

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8   fl_Airbag_Behavior 	  	= 0;
static boolean Airbag_Wait_10s_Flag_u8  = FALSE;
static uint16  Airbag_Timer_10s_Count	= CTT_AIRBAG_TIMER_10000_IN_MS;

static boolean  l_Airbag_Flag = TRUE;
static uint8    l_Proveout_Counter_U8 = FALSE;

static void fttmdl_Airbag_process(void);
static void Airbag_Init_Variable(void);
static boolean f_Airbag_PowerOn_Self_7s_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	Airbag_Init_Variable();
	l_Airbag_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_tiCS_AirbagMdl_TIMdlUpdateTtStatus_Operation(cTTAIRBAG,l_airbag_status_u8);

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
	Airbag_Init_Variable();
	l_Airbag_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_tiCS_AirbagMdl_TIMdlUpdateTtStatus_Operation(cTTAIRBAG,l_airbag_status_u8);

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
	Airbag_Init_Variable();
	l_Airbag_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_tiCS_AirbagMdl_TIMdlUpdateTtStatus_Operation(cTTAIRBAG,l_airbag_status_u8);

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
	Airbag_Init_Variable();
	l_Airbag_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_tiCS_AirbagMdl_TIMdlUpdateTtStatus_Operation(cTTAIRBAG,l_airbag_status_u8);

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
	uint8 fl_ign_substate = eIGN_OFF;
	//uint8 Timer_status_4S = 0;
	uint8 IsEngineCfg_Air = FALSE;

	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AIRBAG,&IsEngineCfg_Air);

	if(TRUE == IsEngineCfg_Air)
	{
		if(eIGN_RUN == fl_ign_substate)
		{
			if(eIGN_OFF == fl_Airbag_IGN_Last_state)
			{
				Airbag_Wait_10s_Flag_u8 = FALSE;
				Airbag_Timer_10s_Count = CTT_AIRBAG_TIMER_10000_IN_MS;
			}
		
			// wait 4s
			if(Airbag_Timer_10s_Count > 0)
			{
				Airbag_Wait_10s_Flag_u8 = FALSE;
				Airbag_Timer_10s_Count--;
			}
			else
			{
				Airbag_Wait_10s_Flag_u8 = TRUE;
			}
		
			if(TRUE == Airbag_Wait_10s_Flag_u8)
			{
				fttmdl_Airbag_process();
			}
			else
			{
				l_airbag_status_u8 = cTT_TURN_OFF;
				fl_Airbag_Behavior = 0;
			}
		}
		else
		{
			Airbag_Wait_10s_Flag_u8 = FALSE;
			l_airbag_status_u8		= cTT_TURN_OFF;
			fl_Airbag_Behavior		= 0;
			Airbag_Timer_10s_Count	= CTT_AIRBAG_TIMER_10000_IN_MS;
		
			l_Airbag_Flag = TRUE;
			l_Proveout_Counter_U8 = FALSE;
		}
		
		fl_Airbag_IGN_Last_state = fl_ign_substate;
		
		Rte_Call_tiCS_AirbagMdl_TIMdlUpdateTtStatus_Operation(cTTAIRBAG,l_airbag_status_u8);
	}
	else
	{
		Rte_Call_tiCS_AirbagMdl_TIMdlUpdateTtStatus_Operation(cTTAIRBAG,FALSE);
	}
	
	Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_AirbagTelltaleBehavior(fl_Airbag_Behavior);
	
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
		
	Function Name	  :  fttmdl_Airbag_process (void)
	
	Description 	  : Airbag Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/

static void Airbag_Init_Variable(void)
{
	l_airbag_status_u8		= cTT_TURN_OFF;
	fl_Airbag_IGN_Last_state = eIGN_OFF;
	fl_Airbag_Behavior		= 0;
	Airbag_Wait_10s_Flag_u8	= FALSE;
	Airbag_Timer_10s_Count	= CTT_AIRBAG_TIMER_10000_IN_MS;
}

static void fttmdl_Airbag_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;
	uint8 fl_airbag_sig_val_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_AirBagFailSts(&fl_airbag_sig_val_U8);

	if(RTE_E_TIMEOUT == u8MsgTout)
	{
		l_airbag_status_u8 = cTT_TURN_ON;
		fl_Airbag_Behavior = 4;
	}
	else if (RTE_E_NEVER_RECEIVED == u8MsgNR)
	{
		f_Airbag_PowerOn_Self_7s_Action();
		
		if(FALSE == l_Airbag_Flag)
		{
			l_airbag_status_u8 = cTT_TURN_ON;
		}
		else
		{
			l_airbag_status_u8 = cTT_TURN_OFF;
		}
	
		fl_Airbag_Behavior = 0;
	}
	else
	{
		switch(fl_airbag_sig_val_U8)
		{
			case AIRBAG_SIG_FALSH_1HZ:
				l_airbag_status_u8 = cTT_FLASH_1HZ;
				fl_Airbag_Behavior = 3;
				break;
			case AIRBAG_SIG_TURN_ON:
				l_airbag_status_u8 = cTT_TURN_ON;
				fl_Airbag_Behavior = 2;
				break;
			default:
				l_airbag_status_u8 = cTT_TURN_OFF;
				fl_Airbag_Behavior = 0;
				break;
		}
	}

#else
	
	static unsigned int u32TimeCnt = 0;
	u32TimeCnt++;
	if(u32TimeCnt < 50)
	{
		l_airbag_status_u8 = cTT_TURN_ON;
		Rte_Call_tiCS_AirbagMdl_TIMdlUpdateTtStatus_Operation(cTTAIRBAG,l_airbag_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_airbag_status_u8 = cTT_TURN_OFF;
		Rte_Call_tiCS_AirbagMdl_TIMdlUpdateTtStatus_Operation(cTTAIRBAG,l_airbag_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}

#endif						
}

static boolean f_Airbag_PowerOn_Self_7s_Action(void)
{
    if(l_Proveout_Counter_U8 >= FUNCTION_AIRBAG_TIME_6500MS)
    {
        l_Airbag_Flag = FALSE;
    }
    else
    {
        l_Airbag_Flag = TRUE;
		l_Proveout_Counter_U8++;
    }
	
    return l_Airbag_Flag;
}

