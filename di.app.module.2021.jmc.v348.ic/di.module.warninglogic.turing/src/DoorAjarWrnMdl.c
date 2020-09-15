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

File Name        :  DoorOpenChime.c
Module Short Name:  DoorOpenChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define DOOROPENCHIME_C

#include "Rte_DoorAjarWrnMdl.h"

#include "CmpLib.h"
#include "WarningCtrl.h"


static void f_poolingCheck_doorState(void);
static uint8 fdoor_get_door_status_boot (void);
static uint8 fdoor_get_door_status_rearright (void);
static uint8 fdoor_get_door_status_rearleft (void);
static uint8 fdoor_get_door_status_frontright (void);
static uint8 fdoor_get_door_status_enghood (void);
static uint8 fdoor_get_door_status_frontleft (void);
static uint8 Get_AllDoorAjar_St(void);
static void f_DoorAjarWrnInitialState(void);
static void f_doorState_init(void);
static uint8 f_read_LFDoorSwitchSt(void);
static uint8 f_read_RFDoorSwitchSt(void);
static uint8 f_read_LRDoorSwitchSt(void);
static uint8 f_read_RRDoorSwitchSt(void);
static uint8 f_read_TrunkSt(void);
static uint8 f_read_EngHoodUnlockWarming(void);
static void f_DoorAjarWrn_Process(void);
static void f_DiffSigAFTACK_Process(uint8 u8Sig);



CMPLIB_INSTANCE(DoorAjarWrnMdl)

/*****************************************************************************
*                                 Macro Definitions                          *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/
#define DOOR_OPEN             (2)
#define DOOR_CLOSED           (0)

/*                                 Type Declarations                          *
******************************************************************************/
typedef enum
{
	FRONT_LEFT_DOOR = 0,
	FRONT_RIGHT_DOOR,
	REAR_LEFT_DOOR,
	REAR_RIGHT_DOOR,
	TRUNK_DOOR,
	ENGHOOD_DOOR,
	DOOR_NUM
}DOOR_OPEN_ID_T;
	
typedef enum
{
	NO_DOOR = 0,
	LF_DOOR = ((uint8)0x01),
	RF_DOOR = ((uint8)0x02),
	LR_DOOR = ((uint8)0x04),
	RR_DOOR = ((uint8)0x08),
	TK_DOOR = ((uint8)0x10),
	HOOD_DOOR = ((uint8)0x20)
}DOOR_OUTPUT_T;

typedef enum
{
  ENGINE_DOOR_WARNING_DISABLE = 0X0,
  ENGINE_DOOR_WARNING_ENABLE = 0X1,
}ENGINE_DOOR_WARNING_CONFIG_T;
typedef uint8 (* DOOROPEN_FUNC_DEF)(void);
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
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/
static uint8 l_door_open_status_U8[DOOR_NUM] = {0};
static uint8 l_Lastdoor_open_status_U8[DOOR_NUM] = {0};
static uint8 l_Warning_DoorOpenSt_U8 = 0;
static DOOROPEN_FUNC_DEF l_door_openstatus_checkStruct[DOOR_NUM] = 
{
	&f_read_LFDoorSwitchSt,
	&f_read_RFDoorSwitchSt,
	&f_read_LRDoorSwitchSt,
	&f_read_RRDoorSwitchSt,
	&f_read_TrunkSt,
	&f_read_EngHoodUnlockWarming
};


/*****************************************************************************
*                   Functions                                                *
******************************************************************************/


//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_DoorAjarWrnInitialState();
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
  f_DoorAjarWrnInitialState();
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
	f_DoorAjarWrnInitialState();
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
  f_DoorAjarWrnInitialState();
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	//EBatteryState fl_Battery_state = 0;
	
	//Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	
    f_DoorAjarWrn_Process();	
	
	//To be Done
	Rte_Write_ppSR_DoorAjarWrnMdl_GdtCtrl_State_GdtCtrl_State(l_Warning_DoorOpenSt_U8);
	if(l_Warning_DoorOpenSt_U8 == 0)
	{
		Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DOOR_AJAR_IG_ON_ID, UI_WARNING_STATUS_OFF);
		Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DOOR_AJAR_IG_OFF_ID, UI_WARNING_STATUS_OFF);
	}
	else 
	{
		Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DOOR_AJAR_IG_ON_ID, UI_WARNING_STATUS_ON);
		Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DOOR_AJAR_IG_OFF_ID, UI_WARNING_STATUS_ON);
		f_DiffSigAFTACK_Process(l_Warning_DoorOpenSt_U8);
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




