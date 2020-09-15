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

File Name        :  TurnIndHazardChime.c
Module Short Name:  TurnIndHazardChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define TURNINDHAZARDCHIME_C

#include "Rte_TurnIndHazardChime.h"
#include "Rte_TurnIndHazardChime_Type.h"
#include "ChimeLogical.h"
#include "CmpLib.h"



CMPLIB_INSTANCE(TurnIndHazardChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/




/*****************************************************************************
*                                 Manifest Constants                         *
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
static SINT8 l_Hazard_turn_tt_last_state_S8;
static SINT8 l_TurnHazardchimeIndex;
// static SINT8 l_IGN_last_state_S8;

#define	TURN_IND_OFF	0
#define	BCM2_NULL		0xff
#define	BCM2_A1_TICK	1
#define	BCM2_A2_TOC		0


// #define cTT_LeftRight_Chime_30s (300)   /* task period= 100ms ,30s means 300(=30000/100) */


/*                                 Type Declarations                          *
******************************************************************************/


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/



/*****************************************************************************
*                   Functions                                                *
******************************************************************************/
static uint8 f_TurnHazard_input_verify(void);
static void f_N356_TurnHazard_input_process(void);
static void f_TurnHazardInitial(void);



//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_TurnHazardInitial();
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
	f_TurnHazardInitial();
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
	f_TurnHazardInitial();
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
	f_TurnHazardInitial();
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
#define	FUNCTION_CALLED_PERIOD	    (100) //ms
#define CHIME_DELAY_TIME_1000MS		(( 1000  + FUNCTION_CALLED_PERIOD - 1) / FUNCTION_CALLED_PERIOD)
static Std_ReturnType CmpActive( void )
{
	EBatteryState fl_Battery_state = eBatteryState_Normal;
	boolean	f_ret = FALSE;
	eIgnState fl_IGN_state = eIGN_OFF;
    uint8 fl_animationFlag = TRUE;
	//static uint8 fl_IgnQuickSwitchCnt = 0;

	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);	
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
    Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
	

	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{
		f_TurnHazardInitial();
		//fl_IgnQuickSwitchCnt=0;
	}
	else
	{
		f_ret = f_TurnHazard_input_verify();
		if((f_ret == TRUE) && (fl_animationFlag == FALSE))
		{
			f_N356_TurnHazard_input_process();
#if 0
			switch (fl_IGN_state)
			{
				case eIGN_RUN:
					if ((FALSE == fl_animationFlag) && (!fl_IgnQuickSwitchCnt))
					{
						f_N356_TurnHazard_input_process();
					}
					else
					{
						l_TurnHazardchimeIndex = BCM2_NULL;
					}

					if (fl_IgnQuickSwitchCnt > 0)
					{
						fl_IgnQuickSwitchCnt--;
					}
					break;

				case eIGN_OFF:
				default:
					f_N356_TurnHazard_input_process();
					fl_IgnQuickSwitchCnt = CHIME_DELAY_TIME_1000MS;
					break;
			}
#endif
		}
		else
		{
			f_TurnHazardInitial();
		}
	}	


	if(fl_IGN_state == eIGN_OFF) 
	{
		Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_TURN_TICK,FALSE);
		Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_TURN_TOC, FALSE);
		switch(l_TurnHazardchimeIndex)
		{
			case BCM2_A1_TICK:							
				Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_D2_TICK,TRUE);
				Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_D2_TOC, FALSE);
				break;
			case BCM2_A2_TOC:
				Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_D2_TICK,FALSE);
				Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_D2_TOC, TRUE);
				break;
			default:				
				//Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_D2_TICK,FALSE);
				//Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_D2_TOC, FALSE);
				break;
		}
	}
	else if(fl_IGN_state == eIGN_RUN)
	{
		Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_D2_TICK,FALSE);
		Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_D2_TOC, FALSE);
		switch(l_TurnHazardchimeIndex)
		{
			case BCM2_A1_TICK:							
				Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_TURN_TICK,TRUE);
				Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_TURN_TOC, FALSE);
				break;
			case BCM2_A2_TOC:
				Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_TURN_TICK,FALSE);
				Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_TURN_TOC, TRUE);
				break;
			default:				
				//Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_TURN_TICK,FALSE);
				//Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_TURN_TOC, FALSE);
				break;
		}
	}
	else 
	{
	}

	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Diagnostic state
