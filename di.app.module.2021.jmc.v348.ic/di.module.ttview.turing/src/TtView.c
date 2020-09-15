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
#include <Rte_TtView.h>
#include "TtView_Cfg.h"
#include "CmpLib.h"
#include "system.h"

#ifndef cTtView_DOut_SetPin
  #error "TtView_Cfg.h: cTtView_DOut_SetPin function macro definition is missing. Configure the respective Rte interface in Excel utility"
#endif

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(TtView)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

#define cTTView_TURN_OFF            ((uint8)0u)
#define cTTView_TURN_ON             ((uint8)1u)
#define cTTView_INPUT               ((uint8)2u)

#define cTT_PHASE_OFF           ((uint8)0u)
#define cTT_PHASE_ON            ((uint8)1u)

#define cTT_CHANGE_PHASE        ((uint8)1u)
#define cTT_PHASE_PATTERN_SIZE  ((uint8)2u)

typedef struct STtViewTelltaleList_t
{
  uint16 FlashOnPhaseCntr;
  uint16 FlashOffPhaseCntr;
  uint8 MinFlashCntr;
  uint8 MaxFlashCntr;
}STtViewTelltaleConfigType;

typedef struct STtViewTelltaleInfo_t
{
  uint16 TelltaleOnOffCntr;
  uint8 TelltaleMinFlashCntr;
  uint8 TelltaleMaxFlashCntr;
  uint8 TelltaleInputStatus;
  uint8 PrevTelltaleInputStatus;
  uint8 TelltalePhase;
  uint8 PatternSize;
  boolean OnePatternFinished;
}STtViewTelltaleInfo;

typedef struct STtViewTelltaleFlashInfo_t
{
  uint16 TelltaleOnOffCntr;
  uint8 TelltalePhase;
  uint8 PatternSize;
  uint8 NoOfActiveClients;
  boolean OnePatternFinished;
  boolean IsFlashRequested;
}STtViewTelltaleFlashInfo;

typedef struct STtTableData_T
{
    uint8 TtType;
    uint8 TtExtByteID;
    uint64 TtBitPos;
	void (*TtViewHandler)(boolean Ttsts); 
}STtTableData;

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static STtTableData TelltaleOutputList[cTOTAL_TT_CONFIGURED] = cTtOutputTableList;
static STtViewTelltaleConfigType const TelltaleConfigTable[cTT_MAX_SUPPORTED_STATES] = cTtConfigTableList;

static STtViewTelltaleInfo TelltaleInfoTable[cTOTAL_TT_CONFIGURED];

#ifdef TT_SYNC_REQUIRED
static STtViewTelltaleFlashInfo TelltaleFlashInfoTable[cTT_MAX_SUPPORTED_STATES];
#endif
static uint64 TtTFTViewData;

