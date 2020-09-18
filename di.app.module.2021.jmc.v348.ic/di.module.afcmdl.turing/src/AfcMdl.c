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
#include "Rte_AFCMdl.h"
#include "CmpLib.h"
#include "AfcMdl_cfg.h"
#include "Mcu.h"
#include "Rte_AFCMdl_Type.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(AFCMdl)

static void fafc_common_init(void);
static void fafc_single_init(void);
static void fafc_accumulate_init(void);
static void fafc_calc_single_afc(uint64 f_distance_value_U64, uint64 f_fuel_value_U64);
static void fafc_calc_accumulate_afc(uint64 f_distance_value_U64, uint64 f_fuel_value_U64);
static uint64 fafc_calc_Fuel_in_ml(uint64 history_fuel_consum_total, uint64 lastFuel, uint64 newFuel);
static uint64 fafc_calc_Distance_in_m(uint64 historyODO, uint64 preODO, uint64 newODO);
static void fafc_ignOn_process(void);
static void fafc_check_idle_status(void);
static uint16 fafc_get_loop_refresh_cycle(void);
static void fafc_can_output(void);
static void fafc_writeValue_to_HMI(void);
static void fafc_calc_init_RAFC(void);
static void fafc_check_first_Battary(void);


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, and resolution                            *
******************************************************************************/
/*count value to indicate a 10s timer*/
static uint16 l_afc_calc_single_count_U16;
static uint16 l_afc_calc_accumulate_count_U16;

/*last fuel consum used to calculate l_afc_fuel_single_consum_total*/
static uint64	l_fuel_consum_signal_value_pre;
/*last odo value used to calute l_afc_odo_single_total*/
static uint64	l_odo_value_pre;

/*indicator of waitting fuel and odo from IGN OFF to IGN ON*/
static boolean l_wait_fuelsig_from_ign_on;
static boolean	l_wait_odo_from_ign_on;

/*indicator of waitting fuel and odo from clear AFC*/
static boolean	l_wait_fuelsig_from_clear;
static boolean  l_wait_odo_from_clear;

/*previus IGN status*/
static uint8 l_pre_ign_status;

/*AFC is cleared*/
static boolean l_AFC_Single_Cleared;
static boolean l_AFC_Accumulate_Cleared;
static boolean l_afc_idle_status;
static boolean l_afc_idle_status_in_30S;


#pragma ghs section bss=".buram"
static uint16  l_afc_RAFC;
/*first time of Battery ON*/
static boolean l_first_Battery_Single_On;
static boolean l_first_Battery_Accumulate_On;
/*AFC display value, resolution 0.1*/
static uint64 l_afc_single_value;
static uint64 l_afc_accumulate_value;
static uint64 l_afc_pre_single_value;
static uint64 l_afc_pre_accumulate_value;
/*A(k) and B(k) in Formula : AFC_D (k) =A (k)* AFC_C (k) + B (k)* AFC_E   zoom 1000 */
static sint32 l_stored_A_single_value;
static sint32 l_stored_B_single_value;				//raw value is 100%
static sint32 l_stored_A_accumulate_value;
static sint32 l_stored_B_accumulate_value;			//raw value is 100%
static uint16 l_afc_num;
/*odo value used to calculate AFC*/
static uint64	l_afc_odo_single_total;
static uint64	l_afc_odo_accumulate_total;
/*fuel value used to calculate AFC*/
static uint64	l_afc_fuel_single_consum_total;
static uint64	l_afc_fuel_accumulate_consum_total;
static boolean  l_afc_abnormal_state;
#pragma ghs section bss=default

#define TEST_AFC

uint16 test_single_afc;
uint32 test_single_odo;
uint32 test_single_fuel;
uint16 test_single_A;
uint16 test_single_B;
uint16 test_accumulate_afc;
uint32 test_accumulate_odo;
uint32 test_accumulate_fuel;

static void test_afc(void);


