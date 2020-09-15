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
#ifndef WARNIGN_CTRL_H
#define WARNIGN_CTRL_H
#include "system.h"

#define UI_NO_ERROR                 0
#define UI_U8_INVALID_VALUE         0xFF
#define UI_U16_INVALID_VALUE        0xFFFF
#define UI_U32_INVALID_VALUE        0xFFFFFFFF

#define  UI_WARNING_STATUS_OFF                  ((uint8)(0))
#define  UI_WARNING_STATUS_ON                   ((uint8)(1))
#define  UI_WARNING_CONDITION_FORCED              ((uint8)(2))
#define  UI_WARNING_STATUS_RETRIG                   ((uint8)(3))
//#define  UI_WARNING_STATUS_UNKNOW              ((uint8)(2))

#define UI_WARNING_EVENT_NONE                 ((uint8)(0))
#define UI_WARNING_EVENT_RETRIGGER            ((uint8)(1))
#define UI_WARNING_EVENT_FORCED               ((uint8)(2))


/*Vehicle configuration define*/
#define EOL_ENABLE_ADAS1  (uint8)0x1
#define EOL_ENABLE_ADAS2  (uint8)0x2
#define EOL_ENABLE_ABS    (uint8)0x1
#define EOL_ENABLE_CC     (uint8)0x1
#define EOL_ENABLE_ACC    (uint8)0x2
#define EOL_ENABLE_IMMO   (uint8)0x1
#define EOL_ENABLE_PEPS   (uint8)0x2
#define EOL_ENABLE_AMT    (uint8)0x2
#define EOL_ENABLE_5MT    (uint8)0x0
#define EOL_ENABLE_6MT    (uint8)0x1
#define EOL_ENABLE_DESIL  (uint8)0x0
#define EOL_ENABLE_GAS    (uint8)0x1
#define EOL_ENABLE_AIRBAG (uint8)0x1
#define EOL_ENABLE_ESP    (uint8)0x1
#define EOL_ENABLE_SCR    (uint8)0x5
#define EOL_ENABLE_HANDBRAKE  (uint8)0x0
#define EOL_ENABLE_ELECBRAKE  (uint8)0x1
typedef struct _UI_WARNING_DATA_T
{
    uint8 status;         /**< current warning status */
    uint8 event;          /**< warning event */
    uint16 payload;       /**< payload, 0xFFFF invalid */
} UI_WARNING_DATA_T, *UI_PTR_WARNING_DATA_T;

