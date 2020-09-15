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
#include "Rte_STTMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"


/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(STTMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

#define STT_STATUS_OFF          ((UINT8)0)
#define STT_GREEN_STATUS_ON     ((uint8)1)
#define STT_GRAY_STATUS_ON      ((uint8)2)
#define STT_YELLOW_STATUS_ON    ((uint8)3)

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static uint8 fl_STT_GRAY_STATUS = cTT_TURN_OFF;
static uint8 fl_STT_GREEN_STATUS = cTT_TURN_OFF;
static uint8 fl_STT_YELLOW_STATUS = cTT_TURN_OFF;

static void ftt_STTMdl_Process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
    fl_STT_GRAY_STATUS = cTT_TURN_OFF;
    fl_STT_GREEN_STATUS = cTT_TURN_OFF;
    fl_STT_YELLOW_STATUS = cTT_TURN_OFF;
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GRAY,fl_STT_GRAY_STATUS);
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GREEN,fl_STT_GREEN_STATUS);
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_YELLOW,fl_STT_YELLOW_STATUS);
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
    fl_STT_GRAY_STATUS = cTT_TURN_OFF;
    fl_STT_GREEN_STATUS = cTT_TURN_OFF;
    fl_STT_YELLOW_STATUS = cTT_TURN_OFF;
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GRAY,fl_STT_GRAY_STATUS);
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GREEN,fl_STT_GREEN_STATUS);
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_YELLOW,fl_STT_YELLOW_STATUS);
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
    fl_STT_GRAY_STATUS = cTT_TURN_OFF;
    fl_STT_GREEN_STATUS = cTT_TURN_OFF;
    fl_STT_YELLOW_STATUS = cTT_TURN_OFF;
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GRAY,fl_STT_GRAY_STATUS);
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GREEN,fl_STT_GREEN_STATUS);
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_YELLOW,fl_STT_YELLOW_STATUS);
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
    fl_STT_GRAY_STATUS = cTT_TURN_OFF;
    fl_STT_GREEN_STATUS = cTT_TURN_OFF;
    fl_STT_YELLOW_STATUS = cTT_TURN_OFF;
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GRAY,fl_STT_GRAY_STATUS);
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GREEN,fl_STT_GREEN_STATUS);
    Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_YELLOW,fl_STT_YELLOW_STATUS);
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
    //boolean IsEngineCfg = FALSE;
	// Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Start_Stop,&IsEngineCfg);
	// if(FALSE != IsEngineCfg) //Gasoline
	// {
		//ftt_STTMdl_Process();
	// }
	// else
	// {
		// NULL	Diesel
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

static void ftt_STTMdl_Process(void)
{
    uint8   u8MsgNR = 0;
    uint8   u8MsgTout = 0;
    uint8   fl_ign_substate = 0;
    uint8   fl_STT_Signal_Status = 0;

    Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(&u8MsgNR);
    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(&u8MsgTout);

    if(fl_ign_substate == eIGN_RUN)
    {
        if(u8MsgNR == RTE_E_NEVER_RECEIVED)
        {
            fl_STT_GRAY_STATUS = cTT_TURN_OFF;
            fl_STT_GREEN_STATUS = cTT_TURN_OFF;
            fl_STT_YELLOW_STATUS = cTT_TURN_OFF;
        }
        else if(u8MsgTout == RTE_E_TIMEOUT)
        {
            fl_STT_GRAY_STATUS = cTT_TURN_OFF;
            fl_STT_GREEN_STATUS = cTT_TURN_OFF;
            fl_STT_YELLOW_STATUS = cTT_TURN_ON;
        }
        else
        {
            Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_StartStopStatus(&fl_STT_Signal_Status);
            switch(fl_STT_Signal_Status)
            {
                case STT_GREEN_STATUS_ON:
                    fl_STT_GREEN_STATUS = cTT_TURN_ON;
                    fl_STT_GRAY_STATUS = cTT_TURN_OFF;
                    fl_STT_YELLOW_STATUS = cTT_TURN_OFF;
                    break;
                case STT_GRAY_STATUS_ON:
                    fl_STT_GRAY_STATUS = cTT_TURN_ON;
                    fl_STT_GREEN_STATUS = cTT_TURN_OFF;
                    fl_STT_YELLOW_STATUS = cTT_TURN_OFF;
                    break;
                case STT_YELLOW_STATUS_ON:
                    fl_STT_YELLOW_STATUS = cTT_TURN_ON;
                    fl_STT_GRAY_STATUS = cTT_TURN_OFF;
                    fl_STT_GREEN_STATUS = cTT_TURN_OFF;
                    break;
                default:
                    fl_STT_GRAY_STATUS = cTT_TURN_OFF;
                    fl_STT_GREEN_STATUS = cTT_TURN_OFF;
                    fl_STT_YELLOW_STATUS = cTT_TURN_OFF;
                    break;
            }
        }
    }
    else
    {
        fl_STT_GRAY_STATUS = cTT_TURN_OFF;
        fl_STT_GREEN_STATUS = cTT_TURN_OFF;
        fl_STT_YELLOW_STATUS = cTT_TURN_OFF;
    }
        Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GRAY,fl_STT_GRAY_STATUS);
        Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GREEN,fl_STT_GREEN_STATUS);
        Rte_Call_rpCS_STTMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_YELLOW,fl_STT_YELLOW_STATUS);
}

