/*============================================================================
**
**                     CONFIDENTIAL VISTEON CORPORATION
**
** This is an unpublished work of authorship, which contains trade secrets,
** created in 2007. Visteon Corporation owns all rights to this work and
** intends to maintain it in confidence to preserve its trade secret status.
** Visteon Corporation reserves the right, under the copyright laws of the
** United States or those of any other country that may have jurisdiction,
** to protect this work as an unpublished work, in the event of an
** inadvertent or deliberate unauthorized publication. Visteon Corporation
** also reserves its rights under all copyright laws to protect this work as
** a published work, when appropriate. Those having access to this work may
** not copy it, use it, modify it or disclose the information contained in
** it without the written authorization of Visteon Corporation.
**
**===========================================================================*/


#include "Rte_FuelLevelMdl.h"
#include "CmpLib.h"
#include "FuelLevelMdl_cfg.h"
#include "linear_interpolation.h"
#include "fhyst.h"
#include "Rte_IoHwAb_Type.h"
#include "system.h"
#include "fuel03.h"
#include "Dem.h"
#include "fuel03_bds_cfg.h"
#include "string.h"


CMPLIB_INSTANCE(FuelLevelMdl)



static void ffuel_compute_resistorValue(void);
static void ffuel_pkg_process_KSSlowTimetask(void);
static void ffuel_pkg_process_Diagntask(void);
static void ffuel_compute_fuelVolume(void);
static void ffuel_display_oneBar(void);
static void ffuel_display_zeroBar(void);
static void ffuel_process_abnormal_resistor(eIgnState fl_IGN_state);
static boolean ffuel_process_resistor_to_fuelVolume(void);
static void ffuel_get_slosh_fuelVolume(void);
static void ffuel_process_fuel_gauge(void);
static void ffuel_calc_fuel_led_bar(void);
static void ffuel_output_canMessage(void);
static void ffuel_calc_fuel_percentage(void);
static boolean ffuel_directConvertVolume(void);
static void ffuel_KSWakeup(void);
static void ffuel_lowfuel_status(void);
static void ffuel_gauge_common_init(void);
static void ffuel_calc_average_rad(void);
static void ffuel_average_rad_processing(void);
static boolean  ffuel_process_start_to_stop(void);
static void ffuel_check_first_battOff_On(void);
static uint16 ffuel_Calc_IGNOFF_ON_Fuel(void);


/*****************************************************************************
*                                 Macro Definitions                          *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/
#define subResolution(x)    					(x-3)
#define addResolution(x)   						(x+3)
#define FUEL_FAST_FILL_ENTRY_VSP_LIMIT   		(uint16)(300)    		//3KM/H		
#define FUEL_FAST_FILL_ENTRY_TACH_LIMIT   		(uint32)(30000)    		//300RPM
#define SHORT_CIRCUIT_RESISTOR    				(uint16)(300)			//3ohm
#define OPEN_CIRCUIT_RESISTOR    				(uint16)(30000)			//300ohm
#define F_STOP_RESISTOR							(uint16)(900)			//9ohm
#define E_STOP_RESISTOR							(uint16)(29040)			//290ohm
#define Fuel_TimeOut_60S                    	(uint16)(60000)			//60s
#define NORMAL_CIRCUIT							(uint16)(0x1)
#define SHORT_CIRCUIT							(uint16)(0x2)
#define OPEN_CIRCUIT							(uint16)(0x3)
#define InvalidFuelSignal        				(uint32)(0x1FFF)		// the max signal value from it is 68.00L < 0x1FFF, OK
#define ValidFuelSignal                 		(uint16)(0x1)
#define RESULT_NORMAL             				(uint16)(0x2)
#define RESULT_FAILED            				(uint16)(0x3)
#define FUEL_SENSOR_FAULT        				(uint16)(10)
#define IOC_FE05_GAUGE_TYPE_FUEL				(uint16)(0x81)
#define FUEL_LEVEL_PERCENT_INVALID				(uint16)(0x7F)
#define FUEL_FULL_VOLUME						(uint16)(7420)
#define FUEL_EMPTY_VOLUME						(uint16)(500)
#define FUEL_INVALID_VOLUME						(uint16)0xffff
#define FULE_MAX_LED_BAR						(uint8)(8)
#define FUEL_ONE_LEDBAR							(uint8)1
#define FUEL_ZERO_LEDBAR						(uint8)0
#define DISPLAY_GEAR_P     						(uint8)0xD
#define DISPLAY_GEAR_N     						(uint8)0x0
#define Fuel_TimeOut_5S							(uint16)(50)
#define Fuel_Sample_Cycle						(uint8)(4)
#define IGNOFF_ON								(uint8)1
#define SAMPLE_RES_CNT							(uint8)15

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
const uint16 NVM_DTE_1_LFW_ON_Thres_U16  = (1100);						/* Default Value : 11L		low fuel */
const uint16 NVM_DTE_1_LFW_OFF_Thres_U16 = (1540);						/* Default Value : 15.4L	low fuel */
const uint32 NVM_FUEL_CALI_Factor[2] = { 688956 , 425531 };


/*********************************************************************************
              Res to Fuel table
*********************************************************************************/
static const uint16 F_FUEL_A_RES_TO_Volum[13][2] =
{
	{ (uint16)(0u), (uint16)(29040u) },		
	{ (uint16)(1010u), (uint16)(8060u) },	
	{ (uint16)(3760u), (uint16)(7420u) },
	{ (uint16)(5980u), (uint16)(6780u) },
	{ (uint16)(8200u), (uint16)(6140u) },
	{ (uint16)(11700u), (uint16)(5180u) },		
	{ (uint16)(15140u), (uint16)(4220u) },		
	{ (uint16)(17790u), (uint16)(3300u) },
	{ (uint16)(21010u), (uint16)(2380u) },
	{ (uint16)(23710u), (uint16)(1540u) },
	{ (uint16)(25010u), (uint16)(1100u) },
	{ (uint16)(27820u), (uint16)(500) },
	{ (uint16)(29040u), (uint16)(0u) },
};


