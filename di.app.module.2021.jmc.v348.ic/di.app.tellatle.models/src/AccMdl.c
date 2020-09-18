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
#include "Rte_AccMdl.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(AccMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================



//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8 TtAccWhiteStatus = cTT_TURN_OFF;
static uint8 TtAccGreenStatus = cTT_TURN_OFF;
static uint8 TtAccRedStatus = cTT_TURN_OFF;

static uint8 TtLdwStatus = cTT_TURN_OFF;

static uint8 TtBsd_CTA_OFF_Status = cTT_TURN_OFF;
static uint8 TtBsd_SOD_DOA_Status = cTT_TURN_OFF;

static uint8 TtAebRedStatus = cTT_TURN_OFF;
static uint8 TtAebYellowStatus = cTT_TURN_OFF;

static uint8 TtFcwRedStatus = cTT_TURN_OFF;
static uint8 TtFcwYellowStatus = cTT_TURN_OFF;

static uint8 TtAutoBeamStatus = cTT_TURN_OFF;

static boolean  l_ACCMDL_Flag = TRUE;
static uint8    l_Proveout_Counter_U8 = 0;

#define TT_ACC_VALUE_1        (1)
#define TT_ACC_VALUE_2        (2)
#define TT_ACC_VALUE_3        (3)
#define TT_ACC_VALUE_4        (4)
#define TT_ACC_VALUE_5        (5)
#define TT_ACC_VALUE_6        (6)
#define TT_ACC_VALUE_7        (7)

#define TT_LDW_VALUE_5        (5)
#define TT_LDW_VALUE_6        (6)
#define TT_LDW_VALUE_7        (7)

#define BSDOFFCTA_VALUE_1     (1)
#define BSDOFFSOD_DOA_VALUE_1 (1)

#define TT_AEB_VALUE_0        (0)
#define TT_AEB_VALUE_1        (1)

#define TT_FCW_VALUE_0        (0)
#define TT_FCW_VALUE_1        (1)

#define AUTO_BEAM_VALUE_3     (3)


#define ADAS_1 (1)
#define ADAS_2 (2)
#define ACC_2  (2)
#define FUNCTION_ACCMDL_TIME_30MS                (30)           
#define FUNCTION_ACCMDL_TIME_6500MS              (6500 / FUNCTION_ACCMDL_TIME_30MS) 

static void fttmdl_Acc_Process(void);
static void fttmdl_Acc_Init(void);

static void fttmdl_Ldw_Process(void);
static void fttmdl_Ldw_Init(void);

static void fttmdl_BsdOff_Process(void);
static void fttmdl_BsdOff_Init(void);

static void fttmdl_AEB_Process(void);
static void fttmdl_AEB_Init(void);

static void fttmdl_Fcw_Process(void);
static void fttmdl_Fcw_Init(void);

static void fttmdl_AutoBeam_Process(void);
static void fttmdl_AutoBeam_Init(void);

static boolean f_ACCMDL_PowerOn_Self_7s_Action(void);
static void fttmdl_AccMdl_All_Init(void);

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	fttmdl_AccMdl_All_Init();

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
	fttmdl_AccMdl_All_Init();

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
	fttmdl_AccMdl_All_Init();

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
	fttmdl_AccMdl_All_Init();

	return E_OK;
}

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
	uint8 fl_ign_substate = eIGN_OFF;
	uint8 IsEngineCfg_Adas = 0;
	uint8 IsEngineCfg_Acc = 0;
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ADAS,&IsEngineCfg_Adas);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Cruise,&IsEngineCfg_Acc);

	if(fl_ign_substate  == eIGN_RUN )
	{
		f_ACCMDL_PowerOn_Self_7s_Action();

		switch(IsEngineCfg_Adas)
		{
			case ADAS_1:
				fttmdl_Ldw_Process();
				fttmdl_BsdOff_Process();
				fttmdl_Fcw_Process();
				fttmdl_AutoBeam_Process();
				break;
			case ADAS_2:
				fttmdl_Ldw_Process();
				fttmdl_BsdOff_Process();
				fttmdl_AEB_Process();
				fttmdl_Fcw_Process();
				fttmdl_AutoBeam_Process();
				
				if(ACC_2 == IsEngineCfg_Acc)
				{
					fttmdl_Acc_Process();
				}
				break;
			default:
				break;
				
		}
	}
	else
	{
		fttmdl_AccMdl_All_Init();
	}

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

