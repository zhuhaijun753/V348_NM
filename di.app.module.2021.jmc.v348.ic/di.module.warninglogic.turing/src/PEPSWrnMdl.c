//---------------------------------------------------------------------------------------------------------------------
//
// VISTEON CORPORATION CONFIDENTIAL
// ________________________________
//
// [2015] Visteon Corporation
// All Rights Reserved.
//
// NOTICE: This is an unpublished work of authorship, which contains trade secrets.
// Visteon Corporation owns all rights to this work and intends to maintain it in confidence to preserve
// its trade secret status. Visteon Corporation reserves the right, under the copyright laws of the United States
// or those of any other country that may have jurisdiction, to protect this work as an unpublished work,
// in the event of an inadvertent or deliberate unauthorized publication. Visteon Corporation also reserves its rights
// under all copyright laws to protect this work as a published work, when appropriate.
// Those having access to this work may not copy it, use it, modify it, or disclose the information contained in it
// without the written authorization of Visteon Corporation.
//
//---------------------------------------------------------------------------------------------------------------------
#include "Rte_PEPSWrnMdl.h"
#include "CmpLib.h"
#include "peps_wrn_cfg.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(PEPSWrnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */


//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================


//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 PepsWarningStatus[PEPS_WARNING_TOTAL] = { 0 };

static eIgnState IGN_state = eIGN_OFF;
boolean bPepsIgOn2OffReady = TRUE; //this extern for hmi_ign_off_bg_screen.c

/*this is 1/2 function*/
static uint8 fl_NoKeyFoundWrnSts_Work_Last_U8 = 0;
static uint8 fl_KeyOutWrnSts_Work_Last_U8 = 0;
static uint8 fl_NoKeyFoundWrnSts_Action_Last_U8 = 0;
static uint8 fl_KeyOutWrnSts_Action_Last_U8 = 0;
static uint8 PEPS_NoKey_Status_Flag_U8 = TRUE;

/*this is 4/5 function*/
static uint8 fl_BrakeIndWrn_TmExt_Timer_IsStarted = FALSE;
static uint8 fl_BrakeIndWrn_Sts_U8 = TRUE;
static uint8 fl_ClutchIndWrn_TmExt_Timer_IsStarted = FALSE;
static uint8 fl_ClutchIndWrn_Sts_U8 = TRUE;

static uint8  l_WarnPeps_Flag = TRUE;
static uint8  l_Warn_ProveoutCounter_U8 = 0;


#define	WARN_FUNCTION_CALLED_PEPS	(100) //ms
#define WARN_DELAY_TIME_PEPS_3000MS	(2800  / WARN_FUNCTION_CALLED_PEPS)

static uint8 PepsWarningId[PEPS_WARNING_TOTAL] =
{
    WARN_PEPS_ESCL_FAILSTATUS_ID,
	WARN_PEPS_ESCL_FAILSTATUS_ACTION_ID,
    WARN_PEPS_IMMO_MATCH_STATUS_ID,
    WARN_PEPS_NO_KEY_FOUND_WARNING_ID,
    WARN_PEPS_KEY_OUT_WARNING_ID,
    WARN_PEPS_KEY_BATTERY_WARNING_ID,
    WARN_PEPS_BRAKE_INDICATION_WARNING_ID,
    WARN_PEPS_BRAKE_INDICATION_WARNING_ACTION_ID,
    WARN_PEPS_NOT_N_WARNING_ID,
    WARN_PEPS_SSB_FAILSTATUS_ID,
    WARN_PEPS_SSB_FAILSTATUS_ACTION_ID,
    WARN_PEPS_CLUTCH_INDICATION_WARNING_ID,
    WARN_PEPS_CLUTCH_INDICATION_WARNING_ACTION_ID
};

