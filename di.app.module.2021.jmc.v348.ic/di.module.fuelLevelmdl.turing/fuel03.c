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
//  Name:               fuel03.c
//
//  Description:        Configurable fuel filter algorithms.
//
//  Organization:       Driver Information Software Section,
//                      DI Core Engineering Department
//
//==========================================================================*/
#define FUEL03_C


/*============================================================================
//                         O V E R V I E W
//==========================================================================*/
/*
** The intent of this code is to provide a configurable fuel filter
** algorithm for use in a variety of instrument cluster applications.
**
** This package is a stripped down single sender version of the Fuel02 package.
** Therefore, the requirements for this code package are detailed in the
** Fuel Algorithm Bookshelf Design Specification (BDS) 4FA1-1A306-CA.
*/


/*============================================================================
//                 I N C L U D E   F I L E S
//==========================================================================*/
#include "system.h"
#include "fuel03.h"
#include "fuel03_cfg.h"
#include "fuel03_bds_cfg.h"
#include "Rte.h"
#include "Rte_type.h"
#include "CmpLib.h"
#include "fuel03_nvm.h"
#include "Rte_FuelLevelMdl.h"


#ifdef NISSAN_CHINA
#include "string.h"
#endif

extern boolean l_fuel_stable_finish;

uint16 test_count;

/*============================================================================
// I N T E R N A L   M A C R O   A N D   T Y P E   D E F I N I T I O N S
//==========================================================================*/
/*
// Define a macro to get past the QAC problem with !TESTBIT
*/
#define FUEL03_NOTTESTBIT( operand, bit_mask ) (((operand) &  (bit_mask)) == ((bit_mask) - (bit_mask)))

#define FUEL03_MS_8_BITS( w )                      ((uint8)((w) >> (8)))
#define FUEL03_MS_16_BITS( l )                     ((uint16)((l) >> (16)))

/* --------------------------------------------------------------------------
** Define bit flags for the FilterFlags member of the FUEL03_FLT structure.
** --------------------------------------------------------------------------
*/
#define FUEL03_FILL_DETECTED           	(0x20u)
#define FUEL03_LOSS_DETECTED           	(0x10u)
#define FUEL03_SIG_MISSING				(0x08u)
#define FUEL03_SIG_INVALID      		(0x04u)


/* --------------------------------------------------------------------------
** Define bit flags for the MoreFilterFlags member of the FUEL03_FLT structure.
** --------------------------------------------------------------------------
*/
#define FUEL03_KEY_OFF_SAMPLES_DONE    (0x80u)
#define FUEL03_FAST_FILL_FLAG          (0x40u)
#define FUEL03_FORCE_FUEL_INIT_FLAG    (0x20u)
#define FUEL03_INIT_FLAG               (0x10u)
#define FUEL03_RE_INIT_FLAG            (0x08u)
#define FUEL03_IG_ON_ENTERY_FLAG	   (0x04u)
#define FUEL03_IG_ON_FAST_FILL_FLAG	   (0x02u)
#define FUEL03_SIG_MIS_INVALID_RECOV   (0x01u)

/* --------------------------------------------------------------------------
** Define general bit flags used in the FuelFlags RAM storage byte.
** --------------------------------------------------------------------------
*/
#define FUEL03_COLD_INIT               (0x80u)
#define FUEL03_FORCE_INIT              (0x40u)
#define FUEL03_KEY_ON_DELAY_EXPIRED    (0x20u)

/* --------------------------------------------------------------------------
** Values used for PID C90A.
** --------------------------------------------------------------------------
*/
#define FUEL03_FILTER_NO_INIT      (0)
#define FUEL03_FILTER_NO_SAMPLES   (1)
#define FUEL03_FILTER_JAM_COLD     (2)
#define FUEL03_FILTER_JAM_FORCED   (3)
#define FUEL03_ZERO_SPEED_REF_USED (4)
#define FUEL03_KEY_OFF_REF_USED    (5)
#define FUEL03_FILTER_MSB_REF_USED (6)

/* --------------------------------------------------------------------------
** Values used for fuel03_invalid_condition variable which is used to
** store the FUEL03_VALID_CONDITION() condition status at different locations
** in the code
** --------------------------------------------------------------------------
*/
#define FUEL03_KEYOFF_PROCESSING_DONE   (0x01u)
#define FUEL03_ZSS_KEYOFF_PROCESSING    (0x02u)
#define FUEL03_KEYOFF_PROCESSING_1      (0x04u)
#define FUEL03_KEYOFF_PROCESSING_2      (0x08u)
#define FUEL03_PERIODIC_PROCESSING_1    (0x10u)
#define FUEL03_PERIODIC_PROCESSING_2    (0x20u)

/* --------------------------------------------------------------------------
** Values used for Fuel snapshot nvm queuing.
** --------------------------------------------------------------------------
*/
#define FUEL03_BLK1_QUEUED      (0x80u)
#define FUEL03_BLK2_QUEUED      (0x40u)

//#define FullStopFuel							(6800)			//68L
#define FullStopFuel							(8060)			//80.6L
#define EmptyStopFuel							(0)				//0L


/* --------------------------------------------------------------------------
** Typedef the fuel gage filter structure.  For each sender in the product,
** an instance of this structure must be created in RAM.
** --------------------------------------------------------------------------
*/
/* ------------------------------------------------------ */
/* Reordered the following for BSDI00024469 23/02/07 jarun*/
/* ------------------------------------------------------ */
/* ------------------------------------------------------------------------  */
/* KOS_ConvCycleCounter_shadow & KOS_SmplCounter_shadow elements             */
/* are added to just to have the copy of ConvCycleCounter & SmplCounter      */
/* variables. These shadow values are used to store in RAM snapshot in       */
/* fuel03_log_init_errors_to_nvm(). The reason to have the shadow is that    */
/* ConvCycleCounter & SmplCounter are cleared once KEY_OFF processing done   */
/* So the information is lost before storing into RAM snapshot which happens */
/* upon IGN trans into RUN. We used shadow to not disturb the existing proven */
/* code                                                                       */
/* -------------------------------------------------------------------------- */

typedef struct {
    uint32 fdelta;                  /* [0] Fill Delta (fdelta) value that is added or subtracted
    from slosh filter value depending on the 'direction' of
    the incoming sender value. */
    uint32 fdelta_base;             /* [0] Fill Delta Base (fdelta_base) value used as the lowest
    value for the fdelta value. */
    uint32 keyon_radsum;            /* [2] accumulator for the good rad samples at keyon       */
    uint32 slosh_filter;            /* [1] slosh filter value                       */
    uint32 slosh_filter_hys;        /* [0] slosh filter with hysteresis             */
    uint16 fdelta_adjust_base;      /* [0] Fill Delta Adjust value used as a small adjustment value
    added to fdelta. */
    uint16 init_pid;                /* [1] Storage location for holding what was used for the
    initialization reference. */
    uint16 FuelFaultCounter;        /* [1] Sender open/short fault counter. */
    uint16 FuelFaultRecoveryCounter;/* [1] Sender open/short recovery fault counter. */

    uint16 rad;                     /* [1] ratio'd fuel a/d value from application */
    uint16 avg_rad;                 /* [0] Average 'rad' set by the average rad processing function and based on different averaging schemes. */
    uint16 old_rad;                 /* [0] Used in the average rad processing to hold the 'old' rad. */
	uint16 fdelta_counter;
    uint16 KeyOffRef;               /* [2] Key off refernce storage location.   Parameter A */
    uint16 IgnOnRef;               /* [2] Key off refernce storage location. */


    uint16  err_counter;            /* [0] Error counter for NVM data logging. */
    uint8  FilterFlags;             /* [1] FilterFlags is used to store various 'flags' that are defined above. */
    uint8  flps;                    /* [1] Fuel Level Percent full value from 0 to 254, 255 indicates invalid. */
    uint8  init_ref;                /* [1] Value used in the fuel filter initialization to hold the 'reference' value. */


    uint8  keyon_good_samples;      /* [2] count up counter used during key on sampling period */
    uint8  keyon_avgflps;           /* [2] Key on average flps storage location. */
    uint8  avgflps_saddle;          /* [1] Average flps if saddle tank configuration is used. */
    uint8  MoreFilterFlags;         /* [1] Additional filter flags defined above. */
    uint8  rc_fill_threshold;       /* [0] rental car fill threshold value from NVM */
    uint8  rc_fill_delta;           /* [0] rental car fill delta value from NVM     */
    uint8  rflps;                   /* [1] reference flps */
    uint8  rfuel;                   /* [1] reference fuel, refer to the bds for more info */
    uint8  zss_ref;                 /* [1] zero speed reference value               */
    uint8   moving_sample_counter;  /* [0] Used in the average rad processing. Know as Sample Counter in the BDS. */
    uint8   use_avg_flag;           /* [0] Flag indicating what to use in the average rad processing. */
    uint8   use_mov_avg;            /* [0] Flag indicating what to use in the average rad processing. */
    boolean log_nvm_data;           /* [0] Flag indicating if the data should be logged in NVM. */
    uint16 sample_array[FUEL03_SMPLS_2_AVG];
    /* [0] array used in average rad processing to hold the
    last 'x' samples for averaging. */
    uint8 queue_result;

    /*                            Note: Values in [] indicates if the parameter is logged in NVM in
    an 'error' is detected.
    0 = No logging
    1 = Log it
    2 = Log it if the feature using the item is enabled.
    */
    uint8  KOS_ConvCycleCounter_shadow;
    uint8  KOS_SmplCounter_shadow;
} FUEL03_FLT;

/*============================================================================
//      I N T E R N A L   F U N C T I O N   P R O T O T Y P E S
//==========================================================================*/
static void fuel03_cold_init( void );
static void fuel03_start_init( void );
//static void fuel03_test_sample( void );
//static void fuel03_init_avgflps_calc(void);
//static void fuel03_ref_fuel_calc( void );
static uint8 fuel03_rad_to_flps( uint16 fuel03_rad );
static void fuel03_slosh_filter(boolean fuel03_live_mode);
static void fuel03_fast_fill_enable_check(void);
static void fuel03_fast_fill_determination(void);
static void fast_fill_determination_f1( void );
//static void fuel03_level_calculation( void );
//static void fuel03_level_calculation_with_hys( void );
static void fuel03_filter_initialize( void );
static void fuel03_fast_fill_samples( void );
//static uint8 fuel03_open_short_test(uint16 fuel03_sample);
static void fuel03_fi_check( void );
static void fuel03_finit_F3( void );
static void fuel03_finit_t3( void );
static void fuel03_key_off_sample_processing( void );
static void fuel03_zss_keyoff_processing( uint16 fuel03_rada );
static void fuel03_common_gauge_init( void );
static void fuel03_average_rad_processing( uint8 Avg_Type );
static void fuel03_fi_compare( void );
static void fuel03_ign_off_on_fill_determination(void);

#if FUEL03_RAM_LOG_FAULT_RECORD_SIZE > 0
static void fuel03_log_init_errors_to_ram( void );
#endif
#if FUEL03_LOG_FAULT_RECORD_SIZE > 0
static void fuel03_log_init_errors_to_nvm( void );
static uint8 fuel03_fault_snapshot_struct_size(void);

#endif



/*============================================================================
//                M E M O R Y   A L L O C A T I O N
//==========================================================================*/

extern boolean l_fuel_abnormal_status;

/* --------------------------------------------------------------------------
** Structure RAM allocation for both sender.
** --------------------------------------------------------------------------
*/
static FUEL03_FLT Fuel03SenderA;

/* --------------------------------------------------------------------------
** Combined sender percent full, with and without hysteresis.
** --------------------------------------------------------------------------
*/
//static uint16 fuel03_level;
static uint16 fuel03_level_with_hys;
/* --------------------------------------------------------------------------
** General flags used by the filter algorithm.
** --------------------------------------------------------------------------
*/
static uint8 Fuel03Flags;

/* --------------------------------------------------------------------------
** State variable to check the IGN change to do a fuel03_koc_setup()      **
** For fast fill mode.  BSDI00020595,  26/02/07 jarun                     **
** --------------------------------------------------------------------------
*/
#define FUEL03_RESET  0
#define FUEL03_KEYON  1
#define FUEL03_KEYOFF 2

/* --------------------------------------------------------------------------
** Timer used during fuel init.  The units of this timer are that of the
** filter iteration rate chosen by the application.
** --------------------------------------------------------------------------
*/
static uint8 Fuel03KeyOnDelayCounter;
static uint8 Fuel03InitDelayCounter;

/* --------------------------------------------------------------------------
** Counter for determining when to store the zero speed fuel sample.
** --------------------------------------------------------------------------
*/
static uint16 Fuel03ZeroSpeedCounter;

#if (FUEL03_REF_AVG_TYPE!=4)
/* --------------------------------------------------------------------------
** Key-off sampling counter.
** --------------------------------------------------------------------------
*/
static uint16 Fuel03KeyOffSampleMaxTimer;
#endif
#if FUEL03_RAM_LOG_FAULT_RECORD_SIZE > 0
static uint8 fuel03_RAM_fault_record;

#endif

/* --------------------------------------------------------------------------
** Rad to flps 16 bit storage location for fuel03_rad_to_flps.
** BSDI00030114 4/10/06 RRUSS1
** --------------------------------------------------------------------------
*/
static uint16 Fuel03PercentFull;

/* --------------------------------------------------------------------------
** Timer for enabling Fast Fill if there are not enough good keyon samples.
** --------------------------------------------------------------------------
*/

//static uint16 Fuel03_AVG_Vol_Invalid_Counter;

/* --------------------------------------------------------------------------
** Timer for entering Fast Fill Mode if FUEL03_MIN_FAST_FILL_TIME elapsed.
** BSDI00044140  03/30/07 JARUN
** --------------------------------------------------------------------------
*/
static uint8  Fuel03MinFastFillTimeCounter;

static uint8  Fuel03FastFillLatencyTimeCounter;

/* --------------------------------------------------------------------------
** Used to store the FUEL03_VALID_CONDITION() condition check status
** This is used for debug purpose
** --------------------------------------------------------------------------
*/
static uint8 fuel03_invalid_condition;

/* --------------------------------------------------------------------------
** Used to track the number of IGN cycles.It is used to see if there were
** fast key cycles
** --------------------------------------------------------------------------
*/
static uint8 fuel03_ignition_status;

static const uint32 fuel03_PRELOAD = FUEL03_SLOSH_FILTER_EMPTY_PRELOAD_A;

static boolean fuel03_ign_off_on_fill_flag;

static boolean fuel03_fastfill_flag;

static boolean fuel03_refuel_flag;

/*============================================================================
//              F U N C T I O N S / S E R V E R S
//==========================================================================*/


/*============================================================================
//      K E Y O F F   C O N V E R G E N C E
//==========================================================================*/



#ifdef FUEL03_NEED_DIAG_EPILOG
/*============================================================================
//
//  Function Name:      void fuel03_KSDiagEpilog(void)
//
//  Visibility:         External
//
//  Description:        Diagnostic epilog routine for the fuel gage.  Start
//                      a re-init.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            08/06/03 by RRUSS1
//
//  Updated:            08/06/03 by RRUSS1
//
//==========================================================================*/
void fuel03_KSDiagEpilog(void)
{
    /*
    ** Start a re-init of the fuel gage via the init function to ensure
    ** that all RAM is reset.
    */
    fuel03_setup_for_init();
}
#endif
#if FUEL03_RAM_LOG_FAULT_RECORD_SIZE > 0

