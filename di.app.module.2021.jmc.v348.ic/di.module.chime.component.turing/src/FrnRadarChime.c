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

File Name        :  FrnRadarChime.c
Module Short Name:  FrnRadarChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define FrnRadarChime_C

#include "Rte_FrnRadarChime.h"
#include "ChimeLogical.h"
#include "CmpLib.h"


CMPLIB_INSTANCE(FrnRadarChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
static uint8    l_FrnRadarChimeState_1HZ_U8 = FALSE;
static uint8    l_FrnRadarChimeState_2HZ_U8 = FALSE;
static uint8    l_FrnRadarChimeState_4HZ_U8 = FALSE;
static uint8    l_FrnRadarChimeState_LONG_U8 = FALSE;

static uint8	fl_IC_AlarmOrNot_U8 = 0;
static uint8	fl_IC_AlarmStatus_U8 = 0;

static boolean  l_FrnRad_Flag       = TRUE;
static uint8    l_ProveoutCounter_U8   = 0;

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
#define CHIME_FRNRADAR_VALUE_0  (0)
#define CHIME_FRNRADAR_VALUE_1  (1)
#define CHIME_FRNRADAR_VALUE_2  (2)
#define CHIME_FRNRADAR_VALUE_3  (3)
#define CHIME_FRNRADAR_VALUE_4  (4)
#define CHIME_FRNRADAR_VALUE_5  (5)

#define IsEngineCfg_FOUR_SENSORS (3)
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
static void    f_Chime_FrnRadar_Initial_State(void);
static boolean f_Chime_FrnRadar_Input_verify(void);
static void    f_Chime_FrnRadar_Process(void);
static boolean f_Chime_FrnRad_PowerOn_Self_3s_Action(void);


//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
    f_Chime_FrnRadar_Initial_State();
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
    f_Chime_FrnRadar_Initial_State();
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
    f_Chime_FrnRadar_Initial_State();   
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
    f_Chime_FrnRadar_Initial_State();   
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{

    eIgnState fl_IGN_state;
	EBatteryState fl_Battery_state;
    uint8 fl_animationFlag = TRUE;
	
	uint8 IsEngineCfg_PAM = 0;
	//Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_REAR_SENSORS,&IsEngineCfg_PAM);
	
	if(IsEngineCfg_FOUR_SENSORS == IsEngineCfg_PAM)
	{
		Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
		Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
		Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
		
		if((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage))
		{
			f_Chime_FrnRadar_Initial_State();
		}
		else
		{
			if(fl_IGN_state == eIGN_RUN)
			{
				if(FALSE == fl_animationFlag) 
				{
					f_Chime_FrnRad_PowerOn_Self_3s_Action();
					
					if(FALSE == l_FrnRad_Flag)
					{
						f_Chime_FrnRadar_Process();
					}
					else
					{
						f_Chime_FrnRadar_Initial_State();
					}
				}
				else
				{
					f_Chime_FrnRadar_Initial_State();
				}
			}
			else
			{
				l_FrnRad_Flag = TRUE;
				l_ProveoutCounter_U8 = 0;
				
				f_Chime_FrnRadar_Initial_State();
			}
		}
		
		Rte_Call_rpCS_FrnRadarChime_UpdateStatus_Operation(CHIME_REQID_UPA_1HZ,l_FrnRadarChimeState_1HZ_U8);
		Rte_Call_rpCS_FrnRadarChime_UpdateStatus_Operation(CHIME_REQID_UPA_2HZ,l_FrnRadarChimeState_2HZ_U8);
		Rte_Call_rpCS_FrnRadarChime_UpdateStatus_Operation(CHIME_REQID_UPA_4HZ,l_FrnRadarChimeState_4HZ_U8);
		Rte_Call_rpCS_FrnRadarChime_UpdateStatus_Operation(CHIME_REQID_UPA_LONG,l_FrnRadarChimeState_LONG_U8);
		
		Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_AlarmOrNot(fl_IC_AlarmOrNot_U8);
		Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_AlarmStatus(fl_IC_AlarmStatus_U8);
	}
	else
	{
		//
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


static void f_Chime_FrnRadar_Initial_State(void)
{
    l_FrnRadarChimeState_1HZ_U8 = FALSE;
    l_FrnRadarChimeState_2HZ_U8 = FALSE;
    l_FrnRadarChimeState_4HZ_U8 = FALSE;
    l_FrnRadarChimeState_LONG_U8 = FALSE;

	fl_IC_AlarmOrNot_U8 = 0;
	fl_IC_AlarmStatus_U8 = 0;
	
	Rte_Call_rpCS_FrnRadarChime_UpdateStatus_Operation(CHIME_REQID_UPA_1HZ,l_FrnRadarChimeState_1HZ_U8);
    Rte_Call_rpCS_FrnRadarChime_UpdateStatus_Operation(CHIME_REQID_UPA_2HZ,l_FrnRadarChimeState_2HZ_U8);
    Rte_Call_rpCS_FrnRadarChime_UpdateStatus_Operation(CHIME_REQID_UPA_4HZ,l_FrnRadarChimeState_4HZ_U8);
    Rte_Call_rpCS_FrnRadarChime_UpdateStatus_Operation(CHIME_REQID_UPA_LONG,l_FrnRadarChimeState_LONG_U8);
}

static boolean f_Chime_FrnRadar_Input_verify(void)
{
    boolean	f_ret = TRUE;
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	uint8 	fl_530_nvr_status_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_Tout(&fl_CAN_msg_status_U8);
	Rte_Read_rpSR_CANMSG_GW_MP5_0x530_ComIn_NR(&fl_530_nvr_status_U8);
	
	if((fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_msg_status_U8 == RTE_E_TIMEOUT) ||
		(fl_530_nvr_status_U8 == RTE_E_NEVER_RECEIVED))
	{
		f_ret = FALSE;
	}
	
	return f_ret;
}

static void f_Chime_FrnRadar_Process(void)
{
	uint8   fl_MP5_0x530_Tout = 0;
	uint8   fl_signal_AlarmStatus_U8 = 0;
	uint8   fl_signal_AlarmOrNot_U8 = 0;
	uint8   fl_signal_RearSysTemWorkSt_U8 = 0;
    uint8   fl_signal_ChimeCmdSoundCadence_U8 = 0;
	
    boolean	f_ret = FALSE;
	uint8   fl_0x530_Status_U8 = FALSE;
	
	Rte_Read_rpSR_CANMSG_GW_MP5_0x530_ComIn_Tout(&fl_MP5_0x530_Tout);
	Rte_Read_rpSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmStatus(&fl_signal_AlarmStatus_U8);
	Rte_Read_rpSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmOrNot(&fl_signal_AlarmOrNot_U8);
	Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_RearSysTemWorkSts(&fl_signal_RearSysTemWorkSt_U8);
    Rte_Read_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundCadence(&fl_signal_ChimeCmdSoundCadence_U8);

	f_ret = f_Chime_FrnRadar_Input_verify();

	if(FALSE == f_ret)
	{
		f_Chime_FrnRadar_Initial_State();
	}
	else
	{
		if((CHIME_FRNRADAR_VALUE_2 == fl_signal_AlarmStatus_U8) && (CHIME_FRNRADAR_VALUE_0 == fl_signal_AlarmOrNot_U8))
		{
			fl_0x530_Status_U8 = TRUE;
		}
		
		if(((TRUE == fl_0x530_Status_U8) || (RTE_E_TIMEOUT == fl_MP5_0x530_Tout)) && (CHIME_FRNRADAR_VALUE_2 == fl_signal_RearSysTemWorkSt_U8))
		{
			switch(fl_signal_ChimeCmdSoundCadence_U8)
			{
				case CHIME_FRNRADAR_VALUE_1:
					fl_IC_AlarmStatus_U8 = 1;
					l_FrnRadarChimeState_1HZ_U8 = FALSE;
					l_FrnRadarChimeState_2HZ_U8 = FALSE;
					l_FrnRadarChimeState_4HZ_U8 = FALSE;
					l_FrnRadarChimeState_LONG_U8 = TRUE;
					break;
				case CHIME_FRNRADAR_VALUE_2:
					fl_IC_AlarmStatus_U8 = 1;
					l_FrnRadarChimeState_1HZ_U8 = FALSE;
					l_FrnRadarChimeState_2HZ_U8 = FALSE;
					l_FrnRadarChimeState_4HZ_U8 = TRUE;
					l_FrnRadarChimeState_LONG_U8 = FALSE;
					break;
				case CHIME_FRNRADAR_VALUE_3:
					fl_IC_AlarmStatus_U8 = 1;
					l_FrnRadarChimeState_1HZ_U8 = FALSE;
					l_FrnRadarChimeState_2HZ_U8 = FALSE;
					l_FrnRadarChimeState_4HZ_U8 = TRUE;
					l_FrnRadarChimeState_LONG_U8 = FALSE;
					break;
				case CHIME_FRNRADAR_VALUE_4:
					fl_IC_AlarmStatus_U8 = 1;
					l_FrnRadarChimeState_1HZ_U8 = FALSE;
					l_FrnRadarChimeState_2HZ_U8 = TRUE;
					l_FrnRadarChimeState_4HZ_U8 = FALSE;
					l_FrnRadarChimeState_LONG_U8 = FALSE;
					break;
				case CHIME_FRNRADAR_VALUE_5:
					fl_IC_AlarmStatus_U8 = 1;
					l_FrnRadarChimeState_1HZ_U8 = TRUE;
					l_FrnRadarChimeState_2HZ_U8 = FALSE;
					l_FrnRadarChimeState_4HZ_U8 = FALSE;
					l_FrnRadarChimeState_LONG_U8 = FALSE;
					break;
				default:
					fl_IC_AlarmStatus_U8 = 0;
					l_FrnRadarChimeState_1HZ_U8 = FALSE;
					l_FrnRadarChimeState_2HZ_U8 = FALSE;
					l_FrnRadarChimeState_4HZ_U8 = FALSE;
					l_FrnRadarChimeState_LONG_U8 = FALSE;
					break;
			}

			fl_IC_AlarmOrNot_U8 = 1;
		}
		else
		{
			f_Chime_FrnRadar_Initial_State();
		}
	}

}


static boolean f_Chime_FrnRad_PowerOn_Self_3s_Action(void)
{
    if(l_ProveoutCounter_U8 >= 30)
    {
        l_FrnRad_Flag = FALSE;
    }
    else
    {
        l_FrnRad_Flag = TRUE;
		l_ProveoutCounter_U8++;
    }

    return l_FrnRad_Flag;
}
