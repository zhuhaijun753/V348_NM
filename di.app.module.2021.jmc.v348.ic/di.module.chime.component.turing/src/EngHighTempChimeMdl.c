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

File Name        :  EngHighTempChimeMdl.c
Module Short Name:  EngHighTempChimeMdl
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define EngHighTempChimeMdl_C

#include "Rte_EngHighTempChimeMdl.h"
#include "CmpLib.h"
#include "ChimeLogical.h"


CMPLIB_INSTANCE(EngHighTempChimeMdl)
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
#define cENG_HIGH_TEMP_ACTIVE                 (uint8)(0x1)
#define cPROVEOUT_TIME_CNT                    (uint8)30      /*3s timer for 100ms period*/


/*                                 Type Declarations                          *
******************************************************************************/


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/
static uint8   l_ProveoutCounter_U8 = 0;
static boolean l_EngHighTempProveOut_B = TRUE;
static eIgnState l_IGN_statePre_U8  = eIGN_OFF;

/*****************************************************************************
*                   Functions                                                *
******************************************************************************/
static void    f_EngHighTempinitialState(void);
//static boolean f_EngHighTemp_input_verify(void);
static void    f_EngHighTempChime_Process(void);
static void    EngHighTemp_PowerOn_Self_Test_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional initialization state
///
/// @return E_OK:            Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:        Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
    f_EngHighTempinitialState();
    return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional de-initialization state
///
/// @return E_OK:            Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:        Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
    f_EngHighTempinitialState();
    return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional activation state
///
/// @return E_OK:            Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:        Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation( void )
{
    f_EngHighTempinitialState();
    return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional de-activation state
///
/// @return E_OK:            Succeed
///<br>     E_NOT_READY:    In progress
///<br>     E_NOT_OK:        Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation( void )
{
    f_EngHighTempinitialState();
    return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief    State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{

    eIgnState     fl_IGN_state     = eIGN_OFF;
    EBatteryState fl_Battery_state = eBatteryState_Normal;
    boolean       fl_animationFlag = TRUE;
    //boolean       f_ret            = FALSE;

    Rte_Read_rpBatteryState_BatteryState(&fl_Battery_state);
    Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

    if(((EBatteryState)eBatteryState_UnderVoltage == fl_Battery_state) ||\
        ((EBatteryState)eBatteryState_OverVoltage == fl_Battery_state))
    {
        f_EngHighTempinitialState();
    }
    else
    {
        if((eIgnState)eIGN_RUN == fl_IGN_state)
        {
            if((eIgnState)eIGN_OFF == l_IGN_statePre_U8 )
            {
                l_EngHighTempProveOut_B = TRUE;
                l_ProveoutCounter_U8 = 0;
            }
            else if((eIgnState)eIGN_RUN == l_IGN_statePre_U8)
            {
                Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
                if ((boolean)FALSE == fl_animationFlag)
                {
                    EngHighTemp_PowerOn_Self_Test_Action();

                    if((boolean)FALSE == l_EngHighTempProveOut_B)
                    {
                        f_EngHighTempChime_Process();
                    }
                }
            }
        }
        else
        {
            l_ProveoutCounter_U8 = 0;
            f_EngHighTempinitialState();
        }

        l_IGN_statePre_U8 = fl_IGN_state;
    }

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief    Diagnostic state
///
/// @return E_OK:            Diagnostic has completed. Go to DiagReturn state.
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:            Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}

static void f_EngHighTempinitialState(void)
{
    Rte_Call_rpCS_ChimeMdl_UpdateStatus_Operation( CHIME_REQID_ENG_HIGH_TEMP , FALSE);
}

/*static boolean f_EngHighTemp_input_verify(void)
{

    boolean   f_ret = TRUE;
    uint8     fl_10B_nvr_status_U8 = 0;
    uint8     fl_10B_msg_status_U8 = 0;

    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&fl_10B_nvr_status_U8);
    Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&fl_10B_msg_status_U8);

    if(((uint8)RTE_E_NEVER_RECEIVED == fl_10B_nvr_status_U8) ||\
        ((uint8)RTE_E_TIMEOUT == fl_10B_msg_status_U8))
    {
        f_ret = FALSE;
    }

    return f_ret;
}*/

static void EngHighTemp_PowerOn_Self_Test_Action(void)
{
    if(cPROVEOUT_TIME_CNT <= l_ProveoutCounter_U8)
    {
        l_EngHighTempProveOut_B = FALSE;
    }
    else
    {
        l_ProveoutCounter_U8++;
        l_EngHighTempProveOut_B = TRUE;
    }
}

static void f_EngHighTempChime_Process(void)
{

    uint8  fl_EngHighTemp_State_U8 = 0;

    Rte_Read_rpSR_WaterTempHigh_STATUS(&fl_EngHighTemp_State_U8);

    if(cENG_HIGH_TEMP_ACTIVE == fl_EngHighTemp_State_U8)
    {
        Rte_Call_rpCS_ChimeMdl_UpdateStatus_Operation( CHIME_REQID_ENG_HIGH_TEMP , TRUE);
    }
    else
    {
        f_EngHighTempinitialState();
    }
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
