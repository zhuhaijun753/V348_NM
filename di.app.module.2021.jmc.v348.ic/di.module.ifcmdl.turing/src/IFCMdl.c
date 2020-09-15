/*****************************************************************************
*                                                                            *
*              CONFIDENTIAL VISTEON CORPORATION                              *
*                                                                            *
* This is an unpublished work of authorship, which contains trade            *
* secrets, created in 2003. Visteon Corporation owns all rights to           *
* this work and intends to maintain it in confidence to preserve             *
* its trade secret status. Visteon Corporation reserves the right,           *
* under the copyright laws of the United States or those of any              *
* other country that may have jurisdiction, to protect this work             *
* as an unpublished work, in the event of an inadvertent or                  *
* deliberate unauthorized publication. Visteon Corporation also              *
* reserves its rights under all copyright laws to protect this               *
* work as a published work, when appropriate. Those having access            *
* to this work may not copy it, use it, modify it or disclose the            *
* information contained in it without the written authorization              *
* of Visteon Corporation.                                                    *
*                                                                            *
******************************************************************************/
/*****************************************************************************

File Name        :  IFCMdl.c
Module Short Name:  IFCMdl
VOBName          :  \sw_di_int\2010_nissan_x32l_ic
Author           :  lizhao
Description      :  This module drives the Instantaneous Fuel Consumption application as per the specific 
                    requirements of the Program

Organization     :  Driver Information Software Section,
                    Visteon Software Operation, India
                    Visteon Corporation

----------------------------------------------------------------------------
Compiler Name    : cosmic.v477 
******************************************************************************/
//#define FIFC_C
/*****************************************************************************
*                                 System Includes                            *
******************************************************************************/
/*****************************************************************************
*                                 Project Includes                           *
******************************************************************************/
#include "Rte.h"
#include "Rte_type.h"
#include "Rte_IFCMdl.h"
#include "CmpLib.h"
#include "IFCMdl_cfg.h"


CMPLIB_INSTANCE(IFCMdl)


/*****************************************************************************
*                                 Type Declarations                          *
******************************************************************************/

/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, and resolution                            *
******************************************************************************/
static uint32 l_fuel_ifc[MAX_FUEL_IFC_COUNT];
static uint8  l_idle_ifc_count_U8;/*for update*/
static uint8  l_non_idle_ifc_count_U8;/*for update*/
static uint8  l_to_idle_ifc_trans_count_U8;/*for transition*/
static uint8  l_to_non_idle_ifc_trans_count_U8;/*for transition*/
static uint8  l_current_vehicle_status_U8;
static uint32 l_pre_Fuel_consum_idle_value_U32;
static uint32 l_pre_Fuel_consum_nonidle_value_U32;
static uint32 l_pre_odo_nonidle_value_U32;
static uint32 l_pre_Fuel_consum_value_U32;
static uint32 l_pre_odo_value_U32;
static boolean IsEngineCfg;

/*****************************************************************************
*                                 Type Decleration                           *
*----------------------------------------------------------------------------*
* Decleration of type shall be accompanied by a comment that explains the    *
* purpose and usage of the type.                                             *
******************************************************************************/
static void IFCMdl_init(void);
static void IFCMdl_KSSlowTimeTask(void);
//static boolean IFCMdl_get_idle_status(void);
static void IFCMdl_calc_idle_value(uint32 p_pre_fuel_value, uint32 p_cur_fuel_value);
static void IFCMdl_calc_non_idle_value(uint32 p_pre_fuel_value, uint32 p_cur_fuel_value, uint32 p_pre_odo_value, uint32 p_cur_odo_value);
static void IFCMdl_stop_calc_IFC_value(void);
static void IFCMdl_reset_all_timer(void);
static void IFCMdl_display_ifc_value(void);
static void IFCMdl_calculate_ifc_value(uint32 fl_cur_Fuel_consum_value_U32);
static void IFCMdl_process_transition_idle_to_noidle(uint16 fl_current_vehicle_speed_U16, uint32  fl_cur_Fuel_consum_value_U32);
static void IFCMdl_calc_update_counter(void);

/*============================================================================
** M E M O R Y   A L L O C A T I O N
**==========================================================================*/


