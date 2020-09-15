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

File Name        :  STTChime.c
Module Short Name:  STTChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define STTChime_C

#include "Rte_STTChimeMdl.h"
#include "CmpLib.h"
#include "ChimeLogical.h"


CMPLIB_INSTANCE(STTChimeMdl)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
static uint8    l_STTChimeState_U8 = FALSE;

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
#define	FUNCTION_CALLED_PERIOD	(100) //ms
#define CHIME_DELAY_TIME_1000MS		(( 1000  + FUNCTION_CALLED_PERIOD - 1) / FUNCTION_CALLED_PERIOD)

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
static void f_STTinitialState(void);
static void f_STTChime_Process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_STTinitialState();
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
	f_STTinitialState();
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
	f_STTinitialState();
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
	f_STTinitialState();
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	boolean IsMT = FALSE;
	boolean IsEngineCfg = FALSE;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Engine,&IsEngineCfg);//TRUE:AT,FALSE:MT
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AT_MT,&IsMT);//TRUE:Gasoline/7000RPM,FALSE:Diesel/5000RPM
	if((IsMT == FALSE) && (IsEngineCfg == TRUE)) //for MT & 汽油
	{
		f_STTChime_Process();
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

static void f_STTinitialState(void)
{
	l_STTChimeState_U8 = FALSE;
	Rte_Call_rpCS_STTChimeMdl_UpdateStatus_Operation(CHIME_REQID_STT,l_STTChimeState_U8);
}

static void f_STTChime_Process(void)
{
	eIgnState fl_IGN_state;
	EBatteryState fl_Battery_state;
	static uint8 fl_IgnQuickSwitchCnt = 0;
	uint8 fl_animationFlag = TRUE;
	uint8 fl_310_msgNR = 0;
	uint8 fl_310_msgTout = 0;
	uint8 fl_366_msgNR = 0;
	uint8 fl_366_msgTout = 0;
	uint8 fl_MT_NeutralSensorSts_U8 = 0;
	uint8 fl_StartStopStatus_U8 = 0;

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&fl_310_msgNR);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&fl_310_msgTout);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_MT_NeutralSensorSts(&fl_MT_NeutralSensorSts_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(&fl_366_msgNR);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(&fl_366_msgTout);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_StartStopStatus(&fl_StartStopStatus_U8);

	if((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage))
	{
		f_STTinitialState();
		fl_IgnQuickSwitchCnt=0;
	}
	else
	{
		switch(fl_IGN_state)
		{
			case eIGN_RUN:
				if ((FALSE == fl_animationFlag) && (!fl_IgnQuickSwitchCnt))
				{
					if((fl_310_msgNR == RTE_E_NEVER_RECEIVED) || (fl_310_msgTout == RTE_E_TIMEOUT)
					|| (fl_366_msgNR == RTE_E_NEVER_RECEIVED) || (fl_366_msgTout == RTE_E_TIMEOUT))
					{
						f_STTinitialState();
					}
					else
					{
						if((fl_MT_NeutralSensorSts_U8 == 1) && (fl_StartStopStatus_U8 == 1))
						{
							l_STTChimeState_U8 = TRUE;
						}
						else
						{
							l_STTChimeState_U8 = FALSE;
						}
						Rte_Call_rpCS_STTChimeMdl_UpdateStatus_Operation(CHIME_REQID_STT,l_STTChimeState_U8);
					}
				}

				if (fl_IgnQuickSwitchCnt > 0)
				{
					fl_IgnQuickSwitchCnt--;
				}
				break;

			case eIGN_OFF:
			default:
				f_STTinitialState();
				fl_IgnQuickSwitchCnt = CHIME_DELAY_TIME_1000MS;
				break;
		}
	}
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
