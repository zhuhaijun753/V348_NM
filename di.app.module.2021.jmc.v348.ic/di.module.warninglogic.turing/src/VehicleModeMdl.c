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
#include "Rte_VehicleModeMdl.h"
#include "CmpLib.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(VehicleModeMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint16 l_CurrentEngSpd_Last_U16 = 0;
static uint8  fl_EngOperationStatus_Last_U8 = FALSE;

#define cRPM_VALUE_THRESHOLD        (uint16)1300   /* value 1300 --> 650rpm */
#define cTRANSPORT_MODE             (uint8)0x1
#define cFACTORY_MODE               (uint8)0x2
#define cTRANSPORT_MODE_PAUSE       (uint8)0x4
#define cFACTORY_MODE_PAUSE         (uint8)0x5
#define cENFOPSTS_RUNGNING          (uint8)0x1
#define cENGOPSTS_IDLE              (uint8)0x2
#define cENGSPEED_INVALID           (uint16)0x7FFF
#define cTIMEOUT_2S                 (uint8)20      /* 2s timer for 100ms period */

//#define ODO_MASTER_VALUE_THRESHOLD  (200000U)
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================


//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static boolean Message_584_Invalid(void);
static boolean Message_10B_Invalid(void);
static void    fun_CarModeWarning_process(void);
static void    Init_CarModeWarning(void);


static uint8 l_EngSpeedInvalid_Cnt_U8;
static uint16 l_CurrentCarMode_U8;
static uint8 l_LastCarMode_U8;
static uint16 l_CurrentEngSpd_U16;
static uint16 l_LastEngSpd_U16;

/*
static boolean Message_510_Invalid(void);
static void InitFactoryModeWarning(void);
static void InitFactoryModePause(void);
static void InitTranspationModeWarning(void);
static void InitTranspationModePause(void);
static void InitBatteryLowWarning(void);
static void FactoryModeWarningProcess(void);
static void FactoryModePauseProcess(void);
static void TranspationModeWarningProcess(void);
static void TranspationModePauseProcess(void);
static void BatteryLowWarningProcess(void);
*/

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------

static Std_ReturnType CmpInit(void)
{
    Init_CarModeWarning();
    
    /*InitFactoryModeWarning();
    InitFactoryModePause();
    InitTranspationModeWarning();
    InitTranspationModePause();
    InitBatteryLowWarning();*/
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
    Init_CarModeWarning();

    /*InitFactoryModeWarning();
    InitFactoryModePause();
    InitTranspationModeWarning();
    InitTranspationModePause();
    InitBatteryLowWarning();*/
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
    Init_CarModeWarning();

    /*InitFactoryModeWarning();
    InitFactoryModePause();
    InitTranspationModeWarning();
    InitTranspationModePause();
    InitBatteryLowWarning();*/
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
    Init_CarModeWarning();

    /*InitFactoryModeWarning();
    InitFactoryModePause();
    InitTranspationModeWarning();
    InitTranspationModePause();
    InitBatteryLowWarning();*/
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
    fun_CarModeWarning_process();

    /*FactoryModeWarningProcess();
    FactoryModePauseProcess();
    TranspationModeWarningProcess();
    TranspationModePauseProcess();
    BatteryLowWarningProcess();*/
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


static boolean Message_584_Invalid(void)
{
    uint8 u8MsgNR   = (uint8)RTE_E_TIMEOUT;
    uint8 u8MsgTout = (uint8)RTE_E_NEVER_RECEIVED;
    boolean ret     = (boolean)FALSE;
    
    Rte_Read_rpSR_CANMSG_BCM_0x584_ComIn_NR(&u8MsgNR);
    Rte_Read_rpSR_CANMSG_BCM_0x584_ComIn_Tout(&u8MsgTout);
    if(((uint8)RTE_E_TIMEOUT == u8MsgTout) || ((uint8)RTE_E_NEVER_RECEIVED == u8MsgNR))
    {
        ret = (boolean)TRUE;
    }
    else
    {
        ret = (boolean)FALSE;
    }
    
    return ret;
}


static boolean Message_10B_Invalid(void)
{
    uint8 u8MsgNR   = (uint8)RTE_E_TIMEOUT;
    uint8 u8MsgTout = (uint8)RTE_E_NEVER_RECEIVED;
    boolean ret     = (boolean)FALSE;
    
    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&u8MsgNR);
    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&u8MsgTout);
    if( ((uint8)RTE_E_TIMEOUT == u8MsgTout) || ((uint8)RTE_E_NEVER_RECEIVED == u8MsgNR))
    {
        ret = (boolean)TRUE;
    }
    else
    {
        ret = (boolean)FALSE;
    }
    
    return ret;
}


