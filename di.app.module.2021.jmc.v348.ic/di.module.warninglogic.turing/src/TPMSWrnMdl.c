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
#include "Rte_TPMSWrnMdl.h"

#include "system.h"
#include "CmpLib.h"
#include "tpms_wrn_cfg.h"
#include "WarningCtrl.h"
#include "hmi_tpms.h"
/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(TPMSWrnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
typedef enum
{
	NO_WARNING = 0,
	HIGH_PRESSURE_WARNING,
	LOW_PRESSURE_WARNING,
	QUIK_LEAKAGE_WARNING, //reserve
	LOST_SENSOR_WARNING,
	SENSOR_BATTERY_LOW_WARNING,
	SENSOR_INVALID,  //reserve
}TPMS_PRESSURE_WARNING_T;

typedef enum
{
	NO_TIRE = 0,
	LF_TIRE ,
	RF_TIRE ,
	RR_TIRE ,
	LR_TIRE 
}TIRE_POSITION_T;


#define HIGH_PRESSURE_BIT		((uint16)0x01)
#define LOW_PRESSURE_BIT		((uint16)0x02)
#define LOST_SENSOR_BIT			((uint16)0x04)
#define SENSOR_BATTERY_LOW_BIT	((uint16)0x08)
#define HIGH_TEMP_BIT			((uint16)0x10)
#define TIRE_PRESSURE_INVALID_BIT		((uint16)0x20)
#define TIRE_TEMP_INVALID_BIT	((uint16)0x40)
#define SIGNAL_TIMEOUT_BIT		((uint16)0x80)
#define UNLEARN_BIT				((uint16)0x100)
#define MESSAGE_NEVER_REV_BIT	((uint16)0x200)
#define SYSTEM_FAILURE_BIT		((uint16)0x400)
#define SENSOR_INVALID_BIT   ((uint16)0x800)
#define MESSAGE_INVALID			((uint8)0xFF)
#define ALL_TIRE_UNLEARN_BIT   ((uint16)0x1000)
#define TIRE_NUM            (4)
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint16 TPMS_LF_Tire_Status = 0;
static uint16 TPMS_RF_Tire_Status = 0;
static uint16 TPMS_LR_Tire_Status = 0;
static uint16 TPMS_RR_Tire_Status = 0;
uint8 TirePos = 0;
static uint8 u8TPMS_LF_pressure_Value = 0;  
static uint8 u8TPMS_RF_pressure_Value = 0;
static uint8 u8TPMS_RR_pressure_Value = 0;
static uint8 u8TPMS_LR_pressure_Value = 0;

static uint8 u8TPMS_LF_Tire_Temp = 60;
static uint8 u8TPMS_RF_Tire_Temp = 60;
static uint8 u8TPMS_RR_Tire_Temp = 60;
static uint8 u8TPMS_LR_Tire_Temp = 60;

static void TPMS_Warning_init(void);
static void TPMS_Message_Process(void);
static void fun_TPMS_Warning_Process(void);
static uint8 message_invalid(void);
static void TPMS_system_failure_process(void);
static void TPMS_tire_unlearned_process(void);
static void TPMS_tire_pressure_invalid_process(void);
static void TPMS_high_temperature_process(void);
static void TPMS_Set_Pressure_Warning_Status(void);
//static void TPMS_sensor_invalid_process(void);
static void TPMS_SignalTimeOutProcess(void);
static void TPMSPressureSingnalWarningProcess(void);
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
  TPMS_Warning_init();
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
  TPMS_Warning_init();
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

static Std_ReturnType CmpActive( void )
{
  //    EBatteryState fl_Battery_state;
  //	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);

  	uint8 tpms_eol;
  	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_TPMS, &tpms_eol);
	if(1 == tpms_eol)
	{
		fun_TPMS_Warning_Process();
	}
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

static uint8 message_invalid(void)
{
  uint8 u8MsgNR = 0;
  uint8 u8MsgTout = 0;
  uint8 ret = FALSE;
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_NR(&u8MsgNR);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tout(&u8MsgTout);

  if((RTE_E_NEVER_RECEIVED == u8MsgNR))
  {
    /*set never receive bit*/
    TPMS_LF_Tire_Status = MESSAGE_NEVER_REV_BIT;
    TPMS_RF_Tire_Status = MESSAGE_NEVER_REV_BIT;
    TPMS_LR_Tire_Status = MESSAGE_NEVER_REV_BIT;
    TPMS_RR_Tire_Status = MESSAGE_NEVER_REV_BIT;
	  TirePos = 0;
    ret =  TRUE;
  }
  else if(RTE_E_TIMEOUT == u8MsgTout)
  {
    /*set time out bit*/
    TPMS_LF_Tire_Status = SIGNAL_TIMEOUT_BIT;
    TPMS_RF_Tire_Status = SIGNAL_TIMEOUT_BIT;
    TPMS_LR_Tire_Status = SIGNAL_TIMEOUT_BIT;
    TPMS_RR_Tire_Status = SIGNAL_TIMEOUT_BIT;
   
    //signal_time_out_prev_flag = TRUE;
    
	  TirePos = 0;
    ret =  TRUE;
  }
  else
  {
     
    /*when signal recover,clear these exception data bit */
    /*Clear Timeout bit*/
    TPMS_LF_Tire_Status &= ~SIGNAL_TIMEOUT_BIT;
    TPMS_RF_Tire_Status &= ~SIGNAL_TIMEOUT_BIT;
    TPMS_LR_Tire_Status &= ~SIGNAL_TIMEOUT_BIT;
    TPMS_RR_Tire_Status &= ~SIGNAL_TIMEOUT_BIT;
    
    /* clear never receive bit*/
    TPMS_LF_Tire_Status &= ~MESSAGE_NEVER_REV_BIT;
    TPMS_RF_Tire_Status &= ~MESSAGE_NEVER_REV_BIT;
    TPMS_LR_Tire_Status &= ~MESSAGE_NEVER_REV_BIT;
    TPMS_RR_Tire_Status &= ~MESSAGE_NEVER_REV_BIT;
    //Read tire position for menu
    Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position(&TirePos);
  }
  return ret;
}



static void TPMS_Warning_init(void)
{
  TPMS_LF_Tire_Status = FALSE;
  TPMS_RF_Tire_Status = FALSE;
  TPMS_LR_Tire_Status = FALSE;
  TPMS_RR_Tire_Status = FALSE;
  TirePos = 0;
  //signal_time_out_prev_flag = FALSE;
 

  // clear_after_ack_flag = FALSE; 
  //Rte_Call_rptimer_TmExt_Timer_Stop(eTimerTpmsWrn);

  for(int i = 0; i < UI_WARNING_TOTAL; i++)
  {
    SET_TPMS_WRN_OFF(i);
    INIT_TPMS_OLD_STS(i);
  }
}



static void TPMS_Message_Process(void)
{
   TPMS_LF_Tire_Status &= ~MESSAGE_NEVER_REV_BIT;
   TPMS_RF_Tire_Status &= ~MESSAGE_NEVER_REV_BIT;
   TPMS_LR_Tire_Status &= ~MESSAGE_NEVER_REV_BIT;
   TPMS_RR_Tire_Status &= ~MESSAGE_NEVER_REV_BIT;
}

static void fun_TPMS_Warning_Process(void)
{
  uint8 fl_ret_u8 = TRUE;
  eIgnState IGN_state = eIGN_OFF;

  Rte_Read_rpIgnState_IGNState(&IGN_state);

  if(eIGN_RUN == IGN_state)
  {
    fl_ret_u8 = message_invalid();
    /*normal logic*/
    if(fl_ret_u8 == FALSE)
    {
      TPMS_Message_Process();
      TPMS_system_failure_process();
      TPMS_tire_unlearned_process();
      TPMSPressureSingnalWarningProcess(); //Hight_press/Low_press/sensor_lost/sensor_batt_low
      TPMS_high_temperature_process();
      TPMS_tire_pressure_invalid_process();
      //TPMS_sensor_invalid_process doesn't gernate warning,it only generate data for menu.
    //  TPMS_sensor_invalid_process();
      // TPMS_Get_Pressure_Warning_Status();/*get warning status and send them to GDT*/
    }
    else
    {
      /*exception logic*/
      /*Just shutdown TPMS warning and clear timer here, don't clear all global varibles*/
      TPMS_SignalTimeOutProcess();
    }
  }
  else //IGN_state == eIGN_OFF
  {
    TPMS_Warning_init();
  }
  TPMS_Set_Pressure_Warning_Status();               /*get warning status and send them to GDT*/
}

static void TPMS_SignalTimeOutProcess(void)
{
  // clear_after_ack_flag = FALSE; 
  //Rte_Call_rptimer_TmExt_Timer_Stop(eTimerTpmsWrn);
  for(int i = 0; i < UI_WARNING_TOTAL; i++)
  {
    SET_TPMS_WRN_OFF(i);
  }
}

static void TPMS_Set_Pressure_Warning_Status(void)
{
  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_FL_Warning_St(TPMS_LF_Tire_Status);
  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_RL_Warning_St(TPMS_LR_Tire_Status);
  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_FR_Warning_St(TPMS_RF_Tire_Status);
  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_RR_Warning_St(TPMS_RR_Tire_Status);

  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_FL_Temp_Value(u8TPMS_LF_Tire_Temp);
  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_FR_Temp_Value(u8TPMS_RF_Tire_Temp);
  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_RR_Temp_Value(u8TPMS_RR_Tire_Temp);
  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_RL_Temp_Value(u8TPMS_LR_Tire_Temp);	

  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_FL_Pressure_Value(u8TPMS_LF_pressure_Value);
  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_RL_Pressure_Value(u8TPMS_LR_pressure_Value);
  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_FR_Pressure_Value(u8TPMS_RF_pressure_Value);
  Rte_Write_ppSR_TIGdt_TPMSWrnMdl_RR_Pressure_Value(u8TPMS_RR_pressure_Value);
}

static void TPMS_system_failure_process(void)
{
  uint8 u8TPMSWarningSt = 0;
  //uint8 u8NotFSt = 0;

  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status(&u8TPMSWarningSt);

  // u8NotFSt = CWarningCtrl_APP_get_notified_warning_status(WARN_TPMS_SYSTEM_FAILURE_ID);
  if(cWRN_ACTIVE == u8TPMSWarningSt)
  {
    TPMS_LF_Tire_Status |= SYSTEM_FAILURE_BIT;
    TPMS_RF_Tire_Status |= SYSTEM_FAILURE_BIT;
    TPMS_RR_Tire_Status |= SYSTEM_FAILURE_BIT;
    TPMS_LR_Tire_Status |= SYSTEM_FAILURE_BIT;
    SET_TPMS_WRN_ON(TPMS_SYSTEM_FAILURE);

  }
  else
  {
    TPMS_LF_Tire_Status &= ~SYSTEM_FAILURE_BIT;
    TPMS_RF_Tire_Status &= ~SYSTEM_FAILURE_BIT;
    TPMS_RR_Tire_Status &= ~SYSTEM_FAILURE_BIT;
    TPMS_LR_Tire_Status &= ~SYSTEM_FAILURE_BIT;
    SET_TPMS_WRN_OFF(TPMS_SYSTEM_FAILURE);
  }
}



static void TPMS_tire_unlearned_process(void)
{
  
  uint8 u8TPMS_Tire_Pos = 0;
  uint8 u8TPMSWarningSt = 0;
  
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position(&u8TPMS_Tire_Pos);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status(&u8TPMSWarningSt);

 	 if (0x2== u8TPMSWarningSt)
  	{
		 TPMS_LF_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;
		 TPMS_RF_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;
		 TPMS_RR_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;
		 TPMS_LR_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;
		 
		 TPMS_LF_Tire_Status &= ~UNLEARN_BIT;
		 TPMS_RF_Tire_Status &= ~UNLEARN_BIT;
		 TPMS_RR_Tire_Status &= ~UNLEARN_BIT;
		 TPMS_LR_Tire_Status &= ~UNLEARN_BIT;  
		 
		 SET_TPMS_WRN_OFF(TPMS_TIRE_UNLEARN);
		 SET_TPMS_WRN_OFF(TPMS_LF_TIRE_UNLEARN);
		 SET_TPMS_WRN_OFF(TPMS_RF_TIRE_UNLEARN);
		 SET_TPMS_WRN_OFF(TPMS_RR_TIRE_UNLEARN);
		 SET_TPMS_WRN_OFF(TPMS_LR_TIRE_UNLEARN);

	  switch(u8TPMS_Tire_Pos)
	  {		  
		  case NO_TIRE:
		  	
			  	  TPMS_LF_Tire_Status |= ALL_TIRE_UNLEARN_BIT;
				  TPMS_RF_Tire_Status |= ALL_TIRE_UNLEARN_BIT;
				  TPMS_RR_Tire_Status |= ALL_TIRE_UNLEARN_BIT; 
				  TPMS_LR_Tire_Status |= ALL_TIRE_UNLEARN_BIT;	  
				  SET_TPMS_WRN_ON(TPMS_TIRE_UNLEARN);

	
			 break;
		  case LF_TIRE:
	  			  

					  TPMS_LF_Tire_Status |= UNLEARN_BIT;
					  SET_TPMS_WRN_ON(TPMS_LF_TIRE_UNLEARN);
	
				  break;
	  
		  case RF_TIRE:
	  

					  TPMS_RF_Tire_Status |= UNLEARN_BIT;
					  SET_TPMS_WRN_ON(TPMS_RF_TIRE_UNLEARN);
							  
				  break;							  
		  case RR_TIRE:
	  
	
					  TPMS_RR_Tire_Status |= UNLEARN_BIT;	
					   SET_TPMS_WRN_ON(TPMS_RR_TIRE_UNLEARN);
	  
				  break;
		  case LR_TIRE:
	  

					  TPMS_LR_Tire_Status |= UNLEARN_BIT;	
					  SET_TPMS_WRN_ON(TPMS_LR_TIRE_UNLEARN);
   
				  break;
		  default:
		  	
				  TPMS_LF_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;
				  TPMS_RF_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;
				  TPMS_RR_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;
				  TPMS_LR_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;

				  TPMS_LF_Tire_Status &= ~UNLEARN_BIT;
				  TPMS_RF_Tire_Status &= ~UNLEARN_BIT;
				  TPMS_RR_Tire_Status &= ~UNLEARN_BIT;
				  TPMS_LR_Tire_Status &= ~UNLEARN_BIT;	

				  SET_TPMS_WRN_OFF(TPMS_TIRE_UNLEARN);
				  SET_TPMS_WRN_OFF(TPMS_LF_TIRE_UNLEARN);
				  SET_TPMS_WRN_OFF(TPMS_RF_TIRE_UNLEARN);
				  SET_TPMS_WRN_OFF(TPMS_RR_TIRE_UNLEARN);
				  SET_TPMS_WRN_OFF(TPMS_LR_TIRE_UNLEARN);
				  
				  break;															  
	  }

  	}
	else 
	{
		  TPMS_LF_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;
		  TPMS_RF_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;
		  TPMS_RR_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;
		  TPMS_LR_Tire_Status &= ~ALL_TIRE_UNLEARN_BIT;

		  TPMS_LF_Tire_Status &= ~UNLEARN_BIT;
		  TPMS_RF_Tire_Status &= ~UNLEARN_BIT;
		  TPMS_RR_Tire_Status &= ~UNLEARN_BIT;
		  TPMS_LR_Tire_Status &= ~UNLEARN_BIT;	

		  SET_TPMS_WRN_OFF(TPMS_TIRE_UNLEARN);
		  SET_TPMS_WRN_OFF(TPMS_LF_TIRE_UNLEARN);
		  SET_TPMS_WRN_OFF(TPMS_RF_TIRE_UNLEARN);
		  SET_TPMS_WRN_OFF(TPMS_RR_TIRE_UNLEARN);
		  SET_TPMS_WRN_OFF(TPMS_LR_TIRE_UNLEARN);
	}
	  
}

static void TPMS_tire_pressure_invalid_process(void)
{
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure(&u8TPMS_LF_pressure_Value);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure(&u8TPMS_LR_pressure_Value);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure(&u8TPMS_RF_pressure_Value);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure(&u8TPMS_RR_pressure_Value);

  if(MESSAGE_INVALID == u8TPMS_LF_pressure_Value)
  {
    TPMS_LF_Tire_Status |= TIRE_PRESSURE_INVALID_BIT;
  }
  else
  {
    TPMS_LF_Tire_Status &= ~TIRE_PRESSURE_INVALID_BIT;

    // Rte_Write_ppSR_TIGdt_TPMSWrnMdl_FL_Pressure_Value(u8TPMS_LF_pressure_Value);
  }

  if(MESSAGE_INVALID == u8TPMS_LR_pressure_Value)
  {
    TPMS_LR_Tire_Status |= TIRE_PRESSURE_INVALID_BIT;
  }
  else
  {
    TPMS_LR_Tire_Status &= ~TIRE_PRESSURE_INVALID_BIT;

    // Rte_Write_ppSR_TIGdt_TPMSWrnMdl_RL_Pressure_Value(u8TPMS_LR_pressure_Value);
  }

  if(MESSAGE_INVALID == u8TPMS_RF_pressure_Value)
  {
    TPMS_RF_Tire_Status |= TIRE_PRESSURE_INVALID_BIT;
  }
  else
  {
    TPMS_RF_Tire_Status &= ~TIRE_PRESSURE_INVALID_BIT;

    // Rte_Write_ppSR_TIGdt_TPMSWrnMdl_FR_Pressure_Value(u8TPMS_RF_pressure_Value);
  }

  if(MESSAGE_INVALID == u8TPMS_RR_pressure_Value)
  {
    TPMS_RR_Tire_Status |= TIRE_PRESSURE_INVALID_BIT;
  }
  else
  {
    TPMS_RR_Tire_Status &= ~TIRE_PRESSURE_INVALID_BIT;

    // Rte_Write_ppSR_TIGdt_TPMSWrnMdl_RR_Pressure_Value(u8TPMS_RR_pressure_Value);
  }
}


/*
static void TPMS_sensor_invalid_process(void)
{
  uint8 u8TPMS_LF_pressure_St = 0;
  uint8 u8TPMS_RF_pressure_St = 0;
  uint8 u8TPMS_RR_pressure_St = 0;
  uint8 u8TPMS_LR_pressure_St = 0;

  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning(&u8TPMS_LF_pressure_St);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning(&u8TPMS_LR_pressure_St);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning(&u8TPMS_RF_pressure_St);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning(&u8TPMS_RR_pressure_St);

  if(SENSOR_INVALID == u8TPMS_LF_pressure_St)
  {
    TPMS_LF_Tire_Status |= SENSOR_INVALID_BIT;
  }
  else
  {
    TPMS_LF_Tire_Status &= ~SENSOR_INVALID_BIT;
  }

  if(SENSOR_INVALID == u8TPMS_RF_pressure_St)
  {
    TPMS_RF_Tire_Status |= SENSOR_INVALID_BIT;
  }
  else
  {
    TPMS_RF_Tire_Status &= ~SENSOR_INVALID_BIT;
  }

  if(SENSOR_INVALID == u8TPMS_RR_pressure_St)
  {
    TPMS_RR_Tire_Status |= SENSOR_INVALID_BIT;
  }
  else
  {
    TPMS_RR_Tire_Status &= ~SENSOR_INVALID_BIT;
  }

  if(SENSOR_INVALID == u8TPMS_LR_pressure_St)
  {
    TPMS_LR_Tire_Status |= SENSOR_INVALID_BIT;
  }
  else
  {
    TPMS_LR_Tire_Status &= ~SENSOR_INVALID_BIT;
  }

}
*/

static void TPMSPressureSingnalWarningProcess(void)
{
  uint8 u8TPMSpressure_St[TIRE_NUM] = {0};
  uint16 *TireStatus[TIRE_NUM] = {&TPMS_LF_Tire_Status,&TPMS_RF_Tire_Status,&TPMS_LR_Tire_Status,&TPMS_RR_Tire_Status};
  uint8 TireRowIndex = 0;
 
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning(&u8TPMSpressure_St[0]);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning(&u8TPMSpressure_St[1]);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning(&u8TPMSpressure_St[2]);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning(&u8TPMSpressure_St[3]);
  for(TireRowIndex = 0; TireRowIndex < TIRE_NUM; TireRowIndex++)
  {
    switch(u8TPMSpressure_St[TireRowIndex])
    {
      /*NO_WARNING = 1*/
      case NO_WARNING:
       (*TireStatus[TireRowIndex]) &= ~HIGH_PRESSURE_BIT;
       (*TireStatus[TireRowIndex]) &= ~LOW_PRESSURE_BIT;
       (*TireStatus[TireRowIndex]) &= ~LOST_SENSOR_BIT;
       (*TireStatus[TireRowIndex]) &= ~SENSOR_BATTERY_LOW_BIT;
        break;
      /*HIGH_PRESSURE_WARNING = 1, */
     
      case HIGH_PRESSURE_WARNING:
        (*TireStatus[TireRowIndex]) |= HIGH_PRESSURE_BIT;
        (*TireStatus[TireRowIndex]) &= ~LOW_PRESSURE_BIT;
        (*TireStatus[TireRowIndex]) &= ~LOST_SENSOR_BIT;
        (*TireStatus[TireRowIndex]) &= ~SENSOR_BATTERY_LOW_BIT;
        break;
      /*LOW_PRESSURE_WARNING = 2,*/
      case LOW_PRESSURE_WARNING:
        (*TireStatus[TireRowIndex]) &= ~HIGH_PRESSURE_BIT;
        (*TireStatus[TireRowIndex]) |= LOW_PRESSURE_BIT;
        (*TireStatus[TireRowIndex]) &= ~LOST_SENSOR_BIT;
        (*TireStatus[TireRowIndex]) &= ~SENSOR_BATTERY_LOW_BIT;
  //      SET_TPMS_WRN_ON(TPMS_PRESSURE_LOW);
        break;
      /*LOW_PRESSURE_WARNING = 4,*/
      case LOST_SENSOR_WARNING:
        (*TireStatus[TireRowIndex]) &= ~HIGH_PRESSURE_BIT;
        (*TireStatus[TireRowIndex]) &= ~LOW_PRESSURE_BIT;
        (*TireStatus[TireRowIndex]) |= LOST_SENSOR_BIT;
        (*TireStatus[TireRowIndex]) &= ~SENSOR_BATTERY_LOW_BIT;
    //    SET_TPMS_WRN_ON(TPMS_SENSOR_DISCONNECTED);
        break;
      /*SENSOR_BATTERY_LOW_WARNING = 5,*/
      case SENSOR_BATTERY_LOW_WARNING:
        (*TireStatus[TireRowIndex]) &= ~HIGH_PRESSURE_BIT;
        (*TireStatus[TireRowIndex]) &= ~LOW_PRESSURE_BIT;
        (*TireStatus[TireRowIndex]) &= ~ LOST_SENSOR_BIT;
        (*TireStatus[TireRowIndex]) |= SENSOR_BATTERY_LOW_BIT;
   //     SET_TPMS_WRN_ON(TPMS_LOW_POWER);
        break;
      default:
        (*TireStatus[TireRowIndex]) &= ~HIGH_PRESSURE_BIT;
        (*TireStatus[TireRowIndex]) &= ~LOW_PRESSURE_BIT;
        (*TireStatus[TireRowIndex]) &= ~LOST_SENSOR_BIT;
        (*TireStatus[TireRowIndex]) &= ~SENSOR_BATTERY_LOW_BIT;
        break;
    }
  }
  
  /* TPMS_LF_Tire_Status  */
	  if((TESTBIT(TPMS_LF_Tire_Status, HIGH_PRESSURE_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_LF_PRESSURE_HIGH);
	      SET_TPMS_WRN_OFF(TPMS_LF_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_LF_SENSOR_DISCONNECTED);
		  SET_TPMS_WRN_OFF(TPMS_LF_SENSOR_LOW_POWER);

	  }
	  else if((TESTBIT(TPMS_LF_Tire_Status, LOW_PRESSURE_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_LF_PRESSURE_LOW);
	      SET_TPMS_WRN_OFF(TPMS_LF_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_LF_SENSOR_DISCONNECTED);
		  SET_TPMS_WRN_OFF(TPMS_LF_SENSOR_LOW_POWER);

	  }
	  else if((TESTBIT(TPMS_LF_Tire_Status, LOST_SENSOR_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_LF_SENSOR_DISCONNECTED);
	      SET_TPMS_WRN_OFF(TPMS_LF_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_LF_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_LF_SENSOR_LOW_POWER);

	  }
	  else if((TESTBIT(TPMS_LF_Tire_Status, SENSOR_BATTERY_LOW_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_LF_SENSOR_LOW_POWER);
	      SET_TPMS_WRN_OFF(TPMS_LF_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_LF_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_LF_SENSOR_DISCONNECTED);

	  }   
	  else
	  {
	      SET_TPMS_WRN_OFF(TPMS_LF_PRESSURE_HIGH);
	      SET_TPMS_WRN_OFF(TPMS_LF_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_LF_SENSOR_DISCONNECTED);
		  SET_TPMS_WRN_OFF(TPMS_LF_SENSOR_LOW_POWER);	  
	  }
  
	  
	  /* TPMS_RF_Tire_Status  */
	  if((TESTBIT(TPMS_RF_Tire_Status, HIGH_PRESSURE_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_RF_PRESSURE_HIGH);
	      SET_TPMS_WRN_OFF(TPMS_RF_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_RF_SENSOR_DISCONNECTED);
		  SET_TPMS_WRN_OFF(TPMS_RF_SENSOR_LOW_POWER);		  
	  }
	  else if((TESTBIT(TPMS_RF_Tire_Status, LOW_PRESSURE_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_RF_PRESSURE_LOW);
	      SET_TPMS_WRN_OFF(TPMS_RF_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_RF_SENSOR_DISCONNECTED);
		  SET_TPMS_WRN_OFF(TPMS_RF_SENSOR_LOW_POWER);	

	  }
	  else if((TESTBIT(TPMS_RF_Tire_Status, LOST_SENSOR_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_RF_SENSOR_DISCONNECTED);
	      SET_TPMS_WRN_OFF(TPMS_RF_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_RF_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_RF_SENSOR_LOW_POWER);	

	  }
	  else if((TESTBIT(TPMS_RF_Tire_Status, SENSOR_BATTERY_LOW_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_RF_SENSOR_LOW_POWER);
	      SET_TPMS_WRN_OFF(TPMS_RF_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_RF_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_RF_SENSOR_DISCONNECTED);	
	  }
	  else
	  {
	      SET_TPMS_WRN_OFF(TPMS_RF_SENSOR_LOW_POWER);
	      SET_TPMS_WRN_OFF(TPMS_RF_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_RF_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_RF_SENSOR_DISCONNECTED); 
	  }
  
	  /* TPMS_LR_Tire_Status  */
	  if((TESTBIT(TPMS_LR_Tire_Status, HIGH_PRESSURE_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_LR_PRESSURE_HIGH);
	      SET_TPMS_WRN_OFF(TPMS_LR_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_LR_SENSOR_DISCONNECTED);
		  SET_TPMS_WRN_OFF(TPMS_LR_SENSOR_LOW_POWER);

	  }
	  else if((TESTBIT(TPMS_LR_Tire_Status, LOW_PRESSURE_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_LR_PRESSURE_LOW);
	      SET_TPMS_WRN_OFF(TPMS_LR_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_LR_SENSOR_DISCONNECTED);
		  SET_TPMS_WRN_OFF(TPMS_LR_SENSOR_LOW_POWER);

	  }
	  else if((TESTBIT(TPMS_LR_Tire_Status, LOST_SENSOR_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_LR_SENSOR_DISCONNECTED);
	      SET_TPMS_WRN_OFF(TPMS_LR_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_LR_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_LR_SENSOR_LOW_POWER);

	  }
	  else if((TESTBIT(TPMS_LR_Tire_Status, SENSOR_BATTERY_LOW_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_LR_SENSOR_LOW_POWER);
	      SET_TPMS_WRN_OFF(TPMS_LR_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_LR_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_LR_SENSOR_DISCONNECTED);

	  }   
	  else
	  {
	      SET_TPMS_WRN_OFF(TPMS_LR_SENSOR_LOW_POWER);
	      SET_TPMS_WRN_OFF(TPMS_LR_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_LR_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_LR_SENSOR_DISCONNECTED);	  
	  }
  
	  /* TPMS_RR_Tire_Status  */
	  if((TESTBIT(TPMS_RR_Tire_Status, HIGH_PRESSURE_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_RR_PRESSURE_HIGH);
	      SET_TPMS_WRN_OFF(TPMS_RR_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_RR_SENSOR_DISCONNECTED);
		  SET_TPMS_WRN_OFF(TPMS_RR_SENSOR_LOW_POWER); 
	  }
	  else if((TESTBIT(TPMS_RR_Tire_Status, LOW_PRESSURE_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_RR_PRESSURE_LOW);
	      SET_TPMS_WRN_OFF(TPMS_RR_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_RR_SENSOR_DISCONNECTED);
		  SET_TPMS_WRN_OFF(TPMS_RR_SENSOR_LOW_POWER); 

	  }
	  else if((TESTBIT(TPMS_RR_Tire_Status, LOST_SENSOR_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_RR_SENSOR_DISCONNECTED );
	      SET_TPMS_WRN_OFF(TPMS_RR_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_RR_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_RR_SENSOR_LOW_POWER); 

	  }
	  else if((TESTBIT(TPMS_RR_Tire_Status, SENSOR_BATTERY_LOW_BIT)))
	  {
	      SET_TPMS_WRN_ON(TPMS_RR_SENSOR_LOW_POWER);
	      SET_TPMS_WRN_OFF(TPMS_RR_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_RR_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_RR_SENSOR_DISCONNECTED); 

	  }   
	  else
	  {
	      SET_TPMS_WRN_OFF(TPMS_RR_SENSOR_LOW_POWER);
	      SET_TPMS_WRN_OFF(TPMS_RR_PRESSURE_HIGH);
		  SET_TPMS_WRN_OFF(TPMS_RR_PRESSURE_LOW);
		  SET_TPMS_WRN_OFF(TPMS_RR_SENSOR_DISCONNECTED); 	  
	  }



}

static void TPMS_high_temperature_process(void)
{
  uint8 u8TPMS_Tire_Pos = 0;
  uint8 u8TPMS_Tire_Temp_St = 0;
  
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position(&u8TPMS_Tire_Pos);
  Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Temperature_Warning(&u8TPMS_Tire_Temp_St);


        TPMS_LF_Tire_Status &= ~HIGH_TEMP_BIT;
		TPMS_RF_Tire_Status &= ~HIGH_TEMP_BIT;
		TPMS_RR_Tire_Status &= ~HIGH_TEMP_BIT;
		TPMS_LR_Tire_Status &= ~HIGH_TEMP_BIT;
		SET_TPMS_WRN_OFF(TPMS_LF_HIGH_TEMPERATURE);				       
		SET_TPMS_WRN_OFF(TPMS_RF_HIGH_TEMPERATURE);				       
		SET_TPMS_WRN_OFF(TPMS_RR_HIGH_TEMPERATURE);			 	        
		SET_TPMS_WRN_OFF(TPMS_LR_HIGH_TEMPERATURE);

	//	TPMS_LF_Tire_Status |= TIRE_TEMP_INVALID_BIT; 
	//	TPMS_RF_Tire_Status |= TIRE_TEMP_INVALID_BIT;
	//	TPMS_RR_Tire_Status |= TIRE_TEMP_INVALID_BIT;
	//	TPMS_LR_Tire_Status |= TIRE_TEMP_INVALID_BIT;
		TPMS_LF_Tire_Status &= ~TIRE_TEMP_INVALID_BIT; 
		TPMS_RF_Tire_Status &= ~TIRE_TEMP_INVALID_BIT;
		TPMS_RR_Tire_Status &= ~TIRE_TEMP_INVALID_BIT;
		TPMS_LR_Tire_Status &= ~TIRE_TEMP_INVALID_BIT;
		
	  switch(u8TPMS_Tire_Pos)
	  {
	    case LF_TIRE:
	      Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Tire_Temperature(&u8TPMS_LF_Tire_Temp);

		if(cWRN_ACTIVE == u8TPMS_Tire_Temp_St)
		{
	        TPMS_LF_Tire_Status |= HIGH_TEMP_BIT;
	        SET_TPMS_WRN_ON(TPMS_LF_HIGH_TEMPERATURE);
		}
	      /*check the invalid message.*/
	      if(MESSAGE_INVALID == u8TPMS_LF_Tire_Temp)
	      {
	        TPMS_LF_Tire_Status |= TIRE_TEMP_INVALID_BIT;	        
	      }
	      else
	      {
	        TPMS_LF_Tire_Status &= ~TIRE_TEMP_INVALID_BIT;
	      }
		 
	    break;
	    case RF_TIRE:
	      Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Tire_Temperature(&u8TPMS_RF_Tire_Temp);
		  
		  if(cWRN_ACTIVE == u8TPMS_Tire_Temp_St)
		  {

	        TPMS_RF_Tire_Status |= HIGH_TEMP_BIT;
	        SET_TPMS_WRN_ON(TPMS_RF_HIGH_TEMPERATURE);
		  }
		  
	      if(MESSAGE_INVALID == u8TPMS_RF_Tire_Temp)
	      {
	        TPMS_RF_Tire_Status |= TIRE_TEMP_INVALID_BIT;
	      }
	      else
	      {
	        TPMS_RF_Tire_Status &= ~TIRE_TEMP_INVALID_BIT; //FIX BUG

	      }
		  
	    break;
	    case RR_TIRE:
	      Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Tire_Temperature(&u8TPMS_RR_Tire_Temp);

		  if(cWRN_ACTIVE == u8TPMS_Tire_Temp_St)
		  {

	        TPMS_RR_Tire_Status |= HIGH_TEMP_BIT;
	        SET_TPMS_WRN_ON(TPMS_RR_HIGH_TEMPERATURE);

		  }
		  
	      if(MESSAGE_INVALID == u8TPMS_RR_Tire_Temp)
	      {
	        TPMS_RR_Tire_Status |= TIRE_TEMP_INVALID_BIT;
	      }
	      else
	      {
	        TPMS_RR_Tire_Status &= ~TIRE_TEMP_INVALID_BIT;
	      }		  

	    break;
	    case LR_TIRE:
	      Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Tire_Temperature(&u8TPMS_LR_Tire_Temp);
		  if(cWRN_ACTIVE == u8TPMS_Tire_Temp_St)
		  {
	        TPMS_LR_Tire_Status |= HIGH_TEMP_BIT;
	        SET_TPMS_WRN_ON(TPMS_LR_HIGH_TEMPERATURE);
		  }

	      if(MESSAGE_INVALID == u8TPMS_LR_Tire_Temp)
	      {
	        TPMS_LR_Tire_Status |= TIRE_TEMP_INVALID_BIT;
	      }
	      else
	      {
	        TPMS_LR_Tire_Status &= ~TIRE_TEMP_INVALID_BIT;
	      }
		  
	    break;
	    default:
			TPMS_LF_Tire_Status &= ~TIRE_TEMP_INVALID_BIT; 
			TPMS_RF_Tire_Status &= ~TIRE_TEMP_INVALID_BIT;
			TPMS_RR_Tire_Status &= ~TIRE_TEMP_INVALID_BIT;
			TPMS_LR_Tire_Status &= ~TIRE_TEMP_INVALID_BIT;
	      break;
	  }

}



