/*---------------------------------------------------------------------------------------------------------------------
**
** VISTEON CORPORATION CONFIDENTIAL
** 
**
** [2015] Visteon Corporation
** All Rights Reserved.
**
** NOTICE: This is an unpublished work of authorship, which contains trade secrets.
** Visteon Corporation owns all rights to this work and intends to maintain it in confidence to preserve
** its trade secret status. Visteon Corporation reserves the right, under the copyright laws of the United States
** or those of any other country that may have jurisdiction, to protect this work as an unpublished work,
** in the event of an inadvertent or deliberate unauthorized publication. Visteon Corporation also reserves its rights
** under all copyright laws to protect this work as a published work, when appropriate.
** Those having access to this work may not copy it, use it, modify it, or disclose the information contained in it
** without the written authorization of Visteon Corporation.
**
**--------------------------------------------------------------------------------------------------------------------*/
#include "Rte_SIAMdl.h"
//#include "Rte_BtnMdl_Type.h"
#include "CmpLib.h"
#include "NvM.h"
#include "NvM_Cfg.h"

/*@brief  Instance of life-cycle state-machine*/

CMPLIB_INSTANCE(SIAMdl)
/*=====================================================================================================================
**  CONSTANTS & TYPES
**====================================================================================================================*/

typedef enum tag_e_SIA_Reset
{
	SIA_RESET_NO = 0,		        /* 0*/
	SIA_RESET_DIAGNOSE,		        /* 1*/
	SIA_RESET_MENU		            /* 2*/
}e_SIA_Reset;

#define SIA_WARN_OUT_UPPER    ((sint16)500)
#define SIA_WARN_OUT_LOWER    ((uint8)0)

#define SIA_CAN_OUTPUT_OFFSET ((uint16)9999)
#define SIA_CAN_OUTPUT_LOW	  ((sint32)-9999)
#define SIA_CAN_OUTPUT_HIGH   ((sint32)7500)
#define SIA_CAN_OUTPUT_ERROR  ((sint32)0x7FFF)

#define BTN_ID_OK 8
#define BTN_EV_LONG_IN 3
#define HMI_BTN_EV_OK_LONG_IN  ((BTN_ID_OK<<4) + BTN_EV_LONG_IN)
#define TIMEOUT_4S   (4000)
#define TIMEOUT_30S  (30000)

//#define WARN_REMAIN_MAINTENANCE_XXXXKM_ID 1
#define DEGBUG_SIA_SET_VALUE 0 /*used for when no diaglog and no E2_Bin Download,temp write initialization*/




/*=====================================================================================================================
** I N T E R N A L   F U N C T I O N   P R O T O T Y P E S
**====================================================================================================================*/
static uint16 f_SIA_get_interval(uint8 fl_SIA_interval_mode);
static void f_SIA_reset_function(void);
static void f_SIA_get_reset_value(void);
static void f_SIA_update(void);

/*=====================================================================================================================
** M E M O R Y   A L L O C A T I O N
**====================================================================================================================*/

const uint16 VeConfig_Maintenance_Interval0_U16 = 5000U;   /* 5000km */
const uint16 VeConfig_Maintenance_Interval1_U16 = 7500U;   /* 7500km */

e_SIA_Reset l_SIA_Reset_Mode;
static uint16 l_SIA_Reset_Value_U16;
static boolean l_SIA_Reset_warning_flag;/*use for SIA reset success_warning*/
static boolean SIA_Output_MaintainRemind_flag;
static boolean SIA_Output_MaintenanceRemind_flag;
//static uint8 l_SIA_WarnFlag=0;
static uint8 l_SIA_TT_Flag=0;


static boolean l_ok_long_in_BOOL=FALSE;
static boolean l_reset_maintenance_flag=FALSE;


