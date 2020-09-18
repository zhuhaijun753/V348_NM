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
#include "Rte_DimDerateMdl.h"
#include "Rte_DimDerateMdl_Type.h"
#include "CmpLib.h"
#include "linear_interpolation.h"
#include "DimDerateMdl_cfg.h"
#include "Dio.h"
#include "Pwm.h"
#include "Dem.h"
#include "hmi_popup_screen_state.h"
#include "TtMdl.h"
#include "ChimeLogical.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(DimDerateMdl)

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static void Dim_InputProcess_TFT_Temperature(void);
static void Dim_DeratedDutyProcess(void);
static void Dim_Set_TFT_And_LedBar_And_TT_PWM(void);
static void Dim_Set_TFT_and_TT_PWM(void);
static void Dim_IgnOn_Process(void);
static void Dim_IgnOff_Process(void);
static void Dim_Turnoff_PWM(void);
static void Dim_Turnoff_LedBar(void);
static void Dim_Common_init(void);
static void Dim_Turnoff_TFT_and_LedBar_if_Warnning(void);
static void Dim_check_timer_isRunning(void);
static boolean Dim_Check_Sys_IsWarning(void);
static void Dim_change_backlight(void);
static void Dim_get_dimming_state(void);
static void Dim_calc_bgLumpValue(void);
static void Dim_pkg_process_Diagntask(void);
static void Dim_pkg_process_KSSlowTimetask(void);
static uint8 Dim_check_bgLumpValue(uint8 bgLumpValue);


//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define cPROCESS_SAMPLE_CNT                   (uint8)10
#define cSENSOR_FAIL_SAMPLE_CNT               (uint8)10
#define cDEFAULT_LCD_TEMP                     (uint16)650
//#define cLCD_TEMP_ADC_MAX                     (uint16)3763			/* -35 deg C */
//#define cLCD_TEMP_ADC_MIN                     (uint16)207			/* +125 deg C */
#define cLCD_TEMP_ADC_MAX                     (uint16)3928			/* -40 deg C */
#define cLCD_TEMP_ADC_MIN                     (uint16)224			/* +125 deg C */
#define cDERATING_LUT_LEN                     (uint8)16
#define cDERATING_LUT_TEMP                    (uint8)0
#define cDERATING_LUT_ADC                     (uint8)1
#define cDERATING_LUT_DUTY                    (uint8)2
#define cDEFAULT_PWM_MIN_DERATE               (uint16)100			//>85 deg C, PWM = 10%
#define cDEFAULT_PWM_MAX_DERATE               (uint16)1000
#define cDEFAULT_LCD_TEMERATURE_T1            (uint8)65
#define cDEFAULT_LCD_TEMERATURE_T2            (uint8)85
#define cCoefPwmDutyDelta					  (uint8)3
#define cLCD_TEMP_ADC_NOMAL_MAX				  (uint16)849		/* 65deg C */
#define cLCD_TEMP_ADC_NOMAL_LV				  (uint16)755		/* 70 deg C */
#define cLCD_TEMP_ADC_NOMAL_MIN				  (uint16)533		/* 85 deg C */
#define cLCD_TEMP_ADC_NOMAL_LV1				  (uint16)3763		/*-30 C */
#define cLCD_TEMP_ADC_NOMAL_LV2				  (uint16)2995		/*0 */
#define cLCD_TEMP_ADC_NOMAL_LV3				  (uint16)2816		/*5*/
#define cLCD_TEMP_ADC_NOMAL_LV4				  (uint16)1509		/*40*/
#define cLCD_TEMP_ADC_NOMAL_LV5				  (uint16)1350		/*45*/
#define ILLUMINATION_FULL_PWM	 			  (uint16)500
#define ILLUMINATION_MIN_PWM	 			  (uint16)50
#define Dim_TimeOut_60S    				      (uint16)60000
#define Dim_Invalid_value					  (uint16)65535
#define RESULT_NORMAL             			  (uint8)(2)
#define RESULT_FAILED            			  (uint8)(3)
#define BACKLIGHT_SENSOR_FAULT        		  (uint8)(3)
#define Dim_TimeOut_5S    				      (uint16)5000
#define Dim_TimeOut_10S    				      (uint16)10000
#define BCM_PosLmpSts_On					  (uint8)2
#define BCM_PosLmpSts_Off					  (uint8)1
#define BCM_FIRST_BATTARY_ON			      (uint8)1
#define LIGHT_LUMINOUSNESS                    (uint8)85
#define MAX_DIMMING_DAY						  (uint16)800
#define MAX_DIMMING_NIGHT					  (uint16)326
#define DimmingDay                            (uint8)0
#define DimmingNight						  (uint8)1
#define ILLUMINATION_MAX_Lump	 			  (uint8)100
#define ILLUMINATION_MIN_Lump	 			  (uint8)5
#define ILLUMINATION_MID_Lump	 			  (uint8)50
#define Dim_Factor							  (uint8)10
#define OAT_MODE							  (uint8)3
#define ILLUMINATION_INVALID_Lump	 		  (uint8)0xFF

