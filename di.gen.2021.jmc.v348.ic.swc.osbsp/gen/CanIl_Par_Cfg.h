#if !defined(CAN_IL_PAR_H)
#define CAN_IL_PAR_H
/* ===========================================================================

                      CONFIDENTIAL VISTEON CORPORATION

   This is an unpublished work of authorship, which contains trade secrets,
   created in 2006.  Visteon Corporation owns all rights to this work and
   intends to maintain it in confidence to preserve its trade secret status.
   Visteon Corporation reserves the right, under the copyright laws of the
   United States or those of any other country that may have jurisdiction, to
   protect this work as an unpublished work, in the event of an inadvertent
   or deliberate unauthorized publication.  Visteon Corporation also reserves
   its rights under all copyright laws to protect this work as a published
   work, when appropriate.  Those having access to this work may not copy it,
   use it, modify it or disclose the information contained in it without the
   written authorization of Visteon Corporation.

  =========================================================================*/
/* ===========================================================================

    Name:           CanIl_Par_Cfg.h

    Description:    CAN Interaction Layer Tx, Rx Parameters Header File

    Organization:   Multiplex Core Technology

   =========================================================================*/ 

#include "CanIl_Defines.h" 

/* ===========================================================================
   P U B L I C   M A C R O S
  =========================================================================*/

/* ===========================================================================
   Interaction Layer Number of Transmit Messages, Signals
  =========================================================================*/


#define Can_Ch0_Il_Tx_Num_Burst_Periodic   (0u) /*no use*/


#define Can_Ch0_Il_Tx_Num_Periodic         (5u) /*no use*/


/* ===========================================================================
     Interaction Layer Number of Receive Messages, Signals
    =========================================================================*/


#define Can_Ch0_Il_Rx_Num_Periodic         (30u)	/*no use*/

#define Can_Ch0_Il_Rx_Num_Periodic_Signals (240u)	/*no use*/

#define Can_Ch0_Il_Rx_Num_Req_Frames       (0u)

/* ===========================================================================
      Interaction Layer support
     =========================================================================*/

  /* ===========================================================================
      Interaction Layer Transmit Message Enumerations
      NB: The below Tx message sequence should match one to one with the
          IL Tx Message Desription table.
          CAN_IL_CHANNEL0_TX_MESSAGE const
          Can_Ch0_Il_Tx_Messages[ Can_Ch0_Il_Tx_Num_Messages ]
     =========================================================================*/

/* ===========================================================================
    Interaction Layer Transmit Message (Frame) Handles
   =========================================================================*/

typedef enum
  {
Can_Ch0_Il_Tx_Msg_IC_0x386_TMH=0,
/*(0) */
Can_Ch0_Il_Tx_Msg_IC_0x392_TMH,
/*(1) */
Can_Ch0_Il_Tx_Msg_IC_0x510_TMH,
/*(2) */
Can_Ch0_Il_Tx_Msg_IC_0x525_TMH,
/*(3) */
Can_Ch0_Il_Tx_Msg_IC_time_0x581_TMH,
/*(4) */
Can_Ch0_Il_Tx_Num_Messages
/*(5) */
}Can_Ch0_Il_Tx_Messages_Macro;


/* ===========================================================================
     Interaction Layer Transmit Signal Enumerations
     NB: The below tx signal sequence should match one to one with the
           IL Tx Signal description table.
         CAN_IL_SIGNAL const
         Can_Il_Tx_signals[ CAN_IL_TX_NUM_SIGNALS  ]

    =========================================================================*/

