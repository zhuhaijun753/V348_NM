

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
#include "Rte_NotInPWrnMdl.h"


#include "CmpLib.h"
//#include "peps_wrn_cfg.h"
#include "WarningCtrl.h"
#include "string.h"
/// @brief  Instance of life-cycle state-machine
//=====================================================================================================================
CMPLIB_INSTANCE(NotInPWrnMdl)

//  CONSTANTS & TYPES
//=====================================================================================================================
#define MSG_GEAR_P              ((uint8)0xd)
#define MSG_GEAR_INVALID        ((uint8)0xF)
#define SPEED_LIMIT             (3u)
#define SPEED_FACTOR            (100u)
#define TACH_SPEED_LIMIT        (450)
#define ENGINE_SPEED_FACTOR     (100)
#define INVALID_SPEED           (4095)
typedef enum
{
  PEPS_OFF = 0x0,
  PEPS_ACC,
  PEPS_ON,
  PEPS_START,
}PEPSPowerMode_t;

typedef enum
{
  GW_TCU_GearInfo_Power,
  BCM_Door,
  ABS,
  Max_Signal_Num,
}InputSingnalTable_t;

typedef struct
{
  uint8 ToutStatus;
  uint8 NRStatus;
  boolean SignalLoseFlag;
  uint8 SignalStatusTimer;
  uint32 SignalValue;
}InputSignalData_t;
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
//static void NotInPMainlogicProcess(void);
//static boolean InvalidSignalFilterProcess(boolean *GearInvalidFlag,boolean *SpeedInvalidFlag);
//static boolean CheckWarningBSignalLose(void);

//static boolean CheckSpeedSignalInvalidProcess(void);
//static boolean CheckGearSignalInvalidProcess(void);

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 NointInP_A_IGON_WarningSts = 0;

//static uint8 NointInP_B_IGON_WarningSts = 0;

static uint8 SpeedInvalidTimer = 0;
static uint8 GearInvalidTimer = 0;

static InputSignalData_t InputData[Max_Signal_Num] = {0};
//static uint8 NointInP_B_HoldTimer = 0;
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
  NointInP_A_IGON_WarningSts  = UI_WARNING_STATUS_OFF;
  //NointInP_B_IGON_WarningSts = UI_WARNING_STATUS_OFF;
  //NointInP_B_HoldTimer = 0;
  SpeedInvalidTimer = 0;
  GearInvalidTimer = 0;
  memset(InputData,0,sizeof(InputData));
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_NOT_IN_P_A_IG_ON_ID, NointInP_A_IGON_WarningSts);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_NOT_IN_P_B_IG_ON_ID, UI_WARNING_STATUS_OFF);
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
  NointInP_A_IGON_WarningSts  = UI_WARNING_STATUS_OFF;
  //NointInP_B_IGON_WarningSts = UI_WARNING_STATUS_OFF;
  //NointInP_B_HoldTimer = 0;
  SpeedInvalidTimer = 0;
  GearInvalidTimer = 0;  
  memset(InputData,0,sizeof(InputData));
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_NOT_IN_P_A_IG_ON_ID, NointInP_A_IGON_WarningSts);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_NOT_IN_P_B_IG_ON_ID, UI_WARNING_STATUS_OFF);
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
  NointInP_A_IGON_WarningSts  = UI_WARNING_STATUS_OFF;
  //NointInP_B_IGON_WarningSts = UI_WARNING_STATUS_OFF;
  //NointInP_B_HoldTimer = 0;
  SpeedInvalidTimer = 0;
  GearInvalidTimer = 0;
  memset(InputData,0,sizeof(InputData));
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_NOT_IN_P_A_IG_ON_ID, UI_WARNING_STATUS_OFF);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_NOT_IN_P_B_IG_ON_ID, UI_WARNING_STATUS_OFF);
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
  NointInP_A_IGON_WarningSts  = UI_WARNING_STATUS_OFF;
  //NointInP_B_IGON_WarningSts = UI_WARNING_STATUS_OFF;
  //NointInP_B_HoldTimer = 0;
  SpeedInvalidTimer = 0;
  GearInvalidTimer = 0;
  memset(InputData,0,sizeof(InputData));
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_NOT_IN_P_A_IG_ON_ID, NointInP_A_IGON_WarningSts);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_NOT_IN_P_B_IG_ON_ID, UI_WARNING_STATUS_OFF);
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

  //NotInPMainlogicProcess();
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_NOT_IN_P_A_IG_ON_ID, NointInP_A_IGON_WarningSts);
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_NOT_IN_P_B_IG_ON_ID, UI_WARNING_STATUS_OFF);
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
#if 0
static void NotInPMainlogicProcess(void)
{
  uint32 SpeedU8 = 0;
  //Boolean KeepLastWarningStsFlag = FALSE;
  boolean SpeedInvalidFlag = FALSE;
  boolean GearInvalidFlag = FALSE;
  boolean SignalLoseFlag_A = FALSE;
  //boolean SignalLoseFlag_B = FALSE;

  Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_NR(&InputData[GW_TCU_GearInfo_Power].NRStatus);
  Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_Tout(&InputData[GW_TCU_GearInfo_Power].ToutStatus);
  Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear(&InputData[GW_TCU_GearInfo_Power].SignalValue);

  Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&InputData[BCM_Door].NRStatus);
  Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&InputData[BCM_Door].ToutStatus);
  Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&InputData[BCM_Door].SignalValue);

  Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&InputData[ABS].NRStatus);
  Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&InputData[ABS].ToutStatus);
  Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&InputData[ABS].SignalValue);
  Rte_Read_NotInPWrnMdl_rpSR_TISpdMdl_Send_DigitalValue_Value(&SpeedU8);

  //Need teset with Speed module
  SpeedU8 /= SPEED_FACTOR;

  SignalLoseFlag_A = InvalidSignalFilterProcess(&GearInvalidFlag, &SpeedInvalidFlag);

  if(SignalLoseFlag_A == TRUE)
  {
    NointInP_A_IGON_WarningSts = UI_WARNING_STATUS_OFF;
    SpeedInvalidTimer = 0;
  }
  else
  {
    if(0xD != InputData[GW_TCU_GearInfo_Power].SignalValue &&2 == InputData[BCM_Door].SignalValue 
      && SpeedU8 < 5 && SpeedInvalidFlag == FALSE && GearInvalidFlag == FALSE)
    {
      NointInP_A_IGON_WarningSts = UI_WARNING_STATUS_ON;
    }
    else
    {
      NointInP_A_IGON_WarningSts = UI_WARNING_STATUS_OFF;
    }
  }

  /*SignalLoseFlag_B = CheckWarningBSignalLose();
  if(SignalLoseFlag_B == TRUE)
  {
    NointInP_B_HoldTimer = 0;
    GearInvalidTimer = 0;
    NointInP_B_IGON_WarningSts = UI_WARNING_STATUS_OFF;
  }
  else
  {
    if(((PEPS_ACC == InputData[PEPS].SignalValue) || (PEPS_OFF == InputData[PEPS].SignalValue)) &&
       (0xD != InputData[GW_TCU_GearInfo_Power].SignalValue) && (SpeedInvalidFlag == FALSE) && (SpeedU8 < 5) &&
       (GearInvalidFlag == FALSE))
    {
      NointInP_B_IGON_WarningSts = UI_WARNING_STATUS_ON;
    }
    else
    {
      if(NointInP_B_HoldTimer < 30)
      {

      }
      else
      {
        NointInP_B_HoldTimer = 0;
        NointInP_B_IGON_WarningSts = UI_WARNING_STATUS_OFF;
      }
    }

    if(UI_WARNING_STATUS_ON == NointInP_B_IGON_WarningSts)
    {
      NointInP_B_HoldTimer++;
    }
    else
    {
      NointInP_B_HoldTimer = 0;
    }
  }*/
}



