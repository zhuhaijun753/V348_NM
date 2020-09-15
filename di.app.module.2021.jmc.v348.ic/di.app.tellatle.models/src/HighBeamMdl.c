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
#include "Rte_HighBeamMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(HighBeamMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

#define TURN_OFF_HIBEAM 	((uint8)1)
#define TURN_ON_HIBEAM 		((uint8)2)
#define TURN_ON_ADAS_AUTO	((uint8)3)

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 l_hibeam_status_u8 = cTT_TURN_OFF;

static uint8 l_lowbeam_status_u8;
static uint8 l_ADAS_AUTO_status_u8;
static uint8 l_last_lowbeam_status_u8;


static void fttmdl_N356hibeam_process(void);
static void fttmdl_N356lowbeam_process(void);
static void fttmdl_ADAS_AUTO_process(void);
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_hibeam_status_u8 = cTT_TURN_OFF;
	l_lowbeam_status_u8 = cTT_TURN_OFF;
	l_ADAS_AUTO_status_u8 = cTT_TURN_OFF;
	l_last_lowbeam_status_u8 = cTT_TURN_OFF;

	Rte_Call_rpiCS_HighBeamMdl_TIMdlUpdateTtStatus_Operation(cTTHIGH_BEAM,l_hibeam_status_u8);
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
	l_hibeam_status_u8 = cTT_TURN_OFF;
	l_lowbeam_status_u8 = cTT_TURN_OFF;
	l_ADAS_AUTO_status_u8 = cTT_TURN_OFF;
	l_last_lowbeam_status_u8 = cTT_TURN_OFF;

	Rte_Call_rpiCS_HighBeamMdl_TIMdlUpdateTtStatus_Operation(cTTHIGH_BEAM,l_hibeam_status_u8);

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
	l_hibeam_status_u8 = cTT_TURN_OFF;
	l_lowbeam_status_u8 = cTT_TURN_OFF;
	l_ADAS_AUTO_status_u8 = cTT_TURN_OFF;
	l_last_lowbeam_status_u8 = cTT_TURN_OFF;

	Rte_Call_rpiCS_HighBeamMdl_TIMdlUpdateTtStatus_Operation(cTTHIGH_BEAM,l_hibeam_status_u8);

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
	l_hibeam_status_u8 = cTT_TURN_OFF;
	l_lowbeam_status_u8 = cTT_TURN_OFF;
	l_ADAS_AUTO_status_u8 = cTT_TURN_OFF;
	l_last_lowbeam_status_u8 = cTT_TURN_OFF;

	Rte_Call_rpiCS_HighBeamMdl_TIMdlUpdateTtStatus_Operation(cTTHIGH_BEAM,l_hibeam_status_u8);

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
	boolean IsEngineCfg = FALSE;
	
	//Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_LDW,&IsEngineCfg);
	
	fttmdl_N356hibeam_process();
	//fttmdl_N356lowbeam_process();
	
	//if(IsEngineCfg == TRUE)
	//{
		//fttmdl_ADAS_AUTO_process();
	//}
	//else
	//{
		//NULL
	//}
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
		
	Function Name	  :  fttmdl_N356hibeam_process (void)
	
	Description 	  : HighBeam Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
static void fttmdl_N356hibeam_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;	
	uint8 fl_hibeam_sig_value_U8 = cTT_TURN_OFF;

	
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_HighBeamSts(&fl_hibeam_sig_value_U8);
	
	
	if ( (RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR) )
	{
		l_hibeam_status_u8 = cTT_TURN_OFF;
	}
	else
	{
		if(TURN_ON_HIBEAM == fl_hibeam_sig_value_U8)
		{
			l_hibeam_status_u8 = cTT_TURN_ON;
		}
		else
		{
			l_hibeam_status_u8 = cTT_TURN_OFF;
		}
	}
	
	Rte_Call_rpiCS_HighBeamMdl_TIMdlUpdateTtStatus_Operation(cTTHIGH_BEAM,l_hibeam_status_u8);
	Rte_Write_HighBeamMdl_rpSR_HighBeamMdl_Status_HighBeamSts(l_hibeam_status_u8);

#else

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_hibeam_status_u8 = cTT_TURN_ON;
		Rte_Call_rpiCS_HighBeamMdl_TIMdlUpdateTtStatus_Operation(cTTHIGH_BEAM,l_hibeam_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_hibeam_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpiCS_HighBeamMdl_TIMdlUpdateTtStatus_Operation(cTTHIGH_BEAM,l_hibeam_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
#endif
}

/*********************************************************************************
		
	Function Name	  :  fttmdl_N356lowbeam_process (void)
	
	Description 	  : LowBeam Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
static void fttmdl_N356lowbeam_process(void)
{
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;	
	uint8 fl_lowbeam_sig_value_U8;

	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamSts(&fl_lowbeam_sig_value_U8);
	
	
	if ( (RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR) )
	{
		l_lowbeam_status_u8 = cTT_TURN_OFF;
	}
	else
	{
		if(TURN_ON_HIBEAM == fl_lowbeam_sig_value_U8)
		{
			l_lowbeam_status_u8 = cTT_TURN_ON;
			l_last_lowbeam_status_u8 = l_lowbeam_status_u8;
		}
		else if(TURN_OFF_HIBEAM == fl_lowbeam_sig_value_U8)
		{
			l_lowbeam_status_u8 = cTT_TURN_OFF;
			l_last_lowbeam_status_u8 = l_lowbeam_status_u8;
		}
		else 
		{
			/* use previous state */
			l_lowbeam_status_u8 = l_last_lowbeam_status_u8;
		}
	}
	Rte_Write_HighBeamMdl_rpSR_HighBeamMdl_Status_LowBeamSts(l_lowbeam_status_u8);
}

static void fttmdl_ADAS_AUTO_process(void)
{
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;
	uint8 fl_ADAS_AUTO_value_U8;
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_HMA_Status(&fl_ADAS_AUTO_value_U8);

	if ( (RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR) )
	{
		l_ADAS_AUTO_status_u8 = cTT_TURN_OFF;
	}
	else
	{
		if(fl_ADAS_AUTO_value_U8 == 3)
		{
			l_ADAS_AUTO_status_u8 = cTT_TURN_ON;
		}
		else
		{
			l_ADAS_AUTO_status_u8 = cTT_TURN_OFF;
		}
	}
	Rte_Call_rpiCS_HighBeamMdl_TIMdlUpdateTtStatus_Operation(cTTADAS_AUTO,l_ADAS_AUTO_status_u8);
}


