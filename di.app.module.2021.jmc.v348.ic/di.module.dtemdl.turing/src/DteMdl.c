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
#include "Rte_DteMdl.h"
#include "CmpLib.h"
#include "DteMdl_cfg.h"
#include "FuelLevelMdl_cfg.h"
#include "fuel03.h"
#include "Mcu.h"
#include "AfcMdl_cfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(DTEMdl);


/*****************************************************************************
*								  Macro Definitions 						 *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the		 *
* purpose of the macro. 													 *
******************************************************************************/
#define DTE_LOOP_NUMBER                 (uint16)(100)
#define DTE_IDLE_SPEED_LOOP_NUMBER      (uint8)(10)
#define DTE_IDLE_SPEED_30S_TIMER        (uint16)(300)
#define DTE_NO_IDLE_SPEED_10S_TIMER     (uint16)(100)
#define DTE_INVALID_VALUE               (uint32)(0xFFFF)
#define DTE_MAX_FUEL_REMAINING          (uint32)(8060)
#define DTE_RAFC_MAX_VALUE              (uint32)(3000)
#define DTE_RAFC_MIN_VALUE              (uint32)(0)
#define DTE_CALC_CONST_VALUE        	(uint32)(131)
#define DTE_CALC_MOVE_VALUE         	(uint16)16
#define DTE_MAX_VALUE               	(uint16)1999
#define DTE_MIN_VALUE         		 	(uint16)50    
#define DTE_MID_VALUE          			(uint16)60    
#define DTE_ZREO                     	(uint8)0
#define DTE_FUEL_E_VALUE             	(uint16)800
#define DTE_ENG_SPD_RESOLUTION          (uint8)2
#define DTE_SPEED_3KMH                  (uint16)(300)
#define DTE_ENGSPEED_300RPM             (uint16)(30000)
#define DTE_CALCULATE_FACTOR            (uint8)10
#define DTE_ODO_5KM            			(uint16)5000
#define DTE_ODO_300M            		(uint16)300
#define DTE_CHANGE_FACTOR               (uint8)5
#define DTE_MAX_FUEL_CONSUM             (uint16)(120)
#define FODO_ODO_NVM_ERROR      		(uint32)(0xFFFFFFFFul)
#define InvalidFuelSignal               (uint16)0x1FFF
#define WhlMilgFrntValid				(uint8)1
#define DTE_RAFC_GAS					(uint16)810
#define DTE_RAFC_DIESEL					(uint16)720
#define ABS_VehSpdLgt_INVALID			(uint16)0xFFF 
#define EMS_EngSpeed_INVALID			(uint16)0x7FFF 
#define DTE_TimeOut_90S					(uint32)90000
#define DTE_TimeOut_120S				(uint32)120000
#define DTE_CALC_FACTOR					(uint16)2
#define DTE_DEFAULT_AFC					(uint16)810
#define DTE_TimeOut_300S				(uint32)(300000)		//300s

/*****************************************************************************
*                                 Type Decleration                           *
*----------------------------------------------------------------------------*
* Decleration of type shall be accompanied by a comment that explains the    *
* purpose and usage of the type.                                             *
******************************************************************************/
static void fdte_common_init(void);
static void fdte_ignOn_service(void);
static void fdte_calc_dte_value(void);
static uint32 fdte_calc_10000_RAFC_value(uint64 fuel_consumption, uint64 distance_moved);
static uint64 fdte_calc_Fuel_in_ml(uint64 history_fuel_consum_total, uint32 lastFuel, uint32 newFuel);
static boolean fdte_vehicle_abnormal_state(void);
static boolean fdte_fuelLevel_state(void);
static void fdte_check_first_Battary(void);
static void fdte_ignoffon_odo(void);
static void fdte_check_idle_status(void);
static void fdte_calc_fuelComsumption(void);
static boolean fdte_get_ABSWhlMil_state(void);
static void fdte_process_DteTask(void);
static void fdte_check_refuel_state(void);
static void fdte_check_EMS_EngSpeed_invalid(void);
static void fdte_check_ABS_VehSpdLgt_invalid(void);
static void fdte_display_value_processing(void);
static void fdte_display_negtive_value_processing(uint16	fl_vehicle_speed);
static void fdte_display_postive_value_processing(uint16	fl_vehicle_speed);
static void fdte_display_negtive_value_compare(uint32 fl_dte_delta_negtive);
static void fdte_display_postive_value_compare(uint32 fl_dte_delta_postive);
static void fdte_get_fast_dte(void);
static void fdte_lowdte_processing(void);
static void fdte_canOut_display_processing(void);
static void fdte_get_default_AFC(void);
static uint16 fdte_get_vehicle_speed(void);
static void fdte_process_DteValue(void);
static void fdte_abnormal_processing(void);

