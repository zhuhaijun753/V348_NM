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
#include "Rte_CruiseTtMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(CruiseTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define CRUISE_TT_OFF			((uint8)0x00)
#define CRUISE_TT_ACTIVE		((uint8)0x01)
#define CRUISE_TT_STANDBY		((uint8)0x02)
#define CRUISE_TT_ERROR			((uint8)0x03)
#define WITH_CRUISE				((uint8)0x01)

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */
#define CUIRSE_INVAILD_TGTSPEED   ((uint16)0x1FF)


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 l_cruise_status_u8;
static uint16 l_cruise_tgtSpeed_u16;
//static uint8 l_last_cruise_status_u8;
static uint8 l_cruise_status_red_u8;



static void fttmdl_N356Cruise_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_cruise_status_u8 = cTT_TURN_OFF;
	l_cruise_status_red_u8 = cTT_TURN_OFF;
	//l_last_cruise_status_u8 = cTT_TURN_OFF;
	//l_cruise_tgtSpeed_u16 = 0;
	Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_CONTROL,l_cruise_status_u8);
	Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_RED,l_cruise_status_red_u8);
	//Rte_Write_ppCS_CruiseMdl_TargetCruiseSpeed(l_cruise_tgtSpeed_u16);
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
	l_cruise_status_u8 = cTT_TURN_OFF;
	l_cruise_status_red_u8 = cTT_TURN_OFF;
	//l_last_cruise_status_u8 = cTT_TURN_OFF;
	//l_cruise_tgtSpeed_u16 = 0;
	Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_CONTROL,l_cruise_status_u8);
	Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_RED,l_cruise_status_red_u8);
	//Rte_Write_ppCS_CruiseMdl_TargetCruiseSpeed(l_cruise_tgtSpeed_u16);
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
	l_cruise_status_u8 = cTT_TURN_OFF;
	l_cruise_status_red_u8 = cTT_TURN_OFF;
	//l_last_cruise_status_u8 = cTT_TURN_OFF;
	//l_cruise_tgtSpeed_u16 = 0;
	Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_CONTROL,l_cruise_status_u8);
	Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_RED,l_cruise_status_red_u8);
	//Rte_Write_ppCS_CruiseMdl_TargetCruiseSpeed(l_cruise_tgtSpeed_u16);
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
	l_cruise_status_u8 = cTT_TURN_OFF;
	l_cruise_status_red_u8 = cTT_TURN_OFF;
	//l_last_cruise_status_u8 = cTT_TURN_OFF;
	//l_cruise_tgtSpeed_u16 = 0;
	Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_CONTROL,l_cruise_status_u8);
	Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_RED,l_cruise_status_red_u8);
	//Rte_Write_ppCS_CruiseMdl_TargetCruiseSpeed(l_cruise_tgtSpeed_u16);
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
	uint8 IsEngineCfg = FALSE;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Cruise,&IsEngineCfg);

	//IsEngineCfg = 0x01;
	if(WITH_CRUISE == IsEngineCfg)//TRUE:Cruise,FALSE:no Cruise 
	{
		fttmdl_N356Cruise_process();
		//Rte_Write_ppCS_CruiseMdl_TargetCruiseSpeed(l_cruise_tgtSpeed_u16);
	}
	else
	{
		Rte_Write_ppSR_CruiseTtMdl_Output_CruiseControlStatus(cTT_TURN_OFF);
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

static void fttmdl_N356Cruise_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;

	uint8 fl_EMS_CruiseControlStatus_sig_val_U8;
	uint16 fl_EMS_TargCruiseSpeed_U16 = 0;
	uint8 fl_ign_substate ;
	//uint8 fl_batt_state_U8;
		
	
	Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus(&fl_EMS_CruiseControlStatus_sig_val_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_TargCruiseSpeed(&fl_EMS_TargCruiseSpeed_U16);
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	//Rte_Read_rpBattState_BatteryState(&fl_batt_state_U8);
	

	if( fl_ign_substate == eIGN_RUN )
	{
		/* If CAN signal is Missing > timeout,TURN OFF TT */ 
		if(u8MsgTout == RTE_E_TIMEOUT)
		{
			l_cruise_status_u8 = cTT_TURN_OFF;
			l_cruise_status_red_u8 = cTT_TURN_OFF;
			l_cruise_tgtSpeed_u16 = 0;
		}
		else if (u8MsgNR == RTE_E_NEVER_RECEIVED)
		{
			l_cruise_status_u8 = cTT_TURN_OFF;
			l_cruise_status_red_u8 = cTT_TURN_OFF;
			l_cruise_tgtSpeed_u16 = 0;
		}
		else
		{
			//if(fl_batt_state_U8 != eBatteryState_Normal )
			//{
				//l_cruise_status_u8 = cTT_TURN_OFF;
				//l_cruise_tgtSpeed_u16 = 0;
			//}
			//else
			//{
				switch( fl_EMS_CruiseControlStatus_sig_val_U8 )
				{
					case CRUISE_TT_OFF:
						l_cruise_status_u8 = cTT_TURN_OFF;
						l_cruise_status_red_u8 = cTT_TURN_OFF;
						//l_last_cruise_status_u8 = l_cruise_status_u8;
						break;
					case CRUISE_TT_ACTIVE:
						l_cruise_status_u8 = cTT_TURN_ON;
						l_cruise_status_red_u8 = cTT_TURN_OFF;
						//l_last_cruise_status_u8 = l_cruise_status_u8;
						break;
					case CRUISE_TT_STANDBY:
						l_cruise_status_u8 = cTT_FLASH_1HZ;
						l_cruise_status_red_u8 = cTT_TURN_OFF;
						//l_last_cruise_status_u8 = l_cruise_status_u8;
						break;
					case CRUISE_TT_ERROR:
						/* kepp the last disp */
						//l_cruise_status_u8 = l_last_cruise_status_u8;
						l_cruise_status_u8 = cTT_TURN_OFF;
						l_cruise_status_red_u8 = cTT_TURN_ON;
						
						break;
					default:
						break;
				}
				l_cruise_tgtSpeed_u16 = fl_EMS_TargCruiseSpeed_U16;
		
		}
	}
	else
	{
		l_cruise_status_u8 = cTT_TURN_OFF;
		l_cruise_status_red_u8 = cTT_TURN_OFF;
		l_cruise_tgtSpeed_u16 = 0;
	}
	
	Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_CONTROL,l_cruise_status_u8);
	Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_RED,l_cruise_status_red_u8);
	Rte_Write_ppSR_CruiseTtMdl_Output_TargetCruiseSpeed(l_cruise_tgtSpeed_u16);
	Rte_Write_ppSR_CruiseTtMdl_Output_CruiseControlStatus(l_cruise_status_u8);
	
#else
	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_cruise_status_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_CONTROL,l_cruise_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_cruise_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_CruiseTtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_CONTROL,l_cruise_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
#endif
}

