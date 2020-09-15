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

#include "Rte_DoorOpenChime.h"
#include "ChimeLogical.h"
#include "CmpLib.h"

CMPLIB_INSTANCE(DoorOpenChime)

/*****************************************************************************
*                                 Macro Definitions                          *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/
//static uint8 fl_count_3s = 0;
//static boolean fl_OnOff_flag = FALSE;


/*                                 Type Declarations                          *
******************************************************************************/

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
#define DOOROPEN_CHIME_SIGNAL   2U
    
/* door open speed judge */
#define SPEED_RELEASES_LOWER_THRESHOLD     (uint16)0x1B   /*3km/h*/
#define INVALID_SPEED_VALUE                (uint16)0xFFF
#define PROVEOUT_TIME_CNT                  (uint8)30      /*3s timer for 100ms period*/
//#define DOOR_LOWER_SPEED  290U                          /* 3 km/h */
//#define DOOR_UPPER_SPEED  22000U                        /* 220 km/h */

/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/
static UINT8     fl_BCM_Door_LastSts_FrntLe = 0;
static UINT8     fl_BCM_Door_LastSts_FrntRi = 0;
static UINT8     fl_BCM_Door_LastSts_RearLe = 0;
static UINT8     fl_BCM_Door_LastSts_RearRi = 0;
static UINT8     fl_BCM_Door_LastSts_Trunk  = 0;
//static UINT8   fl_BCM_Door_LastSts_Enginehood = 0;
static uint8     l_IGN_statePre_U8          = FALSE;
static boolean   l_DoorOpenProveOut_B       = TRUE;
static uint8     l_ProveoutCounter_U8       = FALSE;
static boolean   l_doorOpenChime_Active_B   = FALSE;


/*****************************************************************************
*                   Functions                                                *
******************************************************************************/

static void   f_DoorOpenChimeInit(void);
static void   f_DoorOpenChime_Process(void);
static uint8  f_DoorOpenChime_input_verity(void);
static void   DoorOpen_PowerOn_Self_Test_Action(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief    Transitional initialization state
///
/// @return E_OK:            Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:        Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
    f_DoorOpenChimeInit();
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
    f_DoorOpenChimeInit();
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
    f_DoorOpenChimeInit();
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
    f_DoorOpenChimeInit();
    return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief    State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
    boolean       f_ret            = FALSE;
    eIgnState     fl_IGN_state     = eIGN_OFF;
    EBatteryState fl_Battery_state = eBatteryState_Normal;
    boolean       fl_animationFlag = TRUE;
    //UINT32        Real_Value_U32   = 0;
    uint32        fl_ABS_VehSpdLgt = 0;
    uint8         fl_CHIME_REQID_DOOR_OPEN = FALSE;

    Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
    Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
    //Rte_Read_DoorOpenChime_rpSR_TISpdMdl_Real_Value(&Real_Value_U32);
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_ABS_VehSpdLgt);
    
    if(((EBatteryState)eBatteryState_UnderVoltage == fl_Battery_state) ||\
        ((EBatteryState)eBatteryState_OverVoltage == fl_Battery_state ))
    {
        f_DoorOpenChimeInit();
    }
    else
    {
		if((eIgnState)eIGN_RUN == fl_IGN_state)
		{
			if((eIgnState)eIGN_OFF == l_IGN_statePre_U8)
			{
				l_DoorOpenProveOut_B = TRUE;
				l_ProveoutCounter_U8 = 0;
			}
			else if((eIgnState)eIGN_RUN == l_IGN_statePre_U8)
			{
				Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
				if((boolean)FALSE == fl_animationFlag)
				{
					DoorOpen_PowerOn_Self_Test_Action();
		
					if ((boolean)FALSE == l_DoorOpenProveOut_B)
					{
						f_ret = f_DoorOpenChime_input_verity();
						
						if((boolean)TRUE == f_ret)
						{
							if ((SPEED_RELEASES_LOWER_THRESHOLD > fl_ABS_VehSpdLgt) ||\
								(INVALID_SPEED_VALUE == fl_ABS_VehSpdLgt))
							{
								f_DoorOpenChimeInit();
							}
							else
							{
								f_DoorOpenChime_Process();
							}
						}
						else
						{
							f_DoorOpenChimeInit();
						}
					}
				}
			}
		}
		else
		{
			l_ProveoutCounter_U8 = 0;
			f_DoorOpenChimeInit();
		}

        l_IGN_statePre_U8 = fl_IGN_state;
    }