/*****************************************************************************
* 							   Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable. 													   *
* purpose, critical section, unit, and resolution 						   *
******************************************************************************/
/*ODO(k-1)*/
static uint32 l_dte_pre_odo_value_U32;/*10s calculate*/
/*ODO*/
static uint32 l_dte_current_odo_U32;
/*fuel consumption*/
static boolean l_fuelconsumption_abnormal;
/*indicator of waitting fuel and odo from IGN OFF to IGN ON*/
static boolean l_dte_wait_fuelsig_from_ign_on;
/*fuel value used to calculate DTE*/
static uint32   l_dte_fuel_consum_total;
static uint32 	l_pre_Fuel_consum_value_U32;
static uint16 	l_dte_calc_count_U16;
/*count number used to count 1s timer in idle speed status*/
//static uint8 l_fuel_calc_idle_count;
static boolean l_dte_idle_state;
/*refuel state*/
//static boolean l_refuel_state;
//static boolean l_refuel_pre_state;
//static boolean Refuel_State;
static boolean l_EMS_EngSpeed_invalid;
static boolean l_ABS_VehSpdLgt_invalid;
static uint8   l_EngSpeed_invalid_count;
static uint8   l_VehSpdLgt_invalid_count;
/*abnormal state*/
static boolean l_dte_abnormal_state;
//static boolean l_dte_pre_abnormal_state;
/*dte state*/
static boolean l_dte_start_process;
static boolean l_dte_negtive_state1;
static boolean l_dte_negtive_state2;
static boolean l_dte_negtive_state3;
static boolean l_dte_negtive_state4;
static boolean l_dte_postive_state1;
static boolean l_dte_postive_state2;
static boolean l_dte_postive_state3;
static boolean l_dte_postive_state4;
static uint8   l_dte_pre_ign_status = eIGN_OFF;

/*Buck RAM*/
#pragma ghs section bss=".buram"
/*DTE*/
static uint32 l_dte_display_value_U32;
static uint32 l_dte_calculate_value_U32;
static uint32 l_dte_next_display_value_U32;
/*RAFC(k-1)*/
static uint32 l_pre_dte_RAFC_value_U32;
/*RAFC(k)*/
static uint32 l_cur_dte_RAFC_value_U32;
/*F(k-1)*/
static uint32 l_pre_dte_F_value_U32;
static boolean l_dte_battary_reset;
static boolean l_dte_battary_reset_flag;
static boolean  DTE_First_Battary;
static uint32 l_dte_odo_value_U32;
static boolean l_Lowdte_Status = FALSE;
#pragma ghs section bss=default


#define TEST_DTE

uint16  test_dte_display_value_U32;
uint16  test_dte_calculate_value_U32;
uint16  test_dte_afc;
uint32  test_dte_fuel;
uint32  test_dte_odo;

