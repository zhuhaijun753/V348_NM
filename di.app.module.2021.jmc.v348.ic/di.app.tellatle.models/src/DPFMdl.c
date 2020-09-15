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
#include "Rte_DPFMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(DPFMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

#define GPF_STATUS_OFF_0	((uint8)0X00)
#define GPF_STATUS_ON_4		((uint8)0X04)

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
//static uint8 fl_SCR_status_u8 = cTT_TURN_OFF;
//static uint8 fl_DPF_status_u8 = cTT_TURN_OFF;
//static uint8 fl_GPF_status_u8 = cTT_TURN_OFF;

//static void f_DPFinitialState(void);
//static void	fttmdl_DPF_process(void);
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
    //f_DPFinitialState();
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
    //f_DPFinitialState();
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
    //f_DPFinitialState();
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
    //f_DPFinitialState();
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

    //fttmdl_DPF_process();
   
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

#if 0
static void f_DPFinitialState(void)
{
	//fl_SCR_status_u8 = cTT_TURN_OFF;
	// Rte_Call_tiCS_DPFMdl_TIMdlUpdateTtStatus_Operation(cTTDPF,fl_SCR_status_u8);
	fl_GPF_status_u8 = cTT_TURN_OFF;
	Rte_Call_tiCS_DPFMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,fl_GPF_status_u8);
}


static void	fttmdl_DPF_process(void)
{
	uint8	fl_368_nvr_status_U8 = 0;
	uint8	fl_368_msg_status_U8 = 0;
	
	//uint8 	fl_EMS_UreaWarn_sigval_U8 = 0;
	uint8 	fi_EMS_DieslPrtcWarn_D_Rq = 0;
	boolean IsEngineCfg = FALSE;

	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_NR(&fl_368_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_Tout(&fl_368_msg_status_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_GasPrtcWarn_G_Rq(&fi_EMS_DieslPrtcWarn_D_Rq);

	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Engine,&IsEngineCfg);//TRUE:Gasoline,FALSE:Diesel

	if((RTE_E_NEVER_RECEIVED == fl_368_nvr_status_U8) || (RTE_E_TIMEOUT == fl_368_msg_status_U8))
	{
		fl_GPF_status_u8 = cTT_TURN_OFF;
		//fl_SCR_status_u8  = cTT_TURN_OFF;
	}
	else
	{
		if(FALSE != IsEngineCfg) //Gasoline
		{
			if(GPF_STATUS_ON_4 == fi_EMS_DieslPrtcWarn_D_Rq)
			{
				fl_GPF_status_u8 = cTT_TURN_ON;
			}
			else
			{
				fl_GPF_status_u8 = cTT_TURN_OFF;
			}
		}
		else //Diesel
		{
			#if 1
			fl_GPF_status_u8 = cTT_TURN_OFF;
			//fl_SCR_status_u8  = cTT_TURN_OFF;
			
			#else
			if(fl_EMS_UreaWarn_sigval_U8 == 1)
			{
				fl_SCR_status_u8 = cTT_TURN_ON;
			}
			else if(fl_EMS_UreaWarn_sigval_U8 == 2)
			{
				fl_SCR_status_u8 = cTT_FLASH_2HZ;
			}
			else if(fl_EMS_UreaWarn_sigval_U8 == 3)
			{
				fl_SCR_status_u8 = cTT_FLASH_1HZ;
			}
			else
			{
				fl_SCR_status_u8 = cTT_TURN_OFF;
			}
		
			if((fi_EMS_DieslPrtcWarn_D_Rq == 2)
			|| (fi_EMS_DieslPrtcWarn_D_Rq == 3)
			|| (fi_EMS_DieslPrtcWarn_D_Rq == 4))
			{
				fl_DPF_status_u8 = cTT_TURN_ON;
			}
			else
			{
				fl_DPF_status_u8 = cTT_TURN_OFF;
			}
			Rte_Call_tiCS_DPFMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,fl_DPF_status_u8);
			#endif
		}

	}
	
	Rte_Call_tiCS_DPFMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,fl_GPF_status_u8); 
	// Rte_Call_tiCS_DPFMdl_TIMdlUpdateTtStatus_Operation(cTTDPF,fl_SCR_status_u8);
	
}
#endif