static FUEL03_FAULT_SNAPSHOT_STRUCT fuel03_snapshot_A_RAM[FUEL03_RAM_LOG_FAULT_RECORD_SIZE];

/*
PID C90E -- 4 byte pointer to RAM buffer containing fuel init information (will only need 2 bytes for Mako micros, but will need more for other micros)
PID C90F -- 2 byte size of RAM buffer in PID C90D
PID C910 -- 4 byte pointer to NVM buffer containing fuel init information
PID C911 -- 2 byte size of NVM buffer in PID C90F
*/
#endif


#if FUEL03_LOG_FAULT_RECORD_SIZE > 0

/*============================================================================
//
//  Function Name:      uint8 fuel03_log_init_errors_to_nvm( void )
//
//  Visibility:         External
//
//  Description:        Routine to log the fuel init values to NVM.
//  INPUTS/OUTPUTS
//
//  Input Arguments:
//
//  Output Arguments:
//
//  Critical Section:   None
//
//  Created:            01/13/05 by RRUSS1
//
//  Updated:            01/13/05 by RRUSS1
//                      03/10/08 by CNAGARA1 - BSDI00055411
//                      05/08/08 by CNAGARA1 - To update for peer review comments
//
//==========================================================================*/

#if 0
static void fuel03_log_init_errors_to_nvm( void )
{
    boolean fuel03_epm_result;
    uint8 fuel03_next_fault_location_RAM = 0;
    uint8* fuel03_senderA_nvm_addr;
    uint8 *fuel03_senderA_ram_addr;

    if( Fuel03SenderA.log_nvm_data != FALSE )
    {
        Fuel03SenderA.queue_result = 0;
        /* ------------------------------------------------------------------------
        // Get the last stored record number from NVM and point it to the next one.
        // ----------------------------------------------------------------------*/
        fuel03_next_fault_location_RAM = fuel03_next_fault_log_to_write_NVM;
        fuel03_next_fault_location_RAM++;

        if( fuel03_next_fault_location_RAM >= FUEL03_LOG_FAULT_RECORD_SIZE )
        {
            fuel03_next_fault_location_RAM = 0;
        }

        /* ------------------------------------------------------------------------
        // Write the record number back to NVM.
        // ----------------------------------------------------------------------*/
        fuel03_epm_result = epm_pkg_q_e2_write( (void *)&fuel03_next_fault_log_to_write_NVM,
            (void *)&fuel03_next_fault_location_RAM,
            NULL, 1);

        if( fuel03_epm_result == FALSE )
        {
            return;
        }
        else
        {
            Fuel03SenderA.log_nvm_data = FALSE;
        }
    }

    if ( Fuel03SenderA.queue_result ==  (FUEL03_BLK1_QUEUED | FUEL03_BLK2_QUEUED) )
    {
        return;
    }

    /*
    ** If all the bytes are not queued at first shot then the following assignment
    ** will get the next fault location from NVM.
    */
    fuel03_next_fault_location_RAM = fuel03_next_fault_log_to_write_NVM;

    fuel03_senderA_nvm_addr = (void *)&fuel03_fault_snapshot_A_NVM[fuel03_next_fault_location_RAM];
    fuel03_senderA_ram_addr = (void *)&fuel03_snapshot_A_RAM[fuel03_RAM_fault_record];

    /* ------------------------------------------------------------------------
    // Check the results of the last write operation and only update the
    // fault log record if the record number value was written correctly.
    // ----------------------------------------------------------------------*/
    if( fuel03_fault_snapshot_struct_size() > EPM_MAX_NUM_MODIFY)
    {
        if ( FUEL03_NOTTESTBIT(Fuel03SenderA.queue_result,FUEL03_BLK1_QUEUED) )
        {
            if ( FALSE != epm_pkg_q_e2_write( (void *)fuel03_senderA_nvm_addr,
                (void *)fuel03_senderA_ram_addr,
                NULL,EPM_MAX_NUM_MODIFY ) )
            {
                SETBIT(Fuel03SenderA.queue_result,FUEL03_BLK1_QUEUED);
            }
        }

        if ( FUEL03_NOTTESTBIT(Fuel03SenderA.queue_result,FUEL03_BLK2_QUEUED) )
        {
            if( FALSE != epm_pkg_q_e2_write( (void *)&fuel03_senderA_nvm_addr[EPM_MAX_NUM_MODIFY],
                (void *)&fuel03_senderA_ram_addr[EPM_MAX_NUM_MODIFY],
                NULL,(uint8)( sizeof( FUEL03_FAULT_SNAPSHOT_STRUCT ) - EPM_MAX_NUM_MODIFY) ) )
            {
                SETBIT(Fuel03SenderA.queue_result,FUEL03_BLK2_QUEUED);
            }
        }
    }
    else
    {
        if ( FUEL03_NOTTESTBIT(Fuel03SenderA.queue_result,FUEL03_BLK1_QUEUED) )
        {
            if ( FALSE != epm_pkg_q_e2_write( (void *)fuel03_senderA_nvm_addr,
                (void *)fuel03_senderA_ram_addr,
                NULL,sizeof( FUEL03_FAULT_SNAPSHOT_STRUCT ) ) )
            {
                SETBIT(Fuel03SenderA.queue_result,FUEL03_BLK1_QUEUED);
            }
            /*
            ** The BLK2 will be not used if the size of the structure is less than
            ** EPM_MAX_NUM_MODIFY size. So set FUEL03_BLK2_QUEUED always
            */
            SETBIT(Fuel03SenderA.queue_result,FUEL03_BLK2_QUEUED);
        }
    }
}
#endif


/*============================================================================
//
//  Function Name:     static uint8 fuel03_fault_snap_struct_size(void)
//
//  Visibility:         Internal
//
//  Description:        Routine to return size of FUEL02_FAULT_SNAPSHOT_STRUCT.
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None.
//
//  Output Arguments:   Return size of FUEL02_FAULT_SNAPSHOT_STRUCT.
//
//  Critical Section:   None
//
//  Created:            10/12/10 by KNAGASUB
//
//  Updated:            
//
//==========================================================================*/
static uint8 fuel03_fault_snapshot_struct_size(void)
 {  
    return(sizeof( FUEL03_FAULT_SNAPSHOT_STRUCT));
 }


#endif


/*============================================================================
//
//  Function Name:      void fuel03_average_rad_processing( uint8 Avg_Type )
//
//  Visibility:         Internal
//
//  Description:        This function performs the averaging feature.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    Averaging type (FUEL03_AVG_TYPE or FUEL03_REF_AVG_TYPE)
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            08/06/04 by RRUSS1
//
//  Updated:            06/28/13 by bkoragan
//==========================================================================*/
static void fuel03_average_rad_processing( uint8 Avg_Type )
{

      uint8 fuel03_x;
      uint32 fuel03_temp32;

    #ifdef NISSAN_CHINA
	uint16 sample_array_sorted[FUEL03_SMPLS_2_AVG];
	uint16 swap_u16;
	uint8  i, j;
    #endif
        /* ----------------------------------------------------------------------
         ** (2) FUEL03_AVG_TYPE or FUEL03_REF_AVG_TYPE = 0 ?
         ** ----------------------------------------------------------------------
         ** Check to see if the avg_type is 0. If it is then just set the
         ** avg_rad to the rad value and return.
         ** --------------------------------------------------------------------*/
       if( Avg_Type == 0 )
       {     Fuel03SenderA.avg_rad = Fuel03SenderA.rad;  /* (3) AVG RAD x = RAD x */
            return;
       }
       else
       {
       
            if( Fuel03SenderA.moving_sample_counter == 0 )
            {
                Fuel03SenderA.moving_sample_counter = FUEL03_SMPLS_2_AVG;
                Fuel03SenderA.use_avg_flag = 1;
            }
            Fuel03SenderA.moving_sample_counter--;

            if( TESTBIT(Fuel03SenderA.FilterFlags, FUEL03_SIG_INVALID) )
            {
                if( Avg_Type == 3 )
                {
                    Fuel03SenderA.use_avg_flag = 0;
                }
                else
                {
                    Fuel03SenderA.rad = Fuel03SenderA.old_rad;
                }
            }
            else
            {
                Fuel03SenderA.old_rad = Fuel03SenderA.rad;
            }
            /* ----------------------------------------------------------------------
             ** (13) Sample Sum x = Sample Sum x + RAD x
             ** ----------------------------------------------------------------------
             ** Put the 'rad' value (could be old_rad) into the sample_array[] buffer.
             ** --------------------------------------------------------------------*/
            fuel03_x = Fuel03SenderA.moving_sample_counter;
            Fuel03SenderA.sample_array[fuel03_x] = Fuel03SenderA.rad;

            /* ----------------------------------------------------------------------
             ** (14) Sample Counter x = 0 ?
             ** --------------------------------------------------------------------*/
            if( Fuel03SenderA.moving_sample_counter == 0 )
            {
                if( Avg_Type == 1 )
                {    Fuel03SenderA.use_mov_avg = 1;   }
                else
                {
                    if( Fuel03SenderA.use_avg_flag == 0 )
                    {
                    }
                    else
                    {
                        fuel03_temp32 = 0;
                        for( fuel03_x=0; fuel03_x<FUEL03_SMPLS_2_AVG; fuel03_x++ )
                        {
                            fuel03_temp32 += Fuel03SenderA.sample_array[fuel03_x];
                        }
                        fuel03_temp32 /= (uint32)FUEL03_SMPLS_2_AVG;
                        Fuel03SenderA.avg_rad = (uint16)fuel03_temp32;
                    }
                }
            }

            /* ----------------------------------------------------------------------
             ** (20) Sample Counter x = 0 ?
             ** --------------------------------------------------------------------*/
            if( Avg_Type == 1 )
            {
                #ifdef NISSAN_CHINA
                if( Fuel03SenderA.use_mov_avg == 1 )
                {
                    (void)memcpy(sample_array_sorted, Fuel03SenderA.sample_array, FUEL03_SMPLS_2_AVG * sizeof(uint16));

                    for( i=0; i < (FUEL03_SMPLS_2_AVG-1); i++ )
                        for( j=0; j < (FUEL03_SMPLS_2_AVG-i-1); j++)
                        {
                            if(sample_array_sorted[j] > sample_array_sorted[j+1])
                            {
                                swap_u16 = sample_array_sorted[j];
                                sample_array_sorted[j] = sample_array_sorted[j+1];
                                sample_array_sorted[j+1] = swap_u16;
                            }
                        }

                    fuel03_temp32 = (uint32)0;
                    for( fuel03_x= 2; fuel03_x<FUEL03_SMPLS_2_AVG-2; fuel03_x++ )
                    {
                        fuel03_temp32 += sample_array_sorted[fuel03_x];
                    }

                    Fuel03SenderA.avg_rad = (uint16)(fuel03_temp32 >> 2);
                }
                #else
                if( Fuel03SenderA.use_mov_avg == 1 )
                {
                    fuel03_temp32 = 0;
                    for( fuel03_x=0; fuel03_x<FUEL03_SMPLS_2_AVG; fuel03_x++ )
                    {
                        fuel03_temp32 += Fuel03SenderA.sample_array[fuel03_x];
                    }
                    fuel03_temp32 /= (uint32)FUEL03_SMPLS_2_AVG;
                    Fuel03SenderA.avg_rad = (uint16)fuel03_temp32;
                }
                #endif
            }

       }

}


/*============================================================================
//
//  Function Name:      void fuel03_common_gauge_init(void)
//
//  Visibility:         Internal
//
//  Description:        This function initializes the variables to values that
//                      are common to Cold Reset, Warm Reset, and Transition
//                      into run.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    Address of FUEL03_FLT structure
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            09/15/03 by RRUSS1
//
//  Updated:            08/13/04 by RRUSS1
//                      07/09/12 by BKORAGAN
//==========================================================================*/
static void fuel03_common_gauge_init( void )
{
    /* ----------------------------------------------------------------------
    ** Set the key on number of good samples collected counter to 0. This
    ** counter counts up and is only used during the inital key on sampling
    ** period.
    ** --------------------------------------------------------------------*/
    Fuel03SenderA.keyon_good_samples = 0;


    /* ----------------------------------------------------------------------
    ** Common FilterFlags Configuration
    **
    ** The following bit flags in FilterFlag are cleared for the 3 common
    ** init events.
    ** --------------------------------------------------------------------*/
    CLEARBIT( Fuel03SenderA.FilterFlags, (FUEL03_FILL_DETECTED          |
        FUEL03_LOSS_DETECTED) );

    /* ----------------------------------------------------------------------
    ** Common MoreFilterFlags Configuration
    **
    ** The following bit flags in MoreFilterFlag are cleared for the 3 common
    ** init events.
    ** 8/13/2004 rruss1 : removed the fast fill init flag
    ** --------------------------------------------------------------------*/
    CLEARBIT( Fuel03SenderA.MoreFilterFlags, ( FUEL03_KEY_OFF_SAMPLES_DONE |
        FUEL03_INIT_FLAG) );

    /* ----------------------------------------------------------------------
    ** Set the average flps to 0.
    ** --------------------------------------------------------------------*/
    Fuel03SenderA.keyon_avgflps = 0;

    /* ----------------------------------------------------------------------
    ** Set the fdelta's.
    ** --------------------------------------------------------------------*/
    Fuel03SenderA.fdelta = 0;

	/*
	** Load fdelta, fdelta_base and fdelta_adjust_base from NVM based
	** on speed input if multirate fuel filter is enabled
	*/
	Fuel03SenderA.fdelta_counter = 0;
    Fuel03SenderA.fdelta_base = FUEL03_DELTA_NORMAL_BASE_A_NVM;
    Fuel03SenderA.fdelta_adjust_base = FUEL03_DELTA_NORMAL_ADJUST_A_NVM;

    /* ----------------------------------------------------------------------
    ** Set the flps to invalid.
    ** --------------------------------------------------------------------*/
    Fuel03SenderA.flps = FUEL03_PERCENT_FULL_INVALID_BYTE;

    /* ----------------------------------------------------------------------
    ** Start the init & key on delay counters.
    ** --------------------------------------------------------------------*/
    Fuel03KeyOnDelayCounter = Fuel03KeyOnDelayCounterNVM;
    Fuel03InitDelayCounter = Fuel03InitDelayCounterNVM;

    /* ----------------------------------------------------------------------
    ** Set the Min fast fill time counter to default value.
    ** --------------------------------------------------------------------*/
    Fuel03MinFastFillTimeCounter = Fuel03FastFillTimeCounterNVM;

    Fuel03SenderA.rad = FUEL03_PERCENT_FULL_INVALID_BYTE;

    Fuel03SenderA.keyon_radsum = 0;

#ifdef NISSAN_CHINA
    Fuel03SenderA.zss_ref = FUEL03_PERCENT_FULL_INVALID_BYTE;
#endif

    Fuel03SenderA.moving_sample_counter = 0;

//    Fuel03_AVG_Vol_Invalid_Counter = 0;

}

