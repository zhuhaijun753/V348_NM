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
#include "Rte_AmbientMdl.h"
#include "CmpLib.h"
#include "AmbientMdl_cfg.h"
#include "ClockMdl_cfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(AmbientMdl)


/*****************************************************************************
*								Macro Definitions                            *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/
/* Define outside temperature range, -40 to 70*/
#define AMBIENT_RANGE_MIN							(uint8)(10)			/*means -40 */
#define AMBIENT_RANGE_MAX							(uint8)(158)		/*means 70*/
/* Define invalid value in case of can input value invalid
 * This value must NOT larger than 0x7F to avoid conversion error from uint8 to sint8*/
#define AMBIENT_INVALID_VALUE						(uint16)(0xFF)
#define AMBIENT_ENGINE_FACTOR           			(uint16)(75)
#define AMBIENT_ENGINE_OFFSET           			(uint16)(4800)
/* Define macro for fast conversion without offset*/
#define AMBIENT_OFFSET								(sint8)(-40)
#define AMBIENT_ENGINE_WATER_TEMP       			(uint16)(4500)
#define AMBIENT_ENGINE_WATER_DEFAULT    			(uint16)(4600)
#define AMBIENT_VS_2_SPEED               			(uint16)(20000)	/*from fspeed VS multiply by 100, so 20000 = 200km/h*/
#define AMBIENT_VS_1_SPEED               			(uint16)(12000)
#define AMBIENT_VS_055_SPEED             			(uint16)(3500)
#define AMBIENT_VS_011_SPEED             			(uint16)(300)
#define AMBIENT_VS_DEFAULT_SPEED         			(uint16)(4000)				//0.55c/min (40km/h)
#define AMBIENT_VS_2_RATE               			(uint32)(2000000)	//multiply by 1000000
#define AMBIENT_VS_1_RATE               			(uint32)(1000000)
#define AMBIENT_VS_055_RATE             			(uint32)(550000)
#define AMBIENT_VS_011_RATE             			(uint32)(110000)
#define AMBIENT_VS_005_RATE            	 			(uint32)(50000)
#define AMBIENT_UPDATE_RATE_TIME        			(uint32)(2000)		/*1min == 6 * 10000ms = 60000 / 30 ms*/
#define AMBIENT_UPDATE_DEGREE           			(uint32)(1000000)
#define AMBIENT_HOURS_PASSED            			(uint8)(2)
#define AMBIENT_MINUTES_PASSED          			(uint8)(30)
#define AMBIENT_ENGINE_STOP             			(uint8)(5)
#define AMBIENT_ENGINE_RUN              			(uint8)(4)
#define AMBIENT_ENGINE_MOTOR_CRANK      			(uint8)(3)
#define AMBIENT_ENGINE_START_CRANK      			(uint8)(2)
#define AMBIENT_ENGINE_READY            			(uint8)(1)
#define AMBIENT_ENGINE_STANDBY          			(uint8)(0)
#define AMBIENT_UPDATE_3S_TIMER 	    			(uint8)(100)/*100*30ms=3s*/
#define AMBIENT_HOUR_TO_SEC             			(uint16)(3600) /*1h = 3600s*/
#define AMBIENT_MINUTE_TO_SEC           			(uint8)(60) /*1m = 60s*/
#define AMBIENT_DAY_HOURS               			(uint8)(24)
#define AMBIENT_25HOURS_TO_SEC          			(uint16)(9000)
#define AMBIENT_DAY_OFFSET              			(uint8)(1)
#define AMBIENT_ENGINE_TEMP_OFFSET      			(uint8)(48)
#define AMBIENT_UPDATE_2S_TIMER         			(uint8)(67)
#define Ambient_Outside_Temp_Invalid         		(uint16)(255)
#define Ambient_Vehicle_Speed_Invalid        		(uint16)(4095)
#define Ambient_Engine_Temp_Invalid        			(uint16)(255)
#define NVM_AMBIENT_SNOW_ICON_DISPLAY_valide     	(sint8)(-40) 	
#define NVM_AMBIENT_SNOW_ICON_DISPLAY     			(uint8)(4) 	
#define NVM_AMBIENT_SNOW_ICON_DISPLAY_RELIEVE  		(uint8)(7)
#define NVM_AMBIENT_SNOW_ICON_DISPLAY_RELIEVE_valide 		(uint8)(70)		
#define SPEED_DECODE_FACTOR  						(uint8)(90)
#define SPEED_DECODE_SHIFT  						(uint8)(4)           
#define Dim_TimeOut_30S 							(uint32)(30000)
#define START_TIME 						            (uint8)(67)


