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

File Name        :  TpmsChimeMdl.c
Module Short Name:  TpmsChimeMdl
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define TpmsChimeMdl_C

#include "Rte_TpmsChimeMdl.h"
#include "CmpLib.h"
#include "ChimeLogical.h"


CMPLIB_INSTANCE(TpmsChimeMdl)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
static uint8    l_TpmsChimeState_U8  = FALSE;
static uint8    fl_signal_LF_Pressure_WarningSt_Last_U8 = FALSE;
static uint8    fl_signal_RF_Pressure_WarningSt_Last_U8 = FALSE;
static uint8    fl_signal_RR_Pressure_WarningSt_Last_U8 = FALSE;
static uint8    fl_signal_LR_Pressure_WarningSt_Last_U8 = FALSE;
static uint8    fl_signal_Temperature_WarningSt_Last_U8 = FALSE;
static uint8    fl_signal_TirePosition_WarningSt_Last1_U8 = FALSE;
static uint8    fl_signal_TirePosition_WarningSt_Last2_U8 = FALSE;
static uint8    fl_signal_SystemSt_Last_U8 = FALSE;
static uint8    l_IGN_statePre_U8 = FALSE;
static boolean  l_Tpms_flag = TRUE;
static uint8    l_ProveoutCounter_U8 = FALSE;

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
#define Tpms_LF_Pressure_Waring_1     0x01
#define Tpms_LF_Pressure_Waring_2     0x02
#define Tpms_LF_Pressure_Waring_4     0x04
#define Tpms_LF_Pressure_Waring_5     0x05

#define Tpms_RF_Pressure_Waring_1     0x01
#define Tpms_RF_Pressure_Waring_2     0x02
#define Tpms_RF_Pressure_Waring_4     0x04
#define Tpms_RF_Pressure_Waring_5     0x05

#define Tpms_RR_Pressure_Waring_1     0x01
#define Tpms_RR_Pressure_Waring_2     0x02
#define Tpms_RR_Pressure_Waring_4     0x04
#define Tpms_RR_Pressure_Waring_5     0x05

#define Tpms_LR_Pressure_Waring_1     0x01
#define Tpms_LR_Pressure_Waring_2     0x02
#define Tpms_LR_Pressure_Waring_4     0x04
#define Tpms_LR_Pressure_Waring_5     0x05

#define Temperature_Waring            0x01

#define TriePosition_0                0x00
#define TriePosition_1                0x01
#define TriePosition_2                0x02
#define TriePosition_3                0x03
#define TriePosition_4                0x04

#define System_Status_1               0x01
#define System_Status_2               0x02