/*============================================================================
//
//  Function Name:      void fuel03_cold_init( void )
//
//  Visibility:         Internal
//
//  Description:        Cold init for fuel filter feature server.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    *fuel03_flt - fuel filter structure pointer.
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            09/16/03 by RRUSS1
//
//  Updated:            09/16/03 by RRUSS1
//                      08/07/10 by KNAGASUB
//==========================================================================*/
static void fuel03_cold_init( void )
{
	/* ----------------------------------------------------------------------
	** Cold Reset FilterFlags Configuration
	**
	** The following bit flags in FilterFlag are cleared during a
	** cold reset event.
	** --------------------------------------------------------------------*/

	/* ----------------------------------------------------------------------
	** Cold Reset MoreFilterFlags Configuration
	**
	** The following bit flags in MoreFilterFlag are cleared during a
	** cold reset event.
	** 8/13/2004 rruss1 : added fast fill flag
	** 8/7/2010  KNAGASUB:added re init flag as per Ford_STSS7.0_draft3.
	** --------------------------------------------------------------------*/
	CLEARBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_FAST_FILL_FLAG);
	CLEARBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_RE_INIT_FLAG);
	SETBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_FORCE_FUEL_INIT_FLAG );

	/* ----------------------------------------------------------------------
	** Set the slosh filter and slosh filter with hys to the preload value.
	** --------------------------------------------------------------------*/
	Fuel03SenderA.slosh_filter     = fuel03_PRELOAD;
	Fuel03SenderA.slosh_filter_hys = fuel03_PRELOAD;

	Fuel03SenderA.KeyOffRef = FUEL03_PERCENT_FULL_INVALID_BYTE;

	Fuel03SenderA.init_pid = 0x00ff;

	Fuel03SenderA.zss_ref = FUEL03_PERCENT_FULL_INVALID_BYTE;

	Fuel03SenderA.rflps = FUEL03_PERCENT_FULL_INVALID_BYTE;

	Fuel03SenderA.log_nvm_data = 0;

	Fuel03SenderA.queue_result = 0;

	fuel03_ignition_status = FUEL03_RESET;

	fuel03_fastfill_flag = FALSE;

}

/*============================================================================
//
//  Function Name:      void fuel03_KSColdInit(void)
//
//  Visibility:         External
//
//  Description:        Cold init for fuel filter feature server.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            05/07/03 by RRUSS1
//
//  Updated:            07/16/03 by RRUSS1
//                      02/27/07 by JARUN for BSDI00038410
//                      03/10/08 by CNAGARA1 - BSDI00055412
//==========================================================================*/
void fuel03_KSColdInit(void)
{
    fuel03_common_gauge_init();

    fuel03_cold_init();


    /* ----------------------------------------------------------------------
    ** Set the fuel cold init flag.
    ** --------------------------------------------------------------------*/
    SETBIT( Fuel03Flags, FUEL03_COLD_INIT );

    Fuel03SenderA.rc_fill_threshold = Fuel03SenderA_RCFillThresholdNVM;
    Fuel03SenderA.rc_fill_delta = Fuel03SenderA_RCFillDeltaNVM;
//    fuel03_level          = FUEL03_MS_16_BITS( fuel03_PRELOAD );
    fuel03_level_with_hys = FUEL03_MS_16_BITS( fuel03_PRELOAD );

    /*
    ** Set the COLD init flag and clear all of the others.
    */
    Fuel03Flags = FUEL03_COLD_INIT;

    /*
    ** Set up conditions to start init at the next appropriate time.
    */
    fuel03_setup_for_init();

    /*
    ** If the NVM logging is enabled, set the RAM record pointer to the
    ** max so the first time it is logged to RAM, the pointer will be set to 0.
    */

  #if FUEL03_RAM_LOG_FAULT_RECORD_SIZE > 0
    fuel03_RAM_fault_record = FUEL03_RAM_LOG_FAULT_RECORD_SIZE;
  #endif

    fuel03_invalid_condition = 0;

    fuel03_ign_off_on_fill_flag = FALSE;
}

/*============================================================================
//
//  Function Name:      void fuel03_KSWarmInit(void)
//
//  Visibility:         External
//
//  Description:        Warm init for fuel filter feature server.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            05/07/03 by RRUSS1
//
//  Updated:            07/16/03 by RRUSS1
//==========================================================================*/
void fuel03_KSWarmInit(void)
{
    /*
    ** Set up conditions to start init at the next appropriate time.
    */
    fuel03_setup_for_init();
}


/*============================================================================
//
//  Function Name:      void fuel03_KSWakeup(void)
//
//  Visibility:         External
//
//  Description:        Wakeup for fuel filter feature server.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            05/07/03 by RRUSS1
//
//  Updated:            07/16/03 by RRUSS1
//==========================================================================*/
void fuel03_KSWakeup(void)
{
    /*
    ** Set up conditions to start init at the next appropriate time.
    */
    fuel03_setup_for_init();
}


/*============================================================================
//
//  Function Name:      void fuel03_setup_for_init(void)
//
//  Visibility:         External
//
//  Description:        Function to be called whenever the product specific
//                      code wants to start a fuel gage init.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            05/07/03 by RRUSS1
//
//  Updated:            07/16/03 by RRUSS1
//==========================================================================*/
void fuel03_setup_for_init(void)
{
    fuel03_common_gauge_init();

    /*
    ** Set up conditions to start init at the next appropriate time.
    */
    fuel03_start_init();

    /*
    ** Clear all general fuel flags except COLD INIT and FORCE INIT.
    */
    CLEARBIT(Fuel03Flags, FUEL03_KEY_ON_DELAY_EXPIRED) ;

    /* ----------------------------------------------------------------------
    ** Start the init & key on delay counters.
    ** --------------------------------------------------------------------*/
    Fuel03KeyOnDelayCounter = Fuel03KeyOnDelayCounterNVM;
    Fuel03InitDelayCounter = Fuel03InitDelayCounterNVM;

    /*
    ** Start the zero speed counter.
    */
    Fuel03ZeroSpeedCounter = Fuel03ZeroSpeedCounterNVM;

}

/*============================================================================
//
//  Function Name:      void fuel03_start_init( void )
//
//  Visibility:         Internal
//
//  Description:        Setup conditions for fuel gage init.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    *fuel03_flt - fuel filter structure pointer.
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            05/07/03 by RRUSS1
//
//  Updated:            07/09/12 by BKORAGAN
//==========================================================================*/
static void fuel03_start_init( void )
{
    /*
    ** Set up RAM to perform a fuel init when the periodic filter routine is
    ** iterated.
    */
    Fuel03SenderA.keyon_radsum = 0;
    Fuel03SenderA.keyon_good_samples = 0;
    //Fuel03SenderA.FuelFaultCounter = Fuel03FaultCounterNVM;
    //Fuel03SenderA.FuelFaultRecoveryCounter = Fuel03FaultRecoveryCounterNVM;

	/*
	** Load fdelta, fdelta_base and fdelta_adjust_base from NVM based
	** on speed input if multirate fuel filter is enabled
	*/
     Fuel03SenderA.fdelta = FUEL03_DELTA_NORMAL_BASE_A_NVM;
     Fuel03SenderA.fdelta_base = FUEL03_DELTA_NORMAL_BASE_A_NVM;
     Fuel03SenderA.fdelta_adjust_base = FUEL03_DELTA_NORMAL_ADJUST_A_NVM;

    /*
    ** Clear all flags except the sender open/short flags.
    */
    CLEARBIT(Fuel03SenderA.FilterFlags, (FUEL03_FILL_DETECTED | FUEL03_LOSS_DETECTED) );


    Fuel03SenderA.rflps = FUEL03_PERCENT_FULL_INVALID_BYTE;

    CLEARBIT(Fuel03SenderA.MoreFilterFlags, (FUEL03_KEY_OFF_SAMPLES_DONE | FUEL03_FAST_FILL_FLAG |
        FUEL03_INIT_FLAG  ));

    Fuel03SenderA.queue_result =  (FUEL03_BLK1_QUEUED | FUEL03_BLK2_QUEUED);
}

/*============================================================================
//
//  Function Name:      boolean fuel03_keyoff_processing_done( void )
//
//  Visibility:         External
//
//  Description:        A helper function for key off processing.
//
//  Requirments:        Figure 3.6.4-1 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None.
//
//  Output Arguments:   None.
//
//  Critical Section:   None
//
//  Created:            08/27/03 by RRUSS1
//
//  Updated:            03/15/06 by RRUSS1
//                      05/26/08 by CNAGARA1
//
//==========================================================================*/
boolean fuel03_keyoff_processing_done( void )
{
    boolean fuel03_ret_val = FALSE;

    if( FUEL03_VALID_CONDITION() == FALSE )
    {
        fuel03_ret_val = TRUE;
        fuel03_invalid_condition |= FUEL03_KEYOFF_PROCESSING_DONE;
    }
    else
    {
        if( TESTBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_KEY_OFF_SAMPLES_DONE ) )
        {
            fuel03_ret_val = TRUE;
        }
    }
    return( fuel03_ret_val );
}

/*============================================================================
//
//  Function Name:      void fuel03_zss_keyoff_processing( uint16 rada )
//
//  Visibility:         Internal
//
//  Description:        A helper function for key off processing.
//
//  Requirments:        Figure 3.6.4-1 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    rada, radb
//
//  Output Arguments:   None.
//
//  Critical Section:   None
//
//  Created:            05/07/03 by RRUSS1
//
//  Updated:            02/15/05 by RRUSS1    BSDI00016760
//                      02/27/07 by JARUN    BSDI00038410
//==========================================================================*/
static void fuel03_zss_keyoff_processing( uint16 fuel03_raw )
{
    /* ----------------------------------------------------------------------
    ** Set the key off referance values to INVALID.
    ** --------------------------------------------------------------------*/
    Fuel03SenderA.KeyOffRef = FUEL03_PERCENT_FULL_INVALID_BYTE;

    /* ----------------------------------------------------------------------
    ** Check if the ZSS_REF_ONLY option is enabled.
    ** --------------------------------------------------------------------*/
    if( TESTBIT(Fuel03OptionsNVM, FUEL03_ZSS_REF_ONLY_BIT) )
    {
        if( Fuel03ZeroSpeedCounter == 0 )
        {
            SETBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_KEY_OFF_SAMPLES_DONE );
            return;
        }
        /*Fuel03ZeroSpeedCounter--; removed 3/15/06 rruss1 since it caused the
        counter to decrement at twice the rate */

        fuel03_fast_fill_enable_check();

        /* --------------------------------------*/
        /* VALID CONDITION ? Return if not.      */
        /* --------------------------------------*/
        if( FUEL03_VALID_CONDITION() == FALSE )
        {
            fuel03_invalid_condition |= FUEL03_ZSS_KEYOFF_PROCESSING;
        }
        if( FUEL03_VALID_CONDITION() != FALSE )
        {
            Fuel03SenderA.rad = fuel03_raw;                 /* Store, */

            fuel03_average_rad_processing(FUEL03_REF_AVG_TYPE);

            if( TESTBIT(Fuel03SenderA.FilterFlags, FUEL03_SIG_INVALID) )
            {
            }
            else
            {
            	/* ----------------------------------------------------------
            	 ** RFLPS Calc: fig 3.6.4-4
            	 ** Calculate the Reference FLPS.
            	 ** --------------------------------------------------------*/
            	/* rruss1_08_09_04 : Changed the following from rad to avg_rad per new
                   requirments for 'averaging' and put it into both
                   flps and rflps. */
            	Fuel03SenderA.flps = fuel03_rad_to_flps( Fuel03SenderA.avg_rad );
            	Fuel03SenderA.rflps = Fuel03SenderA.flps;

            	/* ----------------------------------------------------------
                    ** Fast Fill Samples: fig 3.6.4-13
                    ** --------------------------------------------------------*/
            	fuel03_fast_fill_samples();
            }

        }
    }
    /* -------------------------------------------------- */
    /* Added the following for BSDI00023218               */
    /* -------------------------------------------------- */
    else
    {
        SETBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_KEY_OFF_SAMPLES_DONE );
    }
}

/*============================================================================
//
//  Function Name:      void fuel03_key_off_sample_processing( void )
//
//  Visibility:         External
//
//  Description:        A helper function for periodic keyoff processing of
//                      the fuel sender(s).
//
//  Requirments:        Section of Figure 3.6.4-1 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    Pointer to sender data.
//
//  Output Arguments:   Various. See code.
//
//  Critical Section:   None
//
//  Created:            07/09/03 by RRUSS1
//
//  Updated:            08/18/04 by RRUSS1 -  Added KO Convergence.
//                      08/25/04 by RRUSS1 -  Added check for FUEL03_REF_AVG_TYPE == 0.
//                      08/10/09 by knagasub  - updated per Fuel03 BDD1.5
//                      06/28/13 by bkoragan - updated as per Fuel03 BDD 14 P42 Nissan v0.2
//==========================================================================*/
static void fuel03_key_off_sample_processing( void )
{			
	NV_Data_Fuel_Level	nv_data_fuel_level; 
	uint16 fl_gauge_fuel_value;
	
	fl_gauge_fuel_value = ffuel_get_calculate_fuel();
	
    if( FUEL03_NOTTESTBIT(Fuel03SenderA.MoreFilterFlags, FUEL03_KEY_OFF_SAMPLES_DONE) )
    {
		if(fl_gauge_fuel_value > EmptyStopFuel && fl_gauge_fuel_value < FullStopFuel)
		{
			//store value when ign off 
			if(Fuel03KeyOffSampleMaxTimer == 1)
			{
				nv_data_fuel_level.FuelLevelValue = fl_gauge_fuel_value ;			//Parameter A	
				(void)Rte_Write_pprpNV_FuelLevelMdl_FuelLevel_FuelLevelValue(&nv_data_fuel_level);	
			}			

			//store value when ign off >= 10s (100ms/cycle)
			if(Fuel03KeyOffSampleMaxTimer == 100)
			{	
				nv_data_fuel_level.FuelLevelValue = fl_gauge_fuel_value ;			//Parameter A	
				(void)Rte_Write_pprpNV_FuelLevelMdl_FuelLevel_FuelLevelValue(&nv_data_fuel_level);	
			}
		}
    }
}

/*============================================================================
//
//  Function Name:      void fuel03_keyoff_processing(uint16 rada)
//
//  Visibility:         External
//
//  Description:        A function to handle periodic keyoff processing of
//                      the fuel sender(s).
//
//  Requirments:        Figure 3.6.4-1 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    rada - latest ratioed fuel sample for the A sender
//
//  Output Arguments:   TRUE if keyoff processing finished, else FALSE.
//
//  Critical Section:   None
//
//  Created:            07/09/03 by RRUSS1
//
//  Updated:            05/10/13 by BKORAGAN
//
//==========================================================================*/
void fuel03_keyoff_processing(uint16 fuel03_rada )
{
    fuel03_ignition_status = FUEL03_KEYOFF;

    fuel03_ign_off_on_fill_flag = FALSE;

    /* ----------------------------------------------------------------------
    ** HILL_HOLD_ENABLE = 1 ?
    ** --------------------------------------------------------------------*/
    if( FUEL03_NOTTESTBIT(Fuel03OptionsNVM, FUEL03_HILL_HOLD_ENABLE_BIT) )
    {
        fuel03_zss_keyoff_processing( fuel03_rada );
        return;
    }


    #ifdef NISSAN_CHINA
    if(Fuel03KeyOffSampleMaxTimer < Fuel03KeyOffSampleTimeNVM)
    {
        Fuel03SenderA.KeyOffRef = FUEL03_PERCENT_FULL_INVALID_BYTE;
    }
    #endif

	#if 0
    /* ----------------------------------------------------------------------
    ** A Fuel KeyOffRef = 0xFF ?
    ** --------------------------------------------------------------------*/\
    if(Fuel03SenderA.KeyOffRef != FUEL03_PERCENT_FULL_INVALID_BYTE)
    {
       SETBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_KEY_OFF_SAMPLES_DONE );
       return;
    }
	#endif

    /* ----------------------------------------------------------------------
    ** Check if the max time to execute after key off has expired.
    ** --------------------------------------------------------------------*/
    if (Fuel03KeyOffSampleMaxTimer >= Fuel03KeyOffSampleTimeNVM)
    {
        SETBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_KEY_OFF_SAMPLES_DONE );

        return;
    }
    Fuel03KeyOffSampleMaxTimer++;



    /* ----------------------------------------
    ** VALID CONDITION ?
    ** --------------------------------------*/
    if( FUEL03_VALID_CONDITION() == FALSE )
    {
        fuel03_invalid_condition |= FUEL03_KEYOFF_PROCESSING_2;
    }
    if( FUEL03_VALID_CONDITION() != FALSE )
    {
        Fuel03SenderA.rad = fuel03_rada;                      /* Store, */
        fuel03_average_rad_processing(FUEL03_REF_AVG_TYPE);
        fuel03_key_off_sample_processing();

        return;
    }

    return;
}

/*============================================================================
//
//  Function Name:      void fuel03_periodic_processing(uint16 fuel03_rada,
//                                                      boolean fuel03_jam_filter)
//
//  Visibility:         External
//
//  Description:        This function handles fuel inits and normal/fast mode
//                      filtering of the fuel sender(s).
//
//  Requirments:        Figure 3.6.4-2 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    rada - latest ratioed fuel sample for the SENDER A module.
//                      jam_filter - TRUE if filter(s) should be jammed with
//                                   the latest sample(s)
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            03/04/20 by sli34
//
//  Updated:            05/14/20 by sli34
//
//==========================================================================*/
boolean fuel03_periodic_processing(uint16 fuel03_raw, boolean fuel03_jam_filter)
{
    /* Clear Key Off Sample Timer when IGN ON	*/
    Fuel03KeyOffSampleMaxTimer = 0;

    /* ----------------------------------------------------------------------
    ** FUEL KEY ON DELAY COUNTER = 0 ?
    ** ----------------------------------------------------------------------
    ** After the ignition switch transitions, wait a specific amount of
    ** time before taking initialization samples.
    ** --------------------------------------------------------------------*/
    if( Fuel03KeyOnDelayCounter != 0 )
    {
        Fuel03KeyOnDelayCounter--;
        Fuel03InitDelayCounter = Fuel03InitDelayCounterNVM;
        return FALSE;
    }

    /* ----------------------------------------------------------------------
    ** A Fuel Init = 1 ?
    ** ----------------------------------------------------------------------
    ** Take samples until the fuel init period has expired, and then do
    ** a one-time fuel init.
    ** --------------------------------------------------------------------*/
    if( FUEL03_NOTTESTBIT(Fuel03SenderA.MoreFilterFlags, FUEL03_INIT_FLAG) )
    {
        if( Fuel03InitDelayCounter != 0 )		//Fuel03InitDelayCounter ==100ms(1 * 100ms)
        {
            /* ----------------------------------------------------------------------
            ** Count down the timer for IG off to ON delay (100ms)
            ** --------------------------------------------------------------------*/
            Fuel03InitDelayCounter--;

            /* ----------------------------------------
            ** VALID CONDITION ?
            ** --------------------------------------*/
            if( FUEL03_VALID_CONDITION() == FALSE )
            {
                fuel03_invalid_condition |= FUEL03_PERIODIC_PROCESSING_1;
            }

            SETBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_IG_ON_ENTERY_FLAG );
            CLEARBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_SIG_MIS_INVALID_RECOV );
        }
        else
        {
            if( FUEL03_NOTTESTBIT(Fuel03SenderA.MoreFilterFlags, FUEL03_INIT_FLAG) )
            {
                fuel03_filter_initialize();  /* Fuel Init Processing Fig 3.6.4-10           */
            }
        }
        return FALSE;
    }
    else   /* fuel init finished */
    {

        /* ----------------------------------------
        ** VALID CONDITION ? 1. Power Abnormal?
        ** --------------------------------------*/
        if( FUEL03_VALID_CONDITION() == FALSE )
        {
            fuel03_invalid_condition |= FUEL03_PERIODIC_PROCESSING_2;
            return FALSE;
        }

        Fuel03SenderA.rad = fuel03_raw;

        fuel03_average_rad_processing(FUEL03_AVG_TYPE);							/* processing average data			*/

        /*--------------------------------------------------------
        ** Check IGN OFF to ON Parameter A/B determine
		--------------------------------------------------------*/
        if( TESTBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_IG_ON_ENTERY_FLAG ) )
        {
        	fuel03_ign_off_on_fill_determination();								/* check ign on fast fill condition	*/

        	Fuel03SenderA.flps = fuel03_rad_to_flps( Fuel03SenderA.avg_rad );
        	
        	CLEARBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_IG_ON_ENTERY_FLAG );

        	fuel03_slosh_filter( TRUE );
        	
        }
        /*--------------------------------------------------------
        ** Check IGN ON Fast Fill
		--------------------------------------------------------*/
        else
        {
	        if(FUEL03_FAST_FILL_MET_MACRO() == FALSE)		//Car is running
			{
				fuel03_ign_off_on_fill_flag = FALSE;
				
				Rte_Write_ppSR_TIFuel_RefuelState_TIFuel_RefuelState(FALSE);
			}


			if(l_fuel_abnormal_status == TRUE)
			{
				Fuel03SenderA.flps = fuel03_rad_to_flps( Fuel03SenderA.avg_rad );
				
				fuel03_slosh_filter( TRUE );
				
				l_fuel_abnormal_status = FALSE;
			}
        	else
        	{
				Fuel03SenderA.flps = fuel03_rad_to_flps( Fuel03SenderA.avg_rad );

				fuel03_fast_fill_enable_check();   										/* Fast FIll Enable Check Figure 3.6.4-11     */

            	fuel03_fast_fill_determination();  										/* Fast FIll Figure 3.6.4-12                  */
				
				fuel03_slosh_filter( FALSE );
        	}
        	
			Fuel03SenderA.IgnOnRef = (FUEL03_MS_16_BITS(Fuel03SenderA.slosh_filter_hys)) >> 8;	/* Using slosh_filter as the compare base value for fast fill determine */
        }
        
		return TRUE;
    }
}

/*============================================================================
//
//  Function Name:      uint8 fuel03_rad_to_flps(uint16 fuel03_rad)
//
//  Visibility:         Internal
//
//  Description:        Routine to calculate the fuel percent full.
//
//  Requirments:        Figure 3.6.4-5 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    *fuel03_flt - fuel filter structure pointer,
//                      fuel03_rad - latest RAD
//
//  Output Arguments:   1 byte percent full value.
//
//  Critical Section:   None
//
//  Created:            07/23/03 by RRUSS1
//
//  Updated:            04/10/06 by RRUSS1    BSDI00030114
//
//==========================================================================*/
static uint8 fuel03_rad_to_flps(uint16 fuel03_rad)
{
    /* --------------------------------------------------------------------
    ** Use the linear interpolation algorithm to convert from A/D to percent
    ** full.
    ** ------------------------------------------------------------------*/
    Fuel03PercentFull = LinearInterpolateWord((void *)&Fuel03Sender_A_TableNVM[0][0], fuel03_rad);

    return(FUEL03_MS_8_BITS(Fuel03PercentFull));
}

#if 0
/*============================================================================
//
//  Function Name:      void fuel03_init_avgflps_calc(void)
//
//  Visibility:         Internal
//
//  Description:        Perform the one time fuel filter initialization after
//                      the appropriate time.
//
//  Requirments:        Figure 3.6.4-7 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None.
//
//  Output Arguments:   Fuel filter RAM updated if necessary.
//
//  Critical Section:   None
//
//  Created:            07/10/03 by RRUSS1
//
//  Updated:            07/24/03 by RRUSS1
//
//==========================================================================*/
static void fuel03_init_avgflps_calc(void)
{
    /****************/
    /* (1) Sender A */
    /****************/


        /* -------------------------------------------------------------
               ** (14) Cold Init flag = 1 ?
               ** -------------------------------------------------------------
               ** Note that (8)(9)(10) are not evaluated here because we are
               ** just doing A at this time.
               ** -----------------------------------------------------------*/
        if (TESTBIT(Fuel03Flags, FUEL03_COLD_INIT))
        {
            /* -------------------------------------------------------------
                      ** (15) Set x Force Fuel Init flag = 1
                      ** -----------------------------------------------------------*/
            SETBIT(Fuel03SenderA.MoreFilterFlags, FUEL03_FORCE_FUEL_INIT_FLAG);
        }

        /* -------------------------------------------------------------
               ** (16) x Fuel Init = 1
               ** -----------------------------------------------------------*/
        SETBIT(Fuel03SenderA.MoreFilterFlags, FUEL03_INIT_FLAG);

        /* -------------------------------------------------------------
               ** (17) INIT_PID_x = 0x01, 0xFF
               ** -----------------------------------------------------------*/
        Fuel03SenderA.init_pid = 0x01ff;


        /* -------------------------------------------------------------
               ** (6) AVG RAD x to AVGFLPS x
               ** -----------------------------------------------------------*/
        Fuel03SenderA.keyon_avgflps = fuel03_rad_to_flps((uint16)Fuel03SenderA.rad);

        /* -------------------------------------------------------------
               ** (7) AVGFLPS SADDLE x = MSB of AVGFLPS
               ** -----------------------------------------------------------*/
        Fuel03SenderA.avgflps_saddle = Fuel03SenderA.keyon_avgflps;

}

/*============================================================================
//
//  Function Name:      void fuel03_ref_fuel_calc( void )
//
//  Visibility:         Internal
//
//  Description:        A function to calculate the weighted average
//                      for sender A and B.
//
//  Requirments:        Figure 3.6.4-9 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None.
//
//  Output Arguments:   None.
//
//  Critical Section:   None
//
//  Created:            07/10/03 by RRUSS1
//
//  Updated:            03/03/05 by RRUSS1
//
//==========================================================================*/
static void fuel03_ref_fuel_calc( void )
{
    Fuel03SenderA.rfuel = (uint8)(Fuel03SenderA.slosh_filter >> 24u);
}
#endif

/*============================================================================
//
//  Function Name:      void fuel03_finit_t3( void )
//
//  Visibility:         Internal
//
//  Description:        Helper function for Fuel Init.
//
//  Requirments:        Figure 3.6.4-11 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None.
//
//  Output Arguments:   None.
//
//  Critical Section:   None
//
//  Created:            07/23/03 by RRUSS1
//
//  Updated:            11/21/03 by RRUSS1
//
//==========================================================================*/
static void fuel03_finit_t3( void )
{

#if (FUEL03_REF_AVG_TYPE==4)
	SETBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_RE_INIT_FLAG );
#endif
	/* -------------------------------------------------------------
	** (17) x Force Fuel Init flag = 0
	** -----------------------------------------------------------*/
	CLEARBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_FORCE_FUEL_INIT_FLAG );

	/* -------------------------------------------------------------
	** (18) SLOSH FILT x = AVGFLPS x MSB
	** -----------------------------------------------------------*/
	Fuel03SenderA.slosh_filter = (uint32)Fuel03SenderA.keyon_avgflps << 24u;

	/* -------------------------------------------------------------
	** (19) FILT HYS x = AVGFLPS x MSB
	** -----------------------------------------------------------*/
	Fuel03SenderA.slosh_filter_hys = Fuel03SenderA.slosh_filter; /* save some code instead of using
	(uint32)Fuel03SenderA.keyon_avgflps << 24u; */

}

/*============================================================================
//
//  Function Name:      void fuel03_finit_F3( void )
//
//  Visibility:         Internal
//
//  Description:        Helper function for Fuel Init.
//
//  Requirments:        Figure 3.6.4-11 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None.
//
//  Output Arguments:   None.
//
//  Critical Section:   None
//
//  Created:            08/31/04 by RRUSS1
//
//  Updated:            08/31/04 by RRUSS1
//
//==========================================================================*/
static void fuel03_finit_F3( void )
{
    uint8 fuel03_check_value;

    fuel03_check_value = Fuel03SecPosInitFillDeltaNVM_A;

    if( (Fuel03SenderA.keyon_avgflps - Fuel03SenderA.rfuel) > fuel03_check_value )
    {
        SETBIT( Fuel03SenderA.FilterFlags, FUEL03_FILL_DETECTED );

        Fuel03SenderA.init_pid |= (uint16)0x8000u;

        Fuel03SenderA.log_nvm_data = TRUE;

        fuel03_finit_t3();

        return;
    }

    fuel03_check_value = Fuel03SecNegInitFillDeltaNVM_A;
    if( (Fuel03SenderA.rfuel - Fuel03SenderA.keyon_avgflps) > fuel03_check_value )
    {
        SETBIT( Fuel03SenderA.FilterFlags, FUEL03_LOSS_DETECTED );

        Fuel03SenderA.init_pid |= (uint16)0x8000u;

        Fuel03SenderA.log_nvm_data = TRUE;

        fuel03_finit_t3();

        return;
    }
    else
    {
        CLEARBIT( Fuel03SenderA.FilterFlags, FUEL03_LOSS_DETECTED );
        CLEARBIT( Fuel03SenderA.FilterFlags, FUEL03_FILL_DETECTED );
    }
}

/*============================================================================
//
//  Function Name:      void fuel03_fi_compare( void )
//
//  Visibility:         Internal
//
//  Description:        Helper function for Fuel Init.
//
//  Requirments:        Figure 3.6.4-11 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None.
//
//  Output Arguments:   None.
//
//  Critical Section:   None
//
//  Created:            07/23/03 by RRUSS1
//
//  Updated:            11/19/03 by RRUSS1
//
//==========================================================================*/
static void fuel03_fi_compare( void )
{
    uint8 LocalInitFill;

    /* --------------------------------------------------------------------
        ** (22) (INIT REF x MSB - AVGFLPS x MSB) > NEGATIVE_INIT_FILL_DELTA_x ?
        ** ------------------------------------------------------------------*/
    if( Fuel03SenderA.init_ref > Fuel03SenderA.keyon_avgflps )
    {
        LocalInitFill = Fuel03NegativeInitFill_A_NVM;


        if( (Fuel03SenderA.init_ref - Fuel03SenderA.keyon_avgflps) > LocalInitFill )
        {
            /* -------------------------------------------------------------
                      ** (23) x Fuel Lose Detected Flag = 1
			  ** -----------------------------------------------------------*/
            SETBIT( Fuel03SenderA.FilterFlags, FUEL03_LOSS_DETECTED );

            fuel03_finit_t3();

            return;
        }
    }
    else /* N part of (22) */
    {
     /* --------------------------------------------------------------------
        ** (24) (AVGFLPS x MSB - INIT REF x MSB) > POSITIVE_INIT_FILL_DELTA_x ?
        **      or
        ** (25) ( RFUEL x > RENTAL_CAR_FILL_THRESHOLD_x
        **      and
        ** (26)   (AVGFLPS x MSB - INIT REF x MSB) > RENTAL_CAR_FILL_DELTA_x ? )
        ** ------------------------------------------------------------------*/
        LocalInitFill = Fuel03PositiveInitFill_A_NVM;

        if( ( (Fuel03SenderA.keyon_avgflps - Fuel03SenderA.init_ref) > LocalInitFill) ||
            ( (Fuel03SenderA.rfuel > Fuel03SenderA.rc_fill_threshold) &&
            ((Fuel03SenderA.keyon_avgflps - Fuel03SenderA.init_ref) > Fuel03SenderA.rc_fill_delta)) )
        {
            /* -------------------------------------------------------------
                     ** (27) x Fuel Fill Detected Flag = 1
                     ** -----------------------------------------------------------*/
            SETBIT( Fuel03SenderA.FilterFlags, FUEL03_FILL_DETECTED );

            fuel03_finit_t3();

            return;
        }
    }

    fuel03_finit_F3();

}

/*============================================================================
//
//  Function Name:      void fuel03_fi_check( void )
//
//  Visibility:         Internal
//
//  Description:        Helper function for Fuel Init.
//
//  Requirments:        Figure 3.6.4-11 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None.
//
//  Output Arguments:   None.
//
//  Critical Section:   None
//
//  Created:            07/23/03 by RRUSS1
//
//  Updated:            05/10/13 by BKORAGAN
//
//==========================================================================*/
static void fuel03_fi_check( void )
{
	/* -------------------------------------------------------------
	** (3) x Force Fuel Init flag = 1?
	** -----------------------------------------------------------*/
	if( TESTBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_FORCE_FUEL_INIT_FLAG ) )
	{
		/* -------------------------------------------------------------
		** (4)(28) INIT_PID_x = 03,0xff
		** -----------------------------------------------------------*/
		Fuel03SenderA.init_pid = 0x03ff;

		/* -------------------------------------------------------------
		** (27) x Fuel FIll Detected Flag = 1
		** -----------------------------------------------------------*/
		SETBIT( Fuel03SenderA.FilterFlags, FUEL03_FILL_DETECTED );

		fuel03_finit_t3();

		return;

	}

	/* -------------------------------------------------------------
	**    RE-Init flag = 1?
	** -----------------------------------------------------------*/
	if( TESTBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_RE_INIT_FLAG ) )
	{
		/* -------------------------------------------------------------
		** (4)(28) INIT_PID_x = 0f,0xff
		** -----------------------------------------------------------*/
		Fuel03SenderA.init_pid = 0x0fff;

		/* -------------------------------------------------------------
		** (27) x Fuel FIll Detected Flag = 1
		** -----------------------------------------------------------*/
		SETBIT( Fuel03SenderA.FilterFlags, FUEL03_FILL_DETECTED );


		fuel03_finit_t3();

		return;
	}

	/* -------------------------------------------------------------
	** (8) KEY OFF REF x = 0xFF ?
	** -----------------------------------------------------------*/
	if( Fuel03SenderA.KeyOffRef != FUEL03_PERCENT_FULL_INVALID_BYTE )
	{
		/* -------------------------------------------------------------
		** (9) INIT REF x = KEY OFF REF x
		** -----------------------------------------------------------*/
		Fuel03SenderA.init_ref = Fuel03SenderA.KeyOffRef;

		/* -------------------------------------------------------------
		** (10) INIT_PID_x = 05, xx xx = ZSS REF x
		** -----------------------------------------------------------*/
		Fuel03SenderA.init_pid = 0x0500 + (uint16)Fuel03SenderA.KeyOffRef;

		fuel03_fi_compare();

		return;
	}

	/* -------------------------------------------------------------
	** (5) ZSS REF x = 0xFF ?
	** -----------------------------------------------------------*/
	if( Fuel03SenderA.zss_ref != FUEL03_PERCENT_FULL_INVALID_BYTE )
	{
		/* -------------------------------------------------------------
		** (9) INIT REF x = ZSS REF x
		** -----------------------------------------------------------*/
		Fuel03SenderA.init_ref = Fuel03SenderA.zss_ref;

		/* -------------------------------------------------------------
		** (10) INIT_PID_x = 04, xx xx = ZSS REF x
		** -----------------------------------------------------------*/
		Fuel03SenderA.init_pid = 0x0400 + (uint16)Fuel03SenderA.zss_ref;

		fuel03_fi_compare();

		return;
	}

	/* -------------------------------------------------------------
	** (11) HILL_HOLD_ENABLE = 1 ?
	** -----------------------------------------------------------*/
	if( TESTBIT( Fuel03OptionsNVM, FUEL03_HILL_HOLD_ENABLE_BIT ) )
	{
		#if 0
        #if (FUEL03_REF_AVG_TYPE==4)
		if( Fuel03SenderA.koc_fault_flag != FALSE )

		{
			if( Fuel03SenderA.koc_sender_fault_flag != FALSE )
        #endif
			{
				/* -------------------------------------------------------------
				** (14) x Fuel FIll Detected Flag = 1
				** -----------------------------------------------------------*/
				SETBIT( Fuel03SenderA.FilterFlags, FUEL03_FILL_DETECTED );

				Fuel03SenderA.init_pid = 0x02ff;

				fuel03_finit_t3();

				return;
			}

        #if (FUEL03_REF_AVG_TYPE==4)
			else
			{
				Fuel03SenderA.init_pid  = 0x07ff;

				fuel03_finit_F3();
				return;

			}
		}
		else
		{
			Fuel03SenderA.init_pid  = 0x07ff;

			fuel03_finit_F3();
			return;

		}
        #endif

//		#else
#endif
		Fuel03SenderA.init_pid  = 0x07ff;

		fuel03_finit_F3();

		return;
//	   #endif

	}
	else
	{
		/* -------------------------------------------------------------
		** (12) INIT REF x = RFUEL x
		** -----------------------------------------------------------*/
		Fuel03SenderA.init_ref = Fuel03SenderA.rfuel;

		/* -------------------------------------------------------------
		** (13) INIT_PID_x = 06, RFUEL x
		** -----------------------------------------------------------*/
		Fuel03SenderA.init_pid = 0x0600 + (uint16)Fuel03SenderA.rfuel;

		fuel03_fi_compare();

		return;
	}

}

/*============================================================================
//
//  Function Name:      void fuel03_filter_initialize( void )
//
//  Visibility:         Internal
//
//  Description:        Fuel filter initialization function.
//
//  Requirments:        Figure 3.6.4-11 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None.
//
//  Output Arguments:   None.
//
//  Critical Section:   None
//
//  Created:            07/23/03 by RRUSS1
//
//  Updated:            07/26/04 by RRUSS1
//
//==========================================================================*/
static void fuel03_filter_initialize( void )
{
    /* -------------------------------------------------------------
    ** (2) Cold init flag = 0
    ** -----------------------------------------------------------*/
    CLEARBIT( Fuel03Flags, FUEL03_COLD_INIT );

    /* -------------------------------------------------------------
    ** (1) Tank Processing = A
    ** -----------------------------------------------------------*/
    if( FUEL03_NOTTESTBIT(Fuel03SenderA.MoreFilterFlags, FUEL03_INIT_FLAG))
    {
        fuel03_fi_check();
        /* -------------------------------------------------------------
        ** x Fuel Init = 1
        ** -----------------------------------------------------------*/
        SETBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_INIT_FLAG );

    }
}
/*============================================================================
//
//  Function Name:      void fuel03_fast_fill_enable_check(void)
//
//  Visibility:         Internal
//
//  Description:        Fast fill enable check.
//
//  Requirments:        Figure 3.6.4-12 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            05/08/03 by RRUSS1
//
//  Updated:            07/09/12 by BKORAGAN
//
//==========================================================================*/
static void fuel03_fast_fill_enable_check( void )
{

	if( TESTBIT(Fuel03OptionsNVM, FUEL03_FAST_FILL_ENABLE_BIT))
	{
		/* -------------------------------------------------------------
		** Check if the fast fill conditions have been met. The
		** application is responsible for defining what they are and
		** returns FALSE if not.
		** -----------------------------------------------------------*/
		if( FUEL03_FAST_FILL_MET_MACRO() != FALSE )
		{
			if( Fuel03ZeroSpeedCounter != 0 )
			{
				Fuel03ZeroSpeedCounter--;
				return;
			}
			else
			{
				if( TESTBIT(Fuel03SenderA.FilterFlags, FUEL03_SIG_INVALID) )
				{
					  /*
					  ** Load fdelta from NVM
					  */
	                  Fuel03SenderA.fdelta = FUEL03_DELTA_NORMAL_BASE_A_NVM;
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			if( Fuel03ZeroSpeedCounter == 0 )
			{
				/* -------------------------------------------------------------
				** FDELTA A = FDELTA BASE A
				** FDELTA B = FDELTA BASE B
				** -----------------------------------------------------------*/
				/*
				** Load fdelta from NVM
				*/
              Fuel03SenderA.fdelta = FUEL03_DELTA_NORMAL_BASE_A_NVM;
			}
		}
	}

	Fuel03ZeroSpeedCounter = Fuel03ZeroSpeedCounterNVM;

	Fuel03MinFastFillTimeCounter = Fuel03FastFillTimeCounterNVM;		//20s

	Fuel03SenderA.zss_ref = FUEL03_PERCENT_FULL_INVALID_BYTE;

	CLEARBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_FAST_FILL_FLAG );
}

/*============================================================================
//
//  Function Name:      void fuel03_fast_fill_determination( void )
//
//  Visibility:         Internal
//
//  Description:        Fast fill enable check.
//
//  Requirments:        Figure 3.6.4-14 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            07/11/03 by RRUSS1
//
//  Updated:            05/10/13 by BKORAGAN
//
//==========================================================================*/
static void fuel03_ign_off_on_fill_determination(void)
{
	uint16 fl_parameter_A;
	uint16 fl_parameter_B;
	uint16 fl_parameter_C;

	fl_parameter_A = ffuel_get_parameter_A_fuel();			 //Parameter A
	fl_parameter_B = Fuel03SenderA.avg_rad;					 //Parameter B
	fl_parameter_C = ffuel_get_parameter_C_fuel();			 //Parameter C

	/********   IGN ON, |B-C| >= 4L  ? *********/
	if( fl_parameter_B > fl_parameter_C )
	{
		if( (fl_parameter_B - fl_parameter_C) >= 400 )
		{
			
		}
		else
		{
			if(fl_parameter_C > fl_parameter_A)
			{
				if(fl_parameter_C - fl_parameter_A > 800)
				{
					Fuel03SenderA.avg_rad = fl_parameter_C;
				}
				else
				{
					Fuel03SenderA.avg_rad = fl_parameter_A;
				}
			}
			else
			{
				if(fl_parameter_A - fl_parameter_C > 800)
				{
					Fuel03SenderA.avg_rad = fl_parameter_C;
				}
				else
				{
					Fuel03SenderA.avg_rad = fl_parameter_A;
				}
			}		
		}
	}
	else
	{
		if( (fl_parameter_C - fl_parameter_B) >= 400 )
		{
			
		}
		else
		{
			if(fl_parameter_C > fl_parameter_A)
			{
				if(fl_parameter_C - fl_parameter_A > 800)
				{
					Fuel03SenderA.avg_rad = fl_parameter_C;
				}
				else
				{
					Fuel03SenderA.avg_rad = fl_parameter_A;
				}
			}
			else
			{
				if(fl_parameter_A - fl_parameter_C > 800)
				{
					Fuel03SenderA.avg_rad = fl_parameter_C;
				}
				else
				{
					Fuel03SenderA.avg_rad = fl_parameter_A;
				}
			}		
		}
	}

	#if 0
	Fuel03SenderA.KeyOffRef = ffuel_get_parameter_C_fuel(); 		 //Parameter C
	
	/********	IGN ON, |B-C| >= 4L  ? *********/
	if( Fuel03SenderA.avg_rad > Fuel03SenderA.KeyOffRef )
	{
		if( (Fuel03SenderA.avg_rad - Fuel03SenderA.KeyOffRef) >= 400 )
		{
			fuel03_ign_off_on_fill_flag = TRUE;
		}
		else
		{
			Fuel03SenderA.avg_rad = ffuel_get_parameter_A_fuel();		//Parameter A

			fuel03_ign_off_on_fill_flag = FALSE;
		}
	}
	else
	{
		if( (Fuel03SenderA.KeyOffRef - Fuel03SenderA.avg_rad) >= 400 )
		{
			fuel03_ign_off_on_fill_flag = TRUE;
		}
		else
		{
			Fuel03SenderA.avg_rad = ffuel_get_parameter_A_fuel();		//Parameter A

			fuel03_ign_off_on_fill_flag = FALSE;
		}
	}
	#endif

//	Rte_Write_ppSR_TIFuel_RefuelState_TIFuel_RefuelState(fuel03_ign_off_on_fill_flag);
}

boolean fuel03_ign_off_on_fill_notification(void)
{
	return fuel03_ign_off_on_fill_flag;
}

void fuel03_ign_off_on_fill_notification_clr(void)
{
	fuel03_ign_off_on_fill_flag = FALSE;
}

/*============================================================================
//
//  Function Name:      void fuel03_fast_fill_determination( void )
//
//  Visibility:         Internal
//
//  Description:        Fast fill enable check.
//
//  Requirments:        Figure 3.6.4-14 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            07/11/03 by RRUSS1
//
//  Updated:            05/10/13 by BKORAGAN
//
//==========================================================================*/
static void fast_fill_determination_f1( void )
{
    if( TESTBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_FAST_FILL_FLAG ) )
    {
    }
    else
    {
        /* Check if FUEL03_MIN_FAST_FILL_TIME elasped */
        if( Fuel03MinFastFillTimeCounter == 0)
        {
            SETBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_FAST_FILL_FLAG );

            Fuel03SenderA.fdelta_base = FUEL03_DELTA_FAST_BASE_A_NVM;

            Fuel03SenderA.fdelta = Fuel03SenderA.fdelta_base;
            
			Rte_Write_ppSR_TIFuel_RefuelState_TIFuel_RefuelState(TRUE);
			
			fuel03_ign_off_on_fill_flag = TRUE;
			
			fuel03_fastfill_flag = TRUE;
        }
        else
        {
            Fuel03MinFastFillTimeCounter--;					//delay 20s
        }
    }
}


static void fuel03_fast_fill_determination( void )
{
    uint8 delta_ref;
    uint8 fl_Pos_Fast_Fill_Delta;
    uint8 fl_Neg_Fast_Fill_Delta;

    /* -------------------------------------------------------------
    ** Check if the fast fill conditions have been met. The
    ** -----------------------------------------------------------*/
    fl_Pos_Fast_Fill_Delta = Fuel03PositiveInitFill_A_NVM;
    fl_Neg_Fast_Fill_Delta = Fuel03NegativeInitFill_A_NVM;
    delta_ref = Fuel03SenderA.IgnOnRef;

    /* -------------------------------------------------------------
     ** Check for a positive fill delta.
     ** -----------------------------------------------------------*/
    if( Fuel03SenderA.flps >= delta_ref )
    {
    	if ( (Fuel03SenderA.flps - delta_ref) >= fl_Pos_Fast_Fill_Delta )	//target value - indicated value >=4L
    	{
    		fast_fill_determination_f1();		//SysRS_02_Gauges_1258     > 20S

    		Fuel03FastFillLatencyTimeCounter = FUEL03_MIN_FAST_FILL_LATENCY_TIME;
    		return;
    	}
    }

    /* -------------------------------------------------------------
     ** Check for a negative fill delta.
     ** -----------------------------------------------------------*/
    if( delta_ref >= Fuel03SenderA.flps )
    {
    	if ( (delta_ref - Fuel03SenderA.flps) >= fl_Neg_Fast_Fill_Delta )
    	{
    		fast_fill_determination_f1();

    		Fuel03FastFillLatencyTimeCounter = FUEL03_MIN_FAST_FILL_LATENCY_TIME;
    		return;
    	}
    }

    if( Fuel03SenderA.flps == delta_ref )
    {
    	if( Fuel03FastFillLatencyTimeCounter >= FUEL03_MIN_FAST_FILL_LATENCY_TIME )
    	{
    		Fuel03FastFillLatencyTimeCounter -- ;
    	}
    }

    if( ( Fuel03FastFillLatencyTimeCounter < FUEL03_MIN_FAST_FILL_LATENCY_TIME) && (Fuel03FastFillLatencyTimeCounter > 0) )
    {
    	Fuel03FastFillLatencyTimeCounter --;

    	if( Fuel03FastFillLatencyTimeCounter == 0)
    	{
    		CLEARBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_FAST_FILL_FLAG );
    		Rte_Write_ppSR_TIFuel_RefuelState_TIFuel_RefuelState(FALSE);
    		fuel03_ign_off_on_fill_flag = FALSE;
    	}

    	return;		/* Stay FAST FILL Status until Fuel03FastFillLatencyTimeCounter equal to 0 */
    }

#if 0	/* Using flps as fastfill compare factor	*/
    /* -------------------------------------------------------------
     ** Check for a positive fill delta.
     ** -----------------------------------------------------------*/
    if( Fuel03SenderA.avg_rad >= delta_ref )
    {
    	if ( (Fuel03SenderA.avg_rad - delta_ref) > fl_Pos_Fast_Fill_Delta )
    	{
    		fast_fill_determination_f1();
    		return;
    	}
    }

    /* -------------------------------------------------------------
     ** Check for a negative fill delta.
     ** -----------------------------------------------------------*/
    if( delta_ref >= Fuel03SenderA.avg_rad )
    {
    	if ( (delta_ref - Fuel03SenderA.avg_rad) > fl_Neg_Fast_Fill_Delta )
    	{
    		fast_fill_determination_f1();
    		return;
    	}
    }
#endif
    /* ----------------------------------------------------------------------
    ** Set the Min fast fill time counter to default value.
    ** --------------------------------------------------------------------*/
    Fuel03MinFastFillTimeCounter = Fuel03FastFillTimeCounterNVM;		//20s
}

/*============================================================================
//
//  Function Name:      void fuel03_fast_fill_samples( void )
//
//  Visibility:         Internal
//
//  Description:        Fast fill samples.
//
//  Requirments:        Figure 3.6.4-15 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            05/08/03 by RRUSS1
//
//  Updated:            05/08/03 by RRUSS1
//                      02/27/07 by JARUN for BSDI00038410
//==========================================================================*/
static void fuel03_fast_fill_samples( void )
{

}

/*============================================================================
//
//  Function Name:      void fuel03_slosh_filter(boolean fuel03_live_mode )
//
//  Visibility:         Internal
//
//  Description:        Routine to filter slosh from the fuel sender.
//                      error recovery or not.
//
//  Requirments:        Figure 3.6.4-17 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    fuel03_flt - fuel filter pointer
//                      fuel03_live_mode - TRUE: jam filter, FALSE: do not
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            07/17/03 by RRUSS1
//
//  Updated:            07/10/12 by BKORAGAN
//
//==========================================================================*/
static void fuel03_slosh_filter(boolean fuel03_live_mode )
{
//    uint32 fuel03_local_clamp;
    uint16 LocalFullPos16;
	uint32 fuel03_slosh_filter_limit;
	fuel03_slosh_filter_limit =0xFEFFFFFFu;

    if( fuel03_live_mode == 1 )
    {
    	/* Not used	*/
        Fuel03SenderA.slosh_filter = ((uint32)Fuel03SenderA.flps) << 24u;
        Fuel03SenderA.slosh_filter_hys = Fuel03SenderA.slosh_filter;
    }
    else
    {
    	#if 0
		 if( Fuel03PercentFull >= ((uint16)(Fuel03SenderA.slosh_filter >> 16u)))
		 {
			Fuel03SenderA.fdelta_counter=0;
		 }
		 else if( Fuel03PercentFull < ((uint16)(Fuel03SenderA.slosh_filter >> 16u))) 
		 {
			Fuel03SenderA.fdelta_counter++;
		 }

		test_count = Fuel03SenderA.fdelta_counter;
		#endif
    
        /* -------------------------------------------------------------
        ** Check the fast fill flag (x Fast Fill flag = 1?)
        ** -----------------------------------------------------------*/
        if( TESTBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_FAST_FILL_FLAG ) )
        {
            Fuel03SenderA.fdelta_base = FUEL03_DELTA_FAST_BASE_A_NVM;
            Fuel03SenderA.fdelta_adjust_base = FUEL03_DELTA_FAST_ADJUST_A_NVM;
        }
        else
        {
			/*
			** Load fdelta, fdelta_base and fdelta_adjust_base from NVM
			*/
            Fuel03SenderA.fdelta_base = FUEL03_DELTA_NORMAL_BASE_A_NVM;
            //Fuel03SenderA.fdelta_base = FUEL03_DELTA_NORMAL_BASE_A_NVM + FUEL03_DELTA_NORMAL_BASE_B_NVM * Fuel03SenderA.fdelta_counter;
            Fuel03SenderA.fdelta_adjust_base = FUEL03_DELTA_NORMAL_ADJUST_A_NVM;
        }
	
        //if(Fuel03SenderA.fdelta_counter == 100)
		// 	Fuel03SenderA.fdelta_counter = 99;

        /* -------------------------------------------------------------
        ** FLPS x > SLOSH FILT x MSB ?
        ** -------------------------------------------------------------
        ** If the current sample is above the filter, check if the
        ** previous sample was above as well.  If so, add an adjustment
        ** factor to the delta to be added to the filter.  Similarly,
        ** if the current and previous samples were both below the
        ** filter, add the adjustment factor.  Make sure to clamp the
        ** delta at the limits from the config file.
        ** -----------------------------------------------------------*/
        if( Fuel03PercentFull > ((uint16)(Fuel03SenderA.slosh_filter >> 16u)))
        {
        	/* -------------------------------------------------------------
        	 ** FDELTA x = FDELTA BASE x
        	 ** -----------------------------------------------------------*/
        	Fuel03SenderA.fdelta = Fuel03SenderA.fdelta_base;

        	/* -------------------------------------------------------------
        	 ** SLOSH FILT x = SLOSH FILT x + FDELTA x
        	 ** -----------------------------------------------------------*/
        	if((fuel03_slosh_filter_limit-Fuel03SenderA.slosh_filter)>= Fuel03SenderA.fdelta )
        	{
        		Fuel03SenderA.slosh_filter += Fuel03SenderA.fdelta;
        	}
        	else
        	{
        		Fuel03SenderA.slosh_filter = fuel03_slosh_filter_limit;
        	}

#ifdef NISSAN_CHINA
        	if(Fuel03PercentFull < ((uint16)(Fuel03SenderA.slosh_filter >> 16u)))
        	{
        		Fuel03SenderA.slosh_filter = (((uint32)Fuel03PercentFull) << 16u);
        	}
        	else
        	{
        	}
#endif
        }
        /* -------------------------------------------------------------
         ** FLPS x < SLOSH FILT x MSB ?
         ** -----------------------------------------------------------*/
        /*
         ** If the current sample is above the filter, check if the
         ** previous sample was above as well.  If so, add an adjustment
         ** factor to the delta to be added to the filter.  Similarly,
         ** if the current and previous samples were both below the
         ** filter, add the adjustment factor.  Make sure to clamp the
         ** delta at the limits from the config file.
         */
        else if( Fuel03PercentFull < ((uint16)(Fuel03SenderA.slosh_filter >> 16u)))
        {
        	/* -------------------------------------------------------------
        	 ** FDELTA x = FDELTA BASE x
        	 ** -----------------------------------------------------------*/
        	Fuel03SenderA.fdelta = Fuel03SenderA.fdelta_base;

        	/* -------------------------------------------------------------
        	 ** SLOSH FILT x = SLOSH FILT x - FDELTA x
        	 ** -----------------------------------------------------------*/
        	if( Fuel03SenderA.slosh_filter > Fuel03SenderA.fdelta )
        	{
        		Fuel03SenderA.slosh_filter -= Fuel03SenderA.fdelta;
        	}
        	else
        	{
        		Fuel03SenderA.slosh_filter = 0;
        	}

#ifdef NISSAN_CHINA
        	if(Fuel03PercentFull > ((uint16)(Fuel03SenderA.slosh_filter >> 16u)))
        	{
        		Fuel03SenderA.slosh_filter = (((uint32)Fuel03PercentFull) << 16u);
        	}
        	else
        	{
        	}
#endif
        }
        else
        {
        	/* -------------------------------------------------------------
        	 ** x > flag = 0     Clear the sample is above filter flag.
        	 ** x < flag = 0     Clear the sample is below filter flag.
        	 ** -----------------------------------------------------------*/
        	Fuel03SenderA.fdelta = Fuel03SenderA.fdelta_base;
        }
    }

    LocalFullPos16 = Fuel03PercentFullPosHystNVM;
    /* -------------------------------------------------------------
    ** Else perform hysteresis on the percent full data.
    ** -----------------------------------------------------------*/
    if( Fuel03SenderA.slosh_filter > Fuel03SenderA.slosh_filter_hys )
    {
        if( FUEL03_MS_16_BITS(Fuel03SenderA.slosh_filter - Fuel03SenderA.slosh_filter_hys) > LocalFullPos16 )
        {
            Fuel03SenderA.slosh_filter_hys = Fuel03SenderA.slosh_filter;
        }
    }
    else
    {
        LocalFullPos16 = Fuel03PercentFullNegHystNVM;

        if( FUEL03_MS_16_BITS(Fuel03SenderA.slosh_filter_hys - Fuel03SenderA.slosh_filter) > LocalFullPos16 )
        {
            Fuel03SenderA.slosh_filter_hys = Fuel03SenderA.slosh_filter;
        }
    }
}

#if 0
/*============================================================================
//
//  Function Name:      void fuel03_level_calculation( void )
//
//  Visibility:         Internal
//
//  Description:        Routine to calculate the fuel level.
//
//  Requirments:        Figure 3.6.4-20 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            07/17/03 by RRUSS1
//
//  Updated:            02/23/05 by RRUSS1
//  updated:            08/04/08 by KNAGASUB
//==========================================================================*/
static void fuel03_level_calculation( void )
{
    fuel03_level = FUEL03_MS_16_BITS(Fuel03SenderA.slosh_filter);
}


/*============================================================================
//
//  Function Name:      void fuel03_level_calculation_with_hys( void )
//
//  Visibility:         Internal
//
//  Description:        Routine to calculate the fuel level with hysteresis.
//
//  Requirments:        Figure 3.6.4-21 Fuel BDS
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            09/12/03 by RRUSS1
//
//  Updated:            09/12/03 by RRUSS1
//
//==========================================================================*/
static void fuel03_level_calculation_with_hys( void )
{
    fuel03_level_with_hys = FUEL03_MS_16_BITS(Fuel03SenderA.slosh_filter_hys);
}
#endif

/*============================================================================
//
//  Function Name:      uint16 fuel03_get_fuel_level( void )
//
//  Visibility:         External
//
//  Description:        Routine to return the single or combined percent full
//                      value of both senders with hysteresis.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            09/12/03 by RRUSS1
//
//  Updated:            09/12/03 by RRUSS1
//                      02/26/07 by JARUN for BSDI00038410
//
//==========================================================================*/
uint16 fuel03_get_fuel_level(void)
{
    return( FUEL03_MS_16_BITS(Fuel03SenderA.slosh_filter) );
}

/*============================================================================
//
//  Function Name:      uint16 fuel03_get_fuel_level_with_hys( void )
//
//  Visibility:         External
//
//  Description:        Routine to return the single or combined percent full
//                      value of both senders with hysteresis.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            09/12/03 by RRUSS1
//
//  Updated:            09/12/03 by RRUSS1
//
//==========================================================================*/
uint16 fuel03_get_fuel_level_with_hys(void)
{
    fuel03_level_with_hys =  FUEL03_MS_16_BITS(Fuel03SenderA.slosh_filter_hys);
    return(fuel03_level_with_hys );
}

/*============================================================================
//
//  Function Name:      uint16 fuel03_get_senderA_PFH(void)
//
//  Visibility:         External
//
//  Description:        Routine to return the percent full value with
//                      hysteresis of sender A.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None.
//
//  Output Arguments:   Sender A percent full with hysteresis.
//
//  Critical Section:   None
//
//  Created:            09/12/03 by RRUSS1
//
//  Updated:            09/12/03 by RRUSS1
//
//==========================================================================*/
uint16 fuel03_get_senderA_PFH(void)
{
    fuel03_level_with_hys =  FUEL03_MS_16_BITS(Fuel03SenderA.slosh_filter_hys);
    return ( fuel03_level_with_hys );
}

/*============================================================================
//
//  Function Name:      uint16 fuel03_get_senderA_PF(void)
//
//  Visibility:         External
//
//  Description:        Routine to return the percent full value without
//                      hysteresis of sender A.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None.
//
//  Output Arguments:   Sender A percent full without hysteresis.
//
//  Critical Section:   None
//
//  Created:            09/12/03 by RRUSS1
//
//  Updated:            09/12/03 by RRUSS1
//
//==========================================================================*/
uint16 fuel03_get_senderA_PF(void)
{
    return ( FUEL03_MS_16_BITS(Fuel03SenderA.slosh_filter) );
}

