//---------------------------------------------------------------------------------------------------------------------
//
// VISTEON CORPORATION CONFIDENTIAL																																										                                                                                                                           // ________________________________
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
#include "Rte_DrvStyleMdl.h"
#include "Rte_DrvStyleMdl_Type.h"
#include "DrvStyleMdl.h"
#include <CmpLib.h>

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(DrvStyleMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
/* Macro to define CAN state */
#define TCU_SELECTEDAT_MODEL_CAN_DEFAULT    (0xF)
#define TCU_SELECTEDAT_MODEL_CAN_DEGRADE    (0xF)

#define TCU_CAN_AUTO         		(0x0)
#define TCU_CAN_SNOW         		(0x1)
#define TCU_CAN_SPORT        		(0x3)
#define TCU_CAN_ECO       			(0x4)
#define TCU_CAN_BTC          		(0x5)
#define TCU_CAN_ROCK         		(0x6)
#define TCU_CAN_WET          		(0x7)
#define TCU_CAN_SAND         		(0x8)
#define TCU_CAN_ATS          		(0xD)
#define TCU_CAN_ATS_FAILURE         (0xE)
#define TCU_CAN_INVALID      		(0xF)

typedef enum 
{
	AT_MODE_AUTO     		 = 0,
    AT_MODE_SNOW      		 = 1,
    AT_MODE_SPORT    		 = 2,
    AT_MODE_ECO       		 = 3,
    AT_MODE_BTC      		 = 4,
	AT_MODE_ROCK      		 = 5,
	AT_MODE_WET       		 = 6,
	AT_MODE_SAND      		 = 7,
	AT_MODE_ATS       		 = 8,
	AT_MODE_ATS_FAILURE      = 9,
	AT_MODE_INVALID   		 = 10,
}DrvStyle_MODE_AT;


//======================================================================================================================
// FORWARD DECLARATIONS OF PRIVATE FUNCTIONS
//======================================================================================================================
static void DrvStyle_TransIntoBatAbnormal(void);
static void DrvStyle_TransIntoIgnAbnormal(void);

static uint8 DrvStyle_MsgVerify(uint8 fl_can_msg_U8);
static uint8 DrvStyle_GetCanMsg(void);
static void DrvStyle_Process(void);
static void DrvStyle_Runnable(void);
static void DrvStyle_TransDataToHmi(void);




//======================================================================================================================
// PRIVATE DATA
//======================================================================================================================
static uint8 l_tcu_selected_at_model = 0;



//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_tcu_selected_at_model = AT_MODE_INVALID;
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
	l_tcu_selected_at_model = AT_MODE_INVALID;
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

    return  E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive(void)
{
	boolean fl_get_drive_mode_cfg_B = TRUE;
//	Rte_Read_rpSR_GdtCtrl_Drive_Mode_Cfg(&fl_get_drive_mode_cfg_B);
	
	if(TRUE == fl_get_drive_mode_cfg_B)
	{
		DrvStyle_Runnable();
	}
	else
	{
		/*do nothing*/
	}
	
    return  E_OK;
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

static void DrvStyle_Runnable(void)
{
    uint8 fl_ign_state_U8;
	uint8 fl_batt_state_U8;
	
	Rte_Read_rpBattState_BatteryState(&fl_batt_state_U8);	
	Rte_Read_rpIgnState_IGNState(&fl_ign_state_U8);

	if(fl_batt_state_U8 == eBatteryState_UnderVoltage || fl_batt_state_U8 == eBatteryState_OverVoltage)
	{
		DrvStyle_TransIntoBatAbnormal();
	}
	else 
	{
		switch(fl_ign_state_U8)
		{
			case eIGN_OFF:					
				DrvStyle_TransIntoIgnAbnormal();
				break;
			case eIGN_RUN:
				DrvStyle_Process();
				break;
			default:
				break;
		}
    }
    
	DrvStyle_TransDataToHmi();
}

static uint8 DrvStyle_GetCanMsg(void)
{

	uint8 fl_ret_value_U8 = 0;
	uint8  fl_can_0x93_nvr_status_BOOL = FALSE;
	uint8  fl_can_0x93_tmr_status_BOOL = FALSE;
	uint8   fl_tcu_select_at_mode_U8 = TCU_SELECTEDAT_MODEL_CAN_DEFAULT;

    Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(&fl_can_0x93_nvr_status_BOOL);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(&fl_can_0x93_tmr_status_BOOL);
		
	if((fl_can_0x93_nvr_status_BOOL == RTE_E_NEVER_RECEIVED)||(fl_can_0x93_tmr_status_BOOL == RTE_E_TIMEOUT))
	{
	    fl_tcu_select_at_mode_U8 = TCU_SELECTEDAT_MODEL_CAN_DEGRADE;
		fl_ret_value_U8 = fl_tcu_select_at_mode_U8;
	}
    else 
    {
    	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ATS_Driving_mode_confirm(&fl_tcu_select_at_mode_U8);
    }
    fl_ret_value_U8 = DrvStyle_MsgVerify(fl_tcu_select_at_mode_U8);
    
	return fl_ret_value_U8;
}

static uint8 DrvStyle_MsgVerify(uint8 fl_can_msg_U8)
{
   uint8 fl_ret_value_U8 = 0;
   if (fl_can_msg_U8 <= TCU_CAN_INVALID)
   {
   		fl_ret_value_U8 = fl_can_msg_U8;
   }
   else 
   {
        fl_ret_value_U8 =  TCU_SELECTEDAT_MODEL_CAN_DEGRADE;   
   }
   return fl_ret_value_U8;
}

static void DrvStyle_Process(void)
{
    uint8 fl_can_msg_U8 = TCU_SELECTEDAT_MODEL_CAN_DEFAULT;
	fl_can_msg_U8 = DrvStyle_GetCanMsg();
	switch(fl_can_msg_U8)
	{
		case TCU_CAN_AUTO:
			l_tcu_selected_at_model = AT_MODE_AUTO;
			break;
		case TCU_CAN_SNOW:
			l_tcu_selected_at_model = AT_MODE_SNOW;
			break;
		case TCU_CAN_SPORT:
			l_tcu_selected_at_model = AT_MODE_SPORT;
			break;
		case TCU_CAN_ECO:
			l_tcu_selected_at_model = AT_MODE_ECO;
			break;
		case TCU_CAN_BTC:
			l_tcu_selected_at_model = AT_MODE_BTC;
			break;
		case TCU_CAN_ROCK:
			l_tcu_selected_at_model = AT_MODE_ROCK;
			break;
		case TCU_CAN_WET:
			l_tcu_selected_at_model = AT_MODE_WET;
			break;
		case TCU_CAN_SAND:
			l_tcu_selected_at_model = AT_MODE_SAND;
			break;
		case TCU_CAN_ATS:
			l_tcu_selected_at_model = AT_MODE_ATS;
			break;	
		case TCU_CAN_ATS_FAILURE:
			l_tcu_selected_at_model = AT_MODE_ATS_FAILURE;
			break;		
		default:
			l_tcu_selected_at_model = AT_MODE_INVALID;
			break;
	}

}


static void DrvStyle_TransDataToHmi(void)
{
	Rte_Write_rpSR_DrvStyleMdl_DrvStatus_DriverInfo(l_tcu_selected_at_model);
}

static void DrvStyle_TransIntoBatAbnormal(void)
{
	l_tcu_selected_at_model = AT_MODE_INVALID;
}

static void DrvStyle_TransIntoIgnAbnormal(void)
{
	l_tcu_selected_at_model = AT_MODE_INVALID;
}


/*---------------------------------------------------------------------------
Date              : 02-Nov-17
CDSID             : psagathe
Traceability      : 986532
Change Description: Trip2 sw adc count change.
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Date              : 09-Nov-17
CDSID             : gkumarag
Traceability      : 988279
Change Description: Button response issue - Debounce logic added
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Date              : 12-Dec-17
CDSID             : vkrishn6
Traceability      : 1009803
Change Description: Block of TripSW1 buttons and drive mode button during IGN OFF
-----------------------------------------------------------------------------*/



