//---------------------------------------------------------------------------------------------------------------------
//
// VISTEON CORPORATION CONFIDENTIAL
// ________________________________
//
// [2016] Visteon Corporation
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
#ifndef RTCDRVCDD_CFG_H
#define RTCDRVCDD_CFG_H

#include "RtcMgrCdd_Cfg.h"
//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
/* Shall match with RCKSEL[1:0] bits in RTC_WTCR register */
#define MAIN_CLOCK  0
#define SUB_CLOCK   1
#define SLOW_RC_OSC 2

//maximum value for RTC_CALTRG_CALTRG register ((2 ^ 12) - 1)
#define MAX_CALTRG_VALUE 		(4095)
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================
#define RTC_CLOCK_SOURCE                    SUB_CLOCK
#define RTC_MAIN_CLOCK_IN_HZ                8000000

#define RTC_MCU_CLOCK_INIT                  STD_ON
#define RTC_MCU_RUN_CONFIG                  McuConf_McuClockSettingConfig_McuClockSettingConfig_RUN_RTC

/* Set the RTC Auto calibration time in Seconds */
#define RTC_AUTO_CALIB_TRGR_TIME_IN_SEC 	(CALIBRATION_PERIOD_IN_MIN * 60)

#if (RTC_AUTO_CALIB_TRGR_TIME_IN_SEC > MAX_CALTRG_VALUE)
#error "Calibration period exceeds the max possible value. Please change your settings."
#endif

#endif

