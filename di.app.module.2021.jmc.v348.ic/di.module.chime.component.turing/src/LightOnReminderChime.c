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

File Name        :  DoorOpenChime.c
Module Short Name:  DoorOpenChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define LIGHTONREMINDERCHIME_C

#include "Rte_LightOnReminder.h"
#include "ChimeLogical.h"
#include "CmpLib.h"



CMPLIB_INSTANCE(LightOnReminderChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/

static boolean	l_LightOnChimeState_U8;

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
#define LIGHTONREMIND    1
#define KEYINREMIND     19

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
static void f_LightOninitialState(void);
static void f_LightOnReminder_Process(void);
static boolean f_LightOn_input_verify(void);
static void f_LightOn_input_process(void);


//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
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

#if 0
	eIgnState fl_IGN_state;
	EBatteryState fl_Battery_state;
	
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	
	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{
		f_LightOninitialState();
	}
	else
	{
		switch(fl_IGN_state)
		{
			case eIGN_OFF:					
				f_LightOnReminder_Process();		
				break;
			case eIGN_RUN:
				f_LightOninitialState();
				break;
			default:
				break;
		}
	}	

	//Rte_Write_ppSR_CANMSG_IC1_ComOut_IC_KeyInRemind(l_keyInRemind_U8);
	//Rte_Write_ppSR_CANMSG_IC1_ComOut_IC_LightOnRemind(l_lightOnReminder_U8);
	Rte_Call_rpCS_LightOnReminder_UpdateStatus_Operation(CHIME_REQID_D2_LIGHT_ON,l_LightOnChimeState_U8);

#endif
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

#if 0
static void f_LightOninitialState(void)
{
	l_LightOnChimeState_U8 = FALSE;
}

static void f_LightOnReminder_Process(void)
{
	boolean f_ret = FALSE;
	f_ret = f_LightOn_input_verify();
	if(f_ret == TRUE)
	{	
		f_LightOn_input_process();
	}
}

static boolean f_LightOn_input_verify(void)
{
	boolean f_ret = TRUE;
	uint8 	fl_CAN_nvr_status_U8;
	uint8 	fl_CAN_msg_status_U8;

	Rte_Read_rpSR_CANMSG_IBCM3_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_IBCM3_ComIn_Tout(&fl_CAN_msg_status_U8);

	if(fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_LightOninitialState();
		f_ret = FALSE;
	}
	
	return f_ret;
}

static void f_LightOn_input_process(void)
{
	uint8  fl_signal_BuzzerReq_U8 = 0;

	Rte_Read_rpSR_CANMSG_IBCM3_ComIn_BCM_BuzzerRequest(&fl_signal_BuzzerReq_U8);
	f_LightOninitialState();

	switch(fl_signal_BuzzerReq_U8)
	{
		case LIGHTONREMIND:
			l_LightOnChimeState_U8 = TRUE;
			break;
		case KEYINREMIND:
			l_LightOnChimeState_U8 = FALSE;
			break;
		default:
			break;
	}
}

#endif

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