static void CTtView_Update_Output(void);
static void TtViewUpdate_OutputDatas(uint8 Tt_index, boolean Tt_Status);
#ifndef TT_SYNC_REQUIRED
static void TtView_Asynchronous_Flash_Process(uint8 Tt_index);
#endif
#ifdef TT_SYNC_REQUIRED
static void TtView_Synchronous_Flash_Process(void);
#endif
static void TtView_Flash_Min_Max_Cntr_Process(uint8 Tt_index);
static void TtView_Prepare_Flash_Process(uint8 Tt_index, uint8 Tt_target_phase);
static void TtView_Different_Flash_Process(uint8 Tt_index);
static void TtView_Same_Flash_Process(uint8 Tt_index);
static boolean TtViewFlashInfoPatternSts(uint8 PatternIndex);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
    uint8 index;

    for(index=0; index <cTOTAL_TT_CONFIGURED; index++)
    {
        TelltaleInfoTable[index].TelltaleOnOffCntr = 0;
        TelltaleInfoTable[index].TelltaleMinFlashCntr = 0;
        TelltaleInfoTable[index].TelltaleMaxFlashCntr = 0;
        TelltaleInfoTable[index].TelltaleInputStatus = cTT_PHASE_OFF;
        TelltaleInfoTable[index].PrevTelltaleInputStatus = cTT_PHASE_OFF;
        TelltaleInfoTable[index].TelltalePhase = cTT_PHASE_OFF;
        TelltaleInfoTable[index].PatternSize = cTT_PHASE_PATTERN_SIZE;
        TelltaleInfoTable[index].OnePatternFinished = (boolean)FALSE;
    }
    #ifdef TT_SYNC_REQUIRED
    for(index = 0;index < cTT_MAX_SUPPORTED_STATES; index++)
    {
        TelltaleFlashInfoTable[index].TelltaleOnOffCntr = 0;
        TelltaleFlashInfoTable[index].TelltalePhase = cTT_PHASE_OFF;
        TelltaleFlashInfoTable[index].PatternSize = cTT_PHASE_PATTERN_SIZE;
        TelltaleFlashInfoTable[index].NoOfActiveClients = 0;
        TelltaleFlashInfoTable[index].OnePatternFinished = (boolean)FALSE;
        TelltaleFlashInfoTable[index].IsFlashRequested = (boolean)FALSE;
    }
    #endif

    TtTFTViewData = (uint64)0;
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
    uint8 Ix;

    for (Ix = 0; Ix < cTOTAL_TT_CONFIGURED; Ix++)
    {
        TtViewUpdate_OutputDatas(Ix,(boolean)FALSE);
    }
    
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation( void )
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
static Std_ReturnType CmpDeActivation( void )
{
    uint8 Ix;

    for(Ix = 0; Ix < cTOTAL_TT_CONFIGURED; Ix++)
    {
        TelltaleInfoTable[Ix].TelltaleOnOffCntr = 0;
        TelltaleInfoTable[Ix].TelltaleMinFlashCntr = 0;
        TelltaleInfoTable[Ix].TelltaleMaxFlashCntr = 0;
        TelltaleInfoTable[Ix].TelltaleInputStatus = cTT_PHASE_OFF;
        TelltaleInfoTable[Ix].PrevTelltaleInputStatus = cTT_PHASE_OFF;
        TelltaleInfoTable[Ix].TelltalePhase = cTT_PHASE_OFF;
        TelltaleInfoTable[Ix].PatternSize = cTT_PHASE_PATTERN_SIZE;
        TelltaleInfoTable[Ix].OnePatternFinished = (boolean)FALSE;
    }
    #ifdef TT_SYNC_REQUIRED
    for(Ix = 0;Ix < cTT_MAX_SUPPORTED_STATES; Ix++)
    {
        TelltaleFlashInfoTable[Ix].TelltaleOnOffCntr = 0;
        TelltaleFlashInfoTable[Ix].TelltalePhase = cTT_PHASE_OFF;
        TelltaleFlashInfoTable[Ix].PatternSize = cTT_PHASE_PATTERN_SIZE;
        TelltaleFlashInfoTable[Ix].NoOfActiveClients = 0;
        TelltaleFlashInfoTable[Ix].OnePatternFinished = (boolean)FALSE;
        TelltaleFlashInfoTable[Ix].IsFlashRequested = (boolean)FALSE;
    }
    #endif

    for (Ix = 0; Ix < cTOTAL_TT_CONFIGURED; Ix++)
    {
        TtViewUpdate_OutputDatas(Ix,(boolean)FALSE);
    }
    
    return  E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
    CTtView_Update_Output();
    /*RTE Write Interface to TFT byte final*/
	//Yxiong2
    Rte_Write_tiSR_PosLightMd_Send_Value(TtTFTViewData);
    return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Diagnostic state
///
/// @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
    return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}
//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================
/*============================================================================
** Function Name:     void f_tt_update_tt_status(TELLTALE_NAME index, SINT8 tt_status)
** Description:        This function set the telltale pointed to by "array index" 1
**                     or 0 based on the "tt_status" input parameter.                    
** Input Parameters:   Telltale Status (TT_TURN_ON, TT_TURN_OFF and 
**                      Falshing macros defined in the header file )  
** Outputs             : The telltale name pointed by "index" value is set  to TRUE or FALSE
**==========================================================================*/

