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

File Name        :  SeatbeltChime.c
Module Short Name:  SeatbeltChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define SEATBELTCHIME_C

#include "Rte_SeatbeltChime.h"
#include "ChimeLogical.h"
#include "CmpLib.h"


CMPLIB_INSTANCE(SeatbeltChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
//static uint8 fl_IgnQuickSwitchCnt = 0;
static uint16 l_SeatbeltChime_Cnt   = 0;
static uint8  l_CurSeatbelt_State   = 0;
static uint8  l_LastSeatbelt_state  = 0;
static uint8  l_IGN_statePre_U8     = eIGN_OFF;
static boolean l_SeatBeltProveOut_B = TRUE;
static uint8   l_ProveoutCounter_U8 = 0;

//static uint32 fl_ABS_VehSpdLgt = 0;
//static uint8 fl_DoorStsFrntRi_State = 0;
//static uint8 fl_DoorStsFrntLe_State = 0;

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
//#define SEATBELT_40KM_SPEED_L     648U   //648-664
//#define SEATBELT_40KM_SPEED_H     664U
//#define SEATBELT_25KM_SPEED_L     389U   //389-405
//#define SEATBELT_25KM_SPEED_H     405U
//#define SEATBELT_10KM_SPEED_L      130U   //130-146
//#define SEATBELT_10KM_SPEED_H      146U
//#define SEATBELT_Invalid_SPEED      4095U

//#define Door_Open                2U
//#define SeatBelt_Open             2U

//#define SeatBelt_None_Wrn       0U
//#define SeatBelt_First_Wrn      1U
//#define SeatBelt_Second_Wrn     2U

//#define f_VehSpdLgt_Chime_60s   (60000)   /* task period= 100ms ,60s means 600(=60000/100) */
//#define f_VehSpdLgt_Chime_90s   (90000)   /* task period= 100ms ,90s means 900(=90000/100) */

#define SBR_DRIVER_LAMP_ON                 (uint8)0x2
#define INVALID_SPEED_VALUE                (uint16)0xFFF
#define TIMEOUT_60S                        (uint16)600    /*60s timer for 100ms period*/
#define SPEED_RELEASES_LOWER_THRESHOLD     (uint16)0x1B   /*3km/h*/
#define SPEED_RELEASES_UPPER_THRESHOLD     (uint16)0x18D  /*25km/h*/
#define SPEED_VALUE_ABNORMAL_1             (uint16)0xF47
#define SPEED_VALUE_ABNORMAL_2             (uint16)0xFFE
#define SEATBELT_ACTV_CONDITION_1          (uint8)0x1
#define SEATBELT_ACTV_CONDITION_2          (uint8)0x2
#define PROVEOUT_TIME_CNT                  (uint8)30      /*3s timer for 100ms period*/

/*                                 Type Declarations                          *
******************************************************************************/


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/
//static uint8 VehSpdLgt_60s_Flag = 0;
//static uint8 VehSpdLgt_90s_Flag = 0;
//static uint8 SeatBelt_First_Wrn_Done_Flag = 0;

//static UINT8 l_DRV_seatBeltChime_State = 0;
//static UINT8 l_PAS_seatBeltChime_State = 0;
/*****************************************************************************
*                   Functions                                                *
******************************************************************************/
//static void f_SeatBeltChimeInitialState(void);
//static void f_SeatBeltChime_Process(void);
//static void f_SeatBeltChime_input_Process(void);
//static void f_Seatbelt_Timer_Flag(void);
//static void f_Seatbelt_Chime_Logic_Pro(void);
//static uint8 f_Seatbelt_Second_Warning(void);
//static uint8 f_Seatbelt_First_Warning(void);
//static uint8 f_Seatbelt_DRV_warning(void);
//static uint8 f_Seatbelt_PAS_warning(void);
static void f_SeatBeltChimeInitialState(void);
static void f_SeatBeltChime_Process(void);
static boolean f_SeatBeltChime_input_verify(void);
static void SeatBelt_PowerOn_Self_Test_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional initialization state
///
/// @return E_OK:            Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:        Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
    //f_SeatBeltChimeInitialState();
    f_SeatBeltChimeInitialState();
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
    //f_SeatBeltChimeInitialState();
    f_SeatBeltChimeInitialState();
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
    //f_SeatBeltChimeInitialState();
    f_SeatBeltChimeInitialState();
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
    //f_SeatBeltChimeInitialState();
    f_SeatBeltChimeInitialState();
    return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief    State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
    uint8    fl_animationFlag       = TRUE;
    eIgnState fl_IGN_state         = eIGN_OFF;
    EBatteryState fl_Battery_state = eBatteryState_Normal;
    uint8 fl_CHIME_SEATBELT_U8     = 0;

    Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
    Rte_Read_rpBattState_BatteryState(&fl_Battery_state);

    if(((EBatteryState)eBatteryState_UnderVoltage == fl_Battery_state) ||\
        ((EBatteryState)eBatteryState_OverVoltage == fl_Battery_state))
    {
        f_SeatBeltChimeInitialState();
    }
    else
    {    
        if ((eIgnState)eIGN_RUN == fl_IGN_state)
        {
            if((eIgnState)eIGN_OFF == l_IGN_statePre_U8)
            {
                l_SeatBeltProveOut_B = TRUE;
                l_ProveoutCounter_U8 = 0;
            }
            else if((eIgnState)eIGN_RUN == l_IGN_statePre_U8)
            {
                Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
                if ((boolean)FALSE == fl_animationFlag )
                {
                    SeatBelt_PowerOn_Self_Test_Action(); /* Avoid direct response on startup */
                    if ((boolean)FALSE == l_SeatBeltProveOut_B)
                    {
                        f_SeatBeltChime_Process();
                    }
                }
            }
        }
        else
        {
			l_SeatBeltProveOut_B = TRUE;
			l_ProveoutCounter_U8 = 0;
            f_SeatBeltChimeInitialState();
        }

        l_IGN_statePre_U8 = fl_IGN_state;
    }

    Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_DrvSeatbelt(CHIME_REQID_SEATBELT);
    Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_DrvSeatbelt(&fl_CHIME_SEATBELT_U8);
    if ((uint8)TRUE == fl_CHIME_SEATBELT_U8)
    {
        Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation(CHIME_REQID_SEATBELT, FALSE);
    }
    #if 0
    uint8   fl_animationFlag = TRUE;
    eIgnState fl_IGN_state = eIGN_OFF;
    EBatteryState fl_Battery_state = eBatteryState_Normal;

    Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
    Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
    Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);

    if((fl_Battery_state == eBatteryState_UnderVoltage)  || (fl_Battery_state == eBatteryState_OverVoltagePhase1) || (fl_Battery_state == eBatteryState_OverVoltage))
    {
        f_SeatBeltChimeInitialState();
    }
    else
    {    
        switch (fl_IGN_state)
        {
            case eIGN_RUN:
                if (( fl_animationFlag == FALSE ) && (!fl_IgnQuickSwitchCnt))
                {
                    f_SeatBeltChime_input_Process();
                }

                if (fl_IgnQuickSwitchCnt > 0)
                {
                    fl_IgnQuickSwitchCnt--;
                }
                break;
            case eIGN_OFF:            
                f_SeatBeltChimeInitialState();
                fl_IgnQuickSwitchCnt = 10;
                break;
            default:
                break;
        }
    }
    #endif
    
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

