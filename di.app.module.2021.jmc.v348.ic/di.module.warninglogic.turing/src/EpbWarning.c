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
#include "Rte.h"
#include "Rte_type.h"
#include "Rte_EpbWarning.h"
#include "CmpLib.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(EpbWarning)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define INVALID_SPEED_VALUE                (0xFFF)
#define SPEED_FACTOR                       (100u)
#define DELTA_ODO_500M                     (500)
#define DELTA_ODO_1000M                    (1000)

#define CANCEL_OSW_OFFSET                  (3U) 
#define TIMEOUT_60000MS                    (60000)
#define TIMEOUT_90000MS                    (90000)

#define TIMEOUT_10S                        (10000)
#define SPEED_RELEASES_LOWER_THRESHOLD     ((uint16)0x1B)
#define SPEED_RELEASES_UPPER_THRESHOLD     ((uint16)0x18D)

typedef enum
{
  Lamp_Off,
  Reserve1,
  Lamp_On,
  Reserve2
}LampSts_t;

#if 0
typedef enum
{
  Door_NoUsed,
  Door_Closed,
  Door_Open,
  Door_Fault
}DoorSts_t;


typedef enum
{
  Initial,
  WaitAlert,
  Alert,
}SeatBeltWarningStateMachine_t;
#endif

typedef enum
{
  Airbag_Lamp_Off,
  Airbag_Lamp_Flash,
  Airbag_Lamp_On,
  Airbag_Reserved
}AirbagSts_t;


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static boolean Message_0x221_Lose(void);
static boolean Message_0x350_Lose(void);
//static boolean Message_0x310_Lose(void);

static void DriverSeatbeltWarningProcess(void);
#if 0
static void PassengerSeatbeltWarningProcess(void);

static void DriverSeatbeltWarningLevel1Process(uint32 Speed);
static void DriverSeatbeltWarningLevel2Process(uint32 Speed);

static void PassengerSeatbeltWarningLevel1Process(uint32 Speed);
static void PassengerSeatbeltWarningLevel2Process(uint32 Speed);

static void DriverSeatbeltInitial1Process(uint32 Speed);
static void DriverSeatbeltWaitAlert1Process(uint32 Speed);
static void DriverSeatbeltAlert1Process(uint32 Speed);

static void DriverSeatbeltInitial2Process(uint32 Speed);
static void DriverSeatbeltWaitAlert2Process(uint32 Speed);
static void DriverSeatbeltAlert2Process(uint32 Speed);

static void PassengerSeatbeltInitial1Process(uint32 Speed);
static void PassengerSeatbeltWaitAlert1Process(uint32 Speed);
static void PassengerSeatbeltAlert1Process(uint32 Speed);

static void PassengerSeatbeltInitial2Process(uint32 Speed);
static void PassengerSeatbeltWaitAlert2Process(uint32 Speed);
static void PassengerSeatbeltAlert2Process(uint32 Speed);
#endif

//static void OverSpeedWarningProcess(uint32 Speed);

static void AirbagWarningProcess(void);
//static void ReadLastSingleTrip(void);
//static void ReadCurrentSingleTrip(void);
//static void CalculateDeltaODO(void);

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
#if 0
static SeatBeltWarningStateMachine_t DriverSeatBeltLevel1StateMchine = Initial;
static SeatBeltWarningStateMachine_t DriverSeatBeltLevel2StateMchine = Initial;

static SeatBeltWarningStateMachine_t PassengerSeatBeltLevel1StateMchine = Initial;
static SeatBeltWarningStateMachine_t PassengerSeatBeltLevel2StateMchine = Initial;

static boolean fl_DriverSeatbeltHoldTimer60_Running = FALSE;
static boolean fl_DriverSeatbeltHoldTimer90_Running = FALSE;

static boolean fl_PassengerSeatbeltHoldTimer60_Running = FALSE;
static boolean fl_PassengerSeatbeltHoldTimer90_Running = FALSE;

