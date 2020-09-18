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

#include "Rte_ComVnimMdl.h"

#include "CmpLib.h"
#include "Dem.h"
#include "system.h"
#include "Dcm.h"
/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(ComVnimMdl)

static void ComIn_Msg_DTC_Process(void);

extern boolean dem_IGN_ON_5s;
extern boolean dem_batt_state;
extern boolean dem_all_block_read_finish_bool;
extern boolean dem_Can_Busoff_bool;             //Lost Communication DTC Record Must BUS Active
//extern boolean dem_dtc_state;

#define RESULT_NORMAL             	(2)
#define RESULT_FAILED            	(3)

#define CAN_LIMP_HOME_DTC			(1)
#define CAN_LOST_DTC_ESP		    (2)
#define CAN_LOST_DTC_TCU        	(3)
#define CAN_LOST_DTC_BCM        	(4)
#define CAN_LOST_DTC_EMS        	(5)
#define CAN_LOST_DTC_SRS        	(6)
#define CAN_LOST_DTC_TPMS        	(7)
#define CAN_LOST_DTC_EPS        	(8)
#define CAN_LOST_DTC_FUEL_SENSOR	(9)
#define CAN_LOST_DTC_T_BOX        	(10)
#define CAN_LOST_DTC_BELOW_8_5V		(11)
#define CAN_LOST_DTC_ABOVE_16V		(12)
#define CAN_BUS_OFF_DTC				(13)

#if 0
#define CAN_LOST_DTC_MP5        	(12)
#define CAN_LOST_DTC_IPM        	(15)
#define CAN_LOST_DTC_SWM        	(16)
#define CAN_LOST_DTC_PAM        	(17)
#endif

/* ADC channel number to be passed to IoHwAb */