//void CTtView_TtViewUpdateStatus(ETtMdlMask index, ETtMdlMask tt_status)
void CTtView_TtUpdateTtStatus_Operation(ETtMdlMask index, ETtMdlMask tt_status)
{
    if(index<cTOTAL_TT_CONFIGURED)
    {
      TelltaleInfoTable[index].TelltaleInputStatus = tt_status;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  CTtView_Update_Output
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static void CTtView_Update_Output( void )
{
    uint8 index;

    #ifdef TT_SYNC_REQUIRED
      TtView_Synchronous_Flash_Process();
    #endif
    /* 
      Loop until 'Number of Telltales' configured and process below
        1) Telltale OFF -> ON
        2) Telltale OFF -> FLASH
        3) Telltale ON -> OFF
        4) Telltale ON -> FLASH
        5) Telltale FALSH -> OFF
        6) Telltale FLASH -> ON
        7) Telltale FLASH -> FLASH (Switch from one frequency to another)
    */
    for(index = 0; index < cTOTAL_TT_CONFIGURED; index++)
    {
      /*
         Default Entry:
         Previous status of all Telltales will be 'OFF'
      */
      if(TelltaleInfoTable[index].PrevTelltaleInputStatus == cTTView_TURN_OFF)
      {
         if(TelltaleInfoTable[index].TelltaleInputStatus == cTTView_TURN_ON)
         {
           /*
              State Transition - OFF -> ON
               When Telltale is in 'OFF' State, and 'ON' state is requested,
                Telltale shall be switched to 'ON' immediately.
           */
           TelltaleInfoTable[index].TelltalePhase = cTT_PHASE_ON;
           TelltaleInfoTable[index].PrevTelltaleInputStatus = TelltaleInfoTable[index].TelltaleInputStatus;
         }
         else if((TelltaleInfoTable[index].TelltaleInputStatus > cTTView_TURN_ON) &&\
                (TelltaleInfoTable[index].TelltaleInputStatus < cTT_MAX_SUPPORTED_STATES))
         {
           /*
            State Transition - OFF -> FLASH
              When Telltale is in 'OFF' State, and 'FLASH' state is requested,
               switch to 'FLASH' state  and start Flash with On Phase.
           */
           TtView_Prepare_Flash_Process(index,cTT_PHASE_ON);
           TelltaleInfoTable[index].PrevTelltaleInputStatus = TelltaleInfoTable[index].TelltaleInputStatus;
         }
         else
         {
           /*
              No State Transition .Maintain previous state.
           */
           TelltaleInfoTable[index].TelltalePhase = cTT_PHASE_OFF;
           TelltaleInfoTable[index].PrevTelltaleInputStatus = TelltaleInfoTable[index].TelltaleInputStatus;
         }
      }
      else if(TelltaleInfoTable[index].PrevTelltaleInputStatus == cTTView_TURN_ON)
      {
         if(TelltaleInfoTable[index].TelltaleInputStatus == cTTView_TURN_OFF)
         {
           /*
              State Transition - ON -> OFF
               When Telltale is in 'ON' State, and 'OFF' state is requested,
                Telltale shall be switched to 'OFF' immediately.
           */
           TelltaleInfoTable[index].TelltalePhase = cTT_PHASE_OFF;
           TelltaleInfoTable[index].PrevTelltaleInputStatus = TelltaleInfoTable[index].TelltaleInputStatus;
         }
         else if((TelltaleInfoTable[index].TelltaleInputStatus > cTTView_TURN_ON) &&\
                (TelltaleInfoTable[index].TelltaleInputStatus < cTT_MAX_SUPPORTED_STATES))
         {
           /*
            State Transition - ON -> FLASH
              When Telltale is in 'ON' State, and 'FLASH' state is requested,
               switch to 'FLASH' state  and start Flash with Off Phase.
           */
           TtView_Prepare_Flash_Process(index,cTT_PHASE_OFF);
           TelltaleInfoTable[index].PrevTelltaleInputStatus = TelltaleInfoTable[index].TelltaleInputStatus;
         }
         else
         {
           /*
              No State Transition. Maintain previous state and update output accordingly.
           */
           TelltaleInfoTable[index].TelltalePhase = cTT_PHASE_ON;
           TelltaleInfoTable[index].PrevTelltaleInputStatus = TelltaleInfoTable[index].TelltaleInputStatus;
         }
      }
      else if((TelltaleInfoTable[index].PrevTelltaleInputStatus > cTTView_TURN_ON) &&\
              (TelltaleInfoTable[index].PrevTelltaleInputStatus < cTT_MAX_SUPPORTED_STATES))
      {
         #ifndef TT_SYNC_REQUIRED
           TtView_Asynchronous_Flash_Process(index);
         #endif
         /* Process Minimum and Maximum Flash Counter for each Telltale */
         TtView_Flash_Min_Max_Cntr_Process(index);
         if(TelltaleInfoTable[index].TelltaleInputStatus == cTTView_TURN_OFF)
         {
           /*
             State Transition - FLASH -> OFF
              When Telltale is in 'FLASH' State, and 'OFF' state is requested,
               Telltale shall be stopped after finishing minimum Flash times configured.
           */
           if((uint16)0x00u == TelltaleInfoTable[index].TelltaleMinFlashCntr)
           {
             #ifdef TT_SYNC_REQUIRED
             if((uint8)0x00u != TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].NoOfActiveClients)
             {
               TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].NoOfActiveClients--;
             }
             if((uint8)0x00u == TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].NoOfActiveClients)
             {
               TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].IsFlashRequested = (boolean)FALSE;
             }
             #endif
             TelltaleInfoTable[index].TelltalePhase = cTT_PHASE_OFF;
             TelltaleInfoTable[index].PrevTelltaleInputStatus = TelltaleInfoTable[index].TelltaleInputStatus;
           }
           else
           {
             #ifdef TT_SYNC_REQUIRED
             TelltaleInfoTable[index].TelltalePhase = TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].TelltalePhase;
             #endif
           }
         }
         else if(TelltaleInfoTable[index].TelltaleInputStatus == cTTView_TURN_ON)
         {
           /*
             State Transition - FLASH -> ON
               When Telltale is in 'FLASH' State, and 'ON' state is requested,
                If Telltale is in 'ON' Phase, then Telltale shall be Switched to 'ON' Immediately.
                If Telltale is in 'OFF' Phase, then Telltale shall be Switched to 'ON' only after finishing 'OFF' phase..
           */
           #ifdef TT_SYNC_REQUIRED
           if((TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].TelltalePhase ==cTTView_TURN_ON)||\
              ((TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].TelltaleOnOffCntr ==\
                 TelltaleConfigTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].FlashOffPhaseCntr)||\
               (TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].TelltaleOnOffCntr ==\
                 TelltaleConfigTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].FlashOnPhaseCntr)))
           #else
           if((TelltaleInfoTable[index].TelltalePhase ==cTTView_TURN_ON)||\
              ((TelltaleInfoTable[index].TelltaleOnOffCntr ==\
                TelltaleConfigTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].FlashOffPhaseCntr)||\
               (TelltaleInfoTable[index].TelltaleOnOffCntr ==\
                TelltaleConfigTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].FlashOnPhaseCntr)))
           #endif
           {
             #ifdef TT_SYNC_REQUIRED
             if((uint8)0x00u != TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].NoOfActiveClients)
             {
               TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].NoOfActiveClients--;
             }
             if((uint8)0x00u == TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].NoOfActiveClients)
             {
               TelltaleFlashInfoTable[TelltaleInfoTable[index].PrevTelltaleInputStatus].IsFlashRequested = (boolean)FALSE;
             }
             #endif
             TelltaleInfoTable[index].TelltalePhase = cTT_PHASE_ON;
             TelltaleInfoTable[index].PrevTelltaleInputStatus = TelltaleInfoTable[index].TelltaleInputStatus;
           }
         }
         else
         {
           if((TelltaleInfoTable[index].PrevTelltaleInputStatus > cTTView_TURN_ON)&&\
              (TelltaleInfoTable[index].PrevTelltaleInputStatus < cTT_MAX_SUPPORTED_STATES)&&\
              (TelltaleInfoTable[index].PrevTelltaleInputStatus != TelltaleInfoTable[index].TelltaleInputStatus))
           {
             /*
               State Transition - FLASH -> FLASH
               When Telltale is in 'FLASH' State, and 'FLASH' with different frequency is requested,
                If Telltale is in 'ON' Phase, complete 'ON' phase and start new FLASH.
                If Telltale is in 'OFF' Phase, complete 'OFF' phase and start new FLASH.
             */
             TtView_Different_Flash_Process(index);
           }
           else
           {
             /* No State Change. Same FLASH is in progress. Maintain Previous state */
             TtView_Same_Flash_Process(index);
           }/*End of Same Flash Process*/
         }
      }
      else
      {
        /* Do Nothing */
      }

      TtViewUpdate_OutputDatas(index,(boolean)TelltaleInfoTable[index].TelltalePhase);
    }/* End of For Loop */
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  TtView_Different_Flash_Process
///
/// @return None
//---------------------------------------------------------------------------------------------------------------------
static void TtView_Different_Flash_Process(uint8 Tt_index)
{
  #ifdef TT_SYNC_REQUIRED
  /* Check whether 'ON' or 'OFF' phase is finished */
  if((TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].TelltaleOnOffCntr ==\
      TelltaleConfigTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].FlashOnPhaseCntr)||\
      (TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].TelltaleOnOffCntr ==\
      TelltaleConfigTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].FlashOffPhaseCntr))
  {
    if((uint8)0x00u != TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].NoOfActiveClients)
    {
      TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].NoOfActiveClients--;
    }
    if((uint8)0x00u == TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].NoOfActiveClients)
    {
      /* All clients are removed. Hence clear Flash requests for that particular frequency */
      TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].IsFlashRequested = (boolean)FALSE;
    }
    if(TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].IsFlashRequested == (boolean)FALSE)
    {
      /* Request New Flash */
      TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].IsFlashRequested = (boolean)TRUE;
      /* Update New Client to Flash list */
      TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].NoOfActiveClients++;
    }
    else
    {
      /* if the requested frequency is already flashing, increase the client list for the new request */
      TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].NoOfActiveClients++;
    }
    /* Get the Telltale Phase */
    TelltaleInfoTable[Tt_index].TelltalePhase = TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].TelltalePhase;
    /* Load the Flash Counter based on Telltale Phase */
    if(TelltaleInfoTable[Tt_index].TelltalePhase == cTT_PHASE_OFF)
    {
      TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].TelltaleOnOffCntr = \
      TelltaleConfigTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].FlashOffPhaseCntr;
    }
    else
    {
      TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].TelltaleOnOffCntr = \
      TelltaleConfigTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].FlashOnPhaseCntr;
    }
    /* Re Load the patterm size to indicate fresh FLASH */
    TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].PatternSize = cTT_PHASE_PATTERN_SIZE;
    /*  Re load minimum and maximum flash durations for the new flash. */
    TelltaleInfoTable[Tt_index].TelltaleMinFlashCntr = TelltaleConfigTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].MinFlashCntr;
    TelltaleInfoTable[Tt_index].TelltaleMaxFlashCntr = TelltaleConfigTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].MaxFlashCntr;
    /* Update State */
    TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus = TelltaleInfoTable[Tt_index].TelltaleInputStatus;
  }
  #else
  /* Check whether 'ON' or 'OFF' phase is finished */
  if((TelltaleInfoTable[Tt_index].TelltaleOnOffCntr == TelltaleConfigTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].FlashOnPhaseCntr)||\
      (TelltaleInfoTable[Tt_index].TelltaleOnOffCntr == TelltaleConfigTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].FlashOffPhaseCntr))
  {
    /* Load the Flash Counter based on Telltale Phase */
    if(TelltaleInfoTable[Tt_index].TelltalePhase == cTT_PHASE_OFF)
    {
      TelltaleInfoTable[Tt_index].TelltaleOnOffCntr = TelltaleConfigTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].FlashOffPhaseCntr;
    }
    else
    {
      TelltaleInfoTable[Tt_index].TelltaleOnOffCntr = TelltaleConfigTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].FlashOnPhaseCntr;
    }
    /* Re Load the patterm size to indicate fresh FLASH */
    TelltaleInfoTable[Tt_index].PatternSize = cTT_PHASE_PATTERN_SIZE;
    /*  Re Load minimum and maximum flash durations for the new flash. */
    TelltaleInfoTable[Tt_index].TelltaleMinFlashCntr = TelltaleConfigTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].MinFlashCntr;
    TelltaleInfoTable[Tt_index].TelltaleMaxFlashCntr = TelltaleConfigTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].MaxFlashCntr;
    /* Update State */
    TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus = TelltaleInfoTable[Tt_index].TelltaleInputStatus;
  }
  #endif
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  TtView_Same_Flash_Process
///
/// @return None
//---------------------------------------------------------------------------------------------------------------------
static void TtView_Same_Flash_Process(uint8 Tt_index)
{
  /* 
     Process only if Telltale did not finish Maximum Flash Counter
  */
  if((uint8)0x00u != TelltaleInfoTable[Tt_index].TelltaleMaxFlashCntr)
  {
    #ifdef TT_SYNC_REQUIRED
    /*
      Check and update the Telltale Phase as per below
      When a Telltale is currently FLASHING and FLASH request for another Telltale
      with same Frequency is requested, SYNC shall be decided as
      'XXX' - Telltale currently Flashing
      'YYY' - Telltale requested for Flashing with same Frequency.
        1) if Telltale phase of 'XXX' is 'ON' and Telltale Phase of 'YYY' is 'OFF'
         then 'YYY' shall be Synced after finishing current pattern.
        2) if Telltale phase of 'XXX' is 'ON' and Telltale Phase of 'YYY' is 'ON'
         then 'YYY' shall be Synced after finishing current edge.In this case the edge is 'OFF'.
        3) if Telltale phase of 'XXX' is 'OFF' and Telltale Phase of 'YYY' is 'ON'
         then 'YYY' shall be Synced after finishing current pattern.
        4) if Telltale phase of 'XXX' is 'OFF' and Telltale Phase of 'YYY' is 'OFF'
         then 'YYY' shall be Synced after finishing current edge.In this case the edge is 'ON'.
    */
    if((TelltaleInfoTable[Tt_index].TelltalePhase == cTT_PHASE_OFF)&&\
       (TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].TelltaleOnOffCntr ==\
        TelltaleConfigTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].FlashOnPhaseCntr))
    {
      TelltaleInfoTable[Tt_index].TelltalePhase = TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].TelltalePhase;
    }
    else
    {
      if((TelltaleInfoTable[Tt_index].TelltalePhase == cTT_PHASE_ON)&&\
         (TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].TelltaleOnOffCntr ==\
          TelltaleConfigTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].FlashOffPhaseCntr))
      {
        TelltaleInfoTable[Tt_index].TelltalePhase = TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].TelltalePhase;
      }
    }
    #endif
  }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  TtView_Prepare_Flash_Process
