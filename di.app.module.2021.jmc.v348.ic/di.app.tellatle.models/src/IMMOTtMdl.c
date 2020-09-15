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
#include "Rte_IMMOTtMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(IMMOTtMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */
static uint8   l_immo_status_u8 			= cTT_TURN_OFF;

typedef enum {
	IMMO_LAMP_ON = 0,
	IMMO_LAMP_ON_WITH_2500_MS,
	IMMO_LAMP_FALSH_2HZ_WITH_5000_MS,
	IMMO_LAMP_OFF,
}ANTITHEFT_TT_FLASH_T;

#define ANTI_THEFT_WARN_LED_CMD_NO_MATCH			((uint8)0)
#define ANTI_THEFT_WARN_LED_CMD_MATCH_SUCCESS		((uint8)1)
#define ANTI_THEFT_WARN_LED_CMD_MATCH_FAILED		((uint8)2)
#define ANTI_THEFT_WARN_LED_CMD_MATCHING			((uint8)3)

#define CTT_IMMO_TIMER_2500_IN_MS					(2500/30)
#define CTT_IMMO_TIMER_5000_IN_MS					(5000/30)
#define CTT_IMMO_PEPS_TIMER_COUNT_END				((uint8)0)

#define FUNCTION_IMMO_TIME_30MS                (30)           
#define FUNCTION_IMMO_TIME_5000MS              (5180 / FUNCTION_IMMO_TIME_30MS)       

#define VEHICLE_IMMO  (1)
#define VEHICLE_PEPS  (2)

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static uint8   IsImmo_cfg 	  		    	= 0;
static boolean Immo_wait_2500_ms_flag   	= FALSE;
static boolean Immo_wait_5000_ms_flag   	= FALSE;
static uint16  Immo_Timer_2500_Ms_Count 	= CTT_IMMO_TIMER_2500_IN_MS;
static uint16  Immo_Timer_5000_Ms_Count		= CTT_IMMO_TIMER_5000_IN_MS;
static uint8   l_immo_MatchStatus_last_flag = ANTI_THEFT_WARN_LED_CMD_NO_MATCH;
static ANTITHEFT_TT_FLASH_T l_immo_tt_dispose_flag	    = IMMO_LAMP_ON;
static ANTITHEFT_TT_FLASH_T l_immo_tt_dispose_last_flag = IMMO_LAMP_ON;

static boolean  l_Immo_Flag = TRUE;
static uint8    l_Proveout_Counter_U8 = FALSE;