/*********************************************************************************
Function Name	  :  fttmdl_Acc_Process (void)
	
Description 	  :  ACC Telltale Logical
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/

static void fttmdl_Acc_Process(void)
{
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;	
	uint8 u8AccIconDisplay = 0;

	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode(&u8AccIconDisplay);

	if (RTE_E_NEVER_RECEIVED == u8MsgNR)
	{
		if(FALSE == l_ACCMDL_Flag)
		{
			TtAccWhiteStatus = cTT_TURN_OFF;
			TtAccGreenStatus = cTT_TURN_OFF;
			TtAccRedStatus = cTT_TURN_ON;
		}
	}
	else if (RTE_E_TIMEOUT == u8MsgTout)
	{
		TtAccWhiteStatus = cTT_TURN_OFF;
		TtAccGreenStatus = cTT_TURN_OFF;
		TtAccRedStatus = cTT_TURN_ON;
	}
	else
	{
		switch(u8AccIconDisplay)
		{
			// ACCMode ==1/2/4/5 White Turn On
			case TT_ACC_VALUE_1:
			case TT_ACC_VALUE_2:
			case TT_ACC_VALUE_4:
			case TT_ACC_VALUE_5:
				TtAccWhiteStatus = cTT_TURN_ON;
				TtAccRedStatus = cTT_TURN_OFF;
				TtAccGreenStatus = cTT_TURN_OFF;
				break;
			// ACCMode ==3/6 Green Turn On
			case TT_ACC_VALUE_3:
			case TT_ACC_VALUE_6:
				TtAccWhiteStatus = cTT_TURN_OFF;
				TtAccRedStatus = cTT_TURN_OFF;
				TtAccGreenStatus = cTT_TURN_ON;
				break;
			// ACCMode ==0x7 Red Turn On
			case TT_ACC_VALUE_7:
				TtAccWhiteStatus = cTT_TURN_OFF;
				TtAccRedStatus = cTT_TURN_ON;
				TtAccGreenStatus = cTT_TURN_OFF;
				break;
			default:
				TtAccWhiteStatus = cTT_TURN_OFF;
				TtAccRedStatus = cTT_TURN_OFF;
				TtAccGreenStatus = cTT_TURN_OFF;
				break;
		}
	}
	
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTACC_WHITE,TtAccWhiteStatus);
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTACC_RED,TtAccRedStatus);
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTACC_GREEN,TtAccGreenStatus);
}

/*********************************************************************************
Function Name	  :  fttmdl_Ldw_Process (void)
	
Description 	  :  LDW Telltale Logical
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/
static void fttmdl_Ldw_Process(void)
{
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;	
	uint8 u8LdwIconDisplay = 0;

	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status(&u8LdwIconDisplay);


	if(RTE_E_NEVER_RECEIVED == u8MsgNR)
	{
		if(FALSE == l_ACCMDL_Flag)
		{
			TtLdwStatus = cTT_TURN_ON;
		}
	}
	else if(RTE_E_TIMEOUT == u8MsgTout)
	{
		TtLdwStatus = cTT_TURN_ON;
	}
	else
	{
		/*IPM_LaneAssit_Status	0x5 || 0x6 || 0x7	On*/
		if((TT_LDW_VALUE_5 == u8LdwIconDisplay) || (TT_LDW_VALUE_6 == u8LdwIconDisplay) || (TT_LDW_VALUE_7 == u8LdwIconDisplay))
		{
			TtLdwStatus = cTT_TURN_ON;
		}
		else
		{
			TtLdwStatus = cTT_TURN_OFF;
		}	
	}
	
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTLDW,TtLdwStatus);
}