/*****************************************************************************
*		        Globally  accessed Variable Declarations    				 *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, and resolution                            *
******************************************************************************/

/* Global variable for record first initialize status*/
/* Holds snow indicator state:
 * FALSE: don't display indicator
 * TRUE: display indicator in HMI header*/
static boolean l_outside_snow_indicator_status;

/* Holds current snow indicator state:
 * FALSE: don't display indicator
 * TRUE: display indicator in HMI header*/
static boolean l_cur_outside_snow_indicator_status;

/* Last critical outside temperature, used to check the critical transition*/
static uint8 l_last_outside_critical_value_U8;

/* Record the first initialize status*/
static boolean l_snowout_first_initialize_status;

/*Record first from ign off to on*/
static boolean l_ambient_trans_from_off;

/*Value of temperature update*/
static uint32 l_ambient_temp_update_value;

/*IGN off to on, wait 3s display low value*/
static uint8 l_ambient_update_count;

/* Previous IGN state*/
static uint8 l_ambient_pre_ign_state;
static uint8 l_outside_temp_invalid;
static uint8 l_vehicle_speed_invalid;

/* Ambient counter number*/
static uint16 l_ambient_outside_temp_counter;
static uint16 l_ambient_vehicle_speed_counter;
static boolean l_ambient_abnormal_flag;

#pragma ghs section bss=".buram"
/* Global variable for display outside temperature*/
static sint16 l_outside_temp_value;
static Rtc_TimeType l_ambient_engine_off_time;
static uint8 l_ambient_cur_engine_state;
static uint16 l_ambient_first_battary;
#pragma ghs section bss=default

static uint8 l_start_count = 0;

/*****************************************************************************
*                              Public Functions                              *
******************************************************************************/
void Ambient_process_ignOn (void);
void Ambient_process_ignOff (void);
void Ambient_Outside_Temperature_Task (void);
void Ambient_common_init(void);
void Ambient_check_snow_indicator_status(void);
boolean Ambient_check_engine_off_time(void);
uint32 Ambient_calc_ambient_update_rate(uint16 fl_vehicle_speed_U16);
void Ambient_battaryAbnormal_Processing(void);
boolean Ambient_snow_indicator_status(void);
sint8 Ambient_get_outside_temp(void);
sint16 Ambient_calc_outside_temp(uint16 fl_outside_raw);
void Ambient_Outside_temperature_invalid(uint8 fl_outside_temp);
void Ambient_Vehicle_Speed_invalid(uint16 fl_vehicle_speed);
void AmbientMdl_Process_Outside_Temperature(uint16 fl_outside_temp_raw, uint16 fl_vehicle_speed_value);
void AmbientMDl_Check_First_Battary(void);
boolean AmbientMdl_check_temperValue_is_valid(sint16 fl_outside_temp_value);


/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional initialization state
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpInit( void )
{	
    Ambient_common_init();
	AmbientMDl_Check_First_Battary();
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
	Ambient_common_init();
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
	
	}
	else
	{		
		if((fl_IGN_state == eIGN_RUN) && (l_ambient_pre_ign_state == eIGN_OFF))
		{
			l_ambient_trans_from_off = TRUE;
		}
		else if((fl_IGN_state == eIGN_OFF) && (l_ambient_pre_ign_state == eIGN_RUN))
		{
			/*Record  engine off time*/
			l_ambient_engine_off_time = Clock_Get_CurrentTime();
		    l_snowout_first_initialize_status = TRUE; /*Modify:20200728 CSDID:cpang1*/
		}

		l_ambient_pre_ign_state = fl_IGN_state; 	

		if(fl_IGN_state == eIGN_RUN)
		{
			l_start_count ++;
		}
		else
		{
			l_start_count = 0;
		}	
		if(START_TIME < l_start_count)// no respond to signal after IGN in 2s
		{
			Ambient_Outside_Temperature_Task(); 	
		}
		
	}

	Rte_Write_ppSR_AmbientMdl_Output_OutsideTempValue(l_outside_temp_value);
	Rte_Write_ppSR_AmbientMdl_Output_SnowIndicatorValue(l_outside_snow_indicator_status);
	
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
Function Name     : void Ambient_common_init(void)