static void Immo_Init_Variable(void);
static void fttmdl_N356IMMO_process(void);
static boolean f_Immo_PowerOn_Self_5s_Action(void);
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	Immo_Init_Variable();
	l_Immo_Flag = TRUE;
	l_Proveout_Counter_U8 = FALSE;
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
	Immo_Init_Variable();
	l_Immo_Flag = TRUE;
	l_Proveout_Counter_U8 = FALSE;
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
	Immo_Init_Variable();
	l_Immo_Flag = TRUE;
	l_Proveout_Counter_U8 = FALSE;
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
	Immo_Init_Variable();
	l_Immo_Flag = TRUE;
	l_Proveout_Counter_U8 = FALSE;
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
	boolean fl_animationFlag = TRUE;
	uint8   fl_ign_substate = eIGN_OFF;
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);


	if(eIGN_RUN == fl_ign_substate)
	{
		if(FALSE == fl_animationFlag)
		{
			fttmdl_N356IMMO_process();
		}
		else
		{
			Immo_Init_Variable();
		}
	}
	else
	{
		l_Immo_Flag = TRUE;
		l_Proveout_Counter_U8 = FALSE;
		
		Immo_Init_Variable();
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

static void Immo_Init_Variable(void)
{
	l_immo_status_u8 			 = cTT_TURN_OFF;
	l_immo_tt_dispose_flag 		 = IMMO_LAMP_ON;
	l_immo_tt_dispose_last_flag  = l_immo_tt_dispose_flag;
	IsImmo_cfg 					 = 0;
	Immo_wait_2500_ms_flag 		 = FALSE;
	Immo_wait_5000_ms_flag		 = FALSE;
	Immo_Timer_2500_Ms_Count     = CTT_IMMO_TIMER_2500_IN_MS;
	Immo_Timer_5000_Ms_Count     = CTT_IMMO_TIMER_5000_IN_MS;
	l_immo_MatchStatus_last_flag = ANTI_THEFT_WARN_LED_CMD_NO_MATCH;

	Rte_Call_rpCS_IMMOTtMdl_TIMdlUpdateTtStatus_Operation(cTTIMMO,l_immo_status_u8);
}

static void fttmdl_N356IMMO_process(void)
{
#ifndef TT_ONOFF_TEST

	//uint8 Timer_status_2500_ms = 0;
	//uint8 Timer_status_5000_ms = 0;
	
	uint8 u8MsgNR 	= 0;
	uint8 u8MsgTout = 0;
	
	uint8 fl_AntiTheftWarnLedCmd_sig_value_U8 = 0;
	uint8 fl_BCM_ImmoSts_sig_value_U8		  = 0;
	
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_PEPS,&IsImmo_cfg);

	if(VEHICLE_PEPS == IsImmo_cfg)
	{
		Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_NR(&u8MsgNR);
		Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_Tout(&u8MsgTout);
		Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus(&fl_AntiTheftWarnLedCmd_sig_value_U8);
	}
	else if(VEHICLE_IMMO == IsImmo_cfg)
	{
		Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(&u8MsgNR);
		Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(&u8MsgTout);
		Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_ImmoSts(&fl_BCM_ImmoSts_sig_value_U8);		
	}
	else
	{
		//
	}

	if((VEHICLE_PEPS == IsImmo_cfg) || (VEHICLE_IMMO == IsImmo_cfg))
	{
		if(RTE_E_NEVER_RECEIVED == u8MsgNR)
		{
			f_Immo_PowerOn_Self_5s_Action();
			
			if(FALSE == l_Immo_Flag)
			{
				l_immo_status_u8 = cTT_TURN_ON;
			}
			else
			{
				l_immo_status_u8 = cTT_TURN_OFF;
			}
		}
		else if(RTE_E_TIMEOUT == u8MsgTout)
		{
			l_immo_status_u8 = cTT_TURN_ON;
		
			Immo_wait_2500_ms_flag = FALSE;
			Immo_wait_5000_ms_flag = FALSE;
			Immo_Timer_2500_Ms_Count = CTT_IMMO_TIMER_2500_IN_MS;
			Immo_Timer_5000_Ms_Count = CTT_IMMO_TIMER_5000_IN_MS;
		}
		else
		{
			
			if(VEHICLE_PEPS == IsImmo_cfg)
			{
				switch(fl_AntiTheftWarnLedCmd_sig_value_U8)
				{
					case ANTI_THEFT_WARN_LED_CMD_NO_MATCH:
						l_immo_tt_dispose_flag = IMMO_LAMP_ON;
						break;
					case ANTI_THEFT_WARN_LED_CMD_MATCH_FAILED:
						l_immo_tt_dispose_flag = IMMO_LAMP_FALSH_2HZ_WITH_5000_MS;
						break;
					case ANTI_THEFT_WARN_LED_CMD_MATCH_SUCCESS:
						l_immo_tt_dispose_flag = IMMO_LAMP_ON_WITH_2500_MS;
						break;
					case ANTI_THEFT_WARN_LED_CMD_MATCHING:
						l_immo_tt_dispose_flag = l_immo_tt_dispose_last_flag;
						break;
					default:
						break;			
				}
				
				if((ANTI_THEFT_WARN_LED_CMD_MATCHING == l_immo_MatchStatus_last_flag)
				&& (ANTI_THEFT_WARN_LED_CMD_MATCHING != fl_AntiTheftWarnLedCmd_sig_value_U8))
				{
					Immo_wait_2500_ms_flag = FALSE;
					Immo_wait_5000_ms_flag = FALSE;
					Immo_Timer_2500_Ms_Count = CTT_IMMO_TIMER_2500_IN_MS;
					Immo_Timer_5000_Ms_Count = CTT_IMMO_TIMER_5000_IN_MS;
				}
				
				l_immo_MatchStatus_last_flag = fl_AntiTheftWarnLedCmd_sig_value_U8;
			}
			else
			{
				switch(fl_BCM_ImmoSts_sig_value_U8)
				{
					case ANTI_THEFT_WARN_LED_CMD_NO_MATCH:
						l_immo_tt_dispose_flag = IMMO_LAMP_OFF;
						break;
					case ANTI_THEFT_WARN_LED_CMD_MATCH_FAILED:
						l_immo_tt_dispose_flag = IMMO_LAMP_FALSH_2HZ_WITH_5000_MS;
						break;
					case ANTI_THEFT_WARN_LED_CMD_MATCH_SUCCESS:
						l_immo_tt_dispose_flag = IMMO_LAMP_FALSH_2HZ_WITH_5000_MS;
						break;
					case ANTI_THEFT_WARN_LED_CMD_MATCHING:
						l_immo_tt_dispose_flag = IMMO_LAMP_ON_WITH_2500_MS;
						break;
					default:
						break;	
				}
			}

		
			switch(l_immo_tt_dispose_flag)
			{
				case IMMO_LAMP_ON:
					Immo_wait_2500_ms_flag = FALSE;
					Immo_wait_5000_ms_flag = FALSE;
					Immo_Timer_2500_Ms_Count = CTT_IMMO_TIMER_2500_IN_MS;
					Immo_Timer_5000_Ms_Count = CTT_IMMO_TIMER_5000_IN_MS;
					l_immo_status_u8 = cTT_TURN_ON;
					
					break;
				case IMMO_LAMP_ON_WITH_2500_MS:
					Immo_wait_5000_ms_flag = FALSE;
					Immo_Timer_5000_Ms_Count = CTT_IMMO_TIMER_5000_IN_MS;
		
					if(Immo_Timer_2500_Ms_Count > CTT_IMMO_PEPS_TIMER_COUNT_END)
					{
						Immo_Timer_2500_Ms_Count--;
						Immo_wait_2500_ms_flag = FALSE;
					}
					else
					{ 
						Immo_wait_2500_ms_flag = TRUE;
					}
					
					if(TRUE == Immo_wait_2500_ms_flag)
					{
						l_immo_status_u8 = cTT_TURN_OFF;
					}
					else
					{
						l_immo_status_u8 = cTT_TURN_ON;
					}
					
					break;
				case IMMO_LAMP_FALSH_2HZ_WITH_5000_MS:
					Immo_wait_2500_ms_flag = FALSE;
					Immo_Timer_2500_Ms_Count = CTT_IMMO_TIMER_2500_IN_MS;
				
					if(Immo_Timer_5000_Ms_Count > CTT_IMMO_PEPS_TIMER_COUNT_END)
					{
						Immo_Timer_5000_Ms_Count--;
						Immo_wait_5000_ms_flag = FALSE;
					}
					else
					{ 
						Immo_wait_5000_ms_flag = TRUE;
					}
					
					if(TRUE == Immo_wait_5000_ms_flag)
					{
						l_immo_status_u8 = cTT_TURN_ON;
					}
					else
					{
						l_immo_status_u8 = cTT_FLASH_2HZ;
					}
					
					break;
				case IMMO_LAMP_OFF:
					Immo_wait_2500_ms_flag	 = FALSE;
					Immo_wait_5000_ms_flag	 = FALSE;
					Immo_Timer_2500_Ms_Count = CTT_IMMO_TIMER_2500_IN_MS;
					Immo_Timer_5000_Ms_Count = CTT_IMMO_TIMER_5000_IN_MS;				
					l_immo_status_u8		 = cTT_TURN_OFF;
					break;
			}
		
		}
		
		l_immo_tt_dispose_last_flag = l_immo_tt_dispose_flag;
		Rte_Call_rpCS_IMMOTtMdl_TIMdlUpdateTtStatus_Operation(cTTIMMO,l_immo_status_u8);
	}
	else
	{
		Rte_Call_rpCS_IMMOTtMdl_TIMdlUpdateTtStatus_Operation(cTTIMMO,FALSE);
	}

#else

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_immo_status_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_IMMOTtMdl_TIMdlUpdateTtStatus_Operation(cTTIMMO,l_immo_status_u8);

	}
	else if(u32TimeCnt < 100)
	{
		l_immo_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_IMMOTtMdl_TIMdlUpdateTtStatus_Operation(cTTIMMO,l_immo_status_u8);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;
	
#endif
}

static boolean f_Immo_PowerOn_Self_5s_Action(void)
{
    if(l_Proveout_Counter_U8 >= FUNCTION_IMMO_TIME_5000MS)
    {
        l_Immo_Flag = FALSE;
    }
    else
    {
        l_Immo_Flag = TRUE;
		l_Proveout_Counter_U8++;
    }
	
    return l_Immo_Flag;
}

