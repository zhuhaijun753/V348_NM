/*
** These FUEL03 configuration file parameters are automatically
** generated from the Fuel03BToolSingle V1.0 created and used by the
** subsystems group.  DO NOT MODIFY any parameters.  If a change is
** required, get the subsystems engineer to update the Fuel03BToolSingle
** calculations and have them provide a new fuel03_bds.cfg worksheet for
** use by software.
**
*/

/*
** --------------------------------------------------------------------------
** Fuel algorithm configuration parameters
** --------------------------------------------------------------------------
*/
#define NISSAN_CHINA
/*
** Define the table to convert from the 5 bits of the B&A SCEM byte into an
** index that is used by FUEL03 to determine which entry in the various
** configuration parameter arrays are active.
**
** The table is a 1 byte input SCEM byte with a 1 byte output index value.
*/
/*#define FUEL03_INDEX_TABLE   \
  {  1, 0  }, \
  {  2, 1  }, \
  {  4, 2  }
*/


/*
** Define the following macro for the number of tanks, or tank
** combinations used.  The only valid values are from 1 to 5.
*/
#define FUEL03_SCEM_NUMBER_OF_FUEL_TANKS                  (1)

/*
** Define the type of average to use when sampling the fuel A/D input.
** The valid choices are:
** 0 = No average, i.e. single sample
** 1 = Moving average, average of X samples
** 2 = Set with faults, average of X samples, include fault in average
** 3 = Set without faults, average of X samples, ignore faults
*/
#define FUEL03_AVG_TYPE                                   (0)

/*
** Define the number of fuel samples to average if the FUEL03_AVG_TYPE is
** greater than 0. If you set FUEL03_AVG_TYPE to 0 for no average, then
** set this to 1.
*/
#define FUEL03_SMPLS_2_AVG                                (8)

/*
** Define the type of averaging to use to calculate the ZSS and KO
** reference samples.  The valid choices are:
** 0 = No average, i.e. single sample
** 1 = Moving average, average of X samples
** 2 = Set with faults, average of X samples, include fault in average
** 3 = Set without faults, average of X samples, ignore faults
** 4 = Convergence, Ford convergence algorithm only
*/
#define FUEL03_REF_AVG_TYPE                               (0)


/*
** Define the following to select the resolution to use when
** comparing the input FLPS value versus the slosh filter.
** If set to 1, the 1 byte input FLPS will be compared
** to the 1 byte MSB of the slosh filter to see if the filter
** should be incremented or decremented.  If set to 0,
** the comparison will be 2 bytes.
*/
#define FUEL03_FILTER_RES                                 (0)



/*
** --------------------------------------------------------------------------
** Fuel initialization configuration parameters
** --------------------------------------------------------------------------
*/

/*
** Define the following if you want hill hold enabled.  Otherwise, comment
** it out.
*/
#define FUEL03_HILL_HOLD_ENABLE

/*
** Set the following to 1 if you require a filter jam with no good fuel
** init reference samples and hill hold is enabled.  Comment it out
** or set to 0 to disable.
*/
#define FUEL03_FORCE_FILT_JAM                             (0)

/*
** Define the time period after the ignition switch has transitioned into
** RUN before fuel init samples are to be taken.  The time base for this
** counter will be the same as that chosen for the filter rate, and
** the counter should be long enough to ensure that any power fluctuations
** have stabilized enough to get good samples.
*/
#define FUEL03_KEY_ON_DELAY_TIME                          (1)

/*
** Define the time period after power stabilization during which to take
** fuel init samples.  Once this time period has expired, a one-time
** fuel init will take place.  The time base for this counter will be the
** same as that chosen for the filter rate, and the counter should be long
** enough to ensure that a good quantity of init samples will be taken.
*/
//#define FUEL03_INIT_DELAY_COUNT_TIME                      	(30)			/* 100ms * 30 = 3000ms	*/
#define FUEL03_INIT_DELAY_COUNT_TIME                      	(1)				/* 100ms * 1 = 100ms	*/


/*
** Define the minimum delta increase ( >= ) in percent full terms that must
** be detected during fuel init to jam the fuel filter.
** delete 4L / (47.2L-3.8L) * 100 % * 254
** delete 4L / (68L -3.5L) * 100 % * 254
** 4L / (64L -2.9L) * 100 % * 254
*/
//#define FUEL03_POSITIVE_INIT_FILL_DELTA_A                 (14)	
#define FUEL03_POSITIVE_INIT_FILL_DELTA_A                 	(17)				