typedef enum
  {
Can_Ch0_Il_Tx_Sig_IC_BSDSoundSts_902 = 0,
/*(0) */
Can_Ch0_Il_Tx_Sig_IC_Fuel_level_914,
/*(1) */
Can_Ch0_Il_Tx_Sig_IC_DriveTimehour_914,
/*(2) */
Can_Ch0_Il_Tx_Sig_IC_DriveTimeMinute_914,
/*(3) */
Can_Ch0_Il_Tx_Sig_IC_AverageSpeed_914,
/*(4) */
Can_Ch0_Il_Tx_Sig_IC_ChangOilDistance_914,
/*(5) */
Can_Ch0_Il_Tx_Sig_IC_OdometerMasterValue_1296,
/*(6) */
Can_Ch0_Il_Tx_Sig_IC_AirbagTelltaleBehavior_1296,
/*(7) */
Can_Ch0_Il_Tx_Sig_IC_DISFail_1296,
/*(8) */
Can_Ch0_Il_Tx_Sig_IC_QDashACCFail_1296,
/*(9) */
Can_Ch0_Il_Tx_Sig_IC_VehSpd_HMI_1296,
/*(10) */
Can_Ch0_Il_Tx_Sig_IC_Rolling_counter_0x510_1296,
/*(11) */
Can_Ch0_Il_Tx_Sig_IC_Checksum_0x510_1296,
/*(12) */
Can_Ch0_Il_Tx_Sig_IC_AFC_1317,
/*(13) */
Can_Ch0_Il_Tx_Sig_IC_DTE_1317,
/*(14) */
Can_Ch0_Il_Tx_Sig_IC_AlarmStatus_1317,
/*(15) */
Can_Ch0_Il_Tx_Sig_IC_AlarmOrNot_1317,
/*(16) */
Can_Ch0_Il_Tx_Sig_IC_ManRgen_D_Rq_1317,
/*(17) */
Can_Ch0_Il_Tx_Sig_IC_AFC_Type_1317,
/*(18) */
Can_Ch0_Il_Tx_Sig_IC_Fuel_Level_Volumn_1317,
/*(19) */
Can_Ch0_Il_Tx_Sig_IC_Fuel_Level_VolumnQF_1317,
/*(20) */
Can_Ch0_Il_Tx_Sig_IC_Rolling_counter_0x525_1317,
/*(21) */
Can_Ch0_Il_Tx_Sig_IC_Checksum_0x525_1317,
/*(22) */
Can_Ch0_Il_Tx_Sig_IC_Year_1409,
/*(23) */
Can_Ch0_Il_Tx_Sig_IC_Month_1409,
/*(24) */
Can_Ch0_Il_Tx_Sig_IC_Date_1409,
/*(25) */
Can_Ch0_Il_Tx_Sig_IC_Hour_1409,
/*(26) */
Can_Ch0_Il_Tx_Sig_IC_Minute_1409,
/*(27) */
Can_Ch0_Il_Tx_Sig_IC_Second_1409,
/*(28) */
 Can_Ch0_Il_Tx_Num_Signals
/*(29) */
}Can_Ch0_Il_Tx_Signals_Macro;


/* ===========================================================================
      Interaction Layer Receive Message Enumerations
     =========================================================================*/
typedef enum
  {
Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x10B =0,
/*(0) */
Can_Ch0_Il_Rx_Msg_GW_TCU_GearInfo_Power_0x123,
/*(1) */
Can_Ch0_Il_Rx_Msg_GW_EMS_Sts_Power_0x151,
/*(2) */
Can_Ch0_Il_Rx_Msg_PEPS_Start_0x1A5,
/*(3) */
Can_Ch0_Il_Rx_Msg_GW_ABS_Sts_0x221,
/*(4) */
Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,
/*(5) */
Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,
/*(6) */
Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,
/*(7) */
Can_Ch0_Il_Rx_Msg_GW_EPS_Chassis_0x260,
/*(8) */
Can_Ch0_Il_Rx_Msg_GW_PAM_0x271,
/*(9) */
Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,
/*(10) */
Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,
/*(11) */
Can_Ch0_Il_Rx_Msg_BCM_0x313,
/*(12) */
Can_Ch0_Il_Rx_Msg_BCM_Sts_0x315,
/*(13) */
Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,
/*(14) */
Can_Ch0_Il_Rx_Msg_GW_SWM_Body_0x31A,
/*(15) */
Can_Ch0_Il_Rx_Msg_PEPS_0x325,
/*(16) */
Can_Ch0_Il_Rx_Msg_RF_0x326,
/*(17) */
Can_Ch0_Il_Rx_Msg_GW_SRS_0x350,
/*(18) */
Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,
/*(19) */
Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x362,
/*(20) */
Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x366,
/*(21) */
Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,
/*(22) */
Can_Ch0_Il_Rx_Msg_MP5_0x382,
/*(23) */
Can_Ch0_Il_Rx_Msg_MP5_0x384,
/*(24) */
Can_Ch0_Il_Rx_Msg_GW_TCU_Info_0x395,
/*(25) */
Can_Ch0_Il_Rx_Msg_MP5_0x530,
/*(26) */
Can_Ch0_Il_Rx_Msg_GW_TBOX_time_0x582,
/*(27) */
Can_Ch0_Il_Rx_Msg_BCM_0x584,
/*(28) */
Can_Ch0_Il_Rx_Msg_GW_EMS_0x636,
/*(29) */
Can_Ch0_Il_Rx_Num_Messages
/*(30) */
}Can_Ch0_Il_Rx_Messages_Macro;


/* ===========================================================================
     Interaction Layer Receive Signal Enumerations
    =========================================================================*/
  

