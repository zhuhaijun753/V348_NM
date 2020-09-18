/*============================================================================
//
//                     CONFIDENTIAL VISTEON CORPORATION
//
// This is an unpublished work of authorship, which contains trade secrets,
// created in 2000. Visteon Corporation owns all rights to this work and
// intends to maintain it in confidence to preserve its trade secret status.
// Visteon Corporation reserves the right, under the copyright laws of the
// United States or those of any other country that may have  jurisdiction,
// to protect this work as an unpublished work, in the event of an
// inadvertent or deliberate unauthorized publication. Visteon Corporation
// also reserves its rights under all copyright laws to protect this work as
// a published work, when appropriate. Those having access to this work may
// not copy it, use it, modify it or disclose the information contained in
// it without the written authorization of Visteon Corporation.
//
//============================================================================
//
//  Name:               fuel03_nvm.c
//
//  Description:        NVM file for fuel03.c
//
//  Organization:       Driver Information Software Section,
//                      DI Core Engineering Department
//
//==========================================================================*/
#define FUEL03_NVM_C


/*============================================================================
//                 I N C L U D E   F I L E S
//==========================================================================*/
#include "system.h"
#include "fuel03.h"
#include "fuel03_nvm.h"

/*============================================================================
// I N T E R N A L   M A C R O   A N D   T Y P E   D E F I N I T I O N S
//==========================================================================*/

/*============================================================================
//                M E M O R Y   A L L O C A T I O N
//==========================================================================*/

/* --------------------------------------------------------------------------
** NVM allocation.  More details on the meaning of each constant defined
** here can be found in the sample FUEL02.CFG file.
** --------------------------------------------------------------------------
*/
#if 0
#ifdef CWR_SPECTRUM_TYPE
  #pragma push
  #pragma section const_type "FUEL03_CONSTANT_SEGMENT"
#else
  #if defined (__arm)
    #pragma arm section rodata="FUEL03_CONSTANT_SEGMENT"
  #else
    #ifdef __COSMIC_SYSTEMS_ICC
       #pragma section const {FUEL03_CONSEG}
    #else
      #ifdef FUEL03_DEBUG
         #pragma memory=constseg(FUEL03_NVM_SEGMENT)
      #else
         #pragma memory=constseg(FUEL03_CONSTANT_SEGMENT)
      #endif
    #endif
  #endif
#endif
#endif

/* --------------------------------------------------------------------------
** Define the fuel filter base delta and adjustment factor for use when
** in "normal" filter mode.
** --------------------------------------------------------------------------
*/


FUEL03_CONST uint32 FUEL03_DELTA_NORMAL_BASE_A_NVM   = FUEL03_DELTA_NORMAL_BASE_A1;
FUEL03_CONST uint32 FUEL03_DELTA_NORMAL_BASE_B_NVM   = FUEL03_DELTA_NORMAL_BASE_B1;


FUEL03_CONST uint16 FUEL03_DELTA_NORMAL_ADJUST_A_NVM = FUEL03_DELTA_NORMAL_ADJUST_A;
FUEL03_CONST uint32 FUEL03_DELTA_NORMAL_CLAMP_A_NVM  = FUEL03_DELTA_NORMAL_CLAMP_A;

/* --------------------------------------------------------------------------
** Define the fuel filter base delta and ajustment factor for use when
** in "fast fill" filter mode.
** --------------------------------------------------------------------------
*/
FUEL03_CONST uint16 FUEL03_DELTA_FAST_ADJUST_A_NVM = FUEL03_DELTA_FAST_ADJUST_A;
FUEL03_CONST uint32 FUEL03_DELTA_FAST_BASE_A_NVM   = FUEL03_DELTA_FAST_BASE_A;
FUEL03_CONST uint32 FUEL03_DELTA_FAST_CLAMP_A_NVM  = FUEL03_DELTA_FAST_CLAMP_A;

FUEL03_CONST uint8 FUEL03_KO_DELTA_A_NVM = FUEL03_KO_DELTA_A;



/* --------------------------------------------------------------------------
** Define the up and down fill checks used during fuel init.
** --------------------------------------------------------------------------
*/
FUEL03_CONST uint8 Fuel03PositiveInitFill_A_NVM = FUEL03_POSITIVE_INIT_FILL_DELTA_A;
FUEL03_CONST uint8 Fuel03NegativeInitFill_A_NVM = FUEL03_NEGATIVE_INIT_FILL_DELTA_A;

