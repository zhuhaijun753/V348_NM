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
#include "Rte_HmiAccMdl.h"
#include "CmpLib.h"


/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(HmiAccMdl)

#define	FUNCTION_CALLED_PERIOD	(100) //ms
#define HMI_ACC_DELAY_TIME_500MS  ((500  + FUNCTION_CALLED_PERIOD - 1) / FUNCTION_CALLED_PERIOD)

#define HMIACC_LANE_ASSIT_OFF 	(0)
#define HMIACC_LANE_ASSIT_FAULT	(5)

#define EOL_ADAS_1		(0x01)
#define EOL_ADAS_2		(0x02)

typedef struct {
	//GW_MRR_0x245(LDW & TSR)
	uint8 LaneAssitStatus;
	uint8 LeftLane;
	uint8 RightLane;
	uint8 IPM_LaneAssit_HandsonReq;

	uint8 IPM_SLAState;
	uint8 IPM_SLASpdlimit;
	uint8 IPM_SLASpdlimitUnits;

	uint8 IPM_LaneAssitType;

	//GW_MRR_0x246(ACC & AEB & FCW)
	uint8 AccMode;
	uint8 AccdxTarObj;
	uint8 AccTauGapSet;
	uint8 AccTextinfo;
	UInt16 MRR_vSetDis;
	uint8 MRR_ObjValid;
	uint8 MRR_TakeOverReq;

	uint8 MRR_AEB_STATE;

	uint8 MRR_PCW_STATE;
	uint8 PCW_latentWarning;
	uint8 PCW_preWarning;

	//GW_SOD_0x275 BSD
	uint8 SOD_BLIS_display;
	uint8 SODLCA_OFFTelltale;
	uint8 CTA_OFFTelltale;
	uint8 DOA_OFFTelltale;
	uint8 SOD_CTA_warningReqLeft;
	uint8 SOD_CTA_warningReqRight;

	//MRR_FRObj_0x279
	uint16 MRR_F_Obj_dx;
	uint16 MRR_F_Obj_dy;
	uint8 MRR_F_Obj_Class;
} ST_ACC_MDL_INFO;

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static ST_ACC_MDL_INFO stAccMdlInfo;

static ST_ACC_MDL_INFO stAccMdlInfoBk;

static uint8 fl_OverVoltageTout = 0;

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
static void f_HmiAccMdl_init(void);

static void f_HmiAdas_Init(void);
static void f_HmiAdas_Process(void);

static void f_HmiAcc_Init(void);
static void f_HmiAcc_Process(void);

static void f_HmiBsd_Init(void);
static void f_HmiBsd_Process(void);

static void f_HmiObj_Init(void);
static void f_HmiObj_Process(void);

