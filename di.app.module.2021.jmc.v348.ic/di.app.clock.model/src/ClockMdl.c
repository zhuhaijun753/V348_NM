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
#include "Rte_ClockMdl.h"
#include "CmpLib.h"
#include "ClockMdl_cfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(ClockMdl)

/*=====================================================================================================================
**  CONSTANTS & TYPES
**====================================================================================================================*/

static void Clock_Initial(void);
static boolean Clock_Check_TBOX_Signal_InValid(void);
static void Clock_Get_TBOX_Time(void);
static void Clock_Get_RTC_Time(void);
static void Clock_Set_RTCTime(void);
static void Clock_Update_Time(void);
static void Clock_Calibrate_RTCTime(void);
//static void Clock_Check_CurrTime(void);


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

#define INVALID_CLOCK_VALUE 				(0xFF)	  	/*invlid value  --:--*/
#define INVALID_CLOCK_HOURS	 				(23)
#define INVALID_CLOCK_MINUTES    			(59)
#define INVALID_CLOCK_SECONDS	 			(59)
#define INVALID_CLOCK_DATE					(30)
#define INVALID_CLOCK_MONTH					(11)
#define INVALID_CLOCK_YEAR					(255)
#define INITIAL_HOUR                        (12)
#define INITIAL_MINUTE                      (0)
#define INITIAN_SECOND						(0)
#define INITIAN_DAY							(0)
#define CLOCK_UPDATE_2S_TIMER        		(67)					//30ms cycle
#define CLOCK_YEAR_OFFSET					(2017)
#define CLOCK_MONTH_OFFSET					(1)
#define CLOCK_DAY_OFFSET					(1)
#define CLOCK_REALTIME_VALID				(1)
#define	CLOCK_REALTIME_INVALID				(0)


//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static Rtc_TimeType l_rtc_currtime;
static boolean l_Tbox_signal_invalid;
static uint16 l_clock_cali_count;
static boolean l_clock_invalid_value;
//static uint16 l_clock_invalid_value_count;
//static boolean l_clock_invalid_value_state;

#pragma ghs section bss=".buram"
static uint16 l_clock_year;
static uint8 l_clock_month;
static uint8 l_clock_day;
static uint8 l_clock_hour;
static uint8 l_clock_minute;
static uint8 l_clock_second;
static uint8 l_clock_pre_month;
static uint16 l_clock_firstBattary_counter;
#pragma ghs section bss=default