/* --------------------------------------------------------------------------
** Define the up and down fill checks used as one of the entrance conditions
** to "fast fill" mode.
** --------------------------------------------------------------------------
*/

FUEL03_CONST uint8 Fuel03PositiveFastFill_A_NVM = FUEL03_POSITIVE_FAST_FILL_DELTA_A;
FUEL03_CONST uint8 Fuel03NegativeFastFill_A_NVM = FUEL03_NEGATIVE_FAST_FILL_DELTA_A;

/* --------------------------------------------------------------------------
** Define the 'secondary' up and down fill checks
** --------------------------------------------------------------------------
*/
FUEL03_CONST uint8 Fuel03SecPosInitFillDeltaNVM_A = FUEL03_SEC_POS_INIT_FILL_DELTA_A;
FUEL03_CONST uint8 Fuel03SecNegInitFillDeltaNVM_A = FUEL03_SEC_NEG_INIT_FILL_DELTA_A;



/* --------------------------------------------------------------------------
** Define the up and down check limits used for percent full hysteresis.
** --------------------------------------------------------------------------
*/
FUEL03_CONST uint16 Fuel03PercentFullPosHystNVM = FUEL03_PERCENT_FULL_POSITIVE_HYST;
FUEL03_CONST uint16 Fuel03PercentFullNegHystNVM = FUEL03_PERCENT_FULL_NEGATIVE_HYST;

/* --------------------------------------------------------------------------
** Define the threshold at which to check for rental car topoff.
** --------------------------------------------------------------------------
*/
FUEL03_CONST uint8 Fuel03SenderA_RCFillThresholdNVM = FUEL03_SENDERA_RC_FILL_THRESHOLD;


/* --------------------------------------------------------------------------
** Define the fill check to use for rental car topoff.
** --------------------------------------------------------------------------
*/

FUEL03_CONST uint8 Fuel03SenderA_RCFillDeltaNVM = FUEL03_SENDERA_RC_FILL_DELTA;

FUEL03_CONST uint8 Fuel03ZssDelta_A_NVM = FUEL03_ZS_DELTA_A;

FUEL03_CONST uint16 Fuel03A2DShortLimNVM = FUEL03_SHORTED_SENDER_LIMIT;
FUEL03_CONST uint16 Fuel03A2DOpenLimNVM = FUEL03_OPEN_SENDER_LIMIT;


/* --------------------------------------------------------------------------
** Flags to enable/disable tank B, dual sender, zss ref only, hill hold,
** and fast fill.
** --------------------------------------------------------------------------
*/
#ifdef FUEL03_ZSS_REF_ONLY
  #define FUEL03_TEMPZ FUEL03_ZSS_REF_ONLY_BIT
#else
  #define FUEL03_TEMPZ 0
#endif

#ifdef FUEL03_HILL_HOLD_ENABLE
  #define FUEL03_TEMP1 FUEL03_HILL_HOLD_ENABLE_BIT
#else
  #define FUEL03_TEMP1 0
#endif

#ifdef FUEL03_FAST_FILL_ENABLE
  #define FUEL03_TEMP2 FUEL03_FAST_FILL_ENABLE_BIT
#else
  #define FUEL03_TEMP2  0
#endif

FUEL03_CONST uint8 Fuel03OptionsNVM = (FUEL03_TEMP1 | FUEL03_TEMP2 | FUEL03_TEMPZ );
/* --------------------------------------------------------------------------
** Define times for power stabilization delay, fuel samples to be taken,
** and open/short detect time, number of good samples required and
** Fast Fill debounce counter.
** --------------------------------------------------------------------------
*/

FUEL03_CONST uint16 Fuel03Sender_A_TableNVM[FUEL03_SENDER_RAW_TO_PF_TBL_SIZE][2] =
{
//	FUEL03_SENDER_RAW_TO_PF_TBL	
	{(uint16)(0),	  (uint16)(254*256)}, 					
	{(uint16)(0),	  (uint16)(0*256u)}, 			
	{(uint16)(600),   (uint16)(19*256u)},			
	{(uint16)(1000),  (uint16)(32*256u)},			
	{(uint16)(1500),  (uint16)(48*256u)},			
	{(uint16)(2300), (uint16)(74*256u)},				
	{(uint16)(3200), (uint16)(103*256u)},				
	{(uint16)(4100), (uint16)(132*256u)},				
	{(uint16)(5100), (uint16)(163*256u)},				
	{(uint16)(6000), (uint16)(193*256u)},							
	{(uint16)(6600), (uint16)(212*256u)},			
	{(uint16)(7100), (uint16)(228*256u)},			
	{(uint16)(7900), (uint16)(254*256u)},			
};


