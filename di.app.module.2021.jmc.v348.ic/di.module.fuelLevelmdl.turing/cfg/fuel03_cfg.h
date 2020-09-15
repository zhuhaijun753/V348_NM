/*============================================================================
**
**                     CONFIDENTIAL VISTEON CORPORATION
**
** This is an unpublished work of authorship, which contains trade secrets,
** created in 2003. Visteon Corporation owns all rights to this work and
** intends to maintain it in confidence to preserve its trade secret status.
** Visteon Corporation reserves the right, under the copyright laws of the
** United States or those of any other country that may have  jurisdiction,
** to protect this work as an unpublished work, in the event of an
** inadvertent or deliberate unauthorized publication. Visteon Corporation
** also reserves its rights under all copyright laws to protect this work as
** a published work, when appropriate. Those having access to this work may
** not copy it, use it, modify it or disclose the information contained in
** it without the written authorization of Visteon Corporation.
**
**============================================================================
**
**  Name:               fuel03.cf$
**
**  Description:        This file contains a description of the macros that
**                      are used to configure the fuel filter algorithm (FUEL0).
**
**  Organization:       Driver Information Software Section,
**                      DI Core Engineering Department
**
**==========================================================================*/

/*============================================================================
**                         O V E R V I E W
**==========================================================================*/
/*
** The intent of this code is to provide a configurable fuel filter
** algorithm for use in a variety of instrument cluster applications.
**
** Requirements for this code package are detailed in the
** Fuel Algorithm Bookshelf Design Specification (BDS) 4FA1-1A306-CA.
**
**
** Many of the constants defined below are calculated based on the
** expected iteration rate of the filter(s).  On hardwired systems, this
** value is a fixed interval defined by the application, and will normally
** be in the 100ms range.  On network systems, this value is based on
** the rate at which messages are received, and will normally be 133ms
** for SCP based systems.
**
** Also, many of the constants below are defined in SCP percent full units,
** which is defined as 1 byte, 0-255.  Zero represents a 0% full tank, and
** 254 represents a 99.60% (254/255) percent full tank.  The value 255 is
** reserved to indicate a fault.  When a constant defined in terms of
** SCP percent full units is 2 bytes or 4 bytes, the most significant byte
** (unless otherwise noted) represents the SCP percent full unit.  The
** other bytes represent a remainder portion of the data.
**==========================================================================*/

/*============================================================================
**                         I N C L U D E   F I L E S
**==========================================================================*/

/*-------------------------------------------------------------------------*/
/* Define the following if Fuel03 Auto Configuration Generated file
   fuel03_bds.cfg is included for the project.  */
/*-------------------------------------------------------------------------*/
#define FUEL03_BDS_CFG 

/*-------------------------------------------------------------------------*/
#ifdef  FUEL03_BDS_CFG
#include "fuel03_bds_cfg.h"
#else

/*============================================================================
**
**               T A N K   I N D E P E N D E N T   C O N S T A N T S
**
**============================================================================
** The following constants are defined by the Fuel BDS tool.
** These will be avaialble in "fuel03_bds.cfg" auto generated from BDS.
** The constatns available in BDS are :-
** Fuel algorithm configuration parameters
** Fuel initialization configuration parameters
** Fuel open/short configuration parameters
** Fuel fast fill configuration parameters
** Fuel slosh filter configuration parameters
** Reference sample (ZSS, KO) convergence / sampling configuration parameters
** NVM fault logging configuration parameters
**==========================================================================*/
#endif

//#include "ffuel.h"
#include "FuelLevelMdl_cfg.h"
/*
** --------------------------------------------------------------------------
** Non-BDS defined configuration parameters
** --------------------------------------------------------------------------
*/

#define  FUEL03_NUM_FUEL_TABLES  1


