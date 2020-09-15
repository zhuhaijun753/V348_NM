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
#include "Rte_AccWarning.h"
#include "CmpLib.h"
#include "WarningCtrl.h"
#include <string.h>

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(AccWarning)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define ACC_WRN_LANE_ASSIT_FAULT				(5u)
#define ACC_WRN_LANE_ASSIT_CAMERA_BLOCKED		(6u)
#define ACC_WRN_LANE_ASSIT_CAMERA_UNCALIBRATED	(7u)

#define ACC_WRN_SOD_ERROR			(1u)
#define ACC_WRN_SOD_BLINDNESS		(2u)
#define ACC_WRN_SOD_UNCALIBRATION	(3u)
#define ACC_WRN_BSD_TEMP_ERROR		(4u)
#define ACC_WRN_REQ_LEFT_RIGHT      (2U)

#define ACC_WRN_PCW_LATENT_WARNING	(1u)
#define ACC_WRN_PCW_PRE_WARNING		(1u)
#define ACC_WRN_FCW_PRE_WARNING     (1u)

#define DDD_WRN_FIRST_ISSUED        (1U)
#define DDD_WRN_INVALID             (3U)

#define	FUNCTION_CALLED_PERIOD	(100u) //ms
#define ACC_WRN_TIME_3SEC		((3000  + FUNCTION_CALLED_PERIOD - 1) / FUNCTION_CALLED_PERIOD)
#define ACC_WRN_TIME_500MS  	((500   + FUNCTION_CALLED_PERIOD - 1) / FUNCTION_CALLED_PERIOD)

#define HOLD_TIME  (30)

typedef enum
{
    EM_ACC_WRN_HANDS_ON_REQ = 0,
    EM_ACC_WRN_LDW_FAILURE,
    EM_ACC_WRN_CAM_BLOCK_ID,
    EM_ACC_WRN_CAM_NOT_CAL_ID,
    EM_ACC_WRN_AEB,
    EM_ACC_WRN_MRR_BLINDNESS_ID,
    EM_ACC_WRN_ACC_PEBS_ERROR_ID,
    EM_ACC_WRN_ESP_ERROR_ID,
    EM_ACC_WRN_MRR_UNCALIBRATED_ID,
    EM_ACC_WRN_MAX
} EM_ACC_WRN_CTRL_IDX;

typedef enum
{
    AEB_UNAVAILABE,
    AEB_OFF,
    AEB_STANDBY,
    AEB_ACTIVE_NO_INTERVENTION,
    AEB_ACTIVE,
    AEB_RESERVED
} ST_AEB_STATE_T;

typedef struct
{
    uint8 u8_count;
    uint8 u8_timeOutFlg;
    // uint8 u8_reqClrFlg;
} ST_ACC_WRN_CTRL_INFO;

typedef struct
{
    //GW_MRR_0x245
    uint8 IPM_LaneAssit_HandsonReq; //1 wrn
    uint8 IPM_LaneAssit_Status; //3 wrns

    //GW_MRR_0x246
    uint8 PCW_preWarning; // 0.5 wrn
    uint8 PCW_latentWarning; // 0.5 wrn
    uint8 TakeOverReq; //1 wrn
    uint8 AEB_STATE;
    uint8 ACC_Textinfo;

    //GW_IPM_0x275
    uint8 SOD_BLIS_display; //4 wrns
} ST_ACC_WRN_SIG_INFO;

typedef enum
{
    OFF_MODE = 0,
    PASSIVE_MODE,
    STAND_BY_MODE,
    ACTIVE_CONTROL_MODE,
    BRAKE_ONLY_MODE,
    OVERRIDE,
    STANDSTILL,
    FAILURE_MODE
} ACC_HIMODE_T;

typedef enum
{
    NO_TAKEOVER_REQUEST = 0,
    VALID_TAKEOVER_REQUEST = 1
} TAKEOVERREQ_T;

typedef enum
{
    ACC_ON = 1,
    ACC_OFF,
    ACC_CANCEL,
    ACC_ACTIVE,
    MRR_BLINDNESS,
    ACC_PEBS_ERROR,
    EPB_ACTIVATE,
    NO_FORWARD_GEAR,
    SEATBELT_UNBUCKLED,
    ESP_OFF,
    SPEED_OVER,
    DOOR_OPEN,
    OVER_RIDE,
    ESP_ERROR,
    MRR_UNCALIBRATED
} ACC_TextInfo;

typedef enum
{
    BACK_REMIND_NONE,
    BACK_REMIND_LEFT,
    BACK_REMIND_RIGHT,
    BACK_REMIND_BOTH
} BSD_BACK_REMIND;