static boolean fl_DriverSeatbeltHoldTimer60_Elapsed = FALSE;
static boolean fl_DriverSeatbeltHoldTimer90_Elapsed = FALSE;

static boolean fl_PassengerSeatbeltHoldTimer60_Elapsed = FALSE;
static boolean fl_PassengerSeatbeltHoldTimer90_Elapsed = FALSE;


static uint32 LastSingleTrip = 0;
static uint8 DrvStBltLevel1 = FALSE;
static uint8 PasStBltLevel1 = FALSE;
static uint8 DrvStBltLevel2 = FALSE;
static uint8 PasStBltLevel2 = FALSE;
static uint32 DeltaODO = 0;
#endif
//static uint8 DefaultSpeedLimit           = (uint8)90;
static uint8 LastIGNSts                  = (uint8)eIGN_OFF;
                                                          /*TRUE:Warning Active;FALSE:Warning Inactive*/
static boolean DriverSeatbeltWrn_ActvSts = (boolean)FALSE;
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
  //DeltaODO = 0;
  LastIGNSts = (uint8)eIGN_OFF;
  //DrvStBltLevel1 = UI_WARNING_STATUS_OFF;
  //PasStBltLevel1 = UI_WARNING_STATUS_OFF;
  //DrvStBltLevel2 = UI_WARNING_STATUS_OFF;
  //PasStBltLevel2 = UI_WARNING_STATUS_OFF;
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVER_SEATBELT_WARNING_ID, UI_WARNING_STATUS_OFF);
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_PASSENGER_SEATBELT_WARNING_ID, UI_WARNING_STATUS_OFF);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AIRBAG_FAULT_ID,UI_WARNING_STATUS_OFF);
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_OVER_SPEED_ID,UI_WARNING_STATUS_OFF);
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
  LastIGNSts = (uint8)eIGN_OFF;
  //DeltaODO = 0;
  //DrvStBltLevel1 = UI_WARNING_STATUS_OFF;
  //PasStBltLevel1 = UI_WARNING_STATUS_OFF;
  //DrvStBltLevel2 = UI_WARNING_STATUS_OFF;
  //PasStBltLevel2 = UI_WARNING_STATUS_OFF;
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVER_SEATBELT_WARNING_ID, UI_WARNING_STATUS_OFF);
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_PASSENGER_SEATBELT_WARNING_ID, UI_WARNING_STATUS_OFF);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AIRBAG_FAULT_ID,UI_WARNING_STATUS_OFF);
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_OVER_SPEED_ID,UI_WARNING_STATUS_OFF);
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
  LastIGNSts = (uint8)eIGN_OFF;
  //DeltaODO = 0;
  //DrvStBltLevel1 = UI_WARNING_STATUS_OFF;
  //PasStBltLevel1 = UI_WARNING_STATUS_OFF;
  //DrvStBltLevel2 = UI_WARNING_STATUS_OFF;
  //PasStBltLevel2 = UI_WARNING_STATUS_OFF;
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVER_SEATBELT_WARNING_ID, UI_WARNING_STATUS_OFF);
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_PASSENGER_SEATBELT_WARNING_ID, UI_WARNING_STATUS_OFF);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AIRBAG_FAULT_ID,UI_WARNING_STATUS_OFF);
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_OVER_SPEED_ID,UI_WARNING_STATUS_OFF);
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
  LastIGNSts = (uint8)eIGN_OFF;
  //DeltaODO = 0;
  //DrvStBltLevel1 = UI_WARNING_STATUS_OFF;
  //PasStBltLevel1 = UI_WARNING_STATUS_OFF;
  //DrvStBltLevel2 = UI_WARNING_STATUS_OFF;
  //PasStBltLevel2 = UI_WARNING_STATUS_OFF;  
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVER_SEATBELT_WARNING_ID, UI_WARNING_STATUS_OFF);
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_PASSENGER_SEATBELT_WARNING_ID, UI_WARNING_STATUS_OFF);
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AIRBAG_FAULT_ID,UI_WARNING_STATUS_OFF);
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_OVER_SPEED_ID,UI_WARNING_STATUS_OFF);
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
static Std_ReturnType CmpActive( void )
{
  //uint32 SpeedU32 = (uint32)0;
  uint8  IGNSts   = (uint8)eIGN_OFF;
  //Rte_Read_EpbWarning_rpSR_TISpdMdl_Send_DigitalValue_Value(&SpeedU32);
  Rte_Read_EpbWarning_rpIgnState_IGNState(&IGNSts);
  //SpeedU32/= (uint32)SPEED_FACTOR;
  
  if( (uint8)eIGN_RUN == IGNSts )
  {
    //ReadLastSingleTrip();
    //CalculateDeltaODO();
    DriverSeatbeltWarningProcess();
    //PassengerSeatbeltWarningProcess();
    AirbagWarningProcess();
    
    //OverSpeedWarningProcess(SpeedU32);
  }
  else
  {
    //DrvStBltLevel1 = UI_WARNING_STATUS_OFF;
    //PasStBltLevel1 = UI_WARNING_STATUS_OFF;
    //DrvStBltLevel2 = UI_WARNING_STATUS_OFF;
    //PasStBltLevel2 = UI_WARNING_STATUS_OFF;  
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVER_SEATBELT_WARNING_ID, UI_WARNING_STATUS_OFF);
    //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_PASSENGER_SEATBELT_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AIRBAG_FAULT_ID,UI_WARNING_STATUS_OFF);
    //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_OVER_SPEED_ID,UI_WARNING_STATUS_OFF);
  }
  LastIGNSts = IGNSts;
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

