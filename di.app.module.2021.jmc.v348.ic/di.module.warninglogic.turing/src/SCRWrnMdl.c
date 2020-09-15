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
#include "Rte_SCRWrnMdl.h"
#include "CmpLib.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(SCRWrnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

#define MASSAGE_TIME_INVALID          (uint8)0x3F
#define MASSAGE_UREALVL_INVALID       (uint8)0xFF
#define UREATXT_WARNING_VAR2          (uint8)0x2
#define UREATXT_WARNING_VAR3          (uint8)0x3
#define UREATXT_WARNING_VAR4          (uint8)0x4
#define UREATXT_WARNING_VAR5          (uint8)0x5
#define UREAQTY_OBDTXT_WARNING_VAR1   (uint8)0x1
#define UREAQTY_OBDTXT_WARNING_VAR2   (uint8)0x2
#define UREAQTY_OBDTXT_WARNING_VAR3   (uint8)0x3
#define SCR_TIMEOUT_2S                (uint8)20

typedef enum
{
    SCR_LOW_UREA_INDEX                  ,
    SCR_UREA_EXHAUSTED_1_INDEX          ,
    SCR_UREA_EXHAUSTED_2_INDEX          ,
    SCR_UREA_EXHAUSTED_3_INDEX          ,
    SCR_UREA_DISQUALIFICATION_1_INDEX   ,
    SCR_UREA_DISQUALIFICATION_2_INDEX   ,
    SCR_UREA_DISQUALIFICATION_3_INDEX   ,
    SCR_UREA_SYSTEM_FAILURE_1_INDEX     ,
    SCR_UREA_SYSTEM_FAILURE_2_INDEX     ,
    SCR_UREA_SYSTEM_FAILURE_3_INDEX     ,
    SCR_WARNING_TOTAL
}SCR_WARNING_STATUS_T;


static uint8 ScrWarningId_Array[SCR_WARNING_TOTAL] =
{
    WARN_SCR_LOW_UREA_IMMEDIATELY_ADDED_ID            ,
    WARN_SCR_UREA_EXHAUSTED_POWER_TO_REDUCE_ID        ,
    WARN_SCR_UREA_EXHAUSTED_SPEED_LIMIT_ID            ,
    WARN_SCR_UREA_EXHAUSTED_REF_USER_MANUAL_ID        ,
    WARN_SCR_UREA_DISQUALIFICATION_POWER_TO_REDUCE_ID ,
    WARN_SCR_UREA_DISQUALIFICATION_SPEED_LIMIT_ID     ,
    WARN_SCR_UREA_DISQUALIFICATION_REF_USER_MANUAL_ID ,
    WARN_SCR_UREA_SYSTEM_FAILURE_POWER_TO_REDUCE_ID   ,
    WARN_SCR_UREA_SYSTEM_FAILURE_SPEED_LIMIT_ID       ,
    WARN_SCR_UREA_SYSTEM_FAILURE_REF_USER_MANUAL_ID
};

typedef enum
{
    TROQHOUR_INDEX,
    TROQMIN_INDEX,
    SPDHOUR_INDEX,
    SPDMIN_INDEX,
    TIME_SIG_TOTAL
}SCR_TIME_SIG_INDEX_T;

typedef struct
{
  uint8   u8Count;
  boolean bFlag;
}SCR_MSGVAL_TIMEOUT_T;

static uint8 ScrWarning_Status[SCR_WARNING_TOTAL] = { 0 };

/* Description:Record the invalid value status of the time signal
   INdex |  Singal Information
     0   |  TroqLimtHour  info
     1   |  TroqLimtMin   info
     2   |  SpeedLimtHour info
     3   |  SpeedLimtMin  info
*/
static SCR_MSGVAL_TIMEOUT_T TimeValInfo[TIME_SIG_TOTAL] = {{0,FALSE},{0,FALSE},{0,FALSE},{0,FALSE}};

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static uint8 message_366_invalid(void);
static uint8 message_368_invalid(void);
static void  SCRWarning_process(void);
//static void SCRWrn_368_process(void);
static void  SCRWarningInit(void);
static void  SCRTimeInvalid_process(uint8 u8TroqLimtHour,uint8 u8TroqLimtMin,
    uint8 u8SpeedLimtHour,uint8 u8SpeedLimtMin);

