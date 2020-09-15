/******************************************************************************
              CONFIDENTIAL VISTEON CORPORATION

 This is an unpublished work of authorship, which contains trade secrets,
 created in 2012. Visteon Corporation owns all rights to this work and
 intends to maintain it in confidence to preserve its trade secret status.
 Visteon Corporation reserves the right, under the copyright laws of the
 United States or those of any other country that may have  jurisdiction,
 to protect this work as an unpublished work, in the event of an
 inadvertent or deliberate unauthorized publication. Visteon Corporation
 also reserves its rights under all copyright laws to protect this work as
 a published work, when appropriate. Those having access to this work may
 not copy it, use it, modify it or disclose the information contained in
 it without the written authorization of Visteon Corporation
******************************************************************************/
/*****************************************************************************

File Name        :  TripMdl.c
Module Short Name:  TripMdl.c
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define TripMdl_C

#include "Rte_TripMdl.h"
#include "CmpLib.h"
#include "Rte.h"
#include "AfcMdl_cfg.h"
#include "DteMdl_cfg.h"
//#include "dem.h"
//#include "Mcu.h"
//#include "OdoMdl.h"

/*@brief  Instance of life-cycle state-machine*/

CMPLIB_INSTANCE(TripMdl)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/



/*****************************************************************************
*                                 Manifest Constants                         *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/

/*****************************************************************************
*                                 Macro Definitions                          *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/
#define MAX_DISPLAY_TRIP_VALUE	4999900 /*Trip display MAXVAlue*/
//#define CALCULATION_FACTOR 43779
#define   Vehicle_Tire_245_Trip		   46118
#define   Vehicle_Tire_235_Trip		   43529
#define CALCULATION_UINT    10
#define OFFSET_VALUE    100
#define NVM_CRC_TRIP_ODO			0xFFFF
#define MAINTENANCE_SET_5000KM		5000
#define MINIMUM_MAINTENANCE_VALUE	5000
#define UPDATE_TIME					500
#define RPTIMER						490
#define CanOut_TIME					1000
#define MIN_DRIVE_DISTANCE			300
#define INVALID_VALUE_SPEED			0xFF
#define MAX_SPEED					200 //Modify Date: 2020/6/9 author:ZhangTing 
#define AVERAGE_UPDATE_TIME			10000
#define MAX_DRIVE_TIME				720000 /*100 hours */

/*                                 Type Declarations                          *
******************************************************************************/


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/


#pragma ghs section bss=".buram"

uint32 TripA_odo_wheel_increase_value = 0; /*total increase value from odological mdl*/
uint32 TripACurrentOdoKm = 0;    /*total current trip odo*/
//uint32  TripB_odo_wheel_increase_value = 0; /*total increase value from odological mdl*/
uint32 TripBCurrentOdoKm = 0;    /*total current trip odo*/
uint16 Current_maintenence_odo = 0;
static uint32 odo_average_speed = 0;
static uint64 odo_drive_distance = 0;
static uint64  curr_drive_distance = 0;
static uint32   average_speed = 0;
static uint32  drive_time = 0;
static uint32  odo_drive_time = 0;
static	uint8  PowerON_Triaverspeed = 0;
static	uint8  PowerON_odoaverspeed = 0;
uint32  average_speed_wheel_increase_value = 0;
static uint16 Remainder = 0;

#pragma ghs section bss=default

static boolean IGN_OFF_FLAG = FALSE;
//static boolean Reset_TripA_FLAG = FALSE;
//static boolean Reset_TripB_FLAG = FALSE;
static boolean storage_maintenance_falg = FALSE;
extern boolean odo_Reset_flag;
NV_Data_Trip_A Trip_A_dataset;
NV_Data_Trip_A Trip_B_dataset;
extern Boolean bTripA_RstCmd;
static uint32 CALCULATION_FACTOR = 0;
extern void ConvertKmToMiles(uint32 *pCurrentOdoValue );
extern uint32 OdoLogicalMdl_get_main(tODO_UINT uint);



