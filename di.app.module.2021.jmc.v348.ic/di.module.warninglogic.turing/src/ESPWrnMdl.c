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
#include "Rte_ESPWrnMdl.h"
#include "CmpLib.h"
#include "WarningCtrl.h"

// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(ESPWrnMdl)
//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8 LastTcsFlgFlt = (uint8)0;
static uint8 LastVdcFlgFlt = (uint8)0;

//#define HDCWRN_CONFIG
#define MSG_HDC_NOT_AVAILABLE                     ((uint8)0x1)
#define MSG_HDC_AVAILABLE                         ((uint8)0x0)
#define MSG_ATHD_NOT_AVAILABLE                    ((uint8)0x0)
#define MSG_ATHD_AVAILABLE                        ((uint8)0x1)
#define ATS_SHIELD_TIMER                          (30U)
#define PROD_SPEC_ATS_MOD_WARNING_LIST_NUM        (10)
#define WARNING_ACTIVE                            ((uint8)0x1)


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static void ClearAVHWarning(void);
static void ClearABSWarning(void);
static void ClearESPWarning(void);
#ifdef HDCWRN_CONFIG
    static void ClearHDCWarning(void);
    static void HDC_Warning_Process(void);
#endif
static void AVHWarning_process(void);
static void ESP_Warning_Process(void);
static void ABS_Warning_Process(void);
static uint8 message_0x243_invalid(void);
static uint8 message_0x221_invalid(void);

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
    ClearAVHWarning();
    ClearESPWarning();
    ClearABSWarning();
#ifdef HDCWRN_CONFIG
    ClearHDCWarning();
#endif
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
    ClearAVHWarning();
    ClearESPWarning();
    ClearABSWarning();
#ifdef HDCWRN_CONFIG
    ClearHDCWarning();
#endif
    return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
// @brief  Transitional activation state
//
// @return E_OK:           Succeed
//<br>     E_NOT_READY:    In progress. Keeps the component in this state.
//<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation(void)
{
    ClearAVHWarning();
    ClearESPWarning();
    ClearABSWarning();
#ifdef HDCWRN_CONFIG
    ClearHDCWarning();
#endif
    return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
// @brief  Transitional de-activation state
//
// @return E_OK:           Succeed
//<br>     E_NOT_READY:    In progress
//<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation(void)
{
    ClearAVHWarning();
    ClearESPWarning();
    ClearABSWarning();
#ifdef HDCWRN_CONFIG
    ClearHDCWarning();
#endif
    return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
// @brief  State for normal operations
//
// @return The return value is disregarded. Say E_OK
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
    uint8 IGNSts = (uint8)eIGN_OFF;
    Rte_Read_ESPWrnMdl_rpIgnState_IGNState(&IGNSts);

    if ((uint8)eIGN_RUN == IGNSts)
    {
        ESP_Warning_Process();
        ABS_Warning_Process();
        AVHWarning_process();
#ifdef HDCWRN_CONFIG
        HDC_Warning_Process();
#endif
    }
    else
    {
        ClearAVHWarning();
        ClearESPWarning();
        ClearABSWarning();
#ifdef HDCWRN_CONFIG
        ClearHDCWarning();
#endif
    }

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



static uint8 message_0x243_invalid(void)
{
    uint8 u8MsgNR   = (uint8)RTE_E_TIMEOUT;
    uint8 u8MsgTout = (uint8)RTE_E_NEVER_RECEIVED;
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

static uint8 message_0x221_invalid(void)
{
    uint8 u8MsgNR   = (uint8)RTE_E_TIMEOUT;
    uint8 u8MsgTout = (uint8)RTE_E_NEVER_RECEIVED;
    Rte_Read_ESPWrnMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8MsgNR);
    Rte_Read_ESPWrnMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8MsgTout);
    if (((uint8)RTE_E_TIMEOUT == u8MsgTout) || ((uint8)RTE_E_NEVER_RECEIVED == u8MsgNR))
    {
        return (uint8)TRUE;
    }
    else
    {
        return (uint8)FALSE;
    }

}

static void AVHWarning_process(void)
{
    uint8 fl_AVHWarning_U8   = 0;
    uint8 fl_AvhAvailable_U8 = 0;

    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AVHWarningMessage(&fl_AVHWarning_U8);
    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhAvabliable(&fl_AvhAvailable_U8);

    if ((uint8)TRUE == message_0x243_invalid())
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CLOSE_DOOR_OR_SEATBELT_AVH_ID, UI_WARNING_STATUS_OFF);
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AUTOHOLD_FAILURE_ID, UI_WARNING_STATUS_OFF);
    }
    else
    {
        if (WARNING_ACTIVE == fl_AVHWarning_U8)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CLOSE_DOOR_OR_SEATBELT_AVH_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CLOSE_DOOR_OR_SEATBELT_AVH_ID, UI_WARNING_STATUS_OFF);
        }

        if (WARNING_ACTIVE == fl_AvhAvailable_U8)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AUTOHOLD_FAILURE_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AUTOHOLD_FAILURE_ID, UI_WARNING_STATUS_OFF);
        }
    }
}

