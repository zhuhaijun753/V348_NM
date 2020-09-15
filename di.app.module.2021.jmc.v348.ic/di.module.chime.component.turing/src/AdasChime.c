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

File Name        :  AdasChime.c
Module Short Name:  AdasChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define AdasChime_C

#include "Rte_AdasChime.h"
#include "CmpLib.h"
#include "ChimeLogical.h"


CMPLIB_INSTANCE(AdasChime)

static uint8 fl_IC_BSDSwitchSts_U8  = 0;
static uint8 l_ddd_Warning_State = FALSE;

typedef struct {
	//GW_MRR_0x245(LDW & TSR)
	uint8 IPM_LaneAssit_WarningAudio;
	uint8 IPM_LaneAssit_HandsonReq;

	// uint8 IPM_SLASpdlimit;
	// uint8 IPM_SLASpdlimitUnits;

	//DDD_245
	uint8 IPM_DDD_WarningLevel;
	uint8 IPM_DDD_IndexState;

	//GW_MRR_0x246 ACC
	uint8 AccMode;
	uint8 MRR_TakeOverReq;

	//GW_MRR_0x246 PEBS && FCW
	uint8 PCW_preWarning;
	uint8 PCW_latentWarning;
	uint8 FCW_preWarning;
	uint8 FCW_latentWarning;
	
	uint8 pcw_preWarnigFlg;
	uint8 pcw_latentWarningFlg;
	uint8 fcw_preWarnigFlg;
	uint8 fcw_latentWarningFlg;


	//GW_SOD_0x275 BSD
	uint8 SOD_CTA_warningReqLeft;
	uint8 SOD_CTA_warningReqRight;

	uint8 SOD_LCA_warningReqLeft;
	uint8 SOD_LCA_warningReqRight;

	uint8 SOD_DOA_warningReqRightFront;
	uint8 SOD_DOA_warningReqRightRear;
	uint8 SOD_DOA_warningReqLeftFront;
	uint8 SOD_DOA_warningReqLeftRear;

	/* GW_MP5_Nav_533 */
	uint8 Nav_Sts;

	/* MP5_0x382 */
	uint8 MP5_BSDSwitchSts;
} ST_ADAS_CHIME_MSG_INFO;

typedef struct {
	uint8 u8Flag;
	uint8 u8Count;
} ST_ADAS_CHIME_CTRL_INFO;

#define ADAS_ACC_OFF 		(0)
#define ADAS_ACC_FAILURE 	(7)

#define ADAS_FCW_VALUE_0 	(0)
#define ADAS_FCW_VALUE_1 	(1)
#define ADAS_FCW_VALUE_7 	(7)

#define ADAS_BSD_LEVEL_1	(1)
#define ADAS_BSD_LEVEL_2	(2)

#define ADAS_DDD_VALUE_0 	(0)
#define ADAS_DDD_VALUE_1 	(1)
#define ADAS_DDD_VALUE_3 	(3)

typedef enum {
	EM_CHIME_CTRL_FLG_FCW = 0,
	EM_CHIME_CTRL_FLG_TAKE_OVER,
	EM_CHIME_CTRL_FLG_HANDS_ON,
	EM_CHIME_CTRL_FLG_LDW,
	EM_CHIME_CTRL_FLG_TSR,
	EM_CHIME_CTRL_FLG_BSD,
	EM_CHIME_CTRL_FLG_CTA,
	EM_CHIME_CTRL_FLG_DOA,
	EM_CHIME_CTRL_FLG_DDD,
	EM_CHIME_CTRL_FLG_MAX
} EM_CHIME_CTRL_FLG;
	
#define	FUNCTION_CALLED_PERIOD	        (100) //ms
#define ADAS_CHIME_DELAY_TIME_200MS		(( 200  + FUNCTION_CALLED_PERIOD) / FUNCTION_CALLED_PERIOD)
#define ADAS_CHIME_DELAY_TIME_1000MS	(( 900  + FUNCTION_CALLED_PERIOD) / FUNCTION_CALLED_PERIOD)
#define ADAS_CHIME_DELAY_TIME_2000MS	((2000  + FUNCTION_CALLED_PERIOD) / FUNCTION_CALLED_PERIOD)
#define ADAS_CHIME_DELAY_TIME_3000MS	((3000  + FUNCTION_CALLED_PERIOD) / FUNCTION_CALLED_PERIOD)

