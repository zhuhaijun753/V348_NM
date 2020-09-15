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
#include "Rte_FourWDFaultTtMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(FourWDFaultTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 l_FourWDFault_status_u8 = 0;

static void fttmdl_N3564WDFault_process(void);


//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_FourWDFault_status_u8 = cTT_TURN_OFF;
	Rte_Call_rpCS_FourWDFaultTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_L,l_FourWDFault_status_u8);

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
	l_FourWDFault_status_u8 = cTT_TURN_OFF;
	Rte_Call_rpCS_FourWDFaultTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_L,l_FourWDFault_status_u8);

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
	l_FourWDFault_status_u8 = cTT_TURN_OFF;
	Rte_Call_rpCS_FourWDFaultTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_L,l_FourWDFault_status_u8);

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
	l_FourWDFault_status_u8 = cTT_TURN_OFF;
	Rte_Call_rpCS_FourWDFaultTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_L,l_FourWDFault_status_u8);

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
	boolean ATS_FLAG = FALSE;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ATS,&ATS_FLAG);
	if(ATS_FLAG == TRUE)
	{
		fttmdl_N3564WDFault_process();
	}
	else
	{
		//NULL
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

static void fttmdl_N3564WDFault_process(void)
{
	// boolean fl_ign_substate = FALSE;
	uint8 fl_AWD_State_U8 = FALSE;
	uint8 fl_msgTout = 0;
	uint8 fl_msgNR = 0;

	Rte_Read_rpSR_CANMSG_GW_AWD_0x251_ComIn_NR(&fl_msgNR);
	Rte_Read_rpSR_CANMSG_GW_AWD_0x251_ComIn_Tout(&fl_msgTout);
	Rte_Read_rpSR_CANMSG_GW_AWD_0x251_ComIn_AWD_TrfCaseFltLmpReq(&fl_AWD_State_U8);


		if((fl_msgNR == RTE_E_NEVER_RECEIVED) || (fl_msgTout == RTE_E_TIMEOUT))
		{
			l_FourWDFault_status_u8 = cTT_TURN_OFF;
		}
		else
		{
			if(2 == fl_AWD_State_U8)
			{
				l_FourWDFault_status_u8 = cTT_TURN_ON;
			}
			else if(1 == fl_AWD_State_U8)
			{
				l_FourWDFault_status_u8 = cTT_FLASH_1HZ;
			}
			else
			{
				l_FourWDFault_status_u8 = cTT_TURN_OFF;
			}
		}
		
	Rte_Call_rpCS_FourWDFaultTtMdl_TIMdlUpdateTtStatus_Operation(cTTFour_WD,l_FourWDFault_status_u8);
}