/*********************************************************************************
              PF to Fuel table
*********************************************************************************/
static const uint16 F_FUEL_A_PF_TO_FUEL[14][2] =
{
	{ (uint16)(0u), 		(uint16)(254*256u) },
	{ (uint16)(0),			(uint16)(0)}, 
	{ (uint16)(1u),			(uint16)(60u)}, 		/* 1.89	    = 0% = 0.6L 		*/
	{ (uint16)(15u),		(uint16)500u  },		/* 15.7	    = 6.2% = 5L 		*/
	{ (uint16)(34u),		(uint16)1100u },		/* 34.6	    = 13% = 11L		*/
	{ (uint16)(48u),		(uint16)1540u },		/* 48.5	    = 18.5% = 15.4L		*/
	{ (uint16)(75u),		(uint16)2380u },		/* 75.0     = 29% = 23.8L		*/
	{ (uint16)(103u),		(uint16)3300u },		/* 103.9	= 40.5% = 33L		*/
	{ (uint16)(132u),		(uint16)4220u },		/* 132.9	= 52% = 42.2L		*/
	{ (uint16)(163u),		(uint16)5180u },		/* 163.2	= 64% = 51.8L		*/
	{ (uint16)(193u),		(uint16)6140u } ,		/* 193.4	= 76% = 61.4L		*/
	{ (uint16)(213u),		(uint16)6780u },		/* 213.6	= 84% = 67.8L			*/
	{ (uint16)(233u),		(uint16)7420u },		/* 233.8	= 92% = 74.2L		*/
	{ (uint16)(254u),		(uint16)8060u },		/* 254.0	= 100%   = 80.6L			*/
};

/*********************************************************************************
              Fuel to LED BAR table
*********************************************************************************/
static const THRESHOLD F_FUEL_BAR_THRESHOLD[10] =
{
	{ (uint16)8, (uint16)0 },
	{ (uint16)500, (uint16)1540 },
	{ (uint16)1540, (uint16)2380 },
	{ (uint16)2380, (uint16)3300 },
	{ (uint16)3300, (uint16)4220 },
	{ (uint16)4220, (uint16)5180 },
	{ (uint16)5180, (uint16)6140 },
	{ (uint16)6140, (uint16)6780 },
	{ (uint16)6780, (uint16)7420 },
	{ (uint16)7420, (uint16)8060 },
};


/*****************************************************************************
*                                 Type Declarations                          *
******************************************************************************/
static uint8 l_sleep_status_flag_U8;
static uint16 l_fuel_filter_resistance_U16;
static uint16 l_fuel_filter_volume_U16;
static uint16 l_fuel_unfilter_volume_U16;
static uint16 l_fuel_ignoff_instance_volume_U16;
static boolean l_ignon_off_flag = FALSE;

static uint8 l_fuel_ignoff_on;
boolean l_fuel_abnormal_status;
static uint8  l_fuel_ip_counter;     
static uint8 l_ignoff_cnt = 0;
static boolean IGNOFF_ON_FLAG;
static uint16 l_fuel_resistance[15]={0};
static uint8 l_fuel_sample=0;

typedef struct  
{
	uint16 min_rad;
	uint16 max_rad;
	uint16 avg_rad;
	uint16 stable_rad;
	uint16 moving_sample_counter;
	uint8 use_avg_flag;
    uint16 sample_array[FUEL03_SMPLS_2_AVG];
	
}Fuel_Average_Property;

Fuel_Average_Property FuelAvgValue;
static uint8 l_fuel_cycle_count=0;

boolean l_fuel_stable_finish;
boolean l_fuel_start_calc_avg;

#pragma ghs section bss=".buram"
static uint16 l_fuel_bar_level_counts_U16;
static uint16	l_calculate_fuel_value_U16;
static uint16	l_store_fuel_value_U16=0;
static uint16	l_display_fuel_value_U16;
static boolean l_LowFuel_Status_1_BOOL;
static eIgnState l_pre_ign_status;
static uint16 l_fuel_filter_volume_U16;
static uint16 l_fuel_unfilter_volume_U16;
static eIgnState l_pre_ignoff_status = eIGN_OFF;
static uint8 l_fuel_percentage_level_U8;
static uint8 l_pre_fuel_percentage_level_U8;
static boolean l_first_battOff_On = FALSE;
#pragma ghs section bss=default

//Fuel Test page
static void fuel_test(void);
uint16 test_fuel_res;
uint16 test_unfuel;
uint16 test_calfuel;
uint16 test_disfuel;
uint16 test_stable_fuel;

//#define Fuel_Test  


//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	fuel03_KSColdInit();

	ffuel_gauge_common_init();

	ffuel_check_first_battOff_On();
	
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
	fuel03_KSColdInit();

	ffuel_gauge_common_init();
	
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
    ffuel_KSWakeup();
	
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
	ffuel_KSWakeup();
	
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional Active state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------

static Std_ReturnType CmpActive( void )
{
	Dcm_SesCtrlType fl_Diag_Session = DEFAULT_SESSION;
	
	Rte_Call_rpCS_GetCurrentSessionType_Operation(&fl_Diag_Session);

	if(fl_Diag_Session == EOL_SESSION || fl_Diag_Session == EXTENDED_SESSION)
	{
		ffuel_pkg_process_Diagntask();
	}
	else
	{
		ffuel_pkg_process_KSSlowTimetask();
	}


	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Diagnostic state
///
/// @return E_OK:			Diagnostic has completed. Go to DiagReturn state.
///<br> 	E_NOT_READY:	Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:			Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br> 	E_NOT_READY:	Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
	return E_OK;
}

static void ffuel_gauge_common_init(void)
{	
    //l_pre_ignoff_status = eIGN_OFF;
	l_pre_ign_status = eIGN_OFF;
	l_fuel_ignoff_on = 0;
    l_fuel_ip_counter = 0;    
}