Description       : Hmi pay load return fuction to di_info.c

Invocation        : di_info

Parameters        : None.

Return Type       : None.

Critical Section  : None.

******************************************************************************/
void Ambient_common_init(void)
{
    /*initialize default value for snow indicator*/
    l_outside_snow_indicator_status = FALSE;
    l_cur_outside_snow_indicator_status = FALSE;
    l_last_outside_critical_value_U8 = NVM_AMBIENT_SNOW_ICON_DISPLAY_RELIEVE;
    l_snowout_first_initialize_status = TRUE;
    l_ambient_trans_from_off = FALSE;
    l_ambient_temp_update_value = 0;
    l_ambient_update_count =0;
	l_ambient_pre_ign_state = eIGN_OFF;
	l_ambient_outside_temp_counter = 0;
	l_ambient_vehicle_speed_counter = 0;
	l_outside_temp_invalid = FALSE;
	l_vehicle_speed_invalid = FALSE;
	l_ambient_abnormal_flag = TRUE;
	l_start_count = 0;
}


/****************************************************************************
Function Name     : void Ambient_process_ignOn(void)

Description       : This function is used to initlaisse the local variables
                    while Igniton OFF -> ON transistion.

Invocation        : Invoked by the Statemanager 

Parameters        : None

Return Type       : None

Global Variables  : None

External interface: None

Critical Section  : None

******************************************************************************/
void Ambient_process_ignOn (void)
{
    l_ambient_trans_from_off = TRUE;

}


/****************************************************************************
Function Name     : void Ambient_process_ignOff(void)

Description       : To start the engine off time, those variables must
be cleared before being used, every ignition off

Invocation        : Invoked by the Statemanager 

Parameters        : None

Return Type       : None

Global Variables  : None

External interface: None

Critical Section  : None

******************************************************************************/
void Ambient_process_ignOff (void)
{
    l_ambient_trans_from_off = FALSE;
}


