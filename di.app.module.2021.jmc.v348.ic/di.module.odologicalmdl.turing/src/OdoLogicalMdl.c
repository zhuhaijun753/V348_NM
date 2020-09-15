/*---------------------------------------------------------------------------------------------------------------------
**
** VISTEON CORPORATION CONFIDENTIAL
** 
**
** [2015] Visteon Corporation
** All Rights Reserved.
**
** NOTICE: This is an unpublished work of authorship, which contains trade secrets.
** Visteon Corporation owns all rights to this work and intends to maintain it in confidence to preserve
** its trade secret status. Visteon Corporation reserves the right, under the copyright laws of the United States
** or those of any other country that may have jurisdiction, to protect this work as an unpublished work,
** in the event of an inadvertent or deliberate unauthorized publication. Visteon Corporation also reserves its rights
** under all copyright laws to protect this work as a published work, when appropriate.
** Those having access to this work may not copy it, use it, modify it, or disclose the information contained in it
** without the written authorization of Visteon Corporation.
**
**--------------------------------------------------------------------------------------------------------------------*/

#include "Rte_OdoMdl.h"
#include "CmpLib.h"
#include <TmExt_Cfg.h>
//#include <TmExt_Timer.h>
#include "dem.h"
#include "Mcu.h"
#include "Rte.h"
//#include "OdoMdl.h"

/*@brief  Instance of life-cycle state-machine*/

CMPLIB_INSTANCE(OdoLogicalMdl)
/*=====================================================================================================================
**  CONSTANTS & TYPES
**====================================================================================================================*/
/*****************************************************************************
** NAME 	  : DISTANCEFORODORESETVALUE
** DESCRIPTION: DISTANCE FOR ODO RESET VALUE
**				Requirement
**				4.8.2.4.1.3.3.1  odometer shall not be cleared to 0,
							if Odometer is already more than 255km.
** UNIT 	  : NA 
** RESOLUTION : NA (Software value)
** DATA TYPE  :
** REQUIRED   : YES
*****************************************************************************/
#define DISTANCEFORODORESETVALUE 	((uint16)25500)

/*****************************************************************************
** NAME 	  : TRIPA_RESET_TIME
** DESCRIPTION: DIST_INPUT_MAX
**				Requirement
**				4.8.2.4.1.2.3.8.6 Odometer Constant Block Storage Parameters
**				
** UNIT 	  : NA
** RESOLUTION : NA (Software value)
** DATA TYPE  :
** REQUIRED   : YES
*****************************************************************************/
#define MAX_DISPLAY_ODO_VALUE	99999900  /*SysRS_03_HMIFunction-625 Lhbb Here is something err*/
#define MAX_DISPLAY_ODO_MileVALUE	62099938  /*SysRS_03_HMIFunction-625 Lhbb Here is something err*/

/*****************************************************************************
** NAME 	  : MIN_VALID_VEHICLE_SPEED
** DESCRIPTION: DIST_INPUT_MIN
**				Requirement
**				4.8.2.4.1.2.3.8.6 Odometer Constant Block Storage Parameters
**				
** UNIT 	  : NA
** RESOLUTION : NA (Software value)
** DATA TYPE  :
** REQUIRED   : YES
*****************************************************************************/
#define MIN_VALID_VEHICLE_SPEED ((uint16)200)  /*2km.h*/


#define MAX_VALID_VEHICLE_SPEED ((uint16)30000) /*300km.h*/


#define DISTANCE_10KM_VALUE	1000

#define BACKUP_CHECK_ODO_VALUE 255

#define NVM_ODO_BLOCK_NUMBER 5

#define TRIP_NUMBER			2

#define VEHICLE_SPEED_INVALID  (1)

#define INTERVAL_MESSAGE (10)

#define TIMEOUT_1S  1000

#define NVM_DistanceConversionNumerator (1)  /*Numerator*/
#define NVM_DistanceConversionDenominator (4500) /*Denominator*/

#define ODO_DEBUG 0 /*Lhbb20180601*/

#define ODO_UNIT_DIVIDE 100 /*Lhbb20180605 ODO_UNIT_DIVIDE==cCURRENT_ODO_UNITS_SCALING*/

/*
** This macro defines CAN message input status
*/
#define FODO_CAN_SIGNAL_INVALID (RTE_E_TIMEOUT | RTE_E_NEVER_RECEIVED)

/*
** This macro defines wheel mile input status
*/
#define FODO_WHEEL_NONE         ((uint8)0x00)
#define FODO_WHEEL_LEFT         ((uint8)0x01)
#define FODO_WHEEL_RIGHT        ((uint8)0x02)
#define FODO_WHEEL_BOTH         ((uint8)0x03)

/*
** This macro defines transition uints
*/
#define FODO_TRANS_SCALE1       ((uint32)100)
#define FODO_TRANS_SCALE2       ((uint32)10)
#define FODO_TRANS_SCALE3       ((uint32)1)
#define FODO_TRANS_SCALE4       ((uint32)10)
#define FODO_SCALE_COUNT        ((uint8)4)

#define FODO_OK                 ((uint8)1)
#define FODO_NOK                ((uint8)0)

/*
** This macro defines Max and Error value for the odo
*/
#define FODO_MAIN_MAX_VALUE     ((uint32)99999999) /*unit: 10m*/
#define FODO_ODO_NVM_ERROR      (0xFFFFFFFFul)

/*
** This macro defines overflow calculation
*/
#define FODO_OVERFLOW_LIMIT     ((uint16)60)  /* 151 -> (151/3 = 50) */
#define FODO_OVERFLOW_BASE      ((uint16)65535)

#define ODO_RESET_MAX_DISTANCE    ((uint8)100)   
#define NVM_ODO_RESET_COUNTER_U8    ((uint8)2)
#define ODO_CALC_FACTOR_VALUE             (uint32)46118			//modify by sli34 2020/07/09
#define ODO_LOOP_NUM_FOR_1S           		(uint8)50
#define ODO_LOOP_NUM_FOR_100MS          	(uint8)5


/*
** This macro defines valid/invalid values
*/
#define FODO_WHEEL_VALID        ((uint8)0x00)

typedef enum eODO_UINT
{
    ODO_1000M = 0,
    ODO_100M  = 1,
    ODO_10M   = 2,
    ODO_1M    = 3
} tODO_UINT;

typedef struct sWheel_Info
{
    uint8   (*fl_get_missing)(void);
    uint8   (*fl_get_status)(void);
    uint16  (*fl_get_value)(void);
    boolean fl_inited_BOOL;
    uint16  fl_value_U16;
    uint32  fl_overflow_U32;

} tWheel_Info;

typedef struct sODO_Collector
{
    tWheel_Info fl_left_wheel_S;
    tWheel_Info fl_right_wheel_S;
    uint8       fl_strategy_U8;
    uint32      fl_odo_main_counts_U32;
    uint8       fl_odo_increase_counts_U8;
} tODO_Collector;

extern boolean GetCurrentOdo(uint32 *pCurrentOdoValue , boolean CurrentOdoIsMetric);
extern void OdoResetMain(void);
uint8 can_get_WhlMilgFrntLeStatus_status(void);
uint8 can_get_WhlMilgFrntLeStatus_value(void);
uint16 can_get_WhlMilgFrntLe_value(void);
uint8 can_get_WhlMilgFrntRiStatus_status(void);
uint8 can_get_WhlMilgFrntRiStatus_value(void);
uint16 can_get_WhlMilgFrntRi_value(void);

#define WHEEL_LEFT_INFO\
    can_get_WhlMilgFrntLeStatus_status,\
    can_get_WhlMilgFrntLeStatus_value,\
    can_get_WhlMilgFrntLe_value,\
    FALSE,\
    0,\
    0\

#define WHEEL_RIGHT_INFO\
    can_get_WhlMilgFrntRiStatus_status,\
    can_get_WhlMilgFrntRiStatus_value,\
    can_get_WhlMilgFrntRi_value,\
    FALSE,\
    0,\
    0\

#define ODO_COLLECTOR_INFO\
    WHEEL_LEFT_INFO,\
    WHEEL_RIGHT_INFO,\
    FODO_WHEEL_NONE,\
    0,\
    0\

/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, and resolution                            *
******************************************************************************/
/*============================================================================
** M E M O R Y   A L L O C A T I O N
**==========================================================================*/
/*
purpose         : ODO collector which contain all odo related information
critical section: None
unit            : None
resolution      : 1
valid Range     : 0 to 9999 in 0.1km
Validity check  : None
*/
static tODO_Collector l_odo_collector_S = {ODO_COLLECTOR_INFO};

/*
purpose         : Used to get scale for different units
critical section: None
unit            : None
resolution      : 1
valid Range     : 0 to FTC_AFC_DISPLAY_UPDATE_COUNT
Validity check  : None
*/
static const uint32 l_odo_scales_table[FODO_SCALE_COUNT] = {FODO_TRANS_SCALE1, FODO_TRANS_SCALE2, FODO_TRANS_SCALE3, FODO_TRANS_SCALE4};

/*odo count for ifc calculate*/
static uint32 l_odo_count_for_ifc = 0;
/*loop number for ifc calculate*/
static uint8 l_odo_loop_num_for_ifc = 0;
/*odo value in 2s for ifc calculate*/
static uint32 l_odo_value_for_ifc = 0;

/*****************************************************************************
** NAME 	  : TRIPA_RESET_TIME
** DESCRIPTION: TC A reset time
**				Requirement
**				4.8.2.4.2.2.3.7 Auto Rset at 2h after IGN OFF.
**				
** UNIT 	  : 7200 =2h 
** RESOLUTION : NA (Software value)
** DATA TYPE  :
** REQUIRED   : YES
*****************************************************************************/
#define TRIPA_RESET_TIME	7200    /*2h*/
/*=====================================================================================================================
** I N T E R N A L   F U N C T I O N   P R O T O T Y P E S
**====================================================================================================================*/
static void OdoLogicalMdl_ComIn_Process(void);
static void OdoLogicalMdl_Set_Display_Value(void);
static void OdoLogicalMdl_Odo_Write_Data_Process(void);
void OdoLogicalMdl_common_init(void);
uint8 OdoLogicalMdl_check_left_wheel_status(void);
uint8 OdoLogicalMdl_check_right_wheel_status(void);
uint8 OdoLogicalMdl_calc_left_wheel_info(void);
uint8 OdoLogicalMdl_calc_right_wheel_info(void);
void OdoLogicalMdl_calc_odo_with_both_wheel(void);
void OdoLogicalMdl_calc_odo_with_left_wheel(void);
void OdoLogicalMdl_calc_odo_with_right_wheel(void);
void COdoMdl_Manual_Reset(void);
boolean OdoLogicalMdl_Is_Calculate_ODO(void);
void fdiag_app_reset_odo(void);
void OdoLogicalMdl_ifc_add_count(uint8 odo_count);
static void Clear_Odo_Entry_Condition(void);


