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

#include "Rte_BattMdl.h"
#include "CmpLib.h"
#include "Dem.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(BattMdl)

/* ADC channel number to be passed to IoHwAb */
/// @brief The current battery state
static eBatteryState l_BatteryState_U8 = eBatteryState_UnderVoltage; 
/// @brief Previous battery state
static eBatteryState l_PrevBattState_U8 = eBatteryState_Normal;


//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
typedef struct
{
  unsigned int OverVoltageThreshold;      /* 18.5V */
  unsigned int OverVoltagePh1Threshold;   /* 16V */
  unsigned int UnderVoltagePh1Threshold;  /* 9V  */
  unsigned int UnderVoltageThreshold;     /* 7V  */
  unsigned int VoltageHysteresis;         /* 0.5V */
  unsigned int VoltageHysteresis2;        /* 0.4V *//*used for 6V,real is 6.4V*/
} Global_Data_Batt;

//=====================================================================================================================
//  Local Micro Declarations
//=====================================================================================================================
#define DEM_BATT_LOW_VOLTAGE  		900U  //9V
#define DEM_BATT_HIGH_VOLTAGE 		1600U //16V
#define CAN_LOST_DTC_BELOW_9V		(13)
#define CAN_LOST_DTC_ABOVE_16V		(14)
#define RESULT_NORMAL             	(0x2)
#define RESULT_FAILED            	(0x3)


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static boolean l_dem_voltage_abnormal;
boolean dem_batt_in_mormal_5s;

/// @brief NV Battery data 
static Global_Data_Batt BatteryData;

/* Lhbb Get the initstate and average*/
static boolean l_battmdl_init_bool;
static uint8 l_battmdl_ad_value_index;
static uint16 l_battmdl_ad_vbat_filter_buf[20];
//static uint16 l_battmdl_battState_holdtime;