static void fun_ESCL_FailSts_process(void);
static void fun_ImmoMatchSts_process(void);
static void fun_NoKeyFoundWrn_process(void);
static void fun_KeyBatWrn_process(void);
static void fun_BrakeIndWrn_process(void);
static void fun_NotN_process(void);
static void fun_SSB_FailSts_process(void);
static void fun_ClutchIndWrn_process(void);
static void fun_Check_Wrn_Status_Process(void);
static uint8 fun_0x325_Message_Input(void);
static void fun_0x325_Message_Init(void);
static boolean PEPS_PowerOn_3000MS_Self_Test(void);
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
    fun_0x325_Message_Init();
    IGN_state = (eIgnState)eIGN_OFF;

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit(void)
{
    fun_0x325_Message_Init();
    IGN_state = (eIgnState)eIGN_OFF;

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation(void)
{
    fun_0x325_Message_Init();
    IGN_state = (eIgnState)eIGN_OFF;

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation(void)
{
    fun_0x325_Message_Init();
    IGN_state = (eIgnState)eIGN_OFF;

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
/****************************************************************************
Function Name       : CmpActive

Description         : This function process the warning for Security Authentication Failed.

Invocation          : called in cmpActive.

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/

static Std_ReturnType CmpActive(void)
{
	uint8 fl_animationFlag = TRUE;
	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
    Rte_Read_rpIgnState_IGNState(&IGN_state);
	
	if (FALSE == fl_animationFlag)
	{
		PEPS_PowerOn_3000MS_Self_Test();

		if ((boolean)FALSE == fun_0x325_Message_Input()) /*TRUE:Message timeout er never reveice message | FALSE:Message valid*/
		{
			if(FALSE == l_WarnPeps_Flag)
			{
				fun_NoKeyFoundWrn_process(); //1
				fun_NotN_process(); 		 //3
				fun_BrakeIndWrn_process();	 //4
				fun_ClutchIndWrn_process();  //5
				fun_KeyBatWrn_process();	 //6
				fun_ESCL_FailSts_process();  //7
				fun_SSB_FailSts_process();	 //8
				fun_ImmoMatchSts_process();  //9
			}
		
			if ((eIgnState)eIGN_RUN == IGN_state)
			{
				bPepsIgOn2OffReady = (boolean)FALSE;
			}
			else
			{
				bPepsIgOn2OffReady = (boolean)TRUE;
			}
		}
		else
		{
			bPepsIgOn2OffReady = (boolean)TRUE;
			fun_0x325_Message_Init(); //this is message timeout or never reveice message,then clear all
		}	
	}
	else
	{
		fun_0x325_Message_Init();
		
		l_WarnPeps_Flag = TRUE;
		l_Warn_ProveoutCounter_U8 = 0;
	}


    for (uint8 i = 0; i < PEPS_WARNING_TOTAL; i++)
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(PepsWarningId[i], PepsWarningStatus[i]);
    }

    return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief  Diagnostic state
///
/// @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag(void)
{
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn(void)
{
    return E_OK;
}

static uint8 fun_0x325_Message_Input(void)
{
    uint8 fl_Msg325_NR_U8                 = (uint8)0;
    uint8 fl_Msg325_Tout_U8               = (uint8)0;

    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_NR(&fl_Msg325_NR_U8);
    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_Tout(&fl_Msg325_Tout_U8);

    if (((uint8)RTE_E_TIMEOUT == fl_Msg325_Tout_U8) || ((uint8)RTE_E_NEVER_RECEIVED ==  fl_Msg325_NR_U8))
    {
        return TRUE;  /*TRUE:Message Invalid | FALSE:Message valid*/
    }
    else
    {
        return FALSE;
    }
}

static void fun_0x325_Message_Init(void)
{
    for (int i = 0; i < PEPS_WARNING_TOTAL; i++)
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(PepsWarningId[i], UI_WARNING_STATUS_OFF);
        PepsWarningStatus[i] = UI_WARNING_STATUS_OFF;
    }

	/*this is for 1 and 2 function */
	fl_NoKeyFoundWrnSts_Work_Last_U8 = 0;
	fl_KeyOutWrnSts_Work_Last_U8 = 0;
	fl_NoKeyFoundWrnSts_Action_Last_U8 = 0;
	fl_KeyOutWrnSts_Action_Last_U8 = 0;
	PEPS_NoKey_Status_Flag_U8 = TRUE;

	/* this is for 4 function */
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimer_PEPSWrnMdl_1);
	fl_BrakeIndWrn_TmExt_Timer_IsStarted = FALSE;
	fl_BrakeIndWrn_Sts_U8 = TRUE;

	/*this is for 5 function */
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimer_PEPSWrnMdl_2);
	fl_ClutchIndWrn_TmExt_Timer_IsStarted = FALSE;
	fl_ClutchIndWrn_Sts_U8 = TRUE;
}

