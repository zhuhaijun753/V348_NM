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

#ifndef RTCDRVCDD_C
#define RTCDRVCDD_C

#include "Std_Types.h"
#include "RtcDrvCdd.h"
#include "RtcDrvCdd_Cfg.h"
#include "Rte_RtcCdd_Type.h"
#include "s6j3200_RTC.h"
#include "Mcu.h"
#include "Mcu_Cfg.h"
//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
/* Sub Second Count = fCLKRTC/2 * 0.5s - 1 */
#if (RTC_CLOCK_SOURCE == MAIN_CLOCK)
  #if (RTC_MAIN_CLOCK_IN_HZ == 4000000)
    #define RTC_SUB_SECOND_COUNT    0x0F423F
  #elif (RTC_MAIN_CLOCK_IN_HZ == 8000000)
    #define RTC_SUB_SECOND_COUNT    0x1E847F 
  #elif (RTC_MAIN_CLOCK_IN_HZ == 16000000)
    #define RTC_SUB_SECOND_COUNT    0x3D08FF
  #else
    #error "MCU frequency not supported!"
  #endif
#elif (RTC_CLOCK_SOURCE == SUB_CLOCK)
    #define RTC_SUB_SECOND_COUNT    0x001FFF
#elif (RTC_CLOCK_SOURCE == SLOW_RC_OSC)
    #define RTC_SUB_SECOND_COUNT    0x0061A7
#else
    #error "Wrong RTC Clock Configuration!"
#endif

#define MCU_FREQ_CLK_MAIN_HZ ((uint32)MCU_McuClockSettingConfig_RUN_McuClockReferencePoint_MAIN_MCU_CLK_MAIN)	//80MHz
#define MCU_FREQ_CLK_MAIN_PLL_HZ ((uint32)MCU_McuClockSettingConfig_RUN_McuClockReferencePoint_PLL0_MCU_RUN_CLK_PLL0) //240MHz

#define RTC_RUNBIT_WAIT_TIME_IN_US  100

#define RTC_WAIT_TIME_COUNT     ((MCU_FREQ_CLK_MAIN_PLL_HZ / 1000000) * RTC_RUNBIT_WAIT_TIME_IN_US)

typedef uint8 Rtc_StatusType;

#define RTC_ARRANGE_TIME(Hr,Min,Sec)    (uint32)((((uint32)Hr)<<16) | (((uint32)Min)<<8) | ((uint32)Sec))

#define RTC_DRIVER_UNINIT     0
#define RTC_DRIVER_INIT       1

typedef enum ERtcInitState_t
{
    eRtcCheckResetReason,
    eRtcCheckRunProfileUpdate,
    eRtcStartModule,
    eRtcInitEnd
}ERtcInitState;

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================

