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
#include "Rte_TpmsMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(TpmsMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8 l_tpms_status_u8 = cTT_TURN_OFF;
static uint8 l_Proveout_Counter_U8 = 0;
static boolean l_TPMS_Flag = TRUE;

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */
#define TEMP_VALUE_1				((uint8)1u)
#define TEMP_VALUE_2				((uint8)2u)
#define TEMP_VALUE_3				((uint8)3u)
#define TEMP_VALUE_4				((uint8)4u)
#define TEMP_VALUE_5				((uint8)5u)
#define TEMP_VALUE_6				((uint8)6u)
#define TEMP_VALUE_7				((uint8)7u)
#define VEHICLE_TPMS                (1)

#define PRESSURE_INVALID_VALUE		((uint8)0xFF)

#define FUNCTION_TPMS_TIME_30MS                (30)           
#define FUNCTION_TPMS_TIME_6500MS              (6500 / FUNCTION_TPMS_TIME_30MS)                       
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static void fttmdl_N356TPMS_process(void);
static boolean f_TPMS_PowerOn_Self_7s_Action(void);
static void f_TPMS_PowerOn_Self_7s_Init(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_tpms_status_u8 = cTT_TURN_OFF;
	f_TPMS_PowerOn_Self_7s_Init();
	Rte_Call_rpCS_TpmsMdl_TIMdlUpdateTtStatus_Operation(cTTTPMS_FAILURE,l_tpms_status_u8);
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
	l_tpms_status_u8 = cTT_TURN_OFF;
	f_TPMS_PowerOn_Self_7s_Init();
	Rte_Call_rpCS_TpmsMdl_TIMdlUpdateTtStatus_Operation(cTTTPMS_FAILURE,l_tpms_status_u8);
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
	l_tpms_status_u8 = cTT_TURN_OFF;
	f_TPMS_PowerOn_Self_7s_Init();
	Rte_Call_rpCS_TpmsMdl_TIMdlUpdateTtStatus_Operation(cTTTPMS_FAILURE,l_tpms_status_u8);
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
	l_tpms_status_u8 = cTT_TURN_OFF;
	f_TPMS_PowerOn_Self_7s_Init();
	Rte_Call_rpCS_TpmsMdl_TIMdlUpdateTtStatus_Operation(cTTTPMS_FAILURE,l_tpms_status_u8);
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
	uint8 IsEngineCfg_Tpms = FALSE;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_TPMS,&IsEngineCfg_Tpms);

	if(VEHICLE_TPMS == IsEngineCfg_Tpms)
	{
		fttmdl_N356TPMS_process();
	}
	else
	{
		Rte_Call_rpCS_TpmsMdl_TIMdlUpdateTtStatus_Operation(cTTTPMS_FAILURE,FALSE);
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

/*********************************************************************************
		
	Function Name	  :  fttmdl_TPMS_time_process
	
	Description 	  : Tpms time process
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/

/*--TPMS indicator--
input signal =>
CAN signal (0x431):
	1. DDS+_system_status
	
		|	Input										|Output
		|	DDS+_system_status							|Indicator
-------------------------------------------------------------------
IGN ON	|	1											|get first signal, turn on 1.8s,then turn off
		|	2											|flash 2Hz for 3s, then turn off
		|	3,9											|turn off
		|	4,5,7										|turn on
		|	6,B											|flash 2Hz for 60s, then keep turn on
-------------------------------------------------------------------
IGN OFF	|	NC											|turn off

ps:
1.If message signal missing <=5s, the last  valid values shall be used, if no last valid values, shall use CAN_Default value;
*/
/*********************************************************************************
		
	Function Name	  :  fttmdl_Tpms_process (void)
	
	Description 	  : Tpms Telltale Machine Logical
	
	Parameters		  : None
	
	Return Value	  : None
	
	Critical Section  : None
		
***********************************************************************************/
static void fttmdl_N356TPMS_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR 	= 0;
	uint8 u8MsgTout = 0;
	
	uint8 fl_ign_substate = eIGN_OFF;
	uint8 fl_tpms_status_u8 = FALSE;

	/* TPMS System Error */
	uint8	fl_tpms_system_status_value = 0;

    /* TPMS Pressure Warning */
	uint8	fl_tpms_LF_pressure_warn_value = 0;
	uint8	fl_tpms_RF_pressure_warn_value = 0;
	uint8	fl_tpms_LR_pressure_warn_value = 0;
	uint8	fl_tpms_RR_pressure_warn_value = 0;

	/* TPMS Temperature Warning */
	uint8	fl_tpms_temp_warn_value = 0;
	
	/* TPMS Tire_Position  Value*/
	uint8	fl_tpms_tire_pos_value = 0;

	/* TPMS Tire_Pressure  Value*/
	uint8	fl_tpms_LF_pressure_value = 0;
	uint8	fl_tpms_RF_pressure_value = 0;
	uint8	fl_tpms_RR_pressure_value = 0;
	uint8	fl_tpms_LR_pressure_value = 0;
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tout(&u8MsgTout);

	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status(&fl_tpms_system_status_value);

	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning(&fl_tpms_LF_pressure_warn_value);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning(&fl_tpms_RF_pressure_warn_value);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning(&fl_tpms_RR_pressure_warn_value);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning(&fl_tpms_LR_pressure_warn_value);
	
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure(&fl_tpms_LF_pressure_value);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure(&fl_tpms_RF_pressure_value);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure(&fl_tpms_RR_pressure_value);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure(&fl_tpms_LR_pressure_value);
	
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Temperature_Warning(&fl_tpms_temp_warn_value);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position(&fl_tpms_tire_pos_value);

	l_tpms_status_u8 = cTT_TURN_OFF;

	if( eIGN_RUN == fl_ign_substate )
	{ 
		if(RTE_E_TIMEOUT == u8MsgTout)
		{
			l_tpms_status_u8 = cTT_TURN_ON;
		}
		else if (RTE_E_NEVER_RECEIVED == u8MsgNR)
		{
			f_TPMS_PowerOn_Self_7s_Action();
			
			if(FALSE == l_TPMS_Flag)
			{
				l_tpms_status_u8 = cTT_TURN_ON;
			}
			else
			{
				l_tpms_status_u8 = cTT_TURN_OFF;
			}
		}
		else
		{
			/* 1-TPMS System Error */
			if((TEMP_VALUE_1 == fl_tpms_system_status_value) || (TEMP_VALUE_2 == fl_tpms_system_status_value))
			{
				fl_tpms_status_u8 = TRUE;
			}
					
			/* 2-TPMS Pressure Warning */
			switch( fl_tpms_LF_pressure_warn_value )
			{
				case TEMP_VALUE_1:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_2:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_4:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_5:
					fl_tpms_status_u8 = TRUE;
					break;
				default:
					break;
			}

			switch( fl_tpms_RF_pressure_warn_value )
			{
				case TEMP_VALUE_1:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_2:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_4:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_5:
					fl_tpms_status_u8 = TRUE;
					break;
				default:
					break;
			}

			switch( fl_tpms_RR_pressure_warn_value )
			{
				case TEMP_VALUE_1:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_2:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_4:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_5:
					fl_tpms_status_u8 = TRUE;
					break;
				default:
					break;
			}

			switch( fl_tpms_LR_pressure_warn_value )
			{
				case TEMP_VALUE_1:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_2:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_4:
					fl_tpms_status_u8 = TRUE;
					break;
				case TEMP_VALUE_5:
					fl_tpms_status_u8 = TRUE;
					break;
				default:
					break;
			}

			/* 3-TPMS Temperature Warning & Tire Position */
			if( TEMP_VALUE_1 == fl_tpms_temp_warn_value )
			{
				switch( fl_tpms_tire_pos_value )
				{
					case TEMP_VALUE_1:
						fl_tpms_status_u8 = TRUE;
						break;
					case TEMP_VALUE_2:
						fl_tpms_status_u8 = TRUE;
						break;
					case TEMP_VALUE_3:
						fl_tpms_status_u8 = TRUE;
						break;
					case TEMP_VALUE_4:
						fl_tpms_status_u8 = TRUE;
						break;
					default:
						break;
				}
			}
			
			/* 4-TPMS Tire Pressure */
			if((PRESSURE_INVALID_VALUE == fl_tpms_LF_pressure_value)
			|| (PRESSURE_INVALID_VALUE == fl_tpms_RF_pressure_value)
			|| (PRESSURE_INVALID_VALUE == fl_tpms_RR_pressure_value)
			|| (PRESSURE_INVALID_VALUE == fl_tpms_LR_pressure_value))
			{
				fl_tpms_status_u8 = TRUE;
			}

			if(TRUE == fl_tpms_status_u8)
			{
				l_tpms_status_u8 = cTT_TURN_ON;
			}
			else
			{
				l_tpms_status_u8 = cTT_TURN_OFF;
			}
		}
	}
	else
	{
		l_tpms_status_u8 = cTT_TURN_OFF;

		f_TPMS_PowerOn_Self_7s_Init();
	}
	
	Rte_Call_rpCS_TpmsMdl_TIMdlUpdateTtStatus_Operation(cTTTPMS_FAILURE,l_tpms_status_u8);
	
#else

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_tpms_status_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_TpmsMdl_TIMdlUpdateTtStatus_Operation(cTTTPMS_FAILURE,l_tpms_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_tpms_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_TpmsMdl_TIMdlUpdateTtStatus_Operation(cTTTPMS_FAILURE,l_tpms_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
	
#endif
}

static boolean f_TPMS_PowerOn_Self_7s_Action(void)
{
    if(l_Proveout_Counter_U8 >= FUNCTION_TPMS_TIME_6500MS)
    {
        l_TPMS_Flag = FALSE;
    }
    else
    {
        l_TPMS_Flag = TRUE;
		l_Proveout_Counter_U8++;
    }
	
    return l_TPMS_Flag;
}

static void f_TPMS_PowerOn_Self_7s_Init(void)
{
	l_TPMS_Flag = TRUE;
	l_Proveout_Counter_U8 = 0;
}