/* --------------------------------------------------------------------------
** Define a function that returns a UINT8 value that represents the fuel
** tank configuration information that comes from the B&A SCEM byte.  This
** value must be in the format specified by the FUEL BDS, because this is
** the input value that is used in the input column of the
** "FUEL03_INDEX_TABLE" that is auto-generated by the FUEL BDS workbook.
**
** The definition of this macro will be determined by the software engineer,
** and will typically be a function from the source code file FSCEM.C.
** --------------------------------------------------------------------------
*/
/*#define FUEL03_GET_FUEL_SCEM() (f_scem_get_fuel_scem_byte())*/

/* --------------------------------------------------------------------------
** Populate the following with a function that returns 1 when the fast fill
** conditions have been met, 0 otherwise.  Set to 1 if not used.
**
** Typically, this will be a check for zero speed and/or the transmission
** in PARK.  Check your product specific CRS/CDS for requirements for
** this item.
**
** The definition of this macro will be determined by the software engineer,
** and will typically be a function from the source code file FFUEL.C.
** --------------------------------------------------------------------------
*/
#define FUEL03_FAST_FILL_MET_MACRO()   (ffuel_FastFill_Entry_Conditions())


/*
** Populate the following with a function that returns TRUE (1) when the 
** speed threshold is exceeded, FALSE (0) otherwise.
**
** Typically, this will be a check for zero speed. Check your product specific
** CRS/CDS for requirements for this item.
**
** The definition of this macro will be determined by the software engineer,
** and will typically be a function from the source code file FFUEL.C.
*/
#define FUEL03_KO_SPD_THRESHOLD()	(!ffuel_FastFill_Entry_Conditions())

/* --------------------------------------------------------------------------
** Populate the following with a function that returns TRUE when the
** 'Power Mode = Start' condition exists, i.e. ignition switch is in
** CRANK.  Set to FALSE if not used.  If this macro returns TRUE, the
** fuel filter algorithm will not be iterated after fuel init completes.
**
** Check your product specific CRS/CDS for requirements for this item.
**
** The definition of this macro will be determined by the software engineer,
** and will typically be a function from the source code file FIGSW.C.
** --------------------------------------------------------------------------
*/
#define FUEL03_POWER_MODE_MACRO() (FALSE)

/* --------------------------------------------------------------------------
** Populate the following with a function that returns 1 when
** "valid conditions" exist for executing the fuel algorithm.
** Set to 1 if not used.
**
** Typical invalid conditions are voltage out of range, or missing
** network messages for systems that provide the fuel A/D via the
** vehicle network.
**
** The definition of this macro will be determined by the product
** specific CRS, and will typically be a function from the source
** code file FFUEL.C.
** --------------------------------------------------------------------------
*/
#define FUEL03_VALID_CONDITION()  ffuel_valid_condition()

/* --------------------------------------------------------------------------
** Define the following with a function that returns TRUE when a low
** voltage condition exists. Note that this is only used by the
** "FUEL03_keyoff processing_done()" function to allow sleep to occur.
** If you don't need this, then set it to FALSE.
**
** The definition of this macro will be determined by the software engineer,
** and will typically be a function from the source code file IDD01.C.
**
** NOTE:  This should always be set to FALSE unless your product specific
**        CRS indicates otherwise.
** --------------------------------------------------------------------------
*/
#define FUEL03_LOW_VOLTAGE() (FALSE)


#if (FUEL03_MULT_FILT_RATES == 1)
/* --------------------------------------------------------------------------
** The below Macros should be populated if slosh filter time constants
** should be based upon vehicle speed
** --------------------------------------------------------------------------
*/

/* --------------------------------------------------------------------------
** Populate the following with a function that returns 1 when
** Speed Msg is missing.
** Set to 0 if Speed Input is not a CAN signal.
*/
#define  FUEL03_SPEED_MSG_MISSING()      (FALSE)

/* --------------------------------------------------------------------------
** Populate the following with a function that returns 1 when
** Speed Msg is Invalid and Unknown.
** Set to 0 if Speed Input is not a CAN signal.
*/
#define  FUEL03_SPEED_MSG_INVALID()     (FALSE)

