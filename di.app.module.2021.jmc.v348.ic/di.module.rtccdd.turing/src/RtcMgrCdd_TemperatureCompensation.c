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

#ifndef RTCMGRCDD_TEMPERATURECOMPENSATION_C
#define RTCMGRCDD_TEMPERATURECOMPENSATION_C

#include "Rte_RtcCdd.h"
#include "RtcMgrCdd_TemperatureCompensation.h"
#include "RtcMgrCdd_Cfg.h"
#include "RtcIfCdd.h"
#include "MathLib.h"
#include "AssertLib.h"
#include "EcuM.h"

#if (SW_COMPENSATION==STD_ON)
//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
//substates for compensation algorithm
typedef enum CompensationState
{
    CompensationState_Step1,
    CompensationState_Step2,
    CompensationState_Step3,
    CompensationState_Step4
} CompensationState_Type;

//compensation mode
typedef enum CompensationMode
{
    RTC_SLEEP_MODE,
    RTC_NORMAL_MODE
} CompensationMode_Type;

//count of interpolation points
#define cINTERPOLATION_POINTS_COUNT     ((uint8)141)

//temperature value of previous measurement
#define cTEMP_PREV_DEFAULT_VALUE        ((sint32)25)

//general purpose constants
#define c1200                           ((uint32)1200)
#define c24                             ((uint32)24)
#define c250                            ((uint32)250)
#define c5                              ((uint32)5)
#define c25                             ((sint32)25)
#define c3                              ((uint32)3)
#define c2                              ((uint32)2)

//constants for conversion
#define cSecondsInMinute                (60)
#define cSecondsInHour                  (60 * 60)
#define cSecondsIn24Hours               (24 * 60 * 60)

#define cRTC_FIRST_COMPENSATION_DONE    (uint32)0x454E4F44

//flag indication for Gpt timeout IRQ present
static boolean RtcIrqPresent = FALSE;

//compensation state
static CompensationState_Type CompensationState = CompensationState_Step1;

//compensation mode
static CompensationMode_Type CompensationMode = RTC_SLEEP_MODE;

//the following data must be kept alive across wakeup-sleep cycles
#pragma ghs section bss=".buram"
    //accumulated deviation of RTC clock in 1/4 us units
    static sint32 ErrCumulate;
    //value of previous temperature measurement
    static sint32 Temp_Prev;
    //flag which indicates that the first compensation has to be done with cRTC_DEVIATION_LIMIT
    static uint32 RtcFirstCompensationDone;
#pragma ghs section bss=default

//temprature interpolation values
static const sint8 TempDegrees[cINTERPOLATION_POINTS_COUNT] =
{
    (sint8)100,
    (sint8)99,	(sint8)98,	(sint8)97,	(sint8)96,	(sint8)95,	(sint8)94,	(sint8)93,	(sint8)92,	(sint8)91,	(sint8)90,
    (sint8)89,	(sint8)88,	(sint8)87,	(sint8)86,	(sint8)85,	(sint8)84,	(sint8)83,	(sint8)82, 	(sint8)81,	(sint8)80,
    (sint8)79,	(sint8)78,	(sint8)77,	(sint8)76,	(sint8)75,	(sint8)74,	(sint8)73,	(sint8)72,	(sint8)71,	(sint8)70,
    (sint8)69,	(sint8)68,	(sint8)67,	(sint8)66,	(sint8)65,	(sint8)64,	(sint8)63,	(sint8)62,	(sint8)61, 	(sint8)60,
    (sint8)59,	(sint8)58,	(sint8)57,	(sint8)56,	(sint8)55,	(sint8)54,	(sint8)53,	(sint8)52,	(sint8)51,	(sint8)50,
    (sint8)49,	(sint8)48,	(sint8)47,	(sint8)46,	(sint8)45,	(sint8)44,	(sint8)43,	(sint8)42,	(sint8)41,	(sint8)40,
    (sint8)39,	(sint8)38,	(sint8)37,	(sint8)36,	(sint8)35,	(sint8)34,	(sint8)33,	(sint8)32,	(sint8)31,	(sint8)30,
    (sint8)29,	(sint8)28,	(sint8)27,	(sint8)26,	(sint8)25,	(sint8)24,	(sint8)23,	(sint8)22,	(sint8)21,	(sint8)20,
    (sint8)19,	(sint8)18,	(sint8)17,	(sint8)16,	(sint8)15,	(sint8)14,	(sint8)13,	(sint8)12,	(sint8)11,	(sint8)10,
    (sint8)9,	(sint8)8,	(sint8)7,	(sint8)6,	(sint8)5,	(sint8)4,	(sint8)3,	(sint8)2,	(sint8)1,	(sint8)0,
    (sint8)-1,	(sint8)-2,	(sint8)-3,	(sint8)-4,	(sint8)-5,	(sint8)-6,	(sint8)-7,	(sint8)-8,	(sint8)-9,	(sint8)-10,
    (sint8)-11,	(sint8)-12,	(sint8)-13,	(sint8)-14,	(sint8)-15,	(sint8)-16,	(sint8)-17,	(sint8)-18,	(sint8)-19,	(sint8)-20,
    (sint8)-21,	(sint8)-22,	(sint8)-23,	(sint8)-24,	(sint8)-25,	(sint8)-26,	(sint8)-27,	(sint8)-28,	(sint8)-29,	(sint8)-30,
	(sint8)-31,	(sint8)-32,	(sint8)-33,	(sint8)-34,	(sint8)-35,	(sint8)-36,	(sint8)-37,	(sint8)-38,	(sint8)-39,	(sint8)-40
};