/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional initialization state
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpInit( void )
{	
    IFCMdl_init();
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
    IFCMdl_init();
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
	eIgnState fl_IGN_state;
	EBatteryState fl_Battery_state;
	
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);	
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{
		IFCMdl_reset_all_timer();
	}
	else
	{
		if( fl_IGN_state == eIGN_RUN )
		{
			IFCMdl_KSSlowTimeTask();
			
			IFCMdl_display_ifc_value();
		}
		else
		{

		}
	}

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
** E N T R Y   P O I N T S  /  D A T A   A C C E S S   S E R V I C E S
**==========================================================================*/
/*============================================================================
**
** Function Name:       void IFCMdl_init(void)
**
** Visibility:          This function is externally visible via the
**                      IFCMdl.h header file.
**
** Description:         initialize of IFC value
**
** Invocation:          When Igntion RUN_START is detected
**                      Called from PM_Transition_Into_RUN (see power_PS.c)
**
** Inputs/Outputs:      
**
**
** Parameters       : 	None.
**
** Return Type      : 	None.
** 
**Critical Section  : 	None.
**==========================================================================*/
static void IFCMdl_init(void)
{
    l_fuel_ifc[VEHICLE_STATUS_IDLE] = IFC_INVALID_VALUE;
	l_fuel_ifc[VEHICLE_STATUS_NON_IDLE] = IFC_INVALID_VALUE;
	l_idle_ifc_count_U8 = 0;
    l_non_idle_ifc_count_U8 = 0;
    l_to_idle_ifc_trans_count_U8 = 0;
    l_to_non_idle_ifc_trans_count_U8 = 0;
	l_current_vehicle_status_U8 = VEHICLE_STATUS_IDLE;

    l_pre_Fuel_consum_value_U32 = 0;
	l_pre_odo_value_U32 = 0;

    l_pre_Fuel_consum_idle_value_U32 = 0;
    l_pre_Fuel_consum_nonidle_value_U32 = 0;
	l_pre_odo_nonidle_value_U32 = 0;
	IsEngineCfg = TRUE;
}

/*============================================================================
**
** Function Name:       void IFCMdl_KSSlowTimeTask(void)
**
** Visibility:          This function is called by the DI-kernel every 10 ms.
**                      it is called as if it were an interrupt
**
** Description:         This function will calculate the ifc.
**
** Invocation:          This function is called by the DI-Kernel every 10 ms
**                      (see sched.cfg)
**
** Inputs/Outputs:      NVRam constants used in this function must be correct!
**
** Critical Section:    none
**==========================================================================*/
static void IFCMdl_KSSlowTimeTask(void)
{
    boolean fl_currSpeed_invalid_BOOL = FALSE;
	uint16 fl_current_vehicle_speed_U16;
	boolean fl_cur_WhlMilgFrntStatus_invalid_BOOL = FALSE;
	uint8 fl_cur_WhlMilgFrntLeStatus_value_U8;
	uint8 fl_cur_WhlMilgFrntRiStatus_value_U8;
	boolean fl_cur_Fuel_consum_invalid_BOOL = FALSE;
	uint32 fl_cur_Fuel_consum_value_U32;
	uint8 fl_fuel_consum_signal_nr;
	uint8 fl_fuel_consum_signal_tout;
	uint8 fl_ABS_WhlMil_nr;
	uint8 fl_ABS_WhlMil_tout;

	//Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Engine, &IsEngineCfg);
	
	IsEngineCfg = FALSE;				//Default is Gasline 
	
	if(IsEngineCfg == FALSE)		//Diesel/5000RPM
	{
		Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_Fuelcounter(&fl_cur_Fuel_consum_value_U32);
	}
	else							//Gasoline/7000RPM
	{
		Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_FuelconsumptionInst(&fl_cur_Fuel_consum_value_U32);
	}
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(&fl_fuel_consum_signal_nr);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(&fl_fuel_consum_signal_tout);
	
	Rte_Read_rpSR_TISpdMdl_value(&fl_current_vehicle_speed_U16);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRiStatus(&fl_cur_WhlMilgFrntRiStatus_value_U8);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLeStatus(&fl_cur_WhlMilgFrntLeStatus_value_U8);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_ABS_WhlMil_nr);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_ABS_WhlMil_tout);


	if((fl_fuel_consum_signal_nr == RTE_E_NEVER_RECEIVED) || (fl_fuel_consum_signal_tout == RTE_E_TIMEOUT))
	{
		fl_cur_Fuel_consum_invalid_BOOL = TRUE;
	}

	
	if((fl_ABS_WhlMil_nr == RTE_E_NEVER_RECEIVED) || (fl_ABS_WhlMil_tout == RTE_E_TIMEOUT))
	{
		fl_cur_WhlMilgFrntStatus_invalid_BOOL = TRUE;
		fl_currSpeed_invalid_BOOL = TRUE;
	}


	if((fl_cur_WhlMilgFrntStatus_invalid_BOOL != FALSE) ||
	  (fl_cur_Fuel_consum_invalid_BOOL != FALSE) ||
	  (fl_cur_Fuel_consum_value_U32 == 0xFFFF) ||
	  ((fl_cur_WhlMilgFrntLeStatus_value_U8 == 1) && (fl_cur_WhlMilgFrntRiStatus_value_U8 == 1)))
	{
	    IFCMdl_stop_calc_IFC_value();
	}
	else
	{

		/* 
		** if the vehicle speed message is missing, invalid and never Received clear all varient
		*/
			
		if (FALSE == fl_currSpeed_invalid_BOOL)
		{
			IFCMdl_process_transition_idle_to_noidle(fl_current_vehicle_speed_U16, fl_cur_Fuel_consum_value_U32);
		}
		else
		{
			IFCMdl_calc_update_counter();
		}

		IFCMdl_calculate_ifc_value(fl_cur_Fuel_consum_value_U32);

     }
}

