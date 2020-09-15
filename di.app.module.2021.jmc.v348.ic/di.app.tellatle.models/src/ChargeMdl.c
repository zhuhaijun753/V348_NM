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
#include "Rte_ChargeMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(ChargeMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

#define VEHICLE_TACH_INVALID					 (0x7FFF)

#define FUNCTION_CHARGE_TIME_30MS                (30)           
#define FUNCTION_CHARGE_TIME_6500MS              (6500 / FUNCTION_CHARGE_TIME_30MS)                       


/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static uint8 TtChargeStatus               = cTT_TURN_OFF;
static uint8 l_charge_warn_status_u8      = 0;
static uint8 charge_invalid_cnt 		  = 0;
static uint16 l_Pre_EngFilterSpd_U16      = 0;
static boolean  l_Charge_Flag       = TRUE;
static uint8    l_ProveoutCounter_U8   = 0;

static void fttmdl_Charge_process(void);
static boolean Charge_PowerOn_Self_7s_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	charge_invalid_cnt = 0;
	TtChargeStatus = cTT_TURN_OFF;
	
	Rte_Call_rpiCS_ChargeMdl_TIMdlUpdateTtStatus_Operation(cTTCHARGE,TtChargeStatus);
	
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
	charge_invalid_cnt = 0;
	TtChargeStatus = cTT_TURN_OFF;
	
	Rte_Call_rpiCS_ChargeMdl_TIMdlUpdateTtStatus_Operation(cTTCHARGE,TtChargeStatus);
	
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
	charge_invalid_cnt = 0;
	TtChargeStatus = cTT_TURN_OFF;
	
	Rte_Call_rpiCS_ChargeMdl_TIMdlUpdateTtStatus_Operation(cTTCHARGE,TtChargeStatus);
	
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
	charge_invalid_cnt = 0;
	TtChargeStatus = cTT_TURN_OFF;
	
	Rte_Call_rpiCS_ChargeMdl_TIMdlUpdateTtStatus_Operation(cTTCHARGE,TtChargeStatus);
	
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
	fttmdl_Charge_process();

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
		
	Function Name	  :  fttmdl_Charge_process (void)
	
	Description 	  : Charge Warning Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
static void fttmdl_Charge_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 	u8MsgNR = 0;
	uint8 	u8MsgTout = 0;
	uint8	fl_ign_substate = eIGN_OFF;
	uint8 	fl_BatteryVoltageError = 0;
	uint16	fl_EngFilterSpd_U16 = 0;
	
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&u8MsgTout);	
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&fl_EngFilterSpd_U16);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_BatteryVoltageError(&fl_BatteryVoltageError);
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);

	if(fl_ign_substate  == eIGN_RUN )
	{
         if (RTE_E_NEVER_RECEIVED == u8MsgNR)
         {
			 Charge_PowerOn_Self_7s_Action();
			 
         	if(FALSE == l_Charge_Flag)
         	{
				l_charge_warn_status_u8 = cTT_TURN_ON;
			}
         }
         else if (RTE_E_TIMEOUT == u8MsgTout)
         {
         	l_charge_warn_status_u8 = cTT_TURN_ON;	
         }
         else 
         {
         	if(VEHICLE_TACH_INVALID == fl_EngFilterSpd_U16)
         	{		
         		if(charge_invalid_cnt > 66)
         		{
         			fl_EngFilterSpd_U16 = 0;	
         		}
         		else
         		{
         			fl_EngFilterSpd_U16 = l_Pre_EngFilterSpd_U16;
         			charge_invalid_cnt++;
         		}
         	}
         	else
         	{
         		l_Pre_EngFilterSpd_U16 = fl_EngFilterSpd_U16;
         		charge_invalid_cnt = 0;
         	}
         		
         	if(fl_EngFilterSpd_U16 >= 900 && 1 == fl_BatteryVoltageError)  /*EngSpeed >= 450rpm*/
         	{
         		l_charge_warn_status_u8 = cTT_TURN_ON;
				l_Pre_EngFilterSpd_U16 = fl_EngFilterSpd_U16;
         	}
         	else if(fl_EngFilterSpd_U16 < 900)
         	{	
         		l_charge_warn_status_u8 = cTT_TURN_ON;
         	}
         	else
         	{
         		l_charge_warn_status_u8 = cTT_TURN_OFF;
         	}
         }
    }
	else
	{
		charge_invalid_cnt = 0;
		l_charge_warn_status_u8 = cTT_TURN_OFF;
		l_Charge_Flag       = TRUE;
		l_ProveoutCounter_U8   = FALSE;
	}

	Rte_Call_rpiCS_ChargeMdl_TIMdlUpdateTtStatus_Operation(cTTCHARGE,l_charge_warn_status_u8);
	
#else 
		
	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_charge_warn_status_u8 = cTT_TURN_ON;
		Rte_Call_rpiCS_ChargeMdl_TIMdlUpdateTtStatus_Operation(cTTCHARGE,l_charge_warn_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_charge_warn_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpiCS_ChargeMdl_TIMdlUpdateTtStatus_Operation(cTTCHARGE,l_charge_warn_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
	
#endif
}

static boolean Charge_PowerOn_Self_7s_Action(void)
{
    if(l_ProveoutCounter_U8 >= FUNCTION_CHARGE_TIME_6500MS)
    {
        l_Charge_Flag = FALSE;
    }
    else
    {
        l_Charge_Flag = TRUE;
	    l_ProveoutCounter_U8++;
    }

    return l_Charge_Flag;
}