/*****************************************************************************
*                   Functions                                                *
******************************************************************************/
static void calculate_trip_A_distance_Process(void);
#if 0
static void reset_TripB_Process(void);
static void calculate_trip_B_distance_Process(void);
#endif
static void reset_TripA_Process(void);
static void Current_maintenence_odo_Process(void);
static void Reset_maintenence_odo(void);
extern boolean GetCurrentOdo(uint32 *pCurrentOdoValue , boolean CurrentOdoIsMetric);
static void average_speed_cal(void);
static boolean can_signal_isinvalid(void);
static void average_speed_init(void);
static void calculation_average_speed_distance(void);
static void clear_trip_average_speed(void);
static void clear_odo_average_speed(void);
static boolean Idle_speed(void);
static void CANMessage_out_speed(void);
static void average_update_process( void );
//static void calculate_Maintenance_Mileage_Process(void);
static void Running_time(void);
static void Vehicle_cfg_init_Trip(void);
static void get_current_maintenance_distance(void);



#if 0
static void IGNON_Restore_Process(void);
static void TripB_Set_Dispaly_Value(void);
void reset_trip_B_odo(void);
#endif
static void TripA_Set_Dispaly_Value(void);
void reset_trip_A_odo(void);
void write_NVM_Trip_A_odo(void);
void write_NVM_Trip_B_odo(void);
#if 0
void read_NVM_Trip_A_odo(NV_Data_Trip_A Trip_A_dataset);
void read_NVM_Trip_B_odo(NV_Data_Trip_A Trip_B_dataset);
#endif
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
//	TripACurrentOdoKm=0;  /*get Trip totalodo*/
//	TripA_odo_wheel_increase_value=0;  /*get trip increase odo*/
//	TripBCurrentOdoKm=0;
//	TripB_odo_wheel_increase_value=0;
//	odo_average_speed=INVALID_VALUE_SPEED;
//	average_speed=INVALID_VALUE_SPEED;
	average_speed_init();

	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional de-initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
    get_current_maintenance_distance();

	average_speed_init();

	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional activation state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation( void )
{
    get_current_maintenance_distance();
    average_speed_init();
	Vehicle_cfg_init_Trip();

	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional de-activation state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation( void )
{
	average_speed_init();
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{	
	eIgnState fl_ign = 0;
    EBatteryState fl_batt = 0;
    static uint8 fl_ign_sts = 0;

    Rte_Read_rpBattState_BatteryState(&fl_batt);
    if ((fl_batt != eBatteryState_OverVoltage) && (fl_batt != eBatteryState_UnderVoltage)) /*overPh1,normal,underPh1*/
    {
		Rte_Read_rpIgnState_IGNState(&fl_ign);/* ModMdl.c eKeyPos_Run/eKeyPos_Off/eKeyPos_Count*/
 		if(fl_ign == eIGN_RUN)
 		{
             if(eIGN_OFF == fl_ign_sts)
             {
                 fl_ign_sts = eIGN_RUN;
				 Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed(odo_average_speed);
				 Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(average_speed);
             }
             if(IGN_OFF_FLAG == TRUE)/*IGN restore  doing*/
             {

            	// IGNON_Restore_Process();
                 IGN_OFF_FLAG=FALSE;
             }
			 calculation_average_speed_distance();		 
			 average_speed_cal();
             calculate_trip_A_distance_Process();
			 //calculate_Maintenance_Mileage_Process();
			 Current_maintenence_odo_Process();
             TripA_Set_Dispaly_Value();
             reset_TripA_Process();
			#if 0
             reset_TripB_Process();
             TripB_Set_Dispaly_Value();
             calculate_trip_B_distance_Process();
			#endif
			 CANMessage_out_speed();
			 average_update_process();
			 Running_time();


 		}
 		else
 		{
 			if(IGN_OFF_FLAG == FALSE)/*IGN OFF  doing*/
 			{
 				IGN_OFF_FLAG = TRUE;
 			}
            fl_ign_sts = eIGN_OFF;
			/*reset_trip_B_odo();  IGNOFF clear TripB*/
			average_speed_init();
 		}
     }
 	else
 	{
 	  //batt high/low
 	  Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAverageSpeed);
	  //Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(INVALID_VALUE_SPEED);
	  //Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed(INVALID_VALUE_SPEED);
 	}
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Diagnostic state
///
/// @return E_OK:			Diagnostic has completed. Go to DiagReturn state.
///<br> 	E_NOT_READY:	Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:			Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br> 	E_NOT_READY:	Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
	return E_OK;
}

static void calculate_trip_A_distance_Process(void)
{
	uint32 TripCounter = 0;
	TripCounter = TripA_odo_wheel_increase_value * 10 + Remainder;
    if(TripCounter > CALCULATION_FACTOR)
    {
    	TripACurrentOdoKm = TripACurrentOdoKm + OFFSET_VALUE;/*Driving 100 meters*/
    	Remainder = TripCounter- CALCULATION_FACTOR;
    	TripA_odo_wheel_increase_value = Remainder / 10;
		Remainder = Remainder % 10;
    }

}
#if 0
static void calculate_trip_B_distance_Process(void)
{
    if(TripB_odo_wheel_increase_value > (CALCULATION_FACTOR/CALCULATION_UINT))
    {
    	TripBCurrentOdoKm = TripBCurrentOdoKm + OFFSET_VALUE;/*Driving 100 meters*/
    	TripB_odo_wheel_increase_value = TripB_odo_wheel_increase_value - (CALCULATION_FACTOR/CALCULATION_UINT);
    }
}

static void calculate_Maintenance_Mileage_Process(void)
{
    if(TripB_odo_wheel_increase_value > CALCULATION_FACTOR)
    {
		storage_maintenance_falg = TRUE;
    	TripB_odo_wheel_increase_value = TripB_odo_wheel_increase_value - CALCULATION_FACTOR;
    }
}
#endif

/*============================================================================
** Function Name:      reset_TripA_Process()
**
** Description:        reset tripA
**
** Invocation:         called by trip,Get button information from HMI
**
** Inputs/Outputs:     None
**==========================================================================*/
static void reset_TripA_Process(void)
{

	if(bTripA_RstCmd == TRUE)
	{
		reset_trip_A_odo();
		//Reset_TripA_FLAG = FALSE;
		clear_trip_average_speed();/*clear average speed*/
		fafc_clear_single_value();	/* Reset single AFC value */
		bTripA_RstCmd = FALSE;
	}
	if(odo_Reset_flag == TRUE)/*after odo reset,TripA and TripB reset*/
	{
		reset_trip_A_odo();
		/*reset_trip_B_odo();*/
		Reset_maintenence_odo();/*clear remain maintenance odo*/
		clear_odo_average_speed();/*clear odo average speed*/
		odo_Reset_flag = FALSE;		
		fafc_clear_acc_value();		/* Reset Accumulate AFC value */
		fdte_clear_DteValue();		/* Reset DTE value */
	}
}
#if 0
/*============================================================================
** Function Name:      reset_TripB_Process()
**
** Description:        reset tripB
**
** Invocation:         called by trip,Get button information from HMI
**
** Inputs/Outputs:     None
**==========================================================================*/
static void reset_TripB_Process(void)
{
	boolean reset_button_flag=FALSE;
	Rte_Read_rpSR_ResetTripWrnMdl_ResetTripInstruc_ResetTripBInstruc(&reset_button_flag);
	if((Reset_TripB_FLAG == FALSE)&&(reset_button_flag==TRUE))
	{
		Reset_TripB_FLAG = TRUE;
	}
	if(Reset_TripB_FLAG == TRUE)
	{
		if(reset_button_flag == FALSE)
		{
			reset_trip_B_odo();
			Reset_TripB_FLAG = FALSE;
		}
	}
}
/*============================================================================
** Function Name:      IGNON_Restore_Process()
**
** Description:        IGN OFF>>IGNON
**
** Invocation:
**
** Inputs/Outputs:     None
**==========================================================================*/
static void IGNON_Restore_Process(void)
{
    NV_Data_Trip_A Trip_A_lastdataset_tmp;
    NV_Data_Trip_A Trip_B_lastdataset_tmp;
	Rte_Read_TripMdl_pprpNV_TripMdl_TripA_value(&Trip_A_lastdataset_tmp);
	Rte_Read_TripMdl_pprpNV_TripMdl_TripB_value(&Trip_B_lastdataset_tmp);
	TripACurrentOdoKm=Trip_A_lastdataset_tmp.TripTotalODO*OFFSET_VALUE;  /*get Trip totalodo*/
	TripA_odo_wheel_increase_value=Trip_A_lastdataset_tmp.TripIncrease;
	TripBCurrentOdoKm=Trip_B_lastdataset_tmp.TripTotalODO*OFFSET_VALUE;
	TripB_odo_wheel_increase_value=Trip_B_lastdataset_tmp.TripIncrease;
}
#endif
/*============================================================================
** Function Name:      reset_trip_A_odo
**
** Description:        reset tripA
**
** Invocation:         called by trip,Get button information from HMI
**
** Inputs/Outputs:     None
**==========================================================================*/
void reset_trip_A_odo(void)
{
	TripACurrentOdoKm = 0;
	TripA_odo_wheel_increase_value = 0;
	Remainder = 0;
}

#if 0
/*============================================================================
** Function Name:      reset_trip_B_odo
**
** Description:        reset tripB
**
** Invocation:         called by trip,Get button information from HMI
**
** Inputs/Outputs:     None
**==========================================================================*/
void reset_trip_B_odo(void)
{
	TripBCurrentOdoKm = 0;
	TripB_odo_wheel_increase_value = 0;
}
#endif
/*============================================================================
** Function Name:      write_NVM_Trip_A_odo
**
** Description:        write trip odo to EEPROM
**
** Invocation:         called by tripmdl
**
** Inputs/Outputs:     None
**==========================================================================*/
#if 0
void write_NVM_Trip_A_odo(void)
{
	Trip_A_dataset.TripTotalODO=TripACurrentOdoKm/100;
    Trip_A_dataset.TripIncrease=TripA_odo_wheel_increase_value;
	Rte_Write_TripMdl_pprpNV_TripMdl_TripA_value(&Trip_A_dataset);
}
void write_NVM_Trip_B_odo(void)
{
	Trip_B_dataset.TripTotalODO=TripBCurrentOdoKm/100;
    Trip_B_dataset.TripIncrease=TripB_odo_wheel_increase_value;
	Rte_Write_TripMdl_pprpNV_TripMdl_TripB_value(&Trip_B_dataset);
}
#endif
#if 0
void read_NVM_Trip_A_odo(NV_Data_Trip_A Trip_A_dataset)
{
	Rte_Read_TripMdl_pprpNV_TripMdl_TripA_value(&Trip_A_dataset);
}
void read_NVM_Trip_B_odo(NV_Data_Trip_A Trip_B_dataset)
{
	Rte_Read_TripMdl_pprpNV_TripMdl_TripB_value(&Trip_B_dataset);
}
#endif

/*============================================================================
** Function Name:      TripA_Set_Dispaly_Value
**
** Description:        set HMI display value
**
** Invocation:         called by tripmdl
**
** Inputs/Outputs:     None
**==========================================================================*/
static void TripA_Set_Dispaly_Value(void)
{
	//uint8 fl_vehicle_cfg_unit_flg = 0;
	uint32 Trip_meters_Mile = 0;
   	//Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ODO_UNIT,&fl_vehicle_cfg_unit_flg);
	if(TripACurrentOdoKm > MAX_DISPLAY_TRIP_VALUE)
	{
		TripACurrentOdoKm=0;
		//TripA_odo_wheel_increase_value = 0;
		drive_time = 0;
		curr_drive_distance = 0;  /*clear drive time and distance and speed*/
		average_speed = INVALID_VALUE_SPEED;
		PowerON_Triaverspeed = 0;
	}
    
    Rte_Write_TripMdl_ppSR_TripMdl_TripKiloMeters_TripA(TripACurrentOdoKm);
    #if 0
	Trip_meters_Mile = TripACurrentOdoKm;
	ConvertKmToMiles(&Trip_meters_Mile);
	if(fl_vehicle_cfg_unit_flg == FALSE)
	{
    	Rte_Write_TripMdl_ppSR_TripMdl_TripKiloMeters_TripA(TripACurrentOdoKm);
	}
	else 
	{	
		Rte_Write_TripMdl_ppSR_TripMdl_TripKiloMeters_TripA(Trip_meters_Mile);
	}
    #endif
}
#if 0
/*============================================================================
** Function Name:      TripB_Set_Dispaly_Value
**
** Description:        set HMI display value
**
** Invocation:         called by tripmdl
**
** Inputs/Outputs:     None
**==========================================================================*/
static void TripB_Set_Dispaly_Value(void)
{
	if(TripBCurrentOdoKm > MAX_DISPLAY_TRIP_VALUE)
	{
		TripBCurrentOdoKm = 0;
		TripB_odo_wheel_increase_value = 0;
	}
	Rte_Write_TripMdl_ppSR_TripMdl_TripKiloMeters_TripB(TripBCurrentOdoKm);

}
#endif
/*Maintenance mileage calculation*/
static void Current_maintenence_odo_Process(void)
{
	uint32 p_value_odo_P32;
	NV_Data_Trip_A    Maintenance_mileage;
    Rte_Read_ppsr_Maintenance_mileage_distance_value(&Current_maintenence_odo);
    Rte_Read_TripMdl_pprpNV_TripMdl_TripA_value(&Maintenance_mileage);
    p_value_odo_P32 = OdoLogicalMdl_get_main(ODO_1000M);
    if(p_value_odo_P32 == FODO_ODO_NVM_ERROR)
    {
            p_value_odo_P32 = 0;
    }
    if(Maintenance_mileage.TripTotalODO >= p_value_odo_P32 )
    {
	    Current_maintenence_odo = Maintenance_mileage.TripTotalODO - p_value_odo_P32;
    }
    else 
    {
        Current_maintenence_odo = 0;
    }
	Rte_Write_TripMdl_ppsr_Maintenance_mileage_distance_value(Current_maintenence_odo);

	/* cpang1 */
    Rte_Write_ppSR_CANMSG_IC_0x392_ComOut_IC_ChangOilDistance(Current_maintenence_odo);

	
}
void Menu_Send_TripMdl_Reset_maintence_odo(uint16 maintenance)
{
    NV_Data_Trip_B    Backup_odo;
    uint16 backup_odo_count = 0;
    Rte_Read_pprpNV_TripMdl_TripB_value(&Backup_odo);
    backup_odo_count = Backup_odo.TripIncrease;
	Current_maintenence_odo = maintenance;
    Backup_odo.TripTotalODO = maintenance;
    Backup_odo.TripIncrease = backup_odo_count;
    Rte_Write_TripMdl_pprpNV_TripMdl_TripB_value(&Backup_odo);
}
static void Reset_maintenence_odo(void)
{
	NV_Data_Trip_A    Maintenance_mileage;
    NV_Data_Trip_B    Backup_odo;
    uint32 p_value_odo_P32;
    Rte_Read_pprpNV_TripMdl_TripB_value(&Backup_odo);
    
	if(odo_Reset_flag == TRUE)/*after odo reset,TripA and TripB reset*/
    {
       p_value_odo_P32 = 0;
    } 
    else 
    {
        p_value_odo_P32 = OdoLogicalMdl_get_main(ODO_1000M);
        if(p_value_odo_P32 == FODO_ODO_NVM_ERROR)
        {
            p_value_odo_P32 = 0;
        }
    }
    if(Backup_odo .TripTotalODO <= 5000)
    {
        Backup_odo .TripTotalODO = 5000;
    }
    Backup_odo.TripIncrease = 0;
    Maintenance_mileage.TripIncrease = 0;    
	Maintenance_mileage.TripTotalODO = p_value_odo_P32 + Backup_odo .TripTotalODO;
	Rte_Write_TripMdl_pprpNV_TripMdl_TripA_value(&Maintenance_mileage);
    Rte_Write_TripMdl_pprpNV_TripMdl_TripB_value(&Backup_odo);
    Rte_Write_TripMdl_ppsr_Maintenance_mileage_distance_value(Backup_odo .TripTotalODO);
}

void Reset_DPF_Value(void)
{
	Reset_maintenence_odo();
}

/*average speed calculation*/
#if 0
static void average_speed_cal(void)
{
	boolean IsTimerStatrt;
	boolean IsTimerElapsed;
	Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerAverageSpeed,&IsTimerStatrt);
	if(FALSE == can_signal_isinvalid())/*CAN signal status valid*/
	{
		if(TRUE == Idle_speed())/*more then 300rpm start timer*/
		{
			if(FALSE == IsTimerStatrt)
			{
				Rte_Call_rpTimer_TmExt_Timer_Start(eTimerAverageSpeed);
			}
			Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerAverageSpeed,UPDATE_TIME,&IsTimerElapsed);
			rptimer++;/*10ms cycle*/
			if(rptimer >= 0xFFFFFFFF)		
			{
				rptimer = 0;
			}
			if((curr_drive_distance >= MIN_DRIVE_DISTANCE) && (PowerON_Triaverspeed == 0))/*power on less then 300 update 
			averagespeed,after no exe*/
			{
				PowerON_Triaverspeed = 1;
				average_speed = (curr_drive_distance * 360)/rptimer;/*odo_drive_distance*3600*100/1000*/
				if(average_speed >= MAX_SPEED)
				{
					average_speed = MAX_SPEED;
				}
				Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(average_speed);
			}
			if(curr_drive_distance < MIN_DRIVE_DISTANCE)
			{
				average_speed = INVALID_VALUE_SPEED;
				Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(average_speed);
			}
						
			if((odo_drive_distance >= MIN_DRIVE_DISTANCE) && (PowerON_odoaverspeed == 0))/*power on less then 300 update averagespeed*/
			{
				PowerON_odoaverspeed = 1;
				odo_average_speed = (odo_drive_distance * 360) / rptimer;/*odo_drive_distance*3600*100/1000*/
				if(odo_average_speed >= MAX_SPEED)
				{
					odo_average_speed = MAX_SPEED;
				}
				Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed(odo_average_speed);
			}
			if(odo_drive_distance < MIN_DRIVE_DISTANCE)
			{
				odo_average_speed = INVALID_VALUE_SPEED;
				Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed(odo_average_speed);
			}
			if(TRUE == IsTimerElapsed)/*10S update speed*/
			{
				Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAverageSpeed);
				drive_time++;/*time=drivetime*10s,unit 10S*/
				odo_drive_time++;
				if((drive_time >= 0x5FFFA) || (odo_drive_time >= 0x5FFFA))/*max 0xFFFF minute*/
				{
					drive_time = 0;
					curr_drive_distance = 0;  /*clear drive time and distance and speed*/
					average_speed = 0;
					Trip_reached_max = 1;
					odo_drive_distance = 0;
					odo_average_speed = 0;
					odo_drive_time = 0;
					rptimer = 0;
					PowerON_odoaverspeed = 0;
					PowerON_Triaverspeed = 0;

				}
				if(curr_drive_distance >= MIN_DRIVE_DISTANCE)/*more then 300m  or trip reached 0xffff minute*/
				{
					average_speed=(curr_drive_distance * 3600 / 1000) / (drive_time * 10);
					if(average_speed >= MAX_SPEED)
					{
						average_speed = MAX_SPEED;
					}
					Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(average_speed);
				}
				else  /*less then 300m,display ---*/
				{
					Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(INVALID_VALUE_SPEED);
				}
				/*odo average speed*/
				if(odo_drive_distance >= MIN_DRIVE_DISTANCE)/*more then 300m  or trip reached 0xffff minute*/
				{
					odo_average_speed = (odo_drive_distance * 3600 / 1000)/(odo_drive_time * 10);
					if(odo_average_speed >= MAX_SPEED)
					{
						odo_average_speed = MAX_SPEED;
					}
					Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed(odo_average_speed);
				}
				else  /*less then 300m,display ---*/
				{
					Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed(INVALID_VALUE_SPEED);
				}
			}
		}
		else
		{
			Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAverageSpeed);/*less then 300rpm*/
		}
	}
	else
	{
		Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAverageSpeed);/*signal invaliad*/
	}
}
#endif
static boolean can_signal_isinvalid(void)
{
	uint8 can_10BNR_status = 0;
	uint8 can_10BTout_status = 0;
	uint8 can_221NR_status = 0;
	uint8 can_221Tout_status = 0;
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&can_10BNR_status);
	Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&can_10BTout_status);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&can_221NR_status);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&can_221Tout_status);
	if((RTE_E_NEVER_RECEIVED == can_10BNR_status) || (RTE_E_TIMEOUT == can_10BTout_status))
	{
		return TRUE;
	}
	else if((RTE_E_NEVER_RECEIVED == can_221NR_status) || (RTE_E_TIMEOUT == can_221Tout_status))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static boolean Idle_speed(void)
{
	uint32  EMS_EngSpeed = 0;
	uint32	fl_VehicleFilterSpd_U16 = 0;
	Rte_Read_rpSR_TITachMdl_value(&EMS_EngSpeed);
	Rte_Read_rpSR_TISpdMdl_value(&fl_VehicleFilterSpd_U16);
	if((EMS_EngSpeed  >= 30000) && (fl_VehicleFilterSpd_U16 >= 300))
	{
		return  TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

static void average_speed_init(void)
{
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAverageSpeed);
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerTrip10s);
	if(curr_drive_distance < MIN_DRIVE_DISTANCE)
	{
		average_speed = INVALID_VALUE_SPEED;
	}
	if(odo_drive_distance < MIN_DRIVE_DISTANCE)
	{
		odo_average_speed = INVALID_VALUE_SPEED;
	}
	Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed(odo_average_speed);
	Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(average_speed);
	//Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(INVALID_VALUE_SPEED);	/*display---*/
	//drive_time=0;
	//curr_drive_distance=0;  /*clear drive time and distance and speed*/
	//average_speed=0;
	//average_speed_wheel_increase_value=0;
}
static void calculation_average_speed_distance(void)
{
	if((average_speed_wheel_increase_value * 1000) >= CALCULATION_FACTOR)
    {
    	average_speed_wheel_increase_value = (average_speed_wheel_increase_value *1000 - CALCULATION_FACTOR) / 1000;
		curr_drive_distance++;/*get current drive distance,unit 1m*/
		odo_drive_distance++;
    }
}

