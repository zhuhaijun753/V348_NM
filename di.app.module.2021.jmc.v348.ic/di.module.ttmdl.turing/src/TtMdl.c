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
#include "Rte_TtMdl.h"
#include <TmExt_Cfg.h>
#include <TmExt_Timer.h>
#include "CmpLib.h"
#include "TtMdl.h"
#include "TtMdl_Cfg.h"
#include "TtCfg.h"
#include "system.h"
/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(TtMdl)
//=================================================================================//
//  CONSTANTS & TYPES//
//=================================================================================//
static	uint8 IsEngineCfg 	   = FALSE;
static	uint8 IsEngineCfg_Engine = FALSE;
static	uint8 IsEngineCfg_PASSENGER_BELT = FALSE;
static	uint8 IsEngineCfg_PEPS = FALSE;
static	uint8 IsEngineCfg_Acc = FALSE;
static	uint8 IsEngineCfg_STT = FALSE;
static	uint8 IsEngineCfg_4WHL = FALSE;
static	uint8 IsEngineCfg_EPS = FALSE;
static	uint8 IsATCfg_GearBox = FALSE;
static	uint8 ATS_FLAG = FALSE;
static	uint8 STT_YELLOW_FALG = FALSE;
static	uint8 STT_GREEN_FALG = FALSE;
static	uint8 ESP_RED_FALG = FALSE;
static	uint8 IsEngineCfg_Abs = FALSE;
static	uint8 IsEngineCfg_Airbag = FALSE;
static	uint8 IsEngineCfg_Tpms = FALSE;
static	uint8 IsEngineCfg_Esp = FALSE;
static	uint8 IsEngineCfg_SCR = FALSE;
static	uint8 IsEngineCfg_Brake_type = FALSE;
static	uint8 IsEngineCfg_Adas = FALSE;
static	uint8 IsEngineCfg_Overspeed = FALSE;

#define TELLTALE_ADAS_1             (1)
#define TELLTALE_ADAS_2             (2)
#define TELLTALE_CRUISE_1           (1)
#define TELLTALE_ACC_2              (2)
#define TELLTALE_BRAKE_TYPE_0       (0)
#define TELLTALE_BRAKE_TYPE_1       (1)
#define TELLTALE_EMISSION_LEVELS_SIX (5)
#define TELLTALE_VALUE_IMMO_1       (1)
#define TELLTALE_VALUE_PEPS_2       (2)
#define TELLTALE_VEHICLE_TPMS_1     (1)
#define TELLTALE_VEHICLE_TPMS_3     (3)
#define TELLTALE_VEHICLE_GEARBOX_AMT (2)
#define TELLTALE_OVERSPEED_1         (1)
#define TELLTALE_OVERSPEED_2         (2)
#define TELLTALE_OVERSPEED_3         (3)

#if 0
/* Apb and Avh cut Not Count*/
static uint8 ApbStatePrev = 0;
static uint8 AVHStatePrev = 0;
static uint8 ApbStateSet = 1;
static uint8 AVHStateSet = 1;
static uint8 firstflag = 0;
#endif

//static uint16 TT_Demo_ON_Count = 0;


static uint32 fl_avh_status_U32_count1 = 0;

static uint32 fl_epb_status_U32_count1 = 0;
static uint32 fl_epb_status_U32_count2 = 0;
static uint32 fl_epb_status_U32_count3 = 0;
static uint32 fl_epb_status_U32_count4 = 0;
static uint32 fl_epb_status_U32_count5 = 0;
static uint32 fl_epb_status_U32_count6 = 0;
static uint32 fl_epb_status_U32_count7 = 0;
static uint32 fl_epb_status_U32_count8 = 0;
static uint32 fl_epb_status_U32_count9 = 0;
static uint32 fl_epb_status_U32_count10 = 0;
static uint32 fl_epb_status_U32_count11 = 0;
static uint32 fl_epb_status_U32_count12 = 0;
static uint32 fl_epb_status_U32_count13 = 0;
static uint32 fl_epb_status_U32_count14 = 0;
static uint32 fl_epb_status_U32_count15 = 0;
//static uint32 fl_epb_status_U32_count16 = 0;

#define	APB_AVH_CALLED_TELLTALE (30) //ms
#define APB_AVH_TIME_1000MS     (1000 / APB_AVH_CALLED_TELLTALE)
#define APB_AVH_TIME_3600S      (3600 * APB_AVH_TIME_1000MS)
#define APB_AVH_TIME_720HOUR    (30 * 24 * APB_AVH_TIME_3600S)
#define APB_AVH_TIME_180DAY     (6 * APB_AVH_TIME_720HOUR)

#define AVH_TT_STATUS_VALUE_1   (1)

#define APB_TT_STATUS_VALUE_0   (0)
#define APB_TT_STATUS_VALUE_1   (1)
#define APB_TT_STATUS_VALUE_2   (2)
#define APB_TT_STATUS_VALUE_3   (3)
#define APB_TT_STATUS_VALUE_4   (4)
#define APB_TT_STATUS_VALUE_5   (5)
#define APB_TT_STATUS_VALUE_6   (6)
#define APB_TT_STATUS_VALUE_7   (7)
#define APB_TT_STATUS_VALUE_8   (8)
#define APB_TT_STATUS_VALUE_9   (9)
#define APB_TT_STATUS_VALUE_10  (10)
#define APB_TT_STATUS_VALUE_11  (11)
#define APB_TT_STATUS_VALUE_12  (12)
#define APB_TT_STATUS_VALUE_13  (13)
#define APB_TT_STATUS_VALUE_14  (14)
#define APB_TT_STATUS_VALUE_15  (15)
//#define APB_TT_STATUS_VALUE_16  (16)

#define APB_MESSAGE_STATUS_VALUE_0   (0)
#define APB_MESSAGE_STATUS_VALUE_1   (1)
#define APB_MESSAGE_STATUS_VALUE_2   (2)

static uint8 l_cTTAPB_Last_Status = TRUE;
static uint8 l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_0;


#define CTT_MANAGER_PROVEOUT_TIMER_30S (30000) //IGN OFF time 30s flag

/*============================================================================
**      I N T E R N A L   F U N C T I O N   P R O T O T Y P E S
**==========================================================================*/
static void CTtMdl_f_tt_timedtask(void);
static void hmi_TT_Check_handler(void);
static void Update_APB_AVH_Status(void);
static void TT_Demo_Test_Process(void);
static void TT_DEMO_TEST_TURN_OFF(void);
static void TT_Demo_Test_ALL_TURN_OFF(void);
static void TT_Demo_Test_Process_Init(void);
static void Telltale_fdiag_app_get_vehicle_cfg(void);



/*============================================================================
**                M E M O R Y   A L L O C A T I O N
**==========================================================================*/
/*
** Description: Down counter used to determine proveout status.  This variable
**              is decremented each time through f_tt_timedtask until it
**              reaches zero. If proveout is maintained externally, this
**              does not exist.
** Range:       PROVEOUT_TIME to 0. Once at 0, the variable is not decremented
**              further.
** Init:        Set to PROVEOUT_TIME at Cold Init and Transition into run/str.
**              Set to 0 at Transition into off/acc.
*/
#if (cUSE_TT_MANAGER_PROVEOUT != 0)
    boolean    TTtMdlIsProveoutDone = TRUE;             //TRUE:be being check ; FALSE: check finish
    boolean	   TTtMdlIsCheckStateProveoutDone = FALSE;  //TRUE:be being animation ; FALSE: animation finish
	//static uint16 TtMdlTestTimer;//TtMdl task period= 30ms
	static uint8 fl_IgnQuickSwitchCnt = 20;
	static uint8 last_TtMdlIgnSts = eIGN_OFF;
	static boolean IGN_OFF_timer_30s_flag = FALSE;