/****************************************************************************
Function Name     : void fifc_KSSlowTimeTask(void)

Description       : This function is used to process the ambient

Invocation        : Invoked by the Round Robin List

Parameters        : None

Return Type       : None

Global Variables  : None

External interface: None

Critical Section  : None

******************************************************************************/
void Ambient_Outside_Temperature_Task (void)
{
    uint8 fl_outside_temp_raw;	    
    sint16 fl_outside_temp_value;	
	uint8 fl_outside_temp_nr;
	uint8 fl_outside_temp_tout;
    sint32 fl_engine_temp_value_raw;	
    sint32 fl_engine_temp_value;	
	uint8 fl_engine_temp_nr;
	uint8 fl_engine_temp_tout;	
    uint16 fl_vehicle_speed_raw;	
    uint16 fl_vehicle_speed_value;
	uint8 fl_vehicle_speed_nr;
	uint8 fl_vehicle_speed_tout;
    uint8 fl_engine_status_value;
    boolean fl_engine_off_B;	
    boolean fl_outside_temper_valid;

    /*get outside temperature from CAN signal*/
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_OutsideTemp(&fl_outside_temp_raw);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_NR(&fl_outside_temp_nr);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_Tout(&fl_outside_temp_tout);
    /*get engine water temperature from CAN signal*/
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngCoolanTemp(&fl_engine_temp_value_raw);	
    /*get engine status from CAN signal*/
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSts(&fl_engine_status_value);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&fl_engine_temp_nr);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&fl_engine_temp_tout);    
    /*get vehicle speed from CAN signal*/
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_vehicle_speed_raw);
	Rte_Read_rpSR_TISpdMdl_value(&fl_vehicle_speed_value);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_vehicle_speed_nr);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_vehicle_speed_tout);
	
	Ambient_Outside_temperature_invalid(fl_outside_temp_raw);
	Ambient_Vehicle_Speed_invalid(fl_vehicle_speed_raw);	

    /*once can message missing or input value invalid, set water temperature as more than 45 value*/
	if((fl_engine_temp_nr == RTE_E_NEVER_RECEIVED) || (fl_engine_temp_tout == RTE_E_TIMEOUT) || (fl_engine_temp_value_raw == Ambient_Engine_Temp_Invalid))
	{
		fl_engine_temp_value = AMBIENT_ENGINE_WATER_DEFAULT;
		fl_engine_off_B = FALSE;	/*consider the signal Engine off time<2.5 hour*/
	}
    else
    {
        /*Record engine state and record engine off time*/
        l_ambient_cur_engine_state = fl_engine_status_value;    
		fl_engine_temp_value = (fl_engine_temp_value_raw * AMBIENT_ENGINE_FACTOR - AMBIENT_ENGINE_OFFSET);
    }
    
    /*once can message missing or input value invalid, set vehicle speed 0.55c/min.*/
	if((fl_vehicle_speed_nr == RTE_E_NEVER_RECEIVED) || (fl_vehicle_speed_tout == RTE_E_TIMEOUT) || (l_vehicle_speed_invalid == TRUE))
    {
        fl_vehicle_speed_value = AMBIENT_VS_DEFAULT_SPEED;		//40km/h
    }    
	
    /*once can message missing or input value invalid, set temperature as invalid value*/
	if((fl_outside_temp_nr == RTE_E_NEVER_RECEIVED) || (fl_outside_temp_tout == RTE_E_TIMEOUT) || (l_outside_temp_invalid == TRUE))
    {
        //display "- - °C
        /*if ((fl_outside_temp_nr != RTE_E_NEVER_RECEIVED))
        {
	        if (l_outside_temp_invalid)//Modify Date: 2020/7/8 author:ZhangTing
	        {
				if (fl_outside_temp_raw != Ambient_Outside_Temp_Invalid)
				{
					if (fl_outside_temp_raw <= AMBIENT_RANGE_MIN)
					{
						l_outside_temp_value =  NVM_AMBIENT_SNOW_ICON_DISPLAY_valide;
						l_outside_snow_indicator_status = TRUE;	
					}
					else if (fl_outside_temp_raw >= AMBIENT_RANGE_MAX)
					{
						l_outside_temp_value =  NVM_AMBIENT_SNOW_ICON_DISPLAY_RELIEVE_valide;
						l_outside_snow_indicator_status = FALSE;	
					}
					l_ambient_abnormal_flag = FALSE;
				}
	        }
        }
		else
		{
			if (fl_outside_temp_tout == RTE_E_TIMEOUT)
			{
				l_outside_snow_indicator_status = FALSE;
				l_ambient_abnormal_flag = TRUE;
			}
		}*/
		/*Modify:20200728 CSDID:cpang1*/
		l_outside_snow_indicator_status = FALSE;
		l_ambient_abnormal_flag = TRUE;
		l_ambient_trans_from_off = FALSE;
  		return;
    }
	else
	{		
		l_ambient_abnormal_flag = FALSE;
		if(l_ambient_trans_from_off == TRUE)
		{						
			l_ambient_update_count = 0;
			fl_engine_off_B = Ambient_check_engine_off_time();				
			fl_outside_temp_value = Ambient_calc_outside_temp(fl_outside_temp_raw);
			fl_outside_temper_valid = AmbientMdl_check_temperValue_is_valid(fl_outside_temp_value);

			if(fl_outside_temper_valid == TRUE)
			{
				if(fl_outside_temp_value > l_outside_temp_value)
				{
					/*engine off time process*/
					if(TRUE == fl_engine_off_B)
					{
						/*display new temp*/
						l_outside_temp_value = fl_outside_temp_value;
					}
					else
					{
						/*engine water temperature process*/
						if(fl_engine_temp_value > AMBIENT_ENGINE_WATER_TEMP)	// > 45 degree centigrade
						{
						   /*display store temp value*/
						}
						else
						{
							 /*display new temp*/
							 l_outside_temp_value = fl_outside_temp_value;
						}
					}				 
				}
				else
				{	 
					/*display new temp*/
					l_outside_temp_value = fl_outside_temp_value;
				}
				l_ambient_trans_from_off = FALSE;

			}
		}
		else
		{
			AmbientMdl_Process_Outside_Temperature(fl_outside_temp_raw, fl_vehicle_speed_value);
		}		
	}
}