static void fun_ESCL_FailSts_process(void)
{
	/* this is 7 function  */
    uint8 fl_ESCL_FailSts_U8              = (uint8)0;

    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ESCL_FailStatus(&fl_ESCL_FailSts_U8);

	if(eIGN_RUN == IGN_state)
	{
        if (cPEPS_WARNING_ACTIVE == fl_ESCL_FailSts_U8)
        {
            PepsWarningStatus[WARN_PEPS_ESCL_FAILSTATUS] = UI_WARNING_STATUS_ON;
        }
        else
        {
            PepsWarningStatus[WARN_PEPS_ESCL_FAILSTATUS] = UI_WARNING_STATUS_OFF;
        }

		PepsWarningStatus[WARN_PEPS_ESCL_FAILSTATUS_ACTION] = UI_WARNING_STATUS_OFF;
	}
	else
	{
        if (cPEPS_WARNING_ACTIVE == fl_ESCL_FailSts_U8)
        {
            PepsWarningStatus[WARN_PEPS_ESCL_FAILSTATUS_ACTION] = UI_WARNING_STATUS_ON;
        }
        else
        {
            PepsWarningStatus[WARN_PEPS_ESCL_FAILSTATUS_ACTION] = UI_WARNING_STATUS_OFF;
        }

		PepsWarningStatus[WARN_PEPS_ESCL_FAILSTATUS] = UI_WARNING_STATUS_OFF;
	}
	
}

static void fun_ImmoMatchSts_process(void)
{
	/* this is 9 function  */
    uint8 fl_IMMO_MatchSts_U8             = (uint8)0;

    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus(&fl_IMMO_MatchSts_U8);
    /*IGN ON*/
    if ((cPEPS_IMMO_MATCH_WARNING_ACTIVE == fl_IMMO_MatchSts_U8) && ((eIgnState)eIGN_RUN == IGN_state))
    {
        PepsWarningStatus[WARN_PEPS_IMMO_MATCH_STATUS] = UI_WARNING_STATUS_ON;
    }
    else
    {
        PepsWarningStatus[WARN_PEPS_IMMO_MATCH_STATUS] = UI_WARNING_STATUS_OFF;
    }
}