#define ADAS_CHIME_1 (1)
#define ADAS_CHIME_2 (2)

static uint8 fl_IgnQuickSwitchCnt = ADAS_CHIME_DELAY_TIME_3000MS;
//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static ST_ADAS_CHIME_MSG_INFO stAdasChimeMsgInfo;

static ST_ADAS_CHIME_MSG_INFO stAdasChimeMsgInfoBk;

static ST_ADAS_CHIME_CTRL_INFO stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_MAX];

/*****************************************************************************
*                   Functions                                                *
******************************************************************************/
static void f_AdasChime_init(void);
static void f_AdasChime_update(void);

static void f_ChimeInit(void);
static void f_Chime_246_Acc_Pro(void); 
static void f_Chime_246_Pebs_Fcw_Pro(void); 
static void f_Chime_245_HandsOnReq_Pro(void); 
static void f_Chime_245_Ldw_Pro(void); 
static void f_Chime_245_Ddd_Pro(void); 
static void f_Chime_275_Cta_Pro(void); 
static void f_Chime_275_Doa_Pro(void); 
static void f_Chime_382_Bsd_LCA_Pro(void); 

static void f_GW_MRR_0x246_Chime_Acc_init(void);
static void f_GW_MRR_0x246_Chime_Acc_process(void);

static void f_GW_MRR_0x246_Chime_Pebs_Fcw_init(void);
static void f_GW_MRR_0x246_Chime_Pebs_Fcw_process(void);


static void f_GW_IPM_0x245_Chime_LaneAssit_init(void);
static void f_GW_IPM_0x245_Chime_LaneAssit_process(void);

static void f_GW_IPM_0x245_Chime_Ddd_Warning_init(void);
static void f_GW_IPM_0x245_Chime_Ddd_Warning_process(void);

static void f_GW_SOD_0x275_Chime_CTA_init(void);
static void f_GW_SOD_0x275_Chime_CTA_process(void);


static void f_GW_MP5_0x382_Chime_BSDSound_init(void);
static void f_GW_MP5_0x382_Chime_BSDSound_process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_AdasChime_init();
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
	f_AdasChime_init();
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
	f_AdasChime_init();
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
	f_AdasChime_init();
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
	EBatteryState fl_Battery_state = eBatteryState_Normal;
	boolean fl_animationFlag = TRUE;

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);

	if ((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage))
	{
		f_AdasChime_init();
	}
	else
	{
		switch (fl_IGN_state)
		{
			case eIGN_RUN:
				if (FALSE == fl_animationFlag)
				{
					if (fl_IgnQuickSwitchCnt > 0)
					{
						fl_IgnQuickSwitchCnt--;
					}
					
					if(0 == fl_IgnQuickSwitchCnt)
					{
						f_GW_MRR_0x246_Chime_Acc_process(); 	   //14-Acc 
						f_GW_MRR_0x246_Chime_Pebs_Fcw_process();   //15-PEBS
						f_GW_IPM_0x245_Chime_LaneAssit_process();  //16-LaneAssit
						f_GW_IPM_0x245_Chime_Ddd_Warning_process();//22-DDD
						f_GW_SOD_0x275_Chime_CTA_process(); 	   //17-CTA and BSD(BLIS)
						f_GW_MP5_0x382_Chime_BSDSound_process();   //17-CTA and BSD (MP5)
					}
					else
					{
						f_AdasChime_init();

					}
				}
				break;

			case eIGN_OFF:
				f_AdasChime_init();
				fl_IgnQuickSwitchCnt = ADAS_CHIME_DELAY_TIME_3000MS;
			default:
				//
				break;
		}
	}

	f_AdasChime_update();
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

static void f_AdasChime_init(void)
{
	f_GW_MRR_0x246_Chime_Acc_init();
	f_GW_MRR_0x246_Chime_Pebs_Fcw_init();
	f_GW_IPM_0x245_Chime_LaneAssit_init();
	f_GW_IPM_0x245_Chime_Ddd_Warning_init();
	f_GW_SOD_0x275_Chime_CTA_init();
	f_GW_MP5_0x382_Chime_BSDSound_init();
	

	f_ChimeInit();
}