static boolean Message_0x221_Lose(void)
{
  boolean ValidFlag      = (boolean)FALSE;
  uint8   ABS_ToutStatus = (uint8)0;
  uint8   ABS_NRStatus   = (uint8)0;
  Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&ABS_NRStatus);
  Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&ABS_ToutStatus);
  if( ((uint8)RTE_E_NEVER_RECEIVED == ABS_NRStatus) || ((uint8)RTE_E_TIMEOUT == ABS_ToutStatus) )
  {
    ValidFlag = (boolean)TRUE;
  }
  return ValidFlag;
}

static boolean Message_0x350_Lose(void)
{
  boolean ValidFlag    = (boolean)FALSE;
  uint8 SRS_NRstatus   = (uint8)0;
  uint8 SRS_ToutStatus = (uint8)0;

  Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_NR(&SRS_NRstatus);
  Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_Tout(&SRS_ToutStatus);
  if( ((uint8)RTE_E_NEVER_RECEIVED == SRS_NRstatus) || ((uint8)RTE_E_TIMEOUT == SRS_ToutStatus) )
  {
    ValidFlag = (boolean)TRUE;
  }
  return ValidFlag;
}

#if 0
static boolean Message_0x310_Lose(void)
{
  boolean ValidFlag = FALSE;
  uint8 BCM_Door_NRstatus = 0;
  uint8 BCM_Door_ToutStatus = 0;

  Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&BCM_Door_NRstatus);
  Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&BCM_Door_ToutStatus);
  if( RTE_E_NEVER_RECEIVED == BCM_Door_NRstatus || RTE_E_TIMEOUT == BCM_Door_ToutStatus)
  {
    ValidFlag = TRUE;
  }
  return ValidFlag;
}

static void DriverSeatbeltInitial1Process(uint32 Speed)
{
  uint32 DeltaODO = 0;
  uint32 CurrentTrip = 0;
  
  if(Speed >= 25)
  {
    DriverSeatBeltLevel1StateMchine = Alert;
    DrvStBltLevel1 =  UI_WARNING_STATUS_OFF;
  }
  else if(Speed < 25 && 10 <= Speed)
  {
    DriverSeatBeltLevel1StateMchine = WaitAlert;
    
    if(DeltaODO >= DELTA_ODO_500M)
    {
      DrvStBltLevel1 = UI_WARNING_STATUS_ON;
    }
    else
    {
      DrvStBltLevel1 = UI_WARNING_STATUS_OFF;
    }
  }
  else//speed<10
  {
    DriverSeatBeltLevel1StateMchine = Initial;
  }
}