//adc interpolation values
static const uint16 ADCValues[cINTERPOLATION_POINTS_COUNT] =
{
    (uint16)189,
    (uint16)195,	(uint16)202,	(uint16)208,	(uint16)215,	(uint16)222,	(uint16)229,	(uint16)237,
    (uint16)244,	(uint16)252,	(uint16)261,	(uint16)269,	(uint16)278,	(uint16)288,	(uint16)297,
    (uint16)307,	(uint16)317,	(uint16)328,	(uint16)339,	(uint16)351,	(uint16)363,	(uint16)375,
    (uint16)387,	(uint16)401,	(uint16)415,	(uint16)429,	(uint16)444,	(uint16)459,	(uint16)475,
    (uint16)491,	(uint16)507,	(uint16)526,	(uint16)543,	(uint16)562,	(uint16)581,	(uint16)600,
    (uint16)622,	(uint16)643,	(uint16)664,	(uint16)687,	(uint16)710,	(uint16)735,	(uint16)760,
    (uint16)785,	(uint16)811,	(uint16)838,	(uint16)865,	(uint16)894,	(uint16)924,	(uint16)955,
    (uint16)986,	(uint16)1018,	(uint16)1051,	(uint16)1085,	(uint16)1120,	(uint16)1156,	(uint16)1193,
    (uint16)1230,	(uint16)1268,	(uint16)1308,	(uint16)1348,	(uint16)1389,	(uint16)1431,	(uint16)1473,
    (uint16)1516,	(uint16)1560,	(uint16)1606,	(uint16)1651,	(uint16)1696,	(uint16)1743,	(uint16)1789,
    (uint16)1836,	(uint16)1884,	(uint16)1933,	(uint16)1982,	(uint16)2032,	(uint16)2082,	(uint16)2132,
    (uint16)2181,	(uint16)2231,	(uint16)2281,	(uint16)2334,	(uint16)2383,	(uint16)2432,	(uint16)2481,
    (uint16)2530,	(uint16)2579,	(uint16)2627,	(uint16)2675,	(uint16)2723,	(uint16)2770,	(uint16)2818,
    (uint16)2863,	(uint16)2908,	(uint16)2951,	(uint16)2994,	(uint16)3037, 	(uint16)3079,	(uint16)3119,
    (uint16)3159,	(uint16)3197,	(uint16)3235,	(uint16)3271,	(uint16)3307,	(uint16)3342,	(uint16)3375,
    (uint16)3409,	(uint16)3440,	(uint16)3470,	(uint16)3499,	(uint16)3527,	(uint16)3555,	(uint16)3581,
    (uint16)3606,	(uint16)3630,	(uint16)3653,	(uint16)3675,	(uint16)3696,	(uint16)3716,	(uint16)3736,
    (uint16)3754,	(uint16)3772,	(uint16)3789,	(uint16)3805,	(uint16)3820,	(uint16)3834,	(uint16)3848,
    (uint16)3861,	(uint16)3873,	(uint16)3885,	(uint16)3896,	(uint16)3907,	(uint16)3917,	(uint16)3926,
    (uint16)3935,	(uint16)3943,	(uint16)3931,	(uint16)3940,	(uint16)3948,	(uint16)3956,	(uint16)3963
};

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static void 	ApplyCorrection			(uint32 DeviationLimit);
static sint32   CalculateRTCDeviation   (TIOAppData AdcValue, uint16 RTCCalib_K, sint16 RTCCalib_E_R_FC);
static boolean 	IsRtcIrqPresent			(void);
static void 	PerformInit				(void);
static void		AddSeconds				(Rtc_TimeType* Time, sint8 Sec);

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
//---------------------------------------------------------------------------------------------------------------------
/// @brief Applies the correction of the RTC
//---------------------------------------------------------------------------------------------------------------------
static void ApplyCorrection(uint32 DeviationLimit)
{
    Std_ReturnType RetVal;
    Rtc_TimeType CurrTime;
    sint8 SecondsToAdd;

    //read current time
    RetVal = RtcIfCdd_GetTime(&CurrTime);
    if(RetVal == E_OK)
    {
        //calculate new time
        SecondsToAdd = (ErrCumulate > 0) ? (sint8)1 : (sint8)-1;
        AddSeconds(&CurrTime, SecondsToAdd);

        //update time
        RetVal = RtcIfCdd_UpdateTime(CurrTime);
        if(RetVal == E_OK)
        {
            //decrease deviation
            ErrCumulate -= (sint32)(DeviationLimit * SecondsToAdd);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Adds seconds to given Time
///
/// @arg    Time: The Time to be updated
/// @arg    Sec: The seconds to be added to the Time
//---------------------------------------------------------------------------------------------------------------------
static void	AddSeconds (Rtc_TimeType* Time, sint8 Sec)
{
    uint32 TimeInSeconds;
    uint8 Temp;

    //convert Rtc_TimeType to Seconds
    TimeInSeconds = Time->Hour * cSecondsInHour;
    TimeInSeconds += Time->Minute * cSecondsInMinute;
    TimeInSeconds += Time->Second;

    Temp = MathLib_Abs(Sec);
    if ((Sec < 0) && (Temp > TimeInSeconds))
    {
        //underflow detected on Date
        Time->Date = Time->Date - 1;
        TimeInSeconds = TimeInSeconds + cSecondsIn24Hours + Sec;
    }
    else
    {
        TimeInSeconds = TimeInSeconds + Sec;
        if (TimeInSeconds > (cSecondsIn24Hours - 1))
        {
            //overflow detected on Date
            Time->Date = Time->Date + 1;
            TimeInSeconds = TimeInSeconds - cSecondsIn24Hours;
        }
    }

    //convert Seconds to Rtc_TimeType
    Time->Hour = (TimeInSeconds) / cSecondsInHour;
    TimeInSeconds = TimeInSeconds % cSecondsInHour;
    Time->Minute = TimeInSeconds / cSecondsInMinute;
    TimeInSeconds = TimeInSeconds % cSecondsInMinute;
    Time->Second = TimeInSeconds;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Calculates the deviation of the clock regarding measured temperature
///
/// @arg    AdcValue : measured raw Adc Value
///
/// @return The deviation for last measurement period in 1/4 microseconds
//---------------------------------------------------------------------------------------------------------------------
static sint32 CalculateRTCDeviation(TIOAppData AdcValue, uint16 RTCCalib_K, sint16 RTCCalib_E_R_FC)
{
    //data definition
    sint32 CoeffC;
    sint32 Temp_Curr;
    uint32 CoeffA;
    uint32 CoeffB;

    Temp_Curr = MathLib_InterpolateLinearU16_S8(ADCValues, TempDegrees, cINTERPOLATION_POINTS_COUNT, AdcValue);

    //compute according the formula
    CoeffC = c25 - Temp_Prev;
    CoeffA = CoeffC * CoeffC;

    CoeffC = c25 - Temp_Curr;
    CoeffB = CoeffC * CoeffC;

    CoeffA = CoeffA + CoeffB;
    CoeffA = c24 * ((uint32)RTCCalib_K) * CoeffA;
    CoeffA = CoeffA / c5;

    CoeffC = c1200 * ((sint32)RTCCalib_E_R_FC);
    CoeffC = CoeffC + CoeffA;
    CoeffC = CoeffC >> c3;

    //store previous value for temperature
    Temp_Prev = Temp_Curr;

    return CoeffC;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Reads and CLEARS the Gpt interrupt flag
///
/// @return TRUE: IRQ was present; FALSE: IRQ was absent
//---------------------------------------------------------------------------------------------------------------------
static boolean IsRtcIrqPresent(void)
{
    boolean RetValue;

    //enter critical section
    Rte_Enter_RtcCddCompensation_ExclusiveArea();

    RetValue = RtcIrqPresent;
    RtcIrqPresent = FALSE;

    //leave critical section
    Rte_Exit_RtcCddCompensation_ExclusiveArea();

    return RetValue;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Performs one-time init activities like:
//<br>		- init local data
//<br>		- wakeup source evaluation
//---------------------------------------------------------------------------------------------------------------------
static void PerformInit(void)
{
    EcuM_WakeupSourceType WakeupEvents;
    WakeupEvents = EcuM_GetValidatedWakeupEvents();

    //in case of POR, buram data shall be initialized
    if (((WakeupEvents & (ECUM_WKSOURCE_POWER)) != (EcuM_WakeupSourceType)0) ||
        ((WakeupEvents & (ECUM_WKSOURCE_DEBUGGER)) != (EcuM_WakeupSourceType)0))
    {
        ErrCumulate = (sint32)0;
        Temp_Prev = cTEMP_PREV_DEFAULT_VALUE;
        RtcFirstCompensationDone = 0;
    }

    //in case of wakeup for calibration, compensation algorithm shall be run
    if ((WakeupEvents & (ECUM_WKSOURCE_RTC)) != (EcuM_WakeupSourceType)0)
    {
        RtcMgrCdd_TemperatureCompensation_RtcNotification();
    }

    //init internal statemachine
    CompensationState = CompensationState_Step1;
}

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Initialization function for compensation.
///
/// @return E_OK: 			Compensation strategy is successfully Initialized
///<br>     E_NOT_READY:    Compensation strategy is not ready to be Initialized
///<br>     E_NOT_OK:    	Compensation strategy can not be Initialized
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcMgrCdd_TemperatureCompensation_Init (void)
{
    if (CompensationMode == RTC_SLEEP_MODE)
    {
        CompensationMode = RTC_NORMAL_MODE;
        PerformInit();
    }

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief 	DeInitialization function for compensation
///
/// @return E_OK: 			Compensation strategy is successfully DeInitialized
///<br>     E_NOT_READY:    Compensation strategy is not ready to be DeInitialized
///<br>     E_NOT_OK:    	Compensation strategy can not be DeInitialized
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcMgrCdd_TemperatureCompensation_DeInit	(void)
{
    boolean bTemp;

    CompensationMode = RTC_SLEEP_MODE;

    bTemp = IsRtcIrqPresent();
    if (bTemp == TRUE)
    {
        EcuM_SetWakeupEvent(ECUM_WKSOURCE_RTC);
    }

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Notification function when new Time is set in RTC.
//<br>		Compensation strategy shall be reinitialized.
///
/// @return E_OK
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcMgrCdd_TemperatureCompensation_SetTime (void)
{
    TIOAppData RawAdcValue = (TIOAppData)0;
    boolean PendingCalibration = FALSE;

    //Clear accumulated error
    ErrCumulate = (sint32)0;

    //Initialize state machine
    PendingCalibration = IsRtcIrqPresent();
    if ((CompensationState > CompensationState_Step1) || (PendingCalibration == TRUE))
    {
        CompensationState = CompensationState_Step1;
        (void)EcuM_ReleaseRUN(cECUM_USER_RTC);
    }

    //Set previous temperature value
    (void)Rte_Call_TI_IoHwABGet_Impl_GetVoltage(eMUX_MCU_TEMP_BOARD, &RawAdcValue);
    Temp_Prev = MathLib_InterpolateLinearU16_S8(ADCValues, TempDegrees, cINTERPOLATION_POINTS_COUNT, RawAdcValue);

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Periodic function for Compensation strategy
//---------------------------------------------------------------------------------------------------------------------
void RtcMgrCdd_TemperatureCompensation_MainFunction (void)
{
    boolean bContinueSM = FALSE;
    boolean bTemp = FALSE;
    Std_ReturnType RetVal = E_NOT_OK;
    TIOAppData RawAdcValue = (TIOAppData)0;
    uint32 DeviationLimit;
    sint16 RTCCalib_E_R_FC;
    uint16 RTCCalib_K;

    do
    {
        bContinueSM = FALSE;
        switch (CompensationState)
        {

        case CompensationState_Step1:
            //check if Calibration timeout occured
            bTemp = IsRtcIrqPresent();
            if (bTemp == TRUE)
            {
                CompensationState = CompensationState_Step2;
                bContinueSM = TRUE;
            }
            break;

        case CompensationState_Step2:
            //check if there is at least single available ADC measurement
            RetVal = Rte_Call_TI_IoHwABGet_Impl_GetVoltage(eMUX_MCU_TEMP_BOARD, &RawAdcValue);
            
            if (RetVal == E_OK)
            {
                CompensationState = CompensationState_Step3;
                bContinueSM = TRUE;
            }
            break;

        case CompensationState_Step3:
            //calculate last measurement period's deviation
            (void)Rte_Call_TI_IoHwABGet_Impl_GetVoltage(eMUX_MCU_TEMP_BOARD, &RawAdcValue);

            //read data
            Rte_Read_RtcCdd_RtcCddReadK_Value(&RTCCalib_K);
            Rte_Read_RtcCdd_RtcCddReadE_R_FC_Value(&RTCCalib_E_R_FC);

            ErrCumulate += CalculateRTCDeviation(RawAdcValue, RTCCalib_K, RTCCalib_E_R_FC);

            if(0 == RtcFirstCompensationDone)
            {
                DeviationLimit = cRTC_DEVIATION_LIMIT;
            }
            else
            {
                if(0 > RTCCalib_E_R_FC)
                {
                    DeviationLimit = (cRTC_DEVIATION_LIMIT * c3) / c2;      //1,5s = 6000000
                }
                else
                {
                    DeviationLimit = (cRTC_DEVIATION_LIMIT / c2);           //0,5s = 2000000
                }
            }
            //check if accumulated deviation exceeds configured limit
            if (MathLib_Abs(ErrCumulate) >= DeviationLimit)
            {
                CompensationState = CompensationState_Step4;
                ApplyCorrection(DeviationLimit);
                RtcFirstCompensationDone = cRTC_FIRST_COMPENSATION_DONE;
            }
            else
            {
                CompensationState = CompensationState_Step1;
                (void)EcuM_ReleaseRUN(cECUM_USER_RTC);
            }
            break;

        case CompensationState_Step4:
            //if update is completed, let system go to sleep
            RetVal = RtcIfCdd_GetUpdateTimeConfirmation(&bTemp);
            if ((bTemp == TRUE) || (RetVal != E_OK))
            {
                CompensationState = CompensationState_Step1;
                (void)EcuM_ReleaseRUN(cECUM_USER_RTC);
            }
            break;

        default:
            DEBUG_ASSERT(FALSE);
            break;

        }
    } while (bContinueSM == TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Notification function from Gpt module on timeout expiry.
//<br>		Call happnes in interrupt context!
//---------------------------------------------------------------------------------------------------------------------
void RtcMgrCdd_TemperatureCompensation_RtcNotification (void)
{
    if (CompensationMode == RTC_NORMAL_MODE)
    {
        (void)EcuM_RequestRUN(cECUM_USER_RTC);
        RtcIrqPresent = TRUE;
    }
    else
    {
        EcuM_SetWakeupEvent(ECUM_WKSOURCE_RTC);
    }
}

#endif /* SW_COMPENSATION */
#endif