///
/// @return None
//---------------------------------------------------------------------------------------------------------------------
static void TtView_Prepare_Flash_Process(uint8 Tt_index, uint8 Tt_target_phase)
{
  #ifdef TT_SYNC_REQUIRED
  if(TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].IsFlashRequested == (boolean)FALSE)
  {
    TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].IsFlashRequested = (boolean)TRUE;
    TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].PatternSize = cTT_PHASE_PATTERN_SIZE;
    TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].TelltalePhase = Tt_target_phase;
    TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].TelltaleOnOffCntr = TelltaleConfigTable[(TelltaleInfoTable[Tt_index].TelltaleInputStatus)].FlashOnPhaseCntr;
    TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].NoOfActiveClients++;
  }
  else
  {
    TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].NoOfActiveClients++;
  }
  #else
    TelltaleInfoTable[Tt_index].PatternSize = cTT_PHASE_PATTERN_SIZE;
    TelltaleInfoTable[Tt_index].TelltalePhase = Tt_target_phase;
    TelltaleInfoTable[Tt_index].TelltaleOnOffCntr = TelltaleConfigTable[(TelltaleInfoTable[Tt_index].TelltaleInputStatus)].FlashOnPhaseCntr;
  #endif
  TelltaleInfoTable[Tt_index].TelltalePhase = Tt_target_phase;
  TelltaleInfoTable[Tt_index].TelltaleMinFlashCntr = TelltaleConfigTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].MinFlashCntr;
  TelltaleInfoTable[Tt_index].TelltaleMaxFlashCntr = TelltaleConfigTable[TelltaleInfoTable[Tt_index].TelltaleInputStatus].MaxFlashCntr;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  TtView_Flash_Min_Max_Cntr_Process