/****************************************************************************
Function Name     : void AmbientMdl_Process_Outside_Temperature()

Description       : Process Outside Temperature

Parameters        : NA.

Return Type       : void.

Critical Section  : None.

******************************************************************************/
void AmbientMdl_Process_Outside_Temperature(uint16 fl_outside_temp_raw, uint16 fl_vehicle_speed_value)
{
    uint32 fl_update_rate;
	sint16 fl_outside_temp_value;
	boolean fl_outside_temper_valid;

	if((l_ambient_cur_engine_state == AMBIENT_ENGINE_STANDBY) || (l_ambient_cur_engine_state == AMBIENT_ENGINE_READY) ||\
		(l_ambient_cur_engine_state == AMBIENT_ENGINE_RUN) || (l_ambient_cur_engine_state == AMBIENT_ENGINE_STOP))
	{				
		fl_outside_temp_value = Ambient_calc_outside_temp(fl_outside_temp_raw);
		fl_outside_temper_valid = AmbientMdl_check_temperValue_is_valid(fl_outside_temp_value);
		
		if(fl_outside_temper_valid == TRUE)
		{
			/*curse in rise Temperature*/
			if(fl_outside_temp_value > l_outside_temp_value)
			{					
				fl_update_rate = Ambient_calc_ambient_update_rate(fl_vehicle_speed_value);
				l_ambient_temp_update_value +=fl_update_rate;
				if(l_ambient_temp_update_value >= AMBIENT_UPDATE_DEGREE)
				{
					l_outside_temp_value++;
					l_ambient_temp_update_value = 0;
				}
				l_ambient_update_count = 0;
			}
			/*keep same Temperature*/
			else if(fl_outside_temp_value == l_outside_temp_value)
			{
				l_ambient_update_count = 0;
				l_ambient_temp_update_value = 0;
			}
			/*curse in down Temperature*/
			else
			{
				l_ambient_update_count++;
				/*outside temperature less than current wait for 3s update*/
				if(l_ambient_update_count >= AMBIENT_UPDATE_3S_TIMER)
				{
					/*display new temp*/
					l_outside_temp_value = fl_outside_temp_value;
					l_ambient_update_count = 0;
				}				 
				l_ambient_temp_update_value = 0;
			}
			
			/*check snow indicator status*/
			Ambient_check_snow_indicator_status();
		}
	}
}

/****************************************************************************
Function Name     : sint8 Ambient_get_outside_temp(void)

Description       : For CAN output $311 Outside Temperature

Invocation        : 

Parameters        : 

Return Type       : Celsius without offset -40
                     
Critical Section  : None

******************************************************************************/
sint8 Ambient_get_outside_temp(void)
{
    return l_outside_temp_value;
}

/****************************************************************************
Function Name     : boolean Ambient_snow_indicator_status(void)

Description       : Hmi pay load return fuction to di_info.c

Invocation        : di_info

Parameters        : None.

Return Type       : None.

Critical Section  : None.

******************************************************************************/
boolean Ambient_snow_indicator_status(void)
{
    return (l_outside_snow_indicator_status);
}


