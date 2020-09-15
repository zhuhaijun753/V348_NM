//---------------------------------------------------------------------------------------------------------------------
//
// VISTEON CORPORATION CONFIDENTIAL
// ________________________________
//
// [2016] Visteon Corporation
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

#ifndef RTCCDD_C
#define RTCCDD_C

#include "Rte_RtcCdd.h"
#include "CmpLib.h"
#include "RtcCdd_MemMap.h"
#include "RtcMgrCdd.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(RtcCdd)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

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
    static boolean init = TRUE;
    if (FALSE != init)
    {
        Std_ReturnType Ret;
        Ret = RtcMgrCdd_Init();
        if (E_OK == Ret)
        {
            init = FALSE;
            return E_OK;
        }
        else
        {
            return Ret;
        }
    }
    else
    {
        return E_OK;
    }
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
    return E_OK; //RtcMgrCdd_DeInit();
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
    return RtcMgrCdd_Activate();
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
    return RtcMgrCdd_DeActivate();
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive(void)
{
    RtcMgrCdd_MainFunction();
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
    return E_NOT_OK; /// No diag support for now
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn(void)
{
    return E_NOT_OK;
}

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================

//=====================================================================================================================
//  TIRtcCdd
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/// @brief  See RtcMgrCdd
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcCdd_Impl_SetTime(const Rtc_TimeType *arg)
{
    Std_ReturnType RetVal;
    RetVal = RtcMgrCdd_SetTime(*arg);
    return RetVal;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  See RtcMgrCdd
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcCdd_Impl_GetTime(Rtc_TimeType *arg)
{
    Std_ReturnType RetVal;
    RetVal = RtcMgrCdd_GetTime(arg);
    return RetVal;
}

#endif