/*********************************************************************************
Function Name	  :  fttmdl_BsdOff_Process (void)
	
Description 	  :  BSD OFF Telltale Logical
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/
static void fttmdl_BsdOff_Process(void)
{
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;	
	uint8 u8BsdOffSodLcaIconDisplay = 0;
	uint8 u8BsdOffCtaIconDisplay = 0;
	uint8 u8BsdOffDoaIconDisplay = 0;

	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_Tout(&u8MsgTout);

	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_CTA_OFFTelltale(&u8BsdOffCtaIconDisplay);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SODLCA_OFFTelltale(&u8BsdOffSodLcaIconDisplay);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_DOA_OFFTelltale(&u8BsdOffDoaIconDisplay);

	
	if(RTE_E_NEVER_RECEIVED == u8MsgNR)
	{
		if(FALSE == l_ACCMDL_Flag)
		{
			TtBsd_CTA_OFF_Status = cTT_TURN_ON;
			TtBsd_SOD_DOA_Status = cTT_TURN_OFF;	
		}
	}
	else if(RTE_E_TIMEOUT == u8MsgTout)
	{
		TtBsd_CTA_OFF_Status = cTT_TURN_ON;
		TtBsd_SOD_DOA_Status = cTT_TURN_OFF;
	}
	else
	{
		/*Display in the Same place and priority：SODLCA_OFFTelltale<CTA_OFFTelltale*/
		if(BSDOFFCTA_VALUE_1 == u8BsdOffCtaIconDisplay)
		{
			TtBsd_CTA_OFF_Status = cTT_TURN_ON;
			TtBsd_SOD_DOA_Status = cTT_TURN_OFF;
		}
		else if((BSDOFFSOD_DOA_VALUE_1 == u8BsdOffSodLcaIconDisplay) || (BSDOFFSOD_DOA_VALUE_1 == u8BsdOffDoaIconDisplay) )
		{
			TtBsd_CTA_OFF_Status = cTT_TURN_OFF;
			TtBsd_SOD_DOA_Status = cTT_TURN_ON;
		}
		else
		{
			TtBsd_CTA_OFF_Status = cTT_TURN_OFF;
			TtBsd_SOD_DOA_Status = cTT_TURN_OFF;
		}
	}
	
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTBSD_SODLCA,TtBsd_CTA_OFF_Status);
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTBSD_CTA,TtBsd_SOD_DOA_Status);
}

/*********************************************************************************
Function Name	  :  fttmdl_AEB_Process (void)
	
Description 	  :  AEB Telltale Logical
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/
static void fttmdl_AEB_Process(void)
{
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;	
	uint8 u8AebIconDisplay = 0;

	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_AEB_STATE(&u8AebIconDisplay);

	if(RTE_E_NEVER_RECEIVED == u8MsgNR)
	{
		if(FALSE == l_ACCMDL_Flag)
		{
			TtAebRedStatus = cTT_TURN_ON;
			TtAebYellowStatus = cTT_TURN_OFF;
		}
	}
	else if(RTE_E_TIMEOUT == u8MsgTout)
	{
		TtAebRedStatus = cTT_TURN_ON;
		TtAebYellowStatus = cTT_TURN_OFF;
	}
	else
	{
		
		/*AEB_STATE = 0X0  display red AEB icon*/
		if(TT_AEB_VALUE_0 == u8AebIconDisplay)
		{
			TtAebRedStatus = cTT_TURN_ON;
			TtAebYellowStatus = cTT_TURN_OFF;
		}
		/*AEB_STATE = 0X1  display yellow AEB icon*/
		else if(TT_AEB_VALUE_1 == u8AebIconDisplay)
		{
			TtAebYellowStatus = cTT_TURN_ON;
			TtAebRedStatus = cTT_TURN_OFF;
		}
		else
		{
			TtAebRedStatus = cTT_TURN_OFF;
			TtAebYellowStatus = cTT_TURN_OFF;
		}
	}

	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTAEB_YELLOW,TtAebYellowStatus);
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTAEB_RED,TtAebRedStatus);
}

/*********************************************************************************
Function Name	  :  fttmdl_Fcw_Process (void)
	
Description 	  :  FCW Telltale Logical
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/
static void fttmdl_Fcw_Process(void)
{
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;	
	uint8 u8PcwIconDisplay = 0;

	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_STATE(&u8PcwIconDisplay);

	if(RTE_E_NEVER_RECEIVED == u8MsgNR)
	{
		if(FALSE == l_ACCMDL_Flag)
		{
			TtFcwRedStatus = cTT_TURN_ON;
			TtFcwYellowStatus = cTT_TURN_OFF;
		}
	}
	else if(RTE_E_TIMEOUT == u8MsgTout)
	{
		TtFcwRedStatus = cTT_TURN_ON;
		TtFcwYellowStatus = cTT_TURN_OFF;
	}
	else
	{
		
		/* PCW_STATE = 0x0  display red Fcw icon*/
		if(TT_FCW_VALUE_0 == u8PcwIconDisplay)
		{
			TtFcwRedStatus = cTT_TURN_ON;
			TtFcwYellowStatus = cTT_TURN_OFF;
		}
		/* PCW_STATE = 0x1  display yellow Fcw icon*/
		else if(TT_FCW_VALUE_1 == u8PcwIconDisplay)
		{
			TtFcwYellowStatus = cTT_TURN_ON;
			TtFcwRedStatus = cTT_TURN_OFF;
		}
		else
		{
			TtFcwRedStatus = cTT_TURN_OFF;
			TtFcwYellowStatus = cTT_TURN_OFF;
		}
		
	}

	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTFCW_YELLOW,TtFcwYellowStatus);
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTFCW_RED,TtFcwRedStatus);
}

