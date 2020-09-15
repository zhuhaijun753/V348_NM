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

File Name        :  PepsChime.c
Module Short Name:  PepsChime
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define PepsChime_C

#include "Rte_PepsChime.h"
#include "CmpLib.h"
#include "ChimeLogical.h"


CMPLIB_INSTANCE(PepsChime)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
/* 325 message for Active On*/
static uint8 fl_PEPS_Active_NoKeyFoundWarning_Last_U8 = 0; 
static uint8 fl_PEPS_Active_KeyOutWarning1_Last_U8 = 0; 
static uint8 fl_PEPS_Active_BrakeIndicationWarning_Last_U8 = 0; 
static uint8 fl_PEPS_Active_ClutchIndicationWarning_Last_U8 = 0; 
static uint8 fl_PEPS_Active_SSBChimeState_Last_U8 = 0; 

/*325 message for Work On*/
static uint8 fl_PEPS_Work_NoKeyFoundWarning_Last_U8 = 0;
static uint8 fl_PEPS_Work_KeyOutWarning1_Last_U8 = 0;
static uint8 fl_PEPS_Work_NotNWarning_Last_U8 = 0;
static uint8 fl_PEPS_Work_KeyBatteryWarning_Last_U8 = 0;
static uint8 fl_PEPS_Work_SSB_FailStatus_Last_U8 = 0;
static uint8 fl_PEPS_Work_IMMO_MatchStatus_Last_U8 = 0;

/*325 message for 10s On  */
static uint8 fl_TmExt_Timer_IsElapsed_Sts = FALSE;
static uint8 fl_BrakeIndWrn_Chime_First_Sts = TRUE;


/* 318 message */
static uint8 l_PEPS_Work_IMMOChimeState_U8 = FALSE;	
static uint8	fl_0x318_ImmoSts_State_Last_U8 = 0;

/* 326 message*/
static uint8 l_RF_keyLowBatteryWaring_status_u8 = FALSE; 

/*this is 3s time*/
static boolean  l_Peps_Flag  = TRUE;
static uint8    l_ProveoutCounter_U8  = 0;

/* EOL configuration */
static uint8 IsEngineCfg_Chime_Key_Battery = 0;
static uint8 IsEngineCfg_AT_MT = 0;

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
#define PEPS_IMMO_VALUE_1               (1)
#define PEPS_IMMO_VALUE_2               (2)

#define	FUNCTION_CALLED_PEPS	        (100) //ms
#define CHIME_DELAY_TIME_PEPS_3000MS	(3000  / FUNCTION_CALLED_PEPS)

#define cTIMEOUT_CHIME_10S              (10000) //ms
#define cTIMEOUT_CHIME_TRUE             (TRUE)

#define VEHICLE_CHIME_IMMO  (1)
#define VEHICLE_CHIME_PEPS  (2)

#define VEHICLE_CHIME_5MT   (0)
#define VEHICLE_CHIME_6MT   (1)
#define VEHICLE_CHIME_AMT   (2)
/*                                 Type Declarations                          *
******************************************************************************/


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/



/*****************************************************************************
*                   Functions                                                *
******************************************************************************/

static void f_Chime_0x326_IMMO_KeyBattery_Process(void);
static void f_Chime_0x318_IMMO_Warning_Process(void);
static void f_Chime_0x325_PEPS_Active_On_Process(void);
static void f_Chime_0x325_Peps_Work_On_Process(void);
static void f_Chime_0x325_Peps_Work_On_Cycle_10s_Process(void);
static void f_chime_Peps_Work_On_Update_Process(void);
static void f_0x325_PEPS_Active_On_initialState(void);
static void f_0x325_PEPS_Work_On_initialState(void);
static void f_0x326_IMMO_Init(void);
static void f_0x318_IMMO_Init(void);
static boolean f_0x325_PEPS_input_verify(void);