static void f_SeatBeltChimeInitialState(void)
{
    //fl_IgnQuickSwitchCnt = 0;
    l_SeatbeltChime_Cnt  = 0;
    l_CurSeatbelt_State  = 0;
    l_LastSeatbelt_state = 0;
    l_IGN_statePre_U8    = eIGN_OFF;

    Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation( CHIME_REQID_SEATBELT, FALSE );
}

static void f_SeatBeltChime_Process(void)
{
    //uint8 fl_Drv_Status     = 0;
    //uint8 fl_Pas_Status     = 0;
    //boolean fl_BkHomeLampState_U8     = FALSE;
    boolean fl_SeatbeltChimeSts_U8   = FALSE;
    uint8   fl_SRS_SBR_Drive_U8      = 0;
    uint16  fl_ABS_VehSpdLgt_U16     = 0;
    
    //Rte_Read_rp_Seatbelt_TT_Status_Drv_Status(&fl_Drv_Status);   // get TT state value
    //Rte_Read_rp_Seatbelt_TT_Status_Pas_Status(&fl_Pas_Status);

    if ((boolean)FALSE == f_SeatBeltChime_input_verify())
    {
        Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_ABS_VehSpdLgt_U16);
        Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver(&fl_SRS_SBR_Drive_U8);
            
        if ((SBR_DRIVER_LAMP_ON == fl_SRS_SBR_Drive_U8) &&\
            (SPEED_RELEASES_UPPER_THRESHOLD < fl_ABS_VehSpdLgt_U16)&&\
            (INVALID_SPEED_VALUE != fl_ABS_VehSpdLgt_U16))
        {   
            /*
            **In particular, ABS_VehSpdLgt value is between 0xF47 and 0xFFE,
            **The vehicle speed is regarded as 220km/h,so the chime will be actived
            */
            l_CurSeatbelt_State = SEATBELT_ACTV_CONDITION_1;
            fl_SeatbeltChimeSts_U8 = TRUE;
            l_SeatbeltChime_Cnt = 0;
        }
        else if ((SBR_DRIVER_LAMP_ON == fl_SRS_SBR_Drive_U8) &&\
                 (SPEED_RELEASES_UPPER_THRESHOLD >= fl_ABS_VehSpdLgt_U16)&&\
                 (SPEED_RELEASES_LOWER_THRESHOLD < fl_ABS_VehSpdLgt_U16))
        {
             if ((TIMEOUT_60S < l_SeatbeltChime_Cnt))
             {
                 l_CurSeatbelt_State = SEATBELT_ACTV_CONDITION_2;
                 fl_SeatbeltChimeSts_U8 = TRUE;
             }
             else
             {
                l_SeatbeltChime_Cnt++;
             }
        }
        else if ((SBR_DRIVER_LAMP_ON != fl_SRS_SBR_Drive_U8) ||\
                 (INVALID_SPEED_VALUE == fl_ABS_VehSpdLgt_U16)||\
                  (SPEED_RELEASES_LOWER_THRESHOLD > fl_ABS_VehSpdLgt_U16))
        {
             l_CurSeatbelt_State = 0;  /*Seatbelt chime activation condition is dissatified*/
             fl_SeatbeltChimeSts_U8 = FALSE;
             l_SeatbeltChime_Cnt = 0;
        }

        if (l_LastSeatbelt_state != l_CurSeatbelt_State)
        {
           Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation(CHIME_REQID_SEATBELT, fl_SeatbeltChimeSts_U8);
        }
            
        l_LastSeatbelt_state = l_CurSeatbelt_State;
    }
    else
    {
        f_SeatBeltChimeInitialState();
    }