static ST_ACC_WRN_CTRL_INFO stAccWrnCtrlInfoTbl[EM_ACC_WRN_MAX] = {0};
static uint8 fl_OverVoltageTout = 0;
static uint8 HoldTimer1 = 0;
static uint8 HoldTimer2 = 0;
static Boolean KDLevel1 = FALSE;
static Boolean KDLevel2 = FALSE;
//static fl_IPM_HandsonReq = 0;

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
/*static void f_AccWrnMdl_init(void);
static void f_AccWrnMdl_proc(void);

static void f_AccWrnMdl_time_ctrl_init(void);

static void f_AccWrnMdl_0x245_init(void);
static void f_AccWrnMdl_0x246_init(void);
static void f_AccWrnMdl_0x275_init(void);

static void f_AccWrnMdl_0x245_proc(void);
static void f_AccWrnMdl_0x246_proc(void);
static void f_AccWrnMdl_0x275_proc(void);
*/
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
    //f_AccWrnMdl_init();
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
    //f_AccWrnMdl_init();
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
    //f_AccWrnMdl_init();
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
    //f_AccWrnMdl_init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
/****************************************************************************
Function Name     	: CmpActive

Description         : This function process the oilpressure TT ON/OFF for
            ISG /NON ISG Variant.

Invocation          : called in cmpActive.

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static Std_ReturnType CmpActive(void)
{
#if 0 
    uint8 fl_ign_state_U8 = eIGN_OFF;
    uint8 fl_batt_state_U8 = eBatteryState_Normal;

    Rte_Read_rpBattState_BatteryState(&fl_batt_state_U8);
    Rte_Read_rpIgnState_IGNState(&fl_ign_state_U8);

    if (fl_batt_state_U8 != eBatteryState_Normal)
    {
        memset(stAccWrnCtrlInfoTbl, 0, sizeof(stAccWrnCtrlInfoTbl));
        HoldTimer2 = 0;
        HoldTimer1 = 0;
    }
    else
    {
        switch (fl_ign_state_U8)
        {
            case eIGN_OFF:
                f_AccWrnMdl_init();
                break;
            case eIGN_RUN:
                f_AccWrnMdl_proc();
                break;
            default:
                break;
        }
    }

    //Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_QDashACCFail(0);
#endif
	Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_QDashACCFail(0);

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
#if 0
/****************************************************************************
Function Name        : f_AccWrnMdl_init
Description          : None
Invocation           : None
Return Value         : None
Critical Section     : None
Parameters           : None
******************************************************************************/
static void f_AccWrnMdl_init(void)
{
    f_AccWrnMdl_time_ctrl_init();
    f_AccWrnMdl_0x245_init();
    f_AccWrnMdl_0x246_init();
    f_AccWrnMdl_0x275_init();
    HoldTimer1 = 0;
    HoldTimer2 = 0;
    KDLevel1 = FALSE;
    KDLevel2 = FALSE;
}

/****************************************************************************
Function Name        : f_AccWrnMdl_proc
Description          : None
Invocation           : None
Return Value         : None
Critical Section     : None
Parameters           : None
******************************************************************************/
static void f_AccWrnMdl_proc(void)
{
    f_AccWrnMdl_0x245_proc();
    f_AccWrnMdl_0x246_proc();
    f_AccWrnMdl_0x275_proc();
}

/****************************************************************************
Function Name        : f_AccWrnMdl_time_ctrl_init
Description          : None
Invocation           : None
Return Value         : None
Critical Section     : None
Parameters           : None
******************************************************************************/
static void f_AccWrnMdl_time_ctrl_init(void)
{
    memset(stAccWrnCtrlInfoTbl, 0, sizeof(stAccWrnCtrlInfoTbl));
}


/****************************************************************************
Function Name        : f_AccWrnMdl_0x245_init
Description          : None
Invocation           : None
Return Value         : None
Critical Section     : None
Parameters           : None
******************************************************************************/
static void f_AccWrnMdl_0x245_init(void)
{
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_LDW_FAIL_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_HANDON_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_CAM_BLOCK_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_CAM_NOT_CAL_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DDD_ID, UI_WARNING_STATUS_OFF);
    //memset(&stAccWrnCtrlInfoTbl[0], 0, sizeof(stAccWrnCtrlInfoTbl[0]));
    //memset(&stAccWrnCtrlInfoTbl[1], 0, sizeof(stAccWrnCtrlInfoTbl[1]));
    //memset(&stAccWrnCtrlInfoTbl[2], 0, sizeof(stAccWrnCtrlInfoTbl[2]));
    //memset(&stAccWrnCtrlInfoTbl[3], 0, sizeof(stAccWrnCtrlInfoTbl[3]));
    //fl_IPM_HandsonReq = FALSE;
}

