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
#include "Rte.h"
#include "Rte_type.h"
#include "Rte_VehicleCfgMdl.h"
#include "CmpLib.h"
#include "system.h"
//#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(VehicleCfgMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static void VehicleCfg_Test(void);

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 VehicleCfg[20] = {0};
//static uint8 VehicleCfg[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	Rte_Read_NV_Vehicle_Configure_VehicleConfigSetArray(VehicleCfg);
	
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

	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
/****************************************************************************
Function Name     	: CmpActive

Description         : This function process the oilpressure TT ON/OFF for
					  ISG /NON ISG Variant.

Invocation          : called in cmpActive.

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static Std_ReturnType CmpActive( void )
{
	//VehicleCfg_Test();
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

// V348 Vehicle Configuration
void fdiag_app_get_vehicle_cfg(Vehicle_Cfg_Type CfgNO, uint8 *IsCfg)
{
	switch(CfgNO)
	{
		case VEHICLE_CONFIGURATION_ADAS:      	  				//0=no;0001 = adas1;0010=adas2		
			*IsCfg = VehicleCfg[0]&(BIT0|BIT1|BIT2|BIT3);	
		break;
		case VEHICLE_CONFIGURATION_TIRE:      					//00000000=215;00000001=185 
			*IsCfg = VehicleCfg[1] &(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);
		break;
		case VEHICLE_CONFIGURATION_TPMS:     					//00=no;01=yes;11=two rear tire
			*IsCfg = (VehicleCfg[2]&(BIT6|BIT7))>>6;
		break;
		case VEHICLE_CONFIGURATION_Language:      				//000=Chinese;001=English;010=Arabic
			*IsCfg = (VehicleCfg[2]&(BIT3|BIT4|BIT5))>>3;
		break;
		case VEHICLE_CONFIGURATION_ABS:      					//0=no;1=yes
			*IsCfg = TESTBIT(VehicleCfg[2],BIT2) ? TRUE : FALSE;
		break;
		case VEHICLE_CONFIGURATION_Cruise:      		 		//0=no;01=cc;10=acc
			*IsCfg = VehicleCfg[2]&(BIT0|BIT1);
		break;
		case VEHICLE_CONFIGURATION_Engine:     				 	//0000=DISEL;0001=GAS;0010=BEV;0011=PHEV
			*IsCfg = (VehicleCfg[3]&(BIT4|BIT5|BIT6|BIT7))>>4;
		break;
		case VEHICLE_CONFIGURATION_PASSENGER_BELT:     				 //0=NO;1=YES
		*IsCfg = TESTBIT(VehicleCfg[3],BIT0) ? TRUE : FALSE;
		break;
		case VEHICLE_CONFIGURATION_PEPS:      					//00=NO;01=IMMO;10=PEPS;11=NO
			*IsCfg = (VehicleCfg[4]&(BIT6|BIT7))>>6;
		break;
		case VEHICLE_CONFIGURATION_EPS:	  						//00=NO;01=YES
			*IsCfg = TESTBIT(VehicleCfg[4],BIT5) ? TRUE : FALSE;
		break;
		case VEHICLE_CONFIGURATION_AIRBAG:	  					//00=NO;01=YES
			*IsCfg = TESTBIT(VehicleCfg[4],BIT4) ? TRUE : FALSE;
		break;
		case VEHICLE_CONFIGURATION_ESP:      	  				//00=NO;01=YES
			*IsCfg = TESTBIT(VehicleCfg[4],BIT3) ? TRUE : FALSE;
		break;
		case VEHICLE_CONFIGURATION_BRAKE_TYPE:      			//00=Mechanical brake;01=Electronic Brake
			*IsCfg = VehicleCfg[4]&(BIT0|BIT1);
		break;
		case VEHICLE_CONFIGURATION_REAR_SENSORS:				//0000=Not Recognize;0001=TWO SENSORS;0010=Three Sensors;
		*IsCfg = (VehicleCfg[5]&(BIT7|BIT6|BIT5|BIT4))>>4;		//0011=four Sensors;0100=without sensors
		break;
		case VEHICLE_CONFIGURATION_FRONT_SENSORS:       		//0000=Not Recognize;0001=TWO SENSORS;0010=Three Sensors;
			*IsCfg = VehicleCfg[5]&(BIT0|BIT1|BIT2|BIT3);		//0011=four Sensors;0100=without sensors
		break;
		case VEHICLE_CONFIGURATION_FUEL_VOLUME:       			//00000000=63L;00000001=60L;00000010=80L;00000011=100L;00000100=N526
			*IsCfg = VehicleCfg[6]&(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);		
		break;
		case VEHICLE_CONFIGURATION_ELECTRONIC_CLUTCH:       			//00000000=NO;00000001=YES
			*IsCfg = VehicleCfg[7]&(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);		
		break;
		case VEHICLE_CONFIGURATION_AT_MT:       				//00000000=5MT;00000001=6MT;00000010=AMT
			*IsCfg = VehicleCfg[8]&(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);		
		break;
		case VEHICLE_CONFIGURATION_EMISSION_LEVELS:       							//00000000=one;00000001=two;00000010=three;
			*IsCfg = VehicleCfg[9]&(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);		//00000011=four;00000100=five;00000101=six
		break;
		case VEHICLE_CONFIGURATION_OVER_SPEED:       							//00000000=No;00000001=80km/h;00000010=100km/h;00000011=120km/h
			*IsCfg = VehicleCfg[10]&(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7);	
		break;
		default:
			*IsCfg = FALSE;
		break;

	}
}



static void VehicleCfg_Test(void)
{
	uint8 fl_vehicle_ADAS = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_ADAS,&fl_vehicle_ADAS);
	
	uint8 fl_vehicle_TIRE = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_TIRE,&fl_vehicle_TIRE);
	
	uint8 fl_vehicle_TPMS = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_TPMS,&fl_vehicle_TPMS);
	
	uint8 fl_vehicle_Language = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_Language,&fl_vehicle_Language);

	uint8 fl_vehicle_ABS = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_ABS,&fl_vehicle_ABS);

	uint8 fl_vehicle_Cruise = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_Cruise,&fl_vehicle_Cruise);

	uint8 fl_vehicle_PASSENGER_BELT = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_PASSENGER_BELT,&fl_vehicle_PASSENGER_BELT);

	uint8 fl_vehicle_PEPS = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_PEPS,&fl_vehicle_PEPS);

	uint8 fl_vehicle_EPS = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_EPS,&fl_vehicle_EPS);

	uint8 fl_vehicle_AIRBAG = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_AIRBAG,&fl_vehicle_AIRBAG);

	uint8 fl_vehicle_ESP = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_ESP,&fl_vehicle_ESP);

	uint8 fl_vehicle_BRAKE_TYPE = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_BRAKE_TYPE,&fl_vehicle_BRAKE_TYPE);

	uint8 fl_vehicle_REAR_SENSORS = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_REAR_SENSORS,&fl_vehicle_REAR_SENSORS);

	uint8 fl_vehicle_FRONT_SENSORS = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_FRONT_SENSORS,&fl_vehicle_FRONT_SENSORS);

	uint8 fl_vehicle_FUEL_VOLUME = 0;
	fdiag_app_get_vehicle_cfg(VEHICLE_CONFIGURATION_FUEL_VOLUME,&fl_vehicle_FUEL_VOLUME);

	uint8 fl_vehicle = 0;
}