/*
** Define the minimum delta decrease ( >= ) in percent full terms that must
** be detected during fuel init to jam the fuel filter.
** delete 4L / (47.2L-3.8L) * 100 % * 254
** delete 4L / (68L -3.5L) * 100 % * 254
** 4L / (64L -2.9L) * 100 % * 254
*/
//#define FUEL03_NEGATIVE_INIT_FILL_DELTA_A					(14)
#define FUEL03_NEGATIVE_INIT_FILL_DELTA_A					(17)


/*
** Define the secondary delta init fill positive and negative values.
*/
//#define FUEL03_SEC_POS_INIT_FILL_DELTA_A                  (115)

//#define FUEL03_SEC_NEG_INIT_FILL_DELTA_A                  (115)

#define FUEL03_SEC_POS_INIT_FILL_DELTA_A                  	(0xFF)

#define FUEL03_SEC_NEG_INIT_FILL_DELTA_A                  	(0xFF)

/*
** Define the threshold at which to check for rental car topoff.   The
** MSB of the fuel filter must exceed this threshold before a check will
** take place.
**
** By setting this value to 0xFF, rental car topoff will be disabled.
*/
//#define FUEL03_SENDERA_RC_FILL_THRESHOLD                  (115)
#define FUEL03_SENDERA_RC_FILL_THRESHOLD                  	(0xFF)

/*
** Define the fill check to use for rental car topoff.  If the MSB of
** the fuel filter exceeds the above threshold by this amount, the
** fuel filter will be jammed.
**
** By setting this value to 0xFF, rental car topoff will be disabled.
*/
//#define FUEL03_SENDERA_RC_FILL_DELTA                      (9)
#define FUEL03_SENDERA_RC_FILL_DELTA                      (0xFF)


/*
** --------------------------------------------------------------------------
** Fuel open/short configuration parameters
** --------------------------------------------------------------------------
*/

/*
** Ratioed A/D values GREATER than the following are considered open.
** Setting this value to "0xFFFF" will effectively stop open detect.
*/
#define FUEL03_OPEN_SENDER_LIMIT                          (0xFFFF)

/*
** Ratioed A/D values LESS than the following are considered shorted.
** Setting this value to "0" will effectively stop short detect.
*/
#define FUEL03_SHORTED_SENDER_LIMIT                       (0x0)

/*
** Define the time period for debouncing an open/short sender.  The time base
** for these counters will be the same as that chosen for the filter rate.
*/
#define FUEL03_OPEN_SHORT_FAULT_TIME                      (600)

/*
** Define the time period for recovering from an open/short sender.  The time
** base for these counters will be the same as that chosen for the filter
** rate.
*/
#define FUEL03_OPEN_SHORT_RECOVERY_TIME                   (600)

/*
** Define the following if you want the Honda open/short recovery filter jam
** feature enabled.  Otherwise, comment it out.
*/
#define FUEL03_FF_RECOV_FLTR_JAM_ENABLE



/*
** --------------------------------------------------------------------------
** Fuel fast fill configuration parameters
** --------------------------------------------------------------------------
*/

/*
** Define the following if you want fast fill enabled.  Otherwise, comment
** it out.
*/
#define FUEL03_FAST_FILL_ENABLE

/*
** Define the rate at which the zero speed counter is to be incremented.
** This constant times the rate of the fuel filter algorithm will define
** the time after zero speed at which to store the A/D sample for init
** and fast fill purposes.
*/
#define FUEL03_ZERO_SPEED_COUNTER                         (0)	/* Using 0 sec instead of 20s in N330 TFT */

/*
** Define the minimum delta increase ( >= ) in percent full terms used as one
** of the entrance conditions to "fast" fill mode.  This value could be
** set to a very large number as one method to inhibit fast fill mode.
**
** 4L / (47.2L-3.8L) * 100 % * 254
*/
//#define FUEL03_POSITIVE_FAST_FILL_DELTA_A                 (23)	

/*
** Define the minimum delta increase ( >= ) in percent full terms used as one
** of the entrance conditions to "fast" fill mode.  This value could be
** set to a very large number as one method to inhibit fast fill mode.
**
** 4L / (68L -3.5L) * 100 % * 254
*/
//#define FUEL03_POSITIVE_FAST_FILL_DELTA_A                 (157)	
#define FUEL03_POSITIVE_FAST_FILL_DELTA_A                  (400)	  //4L


/*
** Define the minimum delta increase ( >= ) in percent full terms used as one
** of the entrance conditions to "fast" fill mode.  This value could be
** set to a very large number as one method to inhibit fast fill mode.
**
** 4L / (47.2L-3.8L) * 100 % * 254
*/
//#define FUEL03_NEGATIVE_FAST_FILL_DELTA_A                 (23)