static boolean PEPS_PowerOn_3000MS_Self_Test_Action(void);
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	f_0x325_PEPS_Active_On_initialState();
	f_0x325_PEPS_Work_On_initialState();
	f_0x326_IMMO_Init();
	f_0x318_IMMO_Init();
	
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
	f_0x325_PEPS_Active_On_initialState();
	f_0x325_PEPS_Work_On_initialState();
	f_0x326_IMMO_Init();
	f_0x318_IMMO_Init();
	
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
	f_0x325_PEPS_Active_On_initialState();
	f_0x325_PEPS_Work_On_initialState();
	f_0x326_IMMO_Init();
	f_0x318_IMMO_Init();
	
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
	f_0x325_PEPS_Active_On_initialState();
	f_0x325_PEPS_Work_On_initialState();
	f_0x326_IMMO_Init();
	f_0x318_IMMO_Init();
	
	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	uint8 fl_animationFlag = TRUE;
	eIgnState fl_IGN_state = eIGN_OFF;
	EBatteryState fl_Battery_state = eBatteryState_Normal;
    boolean	f_ret = FALSE;

	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_PEPS,&IsEngineCfg_Chime_Key_Battery);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AT_MT,&IsEngineCfg_AT_MT);
	
	if((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage))
	{
		f_0x325_PEPS_Active_On_initialState();
		f_0x325_PEPS_Work_On_initialState();
		f_0x326_IMMO_Init();
		f_0x318_IMMO_Init();
	}
	else
	{
		switch (fl_IGN_state)
		{
			case eIGN_RUN:
				Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
				if (FALSE == fl_animationFlag) 
				{
					PEPS_PowerOn_3000MS_Self_Test_Action();
					switch(IsEngineCfg_Chime_Key_Battery)
					{
						case VEHICLE_CHIME_PEPS:
							if(FALSE == l_Peps_Flag)
							{
								f_ret = f_0x325_PEPS_input_verify();
								if(f_ret == TRUE)
								{
									f_Chime_0x325_Peps_Work_On_Process();
									f_Chime_0x325_Peps_Work_On_Cycle_10s_Process();//The Function is  10s cycle for PEPS
									f_chime_Peps_Work_On_Update_Process();
								}
								else
								{
									f_0x325_PEPS_Work_On_initialState();
								}
							}
							break;
						case VEHICLE_CHIME_IMMO:
							if(FALSE == l_Peps_Flag)
							{
								f_Chime_0x326_IMMO_KeyBattery_Process();
								f_Chime_0x318_IMMO_Warning_Process();							
							}
							break;
						default:
							break;
					}
				}
				else
				{
					f_0x325_PEPS_Work_On_initialState();
					f_0x326_IMMO_Init();
					f_0x318_IMMO_Init();
					
					l_Peps_Flag 			 = TRUE;
					l_ProveoutCounter_U8	 = 0;
				}
				
				f_0x325_PEPS_Active_On_initialState();
				break;
			case eIGN_OFF:
				if(VEHICLE_CHIME_PEPS == IsEngineCfg_Chime_Key_Battery)//Only PEPS
				{
					f_ret = f_0x325_PEPS_input_verify();
					if(f_ret == TRUE)
					{
						f_Chime_0x325_PEPS_Active_On_Process();
					}
					else
					{
						f_0x325_PEPS_Active_On_initialState();
					}
				}
				
				f_0x325_PEPS_Work_On_initialState();
				f_0x326_IMMO_Init();
				f_0x318_IMMO_Init();
				break;
			default:
				break;
		}
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
static void f_Chime_0x326_IMMO_KeyBattery_Process(void)
{
	//RF_326:This Only IMMO Work RF_0x326_ComIn_RF_KeyLowBatteryWarning == 1;
	
	uint8 fl_326msgNR = 0;
	uint8 fl_326msgTout = 0;
	uint8 fl_RF_keyLowBatteryWaring_U8 = 0;
	
	Rte_Read_rpSR_CANMSG_RF_0x326_ComIn_NR(&fl_326msgNR);
	Rte_Read_rpSR_CANMSG_RF_0x326_ComIn_Tout(&fl_326msgTout);
	Rte_Read_rpSR_CANMSG_RF_0x326_ComIn_RF_KeyLowBatteryWarning(&fl_RF_keyLowBatteryWaring_U8);
	
	if((RTE_E_TIMEOUT == fl_326msgTout) || (RTE_E_NEVER_RECEIVED == fl_326msgNR))
	{
		f_0x326_IMMO_Init();
	}
	else
	{
		if(PEPS_IMMO_VALUE_1 == fl_RF_keyLowBatteryWaring_U8)
		{
			l_RF_keyLowBatteryWaring_status_u8 = TRUE;
		}
		else
		{
			l_RF_keyLowBatteryWaring_status_u8 = FALSE;
		}
	}
	Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_PEPS_KEY_BAT,l_RF_keyLowBatteryWaring_status_u8);

}