#define eIODutyOutId_MDO_PWM_Backlight_TACHO	(uint8)7
#define eIODutyOutId_MDO_PWM_Backlight_SPEED	(uint8)8


//======================================================================================================================
//  PRIVATE
//======================================================================================================================
static TIOAppData l_LcdTempIn_ta[cPROCESS_SAMPLE_CNT]={0};
static uint16 l_LcdTempAvr_u16=0;
static uint8 l_SampleCnt_u8=0;
static uint8 l_FailSampleCnt_u8=0;
static uint16 l_LcdRecommendDuty_u16=0;
static boolean l_IsTempSensorFail_bool=0;
static sint8 l_Temperature_T_u8=0;
static uint16 l_DimPwmLv1_u16=0;
static uint16 l_DimPwmLv2_u16=0;
static uint16 l_Temperatureadc1_u16=0;
static uint16 l_Temperatureadc2_u16=0;
static uint16 l_attenuation1_u16=0;
static eIgnState l_pre_dimming_ign_state=0;
boolean dim_IGN_OFF_5s = FALSE;
static boolean l_dim_trans_from_off_to_on=0;
static boolean 	l_CheckStateFinish_u8=0;
static uint8 l_pre_animationFlag=0;
static uint8 BatteryState_fg = 0;


#pragma ghs section bss=".buram"
static uint16 l_dimming_LCD_base_duty_U16=0;
static uint16 l_dimming_pre_LCD_base_duty_U16=0;
static boolean l_first_nvr_rece=FALSE;
static boolean l_first_dimming=FALSE;
static uint8 l_dimming_state=0;
static uint8 l_lcd_Lump_value = 0;
static uint8 Pre_bgLumplvl_value=50;

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
	Dim_Common_init();

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
    return  E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{	
	EBatteryState fl_Battery_state;		
	Dcm_SesCtrlType fl_Diag_Session = DEFAULT_SESSION;
	uint8 fl_BCM_CarModeIndicator = 0;

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);	
	Rte_Call_rpCS_GetCurrentSessionType_Operation(&fl_Diag_Session);
	Rte_Read_rpSR_CANMSG_BCM_0x584_ComIn_BCM_CarModeIndicator(&fl_BCM_CarModeIndicator);

	
	if(fl_Battery_state != eBatteryState_Normal)
	{
		Dim_Turnoff_PWM();
	}
	else if(fl_BCM_CarModeIndicator == OAT_MODE) 
	{
		Dim_Turnoff_PWM();
		Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_TELLTALE_EN_PWM, 0x00);
	}
	else
	{
		if(fl_Diag_Session == EOL_SESSION || fl_Diag_Session == EXTENDED_SESSION)
		{
			Dim_pkg_process_Diagntask();
		}
		else
		{
			Dim_pkg_process_KSSlowTimetask();
		}
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


/*============================================================================
**
** Function Name:      void Dim_pkg_process_Diagntask
**
** Visibility:
**
** Description:        Dim  process Diagnisic task
**
** Invocation:
**
**
** Critical Section:   None
**
**==========================================================================*/
static void Dim_pkg_process_Diagntask(void)
{
	DCM_IOC_Type DID_Value;
	uint16 fl_dimderate_duty;
	uint16 fl_dimderate_display_duty;
	
	Rte_Read_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(&DID_Value);		/*DID for Diagnostic*/
	if(DID_Value.DID_IOC_Number == 0x5182)
	{
		if(DID_Value.DID_IOC_Parament == 0x03)
		{
			fl_dimderate_duty = DID_Value.DID_IOC_State[0];
			if(fl_dimderate_duty >=0 && fl_dimderate_duty <=5)			    //00h-05h: luminance of 5%.
			{
				fl_dimderate_display_duty = 50;
			}
			else if(fl_dimderate_duty >5 && fl_dimderate_duty <= 95) 		//06h-5Fh: luminance of detailed data value
			{
				fl_dimderate_display_duty = fl_dimderate_duty * 10;
			}
			else if(fl_dimderate_duty > 95 && fl_dimderate_duty <= 255)	//60h-FFh: invalid：luminance of 95%													//65h-FFh: invalid：luminance of 5%
			{
				fl_dimderate_display_duty = 950;
			}
			
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_TFT_BL1_PWM,fl_dimderate_display_duty);
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_BACKLIGHT_SCALE_PWM,fl_dimderate_display_duty);
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_BACKLIGHT_DECORATION_PWM,fl_dimderate_display_duty);
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_BAR_EN_PWM,fl_dimderate_display_duty); 
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_TELLTALE_EN_PWM,fl_dimderate_display_duty);
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_PWM_Backlight_TACHO, fl_dimderate_display_duty);
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_PWM_Backlight_SPEED, fl_dimderate_display_duty);
		}
		else if(DID_Value.DID_IOC_Parament == 0x00)
		{
			Dim_pkg_process_KSSlowTimetask();
		}
	}
	else
	{
		Dim_pkg_process_KSSlowTimetask();
	}
}