/*============================================================================
**
** Function Name:      void IFCMdl_calc_idle_value(uint32 p_pre_fuel_value, uint32 p_cur_fuel_value)
**
** Visibility:         IFCMdl.h
**
** Description:        Get the  IFC value
**
** Invocation:         called by calculate function to calculate idle value
**
** Inputs/Outputs:     inputs
**                    
**
** Critical Section:
**
** Created:            
**
**==========================================================================*/
static void IFCMdl_calc_idle_value(uint32 p_pre_fuel_value, uint32 p_cur_fuel_value)
{
	uint32 fl_dt_F;

	if(p_cur_fuel_value != IFC_FUEL_INVALID_VALUE)
	{
        /****************************************************
        	    ** IFC(k) = (3600/1000000)*[ï¿½ï¿½F'(k)/ ï¿½ï¿½T(k)]
                ** the unit of ï¿½ï¿½F'(k) is 0.01mL, the unit of ï¿½ï¿½T(k) is s
                ** to implement 0.1 resolution IFC(k) = [ï¿½ï¿½F'(k)/ ï¿½ï¿½T(k)] * 36/1000
               	** IFC shall be displayed with unit of â€œL/hâ€ and resolution of 0.1L/h in non-idle status.
        	***********************************************/
		if(IsEngineCfg == FALSE)			//Diesel/5000RPM
		{
			if(p_cur_fuel_value >= p_pre_fuel_value)
			{
				fl_dt_F = ((uint64)((uint64)(p_cur_fuel_value - (uint64)p_pre_fuel_value) * IFC_CALC_CONST_VALUE) >> IFC_CALC_MOVE_VALUE);
			}
			else
			{
			   fl_dt_F = ((((uint64)(IDC_FUEL_MAX_VALUE*IFC_CALC_CONST_VALUE)) >> IFC_CALC_MOVE_VALUE) + (((uint64)p_cur_fuel_value * IFC_CALC_CONST_VALUE) >> IFC_CALC_MOVE_VALUE) - (((uint64)p_pre_fuel_value * IFC_CALC_CONST_VALUE) >> IFC_CALC_MOVE_VALUE));
			}
			
			l_fuel_ifc[VEHICLE_STATUS_IDLE] = fl_dt_F * 36 /100 /10;
		}
		else
		{
			fl_dt_F = p_cur_fuel_value * IFC_CALC_FACTOR_Gasoline / 100 ;
			l_fuel_ifc[VEHICLE_STATUS_IDLE] = fl_dt_F * 36 /100;
		}
		
		      
	}
	else
	{
	    l_fuel_ifc[VEHICLE_STATUS_IDLE] = IFC_INVALID_VALUE;
	}	
}

