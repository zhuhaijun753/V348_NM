/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  Copyright (c) 2006-2015 by Vector Informatik GmbH.                                             All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *          File:  Rte_OsApplication.c
 *        Config:  Turing.dpa
 *   ECU-Project:  IC
 *
 *     Generator:  MICROSAR RTE Generator Version 4.8.0
 *                 RTE Core Version 1.8.0
 *       License:  Unlimited license CBD1400814 for Visteon Corporation
 *
 *   Description:  RTE implementation file
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0781, 0857 EOF */ /* MD_Rte_0777, MD_Rte_0779, MD_Rte_0781, MD_MSR_1.1_857 */

#define RTE_CORE
#include "Os.h" /* PRQA S 0828, 0883 */ /* MD_MSR_1.1_828, MD_Rte_Os */
#include "Rte_Type.h"
#include "Rte_Main.h"

#include "Rte_ABSTtMdl.h"
#include "Rte_AEBChime.h"
#include "Rte_AEBWrnMdl.h"
#include "Rte_AFCMdl.h"
#include "Rte_APAChime.h"
#include "Rte_APBTtMdl.h"
#include "Rte_ATSChime.h"
#include "Rte_AVHTtMdl.h"
#include "Rte_AccMdl.h"
#include "Rte_AccWarning.h"
#include "Rte_AdasChime.h"
#include "Rte_AirbagMdl.h"
#include "Rte_AmbientMdl.h"
#include "Rte_AppCtrl.h"
#include "Rte_BackFogMdl.h"
#include "Rte_BarView.h"
#include "Rte_BattMdl.h"
#include "Rte_BkDisChime.h"
#include "Rte_BkHomeChimeMdl.h"
#include "Rte_BswM.h"
#include "Rte_BtnMdl.h"
#include "Rte_CDD_Test.h"
#include "Rte_CDPFailureWrnMdl.h"
#include "Rte_CanEOLMdl.h"
#include "Rte_Can_Dispatcher.h"
#include "Rte_ChargeMdl.h"
#include "Rte_ChimeMdl.h"
#include "Rte_ClockMdl.h"
#include "Rte_ComM.h"
#include "Rte_ComVnimMdl.h"
#include "Rte_CoolantTmpMdl.h"
#include "Rte_CruiseCtrlWrnMdl.h"
#include "Rte_CruiseMdl.h"
#include "Rte_CruiseTtMdl.h"
#include "Rte_DPFDTEMdl.h"
#include "Rte_DPFMdl.h"
#include "Rte_DPFWrnMdl.h"
#include "Rte_Dcm.h"
#include "Rte_DcmExt.h"
#include "Rte_Dem.h"
#include "Rte_Det.h"
#include "Rte_DimDerateMdl.h"
#include "Rte_DimPwm.h"
#include "Rte_Dlt.h"
#include "Rte_DltExt.h"
#include "Rte_DoorAjarWrnMdl.h"
#include "Rte_DoorOpenChime.h"
#include "Rte_DrvStyleMdl.h"
#include "Rte_DteMdl.h"
#include "Rte_EBDTtMdl.h"
#include "Rte_EPBWrnMdl.h"
#include "Rte_ESCLTtMdl.h"
#include "Rte_ESPOffTtMdl.h"
#include "Rte_ESPTtMdl.h"
#include "Rte_ESPWrnMdl.h"
#include "Rte_EasyNaviMdl.h"
#include "Rte_EcuM.h"
#include "Rte_EcumExt.h"
#include "Rte_Eep24xx.h"
#include "Rte_EngHighTempChimeMdl.h"
#include "Rte_EngHighTempWrnMdl.h"
#include "Rte_EngineCoolantMdl.h"
#include "Rte_EpbWarnMdl.h"
#include "Rte_EpbWarning.h"
#include "Rte_EpsWarnMdl.h"
#include "Rte_FourWDFaultTtMdl.h"
#include "Rte_FourWheelDriveHTtMdl.h"
#include "Rte_FourWheelDriveLTtMdl.h"
#include "Rte_FrnRadarChime.h"
#include "Rte_FrontFogMdl.h"
#include "Rte_FuelLevelMdl.h"
#include "Rte_GLOWDisplayWrnMdl.h"
#include "Rte_GdtCdd.h"
#include "Rte_GdtCtrl.h"
#include "Rte_GearBoxTtMdl.h"
#include "Rte_GearMdl.h"
#include "Rte_GlowTtMdl.h"
#include "Rte_HdcTtMdl.h"
#include "Rte_HighBeamMdl.h"
#include "Rte_HmiAccMdl.h"
#include "Rte_HmiAdasMdl.h"
#include "Rte_IFCMdl.h"
#include "Rte_IMMOTtMdl.h"
#include "Rte_ImmoChime.h"
#include "Rte_IoHwAb.h"
#include "Rte_KeyBattLowWrnMdl.h"
#include "Rte_KeyInOnChime.h"
#include "Rte_LAP_AVCV.h"
#include "Rte_LampFaultWrnMdl.h"
#include "Rte_LeftRightTurnTtMdl.h"
#include "Rte_LightOnReminderChime.h"
#include "Rte_LowBrakeFluidWrnMdl.h"
#include "Rte_LowFuelChime.h"
#include "Rte_LowFuelMdl.h"
#include "Rte_LowOilPressureWrnMdl.h"
#include "Rte_ModMdl.h"
#include "Rte_NVMExtMdl.h"
#include "Rte_NotInPChime.h"
#include "Rte_NotInPWrnMdl.h"
#include "Rte_NvM.h"
#include "Rte_OBDTtMdl.h"
#include "Rte_OdoMdl.h"
#include "Rte_OilWaterTtMdl.h"
#include "Rte_OilpressChime.h"
#include "Rte_OilpressMdl.h"
#include "Rte_OverSpdChime.h"
#include "Rte_PEPSTtMdl.h"
#include "Rte_PEPSWrnMdl.h"
#include "Rte_ParkBrakeWrnMdl.h"
#include "Rte_PepsChime.h"
#include "Rte_PkBrkChime.h"
#include "Rte_PosLightMdl.h"
#include "Rte_PosLmpChime.h"
#include "Rte_PowerMangementWrnMdl.h"
#include "Rte_PtrBaseView.h"
#include "Rte_ResetTripWrnMdl.h"
#include "Rte_RtcCdd.h"
#include "Rte_SCRChimeMdl.h"
#include "Rte_SCRTtMdl.h"
#include "Rte_SCRWrnMdl.h"
#include "Rte_SIAMdl.h"
#include "Rte_STTChimeMdl.h"
#include "Rte_STTMdl.h"
#include "Rte_STTWrnMdl.h"
#include "Rte_SVSTtMdl.h"
#include "Rte_SVSWrnMdl.h"
#include "Rte_SeatBeltMdl.h"
#include "Rte_SeatbeltChime.h"
#include "Rte_SiaTtMdl.h"
#include "Rte_SndCdd.h"
#include "Rte_SpdMdl.h"
#include "Rte_StpCddMdl.h"
#include "Rte_StpClutchWrnMdl.h"
#include "Rte_TCUWrnMdl.h"
#include "Rte_TPMSRoutineMdl.h"
#include "Rte_TPMSWrnMdl.h"
#include "Rte_TachMdl.h"
#include "Rte_TmExt.h"
#include "Rte_TpmsChimeMdl.h"
#include "Rte_TpmsMdl.h"
#include "Rte_TripMdl.h"
#include "Rte_TtMdl.h"
#include "Rte_TtView.h"
#include "Rte_TurnIndHazardChime.h"
#include "Rte_VehicleCfgMdl.h"
#include "Rte_VehicleModeMdl.h"
#include "Rte_WarningCtrl.h"
#include "SchM_Adc.h"
#include "SchM_BswM.h"
#include "SchM_Can.h"
#include "SchM_CanIf.h"
#include "SchM_CanSM.h"
#include "SchM_CanTrcv_30_Tja1040.h"
#include "SchM_Com.h"
#include "SchM_ComM.h"
#include "SchM_Dem.h"
#include "SchM_Det.h"
#include "SchM_Dio.h"
#include "SchM_Dlt.h"
#include "SchM_Ea.h"
#include "SchM_EcuM.h"
#include "SchM_Eep_30_XXi2c01.h"
#include "SchM_Fee.h"
#include "SchM_Fls.h"
#include "SchM_Gpt.h"
#include "SchM_I2c.h"
#include "SchM_Icu.h"
#include "SchM_Mcu.h"
#include "SchM_NvM.h"
#include "SchM_PduR.h"
#include "SchM_Port.h"
#include "SchM_Pwm.h"
#include "SchM_Spi.h"
#include "SchM_Wdg.h"
#include "SchM_WdgM.h"
#include "SchM_Xcp.h"

#include "Rte_Hook.h"

#include "Com.h"

#ifndef RTE_COM_VERSION
# define RTE_COM_VERSION 40U
#endif

#if (RTE_COM_VERSION == 20U) || (RTE_COM_VERSION == 21U)
# define RTE_COM_SENDCAST(x) (x)
#else
# define RTE_COM_SENDCAST(x)
#endif

#include "Rte_Cbk.h"

#include "Xcp.h"

#include "NvM.h"

/* AUTOSAR 3.x compatibility */
#if !defined (STATIC)
# define STATIC static
#endif


/**********************************************************************************************************************
 * API for enable / disable interrupts global
 *********************************************************************************************************************/

#if defined(osDisableGlobalKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_DisableAllInterrupts() osDisableGlobalKM()   /* MICROSAR OS */
#else
# define Rte_DisableAllInterrupts() DisableAllInterrupts()   /* AUTOSAR OS */
#endif

#if defined(osEnableGlobalKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_EnableAllInterrupts() osEnableGlobalKM()   /* MICROSAR OS */
#else
# define Rte_EnableAllInterrupts() EnableAllInterrupts()   /* AUTOSAR OS */
#endif

/**********************************************************************************************************************
 * API for enable / disable interrupts up to the systemLevel
 *********************************************************************************************************************/

#if defined(osDisableLevelKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_DisableOSInterrupts() osDisableLevelKM()   /* MICROSAR OS */
#else
# define Rte_DisableOSInterrupts() SuspendOSInterrupts()   /* AUTOSAR OS */
#endif

#if defined(osEnableLevelKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_EnableOSInterrupts() osEnableLevelKM()   /* MICROSAR OS */
#else
# define Rte_EnableOSInterrupts() ResumeOSInterrupts()   /* AUTOSAR OS */
#endif


/**********************************************************************************************************************
 * Buffers for unqueued S/R
 *********************************************************************************************************************/