/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional initialization state
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpInit( void )
{
	NV_Data_Cluster_SIA_Reset_Odo_OffSet fl_SIA_Reset_Odo_Value;

	l_SIA_Reset_Mode = SIA_RESET_NO;
	/* read SIA set value from NVM */
	(void)Rte_Read_pprpNV_SIAMdl_Reset_Odo_OffSet_Offset_Value(&fl_SIA_Reset_Odo_Value);

	l_SIA_Reset_Value_U16 = fl_SIA_Reset_Odo_Value.Store_Kilometer_Value;
	
	l_SIA_Reset_warning_flag = FALSE;
	SIA_Output_MaintainRemind_flag = TRUE;
	SIA_Output_MaintenanceRemind_flag = TRUE;
	l_ok_long_in_BOOL = FALSE;
	l_reset_maintenance_flag = FALSE;

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
	NV_Data_Cluster_SIA_Reset_Odo_OffSet fl_SIA_Reset_Odo_Value;

	l_SIA_Reset_Mode = SIA_RESET_NO;
	l_ok_long_in_BOOL=FALSE;
	
	l_SIA_Reset_warning_flag = FALSE;
	/* read SIA set value from NVM */
	(void)Rte_Read_pprpNV_SIAMdl_Reset_Odo_OffSet_Offset_Value(&fl_SIA_Reset_Odo_Value);

	if ( (fl_SIA_Reset_Odo_Value.Store_Kilometer_Value != VeConfig_Maintenance_Interval0_U16)
		  && (fl_SIA_Reset_Odo_Value.Store_Kilometer_Value != VeConfig_Maintenance_Interval1_U16) )
	{
		l_SIA_Reset_Value_U16 = VeConfig_Maintenance_Interval0_U16;/*E2 SIA error*/
	}
	else
	{
		l_SIA_Reset_Value_U16 = fl_SIA_Reset_Odo_Value.Store_Kilometer_Value;/*E2 SIA correct*/
	}


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
	l_SIA_Reset_warning_flag = FALSE;
    return  E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  State for normal operations
** 
** @return The return value is disregarded. Say E_OK
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpActive( void )
{
	
	/* get the reset status */
	f_SIA_reset_function();
	/* get the reset value */
	f_SIA_get_reset_value();
	/* update the maintenance remind status and maintenance remind display value */
	f_SIA_update();
		
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
Function Name     :  f_SIA_get_interval

Description       :  

Parameters        :  none

Return Value      :  	        

Critical Section  :  none

******************************************************************************/
static uint16 f_SIA_get_interval(uint8 fl_SIA_interval_mode)
{
	uint16 fl_SIA_U16;
	switch(fl_SIA_interval_mode)
	{
		case MAINTENANCE_INTERVAL_SET_5000KM:
			fl_SIA_U16 = VeConfig_Maintenance_Interval0_U16;
			break;

		case MAINTENANCE_INTERVAL_SET_7500KM:
			fl_SIA_U16 = VeConfig_Maintenance_Interval1_U16;
			break;
		
		default:
			fl_SIA_U16 = VeConfig_Maintenance_Interval0_U16;
			break;
	}
	return (fl_SIA_U16);
}

/****************************************************************************
Function Name     :  f_SIA_reset_function

Description       :  

Parameters        :  none

Return Value      :  	        

Critical Section  :  none

******************************************************************************/
static void f_SIA_reset_function(void)
{
    uint8 fl_IgnStatus;
	uint8 fl_button_reset_flag=FALSE;
	uint8 fl_Read_ODO_reset_flag=FALSE;
	static uint8 fl_Write_ODO_reset_flag = FALSE;
	boolean fl_timerRunningSt=FALSE; 
	boolean fl_timerElapsedSt=FALSE;
	/* Read the Menu reset button status */
	(void)Rte_Read_rpSR_BtnMdl_toSIA_KeyValue(&fl_button_reset_flag);/*Lhbb Source is Menu Button*/
	//Rte_Read_pprpSR_TIOdo_SIA_ResetSIA(&fl_Read_ODO_reset_flag);/*Lhbb Source is odoReset,总里程清零时，保养里程也要清零同时记录当前总里程*/
    (void)Rte_Read_rpIgnState_IGNState(&fl_IgnStatus);
	
    /*debug fl_button_reset_flag = HMI_BTN_EV_OK_LONG_IN;*/
	if (fl_button_reset_flag == HMI_BTN_EV_OK_LONG_IN)
	{
		if (fl_IgnStatus == eIGN_OFF)
		{
		   l_ok_long_in_BOOL = TRUE;
		}
		else
	    {
	       //l_ok_long_in_BOOL = FALSE;
		}
	}

    /*Task run next time,first judge l_ok_long_in_BOOL */
    if (TRUE == l_ok_long_in_BOOL)
    {
		if (fl_IgnStatus == eIGN_RUN)
		{
		    if (fl_button_reset_flag == HMI_BTN_EV_OK_LONG_IN)
	        {
			 	/*start 4s timer*/
			    Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerSIAMdl,&fl_timerRunningSt);
				if(fl_timerRunningSt == FALSE)
				{
					Rte_Call_rptimer_TmExt_Timer_Start(eTimerSIAMdl);
				}
				Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerSIAMdl,TIMEOUT_4S,&fl_timerElapsedSt);
				
				//SysRS_
				if(fl_timerElapsedSt == TRUE)
				{
					l_reset_maintenance_flag = TRUE;
					Rte_Call_rptimer_TmExt_Timer_Stop(eTimerSIAMdl);
				}
		    }
			else
		    {
		      l_ok_long_in_BOOL = FALSE;
		      Rte_Call_rptimer_TmExt_Timer_Stop(eTimerSIAMdl);
		    }
		}
    }

	
	
    if(TRUE == l_reset_maintenance_flag)
	{
		/* clear the diagnose reset flag */
		l_SIA_Reset_Mode = SIA_RESET_MENU;
		l_reset_maintenance_flag = FALSE;
		l_ok_long_in_BOOL =FALSE;
		/* write 0 to RTE, to clear the Menu reset button status*/
		//(void)Rte_Write_pprpSR_TISIA_Gdt_MRButtonReset_MR_Button(0);
	}
	else if((TRUE == fl_Read_ODO_reset_flag) && (fl_Write_ODO_reset_flag == FALSE))
	{
		/*lxiong2 add this to fix defect 904958 and 904964*/
		fl_Write_ODO_reset_flag = TRUE;
		l_SIA_Reset_Mode = SIA_RESET_MENU;
	}
	else
	{
		l_SIA_Reset_Mode = SIA_RESET_NO;
	}
}

