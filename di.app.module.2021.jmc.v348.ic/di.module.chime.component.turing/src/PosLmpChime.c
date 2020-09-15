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

File Name        :  PosLmpChime.c
Module Short Name:  PosLmpChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define PosLmpChime_C

#include "Rte_PosLmpChime.h"
#include "CmpLib.h"
#include "ChimeLogical.h"


CMPLIB_INSTANCE(PosLmpChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
static uint8    l_PosLmpChimeState_U8 = FALSE;
// static uint8 	l_PosLmpChimeState_Last_U8;

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
#define POS_LAMP_ON   (uint8)0x2
#define DOOR_OPEN     (uint8)0x2

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
static void f_PosLmpinitialState(void);
static boolean f_PosLmp_input_verify(void);
static void f_PosLmpChime_Process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_PosLmpinitialState();
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
	f_PosLmpinitialState();
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
	f_PosLmpinitialState();
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
	f_PosLmpinitialState();
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	eIgnState     fl_IGN_state     = eIGN_OFF;
	EBatteryState fl_Battery_state = eBatteryState_Normal;
    boolean	      f_ret            = FALSE;

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	if(((EBatteryState)eBatteryState_UnderVoltage == fl_Battery_state) ||\
	   ((EBatteryState)eBatteryState_OverVoltage  == fl_Battery_state))
	{
		f_PosLmpinitialState();
	}
	else
	{
        f_ret = f_PosLmp_input_verify();
        if((boolean)TRUE == f_ret)
        {
	    	switch(fl_IGN_state)
	    	{
		    	case eIGN_RUN:
		    		f_PosLmpinitialState();
			    	break;
		    	case eIGN_OFF:
		    		f_PosLmpChime_Process();
                    break;
		    	default:
		    	    f_PosLmpinitialState();
		    		break;
		    }
        }
        else
        {
            f_PosLmpinitialState();
        }
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

static void f_PosLmpinitialState(void)
{
	l_PosLmpChimeState_U8 = FALSE;
	// l_PosLmpChimeState_Last_U8 = FALSE;
	Rte_Call_rpCS_PosLmpChime_UpdateStatus_Operation(CHIME_REQID_D2_POS_LAMP,l_PosLmpChimeState_U8);
}

static boolean f_PosLmp_input_verify(void)
{
	boolean	f_ret = TRUE;
    uint8 	fl_318_nvr_status_U8 = 0;
	uint8 	fl_318_msg_status_U8 = 0;
	uint8 	fl_310_nvr_status_U8 = 0;
	uint8 	fl_310_msg_status_U8 = 0;
    Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(&fl_318_nvr_status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(&fl_318_msg_status_U8);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&fl_310_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&fl_310_msg_status_U8);
	if((fl_318_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_318_msg_status_U8 == RTE_E_TIMEOUT)
	|| (fl_310_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_310_msg_status_U8 == RTE_E_TIMEOUT))
	{
		f_ret = FALSE;
	}
	
	return f_ret;
}

static void f_PosLmpChime_Process(void)
{
	uint8 	fl_signal_PosLmpSts_U8     = 0;
	uint8	fl_signal_DoorStsFrntLe_U8 = 0;
	// uint8	fl_signal_BackHomeSts_U8 = 0;

	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpSts(&fl_signal_PosLmpSts_U8);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&fl_signal_DoorStsFrntLe_U8);
	//Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BackHomeSts(&fl_signal_BackHomeSts_U8);
	
	if((POS_LAMP_ON == fl_signal_PosLmpSts_U8) && (DOOR_OPEN == fl_signal_DoorStsFrntLe_U8))
	{
		l_PosLmpChimeState_U8 = TRUE;
	}
	else
	{
		l_PosLmpChimeState_U8 = FALSE;
	}

	// l_PosLmpChimeState_Last_U8 = fl_signal_PosLmpStsSt_U8;
	Rte_Call_rpCS_PosLmpChime_UpdateStatus_Operation(CHIME_REQID_D2_POS_LAMP,l_PosLmpChimeState_U8);

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