static void f_Chime_0x318_IMMO_Warning_Process(void)
{
	//BCM_318:This Only IMMO Work BCM_BCM_ImmoSts == 1 || 2;
	uint8 fl_318msgNR = 0;
	uint8 fl_318msgTout = 0;
	uint8 fl_0x318_ImmoSts_State_U8 = 0;
	
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_NR(&fl_318msgNR);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_Tout(&fl_318msgTout);
	Rte_Read_rpSR_CANMSG_BCM_Lamp_0x318_ComIn_BCM_ImmoSts(&fl_0x318_ImmoSts_State_U8);
	
	if((fl_318msgNR == RTE_E_NEVER_RECEIVED) || (fl_318msgTout == RTE_E_TIMEOUT))
	{
		f_0x318_IMMO_Init();
	}
	else
	{
		if((PEPS_IMMO_VALUE_1 == fl_0x318_ImmoSts_State_U8 || PEPS_IMMO_VALUE_2 == fl_0x318_ImmoSts_State_U8 ) 
		&& (fl_0x318_ImmoSts_State_Last_U8 != fl_0x318_ImmoSts_State_U8))
		{
			l_PEPS_Work_IMMOChimeState_U8 = TRUE;
		}
		else
		{
			l_PEPS_Work_IMMOChimeState_U8 = FALSE;
		}
		
		fl_0x318_ImmoSts_State_Last_U8 = fl_0x318_ImmoSts_State_U8;
		
		Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_PEPS_IMMO,l_PEPS_Work_IMMOChimeState_U8);
	}
}

static void f_Chime_0x325_PEPS_Active_On_Process(void)
{	


	//1-NoKeyFound:PEPS_NoKeyFoundWarning_ON == 1	
	uint8 fl_PEPS_Active_NoKeyFoundWarning_sigval_U8 = 0;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NoKeyFoundWarning(&fl_PEPS_Active_NoKeyFoundWarning_sigval_U8);
	if(fl_PEPS_Active_NoKeyFoundWarning_Last_U8 != fl_PEPS_Active_NoKeyFoundWarning_sigval_U8)
	{
		if(fl_PEPS_Active_NoKeyFoundWarning_sigval_U8 == PEPS_IMMO_VALUE_1)
		{
			Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_D2_PEPS_SSB,TRUE);
		}
	}
	fl_PEPS_Active_NoKeyFoundWarning_Last_U8 = fl_PEPS_Active_NoKeyFoundWarning_sigval_U8;
	
	//2-KeyOut:PEPS_KeyOutWarning1_ON == 1
	uint8 fl_PEPS_Active_KeyOutWarning1_sigval_U8 = 0;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyOutWarning1(&fl_PEPS_Active_KeyOutWarning1_sigval_U8);
	if(fl_PEPS_Active_KeyOutWarning1_Last_U8 != fl_PEPS_Active_KeyOutWarning1_sigval_U8)
	{
		if(fl_PEPS_Active_KeyOutWarning1_sigval_U8 == PEPS_IMMO_VALUE_1)
		{
			Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_D2_PEPS_SSB,TRUE);
		}
	}
	fl_PEPS_Active_KeyOutWarning1_Last_U8 = fl_PEPS_Active_KeyOutWarning1_sigval_U8;
	
	//4-BRK_IND:PEPS_BrakeIndicationWarning_ON == 1
	uint8 fl_PEPS_Active_BrakeIndicationWarning_sigval_U8 = 0;
	if(VEHICLE_CHIME_AMT == IsEngineCfg_AT_MT)
	{
		Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_BrakeIndicationWarning(&fl_PEPS_Active_BrakeIndicationWarning_sigval_U8);
	}
	
	if(fl_PEPS_Active_BrakeIndicationWarning_Last_U8 != fl_PEPS_Active_BrakeIndicationWarning_sigval_U8)
	{
		if(fl_PEPS_Active_BrakeIndicationWarning_sigval_U8 == PEPS_IMMO_VALUE_1)
		{	
			Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_D2_PEPS_SSB,TRUE);
		}
	}
	fl_PEPS_Active_BrakeIndicationWarning_Last_U8 = fl_PEPS_Active_BrakeIndicationWarning_sigval_U8;

	//5-CLUTCH_IND:PEPS_ClutchIndicationWarning_ON == 1
	uint8 fl_PEPS_Active_ClutchIndicationWarning_sigval_U8 = 0;
	if((VEHICLE_CHIME_5MT == IsEngineCfg_AT_MT) || (VEHICLE_CHIME_6MT == IsEngineCfg_AT_MT))
	{
		Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ClutchIndicationWarning(&fl_PEPS_Active_ClutchIndicationWarning_sigval_U8);
	}
	
	if(fl_PEPS_Active_ClutchIndicationWarning_Last_U8 != fl_PEPS_Active_ClutchIndicationWarning_sigval_U8)
	{
		if(fl_PEPS_Active_ClutchIndicationWarning_sigval_U8 == PEPS_IMMO_VALUE_1)
		{
			Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_D2_PEPS_SSB,TRUE);
		}
	}
	fl_PEPS_Active_ClutchIndicationWarning_Last_U8 = fl_PEPS_Active_ClutchIndicationWarning_sigval_U8;

	//8-
	uint8 fl_PEPS_Active_SSB_FailStatus_sigval_U8 = 0;
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_SSB_FailStatus(&fl_PEPS_Active_SSB_FailStatus_sigval_U8);
	if(fl_PEPS_Active_SSBChimeState_Last_U8 != fl_PEPS_Active_SSB_FailStatus_sigval_U8)
	{
		if(fl_PEPS_Active_SSB_FailStatus_sigval_U8 == PEPS_IMMO_VALUE_1)
		{
			Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_D2_PEPS_SSB,TRUE);
		}
	}
	fl_PEPS_Active_SSBChimeState_Last_U8 = fl_PEPS_Active_SSB_FailStatus_sigval_U8;
	


	uint8 fl_CHIME_REQID_D2_PEPS_SSB = FALSE;
	Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_PEPSChime(CHIME_REQID_D2_PEPS_SSB);
	Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_PEPSChime(&fl_CHIME_REQID_D2_PEPS_SSB);
	if(TRUE == fl_CHIME_REQID_D2_PEPS_SSB)
	{
		Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_D2_PEPS_SSB,FALSE);
	}
	

}