/*
** Define the minimum delta increase ( >= ) in percent full terms used as one
** of the entrance conditions to "fast" fill mode.  This value could be
** set to a very large number as one method to inhibit fast fill mode.
**
** 4L / (68L -3.5L) * 100 % * 254 * 10
*/
//#define FUEL03_NEGATIVE_FAST_FILL_DELTA_A                 (157)
#define FUEL03_NEGATIVE_FAST_FILL_DELTA_A                 (400)		//4L


/*
** Define the base delta value to be added/subtracted from the 4 byte
** fuel filter when it is appropriate to iterate it.  This value
** more or less determines the "rate" at which the filter moves, and
** is the value used when the filter is running in "fast" mode of
** operation.  It can be set to the same value as the "normal"
** mode base if "fast" mode is not desired.
*/
//#define FUEL03_DELTA_FAST_BASE_A 							(0x483FB7ul)		//4734903
//#define FUEL03_DELTA_FAST_BASE_A 							(894784853ul)
#define FUEL03_DELTA_FAST_BASE_A 							(21378485ul)


/*
** Define the "adjustment" factor that is added to the least significant
** word of the 4 byte fuel filter whenever consecutive samples of the fuel
** A/D fall on the same side of the filter.  This value is used when the
** filter is running in "fast" mode, and can be set to "0" if no adjustment
** is desired.
**
** For example, if two samples in a row fall below the current filter value,
** this constant will be subtracted from the filter along with the delta
** value defined above.  Three samples mean that twice this value is
** subtracted, and so on.  Whenever a sample on the opposite side of the
** filter is encountered, the number of adjustment factors to be added
** is reset to zero.
*/
#define FUEL03_DELTA_FAST_ADJUST_A                        (0x0000)

/*
** Define a max clamp value for the fuel delta when in fast mode.
*/
//#define FUEL03_DELTA_FAST_CLAMP_A                         (0x00FFFFFFuL)
#define FUEL03_DELTA_FAST_CLAMP_A                           (0x3FFFFFFFuL)

/*
** Define the following if you want the Honda fast fill rate check enabled.
** Otherwise, comment it out.
*/
/*#define FUEL03_FFRC_ENABLE*/

/*
** Define the rate at which raw fuel samples are taken to create the moving
** fuel sample window.  This is needed if you enable FUEL03_FFRC_ENABLE.
*/
/*#define FUEL03_RATE_CHECK_TIMER_START_VALUE               (10)*/

/*
** If you define FUEL03_FFRC_ENABLE, then define the number of raw fuel
** samples to be taken at the fuel sample rate in the moving window.
*/
/*#define FUEL03_MAX_RATE_CHECK_SAMPLES                     (5)*/

/*
** If you define FUEL03_FFRC_ENABLE, then set the lower rate check limit.
*/
/*#define FUEL03_LOWER_RATE_CHECK_LIMIT_A                   {325, 325, 234}*/

/*
** If you define FUEL03_FFRC_ENABLE, then set the upper rate check limit.
*/
/*#define FUEL03_UPPER_RATE_CHECK_LIMIT_A                   {4881, 4885, 3524}*/

/*
** Define the time that the fast fill mode is enabled following a key on
** initialization that does not read the required number of good fuel
** samples.
*/
/*#define FUEL03_AVG_VOL_INVALID_TIME                       (900)*/

/*
** Define the time that the fast fill mode determination is debounced
** before fast fill mode can be entered (due to sloshing present fuel
** level)
*/
//#define FUEL03_MIN_FAST_FILL_TIME                         (200)					/* 200 * 100ms = 20s	*/
//#define FUEL03_MIN_FAST_FILL_LATENCY_TIME				  (20)					/* 20 * 100ms = 2 sec	*/

#define FUEL03_MIN_FAST_FILL_TIME                         (40)					/* 40 * 500ms = 20s	*/
#define FUEL03_MIN_FAST_FILL_LATENCY_TIME				  (4)					/* 4 * 500ms = 2 sec	*/

/*
** --------------------------------------------------------------------------
** Fuel slosh filter configuration parameters
** --------------------------------------------------------------------------
*/

/*
** Define the four byte slosh filter empty preload.  This value is stored
** in the slosh filter during a cold init, and is the filter value that will
** end up being used if the fuel init fails.
*/
#define FUEL03_SLOSH_FILTER_EMPTY_PRELOAD_A               (0x07000000uL)

/*
** Define the parameter = 1 to use multiple filter times based upon vehicle
** speed, If only 1 normal filterrate is desired, then set this value to 0
*/
#define FUEL03_MULT_FILT_RATES							(0)

