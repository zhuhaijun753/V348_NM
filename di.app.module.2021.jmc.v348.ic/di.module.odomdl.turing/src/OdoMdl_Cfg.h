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
#ifndef ODOMDL_CFG_H
#define ODOMDL_CFG_H

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define cPROD_SPEC_ODO_STORAGE_FORMAT   cODO_PKG_OFB_EPM

/* Define PROD_SPEC_RESET_STRATEGY if checksum protection for critical variables required. */
/*#define cPROD_SPEC_RESET_STRATEGY*/

/* Define cODO_HALF_STORAGE_UNIT_ERROR_LIMIT_REQUIRED if Half storage unit error limit is required. */
/* If not defined, odoram_whole_counts will be initialised with 0 @ battry connect. */ 
/*#define cODO_HALF_STORAGE_UNIT_ERROR_LIMIT_REQUIRED*/

/*
// define storage units
*/
/* #define cPROD_SPEC_ODO_STORAGE_UNITS cODO_PKG_KM */ 	
 #define cPROD_SPEC_ODO_STORAGE_UNITS    cODO_PKG_KM  


/*
// define how often the storage units are written to NVM
// if one then NVM written every storage unit, if two then
// NVM written every two storage units, etc.
*/
#define cPROD_SPEC_ODO_STORAGE_INCR 1u

/*
 define raw input counts per KM

*/
//#define cPROD_SPEC_COUNTS_PER_KM gl_odo_prod_spec_count_per_km
//#define cPROD_SPEC_COUNTS_PER_KM 43779 /* xgu9 Set it to 42599,according to CX756 requirements */
#define   Vehicle_Tire_245		   44965
#define   Vehicle_Tire_235		   42440

/*
 define ENDIAN as required

*/
/*#define cODO_PKG_BIG_ENDIAN */          /* define this as necessary */
#define cODO_PKG_LITTLE_ENDIAN   /* define this as necessary */


/*
  Define cPROD_SPEC_NVM_ODO_VALUE_CLAMP with a value, if application wants 
  the NVM ODO CLAMP value as a product specific, rather than the one used
  in the package.
  Caution! Please consider the NVM wear out possibility while configuring this macro.
*/
/* #define cPROD_SPEC_NVM_ODO_VALUE_CLAMP           (uint32)(2000000uL) */


/* 
Max number of rolling odo counts to add 
 in one call to OdoAddCounts
*/ 
//#define cODO_JUMP_LIMIT /*Lhbb Not Limit*/
#ifdef cODO_JUMP_LIMIT
#define cMAX_ODO_JUMP_LIMIT (gl_odo_prod_spec_count_per_km / 32)
#endif
 


/* Define cODO_OPTIMIZED_RUNNABLE if execution time for round robin has to be reduced.      */
/* Note: Enabling this will block the traversing through all NVM sets to find out the latest  */
/*       valid NVM set everytime in the round robin. Instead, it will have a global sturcture */
/*       which will hold the last updated NVM set, and it will be considered as the latest    */
/*       valid NVM set. This helps in reducing the Round Robin execution time.                */
/* #define cODO_OPTIMIZED_RUNNABLE */


                                 
/*
// define a macro that allows the odo value to be increased to any value
// NOTE: define this only if the product specifically requires this capability
*/
/*#define cODO_ALLOW_CHANGE*/


 
 /*
// define how many times the odo can be rolled back
// NOTE: define this only if the product requires a roll back function
// if this is not defined than the other roll back configuration items
// will have no effect
*/
/*  #define cPROD_SPEC_RB_MAX_TIMES 	1  */ 	/* number of times roll back can    */
			                                    /* be done and                      */
			                                    /* must be in range 1 ... 255       */
			                                    /* if this is not defined then the  */
			                                    /* cPROD_SPEC_RB_x macros will have  */
			                                    /* have no effect even if defined   */
                                  

/*
// define the odo roll back items if this feature is to be included
*/
/*
**Things to be noticed while configuring the below roll back parameters.
     1. If the storage units is cODO_PKG_KM or cODO_PKG_MILES then the parameters
        has to be configured in resolution 1.(i.e 255 if it is intended to 
        rollback 255 units.
     2. If the storage units is in cTENTH_KM, then values has to be configured
        in 1/10th resolution of storage units.(i.e 2550 if it intended to 
        rollback to 255km.
which means the parameters has to be configured in NVM storage resolution.
*/

 #ifdef cPROD_SPEC_RB_MAX_TIMES 

      #define cPROD_SPEC_RB_MAX_UNITS   2550             /* max total roll back           */
                                                       /* must be in range 1 ... 510    */

      #define cPROD_SPEC_RB_CEILING     2550            /* roll back request above this  */
                                                       /* value will not be honored     */
                                                       /* Shall be a valid odo value in NVM
                                                      storage resolution.
                                                      Note - Shall be greater than zero.*/

      #define cPROD_SPEC_RB_TYPE cODO_PKG_ABSOLUTE
                                                            /* see ODO Programmers Guide     */

      #define cPROD_SPEC_RB_FLOOR       0       /* see ODO Programmers Guide     */

      #define cPROD_SPEC_RB_MAX_PER     2550     /* max roll back amount per each */
                                                   /* request                       */
                                                   /* must be in range 1 ... 255    */
                                               
    #endif
    

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
/// @brief 



#endif  // ODOMDL_CFG_H


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
**Change Description : 1. LITTLE and BIG ENDIAN concepts added. 
**============================================================================
**============================================================================
**Date               : 16-Sep-2016
**CDSID              : nrajaman
**Traceability       : RTC #662890
**Change Description : 1. Removed cOD. 
**============================================================================*/

