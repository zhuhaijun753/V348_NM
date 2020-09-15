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

#include "Rte_TCUWrnMdl.h"
#include "CmpLib.h"
#include "WarningCtrl.h"
#include <string.h>

/// @brief    Instance of life-cycle state-machine
CMPLIB_INSTANCE(TCUWrnMdl)

//=====================================================================================================================
//    CONSTANTS & TYPES
//=====================================================================================================================
#define cTCU_ERROR                    (uint8)(0x1)
#define cTCU_BRAKE_APPLY              (uint8)(0x1)
#define cTCU_BRAKE_AND_DECL           (uint8)(0x2)
#define cTCU_VEL_HIGH                 (uint8)(0x3)
#define cTCU_WRONG_GEAR               (uint8)(0x4)
#define cTCU_UNLOCK_CHANGE_GEAR       (uint8)(0x5)
#define cTCU_UNLOCK_AND_BRAKE         (uint8)(0x6)
#define cTCU_WARNING_TOTAL            (uint8)(0x7)
#define cTCU_ENABLE                   (uint8)(0x2)

typedef enum
{
    WARN_TCU_GEARBOX_FAULT                       ,
    WARN_TCU_BRAKE_APPLY                         ,
    WARN_TCU_BRAKE_AND_DECL                      ,
    WARN_TCU_VEL_HIGH                            ,
    WARN_TCU_WRONG_GEAR                          ,
    WARN_TCU_UNLOCK_CHANGE_GEAR                  ,
    WARN_TCU_UNLOCK_AND_BRAKE                    ,
    TCU_WARNING_TOTAL
}WARN_TCU_T;

static uint8 TCUWarningStatus[TCU_WARNING_TOTAL] = { 0 };
static uint8 TCUWarningIdArray[TCU_WARNING_TOTAL] =
{
    WARN_TCU_GEARBOX_FAULT_ID                    ,
    WARN_TCU_BRAKE_APPLY_ID                      ,
    WARN_TCU_BRAKE_AND_DECELERATION_ID           ,
    WARN_TCU_VELOCITY_TOO_HIGH_ID                ,
    WARN_TCU_WRONG_GEAR_ID                       ,
    WARN_TCU_UNLOCK_TO_CHANGE_GEAR_ID            ,
    WARN_TCU_UNLOCK_AND_BRAKE_TO_CHANGE_GEAR_ID  
};

#if 0
#define MSG_ERROR              ((uint8)0x1)
#define MSG_NO_ACTIVE          ((uint8)0x0)
#define UI_TCU_WARNING_TOTAL     8U
typedef enum
{
    WARN_TCU_GEARBOXP_FAULTY = 0,
    WARN_TCU_GEARBOX_FAULTYD,
    WARN_TCU_GEARBOX_FAULTYP,
    WARN_TCU_GEARBOX_TRANSMISSIONSIGNALS,
    WARN_TCU_GEARBOX,
    WARN_TCU_UNKNOWN_D_P,
    WARN_TCU_DRIVERWARNING,
    WARN_TRANS_OVER_HEAT
}UI_WARNING_TCU_T;

static uint8 TCUWarningStatus[UI_TCU_WARNING_TOTAL] = { 0 };

static uint8 TCUWarningId[UI_TCU_WARNING_TOTAL] =
{
    WARN_TCU_GEARBOXP_FAULTY_ID				,
    WARN_TCU_GEARBOX_FAULTYD_ID				,
    WARN_TCU_GEARBOX_FAULTYP_ID				,
    WARN_TCU_GEARBOX_TRANSMISSIONSIGNALS_ID	,
    WARN_TCU_GEARBOX_ID						,
    WARN_TCU_UNKNOWN_D_P_ID					,
    WARN_TCU_DRIVERWARNING_ID				,
    WARN_TCU_TRANS_OVER_HEAT_ID
};
#endif

//=====================================================================================================================
//    FORWARD DECLARATIONS
//=====================================================================================================================


