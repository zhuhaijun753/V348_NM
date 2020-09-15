//---------------------------------------------------------------------------------------------------------------------
//
// VISTEON CORPORATION CONFIDENTIAL																																										                                                                                                                           // ________________________________
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
#include "Rte_BtnMdl.h"
//#include "Rte_BtnMdl_Type.h"
#include "BtnMdl.h"
#include <CmpLib.h>

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(BtnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#define BIT(b) (1 << (b))

//======================================================================================================================
// FORWARD DECLARATIONS OF PRIVATE FUNCTIONS
//======================================================================================================================
static void Btn_Runnable(void);
static void Btn_TransDataToHmi(void);
static void Btn_TransIntoBatUderVAbnormal(void);
//static void Btn_TransIntoBatOverVAbnormal(void);
static void Btn_TransIntoIgnAbnormal(void);
static void Btn_Process(void);

static boolean vnim_is_0x31A_missing(void);
static boolean vnim_is_0x31A_never_recvd(void);
static boolean vnim_0x31A_missing_state(void);




//======================================================================================================================
// PRIVATE DATA
//======================================================================================================================
static uint8 l_menu_button = 0;




static void Btn_Runnable(void)
{
    uint8 fl_ign_state_U8;
	uint8 fl_batt_state_U8;
	
	Rte_Read_rpBattState_BatteryState(&fl_batt_state_U8);	
	Rte_Read_rpIgnState_IGNState(&fl_ign_state_U8);

	if(fl_ign_state_U8 == eIGN_OFF)
	{
		Btn_TransIntoIgnAbnormal();
	}
	else //IGN ON
	{
		if((fl_batt_state_U8 != eBatteryState_Normal) && (fl_batt_state_U8 != eBatteryState_OverVoltagePhase1))
		{
			Btn_TransIntoBatUderVAbnormal();
		}
		else 
		{
			Btn_Process();
	    }
	}
    
	Btn_TransDataToHmi();
}

static void Btn_TransDataToHmi(void)
{
	Rte_Write_ppCS_BtnMdl_MenuButtonValue(l_menu_button);	
}

static void Btn_TransIntoBatUderVAbnormal(void)
{
	l_menu_button = NO_BUTTON_PRESSED;
}

static void Btn_TransIntoIgnAbnormal(void)
{
	l_menu_button = NO_BUTTON_PRESSED;
}

static boolean vnim_is_0x31A_missing(void)
{
	uint8 fl_can_msgTout_U8 = 0;
	boolean fl_ret_value_BOOL = FALSE;

	Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_Tout(&fl_can_msgTout_U8);
	
    if( RTE_E_TIMEOUT == fl_can_msgTout_U8 )
	{
        fl_ret_value_BOOL = TRUE;
    }
    else
    {
        fl_ret_value_BOOL = FALSE;
    }
    return fl_ret_value_BOOL;

		
}

static boolean vnim_is_0x31A_never_recvd(void)
{
	uint8 fl_can_msgNr_U8 = 0;
	boolean fl_ret_value_BOOL = FALSE;
	
	Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_NR(&fl_can_msgNr_U8);

    if( RTE_E_NEVER_RECEIVED == fl_can_msgNr_U8 )
	{
        fl_ret_value_BOOL = TRUE;
    }
    else
    {
        fl_ret_value_BOOL = FALSE;
    }
    return fl_ret_value_BOOL;
}

static boolean vnim_0x31A_missing_state(void)
{
    boolean fl_0x31A_missing_state = FALSE;
    if((TRUE == vnim_is_0x31A_never_recvd()) 
       || (TRUE == vnim_is_0x31A_missing()) )
    {
        fl_0x31A_missing_state = TRUE;
    }
    else
    {
        fl_0x31A_missing_state = FALSE;
    }
    return fl_0x31A_missing_state;
}

static void Btn_Process(void)
{
	uint8 fl_menuButton_value_u8 = 0;

	if(TRUE == vnim_0x31A_missing_state()) //sig Missing
    {
     	l_menu_button = NO_BUTTON_PRESSED;
    }
    else  //No Sig Missing                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
    {
		//menu up
		
		Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Up(&fl_menuButton_value_u8);
		if(fl_menuButton_value_u8 == BUTTON_PRESSED)
			l_menu_button |= BIT(BTN_MENU_UP_BIT);
		else 
			l_menu_button &= ~BIT(BTN_MENU_UP_BIT);

		//menu down
		Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Down(&fl_menuButton_value_u8);
		if(fl_menuButton_value_u8 == BUTTON_PRESSED)
			l_menu_button |= BIT(BTN_MENU_DOWN_BIT);
		else 
			l_menu_button &= ~BIT(BTN_MENU_DOWN_BIT);

		//menu left
		Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Left(&fl_menuButton_value_u8);
		if(fl_menuButton_value_u8 == BUTTON_PRESSED)
			l_menu_button |= BIT(BTN_MENU_LEFT_BIT);
		else 
			l_menu_button &= ~BIT(BTN_MENU_LEFT_BIT);

		//menu right
		Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Right(&fl_menuButton_value_u8);
		if(fl_menuButton_value_u8 == BUTTON_PRESSED)
			l_menu_button |= BIT(BTN_MENU_RIGHT_BIT);
		else 
			l_menu_button &= ~BIT(BTN_MENU_RIGHT_BIT);

		//menu confirm
		Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Confirm(&fl_menuButton_value_u8);
		if(fl_menuButton_value_u8 == BUTTON_PRESSED)
			l_menu_button |= BIT(BTN_MENU_CONFIRM_BIT);
		else 
			l_menu_button &= ~BIT(BTN_MENU_CONFIRM_BIT);

		//menu return
		Rte_Read_rpSR_CANMSG_GW_SWM_Body_0x31A_ComIn_SWM_Menu_Return(&fl_menuButton_value_u8);
		if(fl_menuButton_value_u8 == BUTTON_PRESSED)
			l_menu_button |= BIT(BTN_MENU_RETURN_BIT);
		else 
			l_menu_button &= ~BIT(BTN_MENU_RETURN_BIT);

    }
    
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	l_menu_button = 0;	
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
	l_menu_button = 0;

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
    return  E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive(void)
{

	Btn_Runnable();

    return  E_OK;
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


/*---------------------------------------------------------------------------
Date              : 02-Nov-17
CDSID             : psagathe
Traceability      : 986532
Change Description: Trip2 sw adc count change.
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Date              : 09-Nov-17
CDSID             : gkumarag
Traceability      : 988279
Change Description: Button response issue - Debounce logic added
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Date              : 12-Dec-17
CDSID             : vkrishn6
Traceability      : 1009803
Change Description: Block of TripSW1 buttons and drive mode button during IGN OFF
-----------------------------------------------------------------------------*/


