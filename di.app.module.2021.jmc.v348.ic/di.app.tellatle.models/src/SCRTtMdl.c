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
#include "Rte_SCRTtMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"
//#include "DpfDteMdl_cfg.h"
//#include "Rte_AbsWarnMdl.h"
//#include "Rte_GlowTtMdl.h"


/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(SCRTtMdl)
static uint8 l_ureawarn_status_u8 = cTT_TURN_OFF;
#define FTT_UREAWARN_OFF_0 	((uint8)0)
#define FTT_UREAWARN_ON_1 	((uint8)1)
#define FTT_UREAWARN_ON_2HZ_2 	((uint8)2)
#define FTT_UREAWARN_ON_1HZ_3 	((uint8)3)
#define VEHICLE_NORMAL_SIX_SCR  (5)



static void fttmdl_N356SCR_process(void);


static Std_ReturnType CmpInit( void )
{
	l_ureawarn_status_u8 = cTT_TURN_OFF;

	Rte_Call_rpCS_SCRTtMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,l_ureawarn_status_u8);		
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
	l_ureawarn_status_u8 = cTT_TURN_OFF;

	Rte_Call_rpCS_SCRTtMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,l_ureawarn_status_u8);		
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
	l_ureawarn_status_u8 = cTT_TURN_OFF;

	Rte_Call_rpCS_SCRTtMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,l_ureawarn_status_u8);	
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
	l_ureawarn_status_u8 = cTT_TURN_OFF;

	Rte_Call_rpCS_SCRTtMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,l_ureawarn_status_u8);	
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
	uint8 IsEngineCfg_Scr = 0;
	
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_EMISSION_LEVELS,&IsEngineCfg_Scr);

	if(VEHICLE_NORMAL_SIX_SCR == IsEngineCfg_Scr)
	{
		fttmdl_N356SCR_process();
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

static void fttmdl_N356SCR_process(void)
{
#ifndef TT_ONOFF_TEST
		uint8 u8MsgNR = 0;
		uint8 u8MsgTout = 0;
		uint8 fl_scr_sig_value_U8 = 0;	
		uint8 fl_ign_substate	  = eIGN_OFF;
		
	
		Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_NR(&u8MsgNR);
		Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_Tout(&u8MsgTout);
		Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaWarn(&fl_scr_sig_value_U8);
		
		Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
		
		
		if(eIGN_RUN == fl_ign_substate)
		{
			if ( (RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR) )
			{
				l_ureawarn_status_u8 = cTT_TURN_OFF;
				
			}
			else 
			{
				
				if (FTT_UREAWARN_ON_1 == fl_scr_sig_value_U8)
				{
					l_ureawarn_status_u8 = cTT_TURN_ON;	
					
				}
				else if (FTT_UREAWARN_ON_2HZ_2 == fl_scr_sig_value_U8)
				{
					l_ureawarn_status_u8 = cTT_FLASH_2HZ;	
					
				}
				else if (FTT_UREAWARN_ON_1HZ_3 == fl_scr_sig_value_U8)
				{
					l_ureawarn_status_u8 = cTT_FLASH_1HZ;	
					
				}
				else
				{
					l_ureawarn_status_u8 = cTT_TURN_OFF;
					
				}
			}
	    }
		else
		{
			l_ureawarn_status_u8 = cTT_TURN_OFF;			
		}
	
		Rte_Call_rpCS_SCRTtMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,l_ureawarn_status_u8);
	
#else
		
		static unsigned int u32TimeCnt = 0;
		if(u32TimeCnt < 50)
		{
			l_ureawarn_status_u8 = cTT_TURN_ON;
			Rte_Call_rpCS_SCRTtMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,l_ureawarn_status_u8);
	
		}
		else if(u32TimeCnt < 100)
		{
			l_ureawarn_status_u8 = cTT_TURN_OFF;
			Rte_Call_rpCS_SCRTtMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,l_ureawarn_status_u8);
		}
		else
		{
			u32TimeCnt = 0;
		}
		u32TimeCnt++;
#endif
	}