#endif
/*
** Description: Array of structure TELLTALE_TABLE_ENTRY.  Each element in this
**              array represents a telltale. The contents of this table are
**              configured in ftt.cfg. This table is traversed each f_tt_timedtask
**              call to process the telltales.  Also, this table is used by the
**              f_tt_all_on and f_tt_all_off routines.
** Range:       N/A.
** Init:        const table, initialized at link time.
*/
static const uint8 telltales_attrib[cTT_NUM_ENTRIES_TELLTALE_CONFIG] =
{
   cPRODUCT_SPECIFIC_TELLTALE_ATTRIB_CONFIG
};

static type_led_proveout telltale_attrib_proveout[cTT_NUM_ENTRIES_TELLTALE_CONFIG] =
{
	cPRODUCT_SPECIFIC_TELLTALE_ATTRIB_CONFIG_PROVEOUT
};
// static uint8 TtMdlIgnitionStatePrev;
// static uint8 TtMdlPrvOIgnitionStatePrev = eIGN_OFF;

static uint8 TtMdlTtSts[cTT_NUM_ENTRIES_TELLTALE_CONFIG];

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================


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
	uint8 i;
	#if (cUSE_TT_MANAGER_PROVEOUT != 0)
	TTtMdlIsProveoutDone           = TRUE;
	TTtMdlIsCheckStateProveoutDone = FALSE;  
	Rte_Write_ppSR_TITtMdl_ProveoutDone_ValueDone(TTtMdlIsProveoutDone);
	Rte_Call_rpTimer_TmExt_Timer_Start(eEPBWarnTimer);
	//TtMdlTestTimer =0;
	#endif
	for(i=0;i<cTT_NUM_ENTRIES_TELLTALE_CONFIG;i++)
	{
		TtMdlTtSts[i]=0;
	}
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
	uint8 i;
	#if (cUSE_TT_MANAGER_PROVEOUT != 0)
	TTtMdlIsProveoutDone           = TRUE;
	TTtMdlIsCheckStateProveoutDone = FALSE; 
	Rte_Write_ppSR_TITtMdl_ProveoutDone_ValueDone(TTtMdlIsProveoutDone);
	//TtMdlTestTimer =0;
	#endif
	for(i=0;i<cTT_NUM_ENTRIES_TELLTALE_CONFIG;i++)
	{
		TtMdlTtSts[i]=0;
	}
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
	#if (cUSE_TT_MANAGER_PROVEOUT != 0)
	TTtMdlIsProveoutDone           = TRUE;
	TTtMdlIsCheckStateProveoutDone = FALSE; 	
	Rte_Write_ppSR_TITtMdl_ProveoutDone_ValueDone(TTtMdlIsProveoutDone);
	//TtMdlTestTimer =0;
	#endif
    return  E_OK;
}
#if 0
static void CTtMdl_f_tt_ProveOutBatAbn2NrmProcess(void)
{
/*(cUSE_TT_MANAGER_PROVEOUT != 0)*/
	uint8 TtMdlPrbOIgnSts;

	static uint32 PrvORemnTime = 0;
	uint8 PrvOTimerElpsed = 0;
	uint8 PrvOTimerStarted = 0;
	uint8 LVI_HVI_status;
	static uint8 prev_LVI_HVI_status;
	(void)Rte_Read_rpBattState_BatteryState(&LVI_HVI_status);
	(void)Rte_Read_rpIgnState_IGNState(&TtMdlPrbOIgnSts);
	if((TtMdlPrvOIgnitionStatePrev == eIGN_OFF) && (TtMdlPrbOIgnSts == eIGN_RUN))
	{
		if((prev_LVI_HVI_status == eBatteryState_Normal) || (prev_LVI_HVI_status == eBatteryState_UnderVoltagePhase1))
		{
			if ((LVI_HVI_status == eBatteryState_UnderVoltage) || (LVI_HVI_status == eBatteryState_OverVoltage)
			||(LVI_HVI_status == eBatteryState_OverVoltagePhase1))
			{
				Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTelltaleProveOutTimer, cTT_MANAGER_PROVEOUT_TIME_IN_MS,&PrvOTimerElpsed);
				if(((uint8)1u) == PrvOTimerElpsed)
				{
					PrvORemnTime = 0;
				}
				else 
				{
					Rte_Call_rpTimer_TmExt_Timer_GetElapsedTime(eTelltaleProveOutTimer,&PrvORemnTime);
					PrvORemnTime = cTT_MANAGER_PROVEOUT_TIME_IN_MS - PrvORemnTime;
					Rte_Call_rpTimer_TmExt_Timer_Stop(eTelltaleProveOutTimer);
				}
			}
			else
			{
				if(0!=PrvORemnTime)
				{
					/*Previous state will be other than RUN state*/
					Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTelltaleProveOutTimer,&PrvOTimerStarted);
					if(((uint8)1u) != PrvOTimerStarted)/* Proveout Timer is not Started  */
					{
						Rte_Call_rpTimer_TmExt_Timer_Start(eTelltaleProveOutTimer);
						TTtMdlIsProveoutDone = FALSE;
					}
					if(eIGN_RUN != TtMdlPrbOIgnSts)
					{
						//TmExt_Timer_Stop(eTelltaleProveOut);
						Rte_Call_rpTimer_TmExt_Timer_Stop(eTelltaleProveOutTimer);
						TTtMdlIsProveoutDone = TRUE;
					}
					Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTelltaleProveOutTimer,&PrvOTimerStarted);
					if(((uint8)1u) == PrvOTimerStarted)
					{
						Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTelltaleProveOutTimer, (PrvORemnTime + 2000),&PrvOTimerElpsed);
						if((FALSE == TTtMdlIsProveoutDone)&&
							(((uint8)1u) == PrvOTimerElpsed))
						{
							/* Proveout Timer is Expired */
							TTtMdlIsProveoutDone = TRUE;
							Rte_Call_rpTimer_TmExt_Timer_Stop(eTelltaleProveOutTimer);
							PrvORemnTime = 0;
						}
					}
				}
				else 
				{
					
					
				}
			}
			TtMdlPrvOIgnitionStatePrev = TtMdlPrbOIgnSts;
		}
	}
	
	/* Preserve the Ignition Status */
	
	prev_LVI_HVI_status = LVI_HVI_status;

}
#endif