#if 0
    if(Real_Value_U32 < DOOR_LOWER_SPEED)
    {
        f_DoorOpenChimeInit();
    }

    if(fl_ABS_VehSpdLgt == 4095)
    {
        if(fl_count_3s >= 20)
        {
            fl_count_3s = 20;
            f_DoorOpenChimeInit();
        }
        else
        {
            fl_count_3s++;
        }
    }
    else
    {
        fl_count_3s = 0;
    }
#endif
    Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_DoorOpenChime(CHIME_REQID_DOOR_OPEN);
    Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_DoorOpenChime(&fl_CHIME_REQID_DOOR_OPEN);
    if((boolean)TRUE == fl_CHIME_REQID_DOOR_OPEN)
    {
        Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN,FALSE);
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

static void f_DoorOpenChimeInit(void)
{
    //fl_count_3s = 0;
    //fl_OnOff_flag = FALSE;
    l_doorOpenChime_Active_B = FALSE;
    fl_BCM_Door_LastSts_FrntLe = 0;
    fl_BCM_Door_LastSts_FrntRi = 0;
    fl_BCM_Door_LastSts_RearLe = 0;
    fl_BCM_Door_LastSts_RearRi = 0;
    fl_BCM_Door_LastSts_Trunk  = 0;
    //fl_BCM_Door_LastSts_Enginehood = 0;
    Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
}

static void DoorOpen_PowerOn_Self_Test_Action(void)
{
    if(PROVEOUT_TIME_CNT <= l_ProveoutCounter_U8)
    {
        l_DoorOpenProveOut_B = FALSE;
    }
    else
    {
        l_ProveoutCounter_U8++;
        l_DoorOpenProveOut_B = TRUE;
    }
}

static void f_DoorOpenChime_Process(void)
{
    uint8  fl_BCM_DoorStsFrntLe_signal_U8 = 0;
    uint8  fl_BCM_DoorStsFrntRi_signal_U8 = 0;
    uint8  fl_BCM_DoorStsRearLe_signal_U8 = 0;
    uint8  fl_BCM_DoorStsRearRi_signal_U8 = 0;
    uint8  fl_BCM_DoorStsTrunk_signal_U8 = 0;
    //uint8  fl_BCM_DoorStsEnginehood_signal_U8 = 0;

    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&fl_BCM_DoorStsFrntLe_signal_U8);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi(&fl_BCM_DoorStsFrntRi_signal_U8);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearLe(&fl_BCM_DoorStsRearLe_signal_U8);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsRearRi(&fl_BCM_DoorStsRearRi_signal_U8);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsTrunk(&fl_BCM_DoorStsTrunk_signal_U8);
    //Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_EnginehoodSts(&fl_BCM_DoorStsEnginehood_signal_U8);

    if( fl_BCM_Door_LastSts_FrntLe != fl_BCM_DoorStsFrntLe_signal_U8 )
    {
        if ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_DoorStsFrntLe_signal_U8)
        {
            l_doorOpenChime_Active_B = TRUE;
            Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
        }
    }

    if ( fl_BCM_Door_LastSts_FrntRi != fl_BCM_DoorStsFrntRi_signal_U8 )
    {
        if ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_DoorStsFrntRi_signal_U8)
        {
            l_doorOpenChime_Active_B = TRUE;
            Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
        }
    }

    if ( fl_BCM_Door_LastSts_RearLe != fl_BCM_DoorStsRearLe_signal_U8 )
    {
        if ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_DoorStsRearLe_signal_U8)
        {
            l_doorOpenChime_Active_B = TRUE;
            Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
        }
    }

    if ( fl_BCM_Door_LastSts_RearRi != fl_BCM_DoorStsRearRi_signal_U8 )
    {
        if ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_DoorStsRearRi_signal_U8)
        {
            l_doorOpenChime_Active_B = TRUE;
            Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
        }
    }

    if ( fl_BCM_Door_LastSts_Trunk  != fl_BCM_DoorStsTrunk_signal_U8 )
    {
        if ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_DoorStsTrunk_signal_U8)
        {
            l_doorOpenChime_Active_B = TRUE;
            Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
        }
    }

    if (((uint8)DOOROPEN_CHIME_SIGNAL != fl_BCM_DoorStsFrntLe_signal_U8) &&\
        ((uint8)DOOROPEN_CHIME_SIGNAL != fl_BCM_DoorStsFrntRi_signal_U8) &&\
        ((uint8)DOOROPEN_CHIME_SIGNAL != fl_BCM_DoorStsRearLe_signal_U8) &&\
        ((uint8)DOOROPEN_CHIME_SIGNAL != fl_BCM_DoorStsRearRi_signal_U8) &&\
        ((uint8)DOOROPEN_CHIME_SIGNAL != fl_BCM_DoorStsTrunk_signal_U8) )
    {
        l_doorOpenChime_Active_B = FALSE;
        Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
    }


    fl_BCM_Door_LastSts_FrntLe = fl_BCM_DoorStsFrntLe_signal_U8;
    fl_BCM_Door_LastSts_FrntRi = fl_BCM_DoorStsFrntRi_signal_U8;
    fl_BCM_Door_LastSts_RearLe = fl_BCM_DoorStsRearLe_signal_U8;
    fl_BCM_Door_LastSts_RearRi = fl_BCM_DoorStsRearRi_signal_U8;
    fl_BCM_Door_LastSts_Trunk  = fl_BCM_DoorStsTrunk_signal_U8;