/*============================================================================
//
//  Function Name:      boolean fuel03_init_finished(void)
//
//  Visibility:         External
//
//  Description:        Routine to determine if fuel init is finished.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   TRUE - fuel init finished
//                      FALSE - fuel init not finished
//
//  Critical Section:   None
//
//  Created:            09/16/03 by RRUSS1
//
//  Updated:            09/16/03 by RRUSS1
//
//==========================================================================*/
boolean fuel03_init_finished(void)
{
    boolean fuel03_retval;

    fuel03_retval = FALSE;

    if( TESTBIT(Fuel03SenderA.MoreFilterFlags, FUEL03_INIT_FLAG) )
    {
        fuel03_retval = TRUE;
    }

    return( fuel03_retval );
}

/*============================================================================
//
//  Function Name:      void fuel03_network_message_missing(void)
//
//  Visibility:         External
//
//  Description:        Routine to set the key-off reference sample to
//                      invalid when the network message containing
//                      the RAD samples is missing.  Since the function
//                      is so small, it isn't conditionally compiled
//                      out on non-network systems.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            09/17/03 by RRUSS1
//
//  Updated:            09/17/03 by RRUSS1
//
//==========================================================================*/
void fuel03_network_message_missing(void)
{
    /*
    ** Set the key off reference to invalid.
    */
    Fuel03SenderA.KeyOffRef = FUEL03_PERCENT_FULL_INVALID_BYTE;
}

/*============================================================================
//
//  Function Name:      boolean fuel03_senderA_fast_fill_active(void)
//
//  Visibility:         External
//
//  Description:        Routine to determine if fast fill is active for Sender A.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   TRUE - fast fill active
//                      FALSE - fast fill not active
//
//  Critical Section:   None
//
//  Created:            09/17/03 by RRUSS1
//
//  Updated:            09/17/03 by RRUSS1
//
//==========================================================================*/
boolean fuel03_senderA_fast_fill_active(void)
{
    boolean fuel03_retval;

    fuel03_retval = FALSE;

    if( TESTBIT( Fuel03SenderA.MoreFilterFlags, FUEL03_FAST_FILL_FLAG ) )
    {
        fuel03_retval = TRUE;
    }

    return( fuel03_retval );
}

/*============================================================================
//
//  Function Name:      boolean fuel03_fast_fill_active(void)
//
//  Visibility:         External
//
//  Description:        Routine to determine if any sender has fast fill active.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   TRUE - fast fill active
//                      FALSE - fast fill not active
//
//  Critical Section:   None
//
//  Created:            09/17/03 by RRUSS1
//
//  Updated:            09/17/03 by RRUSS1
//
//==========================================================================*/
boolean fuel03_fast_fill_active(void)
{
    boolean fuel03_retval;

    fuel03_retval = FALSE;

    if( fuel03_senderA_fast_fill_active() != FALSE )
    {
        fuel03_retval = TRUE;
    }

    return(fuel03_retval);
}


/*============================================================================
//
//  Function Name:      boolean fuel03_senderA_fill_detected(void)
//
//  Visibility:         External
//
//  Description:        Routine to determine if a fill has been detected for
//                      Sender A.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   TRUE - fill detected
//                      FALSE - fill not detected
//
//  Critical Section:   None
//
//  Created:            09/17/03 by RRUSS1
//
//  Updated:            09/17/03 by RRUSS1
//
//==========================================================================*/
boolean fuel03_senderA_fill_detected(void)
{
    boolean fuel03_retval;

    fuel03_retval = FALSE;

    if( TESTBIT(Fuel03SenderA.FilterFlags, FUEL03_FILL_DETECTED) )
    {
        fuel03_retval = TRUE;
    }
    return(fuel03_retval);
}

/*============================================================================
//
//  Function Name:      boolean fuel03_senderA_loss_detected(void)
//
//  Visibility:         External
//
//  Description:        Routine to determine if a loss has been detected for
//                      Sender A.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   TRUE - loss detected
//                      FALSE - loss not detected
//
//  Critical Section:   None
//
//  Created:            05/06/04 by RRUSS1
//
//  Updated:            05/06/04 by RRUSS1
//
//==========================================================================*/
boolean fuel03_senderA_loss_detected(void)
{
    boolean fuel03_retval;

    fuel03_retval = FALSE;

    if( TESTBIT(Fuel03SenderA.FilterFlags, FUEL03_LOSS_DETECTED) )
    {
        fuel03_retval = TRUE;
    }
    return(fuel03_retval);
}

/*============================================================================
//
//  Function Name:      boolean fuel03_fill_detected(void)
//
//  Visibility:         External
//
//  Description:        Routine to determine if a fill has been detected. For
//                      single tank systems only Sender A
//                      fill detected status is returned. For 2 senders, either
//                      A or B fill detected is returned.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   TRUE - fill detected
//                      FALSE - fill not detected
//
//  Critical Section:   None
//
//  Created:            08/17/03 by RRUSS1
//
//  Updated:            09/17/03 by RRUSS1
//
//==========================================================================*/
boolean fuel03_fill_detected(void)
{
    boolean fuel03_retval;

    fuel03_retval = FALSE;

    if (TESTBIT(Fuel03SenderA.FilterFlags, FUEL03_FILL_DETECTED))
    {
        fuel03_retval = TRUE;
    }

    return(fuel03_retval);
}

/*============================================================================
//
//  Function Name:      boolean fuel03_loss_detected(void)
//
//  Visibility:         External
//
//  Description:        Routine to determine if a loss has been detected. For
//                      single tank systems only Sender A
//                      loss detected status is returned. For 2 senders, either
//                      A or B loss detected is returned.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   TRUE - loss detected
//                      FALSE - loss not detected
//
//  Critical Section:   None
//
//  Created:            05/06/04 by RRUSS1
//
//  Updated:            05/06/04 by RRUSS1
//
//==========================================================================*/
boolean fuel03_loss_detected(void)
{
    boolean fuel03_retval;

    fuel03_retval = FALSE;

    if (TESTBIT(Fuel03SenderA.FilterFlags, FUEL03_LOSS_DETECTED))
    {
        fuel03_retval = TRUE;
    }

    return(fuel03_retval);
}

/*============================================================================
//
//  Function Name:      boolean fuel03_fill_loss_detected(void)
//
//  Visibility:         External
//
//  Description:        Routine to determine if a fill or loss has been detected. For
//                      single tank systems only Sender A
//                      fill/loss detected status is returned. For 2 senders, either
//                      A or B fill/loss detected is returned.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   TRUE - fill/loss detected
//                      FALSE - fill/loss not detected
//
//  Critical Section:   None
//
//  Created:            05/06/04 by RRUSS1
//
//  Updated:            05/06/04 by RRUSS1
//
//==========================================================================*/
boolean fuel03_fill_or_loss_detected(void)
{
    boolean fuel03_retval;

    fuel03_retval = FALSE;

    if( fuel03_fill_detected() != FALSE )
    {
        fuel03_retval = TRUE;
    }
    else if( fuel03_loss_detected() != FALSE )
    {
        fuel03_retval = TRUE;
    }
    else
    {
    }

    return(fuel03_retval);
}

uint8 Fuel03_SenderA_keyon_avgflps( void )
{
    return( Fuel03SenderA.keyon_avgflps );
}

uint8 Fuel03_SenderA_KeyOffRef( void )
{
    return( Fuel03SenderA.KeyOffRef );
}

uint8 Fuel03_SenderA_zss_ref( void )
{
    return( Fuel03SenderA.zss_ref );
}

uint8 Fuel03_SenderA_keyon_good_samples( void )
{
    return( Fuel03SenderA.keyon_good_samples );
}

/*============================================================================
//
//  Function Name:      uint8 fuel03_get_SenderA_unfiltered_flps(void)
//
//  Visibility:         External
//
//  Description:        Return the unfiltered flps.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    none
//
//  Output Arguments:   none
//
//  Critical Section:   None
//
//  Created:            02/26/04 by RRUSS1
//
//  Updated:            02/26/04 by RRUSS1
//
//==========================================================================*/
uint8 fuel03_get_SenderA_unfiltered_flps( void )
{
    return( Fuel03SenderA.flps );
}
/*============================================================================
//
//  Function Name:      void Fuel03_clear_DTC_resets(void)
//
//  Visibility:         External
//
//  Description:        Routine will reset the fault flages and timers
//                      Sender A.
//
//  INPUTS/OUTPUTS
//
//  Input Arguments:    None
//
//  Output Arguments:   None
//
//  Critical Section:   None
//
//  Created:            28/10/08 by KNAGASUB
//
//  updated:            24/11/08 by KNAGASUB
//==========================================================================*/
void fuel03_clear_DTC_resets(void)
{

}


void fuel03_set_refuel_flag(boolean flag)
{
	fuel03_refuel_flag = flag;
}

boolean fuel03_get_refuel_flag(void)
{
	return fuel03_refuel_flag;
}

