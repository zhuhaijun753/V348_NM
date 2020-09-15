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
//  Name:               fuel03.h
//
//  Description:        Configurable fuel filter algorithms.
//
//  Organization:       Driver Information Software Section,
//                      DI Core Engineering Department
//
//==========================================================================*/
#ifndef FUEL03FLT_H
#define FUEL03FLT_H

#include "linear_interpolation.h"


/*============================================================================
//                         O V E R V I E W
//==========================================================================*/
/*
** The intent of this code is to provide a configurable fuel filter
** algorithm for use in a variety of instrument cluster applications.
**
** Requirements for this code came from the core fuel gage requirements
** written by Brad Corbin of the SPSS group.
*/

/*============================================================================
//                 I N C L U D E   F I L E S
//==========================================================================*/

/*============================================================================
//   P U B L I C   M A C R O   A N D   T Y P E   D E F I N I T I O N S
//==========================================================================*/

/*
** Define some invalid data indicators
*/
#define FUEL03_PERCENT_FULL_INVALID_BYTE  	(0xFF)
#define FUEL03_PERCENT_FULL_INVALID_WORD  	(0xFFFFu)
#define FUEL03_RAW_INVALID_WORD				(0xFFFFu)

/*============================================================================
//   P U B L I C   F U N C T I O N   P R O T O T Y P E S
//==========================================================================*/
/*
** Add the following to SCHED.CFG in the appropriate list.
*/
void fuel03_KSColdInit(void);
void fuel03_KSWarmInit(void);
void fuel03_KSWakeup(void);

/*
** The following should be called by the product specific fuel routines
** in FFUEL.C when it is necessary to start a fuel init, i.e. on
** a transition to RUN or after a high/low battery condition.
**
** If desired, the application can call this function whenever the
** ignition is OFF or ACC, so that when the ignition transitions into
** RUN the filter code is already primed to perform fuel init.
**
** It does not matter how many times this function is called as it
** is just initializing RAM to begin a fuel init.
*/
void fuel03_setup_for_init(void);

/* ---------------------------------------------------------------------------
** The following should be called by the product specific fuel routines
** in FFUEL.C on a periodic basis.  Do not call this function unless
** the filter should be iterated. For example, do not call in ignition
** OFF or in overvoltage conditions.
**
** The period at which this function is called depends on the type of
** fuel sender input -- network or hardwired.  In the network case,
** the function should be iterated whenever a new data message is received.
** In the hardwired case, the function should be iterated at a fixed rate,
** for example 100ms.
**
** The program subsystem engineer should provide requirements for when
** the filter should be iterated and at what rate.  The rate chosen will
** affect the values of many of the constants in the .CFG file.
**
** The first parameter (rada) in the call is for the SENDER A ratio'd A/D value, and
** the second (radb) is for SENDER B.  In single sender systems, the SENDER B value is
** ignored so the application can put any value in there it desires.
**
** The third parameter should be TRUE when the cluster is in manufacturing
** test mode and wants the filter(s) to be jammed with the current A/D
** value.  This value should be set to FALSE otherwise.
** ---------------------------------------------------------------------------
*/
boolean fuel03_periodic_processing(uint16 fuel03_rada, boolean fuel03_jam_filter);

/*
** The following should be called by the product specific fuel routines
** in FFUEL.C on a periodic basis when the ignition switch is off AND
** key-off fuel A/D sampling is required.  Do not call this function unless
** samples should be taken. For example, do not call in overvoltage
** conditions.
**
** The period at which this function is called depends on the type of
** fuel sender input -- network or hardwired.  In the network case,
** the function should be iterated whenever a new data message is received.
** However, it is very likely that the network is not active when the
** ignition switch is off, so this function will not be needed.
** In the hardwired case, the function should be iterated at a fixed rate,
** for example 100ms.
**
** The program subsystem engineer should provide requirements for when
** the filter should be iterated and at what rate.  The rate chosen will
** affect the values of many of the constants in the .CFG file.
**
** The first parameter in the call is for the FDM ratio'd A/D value, and
** the second is for the PFM.  In single sender systems, the PFM value is
** ignored so the application can put any value in there it desires.
**
** The application must continue to call this function (i.e. not go to
** sleep) in OFF until the keyoff processing is complete.
*/
void fuel03_keyoff_processing(uint16 fuel03_rada);
boolean fuel03_keyoff_processing_done( void );