#if 0    
    if(fl_animationFlag == FALSE)
    {    
        if((fl_Drv_Status == 2) || (fl_Pas_Status == 2))      //If TT on,Then chime on
        {
            fl_BkHomeLampState_U8 = TRUE;
        }
        else
        {
            fl_BkHomeLampState_U8 = FALSE;
        }
    }    

    Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation(CHIME_REQID_SEATBELT, fl_BkHomeLampState_U8);
#endif

}

static boolean f_SeatBeltChime_input_verify(void)
{
    uint8 fl_ret                = TRUE;
    uint8 fl_Msg221_NR_Sts_U8   = RTE_E_NEVER_RECEIVED;
    uint8 fl_Msg221_Tout_Sts_U8 = RTE_E_TIMEOUT;
    uint8 fl_Msg350_NR_Sts_U8   = RTE_E_NEVER_RECEIVED;
    uint8 fl_Msg350_Tout_Sts_U8 = RTE_E_TIMEOUT;

    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_Msg221_NR_Sts_U8);
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_Msg221_Tout_Sts_U8);
    Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_NR(&fl_Msg350_NR_Sts_U8);
    Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_Tout(&fl_Msg350_Tout_Sts_U8);

    if (((uint8)RTE_E_NEVER_RECEIVED == fl_Msg221_NR_Sts_U8)||\
        ((uint8)RTE_E_TIMEOUT      == fl_Msg221_Tout_Sts_U8)||\
        ((uint8)RTE_E_NEVER_RECEIVED == fl_Msg350_NR_Sts_U8)||\
        ((uint8)RTE_E_TIMEOUT      == fl_Msg350_Tout_Sts_U8))
    {
        fl_ret = TRUE;
    }
    else
    {
        fl_ret = FALSE;
    }

    return fl_ret;
}