/*============================================================================
**
** Function Name	:   void fafc_common_init(void)
**
** Visibility		:   This function is initialize varibles
**                      
**
** Description		:    initialize all the varibles
**
** Invocation		:    system up
**
** Inputs/Outputs	:   
**
**
** Parameters       : 	None.
**
** Return Type      : 	None.
** 
**Critical Section  : 	None.
**==========================================================================*/
static void fafc_common_init(void)
{	
	l_wait_fuelsig_from_clear = FALSE;
	l_wait_odo_from_clear = FALSE;	
	l_wait_fuelsig_from_ign_on = TRUE;
	l_wait_odo_from_ign_on = TRUE;
	l_pre_ign_status = eIGN_OFF;	
	l_afc_idle_status = FALSE;
	l_afc_idle_status_in_30S = FALSE;
}

static void fafc_single_init(void)
{	
	l_AFC_Single_Cleared = FALSE;
	l_afc_calc_single_count_U16 = 0;		
}

static void fafc_accumulate_init(void)
{	
	l_AFC_Accumulate_Cleared = FALSE;
	l_afc_calc_accumulate_count_U16 = 0;	
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional initialization state
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpInit( void )
{			
	fafc_common_init();
	fafc_single_init();
	fafc_accumulate_init();
	//fafc_check_battary_state();
	
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
	fafc_common_init();	
	fafc_single_init();
	fafc_accumulate_init();	
	//fafc_check_battary_state();
	
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
static Std_ReturnType CmpActive( void )
{			
	eIgnState fl_IGN_state=eIGN_OFF;
	EBatteryState fl_Battery_state=0;
	
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);	
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{

	}
	else
	{
		if( fl_IGN_state == eIGN_RUN )
		{
			fafc_ignOn_process();
		}
		else
		{
			/* IGN OFF, Stop AFC Fuel consumption Calculation */
			//fafc_ignOff_process();
		}		
		l_pre_ign_status = fl_IGN_state;
	}

	fafc_can_output();

	fafc_writeValue_to_HMI();	

#ifdef TEST_AFC
	test_afc();
#endif
	
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

/*============================================================================
**
** Function Name:       void fafc_ignOn_process(void)
**
** Visibility:          This function is called by the DI-kernel every 10 ms.
**                      it is called as if it were an interrupt
**
** Description:         This function will calculate the ifc.
**
** Invocation:          This function is called by the DI-Kernel every 10 ms
**                      
**
** Inputs/Outputs:      NVRam constants used in this function must be correct!
**
** Critical Section:    none
**==========================================================================*/
static void fafc_ignOn_process(void)
{
	uint8 	fl_engspeed_signal_nr=0;
	uint8   fl_engspeed_signal_tout=0;
	uint8 	fl_fuel_consum_signal_nr=0;
	uint8 	fl_fuel_consum_signal_tout=0;
    uint64	fl_fuel_consum_signal_value=0;
    uint64	fl_odo_value=0;
	uint8 	fl_ABS_WhlMil_nr=0;
	uint8 	fl_ABS_WhlMil_tout=0;
	uint8   fl_WhlMilgFrntRiStatus=0;
	uint8   fl_WhlMilgFrntLeStatus=0;

	uint32 fl_engspeed_signal_value=0;
	Rte_Read_rpSR_AFCMdl_TITachMdl_Send_value(&fl_engspeed_signal_value);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&fl_engspeed_signal_nr);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&fl_engspeed_signal_tout);

	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_Fuelcounter(&fl_fuel_consum_signal_value);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(&fl_fuel_consum_signal_nr);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(&fl_fuel_consum_signal_tout);

	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRiStatus(&fl_WhlMilgFrntRiStatus);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLeStatus(&fl_WhlMilgFrntLeStatus);

	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_ABS_WhlMil_nr);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_ABS_WhlMil_tout);

	fafc_check_first_Battary();

	if( l_pre_ign_status != eIGN_RUN )
	{
		/* IGN OFF -> IGN ON Processing */		
		/* 1. Reset first fuel consum signal value */
		l_wait_fuelsig_from_ign_on = TRUE;
		l_wait_odo_from_ign_on = TRUE;
	}

	//SysRS_03_LCD_Routine_1128
	if( (fl_ABS_WhlMil_nr == RTE_E_NEVER_RECEIVED) || (fl_ABS_WhlMil_tout == RTE_E_TIMEOUT) || ((fl_WhlMilgFrntRiStatus == AFC_INVALID_STATUS) && (fl_WhlMilgFrntLeStatus == AFC_INVALID_STATUS)) ||\
	 (fl_fuel_consum_signal_nr == RTE_E_NEVER_RECEIVED) || (fl_fuel_consum_signal_tout == RTE_E_TIMEOUT) )
	{			
		/* ABS_Sts Signal Missing, Stop AFC Fuel consumption Calculation */
		l_afc_single_value = AFC_INVALID_VALUE;
		l_afc_accumulate_value = AFC_INVALID_VALUE;
		l_afc_abnormal_state = TRUE;
	}
	else
	{
		if((fl_engspeed_signal_nr == RTE_E_NEVER_RECEIVED) || (fl_engspeed_signal_tout == RTE_E_TIMEOUT))		
		{			
			/* EngineSpeed Signal Missing, Stop AFC Fuel consumption Calculation , and display previous AFC value*/
			l_afc_single_value = l_afc_pre_single_value;
			l_afc_accumulate_value = l_afc_pre_accumulate_value;
		}
		else
		{			
			if(l_afc_abnormal_state == TRUE)
			{
				l_afc_single_value = l_afc_pre_single_value;
				l_afc_accumulate_value = l_afc_pre_accumulate_value;
				l_afc_abnormal_state = FALSE;
			}
			else
			{					
				/*check engine status whether is idle or not*/
				fafc_check_idle_status();
				
				/*fuel or odo missing, stop calculate AFC*/
				if(l_afc_idle_status == FALSE)
				{					
					/* Calcaluation Fuel Consumption */
					if( (l_wait_fuelsig_from_ign_on == TRUE) || (l_wait_fuelsig_from_clear == TRUE) )
					{
						l_wait_fuelsig_from_ign_on = FALSE;
						l_fuel_consum_signal_value_pre = fl_fuel_consum_signal_value;											
						l_wait_fuelsig_from_clear = FALSE;
					}
					else
					{
						l_afc_fuel_single_consum_total = fafc_calc_Fuel_in_ml(l_afc_fuel_single_consum_total, l_fuel_consum_signal_value_pre, fl_fuel_consum_signal_value); 																						
						l_afc_fuel_accumulate_consum_total = fafc_calc_Fuel_in_ml(l_afc_fuel_accumulate_consum_total, l_fuel_consum_signal_value_pre, fl_fuel_consum_signal_value); 							
						l_fuel_consum_signal_value_pre = fl_fuel_consum_signal_value;
					}
						
					/* Calculation Odo */
					fl_odo_value = Rte_Read_rpSR_AfcMdl_OdoValue(ODO_1M);
					if( (l_wait_odo_from_ign_on == TRUE) || (l_wait_odo_from_clear == TRUE ) )
					{
						if(fl_odo_value != FODO_ODO_NVM_ERROR) 
						{
							l_odo_value_pre = fl_odo_value;
							
							l_wait_odo_from_ign_on = FALSE;
							l_wait_odo_from_clear = FALSE;
						}
						else
						{
							/* until wait for valid ODO Data	*/
						}	
					}
					else
					{
						if(fl_odo_value != FODO_ODO_NVM_ERROR) 
						{
							l_afc_odo_single_total = fafc_calc_Distance_in_m(l_afc_odo_single_total, l_odo_value_pre, fl_odo_value);
							l_afc_odo_accumulate_total = fafc_calc_Distance_in_m(l_afc_odo_accumulate_total, l_odo_value_pre, fl_odo_value);
							l_odo_value_pre = fl_odo_value;
						}
					}
					
					/* Calculation AFC */
					l_afc_calc_single_count_U16++;
					l_afc_calc_accumulate_count_U16++;
					fafc_calc_single_afc(l_afc_odo_single_total, l_afc_fuel_single_consum_total);
					fafc_calc_accumulate_afc(l_afc_odo_accumulate_total, l_afc_fuel_accumulate_consum_total);
				}				
				
			}											
			l_afc_pre_single_value = l_afc_single_value;
			l_afc_pre_accumulate_value = l_afc_accumulate_value;
		}				
	}
	
}

