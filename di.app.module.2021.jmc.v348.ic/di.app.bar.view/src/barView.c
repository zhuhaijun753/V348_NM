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

// #include "Rte_AbsMdl.h"
#include "Rte_BarView.h"
#include "CmpLib.h"
#include "bar_view_cfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(BarView)

static boolean SetCoolantLight(uint8 targetLevel, uint8 barType);
static void CoolantLightBarProcess(void);
static void FuelLevelLightBarProcess(void);
static void Bar_PowerOn_Self_Test_Action(void);
static void Bar_PowerOff_light(void);
static void Bar_PowerOn_Self_Test(void);

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================

static uint8 l_IGN_statePre_U8;
static uint8 l_ProveoutCounter_U8 = 0;
static uint8 l_coolant_bar_Prelevel = 0;
static uint8 l_fuelLevel_bar_Prelevel = 0;
static boolean l_bar_selfTest_flag;
static boolean l_trans_from_off_to_on;
static boolean l_selfTest_finish;
static uint8 l_pre_animationFlag;
static uint8 l_CheckStateFinish_u8;
static uint8 l_CheckSelfStatus;

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
#define PROVEOUT_MAX	29   //3S self test
#define BAR_MAX			8
#define BAR_AWAIT_MAX	21



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
static Std_ReturnType CmpInit( void )
{
	l_ProveoutCounter_U8 = 0;
	l_IGN_statePre_U8 = eIGN_OFF;
	l_selfTest_finish = FALSE;
	l_CheckStateFinish_u8 = FALSE;
	l_CheckSelfStatus = 0;
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
	l_ProveoutCounter_U8 = 0;
	l_CheckSelfStatus = 0;
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
    return  E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	eIgnState fl_IGN_state_U8;
	EBatteryState fl_Battery_state;
	uint8 fl_animationFlag;
	uint8 l_bar_status = 0;
	
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state_U8);
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);

	if((fl_IGN_state_U8 == eIGN_RUN) && (l_IGN_statePre_U8 == eIGN_OFF))		//ign from off to on
	{
		l_trans_from_off_to_on = TRUE;
		l_selfTest_finish = FALSE;				//initialize state
		l_ProveoutCounter_U8 = 0;				//initialize state when ign from off to on
		l_CheckSelfStatus = 0;
	}
	else if((fl_IGN_state_U8 == eIGN_OFF) && (l_IGN_statePre_U8 == eIGN_RUN))	//ign from on to off
	{
		l_trans_from_off_to_on = FALSE;
		l_CheckStateFinish_u8 = FALSE;			//STOP LED BAR check when ign from on to off
	}

	
	if(fl_animationFlag == FALSE && l_pre_animationFlag == TRUE )					//Animation is over
	{
		l_CheckStateFinish_u8 = TRUE;
	}
	
	l_pre_animationFlag = fl_animationFlag;
	
	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{		
		// Bar_PowerOff_light();
	}
 	else
 	{
		if(fl_IGN_state_U8 == eIGN_RUN) 
		{	
			if(l_trans_from_off_to_on == TRUE)		//ign off --> on	
			{			
				l_trans_from_off_to_on = FALSE;
				Bar_PowerOn_Self_Test();
			}	
			
			if(l_CheckStateFinish_u8 == TRUE && l_selfTest_finish == FALSE)
			{
				l_bar_status = 1;
				Bar_PowerOn_Self_Test_Action();
			}
			
			//ign oning
			if((fl_IGN_state_U8 == eIGN_RUN) && (l_selfTest_finish == TRUE))
			{
				l_bar_status = 2;		
				// CoolantLightBarProcess();
				// FuelLevelLightBarProcess();
			}
		}
		else
		{
			// Bar_PowerOff_light();
		}		
	}	

	l_IGN_statePre_U8 = fl_IGN_state_U8;

	Rte_Write_ppCS_BarStatus_barst(l_bar_status);
	
	Rte_Write_ppSR_BarView_Self_Test_status(l_CheckSelfStatus);
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