#if 0
FUEL03_CONST uint16 Fuel03Sender_A_TableNVM[FUEL03_SENDER_RAW_TO_PF_TBL_SIZE][2] =
{
	//FUEL03_SENDER_RAW_TO_PF_TBL	
	{(uint16)(380u),	  (uint16)(4720u)}, 	\
	{(uint16)(380u),	(uint16)(0u*256u)}, 	\
	{(uint16)(920u),   (uint16)(31u*256u)}, 	\
	{(uint16)(1410u),  (uint16)(60u*256u)}, 	\
	{(uint16)(1940u),  (uint16)(91u*256u)}, 	\
	{(uint16)(2530u), (uint16)(125u*256u)}, 	\
	{(uint16)(2910u), (uint16)(148u*256u)}, 	\
	{(uint16)(3890u), (uint16)(205u*256u)}, 	\
	{(uint16)(4280u), (uint16)(228u*256u)}, 	\
	{(uint16)(4720u), (uint16)(254u*256u)}, 	\
};
#endif

FUEL03_CONST uint8 Fuel03FastFillTimeCounterNVM   = FUEL03_MIN_FAST_FILL_TIME;		//20s

FUEL03_CONST uint8  Fuel03KeyOnDelayCounterNVM    = FUEL03_KEY_ON_DELAY_TIME;		//100ms
FUEL03_CONST uint8  Fuel03InitDelayCounterNVM     = FUEL03_INIT_DELAY_COUNT_TIME;	//3//3s

/* --------------------------------------------------------------------------
** Counter value used to determine when to take the zero speed sample
** used for gage init and fast fill.
** --------------------------------------------------------------------------
*/
FUEL03_CONST uint16 Fuel03ZeroSpeedCounterNVM = FUEL03_ZERO_SPEED_COUNTER;
/*
 ** Define the maximum allowed key-off sampling time.
*/
FUEL03_CONST uint16 Fuel03KeyOffSampleTimeNVM = FUEL03_KEY_OFF_SAMPLE_TIME;

#if( defined( FUEL03_AD_TO_PF_TBL_IN_NVM)||(FUEL03_LOG_FAULT_RECORD_SIZE > 0))
/* If AD_TO_PF_TBL is required to be put in NVM, then allocate NVM segment here */
  #ifdef CWR_SPECTRUM_TYPE
    #pragma section data_type "FUEL03_NVM_SEGMENT" "FUEL03_NVM_SEGMENT"
  #else
    #if defined (__arm)
      #pragma arm section rodata="FUEL03_NVM_SEGMENT"
    #else
      #ifdef __COSMIC_SYSTEMS_ICC
        #pragma section const {FUEL03_NVMSEG}
      #else
        #pragma memory=constseg(FUEL03_NVM_SEGMENT)
      #endif
    #endif
  #endif
  #ifdef FUEL03_AD_TO_PF_TBL_IN_NVM
    #ifdef CWR_SPECTRUM_TYPE
      #if FUEL03_NUM_FUEL_TABLES==1
        FUEL03_NVM uint16 Fuel03Sender_A_TableNVM[FUEL03_SENDER_A_AD_TO_PF_TBL_SIZE][2] =
         {
        		 FUEL03_SENDER_RAW_TO_PF_TBL
         };
      #else
        FUEL03_NVM uint16 Fuel03Sender_A_TableNVM[FUEL03_NUM_FUEL_TABLES][FUEL03_SENDER_A_AD_TO_PF_TBL_SIZE][2] =
         {
        		 FUEL03_SENDER_RAW_TO_PF_TBL
         };
      #endif
    #else
      #if FUEL03_NUM_FUEL_TABLES==1
        FUEL03_CONST uint16 Fuel03Sender_A_TableNVM[FUEL03_SENDER_A_AD_TO_PF_TBL_SIZE][2] =
         {
        		 FUEL03_SENDER_RAW_TO_PF_TBL
         };
      #else
        FUEL03_CONST uint16 Fuel03Sender_A_TableNVM[FUEL03_NUM_FUEL_TABLES][FUEL03_SENDER_A_AD_TO_PF_TBL_SIZE][2] =
         {
        		 FUEL03_SENDER_RAW_TO_PF_TBL
         };
     #endif
    #endif
  #endif

  #if FUEL03_LOG_FAULT_RECORD_SIZE > 0
