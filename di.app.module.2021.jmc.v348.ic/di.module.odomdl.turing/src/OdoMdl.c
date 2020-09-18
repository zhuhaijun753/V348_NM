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

#include "Rte_OdoMdl.h"
#include "Mcu.h"
#include "CmpLib.h"
#include "OdoMdl_Cfg.h"
#include "OdoMdl.h"
#include "MathLib.h"
#include "TmExt_Cfg.h"
#include "TmExt_Timer.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(OdoMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
/* define some internal bit flags */
#define cODO_STARTED_CALLBACK_PROCESSING (1u)/* set true at start of callback */
                                                   /* processing and false at end   */
#define cODO_RETRY_UPDATE_REQUEST (2u) /* TRUE if another update request attempt should be*/
                                         /* made, FALSE otherwise                              */
#define cODO_UPDATE_FAILED (4u)    /* TRUE if an update operation fails, this will cause */
                                         /* an attempt to update another(if any) data set      */
#define cODO_READ_FAILED (0x08u)      /* this flag is set when there has been a failure by */
                                         /* OdoRead to find two valid data sets              */
#define cODO_COUNTS_LOST (0x10u)   /* this is set when odo_write has not been able to   */
                                         /* perform updates and OdoAddCounts detects that   */
                                         /* TOdoOdoramWholeCounts is about to roll over         */

#define cODO_CHANGEUP_PROCESSING (0x80u) /* Updated during after 2 set sucess */
										/*and clear when there is2 successful callbacks from epm for odo changeup ,otherwise when the retry timer expries  */

/*  define types of roll back  */
#define cODO_PKG_ABSOLUTE 0
#define cODO_PKG_RELATIVE 1

/*  define a mask to allow stripping the CRC from the top nibble of a uint32 */
#define cODO_LWR7_NIBBLES_FF (0x0FFFFFFFUL)                                  

/*  define a bad data set index */
#define cODO_PKG_INVALID_SET 0xFF

/*
 define a bad CRC return for a long crc
*/
#define cODO_BAD_CRCL ((uint8)0x80)

/* Timer Related definitions */

#define cTWO_SECOND_TIMEOUT		2000
#define cFIVE_SECOND_TIMEOUT	5000

#define  cODO_ALLOW_CHANGE 0
/*
 define macros for checking the (distance) relationship
 between odo data sets
*/
#define cODORLCON ((uint8) (1+(2*(uint8)cPROD_SPEC_ODO_STORAGE_INCR)))
#define cODORLCON2 ((uint8)(cTOTAL_ODO_INCRS - cODORLCON))
/*
 if input counts are per km then define things appropriatly
*/
#if defined(Vehicle_Tire_235) /*Lhbb cPROD_SPEC_COUNTS_PER_KM is valid*/

  #define cCOUNTS_DELTA_COMP ((cPROD_SPEC_COUNTS_PER_KM)/100)
  
  /*
   if storage units are km then this is a simple case
   there are no fractional input counts per km
  */
  #if cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_KM  /*Lhbb Only cWHOLE_COUNTS_PER_UNIT is valid*/

    #define cWHOLE_COUNTS_PER_UNIT (cPROD_SPEC_COUNTS_PER_KM*cPROD_SPEC_ODO_STORAGE_INCR) /*8000*/

  

  #elif (cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_TENTH_KM)
   /*
    Definitions for 0.1 KM NVM Resolution
   */
   #define cKM_TO_TENTH_KM (0.1f)
   #define cWHOLE_COUNTS_PER_UNIT \
           ((uint16)(1 + (cKM_TO_TENTH_KM*(float32)(cPROD_SPEC_COUNTS_PER_KM*cPROD_SPEC_ODO_STORAGE_INCR))))

   #define cFRAC_TOTAL ((uint16)10000)
   #define cFRAC_COUNTS_PER_UNIT \
           ((uint16)((float32)cFRAC_TOTAL * \
           ((float32)cWHOLE_COUNTS_PER_UNIT - ((float32)((float32)cPROD_SPEC_COUNTS_PER_KM*cKM_TO_TENTH_KM ))) ))

  #else

    /*
     throw an error if no storage unit definition
    */
    #error cPROD_SPEC_ODO_STORAGE_UNITS is neither KM or TENTH_KM
  #endif
#endif

/* define the value that will limit total accumulation */
/* to 1,000,000 miles                                  */
/*                                                     */
/* a clamp at some value is necessary to avoid overflow             */
/* in the computations.                                             */
/* A million miles was chosen as a reasonable limit                 */
/* the max value returned by get_current_odo will be 1,000,00 miles */
/* or equivalent km + the amount contributed by TOdoOdoramWholeCounts */

/* take into account whether or not roll back is configured */
#ifdef cPROD_SPEC_RB_MAX_TIMES  /*Lhbb cPROD_SPEC_RB_MAX_TIMES Not defined*/
    #if cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_KM
         #ifdef cPROD_SPEC_RB_DISPLAY_UNITS
        /* Consider the product specific clamp value, if defined            */
          #ifdef cPROD_SPEC_NVM_ODO_VALUE_CLAMP
            #define cNVM_ODO_VALUE_CLAMP (cPROD_SPEC_NVM_ODO_VALUE_CLAMP + cODO_OFFSET + (uint32)cRB_MAX_UNITS_MILES)
          #else
            #define cNVM_ODO_VALUE_CLAMP (1609344UL + cODO_OFFSET + (uint32)cRB_MAX_UNITS_MILES)
          #endif
        #else
        /* Consider the product specific clamp value, if defined            */
          #ifdef cPROD_SPEC_NVM_ODO_VALUE_CLAMP
            #define cNVM_ODO_VALUE_CLAMP (cPROD_SPEC_NVM_ODO_VALUE_CLAMP + cODO_OFFSET + (uint32)cPROD_SPEC_RB_MAX_UNITS)
          #else
            #define cNVM_ODO_VALUE_CLAMP (1609344UL + cODO_OFFSET + (uint32)cPROD_SPEC_RB_MAX_UNITS)
          #endif
         #endif
    #elif cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_TENTH_KM
    /* Consider the product specific clamp value, if defined            */
      #ifdef cPROD_SPEC_NVM_ODO_VALUE_CLAMP
        #error "There shall be a chance of NVM Wear Out�� Please revisit the configuration items! 1. cPROD_SPEC_NVM_ODO_VALUE_CLAMP 2. cPROD_SPEC_ODO_STORAGE_FORMAT"
      #else
        #define cNVM_ODO_VALUE_CLAMP (16093440UL + cODO_OFFSET + (uint32)cPROD_SPEC_RB_MAX_UNITS)
      #endif
    #endif

/*Roll Back display units conversion start*/
#ifdef cPROD_SPEC_RB_DISPLAY_UNITS

 #if cPROD_SPEC_ODO_STORAGE_UNITS != cODO_PKG_KM
  #error storage units must be Km when rollback in display units
 #endif
   
/* define the roll back parameters to use if the display is in miles */
  #if (103 * cPROD_SPEC_RB_MAX_UNITS) > (64 * 255)
    #error cPROD_SPEC_RB_MAX_UNITS is too big for rollback in display units
  #else  
    #define cRB_MAX_UNITS_MILES ((uint16)( 1.609344f * (float32)cPROD_SPEC_RB_MAX_UNITS))
  #endif  

  #define cRB_CEILING_MILES ((uint32)( 1.609344f * (float32)cPROD_SPEC_RB_CEILING))
  
  #if ( 103 * cPROD_SPEC_RB_FLOOR) > (64 * (2*cPROD_SPEC_RB_MAX_UNITS))
    #error cPROD_SPEC_RB_FLOOR is too big for rollback in display units
  #else
    #define cRB_FLOOR_MILES ((uint16)( 1.609344f * (float32)cPROD_SPEC_RB_FLOOR))
  #endif
  
  #if ( 103 * cPROD_SPEC_RB_MAX_PER) > (64 * 255)
    #error cPROD_SPEC_RB_MAX_PER is too big for rollback in display units
  #else
    #define cRB_MAX_PER_MILES ((UINT8)( 1.609344f * (float32)cPROD_SPEC_RB_MAX_PER))
  #endif
#endif
  /* Roll Back display units conversion end*/

#else /*Lhbb cPROD_SPEC_RB_MAX_TIMES Not defined*/
/* Consider the product specific clamp value, if defined            */
 #ifdef cPROD_SPEC_NVM_ODO_VALUE_CLAMP /*Lhbb cPROD_SPEC_NVM_ODO_VALUE_CLAMP Not defined*/
  
  #if (cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_KM)
    #define cNVM_ODO_VALUE_CLAMP (cPROD_SPEC_NVM_ODO_VALUE_CLAMP + cODO_OFFSET)

  #elif (cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_TENTH_KM)
    #error There shall be a chance of NVM Wear Out�� Please revisit the configuration items! 1. cPROD_SPEC_NVM_ODO_VALUE_CLAMP 2. cPROD_SPEC_ODO_STORAGE_FORMAT

  #endif

 #else /*cPROD_SPEC_NVM_ODO_VALUE_CLAMP not defined,Run below*/
  #if (cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_KM)
    #define cNVM_ODO_VALUE_CLAMP (1609344UL + cODO_OFFSET)

  #elif (cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_TENTH_KM)
    #define cNVM_ODO_VALUE_CLAMP (16093440UL + cODO_OFFSET)

  #endif
 #endif /* cPROD_SPEC_NVM_ODO_VALUE_CLAMP*/
#endif

/*
 when TOdoOdoramWholeCounts exceeds update_count_threshold
 odo_write will attempt an NVM update
  the + 1 is necessary to avoid underflow because of fractional part
  see odo_write function below
*/
#define cUPDATE_COUNT_THRESHOLD (cWHOLE_COUNTS_PER_UNIT + (uint16)1)  /*8001*/


typedef struct SOdoCurrentOdoStruct_t
{
    uint16 NvmCount;       /* count value for the data set                   */
    uint8 NvmIncr;         /* increment value for the data set               */
    uint8 NvmSet[2];       /* indices of the two data sets found by OdoRead */
                        	/* index 0 is second most recent                  */
                        	/* index 1 is most recent                         */
    uint32 NvmValue;       /* integer odo value for the set                  */
    uint32 OdoMathTemp;	/* temp work area                     */
}SOdoCurrentOdoStruct;

typedef struct SOdoReadStructType_t
{
    uint16 HoldCount;
    uint8 HoldIncr;  /* note 0 this is an invalid incr value */
    uint8 NSet;
    uint8 PassCntr;
}SOdoReadStructType;

typedef struct SOdoUwordCrc_t
{
  uint16 value;
  uint8 crc;    
}SOdoUwordCrc;

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

static void OdoMdlInit(void);
#if 0
static void OdoMdlWakeUp(void);
#endif
static boolean OdoRead(SOdoCurrentOdoStruct * pCurrentOdo);
static void OdoReadHelper1(SOdoCurrentOdoStruct * pCurrentOdoPd, uint8 * pNsetPd);
static void OdoReadHelper2(SOdoCurrentOdoStruct * pCurrentOdoPd, SOdoReadStructType * pOdoReadStructPd );
static uint16 GetOdoNvmCount(uint8 NSet);
static uint8 GetOfbNvmIncr( uint8 NSet);
static void InternalCurrentOdo(void);
static uint8 OdoAddCounts(uint8 NewRodo);
/*static*/ boolean GetCurrentOdo(uint32 *pCurrentOdoValue , boolean CurrentOdoIsMetric);
static void GetOdoNvmDataSetBytes( const SOdoCurrentOdoStruct *pCurrentOdo );
static void PerformPostUpdateProcessing(void);
static void OdoMdlRunnable(void);
static boolean OdoWrite(void);
static void OdoStartNextUpdateSet (void);
static void OdoDataNVMCallback(uint8 ServiceType , uint8 Result);
static void PutOfbNvmIncr(uint8 incr, uint8 NvmSet);
static void OdoView(void);
static void EMS_Input_Compare(void);
static void EMS_0x584_CanMessage_Compare(void);



/* Math helpers */
static uint8 OdoComputeCrc(uint16 value);
static uint8 OdoComputeCrcLong(uint32 value);
static void RamCheckCrc(const SOdoUwordCrc *p1);
static void RamCheckCrcl(uint32 p1);
static void AddCrcToUlong(uint32 *value);
static uint8 OdoChkParity( uint8 byte);
/*static*/ void ConvertKmToMiles(uint32 *pCurrentOdoValue );
static void Vehicle_cfg_init(void);

#ifdef cODO_OPTIMIZED_RUNNABLE
static boolean OdoReadDummy(SOdoCurrentOdoStruct * pCurrentOdo);
#endif

#ifdef cPROD_SPEC_RESET_STRATEGY  
static boolean OdoPkgDataStabilityCheck(void);
static boolean OdoPkgCRCStabilityCheck(void);
#endif

#ifdef cODO_ALLOW_CHANGE
#if 0
static void OdoChangeCallback(uint8 ServiceType , uint8 Result);
static boolean OdoCheckChangeUp(void);
#endif
static boolean OdoChangeup(uint32 NewOdoValue);
//static uint8 OdoChangeupHighResolution(uint32 NewOdoValue);
#endif

#ifdef cPROD_SPEC_RB_MAX_TIMES
static boolean PerformRollBack(void);
static void OdoRollBackCb(uint8 ServiceType , uint8 Result);
static boolean OdoCheckRollBack(void);
#ifdef cPROD_SPEC_RB_DISPLAY_UNITS
  static boolean OdoRollBack(boolean MetricDisplay);
#else
  static boolean OdoRollBack(void);
#endif
static uint32 OdoRollBackReport(void);
#endif
extern boolean OdoLogicalMdl_Is_Calculate_ODO(void);
void ODO_WriteNVM_Failure_SubFunction(void);
static boolean ODO_EMS_NR_TOUT(void);
static void OdoLogicalMdl_To_EMS(void);
static void Ems_backupodo_TimerIsStart(void);

extern void EepromInit(void);
extern void I2CInit(void);
extern void I2c_ResetSCLSDA_UnLocked(void);
extern boolean PowerManager_GetPowerUpStatus(void);
extern void NvM_Init(void);
boolean EMS_backupodoMeter_NR = FALSE;
//extern uint32  TripB_odo_wheel_increase_value;/*2019GU*/

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
#pragma ghs section bss=".buram"
static SOdoUwordCrc TOdoOdoramWholeCounts;
static uint32 EMS_backupodoMeter = 0;
#pragma ghs section bss=default
static SOdoUwordCrc TOdoOdoRamWholeCountsLast;
static SOdoUwordCrc TOdoOdoOldRodo; /* saved last rolling odo count */
static uint32 TOdoLastGoodE2Odo; /* last good value read from E2 */
//static uint16 TOdoOdoramWholeCountsSave;   /* save until callback complete */
//static uint32 TOdoLastGoodE2OdoSave;      /* save until callback complete */
static uint8 TOdoOdoInternalFlags;

static uint8 TOdoOdoProtectedSet[2];  /* indices of the two sets to be protected from update */

static uint16 TOdoOdoSetBeingUpdated; /* index of the odo data set being updated */
                                    /* range from 0 to (num of sets - 1)       */
                                    /* or equal the num of sets if no update in progress */
static uint32 TOdoCurrentOdoKm;
static uint32 TOdoCurrentOdoMiles;
static uint32 cPROD_SPEC_COUNTS_PER_KM = 0;

//static uint8 TOdoOdoWriteBytes[sizeof(SOdoDataSet)]; /* buffer holding new values */				  
                                                    /* to be placed in a data set */


/* this table has preprocessed crc values for all nibble values */
  static const uint8 TOdoCrcTable[16]={  0, 13,  7, 10, 14,  3,  9,  4,
                                    1, 12,  6, 11, 15,  2,  8,  5 };

/*
 if necessary define and declare the storage area used to deal with the
 fractional parts of input counts
*/
#ifdef cFRAC_COUNTS_PER_UNIT
  static uint16 TOdoOdoramFracCounts;
#endif

/* take into account whether or not roll back is configured */
#ifdef cPROD_SPEC_RB_MAX_TIMES
    static uint16 TOdoRollBackTemp[2] = {0, 0};   /* part of interface to EEPROM Manager */
    static uint16 TOdoRollBackUnits = 0;
    static uint16 TOdoRollBackCount = 0;
#endif

/*
  Take into account when odo Change is configured
*/
#ifdef cODO_ALLOW_CHANGE

//static uint8 TOdoEpmChangeCounter;
//static uint8 TOdoLocalOdoChangeupAmnt;

#endif

#ifdef cPROD_SPEC_RESET_STRATEGY
/* Start of cPROD_SPEC_RESET_STRATEGY */

/*
** Two more RAM copies to be maintained 
** MMC Keep Alive Memory requirement
*/
/******************* Copy 1 ********************************************************/

static SOdoUwordCrc TOdoOdoramWholeCountsCopy1;
static uint8 TOdoOdoSetBeingUpdatedCopy1 = 0;
static uint8 TOdoOdoInternalFlagsCopy1 = 0;
#ifdef cFRAC_COUNTS_PER_UNIT
static uint16 TOdoOdoramFracCountsCopy1 = 0;
#endif
#ifdef cODO_OPTIMIZED_RUNNABLE
static SOdoCurrentOdoStruct TOdoCurrentOdoDefGlobalCopy1;
#endif
static uint32 TOdoLastGoodE2OdoCopy1 = 0;
/***********************************************************************************/
/******************* Copy 2 ********************************************************/