/****************************************************************************
Function Name        : f_AccWrnMdl_0x245_proc
Description          : None
Invocation           : None
Return Value         : None
Critical Section     : None
Parameters           : None
******************************************************************************/
static void f_AccWrnMdl_0x245_proc(void)
{
    uint8 	fl_CAN_nvr_status_U8 = 0;
    uint8 	fl_CAN_msg_status_U8 = 0;

    //boolean fl_vehicle_cfg_ldw_flg = FALSE;

    uint8 	fl_lane_status_U8 = 0;
    uint8   fl_IPM_LaneAssit_HandsonReq_U8 = 0;
    uint8   fl_IPM_DDD_Warning_Level_U8    = 0;
    uint8   fl_IPM_DDD_IndexStatus_U8      = 0;

    //EM_ACC_WRN_CTRL_IDX  WrnIndex = EM_ACC_WRN_HANDS_ON_REQ;
    //ST_ACC_WRN_CTRL_INFO WrnTempData[4];

    //memset(WrnTempData, 0, sizeof(WrnTempData));
    Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_NR(&fl_CAN_nvr_status_U8);
    Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_Tout(&fl_CAN_msg_status_U8);

    //Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_LDW, &fl_vehicle_cfg_ldw_flg);
    //fl_vehicle_cfg_ldw_flg = (boolean)TRUE;
    //if ((boolean)FALSE != fl_vehicle_cfg_ldw_flg)
    {
        Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status(&fl_lane_status_U8);
        Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_HandsonReq(&fl_IPM_LaneAssit_HandsonReq_U8);
        Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_IndexStatus(&fl_IPM_DDD_IndexStatus_U8);
        Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_WarningLevel(&fl_IPM_DDD_Warning_Level_U8);

        if (((uint8)RTE_E_NEVER_RECEIVED == fl_CAN_nvr_status_U8) || ((uint8)RTE_E_TIMEOUT == fl_CAN_msg_status_U8))
        {
            f_AccWrnMdl_0x245_init();
            /*clear timer when losing signal*/
            //memcpy(stAccWrnCtrlInfoTbl, WrnTempData, sizeof(WrnTempData));
        }
        else
        {
            /*LDW  CAMERA*/
            switch (fl_lane_status_U8)
            {
                //5
                case ACC_WRN_LANE_ASSIT_FAULT:
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_LDW_FAIL_ID, UI_WARNING_STATUS_ON);
                    //stAccWrnCtrlInfoTbl[EM_ACC_WRN_LDW_FAILURE].u8_count = ACC_WRN_TIME_3SEC;
                    break;
                //6
                case ACC_WRN_LANE_ASSIT_CAMERA_BLOCKED:
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_CAM_BLOCK_ID, UI_WARNING_STATUS_ON);
                    //stAccWrnCtrlInfoTbl[EM_ACC_WRN_CAM_BLOCK_ID].u8_count = ACC_WRN_TIME_3SEC;
                    break;
                //7
                case ACC_WRN_LANE_ASSIT_CAMERA_UNCALIBRATED:
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_CAM_NOT_CAL_ID, UI_WARNING_STATUS_ON);
                    //stAccWrnCtrlInfoTbl[EM_ACC_WRN_CAM_NOT_CAL_ID].u8_count = ACC_WRN_TIME_3SEC;
                    break;
                default:
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_LDW_FAIL_ID, UI_WARNING_STATUS_OFF);
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_CAM_BLOCK_ID, UI_WARNING_STATUS_OFF);
                    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_CAM_NOT_CAL_ID, UI_WARNING_STATUS_OFF);
                    break;
            }

            /*for (WrnIndex = EM_ACC_WRN_HANDS_ON_REQ; WrnIndex < EM_ACC_WRN_AEB; WrnIndex++)
            {
                if (stAccWrnCtrlInfoTbl[WrnIndex].u8_count > 0)
                {
                    stAccWrnCtrlInfoTbl[WrnIndex].u8_count--;
                    if (stAccWrnCtrlInfoTbl[WrnIndex].u8_count == 0)
                    {
                        stAccWrnCtrlInfoTbl[WrnIndex].u8_timeOutFlg = TRUE;
                    }
                }
            }*/

            // LDW
            if ((uint8)ACC_WRN_LANE_ASSIT_FAULT != fl_lane_status_U8 /*&&
                    TRUE == stAccWrnCtrlInfoTbl[EM_ACC_WRN_LDW_FAILURE].u8_timeOutFlg*/)
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_LDW_FAIL_ID, UI_WARNING_STATUS_OFF);
                //stAccWrnCtrlInfoTbl[EM_ACC_WRN_LDW_FAILURE].u8_timeOutFlg = FALSE;
            }
            // CAM_BLOCK
            if ((uint8)ACC_WRN_LANE_ASSIT_CAMERA_BLOCKED != fl_lane_status_U8 /*&&
                    TRUE == stAccWrnCtrlInfoTbl[EM_ACC_WRN_CAM_BLOCK_ID].u8_timeOutFlg*/)
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_CAM_BLOCK_ID, UI_WARNING_STATUS_OFF);
                //stAccWrnCtrlInfoTbl[EM_ACC_WRN_CAM_BLOCK_ID].u8_timeOutFlg = FALSE;
            }
            //CAM_NOTCAL
            if ((uint8)ACC_WRN_LANE_ASSIT_CAMERA_UNCALIBRATED != fl_lane_status_U8 /*&&
                    TRUE == stAccWrnCtrlInfoTbl[EM_ACC_WRN_CAM_NOT_CAL_ID].u8_timeOutFlg*/)
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_CAM_NOT_CAL_ID, UI_WARNING_STATUS_OFF);
                //stAccWrnCtrlInfoTbl[EM_ACC_WRN_CAM_NOT_CAL_ID].u8_timeOutFlg = FALSE;
            }

            // HAND ON
            if ((boolean)FALSE != fl_IPM_LaneAssit_HandsonReq_U8)
            {
                //fl_IPM_HandsonReq = TRUE;
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_HANDON_ID, UI_WARNING_STATUS_ON);
                //stAccWrnCtrlInfoTbl[EM_ACC_WRN_HANDS_ON_REQ].u8_count = ACC_WRN_TIME_3SEC;
            }

            if ((boolean)FALSE == fl_IPM_LaneAssit_HandsonReq_U8 /*&& TRUE ==
                stAccWrnCtrlInfoTbl[EM_ACC_WRN_HANDS_ON_REQ].u8_timeOutFlg*/)
            {
                //fl_IPM_HandsonReq = FALSE;
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_HANDON_ID, UI_WARNING_STATUS_OFF);
                //stAccWrnCtrlInfoTbl[EM_ACC_WRN_HANDS_ON_REQ].u8_timeOutFlg = FALSE;
            }

            //Rte_Write_pp_AccWarning_GdtCtrl_Value_IPM_HandsonReq(fl_IPM_HandsonReq);


            //DDD
            if (((uint8)DDD_WRN_FIRST_ISSUED == fl_IPM_DDD_Warning_Level_U8) && \
                    ((uint8)DDD_WRN_INVALID != fl_IPM_DDD_IndexStatus_U8))
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DDD_ID, UI_WARNING_STATUS_ON);
            }
            else
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DDD_ID, UI_WARNING_STATUS_OFF);
            }
        }
    }
}