static void f_Chime_0x325_Peps_Work_On_Process(void)
{	

    boolean fl_BrakeIndWrn_Chime_IsStarted      = (boolean)FALSE;
    boolean fl_BrakeIndWrn_Chime_IsElapsed      = (boolean)FALSE;
    boolean fl_Clutch_Chime_IsStarted      = (boolean)FALSE;
    boolean fl_Clutch_Chime_IsElapsed      = (boolean)FALSE;
	
	// 1/2/3/6/7/8/9 work on 3s then off
	uint8 fl_PEPS_Work_NoKeyFoundWarning_sigval_U8 = 0;
	uint8 fl_PEPS_Work_KeyOutWarning1_sigval_U8 = 0;
	uint8 fl_PEPS_Work_NotNWarning_sigval_U8 = 0;
	uint8 fl_PEPS_Work_KeyBatteryWarning_sigval_U8 = 0;
	uint8 fl_PEPS_Work_SSB_FailStatus_sigval_U8 = 0;
	uint8 fl_PEPS_Work_IMMO_MatchStatus_sigval_U8 = 0;
	
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NoKeyFoundWarning(&fl_PEPS_Work_NoKeyFoundWarning_sigval_U8);		    //1
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyOutWarning1(&fl_PEPS_Work_KeyOutWarning1_sigval_U8);       			//2
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_NotNWarning(&fl_PEPS_Work_NotNWarning_sigval_U8);                        //3
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_KeyBatteryWarning(&fl_PEPS_Work_KeyBatteryWarning_sigval_U8);  			//6
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_SSB_FailStatus(&fl_PEPS_Work_SSB_FailStatus_sigval_U8);       			//8
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_IMMO_MatchStatus(&fl_PEPS_Work_IMMO_MatchStatus_sigval_U8);    			//9

	if (((PEPS_IMMO_VALUE_1 == fl_PEPS_Work_NoKeyFoundWarning_sigval_U8) && (fl_PEPS_Work_NoKeyFoundWarning_Last_U8 != fl_PEPS_Work_NoKeyFoundWarning_sigval_U8))
	||  ((PEPS_IMMO_VALUE_1 == fl_PEPS_Work_KeyOutWarning1_sigval_U8)  && (fl_PEPS_Work_KeyOutWarning1_Last_U8 != fl_PEPS_Work_KeyOutWarning1_sigval_U8))
	||  ((PEPS_IMMO_VALUE_1 == fl_PEPS_Work_NotNWarning_sigval_U8)  && (fl_PEPS_Work_NotNWarning_Last_U8 != fl_PEPS_Work_NotNWarning_sigval_U8))
	||  ((PEPS_IMMO_VALUE_1 == fl_PEPS_Work_KeyBatteryWarning_sigval_U8) && (fl_PEPS_Work_KeyBatteryWarning_Last_U8 != fl_PEPS_Work_KeyBatteryWarning_sigval_U8))
	||  ((PEPS_IMMO_VALUE_1 == fl_PEPS_Work_SSB_FailStatus_sigval_U8) && (fl_PEPS_Work_SSB_FailStatus_Last_U8 != fl_PEPS_Work_SSB_FailStatus_sigval_U8))
	||  ((PEPS_IMMO_VALUE_2 == fl_PEPS_Work_IMMO_MatchStatus_sigval_U8) && (fl_PEPS_Work_IMMO_MatchStatus_Last_U8 != fl_PEPS_Work_IMMO_MatchStatus_sigval_U8)))
	{
		Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_PEPS_SSB,TRUE);
	}
	else
	{
		//
	}

	fl_PEPS_Work_NoKeyFoundWarning_Last_U8 = fl_PEPS_Work_NoKeyFoundWarning_sigval_U8;
	fl_PEPS_Work_KeyOutWarning1_Last_U8 = fl_PEPS_Work_KeyOutWarning1_sigval_U8;
	fl_PEPS_Work_NotNWarning_Last_U8 = fl_PEPS_Work_NotNWarning_sigval_U8;
	fl_PEPS_Work_KeyBatteryWarning_Last_U8 = fl_PEPS_Work_KeyBatteryWarning_sigval_U8;
	fl_PEPS_Work_SSB_FailStatus_Last_U8 = fl_PEPS_Work_SSB_FailStatus_sigval_U8;
	fl_PEPS_Work_IMMO_MatchStatus_Last_U8 = fl_PEPS_Work_IMMO_MatchStatus_sigval_U8;
}