static void ESP_Warning_Process(void)
{
    uint8 fl_read_ESP_TCSAvailable_U8 = (uint8)0;
    uint8 fl_read_ESP_VDCAvailable_U8 = (uint8)0;
    uint8 fl_ESPWarningSts_U8         = (uint8)0;
    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_TcsFlgFlt(&fl_read_ESP_TCSAvailable_U8);
    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_VdcFlgFlt(&fl_read_ESP_VDCAvailable_U8);
    fl_ESPWarningSts_U8 = CWarningCtrl_APP_get_notified_warning_status(WARN_ESP_FAULT_ID);
    if ((uint8)TRUE == message_0x243_invalid())
    {
        ClearESPWarning();
    }
    else
    {
        if (((WARNING_ACTIVE == fl_read_ESP_TCSAvailable_U8) && (WARNING_ACTIVE != LastTcsFlgFlt)) || \
                ((WARNING_ACTIVE == fl_read_ESP_VDCAvailable_U8) && (WARNING_ACTIVE != LastVdcFlgFlt)))
        {
            if ((uint8)UI_WARNING_STATE_NONE == fl_ESPWarningSts_U8)
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ESP_FAULT_ID, UI_WARNING_STATUS_ON);
            }
            else if (((uint8)UI_WARNING_STATE_SHOW == fl_ESPWarningSts_U8) ||
                     ((uint8)UI_WARNING_STATE_ACKNOWLEDED == fl_ESPWarningSts_U8))
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ESP_FAULT_ID, UI_WARNING_STATUS_RETRIG);
            }
            else if ((uint8)UI_WARNING_STATE_TIMEOUT == fl_ESPWarningSts_U8)
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ESP_FAULT_ID, UI_WARNING_CONDITION_FORCED);
            }
            else
            {
                /*Do nothing*/
            }
        }
        else if ((WARNING_ACTIVE != fl_read_ESP_TCSAvailable_U8) && (WARNING_ACTIVE != fl_read_ESP_VDCAvailable_U8))
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ESP_FAULT_ID, UI_WARNING_STATUS_OFF);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ESP_FAULT_ID, UI_WARNING_STATUS_ON);
        }


        LastTcsFlgFlt = fl_read_ESP_TCSAvailable_U8;
        LastVdcFlgFlt = fl_read_ESP_VDCAvailable_U8;
    }
}

static void ABS_Warning_Process(void)
{
    uint8 fl_read_ABS_EbdFlgFlt_U8    = (uint8)0;
    uint8 fl_read_ABS_AbsFlgFlt_U8    = (uint8)0;

    Rte_Read_ESPWrnMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_AbsFlgFlt(&fl_read_ABS_AbsFlgFlt_U8);
    Rte_Read_ESPWrnMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_EbdFlgFlt(&fl_read_ABS_EbdFlgFlt_U8);

    if ((uint8)TRUE == message_0x221_invalid())
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_EBD_FAULT_ID, UI_WARNING_STATUS_OFF);
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ABS_FAULT_ID, UI_WARNING_STATUS_OFF);
    }
    else
    {
        if ((WARNING_ACTIVE == fl_read_ABS_EbdFlgFlt_U8))
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_EBD_FAULT_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_EBD_FAULT_ID, UI_WARNING_STATUS_OFF);
        }

        if ((WARNING_ACTIVE == fl_read_ABS_AbsFlgFlt_U8))
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ABS_FAULT_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ABS_FAULT_ID, UI_WARNING_STATUS_OFF);
        }
    }
}

#ifdef HDCWRN_CONFIG
static void HDC_Warning_Process(void)
{
    uint8 fl_read_ESP_HDCAvailable = 0;
    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_HDCAvailable(&fl_read_ESP_HDCAvailable);
    if (TRUE == message_0x243_invalid())
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_HDC_UNAVIABLE_ID, UI_WARNING_STATUS_OFF);
    }
    else
    {
        if (1 == fl_read_ESP_HDCAvailable)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_HDC_UNAVIABLE_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_HDC_UNAVIABLE_ID, UI_WARNING_STATUS_OFF);
        }
    }
}
* /
#endif

static void ClearAVHWarning(void)
{
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CLOSE_DOOR_OR_SEATBELT_AVH_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AUTOHOLD_FAILURE_ID, UI_WARNING_STATUS_OFF);
}

static void ClearABSWarning(void)
{
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_EBD_FAULT_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ABS_FAULT_ID, UI_WARNING_STATUS_OFF);
}

static void ClearESPWarning(void)
{
    LastTcsFlgFlt = (uint8)0;
    LastVdcFlgFlt = (uint8)0;
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ESP_FAULT_ID, UI_WARNING_STATUS_OFF);
}

#ifdef HDCWRN_CONFIG
static void ClearHDCWarning(void)
{
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_HDC_UNAVIABLE_ID, UI_WARNING_STATUS_OFF);
}
#endif