static void f_AdasChime_update(void)
{
	uint8 i = 0;
	uint8 fl_IPM_LaneAssit_WarningAudio_U8 = 0;
	uint8 fl_Nav_Sts_U8 = 0;
	uint8 fl_IPM_LaneAssit_HandsonReq_U8 = 0;
	uint8 fl_MRR_TakeOverReq_U8 = 0;
	uint8 fl_MRR_PCW_STATE_U8 = 0;
	
	uint8 IsEngineCfg_Adas_Chime = 0;
	//Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ADAS,&IsEngineCfg_Adas_Chime);

	switch(IsEngineCfg_Adas_Chime)
	{
		case ADAS_CHIME_1:
			f_Chime_245_HandsOnReq_Pro();
			f_Chime_245_Ldw_Pro();
			f_Chime_275_Cta_Pro();
			f_Chime_275_Doa_Pro();
			f_Chime_382_Bsd_LCA_Pro();
			f_Chime_245_Ddd_Pro();
			f_Chime_246_Pebs_Fcw_Pro();
		break;
		case ADAS_CHIME_2:
			f_Chime_245_HandsOnReq_Pro();
			f_Chime_245_Ldw_Pro();
			f_Chime_275_Cta_Pro();
			f_Chime_275_Doa_Pro();
			f_Chime_382_Bsd_LCA_Pro();
			f_Chime_245_Ddd_Pro();
			f_Chime_246_Pebs_Fcw_Pro();
			f_Chime_246_Acc_Pro();
		break;
		default:
			//
		break;
	}

	for (i = EM_CHIME_CTRL_FLG_FCW; i < EM_CHIME_CTRL_FLG_MAX; i++)
	{
		if (stAdasChimeCtrlInfo[i].u8Count > 0)
		{
			stAdasChimeCtrlInfo[i].u8Count--;
		}
	}
	
	Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_IPM_LaneAssit_WarningAudio(CHIME_REQID_LaneAssit_WarningAudio);
	Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_Nav_Sts(CHIME_REQID_Nav_Sts);
	Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_IPM_LaneAssit_HandsonReq(CHIME_REQID_LaneAssit_HandsonReq);
	Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_MRR_TakeOverReq(CHIME_REQID_TAKE_OVER_REQ);
	Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_MRR_PCW_STATE(CHIME_REQID_PCW_STATE);

	Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_IPM_LaneAssit_WarningAudio(&fl_IPM_LaneAssit_WarningAudio_U8);
	Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_Nav_Sts(&fl_Nav_Sts_U8);
	Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_IPM_LaneAssit_HandsonReq(&fl_IPM_LaneAssit_HandsonReq_U8);
	Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_MRR_TakeOverReq(&fl_MRR_TakeOverReq_U8);
	Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_MRR_PCW_STATE(&fl_MRR_PCW_STATE_U8);
	
	if (TRUE == fl_IPM_LaneAssit_WarningAudio_U8)
	{
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_LaneAssit_WarningAudio, FALSE);
	}

	if (TRUE == fl_Nav_Sts_U8)
	{
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_Nav_Sts, FALSE);
	}

	if (TRUE == fl_IPM_LaneAssit_HandsonReq_U8)
	{
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_LaneAssit_HandsonReq, FALSE);
	}

	if (TRUE == fl_MRR_TakeOverReq_U8)
	{
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_TAKE_OVER_REQ, FALSE);
	}

	if (TRUE == fl_MRR_PCW_STATE_U8)
	{
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_PCW_STATE, FALSE);
	}
}

static void f_ChimeInit(void)
{
	uint8 i = 0;

	for (i = EM_CHIME_CTRL_FLG_FCW; i < EM_CHIME_CTRL_FLG_MAX; i++)
	{
		stAdasChimeCtrlInfo[i].u8Flag = FALSE;
		stAdasChimeCtrlInfo[i].u8Count = 0;
	}

}