static void f_poolingCheck_doorState(void)
{
  uint8 fl_door_index_U8 = 0;
	uint8 fl_doorState_U8 = 0;
	DOOROPEN_FUNC_DEF fl_checkFunc = NULL; 
	//To be Done
	//Rte_Read_rpSR_CANMSG_IBCM2_ComIn_BCM_TrunkSt(&fl_doorState_U8);
  for(fl_door_index_U8 = 0; fl_door_index_U8 < DOOR_NUM; fl_door_index_U8++)
  {
    fl_checkFunc = l_door_openstatus_checkStruct[fl_door_index_U8];
		fl_doorState_U8 = (*fl_checkFunc)();
		if(fl_doorState_U8 == DOOR_OPEN)
		{
			l_door_open_status_U8[fl_door_index_U8] = DOOR_OPEN;
		}
		else 
		{
			l_door_open_status_U8[fl_door_index_U8] = DOOR_CLOSED;
		}
  }	
}


/****************************************************************************
Function Name     : fdoor_get_door_status_boot

Description       : Return the boot door display status to be displayed.

Invocation        : Invoked by display Module.

Parameters        : None.

Return Type       : uint8.

Range/Return value:
                    0 means display 'Closed'.
                    1 means display 'Open'.

Critical Section  : None.
******************************************************************************/
static uint8 fdoor_get_door_status_boot (void)
{
  return(l_door_open_status_U8[TRUNK_DOOR]);
}

/****************************************************************************
Function Name     : fdoor_get_door_status_rearright

Description       : Return the rear right door display status to be displayed.

Invocation        : Invoked by display Module.

Parameters        : None.

Return Type       : uint8.

Range/Return value:
                    0 means display 'Closed'.
                    1 means display 'Open'.

Critical Section  : None.
******************************************************************************/
static uint8 fdoor_get_door_status_rearright (void)
{
  return(l_door_open_status_U8[REAR_RIGHT_DOOR]);
}

/****************************************************************************
Function Name     : fdoor_get_door_status_rearleft

Description       : Return the rear left door display status to be displayed.

Invocation        : Invoked by display Module.

Parameters        : None.

Return Type       : uint8.

Range/Return value:
                    0 means display 'Closed'.
                    1 means display 'Open'.

Critical Section  : None.
******************************************************************************/
static uint8 fdoor_get_door_status_rearleft (void)
{
  return(l_door_open_status_U8[REAR_LEFT_DOOR]);
}

/****************************************************************************
Function Name     : fdoor_get_door_status_frontright

Description       : Return the front right door display status to be displayed.

Invocation        : Invoked by display Module.

Parameters        : None.

Return Type       : uint8.

Range/Return value:
                    0 means display 'Closed'.
                    1 means display 'Open'.

Critical Section  : None.
******************************************************************************/
static uint8 fdoor_get_door_status_frontright (void)
{
  return(l_door_open_status_U8[FRONT_RIGHT_DOOR]);
}


/****************************************************************************
Function Name     : fdoor_get_door_status_frontleft

Description       : Return the front left door display status to be displayed.

Invocation        : Invoked by display Module.

Parameters        : None.

Return Type       : uint8.

Range/Return value:
                    0 means display 'Closed'.
                    1 means display 'Open'.

Critical Section  : None.
******************************************************************************/
static uint8 fdoor_get_door_status_frontleft (void)
{
  return(l_door_open_status_U8[FRONT_LEFT_DOOR]);
}


static uint8 fdoor_get_door_status_enghood (void)
{
  return(l_door_open_status_U8[ENGHOOD_DOOR]);
}

static uint8 Get_AllDoorAjar_St(void)
{
    uint8 fl_AllDoorAjar_St_U8;
    
    fl_AllDoorAjar_St_U8 = NO_DOOR;
    
    if(DOOR_OPEN == fdoor_get_door_status_frontleft())
    {
        fl_AllDoorAjar_St_U8 |= LF_DOOR;
    }
    else
    {
        /*do nothing*/
    }

    if(DOOR_OPEN == fdoor_get_door_status_frontright())
    {
        fl_AllDoorAjar_St_U8 |= RF_DOOR;
    }
    else
    {
        /*do nothing*/
    }
    
    if(DOOR_OPEN == fdoor_get_door_status_rearleft())
    {
        fl_AllDoorAjar_St_U8 |= LR_DOOR;
    }
    else
    {
        /*do nothing*/
    }

    if(DOOR_OPEN == fdoor_get_door_status_rearright())
    {
        fl_AllDoorAjar_St_U8 |= RR_DOOR;
    }
    else
    {
        /*do nothing*/
    }

    if(DOOR_OPEN == fdoor_get_door_status_boot())
    {
        fl_AllDoorAjar_St_U8 |= TK_DOOR;
    }
    else
    {
        /*do nothing*/
    }
	
    if(DOOR_OPEN == fdoor_get_door_status_enghood())
    {
        fl_AllDoorAjar_St_U8 |= HOOD_DOOR;
    }
    else
    {
        /*do nothing*/
    }

    return fl_AllDoorAjar_St_U8;
}


static void f_DoorAjarWrnInitialState(void)
{
    f_doorState_init();
	l_Warning_DoorOpenSt_U8 = 0;
}