#define  FUEL03_SPEED_MSG_UNKNOWN()     (FALSE)

/* --------------------------------------------------------------------------
** Populate the following with a function that returns the Vehicle Speed.
** If Speed input is not a CAN Signal then populate with an
** interface that returns vehicle Speed.
** Populate the following with a macro that returns the 16-bit
** vehicle speed value in enginering units. This must be the same
** units as the input side of GA_SPEED_COUNTS_LOOKUP_TBL.
*/
#define  FUEL03_SPEED_MSG_RXD()         (FALSE)
#endif



/* --------------------------------------------------------------------------
** Define the following macros if you require these PIDs in your system.
** Whether or not to enable these PIDs can be determined by reviewing
** the PID section of the product specific Part II diagnostic spec.
**
** IMPORTANT:  Don't define the PIDs associated with the second sender
**             if you only have a single sender system!  If you do,
**             the code will not compile.
**
** C903: fuel sender 1 -- number of good samples taken at init
** C904: fuel sender x -- number of good samples required (NVM)
** C909: fuel sender 1 -- average of all good init samples
** C90A: fuel init ref -- reference used at init and value
** 6185: Fuel Level Analog Input Status #1
**
**
** NOTE:  If FUEL03_LOG_INIT_ERRORS_TO_NVM is enabled, the following
**        PIDs will automatically be supported and must be added to
**        your diagnostic config file:
**
** PID C90E: 4 byte pointer to RAM buffer containing fuel init information
**           (will only need 2 bytes for Mako micros, but will need more for
**           other micros)
** PID C90F: 2 byte size of RAM buffer in PID C90E
** PID C910: 4 byte pointer to NVM buffer containing fuel init information
**           (will only need 2 bytes for Mako micros, but will need more for
**           other micros)
** PID C911: 2 byte size of NVM buffer in PID C910

** PID C912: Routine to return 2 byte size of RAM buffer
**           It will return  fuel03_RAM_fault_record value

** PID C913: 2 byte size of NVM buffer in PID C913
**           Next fault log that is going to be written into NVM

** PID C914: 2 byte size of NVM buffer in PID C914
**           It will return the number of fault records that are configured for
**           storing in RAM

** PID C915: 2 byte size of NVM buffer in PID C915
**           the number of fault records that are configured for
**           storing in NVM
** --------------------------------------------------------------------------
*/
/*#define FUEL03_NEED_PID_C903
#define FUEL03_NEED_PID_C904
#define FUEL03_NEED_PID_C909
#define FUEL03_NEED_PID_C90A
#define FUEL03_NEED_PID_6185
#define FUEL03_NEED_PID_C90E
#define FUEL03_NEED_PID_C90F
#define FUEL03_NEED_PID_C910
#define FUEL03_NEED_PID_C911
#define FUEL03_NEED_PID_C912
#define FUEL03_NEED_PID_C913
#define FUEL03_NEED_PID_C914
#define FUEL03_NEED_PID_C915
#define FUEL03_NEED_PID_C90A
*/
/*--------------------------------------------------------------------------
** Define the following if you would like the DTC logging to be active in
** the key on periodic processing.  Comment it out if you want to log
** DTCs yourself (not recommended).
**
** Whether or not you require DTCs to be logged can be determined by
** reviewing the DTC section of the product specific Part II diagnostic
** spec.
** --------------------------------------------------------------------------
*/
/*#define FUEL03_NEED_DTC_LOGGING*/

/* --------------------------------------------------------------------------
** Define the following if you would like to control when the DTC logging
** is active.  Set to TRUE if it's always valid.  This can be used to
** disable DTC logging in CRANK or other modes.
**
** When to log DTCs can be found by reviewing the product specific CRS/CDS
** and/or the DTC section of the product specific Part II diagnostic spec.
** --------------------------------------------------------------------------
*/
#define FUEL03_DTC_LOGGING_ACTIVE() (FALSE)

