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

File Name        :  SCRChimeMdl.c
Module Short Name:  SCRChimeMdl
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define SCRChimeMdl_C

#include "Rte_SCRChimeMdl.h"
#include "CmpLib.h"
#include "ChimeLogical.h"


CMPLIB_INSTANCE(SCRChimeMdl)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
static uint8	l_IGN_statePre_U8 = eIGN_OFF;

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
#define TURN_IN_F_SLOW   3U
#define TURN_IN_F_QUICK	 2U
#define TURN_IN_ON 	     1U
#define TURN_IN_OFF      0U

#define TURN_IN_STATE_6      6U
#define TURN_IN_STATE_5      5U
#define TURN_IN_STATE_4      4U
#define TURN_IN_STATE_3      3U
#define TURN_IN_STATE_2      2U
#define TURN_IN_STATE_1      1U
#define TURN_IN_STATE_0      0U
#define PROVEOUT_TIME_CNT                  (uint8)30      /*3s timer for 100ms period*/
#define VEHICLE_CONFIGURATION_SIX_CHIME (5)

/*                                 Type Declarations                          *
******************************************************************************/


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/
static boolean  l_SCRChime_State_U8_2HZ_1 = FALSE;
static UINT8    l_UreaWarn_Last_State_U8 = FALSE;
static boolean  l_SCRProveOut_B = TRUE;
static uint8    l_ProveoutCounter_U8 = 0;


// static UINT8  l_SCRChime_State_U8_1HZ = FALSE;
// static UINT8  l_SCRChime_State_U8_2HZ_2 = FALSE;
// static UINT8  l_DPF_State_U8 = FALSE;
// static UINT8  l_OBDWarn_Last_State_U8 = FALSE;
// static UINT8  l_DPF_Last_State_U8 = FALSE;

// static UINT8  l_UreaTxt_Last_State_U8       	= FALSE;
// static UINT8  l_AddUreaWarning_Last_State_U8    = FALSE;
// static UINT8  l_UreaQty_Last_State_U8     	    = FALSE;
// static UINT8  l_UreaSysWarning_Last_State_U8    = FALSE;
// static UINT8  l_NoxWarning_Last_State_U8        = FALSE;