//static void  SCRStsProcess(UI_WARNING_ID_T WarnIndex1, UI_WARNING_ID_T WarnIndex2,
//UI_WARNING_ID_T WarnIndex3, uint8 signalValue);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
  SCRWarningInit();
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
  SCRWarningInit();
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
  SCRWarningInit();
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
  SCRWarningInit();
  return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
/****************************************************************************
Function Name         : CmpActive

Description         : This function process the oilpressure TT ON/OFF for
            ISG /NON ISG Variant.

Invocation          : called in cmpActive.

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static Std_ReturnType CmpActive(void)
{
    SCRWarning_process();
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

/*********************************************************************************

  Function Name      :  SCRWarning_process (void)

  Description       :  SCRWarning_process

  Parameters      :  None

  Return Value      :  None

  Critical Section:  None

***********************************************************************************/
static void SCRWarning_process(void)
{
    uint8 u8OBDTxt                     = (uint8)0;
    uint8 u8UreaTxt                    = (uint8)0;
    uint8 u8UreaQty                    = (uint8)0;
    uint8 u8TroqLimtHour               = (uint8)0;
    uint8 u8TroqLimtMin                = (uint8)0;
    uint8 u8SpeedLimtHour              = (uint8)0;
    uint8 u8SpeedLimtMin               = (uint8)0;

    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDTxt(&u8OBDTxt);
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaTxt(&u8UreaTxt);
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaQty(&u8UreaQty);
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtHour(&u8TroqLimtHour);
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtMin(&u8TroqLimtMin); 
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtHour(&u8SpeedLimtHour);
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtMin(&u8SpeedLimtMin);

    SCRTimeInvalid_process(u8TroqLimtHour,u8TroqLimtMin,u8SpeedLimtHour,u8SpeedLimtMin);
    
    if ((uint8)FALSE == message_368_invalid())
    {       
        if (UREATXT_WARNING_VAR2 == u8UreaTxt)
        {
            ScrWarning_Status[SCR_LOW_UREA_INDEX]         = UI_WARNING_STATUS_ON;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_1_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_2_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_3_INDEX] = UI_WARNING_STATUS_OFF;
        }
        else if (UREATXT_WARNING_VAR3 == u8UreaTxt)
        {
            ScrWarning_Status[SCR_LOW_UREA_INDEX]         = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_1_INDEX] = UI_WARNING_STATUS_ON;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_2_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_3_INDEX] = UI_WARNING_STATUS_OFF;
        }
        else if (UREATXT_WARNING_VAR4 == u8UreaTxt)
        {
            ScrWarning_Status[SCR_LOW_UREA_INDEX]         = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_1_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_2_INDEX] = UI_WARNING_STATUS_ON;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_3_INDEX] = UI_WARNING_STATUS_OFF;
        }
        else if (UREATXT_WARNING_VAR5 == u8UreaTxt)
        {
            ScrWarning_Status[SCR_LOW_UREA_INDEX]         = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_1_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_2_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_3_INDEX] = UI_WARNING_STATUS_ON;
        }
        else
        {
            ScrWarning_Status[SCR_LOW_UREA_INDEX]         = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_1_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_2_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_EXHAUSTED_3_INDEX] = UI_WARNING_STATUS_OFF;
        }


        if (UREAQTY_OBDTXT_WARNING_VAR1 == u8UreaQty)
        {
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_1_INDEX] = UI_WARNING_STATUS_ON;
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_2_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_3_INDEX] = UI_WARNING_STATUS_OFF;
        }
        else if (UREAQTY_OBDTXT_WARNING_VAR2 == u8UreaQty)
        {
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_1_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_2_INDEX] = UI_WARNING_STATUS_ON;
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_3_INDEX] = UI_WARNING_STATUS_OFF;
        }
        else if (UREAQTY_OBDTXT_WARNING_VAR3 == u8UreaQty)
        {
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_1_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_2_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_3_INDEX] = UI_WARNING_STATUS_ON;
        }
        else
        {
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_1_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_2_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_DISQUALIFICATION_3_INDEX] = UI_WARNING_STATUS_OFF;
        }
        
    }
    else
    {
        ScrWarning_Status[SCR_LOW_UREA_INDEX]                = UI_WARNING_STATUS_OFF;
        ScrWarning_Status[SCR_UREA_EXHAUSTED_1_INDEX]        = UI_WARNING_STATUS_OFF;
        ScrWarning_Status[SCR_UREA_EXHAUSTED_2_INDEX]        = UI_WARNING_STATUS_OFF;
        ScrWarning_Status[SCR_UREA_EXHAUSTED_3_INDEX]        = UI_WARNING_STATUS_OFF;
        ScrWarning_Status[SCR_UREA_DISQUALIFICATION_1_INDEX] = UI_WARNING_STATUS_OFF;
        ScrWarning_Status[SCR_UREA_DISQUALIFICATION_2_INDEX] = UI_WARNING_STATUS_OFF;
        ScrWarning_Status[SCR_UREA_DISQUALIFICATION_3_INDEX] = UI_WARNING_STATUS_OFF;
    }
    

    if ( ((uint8)FALSE == message_366_invalid()) && ((uint8)FALSE == message_368_invalid()))
    {        
        if (UREAQTY_OBDTXT_WARNING_VAR1 == u8OBDTxt)
        {
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_1_INDEX] = UI_WARNING_STATUS_ON;
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_2_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_3_INDEX] = UI_WARNING_STATUS_OFF;
        }
        else if (UREAQTY_OBDTXT_WARNING_VAR2 == u8OBDTxt)
        {
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_1_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_2_INDEX] = UI_WARNING_STATUS_ON;
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_3_INDEX] = UI_WARNING_STATUS_OFF;
        }
        else if (UREAQTY_OBDTXT_WARNING_VAR3 == u8OBDTxt)
        {
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_1_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_2_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_3_INDEX] = UI_WARNING_STATUS_ON;
        }
        else
        {
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_1_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_2_INDEX] = UI_WARNING_STATUS_OFF;
            ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_3_INDEX] = UI_WARNING_STATUS_OFF;
        }
    }
    else
    {
        ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_1_INDEX] = UI_WARNING_STATUS_OFF;
        ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_2_INDEX] = UI_WARNING_STATUS_OFF;
        ScrWarning_Status[SCR_UREA_SYSTEM_FAILURE_3_INDEX] = UI_WARNING_STATUS_OFF;
    }
    
    Rte_Write_ppSR_SCRWrnMdl_TimeData_TroqLimtHour(u8TroqLimtHour);
    Rte_Write_ppSR_SCRWrnMdl_TimeData_TroqLimtMin(u8TroqLimtMin);
    Rte_Write_ppSR_SCRWrnMdl_TimeData_SpeedLimtHour(u8SpeedLimtHour);
    Rte_Write_ppSR_SCRWrnMdl_TimeData_SpeedLimtMin(u8SpeedLimtMin);

    
    for (uint8 i = 0; i < SCR_WARNING_TOTAL; i++)
    {
      Rte_Call_tiCS_WarningCtrl_Set_Operation(ScrWarningId_Array[i], ScrWarning_Status[i]);
    }

}



static void SCRWarningInit(void)
{

  for (uint8 i = 0; i < SCR_WARNING_TOTAL; i++)
  {
    Rte_Call_tiCS_WarningCtrl_Set_Operation(ScrWarningId_Array[i], UI_WARNING_STATUS_OFF);
    ScrWarning_Status[i] = UI_WARNING_STATUS_OFF;
  }

}


static uint8 message_366_invalid(void)
{

  uint8 u8Msg366NR           = (uint8)0;
  uint8 u8Msg366Tout         = (uint8)0;

  Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(&u8Msg366NR);
  Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(&u8Msg366Tout);

  if (((uint8)RTE_E_TIMEOUT == u8Msg366Tout) || ((uint8)RTE_E_NEVER_RECEIVED == u8Msg366NR))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }

}


static uint8 message_368_invalid(void)
{

  uint8 u8Msg368NR           = (uint8)0;
  uint8 u8Msg368Tout         = (uint8)0;

  Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_NR(&u8Msg368NR);
  Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_Tout(&u8Msg368Tout);

  if (((uint8)RTE_E_TIMEOUT == u8Msg368Tout) || ((uint8)RTE_E_NEVER_RECEIVED == u8Msg368NR))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
  
}

static void  SCRTimeInvalid_process(uint8 u8TroqLimtHour,uint8 u8TroqLimtMin,
    uint8 u8SpeedLimtHour,uint8 u8SpeedLimtMin)
{   
    /* #0: TroqLimtHour value is invalid or not*/
    if (MASSAGE_TIME_INVALID == u8TroqLimtHour)
    {
        TimeValInfo[TROQHOUR_INDEX].u8Count ++;
        if (SCR_TIMEOUT_2S < TimeValInfo[TROQHOUR_INDEX].u8Count)
        {
            if((boolean)FALSE == TimeValInfo[TROQHOUR_INDEX].bFlag)
            {
                TimeValInfo[TROQHOUR_INDEX].bFlag = (boolean)TRUE;
            }
        }
    }
    else
    {
        TimeValInfo[TROQHOUR_INDEX].bFlag = (boolean)FALSE;
        TimeValInfo[TROQHOUR_INDEX].u8Count = (uint8)0;
    }

    /* #1: TroqLimtMin value is invalid or not*/
    if (MASSAGE_TIME_INVALID == u8TroqLimtMin)
    {
        TimeValInfo[TROQMIN_INDEX].u8Count ++;
        if (SCR_TIMEOUT_2S < TimeValInfo[TROQMIN_INDEX].u8Count)
        {
            if ((boolean)FALSE == TimeValInfo[TROQMIN_INDEX].bFlag)
            {
                TimeValInfo[TROQMIN_INDEX].bFlag = (boolean)TRUE;
            }
        }
    }
    else
    {
        TimeValInfo[TROQMIN_INDEX].bFlag = (boolean)FALSE;
        TimeValInfo[TROQMIN_INDEX].u8Count = (uint8)0;
    } 

    /* #2: SpeedLimtHour value is invalid or not*/
    if (MASSAGE_TIME_INVALID == u8SpeedLimtHour)
    {
        TimeValInfo[SPDHOUR_INDEX].u8Count ++;
        if (SCR_TIMEOUT_2S < TimeValInfo[SPDHOUR_INDEX].u8Count)
        {
            if ((boolean)FALSE == TimeValInfo[SPDHOUR_INDEX].bFlag)
            {
                TimeValInfo[SPDHOUR_INDEX].bFlag = (boolean)TRUE;
            }
        }
    }
    else
    {
        TimeValInfo[SPDHOUR_INDEX].bFlag = (boolean)FALSE;
        TimeValInfo[SPDHOUR_INDEX].u8Count = (uint8)0;
    }   

    /* #3: SpeedLimtMin value is invalid or not*/
    if (MASSAGE_TIME_INVALID == u8SpeedLimtMin)
    {
        TimeValInfo[SPDMIN_INDEX].u8Count ++;
        if (SCR_TIMEOUT_2S < TimeValInfo[SPDMIN_INDEX].u8Count)
        {
            if ((boolean)FALSE == TimeValInfo[SPDMIN_INDEX].bFlag)
            {
                TimeValInfo[SPDMIN_INDEX].bFlag = (boolean)TRUE;
            }
        }
    }
    else
    {
        TimeValInfo[SPDMIN_INDEX].bFlag = (boolean)FALSE;
        TimeValInfo[SPDMIN_INDEX].u8Count = (uint8)0;
    } 
}


void Get_ScrTimeMsgSts(uint8 u8Index,boolean * pSts)
{
    switch(u8Index)
    {
    case TROQHOUR_INDEX:
        *pSts = TimeValInfo[TROQHOUR_INDEX].bFlag;
     break;
    case TROQMIN_INDEX:
        *pSts = TimeValInfo[TROQMIN_INDEX].bFlag;
     break;
    case SPDHOUR_INDEX:
        *pSts = TimeValInfo[SPDHOUR_INDEX].bFlag;
     break;
    case SPDMIN_INDEX:
        *pSts = TimeValInfo[SPDMIN_INDEX].bFlag;
     break;
    default:
     break;
    }
}



#if 0
static void SCRWrn_368_process(void)
{
  uint8 u8UreaTxt = 0;
  uint8 u8UreaQty = 0;
  uint8 u8AddUreaWarning = 0;
  uint8 u8UreaSysWarning = 0;
  uint8 u8NoxWarning = 0;
  uint16 u16UreaDTE = 0;
  Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaTxt(&u8UreaTxt);
  Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaQty(&u8UreaQty);
  Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_AddUreaWarning(&u8AddUreaWarning);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaSysWarning(&u8UreaSysWarning);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_NoxWarning(&u8NoxWarning);
	
  Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaDTE(&u16UreaDTE);


  SCRStsProcess(WARN_SCR_NOX_WARNING1, WARN_SCR_NOX_WARNING2,
    WARN_SCR_NOX_WARNING3, u8NoxWarning);
  SCRStsProcess(WARN_UREA_SUPPLY_WARNING1, WARN_UREA_SUPPLY_WARNING2,
    WARN_UREA_SUPPLY_WARNING3, u8UreaSysWarning);
  SCRStsProcess(WARN_UREA_ABNORMAL_CONSUMPTION_WARNING1,
    WARN_UREA_ABNORMAL_CONSUMPTION_WARNING2,
    WARN_UREA_ABNORMAL_CONSUMPTION_WARNING3, u8AddUreaWarning);
  SCRStsProcess(WARN_UREA_QUALITY_WARNING1, WARN_UREA_QUALITY_WARNING2,
    WARN_UREA_QUALITY_WARNING3, u8UreaQty);
  if (5 == u8UreaTxt && MASSAGE_INVALID >= u16UreaDTE)
  {
    ScrWarningStatus[WARN_LOW_UREA_WARNING1] = UI_WARNING_STATUS_ON;
    ScrWarningStatus[WARN_LOW_UREA_WARNING2] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING3] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING4] = UI_WARNING_STATUS_OFF;
  }
  else if (4 == u8UreaTxt && MASSAGE_INVALID >= u16UreaDTE)
  {
    ScrWarningStatus[WARN_LOW_UREA_WARNING1] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING2] = UI_WARNING_STATUS_ON;
    ScrWarningStatus[WARN_LOW_UREA_WARNING3] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING4] = UI_WARNING_STATUS_OFF;
  }
  else if (3 == u8UreaTxt && MASSAGE_INVALID >= u16UreaDTE)
  {
    ScrWarningStatus[WARN_LOW_UREA_WARNING1] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING2] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING3] = UI_WARNING_STATUS_ON;
    ScrWarningStatus[WARN_LOW_UREA_WARNING4] = UI_WARNING_STATUS_OFF;
  }
  else if (2 == u8UreaTxt && MASSAGE_INVALID >= u16UreaDTE)
  {
    ScrWarningStatus[WARN_LOW_UREA_WARNING1] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING2] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING3] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING4] = UI_WARNING_STATUS_ON;
  }
  else
  {
    ScrWarningStatus[WARN_LOW_UREA_WARNING1] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING2] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING3] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WARN_LOW_UREA_WARNING4] = UI_WARNING_STATUS_OFF;
  }
}

static void SCRStsProcess(UI_WARNING_ID_T WarnIndex1, UI_WARNING_ID_T WarnIndex2,
  UI_WARNING_ID_T WarnIndex3, uint8 signalValue)
{
  /*
  uint16 u16UreaDTE = 0;
  Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaDTE(&u16UreaDTE);
  Rte_Write_ppSR_SCRWrnMdl_GdtCtrl_State_GdtCtrl_State(u16UreaDTE);

  if (3 == signalValue && MASSAGE_INVALID >= u16UreaDTE)
  {
    ScrWarningStatus[WarnIndex1] = UI_WARNING_STATUS_ON;
    ScrWarningStatus[WarnIndex2] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WarnIndex3] = UI_WARNING_STATUS_OFF;
  }
  else if (2 == signalValue && MASSAGE_INVALID >= u16UreaDTE)
  {
    ScrWarningStatus[WarnIndex1] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WarnIndex2] = UI_WARNING_STATUS_ON;
    ScrWarningStatus[WarnIndex3] = UI_WARNING_STATUS_OFF;
  }
  else if (1 == signalValue && MASSAGE_INVALID >= u16UreaDTE)
  {
    ScrWarningStatus[WarnIndex1] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WarnIndex2] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WarnIndex3] = UI_WARNING_STATUS_ON;
  }
  else
  {
    ScrWarningStatus[WarnIndex1] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WarnIndex2] = UI_WARNING_STATUS_OFF;
    ScrWarningStatus[WarnIndex3] = UI_WARNING_STATUS_OFF;
  }
  */
}

#endif

