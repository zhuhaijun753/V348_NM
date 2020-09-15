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
#include "Rte_AEBWrnMdl.h"

#include "CmpLib.h"
#include "aeb_wrn_cfg.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(AEBWrnMdl)

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
// static uint8 AEBWarningStatus[UI_WARNING_TOTAL_AEB]={0};

//static uint8 AEBWarningId[UI_WARNING_TOTAL_AEB]=
//{
//	WARN_HAS_FAULT_ID 							,
//	WARN_EBS_OFF_ID 							,
//	WARN_EBS_FAULT_ID							,
//	WARN_EBS_TMP_UNAVAILABLE_ID 				,
//	WARN_FCW_OFF_ID 							,
//	WARN_FCW_FAULT_ID							,
//	WARN_EBS_ON_ID								,
//	WARN_FCW_ON_ID								,
//	WARN_LANE_KEEPING_SYS_OFF_ID				,
//	WARN_LANE_KEEPING_SYS_FAULT_ID				,
//	WARN_CAMERA_BLOCKED_ID						,
//	WARN_LANE_KEEPING_SYS_TMP_UNAVAILABLE_ID	,
//	WARN_HANDS_ON_STEERING_ID 					,
//	WARN_LANE_KEEPING_SYS_ON_ID					
//};

// #define GW_CAMERA_CLEAR	0
// #define GW_RADA2_CLEAR	1
// #define GW_PSA2_CLEAR	2

// static void fun_CAMERA_process(void);
// static void fun_RADA2_process(void);
// static void fun_PSA2_process(void);

// static void ClearWarning(uint8 clearNo_U8);
// static void ClearAllWarning(void);

// static void SetWarningStatus(uint8 clearNo_U8,uint8 index);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
   // ClearAllWarning();
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
   // ClearAllWarning();
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
   // ClearAllWarning();
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
//	int i;
//    EBatteryState Battery_state;
//	boolean IsCfg = FALSE;
//	
//	Rte_Read_rpSR_GdtCtrl_AEB_FCW_ACC_Cfg(&IsCfg);
//    Rte_Read_rpBattState_BatteryState(&Battery_state);
//        
//    if(Battery_state == eBatteryState_UnderVoltage || Battery_state == eBatteryState_OverVoltage)
//    {
//        ClearAllWarning();
//    }
//    else
//    {            
//        fun_CAMERA_process();
//		fun_RADA2_process();
//		fun_PSA2_process();
//    }
//
//	if(FALSE != IsCfg)
//	{    	
//	}
//	else
//	{
//		for(i = 0; i < UI_WARNING_TOTAL_AEB; i++)
//		{
//			if(i == UI_WARNING_HAS_FAULT)
//			{
//			}
//			else 
//			{
//				AEBWarningStatus[i] = 0;
//			}
//		}
//	}
//		
//    for(int i = 0; i < UI_WARNING_TOTAL_AEB; i++)
//    {
//		Rte_Call_rpCS_WarningCtrl_Set_Operation(AEBWarningId[i],AEBWarningStatus[i]);
//    }
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

// static void ClearWarning(uint8 clearNo_U8)
// {
//	int i = 0 ;
//	int operationMin,operationMax;
//	switch(clearNo_U8)
//	{
//		case GW_CAMERA_CLEAR:
//			operationMin = UI_WARNING_LANE_KEEPING_SYS_OFF;
//			operationMax = UI_WARNING_LANE_KEEPING_SYS_ON;
//			break;
//		case GW_RADA2_CLEAR:
//			operationMin = UI_WARNING_EBS_OFF;
//			operationMax = UI_WARNING_FCW_ON;
//			break;
//		case GW_PSA2_CLEAR:
//			return;
//		default:
//			return;
//	}
//	for(i = operationMin; i <= operationMax; i++)
//	{
//		Rte_Call_rpCS_WarningCtrl_Set_Operation(AEBWarningId[i],UI_WARNING_STATUS_OFF);
//		AEBWarningStatus[i] = 0;
//	}	
// }

// static void ClearAllWarning(void)
// {
//	int i;
//	for(i = 0; i < UI_WARNING_TOTAL_AEB; i++)
//	{
//		Rte_Call_rpCS_WarningCtrl_Set_Operation(AEBWarningId[i],UI_WARNING_STATUS_OFF);
//		AEBWarningStatus[i] = 0;
//	}	
// }


// static void fun_CAMERA_process(void)
// {
//    uint8 u8HMIPopupSt;  
//    uint8 u8MsgNR;
//    uint8 u8MsgTout;
//    u8MsgNR = 0;
//    u8MsgTout = 0;
//    u8HMIPopupSt = 0;
//
//    Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_NR(&u8MsgNR);
//    Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_Tout(&u8MsgTout);
//	Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_HMI_Popup_Status(&u8HMIPopupSt);
//
//    if ((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
//    {
//        ClearWarning(GW_CAMERA_CLEAR);
//    }
//    else
//    {
//		switch(u8HMIPopupSt)
//		{
//			case 1:
//				SetWarningStatus(GW_CAMERA_CLEAR,UI_WARNING_LANE_KEEPING_SYS_ON);
//			break;
//			case 2:
//				SetWarningStatus(GW_CAMERA_CLEAR,UI_WARNING_HANDS_ON_STEERING);
//			break;
//			case 3:
//				SetWarningStatus(GW_CAMERA_CLEAR,UI_WARNING_LANE_KEEPING_SYS_TMP);
//			break;
//			case 4:
//				SetWarningStatus(GW_CAMERA_CLEAR,UI_WARNING_CAMERA_BLOCKED);
//			break;
//			case 6:
//				SetWarningStatus(GW_CAMERA_CLEAR,UI_WARNING_LANE_KEEPING_SYS_FAULT);
//			break;
//			case 8:
//				SetWarningStatus(GW_CAMERA_CLEAR,UI_WARNING_LANE_KEEPING_SYS_OFF);
//			break;
//			default:
//				ClearWarning(GW_CAMERA_CLEAR);
//			break;
//		}
//    }
// }


