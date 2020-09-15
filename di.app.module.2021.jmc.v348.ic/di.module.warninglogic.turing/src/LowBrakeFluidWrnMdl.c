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

#include "Rte_LowBrakeFluidWrnMdl.h"
#include "CmpLib.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(LowBrakeFluidWrnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */
#define cLowBrakeFluid_WARNING          (uint8)(2)

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static void fun_LowBrakeFluid_Warning_Process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BRAKE_FLUID_ID,UI_WARNING_STATUS_OFF);
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
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BRAKE_FLUID_ID,UI_WARNING_STATUS_OFF);
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
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BRAKE_FLUID_ID,UI_WARNING_STATUS_OFF);
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
    fun_LowBrakeFluid_Warning_Process();
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


static void fun_LowBrakeFluid_Warning_Process(void)
{
    uint8  fl_BCM_BrakeFluidSts_U8         = (uint8)0;

    uint8  fl_Msg313_NR_U8                 = (uint8)0;
    uint8  fl_Msg313_Tout_U8               = (uint8)0;

    Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_NR(&fl_Msg313_NR_U8);
    Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_Tout(&fl_Msg313_Tout_U8);
    Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_BrakeFluidSts(&fl_BCM_BrakeFluidSts_U8);

    if ( ((uint8)RTE_E_TIMEOUT == fl_Msg313_Tout_U8) || ((uint8)RTE_E_NEVER_RECEIVED == fl_Msg313_NR_U8) )
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BRAKE_FLUID_ID,UI_WARNING_STATUS_OFF);
    }
    else
    {
        if ( cLowBrakeFluid_WARNING == fl_BCM_BrakeFluidSts_U8 )
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BRAKE_FLUID_ID,UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BRAKE_FLUID_ID,UI_WARNING_STATUS_OFF);
        }

    }

}