static void f_HmiAcc_menu_chg_proc(void);

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
static Std_ReturnType CmpInit ( void )
{
	f_HmiAccMdl_init();
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
	f_HmiAccMdl_init();
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	uint8 fl_ign_state_U8;
	uint8 fl_batt_state_U8;

	Rte_Read_rpBattState_BatteryState(&fl_batt_state_U8);
	Rte_Read_rpIgnState_IGNState(&fl_ign_state_U8);

	if ((fl_batt_state_U8 == eBatteryState_UnderVoltage) || (fl_batt_state_U8 == eBatteryState_OverVoltage))
	{
		stAccMdlInfo = stAccMdlInfoBk;
		fl_OverVoltageTout = HMI_ACC_DELAY_TIME_500MS;
	}
	else
	{
		switch(fl_ign_state_U8)
		{
			case eIGN_OFF:					
				f_HmiAccMdl_init();
				break;

			case eIGN_RUN:
				#if 0
				if (fl_OverVoltageTout > 0)
				{
					fl_OverVoltageTout--;
				}

				if (0 == fl_OverVoltageTout)
				{
					// f_HmiAdas_Process();
					 //f_HmiAcc_Process();
					// f_HmiBsd_Process();
					// f_HmiObj_Process();

					f_HmiAcc_menu_chg_proc();
					stAccMdlInfoBk = stAccMdlInfo;
				}
				else
				{
					stAccMdlInfo = stAccMdlInfoBk;
				}
				#else
				f_HmiAcc_Process();
				f_HmiAdas_Process();
				#endif
				break;

			default:
				break;
		}
   }

	Rte_Write_ppSR_HmiAccMdl_LaneStatus_LaneAssit_Status(stAccMdlInfo.LaneAssitStatus);
	Rte_Write_ppSR_HmiAccMdl_LaneStatus_LaneAssit_LeftVisualization(stAccMdlInfo.LeftLane);
	Rte_Write_ppSR_HmiAccMdl_LaneStatus_LaneAssit_RightVisualization(stAccMdlInfo.RightLane);
#if 1	
	Rte_Write_ppSR_HmiAccMdl_LaneStatus_IPM_LaneAssit_HandsonReq(stAccMdlInfo.IPM_LaneAssit_HandsonReq);

	Rte_Write_ppSR_HmiAccMdl_LaneStatus_SLASpdlimit(stAccMdlInfo.IPM_SLASpdlimit);
//	Rte_Write_ppSR_HmiAccMdl_LaneStatus_SLASpdlimitUnits(stAccMdlInfo.IPM_SLASpdlimitUnits);
#endif	
	Rte_Write_ppSR_HmiAccMdl_LaneStatus_SLAState(stAccMdlInfo.IPM_SLAState);
	Rte_Write_ppSR_HmiAccMdl_LaneStatus_IPM_LaneAssitType(stAccMdlInfo.IPM_LaneAssitType);

	Rte_Write_ppSR_HmiAccMdl_AccStatus_MRR_ACCMode(stAccMdlInfo.AccMode);
	Rte_Write_ppSR_HmiAccMdl_AccStatus_MRR_TauGapSet(stAccMdlInfo.AccTauGapSet);
//	Rte_Write_ppSR_HmiAccMdl_AccStatus_MRR_Textinfo(stAccMdlInfo.AccTextinfo);
	Rte_Write_ppSR_HmiAccMdl_AccStatus_MRR_dxTarObj(stAccMdlInfo.AccdxTarObj);
	Rte_Write_ppSR_HmiAccMdl_AccStatus_MRR_vSetDis(stAccMdlInfo.MRR_vSetDis);
	Rte_Write_ppSR_HmiAccMdl_AccStatus_MRR_ObjValid(stAccMdlInfo.MRR_ObjValid);
//	Rte_Write_ppSR_HmiAccMdl_AccStatus_MRR_TakeOverReq(stAccMdlInfo.MRR_TakeOverReq);
#if 0
	Rte_Write_ppSR_HmiAccMdl_AccStatus_MRR_AEB_STATE(stAccMdlInfo.MRR_AEB_STATE);

	Rte_Write_ppSR_HmiAccMdl_AccStatus_MRR_PCW_STATE(stAccMdlInfo.MRR_PCW_STATE);
	Rte_Write_ppSR_HmiAccMdl_AccStatus_PCW_latentWarning(stAccMdlInfo.PCW_latentWarning);
	Rte_Write_ppSR_HmiAccMdl_AccStatus_PCW_preWarning(stAccMdlInfo.PCW_preWarning);

	Rte_Write_ppSR_TIGdt_SOD_BLIS_DISP_SOD_BLIS_display(stAccMdlInfo.SOD_BLIS_display);
	Rte_Write_ppSR_TIGdt_SOD_BLIS_DISP_SODLCA_OFFTelltale(stAccMdlInfo.SODLCA_OFFTelltale);
	Rte_Write_ppSR_TIGdt_SOD_BLIS_DISP_DOA_OFFTelltale(stAccMdlInfo.DOA_OFFTelltale);
	Rte_Write_ppSR_TIGdt_SOD_BLIS_DISP_CTA_OFFTelltale(stAccMdlInfo.CTA_OFFTelltale);
	Rte_Write_ppSR_TIGdt_SOD_BLIS_DISP_SOD_CTA_warningReqLeft(stAccMdlInfo.SOD_CTA_warningReqLeft);
	Rte_Write_ppSR_TIGdt_SOD_BLIS_DISP_SOD_CTA_warningReqRight(stAccMdlInfo.SOD_CTA_warningReqRight);

	Rte_Write_pp_HmiACC_GdtCtrl_Value_MRR_F_Object_dx(stAccMdlInfo.MRR_F_Obj_dx);
	Rte_Write_pp_HmiACC_GdtCtrl_Value_MRR_F_Object_dy(stAccMdlInfo.MRR_F_Obj_dy);
	Rte_Write_pp_HmiACC_GdtCtrl_Value_MRR_F_Obj_0x_class(stAccMdlInfo.MRR_F_Obj_Class);
#endif
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

static void f_HmiAccMdl_init(void)
{
	f_HmiAdas_Init();
	f_HmiAcc_Init();
	f_HmiBsd_Init();
	f_HmiObj_Init();

	fl_OverVoltageTout = 0;
}

static void f_HmiAdas_Init(void)
{
	stAccMdlInfo.LaneAssitStatus = 0; //off
	stAccMdlInfo.LeftLane = 0;
	stAccMdlInfo.RightLane = 0;
	stAccMdlInfo.IPM_LaneAssit_HandsonReq = 0;

	stAccMdlInfoBk.LaneAssitStatus = 0; //off
	stAccMdlInfoBk.LeftLane = 0;
	stAccMdlInfoBk.RightLane = 0;
	stAccMdlInfoBk.IPM_LaneAssit_HandsonReq = 0;

	stAccMdlInfo.IPM_SLASpdlimit = 0;
	stAccMdlInfo.IPM_SLASpdlimitUnits = 0;
	stAccMdlInfo.IPM_SLAState = 0;

	stAccMdlInfo.IPM_LaneAssitType = 0;

	stAccMdlInfoBk.IPM_LaneAssitType = 0;
}

static void f_HmiAdas_Process(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;

	boolean fl_vehicle_cfg_ldw_flg = TRUE;
	boolean fl_vehicle_cfg_tsr_flg = TRUE;
	
	uint8 	fl_lane_status_U8 = 0;
	uint8 	fl_left_lane_U8 = 0;
	uint8 	fl_right_lane_U8 = 0;
	uint8   fl_IPM_LaneAssit_HandsonReq_U8 = 0;

	uint8 	fl_IPM_SLASpdlimit_U8 = 0;
	uint8 	fl_IPM_SLASpdlimitUnits_U8 = 0;
	uint8 	fl_IPM_SLAState_U8 = 0;

	uint8 	fl_IPM_LaneAssitType_U8 = 0;
	uint8   fl_eol_adas;
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ADAS, &fl_eol_adas);
	
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_Tout(&fl_CAN_msg_status_U8);
	
	if(EOL_ADAS_1 == fl_eol_adas)
	{
		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status(&fl_lane_status_U8);
		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_LeftVisualization(&fl_left_lane_U8);
		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_RightVisualization(&fl_right_lane_U8);
//		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_HandsonReq(&fl_IPM_LaneAssit_HandsonReq_U8);

		//Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssitType(&fl_IPM_LaneAssitType_U8);
	}
	else if(EOL_ADAS_2 == fl_eol_adas)
	{
		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_Status(&fl_lane_status_U8);
		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_LeftVisualization(&fl_left_lane_U8);
		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_RightVisualization(&fl_right_lane_U8);
		//Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssit_HandsonReq(&fl_IPM_LaneAssit_HandsonReq_U8);

		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_LaneAssitType(&fl_IPM_LaneAssitType_U8);
	}
	else
	{
	}
	
	if (TRUE == fl_vehicle_cfg_tsr_flg)
	{
		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimit(&fl_IPM_SLASpdlimit_U8);
	//	Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLASpdlimitUnits(&fl_IPM_SLASpdlimitUnits_U8);
		Rte_Read_rpSR_CANMSG_GW_IPM_0x245_ComIn_IPM_SLAState(&fl_IPM_SLAState_U8);
	}
	
	if(fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_HmiAdas_Init();
	}
	else 
	{
		stAccMdlInfo.LaneAssitStatus = fl_lane_status_U8;
		stAccMdlInfo.LeftLane = fl_left_lane_U8;
		stAccMdlInfo.RightLane = fl_right_lane_U8;
		#if 1
		stAccMdlInfo.IPM_LaneAssit_HandsonReq = fl_IPM_LaneAssit_HandsonReq_U8;

		stAccMdlInfo.IPM_SLASpdlimit = fl_IPM_SLASpdlimit_U8;
		stAccMdlInfo.IPM_SLASpdlimitUnits = fl_IPM_SLASpdlimitUnits_U8;
		#endif
		stAccMdlInfo.IPM_SLAState = fl_IPM_SLAState_U8;

		stAccMdlInfo.IPM_LaneAssitType = fl_IPM_LaneAssitType_U8;
	}
}

static void f_HmiAcc_Init(void)
{
	stAccMdlInfo.AccMode = 0;
	stAccMdlInfo.AccdxTarObj = 0;
	stAccMdlInfo.AccTauGapSet = 0;
	stAccMdlInfo.AccTextinfo = 0;
	stAccMdlInfo.MRR_vSetDis = 0;
	stAccMdlInfo.MRR_ObjValid = 0;
	stAccMdlInfo.MRR_TakeOverReq = 0;

	stAccMdlInfoBk.AccMode = 0;
	stAccMdlInfoBk.AccdxTarObj = 0;
	stAccMdlInfoBk.AccTauGapSet = 0;
	stAccMdlInfoBk.AccTextinfo = 0;
	stAccMdlInfoBk.MRR_vSetDis = 0;
	stAccMdlInfoBk.MRR_ObjValid = 0;
	stAccMdlInfoBk.MRR_TakeOverReq = 0;

	stAccMdlInfo.MRR_AEB_STATE = 2; /* 0x2:Standby, Icon disappear when Signal loss */

	stAccMdlInfo.MRR_PCW_STATE = 2; /* 0x2:Standby, Icon disappear when Signal loss */
	stAccMdlInfo.PCW_latentWarning = 0;
	stAccMdlInfo.PCW_preWarning = 0;
}