/*============================================================================
**
** Function Name:      void IFCMdl_calc_idle_value(void)
**
** Visibility:         IFCMdl.h
**
** Description:        Get the  IFC value
**
** Invocation:         called by calculate function to calculate none idle value
**
** Inputs/Outputs:     inputs
**                    
**
** Critical Section:
**
** Created:            
**
**==========================================================================*/
static void IFCMdl_calc_non_idle_value(uint32 p_pre_fuel_value, uint32 p_cur_fuel_value, uint32 p_pre_odo_value, uint32 p_cur_odo_value)
{
	uint64 fl_dt_F;
	uint64 fl_dt_D;
	uint16 fl_ifc_value;

	/**********************************************************************
	**	  ï¿½ï¿½F(k)= [2*Fuel_consumption_accumulation (k)- 
	**						2*Fuel_consumption_accumulation(k-1)+131070*OF (k)]   (ul)
	**	  
	************************************************************************/

	if((p_pre_fuel_value != IFC_FUEL_INVALID_VALUE) &&
		(p_cur_fuel_value != IFC_FUEL_INVALID_VALUE) &&
		(p_pre_odo_value != FODO_ODO_NVM_ERROR) &&
		(p_cur_odo_value != FODO_ODO_NVM_ERROR))
	{
		if(IsEngineCfg == FALSE)			//Diesel/5000RPM
		{
			/**********************************************************************
			**	  ï¿½ï¿½F(k)= [2*Fuel_consumption_accumulation (k)- 
			**						2*Fuel_consumption_accumulation(k-1)+131070*OF (k)]   (ul)
			**	  
			************************************************************************/
			if(p_cur_fuel_value >= p_pre_fuel_value)
			{
			   fl_dt_F = (((uint64)p_cur_fuel_value - (uint64)p_pre_fuel_value) * IFC_CALC_CONST_VALUE) >> IFC_CALC_MOVE_VALUE;
			}
			else
			{
			   fl_dt_F = (((uint64)IDC_FUEL_MAX_VALUE * IFC_CALC_CONST_VALUE) >> IFC_CALC_MOVE_VALUE) + (((uint64)p_cur_fuel_value * IFC_CALC_CONST_VALUE) >> IFC_CALC_MOVE_VALUE) - (((uint64)p_pre_fuel_value * IFC_CALC_CONST_VALUE) >> IFC_CALC_MOVE_VALUE);
			}
		}
		else
		{
			/**********************************************************************
			**	  ï¿½ï¿½F(k)= FuelconsumptionInst_N  *0.05/1000000		(L/100ms)
			**	  
			************************************************************************/
			fl_dt_F = p_cur_fuel_value * IFC_CALC_FACTOR_Gasoline / 100 ;
		}

		fl_dt_D = OdoLogicalMdl_get_odo_for_ifc();
		
        if(fl_dt_D > 0)
        {
        	 /**********************************************************
        		   **  IFC unit: 0.1 L/100km = mL/km
        		   **  ¡÷F(k) unit: uL
        		   **  ¡÷D(k) unit: m
        		   ** IFC(k) = 100*¡÷F(k)/ ¡÷D(k)
        		   ** to implement 0.1 resolution IFC(k) = 1000*¡÷F(k)/ ¡÷D(k)
        		   ** l_fuel_ifc[VEHICLE_STATUS_NON_IDLE] = 1000*fl_dt_F*10/fl_dt_D/1000;
        		  ***********************************************************/
        	 fl_ifc_value = 1000*fl_dt_F/fl_dt_D /100;
        	 l_fuel_ifc[VEHICLE_STATUS_NON_IDLE] = fl_ifc_value;				//hmi_menu display / 10
        }
		else
		{
		    l_fuel_ifc[VEHICLE_STATUS_NON_IDLE] = IFC_INVALID_VALUE;
		}
	}
	else
	{
	    l_fuel_ifc[VEHICLE_STATUS_NON_IDLE] = IFC_INVALID_VALUE;
	}
}