static void SeatBelt_PowerOn_Self_Test_Action(void)
{
    if (PROVEOUT_TIME_CNT <= l_ProveoutCounter_U8)
    {
        l_SeatBeltProveOut_B = FALSE;
    }
    else
    {
        l_ProveoutCounter_U8 ++;
        l_SeatBeltProveOut_B = TRUE;
    }
}

#if 0
static void f_SeatBeltChimeInitialState(void)
{
    l_DRV_seatBeltChime_State = 0;
    l_PAS_seatBeltChime_State = 0;
    fl_IgnQuickSwitchCnt = 0;
    VehSpdLgt_60s_Flag = 0;
    VehSpdLgt_90s_Flag = 0;
    SeatBelt_First_Wrn_Done_Flag = 0;
    fl_ABS_VehSpdLgt = 0;
    fl_DoorStsFrntRi_State = 0;
    fl_DoorStsFrntLe_State = 0;

    Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation( CHIME_REQID_SEATBELT, FALSE );
    Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation( CHIME_REQID_PSNG_SEATBELT, FALSE );
}

static void f_SeatBeltChime_Process(void)
{
    f_Seatbelt_Timer_Flag();

    if((f_Seatbelt_First_Warning()) && (f_Seatbelt_DRV_warning())) /* DRV_warning1 */
    {
        l_DRV_seatBeltChime_State = SeatBelt_First_Wrn;
    }

    if((f_Seatbelt_Second_Warning()) && (f_Seatbelt_DRV_warning())) /* DRV_warning2 */
    {
        l_DRV_seatBeltChime_State = SeatBelt_Second_Wrn;
    }
    
    if((f_Seatbelt_First_Warning()) && (f_Seatbelt_PAS_warning())) /* PAS_warning1 */
    {
        l_PAS_seatBeltChime_State = SeatBelt_First_Wrn;
    }

    if((f_Seatbelt_Second_Warning()) && (f_Seatbelt_PAS_warning())) /* PAS_warning2 */
    {
        l_PAS_seatBeltChime_State = SeatBelt_Second_Wrn;
    }

    if((!f_Seatbelt_DRV_warning()) || (fl_ABS_VehSpdLgt < SEATBELT_10KM_SPEED_L)) /* DRV_NO_warning */
    {
        l_DRV_seatBeltChime_State = SeatBelt_None_Wrn;
    }
    
    if((!f_Seatbelt_PAS_warning()) || (fl_ABS_VehSpdLgt < SEATBELT_10KM_SPEED_L)) /* PAS_NO_warning */
    {
        l_PAS_seatBeltChime_State = SeatBelt_None_Wrn;
    }

    f_Seatbelt_Chime_Logic_Pro();

}