static void hmi_TT_Check_handler(void)
{
	int i;
	DCM_IOC_Type DID_Value = {0};
	(void)Rte_Read_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(&DID_Value);
	
	if(DID_Value.DID_IOC_Parament == 0x03)
	{	
		//BYTE_0 /********************************/
		switch((DID_Value.DID_IOC_State[0]) & BIT0)
		{
			case 0x01:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTLEFT_TURN,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTLEFT_TURN,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[0]) & BIT1)
		{
			case 0x02:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTPOSITION_LAMP,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTPOSITION_LAMP,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[0]) & BIT2)
		{
			//reserve LOWBEAM
		}
		switch((DID_Value.DID_IOC_State[0]) & BIT3)
		{
			case 0x08:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTHIGH_BEAM,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTHIGH_BEAM,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[0]) & BIT4)
		{
			case 0x10:
				if((TELLTALE_ADAS_1 == IsEngineCfg_Adas) || (TELLTALE_ADAS_2 == IsEngineCfg_Adas))
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTADAS_AUTO,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTADAS_AUTO,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTADAS_AUTO,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[0]) & BIT5)
		{
			case 0x20:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTFRONT_FOG,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTFRONT_FOG,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[0]) & BIT6)
		{
			case 0x40:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTREAR_FOG,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTREAR_FOG,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[0]) & BIT7)
		{
			case 0x80:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTRIGHT_TURN,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTRIGHT_TURN,cTT_TURN_OFF);
				break;
		}

		//BYTE_1 /********************************/
		switch((DID_Value.DID_IOC_State[1]) & BIT0)
		{
			case 0x01:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTDRIVER_SEATBELT_REMINDER,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTDRIVER_SEATBELT_REMINDER,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[1]) & BIT1)
		{
			case 0x02:
				if((TELLTALE_VALUE_IMMO_1 == IsEngineCfg_PEPS) || (TELLTALE_VALUE_PEPS_2 == IsEngineCfg_PEPS))
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTIMMO,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTIMMO,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTIMMO,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[1]) & (BIT2 | BIT3 | BIT4))
		{
			case 0x04:
			case 0x08:
			case 0x0C:
				if(TELLTALE_ACC_2 == IsEngineCfg_Acc)
				{
					if(TELLTALE_ADAS_2 == IsEngineCfg_Adas)
					{
						Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_GREEN,cTT_TURN_ON);
						Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_WHITE,cTT_TURN_ON);
						Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_RED,cTT_TURN_ON);
					}
					else
					{
						Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_GREEN,cTT_TURN_OFF);
						Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_WHITE,cTT_TURN_OFF);
						Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_RED,cTT_TURN_OFF);
					}
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_GREEN,cTT_TURN_OFF);
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_WHITE,cTT_TURN_OFF);
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_RED,cTT_TURN_OFF);
				}
				break;
			case 0x10:
			case 0x14:
				if(TELLTALE_CRUISE_1 == IsEngineCfg_Acc)
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_CONTROL,cTT_TURN_ON);
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_RED,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_CONTROL,cTT_TURN_OFF);
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_RED,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_GREEN,cTT_TURN_OFF);
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_WHITE,cTT_TURN_OFF);
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTACC_RED,cTT_TURN_OFF);
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_CONTROL,cTT_TURN_OFF);
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTCRUISE_RED,cTT_TURN_OFF);
				break;
	
		}
		switch((DID_Value.DID_IOC_State[1]) & BIT5)
		{
			case 0x20:
				if((TELLTALE_OVERSPEED_1 == IsEngineCfg_Overspeed) || (TELLTALE_OVERSPEED_2 == IsEngineCfg_Overspeed) 
				|| (TELLTALE_OVERSPEED_3 == IsEngineCfg_Overspeed))
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(CTT_OVERSPEED,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(CTT_OVERSPEED,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(CTT_OVERSPEED,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[1]) & BIT6)
		{

			case 0x40:
				if(TRUE == IsEngineCfg_Airbag) 
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAIRBAG,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAIRBAG,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAIRBAG,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[1]) & BIT7)
		{
			case 0x80:
				//Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTPASSENGER_SEATBELT_REMINDER,cTT_TURN_ON);
				break;
			default:
				//Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTPASSENGER_SEATBELT_REMINDER,cTT_TURN_OFF);
				break;
		}
		
		//BYTE_2 /********************************/
		switch((DID_Value.DID_IOC_State[2]) & BIT0)
		{
			case 0x01:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTOIL_WATER,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTOIL_WATER,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[2]) & BIT1)
		{
			case 0x02:
				//Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_YELLOW,cTT_TURN_ON);
				break;
			default:
				//Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_YELLOW,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[2]) & BIT2)
		{
			case 0x04:
				//Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GREEN,cTT_TURN_ON);
				break;
			default:
				//Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTSTT_GREEN,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[2]) & BIT3)
		{
			case 0x08:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_OIL_PRESSURE,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_OIL_PRESSURE,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[2]) & BIT4)
		{
			case 0x10:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTGLOW,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTGLOW,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[2]) & BIT5)
		{
			case 0x20:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTLOW_FUEL,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[2]) & BIT6)
		{
			case 0x40:
				if((TELLTALE_ADAS_1 == IsEngineCfg_Adas) || (TELLTALE_ADAS_2 == IsEngineCfg_Adas))
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTLDW,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTLDW,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTLDW,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[2]) & BIT7)
		{
			case 0x80:
				//Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTHDC,cTT_TURN_ON);
				break;
			default:
				//Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTHDC,cTT_TURN_OFF);
				break;
		}

		//BYTE_3 /********************************/
		switch((DID_Value.DID_IOC_State[3]) & (BIT0 | BIT1))	
		{
			case 0x01:
				if(TELLTALE_BRAKE_TYPE_1 == IsEngineCfg_Brake_type)
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAVH,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAVH,cTT_TURN_OFF);
				}
				break;
			case 0x02:
				if((TELLTALE_BRAKE_TYPE_0 == IsEngineCfg_Brake_type) || (TELLTALE_BRAKE_TYPE_1 == IsEngineCfg_Brake_type))
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAVH,cTT_TURN_OFF);
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[3]) & BIT2)	
		{
			case 0x04:
				if(TELLTALE_EMISSION_LEVELS_SIX == IsEngineCfg_SCR)
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTGPF,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[3]) & BIT3)	
		{
			case 0x08:
				if(TELLTALE_BRAKE_TYPE_1 == IsEngineCfg_Brake_type)
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAPB_FAULT,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[3]) & BIT4)	
		{
			case 0x10:
				if(TELLTALE_VEHICLE_GEARBOX_AMT == IsATCfg_GearBox)
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTGear_Box,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTGear_Box,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTGear_Box,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[3]) & BIT5)	
		{
			case 0x20:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTOBD,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTOBD,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[3]) & BIT6)	
		{
			case 0x40:
				if((TELLTALE_VALUE_IMMO_1 == IsEngineCfg_PEPS) || (TELLTALE_VALUE_PEPS_2 == IsEngineCfg_PEPS))
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTPEPS_FAULT,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTPEPS_FAULT,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTPEPS_FAULT,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[3]) & BIT7)	
		{
			case 0x80:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTEBD,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTEBD,cTT_TURN_OFF);
				break;
		}
		
		//BYTE_4 /********************************/
		switch((DID_Value.DID_IOC_State[4]) & BIT0)	
		{
			case 0x01:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTCOOLANT_TEMP_HOT,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[4]) & BIT1)	
		{
			case 0x02:
				if(TRUE ==	IsEngineCfg_Abs)
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTABS_SYSTEM_FAILURE,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTABS_SYSTEM_FAILURE,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTABS_SYSTEM_FAILURE,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[4]) & BIT2)	
		{
			case 0x04:
				if(TRUE == IsEngineCfg_Esp)
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTESP,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTESP,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTESP,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[4]) & BIT3)	
		{
			case 0x08:
				if((TELLTALE_ADAS_1 == IsEngineCfg_Adas) || (TELLTALE_ADAS_2 == IsEngineCfg_Adas))
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTBSD_SODLCA,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTBSD_SODLCA,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTBSD_SODLCA,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[4]) & BIT4)	
		{
			case 0x10:
				if(TRUE == IsEngineCfg_Esp)
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTESP_OFF,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTESP_OFF,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTESP_OFF,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[4]) & BIT5)	
		{
			case 0x20:
				if((TELLTALE_ADAS_1 == IsEngineCfg_Adas) || (TELLTALE_ADAS_2 == IsEngineCfg_Adas))
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTBSD_CTA,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTBSD_CTA,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTBSD_CTA,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[4]) & BIT6)	
		{
			case 0x40:
				if((TELLTALE_VEHICLE_TPMS_1 == IsEngineCfg_Tpms) || (TELLTALE_VEHICLE_TPMS_3 == IsEngineCfg_Tpms))
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTTPMS_FAILURE,cTT_TURN_ON);
				}
				else
				{
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTTPMS_FAILURE,cTT_TURN_OFF);
				}
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTTPMS_FAILURE,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[4]) & BIT7)	
		{
			case 0x80:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTCHARGE,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTCHARGE,cTT_TURN_OFF);
				break;
		}
		
		//BYTE_5 /********************************/
		if((TELLTALE_ADAS_1 == IsEngineCfg_Adas) || (TELLTALE_ADAS_2 == IsEngineCfg_Adas))
		{
			switch((DID_Value.DID_IOC_State[5]) & (BIT0 | BIT1))
			{
				case 0x01:
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAEB_YELLOW,cTT_TURN_ON);
					break;
				case 0x02:
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAEB_RED,cTT_TURN_ON);
					break;
				default:
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAEB_YELLOW,cTT_TURN_OFF);
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAEB_RED,cTT_TURN_OFF);
					break;
			}
			switch((DID_Value.DID_IOC_State[5]) & (BIT2 | BIT3) )
			{
				case 0x04:
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTFCW_YELLOW,cTT_TURN_ON);
					break;
				case 0x08:
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTFCW_RED,cTT_TURN_ON);
					break;
				default:
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTFCW_YELLOW,cTT_TURN_OFF);
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTFCW_RED,cTT_TURN_OFF);
					break;
			}
			switch((DID_Value.DID_IOC_State[5]) & (BIT4 | BIT5) )
			{
				case 0x10:
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_YLW,cTT_TURN_ON);
					break;
				case 0x20:
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_RED,cTT_TURN_ON);
					break;
				default:
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_YLW,cTT_TURN_OFF);
					Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_RED,cTT_TURN_OFF);
					break;
			}
		}
		else
		{
			Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAEB_YELLOW,cTT_TURN_OFF);
			Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAEB_RED,cTT_TURN_OFF);
			Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTFCW_YELLOW,cTT_TURN_OFF);
			Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTFCW_RED,cTT_TURN_OFF);
			Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_YLW,cTT_TURN_OFF);
			Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTEPS_RED,cTT_TURN_OFF);
		}
		switch((DID_Value.DID_IOC_State[5]) & BIT6)	
		{
			case 0x40:
				//Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAUTO_HOLD,cTT_TURN_ON);
				break;
			default:
				//Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTAUTO_HOLD,cTT_TURN_OFF);
				break;
		}
		switch((DID_Value.DID_IOC_State[5]) & BIT7)	
		{
			case 0x80:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTSVS,cTT_TURN_ON);
				break;
			default:
				Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(cTTSVS,cTT_TURN_OFF);
				break;
		}
	}
	else
	{
		for(i=0;i<cTT_NUM_ENTRIES_TELLTALE_CONFIG;i++)
		{
			(void)Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(i,cTT_TURN_OFF);
		}
	}

}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
#if (cUSE_TT_MANAGER_PROVEOUT != 0)

	uint8 TtMdlIgnSts;
	uint8 Timer_status = 0;
	uint8 fl_animationFlag = TRUE;

	uint8 i = 0;
	DCM_IOC_Type DID_Value = {0};
	Dcm_SesCtrlType fl_diag_session = DEFAULT_SESSION;
	static Dcm_SesCtrlType flSession_pre = DEFAULT_SESSION;
	(void)Rte_Read_rpSR_TIDcm_DID_IOC_DCM_IOC_Value(&DID_Value);
	Rte_Call_rpCS_GetCurrentSessionType_Operation(&fl_diag_session);

	(void)Rte_Read_rpIgnState_IGNState(&TtMdlIgnSts);
	// CTtMdl_f_tt_ProveOutBatAbn2NrmProcess();
	//TT_Demo_ON_Count++;


	switch (TtMdlIgnSts)
	{
		case eIGN_RUN:
			Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);
			if ((FALSE == fl_animationFlag) && (fl_IgnQuickSwitchCnt == 0))
			{
				if(TRUE == TTtMdlIsCheckStateProveoutDone) // Animation Finish
				{
					TTtMdlIsProveoutDone           = FALSE;
					TTtMdlIsCheckStateProveoutDone = FALSE;
					Rte_Call_rpTimer_TmExt_Timer_Start(eTelltaleProveOutTimer);
					Rte_Write_ppSR_TITtMdl_ProveoutDone_ValueDone(TTtMdlIsProveoutDone);
				}
			}
			else
			{
				TTtMdlIsProveoutDone               = TRUE;
				TTtMdlIsCheckStateProveoutDone     = TRUE;
				Rte_Call_rpTimer_TmExt_Timer_Stop(eTelltaleProveOutTimer);
				Rte_Write_ppSR_TITtMdl_ProveoutDone_ValueDone(TTtMdlIsProveoutDone);
			}

			if (fl_IgnQuickSwitchCnt > 0)
			{
				fl_IgnQuickSwitchCnt--;
			}
			break;

		case eIGN_OFF:
			fl_IgnQuickSwitchCnt 		 = 20; /*Prevent startup self-check before animation starts */
			TTtMdlIsProveoutDone         = TRUE;
			TTtMdlIsCheckStateProveoutDone = FALSE;
			Rte_Write_ppSR_TITtMdl_ProveoutDone_ValueDone(TTtMdlIsProveoutDone);

			CTtMdl_Ignoff_Tt_State();
		default:
			break;
	}

	Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTelltaleProveOutTimer, cTT_MANAGER_PROVEOUT_TIME_IN_MS,&Timer_status);
	if((FALSE == TTtMdlIsProveoutDone)&&/*(TtMdlTestTimer ==1))*/
		(((uint8)1u) == Timer_status))
	{
		/* Proveout Timer is Expired */
		TTtMdlIsProveoutDone		   = TRUE;
		Rte_Call_rpTimer_TmExt_Timer_Stop(eTelltaleProveOutTimer);
		Rte_Write_ppSR_TITtMdl_ProveoutDone_ValueDone(TTtMdlIsProveoutDone);
	}

