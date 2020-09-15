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
#include "Rte_HmiAdasMdl.h"
#include "CmpLib.h"


/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(HmiAdasMdl)

#define BAR1_0	(0)
#define BAR1_1	(1)
#define BAR2_2	(2)
#define BAR2_3	(3)
#define BAR3_4	(4)

#define BAR0	(0)
#define BAR1	(1)
#define BAR2	(2)
#define BAR3	(3)

#define TARGETST_MAX				(8)
#define LANE_MATRIX_MAX				(16)


typedef struct LaneMatrix_t
{
	uint8	displayFlag;
	uint8 	inputLaneMsg;
}LaneMatrix;


typedef struct DistCarDisplay_t
{
	uint8	displayFlag;
	uint8 	distanceSt;
	boolean	carSt;
}DistCarDisplay;

const LaneMatrix l_LaneTable[LANE_MATRIX_MAX] = {
	{LANE_LNONE_RNONE , 0},
	{LANE_LGRAY_RGRAY , 1},
	{LANE_LGRAY_RGRAY , 2},
	{LANE_LGREEN_RGRAY , 3},
	{LANE_LGRAY_RGREEN , 4},
	{LANE_LGREEN_RGREEN , 5},
	{LANE_LYELLOW_RGRAY , 6},
	{LANE_LGRAY_RYELLOW , 7},
	{LANE_LYELLOW_RGREEN , 8},
	{LANE_LGREEN_RYELLOW , 9},
	{LANE_LNONE_RNONE , 10},
	{LANE_LNONE_RNONE , 11},
	{LANE_LNONE_RNONE , 12},
	{LANE_LNONE_RNONE , 13},
	{LANE_LNONE_RNONE , 14},
	{LANE_LNONE_RNONE , 15},

};


const DistCarDisplay l_DisCarDsp[TARGETST_MAX] = {
    {DIST_NONE_DISPLAY_NONE  ,    BAR0 ,   TRUE},
    {DIST_RED_DISPLAY_ON     ,    BAR1 ,   TRUE},
    {DIST_YELLOW_DISPLAY_ON  ,    BAR2 ,   TRUE},
    {DIST_GREEN_DISPLAY_ON   ,    BAR3 ,   TRUE},
    {DIST_NONE_DISPLAY_NONE  ,    BAR0 ,   FALSE},
    {DIST_RED_DISPLAY_OFF    ,    BAR1 ,   FALSE},
    {DIST_YELLOW_DISPLAY_OFF ,    BAR2 ,   FALSE},
    {DIST_GREEN_DISPLAY_OFF  ,    BAR3 ,   FALSE}
}; 



//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static uint8 l_TargetSt_U8;
static uint8 l_LaneSt_U8;
static uint8 l_TgtPosition_U8;



//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static void f_HmiAdasTargetSt(void);
static void f_HmiAdasInit(void);
static void f_HmiAdasLaneSt(void);
static void f_HmiTargetStInit(void);
static void f_HmiLaneStInit(void);


//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit ( void )
{
	f_HmiAdasInit();
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
	f_HmiAdasInit();
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
    boolean fl_VehicleCfg;		
	Rte_Read_rpSR_GdtCtrl_AEB_FCW_ACC_Cfg(&fl_VehicleCfg);
	if(fl_VehicleCfg == FALSE)
	{
		return E_OK;
	}

	f_HmiAdasTargetSt();
	f_HmiAdasLaneSt();

	
	Rte_Write_ppCS_HmiAdas_TargetSt(l_TargetSt_U8);
	Rte_Write_ppCS_HmiAdas_LaneSt(l_LaneSt_U8);
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

static void f_HmiTargetStInit(void)
{
	l_TargetSt_U8 = 0;
	l_TgtPosition_U8 = BAR1;
}

static void f_HmiLaneStInit(void)
{
	l_LaneSt_U8 = 0;
	l_TgtPosition_U8 = BAR1;
}


static void f_HmiAdasInit(void)
{
	l_TargetSt_U8 = 0;
	l_LaneSt_U8 = 0;
	l_TgtPosition_U8 = BAR1;
}



static void f_HmiAdasTargetSt(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	uint8 	fl_Signal_TgtDisp_U8 = 0;
	uint8 	fl_Signal_TgtPosition_U8 = 0;
	uint8   i = 0;

	Rte_Read_rpSR_CANMSG_GW_PSA2_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_PSA2_ComIn_Tout(&fl_CAN_msg_status_U8);
	Rte_Read_rpSR_CANMSG_GW_PSA2_ComIn_RADA_ACC_HMI_TgtDisp_Flag(&fl_Signal_TgtDisp_U8);
	Rte_Read_rpSR_CANMSG_GW_PSA2_ComIn_RADA_TARGET_POSITION(&fl_Signal_TgtPosition_U8);

	if(fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_HmiTargetStInit();
		fl_Signal_TgtDisp_U8 = 0;
	}
	else 
	{
		//target position process
		if((fl_Signal_TgtPosition_U8 == BAR1_0) || (fl_Signal_TgtPosition_U8 == BAR1_1))
		{
			l_TgtPosition_U8 = BAR1;
		}
		else if((fl_Signal_TgtPosition_U8 == BAR2_2) || (fl_Signal_TgtPosition_U8 == BAR2_3))
		{
			l_TgtPosition_U8 = BAR2;
		}
		else if(fl_Signal_TgtPosition_U8 == BAR3_4)
		{
			l_TgtPosition_U8 = BAR3;
		}
		else 
		{
			l_TgtPosition_U8 = BAR0;
		}		
	}

	for(i = 0; i < TARGETST_MAX; i++)
	{
		if((l_TgtPosition_U8 == l_DisCarDsp[i].distanceSt) && (fl_Signal_TgtDisp_U8 == l_DisCarDsp[i].carSt))
		{
			l_TargetSt_U8 = l_DisCarDsp[i].displayFlag;
			break;
		}
	}	
}

static void f_HmiAdasLaneSt(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;
	uint8 	fl_Signal_Lane_U8 = 0;
	uint8   i = 0;

	Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_Tout(&fl_CAN_msg_status_U8);
	Rte_Read_rpSR_CANMSG_GW_Camera_ComIn_Vehicle_Lane_Display(&fl_Signal_Lane_U8);

	if(fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_HmiLaneStInit();
	}
	else 
	{
		for(i = 0; i < LANE_MATRIX_MAX; i++)
		{
			if(fl_Signal_Lane_U8 == l_LaneTable[i].inputLaneMsg)
			{
				l_LaneSt_U8 = l_LaneTable[i].displayFlag;
				break;
			}
		}
	}
}



///=====================================================================================================================
//  PUBLIC
//=====================================================================================================================

/*----------------------------------------------------------------------------
Date              : 19-July-2017
CDSID             : kvakeshk
Traceability      : RTC ID 
Change Description: Speed Out Intial Version
-----------------------------------------------------------------------------*/