static void f_Chime_0x325_Peps_Work_On_Cycle_10s_Process(void)
{
    boolean fl_BrakeIndWrn_Chime_IsStarted      = (boolean)FALSE;
    boolean fl_BrakeIndWrn_Chime_IsElapsed      = (boolean)FALSE;
	
	uint8 fl_PEPS_Work_BrakeIndicationWarning_sigval_U8 = 0;
	uint8 fl_PEPS_Work_ClutchIndicationWarning_sigval_U8 = 0;

	switch(IsEngineCfg_AT_MT)
	{
		case VEHICLE_CHIME_AMT:
			//4-This is  on 3s 
			Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_BrakeIndicationWarning(&fl_PEPS_Work_BrakeIndicationWarning_sigval_U8);
			break;
		case VEHICLE_CHIME_5MT:
		case VEHICLE_CHIME_6MT:
			//5-This is  on 3s 
			Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_PEPS_ClutchIndicationWarning(&fl_PEPS_Work_ClutchIndicationWarning_sigval_U8);
			break;
		default:
			break;
	}
	
	if((PEPS_IMMO_VALUE_1 == fl_PEPS_Work_BrakeIndicationWarning_sigval_U8) ||(PEPS_IMMO_VALUE_1 == fl_PEPS_Work_ClutchIndicationWarning_sigval_U8))
	{
		if(TRUE == fl_BrakeIndWrn_Chime_First_Sts) //This is for  first IGN ON or message time out 
		{
			Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_PEPS_SSB,TRUE);
		}
		
		Rte_Call_rptimer_TmExt_Timer_IsStarted(eTimerPepsChime_BrakeIndWrn, &fl_BrakeIndWrn_Chime_IsStarted);
		Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerPepsChime_BrakeIndWrn, cTIMEOUT_CHIME_10S, &fl_BrakeIndWrn_Chime_IsElapsed);
	
		if(FALSE == fl_BrakeIndWrn_Chime_IsStarted)
		{
			Rte_Call_rptimer_TmExt_Timer_Start(eTimerPepsChime_BrakeIndWrn);
		}
		else
		{
			fl_TmExt_Timer_IsElapsed_Sts = TRUE;
			
			if(cTIMEOUT_CHIME_TRUE == fl_BrakeIndWrn_Chime_IsElapsed)
			{
				Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_PEPS_SSB,TRUE);
				
				Rte_Call_rptimer_TmExt_Timer_Stop(eTimerPepsChime_BrakeIndWrn);
				Rte_Call_rptimer_TmExt_Timer_Start(eTimerPepsChime_BrakeIndWrn);				
			}
			else
			{
				//
			}
		}
	}
	else
	{
		if(TRUE == fl_TmExt_Timer_IsElapsed_Sts)
		{
			Rte_Call_rptimer_TmExt_Timer_IsElapsed(eTimerPepsChime_BrakeIndWrn, cTIMEOUT_CHIME_10S, &fl_BrakeIndWrn_Chime_IsElapsed);
		}
		else
		{
			Rte_Call_rptimer_TmExt_Timer_Stop(eTimerPepsChime_BrakeIndWrn);
		}
	}
	
}


