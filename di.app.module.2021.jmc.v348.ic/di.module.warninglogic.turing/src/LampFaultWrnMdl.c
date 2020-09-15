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
#include "Rte_LampFaultWrnMdl.h"
#include "CmpLib.h"
#include "WarningCtrl.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(LampFaultWrnMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
typedef enum BCM_IMMOSTS_SIGVAL
{
    IMMOSTS_INACTIVE,
    IMMOSTS_TP_MATCH_FAILURE,
    IMMOSTS_EMS_MATCH_FAILURE,
    IMMOSTS_MATCH_SUCCESS
} BCM_IMMOSTS_SIGVAL_E;

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */
#define cWARNING_ACTIVE   (uint8)(1)

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================


//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 message_318_invalid(void);
static uint8 message_invalid2(void);
static void Lamp_Warning_init(void);
static void fun_Lamp_Warning_Process(void);
static void LeftLightOnProcess(void);
static void IMMOProcess(void);
// static uint8 GlowPlugLmpReq_last_status = 0;
static uint8 l_lastIMMOSts_U8 = IMMOSTS_INACTIVE;
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
    // GlowPlugLmpReq_last_status = 0;
    Lamp_Warning_init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit(void)
{
    // GlowPlugLmpReq_last_status = 0;
    Lamp_Warning_init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation(void)
{
    Lamp_Warning_init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation(void)
{
    Lamp_Warning_init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
/****************************************************************************
Function Name       : CmpActive

Description         : This function process the warning for Security Authentication Failed.

Invocation          : called in cmpActive.

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/

static Std_ReturnType CmpActive(void)
{
    if ((uint8)FALSE == message_318_invalid())
    {
        fun_Lamp_Warning_Process();
        IMMOProcess();
    }
    else
    {
        Lamp_Warning_init();
    }

    if (FALSE == message_318_invalid() && FALSE == message_invalid2())
    {
        LeftLightOnProcess();
    }
    else
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LEFT_LIGHT_ON_WARNING_ID, UI_WARNING_STATUS_OFF);
    }

    return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief  Diagnostic state
///
/// @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag(void)
{
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn(void)
{
    return E_OK;
}


static uint8 message_318_invalid(void)
{
    uint8 fl_Msg318_NR_U8       = (uint8)0;
    uint8 fl_Msg318_Tout_U8     = (uint8)0;

    Rte_Read_LampFaultWrnMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(&fl_Msg318_NR_U8);
    Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(&fl_Msg318_Tout_U8);

    if (((uint8)RTE_E_TIMEOUT == fl_Msg318_Tout_U8) || ((uint8)RTE_E_NEVER_RECEIVED == fl_Msg318_NR_U8))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


static uint8 message_invalid2(void)
{
    uint8 fl_Msg318_NR_U8;
    uint8 fl_Msg318_Tout_U8;
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&fl_Msg318_NR_U8);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&fl_Msg318_Tout_U8);

    if ((RTE_E_TIMEOUT == fl_Msg318_Tout_U8) || (RTE_E_NEVER_RECEIVED == fl_Msg318_NR_U8))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


static void Lamp_Warning_init(void)
{

    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BEAM_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_POS_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FRNT_FOG_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_RAER_FOG_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TURN_IND_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_REVERSE_LIGHT_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BRK_LIGHT_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DAY_RUN_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LEFT_LIGHT_ON_WARNING_ID, UI_WARNING_STATUS_OFF);
    Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BCM_IMMO_MATCH_STATUS_ID, UI_WARNING_STATUS_OFF);
    l_lastIMMOSts_U8 = IMMOSTS_INACTIVE;
}

static void fun_Lamp_Warning_Process(void)
{
    //uint8 fl_LowBeam_Status_U8 = 0;
    uint8 fl_PosLmp_Status_U8            = (uint8)0;

    uint8 fl_RearFog_Status_U8           = (uint8)0;
    uint8 fl_TurnIndicator_Status_U8     = (uint8)0;
    uint8 fl_BrakeLight_Status_U8        = (uint8)0;
    uint8 fl_ReverLight_Status_U8        = (uint8)0;
    uint8 fl_DayRun_Status_U8            = (uint8)0;
    //uint8 fl_FrontFog_Status_U8 = 0;

    //Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_LowBeamDig(&fl_LowBeam_Status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpDig(&fl_PosLmp_Status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_RearFogLmpDig(&fl_RearFog_Status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_TurnIndicatorDig(&fl_TurnIndicator_Status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_BrakelightDig(&fl_BrakeLight_Status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_reversinglightDig(&fl_ReverLight_Status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_DayRunningLampDig(&fl_DayRun_Status_U8);
    //Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_FrntFogLmpDig(&fl_FrontFog_Status_U8);

    /*if (cWARNING_ACTIVE == fl_LowBeam_Status_U8)
    {
      Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BEAM_FAULT_WARNING_ID, UI_WARNING_STATUS_ON);
    }
    else
    {
      Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LOW_BEAM_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    }*/

    if (cWARNING_ACTIVE == fl_PosLmp_Status_U8)
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_POS_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_ON);
    }
    else
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_POS_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    }

    /*if (cWARNING_ACTIVE == fl_FrontFog_Status_U8)
    {
      Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FRNT_FOG_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_ON);
    }
    else
    {
      Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_FRNT_FOG_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    }*/

    if (cWARNING_ACTIVE == fl_RearFog_Status_U8)
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_RAER_FOG_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_ON);
    }
    else
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_RAER_FOG_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    }

    if (cWARNING_ACTIVE == fl_TurnIndicator_Status_U8)
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TURN_IND_FAULT_WARNING_ID, UI_WARNING_STATUS_ON);
    }
    else
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_TURN_IND_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    }

    if (cWARNING_ACTIVE == fl_ReverLight_Status_U8)
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_REVERSE_LIGHT_FAULT_WARNING_ID, UI_WARNING_STATUS_ON);
    }
    else
    {

        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_REVERSE_LIGHT_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    }

    if (cWARNING_ACTIVE == fl_BrakeLight_Status_U8)
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BRK_LIGHT_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_ON);
    }
    else
    {

        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BRK_LIGHT_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    }


    if (cWARNING_ACTIVE == fl_DayRun_Status_U8)
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DAY_RUN_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_ON);

    }
    else
    {

        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_DAY_RUN_LMP_FAULT_WARNING_ID, UI_WARNING_STATUS_OFF);
    }

}