static void CoolantLightBarProcess(void)
{
	uint8 fl_coolant_bar_level_current;	
	Rte_Read_rpCoolantTmp_level(&fl_coolant_bar_level_current);

	if(fl_coolant_bar_level_current != l_coolant_bar_Prelevel)
	{
		SetCoolantLight(fl_coolant_bar_level_current, BAR_TYPE_COOLANT);
	}		
}

static void FuelLevelLightBarProcess(void)
{
	uint8 fl_fuelLevel_bar_level_current;	
	Rte_Read_tiSR_FuelLevel_level(&fl_fuelLevel_bar_level_current);

	if(fl_fuelLevel_bar_level_current != l_fuelLevel_bar_Prelevel)
	{
		SetCoolantLight(fl_fuelLevel_bar_level_current, BAR_TYPE_FUEL_LEVEL);
	}		
}



static boolean SetCoolantLight(uint8 barLevel,uint8 barType)
{
	uint8 targetBit;
	uint8 i = 0;
	BarLightTable* barTblArray;
	BarLightTable barTable;

	switch(barType)
	{
		case BAR_TYPE_COOLANT:
			barTblArray = coolantTbl;
			l_coolant_bar_Prelevel = barLevel;
			break;
		case BAR_TYPE_FUEL_LEVEL:			
			barTblArray = fuelLevelTbl;
			l_fuelLevel_bar_Prelevel = barLevel;
			break;
		default:
			break;
	}
	
	if(barLevel < BAR_LED_MIN || barLevel > BAR_LED_MAX)
	{
		targetBit = 0;
	}
	else
	{
		targetBit = barLevel;	
	}	
	
	for(i = BAR_LED_MIN; i <= BAR_LED_MAX; i++)
	{
		barTable = *(barTblArray+(i-1));
		if(i <= targetBit)
		{
			SET_BAR_PIN(barTable.WaterLightNo, 1);
		}
		else
		{
			SET_BAR_PIN(barTable.WaterLightNo, 0);
		}
	}
	return TRUE;
}

void test_BarView(void)
{
	CoolantLightBarProcess();
	FuelLevelLightBarProcess();
}


static void Bar_PowerOn_Self_Test_Action(void)
{
	uint8 fl_bar_num = 0;
	
	if(l_bar_selfTest_flag == TRUE)
	{	
		l_CheckSelfStatus = 0;
		l_ProveoutCounter_U8++;
		
		if(l_ProveoutCounter_U8 <= BAR_MAX)
		{
			fl_bar_num = l_ProveoutCounter_U8;
		}
		else if(l_ProveoutCounter_U8 <= BAR_AWAIT_MAX)
		{
			fl_bar_num = BAR_MAX;
		}
		else
		{
		  
			if(l_ProveoutCounter_U8 >= PROVEOUT_MAX)
			{
				l_ProveoutCounter_U8 = PROVEOUT_MAX;
			}
			
			fl_bar_num = PROVEOUT_MAX - l_ProveoutCounter_U8;
			//self test finsihed
			if(fl_bar_num == 0)
			{
				l_bar_selfTest_flag = FALSE;
				l_selfTest_finish = TRUE;
				l_CheckStateFinish_u8 = FALSE;
				l_CheckSelfStatus = TRUE;
			}
		}
		
		// SetCoolantLight(fl_bar_num, BAR_TYPE_COOLANT);
		// SetCoolantLight(fl_bar_num, BAR_TYPE_FUEL_LEVEL);
	}

	Rte_Write_ppCS_BarStatus_barnum(fl_bar_num);
}

static void Bar_PowerOff_light(void)
{
	SetCoolantLight(0, BAR_TYPE_COOLANT);
	SetCoolantLight(0, BAR_TYPE_FUEL_LEVEL);
}

static void Bar_PowerOn_Self_Test(void)
{
	l_bar_selfTest_flag = TRUE; 
	l_ProveoutCounter_U8 = 0;
}


//=====================================================================================================================
//  PUBLIC
//=====================================================================================================================

