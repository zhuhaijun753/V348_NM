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

File Name        :  LowFuelChime.c
Module Short Name:  LowFuelChime
VOBName          :  
Author           :  
DeLowFueliption      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define LOWFUELCHIME_C

#include "Rte_LowFuelChime.h"
#include "ChimeLogical.h"
#include "CmpLib.h"



CMPLIB_INSTANCE(LowFuelChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/

static boolean  l_lowFuelChimeState_U8 = FALSE;
static boolean  l_lowFuel_tt_state_u8  = FALSE;
// static uint8    l_lowFuel_state_u8     = 2;
static boolean  l_LowFuel_Flag         = FALSE;
static uint8    l_ProveoutCounter_U8   = FALSE;



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
// #define LOWFUEL_500_CHIME_TIMES		5

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
static void f_LowFuelinitialState(void);
static void f_LowFuelChime_Process(void);
static boolean LowFuel_PowerOn_Self_Test_Action(void);


//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_LowFuelinitialState();
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
	f_LowFuelinitialState();
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
	f_LowFuelinitialState();
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
	f_LowFuelinitialState();
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	eIgnState fl_IGN_state = eIGN_OFF;
	eIgnState fl_Battery_state = eBatteryState_Normal;
	
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	
	if((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage))
	{
		f_LowFuelinitialState();
	}
	else
	{
		switch(fl_IGN_state)
		{
	 		case eIGN_RUN:
	  	 		f_LowFuelChime_Process();
				break;
			case eIGN_OFF:	
				l_LowFuel_Flag		   = FALSE;
				l_ProveoutCounter_U8   = FALSE;
				f_LowFuelinitialState();
			default:
				break;
		}
	}

	Rte_Call_rpCS_LowFuelChime_UpdateStatus_Operation(CHIME_REQID_LOWFUEL,l_lowFuelChimeState_U8);
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

static boolean LowFuel_PowerOn_Self_Test_Action(void)
{
	if(l_ProveoutCounter_U8 >= 30)
    {
        l_LowFuel_Flag = TRUE;
    }
    else
    {
        l_LowFuel_Flag = FALSE;
		l_ProveoutCounter_U8++;
    }

    return l_LowFuel_Flag;
}

static void f_LowFuelinitialState(void)
{
	l_lowFuelChimeState_U8 = FALSE;
	l_lowFuel_tt_state_u8  = FALSE;
	// l_lowFuel_state_u8     = 2;
	Rte_Call_rpCS_LowFuelChime_UpdateStatus_Operation(CHIME_REQID_LOWFUEL,FALSE);
}

static void f_LowFuelChime_Process(void)
{
	boolean fl_animationFlag = TRUE;
	boolean fl_fuelWarning   = FALSE;	
	
	Rte_Read_rpSR_TILowFule_tt_state_value(&fl_fuelWarning);   // get TT state value
 	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
	
	
	if(fl_animationFlag == FALSE)
	{
		LowFuel_PowerOn_Self_Test_Action();

		if(l_LowFuel_Flag == TRUE)
		{
			if(fl_fuelWarning == TRUE)	  //If TT on,Then chime on
			{
				l_lowFuelChimeState_U8 = TRUE;
			}
			else
			{
				l_lowFuelChimeState_U8 = FALSE;
			}
		}
		else
		{
			f_LowFuelinitialState();
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