/*
** Define the vehicle Speed Threshold 1 = to or below which the NORMAL_BASE_A1
** will be selected for the slosh filter time.  Setting this to a value
** and SPD_THRESH_2 to max will provide twofilter times;
*/
/*#define FUEL03_SPD_THRESH_1                               (8000)*/

/*
** Define the vehicle Speed Threshold 2 above which the NORMAL_BASE_A3
** will be selected for the slosh filter time; if vehicle speed is between
** Speed Threshold 1 and 2, then NORMAL_BASE_A2 will be selected
*/
/*#define FUEL03_SPD_THRESH_2                               (15000)	*/

/*
** Define the base delta values to be added/subtracted from the 4 byte
** fuel filter when it is appropriate to iterate it.  This value
** more or less determines the "rate" at which the filter moves, and
** is the value used when the filter is running in "normal" mode of
** operation.  Which Base to use will be determined by the vehicle
** speed input evaluated against SPD_THRESH_1 and SPD_THRESH_2
*/

#define FUEL03_DELTA_NORMAL_BASE_A1                 		350446
//#define FUEL03_DELTA_NORMAL_BASE_A1                 		430446
//#define FUEL03_DELTA_NORMAL_BASE_A1                 		400446      
//#define FUEL03_DELTA_NORMAL_BASE_A1                       (0x348B7uL)		//215223
//#define FUEL03_DELTA_NORMAL_BASE_A1                       (179917uL)		


/*
** Define the base delta values to be added/subtracted from the 4 byte
** fuel filter when it is appropriate to iterate it.  This value
** more or less determines the "rate" at which the filter moves, and
** is the value used when the filter is running in "normal" mode of
** operation.  Which Base to use will be determined by the vehicle
** speed input evaluated against SPD_THRESH_1 and SPD_THRESH_2
*/

#define FUEL03_DELTA_NORMAL_BASE_B1                 		180//5004


/*
** Define the "adjustment" factor that is added to the least significant
** word of the 4 byte fuel filter whenever consecutive samples of the fuel
** A/D fall on the same side of the filter.  This value is used when the
** filter is running in "normal" mode, and can be set to "0" if no adjustment
** is desired.
**
** For example, if two samples in a row fall below the current filter value,
** this constant will be subtracted from the filter along with the delta
** value defined above.  Three samples mean that twice this value is
** subtracted, and so on.  Whenever a sample on the opposite side of the
** filter is encountered, the number of adjustment factors to be added
** is reset to zero.
*/
#define FUEL03_DELTA_NORMAL_ADJUST_A                      (0)

/*
** Define a max clamp value for the fuel delta when in normal mode.
*/
#define FUEL03_DELTA_NORMAL_CLAMP_A                       (0x00FFFFFFuL)

/*
** Define the minimum delta increase ( >= ) in percent full terms that
** the percent full value must change by in response to a filter change.
**
** By setting this constant to 0 or 1, there will effectively be
** no hysteresis applied.
*/
#define FUEL03_PERCENT_FULL_POSITIVE_HYST                 (1)

/*
** Define the minimum delta decrease ( >= ) in percent full terms that
** the percent full value must change by in response to a filter change.
**
** By setting this constant to 0 or 1, there will effectively be
** no hysteresis applied.
*/
#define FUEL03_PERCENT_FULL_NEGATIVE_HYST                 (1)

/*
** Define the table to convert sender A from A/D to FLPS.
*/

	
/* 
**		- RAW Data  : 	CAN Signal 'Fuel_Level_Volumn', Resolution = 0.01 
**		- PF		:		0%     = 0u * 256
**						100% = 254u * 256
**		- TABLE		:
**						3.8L		0 degree				0%
**						6.3L		30 degree			12%
**						14.1L	60 degree			23%
**						19.4L	90 degree			35%
**						25.3L	120 degree			49%
**						29.1L	150 degree			58%
**						38.9L	180 degree			80%
**						42.8L	210 degree			89%	       
**						47.2L	240 degree			100%      
*/

#define FUEL03_SENDER_RAW_TO_PF_TBL_SIZE (14)
#define FUEL03_SENDER_RAW_TO_PF_TBL 					\
		{(uint16)(0u),      (uint16)(254*256u)},		\
		{(uint16)(0u), 		(uint16)(0u*256u)},			\
		{(uint16)(350u),    (uint16)(13u*256u)},		\
		{(uint16)(800u),  	(uint16)(29u*256u)},		\
		{(uint16)(1150u),  	(uint16)(42u*256u)},		\
		{(uint16)(1850u), 	(uint16)(69u*256u)},		\
		{(uint16)(2570u), 	(uint16)(95u*256u)},		\
		{(uint16)(2900u), 	(uint16)(108u*256u)},		\
		{(uint16)(3400u), 	(uint16)(127u*256u)},		\
		{(uint16)(3850u), 	(uint16)(143u*256u)},		\			