/****************************************************************************
Function Name       : ffuel_KSWakeup()

Description         : Initializes the variables.

Invocation          : Invoked by the Scheduler during Wakeup

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static void ffuel_KSWakeup(void)
{
    l_sleep_status_flag_U8 = TRUE;
}


/****************************************************************************
Function Name       : ffuel_pkg_process_Diagntask()

Description         : This function process Diagnotic Service of fuel level
                      
Invocation          : Invoked by Scheduler via the timedtask mechanism
                      at 100 millisecond interval

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static void ffuel_pkg_process_Diagntask(void)
{	
	DCM_IOC_Type DID_Value;
	uint16 fl_led_num;
	uint16 fl_ioc_type;

	Rte_Read_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(&DID_Value);			/*DID for Diagnostic*/

	if(DID_Value.DID_IOC_Number == 0x5105)
	{
		if(DID_Value.DID_IOC_Parament == 0x03)
		{
			fl_ioc_type = DID_Value.DID_IOC_State[0];
			if(fl_ioc_type == IOC_FE05_GAUGE_TYPE_FUEL)
			{
				fl_led_num = DID_Value.DID_IOC_State[1];
				if(fl_led_num > FULE_MAX_LED_BAR)
					fl_led_num = FULE_MAX_LED_BAR;
				Rte_Write_tiSR_FuelLevel_level(fl_led_num);
			}
		}
		else if(DID_Value.DID_IOC_Parament == 0x00)
		{
			ffuel_pkg_process_KSSlowTimetask();
		}
	}

}


/****************************************************************************
Function Name       : ffuel_pkg_process_KSSlowTimetask()

Description         : This function calls the fuel filter function
                      fuel03_periodic_processing() periodically at a
                      interval of 100msec and take care of initial fuel
                      display upon Ign OFF to ON

Invocation          : Invoked by Scheduler via the timedtask mechanism
                      at 100 millisecond interval

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static void ffuel_pkg_process_KSSlowTimetask(void)
{
	eIgnState fl_IGN_state;
	boolean fl_fuel_state;
	boolean fl_fuel03_periodic_processing;

	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

	fl_fuel_state = ffuel_process_resistor_to_fuelVolume();
	
#ifdef Fuel_Test
	fuel_test();
#endif
	
	if(fl_fuel_state == FALSE)
		return; 		

	if(fl_IGN_state == eIGN_RUN)		
	{			
		if(l_pre_ign_status == eIGN_OFF)
		{
			fuel03_KSColdInit();				
			ffuel_gauge_common_init();
			l_pre_ign_status = eIGN_RUN;
			l_fuel_ignoff_on = IGNOFF_ON;
			l_fuel_stable_finish = FALSE;
		}
			
		//ffuel_average_rad_processing();
		
		if(l_fuel_ip_counter == 4)			// 500ms refresh cycle
		{
			fl_fuel03_periodic_processing = fuel03_periodic_processing(l_fuel_filter_volume_U16, FALSE);
			
			if(fl_fuel03_periodic_processing == FALSE)
				return;
		
			ffuel_process_fuel_gauge();
		}
			
		l_sleep_status_flag_U8 = FALSE;		
	}
	else
	{
		if(l_pre_ign_status == eIGN_RUN)
		{
			l_ignon_off_flag = TRUE;
			l_fuel_ignoff_instance_volume_U16 = l_fuel_filter_volume_U16;
			l_store_fuel_value_U16 = l_display_fuel_value_U16;						//store parameter A
		}
		
		Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn(l_fuel_ignoff_instance_volume_U16); 

		if(l_ignon_off_flag == TRUE)
		{
			if(l_ignoff_cnt < 100)
			{
				l_ignoff_cnt++;
				if(l_ignoff_cnt % 10 == 0)											//store parameter C after every 1000ms
				{
					l_store_fuel_value_U16 = l_fuel_filter_volume_U16;
				}
			}
			else
			{
			
			}
		}
		
	   	l_fuel_ignoff_on = 0;
	}		
	
	l_pre_ign_status = fl_IGN_state;

	++l_fuel_ip_counter;
	
    if(l_fuel_ip_counter > 4)
    {
        l_fuel_ip_counter = 0;
    }
    else
    {
        /* do nothing */
    }

	ffuel_output_canMessage();


}