/*============================================================================
**
** Function Name:      void Dim_pkg_process_KSSlowTimetask(void)
**
** Visibility:
**
** Description:        Dim process KSSlowTimetask
**
** Invocation:		   None
**
**
** Critical Section:   None
**
**==========================================================================*/
static void Dim_pkg_process_KSSlowTimetask(void)
{
	eIgnState fl_IGN_state;

	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

	if((fl_IGN_state == eIGN_RUN) && (l_pre_dimming_ign_state == eIGN_OFF))
	{
		l_dim_trans_from_off_to_on = TRUE;
	}
	else if((fl_IGN_state == eIGN_OFF) && (l_pre_dimming_ign_state == eIGN_RUN))
	{
		l_dim_trans_from_off_to_on = FALSE;
	}


	//else
	{		
		if(fl_IGN_state == eIGN_RUN)
		{
			Dim_IgnOn_Process();
			dim_IGN_OFF_5s = FALSE;
		}
		else
		{			
			Dim_IgnOff_Process();
		}
	}
	l_pre_dimming_ign_state = fl_IGN_state; 
}


/*============================================================================
**
** Function Name:      void Dim_IgnOn_Process(void)
**
** Visibility:
**
** Description:        Dim Process when ign is on
**
** Invocation:
**
**
** Critical Section:   None
**
**==========================================================================*/
static void Dim_IgnOn_Process(void)
{
	 Dim_get_dimming_state();
	/*Dim calc bgLumpValue*/
	Dim_calc_bgLumpValue();
	/*Dim input process*/
	Dim_InputProcess_TFT_Temperature();
	/*Dim set TFT or Led bar value*/
	Dim_Set_TFT_And_LedBar_And_TT_PWM();
	/*Dim check timer isRunning*/
	Dim_check_timer_isRunning();
}


/*============================================================================
**
** Function Name:      void Dim_IgnOff_Process(void)
**
** Visibility:
**
** Description:        Dim Process when ign is off
**
** Invocation:
**
**
** Critical Section:   None
**
**==========================================================================*/
static void Dim_IgnOff_Process(void)
{
	/*Dim turn off led bar*/
	Dim_Turnoff_LedBar();
	/*Dim turn off TFT and led if there is not warnning*/
	Dim_Turnoff_TFT_and_LedBar_if_Warnning();
}

/*============================================================================
**
** Function Name:      void Dim_Common_init(void)
**
** Visibility:
**
** Description:        Dim init Common value
**
** Invocation:
**
** Inputs/Outputs:     
**
**==========================================================================*/
static void Dim_Common_init(void)
{
	 uint8 i;
	
	 for (i = 0; i < cPROCESS_SAMPLE_CNT; i++)
	 {
	   l_LcdTempIn_ta[i] = cDEFAULT_LCD_TEMP;
	 }
	
	 l_LcdTempAvr_u16 = cDEFAULT_LCD_TEMP;
		
	 l_SampleCnt_u8 = 0;
	
	 l_FailSampleCnt_u8 = 0;
			
	 /* duty of default temperature */
	 l_LcdRecommendDuty_u16 = cDEFAULT_PWM_MAX_DERATE;
		
	 l_IsTempSensorFail_bool = FALSE;
	
	 l_dimming_LCD_base_duty_U16 = ILLUMINATION_FULL_PWM;

	 l_pre_dimming_ign_state = eIGN_OFF;

	 l_CheckStateFinish_u8 = FALSE;

	 dim_IGN_OFF_5s = FALSE;

	 Pre_bgLumplvl_value=50;
	 BatteryState_fg = 1;
	 
	 l_dimming_state = DimmingDay;
}