#if 0
    if((boolean)TRUE == fl_OnOff_flag)
    {
        l_doorOpenChime_Active_B = TRUE;
        Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
        fl_OnOff_flag = FALSE;
    }

    if ( fl_BCM_Door_LastSts_FrntLe != fl_BCM_DoorStsFrntLe_signal_U8 )
    {    
        if ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_DoorStsFrntLe_signal_U8)
        {
            if( ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_FrntRi) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_RearLe) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_RearRi) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_Trunk) /*||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_Enginehood)*/)
            {
                l_doorOpenChime_Active_B = FALSE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
                fl_OnOff_flag = TRUE;
            }
            else
            {
                fl_OnOff_flag = FALSE;
                l_doorOpenChime_Active_B = TRUE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
            }
        }
    }

    if ( fl_BCM_Door_LastSts_FrntRi != fl_BCM_DoorStsFrntRi_signal_U8 ) 
    {    
        if ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_DoorStsFrntRi_signal_U8)
        {
            if( ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_FrntLe) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_RearLe) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_RearRi) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_Trunk) /*||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_Enginehood)*/)
            {            
                l_doorOpenChime_Active_B = FALSE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
                fl_OnOff_flag = TRUE;
            }
            else
            {
                fl_OnOff_flag = FALSE;
                l_doorOpenChime_Active_B = TRUE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
            }
        }
    }
    
    if ( fl_BCM_Door_LastSts_RearLe != fl_BCM_DoorStsRearLe_signal_U8 ) 
    {    
        if ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_DoorStsRearLe_signal_U8)
        {
            if( ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_FrntLe) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_FrntRi) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_RearRi) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_Trunk) /*||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_Enginehood)*/)
            {            
                l_doorOpenChime_Active_B = FALSE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
                fl_OnOff_flag = TRUE;
            }
            else
            {
                fl_OnOff_flag = FALSE;
                l_doorOpenChime_Active_B = TRUE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
            }
        }
    }
    
    if ( fl_BCM_Door_LastSts_RearRi != fl_BCM_DoorStsRearRi_signal_U8 ) 
    {    
        if ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_DoorStsRearRi_signal_U8)
        {
            if( ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_FrntLe) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_FrntRi) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_RearLe) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_Trunk) /*||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_Enginehood)*/)
            {            
                l_doorOpenChime_Active_B = FALSE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
                fl_OnOff_flag = TRUE;
            }
            else
            {
                fl_OnOff_flag = FALSE;
                l_doorOpenChime_Active_B = TRUE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
            }
        }
    }
    
    if ( fl_BCM_Door_LastSts_Trunk != fl_BCM_DoorStsTrunk_signal_U8 ) 
    {    
        if ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_DoorStsTrunk_signal_U8)
        {
            if( ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_FrntLe) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_FrntRi) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_RearLe) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_RearRi) /*||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_Enginehood)*/)
            {            
                l_doorOpenChime_Active_B = FALSE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
                fl_OnOff_flag = TRUE;
            }
            else
            {
                fl_OnOff_flag = FALSE;
                l_doorOpenChime_Active_B = TRUE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
            }
        }
    }
    /*
    if ( fl_BCM_Door_LastSts_Enginehood != fl_BCM_DoorStsEnginehood_signal_U8 ) 
    {    
        if ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_DoorStsEnginehood_signal_U8)
        {
            if( ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_FrntLe) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_FrntRi) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_RearLe) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_RearRi) ||\
                ((uint8)DOOROPEN_CHIME_SIGNAL == fl_BCM_Door_LastSts_Trunk))
            {            
                l_doorOpenChime_Active_B = FALSE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
                fl_OnOff_flag = TRUE;
            }
            else
            {
                fl_OnOff_flag = FALSE;
                l_doorOpenChime_Active_B = TRUE;
                Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
            }
        }
    }*/

    if (((uint8)DOOROPEN_CHIME_SIGNAL != fl_BCM_DoorStsFrntLe_signal_U8) &&\
        ((uint8)DOOROPEN_CHIME_SIGNAL != fl_BCM_DoorStsFrntRi_signal_U8) &&\
        ((uint8)DOOROPEN_CHIME_SIGNAL != fl_BCM_DoorStsRearLe_signal_U8) &&\
        ((uint8)DOOROPEN_CHIME_SIGNAL != fl_BCM_DoorStsRearRi_signal_U8) &&\
        ((uint8)DOOROPEN_CHIME_SIGNAL != fl_BCM_DoorStsTrunk_signal_U8)/* &&\
        ((uint8)DOOROPEN_CHIME_SIGNAL != fl_BCM_DoorStsEnginehood_signal_U8)*/)
    {
        l_doorOpenChime_Active_B = FALSE;
        Rte_Call_rpCS_DoorOpenChime_UpdateStatus_Operation(CHIME_REQID_DOOR_OPEN, l_doorOpenChime_Active_B);
    }
    
    fl_BCM_Door_LastSts_FrntLe = fl_BCM_DoorStsFrntLe_signal_U8;
    fl_BCM_Door_LastSts_FrntRi = fl_BCM_DoorStsFrntRi_signal_U8;
    fl_BCM_Door_LastSts_RearLe = fl_BCM_DoorStsRearLe_signal_U8;
    fl_BCM_Door_LastSts_RearRi = fl_BCM_DoorStsRearRi_signal_U8;
    fl_BCM_Door_LastSts_Trunk  = fl_BCM_DoorStsTrunk_signal_U8;
    //fl_BCM_Door_LastSts_Enginehood = fl_BCM_DoorStsEnginehood_signal_U8;
#endif
}

static boolean f_DoorOpenChime_input_verity(void)
{
    boolean    f_ret = TRUE;
    uint8     fl_CAN_221_nvr_status_U8 = 0;
    uint8     fl_CAN_221_msg_status_U8 = 0;
    uint8     fl_CAN_310_nvr_status_U8 = 0;
    uint8     fl_CAN_310_msg_status_U8 = 0;

    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&fl_CAN_221_nvr_status_U8);
    Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&fl_CAN_221_msg_status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&fl_CAN_310_nvr_status_U8);
    Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&fl_CAN_310_msg_status_U8);
    
    if(((uint8)RTE_E_NEVER_RECEIVED == fl_CAN_221_nvr_status_U8) ||\
        ((uint8)RTE_E_TIMEOUT == fl_CAN_221_msg_status_U8))
    {
        f_DoorOpenChimeInit();
        f_ret = FALSE;
    }
    
    if(((uint8)RTE_E_NEVER_RECEIVED == fl_CAN_310_nvr_status_U8) ||\
        ((uint8)RTE_E_TIMEOUT == fl_CAN_310_msg_status_U8))
    {
        f_DoorOpenChimeInit();
        f_ret = FALSE;
    }

    return f_ret;
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