static void f_SIA_get_reset_value(void)
{
	uint16 fl_SIA_U16;
	uint32 fl_odo_current_value_U32;
	NV_Data_Cluster_SIA_Reset_Odo_OffSet fl_SIA_Reset_Odo_Value;
	
    if(SIA_RESET_NO != l_SIA_Reset_Mode)
    {
		/*Get the current Km From RTE */
		(void)Rte_Read_rpSR_rSIAMdl_OdoInKiloMeters_KiloMeters(&fl_odo_current_value_U32);
		if(SIA_RESET_MENU == l_SIA_Reset_Mode)
		{
			/* get the service maintenance interval from NVM */
			//(void)Rte_Read_rpSR_TINVMExtMdl_Cluster_Optional_Configure_MAINTENANCE_INTERVAL_VARIETY(&l_cluster_SIA_configure);
			//fl_SIA_interval_mode = (uint8)(l_cluster_SIA_configure);/*Lhbb  D53 Not need optionnal,second 7500km */	
			fl_SIA_U16 = f_SIA_get_interval(MAINTENANCE_INTERVAL_SET_7500KM);

			fl_SIA_Reset_Odo_Value.Reset_Kilometer_Offset = fl_odo_current_value_U32;/*store odovalue when reset maintenance */
			fl_SIA_Reset_Odo_Value.Store_Kilometer_Value = fl_SIA_U16;/*default 5000km,then 7500km*/

			/* write current odo and SIA reset value to NVM */
			(void)Rte_Write_pprpNV_SIAMdl_Reset_Odo_OffSet_Offset_Value(&fl_SIA_Reset_Odo_Value);
			/*Lhbb D53 not need write optional*/
			l_SIA_Reset_Value_U16 = fl_SIA_U16;
			l_SIA_Reset_warning_flag = TRUE;
		}
		else
		{
			/* do nothing */
		}
		
    }
	else
	{
		/* do nothing */
	  #if DEGBUG_SIA_SET_VALUE
	  		fl_odo_current_value_U32 = 0; 
			fl_SIA_U16 = 5000;
	     	fl_SIA_Reset_Odo_Value.Reset_Kilometer_Offset = fl_odo_current_value_U32;/*store odovalue when reset maintenance */
			fl_SIA_Reset_Odo_Value.Store_Kilometer_Value = fl_SIA_U16;/*default 5000km,then 7500km*/

			/* write current odo and SIA reset value to NVM */
			(void)Rte_Write_pprpNV_SIAMdl_Reset_Odo_OffSet_Offset_Value(&fl_SIA_Reset_Odo_Value);
			/*Lhbb D53 not need write optional*/
			l_SIA_Reset_Value_U16 = fl_SIA_U16;
			l_SIA_Reset_warning_flag = TRUE;
	  #endif
	}
}