/* ----------------------------------------------------------------------------
** The following should be called to get the percent full value of the
** sender(s) with and without hysteresis.
**
** Typically, the hysteresis value is used to turn on/off the low fuel
** warning light, and the non-hysteresis value is used for fuel gage
** position determination and for data for network transmission of the
** current fuel level.
**
** On single sender systems, the function returns the value for a single
** sender.  On dual sender systems, the function returns the combined value
** for both senders.
**
** These routines do not calculate anything as that is done by the periodic
** processing routines.  They can be called any time to get the latest value.
**
** The return values are in SCP percent full units, i.e. 0 represents 0%
** full, and 254 represents 99.60% full.  A value of 255 is used to represent
** invalid data.  The MSB of the data represents the whole part of the
** percent full value, and the LSB represents remainder.
**
** On single sender systems, the return value is only valid if
** "fuel03_open_short_detected()" is false.  On dual sender systems, the
** return value will be PERCENT_FULL_INVALID_WORD when a fault is
** detected.
** ----------------------------------------------------------------------------
*/
/*
 It would be called by application code when a CAN Diagnostics Clear DTCs
 (Service $14) command is received by the cluster regardless of whether Fuel DTCs are logged or not.
 This is executed only when called by application code, so in a sense it is "asynchronous".
*/
void fuel03_clear_DTC_resets(void);

uint16 fuel03_get_fuel_level(void);
uint16 fuel03_get_fuel_level_with_hys(void);

/* ----------------------------------------------------------------------------
** The following should be called to get the individual SenderA and SenderB
** percent full values with or without hysteresis.
**
** The description of the above functions apply to these as well.
** ----------------------------------------------------------------------------
*/
uint16 fuel03_get_senderA_PFH(void);
uint16 fuel03_get_senderA_PF(void);

uint8 fuel03_get_SenderA_unfiltered_flps( void );

/* ----------------------------------------------------------------------------
** The following function returns TRUE when fuel init is finished, FALSE
** otherwise.
**
** Once TRUE, it will remain true until the "fuel03_setup_for_init()"
** function is called.
** ----------------------------------------------------------------------------
*/
boolean fuel03_init_finished(void);

/*
** The following function returns TRUE if any sender is in fast fill, FALSE
** otherwise.
*/
boolean fuel03_fast_fill_active(void);
boolean fuel03_senderA_fast_fill_active(void);

/*
** The following function returns TRUE if a fill is detected, FALSE otherwise.
**
** On dual sender systems, both senders are tested for a fill and TRUE is
** returned if either is TRUE.
**
** Once TRUE, it will remain true until the "fuel03_setup_for_init()"
** function is called.
*/
boolean fuel03_fill_detected(void);
boolean fuel03_senderA_fill_detected(void);

boolean fuel03_ign_off_on_fill_notification(void);

void fuel03_ign_off_on_fill_notification_clr(void);

/*
** The followings function returns TRUE if a loss is detected, FALSE otherwise.
**
** On dual sender systems, both senders are tested for a loss and TRUE is
** returned if either is TRUE.
**
** Once TRUE, it will remain true until the "fuel03_setup_for_init()"
** function is called.
*/
boolean fuel03_loss_detected(void);
boolean fuel03_senderA_loss_detected(void);

/*
** The following function returns TRUE if a fill or loss is detected, FALSE otherwise.
**
** On dual sender systems, both senders are tested and TRUE is
** returned if either is TRUE.
**
** Once TRUE, it will remain true until the "fuel03_setup_for_init()"
** function is called.
*/
boolean fuel03_fill_or_loss_detected(void);

/*
** Call the following function if the fuel A/D input comes from a network and
** the message containing the data is missing.  It is only necessary to call
** this once when the message is first declared missing.
**
** It is the applications responsibility to log any DTCs for missing messages
** that may be required.
*/
void fuel03_network_message_missing(void);