/****************************************************************************
Function Name        : f_AccWrnMdl_0x246_init
Description          : None
Invocation           : None
Return Value         : None
Critical Section     : None
Parameters           : None
******************************************************************************/
static void f_AccWrnMdl_0x246_init(void)
{
    //HoldTimer1 = 0;
    //HoldTimer2 = 0;
    //KDLevel1 = FALSE;
    //KDLevel2 = FALSE;
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AUTOMATIC_EMERGENCY_BRAKING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_KEEP_DIS_LEVEL1_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_KEEP_DIS_LEVEL2_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_TAKEOVER_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_ON_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_OFF_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_CANCEL_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_ACTIVE_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_BLINDNESS_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_PEBS_ERROR_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_EPB_ACTIVATE_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_NO_FORWARD_GEAR_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_SEATBELT_UNBUCKLED_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ESP_OFF_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_SPEED_OVER_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_DOOR_OPEN_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_OVERRIDE_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ESP_ERROR_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_UNCALIBRATED_ID, UI_WARNING_STATUS_OFF);
}

/****************************************************************************
Function Name        : f_AccWrnMdl_0x246_proc
Description          : None
Invocation           : None
Return Value         : None
Critical Section     : None
Parameters           : None
******************************************************************************/
static void f_AccWrnMdl_0x246_proc(void)
{
    uint8           fl_CAN_nvr_status_U8 = 0;
    uint8           fl_CAN_msg_status_U8 = 0;
    //boolean         fl_vehicle_cfg_acc_flg = FALSE;
    //boolean         fl_vehicle_cfg_fcw_flg = FALSE;
    //boolean         fl_vehicle_cfg_aeb_flg = FALSE;
    uint8           fl_MRR_TakeOverReq_U8 = 0;
    //uint8           fl_PCW_latentWarning_U8 = 0;
    //uint8           fl_vehicle_cfg_ldw_flg = 0;
    uint8           fl_AEB_STATE = 0;
    uint8           fl_PCW_preWarning_U8 = 0;
    uint8           fl_FCW_preWarning_U8 = 0;
    uint8           fl_MRR_ACCHMI_Mode_U8 = 0;
    uint8           fl_ACC_Textinfo = 0;
    EM_ACC_WRN_CTRL_IDX  AccIndex = EM_ACC_WRN_AEB;
    ST_ACC_WRN_CTRL_INFO AccTempData[5];

    memset(AccTempData, 0, sizeof(AccTempData));

    Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR(&fl_CAN_nvr_status_U8);
    Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout(&fl_CAN_msg_status_U8);

    //    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ACC, &fl_vehicle_cfg_acc_flg);
    //    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_FCW, &fl_vehicle_cfg_fcw_flg);
    //    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AEB, &fl_vehicle_cfg_aeb_flg);
    //    Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_LDW, &fl_vehicle_cfg_ldw_flg);

    /*ACC*/
    //    if (TRUE == fl_vehicle_cfg_acc_flg)
    {
        Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TakeOverReq(&fl_MRR_TakeOverReq_U8);
        Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode(&fl_MRR_ACCHMI_Mode_U8);
    }

    /*FCW + PCW*/
    //    if (TRUE == fl_vehicle_cfg_fcw_flg)
    {
        Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_preWarning(&fl_FCW_preWarning_U8);
        Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_preWarning(&fl_PCW_preWarning_U8);
    }

    /*AEB*/
    //    if (TRUE == fl_vehicle_cfg_aeb_flg)
    {
        Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_AEB_STATE(&fl_AEB_STATE);
    }

    Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Textinfo(&fl_ACC_Textinfo);

    if (((uint8)RTE_E_NEVER_RECEIVED == fl_CAN_nvr_status_U8) || ((uint8)RTE_E_TIMEOUT == fl_CAN_msg_status_U8))
    {
        f_AccWrnMdl_0x246_init();
        //memcpy(stAccWrnCtrlInfoTbl, AccTempData, sizeof(AccTempData));
    }
    else
    {
        //AEB
        if (AEB_ACTIVE == fl_AEB_STATE)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AUTOMATIC_EMERGENCY_BRAKING_ID, UI_WARNING_STATUS_ON);
            //stAccWrnCtrlInfoTbl[EM_ACC_WRN_AEB].u8_count = ACC_WRN_TIME_3SEC;
        }

        if (AEB_ACTIVE != fl_AEB_STATE && TRUE == stAccWrnCtrlInfoTbl[EM_ACC_WRN_AEB].u8_timeOutFlg)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_AUTOMATIC_EMERGENCY_BRAKING_ID, UI_WARNING_STATUS_OFF);
            //stAccWrnCtrlInfoTbl[EM_ACC_WRN_HANDS_ON_REQ].u8_timeOutFlg = FALSE;
        }

        /* FCW + PCW*/
        if ((uint8)ACC_WRN_PCW_PRE_WARNING == fl_PCW_preWarning_U8)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_KEEP_DIS_LEVEL2_ID, UI_WARNING_STATUS_ON);
            //KDLevel2 = TRUE;
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_KEEP_DIS_LEVEL2_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)ACC_WRN_FCW_PRE_WARNING == fl_FCW_preWarning_U8)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_KEEP_DIS_LEVEL1_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_KEEP_DIS_LEVEL1_ID, UI_WARNING_STATUS_OFF);
        }
