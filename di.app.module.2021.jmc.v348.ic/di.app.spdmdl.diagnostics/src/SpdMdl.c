
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
#include "Rte_SpdMdl.h"
#include "CmpLib.h"
//#include "MathLib.h"
//#include "string.h"
#include "SpdMdl.h"
#include "hmi_gauge_filter.h"
#include "hmi_gauge_speed.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(SpdMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define VEHICLE_SPEED_CORRECT_RESOLUTION   (1)
#define VEHICLE_SPEED_CORRECT_OFFSET	   (0)
#define VEHICLE_SPEED_DISPLAY_DIVISION     (100)

#define VEHICLE_SPEED_INVALID			(0xFFF)
#define VEHICLE_SPEED_HMI_INVALID		(uint16)(0x1FF)

#define VEHICLE_SPEED_FILTER_COUNTER  	(2)

#define SPEED_DECODE_FACTOR  90
#define SPEED_DECODE_SHIFT  4            //7/10  GS


#define VEHICLE_RAW_SPEED_MIN			(0)
#define VEHICLE_RAW_SPEED_MAX			(20000)            //22000->23028->20000  2019/4/3 MaxSpeed is 200km/h

#define VEHICLE_SPEED_DISPLAY_COEFFICIENT  (167)
#define VEHICLE_SPEED_DISPLAY_RESOLUTION   (103)
#define VEHICLE_SPEED_DISPLAY_OFFSET	   (20000)

#define VEHICLE_SPEED_DISPLAY_THRESHOLD	   (300)

#define SPEED_OFFSET					(10000)
#define SPEED_DISPLAY_THRESHOLD			(3*SPEED_OFFSET)

#define TIMEOUT_350MS				(350)
#define TIMEOUT_1000MS				(2000)
#define VEHICLE_SPEED_MAX			(30000)

#define SpdSampledNumMax			(uint8)10
static	uint16	u16DATCVT_CANSignal_Speed[SpdSampledNumMax];
static	uint8	u8DATCVT_CANSignal_Speed_Id;	/*index of u16DATCVT_CANSignal_Speed*/

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static void f_spdMdl_initial(void);
static void f_sample_speed(void);
static void f_spdMdl_invalidProcess(void);
static void f_spdMdl_Process(void);
static void SpdMdlPowerONDelay(void);


//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint32 l_filter_vehicle_spd_U32 = 0;
static uint32 l_pre_filter_vehicle_spd_U32 = 0;
static uint32 l_display_vehicle_spd_U32 = 0;
static uint32 l_display_vehicleDial_spd_U32 = 0;
static uint8  l_filter_vehicle_spd_counter_U8 = 0;
static uint16 l_VehSpeed_U16 = 0;
static boolean  l_spd_canSt_Flag = 0;
static boolean  l_vehspd_invalid_Flag = 0;
static boolean SpdMdl_PowerON_Delay = FALSE;


//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit ( void )
{
	f_spdMdl_initial();
	l_VehSpeed_U16 = 0;
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
	f_spdMdl_initial();
	l_VehSpeed_U16 = 0;
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
	f_spdMdl_initial();
	l_VehSpeed_U16 = 0;
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
	f_spdMdl_initial();
	l_VehSpeed_U16 = 0;
	Rte_Write_tiSR_TISpdMdl_Send_value(l_display_vehicle_spd_U32);
	Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_VehSpd_HMI(l_display_vehicle_spd_U32/100);
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	eIgnState fl_IGN_state = 0;
	EBatteryState fl_Battery_state = 0;
   // uint8 l_flag = 0;
	uint8 l_OTAMode = 0;
	
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	Rte_Read_rp_GetOTAModeStatus_Value_OTAMode(&l_OTAMode);
    if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage || l_OTAMode == TRUE)
	{
		f_spdMdl_initial();
		l_vehspd_invalid_Flag = FALSE;
	}
	else
	{
    	switch(fl_IGN_state)
    		{
    			case eIGN_OFF:					
    				f_spdMdl_initial();	
                    SpdMdl_PowerON_Delay = FALSE;
    				l_vehspd_invalid_Flag = FALSE;
    				break;
    			case eIGN_RUN:
                    if(TRUE == SpdMdl_PowerON_Delay)
                    { 
                        f_spdMdl_Process();
                    }
    				break;
    			default:
    				l_vehspd_invalid_Flag = FALSE;
    				break;
    		}
	}

	f_sample_speed();
	//move the filter to hmi_gauge_speed.c
	l_VehSpeed_U16 = DATCvtFirstOrderFilterPro(l_VehSpeed_U16,l_display_vehicleDial_spd_U32,SPEED_FILTERCOEF);
	
	SpdMdlPowerONDelay();
	//l_VehSpeed_U16 = (uint32)l_display_vehicleDial_spd_U32;
	Rte_Write_ppSR_TISpdMdl_Real_Value(l_filter_vehicle_spd_U32);
	Rte_Write_ppSR_TISpdMdl_Send_DigitalValue_Value(l_display_vehicle_spd_U32);     //Spddigital
	Rte_Write_tiSR_TISpdMdl_Send_value((uint32)l_VehSpeed_U16);
	if (FALSE == l_vehspd_invalid_Flag)
	{
		Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_VehSpd_HMI(l_display_vehicle_spd_U32 / 100);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_VehSpd_HMI(VEHICLE_SPEED_HMI_INVALID);
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



///=====================================================================================================================
//  PUBLIC
//=====================================================================================================================
static void f_spdMdl_invalidProcess(void)
{	
	boolean	fl_Timer_vehSpdInvalid_RunningSt = 0;
	boolean	fl_Timer_vehSpdInvalid_ElapsedSt = 0;
	Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerHandleVehSpdInvalidWait,&fl_Timer_vehSpdInvalid_RunningSt);
	if(fl_Timer_vehSpdInvalid_RunningSt == FALSE)
	{
		Rte_Call_rpTimer_TmExt_Timer_Start(eTimerHandleVehSpdInvalidWait);
		l_display_vehicle_spd_U32 = l_filter_vehicle_spd_U32 / VEHICLE_SPEED_DISPLAY_DIVISION;
	}
	
	Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerHandleVehSpdInvalidWait,TIMEOUT_1000MS,&fl_Timer_vehSpdInvalid_ElapsedSt);
	//SysRS_02_Gauge-887
	if(fl_Timer_vehSpdInvalid_ElapsedSt == TRUE)
	{
		//SysRS_02_Gauge-888
		f_spdMdl_initial();
		l_vehspd_invalid_Flag = TRUE;
	}
	//clear Sample Timer, to ensure retrieve immediately
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleVehSpdSampleWait);
}
 