static void Init_CarModeWarning(void)
{
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CAR_IN_FACTORY_MODE_ID,UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FACTORY_MODE_PUASE_ID,UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_ID,UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_PAUSE_ID,UI_WARNING_STATUS_OFF);

    l_EngSpeedInvalid_Cnt_U8 = (uint8)0;
    l_CurrentCarMode_U8      = (uint8)0;
    l_LastCarMode_U8         = (uint8)0;
}

static void fun_CarModeWarning_process(void)
{
    uint8  fl_EngOperationStatus_U8  = (uint8)0;
    uint16 fl_EngSpeed_U16           = (uint16)0;

    if ((boolean)FALSE == Message_584_Invalid())
    {
        Rte_Read_rpSR_CANMSG_BCM_0x584_ComIn_BCM_CarModeIndicator(&l_CurrentCarMode_U8);
        l_LastCarMode_U8    = l_CurrentCarMode_U8;
    }
    else
    {   
        /* Keep last car mode */
        l_CurrentCarMode_U8 = l_LastCarMode_U8;
    }
    
    if ( (boolean)FALSE == Message_10B_Invalid() )
    {
        Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&fl_EngSpeed_U16);
        Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus(&fl_EngOperationStatus_U8);
        
        if (cENGSPEED_INVALID == fl_EngSpeed_U16)
        {
            if (cTIMEOUT_2S < l_EngSpeedInvalid_Cnt_U8)
            {
                l_CurrentEngSpd_U16 = (uint8)0;
            }
            else
            {
                l_EngSpeedInvalid_Cnt_U8 ++;
                l_CurrentEngSpd_U16 = l_LastEngSpd_U16;
            }
        }
        else
        {
            /* The EngSpeed value is normal*/
            l_EngSpeedInvalid_Cnt_U8 = (uint8)0;
            l_LastEngSpd_U16 = fl_EngSpeed_U16;
            l_CurrentEngSpd_U16 = fl_EngSpeed_U16;
        }
    }
    else
    {   
        l_EngSpeedInvalid_Cnt_U8 = (uint8)0;
        fl_EngOperationStatus_U8 = (uint8)0;
        l_CurrentEngSpd_U16      = (uint16)0;
    }
	
	/* this is 1th function l_CurrentCarMode_U8 = 2 */
    if((cFACTORY_MODE == l_CurrentCarMode_U8) && (cRPM_VALUE_THRESHOLD > l_CurrentEngSpd_U16))
    {
		if(((cENFOPSTS_RUNGNING > fl_EngOperationStatus_U8) || (cENGOPSTS_IDLE < fl_EngOperationStatus_U8))
      	&& (fl_EngOperationStatus_Last_U8 != fl_EngOperationStatus_U8))
		{
			uint8 fl_CurrentCarMode2_U8 = (uint8)UI_WARNING_STATE_NONE;
			fl_CurrentCarMode2_U8 = CWarningCtrl_APP_get_notified_warning_status(WARN_CAR_IN_FACTORY_MODE_ID);
			
			if((uint8)UI_WARNING_STATE_NONE == fl_CurrentCarMode2_U8)
			{
				Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CAR_IN_FACTORY_MODE_ID,UI_WARNING_STATUS_ON);
			}
			else if (((uint8)UI_WARNING_STATE_SHOW == fl_CurrentCarMode2_U8) || ((uint8)UI_WARNING_STATE_ACKNOWLEDED == fl_CurrentCarMode2_U8))
			{
				Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CAR_IN_FACTORY_MODE_ID,UI_WARNING_STATUS_RETRIG);
			}
			else if ((uint8)UI_WARNING_STATE_TIMEOUT == fl_CurrentCarMode2_U8)
			{
				Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CAR_IN_FACTORY_MODE_ID,UI_WARNING_CONDITION_FORCED);
			}
			else
			{
				//
			}
		}
		else if((cENFOPSTS_RUNGNING > fl_EngOperationStatus_U8) || (cENGOPSTS_IDLE < fl_EngOperationStatus_U8))
		{
			Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CAR_IN_FACTORY_MODE_ID,UI_WARNING_STATUS_ON);
		}
		else
		{
			Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CAR_IN_FACTORY_MODE_ID,UI_WARNING_STATUS_OFF);
		}

    }
    else
    {
         Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CAR_IN_FACTORY_MODE_ID,UI_WARNING_STATUS_OFF);
    }

	/* this is 2th function l_CurrentCarMode_U8 = 5 */
    if(cFACTORY_MODE_PAUSE == l_CurrentCarMode_U8)
    {
    	if(((cENFOPSTS_RUNGNING == fl_EngOperationStatus_U8 || cENGOPSTS_IDLE == fl_EngOperationStatus_U8)
		&& (fl_EngOperationStatus_Last_U8 != fl_EngOperationStatus_U8))
		|| ((cRPM_VALUE_THRESHOLD <= l_CurrentEngSpd_U16) && (l_CurrentEngSpd_Last_U16 != l_CurrentEngSpd_U16)))
    	{
			uint8 fl_CurrentCarMode5_U8 = (uint8)UI_WARNING_STATE_NONE;
			fl_CurrentCarMode5_U8 = CWarningCtrl_APP_get_notified_warning_status(WARN_FACTORY_MODE_PUASE_ID);
			
			if((uint8)UI_WARNING_STATE_NONE == fl_CurrentCarMode5_U8)
			{
				Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FACTORY_MODE_PUASE_ID,UI_WARNING_STATUS_ON);
			}
			else if (((uint8)UI_WARNING_STATE_SHOW == fl_CurrentCarMode5_U8) || ((uint8)UI_WARNING_STATE_ACKNOWLEDED == fl_CurrentCarMode5_U8))
			{
				Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FACTORY_MODE_PUASE_ID,UI_WARNING_STATUS_RETRIG);
			}
			else if ((uint8)UI_WARNING_STATE_TIMEOUT == fl_CurrentCarMode5_U8)
			{
				Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FACTORY_MODE_PUASE_ID,UI_WARNING_CONDITION_FORCED);
			}
			else
			{
				//
			}		
		}
		else if((cENFOPSTS_RUNGNING == fl_EngOperationStatus_U8) || (cENGOPSTS_IDLE == fl_EngOperationStatus_U8)
			 || (cRPM_VALUE_THRESHOLD <= l_CurrentEngSpd_U16))
		{
			Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FACTORY_MODE_PUASE_ID,UI_WARNING_STATUS_ON);
		}
		else
		{
			Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FACTORY_MODE_PUASE_ID,UI_WARNING_STATUS_OFF);
		}
    }
    else
    {
         Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FACTORY_MODE_PUASE_ID,UI_WARNING_STATUS_OFF);
    }
	
	/* this is 3th function l_CurrentCarMode_U8 = 1 */
    if((cTRANSPORT_MODE == l_CurrentCarMode_U8) && (cRPM_VALUE_THRESHOLD > l_CurrentEngSpd_U16)
	&& ((cENFOPSTS_RUNGNING > fl_EngOperationStatus_U8) || (cENGOPSTS_IDLE < fl_EngOperationStatus_U8)))
    {
		Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_ID,UI_WARNING_STATUS_ON);
    }
    else
    {
         Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_ID,UI_WARNING_STATUS_OFF);
    }
	
	/* this is 4th function l_CurrentCarMode_U8 = 4 */
    if(cTRANSPORT_MODE_PAUSE == l_CurrentCarMode_U8)
    {
    	if(((cENFOPSTS_RUNGNING == fl_EngOperationStatus_U8 || cENGOPSTS_IDLE == fl_EngOperationStatus_U8)
		&& (fl_EngOperationStatus_Last_U8 != fl_EngOperationStatus_U8))
		|| ((cRPM_VALUE_THRESHOLD <= l_CurrentEngSpd_U16) && (l_CurrentEngSpd_Last_U16 != l_CurrentEngSpd_U16)))
    	{
			uint8 fl_CurrentCarMode4_U8 = (uint8)UI_WARNING_STATE_NONE;
			fl_CurrentCarMode4_U8 = CWarningCtrl_APP_get_notified_warning_status(WARN_TRANSPATION_MODE_PAUSE_ID);
			
			if((uint8)UI_WARNING_STATE_NONE == fl_CurrentCarMode4_U8)
			{
				Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_PAUSE_ID,UI_WARNING_STATUS_ON);
			}
			else if (((uint8)UI_WARNING_STATE_SHOW == fl_CurrentCarMode4_U8) || ((uint8)UI_WARNING_STATE_ACKNOWLEDED == fl_CurrentCarMode4_U8))
			{
				Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_PAUSE_ID,UI_WARNING_STATUS_RETRIG);
			}
			else if ((uint8)UI_WARNING_STATE_TIMEOUT == fl_CurrentCarMode4_U8)
			{
				Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_PAUSE_ID,UI_WARNING_CONDITION_FORCED);
			}
			else
			{
				//
			}		
		}
		else if((cENFOPSTS_RUNGNING == fl_EngOperationStatus_U8) || (cENGOPSTS_IDLE == fl_EngOperationStatus_U8)
			 || (cRPM_VALUE_THRESHOLD <= l_CurrentEngSpd_U16))
		{
			Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_PAUSE_ID,UI_WARNING_STATUS_ON);
		}
		else
		{
			Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_PAUSE_ID,UI_WARNING_STATUS_OFF);
		}
    }
    else
    {
         Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_PAUSE_ID,UI_WARNING_STATUS_OFF);
    }

	fl_EngOperationStatus_Last_U8 = fl_EngOperationStatus_U8;
	l_CurrentEngSpd_Last_U16 = l_CurrentEngSpd_U16;
}



