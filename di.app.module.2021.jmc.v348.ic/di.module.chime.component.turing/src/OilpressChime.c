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

File Name        :  OilpressChime.c
Module Short Name:  OilpressChime
VOBName          :  
Author           :  
Description      :  For low oil pressure sound function

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define OilpressChime_C

#include "Rte_OilpressChime.h"
#include "CmpLib.h"
#include "ChimeLogical.h"


CMPLIB_INSTANCE(OilpressChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/


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
static uint8  l_invalid_cnt = 0;


/*                                 Type Declarations                          *
******************************************************************************/
#define Engine_Speed_ON		   ((uint32)600)
#define Engine_Speed_OFF	   ((uint32)400)
#define Engine_Speed_Invalid   ((uint32)32767)
#define HW_Invalid			   ((uint8)0xFF)


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/
static uint8  l_Low_OilPressure_State_U8 = FALSE;
// static uint8  l_Low_OilPressure_Last_State_U8 = FALSE;


/*****************************************************************************
*                   Functions                                                *
******************************************************************************/
static void f_Low_OilPressure_initial(void);
static void f_Low_OilPressure_Process(void);
static boolean f_Low_OilPressure_input_verity(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_Low_OilPressure_initial();
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
	f_Low_OilPressure_initial();
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
	f_Low_OilPressure_initial();
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
	f_Low_OilPressure_initial();
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	boolean f_ret = FALSE;
	EBatteryState fl_Battery_state = eBatteryState_Normal;
	eIgnState fl_IGN_state = eIGN_OFF;
	
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

	if ((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage) || (fl_Battery_state == eBatteryState_OverVoltagePhase1)) 
	{
		f_Low_OilPressure_initial();
	}
	else
	{
		//f_ret = f_Low_OilPressure_input_verity();
		if ((f_ret == TRUE) && (fl_IGN_state == eIGN_RUN))
		{
			//f_Low_OilPressure_Process();
		}
		else
		{ 
			f_Low_OilPressure_initial();
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


static void f_Low_OilPressure_initial(void)
{
	l_invalid_cnt = 0;
	l_Low_OilPressure_State_U8 = FALSE;
	// l_Low_OilPressure_Last_State_U8 = FALSE;
	Rte_Call_rpCS_OilpressChime_TIMdlUpdateTtStatus_Operation(CHIME_REQID_Low_OilPress, l_Low_OilPressure_State_U8);
}
#if 0
static void f_Low_OilPressure_Process(void)
{
	uint16 fl_EngSpeed_u16 = 0;
	uint8 OilPressurePin = HW_Invalid;
	
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&fl_EngSpeed_u16);
	Rte_Call_rpCS_TIIoHwAb_DIn_Impl_GetPin(eIOPinInId_MDI_OIL_PRESSURE_SW,&OilPressurePin);

	if(((fl_EngSpeed_u16 > Engine_Speed_ON) && (fl_EngSpeed_u16 < Engine_Speed_Invalid)) && (FALSE == OilPressurePin))  // low level
	{
		if(l_invalid_cnt >= 30)
		{
			l_invalid_cnt = 30;
			l_Low_OilPressure_State_U8 = TRUE;
		}
		else
		{
			l_invalid_cnt++;
			l_Low_OilPressure_State_U8 = FALSE;
		}
	}
	else if((fl_EngSpeed_u16 < Engine_Speed_OFF ) || ( TRUE == OilPressurePin ))
	{
		l_invalid_cnt = 0;
		l_Low_OilPressure_State_U8 = FALSE;
	}
	else
	{
		l_invalid_cnt = 0;
		l_Low_OilPressure_State_U8 = FALSE;
	}

	Rte_Call_rpCS_OilpressChime_TIMdlUpdateTtStatus_Operation(CHIME_REQID_Low_OilPress, l_Low_OilPressure_State_U8);
}


static boolean f_Low_OilPressure_input_verity(void)
{
	boolean f_ret = TRUE;

	uint8 	fl_CAN_10B_msg_status_U8 = 0;
	uint8	fl_CAN_10B_nvr_status_U8 = 0;
	
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&fl_CAN_10B_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&fl_CAN_10B_msg_status_U8);
	if((fl_CAN_10B_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_10B_msg_status_U8 == RTE_E_TIMEOUT))
	{
		f_ret = FALSE;
	}
	
	return f_ret;
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