static void f_chime_Peps_Work_On_Update_Process(void)
{

	uint8 fl_CHIME_REQID_PEPS_SSB = FALSE;
	Rte_Write_ppSR_TIChimeMdl_Recv_ChimeId_PEPSChime(CHIME_REQID_PEPS_SSB);
	Rte_Read_rpSR_TIChimeMdl_Send_ToutValue_PEPSChime(&fl_CHIME_REQID_PEPS_SSB);
	if(TRUE == fl_CHIME_REQID_PEPS_SSB)
	{
		Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_PEPS_SSB,FALSE);
		
		fl_BrakeIndWrn_Chime_First_Sts = FALSE;
	}

}

static void f_0x325_PEPS_Active_On_initialState(void)
{
	//Active
	Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_D2_PEPS_SSB,FALSE);            //8

	fl_PEPS_Active_NoKeyFoundWarning_Last_U8 = 0; 
	fl_PEPS_Active_KeyOutWarning1_Last_U8 = 0; 
	fl_PEPS_Active_BrakeIndicationWarning_Last_U8 = 0; 
	fl_PEPS_Active_ClutchIndicationWarning_Last_U8 = 0; 
	fl_PEPS_Active_SSBChimeState_Last_U8 = 0; 

}

static void f_0x325_PEPS_Work_On_initialState(void)
{
	/*1-there are for PEPS */
	Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_PEPS_SSB,FALSE);             //8th
	Rte_Call_rptimer_TmExt_Timer_Stop(eTimerPepsChime_BrakeIndWrn);

	fl_TmExt_Timer_IsElapsed_Sts = FALSE;
	fl_BrakeIndWrn_Chime_First_Sts = TRUE;
	
	//Last Status
	fl_PEPS_Work_NoKeyFoundWarning_Last_U8 = 0;  //1
	fl_PEPS_Work_KeyOutWarning1_Last_U8 = 0;     //2
	fl_PEPS_Work_NotNWarning_Last_U8 = 0;        //3
	fl_PEPS_Work_KeyBatteryWarning_Last_U8 = 0;  //6
	fl_PEPS_Work_SSB_FailStatus_Last_U8 = 0;     //8
	fl_PEPS_Work_IMMO_MatchStatus_Last_U8 = 0;   //9
}

static void f_0x326_IMMO_Init(void)
{
	l_RF_keyLowBatteryWaring_status_u8 = FALSE;

	Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_PEPS_KEY_BAT,l_RF_keyLowBatteryWaring_status_u8);
}
static void f_0x318_IMMO_Init(void)
{
	l_PEPS_Work_IMMOChimeState_U8 = FALSE;
	fl_0x318_ImmoSts_State_Last_U8 = 0;

	Rte_Call_rpCS_PepsChime_UpdateStatus_Operation(CHIME_REQID_PEPS_IMMO,l_PEPS_Work_IMMOChimeState_U8);
}

static boolean f_0x325_PEPS_input_verify(void)
{
	boolean	f_ret = TRUE;
	
	uint8	fl_can_nvr_status_U8 = 0;
	uint8	fl_can_msg_status_U8 = 0;
	
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_NR(&fl_can_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_PEPS_0x325_ComIn_Tout(&fl_can_msg_status_U8);
	
	if((fl_can_nvr_status_U8 == RTE_E_NEVER_RECEIVED) || (fl_can_msg_status_U8 == RTE_E_TIMEOUT))
	{
		f_ret = FALSE;
	}
	
	return f_ret;
}


static boolean PEPS_PowerOn_3000MS_Self_Test_Action(void)
{
	 if(l_ProveoutCounter_U8 >= CHIME_DELAY_TIME_PEPS_3000MS)
    {
        l_Peps_Flag = FALSE;
    }
    else
    {
        l_Peps_Flag = TRUE;
	    l_ProveoutCounter_U8++;
    }

    return l_Peps_Flag;
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