///
/// @return None
//---------------------------------------------------------------------------------------------------------------------
static void TtView_Flash_Min_Max_Cntr_Process(uint8 Tt_index)
{
  #ifdef TT_SYNC_REQUIRED
  if((boolean)FALSE != TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].OnePatternFinished)
  #else
  if((boolean)FALSE != TelltaleInfoTable[Tt_index].OnePatternFinished)
  #endif
  {
    #ifndef TT_SYNC_REQUIRED
    TelltaleInfoTable[Tt_index].OnePatternFinished = (boolean)FALSE;
    #endif
    /* Check and decrement Telltale Minimum and Maximum Flash counters */
    if((uint8)0x00u != TelltaleInfoTable[Tt_index].TelltaleMinFlashCntr)
    {
      TelltaleInfoTable[Tt_index].TelltaleMinFlashCntr--;
    }
    if(((uint8)0x00u != TelltaleInfoTable[Tt_index].TelltaleMaxFlashCntr)&&\
       (cTT_MAX_FLASH_NA != TelltaleInfoTable[Tt_index].TelltaleMaxFlashCntr))
    {
      TelltaleInfoTable[Tt_index].TelltaleMaxFlashCntr--;
      if((uint8)0x00u == TelltaleInfoTable[Tt_index].TelltaleMaxFlashCntr)
      {
        /* 
          Telltale has finished flashing for maximum flash times configured.
          Forcibly switch the state from 'FLASH' -> 'OFF' and clear the counters.
        */
        #ifdef TT_SYNC_REQUIRED
        if((uint8)0x00u != TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].NoOfActiveClients)
        {
          TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].NoOfActiveClients--;
        }
        if((uint8)0x00u == TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].NoOfActiveClients)
        {
          TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].IsFlashRequested = (boolean)FALSE;
        }
        TelltaleFlashInfoTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].TelltalePhase = cTTView_TURN_OFF;
        TelltaleInfoTable[Tt_index].TelltalePhase = cTTView_TURN_OFF;
        #else
        TelltaleInfoTable[Tt_index].TelltalePhase = cTTView_TURN_OFF;
        #endif
      }
    }
  }
}

