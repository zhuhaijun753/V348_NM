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
#include "Rte_EasyNaviMdl.h"
#include "CmpLib.h"


/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(EasyNaviMdl)




//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#if 0
typedef struct {
	uint8 NavStatus;
	uint8 NavICON;
	uint32 ROUTE_REMAIN_DIS;
	uint8 ROUNG_ABOUT_NUM;
	uint8 SIGNAL_LOST;
} ST_NAVIGATION_MDL_INFO;

static ST_NAVIGATION_MDL_INFO stNavigationMdlInfo;

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================



//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static void f_GW_MP5_Navi_0x382_init(void);
static void f_GW_MP5_Navi_0x382_process(void);
static void f_GW_MP5_Navi_0x382_update(void);

static void f_GW_MP5_Navi_0x384_init(void);
static void f_GW_MP5_Navi_0x384_process(void);
static void f_GW_MP5_Navi_0x384_update(void);
#endif
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit ( void )
{
	//f_GW_MP5_Navi_0x382_init();
	//f_GW_MP5_Navi_0x384_init();
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
	//f_GW_MP5_Navi_0x382_init();
	//f_GW_MP5_Navi_0x384_init();
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
	//f_GW_MP5_Navi_0x382_init();
	//f_GW_MP5_Navi_0x384_init();
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
	//f_GW_MP5_Navi_0x382_init();
	//f_GW_MP5_Navi_0x384_init();
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
    #if 0
	eIgnState fl_IGN_state;
	EBatteryState fl_Battery_state;

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

	if (fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{
		f_GW_MP5_Navi_0x382_init();
		f_GW_MP5_Navi_0x384_init();
	}
	else
	{
		switch (fl_IGN_state)
		{
			case eIGN_RUN:
				f_GW_MP5_Navi_0x382_process();
				f_GW_MP5_Navi_0x384_process();
				break;

			case eIGN_OFF:
			default:
				f_GW_MP5_Navi_0x382_init();
				f_GW_MP5_Navi_0x384_init();
				break;
		}
	}

	f_GW_MP5_Navi_0x382_update();
	f_GW_MP5_Navi_0x384_update();
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
#if 0
static void f_GW_MP5_Navi_0x382_init(void)
{
	stNavigationMdlInfo.NavStatus = 0;
}

static void f_GW_MP5_Navi_0x382_process(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;

	uint8 	fl_MP5_NavStatus_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_MP5_0x382_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_MP5_0x382_ComIn_Tout(&fl_CAN_msg_status_U8);

	Rte_Read_rpSR_CANMSG_GW_MP5_0x382_ComIn_MP5_NavStatus(&fl_MP5_NavStatus_U8);
	
	if (fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_GW_MP5_Navi_0x382_init();
	}
	else 
	{
		stNavigationMdlInfo.NavStatus = fl_MP5_NavStatus_U8;
	}
}

static void f_GW_MP5_Navi_0x382_update(void)
{
	Rte_Write_ppSR_TIGdt_MP5_0x382_MP5_NavStatus(stNavigationMdlInfo.NavStatus);
}

static void f_GW_MP5_Navi_0x384_init(void)
{
	stNavigationMdlInfo.NavICON = 0;
	stNavigationMdlInfo.ROUTE_REMAIN_DIS = 0;
	stNavigationMdlInfo.ROUNG_ABOUT_NUM = 0;
	stNavigationMdlInfo.SIGNAL_LOST = 1;
}

static void f_GW_MP5_Navi_0x384_process(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;

	uint8 	fl_IVI_NavICON_U8 = 0;
	uint32 	fl_IVI_ROUTE_REMAIN_DIS_U32 = 0;
	uint8 	fl_IVI_ROUNG_ABOUT_NUM_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_Tout(&fl_CAN_msg_status_U8);

	Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_NavICON(&fl_IVI_NavICON_U8);
	Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUTE_REMAIN_DIS(&fl_IVI_ROUTE_REMAIN_DIS_U32);
	Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUNG_ABOUT_NUM(&fl_IVI_ROUNG_ABOUT_NUM_U8);
	
	if (fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_GW_MP5_Navi_0x384_init();
	}
	else 
	{
		stNavigationMdlInfo.NavICON = fl_IVI_NavICON_U8;
		stNavigationMdlInfo.ROUTE_REMAIN_DIS = fl_IVI_ROUTE_REMAIN_DIS_U32;
		stNavigationMdlInfo.ROUNG_ABOUT_NUM = fl_IVI_ROUNG_ABOUT_NUM_U8;
		stNavigationMdlInfo.SIGNAL_LOST = 0;
	}
}

static void f_GW_MP5_Navi_0x384_update(void)
{
	static uint8 fl_naviRoadLen = 9;
	uint16 fl_naviRoadNameBuf[12] = {33433, 33993, 36335, 21326, 20013, 26234, 35895, 0, 0, 0, 0, 0};
	
	Rte_Write_ppSR_TIGdt_MP5_0x384_IVI_NavICON(stNavigationMdlInfo.NavICON);
	Rte_Write_ppSR_TIGdt_MP5_0x384_IVI_ROUTE_REMAIN_DIS(stNavigationMdlInfo.ROUTE_REMAIN_DIS);
	Rte_Write_ppSR_TIGdt_MP5_0x384_IVI_ROUNG_ABOUT_NUM(stNavigationMdlInfo.ROUNG_ABOUT_NUM);
	Rte_Write_ppSR_TIGdt_MP5_0x384_IVI_SIGNAL_LOST(stNavigationMdlInfo.SIGNAL_LOST);

	Rte_Write_ppSR_TIGDT_MP5_0x385_NaviLaodLen(fl_naviRoadLen);
	Rte_Write_ppSR_TIGDT_MP5_0x385_NaviLaodValueBuf(fl_naviRoadNameBuf);
}
#endif
///=====================================================================================================================
//  PUBLIC
//=====================================================================================================================