static void f_HmiAcc_Process(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;

	boolean fl_vehicle_cfg_acc_flg = FALSE;
	boolean fl_vehicle_cfg_aeb_flg = FALSE;
	boolean fl_vehicle_cfg_fcw_flg = FALSE;
	
	uint8 	fl_acc_mode_U8 = 0;
	uint8 	fl_acc_dx_tar_obj_U8 = 0;
	uint8 	fl_acc_tau_gap_set_U8 = 0;
	uint8 	fl_acc_text_info_U8 = 0;
	UInt16 	fl_MRR_vSetDis_U16 = 0;
	uint8 	fl_MRR_ObjValid_U8 = 0;
	uint8 	fl_MRR_TakeOverReq_U8 = 0;

	uint8 	fl_MRR_AEB_STATE_U8 = 2;

	uint8 	fl_MRR_PCW_STATE_U8 = 2;
	uint8 	fl_PCW_latentWarning_U8 = 0;
	uint8   fl_PCW_preWarning_U8 = 0;

	//read downline configure
	#if 0
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout(&fl_CAN_msg_status_U8);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_ACC, &fl_vehicle_cfg_acc_flg);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_AEB, &fl_vehicle_cfg_aeb_flg);
	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_FCW, &fl_vehicle_cfg_fcw_flg);

	if (TRUE == fl_vehicle_cfg_acc_flg)
	{
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode(&fl_acc_mode_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_dxTarObj(&fl_acc_dx_tar_obj_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TauGapSet(&fl_acc_tau_gap_set_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Textinfo(&fl_acc_text_info_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_vSetDis(&fl_MRR_vSetDis_U16);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_ObjValid(&fl_MRR_ObjValid_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TakeOverReq(&fl_MRR_TakeOverReq_U8);
	}

	if (TRUE == fl_vehicle_cfg_aeb_flg)
	{
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_AEB_STATE(&fl_MRR_AEB_STATE_U8);
	}

	if (TRUE == fl_vehicle_cfg_fcw_flg)
	{
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_STATE(&fl_MRR_PCW_STATE_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_latentWarning(&fl_PCW_latentWarning_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_PCW_preWarning (&fl_PCW_preWarning_U8);
	}
	#endif

			//read downline configure
	if  (FALSE == fl_vehicle_cfg_acc_flg)
	{
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_NR(&fl_CAN_nvr_status_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_Tout(&fl_CAN_msg_status_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_ACCHMI_Mode(&fl_acc_mode_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_vSetDis(&fl_MRR_vSetDis_U16);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_dxTarObj(&fl_acc_dx_tar_obj_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_ObjValid(&fl_MRR_ObjValid_U8);
		Rte_Read_rpSR_CANMSG_GW_MRR_Chassis_0x246_ComIn_MRR_TauGapSet(&fl_acc_tau_gap_set_U8);

		if(fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
		{
			f_HmiAcc_Init();
		}
		else 
		{
		#if 0
			stAccMdlInfo.AccMode = fl_acc_mode_U8;
			stAccMdlInfo.AccdxTarObj = fl_acc_dx_tar_obj_U8;
			stAccMdlInfo.AccTauGapSet = fl_acc_tau_gap_set_U8;
			stAccMdlInfo.AccTextinfo = fl_acc_text_info_U8;
			stAccMdlInfo.MRR_vSetDis = fl_MRR_vSetDis_U16;
			stAccMdlInfo.MRR_ObjValid = fl_MRR_ObjValid_U8;
			stAccMdlInfo.MRR_TakeOverReq = fl_MRR_TakeOverReq_U8;

			stAccMdlInfo.MRR_AEB_STATE = fl_MRR_AEB_STATE_U8;

			stAccMdlInfo.MRR_PCW_STATE = fl_MRR_PCW_STATE_U8;
			stAccMdlInfo.PCW_latentWarning = fl_PCW_latentWarning_U8;
			stAccMdlInfo.PCW_preWarning = fl_PCW_preWarning_U8;
		#else
			stAccMdlInfo.AccMode = fl_acc_mode_U8;
			stAccMdlInfo.AccdxTarObj = fl_acc_dx_tar_obj_U8;
			stAccMdlInfo.AccTauGapSet = fl_acc_tau_gap_set_U8;
			stAccMdlInfo.MRR_vSetDis = fl_MRR_vSetDis_U16;
			stAccMdlInfo.MRR_ObjValid = fl_MRR_ObjValid_U8;
		#endif
		}
	}
	else
	{
		f_HmiAcc_Init();
	}
}

static void f_HmiBsd_Init(void)
{
	stAccMdlInfo.SOD_BLIS_display = 0;
	stAccMdlInfoBk.SOD_BLIS_display = 0;

	stAccMdlInfo.SODLCA_OFFTelltale = 0;
	stAccMdlInfo.CTA_OFFTelltale = 0;
	stAccMdlInfo.DOA_OFFTelltale = 0;
	stAccMdlInfo.SOD_CTA_warningReqLeft = 0;
	stAccMdlInfo.SOD_CTA_warningReqRight = 0;

	stAccMdlInfoBk.SOD_CTA_warningReqLeft = 0;
	stAccMdlInfoBk.SOD_CTA_warningReqRight = 0;
}

static void f_HmiBsd_Process(void)
{
	uint8 	fl_CAN_nvr_status_U8 = 0;
	uint8 	fl_CAN_msg_status_U8 = 0;

	boolean fl_vehicle_cfg_bsd_flg = FALSE;

	uint8   fl_SOD_BLIS_display = 0;
	uint8   fl_SODLCA_OFFTelltale_U8 = 0;
	uint8   fl_CTA_OFFTelltale_U8 = 0;
	uint8   fl_DOA_OFFTelltale_U8 = 0;
	uint8   fl_SOD_CTA_warningReqLeft_U8 = 0;
	uint8   fl_SOD_CTA_warningReqRight_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_Tout(&fl_CAN_msg_status_U8);

	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_BSD, &fl_vehicle_cfg_bsd_flg);

	if (TRUE == fl_vehicle_cfg_bsd_flg)
	{
		Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_BLIS_display(&fl_SOD_BLIS_display);
		Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SODLCA_OFFTelltale(&fl_SODLCA_OFFTelltale_U8);
		Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_CTA_OFFTelltale(&fl_CTA_OFFTelltale_U8);
		Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_DOA_OFFTelltale(&fl_DOA_OFFTelltale_U8);
		Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqLeft(&fl_SOD_CTA_warningReqLeft_U8);
		Rte_Read_rpSR_CANMSG_GW_IPM_0x275_ComIn_SOD_CTA_warningReqRight(&fl_SOD_CTA_warningReqRight_U8);
	}

	if (fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_HmiBsd_Init();
	}
	else 
	{
		stAccMdlInfo.SOD_BLIS_display = fl_SOD_BLIS_display;
		stAccMdlInfo.SODLCA_OFFTelltale = fl_SODLCA_OFFTelltale_U8;
		stAccMdlInfo.CTA_OFFTelltale = fl_CTA_OFFTelltale_U8;
		stAccMdlInfo.DOA_OFFTelltale = fl_DOA_OFFTelltale_U8;
		stAccMdlInfo.SOD_CTA_warningReqLeft = fl_SOD_CTA_warningReqLeft_U8;
		stAccMdlInfo.SOD_CTA_warningReqRight = fl_SOD_CTA_warningReqRight_U8;
	}
}

static void f_HmiObj_Init(void)
{
	stAccMdlInfo.MRR_F_Obj_dx = 0;
	stAccMdlInfo.MRR_F_Obj_dy = 0;
	stAccMdlInfo.MRR_F_Obj_Class = 0;
}

static void f_HmiObj_Process(void)
{
	uint16 fl_MRR_F_Obj_dx = 0;
	uint16 fl_MRR_F_Obj_dy = 0;
	uint8 fl_MRR_F_Obj_Class = 0;

	uint8 fl_CAN_nvr_status_U8 = 0;
	uint8 fl_CAN_msg_status_U8 = 0;

	Rte_Read_rpSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_NR(&fl_CAN_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_Tout(&fl_CAN_msg_status_U8);

	Rte_Read_rpSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Object_dx(&fl_MRR_F_Obj_dx);
	Rte_Read_rpSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Object_dy(&fl_MRR_F_Obj_dy);
	Rte_Read_rpSR_CANMSG_GW_MRR_FrObj_0x279_ComIn_MRR_F_Obj_0x_class(&fl_MRR_F_Obj_Class);

	if (fl_CAN_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_CAN_msg_status_U8 == RTE_E_TIMEOUT)
	{
		stAccMdlInfo.MRR_F_Obj_dx = 0xFFFF;
		stAccMdlInfo.MRR_F_Obj_dy = 0XFFFF;
	}
	else 
	{
		stAccMdlInfo.MRR_F_Obj_dx = fl_MRR_F_Obj_dx;
		stAccMdlInfo.MRR_F_Obj_dy = fl_MRR_F_Obj_dy;
		stAccMdlInfo.MRR_F_Obj_Class = fl_MRR_F_Obj_Class;
	}
	
}

static void f_HmiAcc_menu_chg_proc(void)
{
	uint8 fl_AdasMenuChgFlg = 0;

	/* IPM_LaneAssit_Status */
	if (stAccMdlInfoBk.LaneAssitStatus != stAccMdlInfo.LaneAssitStatus && HMIACC_LANE_ASSIT_OFF < stAccMdlInfo.LaneAssitStatus && HMIACC_LANE_ASSIT_FAULT > stAccMdlInfo.LaneAssitStatus)
	{
		stAccMdlInfoBk.LaneAssitStatus = stAccMdlInfo.LaneAssitStatus;
		fl_AdasMenuChgFlg = TRUE;
	}

	/* IPM_LaneAssit_LeftVisualization */
	if (stAccMdlInfoBk.LeftLane != stAccMdlInfo.LeftLane)
	{
		stAccMdlInfoBk.LeftLane = stAccMdlInfo.LeftLane;
		fl_AdasMenuChgFlg = TRUE;
	}

	/* IPM_LaneAssit_RightVisualization */
	if (stAccMdlInfoBk.RightLane != stAccMdlInfo.RightLane)
	{
		stAccMdlInfoBk.RightLane = stAccMdlInfo.RightLane;
		fl_AdasMenuChgFlg = TRUE;
	}

	/* IPM_LaneAssitType */
	if (stAccMdlInfoBk.IPM_LaneAssitType != stAccMdlInfo.IPM_LaneAssitType && 0 != stAccMdlInfo.IPM_LaneAssitType)
	{
		stAccMdlInfoBk.IPM_LaneAssitType = stAccMdlInfo.IPM_LaneAssitType;
		fl_AdasMenuChgFlg = TRUE;
	}

	/* dxTarObj */
	if (stAccMdlInfoBk.AccdxTarObj != stAccMdlInfo.AccdxTarObj && 0 != stAccMdlInfo.AccdxTarObj)
	{
		stAccMdlInfoBk.AccdxTarObj = stAccMdlInfo.AccdxTarObj;
		fl_AdasMenuChgFlg = TRUE;
	}

	/* TauGapSet */
	if (stAccMdlInfoBk.AccTauGapSet != stAccMdlInfo.AccTauGapSet && 0 != stAccMdlInfo.AccTauGapSet)
	{
		stAccMdlInfoBk.AccTauGapSet = stAccMdlInfo.AccTauGapSet;
		fl_AdasMenuChgFlg = TRUE;
	}

	/* TakeOverReq */
	if (stAccMdlInfoBk.MRR_TakeOverReq != stAccMdlInfo.MRR_TakeOverReq && 0 != stAccMdlInfo.MRR_TakeOverReq)
	{
		stAccMdlInfoBk.MRR_TakeOverReq = stAccMdlInfo.MRR_TakeOverReq;
		fl_AdasMenuChgFlg = TRUE;
	}

	/* Textinfo */
	if (stAccMdlInfoBk.AccTextinfo != stAccMdlInfo.AccTextinfo && 0 != stAccMdlInfo.AccTextinfo)
	{
		stAccMdlInfoBk.AccTextinfo = stAccMdlInfo.AccTextinfo;
		fl_AdasMenuChgFlg = TRUE;
	}

	/* ObjValid */
	if (stAccMdlInfoBk.MRR_ObjValid != stAccMdlInfo.MRR_ObjValid && 0 != stAccMdlInfo.MRR_ObjValid)
	{
		stAccMdlInfoBk.MRR_ObjValid = stAccMdlInfo.MRR_ObjValid;
		fl_AdasMenuChgFlg = TRUE;
	}

	/* SOD_CTA_warningReqLeft */
	if (stAccMdlInfoBk.SOD_CTA_warningReqLeft != stAccMdlInfo.SOD_CTA_warningReqLeft && 2 == stAccMdlInfo.SOD_CTA_warningReqLeft)
	{
		stAccMdlInfoBk.SOD_CTA_warningReqLeft = stAccMdlInfo.SOD_CTA_warningReqLeft;
		fl_AdasMenuChgFlg = TRUE;
	}

	/* SOD_CTA_warningReqRight */
	if (stAccMdlInfoBk.SOD_CTA_warningReqRight != stAccMdlInfo.SOD_CTA_warningReqRight && 2 == stAccMdlInfo.SOD_CTA_warningReqRight)
	{
		stAccMdlInfoBk.SOD_CTA_warningReqRight = stAccMdlInfo.SOD_CTA_warningReqRight;
		fl_AdasMenuChgFlg = TRUE;
	}

	Rte_Write_ppSR_TIGdt_SOD_BLIS_DISP_MenuChgReq(fl_AdasMenuChgFlg);
}

///=====================================================================================================================
//  PUBLIC
//=====================================================================================================================

/*----------------------------------------------------------------------------
Date              : 19-July-2017
CDSID             : kvakeshk
Traceability      : RTC ID 
Change Description: Speed Out Intial Version
-----------------------------------------------------------------------------*/


