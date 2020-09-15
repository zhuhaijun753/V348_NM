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
#include "Rte_CDPFailureWrnMdl.h"
#include "CmpLib.h"
//#include "peps_wrn_cfg.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(CDPFailureWrnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8 Invalid = 0;

#define RPM_300  (300u)
#define RPM_200  (200u)

#define DELAY_3000MS (3000u)

#define CDP_NOT_AVAILABLE_1           ((uint8)0x1)

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
//static void InitLBFWarning(void);
//static void InitLOPWarning(void);
//static void InitCoolantTmpWarning(void);
//static void InitOutSideTmpWarning(void);
//static void LowBrakeFluidWarningProcess(void);
//static void LowOilPressureWarningProcess(void);
//static void OutSideTmpWarningProcess(void);
//static void CoolantTmpWarningProcess(void);
//static boolean Message_10B_Invalid(void);
//static boolean Message_313_Invalid(void);
static void ClearCDPWarning(void);
static void CDP_Warning_Process(void);
static uint8 message_0x243_invalid(void);

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
  //InitLBFWarning();
  //LowOilPressureWarningProcess();
  //InitCoolantTmpWarning();
  //InitOutSideTmpWarning();
  ClearCDPWarning();
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
  //InitLBFWarning();
  //InitLOPWarning();
  //InitCoolantTmpWarning();
  //InitOutSideTmpWarning();
  ClearCDPWarning();
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
  //InitLBFWarning();
  //InitLOPWarning();
  //InitCoolantTmpWarning();
  //InitOutSideTmpWarning();
  ClearCDPWarning();
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
  //InitLBFWarning();
  //InitLOPWarning();
  //InitCoolantTmpWarning();
  //InitOutSideTmpWarning();
  ClearCDPWarning();
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
  //LowBrakeFluidWarningProcess();
  //LowOilPressureWarningProcess();
  //CoolantTmpWarningProcess();
  //OutSideTmpWarningProcess();
  CDP_Warning_Process();
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

static void ClearCDPWarning(void)
{
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CDP_FAILURE_ID, UI_WARNING_STATUS_OFF);
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

static void CDP_Warning_Process(void)
{
    uint8 fl_CdpAvailable_U8 = 0;
    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_CdpAvailable(&fl_CdpAvailable_U8);
    if ((uint8)TRUE == message_0x243_invalid())
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CDP_FAILURE_ID, UI_WARNING_STATUS_OFF);
    }
    else
    {
        if (CDP_NOT_AVAILABLE_1 == fl_CdpAvailable_U8)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CDP_FAILURE_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CDP_FAILURE_ID, UI_WARNING_STATUS_OFF);
        }
    }
}

/*
static void InitLBFWarning(void)
{
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BRAKE_FLUID_ID,UI_WARNING_STATUS_OFF);
}
*/
/*
static void InitLOPWarning(void)
{
  Rte_Call_rptimer_TmExt_Timer_Stop(eTimerLOPDelay3s);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_OIL_PRESSURE_ID,UI_WARNING_STATUS_OFF);
}
*/
/*
static void InitCoolantTmpWarning(void)
{
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_HIGH_COOLANT_TEMPERATURE_ID,UI_WARNING_STATUS_OFF);
}
*/
/*
static void InitOutSideTmpWarning(void)
{
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVE_CAREFULLY_ID,UI_WARNING_STATUS_OFF);
}*/
/*
static void LowBrakeFluidWarningProcess(void)
{
  uint8 BrakeFluiSts = 0;
  Rte_Read_CDPFailureWrnMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_BrakeFluidSts(&BrakeFluiSts);
  if(2 == BrakeFluiSts && TRUE != Message_313_Invalid())
  {
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BRAKE_FLUID_ID,UI_WARNING_STATUS_ON);
  }
  else
  {
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BRAKE_FLUID_ID,UI_WARNING_STATUS_OFF);
  }
}
*/
/*
static void LowOilPressureWarningProcess(void)
{
  uint8 IGNState = eIGN_OFF;
  uint32 EngSpeed = 0;
  uint8 PowerStage = 0;
  boolean SignalInvalid = FALSE;
  boolean TimerExpired = FALSE;
  boolean TimerStarted = FALSE;
  SignalInvalid = Message_10B_Invalid();
  Rte_Read_CDPFailureWrnMdl_rpIgnState_IGNState(&IGNState);
  Rte_Read_CDPFailureWrnMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&EngSpeed);
  //need IF to read HW input
  if(eIGN_RUN == IGNState)
  {
    if(TRUE != SignalInvalid)
    {
      if(EngSpeed > RPM_300 && 0 == PowerStage)
      {
        Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerLOPDelay3s,&TimerStarted);
        if(FALSE == TimerStarted)
        {
          Rte_Call_rptimer_TmExt_Timer_Start(eTimerLOPDelay3s);
        }
      }

      Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerLOPDelay3s,DELAY_3000MS, &TimerExpired);
      if(FALSE !=TimerExpired)
      {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_OIL_PRESSURE_ID,UI_WARNING_STATUS_ON);
      }
      
      if(EngSpeed< RPM_200 || 1 == PowerStage )
      {
        InitLOPWarning();
      }
    }
    else
    {
      InitLOPWarning();
    }
  }
  else
  {
    InitLOPWarning();
  }
}
*//*
static void OutSideTmpWarningProcess(void)
{
  sint16 OutsideTemp = 0;
  Rte_Read_CDPFailureWrnMdl_rpSR_AmbientMdl_Output_OutsideTempValue(&OutsideTemp);
  if(OutsideTemp >= 3 )
  {
    InitOutSideTmpWarning();
  }
  else if(OutsideTemp < 0)
  {
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVE_CAREFULLY_ID,UI_WARNING_STATUS_ON);
  }
  else
  {
    ; // Do nothing
  }
  
}*/
/*
static void CoolantTmpWarningProcess(void)
{
  boolean TemperatureSts = FALSE;
  Rte_Read_CDPFailureWrnMdl_rpiSR_WaterTempHigh_STATUS(&TemperatureSts);
  if(TemperatureSts == TRUE)
  {
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_HIGH_COOLANT_TEMPERATURE_ID,UI_WARNING_STATUS_ON);
  }
  else
  {
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_HIGH_COOLANT_TEMPERATURE_ID,UI_WARNING_STATUS_OFF);
  }
}
*//*
static boolean Message_313_Invalid(void)
{
  uint8 u8MsgNR = 0;
  uint8 u8MsgTout = 0;

  Rte_Read_CDPFailureWrnMdl_rpSR_CANMSG_BCM_0x313_ComIn_NR(&u8MsgNR);
  Rte_Read_CDPFailureWrnMdl_rpSR_CANMSG_BCM_0x313_ComIn_Tout(&u8MsgTout);
  if ((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
  {
    return TRUE;
  }
  else 
  {
    return FALSE;
  }
  
}

static boolean Message_10B_Invalid(void)
{
  uint8 u8MsgNR = 0;
  uint8 u8MsgTout = 0;
  
  Rte_Read_CDPFailureWrnMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&u8MsgNR);
  Rte_Read_CDPFailureWrnMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&u8MsgTout);
  if ((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
  {
    return TRUE;
  }
  else 
  {
    return FALSE;
  }
}*/