static void LeftLightOnProcess(void)
{
    uint8 DoorStsFrntLe_u8 = 0;
    uint8 BackHomeSts_u8 = 0;
    uint8 PosLmpSts_u8 = 0;

    Rte_Read_LampFaultWrnMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_PosLmpSts(&PosLmpSts_u8);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&DoorStsFrntLe_u8);

    if (2 == PosLmpSts_u8 && 2 == DoorStsFrntLe_u8)
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LEFT_LIGHT_ON_WARNING_ID, UI_WARNING_STATUS_ON);
    }
    else
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_LEFT_LIGHT_ON_WARNING_ID, UI_WARNING_STATUS_OFF);
    }
}

static void IMMOProcess(void)
{
    uint8 ImmoSts = (uint8)IMMOSTS_INACTIVE;
    uint8 NotifySts = UI_WARNING_STATE_NONE;
    Rte_Read_LampFaultWrnMdl_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_ImmoSts(&ImmoSts);
    NotifySts = CWarningCtrl_APP_get_notified_warning_status(WARN_BCM_IMMO_MATCH_STATUS_ID);
    
    /*TRRIGGER IMMO*/  
    if (((uint8)IMMOSTS_TP_MATCH_FAILURE == ImmoSts) || \
            ((uint8)IMMOSTS_EMS_MATCH_FAILURE == ImmoSts))
    {

        if ( ((uint8)UI_WARNING_STATE_SHOW == NotifySts) && (l_lastIMMOSts_U8 != ImmoSts) )
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BCM_IMMO_MATCH_STATUS_ID, UI_WARNING_STATUS_RETRIG);
        }
        else if ( (((uint8)UI_WARNING_STATE_TIMEOUT == NotifySts) ||\
                  ((uint8)UI_WARNING_STATE_ACKNOWLEDED == NotifySts)) && (l_lastIMMOSts_U8 != ImmoSts) )
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BCM_IMMO_MATCH_STATUS_ID, UI_WARNING_CONDITION_FORCED);
        }
        else
        {
            Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BCM_IMMO_MATCH_STATUS_ID, UI_WARNING_STATUS_ON);
        }
    }
    else
    {
        Rte_Call_rpCS_WarningCtrl_Set_Operation(WARN_BCM_IMMO_MATCH_STATUS_ID, UI_WARNING_STATUS_OFF);
    }

    l_lastIMMOSts_U8 = ImmoSts;
}

