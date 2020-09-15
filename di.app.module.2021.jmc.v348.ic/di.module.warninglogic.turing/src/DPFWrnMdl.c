

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
#include "Rte_DPFWrnMdl.h"
#include "CmpLib.h"
#include "WarningCtrl.h"
/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(DPFWrnMdl) 
//=====================================================================================================================

//  CONSTANTS & TYPES
//=====================================================================================================================
/*#define PROD_SPEC_GPF_V2_1_WARNING_LIST                      \
    {MSG_GPF_FULL,	WARN_GPF_FULL_WARNING_ID},               \
    {MSG_GPF_LIMIT,WARN_GPF_LIMIT_WARNING_ID}

#define PROD_SPEC_GPF_V2_1_WARNING_LIST_NUM       2

typedef struct 
{
  uint8           m_GPF_can_value;
  WARN_ID_INDEX   m_GPF_warn_id;
} GPF_WARN_S;


typedef enum 
{
  MSG_GPF_OK_1 = 0, 
  MSG_GPF_FULL = 1, 
  MSG_GPF_OK_2 = 2, 
  MSG_GPF_OK_3 = 3, 
  MSG_GPF_LIMIT = 4, 
  MSG_GPF_REGEN = 5, 
  MSG_GPF_OK_4 = 6, 
  MSG_GPF_OK_5 = 7, 
  MSG_GPF_SOOT = 8, 
} CAN_SIGNAL_VAL_T;
*/

#define DPF_REGENWRN_ONSHOW    (uint8)0x1
#define DPF_REGENWRN_NOTSHOW   (uint8)0x0

typedef enum
{
  MSG_DPF_NORMAL_OPERATION                    ,
  MSG_RNGOFF_LOAD                             ,
  MSG_RNGOFF_OVERLOAD                         ,
  MSG_RGNOFF_AtLimit                          ,
  MSG_DPF_OVERLIMIT                           ,
  MSG_DPF_CLN_EXHAUST_FILTER                  ,
  MSG_OCR_EXHFILTERCLEANNED                   ,
  MSG_OCR_EXHFILTERCLEAN_STOPPED              ,
  MSG_AUTO_EXHFILTERCLEANNED
}CAN_SIGNAL_VAL_T;

typedef enum
{
  DPF_OVERLOAD_STATUS_INDEX                    ,
  DPF_REACHED_LIMIT_STATUS_INDEX               ,
  DPF_MORE_THAN_LIMIT_STATUS_INDEX             ,
  DPF_CLEAN_STATUS_INDEX                       ,
  DPF_CLEANING_INTERRUPTED_STATUS_INDEX        ,
  DPF_WARNING_TOTAL
}DPF_WARNING_STATUS_T;

static uint8 DPFWarning_ID_Array[DPF_WARNING_TOTAL] =
{
  WARN_DPF_OVERLOAD_ID                         ,
  WARN_DPF_REACHED_LIMIT_ID                    ,
  WARN_DPF_MORE_THAN_LIMIT_ID                  ,
  WARN_DPF_CLEAN_ID                            ,
  WARN_DPF_CLEANING_INTERRUPTED_ID              
};


static uint8 DPFWrnStatus[DPF_WARNING_TOTAL]   = {0};
static uint8 DPF_RegenSts_U8 = 0;
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
/*static const GPF_WARN_S c_GPF_V2_1_warn_SA[PROD_SPEC_GPF_V2_1_WARNING_LIST_NUM] =
{
  PROD_SPEC_GPF_V2_1_WARNING_LIST
};*/

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 message_368_invalid(void);
static void DPF_Warning_init(void);
static void fun_DPF_Warning_Process(void);

//static void GPF_Warning_init(void);
//static void GPF_Warning_Process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
  DPF_Warning_init();
  /*GPF_Warning_init();*/
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
  DPF_Warning_init();
  /*GPF_Warning_init();*/
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
  DPF_Warning_init();
  /*GPF_Warning_init();*/
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
  DPF_Warning_init();
  /*GPF_Warning_init();*/
  return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------

/****************************************************************************
Function Name     	: CmpActive

Description         : This function process the oilpressure TT ON/OFF for
            ISG /NON ISG Variant.

Invocation          : called in cmpActive.

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static Std_ReturnType CmpActive(void)
{
  eIgnState IGN_state = eIGN_OFF;

  Rte_Read_rpIgnState_IGNState(&IGN_state);

  if (FALSE == message_368_invalid())
  {
    if (eIGN_RUN == IGN_state)
    {
      fun_DPF_Warning_Process();
      /*GPF_Warning_Process();*/
    }
    else 
    {
      DPF_Warning_init();
      /*GPF_Warning_init();*/
    }

  }
  else 
  {
    DPF_Warning_init();
    /*GPF_Warning_init();*/
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


/*********************************************************************************
    
  Function Name	  :  fun_DPF_Warning_Process (void)
  
  Description 	  : DPF Warning Process
  
  Parameters		  : None
  
  Return Value	  : None
  
  Critical Section  : None
    
***********************************************************************************/
static void fun_DPF_Warning_Process(void)
{
  uint8 fl_DPF_Warning_Sts_U8 = (uint8)MSG_DPF_NORMAL_OPERATION;
  uint8 fl_NotifySts_U8       = (uint8)UI_WARNING_STATE_NONE;
  Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq(&fl_DPF_Warning_Sts_U8);
  fl_NotifySts_U8 = CWarningCtrl_APP_get_notified_warning_status(WARN_DPF_REACHED_LIMIT_ID);

  if ( (uint8)MSG_RNGOFF_OVERLOAD == fl_DPF_Warning_Sts_U8 )
  {
    DPFWrnStatus[DPF_OVERLOAD_STATUS_INDEX]             = UI_WARNING_STATUS_ON;
  }
  else
  {
    DPFWrnStatus[DPF_OVERLOAD_STATUS_INDEX]             = UI_WARNING_STATUS_OFF;
  }
 
  if ( (uint8)MSG_RGNOFF_AtLimit == fl_DPF_Warning_Sts_U8 )
  {
    DPFWrnStatus[DPF_REACHED_LIMIT_STATUS_INDEX]        = UI_WARNING_STATUS_ON;
    if ((uint8)UI_WARNING_STATE_SHOW == fl_NotifySts_U8)
    {
        /*Warning DPF Regen is on showing*/
        DPF_RegenSts_U8 = DPF_REGENWRN_ONSHOW;  
        
    }
    else
    {
        DPF_RegenSts_U8 = DPF_REGENWRN_NOTSHOW;
    }
    Rte_Write_DPFWrnMdl_ppSR_DPFWrnMdl_GdtCtrl_State_DPF_ReGenSts(DPF_RegenSts_U8);
  }
  else
  {
    DPFWrnStatus[DPF_REACHED_LIMIT_STATUS_INDEX]        = UI_WARNING_STATUS_OFF;
  }

  if ( (uint8)MSG_DPF_OVERLIMIT == fl_DPF_Warning_Sts_U8)
  {
    DPFWrnStatus[DPF_MORE_THAN_LIMIT_STATUS_INDEX]      = UI_WARNING_STATUS_ON;
  }
  else
  {
    DPFWrnStatus[DPF_MORE_THAN_LIMIT_STATUS_INDEX]      = UI_WARNING_STATUS_OFF;
  }
  
  if ( (uint8)MSG_OCR_EXHFILTERCLEANNED == fl_DPF_Warning_Sts_U8)
  {
    DPFWrnStatus[DPF_CLEAN_STATUS_INDEX]                = UI_WARNING_STATUS_ON;
  }
  else
  {
    DPFWrnStatus[DPF_CLEAN_STATUS_INDEX]                = UI_WARNING_STATUS_OFF;
  }

  if ( (uint8)MSG_OCR_EXHFILTERCLEAN_STOPPED == fl_DPF_Warning_Sts_U8)
  {
    DPFWrnStatus[DPF_CLEANING_INTERRUPTED_STATUS_INDEX] = UI_WARNING_STATUS_ON;
  }
  else
  {
    DPFWrnStatus[DPF_CLEANING_INTERRUPTED_STATUS_INDEX] = UI_WARNING_STATUS_OFF;
  }
  
  for (uint8 i = 0; i < DPF_WARNING_TOTAL; i ++)
  {
    Rte_Call_tiCS_WarningCtrl_Set_Operation(DPFWarning_ID_Array[i],DPFWrnStatus[i]);
  }
}

/*  
  uint8 DPF_Warning_status_u8 = 0;
  static uint8 DPF_Warning_last_status_u8 = 0;

  Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_GasPrtcWarn_G_Rq(&DPF_Warning_status_u8);

  if (0x1 <= DPF_Warning_status_u8 && 0x8 >= DPF_Warning_status_u8)
  {
    if (DPF_Warning_last_status_u8 != DPF_Warning_status_u8 && 0 != DPF_Warning_last_status_u8)
    {
      Rte_Call_tiCS_WarningCtrl_Set_Operation(WARN_DPF_WARNING_ID, UI_WARNING_CONDITION_FORCED);
    }
    else
    {
      Rte_Call_tiCS_WarningCtrl_Set_Operation(WARN_DPF_WARNING_ID, UI_WARNING_STATUS_ON);
    }

    Rte_Write_ppSR_DPFWrnMdl_GdtCtrl_State_GdtCtrl_State(DPF_Warning_status_u8);
    DPF_Warning_last_status_u8 = DPF_Warning_status_u8 ;
  }
  else
  {
    Rte_Call_tiCS_WarningCtrl_Set_Operation(WARN_DPF_WARNING_ID, UI_WARNING_STATUS_OFF);
  }

  if ((0x2 <= DPF_Warning_status_u8 && 0x7 >= DPF_Warning_status_u8) && (0x5 != DPF_Warning_status_u8))
  {
    if (DPF_Warning_last_status_u8 != DPF_Warning_status_u8)
    {
      Rte_Call_tiCS_WarningCtrl_Set_Operation(WARN_DPF_WARNING_ID, UI_WARNING_CONDITION_FORCED);
    }
    else 
    {
      Rte_Call_tiCS_WarningCtrl_Set_Operation(WARN_DPF_WARNING_ID, UI_WARNING_STATUS_ON);
    }

    Rte_Write_ppSR_DPFWrnMdl_GdtCtrl_State_GdtCtrl_State(DPF_Warning_status_u8);
    DPF_Warning_last_status_u8 = DPF_Warning_status_u8;
  }
  else 
  {
    Rte_Call_tiCS_WarningCtrl_Set_Operation(WARN_DPF_WARNING_ID, UI_WARNING_STATUS_OFF);
  }*/


/*
static void GPF_Warning_Process(void)
{
  uint8 u8SigVal = 0;
  
  Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_GasPrtcWarn_G_Rq(&u8SigVal);
  // MSG_GPF_FULL: 1 
  if (MSG_GPF_FULL == u8SigVal)
  {
    Rte_Call_tiCS_WarningCtrl_Set_Operation(WARN_GPF_FULL_WARNING_ID, UI_WARNING_STATUS_ON);
  }
  else 
  {
    Rte_Call_tiCS_WarningCtrl_Set_Operation(WARN_GPF_FULL_WARNING_ID, UI_WARNING_STATUS_OFF);
  }
  // MSG_GPF_FULL: 4 
  if (MSG_GPF_LIMIT == u8SigVal)
  {
    Rte_Call_tiCS_WarningCtrl_Set_Operation(WARN_GPF_LIMIT_WARNING_ID, UI_WARNING_STATUS_ON);
  }
  else 
  {
    Rte_Call_tiCS_WarningCtrl_Set_Operation(WARN_GPF_LIMIT_WARNING_ID, UI_WARNING_STATUS_OFF);
  }
}
*/

static uint8 message_368_invalid(void)
{
  uint8 u8MsgNR   = (uint8)0;
  uint8 u8MsgTout = (uint8)0;

  Rte_Read_DPFWrnMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_NR(&u8MsgNR);
  Rte_Read_DPFWrnMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_Tout(&u8MsgTout);
  if (((uint8)RTE_E_TIMEOUT == u8MsgTout) || ((uint8)RTE_E_NEVER_RECEIVED == u8MsgNR))
  {
    return TRUE;
  }
  else 
  {
    return FALSE;
  }
}


static void DPF_Warning_init(void)
{
  //DPF_Warning_last_status_u8 = 0;
  //Rte_Call_tiCS_WarningCtrl_Set_Operation(WARN_DPF_WARNING_ID, UI_WARNING_STATUS_OFF);

  for (uint8 i=0;i < DPF_WARNING_TOTAL;i ++)
  {
    Rte_Call_tiCS_WarningCtrl_Set_Operation(DPFWarning_ID_Array[i],UI_WARNING_STATUS_OFF);
    DPFWrnStatus[i] = UI_WARNING_STATUS_OFF;
  }
}

/*
static void GPF_Warning_init(void)
{
  uint8 i = 0;

  for (i = 0; i < PROD_SPEC_GPF_V2_1_WARNING_LIST_NUM; i++)
  {
    Rte_Call_tiCS_WarningCtrl_Set_Operation(c_GPF_V2_1_warn_SA[i].m_GPF_warn_id, UI_WARNING_STATUS_OFF);
  }
}
*/