///
/// @return E_OK:			Diagnostic has completed. Go to DiagReturn state.
///<br> 	E_NOT_READY:	Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
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
	return E_OK;
}

static void f_TurnHazardInitial(void)
{
	l_Hazard_turn_tt_last_state_S8 = 0;
	// l_IGN_last_state_S8            = 0;
	l_TurnHazardchimeIndex = BCM2_NULL;
}
static void f_N356_TurnHazard_input_process(void)
{
	uint8   fl_signal_TurnlightSt_U8 = 0;
	uint8	fl_turnChangeFlag = FALSE;
	uint8 	fl_chimeIndex = BCM2_NULL;
	eIgnState fl_IGN_state = 0;
	// uint8 Timer_status = 0;

	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorSts(&fl_signal_TurnlightSt_U8);

	if(l_Hazard_turn_tt_last_state_S8 != fl_signal_TurnlightSt_U8)
	{		
		fl_turnChangeFlag  = TRUE;
	}


	if(fl_turnChangeFlag)
	{
		if(fl_IGN_state == eIGN_RUN)
		{
			if(l_Hazard_turn_tt_last_state_S8 == TURN_IND_OFF)//off to on
			{
				fl_chimeIndex = BCM2_A1_TICK;
			}
			else if(fl_signal_TurnlightSt_U8 == TURN_IND_OFF)//on to off
			{
				fl_chimeIndex = BCM2_A2_TOC;
			}
			else
			{
				fl_chimeIndex = BCM2_NULL;
			}
		}
		else if(fl_IGN_state == eIGN_OFF)
		{
			if((l_Hazard_turn_tt_last_state_S8 == TURN_IND_OFF) && (fl_signal_TurnlightSt_U8 == 3))//0 to 3
			{
				fl_chimeIndex = BCM2_A1_TICK;
			}
			else if((l_Hazard_turn_tt_last_state_S8 == 3) && (fl_signal_TurnlightSt_U8 == TURN_IND_OFF))//3 to 0
			{
				fl_chimeIndex = BCM2_A2_TOC;
			}
			else{
				fl_chimeIndex = BCM2_NULL;
			}
		}
	}
	
   //wpq //The maximum buzzer duration is 30s
#if 0
	if((l_IGN_last_state_S8 == eIGN_RUN) && (fl_IGN_state == eIGN_OFF)) 
	{
		Rte_Call_rptimer_TmExt_Timer_Start(eTimerTurnIndHazardChime);
	}

	Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerTurnIndHazardChime, cTT_LeftRight_Chime_30s,&Timer_status);

	if(((fl_IGN_state == eIGN_RUN) && (l_IGN_last_state_S8 == eIGN_OFF)) || ((uint8)1u == Timer_status))
	{
		Rte_Call_rptimer_TmExt_Timer_Stop(eTimerTurnIndHazardChime);
	}

	if((uint8)1u == Timer_status)
	{
		fl_chimeIndex = BCM2_NULL;
	}
	
#endif


	if((fl_signal_TurnlightSt_U8 == l_Hazard_turn_tt_last_state_S8) && (fl_signal_TurnlightSt_U8 == 0))
	{
		fl_chimeIndex = BCM2_NULL;
		Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_TURN_TICK,FALSE);
		Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_TURN_TOC, FALSE);
		Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_D2_TICK,FALSE);
		Rte_Call_rpCS_TurnIndHazardChime_UpdateStatus_Operation(CHIME_REQID_D2_TOC, FALSE);
	}



	l_TurnHazardchimeIndex = fl_chimeIndex;
	l_Hazard_turn_tt_last_state_S8 = fl_signal_TurnlightSt_U8;
	//l_IGN_last_state_S8         = fl_IGN_state;

}


static boolean f_TurnHazard_input_verify(void)
{
	boolean	f_ret = TRUE;
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	

	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(&fl_CAN_msg_status_U8);
		
	if(fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_ret = FALSE;
	}
	
	return f_ret;
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