//=====================================================================================================================
//    PRIVATE
//=====================================================================================================================
#if 0
static void fun_TCU_GearboxP_faulty_Process(void);
static void fun_TCU_Gearbox_faultyD_Process(void);
static void fun_TCU_Gearbox_faultyP_Process(void);
static void fun_TCU_Gearbox_Transmission_Process(void);
static void fun_TCU_Gearbox_Process(void);
static void fun_TCU_driver_warning_Process(void);
static void fun_TCU_trans_OverHeat_Process(void);
static void TCU_Warning_init(void);
static uint8 message_invalid(void);
#endif

static void TCU_Warning_init(void);
static void fun_TCU_Warning_Process(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional initialization state
///
/// @return     E_OK:               Succeed
///<br>         E_NOT_READY:        In progress. Keeps the component in this state.
///<br>         E_NOT_OK:           Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit(void)
{
    /*memset(TCUWarningStatus, 0, sizeof(TCUWarningStatus));
        
    uint8 TempWrnArray[]=
    {
         WARN_TCU_GEARBOXP_FAULTY_ID				,
         WARN_TCU_GEARBOX_FAULTYD_ID				,
         WARN_TCU_GEARBOX_FAULTYP_ID				,
         WARN_TCU_GEARBOX_TRANSMISSIONSIGNALS_ID	,
         WARN_TCU_GEARBOX_ID						,
         WARN_TCU_UNKNOWN_D_P_ID					,
         WARN_TCU_DRIVERWARNING_ID				,
         WARN_TCU_TRANS_OVER_HEAT_ID
    };
    memcpy(TCUWarningId, TempWrnArray, sizeof(TCUWarningStatus));*/

        
    TCU_Warning_init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional de-initialization state
///
/// @return E_OK:                     Succeed
///<br>         E_NOT_READY:        In progress. Keeps the component in this state.
///<br>         E_NOT_OK:             Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit(void)
{
    /*memset(TCUWarningStatus, 0, sizeof(TCUWarningStatus));
    
    uint8 TempWrnArray[]=
    {
        WARN_TCU_GEARBOXP_FAULTY_ID				,
        WARN_TCU_GEARBOX_FAULTYD_ID				,
        WARN_TCU_GEARBOX_FAULTYP_ID				,
        WARN_TCU_GEARBOX_TRANSMISSIONSIGNALS_ID	,
        WARN_TCU_GEARBOX_ID						,
        WARN_TCU_UNKNOWN_D_P_ID					,
        WARN_TCU_DRIVERWARNING_ID				,
        WARN_TCU_TRANS_OVER_HEAT_ID
    };
    memcpy(TCUWarningId, TempWrnArray, sizeof(TCUWarningStatus));*/
    
    TCU_Warning_init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional activation state
///
/// @return E_OK:                     Succeed
///<br>         E_NOT_READY:        In progress. Keeps the component in this state.
///<br>         E_NOT_OK:             Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation(void)
{
    /*memset(TCUWarningStatus, 0, sizeof(TCUWarningStatus));
    
    uint8 TempWrnArray[]=
    {
        WARN_TCU_GEARBOXP_FAULTY_ID				,
        WARN_TCU_GEARBOX_FAULTYD_ID				,
        WARN_TCU_GEARBOX_FAULTYP_ID				,
        WARN_TCU_GEARBOX_TRANSMISSIONSIGNALS_ID	,
        WARN_TCU_GEARBOX_ID						,
        WARN_TCU_UNKNOWN_D_P_ID					,
        WARN_TCU_DRIVERWARNING_ID				,
        WARN_TCU_TRANS_OVER_HEAT_ID
    };
    memcpy(TCUWarningId, TempWrnArray, sizeof(TCUWarningStatus));
    */
    
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional de-activation state
///
/// @return E_OK:                     Succeed
///<br>         E_NOT_READY:        In progress
///<br>         E_NOT_OK:             Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation(void)
{
    /*memset(TCUWarningStatus, 0, sizeof(TCUWarningStatus));
    
    uint8 TempWrnArray[]=
    {
        WARN_TCU_GEARBOXP_FAULTY_ID				,
        WARN_TCU_GEARBOX_FAULTYD_ID				,
        WARN_TCU_GEARBOX_FAULTYP_ID				,
        WARN_TCU_GEARBOX_TRANSMISSIONSIGNALS_ID	,
        WARN_TCU_GEARBOX_ID						,
        WARN_TCU_UNKNOWN_D_P_ID					,
        WARN_TCU_DRIVERWARNING_ID				,
        WARN_TCU_TRANS_OVER_HEAT_ID
    };
    memcpy(TCUWarningId, TempWrnArray, sizeof(TCUWarningStatus));*/
    
    TCU_Warning_init();
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief    State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
/****************************************************************************
Function Name         	: CmpActive

Description             : This function process the warning for Security Authentication Failed.

Invocation              : called in cmpActive.

Parameters              : None

Return Value            : None

Critical Section        : None

******************************************************************************/
static Std_ReturnType CmpActive(void)
{
    /*
    eIgnState IGN_state = eIGN_OFF;
    Rte_Read_rpIgnState_IGNState(&IGN_state);

    if (FALSE == message_invalid())
    {
        if (eIGN_RUN == IGN_state)
        {
            fun_TCU_GearboxP_faulty_Process();
            fun_TCU_Gearbox_faultyD_Process();
            fun_TCU_Gearbox_faultyP_Process();
            fun_TCU_Gearbox_Transmission_Process();
            fun_TCU_Gearbox_Process();
            //fun_TCU_unknow_DP_Process(); 
            fun_TCU_driver_warning_Process();
            fun_TCU_trans_OverHeat_Process();
        }
        else
        {
            TCU_Warning_init();
        }

    }
    else
    {
        TCU_Warning_init();
    }

    for (int i = 0; i < UI_TCU_WARNING_TOTAL; i++)
    {
        Rte_Call_tiCS_WarningCtrl_Set_Operation(TCUWarningId[i], TCUWarningStatus[i]);
    }*/

    fun_TCU_Warning_Process();
    
    return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief    Diagnostic state
///
/// @return E_OK:                     Diagnostic has completed. Go to DiagReturn state.
///<br>         E_NOT_READY:        Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag(void)
{
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:                     Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>         E_NOT_READY:        Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn(void)
{
    return E_OK;
}

void TCU_Warning_init(void)
{
    for(uint8 i = 0;i < TCU_WARNING_TOTAL;i ++)
    {
        Rte_Call_tiCS_WarningCtrl_Set_Operation(TCUWarningIdArray[i],UI_WARNING_STATUS_OFF);
        TCUWarningStatus[i] = UI_WARNING_STATUS_OFF;
    }
}

void fun_TCU_Warning_Process(void)
{
    uint8 fl_Msg395_NR_U8                     = (uint8)0;
    uint8 fl_Msg395_Tout_U8                   = (uint8)0;
    uint8 fl_TmSrvcRqd_B_Rq_U8                = (uint8)0;
    uint8 fl_TCU_DrvShiftAllwCondRmd_U8       = (uint8)0;

    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_NR(&fl_Msg395_NR_U8);
    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_Tout(&fl_Msg395_Tout_U8);
    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TrnSrvcRqd_B_Rq(&fl_TmSrvcRqd_B_Rq_U8);
    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_DrvShiftAllwCondRmd(&fl_TCU_DrvShiftAllwCondRmd_U8);

    if ( ((uint8)RTE_E_TIMEOUT == fl_Msg395_Tout_U8) || ((uint8)RTE_E_NEVER_RECEIVED == fl_Msg395_NR_U8) )
    {
        TCU_Warning_init();
    }
    else
    {
        if (cTCU_ERROR == fl_TmSrvcRqd_B_Rq_U8)
        {
            TCUWarningStatus[WARN_TCU_GEARBOX_FAULT] = UI_WARNING_STATUS_ON;
        }
        else
        {
            TCUWarningStatus[WARN_TCU_GEARBOX_FAULT] = UI_WARNING_STATUS_OFF;
        }
        
        if (cTCU_BRAKE_APPLY == fl_TCU_DrvShiftAllwCondRmd_U8)
        {
            TCUWarningStatus[WARN_TCU_BRAKE_APPLY] = UI_WARNING_STATUS_ON;
        }
        else
        {
            TCUWarningStatus[WARN_TCU_BRAKE_APPLY] = UI_WARNING_STATUS_OFF;
        }

        if (cTCU_BRAKE_AND_DECL == fl_TCU_DrvShiftAllwCondRmd_U8)
        {
            TCUWarningStatus[WARN_TCU_BRAKE_AND_DECL] = UI_WARNING_STATUS_ON;
        }
        else
        {
            TCUWarningStatus[WARN_TCU_BRAKE_AND_DECL] = UI_WARNING_STATUS_OFF;
        }
        
        if (cTCU_VEL_HIGH == fl_TCU_DrvShiftAllwCondRmd_U8)
        {
            TCUWarningStatus[WARN_TCU_VEL_HIGH] = UI_WARNING_STATUS_ON;
        }
        else
        {
            TCUWarningStatus[WARN_TCU_VEL_HIGH] = UI_WARNING_STATUS_OFF;
        }

        if (cTCU_WRONG_GEAR == fl_TCU_DrvShiftAllwCondRmd_U8)
        {
            TCUWarningStatus[WARN_TCU_WRONG_GEAR] = UI_WARNING_STATUS_ON;
        }
        else
        {
            TCUWarningStatus[WARN_TCU_WRONG_GEAR] = UI_WARNING_STATUS_OFF;
        }

        if (cTCU_UNLOCK_CHANGE_GEAR == fl_TCU_DrvShiftAllwCondRmd_U8)
        {
            TCUWarningStatus[WARN_TCU_UNLOCK_CHANGE_GEAR] = UI_WARNING_STATUS_ON;
        }
        else
        {
            TCUWarningStatus[WARN_TCU_UNLOCK_CHANGE_GEAR] = UI_WARNING_STATUS_OFF;
        }

        if (cTCU_UNLOCK_AND_BRAKE == fl_TCU_DrvShiftAllwCondRmd_U8)
        {
            TCUWarningStatus[WARN_TCU_UNLOCK_AND_BRAKE] = UI_WARNING_STATUS_ON;
        }
        else
        {
            TCUWarningStatus[WARN_TCU_UNLOCK_AND_BRAKE] = UI_WARNING_STATUS_OFF;
        }


        for(uint8 i = 0;i < TCU_WARNING_TOTAL;i ++)
        {
            Rte_Call_tiCS_WarningCtrl_Set_Operation(TCUWarningIdArray[i],TCUWarningStatus[i]);
        }
    }
    
}





/*********************************************************************************

    Function Name	    : 

    Description 	    : 

    Parameters		    : None

    Return Value	    : None

    Critical Section    : None

***********************************************************************************/

#if 0

static void fun_TCU_GearboxP_faulty_Process(void)
{
    
    uint8 u8PepsWarningSt;
    u8PepsWarningSt = 0;
    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_GearboxP_Faulty(&u8PepsWarningSt);

    if (MSG_ERROR == u8PepsWarningSt)
    {
        TCUWarningStatus[WARN_TCU_GEARBOXP_FAULTY] = UI_WARNING_STATUS_ON;

    }
    else
    {
        TCUWarningStatus[WARN_TCU_GEARBOXP_FAULTY] = UI_WARNING_STATUS_OFF;
    }
    
}

static void fun_TCU_Gearbox_faultyD_Process(void)
{
    
    uint8 u8PepsWarningSt;
    u8PepsWarningSt = 0;
    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_FaultyD(&u8PepsWarningSt);

    if (MSG_ERROR == u8PepsWarningSt)
    {
        TCUWarningStatus[WARN_TCU_GEARBOX_FAULTYD] = UI_WARNING_STATUS_ON;

    }
    else
    {
        TCUWarningStatus[WARN_TCU_GEARBOX_FAULTYD] = UI_WARNING_STATUS_OFF;
    }
    
}

static void fun_TCU_Gearbox_faultyP_Process(void)
{
    
    uint8 u8PepsWarningSt;
    u8PepsWarningSt = 0;
    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_FaultyP(&u8PepsWarningSt);

    if (MSG_ERROR == u8PepsWarningSt)
    {
        TCUWarningStatus[WARN_TCU_GEARBOX_FAULTYP] = UI_WARNING_STATUS_ON;

    }
    else
    {
        TCUWarningStatus[WARN_TCU_GEARBOX_FAULTYP] = UI_WARNING_STATUS_OFF;
    }
    
}

static void fun_TCU_Gearbox_Transmission_Process(void)
{
    uint8 u8PepsWarningSt = 0;

    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox_TransmissionSignals(&u8PepsWarningSt);

    if (MSG_ERROR == u8PepsWarningSt)
    {
        TCUWarningStatus[WARN_TCU_GEARBOX_TRANSMISSIONSIGNALS] = UI_WARNING_STATUS_ON;

    }
    else
    {
        TCUWarningStatus[WARN_TCU_GEARBOX_TRANSMISSIONSIGNALS] = UI_WARNING_STATUS_OFF;
    }
}

static void fun_TCU_Gearbox_Process(void)
{
    uint8 u8PepsWarningSt = 0;

    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Gearbox(&u8PepsWarningSt);

    if (MSG_ERROR == u8PepsWarningSt)
    {
        TCUWarningStatus[WARN_TCU_GEARBOX] = UI_WARNING_STATUS_ON;

    }
    else
    {
        TCUWarningStatus[WARN_TCU_GEARBOX] = UI_WARNING_STATUS_OFF;
    }
}


static void fun_TCU_trans_OverHeat_Process(void)
{
    uint8 u8PepsWarningSt = 0;

    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_Trans_Over_Heat(&u8PepsWarningSt);

    if (MSG_ERROR == u8PepsWarningSt)
    {
        TCUWarningStatus[WARN_TRANS_OVER_HEAT] = UI_WARNING_STATUS_ON;

    }
    else
    {
        TCUWarningStatus[WARN_TRANS_OVER_HEAT] = UI_WARNING_STATUS_OFF;
    }
}

static void fun_TCU_driver_warning_Process(void)
{
    uint8 u8PepsWarningSt = 0;

    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_TCU_Driverwarning(&u8PepsWarningSt);

    if (MSG_ERROR == u8PepsWarningSt)
    {
        TCUWarningStatus[WARN_TCU_DRIVERWARNING] = UI_WARNING_STATUS_ON;

    }
    else
    {
        TCUWarningStatus[WARN_TCU_DRIVERWARNING] = UI_WARNING_STATUS_OFF;
    }
}

static void TCU_Warning_init(void)
{
    for (int i = 0; i < UI_TCU_WARNING_TOTAL; i++)
    {
        Rte_Call_tiCS_WarningCtrl_Set_Operation(TCUWarningId[i], UI_WARNING_STATUS_OFF);
        TCUWarningStatus[i] = 0;
    }
}

static uint8 message_invalid(void)
{
    uint8 u8MsgNR = 0;
    uint8 u8MsgTout = 0;

    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_NR(&u8MsgNR);
    Rte_Read_rpSR_CANMSG_GW_TCU_Info_0x395_ComIn_Tout(&u8MsgTout);


    if ((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif


