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

#include "Rte_CruiseCtrlWrnMdl.h"
#include "CmpLib.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(CruiseCtrlWrnMdl)


//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define CRUISE_CONTROL_ACTIVE  (uint8)0x1
#define CRUISE_CONTROL_ERROR   (uint8)0x3


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================


//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static void fun_CruiseCtrl_init(void);
static void fun_CruiseCtrl_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
    fun_CruiseCtrl_init();
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
    fun_CruiseCtrl_init();
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
    fun_CruiseCtrl_init();
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
    fun_CruiseCtrl_init();
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
    fun_CruiseCtrl_process();
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

static void fun_CruiseCtrl_init(void)
{
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CRUISE_CONTROL_ACTIVE_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CRUISE_CONTROL_FAULT_ID, UI_WARNING_STATUS_OFF);
}


static void fun_CruiseCtrl_process(void)
{
    boolean fl_CruiseCtrlCfg_BOOL = FALSE;

    uint8 fl_Msg151NrSts_U8   = RTE_E_NEVER_RECEIVED;
    uint8 fl_Msg151ToutSts_U8 = RTE_E_TIMEOUT;
    uint8 fl_CruiseCtrlSts_U8 = 0;

    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Cruise, &fl_CruiseCtrlCfg_BOOL);

   // fl_CruiseCtrlCfg_BOOL = TRUE;

    if ((boolean)TRUE == fl_CruiseCtrlCfg_BOOL)
    {
        Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_NR(&fl_Msg151NrSts_U8);
        Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_Tout(&fl_Msg151ToutSts_U8);
        Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus(&fl_CruiseCtrlSts_U8);
        
        if (((uint8)RTE_E_NEVER_RECEIVED == fl_Msg151NrSts_U8) || ((uint8)RTE_E_TIMEOUT == fl_Msg151ToutSts_U8))
        {
            fun_CruiseCtrl_init();
        }
        else
        {
            switch (fl_CruiseCtrlSts_U8)
            {

                case CRUISE_CONTROL_ACTIVE:
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CRUISE_CONTROL_ACTIVE_ID, UI_WARNING_STATUS_ON);
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CRUISE_CONTROL_FAULT_ID, UI_WARNING_STATUS_OFF);
                    break;

                case CRUISE_CONTROL_ERROR:
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CRUISE_CONTROL_ACTIVE_ID, UI_WARNING_STATUS_OFF);
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CRUISE_CONTROL_FAULT_ID, UI_WARNING_STATUS_ON);
                    break;

                default:
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CRUISE_CONTROL_ACTIVE_ID, UI_WARNING_STATUS_OFF);
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CRUISE_CONTROL_FAULT_ID, UI_WARNING_STATUS_OFF);
                    break;

            }
        }
    }
}