#endif

	Telltale_fdiag_app_get_vehicle_cfg();

	if((fl_diag_session == EXTENDED_SESSION) || (fl_diag_session == EOL_SESSION))
	{
		if(fl_diag_session != flSession_pre)
		{
			for(i=0;i<cTT_NUM_ENTRIES_TELLTALE_CONFIG;i++)
			{
				(void)Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(i,cTT_TURN_OFF);
			}
		}
		else
		{
		}

		if(DID_Value.DID_IOC_Number == 0xFD8E)
		{
			hmi_TT_Check_handler();
		}
		else
		{
		}
	}
	else
	{
		CTtMdl_f_tt_timedtask();
	}

	flSession_pre = fl_diag_session;
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


/*============================================================================
**
** Function Name:      void CTtMdl_f_tt_timedtask(void)
**
** Description:        This function essentially turns the telltales on or off
**                     based on the inputs described below.
**
** Input Parameters:   None.
**
** Return Values:      None
**
** Inputs:             1) Igntion Switch Position
**                         cIGN_OFF, cIGN_ACC, cIGN_RUN, cIGN_CRANK
**
**                     2) High Voltage or Low Voltage Inhibit Status
**                         PS_VOLTAGE_LVI() and PS_VOLTAGE_HVI() )
**
**                     3) Proveout Status (This input may be configured
**                        such that it is generated internally by the
**                        telltale manger).
**                        (PROVEOUT_TELLTALES())
**
**                     4) Engineering Test Mode Bulb Test Status
**                         BULB_TEST_MODE_ONGOING() )
**
**                     5) Telltale Table:  This table resides in constant
**                        memory in this module, but it is populated via
**                        the macro PRODUCT_SPECIFIC_TELLTALE_ATTRIB_CONFIG
**                        This table is an array of type UINT8. The attribute
**                        is used in this function
**
** Outputs:            The on/off status of every telltale is updated by this
**                     routine.  Note, this routine does not actually write to
**                     the micro port pins or data buffer directly, that is
**                     done by the TtView module.
**
** Critical Section:   None.
**==========================================================================*/
static void CTtMdl_f_tt_timedtask(void)
{
    uint8 index 			   = 0;
	uint8 l_turn_on_off 	   = 0;
    uint8 tt_state_byte 	   = 0;
	uint8 TtMdlIgnSts 		   = eIGN_OFF;
	uint8 LVI_HVI_status 	   = eBatteryState_UnderVoltage;
	uint8 Timer_30s_status 	   = 0;
	
	Rte_Read_rpIgnState_IGNState(&TtMdlIgnSts);
	Rte_Read_rpBattState_BatteryState(&LVI_HVI_status);
	// Rte_Read_rpSR_TIGdt_Animation_welcome_animation_runing(&fl_animationFlag);

	if (TtMdlIgnSts == eIGN_RUN)
	{
		tt_state_byte |= cTT_RUN;
		IGN_OFF_timer_30s_flag = FALSE;
		Rte_Call_rpTimer_TmExt_Timer_Stop(eEPBWarnTimer);
	}
	else if (TtMdlIgnSts == eIGN_OFF)
	{
		tt_state_byte |= cTT_OFF;

		if(eIGN_RUN == last_TtMdlIgnSts)
		{
			Rte_Call_rpTimer_TmExt_Timer_Start(eEPBWarnTimer);
		}
		// IGN OFF Timing 30 s
		Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eEPBWarnTimer,CTT_MANAGER_PROVEOUT_TIMER_30S,&Timer_30s_status);
		if(((uint8)1u) == Timer_30s_status)
		{
			/* Proveout Timer is Expired */
			IGN_OFF_timer_30s_flag = TRUE;
			Rte_Call_rpTimer_TmExt_Timer_Stop(eEPBWarnTimer);
		}
	}
	else
	{
	   /*
	   ** No code is required in this 'else' statement.  It has been added for defensive
	   ** programming purposes.
	   */
	}

	/*
	** Start with first telltale in the telltable table and
	** iteriate all telltales, the status values will be updated by application
	** in the variable array "telltale_status_S8[]" through f_tt_update_tt_status().
	*/
	/* CYC:BEGIN_TEST_FOR    MAX_ITER=const(cyc_telltale_table_size)*/
	