/*============================================================================
**
** Function Name:      void Dim_InputProcess_TFT_Temperature(void)
**
** Visibility:
**
** Description:        Dim process TFT Temp
**
** Invocation:
**
** Inputs/Outputs:     
**
**==========================================================================*/
static void Dim_InputProcess_TFT_Temperature(void)
{
	TIOAppData fl_LcdTempIn_t;
	uint32 fl_LcdTempSum_u32;
	uint8 i;
	fl_LcdTempSum_u32 = 0;
	fl_LcdTempIn_t = 0;

    /* read tft temperature */
    Rte_Call_rpiCS_TIIoHwAb_AIn_Impl_GetVoltage(eIOVoltageInId_TFT_THERMISTOR, &fl_LcdTempIn_t);

	/* invalid range */
    if ((fl_LcdTempIn_t < cLCD_TEMP_ADC_MIN) || (fl_LcdTempIn_t > cLCD_TEMP_ADC_MAX))
    {
      l_FailSampleCnt_u8++;

      if (l_FailSampleCnt_u8 >= cSENSOR_FAIL_SAMPLE_CNT)
      {
        /* clr fail sample cnt */
        l_FailSampleCnt_u8 = 0;

        /* clr sample cnt */
        l_SampleCnt_u8 = 0;

        l_IsTempSensorFail_bool = TRUE;
      }
    }
    /* valid range */
    else
    {
		l_SampleCnt_u8++;

		/* store the sample in the buffer */
		l_LcdTempIn_ta[l_SampleCnt_u8 - 1] = fl_LcdTempIn_t;

		/* processing rate */
		if (l_SampleCnt_u8 >= cPROCESS_SAMPLE_CNT)
		{
			/* clr sample cnt */
			l_SampleCnt_u8 = 0;

			/* clr fail sample cnt */
			l_FailSampleCnt_u8 = 0;

			/* clr Sensor fail flag */
			l_IsTempSensorFail_bool = FALSE;

			/* average the samples */
			for (i = 0; i < cPROCESS_SAMPLE_CNT; i++)
			{
			  /* summing */
			  fl_LcdTempSum_u32 = fl_LcdTempSum_u32 + (uint32)l_LcdTempIn_ta[i];
			}

			/* average */
			l_LcdTempAvr_u16 = (uint16)(fl_LcdTempSum_u32 / cPROCESS_SAMPLE_CNT);

			/*dim calc Temperature Waring for TFT*/
			Dim_DeratedDutyProcess();
		}
		
    }
}


/*
The following is an algorithm for converting ADC values collected on the TFT screen into temperature. 
Due to the differences in temperature conversion between [-35~0],(0~40],(40~65) and [65~85], 
it can be divided into four types
When collecting adc 2995 <= adc <= 3763, 1 algorithm is adopted
When collecting adc 1509 <= adc < 2995,2 algorithms are adopted
When collecting adc 841 < adc < 1509, 3 algorithms are adopted
When collecting adc 519 <= adc <= 841, 4 algorithms are adopted
2995，2816 , 1350 , 746 Refer to the XLS of the ADC value of the reference screen temperature
y=kx+b For example 65=841k+b;85=519k+b ==> k=-20/322  b=65+ 841*20/322

//			 	 (2995-adc)*30			     	(2816-adc)*15
//1: T[-35~0] =  ------------	2: T(0-40] = 5 +  ------------  					  
//					  768 						  	 576
//				 	 (1350-adc)*5					  (841 -adc)*20
//3: T(40~65) = 45 +  ------------4:  T[65~85] = 65 +  ------------
//				      	147								   322

*/
#if 0

static void Dim_DeratedDutyProcess (void)
{
	l_DimPwmLv1_u16 = cDEFAULT_PWM_MAX_DERATE;				//1
	l_DimPwmLv2_u16 = cDEFAULT_PWM_MIN_DERATE;				//20%
	l_Temperatureadc1_u16 = cLCD_TEMP_ADC_NOMAL_MAX;		//65度对应的ADC值
	l_Temperatureadc2_u16 = cLCD_TEMP_ADC_NOMAL_MIN;		//85度对应的ADC值
	l_attenuation1_u16 = 40000;								//0.04
//	l_attenuation2_u8 = 0;									//0
  
  /* sensor failure */
  if (FALSE != l_IsTempSensorFail_bool)
  {
    /* minimum recommended duty in case of sensor failure */
    l_LcdRecommendDuty_u16 = cDEFAULT_PWM_MIN_DERATE;
  }											
  /* sensor normal */						
  else	
  {
	/*adc convsersion temperature 65<=T<=85*/  //PWM_max VS temp  = R1 - A1*(T-T1)   R1 = 1 ,R2 = 10%
	if((l_LcdTempAvr_u16>=cLCD_TEMP_ADC_NOMAL_MIN)&&(l_LcdTempAvr_u16<=cLCD_TEMP_ADC_NOMAL_MAX))
	{
		l_Temperature_T_u8 = cDEFAULT_LCD_TEMERATURE_T1+(20*(cLCD_TEMP_ADC_NOMAL_MAX-l_LcdTempAvr_u16))/322;  //A1 = -0.045 
	}

    if (l_LcdTempAvr_u16 >= l_Temperatureadc1_u16) 			//  T <65 
    {
      	l_LcdRecommendDuty_u16 = l_DimPwmLv1_u16;
    }
    //65  < T < 85
    else if((l_LcdTempAvr_u16 > l_Temperatureadc2_u16)&&(l_LcdTempAvr_u16 < l_Temperatureadc1_u16))
    {
    	l_LcdRecommendDuty_u16 = 1000 -
			 ((uint32) ((l_Temperature_T_u8 - cDEFAULT_LCD_TEMERATURE_T1) * l_attenuation1_u16 * 1000)) / 1000000;

		//l_LcdRecommendDuty_u16 = (l_LcdRecommendDuty_u16 /100) * (l_dimming_LCD_base_duty_U16 /100) * 10 ;
		
		l_LcdRecommendDuty_u16 = l_LcdRecommendDuty_u16 * l_dimming_LCD_base_duty_U16 /1000 ;			//SysRS_03_Illumination-80009
    }
	else if((l_LcdTempAvr_u16 <= l_Temperatureadc2_u16))    // T > 85
	{
		l_LcdRecommendDuty_u16 = l_DimPwmLv2_u16;
	}
  }
}
#endif