/*============================================================================
**
** Function Name:      void IFCMdl_calculate_ifc_value(void)
**
** Visibility:         IFCMdl.h
**
** Description:        Calculate the  IFC value
**
** Invocation:         called by calculate function to calculate ifc value
**
** Inputs/Outputs:     inputs
**                    
**
** Critical Section:
**
** Created:            
**
**==========================================================================*/
static void IFCMdl_calculate_ifc_value(uint32 fl_cur_Fuel_consum_value_U32)
{
   uint32 fl_current_odo_U32;

	if((l_current_vehicle_status_U8 == VEHICLE_STATUS_IDLE)&&(l_idle_ifc_count_U8 > IFC_LOOP_NUMBER))/*idle mode update 2s timer time out*/
	{
		IFCMdl_calc_idle_value(l_pre_Fuel_consum_value_U32, fl_cur_Fuel_consum_value_U32);/*calc idle mode ifc*/
		l_idle_ifc_count_U8 = 0;/*reset update 2s timer */
	} 
	else if((l_current_vehicle_status_U8 == VEHICLE_STATUS_NON_IDLE)&&(l_non_idle_ifc_count_U8 > IFC_LOOP_NUMBER))
	{
		fl_current_odo_U32 = Rte_Read_rpSR_AfcMdl_OdoValue(ODO_1M);
		IFCMdl_calc_non_idle_value(l_pre_Fuel_consum_value_U32, fl_cur_Fuel_consum_value_U32, l_pre_odo_value_U32, fl_current_odo_U32);/*calc non idle mode ifc*/
		l_pre_odo_value_U32 = fl_current_odo_U32;
	
		l_non_idle_ifc_count_U8 = 0;/*reset update 2s timer */
	}	  
}

/*============================================================================
**
** Function Name:      void IFCMdl_display_ifc_value(void)
**
** Visibility:         IFCMdl.h
**
** Description:        Get ifc value and Write ifc value to RTE
**
** Invocation:         called by period function if meet correct condation
**
** Inputs/Outputs:     inputs
**                    
**
** Critical Section:
**
** Created:            
**
**==========================================================================*/
static void IFCMdl_display_ifc_value(void)
{
	uint8 display_status_U8;
	uint32 fl_ifc_value;
	
    if(l_current_vehicle_status_U8 == VEHICLE_STATUS_IDLE)
	{
	    fl_ifc_value = l_fuel_ifc[VEHICLE_STATUS_IDLE];
	    display_status_U8 = VEHICLE_STATUS_IDLE;
	}
	else
	{
	    fl_ifc_value = l_fuel_ifc[VEHICLE_STATUS_NON_IDLE];
        display_status_U8 = VEHICLE_STATUS_NON_IDLE;
	}

	Rte_Write_ppSR_IFCMdl_IFCVal_IFCVal(fl_ifc_value);
	Rte_Write_rpSR_IFCMdl_IFCStatus_IFCStatus(display_status_U8);
}