static void dte_test(void);

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional initialization state
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpInit( void )
{				
	fdte_common_init();
	return E_OK;	
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional de-initialization state
** 
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDeInit( void )
{	
	fdte_common_init();
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional activation state
** 
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpActivation( void )
{	
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional de-activation state
** 
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDeActivation( void )
{	
    return  E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  State for normal operations
** 
** @return The return value is disregarded. Say E_OK
**---------------------------------------------------------------------------------------------------------------------*/
//#define eeprom_rte
static Std_ReturnType CmpActive( void )
{			
	eIgnState fl_IGN_state=0;
	EBatteryState fl_Battery_state=0;

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);	
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{

	}
	else
	{
		if(fl_IGN_state == eIGN_RUN && l_dte_pre_ign_status == eIGN_OFF)
		{
			fdte_ignoffon_odo();
		}
		
		if( fl_IGN_state == eIGN_RUN )
		{		
			fdte_ignOn_service();
		}	
		
		l_dte_pre_ign_status = fl_IGN_state;
	}

#ifdef TEST_DTE
	dte_test();
#endif

	fdte_canOut_display_processing();

	return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Diagnostic state
** 
** @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
**         E_NOT_READY:    Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDiag( void )
{
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
** 
** @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
**         E_NOT_READY:    Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}

/****************************************************************************
Function Name       : fdte_common_init

Description         : the Function is used to initialize global varible

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_common_init(void)
{
	l_dte_calc_count_U16 = 0;
	l_dte_wait_fuelsig_from_ign_on = TRUE;
	l_dte_fuel_consum_total = 0;
	l_dte_pre_ign_status = eIGN_OFF;
	//l_fuel_calc_idle_count = 0;
	//l_refuel_state =FALSE;  
}


/****************************************************************************
Function Name       : fdte_ignOn_service

Description         : the Function is used to process IGN ON Service

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_ignOn_service(void)
{
	fdte_check_first_Battary();
	
	fdte_process_DteTask();

	fdte_lowdte_processing();
}

/****************************************************************************
Function Name       : fdte_check_first_Battary

Description         : the Function is used to check first battary state

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_check_first_Battary(void)
{
	if(DTE_First_Battary == FALSE)			//first battary
	{
		DTE_First_Battary = TRUE;
		l_dte_display_value_U32 = DTE_INVALID_VALUE;
		l_dte_battary_reset = TRUE;		
		l_dte_battary_reset_flag = TRUE;
		l_dte_odo_value_U32 = OdoLogicalMdl_get_main(ODO_1M);	
		fdte_get_default_AFC();
	}
}


/****************************************************************************
Function Name       : fdte_check_idle_status

Description         : the Function is used to process idle task service

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_check_idle_status(void)
{
	uint32	fl_ABS_VehSpdLgt_value=0;  
	uint32  fl_EMS_EngSpeed_value = 0;
	boolean  fl_timer_resDef_started_bool = FALSE;
	boolean  fl_timer_resDef_isElapsed_bool = FALSE;

	Rte_Read_rpSR_TISpdMdl_value(&fl_ABS_VehSpdLgt_value);
	Rte_Read_rpSR_TITachMdl_value(&fl_EMS_EngSpeed_value);

	if(fl_EMS_EngSpeed_value < DTE_ENGSPEED_300RPM)		//Defect:tach=0,stop refresh afc
	{
		l_dte_idle_state = TRUE;						//stop calc fuel consumption
		Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerDTEIDLEWait, &fl_timer_resDef_started_bool); 
		if(fl_timer_resDef_started_bool == TRUE)	
		{
			Rte_Call_rptimer_TmExt_Timer_Stop(eTimerDTEIDLEWait);
		}
	}
	else
	{
		if(fl_ABS_VehSpdLgt_value < DTE_SPEED_3KMH)
		{
			Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerDTEIDLEWait, &fl_timer_resDef_started_bool); 
			if(fl_timer_resDef_started_bool)
			{
				// abnormal >= 300s
				Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerDTEIDLEWait, DTE_TimeOut_300S, &fl_timer_resDef_isElapsed_bool);
				if(fl_timer_resDef_isElapsed_bool)
				{
					l_dte_idle_state = TRUE;					//stop calc fuel consumption
				}
				else
				{	
					l_dte_idle_state = FALSE;
				}
			}
			else
			{
				Rte_Call_rptimer_TmExt_Timer_Start(eTimerDTEIDLEWait);
			}
		}
		else
		{
			l_dte_idle_state = FALSE;
			Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerDTEIDLEWait, &fl_timer_resDef_started_bool); 
			if(fl_timer_resDef_started_bool == TRUE)
			{
				Rte_Call_rptimer_TmExt_Timer_Stop(eTimerDTEIDLEWait);
			}
		}
	}
}

/****************************************************************************
Function Name       : fdte_calc_fuelComsumption

Description         : the Function is used to process fuel Comsumption service

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_calc_fuelComsumption(void)
{
	uint16  fl_fuel_consum_signal_value=0;  
	uint8   fl_fuel_consum_signal_nr=0;
	uint8   fl_fuel_consum_signal_tout=0;  

	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_Fuelcounter(&fl_fuel_consum_signal_value);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(&fl_fuel_consum_signal_nr);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(&fl_fuel_consum_signal_tout);  

	/*calc fuel consumption*/
	if(l_dte_wait_fuelsig_from_ign_on == TRUE)
	{
		  if( (fl_fuel_consum_signal_nr == RTE_E_NEVER_RECEIVED) || (fl_fuel_consum_signal_tout == RTE_E_TIMEOUT) ||  (fl_fuel_consum_signal_value == DTE_INVALID_VALUE) )
		  {	  	
			  /* until wait for fuel signal valid */
			  l_fuelconsumption_abnormal = TRUE;
		  }
		  else
		  {	  
			  l_pre_Fuel_consum_value_U32 = fl_fuel_consum_signal_value;
			  l_dte_wait_fuelsig_from_ign_on = FALSE;
			  l_fuelconsumption_abnormal = FALSE;
		  }
	}
	else
	{
		if( (fl_fuel_consum_signal_nr == RTE_E_NEVER_RECEIVED) || (fl_fuel_consum_signal_tout == RTE_E_TIMEOUT) ||  (fl_fuel_consum_signal_value == DTE_INVALID_VALUE) )
		{		 		  
		  	 /* Stop calculate fuel consumption when signal is not valid */
			 l_fuelconsumption_abnormal = TRUE;
		}
		else
		{
			 l_fuelconsumption_abnormal = FALSE;
			 if(l_dte_idle_state == FALSE)		//SysRS_03_HMI_Display_30600
			 {
				  /*in none idle speed status,	add fuel consumption*/
				  if(l_dte_fuel_consum_total >= DTE_MAX_FUEL_CONSUM)
				  {
					  /*for fuel consumtion if larger than 0.12L, use 0.12L*/
				  }
				  else
				  {
					 l_dte_fuel_consum_total = fdte_calc_Fuel_in_ml(l_dte_fuel_consum_total, l_pre_Fuel_consum_value_U32, fl_fuel_consum_signal_value);
					 if(l_dte_fuel_consum_total > DTE_MAX_FUEL_CONSUM)
					 {
						 l_dte_fuel_consum_total = DTE_MAX_FUEL_CONSUM;			//SysRS_03_LCD_Routine_1427
					 }
				  }
			  }
			  else
			  {
				  /*vehicle is in idle speed status, not add fuel consmption*/
			  }
			  l_pre_Fuel_consum_value_U32 = fl_fuel_consum_signal_value;
		}
	}
}


/****************************************************************************
Function Name       : fdte_abnormal_processing

Description         : the Function is used to process DTE abnormal state

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_abnormal_processing(void)
{
	if(l_dte_abnormal_state == TRUE)		//abnormal --> normal state
	{
		  if(l_dte_battary_reset == TRUE)		//first battary
		  {
			  l_dte_battary_reset = FALSE;
			  fdte_calc_dte_value();
			  l_dte_display_value_U32 = l_dte_calculate_value_U32;
		  }
		  else
		  {
			  l_dte_display_value_U32 = l_dte_next_display_value_U32;
		  }
		  l_dte_abnormal_state = FALSE;
	}
}


/****************************************************************************
Function Name       : fdte_process_DteValue

Description         : the Function is used to process DTE value service

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_process_DteValue(void)
{
	uint32 fl_distance_moved_U32 = 0;
	
	l_dte_current_odo_U32 = OdoLogicalMdl_get_main(ODO_1M);

	if(l_dte_calc_count_U16 == 0)
	{
		l_dte_pre_odo_value_U32 = OdoLogicalMdl_get_main(ODO_1M);
		++l_dte_calc_count_U16;
	}
	else if(l_dte_calc_count_U16 >= DTE_LOOP_NUMBER)			//refresh cycle is 10s
	{
		if((l_dte_idle_state == TRUE) || (l_fuelconsumption_abnormal == TRUE))		//0x366 missing or idle state
		{
			/*fuel or odo is invalid, not calculate RAFC(k), use RAFC(k-1), stop */
			l_dte_fuel_consum_total = 0;
			l_pre_Fuel_consum_value_U32 = 0;
		}
		else
		{
			if(l_dte_current_odo_U32 >= l_dte_pre_odo_value_U32)
			{
				fl_distance_moved_U32 = l_dte_current_odo_U32 - l_dte_pre_odo_value_U32;
			}
			l_cur_dte_RAFC_value_U32 = fdte_calc_10000_RAFC_value(l_dte_fuel_consum_total, (uint32)fl_distance_moved_U32);
		}
		
		fdte_calc_dte_value();

		fdte_display_value_processing();
		
		l_dte_fuel_consum_total = 0;
		
		l_dte_calc_count_U16 = 0;
	}
	else
	{
		++l_dte_calc_count_U16;
	}
}


