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

#ifndef RTCMGRCDD_COMPENSATIONSTRATEGY_H
#define RTCMGRCDD_COMPENSATIONSTRATEGY_H

#include "Std_Types.h"
#include "RtcMgrCdd_TemperatureCompensation.h"
#include "RtcMgrCdd_Cfg.h"

#if (SW_COMPENSATION==STD_ON)
//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
typedef void (*pfA)(void);
typedef Std_ReturnType (*pfB)(void);

typedef struct CompensationStrategy
{
	pfA MainFunction;
	pfB Init;
	pfB DeInit;
	pfB SetTime;
} CompensationStrategy_Type;

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
CompensationStrategy_Type CompensationStartegy[COMPENSATION_COUNT] = {
	(pfA)RtcMgrCdd_TemperatureCompensation_MainFunction,
	(pfB)RtcMgrCdd_TemperatureCompensation_Init,
	(pfB)RtcMgrCdd_TemperatureCompensation_DeInit,
	(pfB)RtcMgrCdd_TemperatureCompensation_SetTime
};
//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================
#define RtcMgrCdd_CompensationStrategy_Init \
		CompensationStartegy[SW_COMPENSATION_TYPE].Init

#define RtcMgrCdd_CompensationStrategy_DeInit \
		CompensationStartegy[SW_COMPENSATION_TYPE].DeInit

#define RtcMgrCdd_CompensationStrategy_SetTime \
		CompensationStartegy[SW_COMPENSATION_TYPE].SetTime

#define RtcMgrCdd_CompensationStrategy_MainFunction \
		CompensationStartegy[SW_COMPENSATION_TYPE].MainFunction
#endif /* SW_COMPENSATION */
#endif /* RTCMGRCDD_COMPENSATIONSTRATEGY_H */
