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

#ifndef RTCMGRCDD_CFG_H
#define RTCMGRCDD_CFG_H

#include "Std_Types.h"

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
//SW compensation enable flag
#define SW_COMPENSATION				(STD_OFF)

//type for SW compensation
#define TEMP_COMPENSATION			((uint8)0)
#define COMPENSATION_COUNT			((uint8)1)
#define SW_COMPENSATION_TYPE		(TEMP_COMPENSATION)

//calibration period of RTC (in minutes)
#define CALIBRATION_PERIOD_IN_MIN	(5)

//limit in 1/4 microseconds
#define cRTC_DEVIATION_LIMIT		((uint32)4000000)

//RTC user of EcuM (needed for management of RUN requests)
#define cECUM_USER_RTC				(EcuMConf_EcuMFixedUserConfig_EcuM_USER_RTC)
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================

/* RTC Supports only SUB_CLOCK. Main and RC Clock will be supported in future! */
#if (CALIBRATION_PERIOD_IN_MIN != 5)
#error "Compensation part of RtcCdd currently supports only calibration on 5 minutes periods."
#endif

#endif