/****************************************************************************
Function Name     : void Ambient_check_snow_indicator_status(void)

Description       : Hmi pay load return fuction to di_info.c

Invocation        : di_info

Parameters        : None.

Return Type       : None.

Critical Section  : None.

******************************************************************************/
void Ambient_check_snow_indicator_status(void)
{    
    if (l_outside_temp_value < NVM_AMBIENT_SNOW_ICON_DISPLAY )
    {
        /*outside temperature is colder than NVM_SNOW_ICON_DISPLAY_U8, display snow icon*/
        l_cur_outside_snow_indicator_status = TRUE;

        /*update last critical value*/
        l_last_outside_critical_value_U8 = NVM_AMBIENT_SNOW_ICON_DISPLAY;

		/*fix bug of snow icon display when frist IGN ON*/
		if (FALSE != l_snowout_first_initialize_status)
		{
		    l_snowout_first_initialize_status = FALSE;
		}
    }
    else if (l_outside_temp_value > NVM_AMBIENT_SNOW_ICON_DISPLAY_RELIEVE )
    {
        /*outside temperature is warmer than NVM_SNOW_ICON_DISPLAY_RELIEVE_U8, don't display snow icon*/
        l_cur_outside_snow_indicator_status = FALSE;

        /*update last critical value*/
        l_last_outside_critical_value_U8 = NVM_AMBIENT_SNOW_ICON_DISPLAY_RELIEVE;
		/*fix bug of snow icon display when frist IGN ON*/
		if (FALSE != l_snowout_first_initialize_status)
		{
		    l_snowout_first_initialize_status = FALSE;
		}
    }
    else
    {
        /*outside temperature is between NVM_SNOW_ICON_DISPLAY_U8 and NVM_SNOW_ICON_DISPLAY_RELIEVE_U8*/
        if (FALSE != l_snowout_first_initialize_status)
        {
            /*first initialize, temperature is less than NVM_SNOW_ICON_DISPLAY_RELIEVE_U8, display indicator*/
            l_cur_outside_snow_indicator_status = FALSE;//TRUE; /*Modify:20200728 CSDID:cpang1*/

            /*reset first initialize flag*/
            l_snowout_first_initialize_status = FALSE;

            
            /*update last critical value*/                      /*Modify:20200728 CSDID:cpang1*/
            l_last_outside_critical_value_U8 = NVM_AMBIENT_SNOW_ICON_DISPLAY_RELIEVE;//NVM_AMBIENT_SNOW_ICON_DISPLAY;
        }
        else
        {
            /*check last critical outside temperature to decide display or not*/
            if (NVM_AMBIENT_SNOW_ICON_DISPLAY == l_last_outside_critical_value_U8)
            {
                /*temperature changed from icon display, but less than icon display relieve, display indicator*/
                l_cur_outside_snow_indicator_status = TRUE;
            }
            else if (NVM_AMBIENT_SNOW_ICON_DISPLAY_RELIEVE == l_last_outside_critical_value_U8)
            {
                /*temperature changed from icon display relieve, but larger than icon display, don't display indicator*/
                l_cur_outside_snow_indicator_status = FALSE;
            }
            else
            {
                /*should never occurred, nothing to do*/
            }
        }
    }
	l_outside_snow_indicator_status = l_cur_outside_snow_indicator_status;
}


/****************************************************************************
Function Name     : boolean fambient_check_engine_off()

Description       : return engine off time if more than 2.5hs

Invocation        : Ambient_check_engine_off_time

Parameters        : None.

Return Type       : boolean.

Critical Section  : None.

******************************************************************************/
boolean Ambient_check_engine_off_time(void)
{   
	Rtc_TimeType  fl_ambient_time_S;		
    boolean fl_engine_off_B = FALSE;
    uint32 fl_engine_off_time = 0;
    uint32 fl_cur_time = 0;
    uint32 fl_interval_time = 0;

	fl_ambient_time_S = Clock_Get_CurrentTime();
	
    if(fl_ambient_time_S.Date > l_ambient_engine_off_time.Date + AMBIENT_DAY_OFFSET)		/*more than 1 days return ture*/
    {
        fl_engine_off_B = TRUE;
    }
    else if(fl_ambient_time_S.Date == l_ambient_engine_off_time.Date + AMBIENT_DAY_OFFSET)/*equal to 1 day*/
    {
        fl_engine_off_time = l_ambient_engine_off_time.Hour * AMBIENT_HOUR_TO_SEC + l_ambient_engine_off_time.Minute * AMBIENT_MINUTE_TO_SEC + l_ambient_engine_off_time.Second;
        fl_cur_time = (AMBIENT_DAY_HOURS + fl_ambient_time_S.Hour) * AMBIENT_HOUR_TO_SEC + fl_ambient_time_S.Minute * AMBIENT_MINUTE_TO_SEC + fl_ambient_time_S.Second;
        if(fl_cur_time >= fl_engine_off_time)
        {
			fl_interval_time = fl_cur_time - fl_engine_off_time;
        }
        else
        {
            fl_interval_time = 0;
        }
        if(fl_interval_time >= AMBIENT_25HOURS_TO_SEC)/*interval time not less than 2.5h, return true*/
        {
            fl_engine_off_B = TRUE;
        }
        else
        {
            fl_engine_off_B = FALSE;
        }
    }
    else if(fl_ambient_time_S.Date == l_ambient_engine_off_time.Date)/*in the same day*/
    {
        fl_engine_off_time = l_ambient_engine_off_time.Hour * AMBIENT_HOUR_TO_SEC + l_ambient_engine_off_time.Minute * AMBIENT_MINUTE_TO_SEC + l_ambient_engine_off_time.Second;
        fl_cur_time = fl_ambient_time_S.Hour * AMBIENT_HOUR_TO_SEC + fl_ambient_time_S.Minute * AMBIENT_MINUTE_TO_SEC + fl_ambient_time_S.Second;
        if(fl_cur_time >= fl_engine_off_time)
        {
            fl_interval_time = fl_cur_time - fl_engine_off_time;
        }
        else
        {
            fl_interval_time = 0;
        }
        if(fl_interval_time >= AMBIENT_25HOURS_TO_SEC)/*interval time not less than 2.5h, return true*/
        {
            fl_engine_off_B = TRUE;
        }
        else
        {
            fl_engine_off_B = FALSE;
        }
    }
    return fl_engine_off_B;
}