/* ----------------------------------------------------------------------------------
** The following configuration items allow the application to decide which fuel
** values to save.
** Set to 1 to enable storage, 0 to disable storage.
** Create a storage location that the fault log will be written to. This is
** initialized to an invalid number (0xff).
***/
    #ifdef CWR_SPECTRUM_TYPE
      FUEL03_NVM uint8 fuel03_next_fault_log_to_write_NVM = 0xff;
      FUEL03_NVM FUEL03_FAULT_SNAPSHOT_STRUCT fuel03_fault_snapshot_A_NVM[FUEL03_LOG_FAULT_RECORD_SIZE];
    #else
      FUEL03_CONST uint8 fuel03_next_fault_log_to_write_NVM = 0xff;
      FUEL03_CONST FUEL03_FAULT_SNAPSHOT_STRUCT fuel03_fault_snapshot_A_NVM[FUEL03_LOG_FAULT_RECORD_SIZE];
    #endif
  #endif
#endif

#if 0
#ifdef CWR_SPECTRUM_TYPE
  #pragma pop
#else
  #if defined (__arm)
    #pragma arm section
  #else
     #ifdef __COSMIC_SYSTEMS_ICC
       #pragma section const {}
     #else
       #pragma memory=default
     #endif
  #endif
#endif
#endif

/*============================================================================
//
//  $Revision:   1.16  $
//
//============================================================================
//  P V C S    R E V I S I O N    N O T E S
//----------------------------------------------------------------------------
//  For each change to this file, be sure to record:
//  1.  who made the change and when the change was made
//  2.  why the change was made and the intended result
//
//  Date         By      Reason for Change
//  mmm/dd/yy    XXX     -----------------
//
//----------------------------------------------------------------------------
// 07/08/12       BKORAGAN
// Multirate fuel filter implemented as per BSDI00117140.

// 06/15/11       KNAGASUB
// BSDI00103442-MIN_FAST_FILL_TIME parameter is moved to tank dependent constant.

// 06/23/09       KNAGASUB
// changes done per BSDI00071875(Add #pragmas for Kepler platform to FUEL02 and FUEL03).

// 09/10/08       KNAGASUB
// changes done in fault record macro's as per review comments.

// 01/10/08       KNAGASUB
// The changes are done as per the CQ BSDI00061422.
// code modified as per review comments
//To deactive the fault record size's set the macro(FUEL03_RAM_LOG_FAULT_RECORD_SIZE,FUEL03_LOG_FAULT_RECORD_SIZE) to zero.

// 25/09/08       KNAGASUB
// The changes are done as per the CQ BSDI00061422.
// The macro is removed FUEL03_LOG_INIT_ERRORS_TO_NVM.
// NVM fault record array is turned  off separately by these macro's
//  FUEL03_RAM_LOG_FAULT_RECORD_SIZE,FUEL03_LOG_FAULT_RECORD_SIZE.

// 24/09/08       KNAGASUB
// The macro FUEL03_NUM_FUEL_TABLES which is used to remove the unused table NVM bytes allocation.

// 09/08/08       KNAGASUB
// Fuel03A2DOpenLimNVM and Fuel03A2DShortLimNVM
// comapatible to more than one tank also.

// 06/08/08      KNAGASUB
// "Const" used directly instead of defining in the FUELO2_CONST macro.

// 6/05/08       CNAGARA1
// Removed a space at the end of the following line. This was giving an error
// while comparing in clearcase.
// '#pragma section const {FUEL03_NVMSEG}'
// This version is equal to FUEL03_V02.03 with above change.
//
// 4/11/08                  CNAGARA1            BSDI00054834
// Moved AD to PF TBL to a separate segment based on the conditinal compilation.
// If FUEL02_AD_TO_PF_TBL_IN_NVM macro is not defined then
// FUEL02_SENDER_A_AD_TO_PF_TBL is put in const segment otherwise this will be
// in NVM segment.
//
//
// 3/30/07       JARUN   BSDI00044140
//
// 02/28/06     JARUN   BSDI00038410
//   Added support for comsic compiler
//   The constant segment name NVM_VARIABLE
//   segment name was changed to FUEL03_CONSTANT_SEGMENT.
//   Commented out fuel03_tank_array[].
//
// 5/24/06       RRUSS1  BSDI00031391
//
// Updated to work with auto generated config files.
//
// 6/6/05        RRUSS1  BSDI00020719
//
// Initial creation.
//==========================================================================*/

/* end of file ============================================================ */
