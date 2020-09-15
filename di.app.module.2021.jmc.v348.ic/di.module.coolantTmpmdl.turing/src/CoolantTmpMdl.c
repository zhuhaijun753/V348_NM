/******************************************************************************
              CONFIDENTIAL VISTEON CORPORATION

 This is an unpublished work of authorship, which contains trade secrets,
 created in 2012. Visteon Corporation owns all rights to this work and
 intends to maintain it in confidence to preserve its trade secret status.
 Visteon Corporation reserves the right, under the copyright laws of the
 United States or those of any other country that may have  jurisdiction,
 to protect this work as an unpublished work, in the event of an
 inadvertent or deliberate unauthorized publication. Visteon Corporation
 also reserves its rights under all copyright laws to protect this work as
 a published work, when appropriate. Those having access to this work may
 not copy it, use it, modify it or disclose the information contained in
 it without the written authorization of Visteon Corporation
******************************************************************************/
/*****************************************************************************

File Name        :  ftemp.c
Module Short Name:  ftemp
VOBName          :  2015_MnM_U301_IC
Author           :  sgajendr
Description      :  Implemented Temp bar as per requirements

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  IAR
Target Processor :  RL78
******************************************************************************/

#include "Rte_CoolantTmpMdl.h"
#include "CmpLib.h"
#include "fhyst.h"
#include "CoolantTmpMdl_cfg.h"


CMPLIB_INSTANCE(CoolantTmpMdl);

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
/*****************************************************************************
*                                 Macro Definitions                          *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/

#define TEMP_CORRECT_RESOLUTION   				(75)
#define TEMP_CORRECT_OFFSET   					(4800)
#define TEMP_INVALID_CAN_VALUE   				(0xFF)
/* unit in 0.01 centigrade degree */
#define ENG_TEMPERATURE_TABLE_OFFSET			(4800)		
#define ENG_TEMPERATURE(v)  					(ENG_TEMPERATURE_TABLE_OFFSET+(v))
#define HighTemp_TT_ICON_NO_DISPLAY 			ENG_TEMPERATURE(11000)
#define HighTemp_TT_ICON_DISPLAY  				ENG_TEMPERATURE(11500)
#define Coolant_TimeOut_2S         				(2000)
#define IOC_FE05_GAUGE_TYPE_TEMP				(0x80)
#define TEMP_LED_BAR 							(uint8)(8)


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/
static sint32 l_filter_eng_clnt_temp_S32;
static sint32 l_pre_filter_eng_clnt_temp_S32;
static eIgnState l_pre_IGN_state;
static uint16 l_indicated_eng_clnt_temp_counts_U16;
static uint16 l_coolant_display_temp;
static uint8 l_cur_hightemp_tt_indicator_status;
static uint16 l_last_hightemp_tt_critical_value_U16;
static uint8 l_hightemp_tt_first_initialize_status;
static uint8 l_hightemp_tt_indicator_status;
static boolean l_value_invalid;

static const THRESHOLD F_TEMP_BAR_THRESHOLD[9] =
{
	{ (uint16)8, (uint16)(0) },												    /*the count of LED BAR*/
	{ (uint16)(ENG_TEMPERATURE(4800)), (uint16)(ENG_TEMPERATURE(5000)) },		/*1 LED BAR*/
	{ (uint16)(ENG_TEMPERATURE(6000)), (uint16)(ENG_TEMPERATURE(6200)) },		/*2 LED BAR*/
	{ (uint16)(ENG_TEMPERATURE(7200)), (uint16)(ENG_TEMPERATURE(7400)) },		/*3 LED BAR*/
	{ (uint16)(ENG_TEMPERATURE(8300)), (uint16)(ENG_TEMPERATURE(8500)) },		/*4 LED BAR*/
	{ (uint16)(ENG_TEMPERATURE(10100)), (uint16)(ENG_TEMPERATURE(10300)) },		/*5 LED BAR*/
	{ (uint16)(ENG_TEMPERATURE(10600)), (uint16)(ENG_TEMPERATURE(10800)) },		/*6 LED BAR*/
	{ (uint16)(ENG_TEMPERATURE(11100)), (uint16)(ENG_TEMPERATURE(11300)) },		/*7 LED BAR*/
	{ (uint16)(ENG_TEMPERATURE(11500)), (uint16)(ENG_TEMPERATURE(11700)) },		/*8 LED BAR*/
};