static void f_Seatbelt_Chime_Logic_Pro(void)
{
    uint8 chime_status = SeatBelt_None_Wrn;
     
    switch (l_DRV_seatBeltChime_State)
    {
        case SeatBelt_Second_Wrn:
            chime_status = SeatBelt_Second_Wrn;
            break;
            
        case SeatBelt_First_Wrn:
            chime_status = SeatBelt_First_Wrn;
            if (l_PAS_seatBeltChime_State == SeatBelt_Second_Wrn)
            {
                chime_status = SeatBelt_Second_Wrn;
            }
            else
            {
                /* Do nothing */
            }
            break;

        case SeatBelt_None_Wrn:
            chime_status = SeatBelt_None_Wrn;
            if (l_PAS_seatBeltChime_State == SeatBelt_Second_Wrn)
            {
                chime_status = SeatBelt_Second_Wrn;
            }
            else if(l_PAS_seatBeltChime_State == SeatBelt_First_Wrn)
            {
                chime_status = SeatBelt_First_Wrn;
            }
            else
            {
                /* Do nothing */
            }
            break;

        default:
            break;
    }

    switch (chime_status)
    {
        case SeatBelt_Second_Wrn:
            Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation(CHIME_REQID_SEATBELT, FALSE);         //chime 1:30s//
            Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation(CHIME_REQID_PSNG_SEATBELT, TRUE);     //chime 2:90s//
            break;
        case SeatBelt_First_Wrn:
            Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation(CHIME_REQID_SEATBELT, TRUE);          //chime 1:30s//
            Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation(CHIME_REQID_PSNG_SEATBELT, FALSE);     //chime 2:90s//
            break;
        case SeatBelt_None_Wrn:
            Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation(CHIME_REQID_SEATBELT, FALSE);         //chime 1:30s//
            Rte_Call_rpCS_SeatbeltChime_UpdateStatus_Operation(CHIME_REQID_PSNG_SEATBELT, FALSE);     //chime 2:90s//
            break;
        default:

            break;
    }
    
}

/*===========================================================================*/
static uint8 f_Seatbelt_Second_Warning(void)
{
    uint8 ret = 0;

    if((fl_ABS_VehSpdLgt >= SEATBELT_40KM_SPEED_L) || 
        (SeatBelt_First_Wrn_Done_Flag == 1) ||
        ((fl_ABS_VehSpdLgt >= SEATBELT_10KM_SPEED_L) && (fl_ABS_VehSpdLgt <= SEATBELT_40KM_SPEED_H) && (VehSpdLgt_90s_Flag == 1)))
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}
static uint8 f_Seatbelt_First_Warning(void)
{
    uint8  ret = 0;
    
    if((fl_ABS_VehSpdLgt >= SEATBELT_25KM_SPEED_L) ||
        ((fl_ABS_VehSpdLgt >= SEATBELT_10KM_SPEED_L) && (fl_ABS_VehSpdLgt <= SEATBELT_25KM_SPEED_H) && (VehSpdLgt_60s_Flag == 1)))
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}

static uint8 f_Seatbelt_DRV_warning(void)
{
    uint8 ret = 0;
    uint8 fl_SRS_SBR_Driver_signal_U8 = 0;

    Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver(&fl_SRS_SBR_Driver_signal_U8);

    if((fl_DoorStsFrntLe_State != Door_Open) && (fl_SRS_SBR_Driver_signal_U8 == SeatBelt_Open))
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }
    
    return ret;
}

static uint8 f_Seatbelt_PAS_warning(void)
{
    uint8 ret = 0;
    uint8 fl_SRS_SBR_PAMsenger_signal_U8 = 0;

    Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger(&fl_SRS_SBR_PAMsenger_signal_U8);

    if((fl_DoorStsFrntRi_State != Door_Open) && (fl_SRS_SBR_PAMsenger_signal_U8 == SeatBelt_Open))
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}
/*===========================================================================*/