/* --------------------------------------------------------------------------
** If you defined FUEL03_NEED_DTC_LOGGING or FUEL03_NEED_DIAG_SELF_TEST,
** then populate the following with your product specific functions for
** logging sender open or short DTCs.
**
** NOTE:  The CLR macro should always be set to the "KernelErrorNotPresent"
**        function.
** --------------------------------------------------------------------------
*/
#define FUEL03_LOG_FUEL_SENDER_CKT_OPEN()   /* KernelLogError(ERC_FUEL_SENDER_CIRCUIT_OPEN, 0)*/
#define FUEL03_CLR_FUEL_SENDER_CKT_OPEN()   /* KernelErrorNotPresent(ERC_FUEL_SENDER_CIRCUIT_OPEN)*/

#define FUEL03_LOG_FUEL_SENDER_CKT_SHORT()  /* KernelLogError(ERC_FUEL_SENDER_CIRCUIT_SHORT, 0)*/
#define FUEL03_CLR_FUEL_SENDER_CKT_SHORT()   /*KernelErrorNotPresent(ERC_FUEL_SENDER_CIRCUIT_SHORT)*/

/* --------------------------------------------------------------------------
** Define the following if you require a diagnostic self test function
** in your system.  If so, you must define the "GET..." macros that will
** return the current raw fuel A/D for Sender A and/or B.  The "GET..."
** macro for sender 2 is not needed in single sender systems.
**
** If FUEL03_NEED_DIAG_EPILOG is defined, the FUEL03_KSDiagStartSelfTest()
** function is included when fuel03.c is compiled and it is the
** responsibility of the application to call it by adding it to
** the appropriate diagnostic function list.
** --------------------------------------------------------------------------
*/
/*#define FUEL03_NEED_DIAG_SELF_TEST		*/
/*#define FUEL03_GET_LEVEL_SENDER_A() (f_fuel_get_sender_a_ad()) */

/* --------------------------------------------------------------------------
** Define the following if you require a diagnostic epilog function in your
** system.  This function will restart fuel init when exiting diagnostic mode.
**
** If FUEL03_NEED_DIAG_EPILOG is defined, the FUEL03_KSDiagEpilog() function
** is included when FUEL03.c is compiled and it is the responsibility of the
** application to call it by adding it to the appropriate diagnostic function
** list.
** --------------------------------------------------------------------------
*/
/*#define FUEL03_NEED_DIAG_EPILOG*/

/* --------------------------------------------------------------------------
** Define the following if you require a slosh filter hysteris functionality in your
** system.
*/
#define FUEL03_SLOSH_HYS
/* --------------------------------------------------------------------------
** Define the following if you require a Error count in your
*/

/*#define FUEL03_ERR_CNT*/
/* --------------------------------------------------------------------------
** Define the following if you require a RC fill mode feature in your system
*/

#define FUEL03_RC_FILL_ENABLED
/* --------------------------------------------------------------------------
** This macro enables the FUEL03_IGN_STATUS countsin your system
*/

/*#define FUEL03_IGN_STATUS*/
/* --------------------------------------------------------------------------
** This macro enables the key off processing function in your system
*/
#define FUEL03_KEY_OFF_ENABLED

/* --------------------------------------------------------------------------
** This macro enables the EOL live mode feature in your system
*/
#define FUEL03_EOL_LIVE

/* --------------------------------------------------------------------------
** This macro enables the Network Ato D  in your system
*/
/*#define FUEL03_AD_NETWORK*/
/* --------------------------------------------------------------------------
** Define this macro,if you need fuel03_get_SenderA_unfiltered_flps() in your application
*/
#define FUEL03_GET_UNFILTER_FLPS
/* --------------------------------------------------------------------------
** Define this macro,if you need Fuel03_SenderA_keyon_avgflps() in your application
*/

#define FUEL03_GET_KEY_ON_AVGFLPS
 /*--------------------------------------------------------------------------
** Define this macro,if you need Fuel03_SenderA_KeyOffRef() in your application
*/