static SOdoUwordCrc TOdoOdoramWholeCountsCopy2;
static uint8 TOdoOdoSetBeingUpdatedCopy2;
static uint8 TOdoOdoInternalFlagsCopy2;
#ifdef cFRAC_COUNTS_PER_UNIT
static uint16 TOdoOdoramFracCountsCopy2;
#endif
#ifdef cODO_OPTIMIZED_RUNNABLE
static SOdoCurrentOdoStruct TOdoCurrentOdoDefGlobalCopy2;
#endif
static uint32 TOdoLastGoodE2OdoCopy2;

/***********************************************************************************/

#endif /* End of cPROD_SPEC_RESET_STRATEGY */

/* Global structure variable which will hold the odo incr and count values */
/* throughout the file. This made global inorder to avoid the continous    */
/* traverse thought the nvm odo set.                                       */
#ifdef cODO_OPTIMIZED_RUNNABLE
static SOdoCurrentOdoStruct TOdoCurrentOdoDefGlobal;
#endif

static uint32 TOdoMainOdoKm ;
static uint32 TOdoMainOdoMile;

/* NvM mapped variable */

static SOdoDataSet OdoDataSet[5];
static SOdoDataSet *pOdoDataSet = &OdoDataSet[0];


#define cGET_SET_ADDR(NSet)    (&pOdoDataSet[NSet])


#if (cPROD_SPEC_ODO_STORAGE_UNITS != cODO_PKG_TENTH_KM) 
  #define cGET_INCR(NSet) (pOdoDataSet[NSet].increments)
#else
  /* 
  * If PROD_SPEC_ODO_STORAGE_UNITS == ODO_PKG_TENTH_KM, then take 
  * Least Significant two bytes from the ODO NVM Data
  */ 
	#define cGET_INCR(NSet) (pOdoDataSet[NSet].extra_count)
#endif 


