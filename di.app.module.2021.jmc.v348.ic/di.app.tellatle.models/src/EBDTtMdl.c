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
#include "Rte_EBDTtMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"
#include "TmExt_Cfg.h"
#include "TmExt_Timer.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(EBDTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8 l_ebd_status_u8 = cTT_TURN_OFF;
static boolean  l_EBD_Flag = TRUE;
static uint8    l_Proveout_Counter_U8 = FALSE;

#define EBDFLGFLT_SIG_STATE_0		((uint8)0x00)
#define EBDFLGFLT_SIG_STATE_1		((uint8)0x01)
#define EBDFLGFLT_SIG_STATE_2		((uint8)0x02)
#define EBDFLGFLT_SIG_STATE_3		((uint8)0x03)

#define FUNCTION_EBD_TIME_30MS                (30)           
#define FUNCTION_EBD_TIME_6500MS              (6500 / FUNCTION_EBD_TIME_30MS)   

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================


static void fttmdl_N356EBD_process(void);
static boolean f_EBD_PowerOn_Self_7s_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_ebd_status_u8 = cTT_TURN_OFF;
	l_EBD_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_rpCS_EBDTtMdl_TIMdlUpdateTtStatus_Operation(cTTEBD,l_ebd_status_u8);
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
	l_ebd_status_u8 = cTT_TURN_OFF;
	l_EBD_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_rpCS_EBDTtMdl_TIMdlUpdateTtStatus_Operation(cTTEBD,l_ebd_status_u8);
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
	l_ebd_status_u8 = cTT_TURN_OFF;
	l_EBD_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_rpCS_EBDTtMdl_TIMdlUpdateTtStatus_Operation(cTTEBD,l_ebd_status_u8);
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
	l_ebd_status_u8 = cTT_TURN_OFF;
	l_EBD_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
	Rte_Call_rpCS_EBDTtMdl_TIMdlUpdateTtStatus_Operation(cTTEBD,l_ebd_status_u8);
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
	fttmdl_N356EBD_process();
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

static void fttmdl_N356EBD_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR221   = 0;
	uint8 u8MsgTout221 = 0;
	uint8 u8MsgNR313   = 0;
	uint8 u8MsgTout313 = 0;
	
	uint8 fl_ign_substate = eIGN_OFF;
	uint8 fl_ABS_EbdFlgFlt_sig_val_U8 = 0;
	uint8 fl_BrakeFluidSts_sig_val_U8 = 0;
	
	uint8 IsEngineCfg_Abs = FALSE;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ABS,&IsEngineCfg_Abs);

	uint8 IsEngineCfg_Esp = FALSE;	
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ESP,&IsEngineCfg_Esp);
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_NR(&u8MsgNR313);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_Tout(&u8MsgTout313);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_BrakeFluidSts(&fl_BrakeFluidSts_sig_val_U8);

	if((TRUE == IsEngineCfg_Abs) || (TRUE == IsEngineCfg_Esp))
	{
		Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8MsgNR221);
		Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8MsgTout221);
		Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_EbdFlgFlt(&fl_ABS_EbdFlgFlt_sig_val_U8);
	}
	
	if(eIGN_RUN == fl_ign_substate)
	{
		if((RTE_E_TIMEOUT == u8MsgTout221) || (RTE_E_TIMEOUT == u8MsgTout313))
		{
			l_ebd_status_u8	= cTT_TURN_ON;
		}
		else if((RTE_E_NEVER_RECEIVED == u8MsgNR221) || (RTE_E_NEVER_RECEIVED == u8MsgNR313))
		{
			f_EBD_PowerOn_Self_7s_Action();
			
			if(FALSE == l_EBD_Flag)
			{
				l_ebd_status_u8 = cTT_TURN_ON;
			}
			else
			{
				l_ebd_status_u8 = cTT_TURN_OFF;
			}
		}
		else
		{
			if((EBDFLGFLT_SIG_STATE_2 == fl_BrakeFluidSts_sig_val_U8) 
			|| (EBDFLGFLT_SIG_STATE_1 == fl_ABS_EbdFlgFlt_sig_val_U8))
			{
				l_ebd_status_u8 = cTT_TURN_ON;
			}
			else
			{
				l_ebd_status_u8 = cTT_TURN_OFF;
			}
		}
		
	}
	else
	{
		l_ebd_status_u8 = cTT_TURN_OFF;

		l_EBD_Flag = TRUE;
		l_Proveout_Counter_U8 = FALSE;
	}
		
	Rte_Call_rpCS_EBDTtMdl_TIMdlUpdateTtStatus_Operation(cTTEBD,l_ebd_status_u8);
	
#else

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_ebd_status_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_EBDTtMdl_TIMdlUpdateTtStatus_Operation(cTTEBD,l_ebd_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_ebd_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_EBDTtMdl_TIMdlUpdateTtStatus_Operation(cTTEBD,l_ebd_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
#endif
}

static boolean f_EBD_PowerOn_Self_7s_Action(void)
{
    if(l_Proveout_Counter_U8 >= FUNCTION_EBD_TIME_6500MS)
    {
        l_EBD_Flag = FALSE;
    }
    else
    {
        l_EBD_Flag = TRUE;
		l_Proveout_Counter_U8++;
    }
	
    return l_EBD_Flag;
}

