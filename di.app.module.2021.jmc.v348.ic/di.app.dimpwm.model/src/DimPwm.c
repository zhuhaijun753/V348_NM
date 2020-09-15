//---------------------------------------------------------------------------------------------------------------------
//
// VISTEON CORPORATION CONFIDENTIAL																																										                                                                                                                           // ________________________________
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
#include "CmpLib.h"
#include "Rte_DimPwm.h"


/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(DimPwm)

//static void f_dim_Day_and_Night_process(void);
//static void f_Dim_Process(void);

extern FUNC(void, AUTOMATIC) Pwm_SetDutyCycle
(
  VAR(uint8, AUTOMATIC) ChannelNumber,
  VAR(uint16, AUTOMATIC) DutyCycle
);

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
/* Macro define*/
#define DIMMING_LEVEL_1    88
#define DIMMING_LEVEL_2    220
#define DIMMING_LEVEL_3    360
#define DIMMING_LEVEL_4    520
#define DIMMING_LEVEL_5    640
#define DIMMING_LEVEL_6    700
#define DAYTIME_MODE       0x00
#define NIGHTTIME_MODE     0x01

//======================================================================================================================
// FORWARD DECLARATIONS OF PRIVATE FUNCTIONS
//======================================================================================================================
/* SCC_LeftSteeringWheelSwitchSig*/
//======================================================================================================================
// PRIVATE DATA
//======================================================================================================================
//static uint16 backlightDuty = DIMMING_LEVEL_4;
//static uint8  l_day_and_night_mode = DAYTIME_MODE;
//static uint16 l_day_backlight_duty = DIMMING_LEVEL_4;
//static uint16 l_night_backlight_duty = DIMMING_LEVEL_2;
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
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
static Std_ReturnType CmpActive(void)
{
	eIgnState fl_IGN_state;
	EBatteryState fl_Battery_state;	

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
//	f_dim_Day_and_Night_process();
	
	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{
//		backlightDuty = DIMMING_LEVEL_1; 	
//		Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(1,0);
//		Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(0,0);	
	}
//	else if(fl_Battery_state==eBatteryState_UnderVoltagePhase1 || fl_Battery_state== eBatteryState_OverVoltagePhase1)
//	{
//		backlightDuty = DIMMING_LEVEL_1;	
//		Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(1,1);
//		Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(0,1);	
//	}
	else
	{
		switch(fl_IGN_state)
		{
			case eIGN_OFF:
//				Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(1,0);
//				Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(0,0);
				break;
			case eIGN_RUN:
//				f_Dim_Process();	
//				Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(1,1);
//				Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(0,1);
				break;
			default:
				break;
		}
//		if(l_day_and_night_mode == DAYTIME_MODE)
//		{
//			backlightDuty = l_day_backlight_duty;
//		}
//		else/*Nighttime_mode*/
//		{
//			backlightDuty = l_night_backlight_duty;
//		}
//		
	}
//	Rte_Write_ppSR_BrightnessChange_SetDuty(backlightDuty);	
    return  E_OK;
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

#if 0
static void f_Dim_Process(void)
{
		Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(1,1);
		Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(0,1);
		uint8	l_brightenesschange_level_U8 = 0;
		uint8 	fl_CAN_BC_msg_status_U8 = 0;
		uint8 	fl_CAN_BC_nvr_status_U8 = 0;
		Rte_Read_rpSR_CANMSG_MP5_2_ComIn_Tout(&fl_CAN_BC_msg_status_U8);
		Rte_Read_rpSR_CANMSG_MP5_2_ComIn_NR(&fl_CAN_BC_nvr_status_U8);
		Rte_Read_rpSR_CANMSG_MP5_2_ComIn_MP5_BrightnessChange(&l_brightenesschange_level_U8);
		
		if((fl_CAN_BC_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_BC_msg_status_U8 == RTE_E_TIMEOUT))
		{
			if(l_day_and_night_mode == DAYTIME_MODE)
			{
				l_day_backlight_duty = l_day_backlight_duty;
			}
			else/*Nighttime_mode*/
			{
				l_night_backlight_duty = l_night_backlight_duty;
			}
		}
		else
		{
			if(l_day_and_night_mode == DAYTIME_MODE)
			{
				switch(l_brightenesschange_level_U8)
				{
					case 6:				
							l_day_backlight_duty = DIMMING_LEVEL_6;
							
					break;
								
					case 5:
							l_day_backlight_duty = DIMMING_LEVEL_5;
							
					break;

					case 4:
							l_day_backlight_duty = DIMMING_LEVEL_4;
							
					break;
							
					case 3:
							l_day_backlight_duty = DIMMING_LEVEL_3;
							
					break;
							
					case 2:
					    	l_day_backlight_duty = DIMMING_LEVEL_2;
							
					break;
						
					case 1:
							l_day_backlight_duty = DIMMING_LEVEL_1;
							
					break;
						
					case 0:
					case 7:
							l_day_backlight_duty = l_day_backlight_duty;/*keep last value*/
					break;
							
						
					default: 
					break; 
				}
			}
			else/*Nighttime_mode*/
			{
				switch(l_brightenesschange_level_U8)
				{
					case 6:				
							l_night_backlight_duty = DIMMING_LEVEL_6;
							
					break;
								
					case 5:
							l_night_backlight_duty = DIMMING_LEVEL_5;
							
					break;

					case 4:
							l_night_backlight_duty = DIMMING_LEVEL_4;
							
					break;
							
					case 3:
							l_night_backlight_duty = DIMMING_LEVEL_3;
							
					break;
							
					case 2:
					    	l_night_backlight_duty = DIMMING_LEVEL_2;

					break;
						
					case 1:
							l_night_backlight_duty = DIMMING_LEVEL_1;

					break;
						
					case 0:
					case 7:
							l_night_backlight_duty = l_night_backlight_duty;/*keep last value*/
					break;
							
						
					default: 
					break; 
				}
			}
		}		
}
#endif

#if 0
static void f_dim_Day_and_Night_process(void)
{
	 uint8	 l_Positionlampst_value_U8 = 0;
	 uint8	 fl_CAN_Positionlampst_nvr_status_U8 = 0;
	 uint8	 fl_CAN_Positionlampst_msg_status_U8 = 0;
	 Rte_Read_rpSR_CANMSG_IBCM2_ComIn_NR(&fl_CAN_Positionlampst_nvr_status_U8);
	 Rte_Read_rpSR_CANMSG_IBCM2_ComIn_Tout(&fl_CAN_Positionlampst_msg_status_U8);
	 Rte_Read_rpSR_CANMSG_IBCM2_ComIn_BCM_PositionLampSt(&l_Positionlampst_value_U8);
	 if((fl_CAN_Positionlampst_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_Positionlampst_msg_status_U8 == RTE_E_TIMEOUT))
	 {
		l_day_and_night_mode = DAYTIME_MODE;
	 }
	 else
	 {
		if(l_Positionlampst_value_U8 == 0x01)
		{
			l_day_and_night_mode = NIGHTTIME_MODE;
		}
		else if(l_Positionlampst_value_U8 == 0x00)
		{
			l_day_and_night_mode = DAYTIME_MODE;
		}
	 }
}
#endif

/*---------------------------------------------------------------------------
Date              : 02-Nov-17
CDSID             : psagathe
Traceability      : 986532
Change Description: Trip2 sw adc count change.
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Date              : 09-Nov-17
CDSID             : gkumarag
Traceability      : 988279
Change Description: Button response issue - Debounce logic added
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Date              : 12-Dec-17
CDSID             : vkrishn6
Traceability      : 1009803
Change Description: Block of TripSW1 buttons and drive mode button during IGN OFF
-----------------------------------------------------------------------------*/