static boolean l_odomdl_init_status = FALSE;
//extern uint32  TripB_odo_wheel_increase_value;/*2019GU*/
static boolean  l_ems_odometer_update_complete = FALSE;
static uint32 l_ems_backupodometervalue = 0;

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit ( void )
{
  // Ems_backupodo_TimerIsStart();
   return E_OK;
}
    
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation( void )
{
	Vehicle_cfg_init();
    OdoMdlInit();
	//OdoChangeup(0);
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation( void )
{
   return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
#if 1 
    eIgnState fl_ign;
    EBatteryState fl_batt;

	Rte_Read_rpBattState_BatteryState(&fl_batt);
    if ((fl_batt != eBatteryState_OverVoltage) && (fl_batt != eBatteryState_UnderVoltage)) /*overPh1,normal,underPh1*/
    {
        Rte_Read_rpIgnState_IGNState(&fl_ign);
        if(fl_ign == eIGN_RUN)
        {
            OdoMdlRunnable();
        }
        
        (void)OdoView(); /* only write to RTE,No send to displays*/
		OdoLogicalMdl_To_EMS();

    }
    else
    {
        //do nothing;batt high/low not count 
    }

#else
    static uint8 enterflag = 0;
    NVOdoDataSetType odo_data_arry[5];
    NVOdoDataSetType nv_odo_data_arry[5];

    if(1 == enterflag)
    {
        odo_data_arry[0]->count = 0;
        odo_data_arry[0]->extra_count = 0;
        odo_data_arry[0]->increments = 0;
        
        odo_data_arry[1]->count = 0;
        odo_data_arry[1]->extra_count = 0;
        odo_data_arry[1]->increments = 0;
        
        odo_data_arry[2]->count = 0;
        odo_data_arry[2]->extra_count = 0;
        odo_data_arry[2]->increments = 0;
        
        odo_data_arry[3]->count = 0;
        odo_data_arry[3]->extra_count = 0;
        odo_data_arry[3]->increments = 0;  
        
        odo_data_arry[4]->count = 0;
        odo_data_arry[4]->extra_count = 0;
        odo_data_arry[4]->increments = 0; 
        
        Rte_Write_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(odo_data_arry[0]);
        Rte_Write_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(odo_data_arry[1]);
        Rte_Write_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(odo_data_arry[2]);
        Rte_Write_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(odo_data_arry[3]);
        Rte_Write_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(odo_data_arry[4]);
    }
    else if(2 == enterflag)
    {
        (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(nv_odo_data_arry[0]);//Lhbb below five is Rte_MemCpy
        (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(nv_odo_data_arry[1]);
        (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(nv_odo_data_arry[2]);
        (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(nv_odo_data_arry[3]);
        (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(nv_odo_data_arry[4]);
    }
    

#endif

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Diagnostic state
///
/// @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}

///=====================================================================================================================
//  PUBLIC
//=====================================================================================================================

///=====================================================================================================================
// Static helpers
//===================================================================================================================== 

/*--------------------------------------------------------------------------*/
/*  Name:  OdoMdlInit - originally odo_init_at_reset        	            */
/*                                                                          */
/*  Return:     None                                                        */
/*                                                                          */
/*  Parameters:  None                                                       */
/*                                                                          */
/*  Visibility:  OdoMdl                                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This routine performs ODO Package initialization, and must be called    */
/*  prior to any other ODO Package routine,         						*/
/*  									                                     */
/*                                                                          */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      Date     CDSIC    ChangesMade				                        */

/*--------------------------------------------------------------------------*/
static void OdoMdlInit(void)
{

  Mcu_ResetType    ResetReason = Mcu_GetResetReason();
  (void)Rte_Read_NV_OdoMdl_NvMOdoData_OdoDataSetArray(pOdoDataSet); /*OdoRead Get pOdoDataSet*/
  
  SOdoCurrentOdoStruct CurrentOdoDef;
  boolean fl_odo_read_sts_BOOL;

/* initilize the interface that supports the EEPROM Manager */
#if (cPROD_SPEC_ODO_STORAGE_FORMAT==cODO_PKG_OFB_EPM) 

  TOdoOdoInternalFlags &= (uint8)~(cODO_RETRY_UPDATE_REQUEST | cODO_UPDATE_FAILED 
                          | cODO_COUNTS_LOST | cODO_READ_FAILED 
                          | cODO_STARTED_CALLBACK_PROCESSING | cODO_CHANGEUP_PROCESSING); /* clear flAGS */
#else
  TOdoOdoInternalFlags &= (uint8)~(cODO_RETRY_UPDATE_REQUEST | cODO_UPDATE_FAILED  
                          | cODO_COUNTS_LOST | cODO_READ_FAILED |cODO_CHANGEUP_PROCESSING); /* clear flAGS */
#endif

/* Set the RTE transmit odo varaible  to zero */
   TOdoMainOdoKm = 0;
   TOdoMainOdoMile = 0;
   
/* initialize odoram to 0 */
  if((ResetReason != MCU_WAKEUP_RESET) && (ResetReason != MCU_SW_TRIGGER_HW_RESET) && (l_odomdl_init_status == FALSE))
  {
    l_odomdl_init_status = TRUE;
    TOdoOdoramWholeCounts.value = (uint16)0;
  }
  TOdoOdoramWholeCounts.crc = OdoComputeCrc(TOdoOdoramWholeCounts.value);

#ifdef cFRAC_COUNTS_PER_UNIT
  TOdoOdoramFracCounts = (uint16)(cFRAC_COUNTS_PER_UNIT + 1) >> 1;
#endif

/* initialize old rolling odo and apply crc */
  TOdoOdoOldRodo.value = 0x0000;
  TOdoOdoOldRodo.crc = OdoComputeCrc(TOdoOdoOldRodo.value);

  TOdoOdoSetBeingUpdated = cNUMBER_OF_ODO_SETS;
/* get the last odo value in nvm and apply CRC*/

  fl_odo_read_sts_BOOL = OdoRead(&CurrentOdoDef);
  if (fl_odo_read_sts_BOOL != FALSE)
  {                       
    TOdoLastGoodE2Odo = CurrentOdoDef.NvmValue;
  }
  else
  {
    TOdoLastGoodE2Odo = cODO_LWR7_NIBBLES_FF;/*Lhbb No E2,temprary set to 0*/
	//TOdoLastGoodE2Odo = 0;/*Lhbb No E2,temprary set to 0*/
  }

  AddCrcToUlong(&TOdoLastGoodE2Odo);  
/* compute the current odo value */
  InternalCurrentOdo();
  
}
#if 0
/*--------------------------------------------------------------------------*/
/*  Name:  OdoMdlWakeUp                                               */
/*                                                                          */
/*  Return:     void                                                        */
/*                                                                          */
/*  Parameters:  none                                                       */
/*                                                                          */
/*  Visibility:  OdoMdl                                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*                                                                          */
/*  this routine only does something if the E2 manager is used              */
/*    in that case it restarts the call back processing if the flag         */
/*    that call back processing was in prgress is set                       */
/*                                                                          */
/*      1/09/16    	nrajaman          First Version                         */

/*--------------------------------------------------------------------------*/
static void OdoMdlWakeUp(void)
{

	Rte_Read_NV_OdoMdl_NvMOdoData_OdoDataSetArray(pOdoDataSet);

  /* Start of cPROD_SPEC_RESET_STRATEGY */
#ifdef cPROD_SPEC_RESET_STRATEGY   
  boolean FlCrcStatus ;
  boolean FlDataStatus;

  FlCrcStatus = OdoPkgCRCStabilityCheck();
  FlDataStatus = OdoPkgDataStabilityCheck();

  if ((FlCrcStatus == FALSE) || (FlDataStatus == FALSE))
  {
	  /* 
	  ** if the system finds any validity check failure in any of the RAM copies,
	  ** then trigger a cold init to start the accumulation from the stored NVM vlaues
	  */
	  OdoMdlInit();
	}
	else
	{
	  /* The RAM copies are ok, continue odo accumulation from that RAM values */
	}
#endif 
  /* End of cPROD_SPEC_RESET_STRATEGY */

#if (cPROD_SPEC_ODO_STORAGE_FORMAT==cODO_PKG_OFB_EPM) 

/* restart the call back processing */
  if ((TOdoOdoInternalFlags & cODO_STARTED_CALLBACK_PROCESSING) == cODO_STARTED_CALLBACK_PROCESSING)
  {
    TOdoOdoramWholeCounts.value = TOdoOdoramWholeCountsSave;
    TOdoOdoramWholeCounts.crc = OdoComputeCrc(TOdoOdoramWholeCounts.value);
  
#if (defined (cODO_OPTIMIZED_RUNNABLE) && defined (cPROD_SPEC_RESET_STRATEGY))
    TOdoLastGoodE2Odo = TOdoCurrentOdoDefGlobal.NvmValue;
#else
    TOdoLastGoodE2Odo = TOdoLastGoodE2OdoSave;
#endif

    /* Start of cPROD_SPEC_RESET_STRATEGY */
   #ifdef cPROD_SPEC_RESET_STRATEGY
    TOdoOdoramWholeCountsCopy1.value = TOdoOdoramWholeCounts.value;
    TOdoOdoramWholeCountsCopy1.crc = TOdoOdoramWholeCounts.crc;
    #ifndef cODO_OPTIMIZED_RUNNABLE
    TOdoLastGoodE2OdoCopy1 = TOdoLastGoodE2Odo;
	#endif
    TOdoOdoramWholeCountsCopy2.value = TOdoOdoramWholeCounts.value;
    TOdoOdoramWholeCountsCopy2.crc = TOdoOdoramWholeCounts.crc;
    #ifndef cODO_OPTIMIZED_RUNNABLE
    TOdoLastGoodE2OdoCopy2 = TOdoLastGoodE2Odo;
    #endif
   #endif 
   /* End of cPROD_SPEC_RESET_STRATEGY */
  }
#endif

  InternalCurrentOdo();
}
#endif
/* Start of cPROD_SPEC_RESET_STRATEGY */
#ifdef cPROD_SPEC_RESET_STRATEGY  
/*--------------------------------------------------------------------------*/
/*  Name: OdoPkgCRCStabilityCheck                                       */
/*                                                                          */
/*  Return: boolean                                                         */
/*                                                                          */
/*  Parameters: none                                                        */
/*                                                                          */
/*  Visibility: none                                                        */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This function is used to verify CRC checksum for                        */
/*  cPROD_SPEC_RESET_STRATEGY. Returns TRUE if checksum is correct else      */
/*  it will return FALSE                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static boolean OdoPkgCRCStabilityCheck(void)
{
  boolean FlCrcResult = TRUE;

#ifndef cODO_OPTIMIZED_RUNNABLE
  /* check master and two copies CRC values */
  if((OdoComputeCrcLong(TOdoLastGoodE2Odo) & cODO_BAD_CRCL) != FALSE)
  {
    return (FALSE);
  }
  else if((OdoComputeCrcLong(TOdoLastGoodE2OdoCopy1) & cODO_BAD_CRCL) != FALSE)
  {
    return (FALSE);
  }
  else if((OdoComputeCrcLong(TOdoLastGoodE2OdoCopy2) & cODO_BAD_CRCL) != FALSE)
  {
    return (FALSE);
  }
  else
  {
    /* Do nothing */
  }
#endif
  /* check master and two copies CRC values */
  if(OdoComputeCrc(TOdoOdoramWholeCounts.value) != TOdoOdoramWholeCounts.crc)
  {
    return (FALSE);
  }
  else if(OdoComputeCrc(TOdoOdoramWholeCountsCopy1.value) != TOdoOdoramWholeCountsCopy1.crc)
  {
    return (FALSE);
  }
  else if(OdoComputeCrc(TOdoOdoramWholeCountsCopy2.value) != TOdoOdoramWholeCountsCopy2.crc)
  {
    return (FALSE);
  }
  else
  {
    /* Do nothing */
  }

  return FlCrcResult;
}
/*--------------------------------------------------------------------------*/
/*  Name: OdoPkgDataStabilityCheck                                      */
/*                                                                          */
/*  Return: boolean                                                         */
/*                                                                          */
/*  Parameters: none                                                        */
/*                                                                          */
/*  Visibility: none                                                        */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This function is used to verify data values for                         */
/*  cPROD_SPEC_RESET_STRATEGY. Returns TRUE if checksum is correct else      */
/*  it will return FALSE                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static boolean OdoPkgDataStabilityCheck(void)
{
  boolean FlDataResult = TRUE;

#ifndef cODO_OPTIMIZED_RUNNABLE
  if(TOdoLastGoodE2Odo != TOdoLastGoodE2OdoCopy1)
  {
    if(TOdoLastGoodE2Odo != TOdoLastGoodE2OdoCopy2)
    {
      if(TOdoLastGoodE2OdoCopy1 != TOdoLastGoodE2OdoCopy2)
      {
        return (FALSE);
      }
      else
      {
        TOdoLastGoodE2Odo = TOdoLastGoodE2OdoCopy1;
        AddCrcToUlong(&TOdoLastGoodE2Odo);
      }
    }
  }
#endif
  if(TOdoOdoSetBeingUpdated != TOdoOdoSetBeingUpdatedCopy1)
  {
    if(TOdoOdoSetBeingUpdated != TOdoOdoSetBeingUpdatedCopy2)
    {
      if(TOdoOdoSetBeingUpdatedCopy1 != TOdoOdoSetBeingUpdatedCopy2)
      {
        return (FALSE);
      }
      else
      {
        TOdoOdoSetBeingUpdated = TOdoOdoSetBeingUpdatedCopy1;
      }
    }
  }
  if(TOdoOdoramWholeCounts.value != TOdoOdoramWholeCountsCopy1.value)
  {
    if(TOdoOdoramWholeCounts.value != TOdoOdoramWholeCountsCopy2.value)
    {
      if(TOdoOdoramWholeCountsCopy1.value != TOdoOdoramWholeCountsCopy2.value)
	  {
        return (FALSE);
	  }
	  else
	  {
	     TOdoOdoramWholeCounts.value = TOdoOdoramWholeCountsCopy1.value;
		 TOdoOdoramWholeCounts.crc = OdoComputeCrc(TOdoOdoramWholeCounts.value);
	  }
	}
  }
  if(TOdoOdoInternalFlags != TOdoOdoInternalFlagsCopy1)
  {
    if(TOdoOdoInternalFlags != TOdoOdoInternalFlagsCopy2)
    {
      if(TOdoOdoInternalFlagsCopy1 != TOdoOdoInternalFlagsCopy2)
	  {
        return (FALSE);
	  }
	  else
	  {
	    TOdoOdoInternalFlags = TOdoOdoInternalFlagsCopy1;
	  }
	}
  }
#ifdef cFRAC_COUNTS_PER_UNIT
  if(TOdoOdoramFracCounts != TOdoOdoramFracCountsCopy1)
  {
    if(TOdoOdoramFracCounts != TOdoOdoramFracCountsCopy2)
    {
      if(TOdoOdoramFracCountsCopy1 != TOdoOdoramFracCountsCopy2)
      {
        return (FALSE);
	  }
	  else
	  {
	    TOdoOdoramFracCounts = TOdoOdoramFracCountsCopy1;
	  }
	}
  }
#endif
#ifdef cODO_OPTIMIZED_RUNNABLE
  if(TOdoCurrentOdoDefGlobal.NvmCount != TOdoCurrentOdoDefGlobalCopy1.NvmCount)
  {
    if(TOdoCurrentOdoDefGlobal.NvmCount != TOdoCurrentOdoDefGlobalCopy2.NvmCount)
    {
      if(TOdoCurrentOdoDefGlobalCopy1.NvmCount != TOdoCurrentOdoDefGlobalCopy2.NvmCount)
	  {
        return (FALSE);
	  }
	  else
	  {
	    TOdoCurrentOdoDefGlobal.NvmCount = TOdoCurrentOdoDefGlobalCopy1.NvmCount;
	  }
	}
  }
  if(TOdoCurrentOdoDefGlobal.NvmIncr != TOdoCurrentOdoDefGlobalCopy1.NvmIncr)
  {
    if(TOdoCurrentOdoDefGlobal.NvmIncr != TOdoCurrentOdoDefGlobalCopy2.NvmIncr)
    {
      if(TOdoCurrentOdoDefGlobalCopy1.NvmIncr != TOdoCurrentOdoDefGlobalCopy2.NvmIncr)
      {
        return (FALSE);
	  }
	  else
	  {
	    TOdoCurrentOdoDefGlobal.NvmIncr = TOdoCurrentOdoDefGlobalCopy1.NvmIncr;
	  }
	}
  }
  if(TOdoCurrentOdoDefGlobal.NvmSet[0] != TOdoCurrentOdoDefGlobalCopy1.NvmSet[0])
  {
    if(TOdoCurrentOdoDefGlobal.NvmSet[0] != TOdoCurrentOdoDefGlobalCopy2.NvmSet[0])
    {
      if(TOdoCurrentOdoDefGlobalCopy1.NvmSet[0] != TOdoCurrentOdoDefGlobalCopy2.NvmSet[0])
	  {
        return (FALSE);
	  }
	  else
	  {
	    TOdoCurrentOdoDefGlobal.NvmSet[0] = TOdoCurrentOdoDefGlobalCopy1.NvmSet[0];
	  }
	}
  }
  if(TOdoCurrentOdoDefGlobal.NvmSet[1] != TOdoCurrentOdoDefGlobalCopy1.NvmSet[1])
  {
    if(TOdoCurrentOdoDefGlobal.NvmSet[1] != TOdoCurrentOdoDefGlobalCopy2.NvmSet[1])
    {
      if(TOdoCurrentOdoDefGlobalCopy1.NvmSet[1] != TOdoCurrentOdoDefGlobalCopy2.NvmSet[1])
      {
        return (FALSE);
	  }
	  else
	  {
	    TOdoCurrentOdoDefGlobal.NvmSet[1] = TOdoCurrentOdoDefGlobalCopy1.NvmSet[1];
	  }
	}
  }
  if(TOdoCurrentOdoDefGlobal.NvmValue != TOdoCurrentOdoDefGlobalCopy1.NvmValue)
  {
    if(TOdoCurrentOdoDefGlobal.NvmValue != TOdoCurrentOdoDefGlobalCopy2.NvmValue)
    {
      if(TOdoCurrentOdoDefGlobalCopy1.NvmValue != TOdoCurrentOdoDefGlobalCopy2.NvmValue)
      {
        return (FALSE);
	  }
	  else
	  {
	    TOdoCurrentOdoDefGlobal.NvmValue = TOdoCurrentOdoDefGlobalCopy1.NvmValue;
	  }
	}
  }
#endif
  return FlDataResult;
}
#endif  
/* End of cPROD_SPEC_RESET_STRATEGY */

/*--------------------------------------------------------------------------*/
/*  Name:  OdoRead                                                         */
/*                                                                          */
/*  Return:     boolean -  status of the read operation                     */
/*                                                                          */
/*  Parameters:  SOdoCurrentOdoStruct * current_odo - address of area in which*/
/*                                                  to store most recent    */
/*                                                  sets information        */
/*                                                                          */
/*  Visibility:  no external visibility                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  this routine finds the most recent two data sets that have an acceptable*/
/*  relationship (less than cODORLCON difference in value) and stores the    */
/*  information from the most recent in to current_odo                      */
/*                                                                          */
/*  the basic algorithm is to find the two largest values and then check for*/
/*  the proper relationship                                                 */
/*                                                                          */
/*  if the relationship check fails then the next highest pair of values    */
/*  are found and their relationship is checked.  This process continues    */
/*  until a pair passes the relationship check or until all pairs have been */
/*  checked.                                                                */
/*                                                                          */
/*  true is returned for the first valid pair to pass the relationship test */
/*  false is returned if there are no valid pairs or if no pair passes the  */
/*  relationship check                                                      */
/*                                                                          */
/*  when this routine returns true then the structure current_odo will      */
/*  contain the increment and count values for the most recent data set     */
/*  the value of the data set and the index values for the two most recent  */
/*  data sets                                                               */
/*                                                                          */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      07/17/98     Bill Derouchie    First Version                        */
/*                                                                          */
/*      02/17/00     WFD        Modified to not use the data set that is    */
/*                              being updated by the EEPROM Manager.  This  */
/*                              is necessary since the RAM shadow is updated*/
/*                              when the request is queued, but             */
/*                              TOdoOdoramWholeCounts is not updated until the*/
/*                              request is succesfully completed.  This is  */
/*                              part of the interface to the EEPROM manager */
/*                                                                          */
/*      09/20/2     WFD         Moved code from the original function into  */
/*                              two new helper functions.  This dramatically*/
/*                              reduced the number of static paths.         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static boolean OdoRead( SOdoCurrentOdoStruct * pCurrentOdo)
{
    SOdoReadStructType OdoReadStruct = {0, 0, cNUMBER_OF_ODO_SETS, 0};
    /* mark the 2nd most recent as being bad */
    pCurrentOdo->NvmSet[0] = cODO_PKG_INVALID_SET;

  OdoReadHelper1(pCurrentOdo, &OdoReadStruct.NSet);
  
/* if there are other data sets left, then can continue, else can not */
  if ( OdoReadStruct.NSet > 0)
  {
   do          /* loop trying to find valid pairs with proper relationship */
   {
    OdoReadHelper2(pCurrentOdo, &OdoReadStruct);
/* see if have found a second biggest */
    if ( pCurrentOdo->NvmSet[0] != cODO_PKG_INVALID_SET)
    {
      if (( (pCurrentOdo->NvmCount == OdoReadStruct.HoldCount)
          && ( ((sint16)pCurrentOdo->NvmIncr
               - (sint16)OdoReadStruct.HoldIncr - (sint16)cODORLCON)
               < 0) )
         || (((pCurrentOdo->NvmCount - OdoReadStruct.HoldCount) == (uint16)1)
               && ( ((sint16)OdoReadStruct.HoldIncr
                    - (sint16)pCurrentOdo->NvmIncr - (sint16)cODORLCON2)
                     > 0) ) )
      {
/* compute current nvm value from the count and increment values */
        pCurrentOdo->NvmValue = (uint32)((pCurrentOdo->NvmCount * cTOTAL_ODO_INCRS)
                                 + pCurrentOdo->NvmIncr);

        TOdoOdoInternalFlags &= (uint8)~cODO_READ_FAILED;
        return TRUE;
      }
      else
      {      /* failed realtionship check */
/* bump the pass counter */
        OdoReadStruct.PassCntr++;

/* move 2nd to the biggest */
        pCurrentOdo->NvmCount = OdoReadStruct.HoldCount;
        pCurrentOdo->NvmIncr = OdoReadStruct.HoldIncr;
        pCurrentOdo->NvmSet[1] = pCurrentOdo->NvmSet[0];

/* mark 2nd as not yet found */
        pCurrentOdo->NvmSet[0] = cODO_PKG_INVALID_SET;
        OdoReadStruct.HoldIncr = 0;
        OdoReadStruct.HoldCount = 0;

        OdoReadStruct.NSet = cNUMBER_OF_ODO_SETS;
/* go and try to find new 2nd */
      }
    }
    else /* no 2nd biggest found */
    {
      break;                                 /* this ends the search        */
                                             /* when not all sets are valid */
    }
/* CYC:BEGIN_TEST_DO_WHILE MAX_ITER=const(cyc_num_odo_sets_mns3) EXP_ITER=const(cyc_num_odo_sets_div2) */
   } while ( OdoReadStruct.PassCntr <  (cNUMBER_OF_ODO_SETS-3));/* this ends the search   */
/* CYC:END_TEST_DO_WHILE */

  }                                               /* when all sets are valid*/
  else /* nothing to work with, no valid sets */
  {
  }
  TOdoOdoInternalFlags |= cODO_READ_FAILED;
  return FALSE;
}

static void OdoReadHelper1( SOdoCurrentOdoStruct * pCurrentOdoPd, uint8 * pNsetPd)
{
  do
  {
     *pNsetPd = *pNsetPd - 1;
     if ( (*pNsetPd == TOdoOdoSetBeingUpdated) 
       && ((TOdoOdoInternalFlags & cODO_RETRY_UPDATE_REQUEST) != cODO_RETRY_UPDATE_REQUEST) )
     {
       continue; /* this one is being updated */
     }
     pCurrentOdoPd->NvmSet[1] = *pNsetPd;
     pCurrentOdoPd->NvmCount = GetOdoNvmCount(*pNsetPd);

     pCurrentOdoPd->NvmIncr = GetOfbNvmIncr(*pNsetPd);


     if ( (pCurrentOdoPd->NvmCount != cBAD_ODO_VALUE_W)
         && (pCurrentOdoPd->NvmIncr > 0 )
         && (pCurrentOdoPd->NvmIncr <= cTOTAL_ODO_INCRS))
     {
/* valid set has been found */
        break;
     }
/* CYC:BEGIN_TEST_DO_WHILE MAX_ITER=const(cyc_num_odo_sets) EXP_ITER=1 */
  }  while (*pNsetPd > 0);
/* CYC:END_TEST_DO_WHILE */

}

static void OdoReadHelper2(SOdoCurrentOdoStruct * pCurrentOdoPd, SOdoReadStructType * pOdoReadStructPd )
{
  uint16 temp_word;
  uint8 temp_byte;
  
    do         /* loop trying to find highest valid pairs */
    {
      pOdoReadStructPd->NSet--;
      if ( (pOdoReadStructPd->NSet == TOdoOdoSetBeingUpdated) 
        && ((TOdoOdoInternalFlags & cODO_RETRY_UPDATE_REQUEST) != cODO_RETRY_UPDATE_REQUEST) )
      {
        continue; /* this one is being updated */
      }

      temp_byte = GetOfbNvmIncr(pOdoReadStructPd->NSet);

      if ( ( temp_byte > 0 ) && ( temp_byte <= cTOTAL_ODO_INCRS ) )
      {

        temp_word = GetOdoNvmCount(pOdoReadStructPd->NSet);
        if (temp_word != cBAD_ODO_VALUE_W)
        {
/* set is ok, so see if it is bigger than previous biggest */
          if ( ( (temp_word == pCurrentOdoPd->NvmCount)
                 && (temp_byte > pCurrentOdoPd->NvmIncr))
              ||  (temp_word > pCurrentOdoPd->NvmCount) )
          {
            if ( pOdoReadStructPd->PassCntr == 0 )  			/* can only find new biggest on first pass  */
            {                      								/* on other passes the bigger ones are ones */
			                                   					/* that failed relationship tests           */
																/* transfer old biggest to second biggest */
              pOdoReadStructPd->HoldCount = pCurrentOdoPd->NvmCount;
              pOdoReadStructPd->HoldIncr = pCurrentOdoPd->NvmIncr;
              pCurrentOdoPd->NvmSet[0] = pCurrentOdoPd->NvmSet[1];

				/* save as new biggest */
              pCurrentOdoPd->NvmSet[1] = pOdoReadStructPd->NSet;
              pCurrentOdoPd->NvmCount = temp_word;
              pCurrentOdoPd->NvmIncr = temp_byte;
            }
          }
          else
          {
/* if not biggest, see if it is bigger than the second biggest */
            if ( ( ( (temp_word == pOdoReadStructPd->HoldCount) && (temp_byte >= pOdoReadStructPd->HoldIncr))
                || (temp_word > pOdoReadStructPd->HoldCount) ) && (pOdoReadStructPd->NSet != pCurrentOdoPd->NvmSet[1]))
            {
              pOdoReadStructPd->HoldCount = temp_word;
              pOdoReadStructPd->HoldIncr = temp_byte;
              pCurrentOdoPd->NvmSet[0] = pOdoReadStructPd->NSet;
            }
          }
        }
      }
/* CYC:BEGIN_TEST_DO_WHILE MAX_ITER=const(cyc_num_odo_sets_mns3) EXP_ITER=1 */
    } while (pOdoReadStructPd->NSet > 0 );
/* CYC:END_TEST_DO_WHILE */

}

#ifdef cODO_OPTIMIZED_RUNNABLE
/*--------------------------------------------------------------------------*/
/*  Name:  OdoReadDummy                                                   */
/*                                                                          */
/*  Return:     Returns TRUE, if nvm count and incr which is read are       */
/*              correct                                                     */
/*                                                                          */
/*  Parameters: SOdoCurrentOdoStruct * - current odo structure                */
/*                                                                          */
/*  Visibility:  no external visibility                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This routine retrives the incr and count values of the latest valid     */
/*  odo nvm set. This toutine will confirm the validity of the same.        */
/*  If the values are wrong, returns FALSE, and sets the cODO_READ_FAILED    */
/*  flag.                                                                   */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*     07/26/2012     Sherin V Sebastian    First Version                   */
/*     1/09/16    	nrajaman           Modified to Turing  Architecture     */
/*--------------------------------------------------------------------------*/
static BOOLEAN OdoReadDummy(const SOdoCurrentOdoStruct * pCurrentOdo)
{
    if(pCurrentOdo->NvmSet[1] >= cNUMBER_OF_ODO_SETS)
    {
        /* Point to first odo nvm set, if it reached the maximum */
        pCurrentOdo->NvmSet[1] = 0;
    }
    /* get the incr value from the pointing odo nvm set */
    pCurrentOdo->NvmIncr = GetOfbNvmIncr(pCurrentOdo->NvmSet[1]);

    /* get the count value from the pointing odo nvm set */
    pCurrentOdo->NvmCount = GetOdoNvmCount(pCurrentOdo->NvmSet[1]);

    /* check the validity of incr and count values which is read */
     if ( (pCurrentOdo->NvmCount != cBAD_ODO_VALUE_W)
         && (pCurrentOdo->NvmIncr > 0 )
         && (pCurrentOdo->NvmIncr <= cTOTAL_ODO_INCRS))
     {
          /* Calculate the nvm value if the read count and incr are valid */
		     /* NvmValue = (count * cTOTAL_ODO_INCRS) + incr                 */
          pCurrentOdo->NvmValue = (uint32)pCurrentOdo->NvmCount;
          
          pCurrentOdo->OdoMathTemp = (pCurrentOdo->NvmValue * cTOTAL_ODO_INCRS);
          
          pCurrentOdo->NvmValue = pCurrentOdo->OdoMathTemp
                                   + pCurrentOdo->NvmIncr;
                                   
          /* Clear cODO_READ_FAILED flag */
          TOdoOdoInternalFlags &= (uint8)~cODO_READ_FAILED;
          return TRUE;
     }
     else
     {
          /* Set cODO_READ_FAILED flag, if the read incr or count value is not valid */
          TOdoOdoInternalFlags |= (uint8)cODO_READ_FAILED;
          return FALSE;
     }
}
#endif

/*--------------------------------------------------------------------------*/
/*  Name:  GetOdoNvmCount                                                */
/*                                                                          */
/*  Return:     uint16 -  16 bit count value                                 */
/*                                                                          */
/*  Parameters:  uint8  NSet - the 0 based number of the data set           */
/*                                                                          */
/*  Visibility:  no external visibility                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  this routine extracts the 16 bit count value for the data set from the  */
/*  the extra_count byte and the count word.  the crc is computed           */
/*  and checked against the stored version.  if the crc matches then        */
/*  the 16 bit count is returned, else the value 0xFFFF is returned         */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      07/17/98     Bill Derouchie    First Version                        */
/*      12/07/98     Bill Derouchie    created distinct versions for NEC and*/
/*                                     Motorola                             */
/*      02/17/00     Bill Derouchie    reverted to single version for       */
/*                                     ODO-02                               */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static uint16 GetOdoNvmCount(uint8 NSet)
{
  uint8 ThisCrc;
  uint16 TempWord;
  uint8 FlTempCrc;
  SOdoDataSet *pTempPtr;

/* get the address of the data set    */
  pTempPtr = ((SOdoDataSet *)((void *)cGET_SET_ADDR(NSet)));

/* extract the stored crc             */
  ThisCrc = (uint8)((uint8)(pTempPtr->count) & (uint8)0x0F);

/* form the 16 bit value              */
  TempWord = ((pTempPtr->count)>>4)
       | (((uint16)(~pTempPtr->extra_count) & (uint16)0xF0) * (uint16)256 ); /* note 1s compliment */

/* if crcs match then return the value */
  FlTempCrc = OdoComputeCrc(TempWord);	
  if ( FlTempCrc == ThisCrc )
  {
    return TempWord;
  }
  else
  {
    return cBAD_ODO_VALUE_W;
  }
}

/*--------------------------------------------------------------------------*/
/*  Name:  GetOfbNvmIncr                                                 */
/*                                                                          */
/*  Return:     uint8 -  8 bit increment value                              */
/*                                                                          */
/*  Parameters:  uint8  NSet - the 0 based number of the data set           */
/*                                                                          */
/*  Visibility:  no external visibility                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This routine determines if the 8 bit increment value stored in NVM has  */
/*  correct parity(odd).  If correct it returns the increment value adjusted*/
/*  to range from 1 to 128, rather than 0 to 127.  If the parity is not     */
/*  correct, then the illegal value of zero is returned.                    */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      12/18/2000   Bill Derouchie    First Version                        */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#if (cPROD_SPEC_ODO_STORAGE_UNITS != cODO_PKG_TENTH_KM)
static uint8 GetOfbNvmIncr( uint8 NSet)
{ 
  uint8 FlTempParity;
  uint8 temp = cGET_INCR(NSet);
  
  FlTempParity = OdoChkParity(temp);
  if (FlTempParity == 0)
  {
    return 0; /* failed parity check */
  }
  else
  {
    return ((temp & 0x7F) + 1); /* parity was ok */
  }
}  
#else
/* Increment Value Calculation for cODO_PKG_TENTH_KM */
static uint8 GetOfbNvmIncr( uint8 NSet)
{ 
  uint16 temp;
  uint16 NewIncrLimit;
  uint8 LowerByte;
  uint8 OfbNvmIncr;
  
  #ifdef cODO_PKG_LITTLE_ENDIAN
  SOdoDataSet *TempPtr;
  #endif
  
  #ifdef cODO_PKG_BIG_ENDIAN
  /* Read 2 MS Bytes from the Current ODO Set */
  temp = (UWORD)cGET_INCR(NSet);
  #else
  /* get the address of the data set    */
  TempPtr  = ((SOdoDataSet *)((void *)cGET_SET_ADDR(NSet)));
  temp = (((uint16)((uint16)TempPtr->extra_count << 8)) | TempPtr->increments);
  #endif
  

  /* 
  *  Form the new Increment Value 
  *  Extra Increment Bit  (Bit - 8)
  *        Increment Byte (Bit 6-0) 
  */
  NewIncrLimit =  ( (temp >> 1) & (0xFF80UL) );
  NewIncrLimit |=  (temp & 0x007F);
  LowerByte = (uint8)(NewIncrLimit & 0x00FF);

  if (OdoChkParity(LowerByte) != (uint8)((temp & 0x0080) >> 7))
  {
    OfbNvmIncr = 0; /* failed parity check */
  }
  else
  {
    LowerByte++;
    OfbNvmIncr = LowerByte;
  }

  return(OfbNvmIncr);
}  
#endif  /* End of cODO_PKG_TENTH_KM */

/*--------------------------------------------------------------------------*/
/*  Name:  InternalCurrentOdo                                               */
/*                                                                          */
/*  Return:     none                                                        */
/*                       successfully computed                              */
/*                       FALSE otherwise                                    */
/*                                                                          */
/*  Parameters:  uint32 *CurrentOdoValue  Address where the current odo     */
/*                                         value will be stored.            */
/*                                                                          */
/*  Visibility:  none                                                       */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This routine can be called at any time and if successful will provide   */
/*  the current total odo value.  This value includes the most recent NVM   */
/*  value and the counts accumulated in TOdoOdoramWholeCounts.  The scaling */
/*  of the value is defined by the macro cCURRENT_ODO_UNITS_SCALING         */
/*                                                                          */
/*  This routine must not be called at ISR level                            */
/*                                                                          */
/*  The computation of the current odo value is the following but in a more */
/*  efficient manner than relying on the compilers intrinsic functions      */
/*                                                                          */
/*  CurrentOdoValue                                                         */
/*       = ( current_odo.NvmValue                                           */
/*           * cCURRENT_ODO_UNITS_SCALING                                   */
/*           * cPROD_SPEC_ODO_STORAGE_INCR)                                 */
/*         + ( TOdoOdoramWholeCounts                                        */
/*           * cCURRENT_ODO_UNITS_SCALING)                                  */
/*           /cWHOLE_COUNTS_PER_UNIT;                                       */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      01/22/02     Bill Derouchie    First Version                        */
/*                                                                          */
/*      07/07/09     nashokku          Compiler switch is corrected used    */
/*                                     for mul_ulong_by_ubyte.              */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture     */
/*--------------------------------------------------------------------------*/
static void InternalCurrentOdo(void)
{
  SOdoCurrentOdoStruct CurrentOdoDef;
  uint32 CurrentOdoValue ;


  CurrentOdoDef.OdoMathTemp = 0UL;


/* check the crc for TOdoLastGoodE2Odo */
  RamCheckCrcl(TOdoLastGoodE2Odo);
  CurrentOdoDef.NvmValue = TOdoLastGoodE2Odo & cODO_LWR7_NIBBLES_FF;

  if (CurrentOdoDef.NvmValue ==  cODO_LWR7_NIBBLES_FF)
  {
    TOdoCurrentOdoMiles = cODO_LWR7_NIBBLES_FF;
    TOdoCurrentOdoKm = cODO_LWR7_NIBBLES_FF;
  }
  else
  {
/* do not need to check the crc for TOdoOdoramWholeCounts */
/* all callers have just set it or validated it prior to calling this routine */

    CurrentOdoValue = TOdoOdoramWholeCounts.value;

    CurrentOdoDef.OdoMathTemp =
                      CurrentOdoValue *
                      (uint32)(cCURRENT_ODO_UNITS_SCALING); 

/* next divide to get fractional part contributed by odoram */
    CurrentOdoValue = CurrentOdoDef.OdoMathTemp
                       /(cWHOLE_COUNTS_PER_UNIT/cPROD_SPEC_ODO_STORAGE_INCR);

/* offset the nvm value to get actual value           */
#ifdef cPROD_SPEC_RB_MAX_TIMES
    if ( CurrentOdoDef.NvmValue < (cODO_OFFSET + (uint32)TOdoRollBackUnits))
#else
    if ( CurrentOdoDef.NvmValue < cODO_OFFSET)
#endif
    {
       CurrentOdoDef.NvmValue = 0;
    }
    else
    {
#ifdef cPROD_SPEC_RB_MAX_TIMES
      CurrentOdoDef.NvmValue = CurrentOdoDef.NvmValue
                                - (cODO_OFFSET + (uint32)TOdoRollBackUnits);
#else
      CurrentOdoDef.NvmValue = CurrentOdoDef.NvmValue - cODO_OFFSET;
#endif
    }

    CurrentOdoDef.OdoMathTemp =
                      CurrentOdoDef.NvmValue *
                      (uint32)(cCURRENT_ODO_UNITS_SCALING);

/* sum the nvm value and the contribution by odo ram */
    CurrentOdoValue = CurrentOdoValue
      + CurrentOdoDef.OdoMathTemp;

    TOdoCurrentOdoKm = CurrentOdoValue;
    TOdoCurrentOdoMiles = TOdoCurrentOdoKm;
    ConvertKmToMiles(&TOdoCurrentOdoMiles);
  
  }

  AddCrcToUlong(&TOdoCurrentOdoMiles); 
  AddCrcToUlong(&TOdoCurrentOdoKm);

  TOdoOdoRamWholeCountsLast.value = TOdoOdoramWholeCounts.value;
  TOdoOdoRamWholeCountsLast.crc = TOdoOdoramWholeCounts.crc;
}

/*--------------------------------------------------------------------------*/
/*  Name:  OdoAddCounts                                                     */
/*                                                                          */
/*  Return:     void                                                        */
/*                                                                          */
/*  Parameters: uint16 NewRodo the new rolling odo count                   */
/*                                                                          */
/*                                                                          */
/*  Visibility:  OdoMdl                                                 */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  this routine performs GM style processing of incoming rolling odo value */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      02/23/01     Bill Derouchie    First Version                        */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture    */
/*--------------------------------------------------------------------------*/
static uint8 OdoAddCounts(uint8 NewRodo)
{
  uint8 OdoDelta;
 // uint8 temp_old_rodo;
  uint16 temp;

  
/* check the crc for TOdoOdoramWholeCounts */
  RamCheckCrc(&TOdoOdoramWholeCounts);

/* check the crc for TOdoOdoOldRodo */
  RamCheckCrc(&TOdoOdoOldRodo);

#if 0  //wli32
  temp_old_rodo = (uint8)(TOdoOdoOldRodo.value & 0x00FF); /*只取*/
  OdoDelta = NewRodo - temp_old_rodo;/*odo_delta输入值l_distance_counts_U8一直在累加，所以减去上一次保存的值，才是Delta值*/
#else
    OdoDelta = NewRodo;
#endif

  #ifdef cODO_JUMP_LIMIT 
    if (OdoDelta > cMAX_ODO_JUMP_LIMIT )     
    {  
      OdoDelta = cMAX_ODO_JUMP_LIMIT;
    }
  #endif
    
/* look for impending roll over of TOdoOdoramWholeCounts */
#if 0  
  if ((TOdoOdoramWholeCounts.value > ((uint16)65279u)) || ((TOdoOdoInternalFlags & cODO_COUNTS_LOST) == cODO_COUNTS_LOST) )
  {
    TOdoOdoInternalFlags |= cODO_COUNTS_LOST;   /* set flag */
	//TOdoOdoInternalFlags |= cODO_STARTED_CALLBACK_PROCESSING;
    return OdoDelta;                       /* and leave */
  }
    if(TOdoOdoramWholeCounts.value > ((uint16)65279u))
    {
        return OdoDelta;
    }
#endif

/* accumulate the counts */
  
  TOdoOdoramWholeCounts.value += OdoDelta;
  TOdoOdoramWholeCounts.crc = OdoComputeCrc(TOdoOdoramWholeCounts.value);
  

  #ifdef cPROD_SPEC_RESET_STRATEGY
  TOdoOdoramWholeCountsCopy1.value = TOdoOdoramWholeCounts.value;
  TOdoOdoramWholeCountsCopy2.value = TOdoOdoramWholeCounts.value;
  TOdoOdoramWholeCountsCopy1.crc = TOdoOdoramWholeCounts.crc;
  TOdoOdoramWholeCountsCopy2.crc = TOdoOdoramWholeCounts.crc;
  #endif
  
/* save the new as the old for the next time */
  TOdoOdoOldRodo.value = NewRodo;
  TOdoOdoOldRodo.crc = OdoComputeCrc(TOdoOdoOldRodo.value);

/* see if time to update the current odo values */
  RamCheckCrc(&TOdoOdoRamWholeCountsLast);

/* set up so that cCOUNTS_DELTA_COMP can be an explicit number or a calculation */
/* and structure so that there will be no compiler warnings when cCOUNTS_DELTA_COMP is explicitly zero */
  temp = cCOUNTS_DELTA_COMP;
  if (temp == 0)
  {
    temp++;
  }
  if ((TOdoOdoramWholeCounts.value - TOdoOdoRamWholeCountsLast.value) >=  temp)
  {
     InternalCurrentOdo();
  }

  return OdoDelta;
}

 
/*--------------------------------------------------------------------------*/
/*  Name:  GetCurrentOdo                                                  */
/*                                                                          */
/*  Return:     boolean  Set TRUE when the current odo value could be       */
/*                       successfully computed                              */
/*                       FALSE otherwise                                    */
/*                                                                          */
/*  Parameters:  uint32 *CurrentOdoValue  Address where the current odo    */
/*                                         value will be stored.            */
/*                                         If unsuccessfull the value       */
/*                                         0x7FFFFFFF is stored             */
/*                                                                          */
/*               boolean current_odo_is_metric If TRUE current odo vlaue    */
/*                                             will be kilometers, miles    */
/*                                             otherwise                    */
/*                                                                          */
/*  Visibility:  OdoMdl                                                  */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This routine can be called at any time and if successful will provide   */
/*  the current total odo value.  This value includes the most recent NVM   */
/*  value and the counts accumulated in TOdoOdoramWholeCounts.  The scaling   */
/*  of the value is defined by the macro cCURRENT_ODO_UNITS_SCALING          */
/*                                                                          */
/*  This routine must not be called at ISR level                            */
/*                                                                          */
/*  The computation of the current odo value is done as necessary by the    */
/*  OdoAddCounts routine using the InternalCurrentOdo routine.          */
/*  The current odo in miles and km are stored with associated CRCs         */
/*  If the crcs are good then this routine will return the appropriate      */
/*  value to the caller                                                     */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      07/17/98     Bill Derouchie    First Version                        */
/*                                                                          */
/*--------------------------------------------------------------------------*/

/*static*/ boolean GetCurrentOdo(uint32 *pCurrentOdoValue , boolean CurrentOdoIsMetric)
{

/* if odo is bad return false and set the value very large */
  /*Lhbb TOdoOdoInternalFlags=0;*/
  //TOdoOdoInternalFlags &= ~ (cODO_COUNTS_LOST | cODO_READ_FAILED);/*Lhbb temp*/
  if ( (TOdoOdoInternalFlags & (cODO_COUNTS_LOST | cODO_READ_FAILED)) != 0)
  {
    *pCurrentOdoValue = 0x7FFFFFFFUL;
    return FALSE;
  }
  
  RamCheckCrcl(TOdoCurrentOdoMiles);
  RamCheckCrcl(TOdoCurrentOdoKm);

  if (CurrentOdoIsMetric != 0)
  {
    *pCurrentOdoValue = TOdoCurrentOdoKm & cODO_LWR7_NIBBLES_FF;
	
  }
  else
  {
    *pCurrentOdoValue = TOdoCurrentOdoMiles & cODO_LWR7_NIBBLES_FF;
	
  }

  return TRUE;
}


/*--------------------------------------------------------------------------*/
/*  Name:  PerformPostUpdateProcessing                                   */
/*                                                                          */
/*  Return:     void                                                        */
/*                                                                          */
/*  Parameters: none                                                        */
/*                                                                          */
/*  Visibility:  no external visibility                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  this routine performs the required processing after a successful        */
/*  update of EEPROM.  The counts corresponding to one storage unit are     */
/*  subtracted from odoram_whold_counts and the TOdoLastGoodE2Odo value is   */
/*  incremented to account for the successful update                        */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      03/03/01     Bill Derouchie    First Version                        */
/*                                                                          */
/*      01/22/02     Bill Derouchie    Updated to call new function         */
/*                                     InternalCurrentOdo                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static void PerformPostUpdateProcessing(void)
{
/* subtract 1 units worth of counts out of odoram */

  if ( TOdoOdoramWholeCounts.value > cWHOLE_COUNTS_PER_UNIT )  /* protect against wrap     */
  {                                                          /* if called more than once */
/* check the crc for TOdoOdoramWholeCounts */
    RamCheckCrc(&TOdoOdoramWholeCounts);
    TOdoOdoramWholeCounts.value = TOdoOdoramWholeCounts.value - cWHOLE_COUNTS_PER_UNIT;

/* apply CRC to new value of TOdoOdoramWholeCounts */
    TOdoOdoramWholeCounts.crc = OdoComputeCrc(TOdoOdoramWholeCounts.value);
	#ifdef cPROD_SPEC_RESET_STRATEGY
	TOdoOdoramWholeCountsCopy1.crc = TOdoOdoramWholeCounts.crc;
	TOdoOdoramWholeCountsCopy2.crc = TOdoOdoramWholeCounts.crc;
	#endif

/* check the crc for TOdoLastGoodE2Odo */
    RamCheckCrcl(TOdoLastGoodE2Odo);
/* we can add with out stripping the CRC because the sum will not overflow into the CRC */ 
/* just redo the CRC afterwards */ 
    TOdoLastGoodE2Odo = TOdoLastGoodE2Odo + (uint32)cPROD_SPEC_ODO_STORAGE_INCR;   /* inc for the one just written   */ 
    
    AddCrcToUlong(&TOdoLastGoodE2Odo); 

/* make sure internal odo values are uptodate */
    InternalCurrentOdo();
  }

  TOdoOdoSetBeingUpdated = cNUMBER_OF_ODO_SETS; /* indicate no update in progress */
}


/*--------------------------------------------------------------------------*/
/*  Name:  OdoMdlRunnable                                                 */
/*                                                                          */
/*  Return:     void                                                        */
/*                                                                          */
/*  Parameters:  none                                                       */
/*                                                                          */
/*  Visibility:  OdoMdl                                                  */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  the round robin routine for the odo package, enter this function on the */
/*  round robin list in the sched.cfg file                                  */
/*                                                                          */
/*  this function performs                                                  */
/*    a. calls to the odo_write function                                    */
/*    b. finishes call back processing when the EEPROM Manager is used      */
/*    c. If the odo convertion to miles is fail, retry of odo convertion    */
/*       is performed                                                       */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      1/09/16    	nrajaman           First version                        */
/*--------------------------------------------------------------------------*/
static void OdoMdlRunnable(void)
{
uint16 NewRollingOdo;

	/* ODODelta data from OdoLogicalMdl_ComIn_Process() odologicalMdl.c*/
    if(TRUE == OdoLogicalMdl_Is_Calculate_ODO())
    {
    	Rte_Read_OdoMdl_pTIOdoMdl_ComIn_NewRollingOdo( &NewRollingOdo);  
    	(void)OdoAddCounts(NewRollingOdo);
    }
    
	if ((TOdoOdoInternalFlags & cODO_STARTED_CALLBACK_PROCESSING) == cODO_STARTED_CALLBACK_PROCESSING)
	{	/*Lhbb here not run,never cODO_STARTED_CALLBACK_PROCESSING  PerformPostUpdateProcessing will in OdoDataNVMCallback*/
	  PerformPostUpdateProcessing();    
	/* clear the waiting for call back timer and clear our flag saying that
	   we completed callback processing */
	   
	   TOdoOdoInternalFlags &= (uint8)~cODO_STARTED_CALLBACK_PROCESSING;
	}
	
	/* call the odo_write function, it will determine if anything to be done    */
	(void)OdoWrite();
	
	#ifdef cPROD_SPEC_RB_MAX_TIMES

	PerformRollBack();
	

	#endif

	#if cODO_ALLOW_CHANGE

	OdoChangeup(0);

	#endif
	
	}
/*--------------------------------------------------------------------------*/
/*  Name:  OdoWrite                                                        */
/*                                                                          */
/*  Return:     boolean  TRUE when the update was successful                */
/*                       FALSE otherwise                                    */
/*                                                                          */
/*  Parameters:  None                                                       */
/*                                                                          */
/*  Visibility:  OdoMdl                                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This routine must be called must be called periodically, eg once per    */
/*  main loop.  NVM updates will be attempted if necessary. If the update   */
/*  is successful then TOdoOdoramWholeCounts                                  */
/*  will be adjusted (counts subtracted) to reflect the update.  If not     */
/*  successful TOdoOdoramWholeCounts will not be adjusted.  All accesses and  */
/*  modifications to TOdoOdoramWholeCounts will be protected by critical      */
/*  section protocol.                                                       */
/*                                                                          */
/*  an attempt is made to update the data set which logically follows the   */
/*  most recent one found by OdoRead, if not successful the routine        */
/*  attempts to update the next logical data set, unless it is one of the   */
/*  two most recent data sets found by OdoRead                             */
/*                                                                          */
/*  this routine will return true once a data set has been updated with     */
/*  the new value.  If no data set can be found into which the new value    */
/*  can be put, then this routine will return false                         */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      07/17/98     Bill Derouchie    First Version                        */
/*                                                                          */
/*      07/28/99     Bill Derouchie    Fixed Problem in odo uppdate loop    */
/*                                     that resulted in infinite loop.      */
/*                                     The problem existed when unable to   */
/*                                     update any data set and most recent  */
/*                                     set was the first one ( index 0 )    */
/*  02/17/00   WFD   Modified to use the EEPROM Manager instead of direct   */
/*                   writes to E2.                                          */
/*  04/10/01   WFD   Add check of clamp value before proceeding with E2     */
/*                   update                                                 */
/*                                                                          */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture    */
/*--------------------------------------------------------------------------*/
static boolean OdoWrite(void)
{
      SOdoCurrentOdoStruct CurrentOdoDef;
      boolean fl_temp_update_sts_BOOL;
      boolean fl_odo_read_sts_BOOL;
      boolean fl_CallbackTimerOdoMdl_BOOL=FALSE;
	  boolean fl_ThrottleTimerOdoMdl_BOOL=FALSE;
	  	
    /* check for a set being updated */
	 //TOdoOdoInternalFlags |= cODO_UPDATE_FAILED; //Lhbb Temp
	 //TOdoOdoSetBeingUpdated = 1;//Lhbb Temp
    if (TOdoOdoSetBeingUpdated < (uint8) cNUMBER_OF_ODO_SETS)
    {
        if ((TOdoOdoInternalFlags & cODO_CHANGEUP_PROCESSING) == cODO_CHANGEUP_PROCESSING)
        {
			TmExt_Timer_IsElapsed(eCallbackTimerOdoMdl, cTWO_SECOND_TIMEOUT, &fl_CallbackTimerOdoMdl_BOOL); /*Lhbb get timer rturn 3rd parameter*/
			if (TRUE == fl_CallbackTimerOdoMdl_BOOL)
			{
                TOdoOdoSetBeingUpdated=cNUMBER_OF_ODO_SETS;
                TOdoOdoInternalFlags&=(uint8)~cODO_CHANGEUP_PROCESSING;  /*clear flag**/
            #ifdef cPROD_SPEC_RESET_STRATEGY
                TOdoOdoInternalFlagsCopy1 &= (uint8)~cODO_CHANGEUP_PROCESSING;
                TOdoOdoInternalFlagsCopy2 &= (uint8)~cODO_CHANGEUP_PROCESSING;
                TOdoOdoSetBeingUpdatedCopy1 = TOdoOdoSetBeingUpdated;
                TOdoOdoSetBeingUpdatedCopy2 = TOdoOdoSetBeingUpdated; 
            #endif
            return TRUE; 
            }
            else
            {
            	return TRUE;  
            }
        }
        if ((TOdoOdoInternalFlags & cODO_RETRY_UPDATE_REQUEST) == cODO_RETRY_UPDATE_REQUEST)  /* see if previous update request attempt failed */
        {
           
           fl_temp_update_sts_BOOL = Rte_Write_NV_OdoMdl_NvMOdoData_OdoDataSetArray(pOdoDataSet);
           
           if ( fl_temp_update_sts_BOOL != TRUE)		
            {
                TmExt_Timer_Start(eCallbackTimerOdoMdl);

                TOdoOdoInternalFlags &= (uint8)~cODO_RETRY_UPDATE_REQUEST;
                #ifdef cPROD_SPEC_RESET_STRATEGY
                    TOdoOdoInternalFlagsCopy1 &= (uint8)~cODO_RETRY_UPDATE_REQUEST;
                    TOdoOdoInternalFlagsCopy2 &= (uint8)~cODO_RETRY_UPDATE_REQUEST;
                #endif
            }
        }
        else if ( (TOdoOdoInternalFlags & cODO_UPDATE_FAILED) == cODO_UPDATE_FAILED )  /* see if previous update operation failed */
        {
          OdoStartNextUpdateSet();
        }
    #if (cPROD_SPEC_ODO_STORAGE_FORMAT==cODO_PKG_OFB_EPM) 
        else      /* must be waiting for a callback to occur */
        {
        /* if we did not get the callback in time then resubmit the request */

                TmExt_Timer_IsElapsed(eCallbackTimerOdoMdl, cTWO_SECOND_TIMEOUT, &fl_CallbackTimerOdoMdl_BOOL); /*Lhbb get timer rturn 3rd parameter*/
				if (TRUE == fl_CallbackTimerOdoMdl_BOOL)
                {
                  TOdoOdoInternalFlags |= cODO_RETRY_UPDATE_REQUEST;		
                  
                  fl_temp_update_sts_BOOL = Rte_Write_NV_OdoMdl_NvMOdoData_OdoDataSetArray(pOdoDataSet);
                  
                                    
                  if ( fl_temp_update_sts_BOOL != TRUE)		  
	                {
	                 TmExt_Timer_Start(eCallbackTimerOdoMdl);
      
	                 TOdoOdoInternalFlags &= (uint8)~cODO_RETRY_UPDATE_REQUEST;
	                }                                                                                      
        		}
		}
    #endif
        return TRUE;
    }

    /*if timer is running,   do not go any further                                     */
	TmExt_Timer_IsElapsed( eThrottleTimerOdoMdl, cFIVE_SECOND_TIMEOUT,&fl_ThrottleTimerOdoMdl_BOOL);
    if( TRUE == fl_ThrottleTimerOdoMdl_BOOL )
    {                                                                               
       return TRUE;   /* just leave and wait  */
    }              
    /* timer is not running, so it is either clear or expired          */
    /* in either case, go ahead                                        */
   
    /* see if we should really be doing an update */    
    /* check the crc for TOdoOdoramWholeCounts */
    RamCheckCrc(&TOdoOdoramWholeCounts);
    if (TOdoOdoramWholeCounts.value < cUPDATE_COUNT_THRESHOLD) /*8001*/
    {
        return TRUE;  /* just leave now, do not do anything */
    }

    fl_odo_read_sts_BOOL = OdoRead(&CurrentOdoDef);
    if (fl_odo_read_sts_BOOL == FALSE)
    {                       /* can not do update */
        return FALSE;
    }
    else if (CurrentOdoDef.NvmValue <= cNVM_ODO_VALUE_CLAMP) /* do not exceed max count */
    {
            TOdoLastGoodE2Odo = CurrentOdoDef.NvmValue;
            AddCrcToUlong(&TOdoLastGoodE2Odo);   

            TOdoOdoProtectedSet[0] = CurrentOdoDef.NvmSet[0];
            TOdoOdoProtectedSet[1] = CurrentOdoDef.NvmSet[1];
            /* get the bytes to be written */
            GetOdoNvmDataSetBytes( &CurrentOdoDef );
            /* start actual odo write */
            TOdoOdoSetBeingUpdated = TOdoOdoProtectedSet[1];
            /* NOTE: the function OdoStartNextUpdateSet protects against actually */
            /* updating either of the protected sets */
             OdoStartNextUpdateSet();
        }
    else
    {
       /* empty on purpose */
    }
    return TRUE;
}

/*--------------------------------------------------------------------------*/
/*  Name:  OdoStartNextUpdateSet                                        */
/*                                                                          */
/*  Return:     None                                                        */
/*                                                                          */
/*  Parameters:  None                                                       */
/*                                                                          */
/*  Visibility:  OdoMdl                                                  */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*     This routine initiates an attempt to submit an update request for an */
/*     odo data set.  If a previous attempt to do a specific km             */
/*     update failed, this routine has the logic to increment to another     */
/*     data set, and try again.                                             */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      02/17/00     Bill Derouchie    First Version                        */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static void OdoStartNextUpdateSet (void)
{

  boolean FlTempUpdateStatus;
/* CYC:BEGIN_TEST_WHILE MAX_ITER=const(cyc_num_odo_sets_mns2) EXP_ITER=1 */
  while (1)
/* CYC:END_TEST_WHILE */
  {
    TOdoOdoInternalFlags &= (uint8)~cODO_UPDATE_FAILED;   /* clear failure indicator */ 
    TOdoOdoSetBeingUpdated++;

    if( TOdoOdoSetBeingUpdated >= cNUMBER_OF_ODO_SETS ) /* wrap when necessary */
    {
      TOdoOdoSetBeingUpdated = 0;
    }

    if ( TOdoOdoSetBeingUpdated == TOdoOdoProtectedSet[1] ) /* back to beginning */
    {                                 						/* block a retry for 5 seconds */
      TmExt_Timer_Start(eThrottleTimerOdoMdl);
      TOdoOdoSetBeingUpdated = cNUMBER_OF_ODO_SETS; /* indicate no update in progress */
      return;
    }
    else if ( TOdoOdoSetBeingUpdated != TOdoOdoProtectedSet[0] )
    {
      TOdoOdoInternalFlags |= cODO_RETRY_UPDATE_REQUEST; /* this allows the non E2 Manager update to be */
					                                    /* re done if a warm reset occurs during the update */
					                                    /* and will cause E2 Manager systems to retry the request */
                                              
      FlTempUpdateStatus = Rte_Write_NV_OdoMdl_NvMOdoData_OdoDataSetArray(pOdoDataSet);
         
      
      if ( FlTempUpdateStatus != TRUE)	
      {
        /* start a two second timer saying that we are waiting for a callback */
		TmExt_Timer_Start(eCallbackTimerOdoMdl); 
        TOdoOdoInternalFlags &= (uint8)~cODO_RETRY_UPDATE_REQUEST;
      }                                                                                      
      return;
    }
    else
    {
        /* empty on purpose */      
    }
  }
}


/*--------------------------------------------------------------------------*/
/*  Name:  OdoDataNVMCallback                                               */
/*                                                                          */
/*  Return:     None                                                        */
/*                                                                          */
/*  Parameters:  uint8 Result     - TRUE if update successfull		         */
/*                                   FALSE otherwise	                    */
/*                															*/
/*				uint8 ServiceType  - Write type                             */
/*  Visibility:  none                                                       */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*     This routine is the callback for odo data set update operations.     */
/*     This is part of the EEPROM Manager interface.                        */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      02/17/00     Bill Derouchie    First Version                        */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static void OdoDataNVMCallback(uint8 ServiceType , uint8 Result) /*Lhbb Result==TRUE is failed*/
{
  
/* only do processing if it looks like we want this callback */
  //Result = FALSE;/*Lhbb temp*/ 
  if ( TOdoOdoSetBeingUpdated < cNUMBER_OF_ODO_SETS)
  {
    if ( Result != TRUE)   /* successfull update */	 
    {
/* if using e2 manager than just set a flag */
#if (cPROD_SPEC_ODO_STORAGE_FORMAT==cODO_PKG_OFB_EPM) 

/* do not do anything if this is a second callback because of a warm init */
      if ((TOdoOdoInternalFlags & cODO_STARTED_CALLBACK_PROCESSING) != cODO_STARTED_CALLBACK_PROCESSING)
      {
//        TOdoOdoramWholeCountsSave = TOdoOdoramWholeCounts.value;   /* save until callback processing complete */
//        TOdoLastGoodE2OdoSave = TOdoLastGoodE2Odo;               /* save until callback processing complete */
  
/* set a flag so that round robin will perform callback processing */
        TOdoOdoInternalFlags |= cODO_STARTED_CALLBACK_PROCESSING;
      }
#endif

/* do success processing here if not using e2 manager */

      PerformPostUpdateProcessing();
     
    }
    else   /* false */
    {   /* do none successful processing here whether or not e2 manager uesed */

/* because the epm_pkg guarantees that a call back will be able             */
/* to queue an epm request, do not need to worry about success              */
/* of the call to epm_pkg_q_e2_write function below                         */  
/* do not use a call back here, because nothing can be done about a failure */
            
      TOdoOdoInternalFlags |= cODO_UPDATE_FAILED;   /* indicate update failed */

#if (cPROD_SPEC_ODO_STORAGE_FORMAT==cODO_PKG_OFB_EPM) 

/* clear the waiting for call back timer and set our flag saying that
   we completed callback processing */

      TOdoOdoInternalFlags &= (uint8)~cODO_STARTED_CALLBACK_PROCESSING;
	    ODO_WriteNVM_Failure_SubFunction();

#endif
    }
  }  
}



/*--------------------------------------------------------------------------*/
/*  Name:  GetOdoNvmDataSetBytes                                             */
/*                                                                          */
/*  Return:     none                                                        */
/*                                                                          */
/*  Parameters:  CURRENT_ODO_STRUCT * current_odo - address of area in which*/
/*                                                  info from most recent   */
/*                                                  data set is stored      */
/*                                                                          */
/*  Visibility:  no external visibility                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  this routine generates am image of the NVM data set to be used to       */
/*  update NVM.                                                             */
/*                                                                          */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture     */
/*--------------------------------------------------------------------------*/
static void GetOdoNvmDataSetBytes( const SOdoCurrentOdoStruct *pCurrentOdo )
{
  uint8 TempIncr;
  uint16 TempCount;
  uint8 TempNvmSet;

/* compute what the new count and incr values will be */
  TempIncr = pCurrentOdo->NvmIncr + (uint8)cPROD_SPEC_ODO_STORAGE_INCR;
  TempCount = pCurrentOdo->NvmCount;
  if (TempIncr > cTOTAL_ODO_INCRS)
  {
    TempIncr = TempIncr - cTOTAL_ODO_INCRS;
    TempCount = TempCount + 1;
  }

  TempNvmSet = pCurrentOdo->NvmSet[1];
  TempNvmSet++;

  if(TempNvmSet >= cNUMBER_OF_ODO_SETS)	TempNvmSet = 0;

  /* put the top 4 bits of value into the extra_count byte */
  pOdoDataSet[TempNvmSet].extra_count = ~((uint8)(((TempCount) & (uint16)0xF000u)/(uint16)256));

	
/* replace the bottom 4 bits of value with the CRC */
  TempCount = (TempCount << 4) |(uint16)OdoComputeCrc(TempCount);	

/* and place in buffer */  

  pOdoDataSet[TempNvmSet].count = TempCount;


	
  PutOfbNvmIncr(TempIncr,TempNvmSet);

}

/*--------------------------------------------------------------------------*/
/*  Name:  PutOfbNvmIncr                                                 */
/*                                                                          */
/*  Return:     NONE                                                        */
/*                                                                          */
/*  Parameters: uint8  incr - the increment value                           */
/*                                                                          */
/*  Visibility:  no external visibility                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This routine determines the increment value to be stored in offboard    */
/*  NVM.  The parity bit is set to give odd parity and the value is changed */
/*  a range of 0 to 127 from the range of 1 to 128                          */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      12/18/2000   Bill Derouchie    First Version                        */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#if (cPROD_SPEC_ODO_STORAGE_UNITS != cODO_PKG_TENTH_KM) 
static void PutOfbNvmIncr(uint8 incr, uint8 NvmSet)
{
  uint8 FlTempParity;
  incr--;  /* decrement for storage range of 0 to 127 */

/* set parity bit, if required to increment byte */
  FlTempParity = OdoChkParity(incr);
  if (FlTempParity == 0)
  {
    pOdoDataSet[NvmSet].increments = (incr | (uint8)0x80);					   
	
  }
  else
  {
    pOdoDataSet[NvmSet].increments = incr;	
  }
}
#else
/* Increment Value Calculation Before Storing in the NVM */
static void PutOfbNvmIncr(uint8 incr, uint8 NvmSet)
{
  incr--; /* decrement for storage range of 0 to 127 */

/* Extract the Extra Increment Bit & Store it in the Unused Nibble */
  pOdoDataSet[NvmSet].extra_count &= (uint8)0xFE;
  pOdoDataSet[NvmSet].extra_count |= ((incr>>7) & 0x01);

/* set parity bit, if required to increment byte */
  if (OdoChkParity(incr) == 1)
  {
    pOdoDataSet[NvmSet].increments = incr | (uint8)0x80;
  }
  else
  {
    pOdoDataSet[NvmSet].increments = incr & 0x7F;
  }
}
#endif  /* End of cODO_PKG_TENTH_KM */



/* take into account whether or not roll back is configured */
#ifdef cPROD_SPEC_RB_MAX_TIMES

  #if ((cPROD_SPEC_RB_MAX_TIMES < 1) || (cPROD_SPEC_RB_MAX_TIMES > 255))
    #error cPROD_SPEC_RB_MAX_TIMES has bad value
  #endif
  
  #ifndef cPROD_SPEC_RB_MAX_UNITS
    #error cPROD_SPEC_RB_MAX_UNITS is not defined
  #else
    #if (cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_TENTH_KM) 
      #if ((cPROD_SPEC_RB_MAX_UNITS < 1) || (cPROD_SPEC_RB_MAX_UNITS > 5100))
        #error cPROD_SPEC_RB_MAX_UNITS should be defined between 1-5100.
      #endif
    #else
      #if ((cPROD_SPEC_RB_MAX_UNITS < 1) || (cPROD_SPEC_RB_MAX_UNITS > 510))
        #error cPROD_SPEC_RB_MAX_UNITS should be defined between 1-510.
      #endif
    #endif
  #endif

  #ifndef cPROD_SPEC_RB_CEILING
    #error cPROD_SPEC_RB_CEILING is not defined
  #else
      #if (cPROD_SPEC_RB_CEILING < 1)
        #error cPROD_SPEC_RB_CEILING has bad value
      #endif
  #endif

  #ifndef cPROD_SPEC_RB_TYPE
    #define cPROD_SPEC_RB_TYPE cODO_PKG_ABSOLUTE
  #else
    #if ( (cPROD_SPEC_RB_TYPE != cODO_PKG_ABSOLUTE)\
        &&(cPROD_SPEC_RB_TYPE != cODO_PKG_RELATIVE))
      #error cPROD_SPEC_RB_TYPE is neither cODO_PKG_ABSOLUTE or cODO_PKG_RELATIVE
    #endif
  #endif

  #ifndef cPROD_SPEC_RB_FLOOR
    #define cPROD_SPEC_RB_FLOOR 0
  #else
    #if (cPROD_SPEC_RB_TYPE == cODO_PKG_ABSOLUTE)
      #if ((cPROD_SPEC_RB_FLOOR < 0)\
        || (cPROD_SPEC_RB_FLOOR > cPROD_SPEC_RB_CEILING))
        #error cPROD_SPEC_RB_FLOOR has bad value
      #endif
    #elif (cPROD_SPEC_RB_TYPE == cODO_PKG_RELATIVE)
      #if ((cPROD_SPEC_RB_FLOOR < 1)\
        || (cPROD_SPEC_RB_FLOOR > cPROD_SPEC_RB_CEILING))
        #error cPROD_SPEC_RB_FLOOR has bad value
      #endif
    #endif
  #endif

  #ifndef cPROD_SPEC_RB_MAX_PER
    #error cPROD_SPEC_RB_MAX_PER is not defined
  #else
    #if ((cPROD_SPEC_RB_MAX_PER < 1)\
      || (cPROD_SPEC_RB_MAX_PER > cPROD_SPEC_RB_MAX_UNITS))
      #error cPROD_SPEC_RB_MAX_PER has bad value
    #endif
  #endif