static void fun_NoKeyFoundWrn_process(void)
{	
	/* this is 1 and 2 function  */
    uint8 fl_NoKeyFoundWrnSts_U8          = (uint8)0;
    uint8 fl_KeyOutWrnSts_U8              = (uint8)0;

    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NoKeyFoundWarning(&fl_NoKeyFoundWrnSts_U8);
    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyOutWarning1(&fl_KeyOutWrnSts_U8);

	switch(IGN_state)
	{

		case eIGN_RUN:
			if((cPEPS_WARNING_ACTIVE == fl_NoKeyFoundWrnSts_U8) && (fl_NoKeyFoundWrnSts_Work_Last_U8 != fl_NoKeyFoundWrnSts_U8)
			|| (cPEPS_WARNING_ACTIVE == fl_KeyOutWrnSts_U8) && (fl_KeyOutWrnSts_Work_Last_U8 != fl_KeyOutWrnSts_U8))
			{
				uint8 fl_NoKeyFoundWrn_Working_U8 = (uint8)UI_WARNING_STATE_NONE;
				fl_NoKeyFoundWrn_Working_U8 = CWarningCtrl_APP_get_notified_warning_status(WARN_PEPS_NO_KEY_FOUND_WARNING_ID);
				
				if((uint8)UI_WARNING_STATE_NONE == fl_NoKeyFoundWrn_Working_U8)
				{
					PepsWarningStatus[WARN_PEPS_NO_KEY_FOUND_WARNING] = UI_WARNING_STATUS_ON;
				}
				else if (((uint8)UI_WARNING_STATE_SHOW == fl_NoKeyFoundWrn_Working_U8) || ((uint8)UI_WARNING_STATE_ACKNOWLEDED == fl_NoKeyFoundWrn_Working_U8))
				{
					PepsWarningStatus[WARN_PEPS_NO_KEY_FOUND_WARNING] = UI_WARNING_STATUS_RETRIG;
				}
				else if ((uint8)UI_WARNING_STATE_TIMEOUT == fl_NoKeyFoundWrn_Working_U8)
				{
					PepsWarningStatus[WARN_PEPS_NO_KEY_FOUND_WARNING] = UI_WARNING_CONDITION_FORCED;
				}
				else
				{
					//
				}
			
				PEPS_NoKey_Status_Flag_U8 = TRUE;
				
			}
			else if((cPEPS_WARNING_ACTIVE != fl_NoKeyFoundWrnSts_U8) && (cPEPS_WARNING_ACTIVE != fl_KeyOutWrnSts_U8))
			{
				PepsWarningStatus[WARN_PEPS_NO_KEY_FOUND_WARNING] = UI_WARNING_STATUS_OFF;
				PEPS_NoKey_Status_Flag_U8 = TRUE;
			}
			else
			{
				if(TRUE == PEPS_NoKey_Status_Flag_U8)
				{
					PepsWarningStatus[WARN_PEPS_NO_KEY_FOUND_WARNING] = UI_WARNING_STATUS_ON;
			
					/* this is working now	but value 1 from to 0 right away */
					if((cPEPS_WARNING_VALUE_0 == fl_NoKeyFoundWrnSts_U8) && (fl_NoKeyFoundWrnSts_Work_Last_U8 == cPEPS_WARNING_ACTIVE))
					{
						PEPS_NoKey_Status_Flag_U8 = FALSE;
					}
					
					if((cPEPS_WARNING_VALUE_0 == fl_KeyOutWrnSts_U8) && (fl_KeyOutWrnSts_Work_Last_U8 == cPEPS_WARNING_ACTIVE))
					{
						PEPS_NoKey_Status_Flag_U8 = FALSE;
					}
				}
				else
				{
					PepsWarningStatus[WARN_PEPS_NO_KEY_FOUND_WARNING] = UI_WARNING_STATUS_OFF;
				}
			
			}
			
			fl_NoKeyFoundWrnSts_Work_Last_U8 = fl_NoKeyFoundWrnSts_U8;
			fl_KeyOutWrnSts_Work_Last_U8 = fl_KeyOutWrnSts_U8;

			/*clear IGN OFF Status */
			fl_NoKeyFoundWrnSts_Action_Last_U8 = 0;
			fl_KeyOutWrnSts_Action_Last_U8 = 0;
			break;
		case eIGN_OFF:
			if((cPEPS_WARNING_ACTIVE == fl_NoKeyFoundWrnSts_U8) && (fl_NoKeyFoundWrnSts_Action_Last_U8 != fl_NoKeyFoundWrnSts_U8)
			 || (cPEPS_WARNING_ACTIVE == fl_KeyOutWrnSts_U8) && (fl_KeyOutWrnSts_Action_Last_U8 != fl_KeyOutWrnSts_U8))
			 {
				 uint8 fl_NoKeyFoundWrn_U8 = (uint8)UI_WARNING_STATE_NONE;
				 fl_NoKeyFoundWrn_U8 = CWarningCtrl_APP_get_notified_warning_status(WARN_PEPS_NO_KEY_FOUND_WARNING_ID);
				 
				 if((uint8)UI_WARNING_STATE_NONE == fl_NoKeyFoundWrn_U8)
				 {
					 PepsWarningStatus[WARN_PEPS_NO_KEY_FOUND_WARNING] = UI_WARNING_STATUS_ON;
				 }
				 else if (((uint8)UI_WARNING_STATE_SHOW == fl_NoKeyFoundWrn_U8) || ((uint8)UI_WARNING_STATE_ACKNOWLEDED == fl_NoKeyFoundWrn_U8))
				 {
					 PepsWarningStatus[WARN_PEPS_NO_KEY_FOUND_WARNING] = UI_WARNING_STATUS_RETRIG;
				 }
				 else if ((uint8)UI_WARNING_STATE_TIMEOUT == fl_NoKeyFoundWrn_U8)
				 {
					 PepsWarningStatus[WARN_PEPS_NO_KEY_FOUND_WARNING] = UI_WARNING_CONDITION_FORCED;
				 }
				 else
				 {
					 //
				 }
			
			 }
			 else if((cPEPS_WARNING_ACTIVE != fl_NoKeyFoundWrnSts_U8) && (cPEPS_WARNING_ACTIVE != fl_KeyOutWrnSts_U8))
			 {
				 PepsWarningStatus[WARN_PEPS_NO_KEY_FOUND_WARNING] = UI_WARNING_STATUS_OFF;
			 }
			 else
			 {
				 PepsWarningStatus[WARN_PEPS_NO_KEY_FOUND_WARNING] = UI_WARNING_STATUS_ON;
			 }
			
			 fl_NoKeyFoundWrnSts_Action_Last_U8 = fl_NoKeyFoundWrnSts_U8;
			 fl_KeyOutWrnSts_Action_Last_U8 = fl_KeyOutWrnSts_U8;
			 break;
		default:
			break;
			
	}
}