/*============================================================================
//
//  $Revision:   1.17  $
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
 * 11/19/13     BKORAGAN
 * FLPS interface implementation for trip computer request. NISSAN Specific
 * RTC work item:    99371


// 06/28/13       BKORAGAN
// Fuel package update as per Nissan P42M specific BDD v0.2
// updated fuel03_key_off_sample_processing() and fuel03_average_rad_processing()

// 05/21/13       BKORAGAN
// Fuel package update as per Nissan P42M specific BDD v0.2

// 07/10/12       BKORAGAN
// Multirate fuel filter implemented as per BSDI00117140.

// 08/05/11       KNAGASUB
// BSDI00107829-Fuel03:clamp the Slosh filter so that it does not exceed 4 bytes
// fuel03_slosh_filter() function updated.

// 06/16/11       KNAGASUB
// BSDI00103442-FUEL03_DELTA_FAST_CLAMP_A and FUEL03_DELTA_NORMAL_CLAMP_A check limit
// removed from 3bytes to 4bytes.

// 06/16/11       KNAGASUB
// BSDI00103442-MIN_FAST_FILL_TIME parameter is moved to tank dependent constant.

// 05/02/11        KNAGASUB
//Following interfaces are added tosupport BSDI00103693-Separate fuel open short bit check interface 
//required for logging DTC's
//1)boolean fuel03_open_bit_check(void)
//2)boolean fuel03_short_bit_check(void)
//3)boolean fuel03_open_short_bit_check(void)

To resolve the setbit,clearbit QAC7.1warning,macro's are added with proper suffix ("u")

// 08/07/10        KNAGASUB
//To resolve the setbit,clearbit QAC7.1warning,macro's are added with proper suffix ("u")
 

// 10/11/10        KNAGASUB

//Following functions are updated/defined for QAC7.1
//1) static void fuel03_fi_check( void );
//2)static void fuel03_finit_F3( void );
//3)static void fuel03_finit_t3( void );
//4)static void fuel03_key_off_sample_processing( void );
//5)static void fuel03_zss_keyoff_processing( uint16 fuel03_rada );
//6)static void fuel03_common_gauge_init( void );
//7)static void fuel03_average_rad_processing( void );
//8)static void fuel03_fi_compare( void );
//9)static uint8 fuel03_rad_to_flps( uint16 fuel03_rad );
//10)static void fast_fill_determination_f1( void );
//11)static uint8 fuel03_fault_snapshot_struct_size(void);
//12)static void fuel03_koc_f1_setup( void );
//13)static void fuel03_koc_f1( void );
//14)static void fuel03_ffsc_f1( void );
//15)static void fuel03_fast_fill_enable_check_subf1( void  );
//16)static void fuel03_get_tank_config( void );
//17)uint8 fuel03_4190_KSGetPID(uint16 *fuel03_buffer)
//18)static void fuel03_log_init_errors_to_nvm( void )
//19)static void fuel03_average_rad_processing( void )
//20)static void fuel03_fi_check( void )
//21) static uint8 fuel03_fault_snap_struct_size(void) 

// 08/07/10        KNAGASUB
// fuel03_fi_check() updated per BSDI00093940-Fuel03:DCR 1510 Fuel CGEA v7.0 update. 

// 08/07/10        KNAGASUB
// fuel03_cold_init(),fuel03_fast_fill_enable_check(),fuel03_fi_check() 
//functions are updated per FORD STSS7.0 draft3.doc

// 08/31/10        BVIJAYKU
// Updated for BSDI00091411-Kepler platform QAC fix.

// 08/12/09        KNAGASUB
// fuel03_KSColdInit() updated as per peer review comments.

// 08/10/09        KNAGASUB
//The fuel03_key_off_sample_processing function is updated per FUEL03_BDD1.5.doc changes.
//-Figure - Key Off Processing  Deleted the following blocks 45, 46, 47, & 49.
// If a sender was declared faulted, but now is good,
//but has not been declared good yet, then allow key off reference to be taken.
//This meets Ford spec and is ok for non Ford OEMs as well.

// 08/03/09        KNAGASUB
//The following function are updated per FUEL03_BDD1.5.doc changes.
//Per CQBSDI00074093
//1)fuel03_cold_init()-Set the A Force Fuel Init Flag to 1 to force a fuel init after a battery connect
//2)fuel03_debounce_open_short()- Figure - RAD Input - Open / Short Check   Bypass block 31 if Ford is the OEM.
// Add block 33 to check if the FUEL03_REF_AVG_TYPE = 4, if so bypass setting the A Force Fuel Init Flag to 1,
// so Ford specific recovery processing (change 6) can occur.

// 06/26/09        KNAGASUB
//The function are rearranged as per peer review comment.

// 06/25/09        KNAGASUB
//.Two PIDS are modified fuel03_4190_KSGetPID() and fuel03_61B7_KSGetPID()
// per peer review comments.

// 06/22/09        KNAGASUB
//.Two PIDS are modified fuel03_4190_KSGetPID() and fuel03_61B7_KSGetPID()
// per peer review comments.

// 06/02/09        KNAGASUB
//.Two PIDS are newly added fuel03_4190_KSGetPID() and fuel03_61B7_KSGetPID()
// fuel03_fi_check() function are modifed per Fuel03_BDD1.5.doc.

// 30/03/09        JARUN
//.Removed Fuel03SenderA.zs_converged = FALSE; from fn fuel03_koc_f1_setup()
// for the CQ:BSDI00068305
//
// 24/11/08       KNAGASUB.
// The interface function name changed from Fuel03_clear_DTC_resets()  to fuel03_clear_DTC_resets()

// 19/11/08       KNAGASUB.
// The interface function name changed from Fuel03_clear_DTC_flimers()  to Fuel03_clear_DTC_resets()
// as per peer review comments
// fuel03_fast_fill_enable_check() updated as per the CQ BSDI00062869(Gauge takes longer than a minute to full).
// fuel03_periodic_processing() modified as per peer review comments.


// 13/10/08       KNAGASUB

//fuel03_debounce_open_short() function only set's or clear open/short flag bit's
// and   not log or clear the DTC in this function itself.
// The function fuel03_log_dtcs() which is used to log/clear   purpose alone.

//  28/10/08      KNAGASUB
//  new interface function Fuel03_clear_DTC_flimers() added as per STSS9.0_rev2_cahnges.doc.

// 28/10/08       KNAGASUB
// fuel03_fast_fill_enable_check() function modified to set ZSS convergence as "FALSE".

// 24/10/08       KNAGASUB
// fuel03_periodic_processing() updated as per STSS9.0_reve2.doc.
// fuel03_start_init() updated to avoid fuel03_log_init_errors_to_nvm()" function
// will always attempt to program the NVM when init error is not logged in the RAM at start.

// 14/10/08       KNAGASUB
// fuel03_periodic_processing() updated as per peer review comments.

// 09/10/08       KNAGASUB
//  The iginition_status check is introduced as per fuel02 package and also avoid comipiler warnings.

// 09/10/08       KNAGASUB
// changes done in fault record macro's as per review comments.

// 01/10/08       KNAGASUB
// The changes are done as per the CQ BSDI00061422.
// code modified as per review comments
//To deactive the fault record size's set the macro(FUEL03_RAM_LOG_FAULT_RECORD_SIZE,FUEL03_LOG_FAULT_RECORD_SIZE) to zero.

// 25/09/08       KNAGASUB
// The changes done as per the CQ BSDI00061422.
// The macro is removed FUEL03_LOG_INIT_ERRORS_TO_NVM.
// NVM fault record array is turned  off separately by these macro's
// FUEL03_RAM_LOG_FAULT_RECORD_SIZE,FUEL03_LOG_FAULT_RECORD_SIZE.

// 24/09/08       KNAGASUB
// The macro FUEL03_NUM_FUEL_TABLES which is used to remove the unused table NVM bytes allocation.

// 24/09/08       KNAGASUB
// fuel03_periodic_processing() is modified as per ford STSS9.0

// 24/09/08       KNAGASUB
//  fuel03_fast_fill_enable_check_subf1(() modified as per the CQBSDI00061689                                                                 BSDI00061689
//  (Gauge will move accelarate( 17 min)when open short detected)

// 17/09/08       KNAGASUB
//  fuel03_fi_check(() modified to avoid compiler warning
// (line 2481: Warning:  #111-D: statement is unreachable
//  uel03SenderA.init_pid = 0x07ff;)if the configuration of
//  the macro FUEL03_REF_AVG_TYPE  != 4.in fuel03_bds.cfg

// 09/08/08       KNAGASUB
// fuel03_open_short_test() modifed to include the Fuel03A2DOpenLimNVM and Fuel03A2DShortLimNVM
// comapatible to more than one tank also.

//   Fuel03A2DShortLimNVM;

// 08/31/08       KNAGASUB
// The macro FUEL03_ADD_CONVERGENCE_DATA_TO_FAULT_RECORD added as per BSDI00053792

// 08/20/08       KNAGASUB
//  The following functions are modified as per defects found in internal testing
//  1)fuel03_senderA_open_short_detected()
//  2)fuel03_senderA_open_detected()
//  3)fuel03_senderA_short_detected()

//  08/18/08      KNAGASUB
//  The following functions are modified as per functional testing defects
//  1)fuel03_senderA_open_short_detected()
//  2)fuel03_senderA_open_detected()
//  3)fuel03_senderA_short_detected()
//  4)fuel03_periodic_processing()
//  5) fuel03_filter_initialize()

//  08/04/08      KNAGASUB
//   All the open short  interfaces functions  return "TRUE",
//     if the fault_flag  is  set to 1 and slosh filter is set zero when REF_AVG_TYPE =4
//   The modified functions are
//  1)fuel03_senderA_open_short_detected()
//  2)fuel03_senderA_open_detected()
//  3)fuel03_senderA_short_detected

//  08/04/08      KNAGASUB
//  updated fuel03_periodic_processing() function modified as per the
//  fuel03BDD1.3.doc(STSS8.0 changes)

//  08/04/08      KNAGASUB
//  fuel03_periodic_processing() function modified as per the
//  fuel03BDD1.3.doc(STSS8.0 changes)

//  08/04/08      KNAGASUB
//  fuel03_level_calculation() function modified as per the
//  fuel03BDD1.3.doc(STSS8.0 changes)

//  07/13/08      KNAGASUB     BSDI00058789 (spss 6.2 kos issue fix)
//  Modified fuel02_keyoff_processing() and fuel02_koc() function as per
//  BDD (Earlier known as BDS) 1.2 version revision history points 5 & 18

//  07/13/08      KNAGASUB   BSDI00058764
//  Modified fuel03_init_avgflps_calc() function to increment
//  Fuel02_AVG_Vol_Invalid_Counter only for FUEL03_REF_AVG_TYPE 4
//  BDD (Earlier known as BDS) 1.2 version revision history point 8

//  06/12/08      CNAGARA1
//  1. Added the following variable logging into RAM snapshot structure
//     fuel03_invalid_condition & fuel03_ignition_cycle_counter
//  2. modified fuel03_log_init_errors_to_ram() to check for
//     Fuel03SenderA.koc_converged before logging ConvCycleCounter &
//     SmplCounter. This is linked to the following #3 comment.
//
//  05/26/08      CNAGARA1
//  Ref: BSDI00053792(#3, #4)
//  1. Added one byte storage with 1 bit for each of FUEL03_VALID_CONDITION()
//     6 places. This is for debug purpose.
//     And this variable is also assigned to the RAM snapshot structure element
//  2. Added ignition cycle counter to count the number of ignition cycles
//     which is incremented once per fuel03_periodic_processing() function call
//     And this is also assigned to the RAM snapshot structure element
//  3. Modified fuel03_koc_f1() function to take the copy of the following
//     vaiables
//     ConvCycleCounter & SmplCounter
//     These variabels were cleared once KOS is done. So the information is
//     lost before they are loaded into RAM snapshot. To avoid this defect
//     the values are loaded into shadow variabels to not affect the existing
//     code.
//
//  05/08/08      CNAGARA1
//  Ref SMIS Peer review package ID10444, comments 6 to 8
//  1. Modified fuel03_log_init_errors_to_nvm() to take care of above peer
//    review comments
//  2. Modified the following functions to avoid the QAC warnings
//     fuel03_C909_KSGetPID(),
//     fuel03_C90A_KSGetPID(),
//     fuel03_C90e_KSGetPID(),
//     fuel03_C90f_KSGetPID(),
//     fuel03_C910_KSGetPID(),
//     fuel03_C911_KSGetPID()
//  3. Updated fuel03_log_init_errors_to_nvm() for the defect which is found
//     during functinal testing. The defect is logged in ID10444.
//
//  04/18/08      SRATHINA
//  Ref BSDI00053792 : Modified the function fuel03_log_init_errors_to_nvm to
//  split the epm manager queue function into two in order to queue the data
//  more than 32 bytes.
//  Currently fuel03_log_init_errors_to_nvm can support 64 bytes of data to be
//  queued.if the FUEL03_FAULT_SNAPSHOT_STRUCT is added more than 64 bytes then the
//  fuel03_log_init_errors_to_nvm function needs to be modfied accordingly.
//
//  03/03/08      CNAGARA1
//  Updated fuel03_KSDiagStartSelfTest() to fix BSDI00054702
//
//  03/10/08      CNAGARA1
//  Ref BSDI00055412 : 1. Moved fuel03_log_init_errors_to_ram and
//                        fuel03_log_init_errors_to_nvm function prototypes
//                        to compiler switch FUEL03_LOG_INIT_ERRORS_TO_NVM
//                     2. Moved Fuel03State variable declaration and intilization,
//                        its associated macros to compiler switch FUEL03_REF_AVG_TYPE==4
//       BSDI00055424 : ConvSample comparision against FUEL03_KOS_MIN_SMPLS has been
//                      changed from '>' to '>='
//       BSDI00055411(3rd point) : Changed the scope of fuel03_next_fault_location_RAM
//                                 variable from global to function
//
//  10/25/07      RRUSS1
//
//  Ref BSDI00050096 : Updated C910 and C911 to reference NVM instead
//                     of RAM and also updated C90e/C90f.
//
//  03/30/07     JARUN
//  Ref  BSDI00044140: Fuel 03 Fast fill algorithm change.
//    Added a new variable to count 2s "Fuel03MinFastFillTimeCounter"
//   for entering Fast Fill Mode.
//   Updated fuel03_fast_fill_determination(), fast_fill_determination_f1()
//   for the change in fast fill algorithm.
//
//  02/27/07     JARUN    BSDI00038410 FUEL03 made in line with
//                        fuel03 package.
// Added checks for FUEL03_SCEM_NUMBER_OF_FUEL_TANKS = 1 and if true the this
// value is undefined so previous functionality is maintained.
// Removed Fuel02TankIndex = 0 from fuel03_get_tank_config() if
// FUEL03_SCEM_NUMBER_OF_FUEL_TANKS is undefined. (BSDI00036642)
//
// Re-orderd the FUEL03_FLT structure for ARM processors.(BSDI00024469)
//
// 1. Added state variable Fuel02State to the allocation section.
// 2. Added Fuel03State = FUEL03_RESET to the cold init function.
// 3. Added check in the periodic and keyoff processing to check for a change
//    in state and if changed, then call the koc_setup().(BSDI00020595)
//
//  Support for cosmic compiler needs to be provided. (BSDI00040615)
//
//  Added check for REF_AVG=4 in fuel03_key_off_sample_processing().
//  (BSDI00032867)
//
//  Added block 34 Hill Hold Enable check in Key On processing fig 1.3.7-1
//  for fuel03_periodic_processing()
//
// Added blocks for DTC logging as on fuel BDS fig 1.3.5-1
//  RAD Input Open / Short Check  to fuel03_debounce_open_short()
//
//  Modified according to fuel03 bds Figure 3.3-13 Fast Fill Samples
//  the function fuel03_fast_fill_samples()
//
// Ref BSDI00030213 : Changed the fuel03_get_fuel_level() to look like the
//                     fuel03_get_fuel_level_with_hys() to prevent returning
//                     the fuel03_level which isn't updated until the first
//                     iteration after init in the periodic processing function.
//
//  Ref BSDI00028685 : Added functions for access to fuel values.
//
//  (BSDI00037645)Corrected the way the data is returned in
//  fuel03_C909_KSGetPID()
//  fuel03_C90A_KSGetPID()
//  fuel03_C90C_KSGetPID()
//  fuel03_C90D_KSGetPID()
//  fuel03_C90e_KSGetPID()
//  fuel03_C910_KSGetPID()
//--------------------------------------------------------------------------------
//
//  6/19/06      RRUSS1
//
//  Added static to line 292 to prevent QAC warning.
//
//  5/24/06       RRUSS1  BSDI00031391
//
//  Updated to work with auto generated config files.
//
//  4/10/06      RRUSS1
//
//  Ref BSDI00030114 :
//      1. Moved the scope of the variable uint16 Fuel03PercentFull from
//         local within fuel03_rad_to_flps() to global so the 16 bit flps
//         value can be checked if FUEL02_FILTER_RES=0 in fig 3.1.4-17.
//      2. Added checks for FUEL02_FILTER_RES in fuel03_slosh_filter() to
//         decide if the 8bit flps value or the 16bit Fuel03PercentFull value
//         should be used.
//
//  Ref BSDI00028935 : Updates to Ford STSS 6.0
//
//  These changes are identified in the table of changes in the
//  Fuel02 BDS.
//
//  0.1-3. Removed the zss convergence from function fuel03_zss_keyoff_processing().
//
//  0.1-4. Added FUEL03_POWER_MODE_MACRO to periodic_processing function.
//
//  0.1-5. Added Fuel03_AVG_Vol_Invalid_Counter increment in function
//         fuel03_init_avgflps_calc(). This change was tested by watching
//         Fuel03_AVG_Vol_Invalid_Counter using the BDM while cycling
//         ignition off to on with an invalid RAD value.
//
//  0.1-6. Added blocks 39, 40, 41, and 42 from fig 3.6.4-12 to function
//         fuel03_fast_fill_enable_check(). This change was tested by watching
//         Fuel03_AVG_Vol_Invalid_Counter using the BDM while cycling
//         ignition off to on with an invalid RAD value and verifying that the
//         value counts up and then resets to 0.
//
//  0.1-7. Added decision blocks  24 and 25 from fig 3.6.4-14 to function
//         fuel03_fast_fill_determination().
//         Rewrote this function somewhat so _f1() function which was repeated
//         3 times could be a single function and could be called by the added
//         decision blocks. This change was tested by watching
//         Fuel03_AVG_Vol_Invalid_Counter using the BDM while cycling
//         ignition off to on with an invalid RAD value and verifying that the
//         fast fill flag was set.
//
//  0.2-1. Added block 63 to function fuel03_keyoff_processing().
//
//  0.4-2. Added blocks 49 and 50 in fig 3.6.4-2 to function
//         fuel03_periodic_processing() to make KeyOffRef invalid if convergence
//         is selected.
//
//         Added VALID_CONDITION to keyoff_processing_done function.
//
//  9/27/05      RRUSS1
//
//  Ref BSDI00025533 : Changed the function interfaces from ...
//
//       uint8 fuel03_6185_KSGetPID(uint8 *fuel02_buffer)
//       uint8 fuel03_6187_KSGetPID(uint8 *fuel02_buffer)
//
//       to ...
//
//       uint8 fuel03_6185_KSGetPID(void *fuel02_buffer)
//       uint8 fuel03_6187_KSGetPID(void *fuel02_buffer)
//
//       and changed the appropriate type qualifier for the data in these
//       functions to prevent QAC warnings that resulted in a mismatch in type
//       from the PROD_SPEC_DIAG_PID_TABLE.
//
// 9/14/05       RRUSS1  BSDI00024437
//
// Ref BSDI00023220 : Added the following function fuel02_KSColdInit();
//
//                    #if (FUEL03_REF_AVG_TYPE==4)
//                       Fuel03SenderA.koc_fault_flag = TRUE;
//                    #endif
//
//                    Removed Fuel03SenderA.koc_fault_flag = FALSE;
//                    from the fuel03_koc_f1_setup() function.
//
//
// 8/15/05       RRUSS1  BSDI00023109
//
// Ref BSDI00023110 : Added function fuel03_log_dtcs() and updated the
//                    periodic processing function to call it.
//
// 6/24/05        RRUSS1  QAC
//
// Added (void *) to LinearInterpolateWord() statement in function fuel03_rad_to_flps().
//
// 6/15/05        RRUSS1  BSDI00020870
//
// Changed the following defines in function fuel02_debounce_open_short()...
//   FUEL02_LOG_SENDER_A_OPEN_ERROR()     to   FUEL02_LOG_FUEL_SENDER_CKT_OPEN()
//   FUEL02_LOG_SENDER_A_SHORTED_ERROR()  to   FUEL02_LOG_FUEL_SENDER_CKT_SHORT()
//
// 6/15/05        RRUSS1  BSDI00020719
//
// Moved NVM declarations to a seperate file for the ARM processor support.
//
//
// 04/14/05     RRUSS1   1. Ref BSDI0019134 : Incorporated the PID C90A fix
//                          that was identified by EN/FN for Fuel02.
//
//                       2. Removed the previous Fuel02 revision history.
//
// 03/22/05     RRUSS1  Initial version optimized from Fuel02.
//
//==========================================================================*/

/* end of file ============================================================ */