/*=====================================================================================================================
** M E M O R Y   A L L O C A T I O N
**====================================================================================================================*/

//static boolean l_is_speed_invalid_BOOL = FALSE;
//static boolean l_odo_speed_timer_expired_BOOL;
boolean odo_Reset_flag = FALSE;
static eIgnState eCurrentIgnState_status = 0; 
static eIgnState ePrevIgnitionState = 0;     //to store the prev Ignition State
static boolean OK_Button_Pressed = 0;
static boolean l_is_ign_transition = 0;

//extern boolean dem_batt_in_mormal_5s;
//extern boolean dem_IGN_ON_5s;

/*
purpose         : Remainder odometer count storage 
critical section: None
unit            : None
resolution      : 1
valid Range     : 0 to 0x7FFFFFFF
Init            : Set to 0 at Cold Init,warm init,wakeup
*/
//static uint32 l_vehicle_speed_remainder_U32;
//static uint16 l_vehicle_speed_valid_prev_U16;
//static uint8 l_distance_counts_U8;
extern uint32  TripA_odo_wheel_increase_value;/*2019GU*/
//extern uint32  TripB_odo_wheel_increase_value;/*2019GU*/
extern uint32  average_speed_wheel_increase_value;/*2019GU*/

/*****************************************************************************
*                                 Backup Variable Declarations                *
******************************************************************************/

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional initialization state
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpInit( void )
{
    NV_Data_ODO_Reset_Counter fl_odo_reset_cnt;
    
	Rte_Write_OdoMdl_ppSR_TIOdoMdl_ComOut_NewRollingOdo(Rte_InitValue_ppSR_TIOdoMdl_ComOut_NewRollingOdo);

    /* Initial value of nvm variables,this is the solution temporarily */
	Rte_Read_pprpNV_OdoMdl_ODO_Reset_Counter_ResetCounterValue(&fl_odo_reset_cnt);
	if((fl_odo_reset_cnt.ResetCounterValue==0)||(fl_odo_reset_cnt.ResetCounterValue==0xFF))/* NEW E2*/
	{
		fl_odo_reset_cnt.ResetCounterValue = 4; /* E2 fault value is 0,so this write 4*/
		Rte_Write_pprpNV_OdoMdl_ODO_Reset_Counter_ResetCounterValue(&fl_odo_reset_cnt);
	}
    
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional de-initialization state
** 
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDeInit( void )
{
    return E_OK;
}
#if 0
/*if E2_ODO is false, write a init value 0*/
static Std_ReturnType SetOdoNVMInit(NVOdoDataSetType * odo_data_arry) /*Lhbb set a initialization 0*/
{
	NVOdoDataSetType odo_data_arry_ini[5]={{0x0006U, 0xFFU, (uint8)0x80U},
                                 		   {0x0006U, 0xFFU, (uint8)0x80U},
                                 		   {0x0006U, 0xFFU, (uint8)0x80U},
                                 		   {0x0006U, 0xFFU, (uint8)0x80U},
                                 		   {0x0006U, 0xFFU, (uint8)0x80U}};/*Lhbb can't write other value,only can write 0,is a bug*/
	if ( ((odo_data_arry[0]->count != 0xFFFF) && (odo_data_arry[0]->increments > 0 ))
		  && ((odo_data_arry[1]->count != 0xFFFF) && (odo_data_arry[1]->increments > 0))
		  && ((odo_data_arry[2]->count != 0xFFFF) && (odo_data_arry[2]->increments > 0))
		  && ((odo_data_arry[3]->count != 0xFFFF) && (odo_data_arry[3]->increments > 0))
		  && ((odo_data_arry[4]->count != 0xFFFF) && (odo_data_arry[4]->increments > 0))) 
    {
      return E_OK; /*if E2_odo ok,return */
	}
	else
	{
	 Rte_Write_NV_OdoMdl_NvMOdoData_OdoDataSetArray(odo_data_arry_ini[0]);
	}	

	return E_OK;
}

#endif
/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional activation state
** 
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpActivation( void )
{
	//NV_Data_Cluster_PPK fl_ppk_value;
	NVOdoDataSetType nv_odo_data_arry[5];

	eCurrentIgnState_status = 0; 
	ePrevIgnitionState = 0;	 //to store the prev Ignition State
	OK_Button_Pressed = 0;
	l_is_ign_transition = FALSE;
	(void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(nv_odo_data_arry[0]);
	(void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(nv_odo_data_arry[1]);
	(void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(nv_odo_data_arry[2]);
	(void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(nv_odo_data_arry[3]);
	(void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(nv_odo_data_arry[4]);

	(void)Rte_Write_NV_OdoMdl_NvMOdoData_OdoDataSetArray(nv_odo_data_arry[0]);
	
    #if 0 /*if E2_Odo error,Initialize it*/  //wli32 modified
	SetOdoNVMInit(nv_odo_data_arry);
	#endif 
		
	Rte_Write_ppSR_OdoOutKiloMeters_Display_Value_KiloMeters(0x7FFFFFFE);
    /* wli32 memo: add IC_0x510: IC_OdometerMasterValue output */
    
    /* code restart init & warm restart init */
    OdoLogicalMdl_common_init();
    
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional de-activation state
** 
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDeActivation( void )
{
	//Rte_Write_ppSR_OdoOutKiloMeters_Display_Value_KiloMeters(0x7FFFFFFE);
    
    return  E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  State for normal operations
** 
** @return The return value is disregarded. Say E_OK
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpActive( void )
{
	eIgnState fl_ign;
    EBatteryState fl_batt;
    static uint8 fl_ign_sts;

	Rte_Read_rpBattState_BatteryState(&fl_batt);
    if ((fl_batt != eBatteryState_OverVoltage) && (fl_batt != eBatteryState_UnderVoltage)) /*overPh1,normal,underPh1*/
    {
		Rte_Read_rpIgnState_IGNState(&fl_ign);// ModMdl.c eKeyPos_Run/eKeyPos_Off/eKeyPos_Count
		/*fl_ign = 2;*/
		if(fl_ign == eIGN_RUN)
		{
            if(eIGN_OFF == fl_ign_sts)
            {
                OdoLogicalMdl_common_init();
                fl_ign_sts = eIGN_RUN;
            }
			OdoLogicalMdl_ComIn_Process();
		}
		else
		{
            fl_ign_sts = eIGN_OFF;            
	      //nothing,IGN_OFF not count
		}
		OdoLogicalMdl_Odo_Write_Data_Process();
		OdoLogicalMdl_Set_Display_Value();
		//Clear_Odo_Entry_Condition();

    }
	else
	{
	  //nothing,batt high/low not count 
	}

	return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Diagnostic state
** 
** @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
**         E_NOT_READY:    Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDiag( void )
{
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
** 
** @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
**         E_NOT_READY:    Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :OdoLogicalMdl_ComIn_Process
** @Function Description :Requirement 4.8.2.4.1.2.3.8.2
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
static void OdoLogicalMdl_ComIn_Process(void)
{
//	uint16 fl_vehicle_speed_u16;
//	uint8 fl_speed_invalid_u8;
//	uint8 fl_speed_missing_u8;
//	uint8 fl_speed_nv_recd_u8;
//	uint32 fl_intermediate_value_u32;
//	uint8  fl_distance_counts_U8 = 0;
//	boolean fl_TimerExpSts;

#if 0 //ODO_DEBUG
    fl_speed_nv_recd_u8 = 0;
    fl_speed_missing_u8 = 0;
	fl_speed_invalid_u8 = 0;
	fl_vehicle_speed_u16 = 18000;

	//CAN msg 0x0A0
	Rte_Read_rpSR_CANMSG_GW_ESC2_ComIn_NR(&fl_speed_nv_recd_u8);
	Rte_Read_rpSR_CANMSG_GW_ESC2_ComIn_Tout(&fl_speed_missing_u8);

	Rte_Read_rpSR_CANMSG_GW_ESC2_ComIn_VehSpdValidFlag(&fl_speed_invalid_u8); /*Lhbb fl_speed_invalid_u8==1 is invalid*/
	Rte_Read_rpSR_CANMSG_GW_ESC2_ComIn_ESC_VehSpd(&fl_vehicle_speed_u16);
#endif
	
	/**********Odometer Accumulation and Storage Operation******************/
#if 1  //wli32

    /*check left wheel status*/
    if (FODO_OK == OdoLogicalMdl_check_left_wheel_status())
    {
        /*calculate left wheel value and overflow*/
        if (FODO_OK == OdoLogicalMdl_calc_left_wheel_info())
        {
            /*calculate result ok, update calculate strategy*/
            l_odo_collector_S.fl_strategy_U8 |= FODO_WHEEL_LEFT;
        }
        else
        {
            /*calculate result nok, remove calculate strategy*/
            l_odo_collector_S.fl_strategy_U8 &= ~FODO_WHEEL_LEFT;
        }
    }
    else
    {
        /*calculate result nok, remove calculate strategy*/
        l_odo_collector_S.fl_strategy_U8 &= ~FODO_WHEEL_LEFT;
    }

    /*check right wheel status*/
    if (FODO_OK == OdoLogicalMdl_check_right_wheel_status())
    {
        /*calculate left wheel value and overflow*/
        if (FODO_OK == OdoLogicalMdl_calc_right_wheel_info())
        {
            /*calculate result ok, update calculate strategy*/
            l_odo_collector_S.fl_strategy_U8 |= FODO_WHEEL_RIGHT;
        }
        else
        {
            /*calculate result nok, remove calculate strategy*/
            l_odo_collector_S.fl_strategy_U8 &= ~FODO_WHEEL_RIGHT;
        }
    }
    else
    {
        /*calculate result nok, remove calculate strategy*/
        l_odo_collector_S.fl_strategy_U8 &= ~FODO_WHEEL_RIGHT;
    }

    /*calculate odo with evaluated strategy*/
    if (FODO_WHEEL_BOTH == l_odo_collector_S.fl_strategy_U8)
    {
        /*calculate odo with both left and right wheel info*/
        OdoLogicalMdl_calc_odo_with_both_wheel();
    }
    else if (FODO_WHEEL_LEFT == l_odo_collector_S.fl_strategy_U8)
    {
        /*calculate odo with left wheel info only*/
        OdoLogicalMdl_calc_odo_with_left_wheel();
    }
    else if (FODO_WHEEL_RIGHT == l_odo_collector_S.fl_strategy_U8)
    {
        /*calculate odo with right wheel info only*/
        OdoLogicalMdl_calc_odo_with_right_wheel();
    }
    else
    {
        /*invalid strategy, nothing to do*/
    }

    /* RTE get confirm button long press 5 seconds status from GdtCtrl module */
    //OdoLogicalMdl_manual_reset();
#else
	if (fl_speed_nv_recd_u8 == RTE_E_NEVER_RECEIVED) 
	{
		fl_vehicle_speed_u16 = 0;
		l_vehicle_speed_valid_prev_U16 = 0;
		fl_intermediate_value_u32 = 0;
	}
    else if ((fl_vehicle_speed_u16 >= MIN_VALID_VEHICLE_SPEED) && (fl_vehicle_speed_u16 <= MAX_VALID_VEHICLE_SPEED)
		      &&(fl_speed_missing_u8 != RTE_E_TIMEOUT) && (fl_speed_invalid_u8 != VEHICLE_SPEED_INVALID)) /*normal*/
    {
		if (l_is_speed_invalid_BOOL == TRUE)
		{
			TmExt_Timer_Stop(eTimerOdologicalSpdInvalid);
			l_is_speed_invalid_BOOL = FALSE;
		}
		l_vehicle_speed_valid_prev_U16 = fl_vehicle_speed_u16;
		l_odo_speed_timer_expired_BOOL = FALSE;

		/* USING_BASE_ODO : Calculate the odo */
		fl_intermediate_value_u32		= (fl_vehicle_speed_u16 * NVM_DistanceConversionNumerator) + l_vehicle_speed_remainder_U32;  /*NVM_DistanceConversionNumerator=1*/
		fl_distance_counts_U8			= (uint8)(fl_intermediate_value_u32 / NVM_DistanceConversionDenominator);     /*NVM_DistanceConversionDenominator=4500*/
		l_vehicle_speed_remainder_U32	= (fl_intermediate_value_u32 - ((uint32)fl_distance_counts_U8 * NVM_DistanceConversionDenominator));
		l_distance_counts_U8			= l_distance_counts_U8 + fl_distance_counts_U8;
    }
	else /*Speed not in[2,300], include missing or invalid*/
	{
	    if (l_is_speed_invalid_BOOL == TRUE)
	    {
	    	TmExt_Timer_IsElapsed(eTimerOdologicalSpdInvalid,TIMEOUT_1S,&fl_TimerExpSts);
			if (fl_TimerExpSts ==  TRUE)
			{
			  fl_intermediate_value_u32 = 0;
			  l_odo_speed_timer_expired_BOOL = TRUE;
			}
			else
			{
	            if (l_odo_speed_timer_expired_BOOL == FALSE)
                {
                  fl_vehicle_speed_u16 = l_vehicle_speed_valid_prev_U16;
				   /* USING_BASE_ODO : Calculate the odo */
				  fl_intermediate_value_u32		= (fl_vehicle_speed_u16 * NVM_DistanceConversionNumerator) + l_vehicle_speed_remainder_U32;  /*NVM_DistanceConversionNumerator=1*/
				  fl_distance_counts_U8			= (uint8)(fl_intermediate_value_u32 / NVM_DistanceConversionDenominator);     /*NVM_DistanceConversionDenominator=4500*/
				  l_vehicle_speed_remainder_U32	= (fl_intermediate_value_u32 - ((uint32)fl_distance_counts_U8 * NVM_DistanceConversionDenominator));
				  l_distance_counts_U8			= l_distance_counts_U8 + fl_distance_counts_U8;
	            }
			}
		}
		else
		{
		   if((fl_speed_missing_u8 == RTE_E_TIMEOUT) || (fl_speed_invalid_u8 == VEHICLE_SPEED_INVALID))
		   	{
		   	  //start_Timer
		      TmExt_Timer_Start(eTimerOdologicalSpdInvalid);
		      l_is_speed_invalid_BOOL = TRUE;
		      l_odo_speed_timer_expired_BOOL = FALSE;
			  
		      fl_vehicle_speed_u16 = l_vehicle_speed_valid_prev_U16;
			   /* USING_BASE_ODO : Calculate the odo */
			  fl_intermediate_value_u32 	= (fl_vehicle_speed_u16 * NVM_DistanceConversionNumerator) + l_vehicle_speed_remainder_U32;  /*NVM_DistanceConversionNumerator=1*/
			  fl_distance_counts_U8 		= (uint8)(fl_intermediate_value_u32 / NVM_DistanceConversionDenominator);	  /*NVM_DistanceConversionDenominator=4500*/
			  l_vehicle_speed_remainder_U32 = (fl_intermediate_value_u32 - ((uint32)fl_distance_counts_U8 * NVM_DistanceConversionDenominator));
			  l_distance_counts_U8			= l_distance_counts_U8 + fl_distance_counts_U8;
		   	}
		}
	}
    
	(void)Rte_Write_ppSR_TIOdoMdl_ComOut_NewRollingOdo(l_distance_counts_U8);
#endif  //wli32    

}

boolean OdoLogicalMdl_Is_Calculate_ODO(void)
{
    if((FODO_WHEEL_BOTH == l_odo_collector_S.fl_strategy_U8)
        || (FODO_WHEEL_RIGHT == l_odo_collector_S.fl_strategy_U8)
        || (FODO_WHEEL_LEFT == l_odo_collector_S.fl_strategy_U8))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if 0
/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :OdoLogicalMdl_set_odometer_reset_counter
** @Function Description :during software reprogramming, odometer shall not be cleared to 0, 
**							if Odometer is already more than 255km
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
static void OdoLogicalMdl_set_odometer_reset_counter(uint8 reset_counter_value)
{
	uint32 fl_get_odo_value_kilometer_u32;
	uint32 fl_get_nv_odo_value_kilometer_u32;
	uint32 fl_get_odo_value_mile_u32;
	NV_Data_Cluster_Reset_OffSet fl_odo_reset_offset;
	EBtnState Menu_Btn_Status;
	OdoMdl_RW_Interface_Type odo_rw_interface_value;
	NV_Data_Cluster_Variable_Block fl_cluster_variable_block;
	

	
	//Get KiloMeters from OdoMdl
	Rte_Read_tiSR_OdoInKiloMeters_KiloMeters(&fl_get_nv_odo_value_kilometer_u32);
	//Get Miles from OdoMdl
	Rte_Read_tiSR_OdoInMiles_Miles(&fl_get_odo_value_mile_u32);
	//Get offset value from NVM
	(void)Rte_Read_NV_OdoMdl_Reset_OffSet_Offset_Value(&fl_odo_reset_offset);

	(void)Rte_Read_NV_Cluster_Variable_Block_Cluster_Variable_Block(&fl_cluster_variable_block);
	//Get menu button state
	Rte_Read_OdoMdl_rpSR_rOdoMdl_BTN_MENU_TIBtnMdl_State_State_State(&Menu_Btn_Status);

	if(fl_cluster_variable_block.ODO_Backup_Sign == 0)
	{
		fl_get_odo_value_kilometer_u32 = fl_get_nv_odo_value_kilometer_u32 + fl_cluster_variable_block.ODO_Backup_Kilometer_Offset;
	}
	else
	{
		fl_get_odo_value_kilometer_u32 = fl_get_nv_odo_value_kilometer_u32 - fl_cluster_variable_block.ODO_Backup_Kilometer_Offset;
	}
	/*ODO < 255Km, long press Menu for 30s can reset TC one times*/
	if((fl_odo_reset_offset.Reset_Counter == 0) && (fl_get_odo_value_kilometer_u32 <= DISTANCEFORODORESETVALUE))
	{
		if(Menu_Btn_Status != eEBtnState_NotPressed)
		{
			if(l_menu_btn_timer_s == FALSE)
			{
				l_menu_btn_timer_s = TRUE;//Menu key down
				//TmExt_Timer_Start(eCntMenuBtn30sTimer);/*Lhbb20180531*/
			}
			else
			{
			#if 0
				if(TmExt_Timer_IsElapsed(eCntMenuBtn30sTimer,30000) != FALSE)
			#else
			    if(0) /*Lhbb20180531*/
			#endif
				{
					fl_odo_reset_offset.Reset_Counter++;//record the reset counts,but only reset one times 
					//Reset SIA
					Rte_Write_ppSR_TIOdo_SIA_ResetSIA(TRUE);
					// the next two interface not use now
						Rte_Write_ppSR_TITripMdl_TripSR_rpTrip_OdoA_Reset_TripA_Reset(TRUE);
						Rte_Write_ppSR_TITripMdl_TripSR_rpTrip_OdoB_Reset_TripB_Reset(TRUE);
					//Reset Trip ODO
					(void)Rte_Read_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface(&odo_rw_interface_value);
					odo_rw_interface_value.OdoMdl_Trip_Reset_Status[0] = TRUE;
					odo_rw_interface_value.OdoMdl_Trip_Reset_Status[1] = TRUE;
					(void)Rte_Write_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface(&odo_rw_interface_value);
					//Reset AFE
					Rte_Write_ppSR_TITripMdl_TripSR_AFEResetA_AFE_ResetA(TRUE);
					Rte_Write_ppSR_TITripMdl_TripSR_AFEResetB_AFE_ResetB(TRUE);
					if(fl_get_odo_value_kilometer_u32 >= (reset_counter_value * 100))
					{
						fl_odo_reset_offset.Reset_Kilometer_Offset = reset_counter_value * 100;
						fl_odo_reset_offset.Reset_Mile_Offset = reset_counter_value * 621 / 10;
					}
					else
					{
						fl_odo_reset_offset.Reset_Kilometer_Offset = fl_get_odo_value_kilometer_u32;
						fl_odo_reset_offset.Reset_Mile_Offset = fl_get_odo_value_mile_u32;
					}

					fl_odo_reset_offset.Reset_Kilometer_Offset = fl_odo_reset_offset.Reset_Kilometer_Offset / 100;
					fl_odo_reset_offset.Reset_Mile_Offset = fl_odo_reset_offset.Reset_Mile_Offset / 100;

					fl_odo_reset_offset.Reset_Kilometer_Offset = fl_odo_reset_offset.Reset_Kilometer_Offset * 100;
					fl_odo_reset_offset.Reset_Mile_Offset = fl_odo_reset_offset.Reset_Mile_Offset * 100;
					if(l_odo_offset_write_ok_bool == FALSE)
					{
						l_odo_offset_write_ok_bool = TRUE;
						(void)Rte_Write_NV_OdoMdl_Reset_OffSet_Offset_Value(&fl_odo_reset_offset);
					}
					//TmExt_Timer_Stop(eCntMenuBtn30sTimer);/*Lhbb20180531*/
				}
			}
		}
		else
		{
			l_menu_btn_timer_s = FALSE;//Menu key up
			//TmExt_Timer_Stop(eCntMenuBtn30sTimer);/*Lhbb20180531*/
		}
	}
	else
	{
		/*Do Nothing*/
	}
}
#endif
/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :OdoLogicalMdl_set_odometer_reset_counter
** @Function Description :
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
static void OdoLogicalMdl_Set_Display_Value(void)
{
	uint32 kilometers_odo = 0;
	uint32 Mliemeters_odo = 0;
	//Get KiloMeters from OdoMdl
	Rte_Read_tiSR_OdoInKiloMeters_KiloMeters(&kilometers_odo);
	Rte_Read_tiSR_OdoInMiles_Miles(&Mliemeters_odo);
	//kilometers_odo is means NVM value error.
	if(kilometers_odo == 0x7FFFFFFF)
	{
		//Display "ERROR"
		Rte_Write_ppSR_OdoOutKiloMeters_Display_Value_KiloMeters(0x7FFFFFFF);
		Rte_Write_ppSR_OdoOutMiles_Display_Value_Miles(0x7FFFFFFF);

	}
	else
	{
		//Display Kilometers
		if(kilometers_odo >= MAX_DISPLAY_ODO_VALUE)
		{
			kilometers_odo = MAX_DISPLAY_ODO_VALUE;
		}

		//Display Miles
		if(Mliemeters_odo >= MAX_DISPLAY_ODO_MileVALUE)
		{
			Mliemeters_odo = MAX_DISPLAY_ODO_MileVALUE;
		}
		Rte_Write_ppSR_OdoOutKiloMeters_Display_Value_KiloMeters(kilometers_odo / ODO_UNIT_DIVIDE);
		Rte_Write_ppSR_OdoOutMiles_Display_Value_Miles(Mliemeters_odo / ODO_UNIT_DIVIDE);
	}
}

#if 0
/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :OdoLogicalMdl_set_odometer_reset_counter
** @Function Description :if Odo displayed value is already >= 255km, 
**                        then Odo value can��t be changed to a less value by any method.
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
static void OdoLogicalMdl_Odo_Backup_Process(void)
{
	uint8 bcm_msg_nr_s = 0;
	uint8 bcm_msg_tout_s = 0;	
	uint32 bcm_msg_odo_value = 0;
	uint8 bcm_msg_odo_valid_value = 0;
	uint8 bcm_msg_odo_reset_counter = 0;
	uint8 bcm_msg_odo_offset_value = 0;
	uint32 kilometers_odo = 0;
	uint32 display_odo_value = 0;
	uint8 routine_control_status = 0;
	uint16 routine_did_number;
	uint64 temp;
	NV_Data_Cluster_ODO_Backup_Counter fl_odo_backup_counter;
	NV_Data_Cluster_Reset_OffSet fl_odo_reset_offset;
	NV_Data_Cluster_Variable_Block fl_cluster_variable_block;
	OdoMdl_RW_Interface_Type odo_rw_interface_value;
	boolean odo_backup_routine_status;
	//CAN msg 0x660
	Rte_Read_rpSR_CANMSG_BCM_GENERAL_STATUS_2_ComOut_VeCAN_BCM_GENERAL_STATUS_2_NR(&bcm_msg_nr_s);
	Rte_Read_rpSR_CANMSG_BCM_GENERAL_STATUS_2_ComOut_VeCAN_BCM_GENERAL_STATUS_2_TOUT(&bcm_msg_tout_s);
	Rte_Read_rpSR_CANMSG_BCM_GENERAL_STATUS_2_ComOut_VeCAN_EB_ODO(&bcm_msg_odo_value);
	Rte_Read_rpSR_CANMSG_BCM_GENERAL_STATUS_2_ComOut_VeCAN_EB_ODO_VALID(&bcm_msg_odo_valid_value);
	Rte_Read_rpSR_CANMSG_BCM_GENERAL_STATUS_2_ComOut_VeCAN_ODOMETER_OFFSET_BCM(&bcm_msg_odo_offset_value);
	Rte_Read_rpSR_CANMSG_BCM_GENERAL_STATUS_2_ComOut_VeCAN_ODOMETER_RESET_COUNTER_BCM(&bcm_msg_odo_reset_counter);
	
	Rte_Read_tiSR_OdoInKiloMeters_KiloMeters(&kilometers_odo);
	(void)Rte_Read_NV_OdoMdl_Reset_OffSet_Offset_Value(&fl_odo_reset_offset);
	(void)Rte_Read_NV_Cluster_ODO_Backup_Counter_ODO_Backup_Value(&fl_odo_backup_counter);
	Rte_Read_rpSR_rOdoLogicalMdl_TIDcmExt_Routin_DID_Number(&routine_did_number);
	Rte_Read_rpSR_rOdoLogicalMdl_TIDcmExt_Routin_Routin_Type(&routine_control_status);
	(void)Rte_Read_NV_Cluster_Variable_Block_Cluster_Variable_Block(&fl_cluster_variable_block);

	Rte_Read_rpSR_OdoMdl_Backup_Routine_Status_Interface_Routine_Status(&odo_backup_routine_status);

	if(fl_cluster_variable_block.ODO_Backup_Sign == 0)
	{
		display_odo_value = kilometers_odo + fl_cluster_variable_block.ODO_Backup_Kilometer_Offset - fl_odo_reset_offset.Reset_Kilometer_Offset;
	}
	else
	{
		display_odo_value = kilometers_odo - fl_cluster_variable_block.ODO_Backup_Kilometer_Offset - fl_odo_reset_offset.Reset_Kilometer_Offset;
	}
	display_odo_value = display_odo_value / 100;
	//CAN msg 0x660
	if(((bcm_msg_nr_s & RTE_E_NEVER_RECEIVED) == RTE_E_NEVER_RECEIVED) || ((bcm_msg_tout_s & RTE_E_TIMEOUT) == RTE_E_TIMEOUT))
	{
		/*Do Nothing*/
	}
	else
	{
		if((bcm_msg_odo_valid_value == 0) && (bcm_msg_odo_reset_counter <= 1) \
			&& (bcm_msg_odo_value <= 0xBEBC200) && (bcm_msg_odo_offset_value <= BACKUP_CHECK_ODO_VALUE))
		{
			if(((display_odo_value < BACKUP_CHECK_ODO_VALUE) && ((bcm_msg_odo_value / 100) >= BACKUP_CHECK_ODO_VALUE)) \
				|| ((display_odo_value >= BACKUP_CHECK_ODO_VALUE) && (display_odo_value < (bcm_msg_odo_value / 100))))
			{
				//Display "ODO ERROR"
				l_odo_display_error_s = TRUE;
				if(l_odo_display_error_store_value_s == FALSE)
				{
					l_odo_display_error_store_value_s = TRUE;
					l_odo_display_error_odo_preview = kilometers_odo;
				}
				if((kilometers_odo - l_odo_display_error_odo_preview) >= DISTANCE_10KM_VALUE)
				{
					l_odo_display_error_odo_preview = kilometers_odo;
					//CAN msg 0x621, set tick value
					Rte_Call_ppCS_TIOdoMdlSendSignalStatus_Operation(TRUE);
				}
				
				/*DID 0x50E6 will reset TC value*/
				if((routine_did_number == 0x50E0) && (odo_backup_routine_status == 1) && (routine_control_status == 1))
				{
					odo_backup_routine_status = 0;
					Rte_Write_rpSR_OdoMdl_Backup_Routine_Status_Interface_Routine_Status(odo_backup_routine_status);
					//Reset Trip
					Rte_Write_ppSR_TITripMdl_TripSR_rpTrip_OdoA_Reset_TripA_Reset(TRUE);
					Rte_Write_ppSR_TITripMdl_TripSR_rpTrip_OdoB_Reset_TripB_Reset(TRUE);

					(void)Rte_Read_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface(&odo_rw_interface_value);
					odo_rw_interface_value.OdoMdl_Trip_Reset_Status[0] = TRUE;
					odo_rw_interface_value.OdoMdl_Trip_Reset_Status[1] = TRUE;
					(void)Rte_Write_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface(&odo_rw_interface_value);
					//Reset AFE
					Rte_Write_ppSR_TITripMdl_TripSR_AFEResetA_AFE_ResetA(TRUE);
					Rte_Write_ppSR_TITripMdl_TripSR_AFEResetB_AFE_ResetB(TRUE);
					//Reset AVS
					Rte_Write_ppSR_TITripMdl_TripSR_AvgSpdResetA_AvgSpd_ResetA(TRUE);
					Rte_Write_ppSR_TITripMdl_TripSR_AvgSpdResetB_AvgSpd_ResetB(TRUE);
					//Reset ET
					Rte_Write_ppSR_TITripMdl_TripSR_ETResetA_ET_ResetA(TRUE);
					Rte_Write_ppSR_TITripMdl_TripSR_ETResetB_ET_ResetB(TRUE);
					if(fl_odo_backup_counter.ODO_Backup_Value != BACKUP_CHECK_ODO_VALUE)
					{
						fl_odo_backup_counter.ODO_Backup_Value++;
						(void)Rte_Write_NV_Cluster_ODO_Backup_Counter_ODO_Backup_Value(&fl_odo_backup_counter);
					}
					
					fl_odo_reset_offset.Reset_Counter = bcm_msg_odo_reset_counter;
					fl_odo_reset_offset.Reset_Kilometer_Offset = bcm_msg_odo_offset_value * 100;
					temp = bcm_msg_odo_offset_value;
					temp = temp * 621371 / 1000000;
					fl_odo_reset_offset.Reset_Mile_Offset = temp * 100;
					if(kilometers_odo <= (bcm_msg_odo_value + bcm_msg_odo_offset_value * 100))
					{
						fl_cluster_variable_block.ODO_Backup_Kilometer_Offset = ((bcm_msg_odo_value / 100) + bcm_msg_odo_offset_value - (kilometers_odo / 100)) * 100;
						fl_cluster_variable_block.ODO_Backup_Sign = 0;
					}
					else
					{
						fl_cluster_variable_block.ODO_Backup_Kilometer_Offset = ((kilometers_odo / 100) - ((bcm_msg_odo_value / 100) + bcm_msg_odo_offset_value)) * 100;
						fl_cluster_variable_block.ODO_Backup_Sign = 1;
					}
					temp = fl_cluster_variable_block.ODO_Backup_Kilometer_Offset / 100;
					temp = temp * 621371 / 1000000;
					fl_cluster_variable_block.ODO_Backup_Mile_Offset = temp * 100;
					(void)Rte_Write_NV_OdoMdl_Reset_OffSet_Offset_Value(&fl_odo_reset_offset);
					(void)Rte_Write_NV_Cluster_Variable_Block_Cluster_Variable_Block(&fl_cluster_variable_block);
				}
				else
				{
					
				}
			}
			else
			{
				odo_backup_routine_status = 0;
				Rte_Write_rpSR_OdoMdl_Backup_Routine_Status_Interface_Routine_Status(odo_backup_routine_status);
				l_odo_display_error_s = FALSE;
				l_odo_display_error_store_value_s = FALSE;
			}
		}
		else
		{
			odo_backup_routine_status = 0;
			Rte_Write_rpSR_OdoMdl_Backup_Routine_Status_Interface_Routine_Status(odo_backup_routine_status);
			l_odo_display_error_s = FALSE;
			l_odo_display_error_store_value_s = FALSE;
		}
	}
}

#endif

#if 0
/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :OdoLogicalMdl_Trip_Process
** @Function Description :
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
static void OdoLogicalMdl_Trip_Process(void)
{
	uint8 trip_index;
	uint32 kilometers_odo;
	uint32 mils_odo;
	trip_kilometer_value_type trip_kilometer_value;
	trip_mile_value_type trip_mile_value;
	NV_Data_Cluster_Reset_OffSet fl_odo_reset_offset;
	NV_Data_Cluster_Variable_Block fl_cluster_variable_block;
	OdoMdl_RW_Interface_Type odo_rw_interface_value;
	DCM_DID_Read dcm_did_read_value;
	//ODO backup interface
	(void)Rte_Read_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface(&odo_rw_interface_value);
	//ODO value
	(void)Rte_Read_tiSR_OdoInKiloMeters_KiloMeters(&kilometers_odo);
	(void)Rte_Read_tiSR_OdoInMiles_Miles(&mils_odo);
	(void)Rte_Read_NV_OdoMdl_Reset_OffSet_Offset_Value(&fl_odo_reset_offset);
	(void)Rte_Read_NV_Cluster_Variable_Block_Cluster_Variable_Block(&fl_cluster_variable_block);

	for(trip_index = 0; trip_index < TRIP_NUMBER; trip_index++)
	{
		/******Reset Trip*******/
		if(odo_rw_interface_value.OdoMdl_Trip_Reset_Status[trip_index] == TRUE)
		{
			//clear the trip reset flag
			odo_rw_interface_value.OdoMdl_Trip_Reset_Status[trip_index] = FALSE;
			(void)Rte_Write_rpSR_OdoMdl_RW_Interface_OdoMdl_RW_Interface(&odo_rw_interface_value);
			l_odo_trip_init_status_bool[trip_index] = FALSE;
		}
		/******IGN OFF 2H,reset some value*******/
		if(l_odo_trip_tripa_reset_2h == TRUE)
		{
			Rte_Write_ppSR_TripMdl_2H_Reset_Port_AVG_2H_RESET(TRUE);
			Rte_Write_ppSR_TripMdl_2H_Reset_Port_ET_2H_RESET(TRUE);
			Rte_Write_ppSR_TripMdl_2H_Reset_Port_AFE_2H_RESET(TRUE);
			//clear the 2H flag
			l_odo_trip_tripa_reset_2h = FALSE;
			//2H only reset TC1
			l_odo_trip_init_status_bool[0] = FALSE;
		}
		/***ODO Backup to reset Trip****/
		if(l_odo_trip_init_status_bool[trip_index] == FALSE)
		{
			l_odo_trip_init_status_bool[trip_index] = TRUE;
			//copy NVM value to trip base
			//if(fl_cluster_variable_block.ODO_Backup_Sign == 0)
			//{
			l_odo_trip_base_kilometer[trip_index] = kilometers_odo;
			l_odo_trip_base_mile[trip_index] = mils_odo;
				//l_odo_trip_base_kilometer[trip_index] = (kilometers_odo + fl_cluster_variable_block.ODO_Backup_Kilometer_Offset - fl_odo_reset_offset.Reset_Kilometer_Offset);
				//l_odo_trip_base_mile[trip_index] =  (mils_odo + fl_cluster_variable_block.ODO_Backup_Mile_Offset - fl_odo_reset_offset.Reset_Mile_Offset);
			//}
			//else
			//{
				//l_odo_trip_base_kilometer[trip_index] = (kilometers_odo - fl_cluster_variable_block.ODO_Backup_Kilometer_Offset - fl_odo_reset_offset.Reset_Kilometer_Offset);
				//l_odo_trip_base_mile[trip_index] =  (mils_odo - fl_cluster_variable_block.ODO_Backup_Mile_Offset - fl_odo_reset_offset.Reset_Mile_Offset);
			//}
		}
		if(kilometers_odo == 0x7FFFFFFF)
		{
			odo_rw_interface_value.OdoMdl_Trip_Reset_Status[trip_index] = FALSE;
		}
		//Kilometer calculate
		trip_kilometer_value[trip_index] = kilometers_odo;
		//trip_kilometer_value[trip_index] = (kilometers_odo + fl_cluster_variable_block.ODO_Backup_Kilometer_Offset - fl_odo_reset_offset.Reset_Kilometer_Offset);
		trip_kilometer_value[trip_index] = trip_kilometer_value[trip_index] - l_odo_trip_base_kilometer[trip_index];
		trip_kilometer_value[trip_index] = trip_kilometer_value[trip_index] % 1000000;
		//Mile calculate
		trip_mile_value[trip_index] = mils_odo;
		//trip_mile_value[trip_index] = (mils_odo + fl_cluster_variable_block.ODO_Backup_Mile_Offset - fl_odo_reset_offset.Reset_Mile_Offset);
		trip_mile_value[trip_index] = trip_mile_value[trip_index] - l_odo_trip_base_mile[trip_index];
		trip_mile_value[trip_index] = trip_mile_value[trip_index] % 1000000;
		
	}
	//Kilometer(TripA and TripB)
	(void)Rte_Write_ppSR_OdoMdl_Trip_Interface_Trip_Kilometer_Value(trip_kilometer_value);
	//Mile(TripA and TripB)
	(void)Rte_Write_ppSR_OdoMdl_Trip_Interface_Trip_Mile_Value(trip_mile_value);
	/**********************************Diagnostic Service 22**************************************/
	//TripA for DID502D
	trip_kilometer_value[0] = trip_kilometer_value[0] / 10;
	dcm_did_read_value.DID_Data[0] = (uint8)((trip_kilometer_value[0] & 0xFF0000) >> 16);
	dcm_did_read_value.DID_Data[1] = (uint8)((trip_kilometer_value[0] & 0x00FF00) >> 8);
	dcm_did_read_value.DID_Data[2] = (uint8)(trip_kilometer_value[0] & 0x0000FF);
	dcm_did_read_value.DID_Length = 3;
	(void)Rte_Write_ppSR_TIDcm_DID_Read_DCM_DID502D_Read_CurentValue(&dcm_did_read_value);
	//TripB for DID502D
	trip_kilometer_value[1] = trip_kilometer_value[1] / 10;
	dcm_did_read_value.DID_Data[0] = (uint8)((trip_kilometer_value[1] & 0xFF0000) >> 16);
	dcm_did_read_value.DID_Data[1] = (uint8)((trip_kilometer_value[1] & 0x00FF00) >> 8);
	dcm_did_read_value.DID_Data[2] = (uint8)(trip_kilometer_value[1] & 0x0000FF);
	dcm_did_read_value.DID_Length = 3;
	(void)Rte_Write_ppSR_TIDcm_DID_Read_DCM_DID502E_Read_CurentValue(&dcm_did_read_value);
}

#endif
/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :OdoLogicalMdl_set_odometer_reset_counter
** @Function Description :
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
static void OdoLogicalMdl_Odo_Write_Data_Process(void)
{
	NVOdoDataSetType odo_data_arry[5];
	NVOdoDataSetType nv_odo_data_arry[5];

	(void)Rte_Read_NV_OdoMdl_NvMOdoData_OdoDataSetArray(odo_data_arry[0]); //Lhbb Rte_MemCpy32
	(void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(nv_odo_data_arry[0]);//Lhbb below five is Rte_MemCpy
	(void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(nv_odo_data_arry[1]);
	(void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(nv_odo_data_arry[2]);
	(void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(nv_odo_data_arry[3]);
	(void)Rte_Read_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(nv_odo_data_arry[4]);

	
	if((odo_data_arry[0]->count != nv_odo_data_arry[0]->count) || (odo_data_arry[0]->extra_count != nv_odo_data_arry[0]->extra_count) || (odo_data_arry[0]->increments != nv_odo_data_arry[0]->increments))
	{
		(void)Rte_Write_NV_OdoMdl_NvMOdoData_Buffer0_OdoDataSetArray(odo_data_arry[0]);
	}
	else if((odo_data_arry[1]->count != nv_odo_data_arry[1]->count) || (odo_data_arry[1]->extra_count != nv_odo_data_arry[1]->extra_count) || (odo_data_arry[1]->increments != nv_odo_data_arry[1]->increments))
	{
		(void)Rte_Write_NV_OdoMdl_NvMOdoData_Buffer1_OdoDataSetArray(odo_data_arry[1]);
	}
	else if((odo_data_arry[2]->count != nv_odo_data_arry[2]->count) || (odo_data_arry[2]->extra_count != nv_odo_data_arry[2]->extra_count) || (odo_data_arry[2]->increments != nv_odo_data_arry[2]->increments))
	{
		(void)Rte_Write_NV_OdoMdl_NvMOdoData_Buffer2_OdoDataSetArray(odo_data_arry[2]);
	}
	else if((odo_data_arry[3]->count != nv_odo_data_arry[3]->count) || (odo_data_arry[3]->extra_count != nv_odo_data_arry[3]->extra_count) || (odo_data_arry[3]->increments != nv_odo_data_arry[3]->increments))
	{
		(void)Rte_Write_NV_OdoMdl_NvMOdoData_Buffer3_OdoDataSetArray(odo_data_arry[3]);
	}
	else if((odo_data_arry[4]->count != nv_odo_data_arry[4]->count) || (odo_data_arry[4]->extra_count != nv_odo_data_arry[4]->extra_count) || (odo_data_arry[4]->increments != nv_odo_data_arry[4]->increments))
	{
		(void)Rte_Write_NV_OdoMdl_NvMOdoData_Buffer4_OdoDataSetArray(odo_data_arry[4]);
	}
	
	else
	{
		
	} 

}
/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :Sp_OdoResetOffSet_NvMNotifyJobFinished_JobFinished
** @Function Description :
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
void OdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	//l_odo_offset_write_ok_bool = FALSE;
}

/*============================================================================
** Function Name:      void OdoLogicalMdl_common_init(void)
**
** Visibility:         fodo.h
**
** Description:        calculation odo with input singals
**
** Invocation:         called by fodo_KSRobin
**
** Inputs/Outputs:     None
**==========================================================================*/
void OdoLogicalMdl_common_init(void)
{
    /*initialize odo collector information with default value*/
    l_odo_collector_S.fl_left_wheel_S.fl_inited_BOOL = FALSE;
    l_odo_collector_S.fl_left_wheel_S.fl_overflow_U32 = 0;
    l_odo_collector_S.fl_right_wheel_S.fl_inited_BOOL = FALSE;
    l_odo_collector_S.fl_right_wheel_S.fl_overflow_U32 = 0;
    l_odo_collector_S.fl_strategy_U8 = FODO_WHEEL_NONE;
//    l_odo_collector_S.fl_odo_main_counts_U32 = 0;
//    l_odo_collector_S.fl_odo_increase_counts_U8 = 0;

	l_odo_count_for_ifc = 0;
	l_odo_loop_num_for_ifc = 0;
	l_odo_value_for_ifc = 0;
}

/*============================================================================
**
** Function Name:      uint32 OdoLogicalMdl_get_main(tODO_UINT uint)
**
** Description:        Get the value of the main odo
**
** Invocation:         called by other subsystem components
**
** Inputs/Outputs:     main_odo - address to put the odo value
**
** Critical Section:
**
**==========================================================================*/
uint32 OdoLogicalMdl_get_main(tODO_UINT uint)
{
    boolean fl_ret_val_BOOL;
    uint32 p_value_addr_P32;

    fl_ret_val_BOOL = GetCurrentOdo(&p_value_addr_P32, TRUE);
    if(FALSE == fl_ret_val_BOOL)
    {
        p_value_addr_P32 = FODO_ODO_NVM_ERROR;
    }
    else
    {
        if (p_value_addr_P32 > FODO_MAIN_MAX_VALUE)
        {
            switch( uint )
            {
            	case ODO_1000M:
            		p_value_addr_P32 = FODO_MAIN_MAX_VALUE / (uint32)l_odo_scales_table[ODO_1000M];
            		break;
            	case ODO_100M:
            		p_value_addr_P32 = FODO_MAIN_MAX_VALUE / (uint32)l_odo_scales_table[ODO_100M];
            		break;
            	case ODO_10M:
            		p_value_addr_P32 = FODO_MAIN_MAX_VALUE / (uint32)l_odo_scales_table[ODO_10M];
            		break;
            	case ODO_1M:
            		p_value_addr_P32 = FODO_MAIN_MAX_VALUE * (uint32)l_odo_scales_table[ODO_1M];
            		break;
            	default:
            		break;
            }
        }
        else
        {
            switch( uint )
            {
            	case ODO_1000M:
            		p_value_addr_P32 = p_value_addr_P32 / (uint32)l_odo_scales_table[ODO_1000M];
            		break;
            	case ODO_100M:
            		p_value_addr_P32 = p_value_addr_P32 / (uint32)l_odo_scales_table[ODO_100M];
            		break;
            	case ODO_10M:
            		p_value_addr_P32 = p_value_addr_P32 / (uint32)l_odo_scales_table[ODO_10M];
            		break;
            	case ODO_1M:
            		p_value_addr_P32 = p_value_addr_P32 * (uint32)l_odo_scales_table[ODO_1M];
            		break;
            	default:
            		break;
            }
        }
    }

    return (p_value_addr_P32);
}

/*============================================================================
**
** Function Name:      void OdoLogicalMdl_manual_reset(void)
**
** Visibility:         OdoMdl.h
**
** Description:        Reset main odo
**
** Invocation:         called by HMI components
**
** Inputs/Outputs:     None
**
** Critical Section:   None.
**==========================================================================*/
void COdoMdl_Manual_Reset(void)
{
    uint32 fl_odo_main_U32;
//    uint8 fl_odo_reset_counter_U8;
    NV_Data_ODO_Reset_Counter fl_odo_reset_cnt;

    /*get current ODO with unit km*/
    fl_odo_main_U32 = OdoLogicalMdl_get_main(ODO_1000M);
    /* get the odo reset counter nvm value */
    Rte_Read_pprpNV_OdoMdl_ODO_Reset_Counter_ResetCounterValue(&fl_odo_reset_cnt);

    /*compare odo value with max reset value*/
    if (ODO_RESET_MAX_DISTANCE >= fl_odo_main_U32)
    {
        /*current odo is less than max reset odo, check reset counter*/
        if (fl_odo_reset_cnt.ResetCounterValue > NVM_ODO_RESET_COUNTER_U8)
        {
            /*odo can be reset, reset odo collector info*/
            l_odo_collector_S.fl_left_wheel_S.fl_overflow_U32 = 0;
            l_odo_collector_S.fl_right_wheel_S.fl_overflow_U32 = 0;
            l_odo_collector_S.fl_odo_main_counts_U32 = 0;
            l_odo_collector_S.fl_odo_increase_counts_U8 = 0;

            /*call main odo reset in algorithm*/
            OdoResetMain();  
            odo_Reset_flag=TRUE;/*Reset TRipA and TripB*/
            /*update reset counter*/
            fl_odo_reset_cnt.ResetCounterValue = fl_odo_reset_cnt.ResetCounterValue - 1;
            Rte_Write_pprpNV_OdoMdl_ODO_Reset_Counter_ResetCounterValue(&fl_odo_reset_cnt);
#if 0  //wli32           
            (void)epm_pkg_q_e2_write(&NVM_ODO_RESET_COUNTER_U8, &fl_odo_reset_counter_U8, NULL, sizeof(NVM_ODO_RESET_COUNTER_U8));

            fdte_Odo_Reset_Notification(); 
#endif            
        }
        else
        {
            /*reset counter is 0, disable reset feature*/
        }
    }
    else
    {
        /*current odo is larger than max reset odo, disable reset feature*/
    }
}
/*============================================================================
**
** Function Name:      void fdiag_app_reset_odo(void)
**
** Visibility:         OdoMdl.h
**
** Description:        Reset odo by diag
**
** Invocation:         called by Dcm components
**
** Inputs/Outputs:     None
**
** Critical Section:   None.
**==========================================================================*/
void fdiag_app_reset_odo(void)
{
    NV_Data_ODO_Reset_Counter fl_odo_reset_cnt;
	fl_odo_reset_cnt.ResetCounterValue = 4; /* E2 fault value is 0,so this write 4*/
	NVOdoDataSetType odo_data_arry_ini[5]={{0x0006U, 0xFFU, (uint8)0x80U},
                                 	       {0x0006U, 0xFFU, (uint8)0x80U},
                                 	       {0x0006U, 0xFFU, (uint8)0x80U},
                                 		   {0x0006U, 0xFFU, (uint8)0x80U},
                                 		   {0x0006U, 0xFFU, (uint8)0x80U}};
	 Rte_Write_NV_OdoMdl_NvMOdoData_OdoDataSetArray(odo_data_arry_ini[0]);
	 Rte_Write_pprpNV_OdoMdl_ODO_Reset_Counter_ResetCounterValue(&fl_odo_reset_cnt);
	 odo_Reset_flag = TRUE;/*Reset TRipA and TripB*/
}
/*============================================================================
** Function Name:      void OdoLogicalMdl_check_left_wheel_status(void)
**
** Description:        Get the edge base value of 1km for different variant
**
** Invocation:         called by fodo_IDTT
**
** Inputs/Outputs:     None
**==========================================================================*/
uint8 OdoLogicalMdl_check_left_wheel_status(void)
{
    /*if the vehicle speed message is missing, invalid and never Received don't do anything*/
    if (FALSE == (l_odo_collector_S.fl_left_wheel_S.fl_get_missing() & FODO_CAN_SIGNAL_INVALID))
    {
        /*check left wheel first input flag for first init or signal recovery*/
        if (FODO_WHEEL_VALID == l_odo_collector_S.fl_left_wheel_S.fl_get_status())
        {
            /*check if last value inited, if not, update last value*/
            if (FALSE == l_odo_collector_S.fl_left_wheel_S.fl_inited_BOOL)
            {
                l_odo_collector_S.fl_left_wheel_S.fl_inited_BOOL = TRUE;
                l_odo_collector_S.fl_left_wheel_S.fl_value_U16 = l_odo_collector_S.fl_left_wheel_S.fl_get_value();
            }
            else
            {
                /*last value inited, nothing to do*/
            }

            /*input signals valid, return ok result*/
            return FODO_OK;
        }
        else
        {
            /*input signals invalid, return nok result*/
            return FODO_NOK;
        }
    }
    else
    {
        /*input signals missing, update wheel value init flag*/
        l_odo_collector_S.fl_left_wheel_S.fl_inited_BOOL = FALSE;

        /*input signals missing, return nok result*/
        return FODO_NOK;
    }
}

/*============================================================================
** Function Name:      void OdoLogicalMdl_check_right_wheel_status(void)
**
** Description:        Get the edge base value of 1km for different variant
**
** Invocation:         called by fodo_IDTT
**
** Inputs/Outputs:     None
**==========================================================================*/
uint8 OdoLogicalMdl_check_right_wheel_status(void)
{
    /*if the vehicle speed message is missing, invalid and never Received don't do anything*/
    if (FALSE == (l_odo_collector_S.fl_right_wheel_S.fl_get_missing() & FODO_CAN_SIGNAL_INVALID))
    {
        /*check left wheel first input flag for first init or signal recovery*/
        if (FODO_WHEEL_VALID == l_odo_collector_S.fl_right_wheel_S.fl_get_status())
        {
            /*check if last value inited, if not, update last value*/
            if (FALSE == l_odo_collector_S.fl_right_wheel_S.fl_inited_BOOL)
            {
                l_odo_collector_S.fl_right_wheel_S.fl_inited_BOOL = TRUE;
                l_odo_collector_S.fl_right_wheel_S.fl_value_U16 = l_odo_collector_S.fl_right_wheel_S.fl_get_value();
            }
            else
            {
                /*last value inited, nothing to do*/
            }

            /*input signals valid, return ok result*/
            return FODO_OK;
        }
        else
        {
            /*input signals invalid, return nok result*/
            return FODO_NOK;
        }
    }
    else
    {
        /*input signals missing, update wheel value init flag*/
        l_odo_collector_S.fl_right_wheel_S.fl_inited_BOOL = FALSE;

        /*input signals missing, return nok result*/
        return FODO_NOK;
    }
}

/*============================================================================
** Function Name:      void OdoLogicalMdl_calc_left_wheel_info(void)
**
** Description:        calculate overflow for input value.
**
** Invocation:         called by fodo_IDTT
**
** Inputs/Outputs:     None
**==========================================================================*/
uint8 OdoLogicalMdl_calc_left_wheel_info(void)
{
    uint8 fl_calc_result_U8;
    uint16 fl_new_value_U16;

    /*get new left wheel info*/
    fl_new_value_U16 = l_odo_collector_S.fl_left_wheel_S.fl_get_value();

    /*compare new value and last value*/
    if (fl_new_value_U16 >= l_odo_collector_S.fl_left_wheel_S.fl_value_U16)
    {
        /*compare value change with limit value*/
        if (FODO_OVERFLOW_LIMIT >= (fl_new_value_U16 - l_odo_collector_S.fl_left_wheel_S.fl_value_U16))
        {
            /*new value valid, update left wheel info*/
            l_odo_collector_S.fl_left_wheel_S.fl_value_U16 = fl_new_value_U16;

            /*new value valid, update overflow*/
            l_odo_collector_S.fl_left_wheel_S.fl_overflow_U32 = l_odo_collector_S.fl_left_wheel_S.fl_overflow_U32 + 0;

            /*update calculate result*/
            fl_calc_result_U8 = FODO_OK;
        }
        else
        {
            /*new value invalid, calculation cancel*/
            l_odo_collector_S.fl_left_wheel_S.fl_value_U16 = fl_new_value_U16;

            /*update calculate result*/
            fl_calc_result_U8 = FODO_NOK;
        }
    }
    else if (fl_new_value_U16 < l_odo_collector_S.fl_left_wheel_S.fl_value_U16)
    {
        /*compare value change with limit value*/
        if (FODO_OVERFLOW_LIMIT >= ((uint32)fl_new_value_U16 + FODO_OVERFLOW_BASE - l_odo_collector_S.fl_left_wheel_S.fl_value_U16))
        {
            /*new value valid, update left wheel info*/
            l_odo_collector_S.fl_left_wheel_S.fl_value_U16 = fl_new_value_U16;

            /*new value valid, update overflow*/
            l_odo_collector_S.fl_left_wheel_S.fl_overflow_U32 = l_odo_collector_S.fl_left_wheel_S.fl_overflow_U32 + 1;

            /*update calculate result*/
            fl_calc_result_U8 = FODO_OK;
        }
        else
        {
            /*new value invalid, calculation cancel*/
            l_odo_collector_S.fl_left_wheel_S.fl_value_U16 = fl_new_value_U16;

            /*update calculate result*/
            fl_calc_result_U8 = FODO_NOK;
        }
    }
    else
    {
    }

    return fl_calc_result_U8;
}

/*============================================================================
** Function Name:      void OdoLogicalMdl_calc_right_wheel_info(void)
**
** Description:        calculate overflow for input value.
**
** Invocation:         called by fodo_IDTT
**
** Inputs/Outputs:     None
**==========================================================================*/
uint8 OdoLogicalMdl_calc_right_wheel_info(void)
{
    uint8 fl_calc_result_U8;
    uint16 fl_new_value_U16;

    /*get new right wheel info*/
    fl_new_value_U16 = l_odo_collector_S.fl_right_wheel_S.fl_get_value();

    /*compare new value and last value*/
    if (fl_new_value_U16 >= l_odo_collector_S.fl_right_wheel_S.fl_value_U16)
    {
        /*compare value change with limit value*/
        if (FODO_OVERFLOW_LIMIT >= (fl_new_value_U16 - l_odo_collector_S.fl_right_wheel_S.fl_value_U16))
        {
            /*new value valid, update right wheel info*/
            l_odo_collector_S.fl_right_wheel_S.fl_value_U16 = fl_new_value_U16;

            /*new value valid, update overflow*/
            l_odo_collector_S.fl_right_wheel_S.fl_overflow_U32 = l_odo_collector_S.fl_right_wheel_S.fl_overflow_U32 + 0;

            /*update calculate result*/
            fl_calc_result_U8 = FODO_OK;
        }
        else
        {
            /*new value invalid, calculation cancel*/
            l_odo_collector_S.fl_right_wheel_S.fl_value_U16 = fl_new_value_U16;

            /*update calculate result*/
            fl_calc_result_U8 = FODO_NOK;
        }
    }
    else if (fl_new_value_U16 < l_odo_collector_S.fl_right_wheel_S.fl_value_U16)
    {
        /*compare value change with limit value*/
        if (FODO_OVERFLOW_LIMIT >= ((uint32)fl_new_value_U16 + FODO_OVERFLOW_BASE - l_odo_collector_S.fl_right_wheel_S.fl_value_U16))
        {
            /*new value valid, update right wheel info*/
            l_odo_collector_S.fl_right_wheel_S.fl_value_U16 = fl_new_value_U16;

            /*new value valid, update overflow*/
            l_odo_collector_S.fl_right_wheel_S.fl_overflow_U32 = l_odo_collector_S.fl_right_wheel_S.fl_overflow_U32 + 1;

            /*update calculate result*/
            fl_calc_result_U8 = FODO_OK;
        }
        else
        {
            /*new value invalid, calculation cancel*/
            l_odo_collector_S.fl_right_wheel_S.fl_value_U16 = fl_new_value_U16;

            /*update calculate result*/
            fl_calc_result_U8 = FODO_NOK;
        }
    }
    else
    {
    }

    return fl_calc_result_U8;
}

/*============================================================================
** Function Name:      void OdoLogicalMdl_calc_odo_with_both_wheel(void)
**
** Description:        calculate overflow for input value.
**
** Invocation:         called by fodo_IDTT
**
** Inputs/Outputs:     None
**==========================================================================*/
void OdoLogicalMdl_calc_odo_with_both_wheel(void)
{
    uint32 fl_odo_wheel_left_U32;
    uint32 fl_odo_wheel_right_U32;
    uint32 fl_odo_counts_U32;

    /*calculate odo count value for left wheel*/
    fl_odo_wheel_left_U32 = l_odo_collector_S.fl_left_wheel_S.fl_value_U16 + \
                            (l_odo_collector_S.fl_left_wheel_S.fl_overflow_U32 << 16);

    /*calculate odo count value for right wheel*/
    fl_odo_wheel_right_U32 = l_odo_collector_S.fl_right_wheel_S.fl_value_U16 + \
                             (l_odo_collector_S.fl_right_wheel_S.fl_overflow_U32 << 16);

    /*calculate average odo count with left and right wheel values*/
    fl_odo_counts_U32 = (((fl_odo_wheel_left_U32 + fl_odo_wheel_right_U32)) >> 1);

    /*check current odo count status*/
    if (fl_odo_counts_U32 < l_odo_collector_S.fl_odo_main_counts_U32)
    {
        /*current odo count is less than last record, no increase to update*/
        l_odo_collector_S.fl_odo_increase_counts_U8 = 0;
    }
    else if (fl_odo_counts_U32 - l_odo_collector_S.fl_odo_main_counts_U32 > FODO_OVERFLOW_LIMIT)
    {
        /*increase counts is larger than max change, update increase as max*/
        l_odo_collector_S.fl_odo_increase_counts_U8 = 0;
    }
    else
    {
        /*increase counts = current counts - last counts*/
        l_odo_collector_S.fl_odo_increase_counts_U8 = fl_odo_counts_U32 - l_odo_collector_S.fl_odo_main_counts_U32;
    }

    /*call odo pkg algorithm to calculate odo*/
    OdoLogicalMdl_ifc_add_count(l_odo_collector_S.fl_odo_increase_counts_U8);		//add by sli34 2019/03/08
#if 0 //wli32    
    (void)odo_add_counts((UINT8)(l_odo_collector_S.fl_odo_increase_counts_U8), ODO_NORMAL);
    fodo_ifc_add_count((UINT8)(l_odo_collector_S.fl_odo_increase_counts_U8));
#else
    Rte_Write_ppSR_TIOdoMdl_ComOut_NewRollingOdo(l_odo_collector_S.fl_odo_increase_counts_U8);
#endif
    /*record current odo counts for next calculation*/
    l_odo_collector_S.fl_odo_main_counts_U32 = fl_odo_counts_U32;
    TripA_odo_wheel_increase_value += l_odo_collector_S.fl_odo_increase_counts_U8;
    //TripB_odo_wheel_increase_value += l_odo_collector_S.fl_odo_increase_counts_U8;
	average_speed_wheel_increase_value += l_odo_collector_S.fl_odo_increase_counts_U8;
}

/*============================================================================
** Function Name:      void OdoLogicalMdl_calc_odo_with_left_wheel(void)
**
** Description:        calculate overflow for input value.
**
** Invocation:         called by fodo_IDTT
**
** Inputs/Outputs:     None
**==========================================================================*/
void OdoLogicalMdl_calc_odo_with_left_wheel(void)
{
    uint32 fl_odo_counts_U32;

    /*calculate odo count value for left wheel*/
    fl_odo_counts_U32 = l_odo_collector_S.fl_left_wheel_S.fl_value_U16 + \
                        (l_odo_collector_S.fl_left_wheel_S.fl_overflow_U32 << 16);

    /*check current odo count status*/
    if (fl_odo_counts_U32 < l_odo_collector_S.fl_odo_main_counts_U32)
    {
        /*current odo count is less than last record, no increase to update*/
        l_odo_collector_S.fl_odo_increase_counts_U8 = 0;
    }
    else if (fl_odo_counts_U32 - l_odo_collector_S.fl_odo_main_counts_U32 > FODO_OVERFLOW_LIMIT)
    {
        /*increase counts is larger than max change, update increase as max*/
        l_odo_collector_S.fl_odo_increase_counts_U8 = 0;
    }
    else
    {
        /*increase counts = current counts - last counts*/
        l_odo_collector_S.fl_odo_increase_counts_U8 = fl_odo_counts_U32 - l_odo_collector_S.fl_odo_main_counts_U32;
    }

    /*call odo pkg algorithm to calculate odo*/    
    OdoLogicalMdl_ifc_add_count(l_odo_collector_S.fl_odo_increase_counts_U8);			//add by sli34 2019/03/08
#if 0  //wli32    
    (void)odo_add_counts((UINT8)(l_odo_collector_S.fl_odo_increase_counts_U8), ODO_NORMAL);
    fodo_ifc_add_count((UINT8)(l_odo_collector_S.fl_odo_increase_counts_U8));
#else
    Rte_Write_ppSR_TIOdoMdl_ComOut_NewRollingOdo(l_odo_collector_S.fl_odo_increase_counts_U8);
#endif
    /*record current odo counts for next calculation*/
    l_odo_collector_S.fl_odo_main_counts_U32 = fl_odo_counts_U32;
    TripA_odo_wheel_increase_value += l_odo_collector_S.fl_odo_increase_counts_U8;
    //TripB_odo_wheel_increase_value += l_odo_collector_S.fl_odo_increase_counts_U8;
	average_speed_wheel_increase_value += l_odo_collector_S.fl_odo_increase_counts_U8;
}

/*============================================================================
** Function Name:      void OdoLogicalMdl_calc_odo_with_right_wheel(void)
**
** Description:        calculate overflow for input value.
**
** Invocation:         called by fodo_IDTT
**
** Inputs/Outputs:     None
**==========================================================================*/
void OdoLogicalMdl_calc_odo_with_right_wheel(void)
{
    uint32 fl_odo_counts_U32;

    /*calculate odo count value for left wheel*/
    fl_odo_counts_U32 = l_odo_collector_S.fl_right_wheel_S.fl_value_U16 + \
                        (l_odo_collector_S.fl_right_wheel_S.fl_overflow_U32 << 16);

    /*check current odo count status*/
    if (fl_odo_counts_U32 < l_odo_collector_S.fl_odo_main_counts_U32)
    {
        /*current odo count is less than last record, no increase to update*/
        l_odo_collector_S.fl_odo_increase_counts_U8 = 0;
    }
    else if (fl_odo_counts_U32 - l_odo_collector_S.fl_odo_main_counts_U32 > FODO_OVERFLOW_LIMIT)
    {
        /*increase counts is larger than max change, update increase as max*/
        l_odo_collector_S.fl_odo_increase_counts_U8 = 0;
    }
    else
    {
        /*increase counts = current counts - last counts*/
        l_odo_collector_S.fl_odo_increase_counts_U8 = fl_odo_counts_U32 - l_odo_collector_S.fl_odo_main_counts_U32;
    }

    /*call odo pkg algorithm to calculate odo*/    
    OdoLogicalMdl_ifc_add_count(l_odo_collector_S.fl_odo_increase_counts_U8);			//add by sli34 2019/03/08
#if 0 //wli32
    (void)odo_add_counts((UINT8)(l_odo_collector_S.fl_odo_increase_counts_U8), ODO_NORMAL);
    fodo_ifc_add_count((UINT8)(l_odo_collector_S.fl_odo_increase_counts_U8));
#else
    Rte_Write_ppSR_TIOdoMdl_ComOut_NewRollingOdo(l_odo_collector_S.fl_odo_increase_counts_U8);
#endif
    /*record current odo counts for next calculation*/
    l_odo_collector_S.fl_odo_main_counts_U32 = fl_odo_counts_U32;
    TripA_odo_wheel_increase_value += l_odo_collector_S.fl_odo_increase_counts_U8;
    //TripB_odo_wheel_increase_value += l_odo_collector_S.fl_odo_increase_counts_U8;
	average_speed_wheel_increase_value += l_odo_collector_S.fl_odo_increase_counts_U8;
}

uint8 can_get_WhlMilgFrntLeStatus_status(void)
{
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;    
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8MsgNR);
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8MsgTout);
    
    return (uint8)(u8MsgNR | u8MsgTout);
}

uint8 can_get_WhlMilgFrntLeStatus_value(void)
{
    uint8 fl_ABS_WhlMilgFrntLeStatus;
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLeStatus(&fl_ABS_WhlMilgFrntLeStatus);
    return(fl_ABS_WhlMilgFrntLeStatus);
}

uint16 can_get_WhlMilgFrntLe_value(void)
{
    uint16 fl_WhlMilgFrntLe;    
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntLe(&fl_WhlMilgFrntLe);
    return(fl_WhlMilgFrntLe);
}

uint8 can_get_WhlMilgFrntRiStatus_status(void)
{
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;    
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8MsgNR);
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8MsgTout);
    
    return (uint8)(u8MsgNR | u8MsgTout);
}

uint8 can_get_WhlMilgFrntRiStatus_value(void)
{
    uint8 fl_ABS_WhlMilgFrntRiStatus;
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRiStatus(&fl_ABS_WhlMilgFrntRiStatus);
    return(fl_ABS_WhlMilgFrntRiStatus);
}

uint16 can_get_WhlMilgFrntRi_value(void)
{
    uint16 fl_ABS_WhlMilgFrntRi;
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_WhlMilgFrntRi(&fl_ABS_WhlMilgFrntRi);
    return(fl_ABS_WhlMilgFrntRi);
}


//add by sli34 2019/03/20
void OdoLogicalMdl_ifc_add_count(uint8 odo_count)		
{
	boolean IsEngineCfg = FALSE;
	uint8 fl_odo_loop_num;

	//Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Engine, &IsEngineCfg);
	if(IsEngineCfg == FALSE)							//Diesel/5000RPM
	{
		fl_odo_loop_num = ODO_LOOP_NUM_FOR_1S;			//1s period (车辆行驶距离以1s为单位进行计算)
	}
	else												//Gasoline/7000RPM
	{
		fl_odo_loop_num = ODO_LOOP_NUM_FOR_100MS;		//100ms period (每100ms内车辆行驶的里程数，单位 km)
	}
	
	if(odo_count != 0)
	{
		l_odo_loop_num_for_ifc++;
	    l_odo_count_for_ifc += odo_count;
	}
	
	//odologicial refresh period is 10ms
	if(l_odo_loop_num_for_ifc == fl_odo_loop_num)			//1s period (车辆行驶距离以1s为单位进行计算)
	{
		l_odo_value_for_ifc = (l_odo_count_for_ifc * 1000 *10) / ODO_CALC_FACTOR_VALUE;
		l_odo_count_for_ifc = 0;
		l_odo_loop_num_for_ifc = 0;
	}
}

uint32 OdoLogicalMdl_get_odo_for_ifc(void)
{
    return l_odo_value_for_ifc;
}

static void Clear_Odo_Entry_Condition(void)
{
	uint8 fl_menu_button_value_u8 = 0;
	boolean IsTimerStatrt= FALSE;
	boolean IsTimerElapsed= FALSE;
	uint32 fl_odo_main_U32;
		/*get current ODO with unit km*/
	fl_odo_main_U32 = OdoLogicalMdl_get_main(ODO_1000M);

	//uint8 okpress = 0;
	//Rte_Read_rpCS_BtnMdl_MenuButtonValue(&fl_menu_button_value_u8);
	Rte_Read_rpIgnState_IGNState(&eCurrentIgnState_status);
	Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Confirm(&fl_menu_button_value_u8);
	if(fl_menu_button_value_u8 == 1)/*OK press*/
	{
		OK_Button_Pressed = TRUE;
	}
	else
	{
		OK_Button_Pressed = FALSE;
	}
    if( ( ePrevIgnitionState == eIGN_OFF )  && ( eCurrentIgnState_status == eIGN_RUN ) && (OK_Button_Pressed == TRUE) )
    {
        l_is_ign_transition = TRUE;/*IGNOFF long press*/
    }
    ePrevIgnitionState = eCurrentIgnState_status;
	if((l_is_ign_transition == TRUE) && (OK_Button_Pressed == TRUE))/*IGNON long press 5S*/
	{
		if(fl_odo_main_U32 < ODO_RESET_MAX_DISTANCE)
		{
			Rte_Write_ppodo_ems_ischange_isodochange(TRUE);/*ignore ems update odo*/
		}
		else 
		{
			Rte_Write_ppodo_ems_ischange_isodochange(FALSE);
		}
		Rte_Call_rptimer_OdoLogic_TmExt_Timer_IsStarted(ClearOdo5Sec,&IsTimerStatrt);
		if(FALSE == IsTimerStatrt)
		{
			Rte_Call_rptimer_OdoLogic_TmExt_Timer_Start(ClearOdo5Sec);
		}
		Rte_Call_rptimer_OdoLogic_TmExt_Timer_IsElapsed(ClearOdo5Sec,5000,&IsTimerElapsed);
		if(TRUE == IsTimerElapsed)/*5s */
		{
			Rte_Call_rptimer_OdoLogic_TmExt_Timer_Stop(ClearOdo5Sec);
			COdoMdl_Manual_Reset();
			l_is_ign_transition = FALSE;
		}
	}
	else
	{
		Rte_Write_ppodo_ems_ischange_isodochange(FALSE);
		Rte_Call_rptimer_OdoLogic_TmExt_Timer_Stop(ClearOdo5Sec);
		l_is_ign_transition = FALSE;
	}
}

//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================

