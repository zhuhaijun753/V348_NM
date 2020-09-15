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
#include "Rte_EPBWrnMdl.h"
#include "CmpLib.h"
// #include "peps_wrn_cfg.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(EPBWrnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define TIMEOUT_3000MS (3000u)
#define BAT_VOLTAGE_NORMAL     0x02
#define WARNING_ACTIVE   (uint8)0x1
/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static void fun_EPB_Warning_Process(eIgnState ignstate);
static void EPB_Warning_init(void);
static uint8 message_invalid(void);
static eIgnState l_PreIgnState_U8;
//static boolean IsTimerStarted = FALSE;
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
    EPB_Warning_init();
    l_PreIgnState_U8 = (eIgnState)eIGN_OFF;
    //IsTimerStarted = FALSE;
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit(void)
{
    EPB_Warning_init();
    l_PreIgnState_U8 = (eIgnState)eIGN_OFF;
    //IsTimerStarted = FALSE;
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation(void)
{
    EPB_Warning_init();
    l_PreIgnState_U8 = (eIgnState)eIGN_OFF;
    //IsTimerStarted = FALSE;
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation(void)
{
    EPB_Warning_init();
    l_PreIgnState_U8 = (eIgnState)eIGN_OFF;
    //IsTimerStarted = FALSE;
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
/****************************************************************************
Function Name       : CmpActive

Description         : This function process the warning for Security Authentication Failed.

Invocation          : called in cmpActive.

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/

static Std_ReturnType CmpActive(void)
{
    //EBatteryState   Battery_state = BAT_VOLTAGE_NORMAL;
    eIgnState IGN_state = (eIgnState)eIGN_OFF;
    //boolean         EPBWait3s_Elapsed = FALSE;
    //Rte_Read_rpBattState_BatteryState(&Battery_state);
    Rte_Read_rpIgnState_IGNState(&IGN_state);
    if ((uint8)FALSE == message_invalid())
    {
        fun_EPB_Warning_Process(IGN_state);
    }
    else
    {
        EPB_Warning_init();
    }

    l_PreIgnState_U8 = IGN_state;
#if 0
    if (BAT_VOLTAGE_NORMAL == Battery_state)
    {
        Rte_Read_rpIgnState_IGNState(&IGN_state);
        if (eIGN_RUN == IGN_state)
        {
            Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerEPBWait3s, &IsTimerStarted);
            if (FALSE == IsTimerStarted)
            {
                Rte_Call_rpTimer_TmExt_Timer_Start(eTimerEPBWait3s);
            }
            else
            {
                Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerEPBWait3s, TIMEOUT_3000MS, &EPBWait3s_Elapsed);

                if (FALSE != EPBWait3s_Elapsed)
                {
                    if (FALSE == message_invalid())
                    {
                        fun_EPB_Warning_Process();
                    }
                    else
                    {
                        EPB_Warning_init();
                    }
                }
                else
                {
                    EPB_Warning_init();
                }
            }
        }
        else
        {
            EPB_Warning_init();
            IsTimerStarted      = FALSE;
            Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerEPBWait3s);
        }
    }
    else
    {
        EPB_Warning_init();
        IsTimerStarted      = FALSE;
        Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerEPBWait3s);
    }
#endif
    return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief  Diagnostic state
///
/// @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag(void)
{
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn(void)
{
    return E_OK;
}


static uint8 message_invalid(void)
{
    uint8 u8MsgNR   = (uint8)RTE_E_NEVER_RECEIVED;
    uint8 u8MsgTout = (uint8)RTE_E_TIMEOUT;

    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(&u8MsgNR);
    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(&u8MsgTout);


    if (((uint8)RTE_E_TIMEOUT == u8MsgTout) || ((uint8)RTE_E_NEVER_RECEIVED == u8MsgNR))
    {
        return (uint8)TRUE;
    }
    else
    {
        return (uint8)FALSE;
    }
}

static void fun_EPB_Warning_Process(eIgnState ignstate)
{
    uint8 u8_EPBWarningMessage1 = 0;
    uint8 u8_EPBWarningMessage2 = 0;
    uint8 u8_EPBWarningMessage3 = 0;
    uint8 PressBrakePedalWrn_NotiFySts_U8 = (uint8)UI_WARNING_STATE_NONE;

    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage1(&u8_EPBWarningMessage1);
    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage2(&u8_EPBWarningMessage2);
    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage3(&u8_EPBWarningMessage3);
    PressBrakePedalWrn_NotiFySts_U8 = CWarningCtrl_APP_get_notified_warning_status(WARN_CHOOSE_FLAT_ROAD_PARKING_ID);

    if (((eIgnState)eIGN_RUN == l_PreIgnState_U8) && ((eIgnState)eIGN_OFF == ignstate))
    {
        /*IGN ON --> IGN OFF, just run once*/
        if (WARNING_ACTIVE == u8_EPBWarningMessage3)
        {
            if ((uint8)UI_WARNING_STATE_SHOW == PressBrakePedalWrn_NotiFySts_U8)
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CHOOSE_FLAT_ROAD_PARKING_ID, UI_WARNING_STATUS_RETRIG);
            }
            else if ((UI_WARNING_STATE_TIMEOUT == PressBrakePedalWrn_NotiFySts_U8) || \
                     (UI_WARNING_STATE_ACKNOWLEDED == PressBrakePedalWrn_NotiFySts_U8))
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CHOOSE_FLAT_ROAD_PARKING_ID, UI_WARNING_CONDITION_FORCED);
            }
            else
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CHOOSE_FLAT_ROAD_PARKING_ID, UI_WARNING_STATUS_ON);
            }
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CHOOSE_FLAT_ROAD_PARKING_ID, UI_WARNING_STATUS_OFF);
        }
    }
    else
    {
        /*IGN ON or IGN OFF*/
        if (WARNING_ACTIVE == u8_EPBWarningMessage3)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CHOOSE_FLAT_ROAD_PARKING_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CHOOSE_FLAT_ROAD_PARKING_ID, UI_WARNING_STATUS_OFF);
        }
    }

    if ((eIgnState)eIGN_RUN == ignstate)
    {
        if (WARNING_ACTIVE == u8_EPBWarningMessage2)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CLOSE_DOOR_OR_SEATBELT_EPB_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CLOSE_DOOR_OR_SEATBELT_EPB_ID, UI_WARNING_STATUS_OFF);
        }

        if (WARNING_ACTIVE == u8_EPBWarningMessage1)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_PRESS_BRAKE_PEDAL_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_PRESS_BRAKE_PEDAL_ID, UI_WARNING_STATUS_OFF);
        }
    }
    else
    {
        /*Warning does not work*/
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CLOSE_DOOR_OR_SEATBELT_EPB_ID, UI_WARNING_STATUS_OFF);
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_PRESS_BRAKE_PEDAL_ID, UI_WARNING_STATUS_OFF);
    }
}



static void EPB_Warning_init(void)
{
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CHOOSE_FLAT_ROAD_PARKING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CLOSE_DOOR_OR_SEATBELT_EPB_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_PRESS_BRAKE_PEDAL_ID, UI_WARNING_STATUS_OFF);
}