static void clear_trip_average_speed(void)
{
	drive_time = 0;
	curr_drive_distance = 0;  /*clear drive time and distance and speed*/
	average_speed = INVALID_VALUE_SPEED;
	PowerON_Triaverspeed = 0;
	Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(INVALID_VALUE_SPEED);
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAverageSpeed);
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerTrip10s);
}

static void clear_odo_average_speed(void)
{
	odo_drive_distance = 0;/*clear odo drive time and distance and speed*/
	odo_average_speed = INVALID_VALUE_SPEED;
	drive_time = 0;
	odo_drive_time = 0;
	curr_drive_distance = 0;  /*clear trip drive time and distance and speed*/
	average_speed = INVALID_VALUE_SPEED;
	PowerON_odoaverspeed = 0;
	PowerON_Triaverspeed = 0;
	Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed(INVALID_VALUE_SPEED);
	Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(INVALID_VALUE_SPEED);
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAverageSpeed);
	Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerTrip10s);
}


static void CANMessage_out_speed(void)
{
	boolean IsTimerStatrt= FALSE;;
	boolean IsTimerElapsed= FALSE;;
	static uint8   count = 0;
	Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerTripCanOut,&IsTimerStatrt);
	if(FALSE == IsTimerStatrt)
	{
		Rte_Call_rpTimer_TmExt_Timer_Start(eTimerTripCanOut);
	}
	Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerTripCanOut,CanOut_TIME,&IsTimerElapsed);
	if(TRUE == IsTimerElapsed)/*1S update speed*/
	{
		Rte_Write_ppSR_CANMSG_IC_0x392_ComOut_IC_AverageSpeed((uint8)odo_average_speed);
	}
}