typedef enum
{
Can_Ch0_Il_Rx_Sig_EMS_BrkPedalStasus_267 = 0,                       /*(0) */
Can_Ch0_Il_Rx_Sig_EMS_EngCoolanTemp_267,                            /*(1) */
Can_Ch0_Il_Rx_Sig_EMS_EngOperationStatus_267,                       /*(2) */
Can_Ch0_Il_Rx_Sig_EMS_EngSpeedError_267,                            /*(3) */
Can_Ch0_Il_Rx_Sig_EMS_BatteryVoltageError_267,                      /*(4) */
Can_Ch0_Il_Rx_Sig_EMS_EngSpeed_267,                                 /*(5) */
Can_Ch0_Il_Rx_Sig_EMS_CluthPedalStatus_267,                         /*(6) */
Can_Ch0_Il_Rx_Sig_TCU_DisplayGear_291,                              /*(7) */
Can_Ch0_Il_Rx_Sig_TCU_GearSelectorReq_291,                          /*(8) */
Can_Ch0_Il_Rx_Sig_TCU_StGearMode_291,                               /*(9) */
Can_Ch0_Il_Rx_Sig_TCU_Status_AliveCounter_291,                      /*(10) */
Can_Ch0_Il_Rx_Sig_TCU_Status_Checksum_291,                          /*(11) */
Can_Ch0_Il_Rx_Sig_EMS_TargCruiseSpeed_337,                          /*(12) */
Can_Ch0_Il_Rx_Sig_EMS_Rolling_counter_0x151_337,                    /*(13) */
Can_Ch0_Il_Rx_Sig_EMS_CruiseControlStatus_337,                      /*(14) */
Can_Ch0_Il_Rx_Sig_EMS_Checksum_0x151_337,                           /*(15) */
Can_Ch0_Il_Rx_Sig_PEPS_Remotestartstatus_421,                       /*(16) */
Can_Ch0_Il_Rx_Sig_PEPS_RollingCounter_0x1A5_421,                    /*(17) */
Can_Ch0_Il_Rx_Sig_PEPS_Checksum_0x1A5_421,                          /*(18) */
Can_Ch0_Il_Rx_Sig_ABS_WhlMilgFrntLe_545,                            /*(19) */
Can_Ch0_Il_Rx_Sig_ABS_WhlMilgFrntRi_545,                            /*(20) */
Can_Ch0_Il_Rx_Sig_ABS_VehSpdLgt_545,                                /*(21) */
Can_Ch0_Il_Rx_Sig_ABS_EbdFlgFlt_545,                                /*(22) */
Can_Ch0_Il_Rx_Sig_ABS_AbsFlgFlt_545,                                /*(23) */
Can_Ch0_Il_Rx_Sig_ABS_RollingCounter_0x221_545,                     /*(24) */
Can_Ch0_Il_Rx_Sig_ABS_WhlMilgFrntLeStatus_545,                      /*(25) */
Can_Ch0_Il_Rx_Sig_ABS_WhlMilgFrntRiStatus_545,                      /*(26) */
Can_Ch0_Il_Rx_Sig_ABS_Checksum_0x221_545,                           /*(27) */
Can_Ch0_Il_Rx_Sig_ESP_EPBStatus_579,                                /*(28) */
Can_Ch0_Il_Rx_Sig_ESP_EPBErrorStatus_579,                           /*(29) */
Can_Ch0_Il_Rx_Sig_ESP_CdpAvailable_579,                             /*(30) */
Can_Ch0_Il_Rx_Sig_ESP_EPBWarningMessage3_579,                       /*(31) */
Can_Ch0_Il_Rx_Sig_ESP_EPBWarningMessage2_579,                       /*(32) */
Can_Ch0_Il_Rx_Sig_ESP_EPBWarningMessage1_579,                       /*(33) */
Can_Ch0_Il_Rx_Sig_ESP_AVHWarningMessage_579,                        /*(34) */
Can_Ch0_Il_Rx_Sig_ESP_AvhActive_579,                                /*(35) */
Can_Ch0_Il_Rx_Sig_ESP_AvhAvabliable_579,                            /*(36) */
Can_Ch0_Il_Rx_Sig_ESP_VdcFlgFlt_579,                                /*(37) */
Can_Ch0_Il_Rx_Sig_ESP_TcsFlgFlt_579,                                /*(38) */
Can_Ch0_Il_Rx_Sig_ESP_ESPInfoLamp_579,                              /*(39) */
Can_Ch0_Il_Rx_Sig_ESP_EspEnableSts_579,                             /*(40) */
Can_Ch0_Il_Rx_Sig_ESP_RollingCounter_0x243_579,                     /*(41) */
Can_Ch0_Il_Rx_Sig_ESP_Checksum_0x243_579,                           /*(42) */
Can_Ch0_Il_Rx_Sig_IPM_LaneAssitType_581,                            /*(43) */
Can_Ch0_Il_Rx_Sig_IPM_LaneAssit_WarningVibration_581,               /*(44) */
Can_Ch0_Il_Rx_Sig_IPM_LaneAssit_WarningAudio_581,                   /*(45) */
Can_Ch0_Il_Rx_Sig_IPM_LaneAssit_LeftVisualization_581,              /*(46) */
Can_Ch0_Il_Rx_Sig_IPM_LaneAssit_RightVisualization_581,             /*(47) */
Can_Ch0_Il_Rx_Sig_IPM_SLASpdlimitUnits_581,                         /*(48) */
Can_Ch0_Il_Rx_Sig_IPM_SLAState_581,                                 /*(49) */
Can_Ch0_Il_Rx_Sig_IPM_LaneAssit_Status_581,                         /*(50) */
Can_Ch0_Il_Rx_Sig_IPM_SLASpdlimit_581,                              /*(51) */
Can_Ch0_Il_Rx_Sig_IPM_HMA_Status_581,                               /*(52) */
Can_Ch0_Il_Rx_Sig_IPM_DDD_IndexStatus_581,                          /*(53) */
Can_Ch0_Il_Rx_Sig_IPM_DDD_WarningLevel_581,                         /*(54) */
Can_Ch0_Il_Rx_Sig_Rolling_counter_0x245_581,                        /*(55) */
Can_Ch0_Il_Rx_Sig_IPM_LaneAssit_HandsonReq_581,                     /*(56) */
Can_Ch0_Il_Rx_Sig_Checksum_0x245_581,                               /*(57) */
Can_Ch0_Il_Rx_Sig_vSetDis_582,                                      /*(58) */
Can_Ch0_Il_Rx_Sig_MRR_ObjValid_582,                                 /*(59) */
Can_Ch0_Il_Rx_Sig_MRR_TauGapSet_582,                                /*(60) */
Can_Ch0_Il_Rx_Sig_MRR_dxTarObj_582,                                 /*(61) */
Can_Ch0_Il_Rx_Sig_ACCHMI_Mode_582,                                  /*(62) */
Can_Ch0_Il_Rx_Sig_ACCFailInfo_582,                                  /*(63) */
Can_Ch0_Il_Rx_Sig_MRR_TakeOverReq_582,                              /*(64) */
Can_Ch0_Il_Rx_Sig_MRR_AEB_STATE_582,                                /*(65) */
Can_Ch0_Il_Rx_Sig_ACC_Startstop_infor_582,                          /*(66) */
Can_Ch0_Il_Rx_Sig_MRR_PCW_preWarning_582,                           /*(67) */
Can_Ch0_Il_Rx_Sig_MRR_PCW_latentWarning_582,                        /*(68) */
Can_Ch0_Il_Rx_Sig_FCW_preWarning_582,                               /*(69) */
Can_Ch0_Il_Rx_Sig_FCW_latentWarning_582,                            /*(70) */
Can_Ch0_Il_Rx_Sig_MRR_PCW_STATE_582,                                /*(71) */
Can_Ch0_Il_Rx_Sig_Obj_Speed_582,                                    /*(72) */
Can_Ch0_Il_Rx_Sig_Rolling_counter_0x246_582,                        /*(73) */
Can_Ch0_Il_Rx_Sig_Textinfo_582,                                     /*(74) */
Can_Ch0_Il_Rx_Sig_Checksum_0x246_582,                               /*(75) */
Can_Ch0_Il_Rx_Sig_EPS_S_WarningLampYellow_608,                      /*(76) */
Can_Ch0_Il_Rx_Sig_EPS_S_SafeLampRed_608,                            /*(77) */
Can_Ch0_Il_Rx_Sig_Rolling_counter_0x260_608,                        /*(78) */
Can_Ch0_Il_Rx_Sig_Checksum_0x260_608,                               /*(79) */
Can_Ch0_Il_Rx_Sig_PAM_ChimeCmdSoundStatus_625,                      /*(80) */
Can_Ch0_Il_Rx_Sig_PAM_RearSysTemWorkSts_625,                        /*(81) */
Can_Ch0_Il_Rx_Sig_PAM_FrontSysTemWorkSts_625,                       /*(82) */
Can_Ch0_Il_Rx_Sig_PAM_ChimeCmdSoundCadence_625,                     /*(83) */
Can_Ch0_Il_Rx_Sig_SOD_LCA_warningReqRight_629,                      /*(84) */
Can_Ch0_Il_Rx_Sig_SOD_DOA_warningReqRightRear_629,                  /*(85) */
Can_Ch0_Il_Rx_Sig_SOD_DOA_warningReqRightFront_629,                 /*(86) */
Can_Ch0_Il_Rx_Sig_SOD_CTA_warningReqRight_629,                      /*(87) */
Can_Ch0_Il_Rx_Sig_SOD_BLIS_display_629,                             /*(88) */
Can_Ch0_Il_Rx_Sig_SOD_LCA_warningReqLeft_629,                       /*(89) */
Can_Ch0_Il_Rx_Sig_SOD_DOA_warningReqLeftRear_629,                   /*(90) */
Can_Ch0_Il_Rx_Sig_SOD_DOA_warningReqLeftFront_629,                  /*(91) */
Can_Ch0_Il_Rx_Sig_SOD_CTA_warningReqLeft_629,                       /*(92) */
Can_Ch0_Il_Rx_Sig_DOA_OFFTelltale_629,                              /*(93) */
Can_Ch0_Il_Rx_Sig_CTA_OFFTelltale_629,                              /*(94) */
Can_Ch0_Il_Rx_Sig_SODLCA_OFFTelltale_629,                           /*(95) */
Can_Ch0_Il_Rx_Sig_Rolling_counter_0x275_629,                        /*(96) */
Can_Ch0_Il_Rx_Sig_Checksum_0x275_629,                               /*(97) */
Can_Ch0_Il_Rx_Sig_BCM_DoorStsRearRi_784,                            /*(98) */
Can_Ch0_Il_Rx_Sig_BCM_DoorStsRearLe_784,                            /*(99) */
Can_Ch0_Il_Rx_Sig_BCM_DoorStsFrntRi_784,                            /*(100) */
Can_Ch0_Il_Rx_Sig_BCM_DoorStsFrntLe_784,                            /*(101) */
Can_Ch0_Il_Rx_Sig_BCM_DoorLockStsRearRi_784,                        /*(102) */
Can_Ch0_Il_Rx_Sig_BCM_DoorLockStsRearLe_784,                        /*(103) */
Can_Ch0_Il_Rx_Sig_BCM_DoorLockStsFrntRi_784,                        /*(104) */
Can_Ch0_Il_Rx_Sig_BCM_DoorLockStsFrntLe_784,                        /*(105) */
Can_Ch0_Il_Rx_Sig_BCM_AntiTheftSts_784,                             /*(106) */
Can_Ch0_Il_Rx_Sig_BCM_LockUnlockFeedback_784,                       /*(107) */
Can_Ch0_Il_Rx_Sig_BCM_DoorLockStsTrunk_784,                         /*(108) */
Can_Ch0_Il_Rx_Sig_BCM_DoorStsTrunk_784,                             /*(109) */
Can_Ch0_Il_Rx_Sig_BCM_TrunkDoorPreUnlockSts_784,                    /*(110) */
Can_Ch0_Il_Rx_Sig_BCM_EnginehoodSts_784,                            /*(111) */
Can_Ch0_Il_Rx_Sig_BCM_Rolling_counter_0x310_784,                    /*(112) */
Can_Ch0_Il_Rx_Sig_BCM_Checksum_0x310_784,                           /*(113) */
Can_Ch0_Il_Rx_Sig_BCM_OutsideTempDig_787,                           /*(114) */
Can_Ch0_Il_Rx_Sig_BCM_Mechanical_Hand_Brake_787,                    /*(115) */
Can_Ch0_Il_Rx_Sig_BCM_BrakeFluidSts_787,                            /*(116) */
Can_Ch0_Il_Rx_Sig_BCM_OutsideTemp_787,                              /*(117) */
Can_Ch0_Il_Rx_Sig_BCM_PowerMode_787,                                /*(118) */
Can_Ch0_Il_Rx_Sig_BCM_Rollingcounter_0x313_787,                     /*(119) */
Can_Ch0_Il_Rx_Sig_BCM_Checksum_0x313_787,                           /*(120) */
Can_Ch0_Il_Rx_Sig_BCM_BGLLumLvlSettingStatus_789,                   /*(121) */
Can_Ch0_Il_Rx_Sig_BCM_FrntFogLmpSts_792,                            /*(122) */
Can_Ch0_Il_Rx_Sig_BCM_PosLmpSts_792,                                /*(123) */
Can_Ch0_Il_Rx_Sig_BCM_LowBeamSts_792,                               /*(124) */
Can_Ch0_Il_Rx_Sig_BCM_HighBeamSts_792,                              /*(125) */
Can_Ch0_Il_Rx_Sig_BCM_reversinglightSts_792,                        /*(126) */
Can_Ch0_Il_Rx_Sig_BCM_DayRunningLampSts_792,                        /*(127) */
Can_Ch0_Il_Rx_Sig_BCM_TurnIndicatorSts_792,                         /*(128) */
Can_Ch0_Il_Rx_Sig_BCM_RearFogLmpSts_792,                            /*(129) */
Can_Ch0_Il_Rx_Sig_BCM_Highmountedstoplamp_792,                      /*(130) */
Can_Ch0_Il_Rx_Sig_BCM_BrakelightSts_792,                            /*(131) */
Can_Ch0_Il_Rx_Sig_BCM_BackHomeSts_792,                              /*(132) */
Can_Ch0_Il_Rx_Sig_BCM_AutolampSts_792,                              /*(133) */
Can_Ch0_Il_Rx_Sig_BCM_readinglampSts_792,                           /*(134) */
Can_Ch0_Il_Rx_Sig_BCM_DayRunningLampDig_792,                        /*(135) */
Can_Ch0_Il_Rx_Sig_BCM_BrakelightDig_792,                            /*(136) */
Can_Ch0_Il_Rx_Sig_BCM_reversinglightDig_792,                        /*(137) */
Can_Ch0_Il_Rx_Sig_BCM_TurnIndicatorDig_792,                         /*(138) */
Can_Ch0_Il_Rx_Sig_BCM_RearFogLmpDig_792,                            /*(139) */
Can_Ch0_Il_Rx_Sig_BCM_PosLmpDig_792,                                /*(140) */
Can_Ch0_Il_Rx_Sig_BCM_Rollingcounter_0x318_792,                     /*(141) */
Can_Ch0_Il_Rx_Sig_BCM_ImmoSts_792,                                  /*(142) */
Can_Ch0_Il_Rx_Sig_BCM_Checksum_0x318_792,                           /*(143) */
Can_Ch0_Il_Rx_Sig_SWM_Menu_Right_794,                               /*(144) */
Can_Ch0_Il_Rx_Sig_SWM_Menu_Left_794,                                /*(145) */
Can_Ch0_Il_Rx_Sig_SWM_Menu_Down_794,                                /*(146) */
Can_Ch0_Il_Rx_Sig_SWM_Menu_Up_794,                                  /*(147) */
Can_Ch0_Il_Rx_Sig_Rolling_counter_0x31A_794,                        /*(148) */
Can_Ch0_Il_Rx_Sig_SWM_Menu_Confirm_794,                             /*(149) */
Can_Ch0_Il_Rx_Sig_Checksum_0x31A_794,                               /*(150) */
Can_Ch0_Il_Rx_Sig_PEPS_PowerMode_805,                               /*(151) */
Can_Ch0_Il_Rx_Sig_PEPS_IMMO_MatchStatus_805,                        /*(152) */
Can_Ch0_Il_Rx_Sig_PEPS_SystemFailStatus_805,                        /*(153) */
Can_Ch0_Il_Rx_Sig_PEPS_ESCL_FailStatus_805,                         /*(154) */
Can_Ch0_Il_Rx_Sig_PEPS_SSB_FailStatus_805,                          /*(155) */
Can_Ch0_Il_Rx_Sig_PEPS_ClutchIndicationWarning_805,                 /*(156) */
Can_Ch0_Il_Rx_Sig_PEPS_BrakeIndicationWarning_805,                  /*(157) */
Can_Ch0_Il_Rx_Sig_PEPS_NotPNWarning_805,                            /*(158) */
Can_Ch0_Il_Rx_Sig_PEPS_NotNWarning_805,                             /*(159) */
Can_Ch0_Il_Rx_Sig_PEPS_DoorOpenNotOFFWarning_805,                   /*(160) */
Can_Ch0_Il_Rx_Sig_PEPS_DoorOpenWarning_805,                         /*(161) */
Can_Ch0_Il_Rx_Sig_PEPS_KeyBatteryWarning_805,                       /*(162) */
Can_Ch0_Il_Rx_Sig_PEPS_KeyOutWarning1_805,                          /*(163) */
Can_Ch0_Il_Rx_Sig_PEPS_NoKeyFoundWarning_805,                       /*(164) */
Can_Ch0_Il_Rx_Sig_PEPS_KeyReminderWarning_805,                      /*(165) */
Can_Ch0_Il_Rx_Sig_PEPS_RollingCounter_0x325_805,                    /*(166) */
Can_Ch0_Il_Rx_Sig_PEPS_RemainingNumSts_805,                         /*(167) */
Can_Ch0_Il_Rx_Sig_PEPS_Checksum_0x325_805,                          /*(168) */
Can_Ch0_Il_Rx_Sig_RF_KeyLowBatteryWarning_806,                      /*(169) */
Can_Ch0_Il_Rx_Sig_SRS_SBR_PAMsenger_848,                            /*(170) */
Can_Ch0_Il_Rx_Sig_SRS_SBR_Driver_848,                               /*(171) */
Can_Ch0_Il_Rx_Sig_SRS_AirBagFailSts_848,                            /*(172) */
Can_Ch0_Il_Rx_Sig_SRS_SBR_SecondRowRight_848,                       /*(173) */
Can_Ch0_Il_Rx_Sig_SRS_SBR_SecondRowMid_848,                         /*(174) */
Can_Ch0_Il_Rx_Sig_SRS_SBR_SecondRowLeft_848,                        /*(175) */
Can_Ch0_Il_Rx_Sig_Rolling_counter_0x350_848,                        /*(176) */
Can_Ch0_Il_Rx_Sig_Checksum_0x350_848,                               /*(177) */
Can_Ch0_Il_Rx_Sig_TPMS_LF_Pressure_865,                             /*(178) */
Can_Ch0_Il_Rx_Sig_TPMS_RF_Pressure_865,                             /*(179) */
Can_Ch0_Il_Rx_Sig_TPMS_RR_Pressure_865,                             /*(180) */
Can_Ch0_Il_Rx_Sig_TPMS_LR_Pressure_865,                             /*(181) */
Can_Ch0_Il_Rx_Sig_TPMS_Tire_Temperature_865,                        /*(182) */
Can_Ch0_Il_Rx_Sig_TPMS_LF_Pressure_Warning_865,                     /*(183) */
Can_Ch0_Il_Rx_Sig_TPMS_RF_Pressure_Warning_865,                     /*(184) */
Can_Ch0_Il_Rx_Sig_TPMS_System_Status_865,                           /*(185) */
Can_Ch0_Il_Rx_Sig_TPMS_RR_Pressure_Warning_865,                     /*(186) */
Can_Ch0_Il_Rx_Sig_TPMS_LR_Pressure_Warning_865,                     /*(187) */
Can_Ch0_Il_Rx_Sig_Temperature_Warning_865,                          /*(188) */
Can_Ch0_Il_Rx_Sig_resrved_865,                                      /*(189) */
Can_Ch0_Il_Rx_Sig_Tire_Position_865,                                /*(190) */
Can_Ch0_Il_Rx_Sig_TPMS_Lamp_Status_865,                             /*(191) */
Can_Ch0_Il_Rx_Sig_RRI_Pressure_866,                                 /*(192) */
Can_Ch0_Il_Rx_Sig_LRI_Pressure_866,                                 /*(193) */
Can_Ch0_Il_Rx_Sig_RRI_Pressure_Warning_866,                         /*(194) */
Can_Ch0_Il_Rx_Sig_LRI_Pressure_Warning_866,                         /*(195) */
Can_Ch0_Il_Rx_Sig_EMS_Fuelcounter_870,                              /*(196) */
Can_Ch0_Il_Rx_Sig_EMS_RemindShiftGear_MT_870,                       /*(197) */
Can_Ch0_Il_Rx_Sig_EMS_MIL_870,                                      /*(198) */
Can_Ch0_Il_Rx_Sig_EMS_SVS_870,                                      /*(199) */
Can_Ch0_Il_Rx_Sig_EMS_StartStopStatus_870,                          /*(200) */
Can_Ch0_Il_Rx_Sig_EMS_OBDTxt_870,                                   /*(201) */
Can_Ch0_Il_Rx_Sig_EMS_TroqLimtHour_872,                             /*(202) */
Can_Ch0_Il_Rx_Sig_EMS_UreaQty_872,                                  /*(203) */
Can_Ch0_Il_Rx_Sig_EMS_TroqLimtMin_872,                              /*(204) */
Can_Ch0_Il_Rx_Sig_EMS_UreaLvl_872,                                  /*(205) */
Can_Ch0_Il_Rx_Sig_EMS_DieslPrtcWarn_D_Rq_872,                       /*(206) */
Can_Ch0_Il_Rx_Sig_DCM_GlowPlugLmpReq_872,                           /*(207) */
Can_Ch0_Il_Rx_Sig_DCM_WIFFullLmpReq_872,                            /*(208) */
Can_Ch0_Il_Rx_Sig_EMS_SpeedLimtHour_872,                            /*(209) */
Can_Ch0_Il_Rx_Sig_EMS_SpeedLimtMin_872,                             /*(210) */
Can_Ch0_Il_Rx_Sig_EMS_UreaSysWarning_872,                           /*(211) */
Can_Ch0_Il_Rx_Sig_EMS_UreaTxt_872,                                  /*(212) */
Can_Ch0_Il_Rx_Sig_EMS_ManRgenTxt_D_RqDsply_872,                     /*(213) */
Can_Ch0_Il_Rx_Sig_EMS_UreaWarn_872,                                 /*(214) */
Can_Ch0_Il_Rx_Sig_EMS_Soot_Pc_Dsply_872,                            /*(215) */
Can_Ch0_Il_Rx_Sig_MP5_BSDSoundSwitchSts_898,                        /*(216) */
Can_Ch0_Il_Rx_Sig_MP5_NavICON_900,                                  /*(217) */
Can_Ch0_Il_Rx_Sig_MP5_ROUTE_REMAIN_DIS_900,                         /*(218) */
Can_Ch0_Il_Rx_Sig_MP5_ALModeSetting_900,                            /*(219) */
Can_Ch0_Il_Rx_Sig_MP5_ALStartAroundSetting_900,                     /*(220) */
Can_Ch0_Il_Rx_Sig_Trans_Limphome_917,                               /*(221) */
Can_Ch0_Il_Rx_Sig_TrnSrvcRqd_B_Rq_917,                              /*(222) */
Can_Ch0_Il_Rx_Sig_TCU_Gearbox_Gonglimp_917,                         /*(223) */
Can_Ch0_Il_Rx_Sig_TCU_Driverwarning_917,                            /*(224) */
Can_Ch0_Il_Rx_Sig_TCU_DrvShiftAllwCondRmd_917,                      /*(225) */
Can_Ch0_Il_Rx_Sig_MP5_AlarmStatus_1328,                             /*(226) */
Can_Ch0_Il_Rx_Sig_MP5_AlarmOrNot_1328,                              /*(227) */
Can_Ch0_Il_Rx_Sig_TBOX_Year_1410,                                   /*(228) */
Can_Ch0_Il_Rx_Sig_TBOX_Month_1410,                                  /*(229) */
Can_Ch0_Il_Rx_Sig_TBOX_Date_1410,                                   /*(230) */
Can_Ch0_Il_Rx_Sig_TBOX_Hour_1410,                                   /*(231) */
Can_Ch0_Il_Rx_Sig_TBOX_Minute_1410,                                 /*(232) */
Can_Ch0_Il_Rx_Sig_TBOX_Second_1410,                                 /*(233) */
Can_Ch0_Il_Rx_Sig_TBOX_RealTime_UB_Copy_1_1410,                     /*(234) */
Can_Ch0_Il_Rx_Sig_TBOX_RealTime_UB_1410,                            /*(235) */
Can_Ch0_Il_Rx_Sig_BCM_LoadManageLevel_1412,                         /*(236) */
Can_Ch0_Il_Rx_Sig_BCM_CarModeIndicator_1412,                        /*(237) */
Can_Ch0_Il_Rx_Sig_BCM_CarMode_1412,                                 /*(238) */
Can_Ch0_Il_Rx_Sig_EMS_BackupOdometer_1590,                          /*(239) */
Can_Ch0_Il_Rx_Num_Signals              /*(240) */
}Can_Ch0_Il_Rx_Signals_Macro;