/*--------------------------------------------------------------------------*/
/*  Name:  OdoRollBackCb                                                 */
/*                                                                          */
/*  Return:     none                                                        */
/*                                                                          */
/*  Parameters:  uint8 Result     - TRUE if update successfull		         */
/*                                   FALSE otherwise	                    */
/*                															*/
/*				uint8 ServiceType  - Write type                             */
/*                                                                          */
/*  Visibility:  none                                                       */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*     This routine is part of the interface to the EEPROM Manager, and is  */
/*     the 'callback' routine for the E2 update request                     */
/*                                                                          */
/*     This routine sets a flag that odo_chk_roll_back checks to see if this*/
/*     routine has been called by the E2 manager                            */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      02/19/00     Bill Derouchie    First Version                        */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static void OdoRollBackCb(uint8 ServiceType , uint8 Result)
{
  if( Result != TRUE )
  {
    TOdoRollBackTemp[1] = 0;  /* mark that this executed successfully */
  }
}

/*--------------------------------------------------------------------------*/
/*  Name:  PerformRollBack                                                  */
/*                                                                          */
/*  Return:     boolean - TRUE is returned if the roll back has been        */
/*                        performed, FALSE is returned otherwise            */
/*                                                                          */
/*  Parameters:  None                                                       */
/*                                                                          */
/*  Visibility:  OdoMdl                                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  Check whether thr RollBAck has to be performed                          */
/*                                                                          */
/*  It must only be called from non-ISR level                               */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      1/09/16    	nrajaman          First Version                         */
/*--------------------------------------------------------------------------*/
static boolean PerformRollBack(void)
{
uint8 RollbackRequested;
boolean RollbackResult;


	(void)Rte_Read_pOdoMdl_PerfomRollBack_pOdoMdl_PerfomRollBack(&RollbackRequested);

	if(RollbackRequested == TRUE)
	{
	   RollbackResult = OdoRollBack();
	 }
	 return RollbackResult;

}



  
/*--------------------------------------------------------------------------*/
/*  Name:  OdoRollBack                                                      */
/*                                                                          */
/*  Return:     boolean - TRUE is returned if the roll back has been        */
/*                        performed, FALSE is returned otherwise            */
/*                                                                          */
/*  Parameters:  None                                                       */
/*                                                                          */
/*  Visibility:  OdoMdl                                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  Using the configuration this routine determines if a roll back can be   */
/*  performed and if it can then the roll back is performed                 */
/*                                                                          */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      10/09/98     Bill Derouchie    First Version                        */
/*      02/19/00     Bill Derouchie    Modified to use the epm-01 code pkg  */
/*      01/22/02     Bill Derouchie    Modified to call InternalCurrentOdo  */
/*      11/14/2013   ssekar1           Modified the rollback parameter      */
/*                                     resolution for TENTH_KM configuration*/
/*      1/09/16    	nrajaman           Modified to Turing  Architecture      */
/*--------------------------------------------------------------------------*/
#ifdef cPROD_SPEC_RB_DISPLAY_UNITS
static boolean OdoRollBack(boolean MetricDisplay)
#else
static boolean OdoRollBack(void)
#endif
  {
    SOdoCurrentOdoStruct CurrentOdoDef;
    uint32 OdoRbCeiling;
    uint32 MaxRollBack;
    uint16 OdoRbMaxUnits;
    uint16 OdoRbFloor;
    uint8 savi;

#if (cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_TENTH_KM)
   /*Max units rolled back per reset is also limitted to 255 */
   /* One byte memory has been increased only to support tenth_KM configuration units*/
    uint16 OdoRbMaxPer;
#else
    uint8 OdoRbMaxPer; 
#endif
    
#if (cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_TENTH_KM)
/*Roll back parameters if the storage units is ONE_TENTH_KM*/
    OdoRbMaxUnits = (uint16)(cPROD_SPEC_RB_MAX_UNITS);
    OdoRbCeiling = (uint32)(cPROD_SPEC_RB_CEILING);
    OdoRbFloor = (uint16)(cPROD_SPEC_RB_FLOOR);
    OdoRbMaxPer = (uint16)(cPROD_SPEC_RB_MAX_PER);
#else
  #ifdef cPROD_SPEC_RB_DISPLAY_UNITS
    if (MetricDisplay == FALSE)
    {
      OdoRbMaxUnits = cRB_MAX_UNITS_MILES;
      OdoRbCeiling = cRB_CEILING_MILES;
      OdoRbFloor = cRB_FLOOR_MILES;
      OdoRbMaxPer = cRB_MAX_PER_MILES;
    }
    else
    {
#endif

      OdoRbMaxUnits = (uint16)cPROD_SPEC_RB_MAX_UNITS;
      OdoRbCeiling = (uint32)cPROD_SPEC_RB_CEILING;
      OdoRbFloor = (uint16)cPROD_SPEC_RB_FLOOR;
      OdoRbMaxPer = (uint8)cPROD_SPEC_RB_MAX_PER;

  #ifdef cPROD_SPEC_RB_DISPLAY_UNITS
    }
  #endif
#endif
/* if a rollback update is in progress, do not start another */
    if (TOdoRollBackTemp[1] != 0)
    {
      return FALSE;
    }
    
/* get the current odo value in storage units */
    if ( OdoRead(&CurrentOdoDef) != FALSE)
    {
/* off set by cODO_OFFSET and previous roll back amount */
      CurrentOdoDef.NvmValue = CurrentOdoDef.NvmValue
                                - (cODO_OFFSET + (uint32)TOdoRollBackUnits);

/* can a roll back be attempted */
      if ( (TOdoRollBackUnits < OdoRbMaxUnits)
         &&(TOdoRollBackCount < (uint16)cPROD_SPEC_RB_MAX_TIMES)
         &&(CurrentOdoDef.NvmValue  <= OdoRbCeiling) )
      {                                                       /* yes */
/* compute roll back to the floor  */
      #if ( cPROD_SPEC_RB_TYPE == cODO_PKG_ABSOLUTE)
        if (CurrentOdoDef.NvmValue > OdoRbFloor)
        {
          MaxRollBack = (CurrentOdoDef.NvmValue - OdoRbFloor);
        }
        else
        {
          return FALSE;    /* below or at floor now, so can't do roll back */
        }
      #else
        MaxRollBack = (CurrentOdoDef.NvmValue % OdoRbFloor);
      #endif

/* limit roll back to max per instance */
        if (MaxRollBack > OdoRbMaxPer)
        {
          MaxRollBack = OdoRbMaxPer;
        }

/* limit roll back to max total */
        if (MaxRollBack > (uint32)(OdoRbMaxUnits - TOdoRollBackUnits))
        {
          MaxRollBack = (uint32)(OdoRbMaxUnits - TOdoRollBackUnits);
        }

/* update NVM, assume success, because nothing to do if not */
/*      use TOdoRollBackTemp to temporarily hold the values to be written to E2 */
        TOdoRollBackTemp[0] = ((uint16)MaxRollBack + TOdoRollBackUnits);
        TOdoRollBackTemp[1] = TOdoRollBackCount + 1;

/* attempt to issue E2 update request, loop up to 10 times with 12Ms delay between */
/* CYC:BEGIN_TEST_FOR MAX_ITER=9 EXP_ITER=1 */
        for ( savi = 0; savi < 10; savi++)
/* CYC:END_TEST_FOR */
        {
         
          if(TRUE != (Rte_Write_NV_OdoMdl_NvMOdoData_OdoDataSetArray(pOdoDataSet)))  
          {
            break;            /* update request was successfull */
          }
          if (savi == 9)
          {
            return FALSE;      /* update request was not successfull */
          }
          
        }
/* at this point the E2 update was successfully requested */
/* and the shadow RAM has been updated                    */

/* zero accumulated input counts in ram
********************************************************************************
 NOTE: In error conditions where NVM can not be written there may be several
 storage units worth of 'mileage' in ram under these conditions the roll back
 may be greater than would otherwise be allowed.  To do otherwise would involve
 adding complexity, code size etc to this routine.
*******************************************************************************/
        TOdoOdoramWholeCounts.value = 0;
        TOdoOdoramWholeCounts.crc = OdoComputeCrc(TOdoOdoramWholeCounts.value);
        /* Start of cPROD_SPEC_RESET_STRATEGY */
       #ifdef cPROD_SPEC_RESET_STRATEGY
        TOdoOdoramWholeCountsCopy1.value = TOdoOdoramWholeCounts.value;
        TOdoOdoramWholeCountsCopy1.crc = TOdoOdoramWholeCounts.crc;
      

        TOdoOdoramWholeCountsCopy2.value = TOdoOdoramWholeCounts.value;
        TOdoOdoramWholeCountsCopy2.crc = TOdoOdoramWholeCounts.crc;
      
       #endif 
       /* End of cPROD_SPEC_RESET_STRATEGY */

        InternalCurrentOdo();
        
        return TRUE;
      }
    }
    return FALSE;     /* can't do a roll back */
                      /* either not permitted by configuration */
                      /* or OdoRead failed                    */
  }