/*============================================================================
**
** Function Name:      uint64  fafc_calc_Fuel_in_ml()
**
** Visibility:         fafc.h
**
** Description:        calculate fuel consumption value
**
** Invocation:         called by other subsystem components
**                     which need calculate fuel consumption value
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   AFC value
**
** Critical Section:
**
** Created:            02/04/2019 by sli34
**
**==========================================================================*/

static uint64 fafc_calc_Fuel_in_ml(uint64 history_fuel_consum_total, uint64 lastFuel, uint64 newFuel)
{
	uint64 new_fuel_consum_total=0;
	if( newFuel < lastFuel )
	{
	  //new_fuel_consum_total = history_fuel_consum_total + ((uint64)(65535 - lastFuel + newFuel)*AFC_CALC_CONST_VALUE) >> AFC_CALC_MOVE_VALUE;
	  new_fuel_consum_total = history_fuel_consum_total + ((uint64)(65535 + newFuel - lastFuel)*AFC_CALC_FACTOR) /1000;
	}
	else
	{
	  /* Fuel Consumption = History Fuel + Delta			  */
	  /* Delta = ( new - last ) * FACTOR					  */
	  new_fuel_consum_total = history_fuel_consum_total + \
							  (((uint64)newFuel * AFC_CALC_CONST_VALUE) >> AFC_CALC_MOVE_VALUE) - \
							  (((uint64)lastFuel * AFC_CALC_CONST_VALUE) >> AFC_CALC_MOVE_VALUE);
	}

	return new_fuel_consum_total;
}