// static void fun_RADA2_process(void)
// {
//	uint8 u8MsgNR;
//	uint8 u8MsgTout;
//	uint8 u8FEWarningSt = 0;	
//	static uint8 u8LastFESt = 0;
//	static uint8 u8CurrentFESt = 0;
//	u8MsgNR = 0;
//	u8MsgTout = 0;
//
//	Rte_Read_rpSR_CANMSG_GW_RADA2_ComIn_NR(&u8MsgNR);
//	Rte_Read_rpSR_CANMSG_GW_RADA2_ComIn_Tout(&u8MsgTout);
//	Rte_Read_rpSR_CANMSG_GW_RADA2_ComIn_GW_AEB_tips(&u8FEWarningSt);
//
//	if ((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
//	{
//		ClearWarning(GW_RADA2_CLEAR);
//	}
//	else
//	{
//		if(u8CurrentFESt != u8FEWarningSt)
//		{
//			u8LastFESt = u8CurrentFESt;
//		}
//		u8CurrentFESt = u8FEWarningSt;
//		switch(u8FEWarningSt)
//		{
//			case 7:
//				if(2 == u8LastFESt)
//				{
//					SetWarningStatus(GW_RADA2_CLEAR,UI_WARNING_EBS_OFF);
//				}
//				else
//				{
//					ClearWarning(GW_RADA2_CLEAR); 
//				}
//			break;
//			case 2:
//				if(7 == u8LastFESt)
//				{
//					SetWarningStatus(GW_RADA2_CLEAR,UI_WARNING_EBS_ON);
//				}
//				else
//				{
//					ClearWarning(GW_RADA2_CLEAR); 
//				}
//			break;			
//			case 6:
//				SetWarningStatus(GW_RADA2_CLEAR,UI_WARNING_EBS_FAULT);
//			break;
//			case 5:
//				SetWarningStatus(GW_RADA2_CLEAR,UI_WARNING_EBS_TMP_UNAVAILABLE);
//			break;
//			case 4:
//				if(1 == u8LastFESt)
//				{
//					SetWarningStatus(GW_RADA2_CLEAR,UI_WARNING_FCW_OFF);
//				}
//				else
//				{
//					ClearWarning(GW_RADA2_CLEAR);
//				}
//			break;
//			case 1:
//				if(4 == u8LastFESt)
//				{
//					SetWarningStatus(GW_RADA2_CLEAR,UI_WARNING_FCW_ON);
//				}
//				else
//				{
//					ClearWarning(GW_RADA2_CLEAR); 
//				}
//			break;			
//			case 3:
//				SetWarningStatus(GW_RADA2_CLEAR,UI_WARNING_FCW_FAULT);
//			break;
//			default:
//				ClearWarning(GW_RADA2_CLEAR);
//			break;
//		}
//	}	
// }

// static void fun_PSA2_process(void)
// {
//	uint8 u8HSAWarningSt;	
//    uint8 u8MsgNR;
//    uint8 u8MsgTout;
//    u8MsgNR = 0;
//    u8MsgTout = 0;
//	u8HSAWarningSt = 0;
//
//    Rte_Read_rpSR_CANMSG_GW_PSA2_ComIn_NR(&u8MsgNR);
//    Rte_Read_rpSR_CANMSG_GW_PSA2_ComIn_Tout(&u8MsgTout);
//	Rte_Read_rpSR_CANMSG_GW_PSA2_ComIn_ESC_HSAStatus(&u8HSAWarningSt);	
//	
//    if ((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
//    {
//        AEBWarningStatus[UI_WARNING_HAS_FAULT] = UI_WARNING_STATUS_OFF;
//    }
//    else
//    {
//		if(3 == u8HSAWarningSt)
//		{
//			AEBWarningStatus[UI_WARNING_HAS_FAULT] = UI_WARNING_STATUS_ON;
//		}
//		else
//		{
//			AEBWarningStatus[UI_WARNING_HAS_FAULT] = UI_WARNING_STATUS_OFF;
//		}		
//    }
// }

// static void SetWarningStatus(uint8 clearNo_U8,uint8 index)
// {
//	int i = 0 ;
//	int operationMin,operationMax;
//	switch(clearNo_U8)
//	{
//		case GW_CAMERA_CLEAR:
//			operationMin = UI_WARNING_LANE_KEEPING_SYS_OFF;
//			operationMax = UI_WARNING_LANE_KEEPING_SYS_ON;
//			break;
//		case GW_RADA2_CLEAR:
//			operationMin = UI_WARNING_EBS_OFF;
//			operationMax = UI_WARNING_FCW_ON;
//			break;
//		case GW_PSA2_CLEAR:
//			return;
//		default:
//			return;
//	}
//
//	for(i = operationMin; i <= operationMax; i++)
//	{
//		if(i == index )
//		{
//			AEBWarningStatus[i] = UI_WARNING_STATUS_ON;
//		}
//		else
//		{
//			AEBWarningStatus[i] = UI_WARNING_STATUS_OFF;
//		}
//	}
// }