static void DriverSeatbeltInitial2Process(uint32 Speed)
{
  uint32 DeltaODO = 0;
  if(Speed >= 40)
  {
    DriverSeatBeltLevel2StateMchine = Alert;
    DrvStBltLevel2 =  UI_WARNING_STATUS_OFF;
  }
  else if(Speed < 40 && 10 <= Speed)
  {
    DriverSeatBeltLevel2StateMchine = WaitAlert;
    if(DeltaODO == DELTA_ODO_1000M)
    {
      
    }
    else if(fl_DriverSeatbeltHoldTimer90_Running == FALSE)
    {
      Rte_Call_rptimer_TmExt_Timer_Start(eTimerDRVSeatbeltWrn90s);
    }
    else
    {

    }
  }
  else//speed<10
  {
    DriverSeatBeltLevel2StateMchine = Initial;
    Rte_Call_rptimer_TmExt_Timer_Stop(eTimerDRVSeatbeltWrn90s);
  }
}

static void DriverSeatbeltWaitAlert1Process(uint32 Speed)
{
  uint32 DeltaODO = 0;
  if(Speed >= 25)
  {
    DriverSeatBeltLevel1StateMchine = Alert;
  }
  else if(Speed < 25 && 10 <= Speed && DeltaODO == DELTA_ODO_500M)
  {
    DriverSeatBeltLevel1StateMchine = WaitAlert;
    DrvStBltLevel1 = UI_WARNING_STATUS_ON;
  }
  else//speed<=10
  {
    DriverSeatBeltLevel1StateMchine = Initial;
  }
}

static void DriverSeatbeltWaitAlert2Process(uint32 Speed)
{
  uint32 DeltaODO = 0;
  Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerDRVSeatbeltWrn90s,TIMEOUT_90000MS,&fl_DriverSeatbeltHoldTimer90_Elapsed);
  if(Speed >= 40)
  {
    DriverSeatBeltLevel2StateMchine = Alert;
  }
  else if(Speed < 40 && 10 <= Speed && (fl_DriverSeatbeltHoldTimer90_Elapsed == TRUE || DeltaODO == DELTA_ODO_1000M))
  {
    DriverSeatBeltLevel2StateMchine = WaitAlert;
    DrvStBltLevel2 = UI_WARNING_STATUS_ON;
  }
  else//speed<10
  {
    DriverSeatBeltLevel2StateMchine = Initial;
  }
}

static void DriverSeatbeltAlert1Process(uint32 Speed)
{
  if(Speed >= 25)
  {
    DriverSeatBeltLevel1StateMchine = Alert;
    DrvStBltLevel1 = UI_WARNING_STATUS_ON;
  }
  else if(Speed < 25 && 10 <= Speed)
  {
  
  }
  else  //speed<10
  {
    DrvStBltLevel1 = UI_WARNING_STATUS_OFF;
  }
}

static void DriverSeatbeltAlert2Process(uint32 Speed)
{
  if(Speed >= 40)
  {
    DriverSeatBeltLevel2StateMchine = Alert;
    Rte_Call_rptimer_TmExt_Timer_Stop(eTimerDRVSeatbeltWrn90s);
    DrvStBltLevel2 = UI_WARNING_STATUS_ON;
  }
  else if(Speed < 40 && 10 <= Speed)
  {
  
  }
  else  //speed<10
  {
    DrvStBltLevel2 = UI_WARNING_STATUS_OFF;
  }
}