/*
The following is an algorithm for converting ADC values collected on the TFT screen into temperature. 
Due to the differences in temperature conversion between [-40~65] and [65~85], 
it can be divided into two types
When collecting adc 849 <= adc <= 3920, 1 algorithm is adopted
When collecting adc 533 < adc < 849,2 algorithms are adopted


//			 	 			     	        
//1: T[-40~65] =  R1	 2: T(65~85] = R1 - A1*(T-T1)  	3:	 T(>85] = R2			  
// R1 = 1 ,R2 = 10%  ,A1 = -0.045, T1 = 65					 						  	

*/

static void Dim_DeratedDutyProcess (void)
{
	l_DimPwmLv1_u16 = cDEFAULT_PWM_MAX_DERATE;				//1
	l_DimPwmLv2_u16 = cDEFAULT_PWM_MIN_DERATE;				//10%
	l_Temperatureadc1_u16 = cLCD_TEMP_ADC_NOMAL_MAX;		//65度对应的ADC值
	l_Temperatureadc2_u16 = cLCD_TEMP_ADC_NOMAL_MIN;		//85度对应的ADC值
	l_attenuation1_u16 = 900;								//0.9
//	l_attenuation2_u8 = 0;									//0
  
  /* sensor failure */
  if (FALSE != l_IsTempSensorFail_bool)
  {
    /* minimum recommended duty in case of sensor failure */
    l_LcdRecommendDuty_u16 = cDEFAULT_PWM_MIN_DERATE;
  }											
  /* sensor normal */						
  else	
  {
	/*adc convsersion temperature 65<=T<=85*/  //PWM_max VS temp  = R1 - A1*(T-T1)   R1 = 1 ,R2 = 10%
	if((l_LcdTempAvr_u16>=cLCD_TEMP_ADC_NOMAL_MIN)&&(l_LcdTempAvr_u16<=cLCD_TEMP_ADC_NOMAL_MAX))
	{
		l_Temperature_T_u8 = cDEFAULT_LCD_TEMERATURE_T1+(20*(cLCD_TEMP_ADC_NOMAL_MAX-l_LcdTempAvr_u16))/316;  //K = -316/20
	}

    if (l_LcdTempAvr_u16 >= l_Temperatureadc1_u16) 			//  T <65 
    {
      	l_LcdRecommendDuty_u16 = l_DimPwmLv1_u16;
    }
    //65  < T < 85
    else if((l_LcdTempAvr_u16 > l_Temperatureadc2_u16)&&(l_LcdTempAvr_u16 < l_Temperatureadc1_u16))
    {

		l_LcdRecommendDuty_u16 = 1000 -((uint32) ((l_Temperature_T_u8 - cDEFAULT_LCD_TEMERATURE_T1) * l_attenuation1_u16 / 20 ));
	
    }
	else if((l_LcdTempAvr_u16 <= l_Temperatureadc2_u16))    // T > 85
	{
		l_LcdRecommendDuty_u16 = l_DimPwmLv2_u16;
	}

		l_LcdRecommendDuty_u16 = (uint32) (l_LcdRecommendDuty_u16 * l_dimming_LCD_base_duty_U16 /1000) ;			//SysRS_03_Illumination-80009
  }
}