/*============================================================================
**
** Function Name:      uint64  fafc_calc_Distance_in_m()
**
** Visibility:         fafc.h
**
** Description:        calculate odo distance value
**
** Invocation:         called by other subsystem components
**                     which need calculate odo distance value
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   AFC value
**
** Critical Section:
**
** Created:            02/04/2019 by sli34
**
**==========================================================================*/

static uint64 fafc_calc_Distance_in_m(uint64 historyODO, uint64 preODO, uint64 newODO)
{
	uint32 new_ODO_Total=0;
	
	if( newODO >= preODO )
	{
		new_ODO_Total = historyODO + newODO - preODO;
	}
	else
	{
		/* Do nothing	*/
		new_ODO_Total = historyODO;
	}
	
	return new_ODO_Total;
}

/*============================================================================
**
** Function Name:      void  fafc_calc_single_afc()
**
** Visibility:         fafc.h
**
** Description:        calculate single AFC value
**
** Invocation:         called by other subsystem components
**                     which need calculate single AFC value
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   AFC value
**
** Critical Section:
**
** Created:            02/04/2019 by sli34
**
**==========================================================================*/

static void fafc_calc_single_afc(uint64 f_distance_value_U64, uint64 f_fuel_value_U64)
{
	uint32 fl_AFC_CK=0;

    if(l_afc_calc_single_count_U16 >= AFC_NO_IDLE_LOOP_NUMBER)
	{	
	    
		if((f_distance_value_U64 >= MAX_D_VALUE) || (f_distance_value_U64 == 0) || (f_fuel_value_U64 >= MAX_F_VALUE)  )
		//if(f_distance_value_U64 == 0)
		{		 
			l_afc_calc_single_count_U16 = 0;
			return;
		}
		/*****************************
		*** the unit of fl_FK: (mL) ********
		*** the unit of fl_DK: (m)  *******
		*** the unit of fl_AFC_CK:  mL/km ***/
		fl_AFC_CK =  1000 * f_fuel_value_U64 / f_distance_value_U64;
		if(fl_AFC_CK > AFC_MAX_CALC_VALUE)
		{
			fl_AFC_CK = AFC_MAX_CALC_VALUE;
		}
		//fl_AFC_CK = (fl_AFC_CK * AFC_DISPLAY_PERCENT) / 100;
		
		/******************************************************
		** AFC_D (k) =A (k)* AFC_C (k) + B (k)* AFC_E
		** AFC_D  unit: 0.1 L/100km = 1 mL/km
		** AFC_C unit:  mL/m
		** AFC_E unit:   mL/10km
		*/

        if(l_first_Battery_Single_On == TRUE)
		{
		    if(f_distance_value_U64 < AFC_DISTANCE_300M)		//SysRS_03_LCD_Routine_464
			{
			    l_afc_single_value = AFC_INVALID_VALUE;
			}
			else
			{
			    l_first_Battery_Single_On = FALSE;
				l_afc_single_value = ((l_stored_A_single_value * fl_AFC_CK) + ((l_afc_RAFC * l_stored_B_single_value)/10))/1000;
			}
		}
		else if(l_AFC_Single_Cleared == TRUE)
		{
		    if(f_distance_value_U64 < AFC_DISTANCE_300M)		//SysRS_03_LCD_Routine_464
			{
			    l_afc_single_value = AFC_INVALID_VALUE;
			}
			else
			{
				l_AFC_Single_Cleared = FALSE;
				l_afc_single_value = ((l_stored_A_single_value * fl_AFC_CK) + ((l_afc_RAFC * l_stored_B_single_value)/10))/1000;
			}
		}
		else
		{
			if(fl_AFC_CK != 0)
		    {
		        l_stored_A_single_value += NVM_AFC_C_VALUE;
		        l_stored_B_single_value -= NVM_AFC_C_VALUE;
		    }
		    if(l_stored_A_single_value >= MAX_A_VALUE)
		    {
		        l_stored_A_single_value = MAX_A_VALUE;
		    }
		    if(l_stored_B_single_value <= MIN_B_VALUE)
		    {
		        l_stored_B_single_value = MIN_B_VALUE;
		    }
		    
		    l_afc_single_value = ((l_stored_A_single_value * fl_AFC_CK) + ((l_afc_RAFC * l_stored_B_single_value)/10))/1000;
		}
		l_afc_calc_single_count_U16 = 0;
	}
	else
	{
	    /*do nothing*/
	}
}


