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
#include "Rte_ResetTripWrnMdl.h"
#include "hmi_menu.h"

#include "CmpLib.h"
//#include "peps_wrn_cfg.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(ResetTripWrnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define cWRN_RSTTRP_ONEBAR_MAX		((uint8)0x6)//one bar disp 6X100ms=600ms, for 5 bars: 3s=600msX5=3000ms

typedef enum
{
  RESETTRIP_WAITT_OK_STS = 0,
  RESETTRIP_SHOW_OK_STS,
  RESETTRIP_SHOW_COMPLETE_STS
}RestTripProcessStateMachine_t;

typedef enum
{
  RESETTRIP_POPHIDE = 0,
  RESETTRIP_POPSHOW,
}RestTripWindowSts_t;

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

//static uint8 l_BtnInDetect_St_u8 = UI_WARNING_STATUS_OFF;
//static uint8 l_ClrCompl_St_u8 = UI_WARNING_STATUS_OFF;
static uint8 u8PrgBarNum = 0;
Boolean bTripA_RstCmd = FALSE;
static Boolean bTripB_RstCmd = FALSE;
static UI_WARNING_STATE_T LastResetMaintenanceMileageWrnSts = UI_WARNING_STATE_NONE;

static Boolean LastMaintenanceMileageResetFlag = FALSE;
static void ResetTripWrnInitialize(void);
static void MainTenanceWrnInit(void);
static void ResetTripWrnProcess(void);
static void MainTenanceWrnProcess(void);
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
  ResetTripWrnInitialize();
  MainTenanceWrnInit();
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
  ResetTripWrnInitialize();
  MainTenanceWrnInit();
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
  ResetTripWrnInitialize();
  MainTenanceWrnInit();
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
  ResetTripWrnInitialize();
  MainTenanceWrnInit();

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
  ResetTripWrnProcess();
  MainTenanceWrnProcess();
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




static void ResetTripWrnProcess()
{
  static RestTripProcessStateMachine_t WindowStatMachine = RESETTRIP_WAITT_OK_STS;
  static uint8 u83STimer = 0;
  RestTripWindowSts_t  OKWindow = RESETTRIP_POPHIDE;
  uint8  CompleteWindow = RESETTRIP_POPHIDE;
  Boolean bTripA_BtnIn = FALSE;
  EBatteryState eBatSt = 0;
  Rte_Read_rpSR_ResetTripWrnMdl_TripResetActive_TripAResetActive(&bTripA_BtnIn);
  Rte_Read_rpBattState_BatteryState(&eBatSt);
  if (eBatteryState_Normal == eBatSt)
  {
    switch (WindowStatMachine)
    {
    case RESETTRIP_WAITT_OK_STS:
      u8PrgBarNum = 0;
      u83STimer = 0;
      if (bTripA_BtnIn)
      {
        WindowStatMachine = RESETTRIP_SHOW_OK_STS;
        OKWindow = RESETTRIP_POPSHOW;
        u83STimer++;
      }
      else
      {
        WindowStatMachine = RESETTRIP_WAITT_OK_STS;
        OKWindow = RESETTRIP_POPHIDE;
        u8PrgBarNum = 0;
      }
      //bTripA_RstCmd = FALSE;
      break;
    case RESETTRIP_SHOW_OK_STS:
      if (bTripA_BtnIn && u83STimer < 30)
      {
        u83STimer++;
        WindowStatMachine = RESETTRIP_SHOW_OK_STS;
        OKWindow = RESETTRIP_POPSHOW;
        if (u83STimer % cWRN_RSTTRP_ONEBAR_MAX == 0 && u83STimer != 0)
        {
          u8PrgBarNum++;
        }
        if (u8PrgBarNum >= 5)
        {
          u8PrgBarNum = 5;
          bTripA_RstCmd = TRUE;
        }
      }
      else if (bTripA_BtnIn && u83STimer >= 30)
      {
        WindowStatMachine = RESETTRIP_SHOW_COMPLETE_STS;
        OKWindow = RESETTRIP_POPHIDE;
        //SET A VARIABLE FOR 2ND pop
        u83STimer = 0;
        CompleteWindow = RESETTRIP_POPSHOW;
      }
      else if (!bTripA_BtnIn)
      {
        WindowStatMachine = RESETTRIP_WAITT_OK_STS;
        OKWindow = RESETTRIP_POPHIDE;
        u83STimer = 0;
        u8PrgBarNum = 0;
      }
      else
      {
        //impossible branch
      }
      break;
    case RESETTRIP_SHOW_COMPLETE_STS:
      u83STimer++;
      if (30 <= u83STimer)
      {
        if (!bTripA_BtnIn)//3s后松开按键
        {
          WindowStatMachine = RESETTRIP_WAITT_OK_STS;//Restart 
          u83STimer = 30;

        }
        else//3s后还按着
        {
          u83STimer = 30;
          CompleteWindow = RESETTRIP_POPHIDE;
        }
      }
      else
      {
        CompleteWindow = RESETTRIP_POPSHOW;
      }
      break;
    }
  }
  else
  {
    WindowStatMachine = RESETTRIP_WAITT_OK_STS;
    u83STimer = 0;
    u8PrgBarNum = 0;
  }
  Rte_Write_ppSR_ResetTripWrnMdl_ProgressBarSts_ProgressBarSts(u8PrgBarNum);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_RESET_TRIPMETER_WARNING_ID, OKWindow);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_RESET_TRIPMETER_COMPLETE_WARNING_ID, CompleteWindow);
  Rte_Write_ppSR_ResetTripWrnMdl_ResetTripInstruc_ResetTripAInstruc(bTripA_RstCmd);
}

