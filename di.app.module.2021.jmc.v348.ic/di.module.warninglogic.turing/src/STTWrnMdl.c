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
#include "Rte_STTWrnMdl.h"

#include "CmpLib.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(STTWrnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

#if 0
typedef struct
{
  uint8 m_STT_can_value;
  WARN_ID_INDEX m_STT_warn_id;
}STT_WARN_S;
#endif

#if 0
typedef enum 
{
    MSG_STT_NO_DIS0  =	0	,
    MSG_STT_CLM_REQ  =	1	,
    MSG_STT_BRK_LOW  =	2	,
    MSG_STT_FUN_OFF  =	3	,
    MSG_STT_BAT_LOW  =	4	,
    MSG_STT_ECT_LOW  =	5	,
    MSG_STT_APA_INHB =	6	,
    MSG_STT_ACC_INHB =	7	,
    MSG_STT_TCU_INHB =	8	,
    MSG_STT_ST_PTC   =	9	,
    MSG_STT_HK_OPEN  =	10	,
    MSG_STT_DOOR_OPEN  =	11	,
    MSG_STT_ANG_HIGH   =	12	,
    MSG_STT_FUN_FAIL   =	13	,
    MSG_STT_NO_DIS14   =	14	,
    MSG_STT_NO_DIS15   =	15	,
}CAN_SIGNAL_VAL_T;
#endif

#if 0
#define PROD_SPEC_STT_WARNING_LIST \
{MSG_STT_NO_DIS0  , WARN_MAX_ID},\
{MSG_STT_CLM_REQ  , WARN_STT_CLM_REQ_WARNING_ID  },\
{MSG_STT_BRK_LOW  , WARN_STT_BRK_LOW_WARNING_ID  },\
{MSG_STT_FUN_OFF  , WARN_STT_FUN_OFF_WARNING_ID  },\
{MSG_STT_BAT_LOW  , WARN_STT_BAT_LOW_WARNING_ID  },\
{MSG_STT_ECT_LOW  , WARN_STT_ECT_LOW_WARNING_ID  },\
{MSG_STT_APA_INHB , WARN_STT_APA_INHB_WARNING_ID },\
{MSG_STT_ACC_INHB , WARN_STT_ACC_INHB_WARNING_ID },\
{MSG_STT_TCU_INHB , WARN_STT_TCU_INHB_WARNING_ID },\
{MSG_STT_ST_PTC   , WARN_STT_ST_PTC_WARNING_ID   },\
{MSG_STT_HK_OPEN  , WARN_STT_HK_OPEN_WARNING_ID  },\
{MSG_STT_DOOR_OPEN, WARN_STT_DOOR_OPEN_WARNING_ID},\
{MSG_STT_ANG_HIGH , WARN_STT_ANG_HIGH_WARNING_ID },\
{MSG_STT_FUN_FAIL , WARN_STT_FUN_FAIL_WARNING_ID },\
{MSG_STT_NO_DIS14 , WARN_MAX_ID},\
{MSG_STT_NO_DIS15 , WARN_MAX_ID},\

#define PROD_SPEC_STT_WARNING_LIST_NUM 16

static const STT_WARN_S c_STT_warn_SA[PROD_SPEC_STT_WARNING_LIST_NUM] = {PROD_SPEC_STT_WARNING_LIST};
/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */
#endif

//static uint8 Ems_last_Stt_status_u8;

#define STT_WARNING_Init 	((uint8)15)

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
//static uint8 message_invalid(void);
//static uint8 Ems_Stt_status_u8 = STT_WARNING_Init;