/****************************************************************************
Function Name     : uint32 Ambient_calc_ambient_update_rate(uint16 fl_vehicle_speed_U16)

Description       : calculate degree update rate base on input vehicle speed

Invocation        : Ambient_calc_ambient_update_rate

Parameters        : uint16 fl_vehicle_speed_U16 input vehicle speed.

Return Type       : uint32.

Critical Section  : None.

******************************************************************************/
uint32 Ambient_calc_ambient_update_rate(uint16 fl_vehicle_speed_U16)
{
    uint32 fl_update_value_U32 = 0;
    
    if(fl_vehicle_speed_U16 > AMBIENT_VS_2_SPEED)				//200km/h
    {
        fl_update_value_U32 = AMBIENT_VS_2_RATE/AMBIENT_UPDATE_RATE_TIME;    //3313    
    }
    else if(fl_vehicle_speed_U16 > AMBIENT_VS_1_SPEED)			//120km/h
    {
        fl_update_value_U32 = AMBIENT_VS_1_RATE/AMBIENT_UPDATE_RATE_TIME;	//1666
    }
    else if(fl_vehicle_speed_U16 > AMBIENT_VS_055_SPEED)		//35km/h
    {
        fl_update_value_U32 = AMBIENT_VS_055_RATE/AMBIENT_UPDATE_RATE_TIME;
    }
    else if(fl_vehicle_speed_U16 > AMBIENT_VS_011_SPEED)		//3km/h
    {
        fl_update_value_U32 = AMBIENT_VS_011_RATE/AMBIENT_UPDATE_RATE_TIME;
    }
    else
    {
        fl_update_value_U32 = AMBIENT_VS_005_RATE/AMBIENT_UPDATE_RATE_TIME;
    }
    
    return fl_update_value_U32;
}


/****************************************************************************
Function Name     : void Ambient_battaryAbnormal_Processing()

Description       : Proecess battary state is Abnormal or not

Parameters        : NA.

Return Type       : NA.

Critical Section  : None.

******************************************************************************/
void Ambient_battaryAbnormal_Processing(void)
{
	l_outside_temp_value = Ambient_calc_outside_temp(AMBIENT_INVALID_VALUE);
	l_outside_snow_indicator_status = FALSE;
}


/****************************************************************************
Function Name     : void Ambient_calc_outside_temp()

Description       : Calculate Outside Temperature

Parameters        : uint16.

Return Type       : sint16.

Critical Section  : None.

******************************************************************************/
sint16 Ambient_calc_outside_temp(uint16 fl_outside_raw)
{
	#define abs(x) (x>0? x:-x)
	sint16 fl_outside_temperature;
	fl_outside_temperature = (fl_outside_raw * AMBIENT_ENGINE_FACTOR - AMBIENT_ENGINE_OFFSET);
    // vvv ===================================================
	    if(abs(fl_outside_temperature) % 100 >= 50)
	    {
	    	if (fl_outside_temperature < 0)
	    	{
	        	fl_outside_temperature = fl_outside_temperature - 100;
	    	}
			else
			{
				fl_outside_temperature = fl_outside_temperature + 100;
			}
	    }
    // ^^^ ===================================================
	fl_outside_temperature = fl_outside_temperature / 100;
	#undef abs
	return fl_outside_temperature;
}