static void average_speed_cal(void)
{
	boolean IsTimerStatrt= FALSE;
	boolean IsTimerElapsed= FALSE;
	if(FALSE == can_signal_isinvalid())/*CAN signal status valid*/
	{
		if(TRUE == Idle_speed())/*more then 300rpm start timer*/
		{
			Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerAverageSpeed,&IsTimerStatrt);

			if(FALSE == IsTimerStatrt)
			{
				Rte_Call_rpTimer_TmExt_Timer_Start(eTimerAverageSpeed);
			}
			Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerAverageSpeed,RPTIMER,&IsTimerElapsed);
			if(TRUE == IsTimerElapsed)/*500ms update speed*/
			{
				Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAverageSpeed);
				drive_time++;/*time=drivetime*/
				odo_drive_time++;
				if((drive_time >= MAX_DRIVE_TIME) || (odo_drive_time >= MAX_DRIVE_TIME))/*max 100 hours*/
				{
					drive_time = 0;
					curr_drive_distance = 0;  /*clear drive time and distance and speed*/
					average_speed = 0;
					odo_drive_distance = 0;
					odo_average_speed = 0;
					odo_drive_time = 0;
					PowerON_odoaverspeed = 0;
					PowerON_Triaverspeed = 0;

				}
				if(curr_drive_distance >= MIN_DRIVE_DISTANCE)/*more then 300m  or trip reached 0xffff minute*/
				{
					if(drive_time != 0)
					{
						average_speed = (curr_drive_distance * 3600 / UPDATE_TIME) / drive_time;
					}
					if(average_speed >= MAX_SPEED)
					{
						average_speed = MAX_SPEED;
					}
					if(PowerON_Triaverspeed == 0)
					{
						PowerON_Triaverspeed = 1;
						Rte_Write_TripMdl_ppsr_average_speed_average_speed_value((uint8)average_speed);						
					}

					//Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(average_speed);
				}
				else  /*less then 300m,display ---*/
				{
					average_speed = INVALID_VALUE_SPEED;
					Rte_Write_TripMdl_ppsr_average_speed_average_speed_value((uint8)average_speed);
				}
				/*odo average speed*/
				if(odo_drive_distance >= MIN_DRIVE_DISTANCE)/*more then 300m  or trip reached 0xffff minute*/
				{
					if(odo_drive_time != 0)
					{
						odo_average_speed = (odo_drive_distance * 3600 / UPDATE_TIME) / odo_drive_time;
					}
					if(odo_average_speed >= MAX_SPEED)
					{
						odo_average_speed = MAX_SPEED;
					}
					if(PowerON_odoaverspeed == 0)
					{
						PowerON_odoaverspeed = 1;
						Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed((uint8)odo_average_speed);						
					}
				}
				else  /*less then 300m,display ---*/
				{
					odo_average_speed = INVALID_VALUE_SPEED;
					Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed((uint8)odo_average_speed);
				}

			}

		}
		else
		{
			Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAverageSpeed);/*less then 300rpm*/
		}
	}
	else
	{
		Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerAverageSpeed);/*signal invaliad*/
		Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed(INVALID_VALUE_SPEED);
		Rte_Write_TripMdl_ppsr_average_speed_average_speed_value(INVALID_VALUE_SPEED);//Modify Date: 2020/6/9 author:ZhangTing 
	}

}

