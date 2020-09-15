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
#include "Rte_FourWheelDriveHTtMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(FourWheelDriveHTtMdl)

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
// static uint8 l_4WheelDriveH_status_u8;
// static boolean l_4WheelDriveH_input;


// static void fttmdl_N3564WheelDriveH_process(void);
// static void fttmdl_N3564WheelDriveH_HW_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	// l_4WheelDriveH_input = 1;
	// l_4WheelDriveH_status_u8 = cTT_TURN_OFF;
	// Rte_Call_rpCS_FourWheelDriveHTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_H,l_4WheelDriveH_status_u8);

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
	// l_4WheelDriveH_input = 1;
	// l_4WheelDriveH_status_u8 = cTT_TURN_OFF;
	// Rte_Call_rpCS_FourWheelDriveHTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_H,l_4WheelDriveH_status_u8);

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
	// l_4WheelDriveH_input = 1;
	// l_4WheelDriveH_status_u8 = cTT_TURN_OFF;
	// Rte_Call_rpCS_FourWheelDriveHTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_H,l_4WheelDriveH_status_u8);

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
	// l_4WheelDriveH_input = 1;
	// l_4WheelDriveH_status_u8 = cTT_TURN_OFF;
	// Rte_Call_rpCS_FourWheelDriveHTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_H,l_4WheelDriveH_status_u8);

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
	// boolean Four_W_HCfg = FALSE;
	// boolean ATS_FLAG = FALSE;
	// Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_FourW_HL,&Four_W_HCfg);
	// Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ATS,&ATS_FLAG);
	// if(ATS_FLAG == TRUE)
	// {
	// 	//fttmdl_N3564WheelDriveH_process();
	// }
	
	// if(Four_W_HCfg == TRUE)
	// {
	// 	// fttmdl_N3564WheelDriveH_HW_process();
	// }

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
static void fourWheelH_input_check(void)
{
	Boolean b4WheelDriveHPinTmp = FALSE;
	static uint8 chk_cnt = 0;

	boolean fl_ign_substate;
	static boolean fl_ign_lastState = eIGN_OFF; 

	Rte_Call_rpCS_TIIoHwAb_DIn_Impl_GetPin(eIOPinInId_MDI_4W_H,&b4WheelDriveHPinTmp);
	(void)Rte_Read_rpIgnState_IGNState(&fl_ign_substate);


	if(fl_ign_substate != fl_ign_lastState)
	{
		fl_ign_lastState = 	fl_ign_substate;
		chk_cnt = 0;
		l_4WheelDriveH_input = 1;
		
	}

	if(l_4WheelDriveH_input == 1)
	{
		if(b4WheelDriveHPinTmp == 0)
		{
			chk_cnt ++;
			if(chk_cnt == 4)
			{
				chk_cnt = 0;
				l_4WheelDriveH_input = 0;
			}
		}
		else
		{
			chk_cnt = 0;
		}
	}
	else
	{
		chk_cnt = 0;
		l_4WheelDriveH_input = b4WheelDriveHPinTmp;
	}	
}

static void fttmdl_N3564WheelDriveH_process(void)
{
#ifndef TT_ONOFF_TEST

	//uint8 fl_ign_substate;
	boolean fl_ign_substate;
	fl_ign_substate = FALSE;
	uint8 fl_PTCaseMod_U8;
	uint8 fl_msgTout = 0;
	uint8 fl_msgNR = 0;
	
	(void)Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_NR(&fl_msgNR);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_Tout(&fl_msgTout);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_PTCaseMod(&fl_PTCaseMod_U8);
	
	if(eIGN_RUN == fl_ign_substate)
	{
		if((fl_msgNR == RTE_E_NEVER_RECEIVED) || (fl_msgTout == RTE_E_TIMEOUT))
		{
			l_4WheelDriveH_status_u8 = cTT_TURN_OFF;
		}
		else
		{
			if(3 == fl_PTCaseMod_U8)
			{
				l_4WheelDriveH_status_u8 = cTT_TURN_ON;
			}
			else
			{
				l_4WheelDriveH_status_u8 = cTT_TURN_OFF;
			}
		}
		
	}
	else 
	{
		l_4WheelDriveH_status_u8 = cTT_TURN_OFF;
	}
	Rte_Call_rpCS_FourWheelDriveHTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_H,l_4WheelDriveH_status_u8);

#else
	
		static unsigned int u32TimeCnt = 0;
		if(u32TimeCnt < 50)
		{
			l_4WheelDriveH_status_u8 = cTT_TURN_ON;
			Rte_Call_rpCS_FourWheelDriveHTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_H,l_4WheelDriveH_status_u8);
	
		}
		else if(u32TimeCnt < 100)
		{
			l_4WheelDriveH_status_u8 = cTT_TURN_OFF;
			Rte_Call_rpCS_FourWheelDriveHTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_H,l_4WheelDriveH_status_u8);
		}
		else
		{
			u32TimeCnt = 0;
		}
		u32TimeCnt++;
#endif

}

static void fttmdl_N3564WheelDriveH_HW_process(void)
{
	#if 0
	uint8 fl_ign_substate;
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);//获取IG ON 或OFF状态

	boolean b4WheelDriveHPin;
	b4WheelDriveHPin = FALSE;

	fourWheelH_input_check();
	b4WheelDriveHPin = l_4WheelDriveH_input;

	if(fl_ign_substate == eIGN_RUN)
	{
		if(FALSE== b4WheelDriveHPin)//low active
		{
			l_4WheelDriveH_status_u8 = cTT_TURN_ON;
		}
		else
		{
			l_4WheelDriveH_status_u8 = cTT_TURN_OFF;
		}
	}
	else
	{
		l_4WheelDriveH_status_u8 = cTT_TURN_OFF;
	}

	Rte_Call_rpCS_FourWheelDriveHTtMdl_TIMdlUpdateTtStatus_Operation(cTTFOUR_WHEEL_DRIVE_H,l_4WheelDriveH_status_u8);
	#endif
}
#endif