/****************************************************************************
Function Name       : fdte_process_DteTask

Description         : the Function is used to process DTE service

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_process_DteTask(void)
{
	boolean fl_fuel_abnormal = FALSE;
	boolean fl_ABS_WhlMil_abnormal = FALSE;
	
	fl_ABS_WhlMil_abnormal = fdte_get_ABSWhlMil_state();
	
	fl_fuel_abnormal = fdte_fuelLevel_state();	
	
	fdte_check_EMS_EngSpeed_invalid();
	
	fdte_check_ABS_VehSpdLgt_invalid();	
		
	if((fl_fuel_abnormal == TRUE) || (l_ABS_VehSpdLgt_invalid == TRUE) || (l_EMS_EngSpeed_invalid == TRUE))
	{
		/*
		*  case1  Fuel sensor input abnormal		   		  
		*  case2  Invalid signal received信号值无效		   		
		*/
		l_dte_display_value_U32 = DTE_INVALID_VALUE;	    //show "---km"
		l_dte_abnormal_state = TRUE;
	}
	else
	{
		fdte_get_fast_dte();
		
		fdte_abnormal_processing();
		
		if(fl_ABS_WhlMil_abnormal == TRUE)					//0x221 missing
		{
			/* stop calculate RAFC , stop calculate DTE*/
			l_dte_calc_count_U16 = 0;
			l_dte_fuel_consum_total = 0;
			l_pre_Fuel_consum_value_U32 = 0;
		}
		else
		{
			fdte_check_idle_status();
			
			fdte_calc_fuelComsumption();
			
			fdte_process_DteValue();
		}
	}
}


/****************************************************************************
Function Name       : fdte_calc_dte_value

Description         : the Function is used to calculate DTE value

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/

static void fdte_calc_dte_value(void)
{
	uint16 fl_remain_fuel_level_U16=0;

	fl_remain_fuel_level_U16 = ffuel_get_parameter_display_fuel();

	if(fl_remain_fuel_level_U16 > DTE_MAX_FUEL_REMAINING)			//SysRS_03_LCD_Routine_657
	{
	  fl_remain_fuel_level_U16 = DTE_MAX_FUEL_REMAINING;
	}

	if(DTE_ZREO != l_cur_dte_RAFC_value_U32)					
	{   
		if(fl_remain_fuel_level_U16 > 0)
		{
			l_dte_calculate_value_U32 = (fl_remain_fuel_level_U16 * 100) / l_cur_dte_RAFC_value_U32;
		}
	}
	else
	{
	  /* l_cur_dte_RAFC_value_U32 is 0, l_dte_calculate_value_U32 use previous value*/
	}
}


/****************************************************************************
Function Name       : fdte_get_dte_value

Description         : the Function is used to get DTE value

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
uint32 fdte_get_dte_value(void)
{
  uint32 fl_dte_display_value;

  if(DTE_INVALID_VALUE == l_dte_display_value_U32)
  {
	  fl_dte_display_value = DTE_INVALID_VALUE;
  }
  else if(DTE_MAX_VALUE <= l_dte_display_value_U32)
  {
	  fl_dte_display_value = DTE_MAX_VALUE;
  }
  else
  {
	  fl_dte_display_value = l_dte_display_value_U32;
  }
  return fl_dte_display_value;
}


/****************************************************************************
Function Name       : fdte_calc_10000_RAFC_value

Description         : the Function is used to calculate RAFC value

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static uint32 fdte_calc_10000_RAFC_value(uint64 fuel_consumption, uint64 distance_moved)
{
	uint32 fl_F_value_U32;
	uint32 fl_RAFC_vaule_U32;
	/**********************************************
	** fuel_consumption uint : 0.01 L
	** distance moved unit: m
	**F(k) = F(k-1)- (R_AFC(k-1) * d) + Fccd(k) unit: mL/100km
	**R_AFC(k) = F(k)/100 unit : mL/km
	***********************************************/

	fl_F_value_U32 = (l_pre_dte_F_value_U32 - (l_pre_dte_RAFC_value_U32 * distance_moved)/1000) + 10*(fuel_consumption);
	fl_RAFC_vaule_U32 = (fl_F_value_U32/100);

	/*
	When R_AFC>= 0.3L/km, R_AFC is adopted 0.3L/km.
	*/
	if(fl_RAFC_vaule_U32 > DTE_RAFC_MAX_VALUE)
	{
	  fl_RAFC_vaule_U32 = DTE_RAFC_MAX_VALUE;
	  fl_F_value_U32 = DTE_RAFC_MAX_VALUE * 100;
	}
	else
	{

	}
	
	l_pre_dte_F_value_U32 = fl_F_value_U32;
	l_pre_dte_RAFC_value_U32 = fl_RAFC_vaule_U32;

	return fl_RAFC_vaule_U32;
}

