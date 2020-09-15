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
#include "Rte_OilpressMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(OilpressMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */

//#define VEHICLE_TACH_INVALID		  (0x7FFF)	 // Invalid
#define HW_Invalid					  (uint8)0xFF
//#define CTT_OILPRESS_TIMER_3000_IN_MS (3000/30) 	 // OilpressMdl 3S(3000ms) timer 
//#define CTT_OILPRESS_TIMER_2000_IN_MS (2000/30)	 // OilpressMdl 2S(2000ms) timer 

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 TtOilPresStatus = cTT_TURN_OFF;
static uint8 l_oil_TtStatus_U8 = cTT_TURN_OFF;
//static uint8 l_OilPres_input = 0;
//static uint8 Last_b4WheelDriveHPinTmp = HW_Invalid;
//static uint16 Timer_Wait_3s_Count = CTT_OILPRESS_TIMER_3000_IN_MS;
//static uint16 Timer_Wait_2s_Count = CTT_OILPRESS_TIMER_2000_IN_MS;

//static boolean Invalid_flag 			  = FALSE;
//static boolean OilPres_Wait_3S_flag 	  = FALSE;
//static boolean OilPres_Wait_3s_start_flag = FALSE;
//static uint16  Last_EngFilterSpd_U16	  = 0;
//static uint16  Lase_TtOilPresStatus       = cTT_TURN_OFF;
static uint8 l_time_count_msg;
static uint8 l_time_count;
static uint8 l_msg_flag;





static void OilPres_init_variable(void);
static void fttmdl_N356OilPres_process(void);
// static void f_tachMdl_invalidProcess(void);
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	OilPres_init_variable();

	Rte_Call_rpiCS_OilpressMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_OIL_PRESSURE,l_oil_TtStatus_U8);
	
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
	OilPres_init_variable();

	Rte_Call_rpiCS_OilpressMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_OIL_PRESSURE,l_oil_TtStatus_U8);

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
	OilPres_init_variable();

	Rte_Call_rpiCS_OilpressMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_OIL_PRESSURE,l_oil_TtStatus_U8);

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
	OilPres_init_variable();
	
	Rte_Call_rpiCS_OilpressMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_OIL_PRESSURE,l_oil_TtStatus_U8);

	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
/****************************************************************************
Function Name     	: CmpActive

Description         : This function process the oilpressure TT ON/OFF for
					  ISG /NON ISG Variant.

Invocation          : called in cmpActive.

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static Std_ReturnType CmpActive( void )
{
	uint8 fl_animationFlag = TRUE;

	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);

	if(FALSE == fl_animationFlag)
	{
		fttmdl_N356OilPres_process();
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

static void OilPres_init_variable(void)
{
	//TtOilPresStatus 		   = cTT_TURN_OFF;
	l_oil_TtStatus_U8 = cTT_TURN_OFF;
	//l_OilPres_input 		   = HW_Invalid;	
	//Last_b4WheelDriveHPinTmp   = HW_Invalid;
	//Invalid_flag 			   = FALSE;
	//OilPres_Wait_3S_flag 	   = FALSE;
	//OilPres_Wait_3s_start_flag = FALSE;
	//Last_EngFilterSpd_U16	   = 0;
	//Timer_Wait_3s_Count        = CTT_OILPRESS_TIMER_3000_IN_MS;
	//Timer_Wait_2s_Count        = CTT_OILPRESS_TIMER_2000_IN_MS;
	//Lase_TtOilPresStatus	   = cTT_TURN_OFF;
}
#if 0
static void OilPres_input_check(void)
{
	uint8 b4WheelDriveHPinTmp = HW_Invalid;
	
	static uint8 input_cnt = 0;
	static uint8 chk_cnt   = 0;

	uint8 fl_ign_substate		  = eIGN_OFF;
	static uint8 fl_ign_lastState = eIGN_OFF; 

	Rte_Call_rpCS_TIIoHwAb_DIn_Impl_GetPin(eIOPinInId_MDI_OIL_PRESSURE_SW,&b4WheelDriveHPinTmp);
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);

	if(fl_ign_substate != fl_ign_lastState)
	{
		fl_ign_lastState = 	fl_ign_substate;
		chk_cnt = 0;
		l_OilPres_input = HW_Invalid;
		
	}

	if(l_OilPres_input == HW_Invalid)
	{
		if((b4WheelDriveHPinTmp == 0) || (b4WheelDriveHPinTmp == 1))
		{
			chk_cnt ++;
			if(chk_cnt == 2)
			{
				chk_cnt = 0;
				l_OilPres_input = 2;
			}
		}
		else
		{
			chk_cnt = 0;
		}
	}
	else
	{
		chk_cnt = 0;
		if(fl_ign_substate != eIGN_RUN)
		{
			b4WheelDriveHPinTmp = HW_Invalid;
		}

		if(Last_b4WheelDriveHPinTmp != b4WheelDriveHPinTmp)
		{
			input_cnt++;
			if(input_cnt >= 2)
			{
				input_cnt = 2;
				Last_b4WheelDriveHPinTmp = b4WheelDriveHPinTmp;
			}
		}
		else
		{
			input_cnt = 0;
			l_OilPres_input = b4WheelDriveHPinTmp;
		}
	}	
}
#endif
/*
--OilPres indicator--
input signal =>
CAN signal (0x23A):
	1. BCM_LeftligthSt

		|	Input					|Output
		|	BCM_LeftligthSt			|Indicator
------------------------------------------------
		|	0						|turn off
		|	1						|turn on
------------------------------------------------
ps:
1.The Turn Right Indicator and Turn Left Indicator shall not be displayed in Power Down Transition  mode 
2.The Turn Right Indicator and Turn Left Indicator shall not be displayed in Sleep mode
3.key pos ON/OFF
*/