/****************************************************************************
Function Name     : void Ambient_Outside_temperature_invalid()

Description       : Check Outside temperature is invalid or not

Parameters        : NA.

Return Type       : void.

Critical Section  : None.

******************************************************************************/
void Ambient_Outside_temperature_invalid(uint8 fl_outside_temp)
{
	if(fl_outside_temp == Ambient_Outside_Temp_Invalid)		//0xff
	{
		l_ambient_outside_temp_counter++;
		if(l_ambient_outside_temp_counter >= AMBIENT_UPDATE_2S_TIMER)
		{
			l_outside_temp_invalid = TRUE;
			l_ambient_abnormal_flag = TRUE;
			l_outside_temp_value = Ambient_Outside_Temp_Invalid;
			l_outside_snow_indicator_status = FALSE;
			l_ambient_outside_temp_counter = 0;
		}
	}
	/*else if ((fl_outside_temp <= AMBIENT_RANGE_MIN ) || ( fl_outside_temp >= AMBIENT_RANGE_MAX) )
	{
		l_outside_temp_invalid = TRUE;
		l_ambient_outside_temp_counter = 0;
	}*/ 
	else
	{
		l_outside_temp_invalid = FALSE;
		l_ambient_outside_temp_counter = 0;
	}

}


/****************************************************************************
Function Name     : void Ambient_Vehicle_Speed_invalid()

Description       : Check Vehicle Speed is invalid or not

Parameters        : NA.

Return Type       : void.

Critical Section  : None.

******************************************************************************/

void Ambient_Vehicle_Speed_invalid(uint16 fl_vehicle_speed)
{
	if(fl_vehicle_speed == Ambient_Vehicle_Speed_Invalid)
	{
		l_ambient_vehicle_speed_counter++;
		if(l_ambient_vehicle_speed_counter >= AMBIENT_UPDATE_2S_TIMER)
		{
			l_vehicle_speed_invalid = TRUE;
			l_ambient_vehicle_speed_counter = 0;
		}
	}
	else
	{
		l_vehicle_speed_invalid = FALSE;		
		l_ambient_vehicle_speed_counter = 0;
	}
}


/****************************************************************************
Function Name     : void AmbientMDl_Check_First_Battary()

Description       : Check battary state wherther is first Battary off to Battary on

Parameters        : NA.

Return Type       : void.

Critical Section  : None.

******************************************************************************/
void AmbientMDl_Check_First_Battary(void)
{
	l_ambient_first_battary++;
	if(l_ambient_first_battary == 1)
	{
		l_outside_temp_value = AMBIENT_INVALID_VALUE;		//first battary on
	}
	if(l_ambient_first_battary == 100)
		l_ambient_first_battary = 10;
}


/****************************************************************************
Function Name     : boolean HmiGetOutsideTempStatus()

Description       : return Outside Temperature Status

Parameters        : NA.

Return Type       : boolean.

Critical Section  : None.

******************************************************************************/
boolean HmiGetOutsideTempStatus(void)
{
	return l_ambient_abnormal_flag;
}


/****************************************************************************
Function Name     : boolean AmbientMdl_check_temperValue_is_valid()

Description       : if Outside temperature is in range return TRUE;else return FALSE

Parameters        : fl_outside_temp_value.

Return Type       : boolean.

Critical Section  : None.

******************************************************************************/
boolean AmbientMdl_check_temperValue_is_valid(sint16 fl_outside_temp_value)
{
	boolean fl_outside_temp_valid = TRUE;
	#if 0  /*Modify:20200728 CSDID:cpang1*/
	if(fl_outside_temp_value < NVM_AMBIENT_SNOW_ICON_DISPLAY_valide || fl_outside_temp_value > NVM_AMBIENT_SNOW_ICON_DISPLAY_RELIEVE_valide)
	{
		l_cur_outside_snow_indicator_status = FALSE;
	    l_ambient_abnormal_flag = FALSE;//Modify Date: 2020/6/3 author:ZhangTing
		fl_outside_temp_valid = FALSE;
	}
	#endif
	return fl_outside_temp_valid;
}


void AmbientMdl_MPCDateTime_NvMNotifyJobFinished_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
    
}

/*---------------------------------------------------------------------------
Release Label     : Initial Version
Date              : 17-Oct-2016
By                : rkathira
Traceability      : Clock_2016.9.27.doc
Change Description: Initial Release
-----------------------------------------------------------------------------*/