static void f_Chime_246_Acc_Pro(void)
{
	/* 14-ACC takeover */
	if(((ADAS_FCW_VALUE_1 == stAdasChimeMsgInfo.MRR_TakeOverReq)
	&& ((stAdasChimeMsgInfo.AccMode > ADAS_FCW_VALUE_0) && (stAdasChimeMsgInfo.AccMode < ADAS_FCW_VALUE_7)))
	&& ((stAdasChimeMsgInfoBk.MRR_TakeOverReq != stAdasChimeMsgInfo.MRR_TakeOverReq) || (stAdasChimeMsgInfoBk.AccMode != stAdasChimeMsgInfo.AccMode)))
	{
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_TAKE_OVER_REQ, TRUE);
		stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_TAKE_OVER].u8Count = ADAS_CHIME_DELAY_TIME_1000MS;
	}
	else
	{
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_TAKE_OVER_REQ, FALSE);
	}

	stAdasChimeMsgInfoBk.MRR_TakeOverReq = stAdasChimeMsgInfo.MRR_TakeOverReq;
	stAdasChimeMsgInfoBk.AccMode = stAdasChimeMsgInfo.AccMode;

}


static void f_Chime_246_Pebs_Fcw_Pro(void) 
{
	/* 15-PEBS_FCW */
	if (stAdasChimeMsgInfoBk.PCW_preWarning != stAdasChimeMsgInfo.PCW_preWarning)
	{
		stAdasChimeMsgInfoBk.PCW_preWarning = stAdasChimeMsgInfo.PCW_preWarning;
		if (0 != stAdasChimeMsgInfo.PCW_preWarning)
		{
			stAdasChimeMsgInfo.pcw_preWarnigFlg = TRUE;
		}
	}
	
	if (stAdasChimeMsgInfoBk.PCW_latentWarning != stAdasChimeMsgInfo.PCW_latentWarning)
	{
		stAdasChimeMsgInfoBk.PCW_latentWarning = stAdasChimeMsgInfo.PCW_latentWarning;
		if (0 != stAdasChimeMsgInfo.PCW_latentWarning)
		{
			stAdasChimeMsgInfo.pcw_latentWarningFlg = TRUE;
		}
	}

	if (stAdasChimeMsgInfoBk.FCW_preWarning != stAdasChimeMsgInfo.FCW_preWarning)
	{
		stAdasChimeMsgInfoBk.FCW_preWarning = stAdasChimeMsgInfo.FCW_preWarning;
		if (0 != stAdasChimeMsgInfo.FCW_preWarning)
		{
			stAdasChimeMsgInfo.fcw_preWarnigFlg = TRUE;
		}
	}
	
	if (stAdasChimeMsgInfoBk.FCW_latentWarning != stAdasChimeMsgInfo.FCW_latentWarning)
	{
		stAdasChimeMsgInfoBk.FCW_latentWarning = stAdasChimeMsgInfo.FCW_latentWarning;
		if (0 != stAdasChimeMsgInfo.FCW_latentWarning)
		{
			stAdasChimeMsgInfo.fcw_latentWarningFlg = TRUE;
		}
	}
	
	if((TRUE == stAdasChimeMsgInfo.pcw_preWarnigFlg) || (TRUE == stAdasChimeMsgInfo.pcw_latentWarningFlg)
    || (TRUE == stAdasChimeMsgInfo.fcw_preWarnigFlg) || (TRUE == stAdasChimeMsgInfo.fcw_latentWarningFlg))
	{
		stAdasChimeMsgInfo.pcw_preWarnigFlg     = FALSE;
		stAdasChimeMsgInfo.pcw_latentWarningFlg = FALSE;
		stAdasChimeMsgInfo.fcw_preWarnigFlg     = FALSE;
		stAdasChimeMsgInfo.fcw_latentWarningFlg = FALSE;
		
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_PCW_STATE, TRUE);
		stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_FCW].u8Count = ADAS_CHIME_DELAY_TIME_1000MS;
	}
	else
	{

		if (0 == stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_FCW].u8Count)
		{
			Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_PCW_STATE, FALSE);
		}
	}
}

static void f_Chime_245_HandsOnReq_Pro(void)
{
	/* 16-LaneAssit_HandsonReq */
	if (stAdasChimeMsgInfoBk.IPM_LaneAssit_HandsonReq != stAdasChimeMsgInfo.IPM_LaneAssit_HandsonReq)
	{
		stAdasChimeMsgInfoBk.IPM_LaneAssit_HandsonReq = stAdasChimeMsgInfo.IPM_LaneAssit_HandsonReq;

		if (0 != stAdasChimeMsgInfo.IPM_LaneAssit_HandsonReq)
		{
			Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_LaneAssit_HandsonReq, TRUE);
			stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_HANDS_ON].u8Count = ADAS_CHIME_DELAY_TIME_1000MS;
		}
		else
		{
			if (0 == stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_HANDS_ON].u8Count)
			{
				Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_LaneAssit_HandsonReq, FALSE);
			}
		}
	}
}