#define FUEL03_GET_KEYOFFREF
 /*--------------------------------------------------------------------------
** Define this macro,if you need Fuel03_SenderA_zss_ref() in your application
*/
#define  FUEL03_GET_KEY_ZSSFREF
  /*--------------------------------------------------------------------------
** Define this macro,if you need Fuel03_SenderA_keyon_good_samples() in your application
*/
/*#define FUEL03_GET_KEY_ON_GOOD_SAMPLE  */
  /*--------------------------------------------------------------------------
** Define this macro,if you need Fuel03_Get_CANFLPS() in your application
*/
/*#define FUEL03_CANFLPS*/
/* --------------------------------------------------------------------------
** Define this macro,if you need fuel03_clear_DTC_resets() in your application
*/

/*#define FUEL03_CLR_DTC_RESET  */






/* ============================================================================
**
**  $Revision:   1.8  $
**
** ============================================================================
**  P V C S    R E V I S I O N    N O T E S
** ----------------------------------------------------------------------------
**  For each change to this file, be sure to record:
**  1.  who made the change and when the change was made
**  2.  why the change was made and the intended result
**
**  Date         By      Reason for Change
**  mmm/dd/yy    XXX     -----------------
**
** ----------------------------------------------------------------------------
**  05/12/10     JARUN     Added support for slosh filter time constants based upon vehicle speed.
**                         CQ:BSDI00086712
**                         Also removed all fuel constants which are now available in fuel03_bds.cfg
**
**  02/04/10     KNAGASUB  FUEL03_HILL_HOLD_ENABLE Macro definition are repeated two times.
**                         One defeinition removed as per CQ BSDI00073307

**  10/12/09     KNAGASUB  Macro definition are added as per CQ BSDI00073307
**                         Need memory optimized Fuel package for Newton1.5
**                         a)FUEL03_SLOSH_HYS
**                         b)FUEL03_ERR_CNT
**                         c)FUEL03_RC_FILL_ENABLED
**                         d)FUEL03_IGN_STATUS
**                         e)FUEL03_KEY_OFF_ENABLED
**                         f)FUEL03_HILL_HOLD_ENABLE
**                         g)FUEL03_EOL_LIVE
**                         h)FUEL03_AD_NETWORK
**                         i)FUEL03_GET_UNFILTER_FLPS
**                         J)FUEL03_GET_KEY_ON_AVGFLPS
**                         k)FUEL03_GET_KEYOFFREF
**                         l)FUEL03_GET_KEY_ZSSFREF
**                         m)FUEL03_GET_KEY_ON_GOOD_SAMPLE
**                         n)FUEL03_CANFLPS
**                         o)FUEL03_CLR_DTC_RESET


**  3/30/07     JARUN  BSDI00044140 Added a new config item
**                       for Fast fill change.
**
**  03/01/2007   JARUN   Added compiler switch for the
**     inclusion of "fuel03_bds.cfg" for backward compatibility.
**
**  Clearcase  6/15/05  RRUSS1
**
**  Ref BSDI00020125 : Removed the defines for the logging feature.
**
**  PVCS Rev 1.7
**  10/29/03     RRUSS1  Changed names of config items per review comments
**                       pertaining to the .cfg file.
**
**  PVCS Rev 1.0  10/14/03     RRUSS1
**
**  Initial version.*/
/*---------------------------------------------------------------------------
Date              : 25-Jul-2014
By                : sgajendr
Traceability      : 2013_MnM_U301_IC_SRD_V1.8.doc
Change Description: Updated the Macro FUEL03_VALID_CONDITION() as per PR comments. 
					PR id: 75979
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Release Label     : dev-20140820-MY15_MAHINDRA_U301_IC_BASELINE_7 
Date              : 20-Aug-2014
By                : sgajendr
Traceability      : SMIS PR id:75979
Change Description: Updated the Macro FUEL03_VALID_CONDITION() as per PR comments. 
					-----------------------------------------------------------------------------*/
/* end of file =============================================================*/