/*********************************************************************************
Function Name	  :  fttmdl_AutoBeam_Process (void)
	
Description 	  :  AutoBeam Telltale Logical
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/
static void fttmdl_AutoBeam_Process(void)
{
	uint8 u8MsgNR = 0;
	uint8 u8MsgTout = 0;	
	uint8 u8AutoBeamIconDisplay = 0;

	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_HMA_Status(&u8AutoBeamIconDisplay);

	if ((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
	{
		TtAutoBeamStatus = cTT_TURN_OFF;
	}
	else
	{
		/*IPM_HMA_Status== 0x3 -> On    else -> OFF*/
		if(AUTO_BEAM_VALUE_3 == u8AutoBeamIconDisplay)
		{
			TtAutoBeamStatus = cTT_TURN_ON;
		}
		else
		{
			TtAutoBeamStatus = cTT_TURN_OFF;
		}
	}
	
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTADAS_AUTO,TtAutoBeamStatus);
}


/*********************************************************************************
Function Name	  :  fttmdl_Acc_Init (void)
	
Description 	  :  ACC Telltale InitFunction
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/
static void fttmdl_Acc_Init(void)
{
	TtAccWhiteStatus = cTT_TURN_OFF;
	TtAccRedStatus = cTT_TURN_OFF;
	TtAccGreenStatus = cTT_TURN_OFF;
	
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTACC_WHITE,TtAccWhiteStatus);
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTACC_RED,TtAccRedStatus);
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTACC_GREEN,TtAccGreenStatus);
}

/*********************************************************************************
Function Name	  :  fttmdl_Ldw_Init (void)
	
Description 	  :  LDW Telltale InitFunction
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/
static void fttmdl_Ldw_Init(void)
{
	TtLdwStatus = cTT_TURN_OFF;
	
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTLDW,TtLdwStatus);
}

/*********************************************************************************
Function Name	  :  fttmdl_BsdOff_Init (void)
	
Description 	  :  BSD OFF Telltale InitFunction
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/
static void fttmdl_BsdOff_Init(void)
{
	TtBsd_CTA_OFF_Status = cTT_TURN_OFF;
	TtBsd_SOD_DOA_Status = cTT_TURN_OFF;
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTBSD_SODLCA,TtBsd_CTA_OFF_Status);
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTBSD_CTA,TtBsd_SOD_DOA_Status);
}

/*********************************************************************************
Function Name	  :  fttmdl_AEB_Init (void)
	
Description 	  :  AEB Telltale InitFunction
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/
static void fttmdl_AEB_Init(void)
{
	TtAebRedStatus = cTT_TURN_OFF;
	TtAebYellowStatus = cTT_TURN_OFF;
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTAEB_YELLOW,TtAebYellowStatus);
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTAEB_RED,TtAebRedStatus);
}

/*********************************************************************************
Function Name	  :  fttmdl_Fcw_Init (void)
	
Description 	  :  FCW Telltale InitFunction
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/
static void fttmdl_Fcw_Init(void)
{
	TtFcwRedStatus = cTT_TURN_OFF;
	TtFcwYellowStatus = cTT_TURN_OFF;
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTFCW_YELLOW,TtFcwYellowStatus);
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTFCW_RED,TtFcwRedStatus);
}

/*********************************************************************************
Function Name	  :  fttmdl_AutoBeam_Init (void)
	
Description 	  :  AutoBeam Telltale InitFunction
	
Parameters		  : None
	
Return Value	  : None
	
Critical Section  : None
***********************************************************************************/
static void fttmdl_AutoBeam_Init(void)
{
	TtAutoBeamStatus = cTT_TURN_OFF;
	Rte_Call_rpCS_AccMdl_TIMdlUpdateTtStatus_Operation(cTTADAS_AUTO,TtAutoBeamStatus);
}

static boolean f_ACCMDL_PowerOn_Self_7s_Action(void)
{
    if(l_Proveout_Counter_U8 >= FUNCTION_ACCMDL_TIME_6500MS)
    {
        l_ACCMDL_Flag = FALSE;
    }
    else
    {
        l_ACCMDL_Flag = TRUE;
		l_Proveout_Counter_U8++;
    }
	
    return l_ACCMDL_Flag;
}
static void fttmdl_AccMdl_All_Init(void)
{
	fttmdl_Acc_Init();
	fttmdl_Ldw_Init();
	fttmdl_BsdOff_Init();
	fttmdl_AEB_Init();
	fttmdl_Fcw_Init();
	fttmdl_AutoBeam_Init();
	
	l_ACCMDL_Flag = TRUE;
	l_Proveout_Counter_U8 = FALSE;
}

