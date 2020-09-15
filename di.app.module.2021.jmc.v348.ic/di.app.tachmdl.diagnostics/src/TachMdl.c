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

#include "Rte_TachMdl.h"
#include "CmpLib.h"
//#include "MathLib.h"
//#include "string.h"
#include "TachMdl.h"
#include "hmi_gauge_filter.h"



/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(TachMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define VEHICLE_TACH_INVALID			(0x7FFF)         //6/8   GS
#define VEHICLE_RAW_TACH_MAX			(7000000) 
#define ENGSPEED_CORRECT_RESOLUTION   (500)
#define ENGSPEED_DISPLAY_DIVISION     (10)
#define ENGSPEED_FILTER_COUNTER  	(2)
#define TIMEOUT_350MS				(350)
#define TIMEOUT_COUNTER_1000MS		(100)
#define TIMEOUT_2000MS				(2000)
#define TACHO_DIVIDER_VALUE         (100)

//2018/6/8  GS
#define TACH_IDLE_LOW				(650000)			//		3250->650000	/* 650 / 2 * 10		*/
#define TACH_IDLE_HIGH				(850000)						/* 850 / 2 * 10		*/
#define TACH_MAX_VAL				(7000000)						/* 7000 / 2 * 10 	*/

#define TACH_IDLE_VAL				(750000)						/* 750 / 2 * 10		*/

#define TACH_FILTER_VAL1_LOW		(500000)						/* 500 RPM			*/
#define TACH_FILTER_VAL1_HIGH		(1000000)						/* 1000 RPM			*/

#define TACH_FILTER_DIFF			(50000)						/* 50 RPM			*/


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static void f_tachMdl_initial(void);
static void f_sample_tach(void);
static void f_filter_tach(void);
static void f_tachMdl_invalidProcess(void);      //GS
static void f_tachMdl_Input_Process(void);
static void TachMdlPowerONDelay(void);



//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint32 l_filter_eng_tacho_U32 = 0;
static uint32 l_pre_filter_eng_tacho_U32 = 0;
static uint32 l_display_tacho_U32 = 0;
static uint8  l_filter_counter_U8 = 0;
static uint32 l_pre_filter_tacho_value_U32 = 0;
static uint32 l_target_indicated_tach_U32 = 0;
static uint32 l_EngSpeed_U32 = 0;
static boolean TachMdl_PowerON_Delay = FALSE;
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit ( void )
{
	f_tachMdl_initial();
	l_EngSpeed_U32 = 0;
	//Rte_Write_tiSR_TITachMdl_Send_value(l_display_tacho_U32);	
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
	f_tachMdl_initial();
	l_EngSpeed_U32 = 0;
	//Rte_Write_tiSR_TITachMdl_Send_value(l_display_tacho_U32);	
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
	f_tachMdl_initial();
	l_EngSpeed_U32 = 0;
	//Rte_Write_tiSR_TITachMdl_Send_value(l_display_tacho_U32);	
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
	f_tachMdl_initial();
	l_EngSpeed_U32 = 0;
	//Rte_Write_tiSR_TITachMdl_Send_value(l_display_tacho_U32);	
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
	//uint32 fl_tacho_U32;
	uint8 l_OTAMode = 0;
	
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	Rte_Read_rp_GetOTAModeStatus_Value_OTAMode(&l_OTAMode);
	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage || l_OTAMode == TRUE)
	{
		f_tachMdl_initial();
	}
	else
	{

    	switch(fl_IGN_state)
    	{
    		case eIGN_OFF:					
    			f_tachMdl_initial();
                TachMdl_PowerON_Delay = FALSE;
    			break;
    		case eIGN_RUN:
                if(TRUE == TachMdl_PowerON_Delay)
                { 
    			   f_tachMdl_Input_Process();
                }
    			break;
    		default:
    			break;
    	}
	}	
	
	//fl_tacho_U32 = l_display_tacho_U32/TACHO_DIVIDER_VALUE;
    //l_EngSpeed_U16 = DATCvtFirstOrderFilterPro(l_EngSpeed_U16,fl_tacho_U32,TACHO_FILTERCOEF);
	//fl_tacho_U32 = (uint32)l_EngSpeed_U16*TACHO_DIVIDER_VALUE;
	l_EngSpeed_U32 = l_display_tacho_U32;
	//l_EngSpeed_U32 = fl_tacho_U32;
	TachMdlPowerONDelay();

	Rte_Write_tiSR_TITachMdl_Send_value(l_EngSpeed_U32);	
	
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
static void f_tachMdl_initial(void)
{
	l_filter_eng_tacho_U32 = 0;
	l_pre_filter_eng_tacho_U32 = 0;
	l_display_tacho_U32 = 0;
	l_filter_counter_U8 = 0;
	l_pre_filter_tacho_value_U32 = 0;
	l_target_indicated_tach_U32 = 0;
//	TachMdl_PowerON_Delay = FALSE;
	//#ifdef TIMER_ENABLE
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleTachSampleWait);	
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleTachInvalidWait);
	//#endif
}
#if 1
static void f_tachMdl_invalidProcess(void)
{	
	boolean	fl_Timer_tachInvalid_RunningSt = 0;
	boolean	fl_Timer_tachInvalid_ElapsedSt = 0;
	Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerHandleTachInvalidWait,&fl_Timer_tachInvalid_RunningSt);
	if(fl_Timer_tachInvalid_RunningSt == FALSE)
	{
		Rte_Call_rpTimer_TmExt_Timer_Start(eTimerHandleTachInvalidWait);
		l_display_tacho_U32 = l_target_indicated_tach_U32/ENGSPEED_DISPLAY_DIVISION;
	}
	
	Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerHandleTachInvalidWait,TIMEOUT_2000MS,&fl_Timer_tachInvalid_ElapsedSt);
	//SysRS_02_Gauge-887
	if(fl_Timer_tachInvalid_ElapsedSt == TRUE)
	{
		//SysRS_02_Gauge-888
		f_tachMdl_initial();
	}
	//clear Sample Timer, to ensure retrieve immediately
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleTachSampleWait);
}
#endif
static void f_tachMdl_Input_Process(void)
{
	uint32 	fl_raw_tacho_value_U32 = 0;
	uint16	fl_EngFilterSpd_U16 = 0;
	uint8	fl_gws1_nvrd_st_U8 = 0;
	uint8 	fl_gws1_missing_st_U8 = 0;
//	uint8   fl_invalidFlag = FALSE;
//  RTE interface 2018/7/10

	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&fl_gws1_nvrd_st_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&fl_gws1_missing_st_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&fl_EngFilterSpd_U16);


	if((RTE_E_TIMEOUT == fl_gws1_missing_st_U8) || (RTE_E_NEVER_RECEIVED == fl_gws1_nvrd_st_U8))   //Fixed CID 5947837;
	{
		f_tachMdl_initial();
	}
	
	else if(fl_EngFilterSpd_U16 == VEHICLE_TACH_INVALID)
    {
   		f_tachMdl_invalidProcess();
    }
	else
	{		
		/* In order to prevent lose accuracy, we zoom in by 1000 here*/
		Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleTachInvalidWait);
	    fl_raw_tacho_value_U32 = (uint32)(fl_EngFilterSpd_U16 * ENGSPEED_CORRECT_RESOLUTION);

		if(fl_raw_tacho_value_U32 > VEHICLE_RAW_TACH_MAX)
		{
			fl_raw_tacho_value_U32 = VEHICLE_RAW_TACH_MAX;
		}
		//SysRS_02_Gauge-1395
        if(l_filter_counter_U8 < ENGSPEED_FILTER_COUNTER)
        {   
			l_filter_eng_tacho_U32 = fl_raw_tacho_value_U32;       //6/8  GS
            l_filter_counter_U8 ++;
        }
        else
        {
            l_filter_eng_tacho_U32 = ( l_pre_filter_eng_tacho_U32 * 7 + fl_raw_tacho_value_U32) / 8;
        }
		l_pre_filter_eng_tacho_U32 = fl_raw_tacho_value_U32;
		//l_filter_eng_tacho_U32 = fl_raw_tacho_value_U32;
		f_filter_tach();  
		f_sample_tach();
	}
}