/*--------------------------------------------------------------------------*/
/*  Name:  OdoCheckRollBack                                                */
/*                                                                          */
/*  Return:     boolean  TRUE  if the update completed successfully         */
/*                       FALSE otherwise                                    */
/*                                                                          */
/*  Parameters:  none                                                       */
/*                                                                          */
/*  Visibility:  OdoMdl                                                  */
/*                                                                          */
/*  Description: check that the new odo value requested via odo_roll_back   */
/*               has been successfully written to the actual EEPROM         */
/*                                                                          */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      09/19/02     Bill Derouchie    First Version                        */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static boolean OdoCheckRollBack(void)
{
/* check for successfull call back */
  if (TOdoRollBackTemp[1] != 0)
  {
    return FALSE;
  }
  else
  {
/* at this point the E2 update was successfully performed */
    return TRUE;
  }
}

/*--------------------------------------------------------------------------*/
/*  Name:  OdoRollBackReport                                             */
/*                                                                          */
/*  Return:     uint32 - The total odo roll back amout in the least          */
/*                      significant WORD(16 bits) and the number of roll    */
/*                      backs in most significant WORD (16 bits)            */
/*                                                                          */
/*  Parameters:  None                                                       */
/*                                                                          */
/*  Visibility:  OdoMdl                                                  */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  The routine returns information that can be used to respond to          */
/*  diagnostic requests for odo roll back information                       */
/*                                                                          */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      10/09/98     Bill Derouchie    First Version                        */
/*      11/14/2013   ssekar1           Changed the return type to uint32 as  */
/*                                     the datatype of the operands are     */ 
/*                                     increasd to uint16                    */
/*--------------------------------------------------------------------------*/
static uint32 OdoRollBackReport(void)
{
return  ((uint32)TOdoRollBackUnits) | ((uint32)TOdoRollBackCount*65536);
}