static void fun_KeyBatWrn_process(void)
{
	/* this is 6 function  */
    uint8 fl_KeyBatWrnSts_U8              = (uint8)0;
    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyBatteryWarning(&fl_KeyBatWrnSts_U8);

    /*IGN ON*/
    if ((cPEPS_WARNING_ACTIVE == fl_KeyBatWrnSts_U8) && ((eIgnState)eIGN_RUN == IGN_state))
    {
        PepsWarningStatus[WARN_PEPS_KEY_BATTERY_WARNING] = UI_WARNING_STATUS_ON;
    }
    else
    {
        PepsWarningStatus[WARN_PEPS_KEY_BATTERY_WARNING] = UI_WARNING_STATUS_OFF;
    }
}

static void fun_NotN_process(void)
{
	/* this is 3 function  */
    uint8 fl_NotNWrnSts_U8                = (uint8)0;
    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotNWarning(&fl_NotNWrnSts_U8);

    /*IGN ON*/
    if ((cPEPS_WARNING_ACTIVE == fl_NotNWrnSts_U8) && ((eIgnState)eIGN_RUN == IGN_state))
    {
        PepsWarningStatus[WARN_PEPS_NOT_N_WARNING] = UI_WARNING_STATUS_ON;
    }
    else
    {
        PepsWarningStatus[WARN_PEPS_NOT_N_WARNING] = UI_WARNING_STATUS_OFF;
    }
}

static void fun_SSB_FailSts_process(void)
{
	/* this is 8 function  */
    uint8 fl_SSB_FailSts_U8               = (uint8)0;
    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_SSB_FailStatus(&fl_SSB_FailSts_U8);

	if(eIGN_RUN == IGN_state)
	{
		if (cPEPS_WARNING_ACTIVE == fl_SSB_FailSts_U8)
		{
			PepsWarningStatus[WARN_PEPS_SSB_FAILSTATUS] = UI_WARNING_STATUS_ON;
		}
		else
		{
			PepsWarningStatus[WARN_PEPS_SSB_FAILSTATUS] = UI_WARNING_STATUS_OFF;
		}

		PepsWarningStatus[WARN_PEPS_SSB_FAILSTATUS_ACTION] = UI_WARNING_STATUS_OFF;
	}
	else
	{
		if (cPEPS_WARNING_ACTIVE == fl_SSB_FailSts_U8)
		{
			PepsWarningStatus[WARN_PEPS_SSB_FAILSTATUS_ACTION] = UI_WARNING_STATUS_ON;
		}
		else
		{
			PepsWarningStatus[WARN_PEPS_SSB_FAILSTATUS_ACTION] = UI_WARNING_STATUS_OFF;
		}

		PepsWarningStatus[WARN_PEPS_SSB_FAILSTATUS] = UI_WARNING_STATUS_OFF;
	}

}