//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	Clock_Initial();	

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
static Std_ReturnType CmpActive( void )
{	
	EBatteryState fl_Battery_state;		
	eIgnState fl_ign_state;	
	NV_Data_MPC_Day_Time nv_data_day_time;
	uint8 fl_RealTime_UB=0;

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_ign_state);
		
	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{

	}
	else
	{			
		//if( fl_ign_state == eIGN_RUN )
		//{				
			l_Tbox_signal_invalid = Clock_Check_TBOX_Signal_InValid();

			Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_RealTime_UB_Copy_1(&fl_RealTime_UB);

		
			if(l_Tbox_signal_invalid == TRUE || fl_RealTime_UB == CLOCK_REALTIME_INVALID)
			{					
				if(l_clock_firstBattary_counter == 0)
				{
					Rte_Read_NV_AmbientMdl_MPC_Date_Time_Value(&nv_data_day_time);
					l_clock_year = nv_data_day_time.year;
					l_clock_month = nv_data_day_time.month;
					l_clock_day = nv_data_day_time.day;

					Rtc_TimeType rtc_time;
					rtc_time.Date = l_clock_day;
					rtc_time.Hour = 12;
					rtc_time.Minute = 0;
					rtc_time.Second = 0;
					Rte_Call_rpCS_TIRtcCdd_SetTime(&rtc_time);
				}
				Clock_Get_RTC_Time();				
				l_clock_cali_count = 0;
			}
			else if(l_Tbox_signal_invalid == FALSE && fl_RealTime_UB == CLOCK_REALTIME_VALID)
			{				
				Clock_Get_TBOX_Time();

				if(l_clock_invalid_value == FALSE)
				{
					//calibrate time per 5s (30ms cycle)
					l_clock_cali_count++;
					if(l_clock_cali_count == 166)
					{
						l_clock_cali_count = 0;
						Clock_Calibrate_RTCTime();
					}
				}
			}
			l_clock_firstBattary_counter++;
			if(l_clock_firstBattary_counter == 10)
				l_clock_firstBattary_counter = 1;
		//}
		
		/*update time to AmbientMDL*/
		Clock_Update_Time();
		
		/*set output of clock hour and minute into HMI display*/	
		Rte_Write_ppSR_ClockMdl_Output_HourValue(l_clock_hour);
		Rte_Write_ppSR_ClockMdl_Output_MinutesValue(l_clock_minute);
		/*message 0x581 Can OutPut*/
		Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Year(l_clock_year);
		Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Month(l_clock_month);
		Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Date(l_clock_day);
		Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Hour(l_clock_hour);
		Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Minute(l_clock_minute);
		Rte_Write_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Second(l_clock_second);
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


static void Clock_Set_RTCTime(void)
{	
	Rtc_TimeType rtc_time;

	rtc_time.Date = l_clock_day;
	rtc_time.Hour = l_clock_hour;
	rtc_time.Minute = l_clock_minute;
	rtc_time.Second = l_clock_second;

	Rte_Call_rpCS_TIRtcCdd_SetTime(&rtc_time);
}

Rtc_TimeType Clock_Get_CurrentTime(void) 
{
	return l_rtc_currtime;
}


static void Clock_Initial(void)
{	
	l_clock_cali_count = 0;
	l_clock_invalid_value = TRUE;
}

static boolean Clock_Check_TBOX_Signal_InValid(void)
{
	uint8 fl_msg_nr;
	uint8 fl_msg_tout;
	
	/*Read 0x582 msg signal status*/
	Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_Tout(&fl_msg_tout);	
	Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_NR(&fl_msg_nr);
	
	if((fl_msg_nr == RTE_E_NEVER_RECEIVED) || (fl_msg_tout == RTE_E_TIMEOUT))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static void Clock_Get_TBOX_Time(void)
{
	uint8 fl_clock_day;
	uint8 fl_clock_hour;
	uint8 fl_clock_minute;
	uint8 fl_clock_second;
	uint8 fl_clock_month;
	uint8 fl_clock_year;

	/*Read 0x582 msg Year value*/		  
	Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Year(&fl_clock_year);	
	/*Read 0x582 msg Month value*/		  
	Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Month(&fl_clock_month);
	/*Read 0x582 msg Day value*/		  
	(void)Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Date(&fl_clock_day);
	/*Read 0x582 msg Hour value*/		  
	(void)Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Hour(&fl_clock_hour);
	/*Read 0x582 msg Minute value*/	
	(void)Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Minute(&fl_clock_minute);	
	/*Read 0x582 msg Second value*/	
	Rte_Read_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Second(&fl_clock_second);

	if((fl_clock_minute > INVALID_CLOCK_MINUTES) || (fl_clock_hour > INVALID_CLOCK_HOURS) \
		 || (fl_clock_day > INVALID_CLOCK_DATE) || (fl_clock_second > INVALID_CLOCK_SECONDS) || (fl_clock_month > INVALID_CLOCK_MONTH) || (fl_clock_year > INVALID_CLOCK_YEAR)) 
	{		
		Clock_Get_RTC_Time();
		l_clock_invalid_value = TRUE;	
	}
	else
	{
		//dispaly normal value
		l_clock_invalid_value = FALSE;
		l_clock_year = fl_clock_year;
		l_clock_month = fl_clock_month;
		l_clock_day = fl_clock_day;
		l_clock_hour = fl_clock_hour;
		l_clock_minute = fl_clock_minute;
		l_clock_second = fl_clock_second;
	}

}

static void Clock_Get_RTC_Time(void)
{
	Rtc_TimeType rtc_time;
	
	Rte_Call_rpCS_TIRtcCdd_GetTime(&rtc_time);
	
	l_clock_day = rtc_time.Date;
	l_clock_hour = rtc_time.Hour;
	l_clock_minute = rtc_time.Minute;
	l_clock_second = rtc_time.Second;

	//February may be has 28 or 29 day
	if(l_clock_month == 2)
	{
		//2017/2018/2019/2021/2022/2023 February has 28 days
		if(l_clock_year == 0 || l_clock_year == 1|| l_clock_year == 2|| l_clock_year == 4|| l_clock_year == 5|| l_clock_year == 6)
		{
			if(l_clock_day > 28)
			{
				l_clock_month++;
				l_clock_day = l_clock_day % 28;
				rtc_time.Date = l_clock_day;
				Rte_Call_rpCS_TIRtcCdd_SetTime(&rtc_time);
			}
		}
		//2020 February has 29 days
		else if(l_clock_year == 3)
		{
			if(l_clock_day > 29)
			{
				l_clock_month++;
				l_clock_day = l_clock_day % 29;
				rtc_time.Date = l_clock_day;
				Rte_Call_rpCS_TIRtcCdd_SetTime(&rtc_time);
			}
		}
	}
	else
	{
		if(l_clock_month == 1 || l_clock_month == 3 || l_clock_month == 5 || l_clock_month == 7 \
			|| l_clock_month == 8 || l_clock_month == 10 || l_clock_month == 12)
		{
			if(l_clock_day > 31)
			{
				l_clock_day = l_clock_day % 31;
			
				if(l_clock_day == 1)
				{
					l_clock_month++;
					rtc_time.Date = l_clock_day;
					Rte_Call_rpCS_TIRtcCdd_SetTime(&rtc_time);
				}
			}
		}		
		else if(l_clock_month == 4 || l_clock_month == 6 \
			|| l_clock_month == 9 || l_clock_month == 11)
		{
			if(l_clock_day > 30)
			{
				l_clock_day = l_clock_day % 30;
			
				if(l_clock_day == 1)
				{
					l_clock_month++;
					rtc_time.Date = l_clock_day;
					Rte_Call_rpCS_TIRtcCdd_SetTime(&rtc_time);
				}
			}
		}
	}

	if(l_clock_month > 12)
	{
		l_clock_month = l_clock_month % 12;
		if(l_clock_pre_month == 12 && l_clock_month == 1)
			l_clock_year++;
	}
	l_clock_pre_month = l_clock_month;	
}


static void Clock_Update_Time(void)
{
	l_rtc_currtime.Date = l_clock_day;
	l_rtc_currtime.Hour = l_clock_hour;
	l_rtc_currtime.Minute = l_clock_minute;
	l_rtc_currtime.Second = l_clock_second;

	//Rte_Write_ppSR_ClockMdl_Rtc_Time_Value(&l_rtc_currtime);
}

static void Clock_Calibrate_RTCTime(void)
{	
	Rtc_TimeType rtc_time;
	uint8 day;
	uint8 hour;
	uint8 minute;
	uint8 second;

	Rte_Call_rpCS_TIRtcCdd_GetTime(&rtc_time);	
	day = rtc_time.Date;
	hour = rtc_time.Hour;
	minute = rtc_time.Minute;
	second = rtc_time.Second;

	if((day != l_clock_day) || (hour != l_clock_hour) || (minute != l_clock_minute) || (second != l_clock_second))
	{
		Clock_Set_RTCTime();
	}
}