/*============================================================================
**
** Function Name:      void ffuel_Calc_IGNOFF_ON_Fuel(void)
**
** Description:        ffuel calculate parameter B when IGN OFF to ON
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/

static uint16 ffuel_Calc_IGNOFF_ON_Fuel(void)
{
	uint8 i=0;
	uint16 fl_res_sum_b=0;
	uint16 fl_res_average_b=0;
	
	l_fuel_resistance[l_fuel_sample++] = l_fuel_filter_resistance_U16;

	if(l_fuel_sample >= SAMPLE_RES_CNT)
	{
		/* average the samples */
		for(i=0;i<SAMPLE_RES_CNT;i++)
		{
			/* summing */
			fl_res_sum_b += l_fuel_resistance[i];
		}

		/* average */
		fl_res_average_b = fl_res_sum_b / SAMPLE_RES_CNT;

		l_fuel_sample = 0;

		l_fuel_filter_resistance_U16 = fl_res_average_b;
		
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/*============================================================================
**
** Function Name:      void ffuel_process_resistor_to_fuelVolume(void)
**
** Visibility:        
**
** Description:        ffuel process fuelVolume state
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/
static boolean ffuel_process_resistor_to_fuelVolume(void)
{
	boolean fl_timer_resDef_started_bool;
	eIgnState fl_IGN_state;
	boolean fl_calc_state=FALSE;
	
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);	

	ffuel_compute_resistorValue();
	
	// SysRS_02_Gauges_1281, 3 ohm shortCircuit , 9 ohm openCircuit
	if(l_fuel_filter_resistance_U16 < SHORT_CIRCUIT_RESISTOR || l_fuel_filter_resistance_U16 > OPEN_CIRCUIT_RESISTOR)
	{
		ffuel_process_abnormal_resistor(fl_IGN_state);	
		l_pre_ignoff_status = fl_IGN_state;
		return FALSE;
	}
	else
	{			
		Rte_Write_ppSR_DteMdl_AbnormalState_AbnormalState(ValidFuelSignal);
		Rte_Write_ppSR_FuelShortCircuitTimeout_STATUS(FALSE);
		Rte_Write_ppSR_FuelOpenCircuitTimeout_STATUS(FALSE);		
		Rte_Call_rp_TmExt_Wrap_Timer_IsStarted(eTimerHandleFuelInvalidWait, &fl_timer_resDef_started_bool); 
		if(fl_timer_resDef_started_bool)
			Rte_Call_rp_TmExt_Timer_Stop(eTimerHandleFuelInvalidWait);
			
		//resistance is between 9ohm ~ 290ohm
		if(l_fuel_filter_resistance_U16 >= F_STOP_RESISTOR && l_fuel_filter_resistance_U16 <= E_STOP_RESISTOR)
		{
			ffuel_compute_fuelVolume();		
			return TRUE;
		}
		else
		{
			return FALSE;
		}

		//DTC SET NORMAL
		if(fl_IGN_state == eIGN_RUN)
		{
			Dem_SetEventStatus(FUEL_SENSOR_FAULT, RESULT_NORMAL);
		}
		
		l_pre_ignoff_status = fl_IGN_state;
	}
}

/*============================================================================
**
** Function Name:      void ffuel_process_abnormal_resistor(void)
**
** Visibility:        
**
** Description:        ffuel process abnormal resistor
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/
static void ffuel_process_abnormal_resistor(eIgnState fl_IGN_state)
{
	boolean fl_timer_resDef_started_bool;
	boolean fl_timer_resDef_isElapsed_bool;	

	if(fl_IGN_state == eIGN_RUN && l_pre_ignoff_status == eIGN_OFF)				//when ign off --> on
	{
		ffuel_display_zeroBar();			//Fuel Gauge:Scale "E"
		fuel03_KSColdInit();				
		ffuel_gauge_common_init();
		l_pre_ign_status = eIGN_RUN;
		l_fuel_abnormal_status = TRUE;
		l_fuel_ignoff_on = IGNOFF_ON;
	}

	Rte_Call_rp_TmExt_Wrap_Timer_IsStarted(eTimerHandleFuelInvalidWait, &fl_timer_resDef_started_bool); 
	if(fl_timer_resDef_started_bool)
	{
		// abnormal >= 60s
		Rte_Call_rp_TmExt_Wrap_Timer_IsElapsed(eTimerHandleFuelInvalidWait, Fuel_TimeOut_60S, &fl_timer_resDef_isElapsed_bool);
		if(fl_timer_resDef_isElapsed_bool)
		{
			l_fuel_abnormal_status = TRUE;
			ffuel_display_zeroBar();								//display 0 LED Bar 
			Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn(InvalidFuelSignal);						
			Rte_Write_ppSR_CANMSG_IC_0x392_ComOut_IC_Fuel_level(FUEL_LEVEL_PERCENT_INVALID);
			if(l_fuel_filter_resistance_U16 < SHORT_CIRCUIT_RESISTOR)
			{
				Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_VolumnQF(SHORT_CIRCUIT);		//SHORT CIRCUIT
				Rte_Write_ppSR_FuelShortCircuitTimeout_STATUS(TRUE);								//low fuel TT display in SHORT CIRCUIT
			}
			else if(l_fuel_filter_resistance_U16 > OPEN_CIRCUIT_RESISTOR)
			{
				Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_VolumnQF(OPEN_CIRCUIT);			//OPEN CIRCUIT
				Rte_Write_ppSR_FuelOpenCircuitTimeout_STATUS(TRUE);									//low fuel TT display in open CIRCUIT
			}
			
			Rte_Write_ppSR_DteMdl_AbnormalState_AbnormalState(InvalidFuelSignal);

			if(fl_IGN_state == eIGN_RUN)
			{
				/* DTC Trouble code >60s*/			
				Dem_SetEventStatus(FUEL_SENSOR_FAULT, RESULT_FAILED);
			}
		}
		else
		{			
			Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn(l_fuel_filter_volume_U16);
			Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_VolumnQF(NORMAL_CIRCUIT);				//normal circuit
				Rte_Write_ppSR_CANMSG_IC_0x392_ComOut_IC_Fuel_level(l_fuel_percentage_level_U8);			//IC FUEL PERCNET VOLUME
			Rte_Write_ppSR_DteMdl_AbnormalState_AbnormalState(ValidFuelSignal);	

			if(fl_IGN_state == eIGN_RUN)
			{
				/* DTC Trouble code normal*/			
				Dem_SetEventStatus(FUEL_SENSOR_FAULT, RESULT_NORMAL);
			}

			if(fl_IGN_state == eIGN_OFF && l_pre_ignoff_status == eIGN_RUN)								//Parameter A normal stored when ign off
			{
				l_calculate_fuel_value_U16 = l_fuel_filter_volume_U16 ;										//Parameter A
			}
		}
	}
	else
	{
		Rte_Call_rp_TmExt_Timer_Start(eTimerHandleFuelInvalidWait);
	}

}


/*============================================================================
**
**  Function Name:      static void ffuel_directConvertVolume(void)
**
**  Visibility:         Internal
**
**  Description:        This function is used to directly  Convert Fuel Volume
**
**  Input Arguments:    none
**
**  Output Arguments:   none
**
**  Critical Section:   None
**
**  Created:            07/02/19 by Vincent
**
**==========================================================================*/