static void PassengerSeatbeltInitial1Process(uint32 Speed)
{
  if(Speed > 25)
  {
    PassengerSeatBeltLevel1StateMchine = Alert;
  }
  else if(Speed <= 25 && 10 < Speed)
  {
    PassengerSeatBeltLevel1StateMchine = WaitAlert;
  }
  else  //speed<=3
  {
    PassengerSeatBeltLevel1StateMchine = Initial;
    Rte_Call_rptimer_TmExt_Timer_Stop(eTimerPASSeatbeltWrn60s);
    PasStBltLevel1 = UI_WARNING_STATUS_OFF;
  }
}

static void PassengerSeatbeltInitial2Process(uint32 Speed)
{
  if(Speed >= 40)
  {
    PassengerSeatBeltLevel2StateMchine = Alert;
  }
  else if(Speed <= 10 && 40 < Speed)
  {
    PassengerSeatBeltLevel2StateMchine = WaitAlert;
  }
  else  //speed<10
  {
    PassengerSeatBeltLevel2StateMchine = Initial;
    Rte_Call_rptimer_TmExt_Timer_Stop(eTimerPASSeatbeltWrn60s);
    PasStBltLevel2 = UI_WARNING_STATUS_OFF;
  }
}

static void PassengerSeatbeltWaitAlert1Process(uint32 Speed)
{
  if(Speed > 25)
  {
    PassengerSeatBeltLevel1StateMchine = Alert;
  }
  else if(Speed <= 25 && 10 < Speed)
  {
    PassengerSeatBeltLevel1StateMchine = WaitAlert;
    if(fl_PassengerSeatbeltHoldTimer60_Running == FALSE)
    {
      Rte_Call_rptimer_TmExt_Timer_Start(eTimerPASSeatbeltWrn60s);
    }
    Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerPASSeatbeltWrn60s,TIMEOUT_60000MS,&fl_PassengerSeatbeltHoldTimer60_Elapsed);
    if(fl_PassengerSeatbeltHoldTimer60_Elapsed == TRUE)
    {
      PasStBltLevel1 = UI_WARNING_STATUS_ON;
    }
  }
  else  //speed<10
  {
    PassengerSeatBeltLevel1StateMchine = Initial;
  }
}

static void PassengerSeatbeltWaitAlert2Process(uint32 Speed)
{
  if(Speed >= 40)
  {
    PassengerSeatBeltLevel2StateMchine = Alert;
  }
  else if(Speed < 40 && 10 <= Speed)
  {
    PassengerSeatBeltLevel2StateMchine = WaitAlert;
    if(fl_PassengerSeatbeltHoldTimer90_Running == FALSE)
    {
      Rte_Call_rptimer_TmExt_Timer_Start(eTimerPASSeatbeltWrn90s);
    }
    Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerPASSeatbeltWrn90s,TIMEOUT_90000MS,&fl_PassengerSeatbeltHoldTimer90_Elapsed);
    if(fl_PassengerSeatbeltHoldTimer90_Elapsed == TRUE)
    {
      PasStBltLevel2 = UI_WARNING_STATUS_ON;
    }
  }
  else  //speed<10
  {
    PassengerSeatBeltLevel2StateMchine = Initial;
  }
}

static void PassengerSeatbeltAlert1Process(uint32 Speed)
{
  if(Speed >= 25)
  {
    PassengerSeatBeltLevel1StateMchine = Alert;
    Rte_Call_rptimer_TmExt_Timer_Stop(eTimerPASSeatbeltWrn60s);
    PasStBltLevel1 = UI_WARNING_STATUS_ON;
  }
  else if(Speed < 25 && 10 <= Speed)
  {
    PassengerSeatBeltLevel1StateMchine = WaitAlert;
  }
  else//speed<10
  {
    PassengerSeatBeltLevel1StateMchine = Initial;
  }
}

