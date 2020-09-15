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

File Name        :  APAChime.c
Module Short Name:  APAChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define APACHIME_C

#include "Rte_APAChime.h"
#include "ChimeLogical.h"
#include "CmpLib.h"


CMPLIB_INSTANCE(APAChime)

/*****************************************************************************
*                                 Macro Definitions                          *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/
// #define APA2_MISSING_COUNTER	30
// #define APACHIME_MAX			8

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
//static uint8    l_APA2ChimeState_U8;

// ChimeIndex APAChimeGroup[APACHIME_MAX] = 
// {
	// {9 , CHIME_REQID_SYSTEM_FAULT, FALSE},
	// {10 , CHIME_REQID_POWER_ON_SELFCHECK, FALSE},
	// {14 , CHIME_REQID_REAR_UPA_WARN1, FALSE},
	// {13 , CHIME_REQID_REAR_UPA_WARN2, FALSE},
	// {12 , CHIME_REQID_REAR_UPA_WARN3, FALSE},
	// {15 , CHIME_REQID_REAR_UPA_WARNLONG, FALSE}
// };

/*****************************************************************************
*                                 Manifest Constants                         *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/

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
// static void f_APAChimeInitialState(void);
// static void f_APAChime_Process(void);
// static uint8 f_APAChime_input_verify(void);
// static void f_APAChime_input_process(void);
// static void f_APAChimeState_Update(void);



//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	// f_APAChimeInitialState();
	//l_APA2ChimeState_U8 = FALSE;
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
	// f_APAChimeInitialState();
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
	
	// Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	// Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	
	// if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	// {
		// f_APAChimeInitialState();
	// }
	// else
	// {
		// switch(fl_IGN_state)
		// {
			// case eIGN_RUN:
				// f_APAChime_Process();
				// break;
			// case eIGN_OFF:					
				// f_APAChimeInitialState();
			// default:
				// break;
		// }
	// }
	// f_APAChimeState_Update();
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

// static void f_APAChimeInitialState(void)
// {	
	// uint8 i;
	// for(i =0; i < APACHIME_MAX;i++)
	// {
		// APAChimeGroup[i].ChimeState = FALSE;
	// }
// }

// static void f_APAChime_Process(void)
// {
    // boolean fl_animationFlag = TRUE;
	// boolean	f_ret = FALSE;
	
    // Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
    // if(fl_animationFlag == FALSE)
    // {
		// f_ret = f_APAChime_input_verify();
		// if(f_ret == TRUE)
		// {	
			// f_APAChime_input_process();
		// }
    // }	
// }


// static boolean f_APAChime_input_verify(void)
// {
	// boolean	f_ret = TRUE;
	// uint8 	fl_CAN_nvr_status_U8;
	// uint8 	fl_CAN_msg_status_U8;

	// Rte_Read_tiSR_CANMSG_APA2_ComIn_NR(&fl_CAN_nvr_status_U8);
	// Rte_Read_tiSR_CANMSG_APA2_ComIn_Tout(&fl_CAN_msg_status_U8);

	// if(fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	// {
		// f_APAChimeInitialState();
		// f_ret = FALSE;
	// }
	
	// return f_ret;
// }

// static void f_APAChime_input_process(void)
// {	
	// uint8 i;
	// uint8 fl_chimeRequest;
	// Rte_Read_tiSR_CANMSG_APA2_ComIn_APA_ChimeRequest(&fl_chimeRequest);	
		
	// for(i =0; i < APACHIME_MAX;i++)
	// {
		// if(APAChimeGroup[i].Request == fl_chimeRequest)
		// {
			// APAChimeGroup[i].ChimeState = TRUE;
		// }
		// else 
		// {
			// APAChimeGroup[i].ChimeState = FALSE;		
		// }
	// }	
// }

// static void f_APAChimeState_Update(void)
// {
	// uint8 i;
	// uint8 fl_APA2ChimeIndex_U8;
	// uint8 fl_APA2ChimeState_U8;
	// for(i =0; i < APACHIME_MAX;i++)
	// {
		// fl_APA2ChimeIndex_U8 = APAChimeGroup[i].ChimeReqID;
		// fl_APA2ChimeState_U8 = APAChimeGroup[i].ChimeState;
		// Rte_Call_rpCS_APAChime_UpdateStatus_Operation(fl_APA2ChimeIndex_U8,fl_APA2ChimeState_U8);
	// }
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