static boolean ffuel_directConvertVolume(void)
{
	/*********************************************
	*	
	*	58±3    --->    60.07L
	*	83±3    --->    54L
	*	98±3    --->    49.29L
	*	118±3   --->   	42.6L
	*	143±3   --->   	34.83L
	*	168±3   --->   	27.06L
	*	193±3   --->   	19.75L
	*	228±3   --->   	10.7L
	*	253±3   --->   	5.67L
	*	273±3   --->   	2.9L
	*
	**********************************************/	
			
	if ((l_fuel_filter_resistance_U16  <= 28170) && (l_fuel_filter_resistance_U16 >= 27470))	//278.2±3.5
	{
		l_fuel_unfilter_volume_U16 = 500;
		return TRUE;
	}	
	else if ((l_fuel_filter_resistance_U16 <= 25360) && (l_fuel_filter_resistance_U16 >= 24660))	//250.1±3.5
	{
		l_fuel_unfilter_volume_U16 = 1100;			
		return TRUE;
	}	
	else if ((l_fuel_filter_resistance_U16 <= 24020) && (l_fuel_filter_resistance_U16 >= 23400))	//237.1±3.1
	{
		l_fuel_unfilter_volume_U16 = 1540;
		return TRUE;
	}
	else if ((l_fuel_filter_resistance_U16 <= 21290) && (l_fuel_filter_resistance_U16 >= 20810))	//210.1±2.8
	{
		l_fuel_unfilter_volume_U16 = 2380;
		return TRUE;
	}
	else if ((l_fuel_filter_resistance_U16 <= 18030) && (l_fuel_filter_resistance_U16 >= 17550))	//177.9±2.4
	{	
		l_fuel_unfilter_volume_U16 = 3300;		
		return TRUE;
	}
	else if ((l_fuel_filter_resistance_U16 <= 15350) && (l_fuel_filter_resistance_U16 >= 14930))	//151.4±2.1
	{
		l_fuel_unfilter_volume_U16 = 4220;
		return TRUE;
	}
	else if ((l_fuel_filter_resistance_U16 <= 11870) && (l_fuel_filter_resistance_U16 >= 11530))	//117±1.7
	{
		l_fuel_unfilter_volume_U16 = 5180;
		return TRUE;
	}
	else if ((l_fuel_filter_resistance_U16 <= 8330) && (l_fuel_filter_resistance_U16 >= 8070))		//82±1.3
	{
		l_fuel_unfilter_volume_U16 = 6140;		
		return TRUE;
	}
	else if ((l_fuel_filter_resistance_U16 <= 6080) && (l_fuel_filter_resistance_U16 >= 5880))		//59.8±1.0
	{
		l_fuel_unfilter_volume_U16 = 6780;		
		return TRUE;
	}
	else if ((l_fuel_filter_resistance_U16 <= 3860) && (l_fuel_filter_resistance_U16 >= 3660))		//37.6±1.0
	{
		l_fuel_unfilter_volume_U16 = 7420;		
		return TRUE;
	}
	else if ((l_fuel_filter_resistance_U16 <= 1110) && (l_fuel_filter_resistance_U16 >= 910))		//10.1±1.0
	{
		l_fuel_unfilter_volume_U16 = 8060;		
		return TRUE;
	}

	return FALSE;
}


/*============================================================================
**
**  Function Name:      static void Fuel_Compute_FuelVolume(eIgnState fl_IGN_state)
**
**  Visibility:         Internal
**
**  Description:        This function is used to compute fuel volume
**
**  INPUTS/OUTPUTS
**
**  Input Arguments:    none
**
**  Output Arguments:   none
**
**  Critical Section:   None
**
**  Created:            07/02/18 by Vincent
**
**==========================================================================*/
 static void ffuel_compute_fuelVolume()
{	
	boolean fl_directConvert = FALSE;
	
	fl_directConvert = ffuel_directConvertVolume();
	
	if(fl_directConvert == FALSE)
	{
		l_fuel_unfilter_volume_U16 = LinearInterpolateWord((void *)&F_FUEL_A_RES_TO_Volum[0][0], l_fuel_filter_resistance_U16);								
	}

	if(l_pre_ign_status == eIGN_OFF || l_fuel_abnormal_status == TRUE)
	{
		l_fuel_filter_volume_U16 = l_fuel_unfilter_volume_U16;
		//l_fuel_abnormal_status = FALSE;
	}
	else
	{
		l_fuel_filter_volume_U16 = (l_fuel_filter_volume_U16 * 7 + l_fuel_unfilter_volume_U16) / 8;
	}
}



/****************************************************************************
Function Name       : Fuel_Compute_Resistor_Values()

Description         : This function is used compute the RAD value from the Raw AD

Invocation          : called by statemanager 

Parameters          : None

Return Value        : None

Critical Section    : None

------------------------------------------------------------
       P_12VC
         |
         |
         |
         R1
         |
         |
     -------------R2-------------------------MCU
     |                |             |
     |                |             |
     |                |             |
     R'                R3           C
     |                |             |
     |                |             |
     |                |             |
     GND             GND           GND


======= Formula :

			AD_fuel * 1000000
	R = ---------------------------
		 K * AD_bat  - 	B * AD_fuel

			R3			         1				1
	K = ---------- * ---------------------* -------* 1000000	 
		  R2 + R3	   Voltage_monitor_gain		R1
		  1	
	B = ----- * 1000000
		  R1

******************************************************************************/
 static void ffuel_compute_resistorValue(void)
{
	sint32 fl_FuelSender_ATD_value_U16;
	sint32 fl_Vbatt_ATD_value_U16;
	sint32 tmp1;
	sint32 tmp2;

	Rte_Read_tiSR_IoHwAb_Impl_GetVoltage(eIOVoltageInId_MAI_P_BAT_C, &fl_Vbatt_ATD_value_U16);
	Rte_Read_tiSR_IoHwAb_Impl_GetVoltage(eIOVoltageInId_MAI_FUEL, &fl_FuelSender_ATD_value_U16);

	tmp1 = NVM_FUEL_CALI_Factor[0] * fl_Vbatt_ATD_value_U16 / 100;
	tmp2 = NVM_FUEL_CALI_Factor[1] * fl_FuelSender_ATD_value_U16 / 100;

	if (fl_FuelSender_ATD_value_U16 == 0 || fl_Vbatt_ATD_value_U16 == 0)
	{
		l_fuel_filter_resistance_U16 = 0;
	}
	else if ((tmp1 > tmp2) && (fl_FuelSender_ATD_value_U16 < 2690))
	{
		tmp1 = (tmp1 - tmp2) / fl_FuelSender_ATD_value_U16;
		tmp1 = 1000000 / tmp1;
		l_fuel_filter_resistance_U16 = (uint16)tmp1 *100;
	}
	else
	{
		l_fuel_filter_resistance_U16 = 65535;
	}
}