static void PassengerSeatbeltAlert2Process(uint32 Speed)
{
  if(Speed >= 40)
  {
    PassengerSeatBeltLevel2StateMchine = Alert;
    Rte_Call_rptimer_TmExt_Timer_Stop(eTimerPASSeatbeltWrn90s);
    PasStBltLevel2 = UI_WARNING_STATUS_ON;
  }
  else if(Speed < 40 && 10 < Speed)
  {
    PassengerSeatBeltLevel2StateMchine = WaitAlert;
  }
  else//speed<10
  {
    PassengerSeatBeltLevel2StateMchine = Initial;
  }
}


static void DriverSeatbeltWarningLevel1Process(uint32 Speed)
{
  uint8 SBR_Driver = 0;
  uint8 FrontLeftDoorSts = 0;
  
  if(TRUE != Message_0x310_Lose())
  {
    Rte_Read_EpbWarning_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&FrontLeftDoorSts);
  }
  Rte_Read_EpbWarning_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver(&SBR_Driver);
  
  if(TRUE != Message_0x350_Lose() && (Lamp_On == SBR_Driver && Door_Open != FrontLeftDoorSts))
  {
    if(Speed < 10)
    {
      DrvStBltLevel1 = UI_WARNING_STATUS_OFF;
    }
    else if(((10 <= Speed && 25 >= Speed && DeltaODO >= DELTA_ODO_500M) || (25 <= Speed)))
    { 
      DrvStBltLevel1 = UI_WARNING_STATUS_ON;
    }
    else
    {

    }
  }
  else
  {  
     DrvStBltLevel1 = UI_WARNING_STATUS_OFF;
  }
}

static void DriverSeatbeltWarningLevel2Process(uint32 Speed)
{
  uint8 SBR_Driver = 0;
  uint8 FrontLeftDoorSts = 0;

  if(TRUE != Message_0x310_Lose())
  {
    Rte_Read_EpbWarning_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&FrontLeftDoorSts);
  }
  
  Rte_Read_EpbWarning_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver(&SBR_Driver);
  
  if(TRUE != Message_0x350_Lose() && Lamp_On == SBR_Driver && Door_Open != FrontLeftDoorSts)
  {
    //need add voice judge condition
    if(Speed < 10)
    {
      DrvStBltLevel2 = UI_WARNING_STATUS_OFF;
    }
    else if(((10 <= Speed && 40 >= Speed && DeltaODO >= DELTA_ODO_1000M) || (40 <= Speed)))
    {
      DrvStBltLevel2 = UI_WARNING_STATUS_ON;
    }
    else
    {

    }
  }
  else
  {  
     DrvStBltLevel2 = UI_WARNING_STATUS_OFF;
  }
}

static void PassengerSeatbeltWarningLevel1Process(uint32 Speed)
{
  uint8 SBR_Passenger = 0;
  uint8 DoorStsFrntRi = 0;

  if(TRUE != Message_0x310_Lose())
  {
    Rte_Read_EpbWarning_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi(&DoorStsFrntRi);
  }
  
  Rte_Read_EpbWarning_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger(&SBR_Passenger);

  if(TRUE != Message_0x350_Lose()&& Lamp_On == SBR_Passenger&& Door_Open != DoorStsFrntRi)
  {
    if(10 > Speed)
    {
      PasStBltLevel1 = UI_WARNING_STATUS_OFF;
    }
    else if((10 <= Speed && 25 >= Speed && DeltaODO >= DELTA_ODO_500M) || (25 <= Speed))
    {
      PasStBltLevel1 = UI_WARNING_STATUS_ON;
    }
    else
    {

    }
  }
  else
  {  
     PasStBltLevel1 = UI_WARNING_STATUS_OFF;
  }
}