static void f_Seatbelt_Timer_Flag(void)
{
    uint8 fl_VehSpdLgt_60s_IsStartflag = 0;
    uint8 fl_VehSpdLgt_60s_IsElapsedflag = 0;
    uint8 fl_VehSpdLgt_90s_IsStartflag = 0;
    uint8 fl_VehSpdLgt_90s_IsElapsedflag = 0;
    uint8 fl_CHIME_REQID_SEATBELT_U8 = 0;

    if((fl_ABS_VehSpdLgt >= SEATBELT_10KM_SPEED_L) && (fl_ABS_VehSpdLgt <= SEATBELT_25KM_SPEED_H))
    {
        Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerSeatbeltChime60s,&fl_VehSpdLgt_60s_IsStartflag);
        
        if(fl_VehSpdLgt_60s_IsStartflag == 0)
        {
            Rte_Call_rptimer_TmExt_Timer_Start(eTimerSeatbeltChime60s);
        }
    
        Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerSeatbeltChime60s, f_VehSpdLgt_Chime_60s,&fl_VehSpdLgt_60s_IsElapsedflag);

        if(fl_VehSpdLgt_60s_IsElapsedflag == 1)
        {
            VehSpdLgt_60s_Flag = 1;
        }
        else    
        {
            VehSpdLgt_60s_Flag = 0;
        }
    }
    else
    {
        Rte_Call_rptimer_TmExt_Timer_Stop(eTimerSeatbeltChime60s);
        VehSpdLgt_60s_Flag = 0;
    }

    
    if((fl_ABS_VehSpdLgt >= SEATBELT_10KM_SPEED_L) && (fl_ABS_VehSpdLgt <= SEATBELT_40KM_SPEED_H))
    {
        Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerSeatbeltChime90s,&fl_VehSpdLgt_90s_IsStartflag);
        
        if(fl_VehSpdLgt_90s_IsStartflag == 0)
        {
            Rte_Call_rptimer_TmExt_Timer_Start(eTimerSeatbeltChime90s);
        }
        
        Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerSeatbeltChime90s, f_VehSpdLgt_Chime_90s,&fl_VehSpdLgt_90s_IsElapsedflag);
        if(fl_VehSpdLgt_90s_IsElapsedflag == 1)
        {
            VehSpdLgt_90s_Flag = 1;
        }
        else 
        {
            VehSpdLgt_90s_Flag = 0;
        }
    }
    else
    {
        Rte_Call_rptimer_TmExt_Timer_Stop(eTimerSeatbeltChime90s);
        VehSpdLgt_90s_Flag = 0;
    }



    Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_DrvSeatbelt(CHIME_REQID_SEATBELT);
    Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_DrvSeatbelt(&fl_CHIME_REQID_SEATBELT_U8);
    if (TRUE == fl_CHIME_REQID_SEATBELT_U8)
    {
        SeatBelt_First_Wrn_Done_Flag = 1;
    }
    else
    {
        SeatBelt_First_Wrn_Done_Flag = 0;
    }

}

static void f_SeatBeltChime_input_Process(void)
{
    uint8     fl_CAN_nvr_350_status_U8 = 0;
    uint8    fl_CAN_msg_350_status_U8 = 0;
    uint8     fl_CAN_nvr_221_status_U8 = 0;
    uint8    fl_CAN_msg_221_status_U8 = 0;    
    uint8     fl_CAN_nvr_310_status_U8 = 0;
    uint8    fl_CAN_msg_310_status_U8 = 0;
    

    Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_NR(&fl_CAN_nvr_350_status_U8);
    Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_Tout(&fl_CAN_msg_350_status_U8);
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_CAN_nvr_221_status_U8);
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_CAN_msg_221_status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&fl_CAN_nvr_310_status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&fl_CAN_msg_310_status_U8);
    
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_ABS_VehSpdLgt);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi(&fl_DoorStsFrntRi_State);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&fl_DoorStsFrntLe_State);

    if((fl_CAN_msg_221_status_U8 == RTE_E_TIMEOUT) || (SEATBELT_Invalid_SPEED == fl_ABS_VehSpdLgt))
    {
        fl_ABS_VehSpdLgt = 0;
    }

    if(fl_CAN_msg_310_status_U8 == RTE_E_TIMEOUT)
    {
        fl_DoorStsFrntRi_State = 0;
        fl_DoorStsFrntLe_State = 0;
    }

    if ((fl_CAN_nvr_350_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_msg_350_status_U8 == RTE_E_TIMEOUT) ||
        (fl_CAN_nvr_221_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_CAN_nvr_310_status_U8 == RTE_E_NEVER_RECEIVED)) 
    {
        f_SeatBeltChimeInitialState();
    }
    else
    {
        f_SeatBeltChime_Process();
    }
}

#endif
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