static void average_update_process( void )
{
	boolean IsTimerStatrt = FALSE;
	boolean IsTimerElapsed = FALSE;
    static boolean previous_status = FALSE;
    static boolean  current_status = FALSE;
	Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerTrip10s,&IsTimerStatrt);
	if(FALSE == can_signal_isinvalid())
    {
       current_status = FALSE; 
    }
    else 
    {
        current_status = TRUE;
    }
    if(previous_status != current_status)
    {
      	Rte_Write_TripMdl_ppsr_average_speed_average_speed_value((uint8)average_speed);
		Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed((uint8)odo_average_speed);	  
    }
    previous_status = current_status;
	if(FALSE == IsTimerStatrt)
	{
		Rte_Call_rpTimer_TmExt_Timer_Start(eTimerTrip10s);
	}
	Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerTrip10s,AVERAGE_UPDATE_TIME,&IsTimerElapsed);
	if(TRUE == IsTimerElapsed)/*10S update speed*/
	{
		Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerTrip10s);
		Rte_Write_TripMdl_ppsr_average_speed_average_speed_value((uint8)average_speed);
		Rte_Write_TripMdl_ppsr_odo_average_speed_odoaveragespeed((uint8)odo_average_speed);		
	}

}
static void Running_time(void)
{
	uint32 hours = 0;
	uint32 minute = 0;
	uint32 seconds = 0;
	seconds =  odo_drive_time * 5 / 10; /* S*/
	minute  =  odo_drive_time * 5 / 600;/* min*/
	hours   =  odo_drive_time * 5 / 36000;/* hours*/
	if(hours > 0)
	{
		if(minute >= (hours * 60))
		{
			minute = minute - hours * 60;
		}
		else
		{
			minute = 0;
		}
	}
	//Rte_Write_ppSR_CANMSG_IC_0x387_ComOut_IC_DriveTimeMinute((uint8)minute);
	//Rte_Write_ppSR_CANMSG_IC_0x387_ComOut_IC_DriveTimehour((uint8)hours);
}

