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
#ifndef ODOMDL_H
#define ODOMDL_H


//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
/// @brief
#define cODO_PKG_OFB_EPM 0
#define cODO_PKG_KM 0
#define cODO_PKG_TENTH_KM 1

#include "OdoMdl_Cfg.h"    /* special include to a few items */

#if (cPROD_SPEC_ODO_STORAGE_UNITS != cODO_PKG_TENTH_KM) 
    #define cCURRENT_ODO_UNITS_SCALING (100) /*Lhbb cCURRENT_ODO_UNITS_SCALING defined is here*/
    #else
    #define cCURRENT_ODO_UNITS_SCALING (10) 
#endif

#if (cPROD_SPEC_ODO_STORAGE_FORMAT == cODO_PKG_OFB_EPM) 
    #define cNUMBER_OF_ODO_SETS  5
    #define cODO_OFFSET (1ul)
    #define cBAD_ODO_VALUE_W ((uint16)0xFFFFu)
	#if (cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_TENTH_KM)
    
	  #define cTOTAL_ODO_INCRS ((uint8) 250)/*This can go upto 65535*250+250 = 16,384,00.0 KM*/

	#else

	  #define cTOTAL_ODO_INCRS ((uint8) 128)

	#endif
#endif

/**Odo high changeup resolution return value**/

#define  cODO_CHANGEUP_NOT_HONOURED		0X00 		 /***FALSE**/
#define  cODO_CHANGEUP_ACCEPTED			0X01 		 /***TRUE**/
#define  cODO_CHANGEUP_RETRY			0X02         /**RETRY***/

/**Math Library Inputs**/
#define  cCONVERTION_RESOLUTION		0 // Resolution will be set as 1

#define OdoMathConvertDistance(DistanceInMeter,ConvertionUnit,Resolution,RoundOffMethod,ConvertedMiles)		MathLib_ConvertDistance(DistanceInMeter,ConvertionUnit,Resolution,RoundOffMethod,ConvertedMiles)


uint32 gl_odo_prod_spec_count_per_km;
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================


#endif // ODOMDL_H


/*============================================================================
**
**============================================================================
** C M S    R E V I S I O N    N O T E S
**============================================================================
**
** For each change to this file, be sure to record:
** 1.  Who made the change and when the change was made.
** 2.  Why the change was made and the intended result.
**
** CMS Rev #        Date         By
** CMS Rev X.X      mm/dd/yy     CDSID
**
**============================================================================
**============================================================================
**Date               : 2-Sep-2016
**CDSID              : nrajaman
**Traceability       : RTC #662890
**Change Description : Initial Framework version of OdoMdl
**============================================================================
**============================================================================
**Date               : 15-Sep-2016
**CDSID              : nrajaman
**Traceability       : RTC #662890
**Change Description : 1. Incr config added for cODO_PKG_TENTH_KM. 
**============================================================================*/