#define RTE_START_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(UInt32, RTE_VAR_INIT) Rte_AFCMdl_ppSR_AfcMdl_AfcVal_AccumulateVal = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_AFCMdl_ppSR_AfcMdl_AfcVal_AfcRefuelVal = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_AFCMdl_ppSR_AfcMdl_AfcVal_AfcSingleVal = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_AFC = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_AFC_Type = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_AFC_Accumulate, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_AFC_Accumulate = {
  0U, 0U, 0U, 0, 0
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_AFC_Refuel, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_AFC_Refuel = {
  0U, 0U, 0U, 0, 0
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_SingleAFC, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_SingleAFC = {
  0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_APBTtMdl_ppSR_APB_Message1_tt_state_value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_APBTtMdl_ppSR_APB_Message2_tt_state_value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_APBTtMdl_ppSR_APB_tt_state_value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_ATSChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_AVHTtMdl_ppSR_AVH_tt_state_value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_AccWarning_ppSR_AccWarning_GdtCtrl_State_BackRemindSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_QDashACCFail = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_AccWarning_pp_AccWarning_GdtCtrl_Value_IPM_HandsonReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_AdasChime_ppSR_CANMSG_IC_0x386_ComOut_IC_BSDSoundSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_IPM_LaneAssit_HandsonReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_IPM_LaneAssit_WarningAudio = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_MRR_PCW_STATE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_MRR_TakeOverReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_Nav_Sts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_NotInPChimeIgnoff = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_PEPSChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_TPMSChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_AirbagTelltaleBehavior = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_VehSpd_HMI = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16_01, RTE_VAR_INIT) Rte_AmbientMdl_ppSR_AmbientMdl_Output_OutsideTempValue = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16_01, RTE_VAR_INIT) Rte_AmbientMdl_ppSR_AmbientMdl_Output_SnowIndicatorValue = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_MPC_Day_Time, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time = {
  0U, 0U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(EKeyPos, RTE_VAR_INIT) Rte_AppCtrl_rpSR_rKeyPos_TIModMdl_KeyPos_KeyPos = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_BarView_ppCS_BarStatus_barnum = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_BarView_ppCS_BarStatus_barst = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_BarView_ppSR_BarView_Self_Test_status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(EBatteryState, RTE_VAR_INIT) Rte_BattMdl_ppBattState_BatteryState = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_BattMdl_ppBattState_IsHighVoltage = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_BattMdl_ppBattState_IsLowVoltage = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_BattMdl_ppSR_BattState_Dem_state = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_BkHomeChimeMdl_ppSR_TIBkHome_Status_lamp_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_BtnMdl_ppCS_BtnMdl_MenuButtonValue = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(EKeyPos, RTE_VAR_INIT) Rte_CDD_Test_tiSR_TIModMdl_KeyPos_KeyPos = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_CanEOLMdl_ppSR_CanEol_to_Appctrl_SleepMode_flag = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_ATSChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_DPFChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_DoorOpenChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_DrvSeatbelt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_IPM_LaneAssit_HandsonReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_IPM_LaneAssit_WarningAudio = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_MRR_PCW_STATE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_MRR_TakeOverReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_Nav_Sts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_NotInPChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_NotInPChimeIgnoff = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_PEPSChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_PasSeatbelt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_SCRChime_2hz_1 = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_TPMSChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_pp_GetOTAModeStatus_Value_OTAMode = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Date = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Hour = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Minute = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Month = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Second = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Year = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ClockMdl_ppSR_ClockMdl_Output_HourValue = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ClockMdl_ppSR_ClockMdl_Output_MinutesValue = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ClockMdl_ppSR_ClockMdl_Output_SecondsValue = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Rtc_TimeType, RTE_VAR_INIT) Rte_ClockMdl_ppSR_ClockMdl_Rtc_Time_Value = {
  0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ClockMdl_ppSR_ClockMdl_T_Value_T_Date = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ClockMdl_ppSR_ClockMdl_T_Value_T_Hour = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ClockMdl_ppSR_ClockMdl_T_Value_T_Minute = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ClockMdl_ppSR_ClockMdl_T_Value_T_Second = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_APA2_ComIn_APA_ChimeRequest = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_APA2_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_APA2_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_BrakeFluidSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_Mechanical_Hand_Brake = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16_01, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_OutsideTemp = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_PTCaseMod = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_PowerMode = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x584_ComIn_BCM_CarModeIndicator = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x584_ComIn_BCM_LoadManageLevel = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x584_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x584_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearLe = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearRi = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsTrunk = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_EnginehoodSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_MT_NeutralSensorSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BackHomeSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BrakelightDig = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BuzzerReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_DayRunningLampDig = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpDig = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_HighBeamSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_ImmoSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamDig = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpDig = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpDig = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorDig = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_reversinglightDig = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Sts_0x315_ComIn_BCM_BGLLumLvlSettingStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Sts_0x315_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_BCM_Sts_0x315_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_AbsFlgFlt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_EbdFlgFlt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLe = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(ACTEMP_ENABLE_Type, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLeStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRi = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRiStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_AWD_0x251_ComIn_AWD_EngTqAtCluLmtdByTrfCaseEn = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_AWD_0x251_ComIn_AWD_TrfCaseFltLmpReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_AWD_0x251_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_AWD_0x251_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_HMI_Popup_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_Hands_off_warning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_LDW_Flag = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_LDW_Status_Display = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_LKA_Status_Display = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_LKA_actionindication = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_Vehicle_Lane_Display = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_DCM_GlowPlugLmpReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_DCM_WIFFullLmpReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_AddUreaWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_GasPrtcWarn_G_Rq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_ManRgenTxt_D_RqDsply = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_NoxWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_Soot_Pc_Dsply = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtHour = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtMin = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtHour = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtMin = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaDTE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaLvl = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaQty = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaSysWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaTxt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaWarn = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x636_ComIn_EMS_BackupOdometer = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x636_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x636_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_BatteryVoltageError = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_CluthPedalStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngCoolanTemp = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_StartStopMessage = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_FuelconsumptionInst = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_Fuelcounter = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_MIL = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDTxt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDWarn = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_RemindShiftGear_MT = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_SVS = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_StartStopStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_EngineStopStartStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16_01, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_TargCruiseSpeed = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_SafeLampRed = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_WarningLampYellow = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ATS_Driving_mode_confirm = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ATS_failure_Lamp = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AVHWarningMessage = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhActive = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhAvabliable = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhStandby = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_CdpAvailable = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_Driving_mode_req = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage1 = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage2 = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage3 = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ESPInfoLamp = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EspEnableSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_HDCActive = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_HDCAvailable = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_TcsFlgFlt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_VdcFlgFlt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_IndexStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_WarningLevel = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_HMA_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssitType = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_HandsonReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_LeftVisualization = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_RightVisualization = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_WarningAudio = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimit = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimitUnits = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLAState = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_CTA_OFFTelltale = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_DOA_OFFTelltale = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SODLCA_OFFTelltale = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_BLIS_display = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqLeft = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqRight = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftFront = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftRear = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightFront = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightRear = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqLeft = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqRight = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x382_ComIn_MP5_BSDSoundSwitchSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x382_ComIn_MP5_NavStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x382_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x382_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_IVI_NavICON = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_IVI_ROUNG_ABOUT_NUM = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_IVI_ROUTE_REMAIN_DIS = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_MP5_NavICON = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUNG_ABOUT_NUM = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint32_least, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUTE_REMAIN_DIS = 0ULL; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmOrNot = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x530_ComIn_MP5_BSDSwitchSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x530_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x530_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_NAV_0x533_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_NAV_0x533_ComIn_Nav_Sts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_NAV_0x533_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_Nav_TX_0x385_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint32, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_Nav_TX_0x385_ComIn_NavDataSendReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_Nav_TX_0x385_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_latentWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_preWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_AEB_STATE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_ObjValid = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_STATE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_latentWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_preWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TakeOverReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TauGapSet = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_dxTarObj = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Textinfo = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_vSetDis = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Obj_0x_class = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Object_dx = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Object_dy = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundCadence = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_FrontSysTemWorkSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_RearSysTemWorkSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PSA2_SendOut_ESC_HSAStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PSA2_SendOut_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PSA2_SendOut_RADA_ACC_HMI_TgtDisp_Flag = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PSA2_SendOut_RADA_TARGET_POSITION = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PSA2_SendOut_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_EMS_CruiseCtrlSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_EMS_EngGPFStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_EMS_Fuelevel = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_EMS_OilEmptyAlarm = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_EPB_EPBDisplayMessageReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_DesiredHeadway_IconBasic = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_Desired_Headway_Display = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_Desired_Set_Speed = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_Pop_Up_Message = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_Set_Speed_Display = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_Vehicle_IconDisplay = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_AEB_Alert_Req = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_AEB_Fault_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_AEB_Switch_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_AEB_tips = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8_01, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SRS_0x350_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SRS_0x350_ComIn_SRS_AirBagFailSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SRS_0x350_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Confirm = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Down = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Left = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Return = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Right = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Up = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Date = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Hour = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Minute = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Month = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_RealTime_UB = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_RealTime_UB_Copy_1 = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Second = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Year = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_GW_TCU_ATFailure = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_GW_TCU_ShiftLeverPosValidFlag = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_TCU_ActualGear = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_TCU_PRNDLStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_TCU_SelectedATMode = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_TCU_TransmissionMILReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_GearSelectorReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_TargetGearReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Driverwarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_DrvShiftAllwCondRmd = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_GearboxP_Faulty = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_FaultyD = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_FaultyP = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_TransmissionSignals = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_unknown_D_P = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_Trans_Over_Heat = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TrnSrvcRqd_B_Rq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_AutoLockFunctionSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_BackFogligthSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_EngHoodUnlockWarming = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_FollowMeHomeSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_HighBeamStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_LFDoorSwitchSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_LRDoorSwitchSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_LeftTurnSwitchSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_LetfligthSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_LowBeamStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_PositionLampSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_RFDoorSwitchSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_RRDoorSwitchSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_RearWindowHeatSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_RightTurnSwitchrSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_RightligthSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_TrunkSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_frontFogligthSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM3_ComIn_BCM_BuzzerRequest = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM3_ComIn_BCM_IgnitionSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM3_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM3_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_IgnitionNotInOffPosition = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_KeyNearbyStartStopSwitch = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_Nsignal = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_StartupGearLeverError = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningAuthFail = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningESCLError = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningESCLUnLock = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningKeyInCar = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningKeyLowBattery = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningKeyNotInCar = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningNoValidKey = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningPowerRelayError = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningPoweroffNotPGear = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningStartupNotBrake = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_BrightnessChange = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_GearshiftRemindEnableSwitch = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_ICStyle_Set = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_IC_PersonalizedSet_left = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_IC_PersonalizedSet_right = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_Language_Set = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_BrakeIndicationWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_ClutchIndicationWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenNotOFFWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_ESCL_FailStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyBatteryWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyOutWarning1 = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyReminderWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_NoKeyFoundWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotNWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotPNWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_PowerMode = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_RemainingNumSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_SSB_FailStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_SystemFailStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_RF_0x326_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_RF_0x326_ComIn_RF_KeyLowBatteryWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_RF_0x326_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Lamp_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Tire_Temperature = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_Temperature_Warning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_ComVnimMdl_tiSR_CAN_AppCtrl_Flag = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CoolantTmpMdl_ppCoolantTmp_level = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(sint32, RTE_VAR_INIT) Rte_CoolantTmpMdl_ppSR_CoolantMdl_Value_Value = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CoolantTmpMdl_ppSR_WaterTempHigh_STATUS = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_CheckSum_IC = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_CruiseSwitch = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_DriverBeltSwitchSig = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_RollingCount_IC1 = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_TPMSManuStudyCmd = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_TargetCruiseVehS = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_TotalOdmeter = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_passengerBeltSwitchSig = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_NR = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_Tout = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_WriteCruise_Flash = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_WriteCruise_Icon = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_CruiseMdl_ppSR_WriteCruise_Speed = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_CruiseTtMdl_ppSR_CruiseTtMdl_Output_CruiseControlStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16_01, RTE_VAR_INIT) Rte_CruiseTtMdl_ppSR_CruiseTtMdl_Output_TargetCruiseSpeed = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_DTE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_VolumnQF = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_ManRgen_D_Rq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_DPFDTEMdl_ppSR_DPFDTEMdl_DteVal_EMS_UreaLvl_Dsply = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_DPFDTEMdl_ppSR_DPFDTEMdl_DteVal_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_DPFDTEMdl_rpSR_DPF_Generation_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_DPFWrnMdl_ppSR_DPFWrnMdl_GdtCtrl_State_DPF_ReGenSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_DPFWrnMdl_ppSR_DPFWrnMdl_GdtCtrl_State_GdtCtrl_State = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_DTEMdl_ppSR_DteMdl_DteVal_DteVal = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_DTE, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_DTE = {
  0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Assembly_Manufacturing_Date, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Manufacturing_Date = {
  0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Assembly_Plant_Number_10Byte, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Plant_Number; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_ECUManufacturingDate_4Byte, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUManufacturingDate; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_ECUSerialNumber_14Byte, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUSerialNumber; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_ECUProductPartNumber_12Byte, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ProductPartNumber; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_ECUSupplierIdentifier_10Byte, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SupplierIdentifier; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_VIN_Number_17Bytes_Type, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_ClusterVINNumber; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Assembly_Plant_Number_10Byte, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Customer_Reference; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_JMCHardwareVersionNumber_24Bytes_Type, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_JMCHardwareVersionNumber = {
  0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NvM_EOLDataType, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_EOLData = {
  0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_ClusterSettings, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_ClusterSettings = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_VIN_Number_17Bytes_Type, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference = {
  0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_PCBA_Reference_17Bytes_Type, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference_WithoutEEprom = {
  0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_ECUHardwareVersion, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_HardwareVersionNumber = {
  0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_ECUProductPartNumber_12Byte, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescripto_PartNumber_Data = {
  0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Run_In_mode_Control, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Run_In_mode_Control; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_SMD_Manufacturing_Date, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_SMD_Manufacturing_Date; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(SMD_Plant_Number_3Byte, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_SMD_Plant_Number; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Serial_Number_PCBA_5Byte, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Serial_Number_PCBA; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Cluster_Vehicle_Configure, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Vehicle_Configure; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_VIN_Number_17Bytes_Type, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Product_Reference = {
  0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_station_Tracebility, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_station_Tracebility; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_user_product_configuration, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_user_product_configuration = {
  0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 
  0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(DCM_IOC_Type, RTE_VAR_INIT) Rte_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value = {
  0U, 0U, {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U}
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NVDimDataSetType, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Dim_Buffer = {
  {0U, 0U, 0U, 0U, 0U, 0U}
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_DimPwm_ppSR_BrightnessChange_SetDuty = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_DoorAjarWrnMdl_ppSR_DoorAjarWrnMdl_GdtCtrl_State_GdtCtrl_State = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_DoorOpenChime_ppSR_DoorOpenChime_GdtCtrl_State = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_DoorOpenChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_DrvStyleMdl_rpSR_DrvStyleMdl_DrvStatus_DriverInfo = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ESPOffTtMdl_ppSR_ESPOFF_tt_ESPOFF_state_value_l_esp_off_status_u8 = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_EasyNaviMdl_ppSR_TIGDT_MP5_0x385_NaviLaodLen = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NaviRaodName, RTE_VAR_INIT) Rte_EasyNaviMdl_ppSR_TIGDT_MP5_0x385_NaviLaodValueBuf = {
  0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_EasyNaviMdl_ppSR_TIGdt_MP5_0x382_MP5_NavStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_GdtCtrl_rpSR_TIGdt_MP5_0x384_IVI_NavICON = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_GdtCtrl_rpSR_TIGdt_MP5_0x384_IVI_ROUNG_ABOUT_NUM = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_GdtCtrl_rpSR_TIGdt_MP5_0x384_IVI_ROUTE_REMAIN_DIS = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_GdtCtrl_rpSR_TIGdt_MP5_0x384_IVI_SIGNAL_LOST = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(EKeyPos, RTE_VAR_INIT) Rte_EcumExt_rpSR_rKeyPos_TIModMdl_KeyPos_KeyPos = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_AlarmOrNot = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_AlarmStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x392_ComOut_IC_ChangOilDistance = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x392_ComOut_IC_Fuel_level = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_FuelLevelMdl_ppSR_DteMdl_AbnormalState_AbnormalState = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_FuelLevelMdl_ppSR_FuelOpenCircuitTimeout_STATUS = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_FuelLevelMdl_ppSR_FuelShortCircuitTimeout_STATUS = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_FuelLevelMdl_ppSR_LowFuel_STATUS = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_FuelLevelMdl_ppSR_TIFuel_RefuelState_TIFuel_RefuelState = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_FuelLevelMdl_ppiSR_Fuel_to_TT_flag = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Fuel_Level, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_FuelLevel = {
  0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_FuelLevelMdl_tiSR_FuelLevel_level = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_GdtCtrl_AEB_FCW_ACC_Cfg = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_GdtCtrl_APA_Cfg = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_GdtCtrl_Auto_Lock_Cfg = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_GdtCtrl_CC_Cfg = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_GdtCtrl_Drive_Mode_Cfg = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_GdtCtrl_Gear_Cfg = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_GdtCtrl_LDW_LKA_Cfg = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_GdtCtrl_Passage_Seatbelt_Cfg = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_GdtCtrl_T_BOX_Cfg = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_MaintenanceMileageResetFlag_value = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_Remain_Manintenance_value = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_ResetTripWrnMdl_TripResetActive_TripAResetActive = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_ResetTripWrnMdl_TripResetActive_TripBResetActive = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_TIGdt_Animation_IPC_Full_Function = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_TIGdt_Animation_IPC_Partial_Function = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_TIGdt_Animation_close_animation_runing = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_TIGdt_Animation_welcome_animation_runing = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UnitMode, RTE_VAR_INIT) Rte_GdtCtrl_ppSR_TIGdt_Unit_UnitMode = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_Graph_10_MRButtonReset_MR_Button = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Menu_Settings, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Menu_Settings = {
  3U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_GdtCtrl_rpSR_BtnMdl_toSIA_KeyValue = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_GearMdl_ppSR_GearMdl_GearStatus_ATgearBlink = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_GearMdl_ppSR_GearMdl_GearStatus_ATgearBlinkVal = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_GearMdl_ppSR_GearMdl_GearStatus_ATgearInfo = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_GearMdl_ppSR_GearMdl_GearStatus_Flash_status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_GearMdl_ppSR_GearMdl_GearStatus_GearPosition = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_GearMdl_ppSR_GearMdl_GearStatus_GearShiftStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HighBeamMdl_rpSR_HighBeamMdl_Status_HighBeamSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HighBeamMdl_rpSR_HighBeamMdl_Status_LowBeamSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_ACCMode = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_AEB_STATE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_ObjValid = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_PCW_STATE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_TakeOverReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_TauGapSet = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_Textinfo = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_dxTarObj = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_vSetDis = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_PCW_latentWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_PCW_preWarning = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_IPM_HMA_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_IPM_LaneAssitType = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_IPM_LaneAssit_HandsonReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_LaneAssit_LeftVisualization = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_LaneAssit_RightVisualization = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_LaneAssit_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_SLASpdlimit = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_SLASpdlimitUnits = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_SLAState = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_CTA_OFFTelltale = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_DOA_OFFTelltale = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_MenuChgReq = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_SODLCA_OFFTelltale = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_SOD_BLIS_display = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_SOD_CTA_warningReqLeft = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_SOD_CTA_warningReqRight = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_HmiAccMdl_pp_HmiACC_GdtCtrl_Value_MRR_F_Obj_0x_class = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_HmiAccMdl_pp_HmiACC_GdtCtrl_Value_MRR_F_Object_dx = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_HmiAccMdl_pp_HmiACC_GdtCtrl_Value_MRR_F_Object_dy = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_tiSR_HmiAccMdl_HmiAccSpdSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAccMdl_tiSR_HmiAccMdl_HmiAccSpeed = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAdasMdl_ppCS_HmiAdas_LaneSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_HmiAdasMdl_ppCS_HmiAdas_TargetSt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_IFCMdl_ppSR_IFCMdl_IFCVal_IFCVal = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_IFCMdl_rpSR_IFCMdl_IFCStatus_IFCStatus = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_LowFuelMdl_ppSR_TILowFule_tt_state_value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(eIgnState, RTE_VAR_INIT) Rte_ModMdl_ppIgnState_IGNState = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(EKeyPos, RTE_VAR_INIT) Rte_ModMdl_ppSR_pKeyPos_TIModMdl_KeyPos_KeyPos = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_ModMdl_tiSR_IgnState_Dem_state = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Cluster_Optional_Feature_Configure, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Optional_Feature_Configure = {
  63U, 101U, 2U, 1U, 74U, 15U, 0U, 0U, 32U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NVOdoDataSetType, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer0 = {
  {0x0006U, 0xFFU, 0x80U}
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NVOdoDataSetType, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer1 = {
  {0x0006U, 0xFFU, 0x80U}
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NVOdoDataSetType, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer2 = {
  {0x0006U, 0xFFU, 0x80U}
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NVOdoDataSetType, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer3 = {
  {0x0006U, 0xFFU, 0x80U}
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NVOdoDataSetType, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer4 = {
  {0x0006U, 0xFFU, 0x80U}
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Cluster_SIA_Reset_Odo_OffSet, RTE_VAR_INIT) Rte_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Offset_Value = {
  0U, 0U, 5000U, 5000U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Cluster_SIA_Reset_Odo_OffSet, RTE_VAR_INIT) Rte_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Store_Value = {
  0U, 0U, 5000U, 5000U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(ACTEMP_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_ACTEMP_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(AUDIO_INFO_DISPLAY_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_AUDIO_INFO_DISPLAY_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(CALENDAR_CLOCK_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_CALENDAR_CLOCK_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(CAMERA_DISPLAY_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_CAMERA_DISPLAY_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(CRUISE_CONTROL_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_CRUISE_CONTROL_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(DRIVER_FATIGUE_WARNING_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_DRIVER_FATIGUE_WARNING_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(DVR_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_DVR_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(ENGINE_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_ENGINE_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(EPS_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_EPS_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(ESCL_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_ESCL_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(ESPABS_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_ESPABS_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(FUEL_TANK_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_FUEL_TANK_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(GEAR_BOX_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_GEAR_BOX_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(GENERATOR_TYPE_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_GENERATOR_TYPE_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(LANGUAGE_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_LANGUAGE_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(MAINTENANCE_INTERVAL_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_MAINTENANCE_INTERVAL_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NAVIGATION_DISPLAY_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_NAVIGATION_DISPLAY_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(OVERSPEED_ALARM_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_OVERSPEED_ALARM_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(PARKING_BRAKE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_PARKING_BRAKE_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(PAS_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_PAS_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(PEPS_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_PEPS_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(REQUEST_SHIFT_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_REQUEST_SHIFT_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(REVERSE_WARNING_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_REVERSE_WARNING_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(SDM_ENABLE_TYPE, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_SDM_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(SELF_CHECKING_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_SELF_CHECKING_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(STARTSTOP_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_STARTSTOP_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(STARTUP_MUSIC_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_STARTUP_MUSIC_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(TELEPHONE_INFO_DISPLAY_ENABLE_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_TELEPHONE_INFO_DISPLAY_ENABLE = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(TPMS_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_TPMS_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UNIT_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_UNIT_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(VEHICLE_LOGO_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_VEHICLE_LOGO_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(VEHICLE_MODELS_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_VEHICLE_MODELS_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(VEHICLE_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_VEHICLE_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(WHEEL_POSITION_VARIETY_Type, RTE_VAR_INIT) Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_WHEEL_POSITION_VARIETY = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_MPC_Day_Time, RTE_VAR_INIT) Rte_NVMExtMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value = {
  0U, 0U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_NotInPChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_ClusterStpSettings, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Stp_configData = {
  0U, 500U, 0U, 0U, 0U, 0U, 0U, 500U, 0U, 0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Interpolations, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Stp_angle_convertData = {
  {0U, 12000U, 20000U, 20000U, 20000U, 20000U}, {0U, 14220U, 18960U, 18960U, 18960U, 18960U}, {0U, 14220U, 18960U, 
  18960U, 18960U, 18960U}, {0U, 14220U, 18960U, 18960U, 18960U, 18960U}, {0U, 6000U, 6000U, 6000U, 6000U, 6000U}, {0U, 
  18960U, 18960U, 18960U, 18960U, 18960U}, {0U, 18960U, 18960U, 18960U, 18960U, 18960U}, {0U, 18960U, 18960U, 18960U, 
  18960U, 18960U}, {90U, 105U, 120U, 145U, 155U, 170U}, {200U, 2450U, 4700U, 4700U, 8075U, 9200U}, {200U, 2450U, 4700U, 
  4700U, 8075U, 9200U}, {200U, 2450U, 4700U, 4700U, 8075U, 9200U}, {0U, 6912U, 15104U, 30464U, 46336U, 63744U}, {200U, 
  1325U, 2450U, 4700U, 6950U, 9200U}, {200U, 1325U, 2450U, 4700U, 6950U, 9200U}, {200U, 1325U, 2450U, 4700U, 6950U, 
  9200U}
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Cluster_ODO_Backup_Counter, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_ODO_Backup_Counter = {
  0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Cluster_Variable_Block, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Variable_Block = {
  0U, 0U, 0U, 4U, 2U, 7U, 7U, 25U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_EcumExt_NV_Immobilizer_Value = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_EcumExt_NV_Language_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_ODO_Reset_Counter, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_ODO_Reset_Counter = {
  0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Cluster_SIA_Reset_Odo_OffSet, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SIA_Reset_Odo_OffSet = {
  0U, 0U, 5000U, 5000U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Cluster_SIA_Reset_Odo_OffSet, RTE_VAR_INIT) Rte_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Store_Value = {
  0U, 0U, 5000U, 5000U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Trip_A, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_TripA = {
  0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Trip_B, RTE_VAR_INIT) Rte_NvBlockSwComponentType_NVBlockDescriptor_TripB = {
  0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(OdoDataSetType, RTE_VAR_INIT) Rte_OdoMdl_NV_OdoMdl_NvMOdoData_OdoDataSetArray = {
  {0U, 0U, 0U}, {0U, 0U, 0U}, {0U, 0U, 0U}, {0U, 0U, 0U}, {0U, 0U, 0U}
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Cluster_Reset_OffSet, RTE_VAR_INIT) Rte_OdoMdl_NV_OdoMdl_Reset_OffSet_Offset_Value = {
  0U, 0U, 0U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_OdoMdl_ppSR_AfcMdl_OdoVal_OdoValue = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint32, RTE_VAR_INIT) Rte_OdoMdl_ppSR_OdoOutKiloMeters_Display_Value_KiloMeters = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_OdoMdl_ppSR_TIOdoMdl_ComOut_NewRollingOdo = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_SIAMdl_pprpSR_TIOdo_SIA_ResetSIA = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_OdoMdl_ppSR_TRIPMdl_IncreaseB_IncreaseB = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_OdoMdl_ppSR_TRIPMdl_ResetODO_KiloMeters_Resetflag = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(Boolean, RTE_VAR_INIT) Rte_OdoMdl_ppodo_ems_ischange_isodochange = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_Graph_437_distance_value = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_OdoMdl_rpSR_OdoMdl_Backup_Routine_Status_Interface_Routine_Status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(OdoMdl_RW_Interface_Type, RTE_VAR_INIT) Rte_OdoMdl_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface = {
  {FALSE, FALSE}
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(EKeyPos, RTE_VAR_INIT) Rte_OdoMdl_rpSR_rOdoLogicalMdl_TIModMdl_KeyPos_KeyPos = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint32, RTE_VAR_INIT) Rte_OdoMdl_tiSR_OdoOutKiloMeters_KiloMeters = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint32, RTE_VAR_INIT) Rte_OdoMdl_tiSR_OdoOutMiles_Miles = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_PEPSWrnMdl_ppSR_PEPSWrn_GdtCtrl_State_RemainingNum = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ResetTripWrnMdl_ppSR_ResetTripWrnMdl_ProgressBarSts_ProgressBarSts = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_ResetTripWrnMdl_ppSR_ResetTripWrnMdl_ResetTripInstruc_ResetTripAInstruc = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_ResetTripWrnMdl_ppSR_ResetTripWrnMdl_ResetTripInstruc_ResetTripBInstruc = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_DPFChime = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_SCRChime_2hz_1 = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_SCRWrnMdl_ppSR_SCRWrnMdl_GdtCtrl_State_GdtCtrl_State = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_SCRWrnMdl_ppSR_SCRWrnMdl_TimeData_SpeedLimtHour = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_SCRWrnMdl_ppSR_SCRWrnMdl_TimeData_SpeedLimtMin = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_SCRWrnMdl_ppSR_SCRWrnMdl_TimeData_TroqLimtHour = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_SCRWrnMdl_ppSR_SCRWrnMdl_TimeData_TroqLimtMin = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_SIAMdl_ppSR_CANMSG_IC2_ComOut_IC_MaintenceRemind = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_SIAMdl_ppSR_CANMSG_IC2_ComOut_IC_WaterTempHighRemind = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_AvgFuelConsumed = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_CruisRange = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_MaintainRemind = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_MaintenanceLogo = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_MaintenanceMile = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_MaintenceExpLogo = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_Trip = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_SIAMdl_ppSR_Sia_to_TT_st = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(sint32, RTE_VAR_INIT) Rte_SIAMdl_ppSR_pSIAMdl_TISIAMdl_Value_Out_MRDisplayValue_MR_Display_Value = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_SIAMdl_ppSR_pSIAMdl_TISIAMdl_Value_Out_WarningMRStatus_Warning_MR_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(NV_Data_Cluster_Maintenance, RTE_VAR_INIT) Rte_SIAMdl_pprpNV_Cluster_Maintenance_Maintenance_Value = {
  15239U
}; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_STTWrnMdl_rpSR_SttWrnMdl_EMS_StartStopMessage_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_SeatBeltMdl_pp_Seatbelt_TT_Status_Drv_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint8, RTE_VAR_INIT) Rte_SeatBeltMdl_pp_Seatbelt_TT_Status_Pas_Status = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_DrvSeatbelt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_PasSeatbelt = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_SpdMdl_ppSR_TISpdMdl_Real_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_SpdMdl_ppSR_TISpdMdl_Send_DigitalValue_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_SpdMdl_tiSR_TISpdMdl_Send_value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FL_State_FL_Tire_P_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FL_State_FL_Tire_Pressure = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FL_State_FL_Tire_SYS_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FL_State_FL_Tire_T_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(sint32, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FL_State_FL_Tire_Temp = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FR_State_FR_Tire_P_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FR_State_FR_Tire_Pressure = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FR_State_FR_Tire_SYS_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FR_State_FR_Tire_T_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(sint32, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FR_State_FR_Tire_Temp = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RL_State_RL_Tire_P_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RL_State_RL_Tire_Pressure = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RL_State_RL_Tire_SYS_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RL_State_RL_Tire_T_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(sint32, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RL_State_RL_Tire_Temp = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RR_State_RR_Tire_P_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RR_State_RR_Tire_Pressure = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RR_State_RR_Tire_SYS_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RR_State_RR_Tire_T_status = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(sint32, RTE_VAR_INIT) Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RR_State_RR_Tire_Temp = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FL_Pressure_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FL_Temp_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FL_Warning_St = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FR_Pressure_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FR_Temp_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FR_Warning_St = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RL_Pressure_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RL_Temp_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RL_Warning_St = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RR_Pressure_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RR_Temp_Value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt16, RTE_VAR_INIT) Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RR_Warning_St = 0; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_TachMdl_tiSR_TITachMdl_Send_value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_ComVnimMdl_rpSR_CANMSG_IC_0x392_ComOut_IC_AverageSpeed = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_TripMdl_ppSR_TripMdl_TripKiloMeters_TripA = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt32, RTE_VAR_INIT) Rte_TripMdl_ppSR_TripMdl_TripKiloMeters_TripB = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(UInt8, RTE_VAR_INIT) Rte_TripMdl_ppsr_average_speed_average_speed_value = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint16, RTE_VAR_INIT) Rte_TripMdl_ppsr_odo_average_speed_odoaveragespeed = 0U; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(boolean, RTE_VAR_INIT) Rte_TtMdl_ppSR_TITtMdl_ProveoutDone_ValueDone = FALSE; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */
VAR(uint64, RTE_VAR_INIT) Rte_TtView_tiSR_PosLightMd_Send_Value = 0ULL; /* PRQA S 0850, 3408, 1504 */ /* MD_MSR_19.8, MD_Rte_3408, MD_MSR_8.10 */

#define RTE_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/**********************************************************************************************************************
 * Defines for Rte_ComSendSignalProxy
 *********************************************************************************************************************/
#define RTE_COM_SENDSIGNALPROXY_NOCHANGE       (0U)
#define RTE_COM_SENDSIGNALPROXY_SEND           (1U)
#define RTE_COM_SENDSIGNALPROXY_INVALIDATE     (2U)



/**********************************************************************************************************************
 * Timeout Flags for each external Rx Signals with aliveTimeout != 0
 *********************************************************************************************************************/

#define RTE_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(Rte_OsApplication_RxTimeoutFlagsType, RTE_VAR_ZERO_INIT) Rte_OsApplication_RxTimeoutFlags = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
}; /* PRQA S 0850 */ /* MD_MSR_19.8 */

#define RTE_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define Rte_OsApplication_RxTimeoutFlagsInit() (Rte_MemClr(&Rte_OsApplication_RxTimeoutFlags, sizeof(Rte_OsApplication_RxTimeoutFlagsType)))


/**********************************************************************************************************************
 * Never Received Flags for each external Rx Signal with handleNeverReceived != 0
 *********************************************************************************************************************/

#define RTE_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(Rte_OsApplication_RxNeverReceivedFlagsType, RTE_VAR_ZERO_INIT) Rte_OsApplication_RxNeverReceivedFlags = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
}; /* PRQA S 0850 */ /* MD_MSR_19.8 */

#define RTE_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define Rte_OsApplication_RxNeverReceivedFlagsInit() (Rte_MemClr(&Rte_OsApplication_RxNeverReceivedFlags, sizeof(Rte_OsApplication_RxNeverReceivedFlagsType)))


/**********************************************************************************************************************
 * Dirty Flags for NVBlockDescriptors
 *********************************************************************************************************************/

#define RTE_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(Rte_DirtyFlagsType, RTE_VAR_ZERO_INIT) Rte_DirtyFlags = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
}; /* PRQA S 0850 */ /* MD_MSR_19.8 */

#define RTE_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define Rte_DirtyFlagsInit() (Rte_MemClr(&Rte_DirtyFlags, sizeof(Rte_DirtyFlagsType)))


/**********************************************************************************************************************
 * Prototypes for Runnable Entities of Nv Block Components
 *********************************************************************************************************************/

#define RTE_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC FUNC(void, RTE_CODE) NvBlockSwComponentTypeRunnable(void); /* PRQA S 0850 */ /* MD_MSR_19.8 */

#define RTE_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * Internal C/S connections
 *********************************************************************************************************************/
#define Rte_CS_ClientConfigIndex_GdtCdd_TIGdtCtrl_RenderDone 0U

#define RTE_START_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(Rte_OsApplication_ClientIdleFlagsType, RTE_VAR_INIT) Rte_OsApplication_ClientIdleFlags = {
   1
};

#define RTE_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define RTE_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC FUNC(void, RTE_CODE) Rte_MemClr(P2VAR(void, AUTOMATIC, RTE_VAR_NOINIT) ptr, uint16_least num); /* PRQA S 0850 */ /* MD_MSR_19.8 */
FUNC(void, RTE_CODE) Rte_MemCpy(P2VAR(void, AUTOMATIC, RTE_APPL_VAR) destination, P2CONST(void, AUTOMATIC, RTE_APPL_DATA) source, uint16_least num); /* PRQA S 0850, 1505, 3447, 3408 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3447, MD_Rte_3408 */
FUNC(void, RTE_CODE) Rte_MemCpy32(P2VAR(void, AUTOMATIC, RTE_APPL_VAR) destination, P2CONST(void, AUTOMATIC, RTE_APPL_DATA) source, uint16_least num); /* PRQA S 0850, 1505, 3447, 3408 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3447, MD_Rte_3408 */

#define RTE_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 * Data structures for mode management
 *********************************************************************************************************************/





/**********************************************************************************************************************
 * Timer handling
 *********************************************************************************************************************/

#define RTE_USEC_SystemTimer(val) ((TickType)RTE_CONST_USEC_SystemTimer_##val) /* PRQA S 0342 */ /* MD_Rte_0342 */
#define RTE_MSEC_SystemTimer(val) ((TickType)RTE_CONST_MSEC_SystemTimer_##val) /* PRQA S 0342 */ /* MD_Rte_0342 */
#define RTE_SEC_SystemTimer(val)  ((TickType)RTE_CONST_SEC_SystemTimer_##val) /* PRQA S 0342 */ /* MD_Rte_0342 */

#define RTE_CONST_MSEC_SystemTimer_0 (0UL)

#define RTE_CONST_MSEC_SystemTimer_10 (10UL)

#define RTE_CONST_MSEC_SystemTimer_100 (100UL)

#define RTE_CONST_MSEC_SystemTimer_128 (128UL)

#define RTE_CONST_MSEC_SystemTimer_16 (16UL)

#define RTE_CONST_MSEC_SystemTimer_2 (2UL)

#define RTE_CONST_MSEC_SystemTimer_20 (20UL)

#define RTE_CONST_MSEC_SystemTimer_30 (30UL)

#define RTE_CONST_MSEC_SystemTimer_5 (5UL)

#define RTE_CONST_MSEC_SystemTimer_50 (50UL)

#define RTE_CONST_MSEC_SystemTimer_8 (8UL)

#define RTE_CONST_SEC_SystemTimer_0 (0UL)

#define RTE_CONST_SEC_SystemTimer_1 (1000UL)


/**********************************************************************************************************************
 * Internal definitions
 *********************************************************************************************************************/

#define RTE_TASK_TIMEOUT_EVENT_MASK   ((EventMaskType)0x01)
#define RTE_TASK_WAITPOINT_EVENT_MASK ((EventMaskType)0x02)

/**********************************************************************************************************************
 * Buffers for Basic Tasks triggered by OS Schedule Table
 *********************************************************************************************************************/

#define RTE_START_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(uint8, RTE_VAR_ZERO_INIT) Rte_ScheduleTable_OsTask_NvmStack_Step = 0U; /* PRQA S 0850 */ /* MD_MSR_19.8 */

#define RTE_STOP_SEC_VAR_ZERO_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * RTE life cycle API
 *********************************************************************************************************************/

#define RTE_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC FUNC(void, RTE_CODE) Rte_MemClr(P2VAR(void, AUTOMATIC, RTE_VAR_NOINIT) ptr, uint16_least num)
{
  P2VAR(uint8, AUTOMATIC, RTE_VAR_NOINIT) dst = ptr;
  uint16_least i;
  for (i = 0; i < num; i++)
  {
    dst[i] = 0;
  }
}


FUNC(void, RTE_CODE) Rte_InitMemory_OsApplication(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  /* reset Rx Timeout Flags */
  Rte_OsApplication_RxTimeoutFlagsInit();

  /* reset Rx Never Received Flags */
  Rte_OsApplication_RxNeverReceivedFlagsInit();

  /* reset Dirty Flags */
  Rte_DirtyFlagsInit();

  /* reset OsScheduleTable variables */
  Rte_ScheduleTable_OsTask_NvmStack_Step = 0U;

  /* set default values for internal data */
  Rte_AFCMdl_ppSR_AfcMdl_AfcVal_AccumulateVal = 0U;
  Rte_AFCMdl_ppSR_AfcMdl_AfcVal_AfcRefuelVal = 0U;
  Rte_AFCMdl_ppSR_AfcMdl_AfcVal_AfcSingleVal = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_AFC = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_AFC_Type = 0U;
  Rte_APBTtMdl_ppSR_APB_Message1_tt_state_value = 0U;
  Rte_APBTtMdl_ppSR_APB_Message2_tt_state_value = 0U;
  Rte_APBTtMdl_ppSR_APB_tt_state_value = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_ATSChime = 0U;
  Rte_AVHTtMdl_ppSR_AVH_tt_state_value = 0U;
  Rte_AccWarning_ppSR_AccWarning_GdtCtrl_State_BackRemindSts = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_QDashACCFail = 0U;
  Rte_AccWarning_pp_AccWarning_GdtCtrl_Value_IPM_HandsonReq = 0U;
  Rte_AdasChime_ppSR_CANMSG_IC_0x386_ComOut_IC_BSDSoundSts = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_IPM_LaneAssit_HandsonReq = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_IPM_LaneAssit_WarningAudio = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_MRR_PCW_STATE = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_MRR_TakeOverReq = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_Nav_Sts = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_NotInPChimeIgnoff = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_PEPSChime = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_TPMSChime = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_AirbagTelltaleBehavior = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_VehSpd_HMI = 0U;
  Rte_AmbientMdl_ppSR_AmbientMdl_Output_OutsideTempValue = 0U;
  Rte_AmbientMdl_ppSR_AmbientMdl_Output_SnowIndicatorValue = 0U;
  Rte_BarView_ppCS_BarStatus_barnum = 0U;
  Rte_BarView_ppCS_BarStatus_barst = 0U;
  Rte_BarView_ppSR_BarView_Self_Test_status = 0U;
  Rte_BattMdl_ppBattState_BatteryState = 0U;
  Rte_BattMdl_ppBattState_IsHighVoltage = FALSE;
  Rte_BattMdl_ppBattState_IsLowVoltage = FALSE;
  Rte_BattMdl_ppSR_BattState_Dem_state = FALSE;
  Rte_BkHomeChimeMdl_ppSR_TIBkHome_Status_lamp_status = FALSE;
  Rte_BtnMdl_ppCS_BtnMdl_MenuButtonValue = 0U;
  Rte_CanEOLMdl_ppSR_CanEol_to_Appctrl_SleepMode_flag = FALSE;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_ATSChime = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_DPFChime = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_DoorOpenChime = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_DrvSeatbelt = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_IPM_LaneAssit_HandsonReq = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_IPM_LaneAssit_WarningAudio = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_MRR_PCW_STATE = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_MRR_TakeOverReq = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_Nav_Sts = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_NotInPChime = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_NotInPChimeIgnoff = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_PEPSChime = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_PasSeatbelt = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_SCRChime_2hz_1 = 0U;
  Rte_ChimeMdl_ppSR_TIChimeMdl_Send_ToutValue_TPMSChime = 0U;
  Rte_ChimeMdl_pp_GetOTAModeStatus_Value_OTAMode = 0U;
  Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Date = 0U;
  Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Hour = 0U;
  Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Minute = 0U;
  Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Month = 0U;
  Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Second = 0U;
  Rte_ClockMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Year = 0U;
  Rte_ClockMdl_ppSR_ClockMdl_Output_HourValue = 0U;
  Rte_ClockMdl_ppSR_ClockMdl_Output_MinutesValue = 0U;
  Rte_ClockMdl_ppSR_ClockMdl_Output_SecondsValue = 0U;
  Rte_ClockMdl_ppSR_ClockMdl_Rtc_Time_Value = Rte_Rtc_TimeType_Init;
  Rte_ClockMdl_ppSR_ClockMdl_T_Value_T_Date = 0U;
  Rte_ClockMdl_ppSR_ClockMdl_T_Value_T_Hour = 0U;
  Rte_ClockMdl_ppSR_ClockMdl_T_Value_T_Minute = 0U;
  Rte_ClockMdl_ppSR_ClockMdl_T_Value_T_Second = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_APA2_ComIn_APA_ChimeRequest = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_APA2_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_APA2_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_BrakeFluidSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_Mechanical_Hand_Brake = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_OutsideTemp = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_PTCaseMod = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_BCM_PowerMode = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x313_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x584_ComIn_BCM_CarModeIndicator = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x584_ComIn_BCM_LoadManageLevel = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x584_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_0x584_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearLe = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearRi = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsTrunk = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_EnginehoodSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_BCM_MT_NeutralSensorSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Door_0x310_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BackHomeSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BrakelightDig = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BuzzerReq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_DayRunningLampDig = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpDig = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_HighBeamSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_ImmoSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamDig = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpDig = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpDig = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorDig = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_reversinglightDig = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Sts_0x315_ComIn_BCM_BGLLumLvlSettingStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Sts_0x315_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_BCM_Sts_0x315_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_AbsFlgFlt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_EbdFlgFlt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLe = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLeStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRi = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRiStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_AWD_0x251_ComIn_AWD_EngTqAtCluLmtdByTrfCaseEn = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_AWD_0x251_ComIn_AWD_TrfCaseFltLmpReq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_AWD_0x251_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_AWD_0x251_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_HMI_Popup_Status = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_Hands_off_warning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_LDW_Flag = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_LDW_Status_Display = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_LKA_Status_Display = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_LKA_actionindication = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_Camera_ComIn_Vehicle_Lane_Display = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_DCM_GlowPlugLmpReq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_DCM_WIFFullLmpReq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_AddUreaWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_GasPrtcWarn_G_Rq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_ManRgenTxt_D_RqDsply = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_NoxWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_Soot_Pc_Dsply = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtHour = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtMin = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtHour = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtMin = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaDTE = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaLvl = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaQty = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaSysWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaTxt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaWarn = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x368_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x636_ComIn_EMS_BackupOdometer = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x636_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_0x636_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_BatteryVoltageError = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_CluthPedalStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngCoolanTemp = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_StartStopMessage = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_FuelconsumptionInst = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_Fuelcounter = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_MIL = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDTxt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDWarn = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_RemindShiftGear_MT = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_SVS = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_StartStopStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Power_0x366_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_EngineStopStartStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_TargCruiseSpeed = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_SafeLampRed = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_WarningLampYellow = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ATS_Driving_mode_confirm = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ATS_failure_Lamp = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AVHWarningMessage = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhActive = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhAvabliable = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhStandby = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_CdpAvailable = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_Driving_mode_req = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage1 = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage2 = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage3 = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ESPInfoLamp = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EspEnableSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_HDCActive = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_HDCAvailable = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_TcsFlgFlt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_VdcFlgFlt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_IndexStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_WarningLevel = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_HMA_Status = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssitType = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_HandsonReq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_LeftVisualization = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_RightVisualization = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_WarningAudio = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimit = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimitUnits = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLAState = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x245_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_CTA_OFFTelltale = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_DOA_OFFTelltale = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SODLCA_OFFTelltale = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_BLIS_display = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqLeft = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqRight = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftFront = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftRear = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightFront = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightRear = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqLeft = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqRight = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_IPM_0x275_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x382_ComIn_MP5_BSDSoundSwitchSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x382_ComIn_MP5_NavStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x382_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x382_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_IVI_NavICON = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_IVI_ROUNG_ABOUT_NUM = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_IVI_ROUTE_REMAIN_DIS = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_MP5_NavICON = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUNG_ABOUT_NUM = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUTE_REMAIN_DIS = 0ULL;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x384_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmOrNot = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x530_ComIn_MP5_BSDSwitchSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x530_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_0x530_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_NAV_0x533_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_NAV_0x533_ComIn_Nav_Sts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_NAV_0x533_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_Nav_TX_0x385_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_Nav_TX_0x385_ComIn_NavDataSendReq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MP5_Nav_TX_0x385_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_latentWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_preWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_AEB_STATE = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_ObjValid = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_STATE = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_latentWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_preWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TakeOverReq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TauGapSet = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_dxTarObj = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Textinfo = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_vSetDis = 0;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Obj_0x_class = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Object_dx = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Object_dy = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundCadence = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_FrontSysTemWorkSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_PAM_RearSysTemWorkSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PAM_0x271_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PSA2_SendOut_ESC_HSAStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PSA2_SendOut_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PSA2_SendOut_RADA_ACC_HMI_TgtDisp_Flag = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PSA2_SendOut_RADA_TARGET_POSITION = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PSA2_SendOut_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_EMS_CruiseCtrlSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_EMS_EngGPFStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_EMS_Fuelevel = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_EMS_OilEmptyAlarm = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_EPB_EPBDisplayMessageReq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_PT2_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_DesiredHeadway_IconBasic = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_Desired_Headway_Display = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_Desired_Set_Speed = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_Pop_Up_Message = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_Set_Speed_Display = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_ACC_Vehicle_IconDisplay = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_AEB_Alert_Req = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_AEB_Fault_Status = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_AEB_Switch_Status = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_GW_AEB_tips = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_RADA2_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SRS_0x350_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SRS_0x350_ComIn_SRS_AirBagFailSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SRS_0x350_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Confirm = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Down = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Left = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Return = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Right = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Up = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_SWM_Body_0x31A_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Date = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Hour = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Minute = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Month = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_RealTime_UB = FALSE;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_RealTime_UB_Copy_1 = FALSE;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Second = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Year = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TBOX_time_0x582_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_GW_TCU_ATFailure = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_GW_TCU_ShiftLeverPosValidFlag = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_TCU_ActualGear = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_TCU_PRNDLStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_TCU_SelectedATMode = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_TCU_TransmissionMILReq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU1_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_GearSelectorReq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_TargetGearReq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Driverwarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_DrvShiftAllwCondRmd = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_GearboxP_Faulty = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_FaultyD = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_FaultyP = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_TransmissionSignals = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_unknown_D_P = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_Trans_Over_Heat = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_GW_TCU_Info_0x395_ComIn_TrnSrvcRqd_B_Rq = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_AutoLockFunctionSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_BackFogligthSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_EngHoodUnlockWarming = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_FollowMeHomeSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_HighBeamStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_LFDoorSwitchSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_LRDoorSwitchSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_LeftTurnSwitchSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_LetfligthSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_LowBeamStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_PositionLampSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_RFDoorSwitchSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_RRDoorSwitchSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_RearWindowHeatSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_RightTurnSwitchrSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_RightligthSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_TrunkSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_BCM_frontFogligthSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM2_SendOut_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM3_ComIn_BCM_BuzzerRequest = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM3_ComIn_BCM_IgnitionSt = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM3_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM3_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_IgnitionNotInOffPosition = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_KeyNearbyStartStopSwitch = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_Nsignal = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_StartupGearLeverError = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningAuthFail = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningESCLError = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningESCLUnLock = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningKeyInCar = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningKeyLowBattery = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningKeyNotInCar = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningNoValidKey = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningPowerRelayError = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningPoweroffNotPGear = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_PEPS_WarningStartupNotBrake = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_IBCM_PEPS1_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_BrightnessChange = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_GearshiftRemindEnableSwitch = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_ICStyle_Set = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_IC_PersonalizedSet_left = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_IC_PersonalizedSet_right = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_MP5_Language_Set = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_MP5_2_SendOut_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_BrakeIndicationWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_ClutchIndicationWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenNotOFFWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_ESCL_FailStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyBatteryWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyOutWarning1 = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyReminderWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_NoKeyFoundWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotNWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotPNWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_PowerMode = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_RemainingNumSts = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_SSB_FailStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_PEPS_SystemFailStatus = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_PEPS_0x325_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_RF_0x326_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_RF_0x326_ComIn_RF_KeyLowBatteryWarning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_RF_0x326_ComIn_Tout = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_NR = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Lamp_Status = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Tire_Temperature = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_Temperature_Warning = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position = 0U;
  Rte_ComVnimMdl_ppSR_CANMSG_TPMS_Tire_0x361_ComIn_Tout = 0U;
  Rte_ComVnimMdl_tiSR_CAN_AppCtrl_Flag = FALSE;
  Rte_CoolantTmpMdl_ppCoolantTmp_level = 0U;
  Rte_CoolantTmpMdl_ppSR_CoolantMdl_Value_Value = 0;
  Rte_CoolantTmpMdl_ppSR_WaterTempHigh_STATUS = 0U;
  Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_CheckSum_IC = 0U;
  Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_CruiseSwitch = 0U;
  Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_DriverBeltSwitchSig = 0U;
  Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_RollingCount_IC1 = 0U;
  Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_TPMSManuStudyCmd = 0U;
  Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_TargetCruiseVehS = 0;
  Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_TotalOdmeter = 0U;
  Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_IC_passengerBeltSwitchSig = 0U;
  Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_NR = 0U;
  Rte_CruiseMdl_ppSR_CANMSG_IC_ComOut_Tout = 0U;
  Rte_CruiseMdl_ppSR_WriteCruise_Flash = 0U;
  Rte_CruiseMdl_ppSR_WriteCruise_Icon = 0U;
  Rte_CruiseMdl_ppSR_WriteCruise_Speed = 0U;
  Rte_CruiseTtMdl_ppSR_CruiseTtMdl_Output_CruiseControlStatus = 0U;
  Rte_CruiseTtMdl_ppSR_CruiseTtMdl_Output_TargetCruiseSpeed = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_DTE = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_VolumnQF = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_ManRgen_D_Rq = 0U;
  Rte_DPFDTEMdl_ppSR_DPFDTEMdl_DteVal_EMS_UreaLvl_Dsply = 0U;
  Rte_DPFDTEMdl_ppSR_DPFDTEMdl_DteVal_Value = 0U;
  Rte_DPFDTEMdl_rpSR_DPF_Generation_Status = 0U;
  Rte_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply = 0U;
  Rte_DPFWrnMdl_ppSR_DPFWrnMdl_GdtCtrl_State_DPF_ReGenSts = 0U;
  Rte_DPFWrnMdl_ppSR_DPFWrnMdl_GdtCtrl_State_GdtCtrl_State = 0U;
  Rte_DTEMdl_ppSR_DteMdl_DteVal_DteVal = 0U;
  Rte_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value = Rte_ZeroDataDID_IOC;
  Rte_DimPwm_ppSR_BrightnessChange_SetDuty = 0;
  Rte_DoorAjarWrnMdl_ppSR_DoorAjarWrnMdl_GdtCtrl_State_GdtCtrl_State = 0U;
  Rte_DoorOpenChime_ppSR_DoorOpenChime_GdtCtrl_State = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_DoorOpenChime = 0U;
  Rte_DrvStyleMdl_rpSR_DrvStyleMdl_DrvStatus_DriverInfo = 0U;
  Rte_ESPOffTtMdl_ppSR_ESPOFF_tt_ESPOFF_state_value_l_esp_off_status_u8 = 0U;
  Rte_EasyNaviMdl_ppSR_TIGDT_MP5_0x385_NaviLaodLen = 0U;
  Rte_MemCpy32(Rte_EasyNaviMdl_ppSR_TIGDT_MP5_0x385_NaviLaodValueBuf, Rte_C_NaviRaodName_0, sizeof(NaviRaodName));
  Rte_EasyNaviMdl_ppSR_TIGdt_MP5_0x382_MP5_NavStatus = 0U;
  Rte_GdtCtrl_rpSR_TIGdt_MP5_0x384_IVI_NavICON = 0U;
  Rte_GdtCtrl_rpSR_TIGdt_MP5_0x384_IVI_ROUNG_ABOUT_NUM = 0U;
  Rte_GdtCtrl_rpSR_TIGdt_MP5_0x384_IVI_ROUTE_REMAIN_DIS = 0U;
  Rte_GdtCtrl_rpSR_TIGdt_MP5_0x384_IVI_SIGNAL_LOST = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_AlarmOrNot = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x525_ComOut_IC_AlarmStatus = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x392_ComOut_IC_ChangOilDistance = 0;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x392_ComOut_IC_Fuel_level = 0U;
  Rte_FuelLevelMdl_ppSR_DteMdl_AbnormalState_AbnormalState = 0U;
  Rte_FuelLevelMdl_ppSR_FuelOpenCircuitTimeout_STATUS = 0U;
  Rte_FuelLevelMdl_ppSR_FuelShortCircuitTimeout_STATUS = 0U;
  Rte_FuelLevelMdl_ppSR_LowFuel_STATUS = 0U;
  Rte_FuelLevelMdl_ppSR_TIFuel_RefuelState_TIFuel_RefuelState = FALSE;
  Rte_FuelLevelMdl_ppiSR_Fuel_to_TT_flag = 0U;
  Rte_FuelLevelMdl_tiSR_FuelLevel_level = 0U;
  Rte_GdtCtrl_ppSR_GdtCtrl_AEB_FCW_ACC_Cfg = FALSE;
  Rte_GdtCtrl_ppSR_GdtCtrl_APA_Cfg = FALSE;
  Rte_GdtCtrl_ppSR_GdtCtrl_Auto_Lock_Cfg = FALSE;
  Rte_GdtCtrl_ppSR_GdtCtrl_CC_Cfg = FALSE;
  Rte_GdtCtrl_ppSR_GdtCtrl_Drive_Mode_Cfg = FALSE;
  Rte_GdtCtrl_ppSR_GdtCtrl_Gear_Cfg = FALSE;
  Rte_GdtCtrl_ppSR_GdtCtrl_LDW_LKA_Cfg = FALSE;
  Rte_GdtCtrl_ppSR_GdtCtrl_Passage_Seatbelt_Cfg = FALSE;
  Rte_GdtCtrl_ppSR_GdtCtrl_T_BOX_Cfg = FALSE;
  Rte_GdtCtrl_ppSR_MaintenanceMileageResetFlag_value = FALSE;
  Rte_GdtCtrl_ppSR_Remain_Manintenance_value = 0;
  Rte_GdtCtrl_ppSR_ResetTripWrnMdl_TripResetActive_TripAResetActive = FALSE;
  Rte_GdtCtrl_ppSR_ResetTripWrnMdl_TripResetActive_TripBResetActive = FALSE;
  Rte_GdtCtrl_ppSR_TIGdt_Animation_IPC_Full_Function = FALSE;
  Rte_GdtCtrl_ppSR_TIGdt_Animation_IPC_Partial_Function = FALSE;
  Rte_GdtCtrl_ppSR_TIGdt_Animation_close_animation_runing = FALSE;
  Rte_GdtCtrl_ppSR_TIGdt_Animation_welcome_animation_runing = FALSE;
  Rte_GdtCtrl_ppSR_TIGdt_Unit_UnitMode = 0U;
  Rte_Graph_10_MRButtonReset_MR_Button = 0U;
  Rte_GdtCtrl_rpSR_BtnMdl_toSIA_KeyValue = 0U;
  Rte_GearMdl_ppSR_GearMdl_GearStatus_ATgearBlink = 0U;
  Rte_GearMdl_ppSR_GearMdl_GearStatus_ATgearBlinkVal = 0U;
  Rte_GearMdl_ppSR_GearMdl_GearStatus_ATgearInfo = 0U;
  Rte_GearMdl_ppSR_GearMdl_GearStatus_Flash_status = 0U;
  Rte_GearMdl_ppSR_GearMdl_GearStatus_GearPosition = 0U;
  Rte_GearMdl_ppSR_GearMdl_GearStatus_GearShiftStatus = 0U;
  Rte_HighBeamMdl_rpSR_HighBeamMdl_Status_HighBeamSts = 0U;
  Rte_HighBeamMdl_rpSR_HighBeamMdl_Status_LowBeamSts = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_ACCMode = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_AEB_STATE = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_ObjValid = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_PCW_STATE = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_TakeOverReq = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_TauGapSet = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_Textinfo = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_dxTarObj = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_MRR_vSetDis = 0;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_PCW_latentWarning = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_AccStatus_PCW_preWarning = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_IPM_HMA_Status = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_IPM_LaneAssitType = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_IPM_LaneAssit_HandsonReq = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_LaneAssit_LeftVisualization = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_LaneAssit_RightVisualization = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_LaneAssit_Status = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_SLASpdlimit = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_SLASpdlimitUnits = 0U;
  Rte_HmiAccMdl_ppSR_HmiAccMdl_LaneStatus_SLAState = 0U;
  Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_CTA_OFFTelltale = 0U;
  Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_DOA_OFFTelltale = 0U;
  Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_MenuChgReq = 0U;
  Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_SODLCA_OFFTelltale = 0U;
  Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_SOD_BLIS_display = 0U;
  Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_SOD_CTA_warningReqLeft = 0U;
  Rte_HmiAccMdl_ppSR_TIGdt_SOD_BLIS_DISP_SOD_CTA_warningReqRight = 0U;
  Rte_HmiAccMdl_pp_HmiACC_GdtCtrl_Value_MRR_F_Obj_0x_class = 0U;
  Rte_HmiAccMdl_pp_HmiACC_GdtCtrl_Value_MRR_F_Object_dx = 0U;
  Rte_HmiAccMdl_pp_HmiACC_GdtCtrl_Value_MRR_F_Object_dy = 0U;
  Rte_HmiAccMdl_tiSR_HmiAccMdl_HmiAccSpdSt = 0U;
  Rte_HmiAccMdl_tiSR_HmiAccMdl_HmiAccSpeed = 0U;
  Rte_HmiAdasMdl_ppCS_HmiAdas_LaneSt = 0U;
  Rte_HmiAdasMdl_ppCS_HmiAdas_TargetSt = 0U;
  Rte_IFCMdl_ppSR_IFCMdl_IFCVal_IFCVal = 0U;
  Rte_IFCMdl_rpSR_IFCMdl_IFCStatus_IFCStatus = 0U;
  Rte_LowFuelMdl_ppSR_TILowFule_tt_state_value = 0U;
  Rte_ModMdl_ppIgnState_IGNState = 0U;
  Rte_ModMdl_ppSR_pKeyPos_TIModMdl_KeyPos_KeyPos = 0U;
  Rte_ModMdl_tiSR_IgnState_Dem_state = FALSE;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_ACTEMP_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_AUDIO_INFO_DISPLAY_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_CALENDAR_CLOCK_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_CAMERA_DISPLAY_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_CRUISE_CONTROL_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_DRIVER_FATIGUE_WARNING_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_DVR_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_ENGINE_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_EPS_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_ESCL_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_ESPABS_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_FUEL_TANK_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_GEAR_BOX_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_GENERATOR_TYPE_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_LANGUAGE_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_MAINTENANCE_INTERVAL_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_NAVIGATION_DISPLAY_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_OVERSPEED_ALARM_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_PARKING_BRAKE_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_PAS_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_PEPS_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_REQUEST_SHIFT_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_REVERSE_WARNING_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_SDM_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_SELF_CHECKING_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_STARTSTOP_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_STARTUP_MUSIC_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_TELEPHONE_INFO_DISPLAY_ENABLE = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_TPMS_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_UNIT_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_VEHICLE_LOGO_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_VEHICLE_MODELS_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_VEHICLE_VARIETY = 0U;
  Rte_NVMExtMdl_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_WHEEL_POSITION_VARIETY = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_NotInPChime = 0U;
  Rte_OdoMdl_ppSR_AfcMdl_OdoVal_OdoValue = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue = 0U;
  Rte_OdoMdl_ppSR_OdoOutKiloMeters_Display_Value_KiloMeters = 0U;
  Rte_OdoMdl_ppSR_TIOdoMdl_ComOut_NewRollingOdo = 0U;
  Rte_SIAMdl_pprpSR_TIOdo_SIA_ResetSIA = FALSE;
  Rte_OdoMdl_ppSR_TRIPMdl_IncreaseB_IncreaseB = 0U;
  Rte_OdoMdl_ppSR_TRIPMdl_ResetODO_KiloMeters_Resetflag = FALSE;
  Rte_OdoMdl_ppodo_ems_ischange_isodochange = FALSE;
  Rte_Graph_437_distance_value = 0;
  Rte_OdoMdl_rpSR_OdoMdl_Backup_Routine_Status_Interface_Routine_Status = FALSE;
  Rte_OdoMdl_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface = Rte_OdoMdl_RW_Interface_Init_Value;
  Rte_OdoMdl_tiSR_OdoOutKiloMeters_KiloMeters = 0U;
  Rte_OdoMdl_tiSR_OdoOutMiles_Miles = 0U;
  Rte_PEPSWrnMdl_ppSR_PEPSWrn_GdtCtrl_State_RemainingNum = 0U;
  Rte_ResetTripWrnMdl_ppSR_ResetTripWrnMdl_ProgressBarSts_ProgressBarSts = 0U;
  Rte_ResetTripWrnMdl_ppSR_ResetTripWrnMdl_ResetTripInstruc_ResetTripAInstruc = FALSE;
  Rte_ResetTripWrnMdl_ppSR_ResetTripWrnMdl_ResetTripInstruc_ResetTripBInstruc = FALSE;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_DPFChime = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_SCRChime_2hz_1 = 0U;
  Rte_SCRWrnMdl_ppSR_SCRWrnMdl_GdtCtrl_State_GdtCtrl_State = 0U;
  Rte_SCRWrnMdl_ppSR_SCRWrnMdl_TimeData_SpeedLimtHour = 0U;
  Rte_SCRWrnMdl_ppSR_SCRWrnMdl_TimeData_SpeedLimtMin = 0U;
  Rte_SCRWrnMdl_ppSR_SCRWrnMdl_TimeData_TroqLimtHour = 0U;
  Rte_SCRWrnMdl_ppSR_SCRWrnMdl_TimeData_TroqLimtMin = 0U;
  Rte_SIAMdl_ppSR_CANMSG_IC2_ComOut_IC_MaintenceRemind = 0U;
  Rte_SIAMdl_ppSR_CANMSG_IC2_ComOut_IC_WaterTempHighRemind = 0U;
  Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_AvgFuelConsumed = 0;
  Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_CruisRange = 0;
  Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_MaintainRemind = 0U;
  Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_MaintenanceLogo = 0U;
  Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_MaintenanceMile = 0;
  Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_MaintenceExpLogo = 0U;
  Rte_SIAMdl_ppSR_CANMSG_IC_Info_ComOut_IC_Trip = 0;
  Rte_SIAMdl_ppSR_Sia_to_TT_st = 0U;
  Rte_SIAMdl_ppSR_pSIAMdl_TISIAMdl_Value_Out_MRDisplayValue_MR_Display_Value = 0;
  Rte_SIAMdl_ppSR_pSIAMdl_TISIAMdl_Value_Out_WarningMRStatus_Warning_MR_Status = 0U;
  Rte_STTWrnMdl_rpSR_SttWrnMdl_EMS_StartStopMessage_Status = 0U;
  Rte_SeatBeltMdl_pp_Seatbelt_TT_Status_Drv_Status = 0U;
  Rte_SeatBeltMdl_pp_Seatbelt_TT_Status_Pas_Status = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_DrvSeatbelt = 0U;
  Rte_ChimeMdl_rpSR_TIChimeMdl_Recv_ChimeId_PasSeatbelt = 0U;
  Rte_SpdMdl_ppSR_TISpdMdl_Real_Value = 0U;
  Rte_SpdMdl_ppSR_TISpdMdl_Send_DigitalValue_Value = 0U;
  Rte_SpdMdl_tiSR_TISpdMdl_Send_value = 0U;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FL_State_FL_Tire_P_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FL_State_FL_Tire_Pressure = 0U;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FL_State_FL_Tire_SYS_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FL_State_FL_Tire_T_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FL_State_FL_Tire_Temp = 0;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FR_State_FR_Tire_P_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FR_State_FR_Tire_Pressure = 0U;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FR_State_FR_Tire_SYS_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FR_State_FR_Tire_T_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_FR_State_FR_Tire_Temp = 0;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RL_State_RL_Tire_P_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RL_State_RL_Tire_Pressure = 0U;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RL_State_RL_Tire_SYS_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RL_State_RL_Tire_T_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RL_State_RL_Tire_Temp = 0;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RR_State_RR_Tire_P_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RR_State_RR_Tire_Pressure = 0U;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RR_State_RR_Tire_SYS_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RR_State_RR_Tire_T_status = FALSE;
  Rte_TPMSRoutineMdl_ppSR_TPMSRoutineMdl_RR_State_RR_Tire_Temp = 0;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FL_Pressure_Value = 0U;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FL_Temp_Value = 0U;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FL_Warning_St = 0;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FR_Pressure_Value = 0U;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FR_Temp_Value = 0U;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_FR_Warning_St = 0;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RL_Pressure_Value = 0U;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RL_Temp_Value = 0U;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RL_Warning_St = 0;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RR_Pressure_Value = 0U;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RR_Temp_Value = 0U;
  Rte_TPMSWrnMdl_ppSR_TIGdt_TPMSWrnMdl_RR_Warning_St = 0;
  Rte_TachMdl_tiSR_TITachMdl_Send_value = 0U;
  Rte_ComVnimMdl_rpSR_CANMSG_IC_0x392_ComOut_IC_AverageSpeed = 0U;
  Rte_TripMdl_ppSR_TripMdl_TripKiloMeters_TripA = 0U;
  Rte_TripMdl_ppSR_TripMdl_TripKiloMeters_TripB = 0U;
  Rte_TripMdl_ppsr_average_speed_average_speed_value = 0U;
  Rte_TripMdl_ppsr_odo_average_speed_odoaveragespeed = 0U;
  Rte_TtMdl_ppSR_TITtMdl_ProveoutDone_ValueDone = FALSE;
  Rte_TtView_tiSR_PosLightMd_Send_Value = 0ULL;
  Rte_AppCtrl_rpSR_rKeyPos_TIModMdl_KeyPos_KeyPos = 0U;
  Rte_CDD_Test_tiSR_TIModMdl_KeyPos_KeyPos = 0U;
  Rte_EcumExt_rpSR_rKeyPos_TIModMdl_KeyPos_KeyPos = 0U;
  Rte_OdoMdl_rpSR_rOdoLogicalMdl_TIModMdl_KeyPos_KeyPos = 0U;

  Rte_OsApplication_ClientIdleFlags.Rte_Idle_GdtCdd_TIGdtCtrl_RenderDone = 1;

}


/**********************************************************************************************************************
 * Internal/External Tx connections
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_AFCMdl_pprpNV_AFCMdl_Accumulate_Value(P2CONST(NV_Data_AFC_Accumulate, AUTOMATIC, RTE_AFCMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_AFCMdl_pprpNV_AFCMdl_Accumulate_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_AFC_Accumulate = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_AFC_Accumulate = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_AFCMdl_pprpNV_AFCMdl_Accumulate_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_AFCMdl_pprpNV_AFCMdl_Refuel_Value(P2CONST(NV_Data_AFC_Refuel, AUTOMATIC, RTE_AFCMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_AFCMdl_pprpNV_AFCMdl_Refuel_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_AFC_Refuel = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_AFC_Refuel = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_AFCMdl_pprpNV_AFCMdl_Refuel_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_AFCMdl_pprpNV_AFCMdl_SingleAFC_SingleAFCValue(P2CONST(NV_Data_SingleAFC, AUTOMATIC, RTE_AFCMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_AFCMdl_pprpNV_AFCMdl_SingleAFC_SingleAFCValue_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_SingleAFC = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_SingleAFC = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_AFCMdl_pprpNV_AFCMdl_SingleAFC_SingleAFCValue_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_AmbientMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value(P2CONST(NV_Data_MPC_Day_Time, AUTOMATIC, RTE_AMBIENTMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_AmbientMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_AmbientMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ClockMdl_NV_AmbientMdl_MPC_Date_Time_Value(P2CONST(NV_Data_MPC_Day_Time, AUTOMATIC, RTE_CLOCKMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ClockMdl_NV_AmbientMdl_MPC_Date_Time_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_ClockMdl_NV_AmbientMdl_MPC_Date_Time_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ClockMdl_ppSR_ClockMdl_Rtc_Time_Value(P2CONST(Rtc_TimeType, AUTOMATIC, RTE_CLOCKMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ClockMdl_ppSR_ClockMdl_Rtc_Time_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ClockMdl_ppSR_ClockMdl_Rtc_Time_Value = *(data);
  Rte_WriteHook_ClockMdl_ppSR_ClockMdl_Rtc_Time_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x386_ComOut_IC_BSDSoundSts(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x386_ComOut_IC_BSDSoundSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_BSDSoundSts_980680e1_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_BSDSoundSts_980680e1, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x386_ComOut_IC_BSDSoundSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x392_ComOut_IC_AverageSpeed(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x392_ComOut_IC_AverageSpeed_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_AverageSpeed_0b379a58_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_AverageSpeed_0b379a58, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x392_ComOut_IC_AverageSpeed_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x392_ComOut_IC_ChangOilDistance(UInt16 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x392_ComOut_IC_ChangOilDistance_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_ChangOilDistance_0b379a58_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_ChangOilDistance_0b379a58, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x392_ComOut_IC_ChangOilDistance_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x392_ComOut_IC_Fuel_level(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x392_ComOut_IC_Fuel_level_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_Fuel_level_0b379a58_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_Fuel_level_0b379a58, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x392_ComOut_IC_Fuel_level_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_AirbagTelltaleBehavior(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_AirbagTelltaleBehavior_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_AirbagTelltaleBehavior_ffd44f7a_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_AirbagTelltaleBehavior_ffd44f7a, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_AirbagTelltaleBehavior_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(UInt32 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_OdometerMasterValue_ffd44f7a_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_OdometerMasterValue_ffd44f7a, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_QDashACCFail(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_QDashACCFail_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_QDashACCFail_ffd44f7a_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_QDashACCFail_ffd44f7a, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_QDashACCFail_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_VehSpd_HMI(uint16 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_VehSpd_HMI_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_VehSpd_HMI_ffd44f7a_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_VehSpd_HMI_ffd44f7a, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x510_ComOut_IC_VehSpd_HMI_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_AFC(uint16 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_AFC_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_AFC_8eb97ddb_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_AFC_8eb97ddb, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_AFC_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_AlarmOrNot(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_AlarmOrNot_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_AlarmOrNot_8eb97ddb_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_AlarmOrNot_8eb97ddb, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_AlarmOrNot_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_AlarmStatus(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_AlarmStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_AlarmStatus_8eb97ddb_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_AlarmStatus_8eb97ddb, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_AlarmStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_DTE(uint16 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_DTE_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_DTE_8eb97ddb_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_DTE_8eb97ddb, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_DTE_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn(uint16 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_Fuel_Level_Volumn_8eb97ddb_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_Fuel_Level_Volumn_8eb97ddb, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_Volumn_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_VolumnQF(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_VolumnQF_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_Fuel_Level_VolumnQF_8eb97ddb_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_Fuel_Level_VolumnQF_8eb97ddb, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_Fuel_Level_VolumnQF_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_ManRgen_D_Rq(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_ManRgen_D_Rq_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_ManRgen_D_Rq_8eb97ddb_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_ManRgen_D_Rq_8eb97ddb, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_0x525_ComOut_IC_ManRgen_D_Rq_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Date(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Date_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_Date_afccb5cc_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_Date_afccb5cc, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Date_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Hour(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Hour_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_Hour_afccb5cc_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_Hour_afccb5cc, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Hour_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Minute(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Minute_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_Minute_afccb5cc_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_Minute_afccb5cc, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Minute_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Month(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Month_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_Month_afccb5cc_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_Month_afccb5cc, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Month_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Second(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Second_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_Second_afccb5cc_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_Second_afccb5cc, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Second_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Year(uint16 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Year_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_ComHook_IC_Year_afccb5cc_SigTx(&data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  ret |= Com_SendSignal(ComConf_ComSignal_IC_Year_afccb5cc, RTE_COM_SENDCAST(P2VAR(void, AUTOMATIC, RTE_COMVNIMMDL_APPL_DATA))(&data)); /* PRQA S 0850 */ /* MD_MSR_19.8 */
  Rte_WriteHook_ComVnimMdl_ppSR_CANMSG_IC_time_0x581_ComOut_IC_Year_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DteMdl_pprpNV_DTEMdl_RAFC_FK_Value(P2CONST(NV_Data_DTE, AUTOMATIC, RTE_DTEMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DteMdl_pprpNV_DTEMdl_RAFC_FK_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_DTE = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_DTE = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DteMdl_pprpNV_DTEMdl_RAFC_FK_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_AmbientMdl_MPC_Date_Time_Value(P2CONST(NV_Data_MPC_Day_Time, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_AmbientMdl_MPC_Date_Time_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_AmbientMdl_MPC_Date_Time_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Assembly_Manufacturing_Date_Assembly_Manufacturing_Date(P2CONST(NV_Assembly_Manufacturing_Date, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Assembly_Manufacturing_Date_Assembly_Manufacturing_Date_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Manufacturing_Date, *(data), sizeof(NV_Assembly_Manufacturing_Date));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Manufacturing_Date = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Assembly_Manufacturing_Date_Assembly_Manufacturing_Date_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Assembly_Plant_Number_Assembly_Plant_Number(P2CONST(NV_Assembly_Plant_Number_10Byte, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Assembly_Plant_Number_Assembly_Plant_Number_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Plant_Number, *(data), sizeof(NV_Assembly_Plant_Number_10Byte));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Plant_Number = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Assembly_Plant_Number_Assembly_Plant_Number_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Cluster_ECUManufacturingDate_ManufacturingDate(P2CONST(NV_ECUManufacturingDate_4Byte, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Cluster_ECUManufacturingDate_ManufacturingDate_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUManufacturingDate, *(data), sizeof(NV_ECUManufacturingDate_4Byte));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUManufacturingDate = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Cluster_ECUManufacturingDate_ManufacturingDate_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Cluster_ECUSerialNumber_ECUSerialNumber(P2CONST(NV_ECUSerialNumber_14Byte, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Cluster_ECUSerialNumber_ECUSerialNumber_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUSerialNumber, *(data), sizeof(NV_ECUSerialNumber_14Byte));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUSerialNumber = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Cluster_ECUSerialNumber_ECUSerialNumber_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Cluster_ProductPartNumber_ProductPartNumber(P2CONST(NV_ECUProductPartNumber_12Byte, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Cluster_ProductPartNumber_ProductPartNumber_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ProductPartNumber, *(data), sizeof(NV_ECUProductPartNumber_12Byte));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ProductPartNumber = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Cluster_ProductPartNumber_ProductPartNumber_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Cluster_SupplierIdentifier_SupplierIdentifier(P2CONST(NV_ECUSupplierIdentifier_10Byte, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Cluster_SupplierIdentifier_SupplierIdentifier_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SupplierIdentifier, *(data), sizeof(NV_ECUSupplierIdentifier_10Byte));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SupplierIdentifier = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Cluster_SupplierIdentifier_SupplierIdentifier_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Cluster_VIN_Number_VIN_Number(P2CONST(NV_VIN_Number_17Bytes_Type, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Cluster_VIN_Number_VIN_Number_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_NvBlockSwComponentType_NVBlockDescriptor_ClusterVINNumber, *(data), sizeof(NV_VIN_Number_17Bytes_Type));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ClusterVINNumber = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Cluster_VIN_Number_VIN_Number_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Customer_Reference_Customer_Reference(P2CONST(NV_Assembly_Plant_Number_10Byte, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Customer_Reference_Customer_Reference_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Customer_Reference, *(data), sizeof(NV_Assembly_Plant_Number_10Byte));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Customer_Reference = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Customer_Reference_Customer_Reference_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_DcmExt_JMCHardwareVersionNumber_VersionNumber(P2CONST(NV_JMCHardwareVersionNumber_24Bytes_Type, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_DcmExt_JMCHardwareVersionNumber_VersionNumber_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_NvBlockSwComponentType_NVBlockDescriptor_JMCHardwareVersionNumber, *(data), sizeof(NV_JMCHardwareVersionNumber_24Bytes_Type));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_JMCHardwareVersionNumber = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_DcmExt_JMCHardwareVersionNumber_VersionNumber_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_ECUSerialNumber_Value(P2CONST(NvM_ECUSerialNumberType, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_ECUSerialNumber_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_NvBlockSwComponentType_NVBlockDescriptor_EOLData.ECUSerialNumber, *(data), sizeof(NvM_ECUSerialNumberType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_EOLData = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_ECUSerialNumber_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_ETM_Timeouts_ETM_ENTER(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_ETM_Timeouts_ETM_ENTER_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_ClusterSettings.ETM_Enter = *(&data);
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ClusterSettings = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_ETM_Timeouts_ETM_ENTER_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_ETM_Timeouts_ETM_ENTRY(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_ETM_Timeouts_ETM_ENTRY_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_ClusterSettings.ETM_Entry = *(&data);
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ClusterSettings = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_ETM_Timeouts_ETM_ENTRY_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_ETM_Timeouts_ETM_EXIT(UInt8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_ETM_Timeouts_ETM_EXIT_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_ClusterSettings.ETM_Exit = *(&data);
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ClusterSettings = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_ETM_Timeouts_ETM_EXIT_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference(P2CONST(NV_VIN_Number_17Bytes_Type, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference, *(data), sizeof(NV_VIN_Number_17Bytes_Type));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference_WithoutEEprom(P2CONST(NV_PCBA_Reference_17Bytes_Type, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference_WithoutEEprom_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference_WithoutEEprom, *(data), sizeof(NV_PCBA_Reference_17Bytes_Type));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference_WithoutEEprom = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference_WithoutEEprom_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_HardwareVersionNumber_VersionNumber(P2CONST(NV_ECUHardwareVersion, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_HardwareVersionNumber_VersionNumber_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_NvBlockSwComponentType_NVBlockDescriptor_HardwareVersionNumber, *(data), sizeof(NV_ECUHardwareVersion));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_HardwareVersionNumber = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_HardwareVersionNumber_VersionNumber_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_PartNumber_Data_PartNumberData(P2CONST(NV_ECUProductPartNumber_12Byte, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_PartNumber_Data_PartNumberData_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescripto_PartNumber_Data, *(data), sizeof(NV_ECUProductPartNumber_12Byte));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescripto_PartNumber_Data = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_PartNumber_Data_PartNumberData_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Run_In_mode_Control_Run_In_mode_Control(P2CONST(NV_Run_In_mode_Control, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Run_In_mode_Control_Run_In_mode_Control_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Run_In_mode_Control, *(data), sizeof(NV_Run_In_mode_Control));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Run_In_mode_Control = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Run_In_mode_Control_Run_In_mode_Control_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_SMD_Manufacturing_Date_SMD_Manufacturing_Date(P2CONST(NV_SMD_Manufacturing_Date, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_SMD_Manufacturing_Date_SMD_Manufacturing_Date_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_SMD_Manufacturing_Date, *(data), sizeof(NV_SMD_Manufacturing_Date));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_SMD_Manufacturing_Date = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_SMD_Manufacturing_Date_SMD_Manufacturing_Date_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_SMD_Plant_Number_SMD_Plant_Number(P2CONST(SMD_Plant_Number_3Byte, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_SMD_Plant_Number_SMD_Plant_Number_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_SMD_Plant_Number, *(data), sizeof(SMD_Plant_Number_3Byte));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_SMD_Plant_Number = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_SMD_Plant_Number_SMD_Plant_Number_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Serial_Number_PCBA_Serial_Number_PCBA(P2CONST(Serial_Number_PCBA_5Byte, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Serial_Number_PCBA_Serial_Number_PCBA_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Serial_Number_PCBA, *(data), sizeof(Serial_Number_PCBA_5Byte));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Serial_Number_PCBA = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Serial_Number_PCBA_Serial_Number_PCBA_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Vehicle_Configure_VehicleConfigSetArray(P2CONST(NV_Cluster_Vehicle_Configure, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Vehicle_Configure_VehicleConfigSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_NvBlockSwComponentType_NVBlockDescriptor_Vehicle_Configure, *(data), sizeof(NV_Cluster_Vehicle_Configure));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Vehicle_Configure = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Vehicle_Configure_VehicleConfigSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_Visteon_Product_Reference_Product_Reference(P2CONST(NV_VIN_Number_17Bytes_Type, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_Visteon_Product_Reference_Product_Reference_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_NvBlockSwComponentType_NVBlockDescriptor_Product_Reference, *(data), sizeof(NV_VIN_Number_17Bytes_Type));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Product_Reference = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_Visteon_Product_Reference_Product_Reference_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_station_Tracebility_station_Tracebility(P2CONST(NV_station_Tracebility, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_station_Tracebility_station_Tracebility_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_station_Tracebility, *(data), sizeof(NV_station_Tracebility));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_station_Tracebility = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_station_Tracebility_station_Tracebility_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_NV_user_product_configuration_UserProductConfiguration(P2CONST(NV_user_product_configuration, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_NV_user_product_configuration_UserProductConfiguration_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_NvBlockSwComponentType_NVBlockDescriptor_user_product_configuration, *(data), sizeof(NV_user_product_configuration));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_user_product_configuration = 1;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DcmExt_NV_user_product_configuration_UserProductConfiguration_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value(P2CONST(DCM_IOC_Type, AUTOMATIC, RTE_DCMEXT_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value = *(data);
  Rte_WriteHook_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_DimDerateMdl_NV_Dim_NvMDimData_DimDataSetArray(P2CONST(NVDimDataSetType, AUTOMATIC, RTE_DIMDERATEMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_DimDerateMdl_NV_Dim_NvMDimData_DimDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Dim_Buffer, *(data), sizeof(NVDimDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Dim_Buffer = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_DimDerateMdl_NV_Dim_NvMDimData_DimDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_EasyNaviMdl_ppSR_TIGDT_MP5_0x385_NaviLaodValueBuf(P2CONST(NaviRaodName, AUTOMATIC, RTE_EASYNAVIMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_EasyNaviMdl_ppSR_TIGDT_MP5_0x385_NaviLaodValueBuf_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_MemCpy32(Rte_EasyNaviMdl_ppSR_TIGDT_MP5_0x385_NaviLaodValueBuf, *(data), sizeof(NaviRaodName));
  Rte_WriteHook_EasyNaviMdl_ppSR_TIGDT_MP5_0x385_NaviLaodValueBuf_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_FuelLevelMdl_pprpNV_FuelLevelMdl_FuelLevel_FuelLevelValue(P2CONST(NV_Data_Fuel_Level, AUTOMATIC, RTE_FUELLEVELMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_FuelLevelMdl_pprpNV_FuelLevelMdl_FuelLevel_FuelLevelValue_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_FuelLevel = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_FuelLevel = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_FuelLevelMdl_pprpNV_FuelLevelMdl_FuelLevel_FuelLevelValue_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_GdtCtrl_pprpNV_GdtCtrl_Menu_Settings_Value(P2CONST(NV_Data_Menu_Settings, AUTOMATIC, RTE_GDTCTRL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_GdtCtrl_pprpNV_GdtCtrl_Menu_Settings_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_Menu_Settings = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Menu_Settings = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_GdtCtrl_pprpNV_GdtCtrl_Menu_Settings_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_NVMExtMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value(P2CONST(NV_Data_Cluster_Optional_Feature_Configure, AUTOMATIC, RTE_NVMEXTMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Optional_Feature_Configure = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Optional_Feature_Configure = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_NVMExtMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_NVMExtMdl_NV_Dim_NvMOdoData_DimDataSetArray(P2CONST(NVDimDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_NV_Dim_NvMOdoData_DimDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Dim_Buffer, *(data), sizeof(NVDimDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Dim_Buffer = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_NVMExtMdl_NV_Dim_NvMOdoData_DimDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(P2CONST(NVOdoDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer0, *(data), sizeof(NVOdoDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer0 = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(P2CONST(NVOdoDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer1, *(data), sizeof(NVOdoDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer1 = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(P2CONST(NVOdoDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer2, *(data), sizeof(NVOdoDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer2 = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(P2CONST(NVOdoDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer3, *(data), sizeof(NVOdoDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer3 = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(P2CONST(NVOdoDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer4, *(data), sizeof(NVOdoDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer4 = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Offset_Value(P2CONST(NV_Data_Cluster_SIA_Reset_Odo_OffSet, AUTOMATIC, RTE_NVMEXTMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Offset_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Offset_Value = *(data);
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Offset_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Store_Value(P2CONST(NV_Data_Cluster_SIA_Reset_Odo_OffSet, AUTOMATIC, RTE_NVMEXTMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Store_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Store_Value = *(data);
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Store_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_NVMExtMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value(P2CONST(NV_Data_MPC_Day_Time, AUTOMATIC, RTE_NVMEXTMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NVMExtMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value = *(data);
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_NVMExtMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_NV_Cluster_ODO_Backup_Counter_ODO_Backup_Value(P2CONST(NV_Data_Cluster_ODO_Backup_Counter, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_Cluster_ODO_Backup_Counter_ODO_Backup_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_ODO_Backup_Counter = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ODO_Backup_Counter = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_OdoMdl_NV_Cluster_ODO_Backup_Counter_ODO_Backup_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_NV_Cluster_Variable_Block_Cluster_Variable_Block(P2CONST(NV_Data_Cluster_Variable_Block, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_Cluster_Variable_Block_Cluster_Variable_Block_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Variable_Block = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Variable_Block = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_OdoMdl_NV_Cluster_Variable_Block_Cluster_Variable_Block_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_NV_OdoMdl_NvMOdoData_OdoDataSetArray(P2CONST(OdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_OdoMdl_NV_OdoMdl_NvMOdoData_OdoDataSetArray, *(data), sizeof(OdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(P2CONST(NVOdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer0, *(data), sizeof(NVOdoDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer0 = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(P2CONST(NVOdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer1, *(data), sizeof(NVOdoDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer1 = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(P2CONST(NVOdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer2, *(data), sizeof(NVOdoDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer2 = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(P2CONST(NVOdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer3, *(data), sizeof(NVOdoDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer3 = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(P2CONST(NVOdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer4, *(data), sizeof(NVOdoDataSetType));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer4 = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_NV_OdoMdl_Reset_OffSet_Offset_Value(P2CONST(NV_Data_Cluster_Reset_OffSet, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_Reset_OffSet_Offset_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_OdoMdl_NV_OdoMdl_Reset_OffSet_Offset_Value = *(data);
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_OdoMdl_Reset_OffSet_Offset_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_NV_TripMdl_TripA_value(P2CONST(NV_Data_Trip_A, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_TripMdl_TripA_value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_TripA = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_TripA = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_OdoMdl_NV_TripMdl_TripA_value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_NV_TripMdl_TripB_value(P2CONST(NV_Data_Trip_B, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_NV_TripMdl_TripB_value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_TripB = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_TripB = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_OdoMdl_NV_TripMdl_TripB_value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_pprpNV_OdoMdl_ODO_Reset_Counter_ResetCounterValue(P2CONST(NV_Data_ODO_Reset_Counter, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_pprpNV_OdoMdl_ODO_Reset_Counter_ResetCounterValue_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_ODO_Reset_Counter = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ODO_Reset_Counter = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_OdoMdl_pprpNV_OdoMdl_ODO_Reset_Counter_ResetCounterValue_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_OdoMdl_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface(P2CONST(OdoMdl_RW_Interface_Type, AUTOMATIC, RTE_ODOMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_OdoMdl_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_OdoMdl_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface = *(data);
  Rte_WriteHook_OdoMdl_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_PtrBaseView_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP(P2CONST(NV_Data_ClusterStpSettings, AUTOMATIC, RTE_PTRBASEVIEW_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_PtrBaseView_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_Stp_configData = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Stp_configData = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_PtrBaseView_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_PtrBaseView_NV_Cluster_Interpolations_Cluster_Interpolations(P2CONST(NV_Data_Interpolations, AUTOMATIC, RTE_PTRBASEVIEW_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_PtrBaseView_NV_Cluster_Interpolations_Cluster_Interpolations_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_Stp_angle_convertData = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Stp_angle_convertData = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_PtrBaseView_NV_Cluster_Interpolations_Cluster_Interpolations_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_SIAMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value(P2CONST(NV_Data_Cluster_Optional_Feature_Configure, AUTOMATIC, RTE_SIAMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_SIAMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Optional_Feature_Configure = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Optional_Feature_Configure = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_SIAMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_SIAMdl_pprpNV_Cluster_Maintenance_Maintenance_Value(P2CONST(NV_Data_Cluster_Maintenance, AUTOMATIC, RTE_SIAMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_SIAMdl_pprpNV_Cluster_Maintenance_Maintenance_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_SIAMdl_pprpNV_Cluster_Maintenance_Maintenance_Value = *(data);
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_SIAMdl_pprpNV_Cluster_Maintenance_Maintenance_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Offset_Value(P2CONST(NV_Data_Cluster_SIA_Reset_Odo_OffSet, AUTOMATIC, RTE_SIAMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Offset_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SIA_Reset_Odo_OffSet = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SIA_Reset_Odo_OffSet = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Offset_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Store_Value(P2CONST(NV_Data_Cluster_SIA_Reset_Odo_OffSet, AUTOMATIC, RTE_SIAMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Store_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Store_Value = *(data);
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Store_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_SeatBeltMdl_pp_Seatbelt_TT_Status_Drv_Status(uint8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_SeatBeltMdl_pp_Seatbelt_TT_Status_Drv_Status_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_SeatBeltMdl_pp_Seatbelt_TT_Status_Drv_Status = *(&data);
  Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SeatBeltMdl_pp_Seatbelt_TT_Status_Drv_Status = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_SeatBeltMdl_pp_Seatbelt_TT_Status_Drv_Status_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_SeatBeltMdl_pp_Seatbelt_TT_Status_Pas_Status(uint8 data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_SeatBeltMdl_pp_Seatbelt_TT_Status_Pas_Status_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_SeatBeltMdl_pp_Seatbelt_TT_Status_Pas_Status = *(&data);
  Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SeatBeltMdl_pp_Seatbelt_TT_Status_Pas_Status = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_SeatBeltMdl_pp_Seatbelt_TT_Status_Pas_Status_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_StpCddMdl_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP(P2CONST(NV_Data_ClusterStpSettings, AUTOMATIC, RTE_STPCDDMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_StpCddMdl_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_Stp_configData = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Stp_configData = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_StpCddMdl_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_StpCddMdl_NV_Cluster_Interpolations_Cluster_Interpolations(P2CONST(NV_Data_Interpolations, AUTOMATIC, RTE_STPCDDMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_StpCddMdl_NV_Cluster_Interpolations_Cluster_Interpolations_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_Stp_angle_convertData = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Stp_angle_convertData = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_StpCddMdl_NV_Cluster_Interpolations_Cluster_Interpolations_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_TripMdl_pprpNV_TripMdl_TripA_value(P2CONST(NV_Data_Trip_A, AUTOMATIC, RTE_TRIPMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_TripMdl_pprpNV_TripMdl_TripA_value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_TripA = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_TripA = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_TripMdl_pprpNV_TripMdl_TripA_value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_TripMdl_pprpNV_TripMdl_TripB_value(P2CONST(NV_Data_Trip_B, AUTOMATIC, RTE_TRIPMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_TripMdl_pprpNV_TripMdl_TripB_value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_NvBlockSwComponentType_NVBlockDescriptor_TripB = *(data);
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_TripB = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_TripMdl_pprpNV_TripMdl_TripB_value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Write_VehicleCfgMdl_NV_Vehicle_Configure_VehicleConfigSetArray(P2CONST(NV_Cluster_Vehicle_Configure, AUTOMATIC, RTE_VEHICLECFGMDL_APPL_DATA) data) /* PRQA S 0850, 1505 */ /* MD_MSR_19.8, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_WRITE_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_WriteHook_VehicleCfgMdl_NV_Vehicle_Configure_VehicleConfigSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(Rte_NvBlockSwComponentType_NVBlockDescriptor_Vehicle_Configure, *(data), sizeof(NV_Cluster_Vehicle_Configure));
  Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Vehicle_Configure = 1;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3112 */ /* MD_Rte_3112 */
  /* scheduled trigger for runnables: NvBlockSwComponentTypeRunnable */
  (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable); /* PRQA S 3417 */ /* MD_Rte_Os */
  Rte_WriteHook_VehicleCfgMdl_NV_Vehicle_Configure_VehicleConfigSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */


/**********************************************************************************************************************
 * Internal/External Rx connections
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_AFCMdl_pprpNV_AFCMdl_Accumulate_Value(P2VAR(NV_Data_AFC_Accumulate, AUTOMATIC, RTE_AFCMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_AFCMdl_pprpNV_AFCMdl_Accumulate_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_AFC_Accumulate;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_AFCMdl_pprpNV_AFCMdl_Accumulate_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_AFCMdl_pprpNV_AFCMdl_Refuel_Value(P2VAR(NV_Data_AFC_Refuel, AUTOMATIC, RTE_AFCMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_AFCMdl_pprpNV_AFCMdl_Refuel_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_AFC_Refuel;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_AFCMdl_pprpNV_AFCMdl_Refuel_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_AFCMdl_pprpNV_AFCMdl_SingleAFC_SingleAFCValue(P2VAR(NV_Data_SingleAFC, AUTOMATIC, RTE_AFCMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_AFCMdl_pprpNV_AFCMdl_SingleAFC_SingleAFCValue_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_SingleAFC;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_AFCMdl_pprpNV_AFCMdl_SingleAFC_SingleAFCValue_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_AmbientMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value(P2VAR(NV_Data_MPC_Day_Time, AUTOMATIC, RTE_AMBIENTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_AmbientMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_AmbientMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_AmbientMdl_rpSR_ClockMdl_Rtc_Time_Value(P2VAR(Rtc_TimeType, AUTOMATIC, RTE_AMBIENTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_AmbientMdl_rpSR_ClockMdl_Rtc_Time_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_ClockMdl_ppSR_ClockMdl_Rtc_Time_Value;
  Rte_ReadHook_AmbientMdl_rpSR_ClockMdl_Rtc_Time_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ChimeMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(P2VAR(DCM_IOC_Type, AUTOMATIC, RTE_CHIMEMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ChimeMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value;
  Rte_ReadHook_ChimeMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ClockMdl_NV_AmbientMdl_MPC_Date_Time_Value(P2VAR(NV_Data_MPC_Day_Time, AUTOMATIC, RTE_CLOCKMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ClockMdl_NV_AmbientMdl_MPC_Date_Time_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ClockMdl_NV_AmbientMdl_MPC_Date_Time_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_BrakeFluidSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_BrakeFluidSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_BrakeFluidSts_abc8cd44, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_BrakeFluidSts_abc8cd44_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_BrakeFluidSts_abc8cd44 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_BrakeFluidSts_abc8cd44 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_BrakeFluidSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_Mechanical_Hand_Brake(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_Mechanical_Hand_Brake_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_Mechanical_Hand_Brake_abc8cd44, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_Mechanical_Hand_Brake_abc8cd44_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_Mechanical_Hand_Brake_abc8cd44 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_Mechanical_Hand_Brake_abc8cd44 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_Mechanical_Hand_Brake_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_OutsideTemp(P2VAR(UInt16_01, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_OutsideTemp_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_OutsideTemp_abc8cd44, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_OutsideTemp_abc8cd44_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_OutsideTemp_abc8cd44 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_OutsideTemp_abc8cd44 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_OutsideTemp_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_PowerMode(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_PowerMode_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_PowerMode_abc8cd44, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_PowerMode_abc8cd44_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_PowerMode_abc8cd44 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_PowerMode_abc8cd44 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x313_ComIn_BCM_PowerMode_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_0x584_ComIn_BCM_CarModeIndicator(P2VAR(uint8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x584_ComIn_BCM_CarModeIndicator_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_CarModeIndicator_00b00241, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_CarModeIndicator_00b00241_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_CarModeIndicator_00b00241 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_CarModeIndicator_00b00241 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x584_ComIn_BCM_CarModeIndicator_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_0x584_ComIn_BCM_LoadManageLevel(P2VAR(uint8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x584_ComIn_BCM_LoadManageLevel_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_LoadManageLevel_00b00241, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_LoadManageLevel_00b00241_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_LoadManageLevel_00b00241 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_LoadManageLevel_00b00241 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_0x584_ComIn_BCM_LoadManageLevel_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_DoorStsFrntLe_0edb943f, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_DoorStsFrntLe_0edb943f_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_DoorStsFrntLe_0edb943f == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_DoorStsFrntLe_0edb943f != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_DoorStsFrntRi_0edb943f, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_DoorStsFrntRi_0edb943f_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_DoorStsFrntRi_0edb943f == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_DoorStsFrntRi_0edb943f != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearLe(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearLe_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_DoorStsRearLe_0edb943f, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_DoorStsRearLe_0edb943f_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_DoorStsRearLe_0edb943f == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_DoorStsRearLe_0edb943f != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearLe_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearRi(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearRi_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_DoorStsRearRi_0edb943f, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_DoorStsRearRi_0edb943f_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_DoorStsRearRi_0edb943f == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_DoorStsRearRi_0edb943f != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearRi_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsTrunk(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsTrunk_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_DoorStsTrunk_0edb943f, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_DoorStsTrunk_0edb943f_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_DoorStsTrunk_0edb943f == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_DoorStsTrunk_0edb943f != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsTrunk_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_EnginehoodSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_EnginehoodSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_EnginehoodSts_0edb943f, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_EnginehoodSts_0edb943f_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_EnginehoodSts_0edb943f == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_EnginehoodSts_0edb943f != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_EnginehoodSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BackHomeSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BackHomeSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_BackHomeSts_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_BackHomeSts_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_BackHomeSts_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_BackHomeSts_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BackHomeSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BrakelightDig(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BrakelightDig_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_BrakelightDig_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_BrakelightDig_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_BrakelightDig_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_BrakelightDig_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BrakelightDig_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_DayRunningLampDig(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_DayRunningLampDig_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_DayRunningLampDig_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_DayRunningLampDig_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_DayRunningLampDig_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_DayRunningLampDig_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_DayRunningLampDig_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_FrntFogLmpSts_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_FrntFogLmpSts_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_FrntFogLmpSts_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_FrntFogLmpSts_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_HighBeamSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_HighBeamSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_HighBeamSts_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_HighBeamSts_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_HighBeamSts_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_HighBeamSts_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_HighBeamSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_ImmoSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_ImmoSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_ImmoSts_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_ImmoSts_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_ImmoSts_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_ImmoSts_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_ImmoSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_LowBeamSts_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_LowBeamSts_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_LowBeamSts_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_LowBeamSts_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpDig(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpDig_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_PosLmpDig_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_PosLmpDig_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_PosLmpDig_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_PosLmpDig_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpDig_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_PosLmpSts_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_PosLmpSts_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_PosLmpSts_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_PosLmpSts_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpDig(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpDig_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_RearFogLmpDig_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_RearFogLmpDig_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_RearFogLmpDig_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_RearFogLmpDig_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpDig_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_RearFogLmpSts_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_RearFogLmpSts_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_RearFogLmpSts_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_RearFogLmpSts_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorDig(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorDig_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_TurnIndicatorDig_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_TurnIndicatorDig_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_TurnIndicatorDig_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_TurnIndicatorDig_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorDig_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_TurnIndicatorSts_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_TurnIndicatorSts_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_TurnIndicatorSts_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_TurnIndicatorSts_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_reversinglightDig(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_reversinglightDig_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_reversinglightDig_52444383, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_reversinglightDig_52444383_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_reversinglightDig_52444383 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_reversinglightDig_52444383 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_reversinglightDig_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_BCM_Sts_0x315_ComIn_BCM_BGLLumLvlSettingStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Sts_0x315_ComIn_BCM_BGLLumLvlSettingStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_BCM_BGLLumLvlSettingStatus_a1dcf71c, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_BCM_BGLLumLvlSettingStatus_a1dcf71c_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_BCM_BGLLumLvlSettingStatus_a1dcf71c == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_BCM_BGLLumLvlSettingStatus_a1dcf71c != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_BCM_Sts_0x315_ComIn_BCM_BGLLumLvlSettingStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_AbsFlgFlt(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_AbsFlgFlt_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ABS_AbsFlgFlt_2356a03c, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ABS_AbsFlgFlt_2356a03c_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ABS_AbsFlgFlt_2356a03c == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ABS_AbsFlgFlt_2356a03c != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_AbsFlgFlt_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_EbdFlgFlt(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_EbdFlgFlt_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ABS_EbdFlgFlt_2356a03c, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ABS_EbdFlgFlt_2356a03c_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ABS_EbdFlgFlt_2356a03c == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ABS_EbdFlgFlt_2356a03c != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_EbdFlgFlt_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(P2VAR(uint16, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ABS_VehSpdLgt_2356a03c, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ABS_VehSpdLgt_2356a03c_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ABS_VehSpdLgt_2356a03c == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ABS_VehSpdLgt_2356a03c != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLe(P2VAR(uint16, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLe_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ABS_WhlMilgFrntLe_2356a03c, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ABS_WhlMilgFrntLe_2356a03c_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ABS_WhlMilgFrntLe_2356a03c == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ABS_WhlMilgFrntLe_2356a03c != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLe_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLeStatus(P2VAR(ACTEMP_ENABLE_Type, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLeStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ABS_WhlMilgFrntLeStatus_2356a03c, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ABS_WhlMilgFrntLeStatus_2356a03c_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ABS_WhlMilgFrntLeStatus_2356a03c == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ABS_WhlMilgFrntLeStatus_2356a03c != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLeStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRi(P2VAR(uint16, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRi_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ABS_WhlMilgFrntRi_2356a03c, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ABS_WhlMilgFrntRi_2356a03c_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ABS_WhlMilgFrntRi_2356a03c == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ABS_WhlMilgFrntRi_2356a03c != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRi_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRiStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRiStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ABS_WhlMilgFrntRiStatus_2356a03c, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ABS_WhlMilgFrntRiStatus_2356a03c_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ABS_WhlMilgFrntRiStatus_2356a03c == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ABS_WhlMilgFrntRiStatus_2356a03c != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRiStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_DCM_GlowPlugLmpReq(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_DCM_GlowPlugLmpReq_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_DCM_GlowPlugLmpReq_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_DCM_GlowPlugLmpReq_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_DCM_GlowPlugLmpReq_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_DCM_GlowPlugLmpReq_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_DCM_GlowPlugLmpReq_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_DCM_WIFFullLmpReq(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_DCM_WIFFullLmpReq_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_DCM_WIFFullLmpReq_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_DCM_WIFFullLmpReq_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_DCM_WIFFullLmpReq_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_DCM_WIFFullLmpReq_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_DCM_WIFFullLmpReq_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_DieslPrtcWarn_D_Rq_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_DieslPrtcWarn_D_Rq_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_DieslPrtcWarn_D_Rq_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_DieslPrtcWarn_D_Rq_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_ManRgenTxt_D_RqDsply(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_ManRgenTxt_D_RqDsply_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_ManRgenTxt_D_RqDsply_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_ManRgenTxt_D_RqDsply_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_ManRgenTxt_D_RqDsply_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_ManRgenTxt_D_RqDsply_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_ManRgenTxt_D_RqDsply_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_Soot_Pc_Dsply(P2VAR(uint8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_Soot_Pc_Dsply_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_Soot_Pc_Dsply_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_Soot_Pc_Dsply_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_Soot_Pc_Dsply_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_Soot_Pc_Dsply_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_Soot_Pc_Dsply_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtHour(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtHour_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_SpeedLimtHour_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_SpeedLimtHour_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_SpeedLimtHour_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_SpeedLimtHour_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtHour_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtMin(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtMin_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_SpeedLimtMin_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_SpeedLimtMin_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_SpeedLimtMin_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_SpeedLimtMin_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_SpeedLimtMin_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtHour(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtHour_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_TroqLimtHour_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_TroqLimtHour_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_TroqLimtHour_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_TroqLimtHour_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtHour_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtMin(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtMin_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_TroqLimtMin_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_TroqLimtMin_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_TroqLimtMin_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_TroqLimtMin_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_TroqLimtMin_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaLvl(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaLvl_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_UreaLvl_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_UreaLvl_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_UreaLvl_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_UreaLvl_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaLvl_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaQty(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaQty_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_UreaQty_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_UreaQty_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_UreaQty_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_UreaQty_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaQty_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaSysWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaSysWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_UreaSysWarning_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_UreaSysWarning_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_UreaSysWarning_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_UreaSysWarning_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaSysWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaTxt(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaTxt_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_UreaTxt_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_UreaTxt_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_UreaTxt_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_UreaTxt_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaTxt_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaWarn(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaWarn_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_UreaWarn_daa19a1a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_UreaWarn_daa19a1a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_UreaWarn_daa19a1a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_UreaWarn_daa19a1a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaWarn_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x636_ComIn_EMS_BackupOdometer(P2VAR(UInt32, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x636_ComIn_EMS_BackupOdometer_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_BackupOdometer_63950623, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_BackupOdometer_63950623_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_BackupOdometer_63950623 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_BackupOdometer_63950623 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_0x636_ComIn_EMS_BackupOdometer_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_BatteryVoltageError(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_BatteryVoltageError_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_BatteryVoltageError_85187d93, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_BatteryVoltageError_85187d93_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_BatteryVoltageError_85187d93 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_BatteryVoltageError_85187d93 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_BatteryVoltageError_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_CluthPedalStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_CluthPedalStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_CluthPedalStatus_85187d93, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_CluthPedalStatus_85187d93_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_CluthPedalStatus_85187d93 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_CluthPedalStatus_85187d93 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_CluthPedalStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngCoolanTemp(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngCoolanTemp_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_EngCoolanTemp_85187d93, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_EngCoolanTemp_85187d93_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_EngCoolanTemp_85187d93 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_EngCoolanTemp_85187d93 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngCoolanTemp_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_EngOperationStatus_85187d93, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_EngOperationStatus_85187d93_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_EngOperationStatus_85187d93 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngOperationStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(P2VAR(uint16, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_EngSpeed_85187d93, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_EngSpeed_85187d93_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_EngSpeed_85187d93 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_EngSpeed_85187d93 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_Fuelcounter(P2VAR(uint16, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_Fuelcounter_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_Fuelcounter_daed4b4e, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_Fuelcounter_daed4b4e_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_Fuelcounter_daed4b4e == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_Fuelcounter_daed4b4e != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_Fuelcounter_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_MIL(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_MIL_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_MIL_daed4b4e, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_MIL_daed4b4e_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_MIL_daed4b4e == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_MIL_daed4b4e != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_MIL_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDTxt(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDTxt_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_OBDTxt_daed4b4e, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_OBDTxt_daed4b4e_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_OBDTxt_daed4b4e == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_OBDTxt_daed4b4e != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_OBDTxt_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_RemindShiftGear_MT(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_RemindShiftGear_MT_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_RemindShiftGear_MT_daed4b4e, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_RemindShiftGear_MT_daed4b4e_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_RemindShiftGear_MT_daed4b4e == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_RemindShiftGear_MT_daed4b4e != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_RemindShiftGear_MT_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_SVS(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_SVS_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_SVS_daed4b4e, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_SVS_daed4b4e_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_SVS_daed4b4e == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_SVS_daed4b4e != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_SVS_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_StartStopStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_StartStopStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_StartStopStatus_daed4b4e, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_StartStopStatus_daed4b4e_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_StartStopStatus_daed4b4e == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_StartStopStatus_daed4b4e != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Power_0x366_ComIn_EMS_StartStopStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_CruiseControlStatus_341d7c91, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_CruiseControlStatus_341d7c91_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_CruiseControlStatus_341d7c91 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_CruiseControlStatus_341d7c91 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_CruiseControlStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_TargCruiseSpeed(P2VAR(UInt16_01, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_TargCruiseSpeed_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EMS_TargCruiseSpeed_341d7c91, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EMS_TargCruiseSpeed_341d7c91_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EMS_TargCruiseSpeed_341d7c91 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EMS_TargCruiseSpeed_341d7c91 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EMS_Sts_Power_0x151_ComIn_EMS_TargCruiseSpeed_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_SafeLampRed(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_SafeLampRed_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EPS_S_SafeLampRed_e5742092, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EPS_S_SafeLampRed_e5742092_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EPS_S_SafeLampRed_e5742092 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EPS_S_SafeLampRed_e5742092 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_SafeLampRed_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_WarningLampYellow(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_WarningLampYellow_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_EPS_S_WarningLampYellow_e5742092, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_EPS_S_WarningLampYellow_e5742092_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_EPS_S_WarningLampYellow_e5742092 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_EPS_S_WarningLampYellow_e5742092 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_EPS_Chassis_0x260_ComIn_EPS_S_WarningLampYellow_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AVHWarningMessage(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AVHWarningMessage_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_AVHWarningMessage_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_AVHWarningMessage_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_AVHWarningMessage_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_AVHWarningMessage_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AVHWarningMessage_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhActive(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhActive_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_AvhActive_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_AvhActive_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_AvhActive_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_AvhActive_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhActive_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhAvabliable(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhAvabliable_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_AvhAvabliable_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_AvhAvabliable_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_AvhAvabliable_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_AvhAvabliable_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_AvhAvabliable_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_CdpAvailable(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_CdpAvailable_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_CdpAvailable_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_CdpAvailable_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_CdpAvailable_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_CdpAvailable_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_CdpAvailable_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_EPBErrorStatus_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_EPBErrorStatus_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_EPBErrorStatus_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_EPBErrorStatus_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBErrorStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_EPBStatus_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_EPBStatus_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_EPBStatus_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_EPBStatus_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage1(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage1_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_EPBWarningMessage1_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_EPBWarningMessage1_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_EPBWarningMessage1_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_EPBWarningMessage1_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage1_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage2(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage2_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_EPBWarningMessage2_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_EPBWarningMessage2_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_EPBWarningMessage2_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_EPBWarningMessage2_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage2_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage3(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage3_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_EPBWarningMessage3_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_EPBWarningMessage3_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_EPBWarningMessage3_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_EPBWarningMessage3_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EPBWarningMessage3_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ESPInfoLamp(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ESPInfoLamp_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_ESPInfoLamp_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_ESPInfoLamp_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_ESPInfoLamp_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_ESPInfoLamp_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_ESPInfoLamp_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EspEnableSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EspEnableSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_EspEnableSts_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_EspEnableSts_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_EspEnableSts_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_EspEnableSts_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_EspEnableSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_TcsFlgFlt(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_TcsFlgFlt_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_TcsFlgFlt_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_TcsFlgFlt_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_TcsFlgFlt_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_TcsFlgFlt_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_TcsFlgFlt_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_VdcFlgFlt(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_VdcFlgFlt_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ESP_VdcFlgFlt_af66671b, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ESP_VdcFlgFlt_af66671b_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ESP_VdcFlgFlt_af66671b == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ESP_VdcFlgFlt_af66671b != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_ESP_Status_Chassis_0x243_ComIn_ESP_VdcFlgFlt_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_IndexStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_IndexStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_DDD_IndexStatus_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_DDD_IndexStatus_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_DDD_IndexStatus_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_DDD_IndexStatus_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_IndexStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_WarningLevel(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_WarningLevel_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_DDD_WarningLevel_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_DDD_WarningLevel_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_DDD_WarningLevel_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_DDD_WarningLevel_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_DDD_WarningLevel_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_HMA_Status(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_HMA_Status_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_HMA_Status_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_HMA_Status_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_HMA_Status_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_HMA_Status_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_HMA_Status_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssitType(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssitType_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_LaneAssitType_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_LaneAssitType_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_LaneAssitType_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_LaneAssitType_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssitType_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_HandsonReq(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_HandsonReq_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_LaneAssit_HandsonReq_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_LaneAssit_HandsonReq_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_LaneAssit_HandsonReq_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_LaneAssit_HandsonReq_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_HandsonReq_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_LeftVisualization(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_LeftVisualization_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_LaneAssit_LeftVisualization_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_LaneAssit_LeftVisualization_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_LaneAssit_LeftVisualization_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_LaneAssit_LeftVisualization_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_LeftVisualization_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_RightVisualization(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_RightVisualization_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_LaneAssit_RightVisualization_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_LaneAssit_RightVisualization_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_LaneAssit_RightVisualization_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_LaneAssit_RightVisualization_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_RightVisualization_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_LaneAssit_Status_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_LaneAssit_Status_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_LaneAssit_Status_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_LaneAssit_Status_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_WarningAudio(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_WarningAudio_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_LaneAssit_WarningAudio_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_LaneAssit_WarningAudio_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_LaneAssit_WarningAudio_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_LaneAssit_WarningAudio_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_WarningAudio_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimit(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimit_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_SLASpdlimit_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_SLASpdlimit_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_SLASpdlimit_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_SLASpdlimit_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimit_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimitUnits(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimitUnits_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_SLASpdlimitUnits_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_SLASpdlimitUnits_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_SLASpdlimitUnits_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_SLASpdlimitUnits_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimitUnits_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLAState(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLAState_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_IPM_SLAState_32f79b14, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_IPM_SLAState_32f79b14_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_IPM_SLAState_32f79b14 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_IPM_SLAState_32f79b14 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLAState_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_CTA_OFFTelltale(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_CTA_OFFTelltale_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_CTA_OFFTelltale_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_CTA_OFFTelltale_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_CTA_OFFTelltale_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_CTA_OFFTelltale_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_CTA_OFFTelltale_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_DOA_OFFTelltale(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_DOA_OFFTelltale_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_DOA_OFFTelltale_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_DOA_OFFTelltale_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_DOA_OFFTelltale_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_DOA_OFFTelltale_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_DOA_OFFTelltale_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SODLCA_OFFTelltale(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SODLCA_OFFTelltale_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SODLCA_OFFTelltale_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SODLCA_OFFTelltale_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SODLCA_OFFTelltale_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SODLCA_OFFTelltale_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SODLCA_OFFTelltale_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_BLIS_display(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_BLIS_display_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SOD_BLIS_display_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SOD_BLIS_display_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SOD_BLIS_display_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SOD_BLIS_display_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_BLIS_display_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqLeft(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqLeft_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SOD_CTA_warningReqLeft_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SOD_CTA_warningReqLeft_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SOD_CTA_warningReqLeft_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SOD_CTA_warningReqLeft_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqLeft_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqRight(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqRight_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SOD_CTA_warningReqRight_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SOD_CTA_warningReqRight_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SOD_CTA_warningReqRight_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SOD_CTA_warningReqRight_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqRight_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftFront(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftFront_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SOD_DOA_warningReqLeftFront_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SOD_DOA_warningReqLeftFront_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SOD_DOA_warningReqLeftFront_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SOD_DOA_warningReqLeftFront_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftFront_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftRear(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftRear_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SOD_DOA_warningReqLeftRear_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SOD_DOA_warningReqLeftRear_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SOD_DOA_warningReqLeftRear_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SOD_DOA_warningReqLeftRear_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqLeftRear_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightFront(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightFront_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SOD_DOA_warningReqRightFront_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SOD_DOA_warningReqRightFront_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SOD_DOA_warningReqRightFront_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SOD_DOA_warningReqRightFront_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightFront_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightRear(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightRear_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SOD_DOA_warningReqRightRear_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SOD_DOA_warningReqRightRear_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SOD_DOA_warningReqRightRear_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SOD_DOA_warningReqRightRear_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_DOA_warningReqRightRear_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqLeft(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqLeft_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SOD_LCA_warningReqLeft_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SOD_LCA_warningReqLeft_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SOD_LCA_warningReqLeft_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SOD_LCA_warningReqLeft_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqLeft_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqRight(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqRight_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SOD_LCA_warningReqRight_a75280f1, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SOD_LCA_warningReqRight_a75280f1_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SOD_LCA_warningReqRight_a75280f1 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SOD_LCA_warningReqRight_a75280f1 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_LCA_warningReqRight_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x382_ComIn_MP5_BSDSoundSwitchSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x382_ComIn_MP5_BSDSoundSwitchSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MP5_BSDSoundSwitchSts_c90244fe, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MP5_BSDSoundSwitchSts_c90244fe_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MP5_BSDSoundSwitchSts_c90244fe == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MP5_BSDSoundSwitchSts_c90244fe != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x382_ComIn_MP5_BSDSoundSwitchSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_NavICON(P2VAR(uint8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_NavICON_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MP5_NavICON_0f6d4d79, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MP5_NavICON_0f6d4d79_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MP5_NavICON_0f6d4d79 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MP5_NavICON_0f6d4d79 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_NavICON_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUTE_REMAIN_DIS(P2VAR(uint32_least, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUTE_REMAIN_DIS_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MP5_ROUTE_REMAIN_DIS_0f6d4d79, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MP5_ROUTE_REMAIN_DIS_0f6d4d79_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MP5_ROUTE_REMAIN_DIS_0f6d4d79 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MP5_ROUTE_REMAIN_DIS_0f6d4d79 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUTE_REMAIN_DIS_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmOrNot(P2VAR(uint8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmOrNot_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MP5_AlarmOrNot_046cad19, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MP5_AlarmOrNot_046cad19_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MP5_AlarmOrNot_046cad19 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MP5_AlarmOrNot_046cad19 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmOrNot_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmStatus(P2VAR(uint8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MP5_AlarmStatus_046cad19, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MP5_AlarmStatus_046cad19_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MP5_AlarmStatus_046cad19 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MP5_AlarmStatus_046cad19 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MP5_0x530_ComIn_MP5_AlarmStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_ACCHMI_Mode_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_ACCHMI_Mode_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_ACCHMI_Mode_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_ACCHMI_Mode_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_latentWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_latentWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_FCW_latentWarning_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_FCW_latentWarning_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_FCW_latentWarning_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_FCW_latentWarning_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_latentWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_preWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_preWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_FCW_preWarning_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_FCW_preWarning_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_FCW_preWarning_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_FCW_preWarning_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_FCW_preWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_AEB_STATE(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_AEB_STATE_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MRR_AEB_STATE_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MRR_AEB_STATE_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MRR_AEB_STATE_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MRR_AEB_STATE_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_AEB_STATE_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_ObjValid(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_ObjValid_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MRR_ObjValid_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MRR_ObjValid_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MRR_ObjValid_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MRR_ObjValid_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_ObjValid_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_STATE(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_STATE_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MRR_PCW_STATE_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MRR_PCW_STATE_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MRR_PCW_STATE_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MRR_PCW_STATE_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_STATE_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_latentWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_latentWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MRR_PCW_latentWarning_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MRR_PCW_latentWarning_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MRR_PCW_latentWarning_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MRR_PCW_latentWarning_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_latentWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_preWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_preWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MRR_PCW_preWarning_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MRR_PCW_preWarning_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MRR_PCW_preWarning_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MRR_PCW_preWarning_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_preWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TakeOverReq(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TakeOverReq_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MRR_TakeOverReq_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MRR_TakeOverReq_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MRR_TakeOverReq_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MRR_TakeOverReq_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TakeOverReq_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TauGapSet(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TauGapSet_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MRR_TauGapSet_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MRR_TauGapSet_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MRR_TauGapSet_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MRR_TauGapSet_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TauGapSet_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_dxTarObj(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_dxTarObj_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_MRR_dxTarObj_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_MRR_dxTarObj_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_MRR_dxTarObj_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_MRR_dxTarObj_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_dxTarObj_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Textinfo(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Textinfo_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_Textinfo_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_Textinfo_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_Textinfo_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_Textinfo_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Textinfo_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_vSetDis(P2VAR(UInt16, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_vSetDis_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_vSetDis_255e8a04, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_vSetDis_255e8a04_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_vSetDis_255e8a04 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_vSetDis_255e8a04 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_vSetDis_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundCadence(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundCadence_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PAM_ChimeCmdSoundCadence_49f5ddcc, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PAM_ChimeCmdSoundCadence_49f5ddcc_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PAM_ChimeCmdSoundCadence_49f5ddcc == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PAM_ChimeCmdSoundCadence_49f5ddcc != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundCadence_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PAM_ChimeCmdSoundStatus_49f5ddcc, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PAM_ChimeCmdSoundStatus_49f5ddcc_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PAM_ChimeCmdSoundStatus_49f5ddcc == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PAM_ChimeCmdSoundStatus_49f5ddcc != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_ChimeCmdSoundStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_FrontSysTemWorkSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_FrontSysTemWorkSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PAM_FrontSysTemWorkSts_49f5ddcc, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PAM_FrontSysTemWorkSts_49f5ddcc_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PAM_FrontSysTemWorkSts_49f5ddcc == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PAM_FrontSysTemWorkSts_49f5ddcc != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_FrontSysTemWorkSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_RearSysTemWorkSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_RearSysTemWorkSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PAM_RearSysTemWorkSts_49f5ddcc, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PAM_RearSysTemWorkSts_49f5ddcc_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PAM_RearSysTemWorkSts_49f5ddcc == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PAM_RearSysTemWorkSts_49f5ddcc != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_PAM_0x271_ComIn_PAM_RearSysTemWorkSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_AirBagFailSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_AirBagFailSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SRS_AirBagFailSts_9595b6c2, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SRS_AirBagFailSts_9595b6c2_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SRS_AirBagFailSts_9595b6c2 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SRS_AirBagFailSts_9595b6c2 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_AirBagFailSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SRS_SBR_Driver_9595b6c2, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SRS_SBR_Driver_9595b6c2_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SRS_SBR_Driver_9595b6c2 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SRS_SBR_Driver_9595b6c2 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SRS_SBR_PAMsenger_9595b6c2, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SRS_SBR_PAMsenger_9595b6c2_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SRS_SBR_PAMsenger_9595b6c2 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SRS_SBR_PAMsenger_9595b6c2 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Confirm(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Confirm_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SWM_Menu_Confirm_f733f54d, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SWM_Menu_Confirm_f733f54d_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SWM_Menu_Confirm_f733f54d == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SWM_Menu_Confirm_f733f54d != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Confirm_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Down(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Down_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SWM_Menu_Down_f733f54d, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SWM_Menu_Down_f733f54d_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SWM_Menu_Down_f733f54d == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SWM_Menu_Down_f733f54d != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Down_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Left(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Left_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SWM_Menu_Left_f733f54d, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SWM_Menu_Left_f733f54d_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SWM_Menu_Left_f733f54d == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SWM_Menu_Left_f733f54d != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Left_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Right(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Right_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SWM_Menu_Right_f733f54d, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SWM_Menu_Right_f733f54d_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SWM_Menu_Right_f733f54d == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SWM_Menu_Right_f733f54d != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Right_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Up(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Up_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_SWM_Menu_Up_f733f54d, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_SWM_Menu_Up_f733f54d_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SWM_Menu_Up_f733f54d == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_SWM_Menu_Up_f733f54d != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Up_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Date(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Date_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TBOX_Date_4e88e95a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TBOX_Date_4e88e95a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TBOX_Date_4e88e95a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TBOX_Date_4e88e95a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Date_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Hour(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Hour_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TBOX_Hour_4e88e95a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TBOX_Hour_4e88e95a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TBOX_Hour_4e88e95a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TBOX_Hour_4e88e95a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Hour_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Minute(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Minute_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TBOX_Minute_4e88e95a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TBOX_Minute_4e88e95a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TBOX_Minute_4e88e95a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TBOX_Minute_4e88e95a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Minute_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Month(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Month_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TBOX_Month_4e88e95a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TBOX_Month_4e88e95a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TBOX_Month_4e88e95a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TBOX_Month_4e88e95a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Month_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_RealTime_UB_Copy_1(P2VAR(Boolean, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_RealTime_UB_Copy_1_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TBOX_RealTime_UB_Copy_1_4e88e95a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TBOX_RealTime_UB_Copy_1_4e88e95a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TBOX_RealTime_UB_Copy_1_4e88e95a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TBOX_RealTime_UB_Copy_1_4e88e95a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_RealTime_UB_Copy_1_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Second(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Second_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TBOX_Second_4e88e95a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TBOX_Second_4e88e95a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TBOX_Second_4e88e95a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TBOX_Second_4e88e95a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Second_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Year(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Year_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TBOX_Year_4e88e95a, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TBOX_Year_4e88e95a_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TBOX_Year_4e88e95a == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TBOX_Year_4e88e95a != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TBOX_time_0x582_ComIn_TBOX_Year_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TCU_DisplayGear_cb5c6a72, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TCU_DisplayGear_cb5c6a72_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TCU_DisplayGear_cb5c6a72 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TCU_DisplayGear_cb5c6a72 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_DisplayGear_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_GearSelectorReq(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_GearSelectorReq_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TCU_GearSelectorReq_cb5c6a72, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TCU_GearSelectorReq_cb5c6a72_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TCU_GearSelectorReq_cb5c6a72 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TCU_GearSelectorReq_cb5c6a72 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TCU_GearInfo_Power_0x123_ComIn_TCU_GearSelectorReq_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Driverwarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Driverwarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TCU_Driverwarning_94ca6b54, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TCU_Driverwarning_94ca6b54_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TCU_Driverwarning_94ca6b54 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TCU_Driverwarning_94ca6b54 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Driverwarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_DrvShiftAllwCondRmd(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_DrvShiftAllwCondRmd_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TCU_DrvShiftAllwCondRmd_94ca6b54, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TCU_DrvShiftAllwCondRmd_94ca6b54_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TCU_DrvShiftAllwCondRmd_94ca6b54 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TCU_DrvShiftAllwCondRmd_94ca6b54 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_DrvShiftAllwCondRmd_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TrnSrvcRqd_B_Rq(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TrnSrvcRqd_B_Rq_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TrnSrvcRqd_B_Rq_94ca6b54, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TrnSrvcRqd_B_Rq_94ca6b54_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TrnSrvcRqd_B_Rq_94ca6b54 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TrnSrvcRqd_B_Rq_94ca6b54 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TrnSrvcRqd_B_Rq_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_BrakeIndicationWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_BrakeIndicationWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_BrakeIndicationWarning_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_BrakeIndicationWarning_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_BrakeIndicationWarning_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_BrakeIndicationWarning_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_BrakeIndicationWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ClutchIndicationWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ClutchIndicationWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_ClutchIndicationWarning_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_ClutchIndicationWarning_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_ClutchIndicationWarning_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_ClutchIndicationWarning_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ClutchIndicationWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenNotOFFWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenNotOFFWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_DoorOpenNotOFFWarning_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_DoorOpenNotOFFWarning_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_DoorOpenNotOFFWarning_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_DoorOpenNotOFFWarning_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenNotOFFWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_DoorOpenWarning_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_DoorOpenWarning_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_DoorOpenWarning_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_DoorOpenWarning_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_DoorOpenWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ESCL_FailStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ESCL_FailStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_ESCL_FailStatus_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_ESCL_FailStatus_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_ESCL_FailStatus_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_ESCL_FailStatus_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ESCL_FailStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_IMMO_MatchStatus_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_IMMO_MatchStatus_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_IMMO_MatchStatus_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_IMMO_MatchStatus_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyBatteryWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyBatteryWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_KeyBatteryWarning_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_KeyBatteryWarning_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_KeyBatteryWarning_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_KeyBatteryWarning_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyBatteryWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyOutWarning1(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyOutWarning1_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_KeyOutWarning1_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_KeyOutWarning1_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_KeyOutWarning1_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_KeyOutWarning1_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyOutWarning1_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyReminderWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyReminderWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_KeyReminderWarning_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_KeyReminderWarning_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_KeyReminderWarning_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_KeyReminderWarning_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyReminderWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NoKeyFoundWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NoKeyFoundWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_NoKeyFoundWarning_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_NoKeyFoundWarning_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_NoKeyFoundWarning_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_NoKeyFoundWarning_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NoKeyFoundWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotNWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotNWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_NotNWarning_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_NotNWarning_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_NotNWarning_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_NotNWarning_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotNWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotPNWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotPNWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_NotPNWarning_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_NotPNWarning_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_NotPNWarning_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_NotPNWarning_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotPNWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_PowerMode(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_PowerMode_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_PowerMode_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_PowerMode_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_PowerMode_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_PowerMode_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_PowerMode_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_RemainingNumSts(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_RemainingNumSts_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_RemainingNumSts_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_RemainingNumSts_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_RemainingNumSts_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_RemainingNumSts_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_RemainingNumSts_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_SSB_FailStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_SSB_FailStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_SSB_FailStatus_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_SSB_FailStatus_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_SSB_FailStatus_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_SSB_FailStatus_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_SSB_FailStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_SystemFailStatus(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_SystemFailStatus_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_PEPS_SystemFailStatus_9b45c0e5, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_PEPS_SystemFailStatus_9b45c0e5_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_PEPS_SystemFailStatus_9b45c0e5 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_PEPS_SystemFailStatus_9b45c0e5 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_SystemFailStatus_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_RF_0x326_ComIn_RF_KeyLowBatteryWarning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_RF_0x326_ComIn_RF_KeyLowBatteryWarning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_RF_KeyLowBatteryWarning_131d5da3, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_RF_KeyLowBatteryWarning_131d5da3_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_RF_KeyLowBatteryWarning_131d5da3 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_RF_KeyLowBatteryWarning_131d5da3 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_RF_0x326_ComIn_RF_KeyLowBatteryWarning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TPMS_LF_Pressure_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TPMS_LF_Pressure_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TPMS_LF_Pressure_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TPMS_LF_Pressure_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TPMS_LF_Pressure_Warning_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TPMS_LF_Pressure_Warning_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TPMS_LF_Pressure_Warning_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TPMS_LF_Pressure_Warning_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TPMS_LR_Pressure_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TPMS_LR_Pressure_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TPMS_LR_Pressure_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TPMS_LR_Pressure_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TPMS_LR_Pressure_Warning_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TPMS_LR_Pressure_Warning_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TPMS_LR_Pressure_Warning_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TPMS_LR_Pressure_Warning_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TPMS_RF_Pressure_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TPMS_RF_Pressure_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TPMS_RF_Pressure_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TPMS_RF_Pressure_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TPMS_RF_Pressure_Warning_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TPMS_RF_Pressure_Warning_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TPMS_RF_Pressure_Warning_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TPMS_RF_Pressure_Warning_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TPMS_RR_Pressure_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TPMS_RR_Pressure_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TPMS_RR_Pressure_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TPMS_RR_Pressure_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TPMS_RR_Pressure_Warning_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TPMS_RR_Pressure_Warning_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TPMS_RR_Pressure_Warning_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TPMS_RR_Pressure_Warning_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TPMS_System_Status_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TPMS_System_Status_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TPMS_System_Status_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TPMS_System_Status_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Tire_Temperature(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Tire_Temperature_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_TPMS_Tire_Temperature_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_TPMS_Tire_Temperature_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_TPMS_Tire_Temperature_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_TPMS_Tire_Temperature_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_Tire_Temperature_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Temperature_Warning(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Temperature_Warning_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_Temperature_Warning_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_Temperature_Warning_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_Temperature_Warning_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_Temperature_Warning_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Temperature_Warning_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position(P2VAR(UInt8, AUTOMATIC, RTE_COMVNIMMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Com_ReceiveSignal(ComConf_ComSignal_Tire_Position_c030f382, (data)); /* PRQA S 3453 */ /* MD_MSR_19.7 */
  Rte_ComHook_Tire_Position_c030f382_SigRx(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_Tire_Position_c030f382 == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  if (Rte_OsApplication_RxTimeoutFlags.Rte_RxTimeout_Tire_Position_c030f382 != 0)
  {
    ret |= RTE_E_MAX_AGE_EXCEEDED;
  }
  Rte_ReadHook_ComVnimMdl_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_CoolantTmpMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(P2VAR(DCM_IOC_Type, AUTOMATIC, RTE_COOLANTTMPMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_CoolantTmpMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value;
  Rte_ReadHook_CoolantTmpMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DteMdl_pprpNV_DTEMdl_RAFC_FK_Value(P2VAR(NV_Data_DTE, AUTOMATIC, RTE_DTEMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DteMdl_pprpNV_DTEMdl_RAFC_FK_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_DTE;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DteMdl_pprpNV_DTEMdl_RAFC_FK_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_AmbientMdl_MPC_Date_Time_Value(P2VAR(NV_Data_MPC_Day_Time, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_AmbientMdl_MPC_Date_Time_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time;
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_AmbientMdl_MPC_Date_Time_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Assembly_Manufacturing_Date_Assembly_Manufacturing_Date(P2VAR(NV_Assembly_Manufacturing_Date, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Assembly_Manufacturing_Date_Assembly_Manufacturing_Date_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Manufacturing_Date, sizeof(NV_Assembly_Manufacturing_Date));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Assembly_Manufacturing_Date_Assembly_Manufacturing_Date_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Assembly_Plant_Number_Assembly_Plant_Number(P2VAR(NV_Assembly_Plant_Number_10Byte, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Assembly_Plant_Number_Assembly_Plant_Number_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Plant_Number, sizeof(NV_Assembly_Plant_Number_10Byte));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Assembly_Plant_Number_Assembly_Plant_Number_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Cluster_ECUManufacturingDate_ManufacturingDate(P2VAR(NV_ECUManufacturingDate_4Byte, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Cluster_ECUManufacturingDate_ManufacturingDate_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUManufacturingDate, sizeof(NV_ECUManufacturingDate_4Byte));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Cluster_ECUManufacturingDate_ManufacturingDate_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Cluster_ECUSerialNumber_ECUSerialNumber(P2VAR(NV_ECUSerialNumber_14Byte, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Cluster_ECUSerialNumber_ECUSerialNumber_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUSerialNumber, sizeof(NV_ECUSerialNumber_14Byte));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Cluster_ECUSerialNumber_ECUSerialNumber_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Cluster_ProductPartNumber_ProductPartNumber(P2VAR(NV_ECUProductPartNumber_12Byte, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Cluster_ProductPartNumber_ProductPartNumber_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ProductPartNumber, sizeof(NV_ECUProductPartNumber_12Byte));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Cluster_ProductPartNumber_ProductPartNumber_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Cluster_SupplierIdentifier_SupplierIdentifier(P2VAR(NV_ECUSupplierIdentifier_10Byte, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Cluster_SupplierIdentifier_SupplierIdentifier_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SupplierIdentifier, sizeof(NV_ECUSupplierIdentifier_10Byte));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Cluster_SupplierIdentifier_SupplierIdentifier_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Cluster_VIN_Number_VIN_Number(P2VAR(NV_VIN_Number_17Bytes_Type, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Cluster_VIN_Number_VIN_Number_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_ClusterVINNumber, sizeof(NV_VIN_Number_17Bytes_Type));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Cluster_VIN_Number_VIN_Number_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Customer_Reference_Customer_Reference(P2VAR(NV_Assembly_Plant_Number_10Byte, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Customer_Reference_Customer_Reference_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Customer_Reference, sizeof(NV_Assembly_Plant_Number_10Byte));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Customer_Reference_Customer_Reference_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_DcmExt_JMCHardwareVersionNumber_VersionNumber(P2VAR(NV_JMCHardwareVersionNumber_24Bytes_Type, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_DcmExt_JMCHardwareVersionNumber_VersionNumber_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_JMCHardwareVersionNumber, sizeof(NV_JMCHardwareVersionNumber_24Bytes_Type));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_DcmExt_JMCHardwareVersionNumber_VersionNumber_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_ECUDeliveryAssemblyNumber_Value(P2VAR(NvM_ECU24BytesNumberType, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_ECUDeliveryAssemblyNumber_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_EOLData.ECUDeliveryAssemblyNumber, sizeof(NvM_ECU24BytesNumberType));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_ECUDeliveryAssemblyNumber_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_ECUSerialNumber_Value(P2VAR(NvM_ECUSerialNumberType, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_ECUSerialNumber_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_EOLData.ECUSerialNumber, sizeof(NvM_ECUSerialNumberType));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_ECUSerialNumber_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference(P2VAR(NV_VIN_Number_17Bytes_Type, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference, sizeof(NV_VIN_Number_17Bytes_Type));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference_WithoutEEprom(P2VAR(NV_PCBA_Reference_17Bytes_Type, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference_WithoutEEprom_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference_WithoutEEprom, sizeof(NV_PCBA_Reference_17Bytes_Type));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Equipped_PCBA_Reference_PCBA_Reference_WithoutEEprom_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_HardwareVersionNumber_VersionNumber(P2VAR(NV_ECUHardwareVersion, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_HardwareVersionNumber_VersionNumber_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_HardwareVersionNumber, sizeof(NV_ECUHardwareVersion));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_HardwareVersionNumber_VersionNumber_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_PartNumber_Data_PartNumberData(P2VAR(NV_ECUProductPartNumber_12Byte, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_PartNumber_Data_PartNumberData_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescripto_PartNumber_Data, sizeof(NV_ECUProductPartNumber_12Byte));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_PartNumber_Data_PartNumberData_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_ProductInfo_Value(P2VAR(NvM_ProductInfo, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_ProductInfo_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_EOLData.ProductInfo, sizeof(NvM_ProductInfo));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_ProductInfo_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Run_In_mode_Control_Run_In_mode_Control(P2VAR(NV_Run_In_mode_Control, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Run_In_mode_Control_Run_In_mode_Control_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Run_In_mode_Control, sizeof(NV_Run_In_mode_Control));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Run_In_mode_Control_Run_In_mode_Control_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_SMD_Manufacturing_Date_SMD_Manufacturing_Date(P2VAR(NV_SMD_Manufacturing_Date, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_SMD_Manufacturing_Date_SMD_Manufacturing_Date_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_SMD_Manufacturing_Date, sizeof(NV_SMD_Manufacturing_Date));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_SMD_Manufacturing_Date_SMD_Manufacturing_Date_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_SMD_Plant_Number_SMD_Plant_Number(P2VAR(SMD_Plant_Number_3Byte, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_SMD_Plant_Number_SMD_Plant_Number_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_SMD_Plant_Number, sizeof(SMD_Plant_Number_3Byte));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_SMD_Plant_Number_SMD_Plant_Number_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Serial_Number_PCBA_Serial_Number_PCBA(P2VAR(Serial_Number_PCBA_5Byte, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Serial_Number_PCBA_Serial_Number_PCBA_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Serial_Number_PCBA, sizeof(Serial_Number_PCBA_5Byte));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Serial_Number_PCBA_Serial_Number_PCBA_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_VehicleManufacturerECUSoftwareNumber_Value(P2VAR(NvM_ECU24BytesNumberType, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_VehicleManufacturerECUSoftwareNumber_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_EOLData.VehicleManufacturerECUSoftwareNumber, sizeof(NvM_ECU24BytesNumberType));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_VehicleManufacturerECUSoftwareNumber_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Vehicle_Configure_VehicleConfigSetArray(P2VAR(NV_Cluster_Vehicle_Configure, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Vehicle_Configure_VehicleConfigSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Vehicle_Configure, sizeof(NV_Cluster_Vehicle_Configure));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Vehicle_Configure_VehicleConfigSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_Visteon_Product_Reference_Product_Reference(P2VAR(NV_VIN_Number_17Bytes_Type, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Visteon_Product_Reference_Product_Reference_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Product_Reference, sizeof(NV_VIN_Number_17Bytes_Type));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_Visteon_Product_Reference_Product_Reference_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_station_Tracebility_station_Tracebility(P2VAR(NV_station_Tracebility, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_station_Tracebility_station_Tracebility_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_station_Tracebility, sizeof(NV_station_Tracebility));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_station_Tracebility_station_Tracebility_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DcmExt_NV_user_product_configuration_UserProductConfiguration(P2VAR(NV_user_product_configuration, AUTOMATIC, RTE_DCMEXT_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_user_product_configuration_UserProductConfiguration_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_user_product_configuration, sizeof(NV_user_product_configuration));
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DcmExt_NV_user_product_configuration_UserProductConfiguration_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DimDerateMdl_NV_Dim_NvMDimData_DimDataSetArray(P2VAR(NVDimDataSetType, AUTOMATIC, RTE_DIMDERATEMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DimDerateMdl_NV_Dim_NvMDimData_DimDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Dim_Buffer, sizeof(NVDimDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DimDerateMdl_NV_Dim_NvMDimData_DimDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_DimDerateMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(P2VAR(DCM_IOC_Type, AUTOMATIC, RTE_DIMDERATEMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_DimDerateMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value;
  Rte_ReadHook_DimDerateMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_FuelLevelMdl_pprpNV_FuelLevelMdl_FuelLevel_FuelLevelValue(P2VAR(NV_Data_Fuel_Level, AUTOMATIC, RTE_FUELLEVELMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_FuelLevelMdl_pprpNV_FuelLevelMdl_FuelLevel_FuelLevelValue_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_FuelLevel;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_FuelLevelMdl_pprpNV_FuelLevelMdl_FuelLevel_FuelLevelValue_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_FuelLevelMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(P2VAR(DCM_IOC_Type, AUTOMATIC, RTE_FUELLEVELMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_FuelLevelMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value;
  Rte_ReadHook_FuelLevelMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_GdtCtrl_pprpNV_GdtCtrl_Menu_Settings_Value(P2VAR(NV_Data_Menu_Settings, AUTOMATIC, RTE_GDTCTRL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_GdtCtrl_pprpNV_GdtCtrl_Menu_Settings_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_Menu_Settings;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_GdtCtrl_pprpNV_GdtCtrl_Menu_Settings_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_GdtCtrl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(P2VAR(DCM_IOC_Type, AUTOMATIC, RTE_GDTCTRL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_GdtCtrl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value;
  Rte_ReadHook_GdtCtrl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_GdtCtrl_rpSR_TIGDT_MP5_0x385_NaviLaodValueBuf(P2VAR(NaviRaodName, AUTOMATIC, RTE_GDTCTRL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_GdtCtrl_rpSR_TIGDT_MP5_0x385_NaviLaodValueBuf_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_MemCpy32(*(data), Rte_EasyNaviMdl_ppSR_TIGDT_MP5_0x385_NaviLaodValueBuf, sizeof(NaviRaodName));
  Rte_ReadHook_GdtCtrl_rpSR_TIGDT_MP5_0x385_NaviLaodValueBuf_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_NV_Cluster_ODO_Backup_Counter_ODO_Backup_Value(P2VAR(NV_Data_Cluster_ODO_Backup_Counter, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_Cluster_ODO_Backup_Counter_ODO_Backup_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_ODO_Backup_Counter;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_Cluster_ODO_Backup_Counter_ODO_Backup_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value(P2VAR(NV_Data_Cluster_Optional_Feature_Configure, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Optional_Feature_Configure;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_NV_Cluster_Variable_Block_Cluster_Variable_Block(P2VAR(NV_Data_Cluster_Variable_Block, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_Cluster_Variable_Block_Cluster_Variable_Block_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Variable_Block;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_Cluster_Variable_Block_Cluster_Variable_Block_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_NV_Dim_NvMOdoData_DimDataSetArray(P2VAR(NVDimDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_Dim_NvMOdoData_DimDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Dim_Buffer, sizeof(NVDimDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_Dim_NvMOdoData_DimDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(P2VAR(NVOdoDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer0, sizeof(NVOdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(P2VAR(NVOdoDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer1, sizeof(NVOdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(P2VAR(NVOdoDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer2, sizeof(NVOdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(P2VAR(NVOdoDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer3, sizeof(NVOdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(P2VAR(NVOdoDataSetType, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer4, sizeof(NVOdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Offset_Value(P2VAR(NV_Data_Cluster_SIA_Reset_Odo_OffSet, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Offset_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Offset_Value;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Offset_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Store_Value(P2VAR(NV_Data_Cluster_SIA_Reset_Odo_OffSet, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Store_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Store_Value;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_NV_SIAMdl_Reset_Odo_OffSet_Store_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_NVMExtMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value(P2VAR(NV_Data_MPC_Day_Time, AUTOMATIC, RTE_NVMEXTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NVMExtMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_NVMExtMdl_pprpNV_AmbientMdl_MPC_Date_Time_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_NV_Cluster_ODO_Backup_Counter_ODO_Backup_Value(P2VAR(NV_Data_Cluster_ODO_Backup_Counter, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_Cluster_ODO_Backup_Counter_ODO_Backup_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_ODO_Backup_Counter;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_Cluster_ODO_Backup_Counter_ODO_Backup_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_NV_Cluster_Variable_Block_Cluster_Variable_Block(P2VAR(NV_Data_Cluster_Variable_Block, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_Cluster_Variable_Block_Cluster_Variable_Block_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Variable_Block;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_Cluster_Variable_Block_Cluster_Variable_Block_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_NV_OdoMdl_NvMOdoData_OdoDataSetArray(P2VAR(OdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_OdoMdl_NV_OdoMdl_NvMOdoData_OdoDataSetArray, sizeof(OdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(P2VAR(NVOdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer0, sizeof(NVOdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(P2VAR(NVOdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer1, sizeof(NVOdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(P2VAR(NVOdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer2, sizeof(NVOdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(P2VAR(NVOdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer3, sizeof(NVOdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(P2VAR(NVOdoDataSetType, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer4, sizeof(NVOdoDataSetType));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_NV_OdoMdl_Reset_OffSet_Offset_Value(P2VAR(NV_Data_Cluster_Reset_OffSet, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_Reset_OffSet_Offset_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_OdoMdl_NV_OdoMdl_Reset_OffSet_Offset_Value;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_OdoMdl_Reset_OffSet_Offset_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_NV_TripMdl_TripA_value(P2VAR(NV_Data_Trip_A, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_TripMdl_TripA_value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_TripA;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_TripMdl_TripA_value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_NV_TripMdl_TripB_value(P2VAR(NV_Data_Trip_B, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_TripMdl_TripB_value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_TripB;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_NV_TripMdl_TripB_value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_pprpNV_OdoMdl_ODO_Reset_Counter_ResetCounterValue(P2VAR(NV_Data_ODO_Reset_Counter, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_pprpNV_OdoMdl_ODO_Reset_Counter_ResetCounterValue_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_ODO_Reset_Counter;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_pprpNV_OdoMdl_ODO_Reset_Counter_ResetCounterValue_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_OdoMdl_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface(P2VAR(OdoMdl_RW_Interface_Type, AUTOMATIC, RTE_ODOMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_OdoMdl_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_OdoMdl_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface;
  Rte_ReadHook_OdoMdl_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_PtrBaseView_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP(P2VAR(NV_Data_ClusterStpSettings, AUTOMATIC, RTE_PTRBASEVIEW_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_PtrBaseView_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_Stp_configData;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_PtrBaseView_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_PtrBaseView_NV_Cluster_Interpolations_Cluster_Interpolations(P2VAR(NV_Data_Interpolations, AUTOMATIC, RTE_PTRBASEVIEW_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_PtrBaseView_NV_Cluster_Interpolations_Cluster_Interpolations_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_Stp_angle_convertData;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_PtrBaseView_NV_Cluster_Interpolations_Cluster_Interpolations_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_PtrBaseView_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(P2VAR(DCM_IOC_Type, AUTOMATIC, RTE_PTRBASEVIEW_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_PtrBaseView_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value;
  Rte_ReadHook_PtrBaseView_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_SIAMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value(P2VAR(NV_Data_Cluster_Optional_Feature_Configure, AUTOMATIC, RTE_SIAMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_SIAMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Optional_Feature_Configure;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_SIAMdl_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_SIAMdl_pprpNV_Cluster_Maintenance_Maintenance_Value(P2VAR(NV_Data_Cluster_Maintenance, AUTOMATIC, RTE_SIAMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_SIAMdl_pprpNV_Cluster_Maintenance_Maintenance_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_SIAMdl_pprpNV_Cluster_Maintenance_Maintenance_Value;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_SIAMdl_pprpNV_Cluster_Maintenance_Maintenance_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Offset_Value(P2VAR(NV_Data_Cluster_SIA_Reset_Odo_OffSet, AUTOMATIC, RTE_SIAMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Offset_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SIA_Reset_Odo_OffSet;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Offset_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Store_Value(P2VAR(NV_Data_Cluster_SIA_Reset_Odo_OffSet, AUTOMATIC, RTE_SIAMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Store_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Store_Value;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_SIAMdl_pprpNV_SIAMdl_Reset_Odo_OffSet_Store_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_SeatbeltChime_rp_Seatbelt_TT_Status_Drv_Status(P2VAR(uint8, AUTOMATIC, RTE_SEATBELTCHIME_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_SeatbeltChime_rp_Seatbelt_TT_Status_Drv_Status_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_SeatBeltMdl_pp_Seatbelt_TT_Status_Drv_Status;
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SeatBeltMdl_pp_Seatbelt_TT_Status_Drv_Status == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  Rte_ReadHook_SeatbeltChime_rp_Seatbelt_TT_Status_Drv_Status_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_SeatbeltChime_rp_Seatbelt_TT_Status_Pas_Status(P2VAR(uint8, AUTOMATIC, RTE_SEATBELTCHIME_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_SeatbeltChime_rp_Seatbelt_TT_Status_Pas_Status_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_SeatBeltMdl_pp_Seatbelt_TT_Status_Pas_Status;
  if (Rte_OsApplication_RxNeverReceivedFlags.Rte_RxNeverReceived_SeatBeltMdl_pp_Seatbelt_TT_Status_Pas_Status == 0)
  {
    ret = RTE_E_NEVER_RECEIVED;
  }
  Rte_ReadHook_SeatbeltChime_rp_Seatbelt_TT_Status_Pas_Status_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_StpCddMdl_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP(P2VAR(NV_Data_ClusterStpSettings, AUTOMATIC, RTE_STPCDDMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_StpCddMdl_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_Stp_configData;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_StpCddMdl_NVM_V348_NV_Data_ClusterStpSettings_NVM_V348STP_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_StpCddMdl_NV_Cluster_Interpolations_Cluster_Interpolations(P2VAR(NV_Data_Interpolations, AUTOMATIC, RTE_STPCDDMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_StpCddMdl_NV_Cluster_Interpolations_Cluster_Interpolations_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_Stp_angle_convertData;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_StpCddMdl_NV_Cluster_Interpolations_Cluster_Interpolations_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_StpCddMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(P2VAR(DCM_IOC_Type, AUTOMATIC, RTE_STPCDDMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_StpCddMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value;
  Rte_ReadHook_StpCddMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_TripMdl_pprpNV_TripMdl_TripA_value(P2VAR(NV_Data_Trip_A, AUTOMATIC, RTE_TRIPMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_TripMdl_pprpNV_TripMdl_TripA_value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_TripA;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_TripMdl_pprpNV_TripMdl_TripA_value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_TripMdl_pprpNV_TripMdl_TripB_value(P2VAR(NV_Data_Trip_B, AUTOMATIC, RTE_TRIPMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_TripMdl_pprpNV_TripMdl_TripB_value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  *(data) = Rte_NvBlockSwComponentType_NVBlockDescriptor_TripB;
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_TripMdl_pprpNV_TripMdl_TripB_value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_TtMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(P2VAR(DCM_IOC_Type, AUTOMATIC, RTE_TTMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_TtMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  *(data) = Rte_DcmExt_ppSR_TIDcm_DID_IOC_DCM_IOC_Value;
  Rte_ReadHook_TtMdl_rpSR_TIDcm_DID_IOC_DCM_IOC_Value_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

FUNC(Std_ReturnType, RTE_CODE) Rte_Read_VehicleCfgMdl_NV_Vehicle_Configure_VehicleConfigSetArray(P2VAR(NV_Cluster_Vehicle_Configure, AUTOMATIC, RTE_VEHICLECFGMDL_APPL_VAR) data) /* PRQA S 0850, 3673, 1505 */ /* MD_MSR_19.8, MD_Rte_Qac, MD_MSR_8.10 */
{
  Std_ReturnType ret = RTE_E_OK;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_READ_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_VehicleCfgMdl_NV_Vehicle_Configure_VehicleConfigSetArray_Start(data); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_MemCpy32(*(data), Rte_NvBlockSwComponentType_NVBlockDescriptor_Vehicle_Configure, sizeof(NV_Cluster_Vehicle_Configure));
  Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ReadHook_VehicleCfgMdl_NV_Vehicle_Configure_VehicleConfigSetArray_Return(data); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */


/**********************************************************************************************************************
 * Internal C/S connections
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_AppCtrl_rpCS_rSys_SysGdtCdd_TISystem_OnCommand(ECmpCmd cmd) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret;
  EventMaskType ev;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_CALL_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_CallHook_AppCtrl_rpCS_rSys_SysGdtCdd_TISystem_OnCommand_Start(cmd); /* PRQA S 3112 */ /* MD_Rte_3112 */

  {
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (Rte_CS_ServerQueueInfo_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_Free > 0)
    {
      Rte_CS_ServerQueueInfo_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_Free = 0;
      Rte_CS_ServerQueueInfo_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_Active = 1;
      Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_ServerCompleted = FALSE;
      {
        Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_TaskID = OsTask_10msEcuM;
      }
      Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.cmd = cmd;

      Rte_Task_SetEvent(OsTask_LSH_16ms, Rte_Ev_Run_GdtCdd_CGdtCdd_Impl_OnCommand); /* PRQA S 3112 */ /* MD_Rte_3112 */
      /* scheduled trigger for runnables: CGdtCdd_Impl_OnCommand */
      (void)SetEvent(OsTask_LSH_16ms, Rte_Ev_Run_GdtCdd_CGdtCdd_Impl_OnCommand); /* PRQA S 3417 */ /* MD_Rte_Os */

      {
        do
        {
          Rte_Task_WaitEvent(OsTask_10msEcuM, Rte_Ev_WP_OsTask_10msEcuM); /* PRQA S 3112 */ /* MD_Rte_3112 */
          (void)WaitEvent(Rte_Ev_WP_OsTask_10msEcuM); /* PRQA S 3417 */ /* MD_Rte_Os */
          Rte_Task_WaitEventRet(OsTask_10msEcuM, Rte_Ev_WP_OsTask_10msEcuM); /* PRQA S 3112 */ /* MD_Rte_3112 */
          (void)GetEvent(OsTask_10msEcuM, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
          (void)ClearEvent(ev & (Rte_Ev_WP_OsTask_10msEcuM)); /* PRQA S 3417 */ /* MD_Rte_Os */
        }
        while (Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_ServerCompleted == FALSE);
      }

      ret = Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_Result;

      Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      Rte_CS_ServerQueueInfo_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_Free = 1;
    }
    else
    {
      ret = RTE_E_TIMEOUT;
    }
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  Rte_CallHook_AppCtrl_rpCS_rSys_SysGdtCdd_TISystem_OnCommand_Return(cmd); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_AppCtrl_rpCS_rSys_SysGdtCtrl_TISystem_OnCommand(ECmpCmd cmd) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret;
  EventMaskType ev;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_CALL_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_CallHook_AppCtrl_rpCS_rSys_SysGdtCtrl_TISystem_OnCommand_Start(cmd); /* PRQA S 3112 */ /* MD_Rte_3112 */

  {
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (Rte_CS_ServerQueueInfo_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_Free > 0)
    {
      Rte_CS_ServerQueueInfo_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_Free = 0;
      Rte_CS_ServerQueueInfo_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_Active = 1;
      Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_ServerCompleted = FALSE;
      {
        Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_TaskID = OsTask_10msEcuM;
      }
      Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.cmd = cmd;

      Rte_Task_SetEvent(OsTask_LSH_16ms, Rte_Ev_Run_GdtCtrl_CGdtCtrl_Impl_OnCommand); /* PRQA S 3112 */ /* MD_Rte_3112 */
      /* scheduled trigger for runnables: CGdtCtrl_Impl_OnCommand */
      (void)SetEvent(OsTask_LSH_16ms, Rte_Ev_Run_GdtCtrl_CGdtCtrl_Impl_OnCommand); /* PRQA S 3417 */ /* MD_Rte_Os */

      {
        do
        {
          Rte_Task_WaitEvent(OsTask_10msEcuM, Rte_Ev_WP_OsTask_10msEcuM); /* PRQA S 3112 */ /* MD_Rte_3112 */
          (void)WaitEvent(Rte_Ev_WP_OsTask_10msEcuM); /* PRQA S 3417 */ /* MD_Rte_Os */
          Rte_Task_WaitEventRet(OsTask_10msEcuM, Rte_Ev_WP_OsTask_10msEcuM); /* PRQA S 3112 */ /* MD_Rte_3112 */
          (void)GetEvent(OsTask_10msEcuM, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
          (void)ClearEvent(ev & (Rte_Ev_WP_OsTask_10msEcuM)); /* PRQA S 3417 */ /* MD_Rte_Os */
        }
        while (Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_ServerCompleted == FALSE);
      }

      ret = Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_Result;

      Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      Rte_CS_ServerQueueInfo_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_Free = 1;
    }
    else
    {
      ret = RTE_E_TIMEOUT;
    }
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  Rte_CallHook_AppCtrl_rpCS_rSys_SysGdtCtrl_TISystem_OnCommand_Return(cmd); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_Dcm_Dcm_DcmClearDTC(uint32 DTC, Dem_DTCFormatType DTCFormat, Dem_DTCOriginType DTCOrigin) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_CALL_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_Dcm_Dcm_DcmEnableDTCSetting(Dem_DTCGroupType DTCGroup, Dem_DTCKindType DTCKind) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_CALL_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_Dcm_Dcm_DcmGetExtendedDataRecordByDTC(uint32 DTC, Dem_DTCOriginType DTCOrigin, uint8 ExtendedDataNumber, P2VAR(uint8, AUTOMATIC, RTE_DCM_APPL_VAR) DestBuffer, P2VAR(uint16, AUTOMATIC, RTE_DCM_APPL_VAR) BufSize) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_CALL_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_Dcm_RoutineServices_DcmDspRoutine_1_Start(Dcm_OpStatusType OpStatus, P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, RTE_DCM_APPL_VAR) ErrorCode) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_CALL_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_GdtCdd_TIGdtCtrl_RenderDone(void) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */
  boolean callCompleted;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_CALL_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_CallHook_GdtCdd_TIGdtCtrl_RenderDone_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */

  SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  if (Rte_OsApplication_ClientIdleFlags.Rte_Idle_GdtCdd_TIGdtCtrl_RenderDone == 1)
  {
    callCompleted = Rte_CS_ClientQueue_GdtCdd_TIGdtCtrl_RenderDone.Rte_CallCompleted;
    if (callCompleted == TRUE)
    {
      if (Rte_CS_ServerQueueInfo_GdtCtrl_TIGdtCtrl_RenderDone.Rte_Free > 0)
      {
        Rte_OsApplication_ClientIdleFlags.Rte_Idle_GdtCdd_TIGdtCtrl_RenderDone = 0;
        Rte_CS_ClientQueue_GdtCdd_TIGdtCtrl_RenderDone.Rte_CallCompleted = FALSE;
        Rte_CS_ServerQueueInfo_GdtCtrl_TIGdtCtrl_RenderDone.Rte_Free = 0;
        Rte_CS_ServerQueueInfo_GdtCtrl_TIGdtCtrl_RenderDone.Rte_Active = 1;
        Rte_CS_ServerQueue_GdtCtrl_TIGdtCtrl_RenderDone.Rte_ClientId = Rte_CS_ClientConfigIndex_GdtCdd_TIGdtCtrl_RenderDone;
      }
      else
      {
        ret = RTE_E_TIMEOUT;
      }
    }
    else
    {
      ret = RTE_E_TIMEOUT;
    }
  }
  else
  {
    ret = RTE_E_LIMIT;
  }
  ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

  if (ret == RTE_E_OK)
  {

    Rte_Task_SetEvent(OsTask_LSH_16ms, Rte_Ev_Run_GdtCtrl_GdtCtrl_RenderDone); /* PRQA S 3112 */ /* MD_Rte_3112 */
    /* scheduled trigger for runnables: GdtCtrl_RenderDone */
    (void)SetEvent(OsTask_LSH_16ms, Rte_Ev_Run_GdtCtrl_GdtCtrl_RenderDone); /* PRQA S 3417 */ /* MD_Rte_Os */
  }

  Rte_CallHook_GdtCdd_TIGdtCtrl_RenderDone_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Result_GdtCdd_TIGdtCtrl_RenderDone(void) /* PRQA S 0850, 3408, 3673 */ /* MD_MSR_19.8, MD_Rte_3408, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_RESULT_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_ResultHook_GdtCdd_TIGdtCtrl_RenderDone_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */

  {
    boolean callCompleted;
    SuspendOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    callCompleted = Rte_CS_ClientQueue_GdtCdd_TIGdtCtrl_RenderDone.Rte_CallCompleted;
    if ((Rte_OsApplication_ClientIdleFlags.Rte_Idle_GdtCdd_TIGdtCtrl_RenderDone == 0) && (callCompleted == TRUE))
    {
      Rte_OsApplication_ClientIdleFlags.Rte_Idle_GdtCdd_TIGdtCtrl_RenderDone = 1;
    }
    else
    {
      ret = RTE_E_NO_DATA;
    }
    ResumeOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  Rte_ResultHook_GdtCdd_TIGdtCtrl_RenderDone_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NVMExtMdl_rpCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */
  EventMaskType ev;

  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_CALL_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_CallHook_NVMExtMdl_rpCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  {
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    if (Rte_CS_ServerQueueInfo_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_Free > 0)
    {
      Rte_CS_ServerQueueInfo_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_Free = 0;
      Rte_CS_ServerQueueInfo_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_Active = 1;
      Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_ServerCompleted = FALSE;
      {
        Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_TaskID = OsTask_10msEcuM;
      }
      Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.ServiceId = ServiceId;
      Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.JobResult = JobResult;

      Rte_Task_SetEvent(OsTask_RteEvent, Rte_Ev_Run_OdoMdl_OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished); /* PRQA S 3112 */ /* MD_Rte_3112 */
      /* scheduled trigger for runnables: OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished */
      (void)SetEvent(OsTask_RteEvent, Rte_Ev_Run_OdoMdl_OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished); /* PRQA S 3417 */ /* MD_Rte_Os */

      {
        do
        {
          Rte_Task_WaitEvent(OsTask_10msEcuM, Rte_Ev_WP_OsTask_10msEcuM); /* PRQA S 3112 */ /* MD_Rte_3112 */
          (void)WaitEvent(Rte_Ev_WP_OsTask_10msEcuM); /* PRQA S 3417 */ /* MD_Rte_Os */
          Rte_Task_WaitEventRet(OsTask_10msEcuM, Rte_Ev_WP_OsTask_10msEcuM); /* PRQA S 3112 */ /* MD_Rte_3112 */
          (void)GetEvent(OsTask_10msEcuM, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
          (void)ClearEvent(ev & (Rte_Ev_WP_OsTask_10msEcuM)); /* PRQA S 3417 */ /* MD_Rte_Os */
        }
        while (Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_ServerCompleted == FALSE);
      }

      Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      Rte_CS_ServerQueueInfo_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_Free = 1;
    }
    else
    {
      ret = RTE_E_TIMEOUT;
    }
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  Rte_CallHook_NVMExtMdl_rpCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_AFC_Accumulate_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_AFC_Refuel_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Assembly_Manufacturing_Date_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Assembly_Plant_Number_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_ClusterSIAResetOffset_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_ClusterSIAResetOffset_JobFinished_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_Cluster_SIAResetOffset_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Sp_NvMNotifyJobFinished_Cluster_SIAResetOffset(ServiceId, JobResult);
  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_Cluster_SIAResetOffset_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_ClusterSIAResetOffset_JobFinished_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_ClusterSettings_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_ClusterVINNumber_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_ClusterVariableBlock_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_ClusterVariableBlock_JobFinished_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_Cluster_Variable_Block_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Sp_NvMNotifyJobFinished_Cluster_Variable_Block(ServiceId, JobResult);
  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_Cluster_Variable_Block_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_ClusterVariableBlock_JobFinished_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Cluster_Backup_Odo_Counter_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_Cluster_Backup_Odo_Counter_JobFinished_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_Cluster_ODO_Backup_Counter_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Sp_NvMNotifyJobFinished_Cluster_ODO_Backup_Counter(ServiceId, JobResult);
  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_Cluster_ODO_Backup_Counter_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_Cluster_Backup_Odo_Counter_JobFinished_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Cluster_ECUManufacturingDate_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Cluster_ECUSerialNumber_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Cluster_Optional_Feature_Configure_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_Cluster_Optional_Feature_Configure_JobFinished_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_Cluster_Optional_Configure_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Sp_NvMNotifyJobFinished_Cluster_Optional_Configure(ServiceId, JobResult);
  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_Cluster_Optional_Configure_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_Cluster_Optional_Feature_Configure_JobFinished_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Cluster_ProductPartNumber_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Cluster_SupplierIdentifier_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Customer_Reference_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_DTE_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_DemPrimary_000_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_DemPrimary_001_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_DemPrimary_002_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_DemPrimary_032_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_DimBuffer_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_DimBuffer_JobFinished_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_Dim_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Sp_NvMNotifyJobFinished_JobFinished_Dim(ServiceId, JobResult);
  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_Dim_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_DimBuffer_JobFinished_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_EOLData_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_EctConfiguration_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_FuelLevelValue_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_HardwareVersionNumber_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Interpolations_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_JMCHardwareVersionNumber_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_MPCDateTime_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_MenuSettings_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_OdometerBuffer0_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_OdometerBuffer0_JobFinished_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer0_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer0(ServiceId, JobResult);
  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer0_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_OdometerBuffer0_JobFinished_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_OdometerBuffer1_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_OdometerBuffer1_JobFinished_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer1_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer1(ServiceId, JobResult);
  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer1_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_OdometerBuffer1_JobFinished_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_OdometerBuffer2_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_OdometerBuffer2_JobFinished_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer2_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer2(ServiceId, JobResult);
  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer2_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_OdometerBuffer2_JobFinished_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_OdometerBuffer3_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_OdometerBuffer3_JobFinished_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer3_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer3(ServiceId, JobResult);
  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer3_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_OdometerBuffer3_JobFinished_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_OdometerBuffer4_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_OK; /* PRQA S 3197 */ /* MD_Rte_3197 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_OdometerBuffer4_JobFinished_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer4_Start(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */
  Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer4(ServiceId, JobResult);
  Rte_Runnable_NVMExtMdl_Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer4_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  Rte_CallHook_NvM_NvM_RpNotifyJobEnd_OdometerBuffer4_JobFinished_Return(ServiceId, JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_PCBA_Reference_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_PCBA_Reference_WithoutEEprom_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_PartNumber_Data_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_ResetCounterValue_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Run_In_mode_Control_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_SMD_Manufacturing_Date_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_SMD_Plant_Number_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Serial_Number_PCBA_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Stp_ConfigData_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Stp_angle_convertData_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_TripA_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_TripB_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Vehicle_Configure_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_Visteon_Product_Reference_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_station_Tracebility_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Call_NvM_NvM_RpNotifyJobEnd_user_product_configuration_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult) /* PRQA S 0850, 1505, 3206, 3673 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206, MD_Rte_Qac */
{
  Std_ReturnType ret = RTE_E_UNCONNECTED; /* PRQA S 3197 */ /* MD_Rte_3197 */

  return ret;
}


/**********************************************************************************************************************
 * Transmission/Mode Switch Acknowledgement handling (Rte_Feedback/Rte_SwitchAck)
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_SwitchAck_Dcm_DcmEcuReset_DcmEcuReset(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  return RTE_E_UNCONNECTED;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */


/**********************************************************************************************************************
 * Mode Switch API (Rte_Switch)
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_Dcm_DcmControlDtcSetting_DcmControlDtcSetting(uint8 nextMode) /* PRQA S 0850, 1505, 3206 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206 */
{
  nextMode = nextMode;
  return RTE_E_OK;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_Dcm_DcmDiagnosticSessionControl_DcmDiagnosticSessionControl(uint8 nextMode) /* PRQA S 0850, 1505, 3206 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206 */
{
  nextMode = nextMode;
  return RTE_E_OK;
}

FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_Dcm_DcmEcuReset_DcmEcuReset(uint8 nextMode) /* PRQA S 0850, 1505, 3206 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206 */
{
  nextMode = nextMode;
  return RTE_E_OK;
}


/**********************************************************************************************************************
 * Transmission/Mode Switch Acknowledgement handling (Rte_Feedback/Rte_SwitchAck)
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_SwitchAck_EcuM_currentMode_currentMode(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
  return RTE_E_UNCONNECTED;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */


/**********************************************************************************************************************
 * Mode Switch API (Rte_Switch)
 *********************************************************************************************************************/

FUNC(Std_ReturnType, RTE_CODE) Rte_Switch_EcuM_currentMode_currentMode(uint8 nextMode) /* PRQA S 0850, 1505, 3206 */ /* MD_MSR_19.8, MD_MSR_8.10, MD_Rte_3206 */
{
  nextMode = nextMode;
  return RTE_E_OK;
}



/**********************************************************************************************************************
 * Runnable Entities for Nv Block Components
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * Runnable Entity Name: NvBlockSwComponentTypeRunnable
 *
 *---------------------------------------------------------------------------------------------------------------------
 *
 * Executed if at least one of the following trigger conditions occurred:
 *   - triggered on DataReceivedEvent for DataElementPrototype <NVM_V348STP> of PortPrototype <NVM_V348_NV_Data_ClusterStpSettings>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Assembly_Manufacturing_Date> of PortPrototype <NV_Assembly_Manufacturing_Date>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Assembly_Plant_Number> of PortPrototype <NV_Assembly_Plant_Number>
 *   - triggered on DataReceivedEvent for DataElementPrototype <ManufacturingDate> of PortPrototype <NV_Cluster_ECUManufacturingDate>
 *   - triggered on DataReceivedEvent for DataElementPrototype <ECUSerialNumber> of PortPrototype <NV_Cluster_ECUSerialNumber>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Cluster_Interpolations> of PortPrototype <NV_Cluster_Interpolations>
 *   - triggered on DataReceivedEvent for DataElementPrototype <ODO_Backup_Value> of PortPrototype <NV_Cluster_ODO_Backup_Counter>
 *   - triggered on DataReceivedEvent for DataElementPrototype <ODO_Backup_Value> of PortPrototype <NV_Cluster_ODO_Backup_Counter_1>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Cluster_Feature_Configure_Value> of PortPrototype <NV_Cluster_Optional_Feature_Configure>
 *   - triggered on DataReceivedEvent for DataElementPrototype <ProductPartNumber> of PortPrototype <NV_Cluster_ProductPartNumber>
 *   - triggered on DataReceivedEvent for DataElementPrototype <SupplierIdentifier> of PortPrototype <NV_Cluster_SupplierIdentifier>
 *   - triggered on DataReceivedEvent for DataElementPrototype <VIN_Number> of PortPrototype <NV_Cluster_VIN_Number>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Cluster_Variable_Block> of PortPrototype <NV_Cluster_Variable_Block>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Customer_Reference> of PortPrototype <NV_Customer_Reference>
 *   - triggered on DataReceivedEvent for DataElementPrototype <VersionNumber> of PortPrototype <NV_DcmExt_JMCHardwareVersionNumber>
 *   - triggered on DataReceivedEvent for DataElementPrototype <DimDataSetArray> of PortPrototype <NV_DimDerateMdl_NvMDimData>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Value> of PortPrototype <NV_ECUDeliveryAssemblyNumber>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Value> of PortPrototype <NV_ECUSerialNumber>
 *   - triggered on DataReceivedEvent for DataElementPrototype <ETM_ENTER> of PortPrototype <NV_ETM_Timeouts>
 *   - triggered on DataReceivedEvent for DataElementPrototype <ETM_ENTRY> of PortPrototype <NV_ETM_Timeouts>
 *   - triggered on DataReceivedEvent for DataElementPrototype <ETM_EXIT> of PortPrototype <NV_ETM_Timeouts>
 *   - triggered on DataReceivedEvent for DataElementPrototype <PCBA_Reference> of PortPrototype <NV_Equipped_PCBA_Reference>
 *   - triggered on DataReceivedEvent for DataElementPrototype <WithoutEEprom> of PortPrototype <NV_Equipped_PCBA_Reference_PCBA_Reference>
 *   - triggered on DataReceivedEvent for DataElementPrototype <VersionNumber> of PortPrototype <NV_HardwareVersionNumber>
 *   - triggered on DataReceivedEvent for DataElementPrototype <OdoDataSetArray> of PortPrototype <NV_OdoMdl_NvMOdoData_Buffer0>
 *   - triggered on DataReceivedEvent for DataElementPrototype <OdoDataSetArray> of PortPrototype <NV_OdoMdl_NvMOdoData_Buffer1>
 *   - triggered on DataReceivedEvent for DataElementPrototype <OdoDataSetArray> of PortPrototype <NV_OdoMdl_NvMOdoData_Buffer2>
 *   - triggered on DataReceivedEvent for DataElementPrototype <OdoDataSetArray> of PortPrototype <NV_OdoMdl_NvMOdoData_Buffer3>
 *   - triggered on DataReceivedEvent for DataElementPrototype <OdoDataSetArray> of PortPrototype <NV_OdoMdl_NvMOdoData_Buffer4>
 *   - triggered on DataReceivedEvent for DataElementPrototype <PartNumberData> of PortPrototype <NV_PartNumber_Data>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Value> of PortPrototype <NV_ProductInfo>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Run_In_mode_Control> of PortPrototype <NV_Run_In_mode_Control>
 *   - triggered on DataReceivedEvent for DataElementPrototype <SMD_Manufacturing_Date> of PortPrototype <NV_SMD_Manufacturing_Date>
 *   - triggered on DataReceivedEvent for DataElementPrototype <SMD_Plant_Number> of PortPrototype <NV_SMD_Plant_Number>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Serial_Number_PCBA> of PortPrototype <NV_Serial_Number_PCBA>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Value> of PortPrototype <NV_VehicleManufacturerECUSoftwareNumber>
 *   - triggered on DataReceivedEvent for DataElementPrototype <VehicleConfigSetArray> of PortPrototype <NV_Vehicle_Configure>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Product_Reference> of PortPrototype <NV_Visteon_Product_Reference>
 *   - triggered on DataReceivedEvent for DataElementPrototype <station_Tracebility> of PortPrototype <NV_station_Tracebility>
 *   - triggered on DataReceivedEvent for DataElementPrototype <UserProductConfiguration> of PortPrototype <NV_user_product_configuration>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Value> of PortPrototype <pprpNV_AFCMdl_Accumulate>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Value> of PortPrototype <pprpNV_AFCMdl_Refuel>
 *   - triggered on DataReceivedEvent for DataElementPrototype <SingleAFCValue> of PortPrototype <pprpNV_AFCMdl_SingleAFC>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Value> of PortPrototype <pprpNV_AmbientMdl_MPC_Date_Time>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Value> of PortPrototype <pprpNV_DTEMdl_RAFC_FK>
 *   - triggered on DataReceivedEvent for DataElementPrototype <FuelLevelValue> of PortPrototype <pprpNV_FuelLevelMdl_FuelLevel>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Value> of PortPrototype <pprpNV_GdtCtrl_Menu_Settings>
 *   - triggered on DataReceivedEvent for DataElementPrototype <ResetCounterValue> of PortPrototype <pprpNV_OdoMdl_ODO_Reset_Counter>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Offset_Value> of PortPrototype <pprpNV_SIAMdl_Reset_Odo_OffSet>
 *   - triggered on DataReceivedEvent for DataElementPrototype <Store_Value> of PortPrototype <pprpNV_SIAMdl_Reset_Odo_OffSet>
 *   - triggered on DataReceivedEvent for DataElementPrototype <value> of PortPrototype <pprpNV_TripMdl_TripA>
 *   - triggered on DataReceivedEvent for DataElementPrototype <value> of PortPrototype <pprpNV_TripMdl_TripB>
 *
 *********************************************************************************************************************/

STATIC FUNC(void, RTE_CODE) NvBlockSwComponentTypeRunnable(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_AFC_Accumulate == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_AFC_Accumulate, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_AFC_Accumulate = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_AFC_Refuel == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_AFC_Refuel, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_AFC_Refuel = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Manufacturing_Date == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Assembly_Manufacturing_Date, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Manufacturing_Date = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Plant_Number == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Assembly_Plant_Number, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Assembly_Plant_Number = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SIA_Reset_Odo_OffSet == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_ClusterSIAResetOffset, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SIA_Reset_Odo_OffSet = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ClusterSettings == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_ClusterSettings, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ClusterSettings = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ClusterVINNumber == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_ClusterVINNumber, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ClusterVINNumber = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Variable_Block == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_ClusterVariableBlock, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Variable_Block = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ODO_Backup_Counter == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Cluster_Backup_Odo_Counter, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ODO_Backup_Counter = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUManufacturingDate == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Cluster_ECUManufacturingDate, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUManufacturingDate = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUSerialNumber == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Cluster_ECUSerialNumber, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ECUSerialNumber = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Optional_Feature_Configure == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Cluster_Optional_Feature_Configure, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_Optional_Feature_Configure = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ProductPartNumber == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Cluster_ProductPartNumber, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_ProductPartNumber = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SupplierIdentifier == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Cluster_SupplierIdentifier, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Cluster_SupplierIdentifier = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Customer_Reference == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Customer_Reference, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Customer_Reference = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_DTE == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_DTE, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_DTE = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Dim_Buffer == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_DimBuffer, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Dim_Buffer = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_EOLData == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_EOLData, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_EOLData = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_FuelLevel == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_FuelLevelValue, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_FuelLevel = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_HardwareVersionNumber == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_HardwareVersionNumber, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_HardwareVersionNumber = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_JMCHardwareVersionNumber == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_JMCHardwareVersionNumber, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_JMCHardwareVersionNumber = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_MPCDateTime, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_MPC_Date_Time = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Menu_Settings == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_MenuSettings, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Menu_Settings = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer0 == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_OdometerBuffer0, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer0 = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer1 == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_OdometerBuffer1, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer1 = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer2 == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_OdometerBuffer2, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer2 = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer3 == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_OdometerBuffer3, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer3 = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer4 == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_OdometerBuffer4, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Odometer_Buffer4 = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_PCBA_Reference, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference_WithoutEEprom == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_PCBA_Reference_WithoutEEprom, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_PCBA_Reference_WithoutEEprom = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescripto_PartNumber_Data == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_PartNumber_Data, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescripto_PartNumber_Data = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ODO_Reset_Counter == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_ResetCounterValue, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_ODO_Reset_Counter = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Run_In_mode_Control == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Run_In_mode_Control, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Run_In_mode_Control = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_SMD_Manufacturing_Date == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_SMD_Manufacturing_Date, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_SMD_Manufacturing_Date = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_SMD_Plant_Number == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_SMD_Plant_Number, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_SMD_Plant_Number = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Serial_Number_PCBA == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Serial_Number_PCBA, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Serial_Number_PCBA = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_SingleAFC == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_SingleAFC, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_SingleAFC = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Stp_configData == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Stp_ConfigData, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Stp_configData = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Stp_angle_convertData == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Stp_angle_convertData, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Stp_angle_convertData = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_TripA == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_TripA, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_TripA = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_TripB == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_TripB, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_TripB = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Vehicle_Configure == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Vehicle_Configure, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Vehicle_Configure = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Product_Reference == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_Visteon_Product_Reference, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_Product_Reference = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_station_Tracebility == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_station_Tracebility, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_station_Tracebility = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

  if (Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_user_product_configuration == 1)
  {
    (void)NvM_WriteBlock(NvMConf_NvMBlockDescriptor_user_product_configuration, NULL_PTR);
    Rte_DisableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    Rte_DirtyFlags.Rte_DirtyFlag_NvBlockSwComponentType_NVBlockDescriptor_user_product_configuration = 0;
    Rte_EnableOSInterrupts(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }

}



/**********************************************************************************************************************
 * Task bodies for RTE controlled tasks
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Task:     AppTask_10msLP
 * Priority: 5
 * Schedule: NON
 *********************************************************************************************************************/
TASK(AppTask_10msLP) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  EventMaskType ev;

  Rte_Task_Dispatch(AppTask_10msLP); /* PRQA S 3112 */ /* MD_Rte_3112 */

  for(;;)
  {
    Rte_Task_WaitEvent(AppTask_10msLP, Rte_Ev_Cyclic2_AppTask_10msLP_0_10ms | Rte_Ev_Cyclic_AppTask_10msLP_0_10ms); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)WaitEvent(Rte_Ev_Cyclic2_AppTask_10msLP_0_10ms | Rte_Ev_Cyclic_AppTask_10msLP_0_10ms); /* PRQA S 3417 */ /* MD_Rte_Os */
    Rte_Task_WaitEventRet(AppTask_10msLP, Rte_Ev_Cyclic2_AppTask_10msLP_0_10ms | Rte_Ev_Cyclic_AppTask_10msLP_0_10ms); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)GetEvent(AppTask_10msLP, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Cyclic2_AppTask_10msLP_0_10ms | Rte_Ev_Cyclic_AppTask_10msLP_0_10ms)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if ((ev & Rte_Ev_Cyclic_AppTask_10msLP_0_10ms) != (EventMaskType)0)
    {
      Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Cyclic2_AppTask_10msLP_0_10ms) != (EventMaskType)0)
    {
      Rte_CheckDetErrorContinue(Rte_InitState != RTE_STATE_UNINIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Cyclic_AppTask_10msLP_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_SpdMdl_CSpdMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSpdMdl_Impl_MainFunction();
      Rte_Runnable_SpdMdl_CSpdMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_TachMdl_CTachMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CTachMdl_Impl_MainFunction();
      Rte_Runnable_TachMdl_CTachMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Cyclic2_AppTask_10msLP_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_Dlt_Dlt_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      Dlt_MainFunction();
      Rte_Runnable_Dlt_Dlt_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Cyclic_AppTask_10msLP_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_DltExt_DltExt_Main_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      DltExt_Main();
      Rte_Runnable_DltExt_DltExt_Main_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_CDD_Test_CDD_Test_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CDD_Test_MainFunction();
      Rte_Runnable_CDD_Test_CDD_Test_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_AFCMdl_CAFCMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CAFCMdl_Impl_MainFunction();
      Rte_Runnable_AFCMdl_CAFCMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }
    (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_AppTask_10msLP);
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_100ms
 * Priority: 50
 * Schedule: NON
 *********************************************************************************************************************/
TASK(OsTask_100ms) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  EventMaskType ev;

  Rte_Task_Dispatch(OsTask_100ms); /* PRQA S 3112 */ /* MD_Rte_3112 */

  for(;;)
  {
    Rte_Task_WaitEvent(OsTask_100ms, Rte_Ev_Cyclic2_OsTask_100ms_0_1s | Rte_Ev_Cyclic_OsTask_100ms_0_100ms); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)WaitEvent(Rte_Ev_Cyclic2_OsTask_100ms_0_1s | Rte_Ev_Cyclic_OsTask_100ms_0_100ms); /* PRQA S 3417 */ /* MD_Rte_Os */
    Rte_Task_WaitEventRet(OsTask_100ms, Rte_Ev_Cyclic2_OsTask_100ms_0_1s | Rte_Ev_Cyclic_OsTask_100ms_0_100ms); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)GetEvent(OsTask_100ms, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Cyclic2_OsTask_100ms_0_1s | Rte_Ev_Cyclic_OsTask_100ms_0_100ms)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if ((ev & Rte_Ev_Cyclic_OsTask_100ms_0_100ms) != (EventMaskType)0)
    {
      Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Cyclic2_OsTask_100ms_0_1s) != (EventMaskType)0)
    {
      Rte_CheckDetErrorContinue(Rte_InitState != RTE_STATE_UNINIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Cyclic2_OsTask_100ms_0_1s) != (EventMaskType)0)
    {
      /* call schedulable entity */
      Can_MainFunction_BusOff();
    }

    if ((ev & Rte_Ev_Cyclic_OsTask_100ms_0_100ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_CoolantTmpMdl_CCoolantTmpMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CCoolantTmpMdl_Impl_MainFunction();
      Rte_Runnable_CoolantTmpMdl_CCoolantTmpMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Cyclic2_OsTask_100ms_0_1s) != (EventMaskType)0)
    {
      /* call schedulable entity */
      Can_MainFunction_Mode();
    }

    if ((ev & Rte_Ev_Cyclic_OsTask_100ms_0_100ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_FuelLevelMdl_CFuelLevelMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CFuelLevelMdl_Impl_MainFunction();
      Rte_Runnable_FuelLevelMdl_CFuelLevelMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_BarView_CBarView_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CBarView_Impl_MainFunction();
      Rte_Runnable_BarView_CBarView_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Cyclic2_OsTask_100ms_0_1s) != (EventMaskType)0)
    {
      /* call schedulable entity */
      Can_MainFunction_Wakeup();
    }

    if ((ev & Rte_Ev_Cyclic_OsTask_100ms_0_100ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_DimPwm_CDimPwm_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CDimPwm_Impl_MainFunction();
      Rte_Runnable_DimPwm_CDimPwm_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_AEBChime_CAEBChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CAEBChime_Impl_MainFunction();
      Rte_Runnable_AEBChime_CAEBChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_ChimeMdl_CChimeMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CChimeMdl_Impl_MainFunction();
      Rte_Runnable_ChimeMdl_CChimeMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_LightOnReminderChime_CLightOnReminderChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CLightOnReminderChime_Impl_MainFunction();
      Rte_Runnable_LightOnReminderChime_CLightOnReminderChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_WarningCtrl_CWarningCtrl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CWarningCtrl_Impl_MainFunction();
      Rte_Runnable_WarningCtrl_CWarningCtrl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_PosLmpChime_CPosLmpChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CPosLmpChime_Impl_MainFunction();
      Rte_Runnable_PosLmpChime_CPosLmpChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_NotInPChime_CNotInPChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CNotInPChime_Impl_MainFunction();
      Rte_Runnable_NotInPChime_CNotInPChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_AccWarning_CAccWarning_Impl_MainFuncion_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CAccWarning_Impl_MainFunction();
      Rte_Runnable_AccWarning_CAccWarning_Impl_MainFuncion_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_HmiAccMdl_CHmiAccMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CHmiAccMdl_Impl_MainFunction();
      Rte_Runnable_HmiAccMdl_CHmiAccMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_HmiAdasMdl_CHmiAdasMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CHmiAdasMdl_Impl_MainFunction();
      Rte_Runnable_HmiAdasMdl_CHmiAdasMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_DrvStyleMdl_CDrvStyleMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CDrvStyleMdl_Impl_MainFunction();
      Rte_Runnable_DrvStyleMdl_CDrvStyleMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_GearMdl_CGearMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CGearMdl_Impl_MainFunction();
      Rte_Runnable_GearMdl_CGearMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_PEPSWrnMdl_CPEPSWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CPEPSWrnMdl_Impl_MainFunction();
      Rte_Runnable_PEPSWrnMdl_CPEPSWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_AEBWrnMdl_CAEBWrnMdl_Iml_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CAEBWrnMdl_Impl_MainFunction();
      Rte_Runnable_AEBWrnMdl_CAEBWrnMdl_Iml_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_EpbWarning_CEpbWarning_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CEpbWarning_Impl_MainFunction();
      Rte_Runnable_EpbWarning_CEpbWarning_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_APAChime_CAPAChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CAPAChime_Impl_MainFunction();
      Rte_Runnable_APAChime_CAPAChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_DoorOpenChime_CDoorOpenChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CDoorOpenChime_Impl_MainFunction();
      Rte_Runnable_DoorOpenChime_CDoorOpenChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_DimDerateMdl_CDimDerateMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CDimDerateMdl_Impl_MainFunction();
      Rte_Runnable_DimDerateMdl_CDimDerateMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_KeyInOnChime_CKeyInOnChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CKeyInOnChime_Impl_MainFunction();
      Rte_Runnable_KeyInOnChime_CKeyInOnChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_AdasChime_CAdasChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CAdasChime_Impl_MainFunction();
      Rte_Runnable_AdasChime_CAdasChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_ImmoChime_CImmoChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CImmoChime_Impl_MainFunction();
      Rte_Runnable_ImmoChime_CImmoChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_TpmsChimeMdl_CTpmsChimeMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CTpmsChimeMdl_Impl_MainFunction();
      Rte_Runnable_TpmsChimeMdl_CTpmsChimeMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_SCRChimeMdl_CSCRChimeMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSCRChimeMdl_Impl_MainFunction();
      Rte_Runnable_SCRChimeMdl_CSCRChimeMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_BkDisChime_CBkDisChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CBkDisChime_Impl_MainFunction();
      Rte_Runnable_BkDisChime_CBkDisChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_FrnRadarChime_CFrnRadarChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CFrnRadarChime_Impl_MainFunction();
      Rte_Runnable_FrnRadarChime_CFrnRadarChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_PepsChime_CPepsChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CPepsChime_Impl_MainFunction();
      Rte_Runnable_PepsChime_CPepsChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_SeatbeltChime_CSeatbeltChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSeatbeltChime_Impl_MainFunction();
      Rte_Runnable_SeatbeltChime_CSeatbeltChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_CDPFailureWrnMdl_CCDPFailureWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CCDPFailureWrnMdl_Impl_MainFunction();
      Rte_Runnable_CDPFailureWrnMdl_CCDPFailureWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_RtcCdd_CRtcCdd_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CRtcCdd_Impl_MainFunction();
      Rte_Runnable_RtcCdd_CRtcCdd_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_TPMSWrnMdl_CTPMSWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CTPMSWrnMdl_Impl_MainFunction();
      Rte_Runnable_TPMSWrnMdl_CTPMSWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_NotInPWrnMdl_CNotInPWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CNotInPWrnMdl_Impl_MainFunction();
      Rte_Runnable_NotInPWrnMdl_CNotInPWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_EPBWrnMdl_CEPBWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CEPBWrnMdl_Impl_MainFunction();
      Rte_Runnable_EPBWrnMdl_CEPBWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_ResetTripWrnMdl_CResetTripWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CResetTripWrnMdl_Impl_MainFunction();
      Rte_Runnable_ResetTripWrnMdl_CResetTripWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_GLOWDisplayWrnMdl_CGLOWDisplayWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CGLOWDisplayWrnMdl_Impl_MainFunction();
      Rte_Runnable_GLOWDisplayWrnMdl_CGLOWDisplayWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_DoorAjarWrnMdl_CDoorAjarWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CDoorAjarWrnMdl_Impl_MainFunction();
      Rte_Runnable_DoorAjarWrnMdl_CDoorAjarWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_DteMdl_CDTEMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CDTEMdl_Impl_MainFunction();
      Rte_Runnable_DteMdl_CDTEMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_DPFWrnMdl_CDPFWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CDPFWrnMdl_Impl_MainFunction();
      Rte_Runnable_DPFWrnMdl_CDPFWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_SCRWrnMdl_CSCRWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSCRWrnMdl_Impl_MainFunction();
      Rte_Runnable_SCRWrnMdl_CSCRWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_TCUWrnMdl_CTCUWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CTCUWrnMdl_Impl_MainFunction();
      Rte_Runnable_TCUWrnMdl_CTCUWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_PkBrkChime_CPkBrkChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CPkBrkChime_Impl_MainFunction();
      Rte_Runnable_PkBrkChime_CPkBrkChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_TurnIndHazardChime_CTurnIndHazardChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CTurnIndHazardChime_Impl_MainFunction();
      Rte_Runnable_TurnIndHazardChime_CTurnIndHazardChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_VehicleCfgMdl_CVehicleCfgMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CVehicleCfgMdl_Impl_MainFunction();
      Rte_Runnable_VehicleCfgMdl_CVehicleCfgMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_BkHomeChimeMdl_CBkHomeChimeMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CBkHomeChimeMdl_Impl_MainFunction();
      Rte_Runnable_BkHomeChimeMdl_CBkHomeChimeMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_DPFDTEMdl_CDPFDTEMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CDPFDTEMdl_Impl_MainFunction();
      Rte_Runnable_DPFDTEMdl_CDPFDTEMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_STTWrnMdl_CSTTWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSTTWrnMdl_Impl_MainFunction();
      Rte_Runnable_STTWrnMdl_CSTTWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_IFCMdl_CIFCMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CIFCMdl_Impl_MainFunction();
      Rte_Runnable_IFCMdl_CIFCMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_LampFaultWrnMdl_CLampFaultWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CLampFaultWrnMdl_Impl_MainFunction();
      Rte_Runnable_LampFaultWrnMdl_CLampFaultWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_ATSChime_CATSChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CATSChime_Impl_MainFunction();
      Rte_Runnable_ATSChime_CATSChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_STTChimeMdl_CSTTChimeMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSTTChimeMdl_Impl_MainFunction();
      Rte_Runnable_STTChimeMdl_CSTTChimeMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_LowFuelChime_CLowFuelChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CLowFuelChime_Impl_MainFunction();
      Rte_Runnable_LowFuelChime_CLowFuelChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_EasyNaviMdl_CEasyNaviMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CEasyNaviMdl_Impl_MainFunction();
      Rte_Runnable_EasyNaviMdl_CEasyNaviMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_OverSpdChime_COverSpdChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      COverSpdChime_Impl_MainFunction();
      Rte_Runnable_OverSpdChime_COverSpdChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_OilpressChime_COilpressChime_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      COilpressChime_Impl_MainFunction();
      Rte_Runnable_OilpressChime_COilpressChime_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_VehicleModeMdl_CVehicleModeMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CVehicleModeMdl_Impl_MainFunction();
      Rte_Runnable_VehicleModeMdl_CVehicleModeMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_ParkBrakeWrnMdl_CParkBrakeWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CParkBrakeWrnMdl_Impl_MainFunction();
      Rte_Runnable_ParkBrakeWrnMdl_CParkBrakeWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_LowBrakeFluidWrnMdl_CLowBrakeFluidWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CLowBrakeFluidWrnMdl_Impl_MainFunction();
      Rte_Runnable_LowBrakeFluidWrnMdl_CLowBrakeFluidWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_SVSWrnMdl_CSVSWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSVSWrnMdl_Impl_MainFunction();
      Rte_Runnable_SVSWrnMdl_CSVSWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_EngHighTempWrnMdl_CEngHighTempWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CEngHighTempWrnMdl_Impl_MainFunction();
      Rte_Runnable_EngHighTempWrnMdl_CEngHighTempWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_LowOilPressureWrnMdl_CLowOilPressureWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CLowOilPressureWrnMdl_Impl_MainFunction();
      Rte_Runnable_LowOilPressureWrnMdl_CLowOilPressureWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_EngHighTempChimeMdl_CEngHighTempChimeMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CEngHighTempChimeMdl_Impl_MainFunction();
      Rte_Runnable_EngHighTempChimeMdl_CEngHighTempChimeMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_KeyBattLowWrnMdl_CKeyBattLowWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CKeyBattLowWrnMdl_Impl_MainFunction();
      Rte_Runnable_KeyBattLowWrnMdl_CKeyBattLowWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_StpClutchWrnMdl_CStpClutchWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CStpClutchWrnMdl_Impl_MainFunction();
      Rte_Runnable_StpClutchWrnMdl_CStpClutchWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_CruiseCtrlWrnMdl_CCruiseCtrlWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CCruiseCtrlWrnMdl_Impl_MainFunction();
      Rte_Runnable_CruiseCtrlWrnMdl_CCruiseCtrlWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }
    (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_100ms);
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_10msCAN
 * Priority: 210
 * Schedule: NON
 *********************************************************************************************************************/
TASK(OsTask_10msCAN) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  EventMaskType ev;

  Rte_Task_Dispatch(OsTask_10msCAN); /* PRQA S 3112 */ /* MD_Rte_3112 */

  for(;;)
  {
    Rte_Task_WaitEvent(OsTask_10msCAN, Rte_Ev_Cyclic2_OsTask_10msCAN_0_10ms | Rte_Ev_Cyclic2_OsTask_10msCAN_0_20ms | Rte_Ev_Cyclic_OsTask_10msCAN_0_10ms); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)WaitEvent(Rte_Ev_Cyclic2_OsTask_10msCAN_0_10ms | Rte_Ev_Cyclic2_OsTask_10msCAN_0_20ms | Rte_Ev_Cyclic_OsTask_10msCAN_0_10ms); /* PRQA S 3417 */ /* MD_Rte_Os */
    Rte_Task_WaitEventRet(OsTask_10msCAN, Rte_Ev_Cyclic2_OsTask_10msCAN_0_10ms | Rte_Ev_Cyclic2_OsTask_10msCAN_0_20ms | Rte_Ev_Cyclic_OsTask_10msCAN_0_10ms); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)GetEvent(OsTask_10msCAN, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Cyclic2_OsTask_10msCAN_0_10ms | Rte_Ev_Cyclic2_OsTask_10msCAN_0_20ms | Rte_Ev_Cyclic_OsTask_10msCAN_0_10ms)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if ((ev & Rte_Ev_Cyclic_OsTask_10msCAN_0_10ms) != (EventMaskType)0)
    {
      Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if (((ev & Rte_Ev_Cyclic2_OsTask_10msCAN_0_10ms) != (EventMaskType)0) || ((ev & Rte_Ev_Cyclic2_OsTask_10msCAN_0_20ms) != (EventMaskType)0))
    {
      Rte_CheckDetErrorContinue(Rte_InitState != RTE_STATE_UNINIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Cyclic2_OsTask_10msCAN_0_10ms) != (EventMaskType)0)
    {
      /* call schedulable entity */
      Can_MainFunction_Read();

      /* call schedulable entity */
      Can_MainFunction_Write();
    }

    if ((ev & Rte_Ev_Cyclic_OsTask_10msCAN_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_Can_Dispatcher_CanCcl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CanCcl_MainFunction();
      Rte_Runnable_Can_Dispatcher_CanCcl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Cyclic2_OsTask_10msCAN_0_20ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_ComM_ComM_MainFunction_0_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      ComM_MainFunction_0();
      Rte_Runnable_ComM_ComM_MainFunction_0_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Cyclic_OsTask_10msCAN_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_ComVnimMdl_CComVnimMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CComVnimMdl_Impl_MainFunction();
      Rte_Runnable_ComVnimMdl_CComVnimMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_Dcm_Dcm_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      Dcm_MainFunction();
      Rte_Runnable_Dcm_Dcm_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Cyclic2_OsTask_10msCAN_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_Dem_Dem_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      Dem_MainFunction();
      Rte_Runnable_Dem_Dem_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }
    (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_10msCAN);
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_10msEcuM
 * Priority: 200
 * Schedule: NON
 *********************************************************************************************************************/
TASK(OsTask_10msEcuM) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  EventMaskType ev;

  Rte_Task_Dispatch(OsTask_10msEcuM); /* PRQA S 3112 */ /* MD_Rte_3112 */

  for(;;)
  {
    Rte_Task_WaitEvent(OsTask_10msEcuM, Rte_Ev_Cyclic2_OsTask_10msEcuM_0_10ms | Rte_Ev_Cyclic_OsTask_10msEcuM_0_10ms); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)WaitEvent(Rte_Ev_Cyclic2_OsTask_10msEcuM_0_10ms | Rte_Ev_Cyclic_OsTask_10msEcuM_0_10ms); /* PRQA S 3417 */ /* MD_Rte_Os */
    Rte_Task_WaitEventRet(OsTask_10msEcuM, Rte_Ev_Cyclic2_OsTask_10msEcuM_0_10ms | Rte_Ev_Cyclic_OsTask_10msEcuM_0_10ms); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)GetEvent(OsTask_10msEcuM, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Cyclic2_OsTask_10msEcuM_0_10ms | Rte_Ev_Cyclic_OsTask_10msEcuM_0_10ms)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if ((ev & Rte_Ev_Cyclic_OsTask_10msEcuM_0_10ms) != (EventMaskType)0)
    {
      Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Cyclic2_OsTask_10msEcuM_0_10ms) != (EventMaskType)0)
    {
      Rte_CheckDetErrorContinue(Rte_InitState != RTE_STATE_UNINIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Cyclic2_OsTask_10msEcuM_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_EcuM_EcuM_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      EcuM_MainFunction();
      Rte_Runnable_EcuM_EcuM_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_BswM_BswM_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      BswM_MainFunction();
      Rte_Runnable_BswM_BswM_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Cyclic_OsTask_10msEcuM_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_TmExt_TmExt_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      TmExt_MainFunction();
      Rte_Runnable_TmExt_TmExt_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_EcumExt_CEcumExt_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CEcumExt_Impl_MainFunction();
      Rte_Runnable_EcumExt_CEcumExt_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_AppCtrl_AppCtrl_Mainfunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      AppCtrl_Mainfunction();
      Rte_Runnable_AppCtrl_AppCtrl_Mainfunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_OdoMdl_COdoMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      COdoMdl_Impl_MainFunction();
      Rte_Runnable_OdoMdl_COdoMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_OdoMdl_COdoLogicalMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      COdoLogicalMdl_Impl_MainFunction();
      Rte_Runnable_OdoMdl_COdoLogicalMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_SIAMdl_CSIAMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSIAMdl_Impl_MainFunction();
      Rte_Runnable_SIAMdl_CSIAMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_StpCddMdl_CStpCddMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CStpCddMdl_Impl_MainFunction();
      Rte_Runnable_StpCddMdl_CStpCddMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_NVMExtMdl_CNVMExtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CNVMExtMdl_Impl_MainFunction();
      Rte_Runnable_NVMExtMdl_CNVMExtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_TripMdl_CTripMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CTripMdl_Impl_MainFunction();
      Rte_Runnable_TripMdl_CTripMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_CanEOLMdl_CanEOLMdl_Runable_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CanEOLMdl_Runable();
      Rte_Runnable_CanEOLMdl_CanEOLMdl_Runable_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }
    (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_10msEcuM);
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_2ms
 * Priority: 250
 * Schedule: NON
 * Alarm:    Cycle Time 0.002 s Alarm Offset 0 s
 *********************************************************************************************************************/
TASK(OsTask_2ms) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_Dispatch(OsTask_2ms); /* PRQA S 3112 */ /* MD_Rte_3112 */

  /* call runnable */
  Rte_Runnable_SndCdd_CSndCdd_ToneProcess_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  CSndCdd_ToneProcess();
  Rte_Runnable_SndCdd_CSndCdd_ToneProcess_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_2ms);

  (void)TerminateTask(); /* PRQA S 3417 */ /* MD_Rte_Os */
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_30ms
 * Priority: 150
 * Schedule: NON
 *********************************************************************************************************************/
TASK(OsTask_30ms) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  EventMaskType ev;

  Rte_Task_Dispatch(OsTask_30ms); /* PRQA S 3112 */ /* MD_Rte_3112 */

  for(;;)
  {
    Rte_Task_WaitEvent(OsTask_30ms, Rte_Ev_Cyclic_OsTask_30ms_0_30ms | Rte_Ev_Run_BtnMdl_CBtnMdl_Impl_MainFunction); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)WaitEvent(Rte_Ev_Cyclic_OsTask_30ms_0_30ms | Rte_Ev_Run_BtnMdl_CBtnMdl_Impl_MainFunction); /* PRQA S 3417 */ /* MD_Rte_Os */
    Rte_Task_WaitEventRet(OsTask_30ms, Rte_Ev_Cyclic_OsTask_30ms_0_30ms | Rte_Ev_Run_BtnMdl_CBtnMdl_Impl_MainFunction); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)GetEvent(OsTask_30ms, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Cyclic_OsTask_30ms_0_30ms | Rte_Ev_Run_BtnMdl_CBtnMdl_Impl_MainFunction)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if (((ev & Rte_Ev_Cyclic_OsTask_30ms_0_30ms) != (EventMaskType)0) || ((ev & Rte_Ev_Run_BtnMdl_CBtnMdl_Impl_MainFunction) != (EventMaskType)0))
    {
      Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Cyclic_OsTask_30ms_0_30ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_IoHwAb_CIoHwAb_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CIoHwAb_Impl_MainFunction();
      Rte_Runnable_IoHwAb_CIoHwAb_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_BattMdl_CBattMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CBattMdl_Impl_MainFunction();
      Rte_Runnable_BattMdl_CBattMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_TtMdl_CTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CTtMdl_Impl_MainFunction();
      Rte_Runnable_TtMdl_CTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Run_BtnMdl_CBtnMdl_Impl_MainFunction) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_BtnMdl_CBtnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CBtnMdl_Impl_MainFunction();
      Rte_Runnable_BtnMdl_CBtnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Cyclic_OsTask_30ms_0_30ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_TtView_CTtView_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CTtView_Impl_MainFunction();
      Rte_Runnable_TtView_CTtView_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_PosLightMdl_CPosLightMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CPosLightMdl_Impl_MainFunction();
      Rte_Runnable_PosLightMdl_CPosLightMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_HighBeamMdl_CHighBeamMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CHighBeamMdl_Impl_MainFunction();
      Rte_Runnable_HighBeamMdl_CHighBeamMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_ChargeMdl_CChargeMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CChargeMdl_Impl_MainFunction();
      Rte_Runnable_ChargeMdl_CChargeMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_OilpressMdl_COilpressMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      COilpressMdl_Impl_MainFunction();
      Rte_Runnable_OilpressMdl_COilpressMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_EngineCoolantMdl_CEngineCoolantMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CEngineCoolantMdl_Impl_MainFunction();
      Rte_Runnable_EngineCoolantMdl_CEngineCoolantMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_LowFuelMdl_CLowFuelMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CLowFuelMdl_Impl_MainFunction();
      Rte_Runnable_LowFuelMdl_CLowFuelMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_AirbagMdl_CAirbagMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CAirbagMdl_Impl_MainFunction();
      Rte_Runnable_AirbagMdl_CAirbagMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_EpsWarnMdl_CEpsWarnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CEpsWarnMdl_Impl_MainFunction();
      Rte_Runnable_EpsWarnMdl_CEpsWarnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_TpmsMdl_CTpmsMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CTpmsMdl_Impl_MainFunction();
      Rte_Runnable_TpmsMdl_CTpmsMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_EpbWarnMdl_CEpbWarnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CEpbWarnMdl_Impl_MainFunction();
      Rte_Runnable_EpbWarnMdl_CEpbWarnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_SiaTtMdl_CSiaTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSiaTtMdl_Impl_MainFunction();
      Rte_Runnable_SiaTtMdl_CSiaTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_APBTtMdl_CAPBTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CAPBTtMdl_Impl_MainFunction();
      Rte_Runnable_APBTtMdl_CAPBTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_ABSTtMdl_CABSTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CABSTtMdl_Impl_MainFunction();
      Rte_Runnable_ABSTtMdl_CABSTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_EBDTtMdl_CEBDTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CEBDTtMdl_Impl_MainFunction();
      Rte_Runnable_EBDTtMdl_CEBDTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_OBDTtMdl_COBDTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      COBDTtMdl_Impl_MainFunction();
      Rte_Runnable_OBDTtMdl_COBDTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_SVSTtMdl_CSVSTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSVSTtMdl_Impl_MainFunction();
      Rte_Runnable_SVSTtMdl_CSVSTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_ESPTtMdl_CESPTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CESPTtMdl_Impl_MainFunction();
      Rte_Runnable_ESPTtMdl_CESPTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_IMMOTtMdl_CIMMOTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CIMMOTtMdl_Impl_MainFunction();
      Rte_Runnable_IMMOTtMdl_CIMMOTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_ESPOffTtMdl_CESPOffTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CESPOffTtMdl_Impl_MainFunction();
      Rte_Runnable_ESPOffTtMdl_CESPOffTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_ESCLTtMdl_CESCLTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CESCLTtMdl_Impl_MainFunction();
      Rte_Runnable_ESCLTtMdl_CESCLTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_PEPSTtMdl_CPEPSTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CPEPSTtMdl_Impl_MainFunction();
      Rte_Runnable_PEPSTtMdl_CPEPSTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_LeftRightTurnTtMdl_CLeftRightTurnTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CLeftRightTurnTtMdl_Impl_MainFunction();
      Rte_Runnable_LeftRightTurnTtMdl_CLeftRightTurnTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_CruiseTtMdl_CCruiseTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CCruiseTtMdl_Impl_MainFunction();
      Rte_Runnable_CruiseTtMdl_CCruiseTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_OilWaterTtMdl_COilWaterTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      COilWaterTtMdl_Impl_MainFunction();
      Rte_Runnable_OilWaterTtMdl_COilWaterTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_AVHTtMdl_CAVHTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CAVHTtMdl_Impl_MainFunction();
      Rte_Runnable_AVHTtMdl_CAVHTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_FourWheelDriveHTtMdl_CFourWheelDriveHTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CFourWheelDriveHTtMdl_Impl_MainFunction();
      Rte_Runnable_FourWheelDriveHTtMdl_CFourWheelDriveHTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_FourWheelDriveLTtMdl_CFourWheelDriveLTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CFourWheelDriveLTtMdl_Impl_MainFunction();
      Rte_Runnable_FourWheelDriveLTtMdl_CFourWheelDriveLTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_HdcTtMdl_CHdcTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CHdcTtMdl_Impl_MainFunction();
      Rte_Runnable_HdcTtMdl_CHdcTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_GlowTtMdl_CGlowTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CGlowTtMdl_Impl_MainFunction();
      Rte_Runnable_GlowTtMdl_CGlowTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_ClockMdl_CClockMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CClockMdl_Impl_MainFunction();
      Rte_Runnable_ClockMdl_CClockMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_AmbientMdl_CAmbientMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CAmbientMdl_Impl_MainFunction();
      Rte_Runnable_AmbientMdl_CAmbientMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_SeatBeltMdl_CSeatBeltMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSeatBeltMdl_Impl_MainFunction();
      Rte_Runnable_SeatBeltMdl_CSeatBeltMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_FrontFogMdl_CFrontFogMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CFrontFogMdl_Impl_MainFunction();
      Rte_Runnable_FrontFogMdl_CFrontFogMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_BackFogMdl_CBackFogMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CBackFogMdl_Impl_MainFunction();
      Rte_Runnable_BackFogMdl_CBackFogMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_TPMSRoutineMdl_CTPMSRoutineMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CTPMSRoutineMdl_Impl_MainFunction();
      Rte_Runnable_TPMSRoutineMdl_CTPMSRoutineMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_DPFMdl_CDPFMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CDPFMdl_Impl_MainFunction();
      Rte_Runnable_DPFMdl_CDPFMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_STTMdl_CSTTMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSTTMdl_Impl_MainFunction();
      Rte_Runnable_STTMdl_CSTTMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_GearBoxTtMdl_CGearBoxTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CGearBoxTtMdl_Impl_MainFunction();
      Rte_Runnable_GearBoxTtMdl_CGearBoxTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_FourWDFaultTtMdl_CFourWDFaultTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CFourWDFaultTtMdl_Impl_MainFunction();
      Rte_Runnable_FourWDFaultTtMdl_CFourWDFaultTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_AccMdl_CAccMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CAccMdl_Impl_MainFunction();
      Rte_Runnable_AccMdl_CAccMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_PowerMangementWrnMdl_CPowerMangementWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CPowerMangementWrnMdl_Impl_MainFunction();
      Rte_Runnable_PowerMangementWrnMdl_CPowerMangementWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_SCRTtMdl_CSCRTtMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CSCRTtMdl_Impl_MainFunction();
      Rte_Runnable_SCRTtMdl_CSCRTtMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }
    (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_30ms);
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_30msSnd
 * Priority: 160
 * Schedule: NON
 * Alarm:    Cycle Time 0.03 s Alarm Offset 0 s
 *********************************************************************************************************************/
TASK(OsTask_30msSnd) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_Dispatch(OsTask_30msSnd); /* PRQA S 3112 */ /* MD_Rte_3112 */

  /* call runnable */
  Rte_Runnable_SndCdd_CSndCdd_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  CSndCdd_Impl_MainFunction();
  Rte_Runnable_SndCdd_CSndCdd_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_30msSnd);

  (void)TerminateTask(); /* PRQA S 3417 */ /* MD_Rte_Os */
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_50ms
 * Priority: 100
 * Schedule: NON
 * Alarm:    Cycle Time 0.05 s Alarm Offset 0 s
 *********************************************************************************************************************/
TASK(OsTask_50ms) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_Dispatch(OsTask_50ms); /* PRQA S 3112 */ /* MD_Rte_3112 */

  /* call runnable */
  Rte_Runnable_ModMdl_CModMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  CModMdl_Impl_MainFunction();
  Rte_Runnable_ModMdl_CModMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

  /* call runnable */
  Rte_Runnable_DcmExt_dummy_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  dummy();
  Rte_Runnable_DcmExt_dummy_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

  /* call runnable */
  Rte_Runnable_CruiseMdl_CCruiseMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  CCruiseMdl_Impl_MainFunction();
  Rte_Runnable_CruiseMdl_CCruiseMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_50ms);

  (void)TerminateTask(); /* PRQA S 3417 */ /* MD_Rte_Os */
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_5ms
 * Priority: 215
 * Schedule: NON
 * Alarm:    Cycle Time 0.005 s Alarm Offset 0 s
 *********************************************************************************************************************/
TASK(OsTask_5ms) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_Dispatch(OsTask_5ms); /* PRQA S 3112 */ /* MD_Rte_3112 */

  /* call runnable */
  Rte_Runnable_LAP_AVCV_CLAP_AVCV_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  CLAP_AVCV_Impl_MainFunction();
  Rte_Runnable_LAP_AVCV_CLAP_AVCV_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_5ms);

  (void)TerminateTask(); /* PRQA S 3417 */ /* MD_Rte_Os */
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_8ms
 * Priority: 6
 * Schedule: NON
 *********************************************************************************************************************/
TASK(OsTask_8ms) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  EventMaskType ev;

  Rte_Task_Dispatch(OsTask_8ms); /* PRQA S 3112 */ /* MD_Rte_3112 */

  for(;;)
  {
    Rte_Task_WaitEvent(OsTask_8ms, Rte_Ev_Run_CDD_Test_CDD_Test_WdgM_Stub | Rte_Ev_Run_WdgM_WdgM_MainFunction); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)WaitEvent(Rte_Ev_Run_CDD_Test_CDD_Test_WdgM_Stub | Rte_Ev_Run_WdgM_WdgM_MainFunction); /* PRQA S 3417 */ /* MD_Rte_Os */
    Rte_Task_WaitEventRet(OsTask_8ms, Rte_Ev_Run_CDD_Test_CDD_Test_WdgM_Stub | Rte_Ev_Run_WdgM_WdgM_MainFunction); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)GetEvent(OsTask_8ms, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Run_CDD_Test_CDD_Test_WdgM_Stub | Rte_Ev_Run_WdgM_WdgM_MainFunction)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if ((ev & Rte_Ev_Run_CDD_Test_CDD_Test_WdgM_Stub) != (EventMaskType)0)
    {
      Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Run_WdgM_WdgM_MainFunction) != (EventMaskType)0)
    {
      Rte_CheckDetErrorContinue(Rte_InitState != RTE_STATE_UNINIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Run_WdgM_WdgM_MainFunction) != (EventMaskType)0)
    {
      /* call schedulable entity */
      WdgM_MainFunction();
    }

    if ((ev & Rte_Ev_Run_CDD_Test_CDD_Test_WdgM_Stub) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_CDD_Test_CDD_Test_WdgM_Stub_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CDD_Test_WdgM_Stub();
      Rte_Runnable_CDD_Test_CDD_Test_WdgM_Stub_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }
    (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_8ms);
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_LSH_16ms
 * Priority: 180
 * Schedule: NON
 *********************************************************************************************************************/
TASK(OsTask_LSH_16ms) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  EventMaskType ev;

  Rte_Task_Dispatch(OsTask_LSH_16ms); /* PRQA S 3112 */ /* MD_Rte_3112 */

  for(;;)
  {
    Rte_Task_WaitEvent(OsTask_LSH_16ms, Rte_Ev_Cyclic_OsTask_LSH_16ms_0_16ms | Rte_Ev_Run_GdtCdd_CGdtCdd_Impl_OnCommand | Rte_Ev_Run_GdtCtrl_CGdtCtrl_Impl_OnCommand | Rte_Ev_Run_GdtCtrl_GdtCtrl_RenderDone); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)WaitEvent(Rte_Ev_Cyclic_OsTask_LSH_16ms_0_16ms | Rte_Ev_Run_GdtCdd_CGdtCdd_Impl_OnCommand | Rte_Ev_Run_GdtCtrl_CGdtCtrl_Impl_OnCommand | Rte_Ev_Run_GdtCtrl_GdtCtrl_RenderDone); /* PRQA S 3417 */ /* MD_Rte_Os */
    Rte_Task_WaitEventRet(OsTask_LSH_16ms, Rte_Ev_Cyclic_OsTask_LSH_16ms_0_16ms | Rte_Ev_Run_GdtCdd_CGdtCdd_Impl_OnCommand | Rte_Ev_Run_GdtCtrl_CGdtCtrl_Impl_OnCommand | Rte_Ev_Run_GdtCtrl_GdtCtrl_RenderDone); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)GetEvent(OsTask_LSH_16ms, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Cyclic_OsTask_LSH_16ms_0_16ms | Rte_Ev_Run_GdtCdd_CGdtCdd_Impl_OnCommand | Rte_Ev_Run_GdtCtrl_CGdtCtrl_Impl_OnCommand | Rte_Ev_Run_GdtCtrl_GdtCtrl_RenderDone)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if (((ev & Rte_Ev_Cyclic_OsTask_LSH_16ms_0_16ms) != (EventMaskType)0) || ((ev & Rte_Ev_Run_GdtCdd_CGdtCdd_Impl_OnCommand) != (EventMaskType)0) || ((ev & Rte_Ev_Run_GdtCtrl_CGdtCtrl_Impl_OnCommand) != (EventMaskType)0) || ((ev & Rte_Ev_Run_GdtCtrl_GdtCtrl_RenderDone) != (EventMaskType)0))
    {
      Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Cyclic_OsTask_LSH_16ms_0_16ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_GdtCtrl_CGdtCtrl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CGdtCtrl_Impl_MainFunction();
      Rte_Runnable_GdtCtrl_CGdtCtrl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_GdtCdd_CGdtCdd_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CGdtCdd_Impl_MainFunction();
      Rte_Runnable_GdtCdd_CGdtCdd_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Run_GdtCdd_CGdtCdd_Impl_OnCommand) != (EventMaskType)0)
    {
      {
        Rte_CS_ServerQueueInfo_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_Active = 0;

        /* call runnable */
        Rte_Runnable_GdtCdd_CGdtCdd_Impl_OnCommand_Start(Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.cmd); /* PRQA S 3112 */ /* MD_Rte_3112 */
        Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_Result = CGdtCdd_Impl_OnCommand(Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.cmd);
        Rte_Runnable_GdtCdd_CGdtCdd_Impl_OnCommand_Return(Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.cmd); /* PRQA S 3112 */ /* MD_Rte_3112 */

        if (Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_TaskID != INVALID_TASK)
        {
          Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_ServerCompleted = TRUE;
          Rte_Task_SetEvent(Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_TaskID, RTE_TASK_WAITPOINT_EVENT_MASK); /* PRQA S 3112 */ /* MD_Rte_3112 */
          (void)SetEvent(Rte_CS_ServerQueue_GdtCdd_ppCS_GdtCdd_TISystem_OnCommand.Rte_TaskID, RTE_TASK_WAITPOINT_EVENT_MASK); /* PRQA S 3417 */ /* MD_Rte_Os */
        }

        (void)Schedule(); /* PRQA S 3417 */ /* MD_Rte_Os */
      }
    }

    if ((ev & Rte_Ev_Run_GdtCtrl_CGdtCtrl_Impl_OnCommand) != (EventMaskType)0)
    {
      {
        Rte_CS_ServerQueueInfo_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_Active = 0;

        /* call runnable */
        Rte_Runnable_GdtCtrl_CGdtCtrl_Impl_OnCommand_Start(Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.cmd); /* PRQA S 3112 */ /* MD_Rte_3112 */
        Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_Result = CGdtCtrl_Impl_OnCommand(Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.cmd);
        Rte_Runnable_GdtCtrl_CGdtCtrl_Impl_OnCommand_Return(Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.cmd); /* PRQA S 3112 */ /* MD_Rte_3112 */

        if (Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_TaskID != INVALID_TASK)
        {
          Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_ServerCompleted = TRUE;
          Rte_Task_SetEvent(Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_TaskID, RTE_TASK_WAITPOINT_EVENT_MASK); /* PRQA S 3112 */ /* MD_Rte_3112 */
          (void)SetEvent(Rte_CS_ServerQueue_GdtCtrl_ppCS_GdtCtrl_TISystem_OnCommand.Rte_TaskID, RTE_TASK_WAITPOINT_EVENT_MASK); /* PRQA S 3417 */ /* MD_Rte_Os */
        }

        (void)Schedule(); /* PRQA S 3417 */ /* MD_Rte_Os */
      }
    }

    if ((ev & Rte_Ev_Run_GdtCtrl_GdtCtrl_RenderDone) != (EventMaskType)0)
    {
      {
        uint8 clientId;
        Rte_CS_ServerQueueInfo_GdtCtrl_TIGdtCtrl_RenderDone.Rte_Active = 0;
        clientId = Rte_CS_ServerQueue_GdtCtrl_TIGdtCtrl_RenderDone.Rte_ClientId;

        /* call runnable */
        Rte_Runnable_GdtCtrl_GdtCtrl_RenderDone_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
        GdtCtrl_RenderDone();
        Rte_Runnable_GdtCtrl_GdtCtrl_RenderDone_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

        Rte_CS_ClientConfig_GdtCtrl_TIGdtCtrl_RenderDone[clientId].Rte_ClientQueue->Rte_CallCompleted = TRUE;

        Rte_CS_ServerQueueInfo_GdtCtrl_TIGdtCtrl_RenderDone.Rte_Free = 1;

        (void)Schedule(); /* PRQA S 3417 */ /* MD_Rte_Os */
      }
    }

    if ((ev & Rte_Ev_Cyclic_OsTask_LSH_16ms_0_16ms) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_ESPWrnMdl_CESPWrnMdl_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CESPWrnMdl_Impl_MainFunction();
      Rte_Runnable_ESPWrnMdl_CESPWrnMdl_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

      /* call runnable */
      Rte_Runnable_PtrBaseView_CPtrBaseView_Impl_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      CPtrBaseView_Impl_MainFunction();
      Rte_Runnable_PtrBaseView_CPtrBaseView_Impl_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }
    (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_LSH_16ms);
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_NvmStack
 * Priority: 0
 * Schedule: NON
 *********************************************************************************************************************/
TASK(OsTask_NvmStack) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_Dispatch(OsTask_NvmStack); /* PRQA S 3112 */ /* MD_Rte_3112 */

  if (Rte_ScheduleTable_OsTask_NvmStack_Step == 0)
  {
    /* call runnable */
    Rte_Runnable_Eep24xx_Eep24xx_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    Eep24xx_MainFunction();
    Rte_Runnable_Eep24xx_Eep24xx_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

    /* call runnable */
    Rte_Runnable_NvM_NvM_MainFunction_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    NvM_MainFunction();
    Rte_Runnable_NvM_NvM_MainFunction_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

    /* call schedulable entity */
    Ea_MainFunction();

    /* call schedulable entity */
    Fee_MainFunction();

    /* call schedulable entity */
    I2c_MainFunction();

    /* call schedulable entity */
    Fls_MainFunction();

    /* call runnable */
    Rte_Runnable_EcumExt_CEcumExt_NvM_ReadAll_SpeedUp_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    CEcumExt_NvM_ReadAll_SpeedUp();
    Rte_Runnable_EcumExt_CEcumExt_NvM_ReadAll_SpeedUp_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */

    Rte_ScheduleTable_OsTask_NvmStack_Step = 0;
  }
  (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_NvmStack);

  (void)TerminateTask(); /* PRQA S 3417 */ /* MD_Rte_Os */
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_RteEvent
 * Priority: 3
 * Schedule: NON
 *********************************************************************************************************************/
TASK(OsTask_RteEvent) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  EventMaskType ev;

  Rte_Task_Dispatch(OsTask_RteEvent); /* PRQA S 3112 */ /* MD_Rte_3112 */

  for(;;)
  {
    Rte_Task_WaitEvent(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable | Rte_Ev_Run_OdoMdl_OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)WaitEvent(Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable | Rte_Ev_Run_OdoMdl_OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished); /* PRQA S 3417 */ /* MD_Rte_Os */
    Rte_Task_WaitEventRet(OsTask_RteEvent, Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable | Rte_Ev_Run_OdoMdl_OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished); /* PRQA S 3112 */ /* MD_Rte_3112 */
    (void)GetEvent(OsTask_RteEvent, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable | Rte_Ev_Run_OdoMdl_OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if (((ev & Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable) != (EventMaskType)0) || ((ev & Rte_Ev_Run_OdoMdl_OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished) != (EventMaskType)0))
    {
      Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
    }

    if ((ev & Rte_Ev_Run_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable) != (EventMaskType)0)
    {
      /* call runnable */
      Rte_Runnable_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
      NvBlockSwComponentTypeRunnable();
      Rte_Runnable_NvBlockSwComponentType_NvBlockSwComponentTypeRunnable_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
    }

    if ((ev & Rte_Ev_Run_OdoMdl_OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished) != (EventMaskType)0)
    {
      {
        Rte_CS_ServerQueueInfo_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_Active = 0;

        /* call runnable */
        Rte_Runnable_OdoMdl_OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished_Start(Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.ServiceId, Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */
        OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished(Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.ServiceId, Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.JobResult);
        Rte_Runnable_OdoMdl_OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished_Return(Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.ServiceId, Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.JobResult); /* PRQA S 3112 */ /* MD_Rte_3112 */

        if (Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_TaskID != INVALID_TASK)
        {
          Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_ServerCompleted = TRUE;
          Rte_Task_SetEvent(Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_TaskID, RTE_TASK_WAITPOINT_EVENT_MASK); /* PRQA S 3112 */ /* MD_Rte_3112 */
          (void)SetEvent(Rte_CS_ServerQueue_OdoMdl_ppCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished.Rte_TaskID, RTE_TASK_WAITPOINT_EVENT_MASK); /* PRQA S 3417 */ /* MD_Rte_Os */
        }

        (void)Schedule(); /* PRQA S 3417 */ /* MD_Rte_Os */
      }
    }
    (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_RteEvent);
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 * Task:     OsTask_StackMesure
 * Priority: 1
 * Schedule: FULL
 * Alarm:    Cycle Time 0.1 s Alarm Offset 0 s
 *********************************************************************************************************************/
TASK(OsTask_StackMesure) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_14.1 */
{
  Rte_CheckDetErrorContinue(Rte_InitState == RTE_STATE_INIT, RTE_TASK_SERVICE_ID, RTE_E_DET_UNINIT); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  Rte_Task_Dispatch(OsTask_StackMesure); /* PRQA S 3112 */ /* MD_Rte_3112 */

  /* call runnable */
  Rte_Runnable_CDD_Test_CDD_Test_StackMeasure_Start(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  CDD_Test_StackMeasure();
  Rte_Runnable_CDD_Test_CDD_Test_StackMeasure_Return(); /* PRQA S 3112 */ /* MD_Rte_3112 */
  (void)Xcp_Event(XcpConf_XcpEventChannel_Rte_OsTask_StackMesure);

  (void)TerminateTask(); /* PRQA S 3417 */ /* MD_Rte_Os */
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#define RTE_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 MISRA 2004 violations and justifications
 *********************************************************************************************************************/

/* module specific MISRA deviations:
   MD_Rte_0342:  MISRA rule: 19.13
     Reason:     For efficient code the use of macros and the operator '##' is necessary.
     Risk:       The used compiler is not ANSI/ISO C89 compatible. No functional risk.
     Prevention: AUTOSAR is based on ANSI/ISO C89. Compatible compilers are required.

   MD_Rte_0777:  MISRA rule: 5.1
     Reason:     The defined RTE naming convention may result in identifiers with more than 31 characters. The compliance to this rule is under user's control.
                 This is covered in the MISRA C compliance section of the Rte specification.
     Risk:       Ambiguous identifiers can lead to compiler errors / warnings.
     Prevention: Verified during compile time. If the compiler reports an error or warning, the user has to rename the objects to be unique within the significant characters.

   MD_Rte_0779:  MISRA rule: 5.1
     Reason:     The defined RTE naming convention may result in identifiers with more than 31 characters. The compliance to this rule is under user's control.
                 This is covered in the MISRA C compliance section of the Rte specification.
     Risk:       Ambiguous identifiers can lead to compiler errors / warnings.
     Prevention: Verified during compile time. If the compiler reports an error or warning, the user has to rename the objects to be unique within the significant characters.

   MD_Rte_0781:  MISRA rule: 5.6
     Reason:     The name is being used as a structure/union member as well as being a label, tag or ordinary identifier.
                 The compliance to this rule is under user's control.
     Risk:       Ambiguous identifiers can lead to compiler errors / warnings.
     Prevention: Verified during compile time. If the compiler reports an error or warning, the user has to rename the objects leading to the violation.

   MD_Rte_3112:  MISRA rule: 14.2
     Reason:     The structures of macros are specified in the RTE specification [rte_sws_1236].
     Risk:       No functional risk.
     Prevention: Not required.

   MD_Rte_3408:  MISRA rule: 8.8
     Reason:     For the purpose of monitoring during calibration or debugging it is necessary to use non-static declarations.
                 This is covered in the MISRA C compliance section of the Rte specification.
     Risk:       No functional risk.
     Prevention: Not required.

   MD_Rte_3447:  MISRA rule: 8.8
     Reason:     For the purpose of monitoring during calibration or debugging it is necessary to use non-static declarations.
                 This is covered in the MISRA C compliance section of the Rte specification.
     Risk:       No functional risk.
     Prevention: Not required.

   MD_Rte_Os:
     Reason:     This justification is used as summary justification for all deviations caused by the MICROSAR OS
                 which is for testing of the RTE. Those deviations are no issues in the RTE code.
     Risk:       No functional risk.
     Prevention: Not required.

   MD_Rte_Qac:
     Reason:     This justification is used as summary justification for all deviations caused by wrong analysis tool results.
                 The used analysis tool QAC 7.0 sometimes creates wrong messages. Those deviations are no issues in the RTE code.
     Risk:       No functional risk.
     Prevention: Not required.

*/