#if 0
	if(TRUE== TTtMdlIsProveoutDone)
	{
		TT_Demo_Test_Process();
		TT_DEMO_TEST_TURN_OFF();
	}
	else
	{
		TT_Demo_Test_ALL_TURN_OFF();
		TT_Demo_Test_Process_Init();
	}	
#endif	

	Update_APB_AVH_Status();
	//Update_APB_AVH_Status_NotCount();
	
	for(index=0; index < cTT_NUM_ENTRIES_TELLTALE_CONFIG; index++)
	/* CYC:END_TEST_FOR */
	{
		l_turn_on_off = TtMdlTtSts[index];  // åˆå§‹åŒ? == 0

		// if(((cTTLEFT_TURN == index) || (cTTRIGHT_TURN == index) || (TRUE != IGN_OFF_timer_30s_flag)) // IGN off wait 30S ,Not double flash cannot enter
		// && (0x00 != ((telltales_attrib[index]) & tt_state_byte)))
        if (((telltales_attrib[index]) & tt_state_byte) != 0x00)
        {
            /*
            ** Check for Voltage out of range condition and if the telltale
            ** is allowed to be active in the out of range condition.
            **
            ** Note:  The structure of the two if's following this comment
            **        may seem strange but it was done this way for clarity.
            */
            if ((LVI_HVI_status == eBatteryState_UnderVoltage) && (((telltales_attrib[index]) & cTT_LVI) != 0x00))
            {
                /*
                ** Need to turn the telltale off
                */
				l_turn_on_off = cTT_TURN_OFF;

				if(TTtMdlIsCheckStateProveoutDone == TRUE) //animation is not finished
				{
					fl_IgnQuickSwitchCnt = 20;

				}
            }
			else if ((LVI_HVI_status == eBatteryState_OverVoltage)&& (((telltales_attrib[index]) & cTT_HVI) != 0x00))
            {
                /*
                ** Need to turn the telltale off
                */
				l_turn_on_off = cTT_TURN_OFF;
            }
			//else if ((LVI_HVI_status == eBatteryState_OverVoltagePhase1)&& (((telltales_attrib[index]) & cTT_HVI) != 0x00))
            //{//When voltage changes from workable state (â€œNormal voltageâ€? ) to unworkable state ("Over voltage", "Low voltage", "Mid-over voltage"), this telltale shall be turned off.
                /*
                ** Need to turn the telltale off
                */
				//l_turn_on_off = cTT_TURN_OFF;
            //}
            else
            {
                /*
                ** If return value is (TT_CONFIGURED) then the application wants
                ** this TT turned off no matter what..
                ** ie. It is SCEM'ed out....
                */
                if (((telltales_attrib[index]) & cTT_CONFIGURED_OFF) == 0x01)//check on
                {

                     l_turn_on_off = cTT_TURN_OFF;
                }
                else
                {
					 /*
                    ** if proveout is in progress and telltale
                    ** is supposed to be proven out, then light it.
                    */
					#if (cUSE_TT_MANAGER_PROVEOUT != 0)
                    if((eIGN_RUN == TtMdlIgnSts) &&         //IGN  ON
						(FALSE == TTtMdlIsProveoutDone)&&
						((telltale_attrib_proveout[index].read_NV_Value) == (telltale_attrib_proveout[index].Enable_Value)))
                    {
						if(FALSE != TESTBIT(telltales_attrib[index],cTT_CHECK)) // TT this is Self_Test on the basis of TtMdl_Cfg.h value
						{
							l_turn_on_off = cTT_TURN_ON;
						}
						else                                                    // TT this is Self_Test on the basis of Can Message Input
						{
							//ACC green and white 
							if((index == cTTACC_GREEN) || (index == cTTACC_WHITE))
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
							//LOW_FUEL and COOLANT_TEMP_HOT 
							if((index == cTTLOW_FUEL_WHITE) || (index == cTTCOOLANT_TEMP_HOT_WHITE))
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
							//1-ESP,2-AVH,3-BSD_CTA,4-FCW,5-AEB
							if((index == cTTESP) || (index == cTTAVH) || (index == cTTBSD_CTA) 
							|| (index == cTTFCW_YELLOW) || (index == cTTAEB_YELLOW))
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}

						//ADAS
						if(TELLTALE_ADAS_1 == IsEngineCfg_Adas)
						{
							if(index == cTTACC_RED)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}
						else if(TELLTALE_ADAS_2 == IsEngineCfg_Adas)
						{
							//ACC
							if(TELLTALE_ACC_2 != IsEngineCfg_Acc)
							{
								if(index == cTTACC_RED)
								{
									l_turn_on_off = cTT_TURN_OFF;
								}
							}
							else
							{
								//
							}
						}
						else
						{
							if((index == cTTLDW) || (index == cTTBSD_SODLCA) || (index == cTTAEB_RED)
							|| (index == cTTFCW_RED) || (index == cTTACC_RED))
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}

						//TPMS
						if((TELLTALE_VEHICLE_TPMS_1 != IsEngineCfg_Tpms) || (TELLTALE_VEHICLE_TPMS_3 != IsEngineCfg_Tpms))
						{
							if(index == cTTTPMS_FAILURE)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}
						
						//ABS
						if(FALSE == IsEngineCfg_Abs)
						{
							if(index == cTTABS_SYSTEM_FAILURE)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}
						
						//PEPS AND IMMO
						if((IsEngineCfg_PEPS == TELLTALE_VALUE_IMMO_1) || (IsEngineCfg_PEPS == TELLTALE_VALUE_PEPS_2)) //IMMO
						{
							//
						}
						else
						{
							if(index == cTTIMMO)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
							
							if(index == cTTPEPS_FAULT)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}
						
						// EPS
						if(FALSE == IsEngineCfg_EPS)
						{
							if(index == cTTEPS_RED)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}
						
						//AIRBAG
						if(FALSE == IsEngineCfg_Airbag)
						{
							if(index == cTTAIRBAG)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}
						
						//ESPOFF
						if(FALSE == IsEngineCfg_Esp)
						{
							if(index == cTTESP_OFF)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}
						
						//IsEngineCfg_Brake_type
						if(TELLTALE_BRAKE_TYPE_0 == IsEngineCfg_Brake_type)
						{
							if(index == cTTAPB_FAULT)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}
						else if(TELLTALE_BRAKE_TYPE_1 == IsEngineCfg_Brake_type)
						{
							//
						}
						else
						{
							if(index == cTTAPB_FAULT)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						
							if(index == cTTAPB)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}
						
						//SCR
						if(TELLTALE_EMISSION_LEVELS_SIX != IsEngineCfg_SCR)
						{
							if(index == cTTGPF)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}
						
						if(IsEngineCfg_Engine != FALSE)
						{
							// if(index == cTTDPF)
							// {
							// 	l_turn_on_off = cTT_TURN_OFF;
							// }

							if(index == cTTOIL_WATER) //if open oil_water, turn off all time 
							{
								//l_turn_on_off = cTT_TURN_OFF;
							}
						}

						#if 0
						if(IsEngineCfg_STT != TRUE)
						 {
							 if(index == cTTSTT_YELLOW)
							 {
								 l_turn_on_off = cTT_TURN_OFF;
							}
						 }
						 else  //æœ? STT
						 {
							 if((cTTSTT_GRAY == index) || (cTTSTT_GREEN == index))
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						 }
						#else
						
						if((cTTSTT_GRAY == index) || (cTTSTT_GREEN == index))
						{
							l_turn_on_off = cTT_TURN_OFF;
						}
						
						#endif


						if(ATS_FLAG != TRUE) //TRUE:ATS,FALSE:no ATS
						{
							if(index == cTTFour_WD)
							{
								l_turn_on_off = cTT_TURN_OFF;
							}
						}

                    }

					if(TTtMdlIsCheckStateProveoutDone == TRUE)
					{
						l_turn_on_off = cTT_TURN_OFF; /*Avoid responding to boot animations*/
					}

					#endif
            	}

				if((index == cTTRIGHT_TURN) || (index == cTTLEFT_TURN))
				{
					l_turn_on_off = TtMdlTtSts[index];
				}
				
            }
			
        }
		else  // /*Else path is added, if Ignition OFF is sensed,to turn OFF the telltale irrespective of actuall status*/
		{
            l_turn_on_off = cTT_TURN_OFF;
		}

		//if((IsEngineCfg == FALSE) && (index == cTTPASSENGER_SEATBELT_REMINDER))
		//{
			// shield passenger seatbelt
			// l_turn_on_off = cTT_TURN_OFF;
		//}
		
		// APB is on, then AVH is off
		//if((cTT_TURN_ON == TtMdlTtSts[cTTAPB]) || (cTT_FLASH_2HZ == TtMdlTtSts[cTTAPB])) // cTTAPB : 54 , cTTAVH : 32;
		//{
			//TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
		//}

		uint8 l_Tt_Off_Flag_u8 = 0;
		
		Rte_Read_rp_GetOTAModeStatus_Value_OTAMode(&l_Tt_Off_Flag_u8);

		if(TRUE == l_Tt_Off_Flag_u8)
		{
			(void)Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(index,0);
		}
		else
		{
			(void)Rte_Call_rpCS_TtMdl_TIMdlUpdateTtStatus_Operation(index,l_turn_on_off);
		}
	}

	last_TtMdlIgnSts = TtMdlIgnSts;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