/*****************************************************************************
*                   Functions                                                *
******************************************************************************/
static void f_SCRChime_initial(void);
static void f_SCRChime_Process(void);
// static uint8 f_SCRChime_input_verity(void);
static void SCR_PowerOn_Self_Test_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_SCRChime_initial();
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
	f_SCRChime_initial();
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
	f_SCRChime_initial();
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
	f_SCRChime_initial();
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	// boolean  f_ret = FALSE;
	EBatteryState  fl_Battery_state            = eBatteryState_Normal;
	eIgnState  	   fl_IGN_state                = eIGN_OFF;
	boolean        f_animationFlag             = TRUE;
	uint8          fl_CHIME_REQID_SCR_2HZ_1_U8 = 0;
	
	uint8 IsEngineCfg_SCR_Chime = FALSE;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_EMISSION_LEVELS,&IsEngineCfg_SCR_Chime);

	if(VEHICLE_CONFIGURATION_SIX_CHIME == IsEngineCfg_SCR_Chime) 
	{
		Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
		Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
		
		if (((EBatteryState)eBatteryState_UnderVoltage == fl_Battery_state) ||\
			((EBatteryState)eBatteryState_OverVoltage  == fl_Battery_state)) 
		{
			f_SCRChime_initial();
		}
		else
		{
			if((eIgnState)eIGN_RUN == fl_IGN_state)
			{
				if((eIgnState)eIGN_OFF == l_IGN_statePre_U8)
				{
					l_SCRProveOut_B = TRUE;
					l_ProveoutCounter_U8 = 0;
				}
				else if((eIgnState)eIGN_RUN == l_IGN_statePre_U8)
				{
					Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&f_animationFlag);
					if((boolean)FALSE == f_animationFlag)
					{
						SCR_PowerOn_Self_Test_Action(); /* Avoid direct response on startup */
						if((boolean)FALSE == l_SCRProveOut_B)
						{
							f_SCRChime_Process();
						}
					}
				}
			}
			else
			{
				l_ProveoutCounter_U8 = 0;
				f_SCRChime_initial();
			}
		
			l_IGN_statePre_U8 = fl_IGN_state;
		}
		
		Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_SCRChime_2hz_1(CHIME_REQID_SCR_2HZ_1);
		Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_SCRChime_2hz_1(&fl_CHIME_REQID_SCR_2HZ_1_U8);
		
		if ((uint8)TRUE == fl_CHIME_REQID_SCR_2HZ_1_U8)
		{
			Rte_Call_rpCS_SCRChimeMdl_UpdateStatus_Operation(CHIME_REQID_SCR_2HZ_1, FALSE);
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

static void SCR_PowerOn_Self_Test_Action(void)
{
	if(PROVEOUT_TIME_CNT <= l_ProveoutCounter_U8)
    {
        l_SCRProveOut_B = FALSE;
    }
    else
    {
        l_ProveoutCounter_U8++;
        l_SCRProveOut_B = TRUE;
    }
}

static void f_SCRChime_initial(void) 
{
    l_SCRChime_State_U8_2HZ_1 = FALSE;
    l_UreaWarn_Last_State_U8 = FALSE;
    l_IGN_statePre_U8 = eIGN_OFF;
    l_SCRProveOut_B = TRUE;
    l_ProveoutCounter_U8 = 0;

	// l_SCRChime_State_U8_1HZ = FALSE;
	// l_SCRChime_State_U8_2HZ_2 = FALSE;
	// l_DPF_State_U8 = FALSE;
	// l_OBDWarn_Last_State_U8 = FALSE;
	// l_DPF_Last_State_U8 = FALSE;

    // l_UreaTxt_Last_State_U8 = FALSE;
    // l_AddUreaWarning_Last_State_U8 = FALSE;
    // l_UreaQty_Last_State_U8 = FALSE;
    // l_UreaSysWarning_Last_State_U8 = FALSE;
    // l_NoxWarning_Last_State_U8 = FALSE;
}

static void f_SCRChime_Process(void)
{
	uint8 fl_CAN_msg_0x368_status_U8 = 0;
	uint8 fl_CAN_nvr_0x368_status_U8 = 0;
	uint8 fl_EMS_UreaWarn_signal_U8  = 0;
	
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_NR(&fl_CAN_msg_0x368_status_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_Tout(&fl_CAN_nvr_0x368_status_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaWarn(&fl_EMS_UreaWarn_signal_U8);
	
	if(((uint8)RTE_E_NEVER_RECEIVED == fl_CAN_msg_0x368_status_U8 ) ||\
	   ((uint8)RTE_E_TIMEOUT        == fl_CAN_nvr_0x368_status_U8)) 
	{
		f_SCRChime_initial();
		Rte_Call_rpCS_SCRChimeMdl_UpdateStatus_Operation( CHIME_REQID_SCR_2HZ_1 , FALSE);
	}
	else
	{
		if(l_UreaWarn_Last_State_U8 != fl_EMS_UreaWarn_signal_U8)
		{
			if (((uint8)TURN_IN_ON == fl_EMS_UreaWarn_signal_U8) ||\
				((uint8)TURN_IN_F_QUICK == fl_EMS_UreaWarn_signal_U8) ||\
				((uint8)TURN_IN_F_SLOW	== fl_EMS_UreaWarn_signal_U8))
			{
				l_SCRChime_State_U8_2HZ_1 = TRUE;
			}
			else
			{
				l_SCRChime_State_U8_2HZ_1 = FALSE;
			}
			Rte_Call_rpCS_SCRChimeMdl_UpdateStatus_Operation( CHIME_REQID_SCR_2HZ_1 , l_SCRChime_State_U8_2HZ_1 );
		}
		l_UreaWarn_Last_State_U8 = fl_EMS_UreaWarn_signal_U8;
	}
	
}

#if 0
	UINT8 	fl_CAN_msg_0x368_status_U8 = 0;
	UINT8	fl_CAN_nvr_0x368_status_U8 = 0;

  	UINT8   fl_UreaTxt_signal_U8        = 0;
	UINT8   fl_UreaQty_signal_U8        = 0;
	UINT8	fl_AddUreaWarning_signal_U8 = 0;
	UINT8   fl_UreaSysWarning_signal_U8 = 0;
	UINT8   fl_NoxWarning_signal_U8     = 0;

	boolean	SCRChime_UreaTxt_State_U8        = FALSE;
	boolean	SCRChime_AddUreaWarning_State_U8 = FALSE;
	boolean	SCRChime_UreaQty_State_U8        = FALSE;
	boolean	SCRChime_UreaSysWarning_State_U8 = FALSE;
	boolean	SCRChime_NoxWarning_State_U8     = FALSE;
	boolean IsEngineCfg = FALSE;
	
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_NR(&fl_CAN_msg_0x368_status_U8);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_Tout(&fl_CAN_nvr_0x368_status_U8);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Engine,&IsEngineCfg);

	if((fl_CAN_msg_0x368_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_nvr_0x368_status_U8 == RTE_E_TIMEOUT)) 
	{
		l_SCRChime_State_U8_2HZ_1 = FALSE;

		l_UreaTxt_Last_State_U8 = FALSE;
		l_UreaQty_Last_State_U8 = FALSE;
		l_AddUreaWarning_Last_State_U8 = FALSE;
		l_UreaSysWarning_Last_State_U8 = FALSE;
    	l_NoxWarning_Last_State_U8 = FALSE;
	}
	else
	{
		Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaTxt(&fl_UreaTxt_signal_U8);
		Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaQty(&fl_UreaQty_signal_U8);
		Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_AddUreaWarning(&fl_AddUreaWarning_signal_U8);
		Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaSysWarning(&fl_UreaSysWarning_signal_U8);
		Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_NoxWarning(&fl_NoxWarning_signal_U8);		
	}

	if(IsEngineCfg == FALSE) //Diesel
	{
		if(l_UreaTxt_Last_State_U8 != fl_UreaTxt_signal_U8)
		{
			if ((fl_UreaTxt_signal_U8 == TURN_IN_STATE_2) 
			|| (fl_UreaTxt_signal_U8 == TURN_IN_STATE_3) 
			|| (fl_UreaTxt_signal_U8 == TURN_IN_STATE_4) 
			|| (fl_UreaTxt_signal_U8 == TURN_IN_STATE_5))
			{
				SCRChime_UreaTxt_State_U8 = TRUE;
			}
			else
			{
				SCRChime_UreaTxt_State_U8 = FALSE;
			}
		}
		else
		{
			SCRChime_UreaTxt_State_U8 = FALSE;
		}

		if(l_UreaQty_Last_State_U8 != fl_UreaQty_signal_U8)
		{
			if ((fl_UreaQty_signal_U8 == TURN_IN_STATE_1)
			|| (fl_UreaQty_signal_U8 == TURN_IN_STATE_2) 
			|| (fl_UreaQty_signal_U8 == TURN_IN_STATE_3))
			{
				SCRChime_UreaQty_State_U8 = TRUE;
			}
			else
			{
				SCRChime_UreaQty_State_U8 = FALSE;
			}
		}
		else
		{
			SCRChime_UreaQty_State_U8 = FALSE;
		}

		if(l_AddUreaWarning_Last_State_U8 != fl_AddUreaWarning_signal_U8)
		{
			if ((fl_AddUreaWarning_signal_U8 == TURN_IN_STATE_1) 
			|| (fl_AddUreaWarning_signal_U8 == TURN_IN_STATE_2) 
			|| (fl_AddUreaWarning_signal_U8 == TURN_IN_STATE_3))
			{
				SCRChime_AddUreaWarning_State_U8 = TRUE;
			}
			else
			{
				SCRChime_AddUreaWarning_State_U8 = FALSE;
			}
		}
		else
		{
			SCRChime_AddUreaWarning_State_U8 = FALSE;
		}

		if(l_UreaSysWarning_Last_State_U8 != fl_UreaSysWarning_signal_U8)
		{
			if ((fl_UreaSysWarning_signal_U8 == TURN_IN_STATE_1)
			|| (fl_UreaSysWarning_signal_U8 == TURN_IN_STATE_2) 
			|| (fl_UreaSysWarning_signal_U8 == TURN_IN_STATE_3))
			{
				SCRChime_UreaSysWarning_State_U8 = TRUE;
			}
			else
			{
				SCRChime_UreaSysWarning_State_U8 = FALSE;
	  		}
		}
		else
		{
			SCRChime_UreaSysWarning_State_U8 = FALSE;
		}

		if (l_NoxWarning_Last_State_U8 != fl_NoxWarning_signal_U8)
		{
			if ((fl_NoxWarning_signal_U8 == TURN_IN_STATE_1) 
			|| (fl_NoxWarning_signal_U8 == TURN_IN_STATE_2) 
			|| (fl_NoxWarning_signal_U8 == TURN_IN_STATE_3))
			{
				SCRChime_NoxWarning_State_U8 = TRUE;
			}
			else
			{
				SCRChime_NoxWarning_State_U8 = FALSE;
			}
		}
		else
		{
			SCRChime_NoxWarning_State_U8 = FALSE;
		}

		if((SCRChime_UreaTxt_State_U8 == TRUE)
		|| (SCRChime_UreaQty_State_U8 == TRUE)
		|| (SCRChime_AddUreaWarning_State_U8 == TRUE)
		|| (SCRChime_UreaSysWarning_State_U8 == TRUE)
		|| (SCRChime_NoxWarning_State_U8 == TRUE))
		{
			l_SCRChime_State_U8_2HZ_1 = TRUE;
			Rte_Call_rpCS_SCRChimeMdl_UpdateStatus_Operation( CHIME_REQID_SCR_2HZ_1 , l_SCRChime_State_U8_2HZ_1 );
		}
		else
		{
			l_SCRChime_State_U8_2HZ_1 = FALSE;
		}
			
		l_UreaTxt_Last_State_U8 = fl_UreaTxt_signal_U8;
		l_UreaQty_Last_State_U8 = fl_UreaQty_signal_U8;
		l_AddUreaWarning_Last_State_U8 = fl_AddUreaWarning_signal_U8;
		l_UreaSysWarning_Last_State_U8 = fl_UreaSysWarning_signal_U8;
		l_NoxWarning_Last_State_U8 = fl_NoxWarning_signal_U8;					
	}
	else
	{
		/* no work */
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