static void ResetTripWrnInitialize(void)
{
  
 // l_BtnInDetect_St_u8 = UI_WARNING_STATUS_OFF;
 // l_ClrCompl_St_u8 = UI_WARNING_STATUS_OFF;
  u8PrgBarNum = 0;
  bTripA_RstCmd = FALSE;
  bTripB_RstCmd = FALSE;
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_RESET_TRIPMETER_WARNING_ID, l_BtnInDetect_St_u8);
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_RESET_TRIPMETER_COMPLETE_WARNING_ID, l_ClrCompl_St_u8);
  Rte_Write_ppSR_ResetTripWrnMdl_ProgressBarSts_ProgressBarSts(u8PrgBarNum);
  Rte_Write_ppSR_ResetTripWrnMdl_ResetTripInstruc_ResetTripAInstruc(bTripA_RstCmd);
  Rte_Write_ppSR_ResetTripWrnMdl_ResetTripInstruc_ResetTripBInstruc(bTripB_RstCmd);
}
static void MainTenanceWrnInit(void)
{
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_REMAIN_MAINTENANCE_WARNING_ID, UI_WARNING_STATUS_OFF);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_MAINTAIN_IMMEDIATELY_WARNING_ID, UI_WARNING_STATUS_OFF);
  LastMaintenanceMileageResetFlag = FALSE;
  LastResetMaintenanceMileageWrnSts = UI_WARNING_STATE_NONE;
}

static void MainTenanceWrnProcess(void)
{
  uint16 u16MaintenanceKm = 0;
  UI_WARNING_STATE_T ResetMaintenanceMileageWrnSts = UI_WARNING_STATE_NONE;
  Rte_Read_rpsr_Maintenance_mileage_distance_value(&u16MaintenanceKm);
  Boolean MaintenanceMileageResetFlag = FALSE;
  //Rte_Read_ResetTripWrnMdl_rpsr_Maintenance_mileage_distance_value(&MaintenanceMileageResetFlag);
  MaintenanceMileageResetFlag = hmi_GetMaintenanceMileageResetFlag();

  if (u16MaintenanceKm <= (uint16)0)
  {
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_REMAIN_MAINTENANCE_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_MAINTAIN_IMMEDIATELY_WARNING_ID, UI_WARNING_STATUS_ON);
  }
  else if (u16MaintenanceKm <= (uint16)500)
  {
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_REMAIN_MAINTENANCE_WARNING_ID, UI_WARNING_STATUS_ON);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_MAINTAIN_IMMEDIATELY_WARNING_ID, UI_WARNING_STATUS_OFF);
    CWarningCtrl_APP_set_MainTenace_warning_XXXKM(u16MaintenanceKm);
  }
  else
  {
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_REMAIN_MAINTENANCE_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_MAINTAIN_IMMEDIATELY_WARNING_ID, UI_WARNING_STATUS_OFF);
  }



  if (FALSE != MaintenanceMileageResetFlag && FALSE == LastMaintenanceMileageResetFlag)
  {
   // Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_MAINTENANCE_MILEAGE_RESET_SUCCESSFULLY_ID, UI_WARNING_STATUS_ON);
  }

  //hmi_menu modle runs too fast, we can only set MaintenanceMileageResetFlag here, or this warning can not be 
  // displayed for MaintenanceMileageResetFlag is set to FALSE before ResetTripWrnMdl read it
  //But we cat't keep sending FALSE or this warning can only displaye one time.
  if (UI_WARNING_STATE_TIMEOUT == ResetMaintenanceMileageWrnSts && UI_WARNING_STATE_TIMEOUT != LastResetMaintenanceMileageWrnSts)
  {
    //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_MAINTENANCE_MILEAGE_RESET_SUCCESSFULLY_ID, UI_WARNING_STATUS_OFF);
    hmi_SetMaintenanceMileageResetFlag(FALSE);
  }

  LastResetMaintenanceMileageWrnSts = ResetMaintenanceMileageWrnSts;
  LastMaintenanceMileageResetFlag = MaintenanceMileageResetFlag;
}