/****************************************************************************
Function Name     :  f_SIA_update

Description       :  This function for the remind update

Parameters        :  none

Return Value      :  

					SIA_MAINTENANCE_OFF = 0,		   
	                SIA_DISTANCE_TO_MAINTENANCE,		
	                SIA_MAINTENANCE_NOW,		        
	                SIA_OVER_MAINTENANCE		        

Critical Section  :  none

******************************************************************************/
static void f_SIA_update(void)
{
	uint32 fl_odo_current_value_U32;
	sint32 fl_SIA_Display_Value_S32 = 0;
	NV_Data_Cluster_SIA_Reset_Odo_OffSet fl_SIA_Reset_Odo_Value;
	sint32 fl_SIA_Reset_Value_S32 = 0;
	uint8 SIAMdlIgnSts;
	sint32 fl_ic_MaintenanceMile_S16 = 0;/*output*/
	uint8 fl_ic_MaintenanceLogo_U8 = 0;/*output*/
	uint8 fl_ic_MaintenanceExpLogo_U8 = 0;/*output*/
	uint8 fl_ic_MaintainRemind_U8 = 0;/*output*/
	uint8 fl_ic_MaintenanceRemind_U8 = 0;/*output*/
	boolean fl_timerTwoRunSts=FALSE;
	boolean fl_timerTwoExpSts=FALSE;
	boolean fl_timerThreeRunSts=FALSE;
	boolean fl_timerThreeExpSts=FALSE;
	uint32  fl_CurrentVelSpd = 0;
	

	/*Get the current Km From RTE */
	(void)Rte_Read_rpSR_rSIAMdl_OdoInKiloMeters_KiloMeters(&fl_odo_current_value_U32);
    /*Get the SIA reset Value from NVM */
	fl_SIA_Reset_Value_S32 = l_SIA_Reset_Value_U16;/*Lhbb Store_Kilometer_Value*/

	(void)Rte_Read_rpIgnState_IGNState(&SIAMdlIgnSts);
	(void)Rte_Read_rpSR_TISpdMdl_Recv_value(&fl_CurrentVelSpd);
	
	if(SIAMdlIgnSts == eIGN_OFF)
	{
		l_SIA_Reset_warning_flag = FALSE;
	}

	if(0x7FFFFFFF == fl_odo_current_value_U32)
	{
		fl_SIA_Display_Value_S32 = SIA_CAN_OUTPUT_ERROR;
		fl_ic_MaintenanceMile_S16 = SIA_CAN_OUTPUT_ERROR;
	}
	else
	{
		/* read the SIA reset odo value from NVM */
		(void)Rte_Read_pprpNV_SIAMdl_Reset_Odo_OffSet_Offset_Value(&fl_SIA_Reset_Odo_Value);
		//fl_SIA_Reset_Odo_Value.Reset_Kilometer_Offset = 0;//Lhbb NVM temp not set
		fl_SIA_Display_Value_S32 = (sint32)(fl_odo_current_value_U32 - fl_SIA_Reset_Odo_Value.Reset_Kilometer_Offset);
		fl_SIA_Display_Value_S32 = fl_SIA_Reset_Value_S32 - fl_SIA_Display_Value_S32;
	
		if (fl_odo_current_value_U32 >= fl_SIA_Reset_Odo_Value.Reset_Kilometer_Offset)
		{
			/* do nothing */
		}
		else
		{
			fl_SIA_Display_Value_S32 = SIA_CAN_OUTPUT_ERROR;
		}

		/*Transmit the current SIA to RTE Wirte to hmi_menu_interface.c*/
		if(fl_SIA_Display_Value_S32 < SIA_CAN_OUTPUT_LOW)
		{
			(void)Rte_Write_ppSR_pSIAMdl_TISIAMdl_Value_Out_MRDisplayValue_MR_Display_Value(SIA_CAN_OUTPUT_LOW);
		}
		else if(fl_SIA_Display_Value_S32 > SIA_CAN_OUTPUT_HIGH)
		{
			(void)Rte_Write_ppSR_pSIAMdl_TISIAMdl_Value_Out_MRDisplayValue_MR_Display_Value(SIA_CAN_OUTPUT_ERROR);
		}
		else
		{
			(void)Rte_Write_ppSR_pSIAMdl_TISIAMdl_Value_Out_MRDisplayValue_MR_Display_Value(fl_SIA_Display_Value_S32);
		}

		/*Lhbb IGN on only warn one time SysRS_03_HMIFunction-2201*/
		if ((0 == fl_CurrentVelSpd) && (eIGN_RUN == SIAMdlIgnSts))
		{
#if 0
			if(fl_SIA_Display_Value_S32 <= SIA_WARN_OUT_UPPER)
			{
				l_SIA_WarnFlag = 1; 
			}
			else if (TRUE == l_SIA_Reset_warning_flag)
			{
			    l_SIA_WarnFlag = 1;/*SysRS_03_HMIFunction-2208 Lhbb when SIA Reset,display warnnig 3s*/
			}
			else
			{
				l_SIA_WarnFlag = 0;
			}
#endif
			if(fl_SIA_Display_Value_S32 <= SIA_WARN_OUT_UPPER)
			{
				l_SIA_TT_Flag = 1; 
			}
			else
			{
				l_SIA_TT_Flag = 0;
			}
		}
		/*Transmit the current SIA to Warning status to RTE */
		//(void)Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_REMAIN_MAINTENANCE_XXXXKM_ID,l_SIA_WarnFlag);/*SIA warning*/
		/*Transmit the current SIA to TT status to RTE */
		(void)Rte_Write_ppSR_Sia_to_TT_st(l_SIA_TT_Flag);/*SIA TT*/

		
		/* CAN Output for SIA 0x4B0 IC_MaintenanceMile */
		if(fl_SIA_Display_Value_S32 < SIA_CAN_OUTPUT_LOW)
		{
			fl_ic_MaintenanceMile_S16 = (sint16)SIA_CAN_OUTPUT_LOW + SIA_CAN_OUTPUT_OFFSET;
		}
		else if(fl_SIA_Display_Value_S32 > SIA_CAN_OUTPUT_HIGH)
		{
			fl_ic_MaintenanceMile_S16 = (sint16)SIA_CAN_OUTPUT_HIGH + SIA_CAN_OUTPUT_OFFSET;/*SysRS_03_HMIFunction-1042*/
		}
		else
		{
			fl_ic_MaintenanceMile_S16 = (sint16)fl_SIA_Display_Value_S32 + SIA_CAN_OUTPUT_OFFSET; /*Lhbb D53 No need to convert uint16,direct output negative value*/
		}
		/* CAN Output for SIA 0x4B0 IC_MaintenanceLogo  SysRS_03_HMIFunction-1045*/
        if(fl_SIA_Display_Value_S32 <= SIA_WARN_OUT_UPPER)  /*500KM*/
        {
           fl_ic_MaintenanceLogo_U8 = 0;
		}
		else
		{
		   fl_ic_MaintenanceLogo_U8 = 1;
		}
		/* CAN Output for SIA 0x4B0 IC_MaintenanceExpLogo SysRS_03_HMIFunction-1046*/	
		if(fl_SIA_Display_Value_S32 <= SIA_WARN_OUT_LOWER)  /*0KM*/
        {
           fl_ic_MaintenanceExpLogo_U8 = 1;
		}
		else
		{
		   fl_ic_MaintenanceExpLogo_U8 = 0;
		}
		/* CAN Output for SIA 0x4B0 ic_MaintainRemind  SysRS_03_HMIFunction-2200*/	
		if((fl_SIA_Display_Value_S32 <= SIA_WARN_OUT_UPPER) && (TRUE == SIA_Output_MaintainRemind_flag))  /*500KM*/
        {
           fl_ic_MaintainRemind_U8 = 1;
		   // add a start timer 30s
		   Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerSIAMdlTwo,&fl_timerTwoRunSts);
		   if(fl_timerTwoRunSts == FALSE)
		   {
			 Rte_Call_rptimer_TmExt_Timer_Start(eTimerSIAMdlTwo);
		   }
		   Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerSIAMdlTwo,TIMEOUT_30S,&fl_timerTwoExpSts);	
			//SysRS_
		   if(fl_timerTwoExpSts == TRUE)
		   {
			 fl_ic_MaintainRemind_U8 = 0;
			 SIA_Output_MaintainRemind_flag = FALSE;
			 Rte_Call_rptimer_TmExt_Timer_Stop(eTimerSIAMdl);
		   }
		}
		else
		{
		   fl_ic_MaintainRemind_U8 = 0;
		}
		/* CAN Output for SIA 0x392 IC2 IC_MaintenanceRemind SysRS_03_HMIFunction-2201  */
		if((fl_SIA_Display_Value_S32 <= SIA_WARN_OUT_UPPER) && (TRUE == SIA_Output_MaintenanceRemind_flag) && (eIGN_RUN == SIAMdlIgnSts)) /*500KM*/
        {
           fl_ic_MaintenanceRemind_U8 = 1;
		   // add a start timer 30s
		   Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerSIAMdlThree,&fl_timerThreeRunSts);
		   if(fl_timerThreeRunSts == FALSE)
		   {
			 Rte_Call_rptimer_TmExt_Timer_Start(eTimerSIAMdlThree);
		   }
		   Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerSIAMdlThree,TIMEOUT_30S,&fl_timerThreeExpSts);	
			//SysRS_
		   if(fl_timerThreeExpSts == TRUE)
		   {
			 fl_ic_MaintenanceRemind_U8 = 0;
			 SIA_Output_MaintenanceRemind_flag = FALSE;
			 Rte_Call_rptimer_TmExt_Timer_Stop(eTimerSIAMdlThree);
		   }
		}
		else
		{
		   fl_ic_MaintenanceRemind_U8 = 0;
		}
	}    

	/* write the current SIA to CAN Output */
	(void)Rte_Write_ppSR_CANMSG_IC_Info_ComOut_IC_MaintenanceMile(fl_ic_MaintenanceMile_S16);
	(void)Rte_Write_ppSR_CANMSG_IC_Info_ComOut_IC_MaintenanceLogo(fl_ic_MaintenanceLogo_U8);
	(void)Rte_Write_ppSR_CANMSG_IC_Info_ComOut_IC_MaintenceExpLogo(fl_ic_MaintenanceExpLogo_U8);
	(void)Rte_Write_ppSR_CANMSG_IC_Info_ComOut_IC_MaintainRemind(fl_ic_MaintainRemind_U8);
	(void)Rte_Write_ppSR_CANMSG_IC2_ComOut_IC_MaintenceRemind(fl_ic_MaintenanceRemind_U8);
}

/*--------------------------------------------------------------------------*/
/*  Name:  Sp_SIAMAINTENCE_NvMNotifyJobFinished_JobFinished                                               */
/*                                                                          */
/*  Return:     None                                                        */
/*                                                                          */
/*  Parameters:  uint8 JobResult     - TRUE if update successfull		         */
/*                                   FALSE otherwise	                    */
/*                															*/
/*				 uint8 ServiceId  - Write/Read type                             */
/*  Visibility:  none                                                       */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*     This is part of the EEPROM Manager interface.                        */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */  
/*                                                                          */
/*--------------------------------------------------------------------------*/
void SIAMdl_SIAMAINTENCE_NvMNotifyJobFinished_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	//l_SIA_Diag_5003_write_ok_bool = TRUE;
}

void SIAMdl_Reset_NvMNotifyJobFinished_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
}

void SIAMdl_Optional_NvMNotifyJobFinished_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{

}
/*---------------------------------------------------------------------------
Release Label     : Initial Version
Date              : 1-Nov-2016
By                : xsong2
Traceability      : Maintenance Reminder Display_2016.10.19.docx
Change Description: Initial Release
-----------------------------------------------------------------------------*/