/*============================================================================
**
** Function Name:      void IFCMdl_process_transition_idle_to_noidle(uint16 fl_current_vehicle_speed_U16, uint32  fl_cur_Fuel_consum_value_U32)
**
** Visibility:         IFCMdl.h
**
** Description:        transite the  state of ifc from idle to non-idle
**
** Invocation:         called by calculate function to transite the  state of ifc
**
** Inputs/Outputs:     inputs
**                    
**
** Critical Section:
**
** Created:            
**
**==========================================================================*/
static void IFCMdl_process_transition_idle_to_noidle(uint16 fl_current_vehicle_speed_U16, uint32  fl_cur_Fuel_consum_value_U32)
{
	uint32 fl_current_odo_U32;
	
	if(l_current_vehicle_status_U8 == VEHICLE_STATUS_IDLE)/*in idle mode*/
	{	
		l_to_idle_ifc_trans_count_U8 = 0;/*in idle mode transition to idle timer should always 0*/
		l_non_idle_ifc_count_U8 = 0;
		if(fl_current_vehicle_speed_U16 >= FSPEED_3KMH)/*speed more than or equal to 3KM/H*/
		{	
			if(l_to_non_idle_ifc_trans_count_U8 == IFC_LOOP_NUMBER)/*record previous nonidle fuel consum*/
			{
				l_pre_Fuel_consum_nonidle_value_U32 = fl_cur_Fuel_consum_value_U32;
				l_pre_odo_nonidle_value_U32 = Rte_Read_rpSR_AfcMdl_OdoValue(ODO_1M);
			}  
			//l_to_non_idle_ifc_trans_count_U8++;
			if(l_to_non_idle_ifc_trans_count_U8 >= IFC_LOOP_NUMBER * 2)/*transition to non idle 2S timer time out*/
			{
				IFCMdl_reset_all_timer();
				l_current_vehicle_status_U8 = VEHICLE_STATUS_NON_IDLE;/*goto non idle mode*/
				fl_current_odo_U32 = Rte_Read_rpSR_AfcMdl_OdoValue(ODO_1M);
				IFCMdl_calc_non_idle_value(l_pre_Fuel_consum_nonidle_value_U32, fl_cur_Fuel_consum_value_U32, l_pre_odo_nonidle_value_U32, fl_current_odo_U32);
				/*calc none idle mode ifc*/
			}						
			
			l_to_non_idle_ifc_trans_count_U8++;
		}
		else/*speed less than 3km/h*/
		{
			l_to_non_idle_ifc_trans_count_U8 = 0;/*reset transition to non idle 2s timer*/	
			if(l_idle_ifc_count_U8 == 0)
			{
			   l_pre_Fuel_consum_value_U32 = fl_cur_Fuel_consum_value_U32;
			}
			l_idle_ifc_count_U8++;
		}				
	}
	else if(l_current_vehicle_status_U8 == VEHICLE_STATUS_NON_IDLE)/*in non idle mode */
	{
		l_to_non_idle_ifc_trans_count_U8 = 0;/*in non idle mode transition to non idle timer should always 0*/
		l_idle_ifc_count_U8 = 0;
		if(fl_current_vehicle_speed_U16 < FSPEED_3KMH)/*speed less than 3km/h*/
		{
			if(l_to_idle_ifc_trans_count_U8 == IFC_LOOP_NUMBER)	/*record idle previous fuel consum*/
			{
				l_pre_Fuel_consum_idle_value_U32 = fl_cur_Fuel_consum_value_U32;
			}
			
			if(l_to_idle_ifc_trans_count_U8 >= IFC_LOOP_NUMBER * 2)/*transition to idle 2S timer time out*/
			{
				IFCMdl_reset_all_timer();
				l_current_vehicle_status_U8 = VEHICLE_STATUS_IDLE;/*goto non idle mode*/
				IFCMdl_calc_idle_value(l_pre_Fuel_consum_idle_value_U32, fl_cur_Fuel_consum_value_U32);/*calc idle mode ifc*/
			}	
			l_to_idle_ifc_trans_count_U8++;	
		}
		else
		{
			l_to_idle_ifc_trans_count_U8 = 0;/*reset transition to idle 2s timer*/
			if(l_non_idle_ifc_count_U8 == 0)
			{
			   l_pre_Fuel_consum_value_U32 = fl_cur_Fuel_consum_value_U32;
			   l_pre_odo_value_U32 = Rte_Read_rpSR_AfcMdl_OdoValue(ODO_1M);
			}		  
			l_non_idle_ifc_count_U8++;
		}			 
	}			

}

/*============================================================================
**
** Function Name:      void IFCMdl_calc_update_counter(void)
**
** Visibility:         IFCMdl.h
**
** Description:        Calculate the update counter
**
** Invocation:         called by calculate function to calculate the update counter
**
** Inputs/Outputs:     inputs
**                    
**
** Critical Section:
**
** Created:            
**
**==========================================================================*/
static void IFCMdl_calc_update_counter(void)
{
	if(l_current_vehicle_status_U8 == VEHICLE_STATUS_IDLE)
	{
		l_idle_ifc_count_U8++;
	}
	else if(l_current_vehicle_status_U8 == VEHICLE_STATUS_NON_IDLE)
	{
		l_non_idle_ifc_count_U8++;
	}
}




/*============================================================================
**
** Function Name:      void IFCMdl_stop_calc_IFC_value(void)
**
** Visibility:         IFCMdl.h
**
** Description:        stop calculate IFC value if condation not correct
**
** Invocation:         called by period function if meet incorrect condation
**
** Inputs/Outputs:     inputs
**                    
**
** Critical Section:
**
** Created:            
**
**==========================================================================*/
static void IFCMdl_stop_calc_IFC_value(void)
{
    l_idle_ifc_count_U8 = 0;
	l_non_idle_ifc_count_U8 = 0;
    l_to_idle_ifc_trans_count_U8 = 0;
    l_to_non_idle_ifc_trans_count_U8 = 0;
	l_fuel_ifc[VEHICLE_STATUS_NON_IDLE] = IFC_INVALID_VALUE;
	l_fuel_ifc[VEHICLE_STATUS_IDLE] = IFC_INVALID_VALUE;
}