/*============================================================================
**
** Function Name:      void ffuel_process_fuel_gauge(void)
**
** Visibility:         Externally visible via ffuel.h
**
** Description:        Low fuel status routine
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/
static void ffuel_process_fuel_gauge(void)
{
    boolean fl_fuel_init_status_bool;

    fl_fuel_init_status_bool = fuel03_init_finished();

    if(FALSE != fl_fuel_init_status_bool)
    {
    	//get fuel volume
        ffuel_get_slosh_fuelVolume();

		//cacl fuel percentage
        ffuel_calc_fuel_percentage();

        //can output
		//ffuel_output_canMessage();

		//calc fuel led bar
		ffuel_calc_fuel_led_bar();

		//proces low fuel TT state
		ffuel_lowfuel_status();
    }
	
}

/*============================================================================
**
** Function Name:      void ffuel_get_slosh_fuelVolume(void)
**
** Visibility:         Externally visible via ffuel.h
**
** Description:        Low fuel status routine
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/
static void ffuel_get_slosh_fuelVolume(void)
{
    uint16  fl_fuel_percent_U16;
    boolean fl_fuel_init_status_bool;

    fl_fuel_init_status_bool = fuel03_init_finished();

    if(FALSE != fl_fuel_init_status_bool)
    {
        /*
        ** Get the percent full value with hysteresis
        ** for driving the display
        */
        fl_fuel_percent_U16 = MS_8_BITS(fuel03_get_senderA_PF());

        l_calculate_fuel_value_U16 = LinearInterpolateWord( (uint16 *)&F_FUEL_A_PF_TO_FUEL[0][0], fl_fuel_percent_U16);		
		
		if((l_fuel_ignoff_on == IGNOFF_ON) /*&& (l_calculate_fuel_value_U16 != 0)*/)
		{
			l_fuel_bar_level_counts_U16 = fhyst_get_index_with_up_hyst((void *)&F_FUEL_BAR_THRESHOLD[0],(uint16)(l_calculate_fuel_value_U16), l_fuel_bar_level_counts_U16); 		
			l_fuel_percentage_level_U8 = l_calculate_fuel_value_U16 * 100 / FUEL_FULL_VOLUME ;
			l_pre_fuel_percentage_level_U8 = l_fuel_percentage_level_U8;
			l_display_fuel_value_U16 = l_calculate_fuel_value_U16;			
			l_fuel_ignoff_on++;
		}
    }
}

/*============================================================================
**
** Function Name:      void ffuel_calc_fuel_percentage(void)
**
** Visibility:         
**
** Description:        ffuel calc fuel percentage
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/

static void ffuel_calc_fuel_percentage(void)
{
	uint8 fl_fuel_percentage_level_U8;
	boolean fl_fastfill_status;

	//fl_fastfill_status = ffuel_FastFill_Entry_Conditions();

	fl_fastfill_status = fuel03_ign_off_on_fill_notification();
	
	if(l_calculate_fuel_value_U16 <= FUEL_EMPTY_VOLUME)
	{
		l_fuel_percentage_level_U8 = 0;
	}
	else if(l_calculate_fuel_value_U16 >= FUEL_FULL_VOLUME)
	{
		l_fuel_percentage_level_U8 = 100;
	}
	else
	{
		if(fl_fastfill_status == TRUE)
		{
			l_fuel_percentage_level_U8 = l_calculate_fuel_value_U16 * 100 / FUEL_FULL_VOLUME;			
			l_pre_fuel_percentage_level_U8 = l_fuel_percentage_level_U8;
		}
		else
		{
			fl_fuel_percentage_level_U8 =  l_calculate_fuel_value_U16 * 100 / FUEL_FULL_VOLUME ;
			if(fl_fuel_percentage_level_U8 < l_pre_fuel_percentage_level_U8)
			{
				l_fuel_percentage_level_U8 = fl_fuel_percentage_level_U8;
				l_pre_fuel_percentage_level_U8 = l_fuel_percentage_level_U8;
			}
		}
	}
}

/*============================================================================
**
** Function Name:      void ffuel_output_canMessage(void)
**
** Visibility:         
**
** Description:        ffuel output canMessage
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/

static void ffuel_output_canMessage(void)
{	
	Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_VolumnQF(NORMAL_CIRCUIT);		//normal circuit
	Rte_Write_ppSR_CANMSG_IC_0x392_ComOut_IC_Fuel_level(l_fuel_percentage_level_U8);
	if(l_display_fuel_value_U16 < FUEL_EMPTY_VOLUME)		//SysRS_02_Gauges_020413  When Fuel point<E Point (5L), output is 0L
	{
		Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn(0);	
	}
	else
	{
		Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn(l_fuel_unfilter_volume_U16);	
	}
}
/*============================================================================
**
** Function Name:      void ffuel_calc_fuel_led_bar(void)
**
** Visibility:         
**
** Description:        ffuel calc fuel gauge led bar
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/
static void ffuel_calc_fuel_led_bar(void)
{
	boolean fl_fastfill_state = FALSE;

	//fl_fastfill_state = ffuel_FastFill_Entry_Conditions();
	
	fl_fastfill_state = fuel03_ign_off_on_fill_notification();
	
	/***** non-fast fuel/leakage state *****/
	if(fl_fastfill_state == FALSE)
	{
		if(l_calculate_fuel_value_U16 < l_display_fuel_value_U16)			//SysRS_02_Gauges_10029
		{
			//Transform fuel volume to Led Bar
			l_fuel_bar_level_counts_U16 = fhyst_get_index_with_up_hyst((void *)&F_FUEL_BAR_THRESHOLD[0],(uint16)(l_calculate_fuel_value_U16), l_fuel_bar_level_counts_U16);			
			l_display_fuel_value_U16 = l_calculate_fuel_value_U16;
		}
	}
	/***** fast fuel/leakage state *****/
	else		
	{	
		//Transform fuel volume to Led Bar
		l_fuel_bar_level_counts_U16 = fhyst_get_index_with_up_hyst((void *)&F_FUEL_BAR_THRESHOLD[0],(uint16)(l_calculate_fuel_value_U16), l_fuel_bar_level_counts_U16);		
		l_display_fuel_value_U16 = l_calculate_fuel_value_U16;
	}
	
	Rte_Write_tiSR_FuelLevel_level(l_fuel_bar_level_counts_U16);	
}


/*============================================================================
**
** Function Name:      void ffuel_get_gauge_fuel(void)
**
** Visibility:         
**
** Description:        ffuel get gauge fuel volume
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/
uint16 ffuel_get_calculate_fuel(void)
{
	return l_calculate_fuel_value_U16;
}

/*============================================================================
**
** Function Name:      void ffuel_get_parameter_C_fuel(void)
**
** Visibility:         
**
** Description:        ffuel get gauge fuel volume
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/
uint16 ffuel_get_parameter_C_fuel(void)
{
	return l_store_fuel_value_U16;
}


/*============================================================================
**
** Function Name:      void ffuel_get_parameter_C_fuel(void)
**
** Visibility:         
**
** Description:        ffuel get gauge fuel volume
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/
uint16 ffuel_get_parameter_A_fuel(void)
{
	return l_display_fuel_value_U16;
}

/*============================================================================
**
** Function Name:      void ffuel_get_parameter_display_fuel(void)
**
** Visibility:         
**
** Description:        ffuel get gauge fuel volume
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/
uint16 ffuel_get_parameter_display_fuel(void)
{
	return l_display_fuel_value_U16;
}



/****************************************************************************
Function Name       : ffuel_valid_condition()

Description         : This function will check for battary state

Invocation          : Invoked by fuel03.cfg

Parameters          :  None

Return Value        :  TRUE/FALSE

Critical Section    :  None

******************************************************************************/
boolean ffuel_valid_condition(void)
{
	EBatteryState fl_Battery_state;
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);	
	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
		return FALSE;	
	else
		return TRUE;
}