typedef enum//there r other 2 arrays to maintenace in hmi_wrn.c
{
  WARN_DIAGNOSIS_ID                                                                ,
  WARN_RESET_TRIPMETER_WARNING_ID                                                  ,
  WARN_RESET_TRIPMETER_COMPLETE_WARNING_ID                                         ,
  WARN_ADAS_KEEP_DIS_LEVEL1_ID                                                     ,
  WARN_ADAS_KEEP_DIS_LEVEL2_ID                                                     ,
  WARN_ADAS_TAKEOVER_ID                                                            ,
  WARN_ADAS_HANDON_ID                                                              ,
  WARN_ADAS_BACK_TO_REMIND_ID                                                      ,
  WARN_CAR_IN_FACTORY_MODE_ID                                                      ,
  WARN_FACTORY_MODE_PUASE_ID                                                       ,
  WARN_TRANSPATION_MODE_ID                                                         ,
  WARN_TRANSPATION_MODE_PAUSE_ID                                                   ,
  WARN_LEFT_LIGHT_ON_WARNING_ID                                                    ,
  WARN_BATTERY_MANAGEMENT_ID                                                       ,
  WARN_DOOR_AJAR_IG_ON_ID                                                          ,
  WARN_DOOR_AJAR_IG_OFF_ID                                                         ,
  WARN_CHOOSE_FLAT_ROAD_PARKING_ID                                                 ,
  WARN_BRK_LIGHT_LMP_FAULT_WARNING_ID                                              ,
  WARN_TURN_IND_FAULT_WARNING_ID                                                   ,
  WARN_RAER_FOG_LMP_FAULT_WARNING_ID                                               ,
  WARN_POS_LMP_FAULT_WARNING_ID                                                    ,
  WARN_REVERSE_LIGHT_FAULT_WARNING_ID                                              ,
  WARN_DAY_RUN_LMP_FAULT_WARNING_ID                                                ,
  WARN_DRIVER_SEATBELT_WARNING_ID                                                  ,
  WARN_AIRBAG_FAULT_ID                                                             ,
  WARN_CDP_FAILURE_ID                                                              ,
  WARN_AUTOHOLD_FAILURE_ID                                                         ,
  WARN_CLOSE_DOOR_OR_SEATBELT_AVH_ID                                               ,
  WARN_LOW_FUEL_WARNING_ID                                                         ,
  WARN_HIGH_COOLANT_TEMPERATURE_ID                                                 ,
  WARN_LOW_OIL_PRESSURE_ID                                                         ,
  WARN_PARK_BRAKE_WARNING_ID                                                       ,
  WARN_DDD_ID                                                                      ,
  WARN_EBD_FAULT_ID                                                                ,
  WARN_ABS_FAULT_ID                                                                ,
  WARN_ESP_FAULT_ID                                                                ,
  WARN_LOW_BRAKE_FLUID_ID                                                          ,
  WARN_SVS_WARNING_ID                                                              ,
  WARN_TCU_GEARBOX_FAULT_ID                                                        ,
  WARN_TCU_BRAKE_APPLY_ID                                                          ,
  WARN_TCU_BRAKE_AND_DECELERATION_ID                                               ,
  WARN_TCU_VELOCITY_TOO_HIGH_ID                                                    ,
  WARN_TCU_WRONG_GEAR_ID                                                           ,
  WARN_TCU_UNLOCK_TO_CHANGE_GEAR_ID                                                ,
  WARN_TCU_UNLOCK_AND_BRAKE_TO_CHANGE_GEAR_ID                                      ,
  WARN_ADAS_BSD_ERROR_ID                                                           ,
  WARN_ADAS_BSD_BLOCK_ID                                                           ,
  WARN_ADAS_BSD_NOT_CAL_ID                                                         ,
  WARN_ADAS_BSD_OUT_ERR_ID                                                         ,
  WARN_ADAS_LDW_FAIL_ID                                                            ,
  WARN_ADAS_CAM_BLOCK_ID                                                           ,
  WARN_ADAS_CAM_NOT_CAL_ID                                                         ,
  WARN_SCR_UREA_DISQUALIFICATION_REF_USER_MANUAL_ID                                ,
  WARN_SCR_UREA_SYSTEM_FAILURE_REF_USER_MANUAL_ID                                  ,
  WARN_SCR_UREA_EXHAUSTED_REF_USER_MANUAL_ID                                       ,
  WARN_SCR_UREA_DISQUALIFICATION_SPEED_LIMIT_ID                                    ,
  WARN_SCR_UREA_SYSTEM_FAILURE_SPEED_LIMIT_ID                                      ,
  WARN_SCR_UREA_EXHAUSTED_SPEED_LIMIT_ID                                           ,
  WARN_SCR_UREA_DISQUALIFICATION_POWER_TO_REDUCE_ID                                ,
  WARN_SCR_UREA_SYSTEM_FAILURE_POWER_TO_REDUCE_ID                                  ,
  WARN_SCR_UREA_EXHAUSTED_POWER_TO_REDUCE_ID                                       ,
  WARN_SCR_LOW_UREA_IMMEDIATELY_ADDED_ID                                           ,
  WARN_ADAS_ACC_ON_ID                                                              ,
  WARN_ADAS_ACC_OFF_ID                                                             ,
  WARN_ADAS_ACC_CANCEL_ID                                                          ,
  WARN_ADAS_ACC_ACTIVE_ID                                                          ,
  WARN_ADAS_MRR_BLINDNESS_ID                                                       ,
  WARN_ADAS_ACC_PEBS_ERROR_ID                                                      ,
  WARN_ADAS_EPB_ACTIVATE_ID                                                        ,
  WARN_ADAS_NO_FORWARD_GEAR_ID                                                     ,
  WARN_ADAS_SEATBELT_UNBUCKLED_ID                                                  ,
  WARN_ADAS_ESP_OFF_ID                                                             ,
  WARN_ADAS_SPEED_OVER_ID                                                          ,
  WARN_ADAS_DOOR_OPEN_ID                                                           ,
  WARN_ADAS_OVERRIDE_ID                                                            ,
  WARN_ADAS_ESP_ERROR_ID                                                           ,
  WARN_ADAS_MRR_UNCALIBRATED_ID                                                    ,
  WARN_CRUISE_CONTROL_FAULT_ID                                                     ,
  WARN_CRUISE_CONTROL_ACTIVE_ID                                                    ,
  WARN_PEPS_NO_KEY_FOUND_WARNING_ID                                                ,
  WARN_PEPS_KEY_OUT_WARNING_ID                                                     ,
  WARN_PEPS_NOT_N_WARNING_ID                                                       ,
  WARN_PEPS_BRAKE_INDICATION_WARNING_ID                                            ,
  WARN_PEPS_BRAKE_INDICATION_WARNING_ACTION_ID									   ,
  WARN_PEPS_CLUTCH_INDICATION_WARNING_ID                                           ,
  WARN_PEPS_CLUTCH_INDICATION_WARNING_ACTION_ID                                    ,
  WARN_PEPS_KEY_BATTERY_WARNING_ID                                                 ,
  WARN_PEPS_ESCL_FAILSTATUS_ID                                                     ,
  WARN_PEPS_ESCL_FAILSTATUS_ACTION_ID											   ,
  WARN_PEPS_SSB_FAILSTATUS_ID                                                      ,
  WARN_PEPS_SSB_FAILSTATUS_ACTION_ID											   ,
  WARN_PEPS_IMMO_MATCH_STATUS_ID                                                   ,
  WARN_PRESS_BRAKE_PEDAL_ID                                                        ,
  WARN_CLOSE_DOOR_OR_SEATBELT_EPB_ID                                               ,
  WARN_STEP_CLTCH_TO_BUTTON_ID                                                     ,
  WARN_IMMO_KEY_BATTERY_LOW_ID                                                     ,
  WARN_BCM_IMMO_MATCH_STATUS_ID                                                    ,
  WARN_MAINTAIN_IMMEDIATELY_WARNING_ID                                             ,
  WARN_REMAIN_MAINTENANCE_WARNING_ID                                               ,
  WARN_GLOW_DISPLAYING_ID                                                          ,
  WARN_DPF_OVERLOAD_ID                                                             ,
  WARN_DPF_REACHED_LIMIT_ID                                                        ,
  WARN_DPF_MORE_THAN_LIMIT_ID                                                      ,
  WARN_DPF_CLEAN_ID                                                                ,
  WARN_DPF_CLEANING_INTERRUPTED_ID                                                 ,
  WARN_OVER_SPEED_ID                                                               ,
  WARN_NOT_IN_P_A_IG_ON_ID                                                         ,
  WARN_NOT_IN_P_B_IG_ON_ID                                                         ,
  WARN_AUTOMATIC_EMERGENCY_BRAKING_ID                                              ,
  WARN_PASSENGER_SEATBELT_WARNING_ID                                               ,
  WARN_GPF_FULL_WARNING_ID                                                         ,
  WARN_GPF_LIMIT_WARNING_ID                                                        ,
  WARN_HDC_UNAVIABLE_ID                                                            ,
  WARN_GEARBOX_INFO_MATCH_ERROR_ID                                                 ,
  WARN_GEARBOX_SYSTEM_SERIOUS_FAILURE_ID                                           ,
  WARN_GEARBOX_SYSTEM_LIMITED_ID                                                   ,
  WARN_GEARBOX_SYSTEM_FAILURE_ID                                                   ,
  WARN_SWITCH_GEAR_PRESS_THE_BRAKE_PEDAL_ID                                        ,
  WARN_SWITCH_GEAR_PLEASE_SLOW_DOWN_ID                                             ,
  WARN_DRIVE_CAREFULLY_ID                                                          ,
  WARN_LOW_BEAM_FAULT_WARNING_ID                                                   ,
  WARN_FRNT_FOG_LMP_FAULT_WARNING_ID                                               ,
  WARN_MAX_ID                                  
} WARN_ID_INDEX;