static void f_DoorAjarWrn_Process(void)
{
	uint8 	fl_CAN_BCM_0x310_nvr_status_U8 = 0;
	uint8 	fl_CAN_BCM_0x310_msg_status_U8 = 0;
	uint8 	fl_AllDoorOpenSt = 0;

	//To be Done
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&fl_CAN_BCM_0x310_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&fl_CAN_BCM_0x310_msg_status_U8);

	if(fl_CAN_BCM_0x310_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_BCM_0x310_msg_status_U8 == RTE_E_TIMEOUT)
	{
		fl_AllDoorOpenSt = 0;
	}
	else 
	{
		f_poolingCheck_doorState();
		fl_AllDoorOpenSt = Get_AllDoorAjar_St();
	}
	l_Warning_DoorOpenSt_U8 = fl_AllDoorOpenSt;
	
}

static void f_DiffSigAFTACK_Process(uint8 u8Sig)
{
  static uint8 u8PrvSig = 0;
  uint8 u8Notify = 0;
  uint8 IGNSts = eIGN_OFF;
  WARN_ID_INDEX WrnID = WARN_MAX_ID;
  uint8 DoorNumIndex = 0;

  Rte_Read_DoorAjarWrnMdl_rpIgnState_IGNState(&IGNSts);

  if(IGNSts == eIGN_RUN)
  {
    WrnID = WARN_DOOR_AJAR_IG_ON_ID;
  }
  else
  {
    WrnID = WARN_DOOR_AJAR_IG_OFF_ID;
  }

  u8Notify = CWarningCtrl_APP_get_notified_warning_status(WrnID);

  for(DoorNumIndex = 0; DoorNumIndex < DOOR_NUM; DoorNumIndex++)
  {
    if(2 != l_Lastdoor_open_status_U8[DoorNumIndex] && 2 == l_door_open_status_U8[DoorNumIndex]) 
    {
      if(u8PrvSig != u8Sig)
      {
        if((UI_WARNING_STATE_TIMEOUT == u8Notify) || (UI_WARNING_STATE_ACKNOWLEDED == u8Notify))
        {
          Rte_Call_rpCS_WarningCtrl_Set_Operation(WrnID, UI_WARNING_CONDITION_FORCED);
        }
        else
        {
          //Rte_Call_rpCS_WarningCtrl_Set_Operation(WrnID, UI_WARNING_STATUS_RETRIG);
        }
      }
    }

    l_Lastdoor_open_status_U8[DoorNumIndex] = l_door_open_status_U8[DoorNumIndex];
  }

  u8PrvSig = u8Sig;
}



static void f_doorState_init(void)
{
    uint8 fl_index_U8;
    for(fl_index_U8 = 0; fl_index_U8 < DOOR_NUM; fl_index_U8++)
    {
        l_door_open_status_U8[fl_index_U8] = DOOR_CLOSED;
        l_Lastdoor_open_status_U8[fl_index_U8] = DOOR_CLOSED;
    }
}


static uint8 f_read_LFDoorSwitchSt(void)
{
	uint8 fl_read_state_U8 = 0;;
	//To be done
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&fl_read_state_U8);
	return fl_read_state_U8;
}
static uint8 f_read_RFDoorSwitchSt(void)
{
	uint8 fl_read_state_U8 = 0;
	//To be done
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi(&fl_read_state_U8);
	return fl_read_state_U8;
}
static uint8 f_read_LRDoorSwitchSt(void)
{
	uint8 fl_read_state_U8 = 0;
	//To be done
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearLe(&fl_read_state_U8);
	return fl_read_state_U8;
}
static uint8 f_read_RRDoorSwitchSt(void)
{
	uint8 fl_read_state_U8 = 0;
	//To be done
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearRi(&fl_read_state_U8);
	return fl_read_state_U8;
}
static uint8 f_read_TrunkSt(void)
{
	uint8 fl_read_state_U8 = 0;
	//To be done
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsTrunk(&fl_read_state_U8);
	return fl_read_state_U8;
}
static uint8 f_read_EngHoodUnlockWarming(void)
{
	uint8 fl_read_state_U8 = 0;
  uint8 DID_EngineWarningConfig = 0;
  Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ENGINEGATE_WARNING_ENABLE,&DID_EngineWarningConfig); 
	//To be done
	if(ENGINE_DOOR_WARNING_DISABLE != DID_EngineWarningConfig)
	{
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_EnginehoodSts(&fl_read_state_U8);
	}
  else
  {
    ;  // Do nothing
  }
	return fl_read_state_U8;
}

/*End of File*/
/*****************************************************************************
*   for each change to this file, be sure to record:                         *
*      1.  who made the change and when the change was made                  *
*      2.  why the change was made and the intended result                   *
*   Following block needs to be repeated for each change
******************************************************************************
*   Note: In the traceability column we need to trace back to the Design Doc.*
*   For the initial version it is traced to the Design Document section.     *
*   For further changes it shall trace to the source of the change which may *
*   be SPSS/SCR/Defect details(Defect may be Testing/validation defect)/Any  *
*   other reason                                                            *
******************************************************************************/