#if 0
        //KDLevel2 (keep safe distance warning in level 2)
        // priority Level1 < Level2 ,so leve2 process logic need stay before level1
        if (TRUE == KDLevel2)
        {
            HoldTimer2++;
        }
        else
        {
            HoldTimer2  = 0;
        }

        //3s Hold time
        if (HoldTimer2 >= HOLD_TIME)
        {
            HoldTimer2 = HOLD_TIME;
            if (ACC_WRN_PCW_PRE_WARNING != fl_PCW_preWarning_U8)
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_KEEP_DIS_LEVEL2_ID, UI_WARNING_STATUS_OFF);
                HoldTimer2 = 0;
                KDLevel2 = FALSE;
            }
            else
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_KEEP_DIS_LEVEL2_ID, UI_WARNING_STATUS_ON);
            }
        }

        // priority Level1 < Level2 ,only show leve2 once both warning are triggerred
        if (KDLevel2 != UI_WARNING_STATUS_ON)
        {
            if (ACC_WRN_PCW_LATENT_WARNING == fl_PCW_latentWarning_U8)
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_KEEP_DIS_LEVEL1_ID, UI_WARNING_STATUS_ON);
                KDLevel1 = TRUE;
            }
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_KEEP_DIS_LEVEL1_ID, UI_WARNING_STATUS_OFF);
            HoldTimer1 = 0;
            KDLevel1 = FALSE;
        }

        if (TRUE == KDLevel1)
        {
            HoldTimer1++;
        }
        else
        {
            HoldTimer1 = 0;
        }

        if (HoldTimer1 >= HOLD_TIME)
        {
            HoldTimer1 = HOLD_TIME;
            if (ACC_WRN_PCW_LATENT_WARNING != fl_PCW_latentWarning_U8)
            {
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_KEEP_DIS_LEVEL1_ID, UI_WARNING_STATUS_OFF);
                HoldTimer1 = 0;
                KDLevel1 = FALSE;
            }
        }