/** 
 * @name Warning "application specific" chime definitions
 */
//@{
typedef enum
{
  UI_WARNING_CHIME_NONE                  =   0,
  UI_WARNING_CHIME_ACTIVE                =   1
} UI_WARNING_CHIME_T;
//@}

/** 
 * @name Warning "application specific" telltale definitions
 */
//@{
typedef enum
{
  UI_WARNING_TELLTALE_NONE               =   0,
  UI_WARNING_TELLTALE_AMBER              =   1,
  UI_WARNING_TELLTALE_RED                =   2
} UI_WARNING_TELLTALE_T;
//@}

/** 
 * @name Warning's Display States
 */
//@{
typedef enum
{
  UI_WARNING_STATE_NONE                  =   0,  /**< none state */
  UI_WARNING_STATE_ACKNOWLEDED           =   1,  /**< warning has been acknowledged */
  UI_WARNING_STATE_TIMEOUT               =   2,  /**< warning has timed-out */
  UI_WARNING_STATE_HIDDEN                =   3,  /**< warning has been hidden by another */
  UI_WARNING_STATE_SHOW                  =   4,  /**< warning has been displayed */
  UI_WARNING_STATE_ACTIVE                =   5,  /**< warning has been activated by warnings manager */
  UI_WARNING_STATE_INACTIVE              =   6,   /**< warning has been de-activated by warnings manager */
  UI_WARNING_STATE_INTERRUPT             =   7
} UI_WARNING_STATE_T;