#ifndef TT_SYNC_REQUIRED
//---------------------------------------------------------------------------------------------------------------------
/// @brief  TtView_Asynchronous_Flash_Process
///
/// @return None
//---------------------------------------------------------------------------------------------------------------------
static void TtView_Asynchronous_Flash_Process(uint8 Tt_index)
{
  uint8 CurrentPhase;

  /* Check whether the Telltale index is with in the range */
  if(Tt_index < cTOTAL_TT_CONFIGURED)
  {
    if((uint8)0x00u != TelltaleInfoTable[Tt_index].TelltaleMaxFlashCntr)
    {
      /* Validate Counter to process Flash */
      if((uint16)0x00u != TelltaleInfoTable[Tt_index].TelltaleOnOffCntr)
      {
        /* Decrement counter */
        TelltaleInfoTable[Tt_index].TelltaleOnOffCntr--;
        /* Maintain Telltale Phase */
        CurrentPhase = TelltaleInfoTable[Tt_index].TelltalePhase;
      }
      if((uint16)0x00u == TelltaleInfoTable[Tt_index].TelltaleOnOffCntr)
      {
        /* Switch Telltale Phase because Previous Phase is finished */
        CurrentPhase = (uint8)(cTT_CHANGE_PHASE ^ TelltaleInfoTable[Tt_index].TelltalePhase);
        /* 
          Validate Pattern Size
          Each Telltale will have two phases 'OFF' and 'ON' in Flash state.
          When Telltale is completed both 'OFF' and 'ON' phases it is decided as 'One Pattern'
          Pattern size is initially loaded with '2' when ever there is a fresh FLASH request.
          One for 'OFF' phase and other for 'ON' phase.
          Pattern size is decremented by '1' every time a phase is finished.
          Pattern size is reloaded when one pattern is finished in order to repeat the pattern
        */
        
        /* Defensive check added before decrement*/
        if((uint8)0x00u != TelltaleInfoTable[Tt_index].PatternSize)
        {
            TelltaleInfoTable[Tt_index].PatternSize--;
        }
        if((uint8)0x00u == TelltaleInfoTable[Tt_index].PatternSize)
        {
          /* Re load Pattern Size to repeat the pattern */
          TelltaleInfoTable[Tt_index].PatternSize = cTT_PHASE_PATTERN_SIZE;
          TelltaleInfoTable[Tt_index].OnePatternFinished = (boolean)TRUE;
        }
        /* Re Load Flash Counter with new value based on current phase */
        if(CurrentPhase == cTT_PHASE_OFF)
        {
          TelltaleInfoTable[Tt_index].TelltaleOnOffCntr = TelltaleConfigTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].FlashOffPhaseCntr;
        }
        else
        {
          TelltaleInfoTable[Tt_index].TelltaleOnOffCntr = TelltaleConfigTable[TelltaleInfoTable[Tt_index].PrevTelltaleInputStatus].FlashOnPhaseCntr;
        }
      }
      TelltaleInfoTable[Tt_index].TelltalePhase = CurrentPhase;
    }
  }
}
#endif

