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

#ifndef RTCMGRCDD_C
#define RTCMGRCDD_C 

#include "Std_Types.h"
#include "RtcMgrCdd.h"
#include "RtcMgrCdd_Cfg.h"
#include "RtcMgrCdd_CompensationStrategy.h"
//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
#if 0
static uint8 RtcMgrCdd_HexToBcd(uint8 number);
static uint16 RtcMgrCdd_16bitHexToBcd(uint16 number);
static uint8 RtcMgrCdd_BcdToHex(uint8 number);
static uint16 RtcMgrCdd_16bitBcdToHex(uint16 number);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Hex to BCD Conversion
///
/// @return BCD Value
//---------------------------------------------------------------------------------------------------------------------
static uint8 RtcMgrCdd_HexToBcd(uint8 number)
{
    uint8 UpperNibble;
    uint8 LowerNibble;
    uint8 BcdValue;

    UpperNibble = number/10;
    LowerNibble = number%10;
    BcdValue = (UpperNibble<<4)|LowerNibble;
    return BcdValue;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  16 Bit Hex to BCD Conversion
///
/// @return BCD Value
//---------------------------------------------------------------------------------------------------------------------
static uint16 RtcMgrCdd_16bitHexToBcd(uint16 number)
{
    uint16 Nibble0;
    uint16 Nibble1;
    uint16 Nibble2;
    uint16 Nibble3;
    uint16 BcdValue;

    Nibble3 = number/1000;
    Nibble2 = (number%1000)/100;
    Nibble1 = (number%100)/10;
    Nibble0 = (number%10);
    BcdValue = (Nibble3<<12) | (Nibble2<<8) | (Nibble1<<4) | (Nibble0);
    return BcdValue;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Hex to BCD Conversion
///
/// @return Hex Value
//---------------------------------------------------------------------------------------------------------------------
static uint8 RtcMgrCdd_BcdToHex(uint8 number)
{
    uint8 UpperNibble;
    uint8 LowerNibble;
    uint8 HexValue;

    UpperNibble = ((number&0xF0)>>4)*10;
    LowerNibble = number&0x0F;
    HexValue = UpperNibble+LowerNibble;
    return HexValue;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief  16 Bit Hex to BCD Conversion
///
/// @return Hex Value
//---------------------------------------------------------------------------------------------------------------------
static uint16 RtcMgrCdd_16bitBcdToHex(uint16 number)
{
    uint16 Nibble0;
    uint16 Nibble1;
    uint16 Nibble2;
    uint16 Nibble3;
    uint16 HexValue;

    Nibble3 = ((number&0xF000)>>12)*1000;
    Nibble2 = ((number&0x0F00)>>8)*100;
    Nibble1 = ((number&0x00F0)>>4)*10;
    Nibble0 = number&0x000F;

    HexValue = Nibble3+Nibble2+Nibble1+Nibble0;
    return HexValue;
}
#endif

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcMgrCdd_Init(void)
{
    Std_ReturnType RetValue = E_NOT_OK;
    RetValue = RtcIfCdd_Init();
    return RetValue;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcMgrCdd_DeInit(void)
{
	RtcIfCdd_DeInit();
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcMgrCdd_Activate(void)
{
	Std_ReturnType RetValue = E_OK;

#if (SW_COMPENSATION==STD_ON)
	RetValue = RtcMgrCdd_CompensationStrategy_Init();
#endif //(SW_COMPENSATION==STD_ON)

	return RetValue;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcMgrCdd_DeActivate(void)
{
	Std_ReturnType RetValue = E_OK;

#if (SW_COMPENSATION==STD_ON)
	RetValue = RtcMgrCdd_CompensationStrategy_DeInit();
#endif //(SW_COMPENSATION==STD_ON)

	return RetValue;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Main periodic function
//---------------------------------------------------------------------------------------------------------------------
void RtcMgrCdd_MainFunction(void)
{
#if (SW_COMPENSATION==STD_ON)
	RtcMgrCdd_CompensationStrategy_MainFunction();
#endif //(SW_COMPENSATION==STD_ON)
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  RTC Update Time operations
///
/// @return E_OK: Update Sucess; E_NOT_OK: Update Failure
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcMgrCdd_SetTime(Rtc_TimeType Time)
{
	Std_ReturnType RetValue = E_OK;
    //Rtc_TimeType RtcMgrTime;

    //RtcMgrTime.Date = RtcMgrCdd_16bitBcdToHex(Time.Date);
    //RtcMgrTime.Hour = RtcMgrCdd_BcdToHex(Time.Hour);
    //RtcMgrTime.Minute = RtcMgrCdd_BcdToHex(Time.Minute);
    //RtcMgrTime.Second = RtcMgrCdd_BcdToHex(Time.Second);

    RetValue = RtcIfCdd_SetTime(Time);

#if (SW_COMPENSATION==STD_ON)
    if (RetValue == E_OK)
    {
    	RetValue &= RtcMgrCdd_CompensationStrategy_SetTime();
    }
#endif //(SW_COMPENSATION==STD_ON)

    return RetValue;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  RTC Get Time operations
///
/// @return E_OK: Time Read Sucess; E_NOT_OK: Time Read Failure
//---------------------------------------------------------------------------------------------------------------------
Std_ReturnType RtcMgrCdd_GetTime(Rtc_TimeType* p_Time)
{
    Std_ReturnType RetVal;

    RetVal = RtcIfCdd_GetTime(p_Time);
    if(RetVal == E_OK)
    {
        //p_Time->Date = RtcMgrCdd_16bitHexToBcd(p_Time->Date);
        //p_Time->Hour = RtcMgrCdd_HexToBcd(p_Time->Hour);
        //p_Time->Minute = RtcMgrCdd_HexToBcd(p_Time->Minute);
        //p_Time->Second = RtcMgrCdd_HexToBcd(p_Time->Second);
    }
    return RetVal;
}


#endif