static void f_Chime_245_Ldw_Pro(void)
{
	/* 16-LDW LaneAssit_WarningAudio */
	if (stAdasChimeMsgInfoBk.IPM_LaneAssit_WarningAudio != stAdasChimeMsgInfo.IPM_LaneAssit_WarningAudio)
	{
		stAdasChimeMsgInfoBk.IPM_LaneAssit_WarningAudio = stAdasChimeMsgInfo.IPM_LaneAssit_WarningAudio;

		if (0 != stAdasChimeMsgInfo.IPM_LaneAssit_WarningAudio)
		{
			Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_LaneAssit_WarningAudio, TRUE);
			stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_LDW].u8Count = ADAS_CHIME_DELAY_TIME_2000MS;
		}
		else
		{
			if (0 == stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_LDW].u8Count)
			{
				Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_LaneAssit_WarningAudio, FALSE);
			}
		}
	}
}

static void f_Chime_245_Ddd_Pro(void)
{
	/* 22-DDD_Warning */
	if(((ADAS_DDD_VALUE_1 == stAdasChimeMsgInfo.IPM_DDD_WarningLevel) && (ADAS_DDD_VALUE_3 != stAdasChimeMsgInfo.IPM_DDD_IndexState))
	&&((stAdasChimeMsgInfoBk.IPM_DDD_WarningLevel != stAdasChimeMsgInfo.IPM_DDD_WarningLevel) || (stAdasChimeMsgInfoBk.IPM_DDD_IndexState != stAdasChimeMsgInfo.IPM_DDD_IndexState)))
	{
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_Nav_Sts, TRUE);
		stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_DDD].u8Count = ADAS_CHIME_DELAY_TIME_3000MS;
	}
	else
	{
		if (0 == stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_DDD].u8Count)
		{
			Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_Nav_Sts, FALSE);
		}
	}
	stAdasChimeMsgInfoBk.IPM_DDD_WarningLevel = stAdasChimeMsgInfo.IPM_DDD_WarningLevel;
	stAdasChimeMsgInfoBk.IPM_DDD_IndexState = stAdasChimeMsgInfo.IPM_DDD_IndexState;
	
}

static void f_Chime_275_Cta_Pro(void) 
{
	//17-BLIS_CTA 4hz continuous at least 1s
	if ((ADAS_BSD_LEVEL_2 == stAdasChimeMsgInfo.SOD_CTA_warningReqLeft) || (ADAS_BSD_LEVEL_2 == stAdasChimeMsgInfo.SOD_CTA_warningReqRight))
	{
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_CTA_warningReq, TRUE);
		if (FALSE == stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_CTA].u8Flag)
		{
			stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_CTA].u8Count = ADAS_CHIME_DELAY_TIME_1000MS;
			stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_CTA].u8Flag = TRUE;
		}
	}
	else
	{
		if (0 == stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_CTA].u8Count)
		{
			Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_CTA_warningReq, FALSE);
			stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_CTA].u8Flag = FALSE;
		}
	}
}

static void f_Chime_275_Doa_Pro(void) 
{	
	//17-BLIS_CTA 4hz continuous at least 1s
	if ((ADAS_BSD_LEVEL_2 == stAdasChimeMsgInfo.SOD_DOA_warningReqLeftFront)	|| 
	    (ADAS_BSD_LEVEL_2 == stAdasChimeMsgInfo.SOD_DOA_warningReqLeftRear)	|| 
		(ADAS_BSD_LEVEL_2 == stAdasChimeMsgInfo.SOD_DOA_warningReqRightFront) || 
		(ADAS_BSD_LEVEL_2 == stAdasChimeMsgInfo.SOD_DOA_warningReqRightRear))
	{
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_DOA_WarningReq, TRUE);
		if (FALSE == stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_DOA].u8Flag)
		{
			stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_DOA].u8Count = ADAS_CHIME_DELAY_TIME_1000MS;
			stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_DOA].u8Flag = TRUE;
		}
		
	}
	else
	{
		if (0 == stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_DOA].u8Count)
		{
			Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_DOA_WarningReq, FALSE);
			stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_DOA].u8Flag = FALSE;
		}
	}
}