//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static void Rte_Init_Memory( void );
static void ComIn_Msg_DTC_Process(void);
static boolean ComIn_DTC_StoreIn_Condition(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	Rte_Init_Memory();
   
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
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
	uint8 ret_Vehspd;
	uint16 VehSpdLgt = 0;
	uint8 ABS_AbsFlgFlt;
	uint8 ABS_EbdFlgFlt;
	ret_Vehspd =  Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&VehSpdLgt);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_AbsFlgFlt(&ABS_AbsFlgFlt);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_EbdFlgFlt(&ABS_EbdFlgFlt);
    //ABS_WhlMilgFrntLe
    uint16 fl_ABS_WhlMilgFrntLe;
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLe(&fl_ABS_WhlMilgFrntLe);

    //ABS_WhlMilgFrntLeStatus
    uint8 fl_ABS_WhlMilgFrntLeStatus;
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLeStatus(&fl_ABS_WhlMilgFrntLeStatus);

    //ABS_WhlMilgFrntRi
    uint16 fl_ABS_WhlMilgFrntRi;
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRi(&fl_ABS_WhlMilgFrntRi);

    //ABS_WhlMilgFrntRiStatus
    uint8 fl_ABS_WhlMilgFrntRiStatus;
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRiStatus(&fl_ABS_WhlMilgFrntRiStatus);
    
	if((ret_Vehspd &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{
		Rte_Write_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((ret_Vehspd &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		Rte_Write_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(0);
		Rte_Write_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(VehSpdLgt);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_AbsFlgFlt(ABS_AbsFlgFlt);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_EbdFlgFlt(ABS_EbdFlgFlt);
        Rte_Write_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLe(fl_ABS_WhlMilgFrntLe);
        Rte_Write_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLeStatus(fl_ABS_WhlMilgFrntLeStatus); 
        Rte_Write_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRi(fl_ABS_WhlMilgFrntRi);
        Rte_Write_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRiStatus(fl_ABS_WhlMilgFrntRiStatus);
	}

	uint8 ret_Engspd;
	uint16 Engspd = 0;
	uint8 fl_Charge_U8;
	ret_Engspd =  Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&Engspd);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSts(&fl_Charge_U8);
	if((ret_Engspd &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{
		Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((ret_Engspd &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(0);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(Engspd);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSts(fl_Charge_U8);
	}

	uint8 coolantTmp = 0;
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngCoolanTemp(&coolantTmp);	
	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngCoolanTemp(coolantTmp);
	
	/* cpang1 */
	uint8 fl_EngOperationSts;
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus(&fl_EngOperationSts);
	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus(fl_EngOperationSts);

	//kdong
	uint8	fl_BatteryVoltageError;
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_BatteryVoltageError(&fl_BatteryVoltageError);
	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_BatteryVoltageError(fl_BatteryVoltageError);

    /*cpang1 add*/
    uint8 fl_EMS_SpeedLimtHour;
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtHour(&fl_EMS_SpeedLimtHour);
    Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtHour(fl_EMS_SpeedLimtHour);

    /*cpang1 add*/
    uint8 fl_EMS_SpeedLimtMin;
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtMin(&fl_EMS_SpeedLimtMin);
    Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtMin(fl_EMS_SpeedLimtMin);

    /*cpang1 add*/
    uint8 fl_EMS_TroqLimtHour;
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtHour(&fl_EMS_TroqLimtHour);
    Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtHour(fl_EMS_TroqLimtHour);

    /*cpang1 add*/
    uint8 fl_EMS_TroqLimtMin;
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtMin(&fl_EMS_TroqLimtMin);
    Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtMin(fl_EMS_TroqLimtMin);
	
    /*menu button*/
    uint8 menu_up;
    uint8 ret_menu_up;
    //menu up
	ret_menu_up = Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Up(&menu_up);
	if((ret_menu_up &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{	
		Rte_Write_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((ret_menu_up &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		Rte_Write_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_Tout(0);
        Rte_Write_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Up(menu_up);
	}

    //menu down
    uint8 menu_down;
    Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Down(&menu_down);
    Rte_Write_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Down(menu_down);

    //menu left
    uint8 menu_left;
    Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Left(&menu_left);
    Rte_Write_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Left(menu_left);
 
    //menu right
    uint8 menu_right;
    Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Right(&menu_right);
    Rte_Write_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Right(menu_right);

    //menu confirm
    uint8 menu_confirm;
    Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Confirm(&menu_confirm);
    Rte_Write_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Confirm(menu_confirm);

    //menu return
    uint8 menu_return;
    Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Return(&menu_return);
    Rte_Write_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Return(menu_return);
	
	uint8 fl_GW_TCU1_Drivemode_U8;
	uint8 fl_GW_TCU1_Driving_req_U8;
	uint8 fl_ESP_EPBStatus_U8;
	// uint8 fl_ESP_EPBErrorStatus_U8; //was declared but never referenced
	uint8 fl_ESP_ESPInfoLamp_U8;
	uint8 fl_ESP_VdcFlgFlt_U8;
	uint8 fl_ESP_TcsFlgFlt_U8;
	uint8 fl_ESP_EspEnableSts_U8;
	uint8 fl_ESP_CdpAvailable_U8;
	uint8 fl_retGW_TCU1_Drivemode_U8 = 0;
	uint8 u8_EPBWarningMessage1; //myang10
	uint8 u8_EPBWarningMessage2;
	uint8 u8_EPBWarningMessage3;
	uint8 u8_AVHWarningMessage;
	uint8 u8_AvhAvabliable;
	uint8 u8_ESP_AvhActive;
	uint8 u8_ESP_EPBErrorStatus;
	uint8 u8_ESP_EPBStatus;

	
	fl_retGW_TCU1_Drivemode_U8 = Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_VdcFlgFlt(&fl_ESP_VdcFlgFlt_U8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_Driving_mode_req(&fl_GW_TCU1_Driving_req_U8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus(&fl_ESP_EPBStatus_U8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ESPInfoLamp(&fl_ESP_ESPInfoLamp_U8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ATS_Driving_mode_confirm(&fl_GW_TCU1_Drivemode_U8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_TcsFlgFlt(&fl_ESP_TcsFlgFlt_U8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EspEnableSts(&fl_ESP_EspEnableSts_U8);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_CdpAvailable(&fl_ESP_CdpAvailable_U8);

	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage1(&u8_EPBWarningMessage1);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage2(&u8_EPBWarningMessage2);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage3(&u8_EPBWarningMessage3);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AVHWarningMessage(&u8_AVHWarningMessage);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhAvabliable(&u8_AvhAvabliable);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhActive(&u8_ESP_AvhActive);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus(&u8_ESP_EPBErrorStatus);
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus(&u8_ESP_EPBStatus);
	
	if((fl_retGW_TCU1_Drivemode_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retGW_TCU1_Drivemode_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout(0); 
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ATS_Driving_mode_confirm(fl_GW_TCU1_Drivemode_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_Driving_mode_req(fl_GW_TCU1_Driving_req_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus(fl_ESP_EPBStatus_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ESPInfoLamp(fl_ESP_ESPInfoLamp_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_VdcFlgFlt(fl_ESP_VdcFlgFlt_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_TcsFlgFlt(fl_ESP_TcsFlgFlt_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EspEnableSts(fl_ESP_EspEnableSts_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_CdpAvailable(fl_ESP_CdpAvailable_U8);

		Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage1(u8_EPBWarningMessage1);
		Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage2(u8_EPBWarningMessage2);
		Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage3(u8_EPBWarningMessage3);
		Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AVHWarningMessage(u8_AVHWarningMessage);
		Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhAvabliable(u8_AvhAvabliable);
		Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhActive(u8_ESP_AvhActive);
		Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus(u8_ESP_EPBErrorStatus);
		Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus(u8_ESP_EPBStatus);
	}

	uint8 fl_AvhActive_U8 = 0;
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhActive(&fl_AvhActive_U8);
	Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhActive(fl_AvhActive_U8);
	
	uint8 fl_GW_TCU1_U8;
	uint8 fl_retGW_TCU1_U8 = 0;
	uint8 fl_retTCU_DisplayGear_U8 = 0;

	fl_retGW_TCU1_U8 = Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear(&fl_GW_TCU1_U8);
	Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_GearSelectorReq(&fl_retTCU_DisplayGear_U8);
	if((fl_retGW_TCU1_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retGW_TCU1_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_Tout(0); 
	//	Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_GearSelectorReq(fl_GW_TCU1_U8);
	//	Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear(fl_retTCU_DisplayGear_U8);
	 	Rte_Write_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear(fl_GW_TCU1_U8);
		Rte_Write_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_GearSelectorReq(fl_retTCU_DisplayGear_U8);
	}

	//uint8 fl_GW_TCU1_DisplayGear_U8;
	//Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear(&fl_GW_TCU1_DisplayGear_U8);
	//Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear(fl_GW_TCU1_DisplayGear_U8);


	uint8 fl_tbox_hour;
	uint8 fl_tbox_minute;
	uint8 fl_tbox_year;
	uint8 fl_tbox_month;
	uint8 fl_tbox_date;
	uint8 fl_tbox_second;
	uint8 fl_tbox_ret = 0;
	uint8 fl_real_Time_ub;
	

	fl_tbox_ret = Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Hour(&fl_tbox_hour);
	Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Minute(&fl_tbox_minute);
	Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Year(&fl_tbox_year);
	Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Month(&fl_tbox_month);
	Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Date(&fl_tbox_date);
	Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Second(&fl_tbox_second);
	Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_RealTime_UB_Copy_1(&fl_real_Time_ub);
	
	if((fl_tbox_ret &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_tbox_ret &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_Tout(0); 
		Rte_Write_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Hour(fl_tbox_hour);
		Rte_Write_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Minute(fl_tbox_minute);		
		Rte_Write_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Second(fl_tbox_second);
		Rte_Write_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Year(fl_tbox_year);
		Rte_Write_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Month(fl_tbox_month);
		Rte_Write_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Date(fl_tbox_date);
		Rte_Write_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_RealTime_UB_Copy_1(fl_real_Time_ub);
	}
	
	uint8 fl_GEAR_SHIFT_U8;
	uint8 fl_EMS_MIL_U8;
	uint8 fl_EMS_SVS_U8;
	uint8 fl_retGEAR_SHIFT_U8 = 0;

	fl_retGEAR_SHIFT_U8 = Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_RemindShiftGear_MT(&fl_GEAR_SHIFT_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_MIL(&fl_EMS_MIL_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_SVS(&fl_EMS_SVS_U8);
	if((fl_retGEAR_SHIFT_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retGEAR_SHIFT_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(0); 
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_RemindShiftGear_MT(fl_GEAR_SHIFT_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_MIL(fl_EMS_MIL_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_SVS(fl_EMS_SVS_U8);
	}

	//kdong->EMS_OBDWarn
	uint8 fl_EMS_OBDWarn;
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDWarn(&fl_EMS_OBDWarn);
	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDWarn(fl_EMS_OBDWarn);
	//Yxiong2
	uint8 fl_EMS_OBDTxt;
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDTxt(&fl_EMS_OBDTxt);
	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDTxt(fl_EMS_OBDTxt);

	//kdong
	uint8 fl_EMS_StartStopStatus;
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_StartStopStatus(&fl_EMS_StartStopStatus);
	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_StartStopStatus(fl_EMS_StartStopStatus);
	
	uint16 fl_outside_temp_U16;
	uint8 fl_BCM_BrakeFluidSts;	
	uint8 fl_outside_temp_ret = 0;
	uint8 fl_BCM_PowerMode;	
						  
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_OutsideTemp(&fl_outside_temp_U16);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_PowerMode(&fl_BCM_PowerMode);
	fl_outside_temp_ret = Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_BrakeFluidSts(&fl_BCM_BrakeFluidSts);
	if((fl_outside_temp_ret &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_BCM_0x313_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_outside_temp_ret &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_BCM_0x313_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_BCM_0x313_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_BCM_0x313_ComIn_Tout(0); 
		Rte_Write_ppSR_CANMSG_BCM_0x313_ComIn_BCM_OutsideTemp(fl_outside_temp_U16);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_BrakeFluidSts(fl_BCM_BrakeFluidSts);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_PowerMode(fl_BCM_PowerMode);
	}

	uint8 fl_PTCaseMod_U8;
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_PTCaseMod(&fl_PTCaseMod_U8);
	Rte_Write_ppSR_CANMSG_BCM_0x313_ComIn_BCM_PTCaseMod(fl_PTCaseMod_U8);
	
	uint8 fl_EngSts_U16;						  
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSts(&fl_EngSts_U16);
	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSts(fl_EngSts_U16);

	uint8 fl_EMS_CluthPedalStatus;						  
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_CluthPedalStatus(&fl_EMS_CluthPedalStatus);
	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_CluthPedalStatus(fl_EMS_CluthPedalStatus);

	//BCM_PosLmpSts
	uint8 fl_BCM_PosLmpSts;
	uint8 fl_BCM_TurnIndicatorSts_U8;
	uint8 fl_retBCM_TurnIndicatorSts_U8 = 0;

	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpSts(&fl_BCM_PosLmpSts);
	fl_retBCM_TurnIndicatorSts_U8 = Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorSts(&fl_BCM_TurnIndicatorSts_U8);
	
	if((fl_retBCM_TurnIndicatorSts_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retBCM_TurnIndicatorSts_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(0); 
		Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorSts(fl_BCM_TurnIndicatorSts_U8);
    	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpSts(fl_BCM_PosLmpSts);
	}
	//Yxiong2:
	//BCM_PosLmpDig
	//BCM_LowBeamDig
	uint8 fl_Low_U8=0;
	uint8 fl_Pos_U8 = 0;
	uint8 fl_FrnFog_U8 = 0;
	uint8 fl_RrFog_U8 = 0;
	uint8 fl_TrnInd_U8 = 0;
	uint8 fl_Rvr_U8 = 0;
	uint8 fl_Brk_U8 = 0;
	uint8 fl_Day_U8 = 0;
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamDig(&fl_Low_U8);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamDig(fl_Low_U8);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpDig(&fl_Pos_U8);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpDig(fl_Pos_U8);
	
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpDig(&fl_FrnFog_U8);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpDig(fl_FrnFog_U8);
	
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpDig(&fl_RrFog_U8);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpDig(fl_RrFog_U8);
	
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorDig(&fl_TrnInd_U8);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorDig(fl_TrnInd_U8);
	
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_reversinglightDig(&fl_Rvr_U8);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_reversinglightDig(fl_Rvr_U8);
	
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BrakelightDig(&fl_Brk_U8);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BrakelightDig(fl_Brk_U8);
	
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_DayRunningLampDig(&fl_Day_U8);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_DayRunningLampDig(fl_Day_U8);
	//Yxiong2
	
	uint8 fl_SRS_AirBagFailSts_U8;
	uint8 fl_retSRS_AirBagFailSts_U8 = 0;

	fl_retSRS_AirBagFailSts_U8 = Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_AirBagFailSts(&fl_SRS_AirBagFailSts_U8);
	if((fl_retSRS_AirBagFailSts_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_SRS_0x350_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retSRS_AirBagFailSts_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_SRS_0x350_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_GW_SRS_0x350_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_GW_SRS_0x350_ComIn_Tout(0); 
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_SRS_0x350_ComIn_SRS_AirBagFailSts(fl_SRS_AirBagFailSts_U8);
	}

	/* msg id: GW_ESP_Status_Chassis_0x243  start */
	//ESP_EPBErrorStatus
    uint8 fl_ESP_EPBErrorStatus;
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus(&fl_ESP_EPBErrorStatus);
	Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus(fl_ESP_EPBErrorStatus);

	//ESP_HDCAvailable
	uint8 fl_ESP_HDCAvailable;						  
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_HDCAvailable(&fl_ESP_HDCAvailable);
	Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_HDCAvailable(fl_ESP_HDCAvailable);

	//ESP_AvhStandby
	uint8 fl_ESP_AvhStandby;						  
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhStandby(&fl_ESP_AvhStandby);
	Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhStandby(fl_ESP_AvhStandby);

	//kdong
	uint8 fl_ESP_HDCActive;
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_HDCActive(&fl_ESP_HDCActive);
	Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_HDCActive(fl_ESP_HDCActive);

	/* msg id: GW_ESP_Status_Chassis_0x243  end */

	
	/* msg id: GW_EMS_0x368  start */
	//DCM_GlowPlugLmpReq
	uint8 fl_DCM_GlowPlugLmpReq;
    uint8 fl_DCM_GlowPlugLmpReq_ret;
	// uint8 fl_DPF_WRN_UreaWarn;
	
	fl_DCM_GlowPlugLmpReq_ret = Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_DCM_GlowPlugLmpReq(&fl_DCM_GlowPlugLmpReq);
	
	if((fl_DCM_GlowPlugLmpReq_ret &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{	
		Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_DCM_GlowPlugLmpReq_ret &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_Tout(0);
		// Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_GasPrtcWarn_G_Rq(fl_DPF_WRN_UreaWarn);
        Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_DCM_GlowPlugLmpReq(fl_DCM_GlowPlugLmpReq);
	}
	
	//DCM_WIFFullLmpReq
	uint8 fl_DCM_WIFFullLmpReq;						  
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_DCM_WIFFullLmpReq(&fl_DCM_WIFFullLmpReq);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_DCM_WIFFullLmpReq(fl_DCM_WIFFullLmpReq);

	//kdong->EMS_UreaWarn
	uint8 fl_EMS_UreaWarn;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaWarn(&fl_EMS_UreaWarn);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaWarn(fl_EMS_UreaWarn);


	uint8 fl_RF_keyLowBatteryWaring_U8;
	uint8 fl_RF_keyLowBatteryWaring_U8_ret =0;
	fl_RF_keyLowBatteryWaring_U8_ret = Rte_Read_rpSR_CANMSG_RF_0x326_ComIn_RF_KeyLowBatteryWarning(&fl_RF_keyLowBatteryWaring_U8);
	if((fl_RF_keyLowBatteryWaring_U8_ret &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{	
		Rte_Write_ppSR_CANMSG_RF_0x326_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_RF_keyLowBatteryWaring_U8_ret &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		Rte_Write_ppSR_CANMSG_RF_0x326_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_RF_0x326_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_RF_0x326_ComIn_Tout(0);
        Rte_Write_ppSR_CANMSG_RF_0x326_ComIn_RF_KeyLowBatteryWarning(fl_RF_keyLowBatteryWaring_U8);
	}

	 
	//Yxiong2
	uint8 fl_EMS_UreaTxt;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaTxt(&fl_EMS_UreaTxt);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaTxt(fl_EMS_UreaTxt);
	uint16 fl_EMS_UreaDTE;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaDTE(&fl_EMS_UreaDTE);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaDTE(fl_EMS_UreaDTE);
	uint8 fl_EMS_UreaQty;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaQty(&fl_EMS_UreaQty);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaQty(fl_EMS_UreaQty);

	//Pwu5
	uint8 fl_EMS_AddUreaWarning;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_AddUreaWarning(&fl_EMS_AddUreaWarning);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_AddUreaWarning(fl_EMS_AddUreaWarning);
	uint8 fl_EMS_UreaSysWarning;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaSysWarning(&fl_EMS_UreaSysWarning);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaSysWarning(fl_EMS_UreaSysWarning);
	uint8 fl_EMS_NoxWarning;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_NoxWarning(&fl_EMS_NoxWarning);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_NoxWarning(fl_EMS_NoxWarning);
	
	// /*myang10*/
	// uint8 fl_DPF_WRN_UreaWarn;
	// Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq(&fl_DPF_WRN_UreaWarn);
	// Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq(fl_DPF_WRN_UreaWarn);
	/*tzhang1*/
	uint8 ems_urealvl;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaLvl(&ems_urealvl);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaLvl(ems_urealvl);

	/* cpang1 */
	uint8 fl_DieslPrtcWarn_D_Rq;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq(&fl_DieslPrtcWarn_D_Rq);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq(fl_DieslPrtcWarn_D_Rq);

	uint8 fl_EMS_Soot_Pc_Dsply_U8;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_Soot_Pc_Dsply(&fl_EMS_Soot_Pc_Dsply_U8);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_Soot_Pc_Dsply(fl_EMS_Soot_Pc_Dsply_U8);

	/* msg id: GW_EMS_0x368  end */
	
	/* msg id: GW_TCU_Info_0x395 start*/

	uint8 fl_TCU_Driverwarning;
	uint8 fl_TrnSrvcRqd_B_Rq;
	uint8 fl_TCU_Driverwarning_ret;
	fl_TCU_Driverwarning_ret = Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TrnSrvcRqd_B_Rq(&fl_TrnSrvcRqd_B_Rq);
	Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Driverwarning(&fl_TCU_Driverwarning);
	if((fl_TCU_Driverwarning_ret &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{	
		Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_TCU_Driverwarning_ret &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TrnSrvcRqd_B_Rq(fl_TrnSrvcRqd_B_Rq);
		Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_Tout(0);
        Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Driverwarning(fl_TCU_Driverwarning);
	}

	uint8 fl_TCU_Gearbox;
	Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox(&fl_TCU_Gearbox);
	Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox(fl_TCU_Gearbox);

	uint8 fl_TCU_GearboxP_Faulty;
	Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_GearboxP_Faulty(&fl_TCU_GearboxP_Faulty);
	Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_GearboxP_Faulty(fl_TCU_GearboxP_Faulty);

	uint8 fl_TCU_Gearbox_FaultyD;
	Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_FaultyD(&fl_TCU_Gearbox_FaultyD);
	Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_FaultyD(fl_TCU_Gearbox_FaultyD);

	uint8 fl_TCU_Gearbox_FaultyP;
	Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_FaultyP(&fl_TCU_Gearbox_FaultyP);
	Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_FaultyP(fl_TCU_Gearbox_FaultyP);

	uint8 fl_Gearbox_TransmissionSignals;
	Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_TransmissionSignals(&fl_Gearbox_TransmissionSignals);
	Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_TransmissionSignals(fl_Gearbox_TransmissionSignals);

	uint8 fl_TCU_unknown_D_P;
	Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_unknown_D_P(&fl_TCU_unknown_D_P);
	Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_unknown_D_P(fl_TCU_unknown_D_P);

	uint8 fl_Trans_Over_Heat;
	Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_Trans_Over_Heat(&fl_Trans_Over_Heat);
	Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_Trans_Over_Heat(fl_Trans_Over_Heat);

    /* cpang1 */
    uint8 fl_TCU_DrvShiftAllwCondRmd; 
    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_DrvShiftAllwCondRmd(&fl_TCU_DrvShiftAllwCondRmd);
    Rte_Write_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_DrvShiftAllwCondRmd(fl_TCU_DrvShiftAllwCondRmd);
	/* msg id: GW_TCU_Info_0x395 end*/
	
	/* msg id: BCM_Lamp_0x318  start */
	
	//BCM_HighBeamSts
	uint8 fl_BCM_HighBeamSts;						  
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_HighBeamSts(&fl_BCM_HighBeamSts);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_HighBeamSts(fl_BCM_HighBeamSts);

	//BCM_LowBeamSts
	uint8 fl_BCM_LowBeamSts;						  
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamSts(&fl_BCM_LowBeamSts);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamSts(fl_BCM_LowBeamSts);

	//BCM_RearFogLmpSts
	uint8 fl_BCM_RearFogLmpSts;						  
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpSts(&fl_BCM_RearFogLmpSts);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpSts(fl_BCM_RearFogLmpSts);

	//BCM_FrntFogLmpSts
	uint8 fl_BCM_FrntFogLmpSts;						  
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpSts(&fl_BCM_FrntFogLmpSts);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpSts(fl_BCM_FrntFogLmpSts);

	//BCM_TurnIndicatorSts
	//uint8 fl_BCM_TurnIndicatorSts;						  
	//Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorSts(&fl_BCM_TurnIndicatorSts);
	//Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorSts(fl_BCM_TurnIndicatorSts);

	//kdong->BCM_BackHomeSts
	uint8 fl_BCM_BackHomeSts;
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BackHomeSts(&fl_BCM_BackHomeSts);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BackHomeSts(fl_BCM_BackHomeSts);

	//kdong->BCM_BuzzerReq
	uint8 fl_BCM_BuzzerReq;
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BuzzerReq(&fl_BCM_BuzzerReq);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BuzzerReq(fl_BCM_BuzzerReq);

	uint8 fl_ImmoSts_U8;
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_ImmoSts(&fl_ImmoSts_U8);
	Rte_Write_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_ImmoSts(fl_ImmoSts_U8);

	/* msg id: BCM_Lamp_0x318  end */

	/* msg id: GW_SRS_0x350  start */
	//SRS_SBR_Passenger
	uint8 fl_SRS_SBR_Passenger;
  Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger(&fl_SRS_SBR_Passenger);
  Rte_Write_ppSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger(fl_SRS_SBR_Passenger);

	//SRS_SBR_Driver
	uint8 fl_SRS_SBR_Driver;						  
	Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver(&fl_SRS_SBR_Driver);
	Rte_Write_ppSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver(fl_SRS_SBR_Driver);
	/* msg id: GW_SRS_0x350  end */

	//EPS_S_SafeLampRed
	uint8 fl_EPS_S_SafeLampRed;
	uint8 fl_EPS_S_SafeLampRed_ret;

	fl_EPS_S_SafeLampRed_ret = Rte_Read_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_SafeLampRed(&fl_EPS_S_SafeLampRed);
	if((fl_EPS_S_SafeLampRed_ret &	RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{	
		Rte_Write_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_EPS_S_SafeLampRed_ret &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		Rte_Write_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_Tout(0);
		Rte_Write_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_SafeLampRed(fl_EPS_S_SafeLampRed);
	}
	//EPS_S_WarningLampYellow
	uint8 fl_EPS_S_WarningLampYellow;
	Rte_Read_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_WarningLampYellow(&fl_EPS_S_WarningLampYellow);
	Rte_Write_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_WarningLampYellow(fl_EPS_S_WarningLampYellow);
	
	//kdong
	uint8 fl_PEPS_KeyInOnChimeStatus_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyReminderWarning(&fl_PEPS_KeyInOnChimeStatus_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyReminderWarning(fl_PEPS_KeyInOnChimeStatus_U8);

	//kdong
	uint8 fl_PAM_FrontSysTemWorkSts;
	uint8 fl_PAM_RearSysTemWorkSts;
	uint8 fl_PAM_ChimeCmdSoundStatus;
	uint8 fl_PAM_ChimeCmdSoundCadence;
	uint8 fl_PAM_FrontSysTemWorkSts_ret;

	fl_PAM_FrontSysTemWorkSts_ret = Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_FrontSysTemWorkSts(&fl_PAM_FrontSysTemWorkSts);
	Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_RearSysTemWorkSts(&fl_PAM_RearSysTemWorkSts);
	Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundStatus(&fl_PAM_ChimeCmdSoundStatus);
	Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundCadence(&fl_PAM_ChimeCmdSoundCadence);
	if((fl_PAM_FrontSysTemWorkSts_ret &	RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{	
		Rte_Write_ppSR_CANMSG_GW_PAM_0x271_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_PAM_FrontSysTemWorkSts_ret &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		Rte_Write_ppSR_CANMSG_GW_PAM_0x271_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_PAM_0x271_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_PAM_0x271_ComIn_Tout(0);
		Rte_Write_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_FrontSysTemWorkSts(fl_PAM_FrontSysTemWorkSts);
		Rte_Write_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_RearSysTemWorkSts(fl_PAM_RearSysTemWorkSts);
		Rte_Write_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundStatus(fl_PAM_ChimeCmdSoundStatus);
		Rte_Write_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundCadence(fl_PAM_ChimeCmdSoundCadence);
	}

	uint8 fl_AWD_EngTqAtCluLmtdByTrfCaseEn;
	uint8 fl_AWD_EngTqAtCluLmtdByTrfCaseEn_ret;

	fl_AWD_EngTqAtCluLmtdByTrfCaseEn_ret = Rte_Read_rpSR_CANMSG_GW_AWD_0x251_ComIn_AWD_EngTqAtCluLmtdByTrfCaseEn(&fl_AWD_EngTqAtCluLmtdByTrfCaseEn);
	if((fl_AWD_EngTqAtCluLmtdByTrfCaseEn_ret &	RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{	
		Rte_Write_ppSR_CANMSG_GW_AWD_0x251_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_AWD_EngTqAtCluLmtdByTrfCaseEn_ret &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		Rte_Write_ppSR_CANMSG_GW_AWD_0x251_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_AWD_0x251_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_AWD_0x251_ComIn_Tout(0);
		Rte_Write_ppSR_CANMSG_GW_AWD_0x251_ComIn_AWD_EngTqAtCluLmtdByTrfCaseEn(fl_AWD_EngTqAtCluLmtdByTrfCaseEn);
	}

	uint8 fl_AWD_TrfCaseFltLmpReq;
	Rte_Read_rpSR_CANMSG_GW_AWD_0x251_ComIn_AWD_TrfCaseFltLmpReq(&fl_AWD_TrfCaseFltLmpReq);
	Rte_Write_ppSR_CANMSG_GW_AWD_0x251_ComIn_AWD_TrfCaseFltLmpReq(fl_AWD_TrfCaseFltLmpReq);


	//myang10
	uint8 fl_PEPS_SSB_FailStatus_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_SSB_FailStatus(&fl_PEPS_SSB_FailStatus_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_SSB_FailStatus(fl_PEPS_SSB_FailStatus_U8);
	
	uint8 fl_PEPS_DoorOpenNotOFFWarning_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenNotOFFWarning(&fl_PEPS_DoorOpenNotOFFWarning_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenNotOFFWarning(fl_PEPS_DoorOpenNotOFFWarning_U8);

	uint8 fl_PEPS_DoorOpenWarning_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenWarning(&fl_PEPS_DoorOpenWarning_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenWarning(fl_PEPS_DoorOpenWarning_U8);

	uint8 fl_PEPS_RemainingNumSts_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_RemainingNumSts(&fl_PEPS_RemainingNumSts_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_RemainingNumSts(fl_PEPS_RemainingNumSts_U8);

	uint8 fl_PEPS_NoKeyFoundWarning_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NoKeyFoundWarning(&fl_PEPS_NoKeyFoundWarning_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_NoKeyFoundWarning(fl_PEPS_NoKeyFoundWarning_U8);

	uint8 fl_PEPS_KeyOutWarning1_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyOutWarning1(&fl_PEPS_KeyOutWarning1_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyOutWarning1(fl_PEPS_KeyOutWarning1_U8);

	uint8 fl_PEPS_KeyBatteryWarning_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyBatteryWarning(&fl_PEPS_KeyBatteryWarning_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyBatteryWarning(fl_PEPS_KeyBatteryWarning_U8);

	uint8 fl_PEPS_BrakeIndicationWarning_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_BrakeIndicationWarning(&fl_PEPS_BrakeIndicationWarning_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_BrakeIndicationWarning(fl_PEPS_BrakeIndicationWarning_U8);

	uint8 fl_PEPS_ClutchIndicationWarning_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ClutchIndicationWarning(&fl_PEPS_ClutchIndicationWarning_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_ClutchIndicationWarning(fl_PEPS_ClutchIndicationWarning_U8);

    /* cpang1 */
    uint8 fl_PEPS_NotNWarning_U8;
    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotNWarning(&fl_PEPS_NotNWarning_U8);
    Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotNWarning(fl_PEPS_NotNWarning_U8);
    
	uint8 fl_PEPS_NotPNWarning_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotPNWarning(&fl_PEPS_NotPNWarning_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotPNWarning(fl_PEPS_NotPNWarning_U8);
	
	//Yxiong2
	uint8 fl_PEPS_PowerMode_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_PowerMode(&fl_PEPS_PowerMode_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_PowerMode(fl_PEPS_PowerMode_U8);

	uint8 fl_ESP_ATS_failure_Lamp_U8;
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ATS_failure_Lamp(&fl_ESP_ATS_failure_Lamp_U8);
	Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ATS_failure_Lamp(fl_ESP_ATS_failure_Lamp_U8);
	
	uint8 fl_ESP_AvhAvabliable_U8;
	Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhAvabliable(&fl_ESP_AvhAvabliable_U8);
	Rte_Write_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhAvabliable(fl_ESP_AvhAvabliable_U8);
	
	//Chenzb ->
	uint8 fl_BCM_DoorStsFrntLe_U8;
	uint8 fl_DoorSts_signal_value_ret=0;
	

	fl_DoorSts_signal_value_ret = Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&fl_BCM_DoorStsFrntLe_U8);
	if((fl_DoorSts_signal_value_ret &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{	
		Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_DoorSts_signal_value_ret &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		
		Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_Tout(0);
		Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(fl_BCM_DoorStsFrntLe_U8);

	}	

	//uint8 fl_BCM_TurnIndicatorSts_U8;
	//Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_TurnIndicatorSts(&fl_BCM_TurnIndicatorSts_U8);
	//Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_TurnIndicatorSts(fl_BCM_TurnIndicatorSts_U8);
	
	//Chenzb->
	uint8 fl_BCM_DoorStsFrntRi_U8;
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi(&fl_BCM_DoorStsFrntRi_U8);
	Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi(fl_BCM_DoorStsFrntRi_U8);

	uint8 fl_BCM_DoorStsRearLe_U8;
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearLe(&fl_BCM_DoorStsRearLe_U8);
	Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearLe(fl_BCM_DoorStsRearLe_U8);

	uint8 fl_BCM_DoorStsRearRi_U8;
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearRi(&fl_BCM_DoorStsRearRi_U8);
	Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearRi(fl_BCM_DoorStsRearRi_U8);

	uint8 fl_BCM_DoorStsTrunk_U8;
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsTrunk(&fl_BCM_DoorStsTrunk_U8);
	Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsTrunk(fl_BCM_DoorStsTrunk_U8);

	uint8 fl_BCM_EnginehoodSts_U8;
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_EnginehoodSts(&fl_BCM_EnginehoodSts_U8);
	Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_EnginehoodSts(fl_BCM_EnginehoodSts_U8);

	uint8 fl_MT_NeutralSensorSts_U8;
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_MT_NeutralSensorSts(&fl_MT_NeutralSensorSts_U8);
	Rte_Write_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_MT_NeutralSensorSts(fl_MT_NeutralSensorSts_U8);

	//Chenzb->
	uint8 fl_PEPS_ImmoChime_U8;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus(&fl_PEPS_ImmoChime_U8);
	Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus(fl_PEPS_ImmoChime_U8);
	//Yxiong2 ->
	uint8 fl_PEPS_ESCL_FailStatus_U8;
	uint8 fl_PEPS_SystemFailStatus_U8;
	uint8 fl_PEPS_IMMO_MatchStatus_U8;
	uint8 fl_retPEPS_ESCL_FailStatus_U8 = 0;
	fl_retPEPS_ESCL_FailStatus_U8 = Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_SystemFailStatus(&fl_PEPS_SystemFailStatus_U8);
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ESCL_FailStatus(&fl_PEPS_ESCL_FailStatus_U8);
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus(&fl_PEPS_IMMO_MatchStatus_U8);
	if((fl_retPEPS_ESCL_FailStatus_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retPEPS_ESCL_FailStatus_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_PEPS_0x325_ComIn_Tout(0); 
		Rte_Write_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_ESCL_FailStatus(fl_PEPS_ESCL_FailStatus_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_SystemFailStatus(fl_PEPS_SystemFailStatus_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus(fl_PEPS_IMMO_MatchStatus_U8);
	}
	
	uint8 fl_TPMS_LF_Pressure_U8;
	uint8 fl_TPMS_RF_Pressure_U8;
	uint8 fl_TPMS_RR_Pressure_U8;
	uint8 fl_TPMS_LR_Pressure_U8;
	uint8 fl_TPMS_LF_Pressure_Warning_U8;
	uint8 fl_TPMS_RF_Pressure_Warning_U8;
	uint8 fl_TPMS_RR_Pressure_Warning_U8;
	uint8 fl_TPMS_LR_Pressure_Warning_U8;
	uint8 fl_TPMS_Temperature_Warning_U8;
	uint8 fl_Tire_Position_U8;
	uint8 fl_TPMS_System_Status_U8;
	uint8 fl_retTPMS_LF_Pressure_U8 = 0;
	fl_retTPMS_LF_Pressure_U8 = Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure(&fl_TPMS_LF_Pressure_U8);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure(&fl_TPMS_RF_Pressure_U8);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure(&fl_TPMS_RR_Pressure_U8);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure(&fl_TPMS_LR_Pressure_U8);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning(&fl_TPMS_LF_Pressure_Warning_U8);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning(&fl_TPMS_RF_Pressure_Warning_U8);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning(&fl_TPMS_RR_Pressure_Warning_U8);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning(&fl_TPMS_LR_Pressure_Warning_U8);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Temperature_Warning(&fl_TPMS_Temperature_Warning_U8);

	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position(&fl_Tire_Position_U8);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status(&fl_TPMS_System_Status_U8);
	
	if((fl_retTPMS_LF_Pressure_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retTPMS_LF_Pressure_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_Tout(0); 
		Rte_Write_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure(fl_TPMS_LF_Pressure_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure(fl_TPMS_RF_Pressure_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure(fl_TPMS_RR_Pressure_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure(fl_TPMS_LR_Pressure_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning(fl_TPMS_LF_Pressure_Warning_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning(fl_TPMS_RF_Pressure_Warning_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning(fl_TPMS_RR_Pressure_Warning_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning(fl_TPMS_LR_Pressure_Warning_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_Temperature_Warning(fl_TPMS_Temperature_Warning_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position(fl_Tire_Position_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status(fl_TPMS_System_Status_U8);

	}
	uint8 fl_msg_temp_U8; //myang10
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Tire_Temperature(&fl_msg_temp_U8);
	Rte_Write_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Tire_Temperature(fl_msg_temp_U8);
	uint8 fl_lamp_sts_U8;//myang10
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Lamp_Status(&fl_lamp_sts_U8);
	Rte_Write_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Lamp_Status(fl_lamp_sts_U8);


	
	uint8 fl_EMS_CruiseControlStatus_U8;
	uint8 fl_retEMS_CruiseControlStatus_U8 = 0;
	fl_retEMS_CruiseControlStatus_U8 = Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus(&fl_EMS_CruiseControlStatus_U8);
	
	if((fl_retEMS_CruiseControlStatus_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retEMS_CruiseControlStatus_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_Tout(0); 
		Rte_Write_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus(fl_EMS_CruiseControlStatus_U8);
	}

	uint16 fl_EMS_TargCruiseSpeed_U16;						  
	Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_TargCruiseSpeed(&fl_EMS_TargCruiseSpeed_U16);
	Rte_Write_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_TargCruiseSpeed(fl_EMS_TargCruiseSpeed_U16);

	uint8 fl_EMS_EngineStopStartStatus_u8;
	Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_EngineStopStartStatus(&fl_EMS_EngineStopStartStatus_u8);
	Rte_Write_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_EngineStopStartStatus(fl_EMS_EngineStopStartStatus_u8);

	uint8 fl_EMS_CruiseControlStatus_u8;
	Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus(&fl_EMS_CruiseControlStatus_u8);
	Rte_Write_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus(fl_EMS_CruiseControlStatus_u8);
	
	uint8 fl_EMS_EngCoolanTemp_U16;						  
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngCoolanTemp(&fl_EMS_EngCoolanTemp_U16);
	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngCoolanTemp(fl_EMS_EngCoolanTemp_U16);

	uint8 fl_BCM_Mechanical_Hand_Brake;	
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_Mechanical_Hand_Brake(&fl_BCM_Mechanical_Hand_Brake);
	Rte_Write_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_Mechanical_Hand_Brake(fl_BCM_Mechanical_Hand_Brake);
	//<-Yxiong2	

	//sli34
	uint16 fl_fuel_level_volumn;
	Rte_Read_rpSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn(&fl_fuel_level_volumn);
	Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn(fl_fuel_level_volumn);

	
	uint8 fl_fuel_level_volumnQF;
	Rte_Read_rpSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_VolumnQF(&fl_fuel_level_volumnQF);
	Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_VolumnQF(fl_fuel_level_volumnQF);
	
	uint8 fl_IC_ManRgen_D_Rq;
	Rte_Read_rpSR_CANMSG_IC_0x525_ComOut_IC_ManRgen_D_Rq(&fl_IC_ManRgen_D_Rq);
	Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_ManRgen_D_Rq(fl_IC_ManRgen_D_Rq);
	
    //dfang1 add
	uint8 fl_IC_AlarmOrNot;
	Rte_Read_rpSR_CANMSG_IC_0x525_ComOut_IC_AlarmOrNot(&fl_IC_AlarmOrNot);
	Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_AlarmOrNot(fl_IC_AlarmOrNot);

    //dfang1 add
	uint8 fl_IC_AlarmStatus;
	Rte_Read_rpSR_CANMSG_IC_0x525_ComOut_IC_AlarmStatus(&fl_IC_AlarmStatus);
	Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_AlarmStatus(fl_IC_AlarmStatus);
	

//	uint16 fl_IC_AFC_Type;
//	Rte_Read_rpSR_CANMSG_IC_0x525_ComOut_IC_AFC_Type(&fl_IC_AFC_Type);
//	Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_AFC_Type(fl_IC_AFC_Type);

	//GengShuo
	uint16 IC_VehSpd_HMI_U16;
	Rte_Read_rpSR_CANMSG_IC_0x510_ComOut_IC_VehSpd_HMI(&IC_VehSpd_HMI_U16);
	Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_VehSpd_HMI(IC_VehSpd_HMI_U16);

	//kdong
	uint8 IC_AirbagTelltaleBehavior_value;
	Rte_Read_rpSR_CANMSG_IC_0x510_ComOut_IC_AirbagTelltaleBehavior(&IC_AirbagTelltaleBehavior_value);
	Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_AirbagTelltaleBehavior(IC_AirbagTelltaleBehavior_value);

	//add by cwu12 190713
	//modify by dfang1
	uint8 IC_BSDSoundSts;
	Rte_Read_ComVnimMdl_rpSR_CANMSG_IC_0x386_ComOut_IC_BSDSoundSts(&IC_BSDSoundSts);
	Rte_Write_ppSR_CANMSG_IC_0x386_ComOut_IC_BSDSoundSts(IC_BSDSoundSts);

	//XGU9
	uint8 IC_IC_Fuel_level=0;
	Rte_Read_ComVnimMdl_rpSR_CANMSG_IC_0x392_ComOut_IC_Fuel_level(&IC_IC_Fuel_level);
	Rte_Write_ppSR_CANMSG_IC_0x392_ComOut_IC_Fuel_level(IC_IC_Fuel_level);

	//uint8 IC_DriveTimehour;
	//Rte_Read_ComVnimMdl_rpSR_CANMSG_IC_0x387_ComOut_IC_DriveTimehour(&IC_DriveTimehour);
	//Rte_Write_ppSR_CANMSG_IC_0x387_ComOut_IC_DriveTimehour(IC_DriveTimehour);

	//uint8 IC_DriveTimeMinute;
	//Rte_Read_ComVnimMdl_rpSR_CANMSG_IC_0x387_ComOut_IC_DriveTimeMinute(&IC_DriveTimeMinute);
	//Rte_Write_ppSR_CANMSG_IC_0x387_ComOut_IC_DriveTimeMinute(IC_DriveTimeMinute);

	uint8 IC_AverageSpeed = 0;
	Rte_Read_ComVnimMdl_rpSR_CANMSG_IC_0x392_ComOut_IC_AverageSpeed(&IC_AverageSpeed);
	Rte_Write_ppSR_CANMSG_IC_0x392_ComOut_IC_AverageSpeed(IC_AverageSpeed);

	//uint16 IC_ChangOilDistance;
	//Rte_Read_ComVnimMdl_rpSR_CANMSG_IC_0x387_ComOut_IC_ChangOilDistance(&IC_ChangOilDistance);
	//Rte_Write_ppSR_CANMSG_IC_0x387_ComOut_IC_ChangOilDistance(IC_ChangOilDistance);

	/* cpang1 */
	UInt16 IC_ChangOilDistance = 0;
    Rte_Read_rpSR_CANMSG_IC_0x392_ComOut_IC_ChangOilDistance(&IC_ChangOilDistance);
    Rte_Write_ppSR_CANMSG_IC_0x392_ComOut_IC_ChangOilDistance(IC_ChangOilDistance);
    
	//uint8 IC_AverageSpeedType;
	//Rte_Read_ComVnimMdl_rpSR_CANMSG_IC_0x387_ComOut_IC_AverageSpeedType(&IC_AverageSpeedType);
	//Rte_Write_ppSR_CANMSG_IC_0x387_ComOut_IC_AverageSpeedType(IC_AverageSpeedType);

	uint8 EMS_ManRgenTxt_D_RqDsply;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_ManRgenTxt_D_RqDsply(&EMS_ManRgenTxt_D_RqDsply);
	Rte_Write_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_ManRgenTxt_D_RqDsply(EMS_ManRgenTxt_D_RqDsply);
	/*XGU9 end*/
	
	//sli34
	uint16 fl_fuel_consum_signal_value=0;
	uint8 fl_fuel_consum_signal_value_ret=0;
	uint16 fl_fuel_consumptioninst=0;
	fl_fuel_consum_signal_value_ret = Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_Fuelcounter(&fl_fuel_consum_signal_value);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_FuelconsumptionInst(&fl_fuel_consumptioninst);

	if((fl_fuel_consum_signal_value_ret &	RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{	
		Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_fuel_consum_signal_value_ret &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(0);
		Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_Fuelcounter(fl_fuel_consum_signal_value);
		Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_FuelconsumptionInst(fl_fuel_consumptioninst);
	}
	
    //IC_OdometerMasterValue
    uint32 IC_OdometerMasterValue = 0;
    Rte_Read_rpSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(&IC_OdometerMasterValue);
    Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(IC_OdometerMasterValue);

	//IC_QDashACCFail
    uint8 IC_QDashACCFail = 0;
    Rte_Read_rpSR_CANMSG_IC_0x510_ComOut_IC_QDashACCFail(&IC_QDashACCFail);
    Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_QDashACCFail(IC_QDashACCFail);

	uint8 fl_LaneAssit_Status_U8;
	uint8 fl_LaneAssit_RightVisualization_U8;
	uint8 fl_LaneAssit_LeftVisualization_U8;
	uint8 fl_SLASpdlimit_U8;
	uint8 fl_SLASpdlimitUnits_U8;
	uint8 fl_SLAState_U8;
	uint8 fl_IPM_LaneAssit_HandsonReq_U8;
	uint8 fl_IPM_LaneAssit_WarningAudio_U8;
	uint8 fl_IPM_HMA_Status_U8;
	uint8 fl_IPM_LaneAssitType_U8;
	uint8 fl_IPM_DDD_WarningLevel_U8;
	uint8 fl_IPM_DDD_IndexStatus_U8;

	uint8 fl_retLaneAssit_Status_U8 = 0;
	fl_retLaneAssit_Status_U8 = Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status(&fl_LaneAssit_Status_U8);

	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_RightVisualization(&fl_LaneAssit_RightVisualization_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_LeftVisualization(&fl_LaneAssit_LeftVisualization_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimit(&fl_SLASpdlimit_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimitUnits(&fl_SLASpdlimitUnits_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLAState(&fl_SLAState_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_HandsonReq(&fl_IPM_LaneAssit_HandsonReq_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_WarningAudio(&fl_IPM_LaneAssit_WarningAudio_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_HMA_Status(&fl_IPM_HMA_Status_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssitType(&fl_IPM_LaneAssitType_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_WarningLevel(&fl_IPM_DDD_WarningLevel_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_IndexStatus(&fl_IPM_DDD_IndexStatus_U8);

	

	if((fl_retLaneAssit_Status_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_IPM_0x245_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retLaneAssit_Status_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_IPM_0x245_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_IPM_0x245_ComIn_NR(0); 
		Rte_Write_ppSR_CANMSG_GW_IPM_0x245_ComIn_Tout(0); 
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status(fl_LaneAssit_Status_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_RightVisualization(fl_LaneAssit_RightVisualization_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_LeftVisualization(fl_LaneAssit_LeftVisualization_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimit(fl_SLASpdlimit_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimitUnits(fl_SLASpdlimitUnits_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLAState(fl_SLAState_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_HandsonReq(fl_IPM_LaneAssit_HandsonReq_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_WarningAudio(fl_IPM_LaneAssit_WarningAudio_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_HMA_Status(fl_IPM_HMA_Status_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssitType(fl_IPM_LaneAssitType_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_WarningLevel(fl_IPM_DDD_WarningLevel_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_IndexStatus(fl_IPM_DDD_IndexStatus_U8);
	}

	uint8 fl_MRR_ACCMode_U8;
	uint8 fl_MRR_TauGapSet_U8;
	uint8 fl_MRR_dxTarObj_U8;
	uint8 fl_Textinfo_U8;
	UInt16 fl_MRR_vSetDis_U16;
	uint8 fl_MRR_ObjValid_U8;
	uint8 fl_MRR_TakeOverReq_U8;
	uint8 fl_MRR_AEB_STATE_U8;
	uint8 fl_MRR_PCW_STATE_U8;
	uint8 fl_PCW_latentWarning_U8;
	uint8 fl_PCW_preWarning;
	uint8 fl_FCW_preWarning;
	uint8 fl_FCW_latentWarning;
	

	uint8 fl_retMRR_ACCMode_U8 = 0;
	fl_retMRR_ACCMode_U8 = Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode(&fl_MRR_ACCMode_U8);

	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TauGapSet(&fl_MRR_TauGapSet_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_dxTarObj(&fl_MRR_dxTarObj_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Textinfo(&fl_Textinfo_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_vSetDis(&fl_MRR_vSetDis_U16);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_ObjValid(&fl_MRR_ObjValid_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TakeOverReq(&fl_MRR_TakeOverReq_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_AEB_STATE(&fl_MRR_AEB_STATE_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_STATE(&fl_MRR_PCW_STATE_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_latentWarning(&fl_PCW_latentWarning_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_preWarning(&fl_PCW_preWarning);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_preWarning(&fl_FCW_preWarning);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_latentWarning(&fl_FCW_latentWarning);
	
	if((fl_retMRR_ACCMode_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retMRR_ACCMode_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR(0); 
		Rte_Write_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout(0); 
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode(fl_MRR_ACCMode_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TauGapSet(fl_MRR_TauGapSet_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_dxTarObj(fl_MRR_dxTarObj_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Textinfo(fl_Textinfo_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_vSetDis(fl_MRR_vSetDis_U16);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_ObjValid(fl_MRR_ObjValid_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TakeOverReq(fl_MRR_TakeOverReq_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_AEB_STATE(fl_MRR_AEB_STATE_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_STATE(fl_MRR_PCW_STATE_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_latentWarning(fl_PCW_latentWarning_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_preWarning(fl_PCW_preWarning);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_preWarning(fl_FCW_preWarning);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_latentWarning(fl_FCW_latentWarning);
	}
	
	
	uint8 fl_SOD_BLIS_display_U8;
	uint8 fl_SODLCA_OFFTelltale_U8;
	uint8 fl_CTA_OFFTelltale_U8;
	uint8 fl_DOA_OFFTelltale_U8;
	uint8 fl_SOD_CTA_warningReqLeft_U8 ;
	uint8 fl_SOD_CTA_warningReqRight_U8;
	uint8 fl_SOD_LCA_warningReqLeft_U8;
	uint8 fl_SOD_LCA_warningReqRight_U8;
	uint8 fl_SOD_DOA_warningReqRightFront_U8;
	uint8 fl_SOD_DOA_warningReqRightRear_U8;
	uint8 fl_SOD_DOA_warningReqLeftFront_U8;
	uint8 fl_SOD_DOA_warningReqLeftRear_U8;

	uint8 fl_retSOD_BLIS_display_U8 = 0;
	fl_retSOD_BLIS_display_U8 = Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_BLIS_display(&fl_SOD_BLIS_display_U8);

	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_CTA_OFFTelltale(&fl_CTA_OFFTelltale_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_DOA_OFFTelltale(&fl_DOA_OFFTelltale_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SODLCA_OFFTelltale(&fl_SODLCA_OFFTelltale_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqLeft(&fl_SOD_CTA_warningReqLeft_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqRight(&fl_SOD_CTA_warningReqRight_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqLeft(&fl_SOD_LCA_warningReqLeft_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqRight(&fl_SOD_LCA_warningReqRight_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightFront(&fl_SOD_DOA_warningReqRightFront_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightRear(&fl_SOD_DOA_warningReqRightRear_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftFront(&fl_SOD_DOA_warningReqLeftFront_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftRear(&fl_SOD_DOA_warningReqLeftRear_U8);

	if((fl_retSOD_BLIS_display_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_IPM_0x275_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retSOD_BLIS_display_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_IPM_0x275_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_IPM_0x275_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_IPM_0x275_ComIn_Tout(0);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_BLIS_display(fl_SOD_BLIS_display_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_CTA_OFFTelltale(fl_CTA_OFFTelltale_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_DOA_OFFTelltale(fl_DOA_OFFTelltale_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SODLCA_OFFTelltale(fl_SODLCA_OFFTelltale_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqLeft(fl_SOD_CTA_warningReqLeft_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqRight(fl_SOD_CTA_warningReqRight_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqLeft(fl_SOD_LCA_warningReqLeft_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqRight(fl_SOD_LCA_warningReqRight_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightFront(fl_SOD_DOA_warningReqRightFront_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightRear(fl_SOD_DOA_warningReqRightRear_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftFront(fl_SOD_DOA_warningReqLeftFront_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftRear(fl_SOD_DOA_warningReqLeftRear_U8);
	}


	uint16 fl_MRR_F_Obj_dx = 0;
	uint16 fl_MRR_F_Obj_dy = 0;
	uint8 fl_MRR_F_Obj_Class = 0;

	uint8 fl_retfl_MRR_F_Obj_Class = 0;

	Rte_Read_rpSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Object_dx(&fl_MRR_F_Obj_dx);
	Rte_Read_rpSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Object_dy(&fl_MRR_F_Obj_dy);
	fl_retfl_MRR_F_Obj_Class = Rte_Read_rpSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Obj_0x_class(&fl_MRR_F_Obj_Class);

	if((fl_retfl_MRR_F_Obj_Class &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retfl_MRR_F_Obj_Class &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_Tout(0);
		Rte_Write_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Object_dx(fl_MRR_F_Obj_dx);
		Rte_Write_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Object_dy(fl_MRR_F_Obj_dy);
		Rte_Write_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Obj_0x_class(fl_MRR_F_Obj_Class);
	}


	//add by cwu12 190319
	//MP5_0x384
	uint8 fl_IVI_NavICON_U8 = 0;
	uint32 fl_IVI_ROUTE_REMAIN_DIS_U32 = 0;
	uint8 fl_IVI_ROUNG_ABOUT_NUM_U8 = 0;
	uint8 fl_MP5_NavIcon_u8 = 0;
	uint8 fl_MP5_ROUNG_ABOUT_NUM_U8 = 0;
	uint32 fl_ROUTE_REMAIN_DIS_U32 = 0;

	uint8 fl_retIVI_NavICON_U8 = 0;
	fl_retIVI_NavICON_U8 = Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_NavICON(&fl_MP5_NavIcon_u8);
	
	Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUNG_ABOUT_NUM(&fl_MP5_ROUNG_ABOUT_NUM_U8);
	Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUTE_REMAIN_DIS(&fl_ROUTE_REMAIN_DIS_U32);
	
	if((fl_retIVI_NavICON_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_MP5_0x384_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retIVI_NavICON_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_MP5_0x384_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_MP5_0x384_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_MP5_0x384_ComIn_Tout(0);
		
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_MP5_NavICON(fl_MP5_NavIcon_u8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUNG_ABOUT_NUM(fl_MP5_ROUNG_ABOUT_NUM_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUTE_REMAIN_DIS(fl_ROUTE_REMAIN_DIS_U32);
	}

	//add by cwu12 181130
	uint8 fl_Nav_Sts_U8;

	uint8 fl_retNav_Sts_U8 = 0;
	fl_retNav_Sts_U8 = Rte_Read_rpSR_CANMSG_GW_MP5_NAV_0x533_ComIn_Nav_Sts(&fl_Nav_Sts_U8);

	if ((fl_retNav_Sts_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_MP5_NAV_0x533_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if ((fl_retNav_Sts_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_MP5_NAV_0x533_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_MP5_NAV_0x533_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_MP5_NAV_0x533_ComIn_Tout(0);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MP5_NAV_0x533_ComIn_Nav_Sts(fl_Nav_Sts_U8);
	}

	//MP5_BSDSoundSwitchSts moved from 0x530 to 0x382;
	uint8 fl_MP5_BSDSwitchSts_U8;
	uint8 fl_MP5_NavStatus_U8;

	uint8 fl_retMP5_BSDSwitchSts_U8 = 0;
	fl_retMP5_BSDSwitchSts_U8 = Rte_Read_rpSR_CANMSG_GW_MP5_0x382_ComIn_MP5_BSDSoundSwitchSts(&fl_MP5_BSDSwitchSts_U8);
	Rte_Read_rpSR_CANMSG_GW_MP5_0x382_ComIn_MP5_NavStatus(&fl_MP5_NavStatus_U8);

	if ((fl_retMP5_BSDSwitchSts_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_MP5_0x382_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if ((fl_retMP5_BSDSwitchSts_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_MP5_0x382_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_MP5_0x382_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_MP5_0x382_ComIn_Tout(0);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x382_ComIn_MP5_BSDSoundSwitchSts(fl_MP5_BSDSwitchSts_U8);
		Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x382_ComIn_MP5_NavStatus(fl_MP5_NavStatus_U8);
	}

	
	//sli34
	uint32 fl_dte_value;
	Rte_Read_rpSR_CANMSG_IC_0x525_ComOut_IC_DTE(&fl_dte_value);
	Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_DTE(fl_dte_value);

	uint32 fl_afc_value;
	Rte_Read_rpSR_CANMSG_IC_0x525_ComOut_IC_AFC(&fl_afc_value);
	Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_AFC(fl_afc_value);
	
	// //Yxiong2
	// uint8 fl_EMS_StartStopMessage_U8;
	// uint8 fl_retEMS_StartStopMessage_U8 = 0;
	// fl_retEMS_StartStopMessage_U8 = Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x60_ComIn_EMS_StartStopMessage(&fl_EMS_StartStopMessage_U8);
	// if ((fl_retEMS_StartStopMessage_U8 &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	// {   
	//     Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x60_ComIn_NR(RTE_E_NEVER_RECEIVED);
	// }
	// else if ((fl_retEMS_StartStopMessage_U8 &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	// {
	//     Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x60_ComIn_Tout(RTE_E_TIMEOUT);
	// }
	// else
	// {
	// 	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x60_ComIn_NR(0);
	// 	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x60_ComIn_Tout(0);
	// 	Rte_Write_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x60_ComIn_EMS_StartStopMessage(fl_EMS_StartStopMessage_U8);
	// } 
	
	//Yxiong2
	uint8 fl_EMS_StartStopMessage_U8 = 0;
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_StartStopMessage(&fl_EMS_StartStopMessage_U8);	
	Rte_Write_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_StartStopMessage(fl_EMS_StartStopMessage_U8);
	
	//sli34
	uint16 fl_clock_year;
	Rte_Read_rpSR_CANMSG_IC_time_0x581_ComOut_IC_Year(&fl_clock_year);
	Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Year(fl_clock_year);
	
	uint8 fl_clock_month;
	Rte_Read_rpSR_CANMSG_IC_time_0x581_ComOut_IC_Month(&fl_clock_month);
	Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Month(fl_clock_month);
	
	uint8 fl_clock_date;
	Rte_Read_rpSR_CANMSG_IC_time_0x581_ComOut_IC_Date(&fl_clock_date);
	Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Date(fl_clock_date);
	
	uint8 fl_clock_hour;
	Rte_Read_rpSR_CANMSG_IC_time_0x581_ComOut_IC_Hour(&fl_clock_hour);
	Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Hour(fl_clock_hour);
	
	uint8 fl_clock_minute;
	Rte_Read_rpSR_CANMSG_IC_time_0x581_ComOut_IC_Minute(&fl_clock_minute);
	Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Minute(fl_clock_minute);
	
	uint8 fl_clock_second;
	Rte_Read_rpSR_CANMSG_IC_time_0x581_ComOut_IC_Second(&fl_clock_second);
	Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Second(fl_clock_second);

	
	//sli34
	uint8 fl_bgLumplvl_value=0;
	uint8 fl_retbgLumplvl_value=0;

	fl_retbgLumplvl_value = Rte_Read_rpSR_CANMSG_BCM_Sts_0x315_ComIn_BCM_BGLLumLvlSettingStatus(&fl_bgLumplvl_value);
	
	if((fl_retbgLumplvl_value &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_BCM_Sts_0x315_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retbgLumplvl_value &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_BCM_Sts_0x315_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_BCM_Sts_0x315_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_BCM_Sts_0x315_ComIn_Tout(0); 
		Rte_Write_ppSR_CANMSG_BCM_Sts_0x315_ComIn_BCM_BGLLumLvlSettingStatus(fl_bgLumplvl_value);
	}

	//0x584
	uint8 fl_LoadManageLevel_value=0;
	uint8 fl_retManageLevel_value=0;
	

	fl_retManageLevel_value = Rte_Read_rpSR_CANMSG_BCM_0x584_ComIn_BCM_LoadManageLevel(&fl_LoadManageLevel_value);
	
	if((fl_retManageLevel_value &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_BCM_0x584_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retManageLevel_value &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_BCM_0x584_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_BCM_0x584_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_BCM_0x584_ComIn_Tout(0); 
		Rte_Write_ppSR_CANMSG_BCM_0x584_ComIn_BCM_LoadManageLevel(fl_LoadManageLevel_value);
	}

    /* cpang1 */
    uint8 fl_BCM_CarModeIndicator;
    Rte_Read_rpSR_CANMSG_BCM_0x584_ComIn_BCM_CarModeIndicator(&fl_BCM_CarModeIndicator);
    Rte_Write_ppSR_CANMSG_BCM_0x584_ComIn_BCM_CarModeIndicator(fl_BCM_CarModeIndicator);
	
	//pwu5
	uint8 fl_MP5_AlarmOrNot_value=0;
	uint8 fl_MP5_AlarmStatus_value=0;
	uint8 fl_retMP5_AlarmOrNot_value=0;

	fl_retMP5_AlarmOrNot_value = Rte_Read_rpSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmOrNot(&fl_MP5_AlarmOrNot_value);
	fl_retMP5_AlarmOrNot_value = Rte_Read_rpSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmStatus(&fl_MP5_AlarmStatus_value);
	
	if((fl_retMP5_AlarmOrNot_value &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{   
	    Rte_Write_ppSR_CANMSG_GW_MP5_0x530_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_retMP5_AlarmOrNot_value &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
	    Rte_Write_ppSR_CANMSG_GW_MP5_0x530_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
	    Rte_Write_ppSR_CANMSG_GW_MP5_0x530_ComIn_NR(0);
	    Rte_Write_ppSR_CANMSG_GW_MP5_0x530_ComIn_Tout(0); 
		Rte_Write_ppSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmStatus(fl_MP5_AlarmStatus_value);
		Rte_Write_ppSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmOrNot(fl_MP5_AlarmOrNot_value);
	}	
	
	uint32 fl_EMS_BackupOdometer = 0;
	uint8 fl_EMS_BackupOdometer_ret = 0;
	fl_EMS_BackupOdometer_ret = Rte_Read_rpSR_CANMSG_GW_EMS_0x636_ComIn_EMS_BackupOdometer(&fl_EMS_BackupOdometer);
	if((fl_EMS_BackupOdometer_ret &  RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
	{	
		Rte_Write_ppSR_CANMSG_GW_EMS_0x636_ComIn_NR(RTE_E_NEVER_RECEIVED);
	}
	else if((fl_EMS_BackupOdometer_ret &  RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
	{
		Rte_Write_ppSR_CANMSG_GW_EMS_0x636_ComIn_Tout(RTE_E_TIMEOUT);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_GW_EMS_0x636_ComIn_EMS_BackupOdometer(fl_EMS_BackupOdometer);
		Rte_Write_ppSR_CANMSG_GW_EMS_0x636_ComIn_NR(0);
		Rte_Write_ppSR_CANMSG_GW_EMS_0x636_ComIn_Tout(0);
	}

	ComIn_Msg_DTC_Process(); 
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
/// @brief  Transitional state to clean up after diagnostic state. Component behavior to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}

extern uint32 Rte_ComVnimMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue;
static void Rte_Init_Memory( void )
{
	Rte_ComVnimMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue = 0x00FFFFFFU; 
}


static boolean ComIn_DTC_StoreIn_Condition(void)
{
	boolean fl_storeIn = FALSE;

	if((dem_IGN_ON_5s == TRUE) /*&& (dem_dtc_state == TRUE) */&& (dem_all_block_read_finish_bool == TRUE) && (dem_Can_Busoff_bool == FALSE))
	{
		fl_storeIn = TRUE;
	}
	return fl_storeIn;
}


static void ComIn_Msg_DTC_Process(void)
{
	if(ComIn_DTC_StoreIn_Condition())
	{
		/*============================================================================
		** Lost Communication With ESP. ID=243
		**==========================================================================*/
		/* GW_ESP_Status_Chassis_0x243 */
		uint8 fl_ret_ESP_AvhActive = 0;
		uint8 fl_ESP_AvhActive = 0;
		fl_ret_ESP_AvhActive = Rte_Read_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhActive(&fl_ESP_AvhActive);
		
		if ((fl_ret_ESP_AvhActive & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
		{	
			Dem_SetEventStatus(CAN_LOST_DTC_ESP, RESULT_FAILED);
		}
		else if ((fl_ret_ESP_AvhActive & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
		{
			Dem_SetEventStatus(CAN_LOST_DTC_ESP, RESULT_FAILED);
		}
		else 
		{
			Dem_SetEventStatus(CAN_LOST_DTC_ESP, RESULT_NORMAL);
		}

		/*============================================================================
		** Lost Communication With TCU. ID=0x395 or 0x123
		**==========================================================================*/
		/* GW_TCU_Info_0x395 */
		uint8 fl_ret_TCU_GearboxP_Faulty_u8 = 0;
		uint8 fl_TCU_GearboxP_Faulty_u8 = 0;

		/* GW_TCU_GearInfo_Power_0x123 */
		uint8 fl_ret_TCU_DisplayGear_u8 = 0;
		uint8 fl_TCU_DisplayGear_u8 = 0;

		//boolean fl_vehicle_cfg_at_mt_flg = FALSE;
		//Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AT_MT, &fl_vehicle_cfg_at_mt_flg);

		//if (TRUE == fl_vehicle_cfg_at_mt_flg) //0 - MT,  1 - AT
		{
			fl_ret_TCU_GearboxP_Faulty_u8 = Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_GearboxP_Faulty(&fl_TCU_GearboxP_Faulty_u8);
			fl_ret_TCU_DisplayGear_u8 = Rte_Read_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear(&fl_TCU_DisplayGear_u8);

			if (((fl_ret_TCU_GearboxP_Faulty_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED) ||
				((fl_ret_TCU_DisplayGear_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED))
			{
				Dem_SetEventStatus(CAN_LOST_DTC_TCU, RESULT_FAILED);
			}
			else if (((fl_ret_TCU_GearboxP_Faulty_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED) ||
					((fl_ret_TCU_DisplayGear_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED))
			{
				Dem_SetEventStatus(CAN_LOST_DTC_TCU, RESULT_FAILED);
			}
			else 
			{
				Dem_SetEventStatus(CAN_LOST_DTC_TCU, RESULT_NORMAL);
			}
		}
		
		/*============================================================================
		** Lost Communication with BCM. ID=0x313,0x310,0x318,//0x584,0x315
		**==========================================================================*/		
		/* BCM_0x313 */
		uint8 fl_ret_BCM_Mechanical_Hand_Brake_u8 = 0;
		uint8 fl_BCM_Mechanical_Hand_Brake_u8 = 0;
		fl_ret_BCM_Mechanical_Hand_Brake_u8 = Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_BrakeFluidSts(&fl_BCM_Mechanical_Hand_Brake_u8);

		/* BCM_Door_0x310 */
		uint8 fl_ret_BCM_DoorStsRearRi_u8 = 0;
		uint8 fl_BCM_DoorStsRearRi_u8 = 0;
		fl_ret_BCM_DoorStsRearRi_u8 = Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearRi(&fl_BCM_DoorStsRearRi_u8);
		
		/* BCM_Lamp_0x318 */
		uint8 fl_ret_BCM_FrntFogLmpSts_u8 = 0;
		uint8 fl_BCM_FrntFogLmpSts_u8 = 0;
		fl_ret_BCM_FrntFogLmpSts_u8 = Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpSts(&fl_BCM_FrntFogLmpSts_u8);

#if 0
		/* BCM_0X315*/
		uint8 fl_bgLumplvl_value=0;
		uint8 fl_retbgLumplvl_value=0;
		fl_retbgLumplvl_value = Rte_Read_rpSR_CANMSG_BCM_Sts_0x315_ComIn_BCM_BGLLumLvlSettingStatus(&fl_bgLumplvl_value);

		/* BCM_0x584 */
		uint8 fl_ret_BCM_LoadManageLevel_u8 = 0;
		uint8 fl_BCM_LoadManageLevel_u8 = 0;
		fl_ret_BCM_LoadManageLevel_u8 = Rte_Read_rpSR_CANMSG_BCM_0x584_ComIn_BCM_LoadManageLevel(&fl_BCM_LoadManageLevel_u8);
#endif

		if (((fl_ret_BCM_Mechanical_Hand_Brake_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED) ||
				((fl_ret_BCM_DoorStsRearRi_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED) ||
				((fl_ret_BCM_FrntFogLmpSts_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)) /*||
				((fl_retbgLumplvl_value & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED) || 
				((fl_ret_BCM_LoadManageLevel_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED))*/
		{	
			Dem_SetEventStatus(CAN_LOST_DTC_BCM, RESULT_FAILED);
		}
		else if (((fl_ret_BCM_Mechanical_Hand_Brake_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED) ||
				 ((fl_ret_BCM_DoorStsRearRi_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED) ||
				 ((fl_ret_BCM_FrntFogLmpSts_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED) )/*||
				 ((fl_retbgLumplvl_value & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED) || 
				 ((fl_ret_BCM_LoadManageLevel_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED))*/
		{
			Dem_SetEventStatus(CAN_LOST_DTC_BCM, RESULT_FAILED);
		}
		else 
		{
			Dem_SetEventStatus(CAN_LOST_DTC_BCM, RESULT_NORMAL);
		}


		/*============================================================================
		** Lost Communication With EMS. ID=0x10B or 0x366 or 0x151 or 0x368 //or 0x636
		**==========================================================================*/
		/* GW_EMS_Power_0x10B */
		uint8 fl_ret_EMS_EngSts_u8 = 0;
		uint8 fl_EMS_EngSts_u8 = 0;
		fl_ret_EMS_EngSts_u8 = Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSts(&fl_EMS_EngSts_u8);

		/* GW_EMS_Power_0x366 */
		uint16 fl_ret_EMS_Fuelcounter_u8 = 0;
		uint16 fl_EMS_Fuelcounter_u8 = 0;
		fl_ret_EMS_Fuelcounter_u8 = Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_Fuelcounter(&fl_EMS_Fuelcounter_u8);
		
		/* GW_EMS_Sts_Power_0x151 */
		uint8 fl_ret_EMS_CruiseControlStatus_u8 = 0;
		uint8 fl_EMS_CruiseControlStatus_u8 = 0;
		fl_ret_EMS_CruiseControlStatus_u8 = Rte_Read_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus(&fl_EMS_CruiseControlStatus_u8);

		/* GW_EMS_0x368 */
		uint16 fl_ret_EMS_UreaDTE_u8 = 0;
		uint16 fl_EMS_UreaDTE_u8 = 0;
		fl_ret_EMS_UreaDTE_u8 = Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaDTE(&fl_EMS_UreaDTE_u8);

#if 0
		/* GW_EMS_0x636 */
		uint8  fl_ret_EMS_BackupOdometer = 0;
		uint32 fl_EMS_BackupOdometer = 0;
		fl_ret_EMS_BackupOdometer = Rte_Read_rpSR_CANMSG_GW_EMS_0x636_ComIn_EMS_BackupOdometer(&fl_EMS_BackupOdometer);
#endif	

		if (((fl_ret_EMS_EngSts_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED) ||
			((fl_ret_EMS_Fuelcounter_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED) ||
			((fl_ret_EMS_CruiseControlStatus_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED) || 
			((fl_ret_EMS_UreaDTE_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED) )/*|| 
			((fl_ret_EMS_BackupOdometer & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED))*/
		{	
			Dem_SetEventStatus(CAN_LOST_DTC_EMS, RESULT_FAILED);
		}
		else if (((fl_ret_EMS_EngSts_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED) ||
				 ((fl_ret_EMS_Fuelcounter_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED) ||
				 ((fl_ret_EMS_CruiseControlStatus_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED) ||
				 ((fl_ret_EMS_UreaDTE_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED))/* ||
				 ((fl_ret_EMS_BackupOdometer & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED))*/
		{
			Dem_SetEventStatus(CAN_LOST_DTC_EMS, RESULT_FAILED);
		}
		else 
		{
			Dem_SetEventStatus(CAN_LOST_DTC_EMS, RESULT_NORMAL);
		}

		/*============================================================================
		** Lost Communication With SRS. ID=0x350
		**==========================================================================*/
		uint8 fl_ret_SRS_SBR_Passenger_u8 = 0;
		uint8 fl_SRS_SBR_Passenger_u8 = 0;

		fl_ret_SRS_SBR_Passenger_u8 = Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger(&fl_SRS_SBR_Passenger_u8);

		if ((fl_ret_SRS_SBR_Passenger_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
		{	
			Dem_SetEventStatus(CAN_LOST_DTC_SRS, RESULT_FAILED);
		}
		else if ((fl_ret_SRS_SBR_Passenger_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
		{
			Dem_SetEventStatus(CAN_LOST_DTC_SRS, RESULT_FAILED);
		}
		else 
		{
			Dem_SetEventStatus(CAN_LOST_DTC_SRS, RESULT_NORMAL);
		}
	
		/*============================================================================
		** Lost Communication With TPMS. ID=0x361
		**==========================================================================*/
		uint8 fl_ret_TPMS_LF_Pressure_u8 = 0;
		uint8 fl_TPMS_LF_Pressure_u8 = 0;

		fl_ret_TPMS_LF_Pressure_u8 = Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure(&fl_TPMS_LF_Pressure_u8);

		if ((fl_ret_TPMS_LF_Pressure_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
		{	
			Dem_SetEventStatus(CAN_LOST_DTC_TPMS, RESULT_FAILED);
		}
		else if ((fl_ret_TPMS_LF_Pressure_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
		{
			Dem_SetEventStatus(CAN_LOST_DTC_TPMS, RESULT_FAILED);
		}
		else 
		{
			Dem_SetEventStatus(CAN_LOST_DTC_TPMS, RESULT_NORMAL);
		}

		/*============================================================================
		** Lost Communication With EPS. ID=0x260
		**==========================================================================*/
		uint8 fl_ret_EPS_S_SafeLampRed_u8 = 0;
		uint8 fl_EPS_S_SafeLampRed_u8 = 0;

		fl_ret_EPS_S_SafeLampRed_u8 = Rte_Read_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_SafeLampRed(&fl_EPS_S_SafeLampRed_u8);

		if ((fl_ret_EPS_S_SafeLampRed_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
		{	
			Dem_SetEventStatus(CAN_LOST_DTC_EPS, RESULT_FAILED);
		}
		else if ((fl_ret_EPS_S_SafeLampRed_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
		{
			Dem_SetEventStatus(CAN_LOST_DTC_EPS, RESULT_FAILED);
		}
		else 
		{
			Dem_SetEventStatus(CAN_LOST_DTC_EPS, RESULT_NORMAL);
		}
		
		/*============================================================================
		** Lost Communication With T-Box. ID=0x582
		**==========================================================================*/
		uint8 fl_ret_TBOX_Year_u8 = 0;
		uint8 fl_TBOX_Year_u8 = 0;

		fl_ret_TBOX_Year_u8 = Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Year(&fl_TBOX_Year_u8);

		if ((fl_ret_TBOX_Year_u8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
		{	
			Dem_SetEventStatus(CAN_LOST_DTC_T_BOX, RESULT_FAILED);
		}
		else if ((fl_ret_TBOX_Year_u8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
		{
			Dem_SetEventStatus(CAN_LOST_DTC_T_BOX, RESULT_FAILED);
		}
		else 
		{
			Dem_SetEventStatus(CAN_LOST_DTC_T_BOX, RESULT_NORMAL);
		}
		
#if 0
		/*============================================================================
		** Lost Communication With MP5. ID=0x530 or 0x384 or 0x385
		**==========================================================================*/
		/*MP5_0X530 */
		uint8 fl_ret_MP5_AlarmOrNot = 0;
		uint8 fl_MP5_AlarmOrNot = 0;

		fl_ret_MP5_AlarmOrNot = Rte_Read_rpSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmOrNot(&fl_MP5_AlarmOrNot);

		/*MP5_0x384*/


		/*GW_MP5_Nav_0x385*/
		

		if ((fl_ret_MP5_AlarmOrNot & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
		{	
			Dem_SetEventStatus(CAN_LOST_DTC_MP5, RESULT_FAILED);
		}
		else if ((fl_ret_MP5_AlarmOrNot & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
		{
			Dem_SetEventStatus(CAN_LOST_DTC_MP5, RESULT_FAILED);
		}
		else 
		{
			Dem_SetEventStatus(CAN_LOST_DTC_MP5, RESULT_NORMAL);
		}

		/*============================================================================
		** Lost Communication With IPM. ID=0x245
		**==========================================================================*/		
		uint8 fl_retLaneAssit_Status_U8 = 0;		
		uint8 fl_LaneAssit_Status_U8=0;
		fl_retLaneAssit_Status_U8 = Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status(&fl_LaneAssit_Status_U8);

		if ((fl_retLaneAssit_Status_U8 & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
		{	
			Dem_SetEventStatus(CAN_LOST_DTC_IPM, RESULT_FAILED);
		}
		else if ((fl_retLaneAssit_Status_U8 & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
		{
			Dem_SetEventStatus(CAN_LOST_DTC_IPM, RESULT_FAILED);
		}
		else 
		{
			Dem_SetEventStatus(CAN_LOST_DTC_IPM, RESULT_NORMAL);
		}

		/*============================================================================
		** Lost Communication With SWM. ID=0x31A
		**==========================================================================*/		
		uint8 menu_up=0;
		uint8 ret_menu_up=0;
		//menu up
		ret_menu_up = Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Up(&menu_up);

		if ((ret_menu_up & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
		{	
			Dem_SetEventStatus(CAN_LOST_DTC_SWM, RESULT_FAILED);
		}
		else if ((ret_menu_up & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
		{
			Dem_SetEventStatus(CAN_LOST_DTC_SWM, RESULT_FAILED);
		}
		else 
		{
			Dem_SetEventStatus(CAN_LOST_DTC_SWM, RESULT_NORMAL);
		}		

		/*============================================================================
		** Lost Communication With PAM. ID=0x271
		**==========================================================================*/	
		uint8 fl_ret_PAM_FrontSysTemWorkSts=0;
		uint8 fl_PAM_FrontSysTemWorkSts=0;
		
		fl_ret_PAM_FrontSysTemWorkSts = Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_FrontSysTemWorkSts(&fl_PAM_FrontSysTemWorkSts);

		if ((fl_ret_PAM_FrontSysTemWorkSts & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED)
		{	
			Dem_SetEventStatus(CAN_LOST_DTC_PAM, RESULT_FAILED);
		}
		else if ((fl_ret_PAM_FrontSysTemWorkSts & RTE_E_MAX_AGE_EXCEEDED) == RTE_E_MAX_AGE_EXCEEDED)
		{
			Dem_SetEventStatus(CAN_LOST_DTC_PAM, RESULT_FAILED);
		}
		else 
		{
			Dem_SetEventStatus(CAN_LOST_DTC_PAM, RESULT_NORMAL);
		}		
		#endif
	}
	else 
	{
		Dem_SetEventStatus(CAN_LIMP_HOME_DTC, RESULT_NORMAL);
		
		Dem_SetEventStatus(CAN_LOST_DTC_ESP, RESULT_NORMAL);
		
		Dem_SetEventStatus(CAN_LOST_DTC_EMS, RESULT_NORMAL);

		Dem_SetEventStatus(CAN_LOST_DTC_SRS, RESULT_NORMAL);

		Dem_SetEventStatus(CAN_LOST_DTC_TCU, RESULT_NORMAL);

		Dem_SetEventStatus(CAN_LOST_DTC_BCM, RESULT_NORMAL);

		Dem_SetEventStatus(CAN_LOST_DTC_TPMS, RESULT_NORMAL);
	
		Dem_SetEventStatus(CAN_LOST_DTC_EPS, RESULT_NORMAL);
		
		Dem_SetEventStatus(CAN_LOST_DTC_T_BOX, RESULT_NORMAL);
		
		Dem_SetEventStatus(CAN_LOST_DTC_FUEL_SENSOR, RESULT_NORMAL);

		Dem_SetEventStatus(CAN_LOST_DTC_BELOW_8_5V, RESULT_NORMAL);
		
		Dem_SetEventStatus(CAN_LOST_DTC_ABOVE_16V, RESULT_NORMAL);
		
		Dem_SetEventStatus(CAN_BUS_OFF_DTC, RESULT_NORMAL);
		

		//Dem_SetEventStatus(CAN_LOST_DTC_MP5, RESULT_NORMAL);

		//Dem_SetEventStatus(CAN_LOST_DTC_IPM, RESULT_NORMAL);

		//Dem_SetEventStatus(CAN_LOST_DTC_SWM, RESULT_NORMAL);
		
		//Dem_SetEventStatus(CAN_LOST_DTC_PAM, RESULT_NORMAL);
	}
}