#ifdef TT_SYNC_REQUIRED
//---------------------------------------------------------------------------------------------------------------------
/// @brief  TtView_Synchronous_Flash_Process
///
/// @return None
//---------------------------------------------------------------------------------------------------------------------
static void TtView_Synchronous_Flash_Process(void)
{
  uint8 CurrentPhase = cTT_PHASE_OFF;
  uint8 index;

  /* Check whether the Telltale index is with in the range */
  for(index = 0; index < cTT_MAX_SUPPORTED_STATES; index++)
  {
    if((boolean)FALSE != TelltaleFlashInfoTable[index].OnePatternFinished)
    {
      TelltaleFlashInfoTable[index].OnePatternFinished = (boolean)FALSE;
    }
    if((boolean)FALSE != TelltaleFlashInfoTable[index].IsFlashRequested)
    {
      /* Validate Counter to process Flash */
      if((uint16)0x00u != TelltaleFlashInfoTable[index].TelltaleOnOffCntr)
      {
        /* Decrement counter */
        TelltaleFlashInfoTable[index].TelltaleOnOffCntr--;
        /* Maintain Telltale Phase */
        CurrentPhase = TelltaleFlashInfoTable[index].TelltalePhase;
      }
      if((uint16)0x00u == TelltaleFlashInfoTable[index].TelltaleOnOffCntr)
      {
        /* Switch Telltale Phase because Previous Phase is finished */
        CurrentPhase = (uint8)(cTT_CHANGE_PHASE ^ TelltaleFlashInfoTable[index].TelltalePhase);
        /* 
          Validate Pattern Size
          Each Telltale will have two phases 'OFF' and 'ON' in Flash state.
          When Telltale is completed both 'OFF' and 'ON' phases it is decided as 'One Pattern'
          Pattern size is initially loaded with '2' when ever there is a fresh FLASH request.
          One for 'OFF' phase and other for 'ON' phase.
          Pattern size is decremented by '1' every time a phase is finished.
          Pattern size is reloaded when one pattern is finished in order to repeat the pattern
        */
        
        /* Defensive check added before decrement*/
        if((uint8)0x00u != TelltaleFlashInfoTable[index].PatternSize)
        {
            TelltaleFlashInfoTable[index].PatternSize--;
        }
        if((uint8)0x00u == TelltaleFlashInfoTable[index].PatternSize)
        {
          /* Re load Pattern Size to repeat the pattern */
          TelltaleFlashInfoTable[index].PatternSize = cTT_PHASE_PATTERN_SIZE;
          TelltaleFlashInfoTable[index].OnePatternFinished = (boolean)TRUE;
        }
        /* Re Load Flash Counter with new value based on current phase */
        if(CurrentPhase == cTT_PHASE_OFF)
        {
          TelltaleFlashInfoTable[index].TelltaleOnOffCntr = TelltaleConfigTable[index].FlashOffPhaseCntr;
        }
        else
        {
          TelltaleFlashInfoTable[index].TelltaleOnOffCntr = TelltaleConfigTable[index].FlashOnPhaseCntr;
        }
      }
      TelltaleFlashInfoTable[index].TelltalePhase = CurrentPhase;
    }
  }
}
#endif