static void f_spdMdl_Process(void)
{
	uint32 	fl_raw_vehicleSpd_value_U32 = 0;
	uint16	fl_VehicleFilterSpd_U16 = 0;
	uint8	fl_esc2_nvrd_st_U8 = 0;
	uint8 	fl_esc2_missing_st_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_esc2_nvrd_st_U8);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_esc2_missing_st_U8);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_VehicleFilterSpd_U16);


	if(fl_esc2_missing_st_U8 == RTE_E_TIMEOUT)
	{
		//SysRS_02_Gauge-1176
    	f_spdMdl_initial();
		l_vehspd_invalid_Flag = TRUE;
	}
	else if(fl_esc2_nvrd_st_U8 == RTE_E_NEVER_RECEIVED)
	{
    	f_spdMdl_initial();
		l_vehspd_invalid_Flag = TRUE;
	}
	else if(fl_VehicleFilterSpd_U16 == VEHICLE_SPEED_INVALID)
    {
   		f_spdMdl_invalidProcess();
    }
	else
	{
		l_vehspd_invalid_Flag = FALSE;
		
		if(l_spd_canSt_Flag == FALSE)
		{
			Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleVehSpdSampleWait);
			l_spd_canSt_Flag = TRUE;
		}
		//clear Invallid Timer
		Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleVehSpdInvalidWait);	
        
        fl_raw_vehicleSpd_value_U32 = DATCvtAvgFilterPro(u16DATCVT_CANSignal_Speed,&u8DATCVT_CANSignal_Speed_Id,fl_VehicleFilterSpd_U16);//CAN raw value process
		/* In order to prevent lose accuracy, we zoom in by 100 here*/
		fl_raw_vehicleSpd_value_U32 = (((uint32)fl_raw_vehicleSpd_value_U32)* SPEED_DECODE_FACTOR) >> SPEED_DECODE_SHIFT;
		//fl_raw_vehicleSpd_value_U32 = (uint32)fl_VehicleFilterSpd_U16;
		
		//SysRS_02_Gauge-843
		if(fl_raw_vehicleSpd_value_U32 > VEHICLE_RAW_SPEED_MAX)
		{
			fl_raw_vehicleSpd_value_U32 = VEHICLE_RAW_SPEED_MAX;
		}

        //SysRS_02_Gauge-1395
        if(l_filter_vehicle_spd_counter_U8 < VEHICLE_SPEED_FILTER_COUNTER)
        {   
            l_filter_vehicle_spd_U32 = fl_raw_vehicleSpd_value_U32;      //7/10 GS
            l_filter_vehicle_spd_counter_U8 ++;
        }
        else
        {
            l_filter_vehicle_spd_U32 = ( l_pre_filter_vehicle_spd_U32 * 7 + fl_raw_vehicleSpd_value_U32) / 8;
        }
        l_pre_filter_vehicle_spd_U32 = fl_raw_vehicleSpd_value_U32;
    }
}

static void f_spdMdl_initial(void)
{
	l_filter_vehicle_spd_U32 = 0;
	l_pre_filter_vehicle_spd_U32 = 0;
	l_display_vehicle_spd_U32 = 0;
	l_display_vehicleDial_spd_U32 = 0;
	l_filter_vehicle_spd_counter_U8 = 1;
	l_spd_canSt_Flag = FALSE;
    //SpdMdl_PowerON_Delay = FALSE;
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleVehSpdSampleWait);
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleVehSpdInvalidWait);
}