/*============================================================================
**
** Function Name:      uint32  fafc_calc_accumulate_afc()
**
** Visibility:         fafc.h
**
** Description:        calculate accumulate AFC value
**
** Invocation:         called by other subsystem components
**                     which need calculate accumulate AFC value
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   AFC value
**
** Critical Section:
**
** Created:            02/04/2019 by sli34
**
**==========================================================================*/
static void fafc_calc_accumulate_afc(uint64 f_distance_value_U64, uint64 f_fuel_value_U64)
{
	uint32 fl_AFC_CK=0;

    if(l_afc_calc_accumulate_count_U16 >= AFC_NO_IDLE_LOOP_NUMBER)
	{	
		if((f_distance_value_U64 >= MAX_D_VALUE) || (f_distance_value_U64 == 0) || (f_fuel_value_U64 >= MAX_F_VALUE)  )
		//if(f_distance_value_U64 == 0)
		{
		    /*D(k) or F(k) is bigger than max value, should keep current AFC display*/
			l_afc_calc_accumulate_count_U16 = 0;
			return;
		}
		/*****************************
		*** the unit of fl_FK: (uL) ********
		*** the unit of fl_DK: (m)  *******
		*** the unit of fl_AFC_CK:  mL/km ***/
		fl_AFC_CK =  1000 * f_fuel_value_U64 / f_distance_value_U64;
		if(fl_AFC_CK > AFC_MAX_CALC_VALUE)
		{
			fl_AFC_CK = AFC_MAX_CALC_VALUE;
		}
		//fl_AFC_CK = (fl_AFC_CK * AFC_DISPLAY_PERCENT) / 100;
		
		/******************************************************
		** AFC_D (k) =A (k)* AFC_C (k) + B (k)* AFC_E
		** AFC_D  unit: 0.1 L/100km = 1 mL/km
		** AFC_C unit:  mL/m
		** AFC_E unit:   mL/10km
		*/

        if(l_first_Battery_Accumulate_On == TRUE)
		{
		    if(f_distance_value_U64 < AFC_DISTANCE_300M)
			{
			    l_afc_accumulate_value = AFC_INVALID_VALUE;
			}
			else
			{
			    l_first_Battery_Accumulate_On = FALSE;
				l_afc_accumulate_value = ((l_stored_A_accumulate_value * fl_AFC_CK) + ((l_afc_RAFC * l_stored_B_accumulate_value)/10))/1000;
			}
		}
		else if(l_AFC_Accumulate_Cleared == TRUE)
		{
		    if(f_distance_value_U64 < AFC_DISTANCE_300M)
			{
			    l_afc_accumulate_value = AFC_INVALID_VALUE;
			}
			else
			{
				l_AFC_Accumulate_Cleared = FALSE;
				l_afc_accumulate_value = ((l_stored_A_accumulate_value * fl_AFC_CK) + ((l_afc_RAFC * l_stored_B_accumulate_value)/10))/1000;
			}
		}
		else
		{
			if(fl_AFC_CK != 0)
		    {
		        l_stored_A_accumulate_value += NVM_AFC_C_VALUE;
		        l_stored_B_accumulate_value -= NVM_AFC_C_VALUE;
		    }
		    if(l_stored_A_accumulate_value >= MAX_A_VALUE)
		    {
		        l_stored_A_accumulate_value = MAX_A_VALUE;
		    }
		    if(l_stored_B_accumulate_value <= MIN_B_VALUE)
		    {
		        l_stored_B_accumulate_value = MIN_B_VALUE;
		    }
		    
		    l_afc_accumulate_value = ((l_stored_A_accumulate_value * fl_AFC_CK) + ((l_afc_RAFC * l_stored_B_accumulate_value)/10))/1000;
		}
		l_afc_calc_accumulate_count_U16 = 0;
	}
	else
	{
	    /*do nothing*/
	}
}