static void PassengerSeatbeltWarningLevel2Process(uint32 Speed)
{
  uint8 SBR_Passenger = 0;
  uint8 DoorStsFrntRi = 0;

  if(TRUE != Message_0x310_Lose())
  {
    Rte_Read_EpbWarning_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi(&DoorStsFrntRi);
  }
  Rte_Read_EpbWarning_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger(&SBR_Passenger);

  if(TRUE != Message_0x350_Lose()&& Lamp_On == SBR_Passenger&& Door_Open != DoorStsFrntRi)
  {
    /*need judge voice condition here*/
    if(10 > Speed)
    {
      PasStBltLevel2 = UI_WARNING_STATUS_OFF;
    }
    else if(((10 <= Speed && 40 >= Speed && DeltaODO >= DELTA_ODO_1000M) || (40 <= Speed)))
    {
      PasStBltLevel2 = UI_WARNING_STATUS_ON;
    }
    else
    {

    }
  }
  else
  {  
     PasStBltLevel2 = UI_WARNING_STATUS_OFF;
  }
}

static void OverSpeedWarningProcess(uint32 Speed)
{
  if( TRUE == Message_0x221_Lose())
  {
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_OVER_SPEED_ID, UI_WARNING_STATUS_OFF);
  }
  else
  {
    if(Speed>= DefaultSpeedLimit)
    {
      Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_OVER_SPEED_ID, UI_WARNING_STATUS_ON);
    }

    if(Speed < DefaultSpeedLimit- CANCEL_OSW_OFFSET)
    {
      Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_OVER_SPEED_ID, UI_WARNING_STATUS_OFF);
    }
  }
}
#endif

static void AirbagWarningProcess(void)
{ /*Modified by cpang1*/

  uint8 AirbagFailSts = (uint8)0;
  uint8 CurrentIGNSts = (uint8)eIGN_OFF;
  boolean AirbagDelayTimerEnd = (boolean)FALSE;
  Rte_Read_EpbWarning_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_AirBagFailSts(&AirbagFailSts);
  Rte_Read_EpbWarning_rpIgnState_IGNState(&CurrentIGNSts);

  if(((uint8)eIGN_RUN != LastIGNSts) && ((uint8)eIGN_RUN == CurrentIGNSts))
  {
    Rte_Call_rptimer_TmExt_Timer_Start(eTimerAirbagDelay10s);
  }

  Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerAirbagDelay10s,TIMEOUT_10S,&AirbagDelayTimerEnd);
  
  if((boolean)FALSE != AirbagDelayTimerEnd)
  { 
    if ( (boolean)TRUE != Message_0x350_Lose() )
    {
      if ( ((uint8)Airbag_Lamp_Flash == AirbagFailSts) || ((uint8)Airbag_Lamp_On == AirbagFailSts) )
      {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AIRBAG_FAULT_ID,UI_WARNING_STATUS_ON);
      }
      else
      {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AIRBAG_FAULT_ID,UI_WARNING_STATUS_OFF);
      }
    }
    else
    {
      Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AIRBAG_FAULT_ID,UI_WARNING_STATUS_OFF);
    }
  }
  else
  {
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AIRBAG_FAULT_ID,UI_WARNING_STATUS_OFF);
  }
}