#define VEHICLE_TPMS_CHIME            (1)

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
static void f_TpmsinitialState(void);
static boolean f_Tpms_input_verify(void);
static void f_TpmsChime_Process(void);
static void f_Tpms_System_Error_Process(void);
static void f_Tpms_Pressure_Process(void);
static void f_Tpms_Temperature_Warning_Process(void);
static boolean Tpms_PowerOn_Self_Test_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
    f_TpmsinitialState();
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
	f_TpmsinitialState();
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
	f_TpmsinitialState();
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
	f_TpmsinitialState();
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	uint8         fl_animationFlag       = TRUE;
	uint8         fl_CHIME_REQID_TPMS_U8 = 0;
	eIgnState     fl_IGN_state           = eIGN_OFF;
	EBatteryState fl_Battery_state       = eBatteryState_Normal;
	
	uint8 IsEngineCfg_Tpms_Chime = FALSE;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_TPMS,&IsEngineCfg_Tpms_Chime);

	if(VEHICLE_TPMS_CHIME == IsEngineCfg_Tpms_Chime)
	{
		Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
		Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
		
		if((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage))
		{
			f_TpmsinitialState();
		}
		else
		{
			if(fl_IGN_state == eIGN_RUN)
			{
				Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
				
				if(fl_animationFlag == FALSE)
				{
					Tpms_PowerOn_Self_Test_Action();
					
					if(l_Tpms_flag == FALSE)
					{
						f_TpmsChime_Process();
					}
					else
					{
						f_TpmsinitialState();
					}						
				}
			}
			else
			{
				l_ProveoutCounter_U8 = 0;
				l_Tpms_flag = TRUE; 
				f_TpmsinitialState();
			}
		}
		
		Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_TPMSChime(CHIME_REQID_TPMS_WARN);
		Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_TPMSChime(&fl_CHIME_REQID_TPMS_U8);
		if (TRUE == fl_CHIME_REQID_TPMS_U8)
		{
			Rte_Call_rpCS_TpmsChimeMdl_UpdateStatus_Operation(CHIME_REQID_TPMS_WARN, FALSE);
		}
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

static void f_TpmsinitialState(void)
{
	l_TpmsChimeState_U8 = FALSE;
    fl_signal_LF_Pressure_WarningSt_Last_U8 = FALSE;
    fl_signal_RF_Pressure_WarningSt_Last_U8 = FALSE;
    fl_signal_RR_Pressure_WarningSt_Last_U8 = FALSE;
    fl_signal_LR_Pressure_WarningSt_Last_U8 = FALSE;
    fl_signal_Temperature_WarningSt_Last_U8 = FALSE;
    fl_signal_TirePosition_WarningSt_Last1_U8 = FALSE;
    fl_signal_TirePosition_WarningSt_Last2_U8 = FALSE;
    fl_signal_SystemSt_Last_U8 = FALSE;

	Rte_Call_rpCS_TpmsChimeMdl_UpdateStatus_Operation(CHIME_REQID_TPMS_WARN,l_TpmsChimeState_U8);
}

static boolean f_Tpms_input_verify(void)
{
	boolean	f_ret = TRUE;
	uint8 	fl_can_nvr_status_U8 = 0;
	uint8 	fl_can_msg_status_U8 = 0;
	
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_NR(&fl_can_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tout(&fl_can_msg_status_U8);
	
	if(fl_can_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_can_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_ret = FALSE;
	}
	
	return f_ret;
}

static boolean Tpms_PowerOn_Self_Test_Action(void)
{
    if(l_ProveoutCounter_U8 >= 30)
    {
        l_Tpms_flag = FALSE;
    }
    else
    {
        l_Tpms_flag = TRUE;
		l_ProveoutCounter_U8++;
    }

    return l_Tpms_flag;
}


static void f_TpmsChime_Process(void)
{
    boolean	 f_ret = FALSE;
	
	f_ret = f_Tpms_input_verify();
	if(FALSE == f_ret)
	{
		f_TpmsinitialState();
	}
	else
	{
		f_Tpms_System_Error_Process();
		f_Tpms_Pressure_Process();
		f_Tpms_Temperature_Warning_Process();
	}

}


static void f_Tpms_System_Error_Process(void)
{
    uint8    fl_signal_SystemSt_U8 = 0;
    uint8    fl_signal_TirePosition_WarningSt_U8 = 0;

	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position(&fl_signal_TirePosition_WarningSt_U8); 
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status(&fl_signal_SystemSt_U8);


	if((fl_signal_SystemSt_U8 == System_Status_1) && (fl_signal_SystemSt_Last_U8 != fl_signal_SystemSt_U8))
	{
		l_TpmsChimeState_U8 = TRUE;
		Rte_Call_rpCS_TpmsChimeMdl_UpdateStatus_Operation(CHIME_REQID_TPMS_WARN,l_TpmsChimeState_U8);
	}
	else if((fl_signal_SystemSt_U8 == System_Status_2)
	&& ((fl_signal_TirePosition_WarningSt_U8 == TriePosition_0)
	|| (fl_signal_TirePosition_WarningSt_U8 == TriePosition_1)
	|| (fl_signal_TirePosition_WarningSt_U8 == TriePosition_2)
	|| (fl_signal_TirePosition_WarningSt_U8 == TriePosition_3)
	|| (fl_signal_TirePosition_WarningSt_U8 == TriePosition_4))
	&& ((fl_signal_SystemSt_Last_U8 != fl_signal_SystemSt_U8)
	|| (fl_signal_TirePosition_WarningSt_Last1_U8 != fl_signal_TirePosition_WarningSt_U8)))
	{
		l_TpmsChimeState_U8 = TRUE;
		Rte_Call_rpCS_TpmsChimeMdl_UpdateStatus_Operation(CHIME_REQID_TPMS_WARN,l_TpmsChimeState_U8);
	}
	
	fl_signal_TirePosition_WarningSt_Last1_U8 = fl_signal_TirePosition_WarningSt_U8;
	fl_signal_SystemSt_Last_U8 = fl_signal_SystemSt_U8;

	
}

static void f_Tpms_Pressure_Process(void)
{
	uint8    fl_signal_LF_Pressure_WarningSt_U8 = 0;
    uint8    fl_signal_RF_Pressure_WarningSt_U8 = 0;
    uint8    fl_signal_RR_Pressure_WarningSt_U8 = 0;
    uint8    fl_signal_LR_Pressure_WarningSt_U8 = 0;
	
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning(&fl_signal_LF_Pressure_WarningSt_U8); 
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning(&fl_signal_RF_Pressure_WarningSt_U8); 	
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning(&fl_signal_RR_Pressure_WarningSt_U8); 
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning(&fl_signal_LR_Pressure_WarningSt_U8);

	if((fl_signal_LF_Pressure_WarningSt_Last_U8 != fl_signal_LF_Pressure_WarningSt_U8)
	&& ((fl_signal_LF_Pressure_WarningSt_U8 == Tpms_LF_Pressure_Waring_1)
	|| (fl_signal_LF_Pressure_WarningSt_U8 == Tpms_LF_Pressure_Waring_2)
	|| (fl_signal_LF_Pressure_WarningSt_U8 == Tpms_LF_Pressure_Waring_4)
	|| (fl_signal_LF_Pressure_WarningSt_U8 == Tpms_LF_Pressure_Waring_5)))
	{
		l_TpmsChimeState_U8 = TRUE;
		Rte_Call_rpCS_TpmsChimeMdl_UpdateStatus_Operation(CHIME_REQID_TPMS_WARN,l_TpmsChimeState_U8);
	}
	
	if(((fl_signal_RF_Pressure_WarningSt_U8 == Tpms_RF_Pressure_Waring_1)
	|| (fl_signal_RF_Pressure_WarningSt_U8 == Tpms_RF_Pressure_Waring_2)
	|| (fl_signal_RF_Pressure_WarningSt_U8 == Tpms_RF_Pressure_Waring_4)
	|| (fl_signal_RF_Pressure_WarningSt_U8 == Tpms_RF_Pressure_Waring_5))
	&& (fl_signal_RF_Pressure_WarningSt_Last_U8 != fl_signal_RF_Pressure_WarningSt_U8))
	{
		l_TpmsChimeState_U8 = TRUE;
		Rte_Call_rpCS_TpmsChimeMdl_UpdateStatus_Operation(CHIME_REQID_TPMS_WARN,l_TpmsChimeState_U8);
	}
	
	if(((fl_signal_RR_Pressure_WarningSt_U8 == Tpms_RR_Pressure_Waring_1)
	|| (fl_signal_RR_Pressure_WarningSt_U8 == Tpms_RR_Pressure_Waring_2)
	|| (fl_signal_RR_Pressure_WarningSt_U8 == Tpms_RR_Pressure_Waring_4)
	|| (fl_signal_RR_Pressure_WarningSt_U8 == Tpms_RR_Pressure_Waring_5))
	&& (fl_signal_RR_Pressure_WarningSt_Last_U8 != fl_signal_RR_Pressure_WarningSt_U8))
	{
		l_TpmsChimeState_U8 = TRUE;
		Rte_Call_rpCS_TpmsChimeMdl_UpdateStatus_Operation(CHIME_REQID_TPMS_WARN,l_TpmsChimeState_U8);
	}
	
	if(((fl_signal_LR_Pressure_WarningSt_U8 == Tpms_LR_Pressure_Waring_1)
	|| (fl_signal_LR_Pressure_WarningSt_U8 == Tpms_LR_Pressure_Waring_2)
	|| (fl_signal_LR_Pressure_WarningSt_U8 == Tpms_LR_Pressure_Waring_4)
	|| (fl_signal_LR_Pressure_WarningSt_U8 == Tpms_LR_Pressure_Waring_5))
	&& (fl_signal_LR_Pressure_WarningSt_Last_U8 != fl_signal_LR_Pressure_WarningSt_U8))
	{
		l_TpmsChimeState_U8 = TRUE;
		Rte_Call_rpCS_TpmsChimeMdl_UpdateStatus_Operation(CHIME_REQID_TPMS_WARN,l_TpmsChimeState_U8);
	}
	
	fl_signal_LF_Pressure_WarningSt_Last_U8 = fl_signal_LF_Pressure_WarningSt_U8;
	fl_signal_RF_Pressure_WarningSt_Last_U8 = fl_signal_RF_Pressure_WarningSt_U8;
	fl_signal_RR_Pressure_WarningSt_Last_U8 = fl_signal_RR_Pressure_WarningSt_U8;
	fl_signal_LR_Pressure_WarningSt_Last_U8 = fl_signal_LR_Pressure_WarningSt_U8;
}

static void f_Tpms_Temperature_Warning_Process(void)
{
    uint8    fl_signal_Temperature_WarningSt_U8 = 0;
    uint8    fl_signal_TirePosition_WarningSt_U8 = 0;

	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Temperature_Warning(&fl_signal_Temperature_WarningSt_U8); 
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position(&fl_signal_TirePosition_WarningSt_U8); 
	
	if((fl_signal_Temperature_WarningSt_U8 == Temperature_Waring)
	&& ((fl_signal_TirePosition_WarningSt_U8 == TriePosition_1)
	||  (fl_signal_TirePosition_WarningSt_U8 == TriePosition_2)
	||  (fl_signal_TirePosition_WarningSt_U8 == TriePosition_3)
	||  (fl_signal_TirePosition_WarningSt_U8 == TriePosition_4))
	&& ((fl_signal_Temperature_WarningSt_Last_U8 != fl_signal_Temperature_WarningSt_U8)
	||  (fl_signal_TirePosition_WarningSt_Last2_U8 != fl_signal_TirePosition_WarningSt_U8)))
	{
		l_TpmsChimeState_U8 = TRUE;
		Rte_Call_rpCS_TpmsChimeMdl_UpdateStatus_Operation(CHIME_REQID_TPMS_WARN,l_TpmsChimeState_U8);
	}
	
	fl_signal_Temperature_WarningSt_Last_U8 = fl_signal_Temperature_WarningSt_U8;
	fl_signal_TirePosition_WarningSt_Last2_U8 = fl_signal_TirePosition_WarningSt_U8;
	
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
