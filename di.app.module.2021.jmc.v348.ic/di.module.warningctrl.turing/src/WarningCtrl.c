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
#include "Rte.h"
#include "Rte_type.h"
#include "Rte_WarningCtrl.h"
#include "CmpLib.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(WarningCtrl)


//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
//static boolean l_ui_warning_display_status_array[WARN_MAX_ID];
UI_WARNING_DATA_T l_ui_warnings_status_array[WARN_MAX_ID] = {0};
static UINT8    l_notified_warning_status_array[WARN_MAX_ID] = {0};
static	uint16 l_MainTenaceKM;

static void ClearAllWarningSt(void);
static boolean CWrnCtrl_IsFunCfg(UINT8 u8Id);
void CWarningCtrl_Set_Warning(UInt8 WarningID ,UInt8 Status);


//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	ClearAllWarningSt();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
	ClearAllWarningSt();
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
	ClearAllWarningSt();
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
	ClearAllWarningSt();
    return  E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive(void)
{
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
/// @brief  Transitional state to clean up after diagnostic state. Component behavior to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}


static void ClearAllWarningSt(void)
{
	uint32 index = 0;
	for(index = 0; index < WARN_MAX_ID; index++)
	{
	
		l_ui_warnings_status_array[index].status = UI_WARNING_STATUS_OFF;
		l_ui_warnings_status_array[index].event = UI_WARNING_EVENT_NONE;
		l_ui_warnings_status_array[index].payload = UI_U16_INVALID_VALUE;
		//l_ui_warning_display_status_array[index] = UI_WARNING_STATUS_OFF;

	}
}

void CWarningCtrl_Set_Warning(UInt8 WarningID, UInt8 Status)
{
    boolean bIsCfg = FALSE;
    UINT8 Test_status = 0;
    UINT8 IGN_status = 0;

    bIsCfg = CWrnCtrl_IsFunCfg(WarningID);

    Rte_Read_rpSR_BarView_Self_Test_status(&Test_status);
    Rte_Read_rpIgnState_IGNState(&IGN_status);

    if (FALSE != bIsCfg)
    {
        if (WarningID < WARN_MAX_ID)
        {
            if (IGN_status == eIGN_RUN)
            {
                if (Test_status == 1) 
                {
                    if ((UI_WARNING_STATUS_OFF == Status) || (UI_WARNING_STATUS_ON == Status) ||
                            (UI_WARNING_CONDITION_FORCED == Status) || (UI_WARNING_STATUS_RETRIG == Status))
                    {
                        l_ui_warnings_status_array[WarningID].status = Status;
                        l_ui_warnings_status_array[WarningID].event = UI_WARNING_EVENT_NONE;
                        l_ui_warnings_status_array[WarningID].payload = UI_U16_INVALID_VALUE;
                    }
                    else
                    {
                        //Invalid,do nothing
                    }
                }
                else
                {
                    /*Warning is not displayed during the precheck*/
                    l_ui_warnings_status_array[WarningID].status = UI_WARNING_STATUS_OFF;
                    l_ui_warnings_status_array[WarningID].event = UI_WARNING_EVENT_NONE;
                    l_ui_warnings_status_array[WarningID].payload = UI_U16_INVALID_VALUE;
                }
            }
            else
            {
                if ((UI_WARNING_STATUS_OFF == Status) || (UI_WARNING_STATUS_ON == Status) ||
                        (UI_WARNING_CONDITION_FORCED == Status) || (UI_WARNING_STATUS_RETRIG == Status))
                {
                    l_ui_warnings_status_array[WarningID].status = Status;
                    l_ui_warnings_status_array[WarningID].event = UI_WARNING_EVENT_NONE;
                    l_ui_warnings_status_array[WarningID].payload = UI_U16_INVALID_VALUE;
                }
            }
        }
        else
        {
            //Invalid,do nothing
        }
    }
    else
    {
        //Not Cfg,do nothing
    }

}



void CWarningCtrl_Set_Warning_With_Event(UInt8 WarningID, UInt8 Status, UInt8 WarningEventStatus)
{
    if (WarningID < WARN_MAX_ID)
    {
        if ((UI_WARNING_STATUS_OFF == Status) || (UI_WARNING_STATUS_ON == Status)/*||(UI_WARNING_STATUS_RETRIG == Status )*/)
        {
            l_ui_warnings_status_array[WarningID].status = Status;
        }
        else
        {
            l_ui_warnings_status_array[WarningID].status = UI_WARNING_STATUS_OFF;
        }

        if (UI_WARNING_EVENT_FORCED >= WarningEventStatus)
        {
            l_ui_warnings_status_array[WarningID].event = WarningEventStatus;
        }
        else
        {
            l_ui_warnings_status_array[WarningID].event = UI_WARNING_EVENT_NONE;
        }

        l_ui_warnings_status_array[WarningID].payload = UI_U16_INVALID_VALUE;


    }
    else
    {
        //Do nothing
    }
}

void CWarningCtrl_UI_Get_Warning_status(UInt8 WarningID, UInt8 *Status)
{
    UInt8 u8WarningSt = 0;
    if (UI_WARNING_EVENT_NONE != l_ui_warnings_status_array[WarningID].event)
    {
        u8WarningSt = l_ui_warnings_status_array[WarningID].event;

    }
    else
    {
        u8WarningSt = l_ui_warnings_status_array[WarningID].status;
    }
    *Status = u8WarningSt;

}

/****************************************************************************
Function Name     : ui_set_warning_status

Description       : This function will give the warning status

Invocation        : Invoked by HMI application.

Parameters        : warning_id --> Warning id passed by HMI
                    warning_status_U8 --> status of the warning "warning_id"
                      0 -- none state
                      1 -- warning has been acknowledged
                      2 -- warning has timed-out
                      3 -- warning has been hidden by another
                      4 -- warning has been displayed
                      5 -- warning has been activated by warnings manager
                      6 -- warning has been de-activated by warnings manager

Return Type       : None.

Critical Section  : None.

******************************************************************************/

void CWarningCtrl_ui_set_warning_status(WARN_ID_INDEX  warning_id, UI_WARNING_STATE_T warning_status,  UI_WARNING_CHIME_T chime_id, UI_WARNING_TELLTALE_T telltale_id)
{
//    if((l_notified_warning_status_array[warning_id] != UI_WARNING_STATE_ACKNOWLEDED) &&
//        (warning_status == UI_WARNING_STATE_ACKNOWLEDED))
//    {
//      if(c_warning_detail_U16A[warning_id].warnackcallback != NULL)
//      {
//        c_warning_detail_U16A[warning_id].warnackcallback(warning_id);
//      }
//    }
    l_notified_warning_status_array[warning_id] = warning_status;
    l_ui_warnings_status_array[warning_id].event = UI_WARNING_EVENT_NONE;
}

/****************************************************************************
Function Name     : di_get_notified_warning_status

Description       : This function will return the warning status notified by HMI.
                    
Invocation        : Invoked by application as and when needed.

Parameters        : warning_id --> Warning id, whose status want to know

Return Type       : warning_status_U8 --> status of the warning "warning_id"
                      0 -- none state
                      1 -- warning has been acknowledged
                      2 -- warning has timed-out
                      3 -- warning has been hidden by another
                      4 -- warning has been displayed
                      5 -- warning has been activated by warnings manager
                      6 -- warning has been de-activated by warnings manager

Critical Section  : None.

******************************************************************************/
UINT8 CWarningCtrl_APP_get_notified_warning_status(WARN_ID_INDEX  warning_id_to_get)
{
    return (l_notified_warning_status_array[warning_id_to_get]);
}

static boolean CWrnCtrl_IsFunCfg(UINT8 u8Id)
{
    boolean bRet          = FALSE;
    uint8 bPEPSCfg        = 0;              /*00:no 01:IMMO 10:PEPS 11:no      */
    uint8 bADASCfg        = 0;              /*0=no;0001 = adas1;0010=adas2	   */
    uint8 bABSCfg         = 0;              /*0:without ABS 1:with ABS         */
    uint8 bEngineCfg      = 0;              /*0:Desil 1:Gas                    */
    uint8 bCruiseCfg      = 0;              /*00:without 01:cc 10:ACC          */
    uint8 bAirBagCfg      = 0;              /*0:without Airbag 1:with Airbag   */
    uint8 bESPCfg         = 0;              /*0:without ESP 1:with ESP         */
    uint8 bBrakeTypeCfg   = 0;              /*00:Hand Brake  01:Elec Brake     */
    uint8 bAT_MTCfg       = 0;              /*00:5MT 01:6MT 10:AMT             */
    uint8 bEmissionLvlCfg = 0;              /*101:six 100:five                 */
    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Engine, &bEngineCfg);
    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AT_MT, &bAT_MTCfg);
    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ABS, &bABSCfg);
    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Cruise, &bCruiseCfg);
    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ADAS, &bADASCfg);
    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AIRBAG, &bAirBagCfg);
    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ESP, &bESPCfg);
    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_EMISSION_LEVELS, &bEmissionLvlCfg);
    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_BRAKE_TYPE, &bBrakeTypeCfg);
    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_PEPS, &bPEPSCfg);
    
    switch (u8Id)
    {
        case WARN_DPF_CLEANING_INTERRUPTED_ID:
        case WARN_DPF_CLEAN_ID:
        case WARN_DPF_MORE_THAN_LIMIT_ID:
        case WARN_DPF_OVERLOAD_ID:
        case WARN_DPF_REACHED_LIMIT_ID:
            if (EOL_ENABLE_DESIL == bEngineCfg)
            {
                bRet = TRUE;                              //Desil
            }
            else
            {
                bRet = FALSE;
            }
            break;
        case WARN_BCM_IMMO_MATCH_STATUS_ID:
        case WARN_IMMO_KEY_BATTERY_LOW_ID:
            if (EOL_ENABLE_IMMO == bPEPSCfg)
            {
                bRet = TRUE;                              //IMMO
            }
            else
            {
                bRet = FALSE;
            }
            break;
        case WARN_PEPS_ESCL_FAILSTATUS_ID:
		case WARN_PEPS_ESCL_FAILSTATUS_ACTION_ID:
        case WARN_PEPS_IMMO_MATCH_STATUS_ID:
        case WARN_PEPS_NO_KEY_FOUND_WARNING_ID:
        case WARN_PEPS_KEY_OUT_WARNING_ID:
        case WARN_PEPS_KEY_BATTERY_WARNING_ID:
        case WARN_PEPS_NOT_N_WARNING_ID:
        case WARN_PEPS_SSB_FAILSTATUS_ID:
		case WARN_PEPS_SSB_FAILSTATUS_ACTION_ID:
            if (EOL_ENABLE_PEPS == bPEPSCfg)
            {
                bRet = TRUE;                              //PEPS
            }
            else
            {
                bRet = FALSE;                             
            }
            break;
        case WARN_PEPS_CLUTCH_INDICATION_WARNING_ID:
		case WARN_PEPS_CLUTCH_INDICATION_WARNING_ACTION_ID:
            if ((EOL_ENABLE_PEPS == bPEPSCfg) && ((EOL_ENABLE_5MT == bAT_MTCfg) || (EOL_ENABLE_6MT == bAT_MTCfg)))
            {
                bRet = TRUE;                              //PEPS&&MT
            }
            else
            {
                bRet = FALSE;                             
            }
            break;
        case WARN_PEPS_BRAKE_INDICATION_WARNING_ID:
		case WARN_PEPS_BRAKE_INDICATION_WARNING_ACTION_ID:
            if ((EOL_ENABLE_PEPS == bPEPSCfg) && (EOL_ENABLE_AMT == bAT_MTCfg))
            {
                bRet = TRUE;                              //PEPS && AT
            }
            else
            {
                bRet = FALSE;
            }
            break;
        case WARN_ADAS_LDW_FAIL_ID:
        case WARN_ADAS_CAM_BLOCK_ID:
        case WARN_ADAS_CAM_NOT_CAL_ID:
        case WARN_ADAS_HANDON_ID:
        case WARN_ADAS_BSD_ERROR_ID:
        case WARN_ADAS_BSD_BLOCK_ID:
        case WARN_ADAS_BSD_NOT_CAL_ID:
        case WARN_ADAS_BSD_OUT_ERR_ID:
        case WARN_ADAS_BACK_TO_REMIND_ID:
        case WARN_DDD_ID:
        case WARN_ADAS_KEEP_DIS_LEVEL1_ID:
        case WARN_ADAS_KEEP_DIS_LEVEL2_ID:
            if ((EOL_ENABLE_ADAS1 == bADASCfg) || (EOL_ENABLE_ADAS2 == bADASCfg))
            {
                bRet = TRUE;                     //ADAS1 or ADAS2
            }
            else
            {
                bRet = FALSE;
            }
            break;

        case WARN_ADAS_TAKEOVER_ID:
        case WARN_ADAS_ACC_ACTIVE_ID:
        case WARN_ADAS_ACC_CANCEL_ID:
        case WARN_ADAS_ACC_OFF_ID:
        case WARN_ADAS_ACC_ON_ID:
        case WARN_ADAS_MRR_BLINDNESS_ID:
        case WARN_ADAS_ACC_PEBS_ERROR_ID:
        case WARN_ADAS_EPB_ACTIVATE_ID:
        case WARN_ADAS_NO_FORWARD_GEAR_ID:
        case WARN_ADAS_SEATBELT_UNBUCKLED_ID:
        case WARN_ADAS_ESP_OFF_ID:
        case WARN_ADAS_SPEED_OVER_ID:
        case WARN_ADAS_DOOR_OPEN_ID:
        case WARN_ADAS_OVERRIDE_ID:
        case WARN_ADAS_ESP_ERROR_ID:
        case WARN_ADAS_MRR_UNCALIBRATED_ID:
            if ((EOL_ENABLE_ADAS2 == bADASCfg) || (EOL_ENABLE_ACC == bCruiseCfg))
            {
                bRet = TRUE;                        //ADAS2 or ACC
            }
            else
            {
                bRet = FALSE;
            }
            break;

        case WARN_ABS_FAULT_ID:
		case WARN_EBD_FAULT_ID:
            if ((EOL_ENABLE_ABS == bABSCfg) || (EOL_ENABLE_ESP == bESPCfg))
            {
                bRet = TRUE;                         //ABS or ESP
            }
            else
            {
                bRet = FALSE;
            }
            break;

        case WARN_CRUISE_CONTROL_ACTIVE_ID:
        case WARN_CRUISE_CONTROL_FAULT_ID:
            if (EOL_ENABLE_CC == bCruiseCfg)
            {
                bRet = TRUE;                        //CC
            }
            else
            {
                bRet = FALSE;
            }
            break;
        case WARN_STEP_CLTCH_TO_BUTTON_ID:
            if (((EOL_ENABLE_5MT == bAT_MTCfg) || (EOL_ENABLE_6MT == bAT_MTCfg)) && (EOL_ENABLE_PEPS != bPEPSCfg))
            {
                bRet = TRUE;                       //MT&Non PEPS
            }
            else
            {
                bRet = FALSE;
            }
            break;
        case WARN_TCU_BRAKE_AND_DECELERATION_ID:
        case WARN_TCU_BRAKE_APPLY_ID:
        case WARN_TCU_GEARBOX_FAULT_ID:
        case WARN_TCU_UNLOCK_AND_BRAKE_TO_CHANGE_GEAR_ID:
        case WARN_TCU_UNLOCK_TO_CHANGE_GEAR_ID:
        case WARN_TCU_VELOCITY_TOO_HIGH_ID:
        case WARN_TCU_WRONG_GEAR_ID:
            if (EOL_ENABLE_AMT == bAT_MTCfg)
            {
                bRet = TRUE;                      //AT
            }
            else
            {
                bRet = FALSE;
            }
            break;
        case WARN_AIRBAG_FAULT_ID:
            if (EOL_ENABLE_AIRBAG == bAirBagCfg)
            {
                bRet = TRUE;                      //AirBag
            }
            else
            {
                bRet = FALSE;
            }
            break;
        case WARN_ESP_FAULT_ID:
            if (EOL_ENABLE_ESP == bESPCfg)
            {
                bRet = TRUE;                      //ESP
            }
            else
            {
                bRet = FALSE;
            }
            break;
        case WARN_SCR_LOW_UREA_IMMEDIATELY_ADDED_ID:
        case WARN_SCR_UREA_DISQUALIFICATION_POWER_TO_REDUCE_ID:
        case WARN_SCR_UREA_DISQUALIFICATION_REF_USER_MANUAL_ID:
        case WARN_SCR_UREA_DISQUALIFICATION_SPEED_LIMIT_ID:
        case WARN_SCR_UREA_EXHAUSTED_POWER_TO_REDUCE_ID:
        case WARN_SCR_UREA_EXHAUSTED_REF_USER_MANUAL_ID:
        case WARN_SCR_UREA_EXHAUSTED_SPEED_LIMIT_ID:
        case WARN_SCR_UREA_SYSTEM_FAILURE_POWER_TO_REDUCE_ID:
        case WARN_SCR_UREA_SYSTEM_FAILURE_REF_USER_MANUAL_ID:
        case WARN_SCR_UREA_SYSTEM_FAILURE_SPEED_LIMIT_ID:
            if (EOL_ENABLE_SCR == bEmissionLvlCfg)
            {
                bRet = TRUE;                     //Lvl 6 ,enable SCR
            }
            else
            {
                bRet = FALSE;
            }
            break;
        case WARN_PARK_BRAKE_WARNING_ID:
            if (EOL_ENABLE_HANDBRAKE == bBrakeTypeCfg)
            {
                bRet = TRUE;                    //Hand brake
            }
            else
            {
                bRet = FALSE;
            }
            break;
        case WARN_PRESS_BRAKE_PEDAL_ID:
        case WARN_CLOSE_DOOR_OR_SEATBELT_EPB_ID:
        case WARN_CHOOSE_FLAT_ROAD_PARKING_ID:
        case WARN_CLOSE_DOOR_OR_SEATBELT_AVH_ID:
        case WARN_AUTOHOLD_FAILURE_ID:
        case WARN_CDP_FAILURE_ID:
            if (EOL_ENABLE_ELECBRAKE == bBrakeTypeCfg)
            {
                bRet = TRUE;                     //Elec brake
            }
            else
            {
                bRet = FALSE;
            }
            break;
        default:
            bRet = TRUE;
            break;
    }
    return bRet;
}



uint16 CWarningCtrl_APP_get_MainTenace_warning_XXXKM(void)
{
	return l_MainTenaceKM;
}

void CWarningCtrl_APP_set_MainTenace_warning_XXXKM(uint16 u16MainTenaceKM)
{
	l_MainTenaceKM = u16MainTenaceKM;
}