/****************************************************************************
Function Name		: fdte_calc_Fuel_in_ml

Description 		: calculate fuel consumption from the begining point of 10s timer

Invocation			: Internal

Parameters			: None

Return Value		: None

Critical Section	: None
******************************************************************************/
static uint64 fdte_calc_Fuel_in_ml(uint64 history_fuel_consum_total, uint32 lastFuel, uint32 newFuel)
{
	uint64 new_fuel_consum_total;
	if( newFuel < lastFuel )
	{
	   new_fuel_consum_total = history_fuel_consum_total + ((uint64)(65535 + newFuel - lastFuel)*DTE_CALC_FACTOR) /1000;
	}
	else
	{
	  /* Fuel Consumption = History Fuel + Delta			  */
	  /* Delta = ( new - last ) * FACTOR					  */
	  new_fuel_consum_total = history_fuel_consum_total + \
							  (((uint64)newFuel * DTE_CALC_CONST_VALUE) >> DTE_CALC_MOVE_VALUE) - \
							  (((uint64)lastFuel * DTE_CALC_CONST_VALUE) >> DTE_CALC_MOVE_VALUE);
	}

	return new_fuel_consum_total;
}

/****************************************************************************
Function Name		: fdte_vehicle_abnormal_state

Description 		: tcheck if vehicle speed is less than 3 km, in this case, not add fuel consumption to calculate DTE

Invocation			: Internal

Parameters			: None

Return Value		: None

Critical Section	: None
******************************************************************************/
static boolean fdte_vehicle_abnormal_state(void)
{
	boolean fl_speed_abnormal_state=FALSE;
	uint8 	fl_ABS_VehSpdLgt_nr=0;
	uint8   fl_ABS_VehSpdLgt_tout=0;

	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_ABS_VehSpdLgt_nr);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_ABS_VehSpdLgt_tout);


	if(fl_ABS_VehSpdLgt_nr == RTE_E_NEVER_RECEIVED || fl_ABS_VehSpdLgt_tout == RTE_E_TIMEOUT)
	{		
		fl_speed_abnormal_state = TRUE;
	}
	else
	{
		fl_speed_abnormal_state = FALSE;
	}
	return fl_speed_abnormal_state;
}


/****************************************************************************
Function Name       : fdte_fuelLevel_state

Description         : the Function is used to get fuel level abnormal state

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static boolean fdte_fuelLevel_state(void)
{
	uint16 fl_fuel_state;

	Rte_Read_rpSR_DteMdl_AbnormalState_AbnormalState(&fl_fuel_state);

	if(fl_fuel_state == TRUE)
	{
		return TRUE;				//abnormal
	}
	else
	{
		return FALSE;				//normal
	}
}

/****************************************************************************
Function Name       : fdte_clear_DteValue

Description         : the Function is used to clear dte value

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
void fdte_clear_DteValue(void)
{
	fdte_get_default_AFC();
	l_dte_calc_count_U16 = 0;
	l_dte_fuel_consum_total = 0;
}


/****************************************************************************
Function Name       : fdte_ignoffon_odo

Description         : the Function is used to get previous odo value when ign off to on

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/

static void fdte_ignoffon_odo(void)
{	
	//IGN OFF -> IGN ON Processing,
	l_dte_wait_fuelsig_from_ign_on = TRUE;			// Reset first fuel consum signal value
	
	l_dte_pre_odo_value_U32 = OdoLogicalMdl_get_main(ODO_1M);	
	
	if((l_dte_display_value_U32 >= DTE_MIN_VALUE) && (l_dte_display_value_U32 <= DTE_MID_VALUE))		//SysRS_03_HMI_Display_031833
	{
		l_Lowdte_Status = FALSE;
	}
	else
	{
		l_Lowdte_Status = TRUE;
	}
}


/****************************************************************************
Function Name       : fdte_get_ABSWhlMil_state

Description         : the Function is used to get ABSWhlMil state

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : check if vehicle speed is less than 3 km, in this case, not add fuel consumption to calculate DTE
******************************************************************************/
static boolean fdte_get_ABSWhlMil_state(void)
{
	uint8 	fl_ABS_WhlMil_nr=0;
	uint8 	fl_ABS_WhlMil_tout=0;
	boolean fl_ABS_WhlMil_abnormal = FALSE;
	
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_ABS_WhlMil_nr);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_ABS_WhlMil_tout);

	if((fl_ABS_WhlMil_nr == RTE_E_NEVER_RECEIVED) || (fl_ABS_WhlMil_tout == RTE_E_TIMEOUT))
	{
		fl_ABS_WhlMil_abnormal = TRUE;
	}
	
	return fl_ABS_WhlMil_abnormal;
}