static uint32 f_vehSpeed_convert(void)
{
	uint32 	fl_vehicle_spd_point_U32 = 0;
    uint32   u8_remainder = 0;
    uint16  u8_vehicle_5to = 0;
	/* In order to prevent lose accuracy, we zoom in by 100 here*/
	if(l_filter_vehicle_spd_U32 <= VEHICLE_SPEED_DISPLAY_THRESHOLD )
	{
		fl_vehicle_spd_point_U32 = l_filter_vehicle_spd_U32 * VEHICLE_SPEED_DISPLAY_COEFFICIENT;
        u8_remainder = (l_filter_vehicle_spd_U32 * VEHICLE_SPEED_DISPLAY_COEFFICIENT) % 10000;
        if(u8_remainder >= 5000)
         {
            u8_vehicle_5to = 10000;/*4she5ru*/
         }
        fl_vehicle_spd_point_U32 += u8_vehicle_5to;
	}
	else 
	{
	    u8_remainder = (l_filter_vehicle_spd_U32 * VEHICLE_SPEED_DISPLAY_RESOLUTION) % 10000;
        if(u8_remainder >= 5000)
         {
            u8_vehicle_5to = 10000;/*4she5ru*/
         }
		fl_vehicle_spd_point_U32 = l_filter_vehicle_spd_U32 * VEHICLE_SPEED_DISPLAY_RESOLUTION +u8_vehicle_5to+ VEHICLE_SPEED_DISPLAY_OFFSET; //+ VEHICLE_SPEED_DISPLAY_OFFSET*VEHICLE_SPEED_DISPLAY_DIVISION;
	    if((fl_vehicle_spd_point_U32 / 100) >= VEHICLE_RAW_SPEED_MAX)	
        {
            fl_vehicle_spd_point_U32 = VEHICLE_RAW_SPEED_MAX * 100;
        }   
	}
	
	return fl_vehicle_spd_point_U32;
}

static void f_sample_speed(void)
{
	boolean	fl_Timer_vehSpdSample_RunningSt = 0;
	boolean	fl_Timer_vehSpdSample_ElapsedSt = 0;

	//start period sample
	Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerHandleVehSpdSampleWait,&fl_Timer_vehSpdSample_RunningSt);
	if(fl_Timer_vehSpdSample_RunningSt == FALSE)
	{
		Rte_Call_rpTimer_TmExt_Timer_Start(eTimerHandleVehSpdSampleWait);
		l_display_vehicle_spd_U32 = f_vehSpeed_convert() / VEHICLE_SPEED_DISPLAY_DIVISION;
	}
	
	Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerHandleVehSpdSampleWait,TIMEOUT_350MS,&fl_Timer_vehSpdSample_ElapsedSt);
	
	if(fl_Timer_vehSpdSample_ElapsedSt == TRUE)
	{
		Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleVehSpdSampleWait);
		//l_display_vehicle_spd_U32 = (uint16)(l_filter_vehicle_spd_U32 / VEHICLE_SPEED_DISPLAY_DIVISION);
		l_display_vehicle_spd_U32 = f_vehSpeed_convert() / VEHICLE_SPEED_DISPLAY_DIVISION;
	}	

	l_display_vehicleDial_spd_U32 = f_vehSpeed_convert() / VEHICLE_SPEED_DISPLAY_DIVISION;
}

static void SpdMdlPowerONDelay(void)
{	
	boolean	ul_Timer_vehSpdInvalid_RunningSt = 0;
	boolean	ul_Timer_vehSpdInvalid_ElapsedSt = 0;
    if(SpdMdl_PowerON_Delay == FALSE)
    {
    	Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerSpdTurnonDelay,&ul_Timer_vehSpdInvalid_RunningSt);
    	if(ul_Timer_vehSpdInvalid_RunningSt == FALSE)
    	{
    		Rte_Call_rpTimer_TmExt_Timer_Start(eTimerSpdTurnonDelay);
    	}
    	
    	Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerSpdTurnonDelay,5000,&ul_Timer_vehSpdInvalid_ElapsedSt);
    	if(ul_Timer_vehSpdInvalid_ElapsedSt == TRUE)
    	{
    		Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerSpdTurnonDelay);
            SpdMdl_PowerON_Delay = TRUE;
    	}
     }
}



/*----------------------------------------------------------------------------
Date              : 19-July-2017
CDSID             : kvakeshk
Traceability      : RTC ID 805747 & 875041
Change Description: Speed gauge Intial Version
-----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
Date              : 22-Aug-2017
CDSID             : pselvara
Traceability      : story ID :918430 Task ID :922561
Change Description: Added display_speed calculation and related Rte interfaces
-----------------------------------------------------------------------------*/