/*============================================================================
**
** Function Name:      void Dim_Set_TFT_And_LedBar_And_TT_PWM(void)
**
** Visibility:
**
** Description:        Dim process TFT PWM and led bar PWM
**
** Invocation:
**
** Inputs/Outputs:     
**
**==========================================================================*/
static void Dim_Set_TFT_And_LedBar_And_TT_PWM(void)
{
	uint8 fl_animationFlag;
	
	if (l_dimming_LCD_base_duty_U16 > l_LcdRecommendDuty_u16)
	{
	  	l_dimming_LCD_base_duty_U16 = l_LcdRecommendDuty_u16;

		if(l_dimming_LCD_base_duty_U16 < ILLUMINATION_MIN_PWM)
			l_dimming_LCD_base_duty_U16 = ILLUMINATION_MIN_PWM;
	}

	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);

	if(fl_animationFlag == FALSE && l_pre_animationFlag == TRUE )		//Animation is over
	{
		l_CheckStateFinish_u8 = TRUE;
	}
	else
	{
		l_CheckStateFinish_u8 = FALSE;									//Animation is running 
	}
	l_pre_animationFlag = fl_animationFlag;
	

	if(l_dim_trans_from_off_to_on == TRUE)			//IGN OFF - ON
	{
		if(l_CheckStateFinish_u8 == TRUE)			//Animation is over
		{
			l_dim_trans_from_off_to_on = FALSE;
		}
		else										//turn off backlight
		{	
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_BACKLIGHT_SCALE_PWM, 0x00);
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_BACKLIGHT_DECORATION_PWM, 0x00);
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_BAR_EN_PWM, 0x00);
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_PWM_Backlight_TACHO, 0x00);
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_PWM_Backlight_SPEED, 0x00);
			Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_TELLTALE_EN_PWM, l_dimming_LCD_base_duty_U16);	
		}
	}
	else											//turn on backlight
	{
		Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_BACKLIGHT_SCALE_PWM,l_dimming_LCD_base_duty_U16);
		Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_BACKLIGHT_DECORATION_PWM,l_dimming_LCD_base_duty_U16);
		Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_BAR_EN_PWM,l_dimming_LCD_base_duty_U16);	
		Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_TELLTALE_EN_PWM,l_dimming_LCD_base_duty_U16);
		Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_PWM_Backlight_TACHO, l_dimming_LCD_base_duty_U16);
		Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_PWM_Backlight_SPEED, l_dimming_LCD_base_duty_U16);
	}	

	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_TFT_BL1_PWM,l_dimming_LCD_base_duty_U16);	
}

/*============================================================================
**
** Function Name:      void Dim_Set_TFT_and_TT_PWM(void)
**
** Visibility:
**
** Description:        Dim process TFT PWM
**
** Invocation:
**
** Inputs/Outputs:     
**
**==========================================================================*/
static void Dim_Set_TFT_and_TT_PWM(void)
{
	if (l_dimming_LCD_base_duty_U16 > l_LcdRecommendDuty_u16)
	{
	  	l_dimming_LCD_base_duty_U16 = l_LcdRecommendDuty_u16;

		if(l_dimming_LCD_base_duty_U16 < ILLUMINATION_MIN_Lump)
			l_dimming_LCD_base_duty_U16 = ILLUMINATION_MIN_Lump;
	}

	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_TFT_BL1_PWM,l_dimming_LCD_base_duty_U16);
	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_TELLTALE_EN_PWM,l_dimming_LCD_base_duty_U16);
}

/*============================================================================
**
** Function Name:      void Dim_Turnoff_PWM(void)
**
** Visibility:
**
** Description:        Dim turn off TFT and Led Bar
**
** Invocation:
**
**
** Critical Section:   None
**
**==========================================================================*/
static void Dim_Turnoff_PWM(void)
{
	EBatteryState fl_Battery_state;	
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);	
	
	/*turn off TFT*/
	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_TFT_BL1_PWM, 0x00);	
	/*turn off Left&Right Small Scale*/
	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_BACKLIGHT_SCALE_PWM, 0x00);
	/*turn off Decoration*/
	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_BACKLIGHT_DECORATION_PWM, 0x00);
	/*turn off LED BAR*/
	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_BAR_EN_PWM, 0x00);
	/*turn off TT*/
	if(eBatteryState_OverVoltage == fl_Battery_state || eBatteryState_UnderVoltage == fl_Battery_state)  
	{
		Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_TELLTALE_EN_PWM, 0x00);
	}
	else
	{
		Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_TELLTALE_EN_PWM,l_dimming_LCD_base_duty_U16);
	}

	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_PWM_Backlight_TACHO, 0x00);
	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_PWM_Backlight_SPEED, 0x00);

}

/*============================================================================
**
** Function Name:      void Dim_Turnoff_LedBar(void)
**
** Visibility:
**
** Description:        Dim turn off Led Bar
**
** Invocation:
**
**
** Critical Section:   None
**
**==========================================================================*/
static void Dim_Turnoff_LedBar(void)
{
	/*turn off Left&Right Small Scale*/
	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_BACKLIGHT_SCALE_PWM, 0x00);
	/*turn off Decoration*/
	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_BACKLIGHT_DECORATION_PWM, 0x00);
	/*turn off Led Bar*/
	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_BAR_EN_PWM, 0x00);

	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_PWM_Backlight_TACHO, 0x00);
	Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_PWM_Backlight_SPEED, 0x00);
}



void Sp_DimDataSet_NvMNotifyJobFinished_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{

}