//=====================================================================================================================
//  Local Function Declarations 
//=====================================================================================================================
static void battmdl_dem_process(TIOAppData volt);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
    // Write initialization value which is Battery state - Normal
    (void)Rte_Write_ppBattState_BatteryState(l_BatteryState_U8);
    (void)Rte_Write_ppBattState_IsHighVoltage(FALSE);
    (void)Rte_Write_ppBattState_IsLowVoltage(FALSE);

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
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
	/*This configuration must be updated from BDC HSIS*/
	BatteryData.OverVoltageThreshold = 1850U;	  /* 18.5V  */
	BatteryData.OverVoltagePh1Threshold = 1600U;  /* 16V  */
	BatteryData.UnderVoltagePh1Threshold = 900U;  /* 9V   */
	BatteryData.UnderVoltageThreshold = 700U; 	  /* 7V   */
	BatteryData.VoltageHysteresis = 50U;          /* 0.5V */
	BatteryData.VoltageHysteresis2 = 40U;         /* 0.4V */

    l_BatteryState_U8 = eBatteryState_UnderVoltage;
	l_PrevBattState_U8 = eBatteryState_Normal;
	
    l_battmdl_init_bool = FALSE;
    l_battmdl_ad_value_index = 0;

	l_dem_voltage_abnormal = FALSE;
	dem_batt_in_mormal_5s = FALSE;
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
    return  E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive(void)  //30ms cycle 
{
    TIOAppData Voltage = 0;
    TIOAppData AD_VBAT_Voltage = 0;
	uint32 sum = 0;
	uint8 i;
	//eBatteryState l_TempBattState_U8 = 0xFF;
	//uint8 RetVal = 0;
	uint8 OverVoltage_Started = FALSE;
	uint8 OverVoltage_Exp = FALSE;
	uint8 UnderVoltage_Started = FALSE;
	uint8 UnderVoltage_Exp = FALSE;
	//uint8 NormalVoltage_Started = FALSE;
	//uint8 NormalVoltage_Exp = FALSE;
	
    (void)Rte_Call_rpIoHwAbGet_Impl_GetVoltage(eIOVoltageInId_MAI_BAT ,&AD_VBAT_Voltage);

	//10*30 ms delay
	if(l_battmdl_ad_value_index >= 20) /*when >= 20,always use [19]*/
	{
		for(i = 10; i < 20; i++) /*calculate the sum of 10->20 */
		{
			sum = sum + l_battmdl_ad_vbat_filter_buf[i];
		}
		Voltage = (uint16)(sum / 10);
		for(i = 10; i < 19; i++) /*shift one index to the last */
		{
			l_battmdl_ad_vbat_filter_buf[i] =  l_battmdl_ad_vbat_filter_buf[i+1];
		}
		l_battmdl_ad_vbat_filter_buf[19] = AD_VBAT_Voltage;/*Set the new data in [19]*/
	}
	else 
	{
		l_battmdl_ad_vbat_filter_buf[l_battmdl_ad_value_index] = AD_VBAT_Voltage;
		l_battmdl_ad_value_index++; 
		if(l_battmdl_ad_value_index >= 20)
		{
			for(i = 10; i < 20; i++)  /*calculate the sum of 10->20 */ 
			{
				sum = sum + l_battmdl_ad_vbat_filter_buf[i];
			}
			Voltage = (uint16)(sum / 10);
		}
	}

	if(l_battmdl_init_bool == FALSE)
	{
		if(l_battmdl_ad_value_index >= 20)
		{
			l_battmdl_init_bool = TRUE;
		}
		else
		{
			l_battmdl_init_bool = FALSE;
		}
	}
	else
	{	
		l_PrevBattState_U8 = l_BatteryState_U8;
	    switch ( l_BatteryState_U8 )
	    {
			case eBatteryState_UnderVoltage:				
				if ( Voltage > BatteryData.OverVoltageThreshold )  /*18.5V*/ 
				{
					l_BatteryState_U8 = eBatteryState_OverVoltage;
				}
				else if ( Voltage > BatteryData.OverVoltagePh1Threshold ) /*16V*/
				{
					l_BatteryState_U8 = eBatteryState_OverVoltagePhase1;
				}
				else if ( Voltage >= BatteryData.UnderVoltagePh1Threshold ) /*9V*/
				{
					l_BatteryState_U8 = eBatteryState_Normal;
				}
				else if ( Voltage > BatteryData.UnderVoltageThreshold)
				{
					l_BatteryState_U8 = eBatteryState_UnderVoltagePhase1;
				}
				else
				{

				}
				break;

			case eBatteryState_UnderVoltagePhase1:
				if ( Voltage < BatteryData.UnderVoltageThreshold - BatteryData.VoltageHysteresis )  /*7 - 0.5V*/ 
				{
					l_BatteryState_U8 = eBatteryState_UnderVoltage;
				}
				else if ( Voltage > BatteryData.OverVoltageThreshold ) /*18.5V*/
				{
					l_BatteryState_U8 = eBatteryState_OverVoltage;
				}
				else if ( Voltage > BatteryData.OverVoltagePh1Threshold ) /*16V*/
				{
					l_BatteryState_U8 = eBatteryState_OverVoltagePhase1;
				}
				else if ( Voltage >= BatteryData.UnderVoltagePh1Threshold) /*9V*/
				{
					l_BatteryState_U8 = eBatteryState_Normal;
				}
				else
				{

				}
				break;
				
			case eBatteryState_Normal:
				if ( Voltage < BatteryData.UnderVoltageThreshold - BatteryData.VoltageHysteresis ) /*7 - 0.5V*/
				{
					l_BatteryState_U8 = eBatteryState_UnderVoltage;
				}
				else if ( Voltage <= BatteryData.UnderVoltagePh1Threshold - BatteryData.VoltageHysteresis )/*9 - 0.5V*/
				{
					l_BatteryState_U8 = eBatteryState_UnderVoltagePhase1;
				}
				else if ( Voltage > BatteryData.OverVoltageThreshold ) /*18.5V*/ 
				{
					l_BatteryState_U8 = eBatteryState_OverVoltage;
				}
				else if ( Voltage > BatteryData.OverVoltagePh1Threshold ) /*16V*/
				{
					l_BatteryState_U8 = eBatteryState_OverVoltagePhase1;
				}
				else
				{
	
				}
				break;
			
			case eBatteryState_OverVoltagePhase1:
				if ( Voltage < BatteryData.UnderVoltageThreshold - BatteryData.VoltageHysteresis ) /* 7 - 0.5V */
				{
					l_BatteryState_U8 = eBatteryState_UnderVoltage;
				}
				else if ( Voltage <= BatteryData.UnderVoltagePh1Threshold - BatteryData.VoltageHysteresis )/*9 - 0.5V*/
				{
					l_BatteryState_U8 = eBatteryState_UnderVoltagePhase1;
				}
				else if ( Voltage <= BatteryData.OverVoltagePh1Threshold )  /*16V*/
				{
					l_BatteryState_U8 = eBatteryState_Normal;
				}
				else if ( Voltage > BatteryData.OverVoltageThreshold ) /*18.5V*/
				{
					l_BatteryState_U8 = eBatteryState_OverVoltage;
				}
				else
				{

				}
				break;
			
			case eBatteryState_OverVoltage:
				if ( Voltage < BatteryData.UnderVoltageThreshold - BatteryData.VoltageHysteresis ) /* 7 - 0.5V */
				{
					l_BatteryState_U8 = eBatteryState_UnderVoltage;
				}
				else if ( Voltage <= BatteryData.UnderVoltagePh1Threshold - BatteryData.VoltageHysteresis )/*9 - 0.5V*/
				{
					l_BatteryState_U8 = eBatteryState_UnderVoltagePhase1;
				}
				else if ( Voltage <= BatteryData.OverVoltagePh1Threshold )  /*16V*/
				{
					l_BatteryState_U8 = eBatteryState_Normal;
				}
				else if ( Voltage <= BatteryData.OverVoltageThreshold - BatteryData.VoltageHysteresis ) /*18.5 - 0.5V*/
				{
					l_BatteryState_U8 = eBatteryState_OverVoltagePhase1;
				}			
				else
				{

				}
				break;

		    // polyspace<RTE:UNR> Unreachable code from Polyspace point of view, however it is tested with VectorCast.
		    default: 
		        break;
	    }

		if((l_BatteryState_U8 == eBatteryState_OverVoltage) || (l_BatteryState_U8 == eBatteryState_OverVoltagePhase1))		//OverVoltage
		{	
			//Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerNormalVoltage,&NormalVoltage_Started);
			//if(NormalVoltage_Started == TRUE)
			//{
			//	Rte_Call_rptimer_TmExt_Timer_Stop(eTimerNormalVoltage);
			//}
			
			Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerOverVoltage,&OverVoltage_Started);
			if (OverVoltage_Started == FALSE)
			{
				Rte_Call_rptimer_TmExt_Timer_Start(eTimerOverVoltage);
			}
		
			Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerOverVoltage,2000,&OverVoltage_Exp);
			if (OverVoltage_Exp == TRUE)
			{
				Rte_Write_ppBattState_BatteryState(l_BatteryState_U8);
				Rte_Write_ppBattState_IsHighVoltage(TRUE);											//OverVoltage interface
				l_dem_voltage_abnormal = TRUE;			
			}
			else
			{
				Rte_Write_ppBattState_IsHighVoltage(FALSE);	
				l_dem_voltage_abnormal = FALSE;
			}
		}
		else if((l_BatteryState_U8 == eBatteryState_UnderVoltage) || (l_BatteryState_U8 == eBatteryState_UnderVoltagePhase1))		//UnderVoltage
		{
			//Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerNormalVoltage,&NormalVoltage_Started);
			//if(NormalVoltage_Started == TRUE)
			//{
			//	Rte_Call_rptimer_TmExt_Timer_Stop(eTimerNormalVoltage);
			//}
		
			Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerUnderVoltage,&UnderVoltage_Started);
			if (UnderVoltage_Started == FALSE)
			{
				Rte_Call_rptimer_TmExt_Timer_Start(eTimerUnderVoltage);
			}
		
			Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerUnderVoltage,2000,&UnderVoltage_Exp);
			if (UnderVoltage_Exp == TRUE)
			{
				Rte_Write_ppBattState_BatteryState(l_BatteryState_U8);
				Rte_Write_ppBattState_IsLowVoltage(TRUE); 										//UnderVoltage interface
				l_dem_voltage_abnormal = TRUE;
			}
			else
			{
				Rte_Write_ppBattState_IsLowVoltage(FALSE);			
				l_dem_voltage_abnormal = FALSE;
			}
		}
		else
		{
			l_dem_voltage_abnormal = FALSE;
			Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerOverVoltage,&OverVoltage_Started);
			if(OverVoltage_Started == TRUE)
			{
				Rte_Call_rptimer_TmExt_Timer_Stop(eTimerOverVoltage);
			}
			
			Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerUnderVoltage,&UnderVoltage_Started);
			if(UnderVoltage_Started == TRUE)
			{
				Rte_Call_rptimer_TmExt_Timer_Stop(eTimerUnderVoltage);
			}

			//Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerNormalVoltage,&NormalVoltage_Started);
			//if (NormalVoltage_Started == FALSE)
			//{
			//	Rte_Call_rptimer_TmExt_Timer_Start(eTimerNormalVoltage);
			//}
		
			//Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerNormalVoltage,300,&NormalVoltage_Exp);
			//if (NormalVoltage_Exp == TRUE)
			//{
				Rte_Write_ppBattState_BatteryState(l_BatteryState_U8);		 //Normal voltage
				Rte_Write_ppBattState_IsLowVoltage(FALSE);
				Rte_Write_ppBattState_IsHighVoltage(FALSE);
			//}
		}
	}

	battmdl_dem_process(Voltage);

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
/// @brief  Transitional state to clean up after diagnostic state. Component behavior to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;

}