#endif
        /*ACC*/
        if (((uint8)VALID_TAKEOVER_REQUEST == fl_MRR_TakeOverReq_U8) && \
                ((uint8)FAILURE_MODE != fl_MRR_ACCHMI_Mode_U8) && \
                ((uint8)OFF_MODE != fl_MRR_ACCHMI_Mode_U8))
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_TAKEOVER_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_TAKEOVER_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)ACC_ON == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_ON_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_ON_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)ACC_OFF == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_OFF_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_OFF_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)ACC_CANCEL == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_CANCEL_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_CANCEL_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)ACC_ACTIVE == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_ACTIVE_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_ACTIVE_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)EPB_ACTIVATE == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_EPB_ACTIVATE_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_EPB_ACTIVATE_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)NO_FORWARD_GEAR == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_NO_FORWARD_GEAR_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_NO_FORWARD_GEAR_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)SEATBELT_UNBUCKLED == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_SEATBELT_UNBUCKLED_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_SEATBELT_UNBUCKLED_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)ESP_OFF == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ESP_OFF_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ESP_OFF_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)SPEED_OVER == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_SPEED_OVER_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_SPEED_OVER_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)DOOR_OPEN == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_DOOR_OPEN_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_DOOR_OPEN_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)OVER_RIDE == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_OVERRIDE_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_OVERRIDE_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)MRR_BLINDNESS == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_BLINDNESS_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_BLINDNESS_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)ACC_PEBS_ERROR == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_PEBS_ERROR_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_PEBS_ERROR_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)ESP_ERROR == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ESP_ERROR_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ESP_ERROR_ID, UI_WARNING_STATUS_OFF);
        }

        if ((uint8)MRR_UNCALIBRATED == fl_ACC_Textinfo)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_UNCALIBRATED_ID, UI_WARNING_STATUS_ON);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_UNCALIBRATED_ID, UI_WARNING_STATUS_OFF);
        } 

