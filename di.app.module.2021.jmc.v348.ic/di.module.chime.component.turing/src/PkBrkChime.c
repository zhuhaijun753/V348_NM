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

File Name        :  PkBrkChime.c
Module Short Name:  PkBrkChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define PKBRK_CHIME_VEHSPDLGT (0x1B) 
#define PKBRK_INVALID         (0xFFF)

#include "Rte_PkBrkChime.h"
#include "CmpLib.h"
#include "ChimeLogical.h"


CMPLIB_INSTANCE(PkBrkChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
static uint8    l_PkBrkChimeState_U8;
static boolean  l_PkBrk_flag         = TRUE;
static uint8    l_ProveoutCounter_U8 = FALSE;
// static uint8 	l_Last_pbrake_status_U8;

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
//#define VehsepLgt_Min 		300
//#define VehsepLgt_Max		4095

// #define PkBrk_Invalid_OFF   0x00
// #define PkBrk_Released_OFF  0x01
 #define PkBrk_Locked_ON     0x02
// #define PkBrk_Reserved_OFF  0x03



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
static void f_PkBrkinitialState(void);
static boolean f_PkBrk_input_verify(void);
static void f_PkBrkChime_Process(void);
static boolean PkBrk_PowerOn_Self_Test_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_PkBrkinitialState();
    Rte_Call_rpCS_PkBrkChime_UpdateStatus_Operation( CHIME_REQID_PRK_BRK , l_PkBrkChimeState_U8 );
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
	f_PkBrkinitialState();
    Rte_Call_rpCS_PkBrkChime_UpdateStatus_Operation( CHIME_REQID_PRK_BRK , l_PkBrkChimeState_U8 );
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
	f_PkBrkinitialState();
    Rte_Call_rpCS_PkBrkChime_UpdateStatus_Operation( CHIME_REQID_PRK_BRK , l_PkBrkChimeState_U8 );
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
	f_PkBrkinitialState();
    Rte_Call_rpCS_PkBrkChime_UpdateStatus_Operation( CHIME_REQID_PRK_BRK , l_PkBrkChimeState_U8 );
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

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	if((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage))
	{
		f_PkBrkinitialState();
	}
	else
	{
		if(fl_IGN_state == eIGN_RUN)
		{
			f_PkBrkChime_Process();
		}
		else
		{
			l_PkBrk_flag = TRUE; 
			l_ProveoutCounter_U8 = 0;
			f_PkBrkinitialState();
		}
	}

    Rte_Call_rpCS_PkBrkChime_UpdateStatus_Operation( CHIME_REQID_PRK_BRK , l_PkBrkChimeState_U8 );

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

static void f_PkBrkinitialState(void)
{
     l_PkBrkChimeState_U8 = FALSE;
}

static boolean f_PkBrk_input_verify(void)
{
    boolean	f_ret = TRUE;

	uint8 	fl_313_nvr_status_U8 = 0;
	uint8 	fl_313_msg_status_U8 = 0;
	uint8 	fl_221_nvr_status_U8 = 0;
	uint8 	fl_221_msg_status_U8 = 0;
	
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_221_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_221_msg_status_U8);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_NR(&fl_313_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_Tout(&fl_313_msg_status_U8);

	if((fl_313_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_313_msg_status_U8 == RTE_E_TIMEOUT))
	{
		f_ret = FALSE;
	}
	if((fl_221_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_221_msg_status_U8 == RTE_E_TIMEOUT))
	{
		f_ret = FALSE;
	}

	return f_ret;

}


static boolean PkBrk_PowerOn_Self_Test_Action(void)
{
 
    if(l_ProveoutCounter_U8 >= 30)
    {
        l_PkBrk_flag = FALSE;
    }
    else
    {
        l_PkBrk_flag = TRUE;
		l_ProveoutCounter_U8++;
    }

    return l_PkBrk_flag;
}


static void f_PkBrkChime_Process(void)
{
    uint8    fl_signal_Mechanical_Hand_Brake_U8;
	uint16   fl_signal_VehsepLgt_U16;
    //uint32   fl_signal_VehsepLgt_U32;
	uint8 	 fl_animationFlag = TRUE;
    boolean	f_ret = FALSE;

	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_signal_VehsepLgt_U16);
    //Rte_Read_rpSR_SpdMdl_VehicleIndicatedspeed_value(&fl_signal_VehsepLgt_U32);
	Rte_Read_rpSR_CANMSG_BCM_0x313_ComIn_BCM_Mechanical_Hand_Brake(&fl_signal_Mechanical_Hand_Brake_U8);
	
	if(fl_animationFlag == FALSE)
	{
		PkBrk_PowerOn_Self_Test_Action();
		if(l_PkBrk_flag == FALSE)
		{
			f_ret = f_PkBrk_input_verify();
			if(f_ret == TRUE)
			{
				if(fl_signal_VehsepLgt_U16 != PKBRK_INVALID)
				{
					if ((fl_signal_VehsepLgt_U16  > PKBRK_CHIME_VEHSPDLGT) && (fl_signal_Mechanical_Hand_Brake_U8 == PkBrk_Locked_ON))
					{
						l_PkBrkChimeState_U8 = TRUE;
					}
					else
					{
						f_PkBrkinitialState();
					}
				}
				else
				{
					f_PkBrkinitialState();
				}
			}
			else
			{
				f_PkBrkinitialState();
			}
		}
		else
		{
			f_PkBrkinitialState();
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