//		{(uint16)(4450u), 	(uint16)(166u*256u)}, 		\
//		{(uint16)(4850u), 	(uint16)(181u*256u)},		\
//		{(uint16)(5250u), 	(uint16)(196u*256u)},		\
//		{(uint16)(5700u), 	(uint16)(212u*256u)}, 		\
//		{(uint16)(6150u), 	(uint16)(229u*256u)}, 		\
//		{(uint16)(6528u), 	(uint16)(243u*256u)}, 		\
//		{(uint16)(6800u), 	(uint16)(254u*256u)}, 		\


/*
** --------------------------------------------------------------------------
** Reference sample (ZSS, KO) convergence / sampling configuration parameters
** --------------------------------------------------------------------------
*/

/*
** Define the maximum allowed key-off sampling time (in units of the
** filter iteration rate).
*/
#define FUEL03_KEY_OFF_SAMPLE_TIME                        (100)

/*
** Define the following for Key Off Convergence (FUEL03_REF_AVG_TYPE=4).
** This controls the number of raw samples taken in a convergence cycle.
** If you don't use KOC, then any value is OK since it won't be used.
*/
/*#define FUEL03_KOS_COUNTS                                 (50)*/

/*
** Define the following for Key Off Convergence (FUEL03_REF_AVG_TYPE=4).
** This controls the maximum number of convergence cycles to take.
** If you don't use KOC, then any value is OK since it won't be used.
*/
/*#define FUEL03_KOS_CYCLES                                 (4)*/

/*
** Define the following for Key Off Convergence (FUEL03_REF_AVG_TYPE=4).
** This controls the minimum number of unfaulted samples in a convergence
** cycle that is required.  If we don't get this many, then that cycle
** did not converge.  If you don't use KOC, then any value is OK since
** it won't be used.
*/
/*#define FUEL03_KOS_MIN_SMPLS                              (20)*/

/*
** Define the delta between the min and max sample in a key-off
** convergence window.  If the delta is less than this value, the
** samples are considered converged.
*/
#define FUEL03_KO_DELTA_A                                 (4)

/*
** Define the following 3 values for the fast fill convergence.
** These definitions are identical to the 3 defined above, except
** these are for fast fill.
*/
#define FUEL03_ZS_COUNTS                                  (50)
#define FUEL03_ZS_CYCLES                                  (4)
#define FUEL03_ZS_MIN_SMPLS                               (20)

/*
** Define the delta between the min and max sample in a fast fill
** convergence window.  If the delta is less than this value, the
** samples are considered converged.
*/
#define FUEL03_ZS_DELTA_A                                 (4)



/*
** --------------------------------------------------------------------------
** NVM fault logging configuration parameters
** --------------------------------------------------------------------------
*/

/*
** Define the following if you would like log the fuel init values to NVM.
** Comment it out if not used.
**
** Note: If you use this NVM logging, you need to make sure the queue size
**       in the eprom manager is big enough (EPM_Q_SIZE).
*/

/*
** Define the number of 'records' you want to store in RAM.
** Comment it out if not used.
*/
/*#define FUEL03_RAM_LOG_FAULT_RECORD_SIZE                  (10) */

/*
** Define the number of 'records' you want to store in NVM.
** Comment it out if not used.
*/
/*#define FUEL03_LOG_FAULT_RECORD_SIZE                      (5)  */

/*
** Set the following to the number of samples on the same side of the
** filter that need to occur before the fuel data is declared faulted
** and logged to NVM.
*/
#define FUEL03_ERR_CNT                                    (3000)

/*---------------------------------------------------------------------------
Release Label     : dev-20140820-MY15_MAHINDRA_U301_IC_BASELINE_7 
Date              : 20-Aug-2014
By                : sgajendr
Traceability      : SMIS PR id:75979
Change Description: Updated the FUEL CONSTANTS macros as per PR comments. 
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Release Label     : dev-20140820-MY15_MAHINDRA_U301_IC_BASELINE_12 
Date              : 29-Sep-2014
By                : sgajendr
Traceability      : 2013_MnM_U301_IC_SRD_V2.3.doc
Change Description: Updated the Key Off sampling time macro (15sec). 
-----------------------------------------------------------------------------*/