/*============================================================================
**
** Function Name:      void ffuel_lowfuel_processing(void)
**
** Visibility:         Externally visible via ffuel.h
**
** Description:        Low fuel status routine
**
** Invocation:         Application code
**
** Inputs/Outputs:     Returns the low fuel level status.
**                     TRUE  => Low fuel level true
**                     FALSE => Low fuel level false
**
** Critical Section:   None.
**
**==========================================================================*/
static void ffuel_lowfuel_status(void)
{
    boolean fl_fuel_init_status_bool;

    fl_fuel_init_status_bool = fuel03_init_finished();

    if(FALSE != fl_fuel_init_status_bool)
    {
        /*
        ** Get the percent full value with hysteresis
        ** for driving the display
        */

        /*Check if fuel level for 1st low fuel warning*/
        if(l_display_fuel_value_U16 <= NVM_DTE_1_LFW_ON_Thres_U16)
        {
            /*Fuel is low. set the flag and start the warning*/
            l_LowFuel_Status_1_BOOL = TRUE;
        }
        else if(l_display_fuel_value_U16 >= NVM_DTE_1_LFW_OFF_Thres_U16)
        {
            /*Check if fuel level 1st warning can be turned OFF*/
            l_LowFuel_Status_1_BOOL = FALSE;
        }
        else
        {
            /* No intention of use - defensive practice only*/
        }
    }
    else
    {
        /*Fuel init is not completed till then the maintain the previous state of
        low fuel to resolve DTE issue*/ /* sbuya*/
    }
    
	Rte_Write_ppSR_LowFuel_STATUS(l_LowFuel_Status_1_BOOL); 			//low fuel TT in normal resistor state
}


/*============================================================================
**
** Function Name:      void ffuel_zerofuel_status(void)
**
** Visibility:         Externally visible via ffuel.h
**
** Description:        Low fuel status routine
**
** Invocation:         Application code
**
** Inputs/Outputs:     Returns the low fuel level status.
**                     TRUE  => Low fuel level true
**                     FALSE => Low fuel level false
**
** Critical Section:   None.
**
**==========================================================================*/
boolean ffuel_zerofuel_status(void)
{
    uint16  fl_fuel_percent_U16;
    boolean fl_fuel_init_status_bool;
    boolean	fl_zerofuel_BOOL = FALSE;

    fl_fuel_init_status_bool = fuel03_init_finished();

    if(FALSE != fl_fuel_init_status_bool)
    {
        /*
        ** Get the percent full value with hysteresis
        ** for driving the display
        */
        fl_fuel_percent_U16 = MS_8_BITS(fuel03_get_senderA_PF());
            
		/*Check if fuel level for 1st low fuel warning*/
		if(fl_fuel_percent_U16 == 0)
		{
			/*Fuel is low. set the flag and start the warning*/
			fl_zerofuel_BOOL = TRUE;
		}
		else
		{
			fl_zerofuel_BOOL = FALSE;
		}
    }
    else
    {

    }

    return (fl_zerofuel_BOOL);
}


/*============================================================================
**
** Function Name:      void ffuel_display_oneBar(void)
**
** Visibility:         Externally visible via ffuel.h
**
** Description:        ffuel display oneBar
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/
static void ffuel_display_oneBar(void)
{
	Rte_Write_tiSR_FuelLevel_level(FUEL_ONE_LEDBAR);
}


/*============================================================================
**
** Function Name:      void ffuel_display_zeroBar(void)
**
** Visibility:         Externally visible via ffuel.h
**
** Description:        ffuel display zeroBar
**
** Invocation:         Application code
**
** Inputs/Outputs:     
**                     
**                     
** Critical Section:   None.
**
**==========================================================================*/
static void ffuel_display_zeroBar(void)
{
	Rte_Write_tiSR_FuelLevel_level(FUEL_ZERO_LEDBAR);
}


/****************************************************************************
Function Name       : ffuel_FastFill_Entry_Conditions()

Description         : This function checks for speed < 3 Kmph and valid
                      and returns the status to the FUEL03.cfg

Invocation          : Invoked by fuel03.cfg

Parameters          : None

Return Value        : True: Fast Fill entry conditions are met.
                      False:Fast Fill entry conditions are not met.

Critical Section    : None

******************************************************************************/
boolean ffuel_FastFill_Entry_Conditions(void)
{
	uint32 fl_VehSpdLgt=0;
	uint8 fl_vehicle_speed_nr=0;
	uint8 fl_vehicle_speed_tout=0;
	uint8 	fl_engspeed_signal_nr = 0;
	uint8   fl_engspeed_signal_tout = 0;
    uint32	fl_engspeed_signal_value = 0;
    boolean fl_fastfill_enter_bool=FALSE;

	Rte_Read_rpSR_FuelLevelMdl_TISpdMdl_Send_value(&fl_VehSpdLgt);	
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_vehicle_speed_nr);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_vehicle_speed_tout);

	Rte_Read_rpSR_FuelLevelMdl_TITachMdl_Send_value(&fl_engspeed_signal_value);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&fl_engspeed_signal_nr);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&fl_engspeed_signal_tout);


	if((fl_vehicle_speed_nr == RTE_E_NEVER_RECEIVED) || (fl_vehicle_speed_tout == RTE_E_TIMEOUT) || \
	(fl_engspeed_signal_nr == RTE_E_NEVER_RECEIVED) || (fl_engspeed_signal_tout == RTE_E_TIMEOUT))
    {
    	fl_fastfill_enter_bool = FALSE;
    }
    else
    {
    	if((fl_VehSpdLgt <= FUEL_FAST_FILL_ENTRY_VSP_LIMIT) && (fl_engspeed_signal_value <= FUEL_FAST_FILL_ENTRY_TACH_LIMIT))					//spd < 3km/h
    	{
			fl_fastfill_enter_bool = TRUE;
    	}
    	else
    	{
    		fl_fastfill_enter_bool = FALSE;
    	}
    }

    return(fl_fastfill_enter_bool);
}

void FuelLevelMdl_FuelLevel_NvMNotifyJobFinished_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
}


/*
Description         : This function is used to process vehicle stable state 
					  when vehicle is in stop mode

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void ffuel_average_rad_processing(void)
{
	boolean fl_vehicle_stop_state;
	
	fl_vehicle_stop_state = ffuel_FastFill_Entry_Conditions();
	
	if(fl_vehicle_stop_state == TRUE)
	{
		if(l_fuel_stable_finish != TRUE)
			ffuel_calc_average_rad();
	}
	else
	{
		l_fuel_stable_finish = FALSE;
		l_fuel_cycle_count = 0;
		FuelAvgValue.moving_sample_counter = 0;
		memset(&FuelAvgValue.sample_array, 0, FUEL03_SMPLS_2_AVG * sizeof(uint16));
	}
}


/****************************************************************************
Function Name       : ffuel_calc_average_rad()

Description         : This function is used to calculate vehicle fuel stable value
Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/

static void ffuel_calc_average_rad(void)
{
	uint8   fuel03_x;
	uint32  fuel03_temp32;
	uint16  sample_array_sorted[FUEL03_SMPLS_2_AVG];
	uint16  swap_u16;
	uint8   i, j;
	uint16  fl_fuel_delte;

	if(l_fuel_cycle_count > 3)
	{
		FuelAvgValue.stable_rad = l_fuel_filter_volume_U16;		
		l_fuel_stable_finish = TRUE;
		l_fuel_cycle_count = 0;
		FuelAvgValue.moving_sample_counter = 0;
		memset(&FuelAvgValue.sample_array, 0, FUEL03_SMPLS_2_AVG * sizeof(uint16));
	}

	if(FuelAvgValue.moving_sample_counter == Fuel_TimeOut_5S)		//50 * 100ms 
	{
		(void)memcpy(sample_array_sorted, FuelAvgValue.sample_array, FUEL03_SMPLS_2_AVG * sizeof(uint16));
		
		for( i=0; i < (FUEL03_SMPLS_2_AVG-1); i++ )
			for( j=0; j < (FUEL03_SMPLS_2_AVG-i-1); j++)
			{
				if(sample_array_sorted[j] > sample_array_sorted[j+1])
				{
					swap_u16 = sample_array_sorted[j];
					sample_array_sorted[j] = sample_array_sorted[j+1];
					sample_array_sorted[j+1] = swap_u16;
				}
			}

		fuel03_temp32 = (uint32)0;
		for( fuel03_x= 0; fuel03_x<FUEL03_SMPLS_2_AVG; fuel03_x++ )
		{
			fuel03_temp32 += sample_array_sorted[fuel03_x];
		}

		FuelAvgValue.avg_rad = (uint16)(fuel03_temp32 / FUEL03_SMPLS_2_AVG);
		FuelAvgValue.max_rad = sample_array_sorted[FUEL03_SMPLS_2_AVG-1];
		FuelAvgValue.min_rad = sample_array_sorted[0];
	
		fl_fuel_delte = FuelAvgValue.max_rad - FuelAvgValue.min_rad;
		if(fl_fuel_delte <= 100)				//max - min <= 1L
		{
			FuelAvgValue.stable_rad = FuelAvgValue.avg_rad;
			l_fuel_stable_finish = TRUE;
			l_fuel_cycle_count = 0;
			FuelAvgValue.moving_sample_counter = 0;
			memset(&FuelAvgValue.sample_array, 0, FUEL03_SMPLS_2_AVG * sizeof(uint16));
		}
		else
		{
			l_fuel_cycle_count++;
			memset(&FuelAvgValue.sample_array, 0, FUEL03_SMPLS_2_AVG * sizeof(uint16));
		}
		FuelAvgValue.moving_sample_counter = 0;
	}
	else
	{
		//l_fuel_stable_finish = FALSE;
		if(FuelAvgValue.moving_sample_counter < FUEL03_SMPLS_2_AVG)
		{
			FuelAvgValue.sample_array[FuelAvgValue.moving_sample_counter] = l_fuel_filter_volume_U16;
		}
		FuelAvgValue.moving_sample_counter++;
	}
}


static boolean  ffuel_process_start_to_stop(void)
{
	boolean fl_start_to_stop = FALSE;
		
	if(l_fuel_stable_finish == TRUE)	//vehicle is in stop mode to process stable value
	{
		fl_start_to_stop = TRUE;
		l_fuel_stable_finish = FALSE;
		l_fuel_start_calc_avg = FALSE;
	}
	else
	{
		fl_start_to_stop = FALSE;
	}

	return fl_start_to_stop;
}


static void ffuel_check_first_battOff_On(void)
{
	if(l_first_battOff_On == FALSE)
	{
		l_first_battOff_On = TRUE;
		l_fuel_bar_level_counts_U16 = 8;
	}
}


static void fuel_test(void)
{
	 test_fuel_res = l_fuel_filter_resistance_U16/100;
	 test_unfuel = l_fuel_unfilter_volume_U16;
	 test_calfuel = l_calculate_fuel_value_U16;
	 test_disfuel = l_display_fuel_value_U16;
	 test_stable_fuel = FuelAvgValue.stable_rad;
}



/*End of File*/
/*****************************************************************************
*   for each change to this file, be sure to record:                         *
*      1.  who made the change and when the change was made                  *
*      2.  why the change was made and the intended result                   *
*   Following block needs to be repeated for each change
******************************************************************************
*   Note: In the traceability column we need to trace back to the Design Doc.*
*   For the initial version it is traced to the Design Document section.     *
*   For further changes it shall trace to the source of the change which may *
*   be SPSS/SCR/Defect details(Defect may be Testing/validation defect)/Any  *
*   other reason                                                            *
******************************************************************************/