#if 0
static void InitFactoryModeWarning(void)
{
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CAR_IN_FACTORY_MODE_ID,UI_WARNING_STATUS_OFF);
}

static void InitFactoryModePause(void)
{
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FACTORY_MODE_PUASE_ID,UI_WARNING_STATUS_OFF);
}

static void InitTranspationModeWarning(void)
{
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_ID,UI_WARNING_STATUS_OFF);
}

static void InitTranspationModePause(void)
{
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_PAUSE_ID,UI_WARNING_STATUS_OFF);
}

static void InitBatteryLowWarning(void)
{
  //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BATTERY_MANAGEMENT_ID,UI_WARNING_STATUS_OFF);
}

static void FactoryModeWarningProcess(void)
{
  uint8 EngOperationStatus = 0;
  uint16 EMS_EngSpeed = 0;
  uint32 OdometerMasterValue = 0;
  uint8 CarModeIndicatorTemp = 0;
  static uint8 CarModeIndicator = 0;
  //Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus(&EngOperationStatus);
  Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&EMS_EngSpeed);
  //Rte_Read_VehicleModeMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(&OdometerMasterValue);//can read from ODO
  //if(TRUE != Message_584_Invalid() )
  //{
  // 
  // Rte_Read_VehicleModeMdl_rpSR_CANMSG_BCM_CarModeIndicator(&CarModeIndicatorTemp);
  // if(CarModeIndicator < 6)
  // {
  //    CarModeIndicator = CarModeIndicatorTemp;
  // }
  //}
  if(TRUE != Message_10B_Invalid())
  {
    if((2 != EngOperationStatus || 3 != EngOperationStatus|| RPM_VALUE_THRESHOLD > EMS_EngSpeed) && 
    (ODO_MASTER_VALUE_THRESHOLD > OdometerMasterValue) && (5 == CarModeIndicator))
    {
      //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CAR_IN_FACTORY_MODE_ID,UI_WARNING_STATUS_ON);
    }
  }
  else
  {
    //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_CAR_IN_FACTORY_MODE_ID,UI_WARNING_STATUS_OFF);
  }
}