static void f_sample_tach(void)
{
#if 0
	boolean	fl_Timer_tachSample_RunningSt;
	boolean	fl_Timer_tachSample_ElapsedSt;

	Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerHandleTachSampleWait,&fl_Timer_tachSample_RunningSt);
	if(fl_Timer_tachSample_RunningSt == FALSE)
	{
		Rte_Call_rpTimer_TmExt_Timer_Start(eTimerHandleTachSampleWait);
		l_display_tacho_U32 = l_target_indicated_tach_U32/ENGSPEED_DISPLAY_DIVISION;
	}
	
	Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerHandleTachSampleWait,TIMEOUT_350MS,&fl_Timer_tachSample_ElapsedSt);
	
	if(fl_Timer_tachSample_ElapsedSt == TRUE)
	{
		Rte_Call_rpTimer_TmExt_Timer_Start(eTimerHandleTachSampleWait);
		l_display_tacho_U32 = l_target_indicated_tach_U32/ENGSPEED_DISPLAY_DIVISION;
	}
#endif
		l_display_tacho_U32 = l_target_indicated_tach_U32/ENGSPEED_DISPLAY_DIVISION;
}

static void f_filter_tach(void)
{
	if( l_filter_eng_tacho_U32 >= TACH_MAX_VAL)
	{
		l_target_indicated_tach_U32 = TACH_MAX_VAL;
		l_pre_filter_tacho_value_U32 = TACH_MAX_VAL;
	}
	else if(l_filter_eng_tacho_U32 < TACH_FILTER_VAL1_HIGH) 
	{
		if( l_filter_eng_tacho_U32 > l_pre_filter_tacho_value_U32 )
		{
			if( (l_filter_eng_tacho_U32 - l_pre_filter_tacho_value_U32) > TACH_FILTER_DIFF )
			{
				l_target_indicated_tach_U32 = l_filter_eng_tacho_U32;
			}
			else
			{
				/* no change for 'l_target_indicated_tach_U32' */
			}
		}
		else
		{
			if( (l_pre_filter_tacho_value_U32 - l_filter_eng_tacho_U32 ) > TACH_FILTER_DIFF )
			{
				l_target_indicated_tach_U32 = l_filter_eng_tacho_U32;
			}
			else
			{
				/* no change for 'l_target_indicated_tach_U32' */
			}
		}

		l_pre_filter_tacho_value_U32 = l_target_indicated_tach_U32;

        
		if( (l_target_indicated_tach_U32 >= TACH_IDLE_LOW) && (l_target_indicated_tach_U32 <= TACH_IDLE_HIGH) )
		{
			l_target_indicated_tach_U32 = TACH_IDLE_VAL;
		}            
                       
	}
	else
	{
		l_target_indicated_tach_U32 = l_filter_eng_tacho_U32;

		l_pre_filter_tacho_value_U32 = 0;
	}
}
static void TachMdlPowerONDelay(void)
{	
	boolean	ul_Timer_vehSpdInvalid_RunningSt = 0;
	boolean	ul_Timer_vehSpdInvalid_ElapsedSt = 0;
    if(TachMdl_PowerON_Delay == FALSE)
    {
    	Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerTachTurnonDelay,&ul_Timer_vehSpdInvalid_RunningSt);
    	if(ul_Timer_vehSpdInvalid_RunningSt == FALSE)
    	{
    		Rte_Call_rpTimer_TmExt_Timer_Start(eTimerTachTurnonDelay);
    	}
    	
    	Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerTachTurnonDelay,5000,&ul_Timer_vehSpdInvalid_ElapsedSt);
    	if(ul_Timer_vehSpdInvalid_ElapsedSt == TRUE)
    	{
    		Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerTachTurnonDelay);
            TachMdl_PowerON_Delay = TRUE;
    	}
     }
}





/*----------------------------------------------------------------------------
Date              : 19-July-2017
CDSID             : kvakeshk
Traceability      : RTC ID 805747 & 875041
Change Description: tacho gauge Intial Version
-----------------------------------------------------------------------------*/