static void battmdl_dem_process(TIOAppData volt)
{
	boolean fl_timerStarted = FALSE;
	boolean fl_timerExp = FALSE;
	boolean fl_ignState_dem = FALSE;
	boolean fl_battBelow9V_timerStarted = FALSE;
	boolean fl_battOver16V_timerStarted = FALSE;
	boolean fl_battBelow9V_timerExp = FALSE;
	boolean fl_battOver16V_timerExp = FALSE;
  
	if (l_dem_voltage_abnormal == FALSE)
	{
		if (dem_batt_in_mormal_5s == FALSE)
		{
			Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerBattMdlDemWait,&fl_timerStarted);
			if (fl_timerStarted == FALSE)
			{
				Rte_Call_rptimer_TmExt_Timer_Start(eTimerBattMdlDemWait);
			}

			Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerBattMdlDemWait,1000,&fl_timerExp);
			if (fl_timerExp == TRUE)
			{
				dem_batt_in_mormal_5s = TRUE;
				Rte_Call_rptimer_TmExt_Timer_Stop(eTimerBattMdlDemWait);
			}
		}
	}
	else
	{
		dem_batt_in_mormal_5s = FALSE;
		Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerBattMdlDemWait,&fl_timerStarted);/*batt shake in 1s*/
		if (fl_timerStarted == TRUE)
		{
			Rte_Call_rptimer_TmExt_Timer_Stop(eTimerBattMdlDemWait);
		}
	}
	Rte_Write_ppSR_BattState_Dem_state(dem_batt_in_mormal_5s);
	

	Rte_Read_rpSR_IgnState_Dem_state(&fl_ignState_dem);
	if ( TRUE == Rte_Call_EcuMExt_GetNvmReadAllStatus_Operation() )
	{
		if((fl_ignState_dem == TRUE))
		{
			if (volt < DEM_BATT_LOW_VOLTAGE)
			{ 
				Rte_Call_rptimer_TmExt_Timer_Stop(eTimerBattOver16V);
				Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerBattBelow9V,&fl_battBelow9V_timerStarted);
				if (fl_battBelow9V_timerStarted == FALSE)
				{
					Rte_Call_rptimer_TmExt_Timer_Start(eTimerBattBelow9V);
				}
				Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerBattBelow9V,1000,&fl_battBelow9V_timerExp);
				if (fl_battBelow9V_timerExp == TRUE)
				{
					(void)Dem_SetEventStatus(CAN_LOST_DTC_BELOW_9V,RESULT_FAILED);
//					Rte_Call_rptimer_TmExt_Timer_Stop(eTimerBattBelow9V);
				}
			}
			else if (volt > DEM_BATT_HIGH_VOLTAGE)
			{
				Rte_Call_rptimer_TmExt_Timer_Stop(eTimerBattBelow9V);
				Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerBattOver16V,&fl_battOver16V_timerStarted);
				if (fl_battOver16V_timerStarted == FALSE)
				{
					Rte_Call_rptimer_TmExt_Timer_Start(eTimerBattOver16V);
				}
				Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerBattOver16V,1000,&fl_battOver16V_timerExp);
				if (fl_battOver16V_timerExp == TRUE)
				{
					(void)Dem_SetEventStatus(CAN_LOST_DTC_ABOVE_16V,RESULT_FAILED);
//					Rte_Call_rptimer_TmExt_Timer_Stop(eTimerBattOver16V);
				}
			}
			else
			{
				Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerBattBelow9V,&fl_battBelow9V_timerStarted);
				Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerBattOver16V,&fl_battOver16V_timerStarted);
				if (fl_battBelow9V_timerStarted != FALSE)
				{
					Rte_Call_rptimer_TmExt_Timer_Stop(eTimerBattBelow9V);
				}
				if(fl_battOver16V_timerStarted != FALSE)
				{
					Rte_Call_rptimer_TmExt_Timer_Stop(eTimerBattOver16V);
				}
				
				(void)Dem_SetEventStatus(CAN_LOST_DTC_BELOW_9V,RESULT_NORMAL);
				(void)Dem_SetEventStatus(CAN_LOST_DTC_ABOVE_16V,RESULT_NORMAL);
			}
		}
		else
		{
			(void)Dem_SetEventStatus(CAN_LOST_DTC_BELOW_9V,RESULT_NORMAL);
			(void)Dem_SetEventStatus(CAN_LOST_DTC_ABOVE_16V,RESULT_NORMAL);
		}
	}
}