#if 0
        switch (fl_ACC_Textinfo)
        {
            case ACC_ON:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_ON_ID, UI_WARNING_STATUS_ON);
                break;

            case ACC_OFF:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_OFF_ID, UI_WARNING_STATUS_ON);
                break;

            case ACC_CANCEL:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_CANCEL_ID, UI_WARNING_STATUS_ON);
                break;

            case ACC_ACTIVE:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_ACTIVE_ID, UI_WARNING_STATUS_ON);
                break;

            case EPB_ACTIVATE:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_EPB_ACTIVATE_ID, UI_WARNING_STATUS_ON);
                break;

            case NO_FORWARD_GEAR:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_NO_FORWARD_GEAR_ID, UI_WARNING_STATUS_ON);
                break;

            case SEATBELT_UNBUCKLED:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_SEATBELT_UNBUCKLED_ID, UI_WARNING_STATUS_ON);
                break;

            case ESP_OFF:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ESP_OFF_ID, UI_WARNING_STATUS_ON);
                break;

            case SPEED_OVER:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_SPEED_OVER_ID, UI_WARNING_STATUS_ON);
                break;

            case DOOR_OPEN:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_DOOR_OPEN_ID, UI_WARNING_STATUS_ON);
                break;

            case OVER_RIDE:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_OVERRIDE_ID, UI_WARNING_STATUS_ON);
                break;

            case MRR_BLINDNESS:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_BLINDNESS_ID, UI_WARNING_STATUS_ON);
                //stAccWrnCtrlInfoTbl[EM_ACC_WRN_MRR_BLINDNESS_ID].u8_count = ACC_WRN_TIME_3SEC;
                break;

            case ACC_PEBS_ERROR:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_PEBS_ERROR_ID, UI_WARNING_STATUS_ON);
                //stAccWrnCtrlInfoTbl[EM_ACC_WRN_ACC_PEBS_ERROR_ID].u8_count = ACC_WRN_TIME_3SEC;
                break;

            case ESP_ERROR:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ESP_ERROR_ID, UI_WARNING_STATUS_ON);
                //stAccWrnCtrlInfoTbl[EM_ACC_WRN_ESP_ERROR_ID].u8_count = ACC_WRN_TIME_3SEC;
                break;

            case MRR_UNCALIBRATED:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_UNCALIBRATED_ID, UI_WARNING_STATUS_ON);
                //stAccWrnCtrlInfoTbl[EM_ACC_WRN_MRR_UNCALIBRATED_ID].u8_count = ACC_WRN_TIME_3SEC;
                break;

            default:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_ON_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_OFF_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_CANCEL_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_ACTIVE_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_BLINDNESS_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_PEBS_ERROR_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_EPB_ACTIVATE_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_NO_FORWARD_GEAR_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_SEATBELT_UNBUCKLED_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ESP_OFF_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_SPEED_OVER_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_DOOR_OPEN_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_OVERRIDE_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ESP_ERROR_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_UNCALIBRATED_ID, UI_WARNING_STATUS_OFF);
                break;
        }
        
        if (MRR_BLINDNESS != fl_ACC_Textinfo && TRUE == stAccWrnCtrlInfoTbl[EM_ACC_WRN_MRR_BLINDNESS_ID].u8_timeOutFlg)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_BLINDNESS_ID, UI_WARNING_STATUS_OFF);
            stAccWrnCtrlInfoTbl[EM_ACC_WRN_MRR_BLINDNESS_ID].u8_timeOutFlg = FALSE;
        }

        if (ACC_PEBS_ERROR != fl_ACC_Textinfo && TRUE == stAccWrnCtrlInfoTbl[EM_ACC_WRN_ACC_PEBS_ERROR_ID].u8_timeOutFlg)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ACC_PEBS_ERROR_ID, UI_WARNING_STATUS_OFF);
            stAccWrnCtrlInfoTbl[EM_ACC_WRN_ACC_PEBS_ERROR_ID].u8_timeOutFlg = FALSE;
        }

        if (ESP_ERROR != fl_ACC_Textinfo && TRUE == stAccWrnCtrlInfoTbl[EM_ACC_WRN_ESP_ERROR_ID].u8_timeOutFlg)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_ESP_ERROR_ID, UI_WARNING_STATUS_OFF);
            stAccWrnCtrlInfoTbl[EM_ACC_WRN_ESP_ERROR_ID].u8_timeOutFlg = FALSE;
        }

        if (MRR_UNCALIBRATED != fl_ACC_Textinfo && TRUE == stAccWrnCtrlInfoTbl[EM_ACC_WRN_MRR_UNCALIBRATED_ID].u8_timeOutFlg)
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_MRR_UNCALIBRATED_ID, UI_WARNING_STATUS_OFF);
            stAccWrnCtrlInfoTbl[EM_ACC_WRN_MRR_UNCALIBRATED_ID].u8_timeOutFlg = FALSE;
        }

        for (AccIndex = EM_ACC_WRN_AEB; AccIndex < EM_ACC_WRN_MAX; AccIndex++)
        {
            //u8_count > 0 means  warning is triggerred need hold 3s
            if (stAccWrnCtrlInfoTbl[AccIndex].u8_count > 0)
            {
                stAccWrnCtrlInfoTbl[AccIndex].u8_count--;
                if (0 == stAccWrnCtrlInfoTbl[AccIndex].u8_count)
                {
                    stAccWrnCtrlInfoTbl[AccIndex].u8_timeOutFlg = TRUE;
                }
            }
        }
#endif
    }
}

/****************************************************************************
Function Name        : f_AccWrnMdl_0x275_init
Description          : None
Invocation           : None
Return Value         : None
Critical Section     : None
Parameters           : None
******************************************************************************/
static void f_AccWrnMdl_0x275_init(void)
{
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_ERROR_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_BLOCK_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_NOT_CAL_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_OUT_ERR_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BACK_TO_REMIND_ID, UI_WARNING_STATUS_OFF);
}