static void STT_Warning_Init(void);
static void STT_Warning_Process(void); 
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
    STT_Warning_Init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
    STT_Warning_Init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation( void )
{
    STT_Warning_Init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation( void )
{
    STT_Warning_Init();
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
static Std_ReturnType CmpActive( void )
{
  EBatteryState fl_Battery_state = eBatteryState_Normal;
	eIgnState IGN_state = eIGN_RUN;
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
  Rte_Read_rpIgnState_IGNState(&IGN_state);
	
	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{
		STT_Warning_Init();
	}
	else
	{
		switch(IGN_state)
		{
			case eIGN_OFF:					
				STT_Warning_Init();		
				break;
			case eIGN_RUN:
				STT_Warning_Process();
				break;
			default:
				break;
		}
	}
	
	//Rte_Write_STTWrnMdl_rpSR_SttWrnMdl_EMS_StartStopMessage_Status(Ems_Stt_status_u8);

	//26	start-stop system fail warning
	/*
	uint8 u8Msg366NR = 0;
	uint8 u8Msg366Tout = 0;
	uint8 u8Msg310NR = 0;
	uint8 u8Msg310Tout = 0;	
	uint8 u8StartStopStatus_U8 = 0;
	uint8 u8Msg10BNR = 0;
	uint8 u8Msg10BTout = 0;	
	uint8 u8NeutralSensorSts_U8 = 0;
  uint8 StartStopMessage = 0;
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR(&u8Msg366NR);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout(&u8Msg366Tout);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_StartStopStatus(&u8StartStopStatus_U8);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&u8Msg310NR);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&u8Msg310Tout);
  Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&u8Msg10BNR);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&u8Msg10BTout);
  Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_StartStopMessage(&StartStopMessage);
	Rte_Read_STTWrnMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_MT_NeutralSensorSts(&u8NeutralSensorSts_U8);
	if ( (RTE_E_TIMEOUT == u8Msg10BTout) || (RTE_E_NEVER_RECEIVED == u8Msg10BNR) )
	{
		CWarningCtrl_Set_Warning(WARN_STT_NEW_FUN_FAIL_WARNING_ID ,UI_WARNING_STATUS_OFF);
		CWarningCtrl_Set_Warning(WARN_STT_NEW_FUN_CLOSE_WARNING_ID ,UI_WARNING_STATUS_OFF);
	}
	else
	{
		if((uint8)0xD == StartStopMessage)
		{
		  CWarningCtrl_Set_Warning(WARN_STT_NEW_FUN_FAIL_WARNING_ID ,UI_WARNING_STATUS_ON);
		  CWarningCtrl_Set_Warning(WARN_STT_NEW_FUN_CLOSE_WARNING_ID ,UI_WARNING_STATUS_OFF);
		}
		else if((uint8)3==StartStopMessage)
		{
			CWarningCtrl_Set_Warning(WARN_STT_NEW_FUN_CLOSE_WARNING_ID ,UI_WARNING_STATUS_ON);
			CWarningCtrl_Set_Warning(WARN_STT_NEW_FUN_FAIL_WARNING_ID ,UI_WARNING_STATUS_OFF);
		}
		else
		{
      CWarningCtrl_Set_Warning(WARN_STT_NEW_FUN_CLOSE_WARNING_ID ,UI_WARNING_STATUS_OFF);
			CWarningCtrl_Set_Warning(WARN_STT_NEW_FUN_FAIL_WARNING_ID ,UI_WARNING_STATUS_OFF);
		}
	}

	if ( (RTE_E_TIMEOUT == u8Msg366Tout) || (RTE_E_NEVER_RECEIVED == u8Msg366NR)
		|| (RTE_E_TIMEOUT == u8Msg310Tout) || (RTE_E_NEVER_RECEIVED == u8Msg310NR) )
	{
		CWarningCtrl_Set_Warning(WARN_STT_NEW_NEUTRAL_WARNING_ID ,UI_WARNING_STATUS_OFF);
	}
	else
	{
		if(((uint8)1==u8StartStopStatus_U8)&&((uint8)1==u8NeutralSensorSts_U8))
		{
			CWarningCtrl_Set_Warning(WARN_STT_NEW_NEUTRAL_WARNING_ID ,UI_WARNING_STATUS_ON);
		}
		else
		{
			CWarningCtrl_Set_Warning(WARN_STT_NEW_NEUTRAL_WARNING_ID ,UI_WARNING_STATUS_OFF);
		}
	}
	*/
    return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief  Diagnostic state
///
/// @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}

#if 0
static uint8 message_invalid(void)
{
   uint8 u8MsgNR = 0;
   uint8 u8MsgTout = 0;

   Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&u8MsgNR);
   Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&u8MsgTout);
   
   
   if ((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
   {
       return TRUE;
   }
   else
   {
       return FALSE;
   }
}
extern void CWarningCtrl_Set_Warning(UInt8 WarningID ,UInt8 Status);
#endif
static void STT_Warning_Init(void)
{
#if 0
    uint8 i = 0;
    for(i = 0; i < PROD_SPEC_STT_WARNING_LIST_NUM; i++)
    {
        CWarningCtrl_Set_Warning(c_STT_warn_SA[i].m_STT_warn_id ,UI_WARNING_STATUS_OFF);
    }
#endif
	//Rte_Write_HighBeamMdl_rpSR_HighBeamMdl_Status_LowBeamSts(l_lowbeam_status_u8);
	
	//Ems_last_Stt_status_u8 = STT_WARNING_Init;

  /*Ems_Stt_status_u8 = STT_WARNING_Init;
	Rte_Write_STTWrnMdl_rpSR_SttWrnMdl_EMS_StartStopMessage_Status(Ems_Stt_status_u8);

	CWarningCtrl_Set_Warning(WARN_STT_NEW_FUN_FAIL_WARNING_ID ,UI_WARNING_STATUS_OFF);
	CWarningCtrl_Set_Warning(WARN_STT_NEW_FUN_CLOSE_WARNING_ID ,UI_WARNING_STATUS_OFF);
  */
}

static void STT_Warning_Process(void)
{
#if 0
    uint8 u8SigVal = 0;

    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_StartStopMessage(&u8SigVal);

	uint8 i = 0;
	for(i = 0; i < PROD_SPEC_STT_WARNING_LIST_NUM; i++)
	{
		if(c_STT_warn_SA[i].m_STT_can_value == u8SigVal )
		{
			CWarningCtrl_Set_Warning(c_STT_warn_SA[i].m_STT_warn_id ,UI_WARNING_STATUS_ON);
		}
		else
		{
			CWarningCtrl_Set_Warning(c_STT_warn_SA[i].m_STT_warn_id ,UI_WARNING_STATUS_OFF);
		}
	}
#endif
/*
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;	
	uint8 fl_stt_sig_value_U8 = STT_WARNING_Init;

	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&u8MsgTout);
	

	if ( (RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR) )
	{
		Ems_Stt_status_u8 = STT_WARNING_Init;
	}
	else
	{
		Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_StartStopMessage(&fl_stt_sig_value_U8);
		
		Ems_Stt_status_u8 = fl_stt_sig_value_U8;
	}
	*/
}