/*============================================================================
**
** Function Name:      uint32  fafc_get_single_value()
**
** Visibility:         fafc.h
**
** Description:        Get the value of the main AFC
**
** Invocation:         called by other subsystem components
**                     which need the ifc value
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   AFC value
**
** Critical Section:
**
** Created:            02/04/2019 by sli34
**
**==========================================================================*/
uint32 fafc_get_single_value(void)
{
    return l_afc_single_value;
}

/*============================================================================
**
** Function Name:      void fafc_clear_single_value()
**
** Visibility:         fafc.h
**
** Description:        clear AFC value by press button and calculate new
**
** Invocation:         called by other subsystem components
**                     which need clear AFC value
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   NULL
**
** Critical Section:
**
** Created:            02/04/2019 by sli34
**
**==========================================================================*/
void fafc_clear_single_value(void)
{
	l_afc_fuel_single_consum_total = 0;
	l_afc_odo_single_total = 0;
	
	l_wait_fuelsig_from_clear = TRUE;
	l_wait_odo_from_clear = TRUE;

	l_AFC_Single_Cleared = TRUE;
	l_stored_A_single_value = 0;
	l_stored_B_single_value = 1000;
	
	l_afc_single_value = AFC_INVALID_VALUE;
	l_afc_calc_single_count_U16 = 0;

	fafc_calc_init_RAFC();
}