static void coolant_dispaly_noBar(void);
static void coolant_reset(void);
static boolean coolant_check_CanSignal_valid(void);
static void coolant_calculate_bar(void);
static void coolant_process_keyon(void);
static void coolant_process_keyoff(void);
static void coolant_check_TT_indicator_status(void);
static void coolant_pkg_process_Diagntask(void);
static void coolant_pkg_process_SlowTimetask(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	coolant_reset();
	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional de-initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
	coolant_reset();
	Rte_Write_ppCoolantTmp_level(l_indicated_eng_clnt_temp_counts_U16);
	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional activation state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation( void )
{
	coolant_reset();
	Rte_Write_ppCoolantTmp_level(l_indicated_eng_clnt_temp_counts_U16);
	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional de-activation state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation( void )
{
	coolant_reset();
	Rte_Write_ppCoolantTmp_level(l_indicated_eng_clnt_temp_counts_U16);
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	Dcm_SesCtrlType fl_Diag_Session = DEFAULT_SESSION;

	Rte_Call_rpCS_GetCurrentSessionType_Operation(&fl_Diag_Session);
	
	if(fl_Diag_Session == EOL_SESSION || fl_Diag_Session == EXTENDED_SESSION)
	{
		coolant_pkg_process_Diagntask();
	}
	else
	{
		coolant_pkg_process_SlowTimetask();
	}
	
	return E_OK;
}

/****************************************************************************
Function Name       : coolant_pkg_process_Diagntask

Description         : the Function is used to process diagnotic service

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void coolant_pkg_process_Diagntask(void)
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
			if(fl_ioc_type == IOC_FE05_GAUGE_TYPE_TEMP)
			{
				fl_led_num = DID_Value.DID_IOC_State[1];
				if(fl_led_num > TEMP_LED_BAR)
					fl_led_num = TEMP_LED_BAR;
				Rte_Write_ppCoolantTmp_level(fl_led_num);
			}
		}
		else if(DID_Value.DID_IOC_Parament == 0x00)
		{
			coolant_pkg_process_SlowTimetask();
		}
	}
}


/****************************************************************************
Function Name       : coolant_pkg_process_SlowTimetask

Description         : the Function is used to process task

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void coolant_pkg_process_SlowTimetask(void)
{
	eIgnState fl_IGN_state = FALSE;
	
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

	if(fl_IGN_state == eIGN_RUN)
	{
		coolant_process_keyon();
	}
	else
	{
		coolant_process_keyoff();
	}		
	l_pre_IGN_state = fl_IGN_state;
		
	Rte_Write_ppCoolantTmp_level(l_indicated_eng_clnt_temp_counts_U16);
	Rte_Write_ppSR_WaterTempHigh_STATUS(l_hightemp_tt_indicator_status);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Diagnostic state
///
/// @return E_OK:			Diagnostic has completed. Go to DiagReturn state.
///<br> 	E_NOT_READY:	Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
	coolant_reset();
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
	coolant_reset();
	return E_OK;
}


/****************************************************************************
Function Name       : coolant_reset

Description         : the Function is used to set 0 led bar

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/

static void coolant_dispaly_noBar(void)
{
	l_indicated_eng_clnt_temp_counts_U16 = (uint16)0;	
	l_hightemp_tt_first_initialize_status = TRUE;	
	l_hightemp_tt_indicator_status = FALSE;	
	l_pre_filter_eng_clnt_temp_S32 = 0;
	l_filter_eng_clnt_temp_S32 = (sint32)0;
}

/****************************************************************************
Function Name       : coolant_reset

Description         : Initializes the Target Temperature counts.

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void coolant_reset (void)
{
	l_indicated_eng_clnt_temp_counts_U16 = (uint16)0;
	l_pre_filter_eng_clnt_temp_S32 = 0;
	l_filter_eng_clnt_temp_S32 = (sint32)0;
	l_hightemp_tt_first_initialize_status = TRUE;
	l_value_invalid = FALSE;
}

/****************************************************************************
Function Name       : coolant_process_keyoff

Description         : process coolant led bar when ign is off

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void coolant_process_keyoff(void)
{
	coolant_dispaly_noBar();
}


/****************************************************************************
Function Name       : coolant_process_keyon

Description         : process coolant led bar when ign is off

Invocation          : Internal

Parameters          : None

Return Value        : None

Critical Section    : None
******************************************************************************/
static void coolant_process_keyon(void)
{
	uint8 fl_can_valid;

	fl_can_valid = coolant_check_CanSignal_valid();

	if(fl_can_valid)
	{
		coolant_calculate_bar();
		if(l_value_invalid != TRUE)
		{
			coolant_check_TT_indicator_status();
		}		
	}
	else
	{
		coolant_dispaly_noBar();		//if signal is missing > 2s, show 0 led bar
	}
}


/****************************************************************************
Function Name       : coolant_check_CanSignal_valid

Description         : This function is used is used to check can signal valid or invalid
                      
Invocation          : called in state manager

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/

static boolean coolant_check_CanSignal_valid(void)
{
	uint8 	fl_CAN_10B_nvr_status = 0;
	uint8 	fl_CAN_10B_timeout_status = 0;

	Rte_Read_CoolantTmpMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&fl_CAN_10B_nvr_status);
	Rte_Read_CoolantTmpMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&fl_CAN_10B_timeout_status);

	if(fl_CAN_10B_timeout_status == RTE_E_TIMEOUT || RTE_E_NEVER_RECEIVED == fl_CAN_10B_nvr_status)
    {
    	//SysRS_02_Gauges_128
    	return FALSE;
    }
	return TRUE;
}


/****************************************************************************
Function Name       : ftemp_TransOutOf_Normal

Description         : This function is used is used to calculate led bar
                      
Invocation          : called in state manager

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static void coolant_calculate_bar()
{
	uint8  fl_raw_input_eng_clnt_value_U16=0;
	sint16 fl_raw_eng_clnt_temp_S16 = 0;	
	boolean fl_timer_resDef_started_bool;
	boolean fl_timer_resDef_isElapsed_bool; 
	
	Rte_Read_CoolantTmpMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngCoolanTemp(&fl_raw_input_eng_clnt_value_U16);

	/*******************************************************
	*	
	*	eng_clnt_temp = CAN_eng_clnt_temp x 0.75 - 48
	*	In order to prevent lose accuracy, we zoom in by 100 here
	*			
	********************************************************/
	if((fl_raw_input_eng_clnt_value_U16 != TEMP_INVALID_CAN_VALUE))		//SysRS_02_Gauges_110
	{		 
		
		fl_raw_eng_clnt_temp_S16 =	fl_raw_input_eng_clnt_value_U16 * TEMP_CORRECT_RESOLUTION - TEMP_CORRECT_OFFSET;		
		/*******************************************************
		*	
		*	Input CAN signal filter algorithm
		*	Cn=[Cin_n-1*7]/8+Cin_n/8 (n>=2)
		*	n=1, C1 means that the cluster receives the first valid signal when iginition changes OFF→ON 
		*
		********************************************************/
		if(l_pre_IGN_state == eIGN_OFF )	//SysRS_02_Gauges_527
		{								
			l_filter_eng_clnt_temp_S32 = (sint32)fl_raw_eng_clnt_temp_S16;
		}
		else
		{
			l_filter_eng_clnt_temp_S32 = (sint32)((sint32)(l_pre_filter_eng_clnt_temp_S32 * 7 + (sint32)fl_raw_eng_clnt_temp_S16) / 8);
		}
		
		l_pre_filter_eng_clnt_temp_S32 = fl_raw_eng_clnt_temp_S16;
		l_coolant_display_temp = l_filter_eng_clnt_temp_S32 + ENG_TEMPERATURE_TABLE_OFFSET;
		l_indicated_eng_clnt_temp_counts_U16 = fhyst_get_index_with_up_hyst((void *)&F_TEMP_BAR_THRESHOLD[0], l_coolant_display_temp, l_indicated_eng_clnt_temp_counts_U16);

		Rte_Call_rp_TmExt_Wrap_Timer_IsStarted(eTimerHandleCoolantMissingWait, &fl_timer_resDef_started_bool);
		if(fl_timer_resDef_started_bool == TRUE)
			Rte_Call_rp_TmExt_Timer_Stop(eTimerHandleCoolantMissingWait);		
		l_value_invalid = FALSE;
	}
	else
	{		
		Rte_Call_rp_TmExt_Wrap_Timer_IsStarted(eTimerHandleCoolantMissingWait, &fl_timer_resDef_started_bool); 
		if(fl_timer_resDef_started_bool)
		{
			// abnormal >= 2s
			Rte_Call_rp_TmExt_Wrap_Timer_IsElapsed(eTimerHandleCoolantMissingWait, Coolant_TimeOut_2S, &fl_timer_resDef_isElapsed_bool);
			if(fl_timer_resDef_isElapsed_bool)
			{
				coolant_dispaly_noBar();		//if value is invalid ,then show 0 led bar
				l_value_invalid = TRUE;
			}
			else
			{
				l_value_invalid = FALSE;
			}
		}
		else
		{
			Rte_Call_rp_TmExt_Timer_Start(eTimerHandleCoolantMissingWait);
		}	
	}
}

static void coolant_check_TT_indicator_status(void)
{
    uint16 fl_coolant_display_temp;

    fl_coolant_display_temp = l_coolant_display_temp;

    if (fl_coolant_display_temp <= HighTemp_TT_ICON_NO_DISPLAY )
    {
        /* temperature is < HighTemp_TT_ICON_NO_DISPLAY, don't display TT icon*/
        l_cur_hightemp_tt_indicator_status = FALSE;

        /*update last critical value*/
        l_last_hightemp_tt_critical_value_U16 = HighTemp_TT_ICON_NO_DISPLAY;

		/*fix bug of snow TT display when frist IGN ON*/
		if (FALSE != l_hightemp_tt_first_initialize_status)
		{
		    l_hightemp_tt_first_initialize_status = FALSE;
		}
    }
    else if (fl_coolant_display_temp >= HighTemp_TT_ICON_DISPLAY )
    {
        /*temperature is > HighTemp_TT_ICON_DISPLAY, display TT icon*/
        l_cur_hightemp_tt_indicator_status = TRUE;

        /*update last critical value*/
        l_last_hightemp_tt_critical_value_U16 = HighTemp_TT_ICON_DISPLAY;
		/*fix bug of TT icon display when frist IGN ON*/
		if (FALSE != l_hightemp_tt_first_initialize_status)
		{
		    l_hightemp_tt_first_initialize_status = FALSE;
		}
    }
    else
    {
        /*temperature is between HighTemp_TT_ICON_DISPLAY and HighTemp_TT_ICON_NO_DISPLAY*/
        if (FALSE != l_hightemp_tt_first_initialize_status)
        {
            /*first initialize, temperature is less than HighTemp_TT_ICON_DISPLAY, don't display indicator*/
            l_cur_hightemp_tt_indicator_status = FALSE;

            /*reset first initialize flag*/
            l_hightemp_tt_first_initialize_status = FALSE;

            /*update last critical value*/
            l_last_hightemp_tt_critical_value_U16 = HighTemp_TT_ICON_NO_DISPLAY;
        }
        else
        {
            /*check last critical outside temperature to decide display or not*/
            if (HighTemp_TT_ICON_NO_DISPLAY == l_last_hightemp_tt_critical_value_U16)
            {
                /*temperature changed from icon display, but less than HighTemp_TT_ICON_NO_DISPLAY, don't display indicator*/
                l_cur_hightemp_tt_indicator_status = FALSE;
            }
            else if (HighTemp_TT_ICON_DISPLAY == l_last_hightemp_tt_critical_value_U16)
            {
                /*temperature changed from icon display relieve, but larger than HighTemp_TT_ICON_DISPLAY, display indicator*/
                l_cur_hightemp_tt_indicator_status = TRUE;
            }
            else
            {
                /*should never occurred, nothing to do*/
            }
        }
    }
    l_hightemp_tt_indicator_status = l_cur_hightemp_tt_indicator_status;
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