static void fun_BrakeIndWrn_process(void)
{
	/* this is 4 function  */
    uint8   fl_BrakeIndicationWrnSts_U8   = (uint8)0;
    uint8   fl_BrakeSts_U8                 = (uint8)UI_WARNING_STATE_NONE;
    boolean fl_BrakeIndWrn_IsElapsed      = (boolean)FALSE;
    boolean fl_BrakeIndWrn_IsStarted      = (boolean)FALSE;

	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_BrakeIndicationWarning(&fl_BrakeIndicationWrnSts_U8);

	switch(IGN_state)
	{
		case eIGN_RUN:
			if(cPEPS_WARNING_ACTIVE == fl_BrakeIndicationWrnSts_U8)
			{
				if(TRUE == fl_BrakeIndWrn_Sts_U8) //This is for  first IGN ON or message time out 
				{
					PepsWarningStatus[WARN_PEPS_BRAKE_INDICATION_WARNING] = UI_WARNING_STATUS_ON;	
				}
				
				Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimer_PEPSWrnMdl_1, &fl_BrakeIndWrn_IsStarted);
				Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimer_PEPSWrnMdl_1, cTIMEOUT_10S, &fl_BrakeIndWrn_IsElapsed);
			
				if ((boolean)FALSE == fl_BrakeIndWrn_IsStarted)
				{
					Rte_Call_rpTimer_TmExt_Timer_Start(eTimer_PEPSWrnMdl_1);
				}
				else
				{
					fl_BrakeIndWrn_TmExt_Timer_IsStarted = TRUE;
					
					if ((boolean)FALSE != fl_BrakeIndWrn_IsElapsed)
					{
						fl_BrakeIndWrn_Sts_U8 = TRUE; 
						
						Rte_Call_rpTimer_TmExt_Timer_Stop(eTimer_PEPSWrnMdl_1);
						Rte_Call_rpTimer_TmExt_Timer_Start(eTimer_PEPSWrnMdl_1);
					
						
						fl_BrakeSts_U8 = CWarningCtrl_APP_get_notified_warning_status(WARN_PEPS_BRAKE_INDICATION_WARNING_ID);
						if ((uint8)UI_WARNING_STATE_TIMEOUT == fl_BrakeSts_U8)
						{
							PepsWarningStatus[WARN_PEPS_BRAKE_INDICATION_WARNING] = UI_WARNING_STATUS_OFF;
						}
					}
					else
					{
			
					}
				}
			}
			else
			{	
				if(TRUE == fl_BrakeIndWrn_TmExt_Timer_IsStarted)
				{
					Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimer_PEPSWrnMdl_1, cTIMEOUT_10S, &fl_BrakeIndWrn_IsElapsed);

					if(TRUE == fl_BrakeIndWrn_IsElapsed)
					{
						fl_BrakeIndWrn_Sts_U8 = TRUE; 
					}
					else
					{
						fl_BrakeIndWrn_Sts_U8 = FALSE; 
					}
				}
				else
				{
					Rte_Call_rpTimer_TmExt_Timer_Stop(eTimer_PEPSWrnMdl_1);
				}
				
				PepsWarningStatus[WARN_PEPS_BRAKE_INDICATION_WARNING] = UI_WARNING_STATUS_OFF;	
			}			
			break;
		case eIGN_OFF:
			if (cPEPS_WARNING_ACTIVE == fl_BrakeIndicationWrnSts_U8)
			{
				PepsWarningStatus[WARN_PEPS_BRAKE_INDICATION_WARNING_ACTION] = UI_WARNING_STATUS_ON;
			}
			else
			{
				PepsWarningStatus[WARN_PEPS_BRAKE_INDICATION_WARNING_ACTION] = UI_WARNING_STATUS_OFF;
			}
			
			Rte_Call_rpTimer_TmExt_Timer_Stop(eTimer_PEPSWrnMdl_1);			
			break;
		default:
			break;
	}
}