/*============================================================================
**
** Function Name:      uint32  fafc_get_accumulate_value()
**
** Visibility:         fafc.h
**
** Description:        Get the value of the main AFC
**
** Invocation:         called by other subsystem components
**                     which need the ifc value
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   AFC value
**
** Critical Section:
**
** Created:            02/04/2019 by sli34
**
**==========================================================================*/
uint32 fafc_get_accumulate_value(void)
{
	return l_afc_accumulate_value;
}

/*============================================================================
**
** Function Name:      void fafc_clear_acc_value()
**
** Visibility:         fafc.h
**
** Description:        clear AFC value by press button and calculate new
**
** Invocation:         called by other subsystem components
**                     which need clear AFC value
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   NULL
**
** Critical Section:
**
** Created:            02/04/2019 by sli34
**
**==========================================================================*/
void fafc_clear_acc_value(void)
{
	l_afc_fuel_accumulate_consum_total = 0;
	l_afc_odo_accumulate_total = 0;
	
	l_wait_fuelsig_from_clear = TRUE;
	l_wait_odo_from_clear = TRUE;

	l_AFC_Accumulate_Cleared = TRUE;
	l_stored_A_accumulate_value = 0;
	l_stored_B_accumulate_value = 1000;
	
	l_afc_accumulate_value = AFC_INVALID_VALUE;
	
	l_afc_calc_accumulate_count_U16 = 0;

}

/*============================================================================
**
** Function Name:      void fafc_check_idle_status()
**
** Visibility:         AfcMdl_cfg.h
**
** Description:        check engine status whether is idle or not
**
** Invocation:         called by other subsystem components
**                     which need check engine status
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   NULL
**
** Critical Section:
**
** Created:            02/04/2020 by sli34
**
**==========================================================================*/
static void fafc_check_idle_status(void)
{
	uint8 fl_timer_resDef_started_bool=0;
	uint8 fl_timer_resDef_isElapsed_bool=0;
    uint32	fl_engspeed_signal_value=0;    
	uint16  fl_ABS_VehSpdLgt=0;
	
	Rte_Read_rpSR_AFCMdl_TITachMdl_Send_value(&fl_engspeed_signal_value);
	Rte_Read_rpSR_TISpdMdl_value(&fl_ABS_VehSpdLgt);

	if(fl_engspeed_signal_value < AFC_TACH_300RPM)		//Defect:tach=0,stop refresh afc
	{
		l_afc_idle_status = TRUE;						//stop calc fuel consumption
		Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerAFCWait, &fl_timer_resDef_started_bool); 
		if(fl_timer_resDef_started_bool == TRUE)			
			Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAFCWait);
	}
	else
	{
		if(fl_ABS_VehSpdLgt < AFC_SPEED_3KM)
		{
			Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerAFCWait, &fl_timer_resDef_started_bool); 
			if(fl_timer_resDef_started_bool)
			{
				// abnormal >= 300s
				Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerAFCWait, AFC_TimeOut_300S, &fl_timer_resDef_isElapsed_bool);
				if(fl_timer_resDef_isElapsed_bool)
				{
					l_afc_idle_status = TRUE;					//stop calc fuel consumption
				}
				else
				{	
					l_afc_idle_status = FALSE;
				}
			}
			else
			{
				Rte_Call_rpTimer_TmExt_Timer_Start(eTimerAFCWait);
			}
		}
		else
		{
			l_afc_idle_status = FALSE;
			Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerAFCWait, &fl_timer_resDef_started_bool); 
			if(fl_timer_resDef_started_bool == TRUE)			
				Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAFCWait);
		}
	}
}

/*============================================================================
**
** Function Name:      void fafc_get_loop_refresh_cycle()
**
** Visibility:         AfcMdl_cfg.h
**
** Description:        get loop refresh cycle 
**
** Invocation:         called by other subsystem components
**                     which need get refresh cycle 
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   NULL
**
** Critical Section:
**
** Created:            02/04/2019 by sli34
**
**==========================================================================*/
static uint16 fafc_get_loop_refresh_cycle(void)
{
	uint16 fl_afc_loop_number = AFC_NO_IDLE_LOOP_NUMBER;
	
	if(l_afc_idle_status_in_30S == TRUE)
		fl_afc_loop_number = AFC_IDLE_LOOP_NUMBER;			//refresh cycle is 30s
	else
		fl_afc_loop_number = AFC_NO_IDLE_LOOP_NUMBER;		//refresh cycle is 10s
	return fl_afc_loop_number;
}	

/*============================================================================
**
** Function Name:      void fafc_can_output()
**
** Visibility:         AfcMdl_cfg.h
**
** Description:        output can value by RTE 
**
** Invocation:         called by other subsystem components
**                     which need output can value 
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   NULL
**
** Critical Section:
**
** Created:            02/04/2019 by sli34
**
**==========================================================================*/
static void fafc_can_output(void)
{
	if(l_afc_accumulate_value == AFC_INVALID_VALUE)
		Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_AFC(0x1ff);
	else
		Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_AFC(l_afc_accumulate_value);
}


/*============================================================================
**
** Function Name:      void fafc_writeValue_to_HMI()
**
** Visibility:         AfcMdl_cfg.h
**
** Description:        write value to HMI 
**
** Invocation:         called by other subsystem components
**                     which need write value to HMI
**                     NOTE: 
**
** Inputs/Outputs:     inputs
**                     units        -   specified units and scaling
**                  
**                     return value -   NULL
**
** Critical Section:
**
** Created:            02/04/2019 by sli34
**
**==========================================================================*/
static void fafc_writeValue_to_HMI(void)
{
	//transmit l_afc_single_value / l_afc_accumulate_value to HMI By RTE 
	Rte_Write_ppSR_AfcMdl_AfcVal_AfcSingleVal(l_afc_single_value);
	Rte_Write_ppSR_AfcMdl_AfcVal_AccumulateVal(l_afc_accumulate_value);
}

static void fafc_calc_init_RAFC(void)
{
	l_afc_RAFC = AFC_GAS_RAFC;			//Gasoline/7000RPM

#if 0
	boolean IsEngineCfg=0;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Engine, &IsEngineCfg);
	if(IsEngineCfg)
		l_afc_RAFC = AFC_GAS_RAFC;			//Gasoline/7000RPM
	else
	    l_afc_RAFC = AFC_DIESEL_RAFC;		//Diesel/5000RPM
#endif
}

static void fafc_check_first_Battary(void)
{
	l_afc_num++;
	if(l_afc_num == 1)			//first battary
	{
		l_stored_A_single_value = 0;
		l_stored_B_single_value = 1000;				//raw value is 100%		
		l_first_Battery_Single_On = TRUE;
		l_afc_single_value = AFC_INVALID_VALUE;
		l_afc_pre_single_value = AFC_INVALID_VALUE;
		l_stored_A_accumulate_value = 0;
		l_stored_B_accumulate_value = 1000;			//raw value is 100%
		l_first_Battery_Accumulate_On = TRUE;		
		l_afc_accumulate_value = AFC_INVALID_VALUE;
		l_afc_pre_accumulate_value = AFC_INVALID_VALUE;
		l_afc_odo_single_total = 0;
		l_afc_odo_accumulate_total = 0;
		l_afc_fuel_single_consum_total = 0;
		l_afc_fuel_accumulate_consum_total = 0;
		l_afc_abnormal_state = FALSE;
		fafc_calc_init_RAFC();
	}

	if(l_afc_num == 100)
		l_afc_num = 10;
}


/****************************************************************************
Function Name       : test_afc

Description         : the Function is used to display AFC TEST PAGE

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void test_afc(void)
{
	test_single_afc = l_afc_single_value;
	test_single_A = l_stored_A_single_value;
	test_single_B = l_stored_B_single_value;
	test_single_odo = l_afc_odo_single_total;
	test_single_fuel = l_afc_fuel_single_consum_total;
	
	test_accumulate_afc = l_afc_accumulate_value;
	test_accumulate_odo = l_afc_odo_accumulate_total;
	test_accumulate_fuel = l_afc_fuel_accumulate_consum_total;
}