/* ===========================================================================
    Interaction Layer Receive Signal (Multiple Bytes) Rx Get Macros
   =========================================================================*/
/* ===========================================================================
   P U B L I C   M E M O R Y
  =========================================================================*/

extern CAN_IL_SIGNAL          const      Can_Ch0_Il_Tx_Signals[ Can_Ch0_Il_Tx_Num_Signals ];

extern CAN_IL_TX_MESSAGE      const      Can_Ch0_Il_Tx_Messages[ Can_Ch0_Il_Tx_Num_Messages ];

extern CAN_IL_TX_FRAME        const      Can_Ch0_Il_Tx_Frame_Table[ Can_Ch0_Il_Tx_Num_Messages ];

extern CAN_IL_SIGNAL          const      Can_Ch0_Il_Rx_Signals[Can_Ch0_Il_Rx_Num_Signals  ];

/*extern CAN_IL_RX_MESSAGE      const      Can_Ch0_Il_Rx_Messages[ ];*/

extern CAN_IL_RX_FRAME        const      Can_Ch0_Il_Rx_Frame_Table[ Can_Ch0_Il_Rx_Num_Messages ];

/*extern Can_Il_Rx_Data_Pointer const      Can_Ch0_Il_Receive_Data_Table[ ];*/

extern pTxPrecopyfn           const      Can_Ch0_Il_Tx_Precopy_Function_Table[ Can_Ch0_Il_Tx_Num_Messages ];