#if 0
/****************************************************************************
Function Name       : fdte_check_refuel_state

Description         : the Function is used to get refuel state
Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_check_refuel_state(void)
{
	uint8 fl_refuel_state = FALSE;

	Rte_Read_rpSR_TIFuel_RefuelState_TIFuel_RefuelState(&fl_refuel_state);

	if(fl_refuel_state == TRUE)
	{
		l_refuel_state = TRUE;
	}
	else
	{
		l_refuel_state = FALSE;
	}
}
#endif

/****************************************************************************
Function Name       : fdte_check_EMS_EngSpeed_invalid

Description         : the Function is used to check EMS EngSpeed is invalid or not
Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_check_EMS_EngSpeed_invalid(void)
{
	uint16 fl_EMS_EngSpeed_raw=0;
	
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&fl_EMS_EngSpeed_raw);

	if(fl_EMS_EngSpeed_raw == EMS_EngSpeed_INVALID)
	{
		l_EngSpeed_invalid_count++;
		if(l_EngSpeed_invalid_count >= 20)				//EMS_EngSpeed=0xFFF for more than 2S
		{
			l_EMS_EngSpeed_invalid = TRUE;
			l_EngSpeed_invalid_count = 0;
		}
	}
	else
	{
		l_EMS_EngSpeed_invalid = FALSE;		
		l_EngSpeed_invalid_count = 0;
	}
}

/****************************************************************************
Function Name       : fdte_check_ABS_VehSpdLgt_invalid

Description         : the Function is used to check ABS EngSpeed is invalid or not
Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_check_ABS_VehSpdLgt_invalid(void)
{
	uint16 fl_ABS_VehSpdLgt_raw=0;
	
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_ABS_VehSpdLgt_raw);

	if(fl_ABS_VehSpdLgt_raw == ABS_VehSpdLgt_INVALID)
	{
		l_VehSpdLgt_invalid_count++;
		if(l_VehSpdLgt_invalid_count >= 20)				//ABS_VehSpdLgt=0x7FFF for more than 2S
		{
			l_ABS_VehSpdLgt_invalid = TRUE;
			l_VehSpdLgt_invalid_count = 0;
		}
	}
	else
	{
		l_ABS_VehSpdLgt_invalid = FALSE;		
		l_VehSpdLgt_invalid_count = 0;
	}
}


/****************************************************************************
Function Name       : fdte_display_negtive_value_processing

Description         : the Function is used to process display negtive value
Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_display_negtive_value_processing(uint16	fl_vehicle_speed)
{
	boolean fl_timer_resDef_started_bool=FALSE;
	boolean fl_timer_resDef_isElapsed_bool=FALSE;
	
	if(l_dte_negtive_state1 == TRUE)						//DTE display - calculate > 60km
	{
		if(l_dte_current_odo_U32 - l_dte_odo_value_U32 > 9000)	//ODO > 9KM
		{
			l_dte_display_value_U32++;
			l_dte_start_process = FALSE;
			l_dte_negtive_state1 = FALSE;
			l_dte_odo_value_U32 = l_dte_current_odo_U32;
			l_dte_next_display_value_U32 = l_dte_display_value_U32;
		}
	}
	
	if(l_dte_negtive_state2 == TRUE)						//DTE display - calculate > 40km
	{
		if(l_dte_current_odo_U32 - l_dte_odo_value_U32 > 6000)	//ODO > 6KM
		{
			l_dte_display_value_U32--;
			l_dte_start_process = FALSE;
			l_dte_negtive_state2 = FALSE;
			l_dte_odo_value_U32 = l_dte_current_odo_U32;
			l_dte_next_display_value_U32 = l_dte_display_value_U32;
		}
	}
	
	if(l_dte_negtive_state3 == TRUE)						//DTE display - calculate > 20km
	{
		if(l_dte_current_odo_U32 - l_dte_odo_value_U32 > 3000)	//ODO > 3KM
		{
			l_dte_display_value_U32--;
			l_dte_start_process = FALSE;
			l_dte_negtive_state3 = FALSE;
			l_dte_odo_value_U32 = l_dte_current_odo_U32;
			l_dte_next_display_value_U32 = l_dte_display_value_U32;
		}
	}
	
	if(l_dte_negtive_state4 == TRUE)						//DTE display - calculate < 20km
	{
		if(fl_vehicle_speed >= DTE_SPEED_3KMH)
		{
			//start timer 120s
			Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerDTEWait, &fl_timer_resDef_started_bool); 
			if(fl_timer_resDef_started_bool)
			{
				// abnormal >= 120s
				Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerDTEWait, DTE_TimeOut_120S, &fl_timer_resDef_isElapsed_bool);
				if(fl_timer_resDef_isElapsed_bool)
				{
					l_dte_display_value_U32--;
					l_dte_start_process = FALSE;
					l_dte_negtive_state4 = FALSE;
					l_dte_odo_value_U32 = l_dte_current_odo_U32;
					l_dte_next_display_value_U32 = l_dte_display_value_U32;

					Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerDTEWait, &fl_timer_resDef_started_bool); 
					if(fl_timer_resDef_started_bool)
						Rte_Call_rptimer_TmExt_Timer_Stop(eTimerDTEWait);
				}
			}
			else
			{
				Rte_Call_rptimer_TmExt_Timer_Start(eTimerDTEWait);
			}		

		}
	}
}


/****************************************************************************
Function Name       : fdte_display_negtive_value_processing

Description         : the Function is used to process display postive value
Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_display_postive_value_processing(uint16	fl_vehicle_speed)
{
	boolean fl_timer_resDef_started_bool=FALSE;
	boolean fl_timer_resDef_isElapsed_bool=FALSE;
	
	if(l_dte_postive_state1 == TRUE)								//display - calculate > 60
	{
		if(l_dte_current_odo_U32 - l_dte_odo_value_U32 > 1000)			//ODO > 1KM
		{
			l_dte_display_value_U32 = l_dte_display_value_U32 - 3;
			l_dte_start_process = FALSE;
			l_dte_postive_state1 = FALSE;
			l_dte_odo_value_U32 = l_dte_current_odo_U32;
			l_dte_next_display_value_U32 = l_dte_display_value_U32;
		}
	}
	
	if(l_dte_postive_state2 == TRUE)								//DTE display - calculate > 30km
	{
		if(l_dte_current_odo_U32 - l_dte_odo_value_U32 > 1000)			//ODO > 1KM
		{
			l_dte_display_value_U32 = l_dte_display_value_U32 - 2;
			l_dte_start_process = FALSE;
			l_dte_postive_state2 = FALSE;
			l_dte_odo_value_U32 = l_dte_current_odo_U32;
			l_dte_next_display_value_U32 = l_dte_display_value_U32;
		}
	}
	
	if(l_dte_postive_state3 == TRUE)								//DTE display - calculate > 10km
	{
		if(l_dte_current_odo_U32 - l_dte_odo_value_U32 > 1000)			//ODO > 1KM
		{
			l_dte_display_value_U32 = l_dte_display_value_U32 - 1;
			l_dte_start_process = FALSE;
			l_dte_postive_state3 = FALSE;
			l_dte_odo_value_U32 = l_dte_current_odo_U32;
			l_dte_next_display_value_U32 = l_dte_display_value_U32;
		}
	}
	
	if(l_dte_postive_state4 == TRUE)								//DTE display - calculate < 10km
	{
		if(fl_vehicle_speed >= DTE_SPEED_3KMH)
		{
			//start timer 90s	
			Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerDTEWait, &fl_timer_resDef_started_bool); 
			if(fl_timer_resDef_started_bool)
			{
				// abnormal >= 90s
				Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerDTEWait, DTE_TimeOut_90S, &fl_timer_resDef_isElapsed_bool);
				if(fl_timer_resDef_isElapsed_bool)
				{
					l_dte_display_value_U32 = l_dte_display_value_U32 - 1;
					l_dte_start_process = FALSE;
					l_dte_postive_state4 = FALSE;
					l_dte_odo_value_U32 = l_dte_current_odo_U32;
					l_dte_next_display_value_U32 = l_dte_display_value_U32;
					
					Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerDTEWait, &fl_timer_resDef_started_bool); 
					if(fl_timer_resDef_started_bool)
						Rte_Call_rptimer_TmExt_Timer_Stop(eTimerDTEWait);
				}
			}
			else
			{
				Rte_Call_rptimer_TmExt_Timer_Start(eTimerDTEWait);
			}
		}
	}
}


/****************************************************************************
Function Name       : fdte_display_negtive_value_compare

Description         : the Function is used to compare negtive value
Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_display_negtive_value_compare(uint32 fl_dte_delta_negtive)
{
	if(fl_dte_delta_negtive > 250)
	{
		l_dte_display_value_U32 = l_dte_calculate_value_U32;
		l_dte_odo_value_U32 = l_dte_current_odo_U32;
		l_dte_next_display_value_U32 = l_dte_display_value_U32;
	}
	else if(fl_dte_delta_negtive > 60)
	{
		l_dte_start_process = TRUE;
		l_dte_negtive_state1 = TRUE;
	}
	else if(fl_dte_delta_negtive > 40)
	{
	
		l_dte_start_process = TRUE;
		l_dte_negtive_state2 = TRUE;
	}
	else if(fl_dte_delta_negtive > 20)
	{
	
		l_dte_start_process = TRUE;
		l_dte_negtive_state3 = TRUE;
	}
	else if(fl_dte_delta_negtive < 20)
	{
		l_dte_start_process = TRUE;
		l_dte_negtive_state4 = TRUE;
	}
}

/****************************************************************************
Function Name       : fdte_display_postive_value_compare

Description         : the Function is used to compare postive value
Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_display_postive_value_compare(uint32 fl_dte_delta_postive)
{
	if(fl_dte_delta_postive > 120)
	{
		l_dte_display_value_U32 = l_dte_calculate_value_U32;
		l_dte_odo_value_U32 = l_dte_current_odo_U32;
		l_dte_next_display_value_U32 = l_dte_display_value_U32;
	}
	else if(fl_dte_delta_postive > 60)
	{
		l_dte_start_process = TRUE;
		l_dte_postive_state1 = TRUE;
	}
	else if(fl_dte_delta_postive > 30)
	{
		l_dte_start_process = TRUE;
		l_dte_postive_state2 = TRUE;
	}
	else if(fl_dte_delta_postive > 10)
	{
		l_dte_start_process = TRUE;
		l_dte_postive_state3 = TRUE;
	}
	else if(fl_dte_delta_postive < 10)
	{
		l_dte_start_process = TRUE;
		l_dte_postive_state4 = TRUE;
	}

}


/****************************************************************************
Function Name       : fdte_display_value_processing

Description         : the Function is used to process display value
Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_display_value_processing(void)
{
	uint32 fl_dte_delta_postive = 0;
	uint32 fl_dte_delta_negtive = 0;
	uint16	fl_vehicle_speed=0;

	if(l_dte_start_process == FALSE)
	{
		if(l_dte_calculate_value_U32 > l_dte_next_display_value_U32)
		{
			fl_dte_delta_negtive = l_dte_calculate_value_U32 - l_dte_next_display_value_U32;
			fdte_display_negtive_value_compare(fl_dte_delta_negtive);
		}
		else if(l_dte_calculate_value_U32 < l_dte_next_display_value_U32)
		{
			fl_dte_delta_postive = l_dte_next_display_value_U32 - l_dte_calculate_value_U32;
			fdte_display_postive_value_compare(fl_dte_delta_postive);
		}
	}

	if(l_dte_start_process == TRUE)
	{
		fl_vehicle_speed = fdte_get_vehicle_speed();
		fdte_display_negtive_value_processing(fl_vehicle_speed);		
		fdte_display_postive_value_processing(fl_vehicle_speed);
	}
}


/****************************************************************************
Function Name       : fdte_get_fast_dte

Description         : the Function is used to process refuel state
Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_get_fast_dte(void)
{
	uint8 fl_refuel_state = FALSE;

	Rte_Read_rpSR_TIFuel_RefuelState_TIFuel_RefuelState(&fl_refuel_state);

	if(l_dte_battary_reset_flag == TRUE || fl_refuel_state == TRUE)				//Fast fuel state or First Battary
	{
		fdte_calc_dte_value();
		if(l_dte_calculate_value_U32 != DTE_ZREO)
		{
			  l_dte_battary_reset_flag = FALSE; 				  
			  if(l_dte_calculate_value_U32 > l_dte_next_display_value_U32)
			  {
				  if(l_dte_calculate_value_U32 - l_dte_next_display_value_U32 > 10)
				  {
					  l_dte_display_value_U32 = l_dte_calculate_value_U32;
					  l_dte_next_display_value_U32 = l_dte_display_value_U32;
				  }
			  }
			  else
			  {
				  if(l_dte_next_display_value_U32 - l_dte_calculate_value_U32 > 10)
				  {
					  l_dte_display_value_U32 = l_dte_calculate_value_U32;
					  l_dte_next_display_value_U32 = l_dte_display_value_U32;
				  }
			  }
		}
	}
}

/****************************************************************************
Function Name       : dte_test

Description         : the Function is used to DTE TEST Page

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void dte_test(void)
{
	test_dte_display_value_U32 = (uint16)l_dte_display_value_U32;
	test_dte_calculate_value_U32 = (uint16)l_dte_calculate_value_U32;
	test_dte_afc = (uint16)l_cur_dte_RAFC_value_U32;
}


/****************************************************************************
Function Name       : fdte_lowdte_processing

Description         : the Function is used to process low DTE Value

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_lowdte_processing(void)
{
	if(l_dte_display_value_U32 < DTE_MIN_VALUE)
	{
		l_Lowdte_Status = TRUE;
	}
	else if(l_dte_display_value_U32 > DTE_MID_VALUE)
	{
		l_Lowdte_Status = FALSE;
	}
	else
	{
		/*between */
	}
}

