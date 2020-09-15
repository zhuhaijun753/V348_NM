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

File Name        :  KeyInOnChime.c
Module Short Name:  KeyInOnChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define KeyInOnChime_C

#include "Rte_KeyInOnChime.h"
#include "ChimeLogical.h"
#include "CmpLib.h"


CMPLIB_INSTANCE(KeyInOnChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
// static uint8    l_KeyInOnChimeState_U8;
// static uint8    l_KeyInOnChimeState_Last_U8 = 0;

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
// #define KeyReminder_OFF   0x00
// #define KeyReminder_ON    0x01

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
// static void f_KeyInOninitialState(void);
// static boolean f_KeyInOn_input_verify(void);
// static void f_KeyInOnChime_Process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
    // f_KeyInOninitialState();
    // Rte_Call_rpCS_KeyInOnChime_UpdateStatus_Operation(CHIME_REQID_D2_KEY_IN_ON,l_KeyInOnChimeState_U8);
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
    // f_KeyInOninitialState();
    // Rte_Call_rpCS_KeyInOnChime_UpdateStatus_Operation(CHIME_REQID_D2_KEY_IN_ON,l_KeyInOnChimeState_U8);
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
    // f_KeyInOninitialState();
    // Rte_Call_rpCS_KeyInOnChime_UpdateStatus_Operation(CHIME_REQID_D2_KEY_IN_ON,l_KeyInOnChimeState_U8);
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
    // f_KeyInOninitialState();
    // Rte_Call_rpCS_KeyInOnChime_UpdateStatus_Operation(CHIME_REQID_D2_KEY_IN_ON,l_KeyInOnChimeState_U8);
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
    boolean	f_ret = FALSE;

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{
		f_KeyInOninitialState();
	}
	else
	{
        f_ret = f_KeyInOn_input_verify();
        if(f_ret == TRUE)
        {
	    	switch(fl_IGN_state)
	    	{
		    	case eIGN_RUN:
		    		f_KeyInOninitialState();
			    	break;
		    	case eIGN_OFF:					
		    		// f_KeyInOnChime_Process();                                    
                    break;
		    	default:
		    		break;
		    }
        }
        else
        {
            f_KeyInOninitialState();
        }
	}
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
static void f_KeyInOninitialState(void)
{
	l_KeyInOnChimeState_U8 = FALSE;
    l_KeyInOnChimeState_Last_U8 = FALSE;
    Rte_Call_rpCS_KeyInOnChime_UpdateStatus_Operation(CHIME_REQID_D2_KEY_IN_ON,FALSE);
}

static boolean f_KeyInOn_input_verify(void)
{
    boolean	f_ret = TRUE;
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_Tout(&fl_CAN_msg_status_U8);	
	if(fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_ret = FALSE;
	}
	
	return f_ret;

}

static void f_KeyInOnChime_Process(void)
{
    uint8   fl_signal_KeyInOnSt_U8 = 0;
    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyReminderWarning(&fl_signal_KeyInOnSt_U8);
    if( l_KeyInOnChimeState_Last_U8 != fl_signal_KeyInOnSt_U8 )
    {
        switch(fl_signal_KeyInOnSt_U8)
        {
            case KeyReminder_OFF:
                l_KeyInOnChimeState_U8 = FALSE;
                break;
            case KeyReminder_ON:
                l_KeyInOnChimeState_U8 = TRUE;
                break;
            default:
                l_KeyInOnChimeState_U8 = FALSE;
                break;
        }
        Rte_Call_rpCS_KeyInOnChime_UpdateStatus_Operation(CHIME_REQID_D2_KEY_IN_ON,l_KeyInOnChimeState_U8);

    }

    l_KeyInOnChimeState_Last_U8 = fl_signal_KeyInOnSt_U8;
}
#endif