static void FactoryModePauseProcess(void)
{
  uint8 EngOperationStatus = 0;
  uint16 EMS_EngSpeed = 0;
  uint32 OdometerMasterValue = 0;
  uint8 CarModeIndicatorTemp = 0;
  static uint8 CarModeIndicator = 0;
  //Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus(&EngOperationStatus);
  Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&EMS_EngSpeed);
  //Rte_Read_VehicleModeMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(&OdometerMasterValue);//can read from ODO
  //if(TRUE != Message_584_Invalid() )
  //{
  // 
  // Rte_Read_VehicleModeMdl_rpSR_CANMSG_BCM_CarModeIndicator(&CarModeIndicatorTemp);
  // if(CarModeIndicator < 6)
  // {
  //    CarModeIndicator = CarModeIndicatorTemp;
  // }
  //}
  if(TRUE != Message_10B_Invalid())
  {
    if((2 != EngOperationStatus || 3 != EngOperationStatus|| RPM_VALUE_THRESHOLD > EMS_EngSpeed) && 
    (ODO_MASTER_VALUE_THRESHOLD > OdometerMasterValue) && (2 == CarModeIndicator))
    {
      //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FACTORY_MODE_PUASE_ID,UI_WARNING_STATUS_ON);
    }
  }
  else
  {
    //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FACTORY_MODE_PUASE_ID,UI_WARNING_STATUS_OFF);
  }
}