/*********************************************************************************
		
	Function Name	  :  fttmdl_N356OilPres_process (void)
	
	Description 	  : OilPres Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
#define LOW_LEVEL  (0)

static void fttmdl_N356OilPres_process(void)
{	
#ifndef TT_ONOFF_TEST

	//uint8 Timer_status_2S = 0;
	//uint8 Timer_status_3S = 0;
	uint8 MsgNR_10B_U8    = 0;
	uint8 MsgTout_10B_U8  = 0;
	
	uint8 fl_ign_substate 		   = eIGN_OFF;
	//EBatteryState fl_Battery_state = eBatteryState_UnderVoltage;
	uint8 OilPressurePin	       = HW_Invalid;
	uint16 fl_EngFilterSpd_U16     = 0;
	uint8 fl_OilPressPin = 0;
	uint8 fl_PinLevel_flag = FALSE;

	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	//Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&MsgNR_10B_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&MsgTout_10B_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&fl_EngFilterSpd_U16);
	Rte_Call_rpCS_TIIoHwAb_DIn_Impl_GetPin(eIOPinInId_MDI_OIL_PRESSURE_SW,&fl_OilPressPin);
#if 1
	if(eIGN_RUN == fl_ign_substate)
	{
	
		if(LOW_LEVEL == fl_OilPressPin)
		{
			fl_PinLevel_flag = TRUE;
		}
		else
		{
			fl_PinLevel_flag = FALSE;
		}
		
		if ((RTE_E_TIMEOUT == MsgTout_10B_U8)||(RTE_E_NEVER_RECEIVED == MsgNR_10B_U8))
		{
			l_msg_flag = FALSE;
			l_time_count_msg = 0;
		}
		else 
		{
			if(0x7fff == fl_EngFilterSpd_U16)
			{
				if(l_time_count_msg > 66)
				{
					l_msg_flag = FALSE;
				}
				else
				{
					l_time_count_msg++;
				}
			}
			else 
			{
				l_time_count_msg = 0;
				if(fl_EngFilterSpd_U16 > 600 )
				{
					l_msg_flag =TRUE;
				}
				else if(fl_EngFilterSpd_U16 < 400)
				{
					l_msg_flag = FALSE;
				}
				else
				{
  	
				}
			}
			
		}
		
		if(TRUE == l_msg_flag && TRUE == fl_PinLevel_flag)
		{
			
			if(l_time_count > 100)
			{
				l_oil_TtStatus_U8 = cTT_TURN_ON;
			}
			else
			{
				l_oil_TtStatus_U8 = cTT_TURN_OFF;
				l_time_count++;
			}
			
		}
		else
		{
			l_time_count = 0;
			l_msg_flag = FALSE;
			l_oil_TtStatus_U8 = cTT_TURN_OFF;
		}
	}
	else
	{
		l_time_count = 0;
	    l_msg_flag = FALSE;
		l_oil_TtStatus_U8 = cTT_TURN_OFF;
	}
	
	Rte_Call_rpiCS_OilpressMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_OIL_PRESSURE,l_oil_TtStatus_U8);
	
#else
	OilPres_input_check();

	OilPressurePin = l_OilPres_input;
	
	if(eIGN_RUN != fl_ign_substate)
	{
		OilPressurePin = HW_Invalid;
	}

	if(eIGN_RUN == fl_ign_substate)
	{
		if(RTE_E_NEVER_RECEIVED == MsgNR_10B_U8)
		{
			TtOilPresStatus = cTT_TURN_OFF;
		}
		else
		{
			if(RTE_E_TIMEOUT == MsgTout_10B_U8)
			{
				fl_EngFilterSpd_U16 = 0;
			}

		#if 1
		
			if(VEHICLE_TACH_INVALID != fl_EngFilterSpd_U16)
			{
				Invalid_flag = FALSE;
			}	
			else if(TRUE == Invalid_flag)
			{
				fl_EngFilterSpd_U16 = 0;
			}
			else
			{
				/* Invalid_flag != TRUE : The timing is not complete */
				/* fl_EngFilterSpd_U16 == VEHICLE_TACH_INVALID */
			}
			
			if(VEHICLE_TACH_INVALID != fl_EngFilterSpd_U16)
			{					
				/* process */
				if((fl_EngFilterSpd_U16 > 600)     	 // rpm > 300
				&& (FALSE == OilPressurePin))			 // low level 
				{
					if(Timer_Wait_3s_Count > 0)
					{
						Timer_Wait_3s_Count--;	
					}
					else
					{
						TtOilPresStatus = cTT_TURN_ON;
					}
				}

				else
				{	
					//Timer_Wait_3s_Count = CTT_OILPRESS_TIMER_3000_IN_MS;
					TtOilPresStatus = cTT_TURN_OFF;
				}

				Timer_Wait_2s_Count = CTT_OILPRESS_TIMER_2000_IN_MS;
			}
			else
			{	
				if(Timer_Wait_2s_Count > 0)
				{
					Timer_Wait_2s_Count--;
				}
				else
				{
					Invalid_flag = TRUE;
				}
				
				TtOilPresStatus = Lase_TtOilPresStatus;
				Timer_Wait_3s_Count = CTT_OILPRESS_TIMER_3000_IN_MS;
			}
			
		#else
			if(VEHICLE_TACH_INVALID != fl_EngFilterSpd_U16)
			{
				Invalid_flag = FALSE;
				Rte_Call_rptimer_TmExt_Timer_Stop(eOilPresTimer); // timer end
			}	
			else if(TRUE == Invalid_flag)
			{
				fl_EngFilterSpd_U16 = 0;
			}
			else
			{
				/* Invalid_flag != TRUE : The timing is not complete */
				/* fl_EngFilterSpd_U16 == VEHICLE_TACH_INVALID */
				if(Last_EngFilterSpd_U16 != VEHICLE_TACH_INVALID)
				{
					Rte_Call_rptimer_TmExt_Timer_Start(eOilPresTimer);	// timer start
				}
				
				OilPres_Wait_3S_flag = FALSE;
			}
			
			if(VEHICLE_TACH_INVALID != fl_EngFilterSpd_U16)
			{
				/* process */
				if((fl_EngFilterSpd_U16 >= 600)     		 // rpm >= 900
				&& (FALSE == OilPressurePin))			 // low level 
				{
					if(OilPres_Wait_3S_flag == TRUE)
					{
						TtOilPresStatus = cTT_TURN_ON;
					}
					else if(FALSE == OilPres_Wait_3s_start_flag)
					{
						Rte_Call_rptimer_TmExt_Timer_Start(eOilPresTimer);	// timer start
						OilPres_Wait_3s_start_flag = TRUE;			
					}
					else
					{
						Rte_Call_rptimer_TmExt_Timer_IsElapsed(eOilPresTimer, CTT_OILPRESS_TIMER_3000_IN_MS,&Timer_status_3S);
						// wait 3s
						if((FALSE == OilPres_Wait_3S_flag)&&(((uint8)1u) == Timer_status_3S))
						{
							OilPres_Wait_3S_flag = TRUE;
							OilPres_Wait_3s_start_flag = FALSE;
					
							Rte_Call_rptimer_TmExt_Timer_Stop(eOilPresTimer); // timer end
						}	
					}
				}
				else
				{	
					OilPres_Wait_3S_flag = FALSE;
					TtOilPresStatus = cTT_TURN_OFF;

					Rte_Call_rptimer_TmExt_Timer_Stop(eOilPresTimer); // timer end
				}
			}
			else
			{
				Rte_Call_rptimer_TmExt_Timer_IsElapsed(eOilPresTimer, CTT_OILPRESS_TIMER_2000_IN_MS,&Timer_status_2S);
				// wait 2s
				if((FALSE == Invalid_flag)&&(((uint8)1u) == Timer_status_2S))
				{
					Invalid_flag = TRUE;
					
					Rte_Call_rptimer_TmExt_Timer_Stop(eOilPresTimer); // timer end
				}
			}
			#endif
		}
	}
	else
	{
		TtOilPresStatus 		   = cTT_TURN_OFF;
		Invalid_flag 			   = FALSE;
		//OilPres_Wait_3S_flag 	   = FALSE;
		//OilPres_Wait_3s_start_flag = FALSE;
	}
	Lase_TtOilPresStatus = TtOilPresStatus;
	//Last_EngFilterSpd_U16 = fl_EngFilterSpd_U16;
#endif	

#else
	
	static unsigned int u32TimeCnt = 0;
	u32TimeCnt++;
	if(u32TimeCnt < 50)
	{
		TtOilPresStatus = cTT_TURN_ON;
		Rte_Call_rpiCS_OilpressMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_OIL_PRESSURE,l_oil_TtStatus_U8);
	}
	else if(u32TimeCnt < 100)
	{
		TtOilPresStatus = cTT_TURN_OFF;
		Rte_Call_rpiCS_OilpressMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_OIL_PRESSURE,l_oil_TtStatus_U8);
	}
	else
	{
		u32TimeCnt = 0;
	}

#endif
}