//---------------------------------------------------------------------------------------------------------------------
/// @brief  TtViewUpdate_OutputDatas
///
/// @return None
//---------------------------------------------------------------------------------------------------------------------
static void TtViewUpdate_OutputDatas(uint8 Tt_index, boolean Tt_Status)
{
    if(TelltaleOutputList[Tt_index].TtType == cTT_HARDWARE_DRIVEN)
    {
       if(TelltaleOutputList[Tt_index].TtViewHandler == NULL) 
	   {
       		Rte_Call_tiCS_TIIoHwAb_DOut_SetPin(TelltaleOutputList[Tt_index].TtExtByteID,Tt_Status);
	   }
	   else
	   {
	   		if (FALSE != TtViewFlashInfoPatternSts(TelltaleInfoTable[Tt_index].TelltaleInputStatus))
			{
	   			TelltaleOutputList[Tt_index].TtViewHandler(Tt_Status);
			}
	   }
    }
    else if(TelltaleOutputList[Tt_index].TtType == cTT_TFT_DRIVEN)
    {
        if(Tt_Status == (boolean)TRUE)
        {
            SETBIT(TtTFTViewData,TelltaleOutputList[Tt_index].TtBitPos);
        }
        else
        {
            CLEARBIT(TtTFTViewData, TelltaleOutputList[Tt_index].TtBitPos);
        }
    }
    else
    {
        /* To satisfy Static Analysis */
    }
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief  TtViewFlashInfoPatternSts
///	@Param  Pattern ID
/// @return TRUE- TT Pattern is Finished, New Pattern can be started.
///         FALSE- TT Pattern not finished, New pattern Cannot be started.			
//---------------------------------------------------------------------------------------------------------------------
static boolean TtViewFlashInfoPatternSts(uint8 PatternIndex)
{
	boolean RetSts;

	RetSts = FALSE;
    
	if(TelltaleFlashInfoTable[PatternIndex].IsFlashRequested == 0u)
	{
		/* TT flash is not in Progress or TT is in ON/OFF State */
		RetSts = TRUE;
	}
	else
	{
	   	/* TT Flash Request in Progress. So Check the TT Phase to Sync*/
	   	if(TelltaleFlashInfoTable[PatternIndex].TelltalePhase == cTT_PHASE_OFF)
		{
		    /* TT is in OFF phase */
		    if(TelltaleFlashInfoTable[PatternIndex].TelltaleOnOffCntr == \
			TelltaleConfigTable[PatternIndex].FlashOffPhaseCntr)
			{
				/* New Pattern Shall be Started */
				RetSts = TRUE;
			}
		}
		else
		{
			/* TT is in ON phase */
			if(TelltaleFlashInfoTable[PatternIndex].TelltaleOnOffCntr == \
			TelltaleConfigTable[PatternIndex].FlashOnPhaseCntr)
			{
				/* New Pattern Shall be Started */
				RetSts = TRUE;
			}
		}
	}
	return (RetSts);
}

/*============================================================================
**============================================================================*/
/***  P V C S    R E V I S I O N    N O T E S**/
/**----------------------------------------------------------------------------
**  For each change to this file, be sure to record:
**  1.  who made the change and when the change was made
**  2.  why the change was made and the intended result
**
**  Date         By      Reason for Change
**  mmm/dd/yy    XXX
**----------------------------------------------------------------------------*/