#endif   /* end of roll back section */


#ifdef cODO_ALLOW_CHANGE
/*--------------------------------------------------------------------------*/
/*  Name:  OdoChangeCallback                                              */
/*                                                                          */
/*  Return:     none                                                        */
/*                                                                          */
/*  Parameters:  uint8 Result     - TRUE if update successfull		         */
/*                                   FALSE otherwise	                    */
/*                															*/
/*				uint8 ServiceType  - Write type                             */
/*                                                                          */
/*  Visibility:  none                                                       */
/*                                                                          */
/*  Description: check that the new odo value requested via OdoChangeup    */
/*               has been successfully written to at least 2 data sets      */
/*                                                                          */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      05/01/00     Bill Derouchie    First Version                        */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#if 0
static void OdoChangeCallback(uint8 ServiceType , uint8 Result)
{
  if (Result != TRUE)
  {
/* count up the successfull attempts */
    TOdoEpmChangeCounter++;

/* if this is the second success then do something special*/
    if (TOdoEpmChangeCounter == 2)
    {
	TOdoOdoSetBeingUpdated=cNUMBER_OF_ODO_SETS;
			TOdoOdoInternalFlags&=(uint8)~cODO_CHANGEUP_PROCESSING;
			 #ifdef cPROD_SPEC_RESET_STRATEGY
				TOdoOdoInternalFlagsCopy1 &= (uint8)~cODO_CHANGEUP_PROCESSING;
				TOdoOdoInternalFlagsCopy2 &= (uint8)~cODO_CHANGEUP_PROCESSING;
				TOdoOdoSetBeingUpdatedCopy1 = TOdoOdoSetBeingUpdated;
				TOdoOdoSetBeingUpdatedCopy2 = TOdoOdoSetBeingUpdated; 
			#endif
/* submit a request to update odo_changeup_amnt, assume that it is successfull */
/* call the eeprom manager routine to queue the update request */
/* and return its status                                       */

                                    
    Rte_Write_NV_OdoMdl_NvMOdoData_OdoDataSetArray(pOdoDataSet);     
    }
  }
}
#endif
#endif
/*--------------------------------------------------------------------------*/
/*  Name:  OdoChangeup                                                     */
/*                                                                          */
/*  Return:     boolean  Set TRUE when the update was successful            */
/*                       FALSE otherwise                                    */
/*                                                                          */
/*  Parameters:  uint32 NewOdoValue  Requested new odo value in .1Km units */
/*                                                                          */
/*  Visibility:  OdoMdl                                                     */
/*                                                                          */
/*  Description: Change the odo value to a new value which must be larger   */
/*                                                                          */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      05/01/00     Bill Derouchie    First Version                        */
/*      01/22/02     Bill Derouchie    Modified to call InternalCurrentOdo  */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static boolean OdoChangeup(uint32 NewOdoValue)
{
  SOdoCurrentOdoStruct CurrentOdoDef;
  uint8 OkCntr = 0;
  uint8 TryCntr = 0;
  uint8 NSet;
  //uint16 tempuw;

/* compensate for the increment that will be added by GetOdoNvmDataSetBytes */
/* also compensate for the initial odo value - cODO_OFFSET */
#ifdef cPROD_SPEC_RB_MAX_TIMES
    CurrentOdoDef.NvmValue = cODO_OFFSET + (uint32)TOdoRollBackUnits + NewOdoValue - (uint32)cPROD_SPEC_ODO_STORAGE_INCR;
#else
    CurrentOdoDef.NvmValue = cODO_OFFSET + NewOdoValue - (uint32)cPROD_SPEC_ODO_STORAGE_INCR;
#endif    

/* decompose into count and increment values */
    CurrentOdoDef.NvmCount = (uint16)(CurrentOdoDef.NvmValue/cTOTAL_ODO_INCRS);
    CurrentOdoDef.NvmIncr = (uint8)(CurrentOdoDef.NvmValue % cTOTAL_ODO_INCRS);

/* get the bytes to be written */
	for(NSet = 0; NSet < cNUMBER_OF_ODO_SETS; NSet++)
	{
	  CurrentOdoDef.NvmSet[1] = NSet;
	  GetOdoNvmDataSetBytes( &CurrentOdoDef );
	}

/* try to update three data sets with new value */
/* protect the two biggest sets found by get_current_odo */
 //   TOdoEpmChangeCounter = 0;
/* CYC:BEGIN_TEST_WHILE MAX_ITER=4 EXP_ITER=3 */
    while ( TryCntr < 3)
/* CYC:END_TEST_WHILE */
    {
        if(TRUE != (Rte_Write_NV_OdoMdl_NvMOdoData_OdoDataSetArray(pOdoDataSet))) 
        {
          OkCntr++;
        }
        TryCntr++;
    }

/* success if at least two requests succeeded */
  if (OkCntr > 0)
  {
    TOdoOdoramWholeCounts.value = 0;   /* start accumulating over again */
    TOdoOdoramWholeCounts.crc = OdoComputeCrc(TOdoOdoramWholeCounts.value);
    /* Start of cPROD_SPEC_RESET_STRATEGY */
   #ifdef cPROD_SPEC_RESET_STRATEGY
    TOdoOdoramWholeCountsCopy1.value = TOdoOdoramWholeCounts.value;
    TOdoOdoramWholeCountsCopy1.crc = TOdoOdoramWholeCounts.crc;
  

    TOdoOdoramWholeCountsCopy2.value = TOdoOdoramWholeCounts.value;
    TOdoOdoramWholeCountsCopy2.crc = TOdoOdoramWholeCounts.crc;
  
   #endif 
   /* End of cPROD_SPEC_RESET_STRATEGY */

    TOdoLastGoodE2Odo = CurrentOdoDef.NvmValue + (uint32)cPROD_SPEC_ODO_STORAGE_INCR;
    /* Start of cPROD_SPEC_RESET_STRATEGY */
   #if (defined (cPROD_SPEC_RESET_STRATEGY) && !defined (cODO_OPTIMIZED_RUNNABLE))
    TOdoLastGoodE2OdoCopy1 = TOdoLastGoodE2Odo;
    TOdoLastGoodE2OdoCopy2 = TOdoLastGoodE2Odo;
   #endif
    AddCrcToUlong(&TOdoLastGoodE2Odo);
   #if (defined (cPROD_SPEC_RESET_STRATEGY) && !defined (cODO_OPTIMIZED_RUNNABLE))
    AddCrcToUlong(&TOdoLastGoodE2OdoCopy1);
    AddCrcToUlong(&TOdoLastGoodE2OdoCopy2);
   #endif 
    InternalCurrentOdo();
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

void COdoMdl_SetOdoTotalMeter(uint32 TotalMeter,boolean *ret)
{
	*ret = OdoChangeup(TotalMeter);
}

/*--------------------------------------------------------------------------*/
/*  Name:  OdoChangeupHighResolution                                        */
/*                                                                          */
/* Return:     uint8  Set ODO_CHANGEUP_ACCEPTED when the update was 		*/
/*				successful , ODO_CHANGEUP_NOT_HONOURED  if fails,			*/
/*				otherwise ,                                      			*/	
/* 				ODO_CHANGEUP_RETRY if busy with Normal odo     				*/
/*                                                                          */
/*  Parameters:  uint32 NewOdoValue  Requested new odo value in .01Km units*/
/*               (Eg: 500 KM means 50000 is the input parameter)            */
/*                                                                          */
/*  Visibility:  OdoMdl                                                      */
/*                                                                          */
/*  Description: Change the odo value to a new value in resolution of       */
/*               1/100 which must be larger than current odo value          */
/*                                                                          */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      10/11/2011     rananth1       First Version                         */
/*      10/12/2013     bsubram1                                             */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture     */
/*--------------------------------------------------------------------------*/
#if 0
static uint8 OdoChangeupHighResolution(uint32 NewOdoValue)
{
  SOdoCurrentOdoStruct CurrentOdoDef;
#ifndef cPROD_SPEC_RB_MAX_TIMES
  uint32 BackNewOdoValue;
#endif
  uint32 FlOdoChangeupFractionValue;
  uint16 FlOdoChangeupFractionCount;
  uint16 tempuw;
  uint8 OkCntr = 0;
  uint8 TryCntr = 0;
  uint8 NSet;

#ifndef cPROD_SPEC_RB_MAX_TIMES
  BackNewOdoValue = NewOdoValue;
#endif
 if(TOdoOdoSetBeingUpdated!=cNUMBER_OF_ODO_SETS){
 
	return (ODO_CHANGEUP_RETRY);
  }
  if (OdoRead(&CurrentOdoDef) == FALSE)
  {                       /* can not do update */
    
	return (ODO_CHANGEUP_NOT_HONOURED);	
	
  }
  else
  {

  /* Store odo truncating decimal value*/
  FlOdoChangeupFractionValue = NewOdoValue / 100UL;

  /* Converting Fractional Odo values in terms of counts */
  FlOdoChangeupFractionCount = (uint16)((uint32)((NewOdoValue - (FlOdoChangeupFractionValue*100UL))*cWHOLE_COUNTS_PER_UNIT)/100UL);

  /* Store odo truncating decimal value, thus New Odo value in 1 mile resolution */
  NewOdoValue = NewOdoValue /100UL;

#if cPROD_SPEC_ODO_STORAGE_UNITS == cODO_PKG_TENTH_KM
    NewOdoValue = NewOdoValue * 10UL;
#endif

/* if the requested new value is not greater than current, reject the request */
/* also reject if to big                                                      */
#ifdef cPROD_SPEC_RB_MAX_TIMES
    if ( (NewOdoValue <= (CurrentOdoDef.NvmValue - cODO_OFFSET - TOdoRollBackUnits))
#else
    if ( ( (BackNewOdoValue) <=
         ( (TOdoCurrentOdoKm & cODO_LWR7_NIBBLES_FF) - cODO_OFFSET))
#endif
      || (NewOdoValue > cNVM_ODO_VALUE_CLAMP) )
    {
       return ODO_CHANGEUP_NOT_HONOURED;	
    }

/* sum the roll up amount in thousands, clamp to 255 */
    tempuw = (uint16)((uint16)odo_changeup_amnt + 1 + (uint16)((NewOdoValue - CurrentOdoDef.NvmValue)/(uint32)1000));
    if (tempuw > 255)
    {
      TOdoLocalOdoChangeupAmnt = 255;
    }
    else
    {
      TOdoLocalOdoChangeupAmnt = (uint8)tempuw;
    }

/* compensate for the increment that will be added by GetOdoNvmDataSetBytes */
/* also compensate for the initial odo value - cODO_OFFSET */
#ifdef cPROD_SPEC_RB_MAX_TIMES
    CurrentOdoDef.NvmValue = cODO_OFFSET + (uint32)TOdoRollBackUnits + NewOdoValue - (uint32)cPROD_SPEC_ODO_STORAGE_INCR;
#else
    CurrentOdoDef.NvmValue = cODO_OFFSET + NewOdoValue - cPROD_SPEC_ODO_STORAGE_INCR;
#endif

/* decompose into count and increment values */
    CurrentOdoDef.NvmCount = (uint16)(CurrentOdoDef.NvmValue/cTOTAL_ODO_INCRS);
    CurrentOdoDef.NvmIncr = (uint8)(CurrentOdoDef.NvmValue % cTOTAL_ODO_INCRS);

/* get the bytes to be written */
    GetOdoNvmDataSetBytes( &CurrentOdoDef );
/* start actual odo write */
    NSet = CurrentOdoDef.NvmSet[1];

/* try to update three data sets with new value */
/* protect the two biggest sets found by get_current_odo */
    TOdoEpmChangeCounter = 0;
/* CYC:BEGIN_TEST_WHILE MAX_ITER=4 EXP_ITER=3 */
    while ( TryCntr < 3)
/* CYC:END_TEST_WHILE */
    {
      NSet++;

      if( NSet >= cNUMBER_OF_ODO_SETS ) /* wrap when necessary */
      {
        NSet = 0;
      }

      if ( NSet == CurrentOdoDef.NvmSet[1] ) /* back to beginning so get out */
      {
        break;
      }
      else if ( NSet != CurrentOdoDef.NvmSet[0] )
      {
        if(TRUE != (Rte_Write_NV_OdoMdl_NvMOdoData_OdoDataSetArray(pOdoDataSet))) 
        {
          OkCntr++;
        }
        TryCntr++;
      }
      else
      {
             /* do not count this one */
      }
    }
  }

/* success if at least two requests succeeded */
  if (OkCntr > 1)
  {
  
	TOdoOdoSetBeingUpdated=NSet;
	TOdoOdoInternalFlags |= (uint8)cODO_CHANGEUP_PROCESSING;
	 #ifdef cPROD_SPEC_RESET_STRATEGY
        TOdoOdoInternalFlagsCopy1 |= (uint8)cODO_CHANGEUP_PROCESSING;
        TOdoOdoInternalFlagsCopy2 |= (uint8)cODO_CHANGEUP_PROCESSING;
		TOdoOdoSetBeingUpdatedCopy1 = TOdoOdoSetBeingUpdated;
		TOdoOdoSetBeingUpdatedCopy2 = TOdoOdoSetBeingUpdated; 
        #endif
	TmExt_Timer_Start(eCallbackTimerOdoMdl);
	
	TOdoLastGoodE2Odo = CurrentOdoDef.NvmValue + (uint32)cPROD_SPEC_ODO_STORAGE_INCR;
    AddCrcToUlong(&TOdoLastGoodE2Odo);
    /* accumulate the counts */
    
        
    /*
	** Add the fractional odo counts to the accumulated counts
	*/
    TOdoOdoramWholeCounts.value = FlOdoChangeupFractionCount;
    TOdoOdoramWholeCounts.crc = OdoComputeCrc(TOdoOdoramWholeCounts.value);
    
        
    /* Start of cPROD_SPEC_RESET_STRATEGY */
   #ifdef cPROD_SPEC_RESET_STRATEGY
    TOdoOdoramWholeCountsCopy1.value = TOdoOdoramWholeCounts.value;
    TOdoOdoramWholeCountsCopy1.crc = TOdoOdoramWholeCounts.crc;
  
    TOdoOdoramWholeCountsCopy2.value = TOdoOdoramWholeCounts.value;
    TOdoOdoramWholeCountsCopy2.crc = TOdoOdoramWholeCounts.crc;
  
   #endif 
   /* End of cPROD_SPEC_RESET_STRATEGY */
    InternalCurrentOdo();
  
	return (cODO_CHANGEUP_ACCEPTED);
  }
  else
  {
	return (cODO_CHANGEUP_NOT_HONOURED);	
  
  }
}
#endif
/*--------------------------------------------------------------------------*/
/*  Name:  OdoCheckChangeUp                                                 */
/*                                                                          */
/*  Return:     boolean  TRUE when at least 2 data sets were updated        */
/*                       FALSE otherwise                                    */
/*                                                                          */
/*  Parameters:  none                                                       */
/*                                                                          */
/*  Visibility:  OdoMdl                                                  */
/*                                                                          */
/*  Description: check that the new odo value requested via OdoChangeup     */
/*               has been successfully written to at least 2 data sets      */
/*                                                                          */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      05/01/00     Bill Derouchie    First Version                        */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#if 0
static boolean OdoCheckChangeUp(void)
{
  SOdoCurrentOdoStruct CurrentOdoDef;

/* have there been at least two data sets updated successfully */
  if (TOdoEpmChangeCounter > 1)
  {
      /* get the last odo value in nvm and apply CRC*/
    if (OdoRead(&CurrentOdoDef) != FALSE)
    {                       
      TOdoLastGoodE2Odo = CurrentOdoDef.NvmValue;
    }  
    else
    {
      TOdoLastGoodE2Odo = cODO_LWR7_NIBBLES_FF;
    }
    AddCrcToUlong(&TOdoLastGoodE2Odo);

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
#endif

///=====================================================================================================================
// Math helpers
//=====================================================================================================================
/*--------------------------------------------------------------------------*/
/*  Name:  OdoComputeCrc                                                    */
/*                                                                          */
/*  Return:     uint8 - bottom 4 bits has the CRC                           */
/*                                                                          */
/*  Parameters:  uint16 value - the 16 bit value for which the CRC will be   */
/*                             generated                                    */
/*                                                                          */
/*  Visibility:  no external visibility                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This routine computes a 4 bit Cyclic Redundancy Check for the input     */
/*  16 bits.  Please refer to "Numerical Recipes In C" by Press etal.       */
/*  Second Edition for a complete explanation of the algorithm              */
/*                                                                          */
/*  The generator polynomial is X^4 + x^3 + x^2 + 1                         */
/*                                                                          */
/*  A precomputed table for each possible nibble value is used to optimize  */
/*  the computation                                                         */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      07/17/98     Bill Derouchie    First Version                        */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static uint8 OdoComputeCrc(uint16 value)
{
/*
 the generator polynomial which is the 4 low order bits of
 the polynomial  x^4 + x^3 + x^2 + 1
*/
  uint8 FlTempCrcResult = 0x0F;     /* init all ones to detect missing leading zeros */

  uint8 i = 0;          /* nibble counter */

/* table driven implementation that processes a nibble at a time */
  do
  {
     FlTempCrcResult = TOdoCrcTable[FlTempCrcResult] ^((uint8)(value&0x000f));
     value = value >>4;
     i=i+1;
/* CYC:BEGIN_TEST_DO_WHILE MAX_ITER=4 EXP_ITER=4 */
  } while (i<4);
/* CYC:END_TEST_DO_WHILE */  

/* to get the actual crc we would need to add the line            */
/*  fl_temp_crc_U8 = crc_table[fl_temp_crc_U8];                                         */
/* but this provides no advantage for the way we use it here      */
/* so save the rom and processor bandwidth                        */

  return FlTempCrcResult;

}

/*--------------------------------------------------------------------------*/
/*  Name:  OdoComputeCrcLong                                                         */
/*                                                                          */
/*  Return:     uint8 - bottom 4 bits has the CRC                           */
/*                      top bit is set if the top nibble of value does not  */
/*                      equal the computed CRC                              */
/*                                                                          */
/*  Parameters:  uint32 value - the 28 bit value for which the CRC will be   */
/*                             generated                                    */
/*                                                                          */
/*  Visibility:  no external visibility                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This routine computes a 4 bit Cyclic Redundancy Check for the bottom    */
/*  28 bits of the input value.  Please refer to "Numerical Recipes In C"   */
/*  by Press etal. 2nd Edition for a complete explanation of the algorithm  */
/*                                                                          */
/*  The generator polynomial is X^4 + x^3 + x^2 + 1                         */
/*                                                                          */
/*  A precomputed table for each possible nibble value is used to optimize  */
/*  the computation                                                         */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      02/23/01     Bill Derouchie    First Version                        */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static uint8 OdoComputeCrcLong(uint32 value)
{
 /*
 the generator polynomial which is the 4 low order bits of
 the polynomial  x^4 + x^3 + x^2 + 1
 */
  uint8 FlTempCrcResult = 0x0F;     /* init all ones to detect missing leading zeros */

  uint8 i = 0;          /* nibble counter */

/* table driven implementation that processes a nibble at a time */
  do
  {
     FlTempCrcResult = TOdoCrcTable[FlTempCrcResult] ^((uint8)(value&0x0000000fUL));
     value = value >>4;
     i=i+1;
/* CYC:BEGIN_TEST_DO_WHILE MAX_ITER=7 EXP_ITER=7 */
  } while (i<7);
/* CYC:END_TEST_DO_WHILE */  

/* to get the actual crc we would need to add the line            */
/*  fl_temp_crc_U8 = crc_table[fl_temp_crc_U8];                               */
/* but this provides no advantage for the way we use it here      */
/* so save the rom and processor bandwidth                        */

/* set the top bit of the return value if the top nibble of input value */
/* is not the same as the computed CRC                            */

  if (FlTempCrcResult != value)
  {
    FlTempCrcResult |= cODO_BAD_CRCL;
  }

  return FlTempCrcResult;
}

/*--------------------------------------------------------------------------*/
/*  Name:  AddCrcToUlong                                                    */
/*                                                                          */
/*  Return:     void                                                        */
/*                                                                          */
/*  Parameters: uint32 * pointer to the ULONG                                */
/*                                                                          */
/*  Visibility:  no external visibility                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  this routine replaces the top nibble of a uint32 with a 4 bit CRC of the */
/*  bottom most 7 nibbles of the uint32                                      */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      02/23/01     Bill Derouchie    First Version                        */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static void AddCrcToUlong(uint32 *pValue)
{  
  
#if defined(cODO_PKG_LITTLE_ENDIAN)
  ((uint8 *)(void *)pValue)[3] = (((uint8 *)(void *)pValue)[3] & (uint8)0x0F) | ((OdoComputeCrcLong(*pValue) & (uint8)0x0F)<<4);
#else
  ((uint8 *)(void *)pValue)[0] = (uint8)((((uint8 *)(void *)pValue)[0] & (uint8)0x0F) | ((OdoComputeCrcLong(*pValue) & (uint8)0x0F)<<4));
#endif
}

/*--------------------------------------------------------------------------*/
/* define some helper functions to check crcs                               */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      1/09/16    	nrajaman           Modified to Turing  Architecture    */
/*--------------------------------------------------------------------------*/
static void RamCheckCrc(const SOdoUwordCrc *p1)
{
  uint8 fl_crc_U8;
  fl_crc_U8 = OdoComputeCrc(p1->value);
  if(fl_crc_U8 != p1->crc)
  {
   
  }
}

static void RamCheckCrcl(uint32 p1)
{
	//uint8 fl_crc_U8;
  if((OdoComputeCrcLong(p1) & cODO_BAD_CRCL) != 0)
  {
   
  }
}


/*--------------------------------------------------------------------------*/
/*  Name: OdoChkParity                                                    */
/*                                                                          */
/*  Return:     uint8 -  1 or zero                                          */
/*                                                                          */
/*  Parameters:  uint8  byte - value to be operated on                      */
/*                                                                          */
/*  Visibility:  no external visibility                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  This routine counts the nuber of '1' bits in the input parameter and    */
/*  returns 1 if odd and zero if even.                                      */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      12/18/2000   Bill Derouchie    First Version                        */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static uint8 OdoChkParity( uint8 byte)
{
  uint8 bit_cntr = 0;

/* CYC:BEGIN_TEST_WHILE MAX_ITER=7 EXP_ITER=4 */
  while ( byte > 0)
/* CYC:END_TEST_WHILE */
  {
    bit_cntr++;

    byte = byte & (uint8)( byte - 1 ) ;  /* clear one of the '1' bits */
  }

  return (bit_cntr & 1);  /* return one if odd parity, zero otherwise */
}


/*--------------------------------------------------------------------------*/
/*  Name:  ConvertKmToMiles                                                 */
/*                                                                          */
/*  Return:     void                                                        */
/*                                                                          */
/*  Parameters:  uint32 *current_odo_value  Address where the odo value in   */
/*                                         km is upon entry and where       */
/*                                         the value in miles is upon exit  */
/*                                                                          */
/*  Visibility:  OdoMdl                                                    */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  the conversion from km to miles is implemented using                    */
/*   Math library                                                           */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      09/01/98     Bill Derouchie      First Version                      */
/*      11/29/2005   Bill Derouchie      corrected overflow in arm code     */
/*                                                                          */
/*      1/09/16    	nrajaman             Modified to Turing  Architecture   */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*static*/ void ConvertKmToMiles(uint32 *pCurrentOdoValue )
{
  // Variables used to use the MAthConvertion library
  uint64 KmToMeter; // Varaible used to hold value in meters
 
 

  KmToMeter = ((uint64)(*pCurrentOdoValue) * 1000);
 
 
  (void)OdoMathConvertDistance( KmToMeter , eDistanceUnits_Miles , cCONVERTION_RESOLUTION  , eRoundingMethods_Floor , pCurrentOdoValue );
  

}

/*--------------------------------------------------------------------------*/
/*  Name    : OdoView                                                       */
/*                                                                          */
/*  Return:     void                                                        */
/*                                                                          */
/*  Parameters:  void                                                       */
/*                                                                          */
/*  Visibility:  OdoMdl                                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  Get the current odo data and provides to the external word              */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      1/09/16    	nrajaman          First Version                         */
/*--------------------------------------------------------------------------*/
void OdoView(void)
{
    if(TRUE == ODO_EMS_NR_TOUT())/* 636 NR*/
    {
        EMS_backupodoMeter_NR = TRUE;
    }
    else 
    {

        EMS_backupodoMeter_NR = FALSE;
    }
		/*Get the current odo value in miles	*/
    GetCurrentOdo(&TOdoMainOdoMile, FALSE);
    
    /*Transmit the current miles to RTE */
    Rte_Write_tiSR_OdoOutMiles_Miles(TOdoMainOdoMile); 

    /*Get the current odo value in Km	*/
    (void)GetCurrentOdo(&TOdoMainOdoKm, TRUE);
    
    /*Transmit the current Km to RTE */
    Rte_Write_tiSR_OdoOutKiloMeters_KiloMeters(TOdoMainOdoKm);

}

/*--------------------------------------------------------------------------*/
/*  Name:  OdoResetMain                                                     */
/*                                                                          */
/*  Return:     void                                                        */
/*                                                                          */
/*  Parameters: void                                                        */
/*                                                                          */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      08/31/18      wli32     First Version                               */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void OdoResetMain(void)
{
   // uint8 fl_index;
	NVOdoDataSetType nv_odo_data_arry[5]={{0x0006U, 0xFFU, (uint8)0x80U},
                                 		   {0x0006U, 0xFFU, (uint8)0x80U},
                                 		   {0x0006U, 0xFFU, (uint8)0x80U},
                                 		   {0x0006U, 0xFFU, (uint8)0x80U},
                                 		   {0x0006U, 0xFFU, (uint8)0x80U}};


    TOdoLastGoodE2Odo = 0;
    AddCrcToUlong(&TOdoLastGoodE2Odo);
    TOdoCurrentOdoKm = 0;
    AddCrcToUlong(&TOdoCurrentOdoKm);
    TOdoOdoramWholeCounts.value = 0;
    TOdoOdoramWholeCounts.crc = OdoComputeCrc(TOdoOdoramWholeCounts.value);
    TOdoOdoRamWholeCountsLast.value = TOdoOdoramWholeCounts.value;
    TOdoOdoRamWholeCountsLast.crc = TOdoOdoramWholeCounts.crc;
    OdoChangeup(0);

#if 0
    odo_write_bytes[0] = 0x00;
    odo_write_bytes[1] = 0x06;
    odo_write_bytes[2] = 0xff;
    odo_write_bytes[3] = 0x80;
    reset_trip_odo(ODO_PKG_TRIP_1); //modified by wli32
    for (fl_index = 0; fl_index < (UINT8) NUMBER_OF_ODO_SETS; fl_index++)
    {
        (void)perform_update_request( GET_SET_ADDR(fl_index),
                                    &odo_write_bytes[0],
                                    NULL,
                                    sizeof(odo_write_bytes) );
    }
#else
    (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(nv_odo_data_arry[0]);
    (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(nv_odo_data_arry[1]);
    (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(nv_odo_data_arry[2]);
    (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(nv_odo_data_arry[3]);
    (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(nv_odo_data_arry[4]);
#endif
    
}

/*--------------------------------------------------------------------------*/
/*  Name    : Sp_OdoDataSet_NvMNotifyJobFinished_JobFinished                                                       */
/*                                                                          */
/*  Return:     void                                                        */
/*                                                                          */
/*  Parameters:  ServiceId - Write / rear service                           */
/*               JobResult -True/false                                      */
/*                                                                          */
/*  Visibility:  OdoMdl                                                     */
/*                                                                          */
/*  Description:                                                            */
/*                                                                          */
/*  Get the current odo data and provides to the external word              */
/*                                                                          */
/* CHANGE HISTORY:                                                          */
/*                                                                          */
/*      1/09/16    	nrajaman          First Version                         */
/*--------------------------------------------------------------------------*/
void Sp_OdoDataSet_NvMNotifyJobFinished_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	(void)OdoDataNVMCallback( ServiceId ,JobResult);
}


void ODO_WriteNVM_Failure_SubFunction(void)
{
 if(TRUE==PowerManager_GetPowerUpStatus())
  {
     NVOdoDataSetType odo_data_arry[5];
     I2c_ResetSCLSDA_UnLocked();
     //NvM_Init();
     EepromInit();
     I2CInit();
     TOdoOdoInternalFlags = 0x01;
      (void)Rte_Read_NV_OdoMdl_NvMOdoData_OdoDataSetArray(odo_data_arry[0]); //Lhbb Rte_MemCpy32
      //  (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(nv_odo_data_arry[0]);//Lhbb below five is Rte_MemCpy
      //  (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(nv_odo_data_arry[1]);
      //  (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(nv_odo_data_arry[2]);
      //  (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(nv_odo_data_arry[3]);
      //  (void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(nv_odo_data_arry[4]);
      if(TOdoOdoSetBeingUpdated == 0)
	  {
         (void)Rte_Write_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(odo_data_arry[0]);
	  }
      if(TOdoOdoSetBeingUpdated == 1)
	  {
         (void)Rte_Write_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(odo_data_arry[1]);
	  }
      if(TOdoOdoSetBeingUpdated == 2)
	  {
         (void)Rte_Write_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(odo_data_arry[2]);
	  }
      if(TOdoOdoSetBeingUpdated == 3)
	  {
         (void)Rte_Write_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(odo_data_arry[3]);
	  }
      if(TOdoOdoSetBeingUpdated == 4)
	  {
         (void)Rte_Write_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(odo_data_arry[4]);
	  }
  }
}

static boolean ODO_EMS_NR_TOUT(void)
{
	uint8  EMS_BackupOdometer_NR = 0;
	uint8  EMS_BackupOdometer_Tout = 0;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x636_ComIn_NR(&EMS_BackupOdometer_NR);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x636_ComIn_Tout(&EMS_BackupOdometer_Tout);
	if((RTE_E_NEVER_RECEIVED == EMS_BackupOdometer_NR) || (RTE_E_TIMEOUT == EMS_BackupOdometer_Tout))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
static void OdoLogicalMdl_To_EMS(void)
{
	uint32 kilometers_odo = 0;
	//uint32 EMS_BackupOdometer = 0;
	boolean IsTimerElapsed= FALSE;
	boolean IsTimerStatrt= FALSE;
	boolean isodochange = FALSE;
	eIgnState fl_ign = 0;
	NV_Data_Trip_A    Maintenance_mileage;
    NV_Data_Trip_B    Backup_odo;
    uint16 backup_odo_count = 0;
    //uint32 datla_currentodo = 0;
    uint32 current_maintance = 0;

	//Get KiloMeters from OdoMdl
	Rte_Read_rpIgnState_IGNState(&fl_ign);// ModMdl.c eKeyPos_Run/eKeyPos_Off/eKeyPos_Count
	Rte_Read_odo_ems_ischange_isodochange(&isodochange);
	if(fl_ign == eIGN_RUN)
	{													/*odo clearing 0 cannot ems odo*/
		if((FALSE == l_ems_odometer_update_complete) && (FALSE == isodochange))
		{
		    EMS_0x584_CanMessage_Compare();
		    EMS_Input_Compare();
			Rte_Read_tiSR_OdoInKiloMeters_KiloMeters(&kilometers_odo);
			if(kilometers_odo == 0x7FFFFFFF)  //SysRS_03_HMIFunction-633 ,if error,CAN output 0xFFFFF
			{
				l_ems_odometer_update_complete = TRUE;
			}
			else 
			{
				Rte_Call_rptimer_OdoLogic_TmExt_Timer_IsStarted(eTimerEMS_ODO_2S,&IsTimerStatrt);
				if(FALSE == IsTimerStatrt)
				{
					Rte_Call_rptimer_OdoLogic_TmExt_Timer_Start(eTimerEMS_ODO_2S);
				}
				Rte_Call_rptimer_OdoLogic_TmExt_Timer_IsElapsed(eTimerEMS_ODO_2S,3000,&IsTimerElapsed);
				if(TRUE == IsTimerElapsed)/*3s time up */
				{
					Rte_Call_rptimer_OdoLogic_TmExt_Timer_Stop(eTimerEMS_ODO_2S);
					if(TRUE == ODO_EMS_NR_TOUT())
					{
						l_ems_backupodometervalue = 0;
					}
					else 
					{
						//Rte_Read_rpSR_CANMSG_GW_EMS_0x636_ComIn_EMS_BackupOdometer(&EMS_BackupOdometer);
						//l_ems_backupodometervalue = EMS_BackupOdometer;
						if(l_ems_backupodometervalue > 9999990)
						{
							l_ems_backupodometervalue = 0;
						}
					}
					if((l_ems_backupodometervalue / 10) > (kilometers_odo / 100))/*update odo */
					{
					    Rte_Read_NV_TripMdl_TripA_value(&Maintenance_mileage);
						EMS_backupodoMeter = l_ems_backupodometervalue;
						OdoChangeup(l_ems_backupodometervalue / 10);
                        Maintenance_mileage.TripIncrease = l_ems_backupodometervalue / 10;
                        Rte_Read_NV_TripMdl_TripB_value(&Backup_odo);
                        backup_odo_count = Backup_odo.TripIncrease +1;
                        Backup_odo.TripIncrease = backup_odo_count;
                        Rte_Write_NV_TripMdl_TripB_value(&Backup_odo);
                        //datla_currentodo = (l_ems_backupodometervalue / 10) - (kilometers_odo / 100);
                        current_maintance = Maintenance_mileage.TripTotalODO - (kilometers_odo / 100);
                        Maintenance_mileage.TripTotalODO = current_maintance + l_ems_backupodometervalue / 10; 
                        Rte_Write_NV_TripMdl_TripA_value(&Maintenance_mileage);
                        Rte_Write_pprpMaintenance_mileage_distance_value(current_maintance); 

					}
					l_ems_odometer_update_complete = TRUE ;
				}
				else
				{
					(void)Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(0xFFFFFF);	/*in 3s send invalue*/
				}
			}
		}
		else 
		{
			Rte_Call_rptimer_OdoLogic_TmExt_Timer_Stop(eTimerEMS_ODO_2S);/*odo clearing 0 cannot ems odo*/
			l_ems_odometer_update_complete = TRUE ;
		}

	}
	else
	{
		l_ems_odometer_update_complete = FALSE; /*IGNoff */
		Rte_Read_tiSR_OdoInKiloMeters_KiloMeters(&kilometers_odo);
		/* Total ODO output to CAN 0x510 */
		if((kilometers_odo == 0x7FFFFFFF) || (TRUE == ODO_EMS_NR_TOUT())) //SysRS_03_HMIFunction-633 ,if error,CAN output 0xFFFFF
		{
			(void)Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(0xFFFFFF);
		}
		else
		{

    			//output Kilometers
        	if(kilometers_odo >= 99999900)
        	{
        		kilometers_odo = 99999900;
        	}
    		(void)Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(kilometers_odo / 10);
          
		}

	}

	if(TRUE == l_ems_odometer_update_complete)
	{
		Rte_Read_tiSR_OdoInKiloMeters_KiloMeters(&kilometers_odo);
        if(TRUE == ODO_EMS_NR_TOUT())/* 636 NR*/
		{
            (void)Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(0xFFFFFF);
		}
        else
        {
    		/* Total ODO output to CAN 0x510 */
    		if(kilometers_odo == 0x7FFFFFFF)  //SysRS_03_HMIFunction-633 ,if error,CAN output 0xFFFFF
    		{
    			(void)Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(0xFFFFFF);
    	
            }   
            else
            {
        		if(kilometers_odo >= 99999900)
        		{
        			kilometers_odo = 99999900;
        		}
    			(void)Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(kilometers_odo / 10);
            }
        }
	}

}

static void Ems_backupodo_TimerIsStart(void)
{
	boolean IsTimerStatrt= FALSE;
	Rte_Call_rptimer_OdoLogic_TmExt_Timer_IsStarted(eTimerEMS_ODO_2S,&IsTimerStatrt);
	if(FALSE == IsTimerStatrt)
	{
		Rte_Call_rptimer_OdoLogic_TmExt_Timer_Start(eTimerEMS_ODO_2S);
	}
	(void)Rte_Write_ppSR_CANMSG_IC_0x510_ComOut_IC_OdometerMasterValue(0xFFFFFF);

}

static void Vehicle_cfg_init(void)
{
	uint8 fl_vehicle_cfg_Tire_flg = 0;

	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_TIRE,&fl_vehicle_cfg_Tire_flg);
	if(fl_vehicle_cfg_Tire_flg != 0 )
	{
		cPROD_SPEC_COUNTS_PER_KM = Vehicle_Tire_245; /*185*/
	}
	else 
	{
		cPROD_SPEC_COUNTS_PER_KM = Vehicle_Tire_235;/*215*/
	}
	
}

static void EMS_Input_Compare(void)
{
    static uint8 countevalue = 0;
    uint32 u32_EMS_BackupOdometer = 0;
    static uint32 Pre_EMSbackupodometer = 0;
    static uint8 counter = 0;
    counter++;
    if(counter >= 5)
    {
    
        Rte_Read_rpSR_CANMSG_GW_EMS_0x636_ComIn_EMS_BackupOdometer(&u32_EMS_BackupOdometer);
        if(u32_EMS_BackupOdometer != 0)
        {
            if(Pre_EMSbackupodometer == u32_EMS_BackupOdometer)
            {
               countevalue++; 
            }
            else 
            {
              countevalue = 0;  
              l_ems_backupodometervalue = 0;
            }
            Pre_EMSbackupodometer = u32_EMS_BackupOdometer;
            if(countevalue >= 10)
            {
                l_ems_backupodometervalue = u32_EMS_BackupOdometer; 
                countevalue = 0;
            }
        }
        else 
        {
            l_ems_backupodometervalue = 0;
            countevalue = 0;
        }
        counter = 0;
     }
}

static void EMS_0x584_CanMessage_Compare(void)
{
    uint8 CarModeIndicator = 0;
    Rte_Read_rpSR_CANMSG_BCM_0x584_ComIn_BCM_CarModeIndicator(&CarModeIndicator);
    if((CarModeIndicator == 5) || (CarModeIndicator == 2))
    {
        l_ems_odometer_update_complete = TRUE;
    }
}
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
**Change Description : Initial version of OdoMdl
**============================================================================
**============================================================================
**Date               : 15-Sep-2016
**CDSID              : nrajaman
**Traceability       : RTC #662890
**Change Description : 1.Review comments updated.
                       2. Odo call back function from RTE interface is added.
                       3. LITTLE and BIG ENDIAN concepts added. 
**============================================================================
**============================================================================
**Date               : 16-Sep-2016
**CDSID              : nrajaman
**Traceability       : RTC #662890
**Change Description : 1.Review comments updated.                       
**============================================================================*/