boolean CTtMdl_Ignoff_Tt_State(void)
{
	boolean Ignoff_tt_state = FALSE;
	if((TtMdlTtSts[cTTEBD] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTESCL] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTPEPS_FAULT] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTLEFT_TURN] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTRIGHT_TURN] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTFRONT_FOG] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTREAR_FOG] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTHIGH_BEAM] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTPOSITION_LAMP] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTAVH] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTEPB] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTEPB] == cTT_FLASH_2HZ)
	|| (TtMdlTtSts[cTTAPB] == cTT_TURN_ON)
	|| (TtMdlTtSts[cTTAPB] == cTT_FLASH_2HZ)
	|| (TtMdlTtSts[cTTAPB_FAULT] == cTT_TURN_ON))
	{
		Ignoff_tt_state = TRUE;
	}
	else
	{
		Ignoff_tt_state = FALSE;
	}
	return Ignoff_tt_state;
}

//void CTtMdl_TtMdlUpdateStatus(ETtMdlMask index, ETtMdlMask tt_status)
void CTtMdl_TtUpdateTtStatus_Operation(ETtMdlMask index, ETtMdlMask tt_status)
{
	if(index<cTT_NUM_ENTRIES_TELLTALE_CONFIG)
	{
		TtMdlTtSts[index]=tt_status;
	}

	/* // APB is on, then AVH is off
	if(cTT_TURN_ON == TtMdlTtSts[cTTAPB]) // cTTAPB : 54 , cTTAVH : 32;
	{
		TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
	} */
}