static void f_Chime_382_Bsd_LCA_Pro(void) //5-2 4hz continuous at least 1s
{
	//17-BLIS_CTA
	if (((ADAS_BSD_LEVEL_2 == stAdasChimeMsgInfo.SOD_LCA_warningReqLeft) && (ADAS_BSD_LEVEL_1 == stAdasChimeMsgInfo.MP5_BSDSwitchSts))
	 || ((ADAS_BSD_LEVEL_2 == stAdasChimeMsgInfo.SOD_LCA_warningReqRight) && (ADAS_BSD_LEVEL_1 == stAdasChimeMsgInfo.MP5_BSDSwitchSts)))
	{
		Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_BSDSound_SwitchSts, TRUE);
		if (FALSE == stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_BSD].u8Flag)
		{
			stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_BSD].u8Count = ADAS_CHIME_DELAY_TIME_1000MS;
			stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_BSD].u8Flag = TRUE;
		}
	}
	else
	{
		if (0 == stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_BSD].u8Count)
		{
			Rte_Call_rpCS_AdasChime_UpdateStatus_Operation(CHIME_REQID_BSDSound_SwitchSts, FALSE);
			stAdasChimeCtrlInfo[EM_CHIME_CTRL_FLG_BSD].u8Flag = FALSE;
		}
	}

	Rte_Write_ppSR_CANMSG_IC_0x386_ComOut_IC_BSDSoundSts(fl_IC_BSDSwitchSts_U8);
}

static void f_GW_MRR_0x246_Chime_Acc_init(void)
{
	stAdasChimeMsgInfo.AccMode           = 0;
	stAdasChimeMsgInfo.MRR_TakeOverReq   = 0;

	stAdasChimeMsgInfoBk.AccMode         = 0;
	stAdasChimeMsgInfoBk.MRR_TakeOverReq = 0;
}

static void f_GW_MRR_0x246_Chime_Acc_process(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	
	uint8 	fl_MRR_TakeOverReq_U8 = 0;
	uint8 	fl_acc_mode_U8        = 0;

	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout(&fl_CAN_msg_status_U8);
	
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TakeOverReq(&fl_MRR_TakeOverReq_U8);//14-Acc
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode(&fl_acc_mode_U8);           //14-Acc
	
	if (fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_GW_MRR_0x246_Chime_Acc_init();
	}
	else 
	{
		stAdasChimeMsgInfo.MRR_TakeOverReq = fl_MRR_TakeOverReq_U8;
		stAdasChimeMsgInfo.AccMode = fl_acc_mode_U8;
	}
}

static void f_GW_MRR_0x246_Chime_Pebs_Fcw_init(void)
{
	stAdasChimeMsgInfo.PCW_preWarning    = 0;
	stAdasChimeMsgInfo.PCW_latentWarning = 0;
	stAdasChimeMsgInfo.FCW_preWarning    = 0;
	stAdasChimeMsgInfo.FCW_latentWarning = 0;
	
	stAdasChimeMsgInfo.pcw_preWarnigFlg     = FALSE;
	stAdasChimeMsgInfo.pcw_latentWarningFlg = FALSE;
	stAdasChimeMsgInfo.fcw_preWarnigFlg     = FALSE;
	stAdasChimeMsgInfo.fcw_latentWarningFlg = FALSE;

	stAdasChimeMsgInfoBk.PCW_preWarning    = 0;
	stAdasChimeMsgInfoBk.PCW_latentWarning = 0;
	stAdasChimeMsgInfoBk.FCW_preWarning    = 0;
	stAdasChimeMsgInfoBk.FCW_latentWarning = 0;
}

