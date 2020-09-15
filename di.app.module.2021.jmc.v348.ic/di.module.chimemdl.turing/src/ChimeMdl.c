/*---------------------------------------------------------------------------------------------------------------------
**
** VISTEON CORPORATION CONFIDENTIAL
**
**
** [2015] Visteon Corporation
** All Rights Reserved.
**
** NOTICE: This is an unpublished work of authorship, which contains trade secrets.
** Visteon Corporation owns all rights to this work and intends to maintain it in confidence to preserve
** its trade secret status. Visteon Corporation reserves the right, under the copyright laws of the United States
** or those of any other country that may have jurisdiction, to protect this work as an unpublished work,
** in the event of an inadvertent or deliberate unauthorized publication. Visteon Corporation also reserves its rights
** under all copyright laws to protect this work as a published work, when appropriate.
** Those having access to this work may not copy it, use it, modify it, or disclose the information contained in it
** without the written authorization of Visteon Corporation.
**
**--------------------------------------------------------------------------------------------------------------------*/

#include "Rte_ChimeMdl.h"
#include "Rte_ChimeMdl_Type.h"
#include "CmpLib.h"
#include "ChimeLogical.h"



static UINT8 gs_u8Chime_QuietCount;
static uint8 fl_flag;

/*@brief  Instance of life-cycle state-machine*/

CMPLIB_INSTANCE(ChimeMdl)
/*=====================================================================================================================
**  CONSTANTS & TYPES
**====================================================================================================================*/
static uint8 OTAMODE = 0;
/*=====================================================================================================================
** I N T E R N A L   F U N C T I O N   P R O T O T Y P E S
**====================================================================================================================*/
#define OTAMODE_ON  3
/*=====================================================================================================================
** M E M O R Y   A L L O C A T I O N
**====================================================================================================================*/
static void CChimeMdl_OTAMode_CloseAllChime(void);

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional initialization state
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpInit( void )
{
    Chime_Mgr_InitHW();
    Chime_Mgr_InitSW();
    fl_flag = FALSE;
    gs_u8Chime_QuietCount = 0;
	OTAMODE = 0;
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional de-initialization state
**
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDeInit( void )
{
    Chime_Mgr_ExitHW();
    Chime_Mgr_ExitSW();
    fl_flag = FALSE;
	OTAMODE = 0;
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional activation state
**
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpActivation( void )
{
    Chime_Mgr_Notify_Restore();
    fl_flag = FALSE;
    //Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(eIOPinOutId_AMP_SHTD, TRUE);
    gs_u8Chime_QuietCount = 0;
	OTAMODE = 0;

    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional de-activation state
**
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDeActivation( void )
{
    Chime_Mgr_Notify_Shutdown();
    fl_flag = FALSE;
    //Rte_Call_rpCS_TIIoHwAb_DOut_SetPin(eIOPinOutId_AMP_SHTD, FALSE);
	OTAMODE = 0;

    if (gs_u8Chime_QuietCount >= 10)
    {
        return  E_OK;
    }
    else
    {
        gs_u8Chime_QuietCount++;
        return  E_NOT_OK;
    }
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  State for normal operations
**
** @return The return value is disregarded. Say E_OK
**---------------------------------------------------------------------------------------------------------------------*/
static uint8 fl_IgnQuickSwitchCnt = 30;
static Std_ReturnType CmpActive( void )
{
    uint8 ChimeID = 0;
	uint8 Ign_state = 0;
	uint8 fl_ChimeClose_u8 = 0; 
    DCM_IOC_Type DID_Value = {0};
	Dcm_SesCtrlType fl_diag_session = DEFAULT_SESSION;
    static Dcm_SesCtrlType flSession_pre = DEFAULT_SESSION;
	(void)Rte_Read_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(&DID_Value);
	Rte_Call_rpCS_GetCurrentSessionType_Operation(&fl_diag_session);
	
	CChimeMdl_OTAMode_CloseAllChime();
	
    if(fl_diag_session == EOL_SESSION)
    {
        if(fl_diag_session != flSession_pre)
		{
            fl_flag = FALSE;
            fl_IgnQuickSwitchCnt = 30;
			for(ChimeID = 0; ChimeID < CHIME_REQ_TOTAL_NUM; ChimeID++)
			{
				CChimeMdl_UpdateStatus_Operation(ChimeID,FALSE);
			}
		}
		else
		{
		}

        if((DID_Value.DID_IOC_Number == 0xFD05) && (DID_Value.DID_IOC_Parament == 0x03))
        {
            if(DID_Value.DID_IOC_State[0] == 0x00)
            {
                if(fl_flag == FALSE)
                {
                    if(fl_IgnQuickSwitchCnt > 0)
                    {
                        fl_IgnQuickSwitchCnt--;
                        OnTask10ms_Chime_Routine();
                        CChimeMdl_UpdateStatus_Operation(CHIME_REQID_TPMS_WARN,TRUE);
                    }
                    else
                    {
                        fl_flag = TRUE;
                        CChimeMdl_UpdateStatus_Operation(CHIME_REQID_TPMS_WARN,FALSE);
                    }
                }
            }
            else
            {
                fl_flag = FALSE;
                fl_IgnQuickSwitchCnt = 30;
                for(ChimeID = 0; ChimeID < CHIME_REQ_TOTAL_NUM; ChimeID++)
                {
                    CChimeMdl_UpdateStatus_Operation(ChimeID, FALSE);
                }
            }
        }
        else
        {
            fl_flag = FALSE;
            fl_IgnQuickSwitchCnt = 30;
        }
    }
    else
    {
        fl_flag = FALSE;
        fl_IgnQuickSwitchCnt = 10;			
		
		Rte_Read_rpIgnState_IGNState(&Ign_state);
		Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_ChimeClose_u8);
		if((fl_ChimeClose_u8 == TRUE) && (Ign_state == eIGN_RUN))
		{
            OnTask10ms_Chime_Routine();
			for(ChimeID = 3; ChimeID < eSnd_MaxNumOfChimes; ChimeID++)
			{
				CSndCdd_RequestToStop(ChimeID, (UINT8)CHIME_STOP_SU);
			}
		}
		else if ( OTAMODE ) //if OTAMODE is TRUE, turn off all chime.
		{
            OnTask10ms_Chime_Routine();
			for(ChimeID = 1; ChimeID < eSnd_MaxNumOfChimes; ChimeID++)
			{
				CSndCdd_RequestToStop(ChimeID, (UINT8)CHIME_STOP_SU);
			}
		}
		else
		{
			OnTask10ms_Chime_Routine();
		}
	}

    flSession_pre = fl_diag_session;

    CChimeMdl_Ignoff_Chime_State__();
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Diagnostic state
**
** @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
**         E_NOT_READY:    Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDiag( void )
{
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
**
** @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
**         E_NOT_READY:    Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}

static void CChimeMdl_OTAMode_CloseAllChime( void )
{
	uint8 fl_BCM_CarModeIndicator = 0;
	Rte_Read_rpSR_CANMSG_BCM_0x584_ComIn_BCM_CarModeIndicator(&fl_BCM_CarModeIndicator);

	if(OTAMODE_ON == fl_BCM_CarModeIndicator)
	{
		OTAMODE = 1;
	}
	else
	{
		OTAMODE = 0;
	}

	Rte_Write_pp_GetOTAModeStatus_Value_OTAMode(OTAMODE);
}