extern CAN_UINT8 Can_Ch0_Il_Rx_Frame_Data[ Can_Ch0_Il_Rx_Num_Messages ][ CAN_MAX_DATA_LENGTH ];

extern CAN_UINT8 Can_Ch0_Il_Rx_Frame_Status[ Can_Ch0_Il_Rx_Num_Messages ];

extern CAN_UINT16 Can_Ch0_Il_Rx_Timeout_Count[ Can_Ch0_Il_Rx_Num_Messages ];

extern Can_Il_Rx_Data_Pointer Can_Ch0_Il_Receive_Ptr[ Can_Ch0_Il_Rx_Num_Messages ][ CAN_MAX_DATA_LENGTH ];

#ifdef CAN_IL_TX_BURST_MODE

 #if ( Can_Ch0_Il_Tx_Num_Burst_Periodic > 0 )

  extern CAN_UINT8 Can_Ch0_Il_Tx_Burst_Count[ Can_Ch0_Il_Tx_Num_Burst_Periodic ];

 #endif

#endif

extern void Can_Ch0_Il_Precopy_Function(void);


#endif  /* CAN_IL_PAR_H */ 
/*****************************************************************************
    R E V I S I O N     N O T E S
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  -
For each change to this file, be sure to record:
1.   Who made the change and when the change was made.
2.   Why the change was made and the intended result.

Date       By         Reason For Change
------------------------------------------------------------------------------

******************************************************************************/
/*****************************************************************************
Date          : 2020-08-27 15:12:52
By              : ChengJia
Traceability    : JMC_SUV_Body_CAN_Matrix_20200827.dbc
Change Description  : Tool Generated code
*****************************************************************************/