static void f_GW_MRR_0x246_Chime_Pebs_Fcw_process(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	
	uint8 	fl_pcw_preWarning_U8    = 0;
	uint8 	fl_pcw_latentWarning_U8 = 0;
	uint8 	fl_fcw_preWarning_U8    = 0;
	uint8 	fl_fcw_latentWarning_U8 = 0;
	
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout(&fl_CAN_msg_status_U8);

	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_preWarning(&fl_pcw_preWarning_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_latentWarning(&fl_pcw_latentWarning_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_preWarning(&fl_fcw_preWarning_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_latentWarning(&fl_fcw_latentWarning_U8);
	
	if (fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_GW_MRR_0x246_Chime_Pebs_Fcw_init();
	}
	else 
	{
		stAdasChimeMsgInfo.PCW_preWarning    = fl_pcw_preWarning_U8;
		stAdasChimeMsgInfo.PCW_latentWarning = fl_pcw_latentWarning_U8;
		stAdasChimeMsgInfo.FCW_preWarning    = fl_fcw_preWarning_U8;
		stAdasChimeMsgInfo.FCW_latentWarning = fl_fcw_latentWarning_U8;
	}
}


static void f_GW_IPM_0x245_Chime_LaneAssit_init(void)
{
	stAdasChimeMsgInfo.IPM_LaneAssit_HandsonReq = 0;
	stAdasChimeMsgInfo.IPM_LaneAssit_WarningAudio = 0;

	stAdasChimeMsgInfoBk.IPM_LaneAssit_HandsonReq = 0;
	stAdasChimeMsgInfoBk.IPM_LaneAssit_WarningAudio = 0;
}

static void f_GW_IPM_0x245_Chime_LaneAssit_process(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	
	uint8   fl_IPM_LaneAssit_HandsonReq_U8 = 0;
	uint8   fl_IPM_LaneAssit_WarningAudio_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_Tout(&fl_CAN_msg_status_U8);

	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_HandsonReq(&fl_IPM_LaneAssit_HandsonReq_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_WarningAudio(&fl_IPM_LaneAssit_WarningAudio_U8);

	if (fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_GW_IPM_0x245_Chime_LaneAssit_init();
	}
	else 
	{
		stAdasChimeMsgInfo.IPM_LaneAssit_HandsonReq = fl_IPM_LaneAssit_HandsonReq_U8;
		stAdasChimeMsgInfo.IPM_LaneAssit_WarningAudio = fl_IPM_LaneAssit_WarningAudio_U8;
	}
}

static void f_GW_IPM_0x245_Chime_Ddd_Warning_init(void)
{
	stAdasChimeMsgInfo.IPM_DDD_WarningLevel = 0;
	stAdasChimeMsgInfo.IPM_DDD_IndexState = 0;

	stAdasChimeMsgInfoBk.IPM_DDD_WarningLevel = 0;
	stAdasChimeMsgInfoBk.IPM_DDD_IndexState = 0;
}

static void f_GW_IPM_0x245_Chime_Ddd_Warning_process(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	
	uint8   fl_IPM_DDD_WarningLevel_U8 = 0;
	uint8   fl_IPM_DDD_IndexState_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_Tout(&fl_CAN_msg_status_U8);

	if (fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_GW_IPM_0x245_Chime_Ddd_Warning_init();
	}
	else 
	{
		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_WarningLevel(&fl_IPM_DDD_WarningLevel_U8);
		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_IndexStatus(&fl_IPM_DDD_IndexState_U8);
		
		stAdasChimeMsgInfo.IPM_DDD_WarningLevel = fl_IPM_DDD_WarningLevel_U8;
		stAdasChimeMsgInfo.IPM_DDD_IndexState = fl_IPM_DDD_IndexState_U8;
	}
}

static void f_GW_SOD_0x275_Chime_CTA_init(void)
{
	stAdasChimeMsgInfo.SOD_LCA_warningReqLeft = 0;
	stAdasChimeMsgInfo.SOD_LCA_warningReqRight = 0;

	stAdasChimeMsgInfo.SOD_CTA_warningReqLeft = 0;
	stAdasChimeMsgInfo.SOD_CTA_warningReqRight = 0;

	stAdasChimeMsgInfo.SOD_DOA_warningReqRightFront = 0;
	stAdasChimeMsgInfo.SOD_DOA_warningReqRightRear = 0;
	stAdasChimeMsgInfo.SOD_DOA_warningReqLeftFront = 0;
	stAdasChimeMsgInfo.SOD_DOA_warningReqLeftRear = 0;

	stAdasChimeMsgInfoBk.SOD_LCA_warningReqLeft = 0;
	stAdasChimeMsgInfoBk.SOD_LCA_warningReqRight = 0;

	stAdasChimeMsgInfoBk.SOD_CTA_warningReqLeft = 0;
	stAdasChimeMsgInfoBk.SOD_CTA_warningReqRight = 0;

	stAdasChimeMsgInfoBk.SOD_DOA_warningReqRightFront = 0;
	stAdasChimeMsgInfoBk.SOD_DOA_warningReqRightRear = 0;
	stAdasChimeMsgInfoBk.SOD_DOA_warningReqLeftFront = 0;
	stAdasChimeMsgInfoBk.SOD_DOA_warningReqLeftRear = 0;
}

static void f_GW_SOD_0x275_Chime_CTA_process(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	
	uint8   fl_SOD_LCA_warningReqLeft_U8 = 0;
	uint8   fl_SOD_LCA_warningReqRight_U8 = 0;

	uint8   fl_SOD_CTA_warningReqLeft_U8 = 0;
	uint8   fl_SOD_CTA_warningReqRight_U8 = 0;

	uint8   fl_SOD_DOA_warningReqLeftFront_U8 = 0;
	uint8   fl_SOD_DOA_warningReqLeftRear_U8 = 0;
	uint8   fl_SOD_DOA_warningReqRightFront_U8 = 0;
	uint8   fl_SOD_DOA_warningReqRightRear_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_Tout(&fl_CAN_msg_status_U8);

	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqLeft(&fl_SOD_LCA_warningReqLeft_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqRight(&fl_SOD_LCA_warningReqRight_U8);

	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqLeft(&fl_SOD_CTA_warningReqLeft_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqRight(&fl_SOD_CTA_warningReqRight_U8);

	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftFront(&fl_SOD_DOA_warningReqLeftFront_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftRear(&fl_SOD_DOA_warningReqLeftRear_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightFront(&fl_SOD_DOA_warningReqRightFront_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightRear(&fl_SOD_DOA_warningReqRightRear_U8);

	if (fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_GW_SOD_0x275_Chime_CTA_init();
	}
	else 
	{
		stAdasChimeMsgInfo.SOD_LCA_warningReqLeft = fl_SOD_LCA_warningReqLeft_U8;
		stAdasChimeMsgInfo.SOD_LCA_warningReqRight = fl_SOD_LCA_warningReqRight_U8;

		stAdasChimeMsgInfo.SOD_CTA_warningReqLeft = fl_SOD_CTA_warningReqLeft_U8;
		stAdasChimeMsgInfo.SOD_CTA_warningReqRight = fl_SOD_CTA_warningReqRight_U8;

		stAdasChimeMsgInfo.SOD_DOA_warningReqLeftFront = fl_SOD_DOA_warningReqLeftFront_U8;
		stAdasChimeMsgInfo.SOD_DOA_warningReqLeftRear = fl_SOD_DOA_warningReqLeftRear_U8;
		stAdasChimeMsgInfo.SOD_DOA_warningReqRightFront = fl_SOD_DOA_warningReqRightFront_U8;
		stAdasChimeMsgInfo.SOD_DOA_warningReqRightRear = fl_SOD_DOA_warningReqRightRear_U8;
	}
}


static void f_GW_MP5_0x382_Chime_BSDSound_init(void)
{
	stAdasChimeMsgInfo.MP5_BSDSwitchSts = 0;

	stAdasChimeMsgInfoBk.MP5_BSDSwitchSts = 0;
	fl_IC_BSDSwitchSts_U8 = 0;
}

static void f_GW_MP5_0x382_Chime_BSDSound_process(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	
	uint8   fl_MP5_BSDSwitchSts_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_MP5_0x382_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_MP5_0x382_ComIn_Tout(&fl_CAN_msg_status_U8);

	Rte_Read_rpSR_CANMSG_GW_MP5_0x382_ComIn_MP5_BSDSoundSwitchSts(&fl_MP5_BSDSwitchSts_U8);

	if (fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_GW_MP5_0x382_Chime_BSDSound_init();
	}
	else 
	{
		stAdasChimeMsgInfo.MP5_BSDSwitchSts = fl_MP5_BSDSwitchSts_U8;
	}

	if(ADAS_BSD_LEVEL_1 == stAdasChimeMsgInfo.MP5_BSDSwitchSts)
	{
		fl_IC_BSDSwitchSts_U8 = 1;
	}
	else
	{
		fl_IC_BSDSwitchSts_U8 = 0;
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
