

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
#include "Rte_StpClutchWrnMdl.h"
#include "CmpLib.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(StpClutchWrnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

typedef enum BCM_POWERMODE_SIGVAL
{
    POWERMODE_OFF,
    POWERMODE_ACC,
    POWERMODE_ON,
    POWERMODE_START
} BCM_POWERMODE_SIGVAL_E;

typedef enum EMS_CLUTHPEDALSTS_SIGVAL
{
    CLUTHPEDALSTS_NOT_PRESSED,
    CLUTHPEDALSTS_PRESSED,
    CLUTHPEDALSTS_RESERVED,
    CLUTHPEDALSTS_ERROR
} EMS_CLUTHPEDALSTS_SIGVAL_E;


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static void fun_StpClutchWrn_process(void);
static void fun_StpClutchWrn_init(void);

//======================================================================================================================
//  PRIVATE
//======================================================================================================================
static eIgnState l_last_ign_status_U8;

//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
    fun_StpClutchWrn_init();
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
    fun_StpClutchWrn_init();
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
    fun_StpClutchWrn_init();
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
    fun_StpClutchWrn_init();
    return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------

/****************************************************************************
Function Name     	: CmpActive

Description         :

Invocation          : called in cmpActive.

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static Std_ReturnType CmpActive(void)
{
    fun_StpClutchWrn_process();
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

static void fun_StpClutchWrn_init(void)
{
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_STEP_CLTCH_TO_BUTTON_ID, UI_WARNING_STATUS_OFF);
    l_last_ign_status_U8 = (eIgnState)eIGN_OFF;
}

static void fun_StpClutchWrn_process(void)
{
    uint8   fl_Msg313_NR_U8    = (uint8)RTE_E_NEVER_RECEIVED;
    uint8   fl_Msg313_Tout_U8  = (uint8)RTE_E_TIMEOUT;
    uint8   fl_PowerMode_U8    = (uint8)POWERMODE_OFF;
    uint8   fl_Msg10B_NR_U8    = (uint8)RTE_E_NEVER_RECEIVED;
    uint8   fl_Msg10B_Tout_U8  = (uint8)RTE_E_TIMEOUT;
    uint8   fl_CluthPedalSts_U8 = (uint8)CLUTHPEDALSTS_NOT_PRESSED;
    uint8   fl_CurIgnSts_U8    = (eIgnState)eIGN_OFF;
    uint8   fl_StpClutchNotFy_U8 = (uint8)UI_WARNING_STATE_NONE;

    Rte_Read_rpIgnState_IGNState(&fl_CurIgnSts_U8);

    Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_NR(&fl_Msg313_NR_U8);
    Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_Tout(&fl_Msg313_Tout_U8);
    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&fl_Msg10B_NR_U8);
    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&fl_Msg10B_Tout_U8);
    Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_PowerMode(&fl_PowerMode_U8);
    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_CluthPedalStatus(&fl_CluthPedalSts_U8);

    if (((uint8)RTE_E_NEVER_RECEIVED == fl_Msg313_NR_U8) || \
            ((uint8)RTE_E_TIMEOUT        == fl_Msg313_Tout_U8) || \
            ((uint8)RTE_E_NEVER_RECEIVED == fl_Msg10B_NR_U8) || \
            ((uint8)RTE_E_TIMEOUT        == fl_Msg10B_Tout_U8))
    {
        fun_StpClutchWrn_init();
    }
    else
    {
        if (((uint8)POWERMODE_START == fl_PowerMode_U8) && \
                ((uint8)CLUTHPEDALSTS_NOT_PRESSED == fl_CluthPedalSts_U8))
        {
            if (((eIgnState)eIGN_OFF == fl_CurIgnSts_U8) && (eIgnState)eIGN_OFF != l_last_ign_status_U8)
            {
                fl_StpClutchNotFy_U8 = CWarningCtrl_APP_get_notified_warning_status(WARN_STEP_CLTCH_TO_BUTTON_ID);
                if ((uint8)UI_WARNING_STATE_SHOW == fl_StpClutchNotFy_U8)
                {
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_STEP_CLTCH_TO_BUTTON_ID, UI_WARNING_STATUS_RETRIG);
                }
                else if (((uint8)UI_WARNING_STATE_TIMEOUT == fl_StpClutchNotFy_U8) || \
                         ((uint8)UI_WARNING_STATE_ACKNOWLEDED == fl_StpClutchNotFy_U8))
                {
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_STEP_CLTCH_TO_BUTTON_ID, UI_WARNING_CONDITION_FORCED);
                }
            }
            else
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_STEP_CLTCH_TO_BUTTON_ID, UI_WARNING_STATUS_ON);
            }
        }
        else
        {
            fun_StpClutchWrn_init();
        }
    }

    l_last_ign_status_U8 = fl_CurIgnSts_U8;

}