/****************************************************************************
Function Name        : f_AccWrnMdl_0x275_proc
Description          : None
Invocation           : None
Return Value         : None
Critical Section     : None
Parameters           : None
******************************************************************************/
static void f_AccWrnMdl_0x275_proc(void)
{
    uint8           fl_CAN_nvr_status_U8 = 0;
    uint8           fl_CAN_msg_status_U8 = 0;
    uint8           fl_SOD_BLIS_display = 0;
    uint8           fl_SOD_CTA_wrnReqLeft = 0;
    uint8           fl_SOD_CTA_wrnReqRight = 0;
    uint8           fl_SOD_CTA_wrnReqStaus = 0;
    //boolean         fl_vehicle_cfg_bsd_flg = FALSE;

    Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_NR(&fl_CAN_nvr_status_U8);
    Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_Tout(&fl_CAN_msg_status_U8);
    //Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_BSD, &fl_vehicle_cfg_bsd_flg);

    //if (TRUE == fl_vehicle_cfg_bsd_flg)
    {
        Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_BLIS_display(&fl_SOD_BLIS_display);
        Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqLeft(&fl_SOD_CTA_wrnReqLeft);
        Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqRight(&fl_SOD_CTA_wrnReqRight);
    }

    if (((uint8)RTE_E_NEVER_RECEIVED == fl_CAN_nvr_status_U8) || ((uint8)RTE_E_TIMEOUT == fl_CAN_msg_status_U8))
    {
        f_AccWrnMdl_0x275_init();
    }
    else
    {
        switch (fl_SOD_BLIS_display)
        {
            case ACC_WRN_SOD_ERROR:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_ERROR_ID, UI_WARNING_STATUS_ON);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_BLOCK_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_NOT_CAL_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_OUT_ERR_ID, UI_WARNING_STATUS_OFF);
                break;

            case ACC_WRN_SOD_BLINDNESS:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_ERROR_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_BLOCK_ID, UI_WARNING_STATUS_ON);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_NOT_CAL_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_OUT_ERR_ID, UI_WARNING_STATUS_OFF);
                break;

            case ACC_WRN_SOD_UNCALIBRATION:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_ERROR_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_BLOCK_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_NOT_CAL_ID, UI_WARNING_STATUS_ON);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_OUT_ERR_ID, UI_WARNING_STATUS_OFF);
                break;

            case ACC_WRN_BSD_TEMP_ERROR:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_ERROR_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_BLOCK_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_NOT_CAL_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_OUT_ERR_ID, UI_WARNING_STATUS_ON);
                break;

            default:
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_ERROR_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_BLOCK_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_NOT_CAL_ID, UI_WARNING_STATUS_OFF);
                Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BSD_OUT_ERR_ID, UI_WARNING_STATUS_OFF);
                break;
        }

        if (((uint8)ACC_WRN_REQ_LEFT_RIGHT == fl_SOD_CTA_wrnReqLeft) && \
                ((uint8)ACC_WRN_REQ_LEFT_RIGHT != fl_SOD_CTA_wrnReqRight))
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BACK_TO_REMIND_ID, UI_WARNING_STATUS_ON);
            fl_SOD_CTA_wrnReqStaus = BACK_REMIND_LEFT;
        }
        else if (((uint8)ACC_WRN_REQ_LEFT_RIGHT != fl_SOD_CTA_wrnReqLeft) && \
                 ((uint8)ACC_WRN_REQ_LEFT_RIGHT == fl_SOD_CTA_wrnReqRight))
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BACK_TO_REMIND_ID, UI_WARNING_STATUS_ON);
            fl_SOD_CTA_wrnReqStaus = BACK_REMIND_RIGHT;
        }
        else if (((uint8)ACC_WRN_REQ_LEFT_RIGHT == fl_SOD_CTA_wrnReqLeft) && \
                 ((uint8)ACC_WRN_REQ_LEFT_RIGHT == fl_SOD_CTA_wrnReqRight))
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BACK_TO_REMIND_ID, UI_WARNING_STATUS_ON);
            fl_SOD_CTA_wrnReqStaus = BACK_REMIND_BOTH;
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_ADAS_BACK_TO_REMIND_ID, UI_WARNING_STATUS_OFF);
            fl_SOD_CTA_wrnReqStaus = BACK_REMIND_NONE;
        }

        Rte_Write_ppSR_AccWarning_GdtCtrl_State_BackRemindSts(fl_SOD_CTA_wrnReqStaus);
    }
}
#endif