/****************************************************************************
Function Name       : fdte_canOut_display_processing

Description         : the Function is used to process DTE can output value 

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_canOut_display_processing(void)
{
	if(l_dte_display_value_U32 == DTE_INVALID_VALUE)
	{
		Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_DTE(0xfff);
		
		Rte_Write_ppSR_DteMdl_DteVal_DteVal(DTE_INVALID_VALUE);
	}
	else
	{
		if(l_dte_display_value_U32 >= DTE_MAX_VALUE)
		{
			Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_DTE(DTE_MAX_VALUE);

			Rte_Write_ppSR_DteMdl_DteVal_DteVal(DTE_MAX_VALUE);
		}
		else
		{
			if(l_Lowdte_Status == TRUE)
			{
				Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_DTE(0xfff);

				Rte_Write_ppSR_DteMdl_DteVal_DteVal(DTE_INVALID_VALUE);
			}
			else
			{
				Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_DTE(l_dte_display_value_U32);
				
				Rte_Write_ppSR_DteMdl_DteVal_DteVal(l_dte_display_value_U32);
			}
		}
	}
}


/****************************************************************************
Function Name       : fdte_get_default_AFC

Description         : the Function is used to get default AFC Value

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void fdte_get_default_AFC(void)
{
	//uint8 IsEngineCfg=0;
	//uint16 fl_Default_AFC_Value=0;

	#if 0
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION, &IsEngineCfg);

	if(IsEngineCfg <=8)
	{
		fl_Default_AFC_Value = NVM_AFC_AFE_VALUE[IsEngineCfg];
	}
	else
	{
		fl_Default_AFC_Value = DTE_DEFAULT_AFC;
	}
	#endif

	l_pre_dte_RAFC_value_U32 = DTE_DEFAULT_AFC;
	l_pre_dte_F_value_U32 = DTE_DEFAULT_AFC * 100;
	l_cur_dte_RAFC_value_U32 = l_pre_dte_RAFC_value_U32;
}

static uint16 fdte_get_vehicle_speed(void)
{
	uint32 fl_ABS_VehSpdLgt_value=0;
	
	Rte_Read_rpSR_TISpdMdl_value(&fl_ABS_VehSpdLgt_value);

	return fl_ABS_VehSpdLgt_value;
}


