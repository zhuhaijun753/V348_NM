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
#include "Rte_LeftRightTurnTtMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(LeftRightTurnTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

#define FTT_BOTH_TURN_IND_OFF 	((uint8)0)
#define FTT_LEFT_TURN_IND_ON 	((uint8)1)
#define FTT_RIGHT_TURN_IND_ON 	((uint8)2)
#define FTT_BOTH_TURN_IND_ON 	((uint8)3)

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 l_lturn_status_u8 = cTT_TURN_OFF;
static uint8 l_rturn_status_u8 = cTT_TURN_OFF;

static void fttmdl_N356lrturn_process(void);
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_lturn_status_u8 = cTT_TURN_OFF;
	l_rturn_status_u8 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTLEFT_TURN,l_lturn_status_u8);
	Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTRIGHT_TURN,l_rturn_status_u8);
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
	l_lturn_status_u8 = cTT_TURN_OFF;
	l_rturn_status_u8 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTLEFT_TURN,l_lturn_status_u8);
	Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTRIGHT_TURN,l_rturn_status_u8);
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
	l_lturn_status_u8 = cTT_TURN_OFF;
	l_rturn_status_u8 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTLEFT_TURN,l_lturn_status_u8);
	Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTRIGHT_TURN,l_rturn_status_u8);
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
	l_lturn_status_u8 = cTT_TURN_OFF;
	l_rturn_status_u8 = cTT_TURN_OFF;
	
	Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTLEFT_TURN,l_lturn_status_u8);
	Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTRIGHT_TURN,l_rturn_status_u8);
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
	fttmdl_N356lrturn_process();
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


static void fttmdl_N356lrturn_process(void)
{
#ifndef TT_ONOFF_TEST
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;
	uint8 fl_turn_ind_value_U8 = cTT_TURN_OFF;
	uint8 fl_ign_substate 	   = eIGN_OFF;
	
	//Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&u8MsgNR);
	//Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&u8MsgTout);
	//Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_TurnIndicatorSts(&fl_turn_ind_value_U8);

	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorSts(&fl_turn_ind_value_U8);
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	
	if ( (RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR) )
	{
		l_lturn_status_u8 = cTT_TURN_OFF;
		l_rturn_status_u8 = cTT_TURN_OFF;
	}
	else 
	{
		
		if ((FTT_LEFT_TURN_IND_ON == fl_turn_ind_value_U8))
		{
			l_lturn_status_u8 = cTT_TURN_ON;
			l_rturn_status_u8 = cTT_TURN_OFF;
		}
		else if ((FTT_RIGHT_TURN_IND_ON == fl_turn_ind_value_U8))
		{
			l_lturn_status_u8 = cTT_TURN_OFF;
			l_rturn_status_u8 = cTT_TURN_ON;
		}
		else if ((FTT_BOTH_TURN_IND_ON == fl_turn_ind_value_U8))
		{
			l_lturn_status_u8 = cTT_TURN_ON;
			l_rturn_status_u8 = cTT_TURN_ON;
		}
		else
		{
			l_lturn_status_u8 = cTT_TURN_OFF;
			l_rturn_status_u8 = cTT_TURN_OFF;
		}
	}

	if((eIGN_OFF == fl_ign_substate) && (FTT_BOTH_TURN_IND_ON != fl_turn_ind_value_U8))
	{
		l_lturn_status_u8 = cTT_TURN_OFF;
		l_rturn_status_u8 = cTT_TURN_OFF;
	}

	Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTLEFT_TURN,l_lturn_status_u8);
	Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTRIGHT_TURN,l_rturn_status_u8);

#else
	
	//left turn TT
	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_lturn_status_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTLEFT_TURN,l_lturn_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_lturn_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTLEFT_TURN,l_lturn_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
	
	//right turn TT
	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_rturn_status_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTRIGHT_TURN,l_rturn_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_rturn_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_LeftRightTurnTtMdl_TIMdlUpdateTtStatus_Operation(cTTRIGHT_TURN,l_rturn_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
#endif
}
