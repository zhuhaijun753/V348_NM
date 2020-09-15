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

#include "Rte_LowOilPressureWrnMdl.h"
#include "CmpLib.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(LowOilPressureWrnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8 l_LowOil_Inlalid_TimeCnt_U8 = 0;
static uint8 l_LowOil_Inlalid_Status = TRUE;

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */
#define cLOWOIL_ENGSPEED_INVALID               (uint16)(32767)
#define cLOWOIL_ENGSPEED_300RPM                (uint16)(600)  /*Resolution:0.5,600->300rpm */
#define cLOWOIL_ENGSPEED_200RPM                (uint16)(400)  /*Resolution:0.5,200rpm */
#define cLOWOIL_HOLDTIME_2S                	   (uint8)(2000 / 100)    /*3s timer for 100ms period */
#define cLOWOIL_HOLDTIME_3S                	   (uint8)(3000 / 100)    /*3s timer for 100ms period */
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================


//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8   l_LowOilHoldTimeCnt_U8 = 0;
static boolean l_LowOilWrnActiveFlg_B = FALSE;
static void fun_LowOilPressure_Warning_Process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
    l_LowOilHoldTimeCnt_U8 = (uint8)0;
    l_LowOilWrnActiveFlg_B = (boolean)FALSE;
	l_LowOil_Inlalid_Status = TRUE;
	l_LowOil_Inlalid_TimeCnt_U8 = 0;
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_OIL_PRESSURE_ID,UI_WARNING_STATUS_OFF);
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
    l_LowOilHoldTimeCnt_U8 = (uint8)0;
    l_LowOilWrnActiveFlg_B = (boolean)FALSE;
	l_LowOil_Inlalid_Status = TRUE;
	l_LowOil_Inlalid_TimeCnt_U8 = 0;
	Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_OIL_PRESSURE_ID,UI_WARNING_STATUS_OFF);
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
    l_LowOilHoldTimeCnt_U8 = (uint8)0;
    l_LowOilWrnActiveFlg_B = (boolean)FALSE;
	l_LowOil_Inlalid_Status = TRUE;
	l_LowOil_Inlalid_TimeCnt_U8 = 0;
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_OIL_PRESSURE_ID,UI_WARNING_STATUS_OFF);
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
    fun_LowOilPressure_Warning_Process();
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


static void fun_LowOilPressure_Warning_Process(void)
{
    uint16  fl_EMS_EngSpeed_U16             = (uint8)0;
    boolean fl_LowOilLvl_B                  = (boolean)FALSE;

    uint8   fl_Msg10B_NR_U8                 = (uint8)0;
    uint8   fl_Msg10B_Tout_U8               = (uint8)0;

    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&fl_Msg10B_NR_U8);
    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&fl_Msg10B_Tout_U8);
    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&fl_EMS_EngSpeed_U16);
    Rte_Call_rpCS_pDI_TIIoHwAb_DIn_Impl_GetPin(eIOPinInId_MDI_OIL_PRESSURE_SW,&fl_LowOilLvl_B);

    if ( ((uint8)RTE_E_TIMEOUT == fl_Msg10B_Tout_U8) || ((uint8)RTE_E_NEVER_RECEIVED == fl_Msg10B_NR_U8) )
    {
        l_LowOilWrnActiveFlg_B = (boolean)FALSE;
        l_LowOilHoldTimeCnt_U8 = (uint8)0;

		l_LowOil_Inlalid_Status = TRUE;
		l_LowOil_Inlalid_TimeCnt_U8 = 0;
		
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_OIL_PRESSURE_ID,UI_WARNING_STATUS_OFF);
    }
    else
    {
		if ((cLOWOIL_ENGSPEED_300RPM < fl_EMS_EngSpeed_U16) && ((boolean)FALSE == fl_LowOilLvl_B))
		{
			if(TRUE == l_LowOil_Inlalid_Status)
			{
				l_LowOilHoldTimeCnt_U8 ++;
				if (cLOWOIL_HOLDTIME_3S < l_LowOilHoldTimeCnt_U8)
				{
					if ((boolean)FALSE == l_LowOilWrnActiveFlg_B)
					{
						l_LowOilWrnActiveFlg_B = (boolean)TRUE;
						Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_OIL_PRESSURE_ID,UI_WARNING_STATUS_ON);
					}
				}
			}

			if(cLOWOIL_ENGSPEED_INVALID == fl_EMS_EngSpeed_U16)
			{
				if(l_LowOil_Inlalid_TimeCnt_U8 > cLOWOIL_HOLDTIME_2S)
				{
					l_LowOil_Inlalid_Status = FALSE;
					Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_OIL_PRESSURE_ID,UI_WARNING_STATUS_OFF);
				}
				else
				{
					l_LowOil_Inlalid_TimeCnt_U8 ++;
				}
			}
			else
			{
				l_LowOil_Inlalid_Status = TRUE;
				l_LowOil_Inlalid_TimeCnt_U8 = 0;
			}
			
		}
		else if ( (cLOWOIL_ENGSPEED_200RPM > fl_EMS_EngSpeed_U16) || ((boolean)FALSE != fl_LowOilLvl_B) )
		{
			l_LowOilWrnActiveFlg_B = (boolean)FALSE;
			l_LowOilHoldTimeCnt_U8 = (uint8)0;

			l_LowOil_Inlalid_Status = TRUE;
			l_LowOil_Inlalid_TimeCnt_U8 = 0;
			Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_OIL_PRESSURE_ID,UI_WARNING_STATUS_OFF);
		}
		else
		{
		
		}
        
    }

}