/*============================================================================
**
** Function Name:      void IFCMdl_reset_all_timer(void)
**
** Visibility:         IFCMdl.h
**
** Description:        reset all timer when mode switch
**
** Invocation:         called by period function if meet incorrect condation
**
** Inputs/Outputs:     inputs
**                    
**
** Critical Section:
**
** Created:            
**
**==========================================================================*/
static void IFCMdl_reset_all_timer(void)
{
    l_idle_ifc_count_U8 = 0;
	l_non_idle_ifc_count_U8 = 0;
    l_to_idle_ifc_trans_count_U8 = 0;
    l_to_non_idle_ifc_trans_count_U8 = 0;
}


#if 0
/*============================================================================
**
** Function Name:      boolean IFCMdl_get_idle_status(void)
**
** Visibility:         IFCMdl.h
**
** Description:        Get the idle  statuc of IFC
**
** Invocation:         called by other subsystem components
**                     which need the ifc value
**                     NOTE: if this function returns FALSE,  IFC is in none-idle status
**                     if returns TRUE, IFC is in idle status
**
** Inputs/Outputs:     inputs
**                     value_addr   -   address to put the ifc value
**                     units        -   specified units and scaling
**                  
**                     return value -   TRUE if no error
**                                      FALSE if NVM error
**
** Critical Section:
**
** Created:            9/11/2015 by lizhao
**
**==========================================================================*/
static boolean IFCMdl_get_idle_status(void)
{
    boolean fl_idle_status_BOOL = FALSE;
    if(l_current_vehicle_status_U8 == VEHICLE_STATUS_IDLE)
	{
	    fl_idle_status_BOOL = TRUE;
	}
	return fl_idle_status_BOOL;
}
#endif


/*============================================================================
**
** Function Name:      uint32 IFCMdl_calc_ifc_value(void)
**
** Visibility:         IFCMdl.h
**
** Description:        Get the  IFC value
**
** Invocation:         called by other subsystem components
**                     which need the ifc value
**
** Inputs/Outputs:     inputs
**                     value_addr   -   address to put the ifc value
**                     units        -   specified units and scaling
**                  
**                     return value -   TRUE if no error
**                                      FALSE if NVM error
**
** Critical Section:
**
** Created:           
**
**==========================================================================*/
 uint32 IFCMdl_calc_ifc_value(uint8* display_status_U8)
{
    uint32 fl_ifc_value=0;
    uint16 fl_engspeed_signal_value=0;    
    uint8  fl_engspeed_signal_nr=0;
    uint8  fl_engspeed_signal_tout=0;
    uint8 fl_currSpeed_invalid_BOOL=0;

	Rte_Read_rpSR_TISpdMdl_value(&fl_engspeed_signal_value);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_engspeed_signal_nr);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_engspeed_signal_tout);

	
	if((fl_engspeed_signal_nr == RTE_E_NEVER_RECEIVED) || (fl_engspeed_signal_tout == RTE_E_TIMEOUT))								/* Enging Speed signal is valid */
	{
		fl_currSpeed_invalid_BOOL = TRUE;
	}
    
    if(l_current_vehicle_status_U8 == VEHICLE_STATUS_IDLE)
	{
	    fl_ifc_value = l_fuel_ifc[VEHICLE_STATUS_IDLE];
        if(fl_currSpeed_invalid_BOOL == FALSE)
        {
            if(fl_engspeed_signal_value < FENGSPEED_300RPM)
            {
                *display_status_U8 = IFC_INVALID_DISPLAY;
            }
            else
            {
                *display_status_U8 = IFC_IDLE_DISPLAY;
            }
        }
        else
        {
            *display_status_U8 = IFC_INVALID_DISPLAY;
        }
	}
	else
	{
	    fl_ifc_value = l_fuel_ifc[VEHICLE_STATUS_NON_IDLE];
        *display_status_U8 = IFC_NONE_IDLE_DISPLAY;
	}
	return fl_ifc_value;
}


/*============================================================================
**
** $Revision:   1.4  $
**
**============================================================================
** C L E A R C A S E    R E V I S I O N    N O T E S
**============================================================================
**
** For each change to this file, be sure to record:
** 1.  Who made the change and when the change was made.
** 2.  Why the change was made and the intended result.
**
** CLEARCASE Rev #      Date        By
** CLEARCASE Rev X.X    mm/dd/yy    CDSID
**
**==========================================================================*/