static void Update_APB_AVH_Status(void)
{
	uint8  fl_epb_status_U8 = FALSE;
	uint8  fl_avh_status_U8 = FALSE;
	uint8  fl_epb_Message1_status_U8 = FALSE;
	uint8  fl_epb_Message2_status_U8 = FALSE;
	
	Rte_Read_TtMdl_rpSR_AVH_tt_state_value(&fl_avh_status_U8);
	Rte_Read_TtMdl_rpSR_APB_tt_state_value(&fl_epb_status_U8);
	Rte_Read_rpSR_APB_Message1_tt_state_value(&fl_epb_Message1_status_U8);
	Rte_Read_rpSR_APB_Message2_tt_state_value(&fl_epb_Message2_status_U8);

	//************************************************************************************************//
	//fl_avh_status_U8 = 1;
	if(AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8)
	{
		if((FALSE == fl_epb_status_U8) && (FALSE == fl_epb_Message1_status_U8) && (FALSE == fl_epb_Message2_status_U8))
		{
			fl_avh_status_U32_count1 = 2;
		}
		else
		{
			fl_avh_status_U32_count1++;
		}
	}
	else
	{
		fl_avh_status_U32_count1 = 0;
	}

	if(FALSE == fl_epb_status_U8 )
	{
		l_cTTAPB_Last_Status = TRUE;
	}

	//fl_epb_status_U8 = 1;
	if(APB_TT_STATUS_VALUE_1 == fl_epb_status_U8 )
	{
		fl_epb_status_U32_count1++;
	}
	else
	{
		fl_epb_status_U32_count1 = 0;
	}
	
	//fl_epb_status_U8 = 2;
	if(APB_TT_STATUS_VALUE_2 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count2++;
	}
	else
	{
		fl_epb_status_U32_count2 = 0;
	}	
	
	//fl_epb_status_U8 = 3;
	if(APB_TT_STATUS_VALUE_3 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count3++;
	}
	else
	{
		fl_epb_status_U32_count3 = 0;
	}
	
	//fl_epb_status_U8 = 4;
	if(APB_TT_STATUS_VALUE_4 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count4++;
	}
	else
	{
		fl_epb_status_U32_count4 = 0;
	}
	
	//fl_epb_status_U8 = 5;
	if(APB_TT_STATUS_VALUE_5 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count5++;
	}
	else
	{
		fl_epb_status_U32_count5 = 0;
	}
	
	//fl_epb_status_U8 = 6;
	if(APB_TT_STATUS_VALUE_6 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count6++;
	}
	else
	{
		fl_epb_status_U32_count6 = 0;
	}
	
	//fl_epb_status_U8 = 7;
	if(APB_TT_STATUS_VALUE_7 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count7++;
	}
	else
	{
		fl_epb_status_U32_count7 = 0;
	}
	
	//fl_epb_status_U8 = 8;
	if(APB_TT_STATUS_VALUE_8 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count8++;
	}
	else
	{
		fl_epb_status_U32_count8 = 0;
	}
	
	//fl_epb_status_U8 = 9;
	if(APB_TT_STATUS_VALUE_9 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count9++;
	}
	else
	{
		fl_epb_status_U32_count9 = 0;
	}
	
	//fl_epb_status_U8 = 10;
	if(APB_TT_STATUS_VALUE_10 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count10++;
	}
	else
	{
		fl_epb_status_U32_count10 = 0;
	}
	
	//fl_epb_status_U8 = 11;
	if(APB_TT_STATUS_VALUE_11 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count11++;
	}
	else
	{
		fl_epb_status_U32_count11 = 0;
	}
	
	//fl_epb_status_U8 = 12;
	if(APB_TT_STATUS_VALUE_12 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count12++;
	}
	else
	{
		fl_epb_status_U32_count12 = 0;
	}
	
	//fl_epb_status_U8 = 13;
	if(APB_TT_STATUS_VALUE_13 == fl_epb_status_U8)
	{
		fl_epb_status_U32_count13++;
	}
	else
	{
		fl_epb_status_U32_count13 = 0;
	}
	
	//fl_epb_status_U8 = 14;
	if(APB_TT_STATUS_VALUE_14 == fl_epb_Message1_status_U8)
	{
		fl_epb_status_U32_count14++;
	}
	else
	{
		l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_0;
		fl_epb_status_U32_count14 = 0;
	}
	
	//fl_epb_status_U8 = 15;
	if(APB_TT_STATUS_VALUE_15 == fl_epb_Message2_status_U8)
	{
		fl_epb_status_U32_count15++;
	}
	else
	{
		fl_epb_status_U32_count15 = 0;
	}
	
	//************************************************************************************************//
	//STATUS-1 cTTAPB = cTT_TURN_ON
	if((APB_TT_STATUS_VALUE_1 == fl_epb_status_U8 ) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count1 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count1 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count1 = 5;		
				fl_avh_status_U32_count1 = 6;
			}
		}
	}
	//STATUS-2 cTTAPB = cTT_TURN_ON
	if((APB_TT_STATUS_VALUE_2 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count2 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count2 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count2 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count2 = 5;
				fl_avh_status_U32_count1 = 6;
			}
		}		
	}
	//STATUS-3 cTTAPB = cTT_TURN_ON
	if((APB_TT_STATUS_VALUE_3 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count3 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count3 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count3 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count3 = 5;
				fl_avh_status_U32_count1 = 6;
			}			
		}		
	}
	//STATUS-4 cTTAPB = cTT_FLASH_2HZ
	if((APB_TT_STATUS_VALUE_4 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count4 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count4 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count4 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count4 = 5;
				fl_avh_status_U32_count1 = 6;
			}			
		}	
	}	
	//STATUS-5 cTTAPB = cTT_FLASH_2HZ
	if((APB_TT_STATUS_VALUE_5 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count5 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count5 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count5 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count5 = 5;
				fl_avh_status_U32_count1 = 6;
			}			
		}	
	}
	//STATUS-6 cTTAPB = cTT_FLASH_2HZ
	if((APB_TT_STATUS_VALUE_6 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count6 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count6 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count6 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count6 = 5;
				fl_avh_status_U32_count1 = 6;
			}			
		}	
	}
	//STATUS-7 cTTAPB = cTT_FLASH_2HZ
	if((APB_TT_STATUS_VALUE_7 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count7 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count7 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count7 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count7 = 5;
				fl_avh_status_U32_count1 = 6;
			}			
		}	
	}
	//STATUS-8 cTTAPB = cTT_FLASH_2HZ
	if((APB_TT_STATUS_VALUE_8 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count8 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count8 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count8 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count8 = 5;
				fl_avh_status_U32_count1 = 6;
			}			
		}
	}
	//STATUS-9 cTTAPB = cTT_FLASH_2HZ
	if((APB_TT_STATUS_VALUE_9 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count9 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count9 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count9 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count9 = 5;
				fl_avh_status_U32_count1 = 6;
			}			
		}		
	}
	//STATUS-10 cTTAPB = cTT_FLASH_2HZ
	if((APB_TT_STATUS_VALUE_10 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count10 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count10 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count10 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count10 = 5;
				fl_avh_status_U32_count1 = 6;
			}			
		}		
	}
	//STATUS-11 cTTAPB = cTT_FLASH_2HZ
	if((APB_TT_STATUS_VALUE_11 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count11 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count11 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count11 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count11 = 5;
				fl_avh_status_U32_count1 = 6;
			}			
		}		
	}	
	//STATUS-12 cTTAPB = cTT_FLASH_2HZ
	if((APB_TT_STATUS_VALUE_12 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count12 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count12 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count12 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count12 = 5;
				fl_avh_status_U32_count1 = 6;
			}
		}
	}
	//STATUS-13 cTTAPB = cTT_FLASH_2HZ
	if((APB_TT_STATUS_VALUE_13 == fl_epb_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if(fl_epb_status_U32_count13 > fl_avh_status_U32_count1)
		{
			TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = FALSE;
			l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
			
			if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count13 = 6;
				fl_avh_status_U32_count1 = 5;
			}
		}
		else
		{
			TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
			l_cTTAPB_Last_Status = TRUE;
			
			if(fl_epb_status_U32_count13 > APB_AVH_TIME_180DAY)
			{
				fl_epb_status_U32_count13 = 5;
				fl_avh_status_U32_count1 = 6;
			}
		}
	}
	//STATUS-14 cTTAPB = cTT_TURN_ON for fl_EPBWarningMessage1_u8 = 1
	if((APB_TT_STATUS_VALUE_14 == fl_epb_Message1_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if((FALSE == l_cTTAPB_Last_Status) || (FALSE == fl_epb_status_U8))
		{
			if(fl_epb_status_U32_count14 > fl_avh_status_U32_count1)
			{
				TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
				l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_1;
				
				if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
				{
					fl_epb_status_U32_count14 = 6;
					fl_avh_status_U32_count1 = 5;
				}
			}
			else
			{
				TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
				TtMdlTtSts[cTTAPB] = cTT_TURN_ON;
				l_cTTAPB_Message1_Status = APB_MESSAGE_STATUS_VALUE_2;
				
				if(fl_epb_status_U32_count14 > APB_AVH_TIME_180DAY)
				{
					fl_epb_status_U32_count14 = 5;
					fl_avh_status_U32_count1 = 6;
				}
			}
		}
	}
	//STATUS-15 cTTAPB = cTT_TURN_ON for fl_EPBWarningMessage2_u8 = 1
	if((APB_TT_STATUS_VALUE_15 == fl_epb_Message2_status_U8) && (AVH_TT_STATUS_VALUE_1 == fl_avh_status_U8))
	{
		if((FALSE == l_cTTAPB_Last_Status) || (FALSE == fl_epb_status_U8))
		{
			if((APB_MESSAGE_STATUS_VALUE_1 == l_cTTAPB_Message1_Status) || (APB_MESSAGE_STATUS_VALUE_0 == l_cTTAPB_Message1_Status))
			{
				if(fl_epb_status_U32_count15 > fl_avh_status_U32_count1)
				{
					TtMdlTtSts[cTTAPB] = cTT_TURN_OFF;
					
					if(fl_avh_status_U32_count1 > APB_AVH_TIME_180DAY)
					{
						fl_epb_status_U32_count15 = 6;
						fl_avh_status_U32_count1 = 5;
					}
				}
				else
				{
					TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
					TtMdlTtSts[cTTAPB] = cTT_TURN_ON;
					
					if(fl_epb_status_U32_count15 > APB_AVH_TIME_180DAY)
					{
						fl_epb_status_U32_count15 = 5;
						fl_avh_status_U32_count1 = 6;
					}
				}
			}
		}
	}

}

