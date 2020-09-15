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
#include "Rte_GearMdl.h"
#include "Rte_GearMdl_Type.h"

#include "GearMdl.h"
#include <CmpLib.h>



/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(GearMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
/* Macro to define CAN state */
#define TCU_DisplayGear_CAN_DEFAULT    (0xF)
#define TCU_DisplayGear_CAN_DEGRADE    (0xF)

#define TCU_DisplayGear_CAN_N          (0x0)
#define TCU_DisplayGear_CAN_M1         (0x1)
#define TCU_DisplayGear_CAN_M2         (0x2)
#define TCU_DisplayGear_CAN_M3         (0x3)
#define TCU_DisplayGear_CAN_M4         (0x4)
#define TCU_DisplayGear_CAN_M5         (0x5)
#define TCU_DisplayGear_CAN_M6         (0x6)
#define TCU_DisplayGear_CAN_M7         (0x7)
#define TCU_DisplayGear_CAN_M8         (0x8)
#define TCU_DisplayGear_CAN_D		   (0x9)
#define TCU_DisplayGear_CAN_R          (0xB)
#define TCU_DisplayGear_CAN_P          (0xD)
#define TCU_DisplayGear_CAN_INVALID    (0xF)

#define TCU_GearSelectorReq_CAN_N          (0x6)
#define TCU_GearSelectorReq_CAN_R          (0x7)
#define TCU_GearSelectorReq_CAN_M    	   (0x8)
#define TCU_GearSelectorReq_CAN_D    	   (0x5)
#define TCU_GearSelectorReq_CAN_NINE   	   (0x9)
#define TCU_GearSelectorReq_CAN_INVALID    (0xF)

#define EMS_RemindShiftGear_CAN_DEFAULT    (0x0)
#define EMS_RemindShiftGear_CAN_UP    	   (0x1)
#define EMS_RemindShiftGear_CAN_DOWN       (0x2)
#define WITH_AMT				(0x02)


typedef enum 
{
	AT_GEAR_N         = 0,
    AT_GEAR_M1        ,
    AT_GEAR_M2        ,
    AT_GEAR_M3        ,
	AT_GEAR_M4        ,
	AT_GEAR_M5        ,
	AT_GEAR_M6        ,
	AT_GEAR_M7        ,
	AT_GEAR_M8        ,
	AT_GEAR_D         ,
	AT_GEAR_R         ,
	AT_GEAR_P         ,
	AT_GEAR_D1        ,
	AT_GEAR_D2        ,
	AT_GEAR_D3        ,
	AT_GEAR_D4        ,
	AT_GEAR_D5        ,
	AT_GEAR_D6        ,
	AT_GEAR_INVALID   ,
}TCU_GEAR_AT;



//======================================================================================================================
// FORWARD DECLARATIONS OF PRIVATE FUNCTIONS
//======================================================================================================================
static void Gear_TransIntoBatAbnormal(void);
static void Gear_TransIntoIgnAbnormal(void);

static uint8 Gear_MsgVerify(uint8 fl_can_msg_U8);
static uint8 Gear_GetCanMsg(void);
static uint8 GearShift_GetCanMsg(void);
static void Gear_Process(void);
static void Gear_Runnable(void);
static void Gear_TransDataToHmi(void);
static void Gear_TransDataToDOut(void);
static boolean Check_TCU_0x395_Signal_Valid(void);
static boolean IsGearFlashTimerOut(void);
static void Gear_TCU_Flash_ouput(void);










//======================================================================================================================
// PRIVATE DATA
//======================================================================================================================
static uint8 l_tcu_selected_at_gear = 0;
static uint8 l_tcu_selected_mt_gearshift = 0;
#define Gear_TimeOut_3Sec (3000)

static boolean Flash_status;
//static boolean f_GearTimerStart;
//static boolean f_GearTimerOut;



//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_tcu_selected_at_gear = AT_GEAR_INVALID;
	l_tcu_selected_mt_gearshift = EMS_RemindShiftGear_CAN_DEFAULT;
//	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleGearCnt);
	Flash_status = FALSE;
//	f_GearTimerStart = FALSE;
//	f_GearTimerOut = FALSE;
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
	l_tcu_selected_at_gear = AT_GEAR_INVALID;
	l_tcu_selected_mt_gearshift = EMS_RemindShiftGear_CAN_DEFAULT;
//	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleGearCnt);
	Flash_status = FALSE;
//	f_GearTimerStart = FALSE;
//	f_GearTimerOut = FALSE;
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
	boolean fl_get_drive_gear_cfg_B = TRUE;
//	Rte_Read_rpSR_GdtCtrl_Drive_Mode_Cfg(&fl_get_drive_gear_cfg_B);
	
	if(TRUE == fl_get_drive_gear_cfg_B)
	{
		Gear_Runnable();
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

static void Gear_Runnable(void)
{
    uint8 fl_ign_state_U8;
	uint8 fl_batt_state_U8;
	
	Rte_Read_rpBattState_BatteryState(&fl_batt_state_U8);	
	Rte_Read_rpIgnState_IGNState(&fl_ign_state_U8);

	if(fl_batt_state_U8 == eBatteryState_UnderVoltage || fl_batt_state_U8 == eBatteryState_OverVoltage)
	{
		Gear_TransIntoBatAbnormal();
	}
	else 
	{
		switch(fl_ign_state_U8)
		{
			case eIGN_OFF:					
				Gear_TransIntoIgnAbnormal();
				break;
			case eIGN_RUN:
				Gear_Process();
				break;
			default:
				break;
		}
    }
    
	Gear_TransDataToHmi();
	Gear_TransDataToDOut();
}

static uint8 Gear_GetCanMsg(void)
{

	uint8  fl_ret_value_U8 = 0;
	uint8  fl_can_0x123_nvr_status_BOOL = FALSE;
	uint8  fl_can_0x123_tmr_status_BOOL = FALSE;
	uint8  fl_tcu_select_at_gear_U8 = TCU_DisplayGear_CAN_DEFAULT;
	uint8  fl_tcu_gear_selector_req_U8 = TCU_GearSelectorReq_CAN_INVALID;
	
    Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_NR(&fl_can_0x123_nvr_status_BOOL);
	Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_Tout(&fl_can_0x123_tmr_status_BOOL);
		
	if((fl_can_0x123_nvr_status_BOOL == RTE_E_NEVER_RECEIVED)||(fl_can_0x123_tmr_status_BOOL == RTE_E_TIMEOUT))
	{
	    fl_tcu_select_at_gear_U8 = AT_GEAR_INVALID;
		fl_ret_value_U8 = fl_tcu_select_at_gear_U8;
	}
    else 
    {
		Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_GearSelectorReq(&fl_tcu_gear_selector_req_U8);	
		Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear(&fl_tcu_select_at_gear_U8);
		switch(fl_tcu_gear_selector_req_U8)
		{
			case TCU_GearSelectorReq_CAN_D:
				//fl_ret_value_U8 = AT_GEAR_D;
				switch(fl_tcu_select_at_gear_U8)
				{
					case TCU_DisplayGear_CAN_M1:
						fl_ret_value_U8 = AT_GEAR_D1;
					break;
					case TCU_DisplayGear_CAN_M2:
						fl_ret_value_U8 = AT_GEAR_D2;
					break;
					case TCU_DisplayGear_CAN_M3:
						fl_ret_value_U8 = AT_GEAR_D3;
					break;
					case TCU_DisplayGear_CAN_M4:
						fl_ret_value_U8 = AT_GEAR_D4;
					break;
					case TCU_DisplayGear_CAN_M5:
						fl_ret_value_U8 = AT_GEAR_D5;
					break;
					case TCU_DisplayGear_CAN_M6:
						fl_ret_value_U8 = AT_GEAR_D6;
					break;
					/*case TCU_DisplayGear_CAN_M7:
						fl_ret_value_U8 = AT_GEAR_M7;
					break;*/
					default:
						fl_ret_value_U8 = AT_GEAR_INVALID;
				}
				
			break;
			case TCU_GearSelectorReq_CAN_N:
				fl_ret_value_U8 = AT_GEAR_N;
			break;
			case TCU_GearSelectorReq_CAN_R:
				fl_ret_value_U8 = AT_GEAR_R;
			break;
			case TCU_GearSelectorReq_CAN_M:
			{
				Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear(&fl_tcu_select_at_gear_U8);
				switch(fl_tcu_select_at_gear_U8)
				{
					case TCU_DisplayGear_CAN_M1:
						fl_ret_value_U8 = AT_GEAR_M1;
					break;
					case TCU_DisplayGear_CAN_M2:
						fl_ret_value_U8 = AT_GEAR_M2;
					break;
					case TCU_DisplayGear_CAN_M3:
						fl_ret_value_U8 = AT_GEAR_M3;
					break;
					case TCU_DisplayGear_CAN_M4:
						fl_ret_value_U8 = AT_GEAR_M4;
					break;
					case TCU_DisplayGear_CAN_M5:
						fl_ret_value_U8 = AT_GEAR_M5;
					break;
					case TCU_DisplayGear_CAN_M6:
						fl_ret_value_U8 = AT_GEAR_M6;
					break;
					/*case TCU_DisplayGear_CAN_M7:
						fl_ret_value_U8 = AT_GEAR_M7;
					break;*/
					default:
						fl_ret_value_U8 = AT_GEAR_INVALID;
				}
			}
			break;
			default:
				fl_ret_value_U8 = AT_GEAR_INVALID;
			break;
		}
    }
	return fl_ret_value_U8;
}

static uint8 GearShift_GetCanMsg(void)
{

	uint8  fl_ret_value_U8 = 0;
	uint8  fl_can_0x366_nvr_status_BOOL = FALSE;
	uint8  fl_can_0x366_tmr_status_BOOL = FALSE;
	uint8  fl_tcu_gearshift_U8 = EMS_RemindShiftGear_CAN_DEFAULT;
	
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(&fl_can_0x366_nvr_status_BOOL);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(&fl_can_0x366_tmr_status_BOOL);
		
	if((fl_can_0x366_nvr_status_BOOL == RTE_E_NEVER_RECEIVED)|| (fl_can_0x366_tmr_status_BOOL == RTE_E_TIMEOUT))
	{
	    fl_tcu_gearshift_U8 = EMS_RemindShiftGear_CAN_DEFAULT;
		fl_ret_value_U8 = fl_tcu_gearshift_U8;
	}
    else 
    {
		Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_RemindShiftGear_MT(&fl_tcu_gearshift_U8);
		
		if(fl_tcu_gearshift_U8 == EMS_RemindShiftGear_CAN_UP) 
		{
			//fl_tcu_gearshift_U8 == EMS_RemindShiftGear_CAN_UP;
			fl_ret_value_U8 = EMS_RemindShiftGear_CAN_UP;
		}
		else if(fl_tcu_gearshift_U8 == EMS_RemindShiftGear_CAN_DOWN)
		{
			//fl_tcu_gearshift_U8 == EMS_RemindShiftGear_CAN_DOWN;
			fl_ret_value_U8 = EMS_RemindShiftGear_CAN_DOWN;
		}
		else
		{
			//fl_tcu_gearshift_U8 == EMS_RemindShiftGear_CAN_DEFAULT;
			fl_ret_value_U8 = EMS_RemindShiftGear_CAN_DEFAULT;
		}		

    }
	return fl_ret_value_U8;
}

#if 0
static uint8 Gear_MsgVerify(uint8 fl_can_msg_U8)
{
   uint8 fl_ret_value_U8 = 0;
   if (fl_can_msg_U8 <= TCU_DisplayGear_CAN_INVALID)
   {
   		fl_ret_value_U8 = fl_can_msg_U8;
   }
   else 
   {
        fl_ret_value_U8 =  TCU_DisplayGear_CAN_DEGRADE;   
   }
   return fl_ret_value_U8;
}
#endif
static void Gear_Process(void)
{
    uint8 fl_can_msg_U8;
	uint8 f2_can_msg_U8;
	uint8 IsMTCfg = 0;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AT_MT,&IsMTCfg);
	if(WITH_AMT !=  IsMTCfg)
	{
		f2_can_msg_U8 = GearShift_GetCanMsg();
		l_tcu_selected_mt_gearshift = f2_can_msg_U8;
	}
	else
	{
		fl_can_msg_U8 = Gear_GetCanMsg();
		l_tcu_selected_at_gear = fl_can_msg_U8;
		Gear_TCU_Flash_ouput();
	}
	
	
	
}


static void Gear_TransDataToHmi(void)
{
	Rte_Write_ppSR_GearMdl_GearStatus_GearPosition(l_tcu_selected_at_gear);
	Rte_Write_ppSR_GearMdl_GearStatus_GearShiftStatus(l_tcu_selected_mt_gearshift);
}

static void Gear_TransDataToDOut(void)
{
	uint8 fl_AT_Out_U8 = AT_GEAR_INVALID;
	fl_AT_Out_U8 = l_tcu_selected_at_gear;
	
	switch(fl_AT_Out_U8)
	{
		case AT_GEAR_N:
			Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(eIOPinOutId_DO_TCU_N_UC, 1);
			Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(eIOPinOutId_DO_TCU_S_UC, 0);
			Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(eIOPinOutId_DO_TCU_D_UC, 0);
			break;
		case AT_GEAR_D:
			Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(eIOPinOutId_DO_TCU_N_UC, 0);
			Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(eIOPinOutId_DO_TCU_S_UC, 0);
			Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(eIOPinOutId_DO_TCU_D_UC, 1);
			break;
		default:
			Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(eIOPinOutId_DO_TCU_N_UC, 0);
			Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(eIOPinOutId_DO_TCU_S_UC, 0);
			Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(eIOPinOutId_DO_TCU_D_UC, 0);
			break;

	}
}

static void Gear_TransIntoBatAbnormal(void)
{
	l_tcu_selected_at_gear = AT_GEAR_INVALID;
	l_tcu_selected_mt_gearshift = EMS_RemindShiftGear_CAN_DEFAULT;
//	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleGearCnt);
	Flash_status = FALSE;
//	f_GearTimerStart = FALSE;
//	f_GearTimerOut = FALSE;
}

static void Gear_TransIntoIgnAbnormal(void)
{
	l_tcu_selected_at_gear = AT_GEAR_INVALID;
	l_tcu_selected_mt_gearshift = EMS_RemindShiftGear_CAN_DEFAULT;
//	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleGearCnt);
	Flash_status = FALSE;
//	f_GearTimerStart = FALSE;
//	f_GearTimerOut = FALSE;
}
#if 1
static void Gear_TCU_Flash_ouput(void)
{
	uint8 TCU_Driverwarning;
	Rte_Read_GearMdl_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Driverwarning(&TCU_Driverwarning);
	if(TRUE == Check_TCU_0x395_Signal_Valid())
	{
		if(TCU_Driverwarning == 0x1)
		{
		#if 0
			if(IsGearFlashTimerOut())
			{
				Flash_status = FALSE;
			}
			else
			{
				Flash_status = TRUE;
			}
		#endif
			Flash_status = TRUE;
		}
		else
		{
			//f_GearTimerStart = FALSE;
			Flash_status = FALSE;
		}
	}
	else
	{
	 //	f_GearTimerStart = FALSE;
		Flash_status = FALSE;
	}
	Rte_Write_ppSR_GearMdl_GearStatus_Flash_status(Flash_status);
}

#endif
static boolean Check_TCU_0x395_Signal_Valid(void)
{
	uint8 fl_msg_nr;
	uint8 fl_msg_tout;
	/*Read 0x395 msg signal status*/
	Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_NR(&fl_msg_nr);
	Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_Tout(&fl_msg_tout);
	
	if((fl_msg_nr == RTE_E_NEVER_RECEIVED) || (fl_msg_tout == RTE_E_TIMEOUT))
	{
		return FALSE;
	}
	
	return TRUE;	
}

#if 0
static boolean IsGearFlashTimerOut(void)
{
	boolean Ret = FALSE;
	boolean timerstatus = FALSE;
	Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerHandleGearCnt, &timerstatus);
	if(FALSE != timerstatus)
	{   
		Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerHandleGearCnt, Gear_TimeOut_3Sec, &Ret);
		if(FALSE != Ret)
		{
			f_GearTimerOut = FALSE;
			Rte_Call_rptimer_TmExt_Timer_Stop(eTimerHandleGearCnt);
		}
		/*else
		{
			f_GearTimerOut = FALSE;
		}*/
	}
	else
	{
		if(TRUE != f_GearTimerStart)
		{
			f_GearTimerStart = TRUE;
			Rte_Call_rpTimer_TmExt_Timer_Start(eTimerHandleGearCnt);
		}
	}
	return f_GearTimerOut;
}
#endif

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