static void TranspationModeWarningProcess(void)
{
  uint8 EngOperationStatus = 0;
  uint16 EMS_EngSpeed = 0;
  uint32 OdometerMasterValue = 0;
  uint8 CarModeIndicatorTemp = 0;
  static uint8 CarModeIndicator = 0;
  //Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus(&EngOperationStatus);
  Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&EMS_EngSpeed);
  //Rte_Read_VehicleModeMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(&OdometerMasterValue);//can read from ODO
  //if(TRUE != Message_584_Invalid() )
  //{
  // 
  // Rte_Read_VehicleModeMdl_rpSR_CANMSG_BCM_CarModeIndicator(&CarModeIndicatorTemp);
  // if(CarModeIndicator < 6)
  // {
  //    CarModeIndicator = CarModeIndicatorTemp;
  // }
  //}
  if(TRUE != Message_10B_Invalid())
  {
    if((2 != EngOperationStatus || 3 != EngOperationStatus|| RPM_VALUE_THRESHOLD > EMS_EngSpeed) && 
    (ODO_MASTER_VALUE_THRESHOLD > OdometerMasterValue) && (1 == CarModeIndicator))
    {
      //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_ID,UI_WARNING_STATUS_ON);
    }
  }
  else
  {
    //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_ID,UI_WARNING_STATUS_OFF);
  }
}

static void TranspationModePauseProcess(void)
{
  uint8 EngOperationStatus = 0;
  uint16 EMS_EngSpeed = 0;
  uint32 OdometerMasterValue = 0;
  uint8 CarModeIndicatorTemp = 0;
  static uint8 CarModeIndicator = 0;
  //Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus(&EngOperationStatus);
  Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&EMS_EngSpeed);
  //Rte_Read_VehicleModeMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(&OdometerMasterValue);//can read from ODO
  //if(TRUE != Message_584_Invalid())
  //{
  // 
  // Rte_Read_VehicleModeMdl_rpSR_CANMSG_BCM_CarModeIndicator(&CarModeIndicatorTemp);
  // if(CarModeIndicator < 6)
  // {
  //    CarModeIndicator = CarModeIndicatorTemp;
  // }
  //}
  if(TRUE != Message_10B_Invalid())
  {
    if((2 != EngOperationStatus || 3 != EngOperationStatus|| RPM_VALUE_THRESHOLD > EMS_EngSpeed) && 
    (ODO_MASTER_VALUE_THRESHOLD > OdometerMasterValue) && (1 == CarModeIndicator))
    {
      //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_PAUSE_ID,UI_WARNING_STATUS_ON);
    }
  }
  else
  {
    //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TRANSPATION_MODE_PAUSE_ID,UI_WARNING_STATUS_OFF);
  }
}

static void BatteryLowWarningProcess(void)
{
  uint8 LoadManageLevel = 0;
  
  if(TRUE != Message_584_Invalid())
  {
    //Rte_Read_rpSR_CANMSG_BCM_0X584_ComIn_BCM_LoadManageLevel(LoadManageLevel);
    if(1 == LoadManageLevel || 2 == LoadManageLevel)
    {
      //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BATTERY_MANAGEMENT_ID,UI_WARNING_STATUS_ON);
    }
    else
    {
      //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BATTERY_MANAGEMENT_ID,UI_WARNING_STATUS_OFF);
    }
  }
  else
  {
    //Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BATTERY_MANAGEMENT_ID,UI_WARNING_STATUS_OFF);
  }
}
#endif