/*************************************************************************//**
 * \details     This function notifies the status/state of the warning based on 
 *              the changes in warnings manager and it also notifies 
 *              associated telltale and chime
 *
 * \param[in]   warning_id - UI_WARNING_ID_T system warning unique id
 *
 * \param[in]   warning_state - UI_WARNING_STATE_T warning display state
 *
 * \param[in]   chime_id - UI_WARNING_CHIME_T chime id
 *
 * \param[in]   telltale_id - UI_WARNING_TELLTALE_T telltale id
 *
 * \retval      None
 *
 ****************************************************************************/
void CWarningCtrl_ui_set_warning_status (WARN_ID_INDEX  warning_id,    UI_WARNING_STATE_T warning_status,    UI_WARNING_CHIME_T chime_id,   UI_WARNING_TELLTALE_T telltale_id);
/****************************************************************************
Function Name     : di_get_notified_warning_status

Description       : This function will return the warning status notified by HMI.
                    
Invocation        : Invoked by application as and when needed.

Parameters        : warning_id --> Warning id, whose status want to know

Return Type       : warning_status_U8 --> status of the warning "warning_id"
				    NONE          (0)   
           *        ACKNOWLEDGED  (BIT0)
           *        TIMEOUT       (BIT1)
           *        HIDDEN        (BIT2)
           *        SHOW          (BIT3)
           *        ACTIVE        (BIT4)
           *        INACTIVE      (BIT5)

Critical Section  : None.

******************************************************************************/
extern  uint8 CWarningCtrl_APP_get_notified_warning_status (WARN_ID_INDEX warning_id_to_get);
extern  uint16 CWarningCtrl_APP_get_MainTenace_warning_XXXKM(void);
extern 	void CWarningCtrl_APP_set_MainTenace_warning_XXXKM (uint16 u16MainTenaceKM);

#endif