static void fun_ClutchIndWrn_process(void)
{
	/* this is 5 function  */
    uint8   fl_ClutchIndWrnSts_U8         = (uint8)0;
    uint8   fl_ClutchSts_U8               = (uint8)UI_WARNING_STATE_NONE;
    boolean fl_ClutchIndWrn_IsElapsed     = (boolean)FALSE;
    boolean fl_ClutchIndWrn_IsStarted     = (boolean)FALSE;
	
    Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ClutchIndicationWarning(&fl_ClutchIndWrnSts_U8);
	switch(IGN_state)
	{
		case eIGN_RUN:
			if (cPEPS_WARNING_ACTIVE == fl_ClutchIndWrnSts_U8)
			{
				if(TRUE == fl_ClutchIndWrn_Sts_U8)//This is for  first IGN ON or message time out 
				{
					PepsWarningStatus[WARN_PEPS_CLUTCH_INDICATION_WARNING] = UI_WARNING_STATUS_ON;
				}
				
				Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimer_PEPSWrnMdl_2, &fl_ClutchIndWrn_IsStarted);
				Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimer_PEPSWrnMdl_2, cTIMEOUT_10S, &fl_ClutchIndWrn_IsElapsed);
				
				if ((boolean)FALSE == fl_ClutchIndWrn_IsStarted)
				{
					Rte_Call_rpTimer_TmExt_Timer_Start(eTimer_PEPSWrnMdl_2);
				}
				else
				{
					fl_ClutchIndWrn_TmExt_Timer_IsStarted = TRUE;
					
					if ((boolean)FALSE != fl_ClutchIndWrn_IsElapsed)
					{
						fl_ClutchIndWrn_Sts_U8 = TRUE;
						
						Rte_Call_rpTimer_TmExt_Timer_Stop(eTimer_PEPSWrnMdl_2);
						Rte_Call_rpTimer_TmExt_Timer_Start(eTimer_PEPSWrnMdl_2);
						
						fl_ClutchSts_U8 = CWarningCtrl_APP_get_notified_warning_status(WARN_PEPS_CLUTCH_INDICATION_WARNING_ID);
						if ((uint8)UI_WARNING_STATE_TIMEOUT == fl_ClutchSts_U8)
						{							
							PepsWarningStatus[WARN_PEPS_CLUTCH_INDICATION_WARNING] = UI_WARNING_STATUS_OFF;
						}
					}
					else
					{
			
					}
				}
			}
			else
			{
				if(TRUE == fl_ClutchIndWrn_TmExt_Timer_IsStarted)
				{
					Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimer_PEPSWrnMdl_2, cTIMEOUT_10S, &fl_ClutchIndWrn_IsElapsed);
			
					if(TRUE == fl_ClutchIndWrn_IsElapsed)
					{
						fl_ClutchIndWrn_Sts_U8 = TRUE;
					}
					else
					{
						fl_ClutchIndWrn_Sts_U8 = FALSE;
					}
				}
				else
				{
					Rte_Call_rpTimer_TmExt_Timer_Stop(eTimer_PEPSWrnMdl_2);
				}
				
				PepsWarningStatus[WARN_PEPS_CLUTCH_INDICATION_WARNING] = UI_WARNING_STATUS_OFF;
			}				
			break;
		case eIGN_OFF:
			if (cPEPS_WARNING_ACTIVE == fl_ClutchIndWrnSts_U8)
			{
				PepsWarningStatus[WARN_PEPS_CLUTCH_INDICATION_WARNING_ACTION] = UI_WARNING_STATUS_ON;
			}
			else
			{
				PepsWarningStatus[WARN_PEPS_CLUTCH_INDICATION_WARNING_ACTION] = UI_WARNING_STATUS_OFF;
			}
			
			Rte_Call_rpTimer_TmExt_Timer_Stop(eTimer_PEPSWrnMdl_2);
			break;
		default:
			break;
	}

}

static boolean PEPS_PowerOn_3000MS_Self_Test(void)
{
	 if(l_Warn_ProveoutCounter_U8 >= WARN_DELAY_TIME_PEPS_3000MS)
    {
        l_WarnPeps_Flag = FALSE;
    }
    else
    {
        l_WarnPeps_Flag = TRUE;
	    l_Warn_ProveoutCounter_U8++;
    }

    return l_WarnPeps_Flag;
}