static Rtc_StatusType  Rtc_Status = RTC_DRIVER_UNINIT;
static ERtcInitState RtcInitState = eRtcCheckResetReason;

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/// @brief  RTC Initialization operations
///
/// @return NA
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcDrvCdd_Init(void)
{
    uint32 timeout_count;
    uint8 resetReason;
    Std_ReturnType RetValue = E_NOT_READY;

    switch(RtcInitState)
    {
    case eRtcCheckResetReason:
        resetReason = Mcu_GetResetReason();
        if((MCU_WAKEUP_RESET != resetReason) && (resetReason != MCU_SW_TRIGGER_HW_RESET))
        {
           #if (RTC_MCU_CLOCK_INIT == STD_ON)
            (void)Mcu_InitClock(RTC_MCU_RUN_CONFIG);
           #endif
            RtcInitState = eRtcCheckRunProfileUpdate;
        }
        else
        {
            Rtc_Status = RTC_DRIVER_INIT;
            RetValue = E_OK;
        }
        break;

    case eRtcCheckRunProfileUpdate:
        if(MCU_PLL_LOCKED == Mcu_GetPllStatus())
        {
            RtcInitState = eRtcStartModule;
        }
        break;

    case eRtcStartModule:
        timeout_count = 0;
        /* Reset the Sub-second Counter */
        RTC_WTCR_ST = 1; 	/* Start RTC */
        while((RTC_WTSR_RUN == 0) && (timeout_count <= RTC_WAIT_TIME_COUNT))
        {
            timeout_count++;
        }

        if(timeout_count < RTC_WAIT_TIME_COUNT)
        {
            RTC_WTCR_ST = 0;	/* Stop RTC */

            RTC.unWTCR.stcField.u1CSM = 1;/*Immediate Switch*/
            while(RTC.unWTSR.stcField.u1CLK_STS!=0);/* Wait till Clock switching is done */

            RTC.unWTSR.stcField.u1CSF = 0; /* Clear Clock Switched Flag */

            /* Set the RTC Clock and Sub-second count */
            RTC_WTCR_RCKSEL = RTC_CLOCK_SOURCE;
            /* WTBR shall be updated while WTCR:ST bit is 0. */
            RTC_WTBR_WTBR = RTC_SUB_SECOND_COUNT;

           #if (RTC_CLOCK_SOURCE == SUB_CLOCK)
            /* Auto Calibration Settings */
            RTC_DURMW_DURMW = MCU_FREQ_CLK_MAIN_HZ / 4; /* Main Clock/4 :- for 0.25Sec*/
            RTC_WTCR_SCAL = 0; /* No Scaling as the Duration is 0.25s*/
            RTC_WTCR_CCKSEL = 0; /* Subclock as Clock to be calibrated*/
            RTC_WINE_CALDE = 0;/*Enable Calibration Interrupt */
            RTC_CALTRG_CALTRG = (RTC_AUTO_CALIB_TRGR_TIME_IN_SEC - 1); /* Auto Calibration happens every CALTRG+1 sec */
            RTC_WTCR_ENUP = 0;/*Disable Auto Transfer of Calibration value to Subsecond register at every calibration cycle*/
            RTC_WINE_CFDE = 0;/*Disable Calibration Failure Interrupt */
            RTC_DEBUG_DBGEN = 0;/*Debug mode enabled */

            /* changing ACAL from '0' to '1' reloads the Calibration trigger counter with the RTC_CALTRG value */
            RTC_WTCR_ACAL = 0;/*Disable Auto Calibration */
            RTC_WTCR_ACAL = 1;/*Enable Auto Calibration */
           #endif
            timeout_count = 0;
            /* Start the RTC and wait till RUN flag is set */
            RTC_WTCR_ST = 1;
            while((RTC_WTSR_RUN == 0) && (timeout_count <= RTC_WAIT_TIME_COUNT))
            {
                timeout_count++;
            }
            if(timeout_count < RTC_WAIT_TIME_COUNT)
            {
                /* Set RTC Driver Status to Initialized */
                Rtc_Status = RTC_DRIVER_INIT;
				RtcInitState = eRtcInitEnd;
                RetValue = E_OK;
            }
        }
        break;
	case eRtcInitEnd:
	{
		RtcInitState = eRtcInitEnd;
        RetValue = E_OK;
	}break;
    default:
        RtcInitState = eRtcCheckResetReason;
        break;
    }

    return RetValue;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  RTC DeInitialization operations
///
/// @return NA
//---------------------------------------------------------------------------------------------------------------------
void RtcDrvCdd_DeInit(void)
{

    uint32 timeout_count;

    timeout_count = 0;
    /* Reset the RTC and Stop */
    RTC_WTCR_ST = 1;
    while((RTC_WTSR_RUN == 0) && (timeout_count <= RTC_WAIT_TIME_COUNT))
    {
        timeout_count++;
    }
    if(timeout_count < RTC_WAIT_TIME_COUNT)
    {
        RTC_WTCR_ST = 0;
        Rtc_Status = RTC_DRIVER_UNINIT;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  RTC Update Time operations.
///
/// @return E_OK: Update Sucess; E_NOT_OK: Update Failure
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcDrvCdd_UpdateTime(Rtc_TimeType Time)
{
    Std_ReturnType RetVal = E_NOT_OK;
    if(Rtc_Status == RTC_DRIVER_INIT)
    {
        /* Write the Date in RTR1 Register */
        RTC_RTR1 = (uint32)Time.Date;

        /* Write the Time in WTR Register */
        RTC_WRT = RTC_ARRANGE_TIME(Time.Hour,Time.Minute,Time.Second);

        /* Update the Time */
        RTC_WTCR_UPDT = 1;

        /* Update the Time */
        RetVal = E_OK;
    }
    return RetVal;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  RTC Set Time operations
///
/// @return E_OK: Update Sucess; E_NOT_OK: Update Failure
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcDrvCdd_SetTime(Rtc_TimeType Time)
{
    Std_ReturnType RetVal;
    uint32 timeout_count;
    RetVal = E_NOT_OK;
    if(Rtc_Status == RTC_DRIVER_INIT)
    {
        /* Write the Date in RTR1 Register */
        RTC_RTR1 = (uint32)Time.Date;

        /* Write the Time in WTR Register */
        RTC_WRT = RTC_ARRANGE_TIME(Time.Hour,Time.Minute,Time.Second);

        RTC_WTCR_ST = 0;
        timeout_count = 0;
        while((RTC_WTSR_RUN != 0) && (timeout_count <= RTC_WAIT_TIME_COUNT))
        {
            timeout_count++;
        }
        if(timeout_count < RTC_WAIT_TIME_COUNT)
        {
            /* Update the Time and wait till it completes */
            RTC_WTCR_UPDT = 1;
            timeout_count = 0;
            while((RTC_WTCR_UPDT != 0) && (timeout_count <= RTC_WAIT_TIME_COUNT))
            {
                timeout_count++;
            }
            if(timeout_count < RTC_WAIT_TIME_COUNT)
            {
                /* changing ACAL from '0' to '1' reloads the Calibration trigger
                 * counter with the RTC_CALTRG value */
                RTC_WTCR_ACAL = 0;
                RTC_WTCR_ACAL = 1;

                RTC_WTCR_ST = 1;
                timeout_count = 0;
                while((RTC_WTSR_RUN == 0) && (timeout_count <= RTC_WAIT_TIME_COUNT))
                {
                    timeout_count++;
                }
                if(timeout_count < RTC_WAIT_TIME_COUNT)
                {
                    RetVal = E_OK;
                }
            }
        }
    }
    return RetVal;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  RTC Get Time operations
///
/// @return E_OK: Time Read Sucess; E_NOT_OK: Time Read Failure
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcDrvCdd_GetTime(Rtc_TimeType* p_Time)
{
    Std_ReturnType RetVal;
    uint32  RtcTimeVal;

    RetVal = E_NOT_OK;
    if(Rtc_Status == RTC_DRIVER_INIT)
    {
        p_Time->Date = RTC_RTR1_WTDR;
        RtcTimeVal = RTC_WRT;
        p_Time->Hour = (uint8)((RtcTimeVal & 0x001F0000)>>16);
        p_Time->Minute = (uint8)((RtcTimeVal & 0x0003F00)>>8);
        p_Time->Second = (uint8)(RtcTimeVal & 0x000003F);
        if((p_Time->Second == 0) && (p_Time->Minute == 0) && (p_Time->Hour == 0))
        {
            /* There is a chance that the date is incremented after we read the Time (WRT) register */
            /* Read the Date register once again to get the actual value */
            p_Time->Date = RTC_RTR1_WTDR;
        }
        RetVal = E_OK;
    }
    return RetVal;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Function to confirm previously applied update operation
///
/// @return E_OK: Time Read Sucess;
///			E_NOT_OK: Time Read Failure
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcDrvCdd_GetUpdateTimeConfirmation(boolean* UpdateConfirmed)
{
    Std_ReturnType RetVal = E_NOT_OK;
    if(Rtc_Status == RTC_DRIVER_INIT)
    {
    	/* Check if UPDT bit is still raised */
    	if (RTC_WTCR_UPDT == 1)
    	{
    		*UpdateConfirmed = FALSE;
    	}
    	else
    	{
    		*UpdateConfirmed = TRUE;
    	}

		/* Function handled successfully */
		RetVal = E_OK;
    }
    return RetVal;
}

#endif