static boolean InvalidSignalFilterProcess(boolean *GearInvalidFlag,boolean *SpeedInvalidFlag)
{
  //TIMER const variable should be change according to task period
  uint8 SignalIndex = 0;
  //Signal miss
  boolean ret = FALSE;
  for(;SignalIndex < Max_Signal_Num ; SignalIndex++)
  {
    if(RTE_E_TIMEOUT == InputData[SignalIndex].ToutStatus || 
      RTE_E_NEVER_RECEIVED == InputData[SignalIndex].NRStatus)
    {
      InputData[SignalIndex].SignalValue = 0;
      InputData[SignalIndex].SignalLoseFlag = TRUE;
    }
    else
    {
      InputData[SignalIndex].SignalLoseFlag = FALSE;
    }
   ret |=  InputData[SignalIndex].SignalLoseFlag;  
  }
  
  if(RTE_E_TIMEOUT != InputData[GW_TCU_GearInfo_Power].ToutStatus && 
      RTE_E_NEVER_RECEIVED != InputData[GW_TCU_GearInfo_Power].NRStatus)
  {
    *GearInvalidFlag = CheckGearSignalInvalidProcess();
  }
  else
  {
    GearInvalidTimer = 0;
  }

  if(RTE_E_TIMEOUT != InputData[ABS].ToutStatus && 
    RTE_E_NEVER_RECEIVED != InputData[ABS].NRStatus)
  {
    *SpeedInvalidFlag = CheckSpeedSignalInvalidProcess();
  }
 return ret;
}


static boolean CheckSpeedSignalInvalidProcess(void)
{
  boolean ret = FALSE;
  if(INVALID_SPEED <= InputData[ABS].SignalValue)
  {
    if(SpeedInvalidTimer >=20)
    {
      SpeedInvalidTimer = 20;
      ret = TRUE;
    }
    else
    {

      ret = FALSE;
      SpeedInvalidTimer++;
    }
  }
  else
  {
    ret = FALSE;
    SpeedInvalidTimer = 0;
  }
  return ret;
}

static boolean CheckGearSignalInvalidProcess(void)
{
  boolean ret = FALSE;
  static uint8 LastGearValue = 0 ;
  if(0xF == InputData[GW_TCU_GearInfo_Power].SignalValue)
  {
    if(GearInvalidTimer >=20)
    {
      GearInvalidTimer = 20;
      ret = TRUE;
    }
    else
    {

      ret = FALSE;
      GearInvalidTimer++;
      InputData[GW_TCU_GearInfo_Power].SignalValue = LastGearValue;
    }
  }
  else
  {
    ret = FALSE;
    GearInvalidTimer = 0;
  }
  LastGearValue = InputData[GW_TCU_GearInfo_Power].SignalValue;
  return ret;
}

static boolean CheckWarningBSignalLose(void)
{
  uint8 SignalIndex = 0;
  //Signal miss
  boolean ret = FALSE;
  for(;SignalIndex < Max_Signal_Num ; SignalIndex++)
  {
    if(SignalIndex != BCM_Door)
    {
        if(RTE_E_TIMEOUT == InputData[SignalIndex].ToutStatus || 
          RTE_E_NEVER_RECEIVED == InputData[SignalIndex].NRStatus)
        {
          InputData[SignalIndex].SignalValue = 0;
          InputData[SignalIndex].SignalLoseFlag = TRUE;
        }
        else
        {
          InputData[SignalIndex].SignalLoseFlag = FALSE;
        }
       ret |=  InputData[SignalIndex].SignalLoseFlag;  
    }
  }
  return ret;
}
#endif