/*
** Use the following to get the CANFLPS value from the table contained
** in the config file. Note that you must pass the RAD value.
*/
uint16 Fuel03_Get_CANFLPS( uint16 radin );

void fuel03_set_refuel_flag(boolean flag);
boolean fuel03_get_refuel_flag(void);


/* --------------------------------------------------------------------------
** Bits used with Fuel03OptionsNVM.
** --------------------------------------------------------------------------
*/
#define FUEL03_HILL_HOLD_ENABLE_BIT   (0x80)
#define FUEL03_FAST_FILL_ENABLE_BIT   (0x40)
#define FUEL03_ZSS_REF_ONLY_BIT       (0x20)

uint8 Fuel03_SenderA_keyon_avgflps( void );
uint8 Fuel03_SenderA_KeyOffRef( void );
uint8 Fuel03_SenderA_zss_ref( void );
uint8 Fuel03_SenderA_keyon_good_samples( void );
uint8 Fuel03_SenderA_ConvSample( void );



/*============================================================================
//   P U B L I C   D A T A   A N D   A C C E S S   M A C R O S
//==========================================================================*/


/*============================================================================
//
//  $Revision:   1.6  $
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
 *
 *
 * 11/19/13     BKORAGAN
 * FLPS interface implementation for trip computer request. NISSAN Specific
 * RTC work item:    99371
 *
 *
// 05/02/11        KNAGASUB
//Following interfaces are added tosupport BSDI00103693-Separate fuel open short bit check interface 
//required for logging DTC's
//1)boolean fuel03_open_bit_check(void)
//2)boolean fuel03_short_bit_check(void)
//3)boolean fuel03_open_short_bit_check(void)

// 06/22/09       KNAGASUB.
// The following macros are removed per peer review comments
// a)FUEL03_NEED_PID_C903
// b)FUEL03_NEED_PID_C904
// C) FUEL03_NEED_PID_C909
// d)FUEL03_NEED_PID_C90A
// e)FUEL03_NEED_PID_C90B
// f)FUEL03_NEED_PID_C90C
// g)FUEL03_NEED_PID_C90D
// h)FUEL03_NEED_PID_6185
// i)FUEL03_NEED_PID_6187


// 06/02/09       KNAGASUB.
// The following macros are included to avoid compiler warnings
// a)FUEL03_NEED_PID_C903
// b)FUEL03_NEED_PID_C904
// C) FUEL03_NEED_PID_C909
// d)FUEL03_NEED_PID_C90A
// e)FUEL03_NEED_PID_C90B
// f)FUEL03_NEED_PID_C90C
// g)FUEL03_NEED_PID_C90D
// h)FUEL03_NEED_PID_6185
// i)FUEL03_NEED_PID_6187
Following functions are included per fuel03_BDD1.5.doc.
  a)uint8 fuel03_61B7_KSGetPID(uint16 *);
  b)uint8 fuel03_4190_KSGetPID(UINT32 *);

// 24/11/08       KNAGASUB.
// The interface function name changed from Fuel03_clear_DTC_resets()  to fuel03_clear_DTC_resets()

// 19/11/08       KNAGASUB.
// The interface function name changed from Fuel03_clear_DTC_flimers()  to Fuel03_clear_DTC_resets()

//  28/10/08      KNAGASUB
//  new interface function Fuel03_clear_DTC_flimers() added as per fuel03_BDD1.4.doc

//  05/08/08      CNAGARA1
//  Added prototypes for C912,C913,C914 & C915 PID interface function per BSDI00055595
//
//  10/25/07      RRUSS1
//
//  Ref BSDI00050096 : Updated C90e - C911 interfaces.
//
//  02/28/07     JARUN   Added interfaces for P356 ETM mode.
//
//  9/21/05      RRUSS1
//
//  Ref BSDI00025533 : Changed the function interfaces for
//                     fuel03_6185_KSGetPID() and fuel03_6187_KSGetPID().
//
// 6/15/05        RRUSS1  BSDI00020719
//
// Moved NVM declarations to a seperate file for the ARM processor support so
// had to add certain bit flags here.
//
//  03/17/05     RRUSS1  Initial creation from fuel02 package.
//
//==========================================================================*/

/* end of file =============================================================*/

#endif