#if 0
static void Update_APB_AVH_Status_NotCount(void)
{
	if((0 !=TtMdlTtSts[cTTAPB])&&(0 !=TtMdlTtSts[cTTAVH]))
	{
		firstflag ++;
		if((TtMdlTtSts[cTTAPB] != ApbStatePrev)&&(firstflag == 1))
		{
			AVHStateSet = cTT_TURN_OFF;
			ApbStateSet = TtMdlTtSts[cTTAPB];
		}
		else if((TtMdlTtSts[cTTAVH] != AVHStatePrev)&&(firstflag == 1))
		{
			ApbStateSet = cTT_TURN_OFF;
			AVHStateSet = TtMdlTtSts[cTTAVH];
		}
		else if((firstflag >1)&&(TtMdlTtSts[cTTAPB] != ApbStateSet)&&(0 != ApbStateSet))
		{
			ApbStateSet = TtMdlTtSts[cTTAPB];
			AVHStateSet = cTT_TURN_OFF;
		}
		else	
		{
		}
		
		if(ApbStatePrev != ApbStateSet)
		{
			ApbStatePrev = ApbStateSet;
		}
		if(AVHStatePrev != AVHStateSet)
		{
			AVHStatePrev = AVHStateSet;
		}
		TtMdlTtSts[cTTAPB] = ApbStateSet;
		TtMdlTtSts[cTTAVH] = AVHStateSet;
		
		if(firstflag > 99)
		{
			firstflag = 99;
		}
	}
	else if((0 !=TtMdlTtSts[cTTAPB])&&(0 !=TtMdlTtSts[cTTAVH]))
	{	
		firstflag = 0;
		ApbStatePrev = 0;
		AVHStatePrev = 0;
	}
	else 
	{
		firstflag = 0;
		ApbStatePrev = TtMdlTtSts[cTTAPB];
		AVHStatePrev = TtMdlTtSts[cTTAVH];
	}
}



static void TT_Demo_Test_Process(void)
{
	
	if(TT_Demo_ON_Count > 700)
	{
		TT_Demo_Test_ALL_TURN_OFF();
		TT_Demo_ON_Count = 0;
	}
	else if(TT_Demo_ON_Count > 550)
	{
		for(int i=0;i<cTT_NUM_ENTRIES_TELLTALE_CONFIG;i++)
		{
			TtMdlTtSts[i]=cTT_TURN_ON;
		}
	}
	else if(TT_Demo_ON_Count > 500)
	{
		TT_Demo_Test_ALL_TURN_OFF();
		TtMdlTtSts[cTTDRIVER_SEATBELT_REMINDER]=cTT_TURN_ON;
		TtMdlTtSts[cTTAIRBAG]=cTT_TURN_ON;
		TtMdlTtSts[cTTPEPS_FAULT]=cTT_TURN_ON;
		TtMdlTtSts[cTTCHARGE]=cTT_TURN_ON;
		TtMdlTtSts[cTTGLOW]=cTT_TURN_ON;
	}
	else if(TT_Demo_ON_Count > 450)
	{
		TT_Demo_Test_ALL_TURN_OFF();
		TtMdlTtSts[cTTDRIVER_SEATBELT_REMINDER]=cTT_TURN_ON;
		TtMdlTtSts[cTTAIRBAG]=cTT_TURN_ON;
		TtMdlTtSts[cTTPEPS_FAULT]=cTT_TURN_ON;
		TtMdlTtSts[cTTCHARGE]=cTT_TURN_ON;
	
		
	}
	else if(TT_Demo_ON_Count > 400)
	{
		TT_Demo_Test_ALL_TURN_OFF();
		TtMdlTtSts[cTTDRIVER_SEATBELT_REMINDER]=cTT_TURN_ON;
		TtMdlTtSts[cTTAIRBAG]=cTT_TURN_ON;
		TtMdlTtSts[cTTPEPS_FAULT]=cTT_TURN_ON;
	
	}
	else if(TT_Demo_ON_Count > 350)
	{
		TT_Demo_Test_ALL_TURN_OFF();
		TtMdlTtSts[cTTDRIVER_SEATBELT_REMINDER]=cTT_TURN_ON;
		TtMdlTtSts[cTTAIRBAG]=cTT_TURN_ON;
	}
	else if(TT_Demo_ON_Count > 300)
	{
		TT_Demo_Test_ALL_TURN_OFF();
		TtMdlTtSts[cTTDRIVER_SEATBELT_REMINDER]=cTT_TURN_ON;
	}
	else
	{
		TT_Demo_Test_ALL_TURN_OFF();
	}
	
	TtMdlTtSts[cTTLOW_FUEL]=cTT_TURN_ON;
	TtMdlTtSts[cTTCOOLANT_TEMP_HOT]=cTT_TURN_ON;
}

static void TT_DEMO_TEST_TURN_OFF(void)
{
	TtMdlTtSts[cTTESP] = cTT_TURN_OFF;
	TtMdlTtSts[cTTPARK_LAMPS] = cTT_TURN_OFF;
	TtMdlTtSts[cTTCRUISE_CONTROL] = cTT_TURN_OFF;
	TtMdlTtSts[cTTCRUISE_RED] = cTT_TURN_OFF;
	TtMdlTtSts[cTTACC_WHITE] = cTT_TURN_OFF;
	TtMdlTtSts[cTTACC_RED] = cTT_TURN_OFF;
	TtMdlTtSts[cTTSTT_GRAY] = cTT_TURN_OFF;
	TtMdlTtSts[cTTSTT_GREEN] = cTT_TURN_OFF;
	TtMdlTtSts[cTTSTT_YELLOW] = cTT_TURN_OFF;
	TtMdlTtSts[cTTHDC] = cTT_TURN_OFF;
	TtMdlTtSts[cTTHandson] = cTT_TURN_OFF;
	TtMdlTtSts[cTTLOW_FUEL_WHITE] = cTT_TURN_OFF;
	TtMdlTtSts[cTTADAS_AUTO] = cTT_TURN_OFF;
	TtMdlTtSts[cTTAUTO_HOLD] = cTT_TURN_OFF;
	TtMdlTtSts[cTTBSD_CTA] = cTT_TURN_OFF;
	TtMdlTtSts[cTTAEB_YELLOW] = cTT_TURN_OFF;
	TtMdlTtSts[cTTFCW_YELLOW] = cTT_TURN_OFF;
	TtMdlTtSts[cTTAVH] = cTT_TURN_OFF;
	TtMdlTtSts[cTTEPS_YLW] = cTT_TURN_OFF;
}

static void TT_Demo_Test_ALL_TURN_OFF(void)
{
	int i =0;
	for(i=0;i<cTT_NUM_ENTRIES_TELLTALE_CONFIG;i++)
	{
		TtMdlTtSts[i]=cTT_TURN_OFF;
	}

}

static void TT_Demo_Test_Process_Init(void)
{
	TT_Demo_ON_Count = 0;
	TT_Demo_Test_ALL_TURN_OFF();
}
#endif

static void Telltale_fdiag_app_get_vehicle_cfg(void)
{
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Engine,&IsEngineCfg_Engine);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_PASSENGER_BELT,&IsEngineCfg_PASSENGER_BELT);//PASSENGER_BELT
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_PEPS,&IsEngineCfg_PEPS);//PEPS AND IMMO
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Cruise,&IsEngineCfg_Acc);//CC
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Start_Stop,&IsEngineCfg_STT);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_FourW_HL,&IsEngineCfg_4WHL);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_EPS,&IsEngineCfg_EPS);//EPS
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ATS,&ATS_FLAG);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AT_MT,&IsATCfg_GearBox);//GEARBOX
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ABS,&IsEngineCfg_Abs);//ABS
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AIRBAG,&IsEngineCfg_Airbag);//AIRBAG
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_TPMS,&IsEngineCfg_Tpms);//TPMS
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ESP,&IsEngineCfg_Esp);//ESP
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_EMISSION_LEVELS,&IsEngineCfg_SCR);//SCR
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_BRAKE_TYPE,&IsEngineCfg_Brake_type);//BRAKE_TYPE
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ADAS,&IsEngineCfg_Adas);//ADAS
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_OVER_SPEED,&IsEngineCfg_Overspeed);//OVER_SPEED
}