static void DriverSeatbeltWarningProcess(void)
{
  uint16  ABS_VehSpdLgt_Value            = (uint16)0;
  uint8   SRS_SBR_Driver_Sts             = (uint8)0;
  boolean DriverSeatbeltWrn_IsElapsed    = (boolean)FALSE;
  boolean DriverSeatbeltWrn_IsStarted    = (boolean)FALSE;

  //uint8 Drvstblt = 0;
  //Rte_Read_EpbWarning_rpSR_TISpdMdl_Send_DigitalValue_Value(&SpeedU32);
  //SpeedU32/= SPEED_FACTOR;
  //DriverSeatbeltWarningLevel1Process(SpeedU32);
  //DriverSeatbeltWarningLevel2Process(SpeedU32);
  //Drvstblt = DrvStBltLevel1|DrvStBltLevel2;
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVER_SEATBELT_WARNING_ID,Drvstblt);

  Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&ABS_VehSpdLgt_Value);
  //Rte_Read_EpbWarning_rpSR_TISpdMdl_Send_DigitalValue_Value(&ABS_VehSpdLgt_Value);
  Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver(&SRS_SBR_Driver_Sts);

  //ABS_VehSpdLgt_Value /= (uint32)SPEED_FACTOR;
  if ( ((boolean)FALSE != Message_0x221_Lose()) || ((boolean)FALSE != Message_0x350_Lose()) )
  {
     Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVER_SEATBELT_WARNING_ID,UI_WARNING_STATUS_OFF);
     DriverSeatbeltWrn_ActvSts = (boolean)FALSE;
  }
  else
  { 
    if ( (uint16)INVALID_SPEED_VALUE == ABS_VehSpdLgt_Value )
    {
      ABS_VehSpdLgt_Value = (uint16)0;
    }

    if ( (SPEED_RELEASES_UPPER_THRESHOLD < ABS_VehSpdLgt_Value) && ((uint8)Lamp_On == SRS_SBR_Driver_Sts) )
    {
      Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVER_SEATBELT_WARNING_ID,UI_WARNING_STATUS_ON);
      DriverSeatbeltWrn_ActvSts = (boolean)TRUE;
    }
    else if ( (SPEED_RELEASES_LOWER_THRESHOLD < ABS_VehSpdLgt_Value) &&\
              (SPEED_RELEASES_UPPER_THRESHOLD >= ABS_VehSpdLgt_Value)&&\
              ((uint8)Lamp_On == SRS_SBR_Driver_Sts) )
    {
      Rte_Call_rptimer_TmExt_Timer_IsStarted((HTimer)eTimerDRVSeatbeltWrn60s,&DriverSeatbeltWrn_IsStarted);
      Rte_Call_rptimer_TmExt_Timer_IsElapsed((HTimer)eTimerDRVSeatbeltWrn60s,TIMEOUT_60000MS,&DriverSeatbeltWrn_IsElapsed);
      if ( ((boolean)FALSE == DriverSeatbeltWrn_IsStarted) && ((boolean)FALSE == DriverSeatbeltWrn_ActvSts) )
      {
        Rte_Call_rptimer_TmExt_Timer_Start((HTimer)eTimerDRVSeatbeltWrn60s);
      }
      else
      {
        if ( (boolean)FALSE != DriverSeatbeltWrn_IsElapsed)
        {
          Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVER_SEATBELT_WARNING_ID,UI_WARNING_STATUS_ON);
          Rte_Call_rptimer_TmExt_Timer_Stop((HTimer)eTimerDRVSeatbeltWrn60s);
          DriverSeatbeltWrn_ActvSts = (boolean)TRUE;
        }
      }
    }
    else
    {
      Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DRIVER_SEATBELT_WARNING_ID,UI_WARNING_STATUS_OFF);
      DriverSeatbeltWrn_ActvSts = (boolean)FALSE;
    }
  } 
}

#if 0
static void PassengerSeatbeltWarningProcess(void)
{
  uint32 SpeedU32 = 0;
  uint8 Psgstblt = 0;
  Rte_Read_EpbWarning_rpSR_TISpdMdl_Send_DigitalValue_Value(&SpeedU32);
  SpeedU32/= SPEED_FACTOR;
  PassengerSeatbeltWarningLevel1Process(SpeedU32);
  PassengerSeatbeltWarningLevel2Process(SpeedU32);
  Psgstblt = PasStBltLevel1|PasStBltLevel2;
  Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_PASSENGER_SEATBELT_WARNING_ID,Psgstblt);
}

static void ReadLastSingleTrip(void)
{
  uint8 IGNSts = 0;
  Rte_Read_EpbWarning_rpIgnState_IGNState(&IGNSts);
  if(eIGN_RUN== IGNSts && eIGN_RUN != LastIGNSts)
  {
    /*read last single trip (uint: m)*/
  }
}

static void CalculateDeltaODO(void)
{
  uint32 CurrentSingleTrip = 0;
  ReadLastSingleTrip();
  /*need read current single trip here*/
  
  DeltaODO = CurrentSingleTrip - LastSingleTrip;
}
#endif