/*============================================================================
**
** Function Name:      void Dim_Turnoff_TFT_and_LedBar_if_Warnning(void)
**
** Visibility:
**
** Description:        Dim turn off TFT and Led Bar Action in 5s
**
** Invocation:
**
**
** Critical Section:   None
**
**==========================================================================*/
static void Dim_Turnoff_TFT_and_LedBar_if_Warnning(void)
{
	boolean fl_dim_warning_status;
	boolean fl_timer_resDef_started_5S_bool;
	boolean fl_timer_resDef_isElapsed_5S_bool;

	fl_dim_warning_status = Dim_Check_Sys_IsWarning();	
	if(fl_dim_warning_status == FALSE)
	{
		//when there is no warning, turn off Dim TFT and LED after 5s
		Rte_Call_rp_TmExt_Wrap_Timer_IsStarted(eTimerDimWait5S, &fl_timer_resDef_started_5S_bool); 
	
		if(fl_timer_resDef_started_5S_bool)
		{
			// abnormal >= 5s
			Rte_Call_rp_TmExt_Wrap_Timer_IsElapsed(eTimerDimWait5S, Dim_TimeOut_5S, &fl_timer_resDef_isElapsed_5S_bool);
			if(fl_timer_resDef_isElapsed_5S_bool)
			{
				dim_IGN_OFF_5s = TRUE;
				Dim_Turnoff_PWM();
				Rte_Call_rpCS_TIIoHwAb_PwmOut_SetDuty(eIODutyOutId_MDO_SHIFT_TELLTALE_EN_PWM, 0x00);
			}
			else
			{
				dim_IGN_OFF_5s = FALSE;
				Dim_change_backlight();
			}
		}
		else
		{
			Rte_Call_rp_TmExt_Timer_Start(eTimerDimWait5S);
		}
	}
	else
	{
		dim_IGN_OFF_5s = FALSE;
		Dim_change_backlight();
		Rte_Call_rp_TmExt_Wrap_Timer_IsStarted(eTimerDimWait5S, &fl_timer_resDef_started_5S_bool);
		if(fl_timer_resDef_started_5S_bool)
			Rte_Call_rp_TmExt_Timer_Stop(eTimerDimWait5S);
	}
}

/*============================================================================
**
** Function Name:      void Dim_check_timer_isRunning(void)
**
** Visibility:
**
** Description:        Dim check timer whether is running
**
** Invocation:
**
**
** Critical Section:   None
**
**==========================================================================*/
static void Dim_check_timer_isRunning(void)
{
	boolean fl_timer_resDef_started_5S_bool;

	//check timer is running , if running ,then turn off timer
	Rte_Call_rp_TmExt_Wrap_Timer_IsStarted(eTimerDimWait5S, &fl_timer_resDef_started_5S_bool);
	if(fl_timer_resDef_started_5S_bool)
		Rte_Call_rp_TmExt_Timer_Stop(eTimerDimWait5S);	
}

/*============================================================================
**
** Function Name:      boolean Dim_Check_Sys_IsWarning(void)
**
** Visibility:
**
** Description:        check system is in warning or not 
**
** Invocation:		   return warning status
**
**
** Critical Section:   None
**
**==========================================================================*/
static boolean Dim_Check_Sys_IsWarning(void)
{
	boolean fl_dim_popup_warning_status;
	boolean fl_dim_tt_warning_status;	
	boolean fl_dim_chime_warning_status;
	boolean fl_dim_warning_status = FALSE;
	
	fl_dim_popup_warning_status = hmi_get_popup_warning_status();
	fl_dim_tt_warning_status = CTtMdl_Ignoff_Tt_State();
	fl_dim_chime_warning_status = CChimeMdl_Ignoff_Chime_State__();

	if((fl_dim_popup_warning_status == TRUE) || (fl_dim_tt_warning_status == TRUE) || (fl_dim_chime_warning_status == TRUE))
	{
		fl_dim_warning_status = TRUE;
	}
	else
	{
		fl_dim_warning_status = FALSE;
	}
	return fl_dim_warning_status;
}


/*============================================================================
**
** Function Name:      void Dim_change_backlight(void))
**
** Visibility:
**
** Description:        Dim change backlight
**
** Invocation:		   None
**
**
** Critical Section:   None
**
**==========================================================================*/

static void Dim_change_backlight(void)
{
	Dim_calc_bgLumpValue();
	/*Dim Set TFT and TT PWM*/
	Dim_Set_TFT_and_TT_PWM();
}


/*============================================================================
**
** Function Name:      void Dim_get_dimming_state(void)
**
** Visibility:
**
** Description:        Dim get dimming state
**
** Invocation:		   None
**
**
** Critical Section:   None
**
**==========================================================================*/
static void Dim_get_dimming_state(void)
{
	uint8 fl_BCM_PosLmpSts_value=0;
	uint8 fl_BCM_PosLmpSts_tout=0;
	uint8 fl_BCM_PosLmpSts_nr=0;

	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpSts(&fl_BCM_PosLmpSts_value);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(&fl_BCM_PosLmpSts_nr);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(&fl_BCM_PosLmpSts_tout);

	if((fl_BCM_PosLmpSts_nr == RTE_E_NEVER_RECEIVED) || (fl_BCM_PosLmpSts_tout == RTE_E_TIMEOUT))
	{
		l_dimming_state = DimmingDay;
	}
	else
	{
		if(fl_BCM_PosLmpSts_value == BCM_PosLmpSts_On)			//SysRS_08_Illumination_80006
		{
			l_dimming_state = DimmingNight;
		}
		else //if(fl_BCM_PosLmpSts_value == BCM_PosLmpSts_Off)
		{
			l_dimming_state = DimmingDay;
		}
	}
}