static void Vehicle_cfg_init_Trip(void)
{
	uint8 fl_vehicle_cfg_Tire_flg = 0;

	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_TIRE,&fl_vehicle_cfg_Tire_flg);
	if(fl_vehicle_cfg_Tire_flg != 0 )
	{
		CALCULATION_FACTOR = Vehicle_Tire_245_Trip; /*185*/
	}
	else 
	{
		CALCULATION_FACTOR = Vehicle_Tire_235_Trip;/*215*/
	}
	
}

static void get_current_maintenance_distance(void)
{
    NV_Data_Trip_A    Maintenance_mileage;
	uint32 p_value_odo_P32;
    p_value_odo_P32 = OdoLogicalMdl_get_main(ODO_1000M);
	Rte_Read_TripMdl_pprpNV_TripMdl_TripA_value(&Maintenance_mileage);
    if(Maintenance_mileage.TripTotalODO >= p_value_odo_P32 )
    {
	    Current_maintenence_odo = Maintenance_mileage.TripTotalODO - p_value_odo_P32;
    }
    else 
    {
        Current_maintenence_odo = 0;
    }
    Rte_Write_TripMdl_ppsr_Maintenance_mileage_distance_value(Current_maintenence_odo);
	Rte_Write_ppSR_CANMSG_IC_0x392_ComOut_IC_ChangOilDistance(Current_maintenence_odo);
}

