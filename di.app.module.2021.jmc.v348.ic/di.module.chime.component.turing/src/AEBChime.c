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

File Name        :  AEBChime.c
Module Short Name:  AEBChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define AEBCHIME_C

#include "Rte_AEBChime.h"
#include "CmpLib.h"
#include "ChimeLogical.h"

// #define AEBCHIME_MAX	7

// #define FCWS_WARNING				0
// #define AEB_WARNING					1
// #define TAKEOVER_WARNING			2
// #define TAKEOVER_IMMEDIATE_WARNING	3
// #define LLDW_WARNING				4
// #define RLDW_WARNING				5
// #define HANDOFF_WARNING				6


CMPLIB_INSTANCE(AEBChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
// static uint8	l_AEBChimeIndex_U8;

// ChimeIndex AEBChimeGroup[AEBCHIME_MAX] = 
// {
//     {FCWS_WARNING , CHIME_REQID_FCWS_AEB, FALSE},
//     {AEB_WARNING ,  CHIME_REQID_EBA_AEB,  FALSE},
//     {TAKEOVER_WARNING , CHIME_REQID_TAKEOVER, FALSE},
//     {TAKEOVER_IMMEDIATE_WARNING , CHIME_REQID_TAKEOVER_IMMEDIATE, FALSE},
//     {LLDW_WARNING , CHIME_REQID_LLDW_AEB, FALSE},
//     {RLDW_WARNING , CHIME_REQID_RLDW_AEB, FALSE},
//     {HANDOFF_WARNING , CHIME_REQID_HANDOFF_AEB, FALSE}
// };




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
//#define	AEB_MISSING_COUNTER 	10


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
// static void f_AEBChimeInitialState(void);
// static void f_AEBChimePriorityProc(void);
// static void f_AEBChimeState_Update(void);
// static void f_AEBChime_Process(void);
// static boolean f_AEBChime_input_verify(void);
// static void f_AEBChime_input_process(void);



//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	//f_AEBChimeInitialState();
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
	//f_AEBChimeInitialState();
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
	//f_AEBChimeInitialState();
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
	//f_AEBChimeInitialState();
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	// eIgnState fl_IGN_state;
	// EBatteryState fl_Battery_state;
    // boolean fl_VehicleCfg;

	// Rte_Read_rpSR_GdtCtrl_AEB_FCW_ACC_Cfg(&fl_VehicleCfg);
	// if(fl_VehicleCfg == FALSE)
	// {
	// 	return E_OK;
	// }
	
	// Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	// Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

	// f_AEBChimeInitialState();
	// if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	// {		
	// }
	// else
	// {
	// 	switch(fl_IGN_state)
	// 	{
	// 		case eIGN_RUN:
	// 			f_AEBChime_Process();
	// 			break;
	// 		case eIGN_OFF:	
	// 		default:
	// 			break;
	// 	}
	// }
	// f_AEBChimeState_Update();
	//Rte_Call_rpCS_AEBChimel_UpdateStatus_Operation(CHIME_REQID_FCWS_AEB,TRUE);
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

// static void f_AEBChimeInitialState(void)
// {	
// 	uint8 i;
	
// 	l_AEBChimeIndex_U8 = 0xFF;
// 	for(i =0; i < AEBCHIME_MAX;i++)
// 	{
// 		AEBChimeGroup[i].ChimeState = FALSE;
// 	}
// }

// static void f_AEBChimePriorityProc(void)
// {
// 	uint8 i;
// 	for(i =0; i < AEBCHIME_MAX;i++)
// 	{
// 		if(AEBChimeGroup[i].ChimeState == TRUE)
// 		{
// 			l_AEBChimeIndex_U8 = AEBChimeGroup[i].ChimeReqID;
// 			break;
// 		}
// 	}
// }

// static void f_AEBChimeState_Update(void)
// {
// 	uint8 i;
// 	for(i =0; i < AEBCHIME_MAX;i++)
// 	{
// 		if(AEBChimeGroup[i].ChimeReqID == l_AEBChimeIndex_U8)
// 		{
// 			Rte_Call_rpCS_AEBChimel_UpdateStatus_Operation(AEBChimeGroup[i].ChimeReqID ,TRUE);
// 		}
// 		else 
// 		{
// 			Rte_Call_rpCS_AEBChimel_UpdateStatus_Operation(AEBChimeGroup[i].ChimeReqID ,FALSE);
// 		}		
// 	}
// }

// static void f_AEBChime_Process(void)
// {
//     boolean fl_animationFlag = TRUE;
// 	boolean	f_ret = FALSE;
// 	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
//     if(fl_animationFlag == FALSE)
//     {
// 		f_ret = f_AEBChime_input_verify();
// 		if(f_ret == TRUE)
// 		{	
// 			f_AEBChime_input_process();
// 			f_AEBChimePriorityProc();
// 		}
//     }
// }


// static boolean f_AEBChime_input_verify(void)
// {
// 	boolean	f_ret = TRUE;
// 	uint8 	fl_CAN_Camera_nvr_status_U8;
// 	uint8 	fl_CAN_Camera_msg_status_U8;
// 	uint8 	fl_CAN_RADA2_nvr_status_U8;
// 	uint8 	fl_CAN_RADA2_msg_status_U8;

// 	Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_NR(&fl_CAN_Camera_nvr_status_U8);
// 	Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_Tout(&fl_CAN_Camera_msg_status_U8);
// 	Rte_Read_rpSR_CANMSG_GW_RADA2_ComIn_NR(&fl_CAN_RADA2_nvr_status_U8);
// 	Rte_Read_rpSR_CANMSG_GW_RADA2_ComIn_Tout(&fl_CAN_RADA2_msg_status_U8);

// 	if(fl_CAN_Camera_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_Camera_msg_status_U8 == RTE_E_TIMEOUT)
// 	{
// 		f_AEBChimeInitialState();
// 		f_ret = FALSE;
// 	}

// 	if(fl_CAN_RADA2_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_RADA2_msg_status_U8 == RTE_E_TIMEOUT)
// 	{
// 		f_AEBChimeInitialState();
// 		f_ret = FALSE;		
// 	}
	
// 	return f_ret;
// }

// static void f_AEBChime_input_process(void)
// {	
// 	uint8 fl_AEB_AlertReq_U8;
// 	uint8 fl_ACC_PopUp_U8;
// 	uint8 fl_LDW_Flag_U8;
// 	uint8 fl_HandsOff_U8;
	
// 	Rte_Read_rpSR_CANMSG_GW_RADA2_ComIn_GW_AEB_Alert_Req(&fl_AEB_AlertReq_U8);
// 	Rte_Read_rpSR_CANMSG_GW_RADA2_ComIn_GW_ACC_Pop_Up_Message(&fl_ACC_PopUp_U8);
// 	Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_LDW_Flag(&fl_LDW_Flag_U8);
// 	Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_Hands_off_warning(&fl_HandsOff_U8);

// 	if(fl_AEB_AlertReq_U8 == 1)
// 	{
// 		AEBChimeGroup[FCWS_WARNING].ChimeState = TRUE;
// 	}
// 	else if(fl_AEB_AlertReq_U8 == 2)
// 	{
// 		AEBChimeGroup[AEB_WARNING].ChimeState = TRUE;
// 	}

// 	if(fl_ACC_PopUp_U8 == 7)
// 	{
// 		AEBChimeGroup[TAKEOVER_WARNING].ChimeState = TRUE;
// 	}
// 	else if(fl_ACC_PopUp_U8 == 8)
// 	{
// 		AEBChimeGroup[TAKEOVER_IMMEDIATE_WARNING].ChimeState = TRUE;
// 	}

// 	if(fl_LDW_Flag_U8 == 1)
// 	{
// 		AEBChimeGroup[LLDW_WARNING].ChimeState = TRUE;
// 	}
// 	else if(fl_LDW_Flag_U8 == 2)
// 	{
// 		AEBChimeGroup[RLDW_WARNING].ChimeState = TRUE;
// 	}

// 	if(fl_HandsOff_U8 == 1)
// 	{
// 		AEBChimeGroup[HANDOFF_WARNING].ChimeState = TRUE;
// 	}	
// }





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