/*============================================================================
**
** Function Name:      void Dim_calc_bgLumpValue(void)
**
** Visibility:
**
** Description:        Dim calc bgLumpValue
**
** Invocation:		   none
**
**
** Critical Section:   None
**
**==========================================================================*/
static void Dim_calc_bgLumpValue(void)
{
	uint8 fl_bgLumplvl_tout=0;
	uint8 fl_bgLumplvl_nr=0;
	uint8 fl_bgLumplvl_value=0;

	Rte_Read_rpSR_CANMSG_BCM_Sts_0x315_ComIn_BCM_BGLLumLvlSettingStatus(&fl_bgLumplvl_value);
	Rte_Read_rpSR_CANMSG_BCM_Sts_0x315_ComIn_NR(&fl_bgLumplvl_nr);
	Rte_Read_rpSR_CANMSG_BCM_Sts_0x315_ComIn_Tout(&fl_bgLumplvl_tout);
	
	if(fl_bgLumplvl_tout == RTE_E_TIMEOUT || fl_bgLumplvl_nr == RTE_E_NEVER_RECEIVED)
	{

		if(BatteryState_fg == 1)
		{
			l_lcd_Lump_value = ILLUMINATION_MAX_Lump; //080041 add by sw

		}
		else 
		{
			l_lcd_Lump_value = Pre_bgLumplvl_value;
		}
		
	}
	else 
	{
		l_lcd_Lump_value = Dim_check_bgLumpValue(fl_bgLumplvl_value);
		Pre_bgLumplvl_value = l_lcd_Lump_value;
		BatteryState_fg = 0;
	}


/*	if(fl_bgLumplvl_tout == RTE_E_TIMEOUT || fl_bgLumplvl_nr == RTE_E_NEVER_RECEIVED)
	{
		// if(l_first_nvr_rece == FALSE)
		// {
		// 	l_first_nvr_rece = TRUE;
		// 	if(l_dimming_state == DimmingNight)
		// 	{
				l_lcd_Lump_value = ILLUMINATION_MID_Lump;
		// 	}
		// 	else if(l_dimming_state == DimmingDay)
		// 	{
		// 		l_lcd_Lump_value = ILLUMINATION_MAX_Lump;
		// 	}
		// }
	}
	else
	{
		l_lcd_Lump_value = Dim_check_bgLumpValue(fl_bgLumplvl_value);
	} 
	*/
	if(l_dimming_state == DimmingNight)
	{		
		l_dimming_LCD_base_duty_U16 = l_lcd_Lump_value *5 /10 * Dim_Factor;		//SysRS_08_Illumination_80006
	}
	else if(l_dimming_state == DimmingDay)
	{
		l_dimming_LCD_base_duty_U16 = l_lcd_Lump_value * Dim_Factor;
	}	
	
	//l_dimming_LCD_base_duty_U16 = l_lcd_Lump_value * Dim_Factor;

	if(l_first_dimming == FALSE)
	{
		l_dimming_pre_LCD_base_duty_U16 = l_dimming_LCD_base_duty_U16;
		l_first_dimming = TRUE;
	}
	else
	{
		l_dimming_LCD_base_duty_U16 = (l_dimming_LCD_base_duty_U16 + 7*l_dimming_pre_LCD_base_duty_U16)/8;
	}

	l_dimming_pre_LCD_base_duty_U16 = l_dimming_LCD_base_duty_U16;

}


/*============================================================================
**
** Function Name:      uint8 Dim_check_bgLumpValue(uint8 bgLumpValue)
**
** Visibility:
**
** Description:        Dim check bgLumpValue 
**
** Invocation:		   return Dim bgLumpValue
**
**
** Critical Section:   None
**
**==========================================================================*/
static uint8 Dim_check_bgLumpValue(uint8 bgLumpValue)
{
	uint8 fl_bgLumpValue = 0;
	
	fl_bgLumpValue = bgLumpValue;
	if(fl_bgLumpValue < ILLUMINATION_MIN_Lump)
	{
		fl_bgLumpValue = ILLUMINATION_MIN_Lump;
	}
	else if(fl_bgLumpValue > ILLUMINATION_MAX_Lump)	
	{	
		if(fl_bgLumpValue == ILLUMINATION_INVALID_Lump)
		{
			if(l_dim_trans_from_off_to_on == TRUE)
			{
				fl_bgLumpValue = ILLUMINATION_MAX_Lump;
			}
			else
			{
				fl_bgLumpValue = Pre_bgLumplvl_value;
			}
		}
		else 
		{
			fl_bgLumpValue = ILLUMINATION_MAX_Lump;
		}
		
	}

	return fl_bgLumpValue;
}


