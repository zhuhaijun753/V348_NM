#if !defined( CCL_APP_CFG_H )
  #define CCL_APP_CFG_H

  /* ===========================================================================
  **
  **                     CONFIDENTIAL VISTEON CORPORATION
  **
  **  This is an unpublished work of authorship  which contains trade secrets 
  **  created in 2007.  Visteon Corporation owns all rights to this work and
  **  intends to maintain it in confidence to preserve its trade secret status.
  **  Visteon Corporation reserves the right  under the copyright laws of the
  **  United States or those of any other country that may have jurisdiction  to
  **  protect this work as an unpublished work  in the event of an inadvertent
  **  or deliberate unauthorized publication.  Visteon Corporation also reserves
  **  its rights under all copyright laws to protect this work as a published
  **  work  when appropriate.  Those having access to this work may not copy it 
  **  use it  modify it or disclose the information contained in it without the
  **  written authorization of Visteon Corporation.
  **
  ** =========================================================================*/

  /* ===========================================================================
  **
  **  Name:           CanCcl_Par_Cfg.h
  **
  **  Description:    CAN CCL configuration parameters for configured database
  **
  **  Organization:   Vehicle Communications
  **                  Visteon Corporation
  **
  ** =========================================================================*/

  /* ===========================================================================
  **  I N C L U D E   F I L E S
  ** =========================================================================*/

  #include "CanCcl_Cfg.h"
  #include "CanCcl_Defines.h"
  #include "Com_Handles.h"
   

/* ===========================================================================
  **  M A C R O   D E F I N I T I O N S
  ** =========================================================================*/

  /* DBC Tx & Rx CFG */

  #ifndef Can_ControllerId
  #define Can_ControllerId                        (0u)
  #endif

#define Can_Ccl_Rx_Num_Messages                 (30u)
#define Can_Ccl_Rx_Num_Periodic_Messages        (30u) /*no use*/
#define Can_Ccl_Rx_Num_Signals                  (240u)
#define Can_Ccl_Rx_Num_Periodic_Signals         (240u) /*no use*/
#define Can_Ccl_Rx_Num_RxAck_Ind                (240u)
#define Can_Ccl_Rx_Num_RxTOut_Ind               (240u)
#define Can_Ccl_Rx_Num_RxInv_Ind                (1u)

#define Can_Ccl_Tx_Num_Messages                 (5u)
#define Can_Ccl_Tx_Signal_Num_Signals           (29u)
#define Can_Ccl_Tx_Num_Periodic                 (5u)
#define Can_Ccl_Tx_Num_Burst_Periodic           (0u)

#define Can_Ccl_Num_Of_PduGroups                (2u)
  #define CCL_MAX_IPDUGROUPVECTOR_BYTES           (1u) 


/*Handle IDs active in all predefined variants (the application has not to take the active variant into account) */
  /*      Symbolic Name                                  Value   Active in predefined variant(s) */
  #define CanCclConf_IPduGroup_TuringECU_CAN_Rx           1u
  #define CanCclConf_IPduGroup_TuringECU_CAN_Tx           0u



  /* Handle IDs of handle space CclRxSig [Rx Signals] */
	/*		Symbolic Name																	   Value   Active in predefined variant(s) */
#define CanCclRxHndlCh0_EMS_BrkPedalStasus_267                          0u
#define CanCclRxHndlCh0_EMS_EngCoolanTemp_267                           1u
#define CanCclRxHndlCh0_EMS_EngOperationStatus_267                      2u
#define CanCclRxHndlCh0_EMS_EngSpeedError_267                           3u
#define CanCclRxHndlCh0_EMS_BatteryVoltageError_267                     4u
#define CanCclRxHndlCh0_EMS_EngSpeed_267                                5u
#define CanCclRxHndlCh0_EMS_CluthPedalStatus_267                        6u
#define CanCclRxHndlCh0_TCU_DisplayGear_291                             7u
#define CanCclRxHndlCh0_TCU_GearSelectorReq_291                         8u
#define CanCclRxHndlCh0_TCU_StGearMode_291                              9u
#define CanCclRxHndlCh0_TCU_Status_AliveCounter_291                     10u
#define CanCclRxHndlCh0_TCU_Status_Checksum_291                         11u
#define CanCclRxHndlCh0_EMS_TargCruiseSpeed_337                         12u
#define CanCclRxHndlCh0_EMS_Rolling_counter_0x151_337                   13u
#define CanCclRxHndlCh0_EMS_CruiseControlStatus_337                     14u
#define CanCclRxHndlCh0_EMS_Checksum_0x151_337                          15u
#define CanCclRxHndlCh0_PEPS_Remotestartstatus_421                      16u
#define CanCclRxHndlCh0_PEPS_RollingCounter_0x1A5_421                   17u
#define CanCclRxHndlCh0_PEPS_Checksum_0x1A5_421                         18u
#define CanCclRxHndlCh0_ABS_WhlMilgFrntLe_545                           19u
#define CanCclRxHndlCh0_ABS_WhlMilgFrntRi_545                           20u
#define CanCclRxHndlCh0_ABS_VehSpdLgt_545                               21u
#define CanCclRxHndlCh0_ABS_EbdFlgFlt_545                               22u
#define CanCclRxHndlCh0_ABS_AbsFlgFlt_545                               23u
#define CanCclRxHndlCh0_ABS_RollingCounter_0x221_545                    24u
#define CanCclRxHndlCh0_ABS_WhlMilgFrntLeStatus_545                     25u
#define CanCclRxHndlCh0_ABS_WhlMilgFrntRiStatus_545                     26u
#define CanCclRxHndlCh0_ABS_Checksum_0x221_545                          27u
#define CanCclRxHndlCh0_ESP_EPBStatus_579                               28u
#define CanCclRxHndlCh0_ESP_EPBErrorStatus_579                          29u
#define CanCclRxHndlCh0_ESP_CdpAvailable_579                            30u
#define CanCclRxHndlCh0_ESP_EPBWarningMessage3_579                      31u
#define CanCclRxHndlCh0_ESP_EPBWarningMessage2_579                      32u
#define CanCclRxHndlCh0_ESP_EPBWarningMessage1_579                      33u
#define CanCclRxHndlCh0_ESP_AVHWarningMessage_579                       34u
#define CanCclRxHndlCh0_ESP_AvhActive_579                               35u
#define CanCclRxHndlCh0_ESP_AvhAvabliable_579                           36u
#define CanCclRxHndlCh0_ESP_VdcFlgFlt_579                               37u
#define CanCclRxHndlCh0_ESP_TcsFlgFlt_579                               38u
#define CanCclRxHndlCh0_ESP_ESPInfoLamp_579                             39u
#define CanCclRxHndlCh0_ESP_EspEnableSts_579                            40u
#define CanCclRxHndlCh0_ESP_RollingCounter_0x243_579                    41u
#define CanCclRxHndlCh0_ESP_Checksum_0x243_579                          42u
#define CanCclRxHndlCh0_IPM_LaneAssitType_581                           43u
#define CanCclRxHndlCh0_IPM_LaneAssit_WarningVibration_581              44u
#define CanCclRxHndlCh0_IPM_LaneAssit_WarningAudio_581                  45u
#define CanCclRxHndlCh0_IPM_LaneAssit_LeftVisualization_581              46u
#define CanCclRxHndlCh0_IPM_LaneAssit_RightVisualization_581              47u
#define CanCclRxHndlCh0_IPM_SLASpdlimitUnits_581                        48u
#define CanCclRxHndlCh0_IPM_SLAState_581                                49u
#define CanCclRxHndlCh0_IPM_LaneAssit_Status_581                        50u
#define CanCclRxHndlCh0_IPM_SLASpdlimit_581                             51u
#define CanCclRxHndlCh0_IPM_HMA_Status_581                              52u
#define CanCclRxHndlCh0_IPM_DDD_IndexStatus_581                         53u
#define CanCclRxHndlCh0_IPM_DDD_WarningLevel_581                        54u
#define CanCclRxHndlCh0_Rolling_counter_0x245_581                       55u
#define CanCclRxHndlCh0_IPM_LaneAssit_HandsonReq_581                    56u
#define CanCclRxHndlCh0_Checksum_0x245_581                              57u
#define CanCclRxHndlCh0_vSetDis_582                                     58u
#define CanCclRxHndlCh0_MRR_ObjValid_582                                59u
#define CanCclRxHndlCh0_MRR_TauGapSet_582                               60u
#define CanCclRxHndlCh0_MRR_dxTarObj_582                                61u
#define CanCclRxHndlCh0_ACCHMI_Mode_582                                 62u
#define CanCclRxHndlCh0_ACCFailInfo_582                                 63u
#define CanCclRxHndlCh0_MRR_TakeOverReq_582                             64u
#define CanCclRxHndlCh0_MRR_AEB_STATE_582                               65u
#define CanCclRxHndlCh0_ACC_Startstop_infor_582                         66u
#define CanCclRxHndlCh0_MRR_PCW_preWarning_582                          67u
#define CanCclRxHndlCh0_MRR_PCW_latentWarning_582                       68u
#define CanCclRxHndlCh0_FCW_preWarning_582                              69u
#define CanCclRxHndlCh0_FCW_latentWarning_582                           70u
#define CanCclRxHndlCh0_MRR_PCW_STATE_582                               71u
#define CanCclRxHndlCh0_Obj_Speed_582                                   72u
#define CanCclRxHndlCh0_Rolling_counter_0x246_582                       73u
#define CanCclRxHndlCh0_Textinfo_582                                    74u
#define CanCclRxHndlCh0_Checksum_0x246_582                              75u
#define CanCclRxHndlCh0_EPS_S_WarningLampYellow_608                     76u
#define CanCclRxHndlCh0_EPS_S_SafeLampRed_608                           77u
#define CanCclRxHndlCh0_Rolling_counter_0x260_608                       78u
#define CanCclRxHndlCh0_Checksum_0x260_608                              79u
#define CanCclRxHndlCh0_PAM_ChimeCmdSoundStatus_625                     80u
#define CanCclRxHndlCh0_PAM_RearSysTemWorkSts_625                       81u
#define CanCclRxHndlCh0_PAM_FrontSysTemWorkSts_625                      82u
#define CanCclRxHndlCh0_PAM_ChimeCmdSoundCadence_625                    83u
#define CanCclRxHndlCh0_SOD_LCA_warningReqRight_629                     84u
#define CanCclRxHndlCh0_SOD_DOA_warningReqRightRear_629                 85u
#define CanCclRxHndlCh0_SOD_DOA_warningReqRightFront_629                86u
#define CanCclRxHndlCh0_SOD_CTA_warningReqRight_629                     87u
#define CanCclRxHndlCh0_SOD_BLIS_display_629                            88u
#define CanCclRxHndlCh0_SOD_LCA_warningReqLeft_629                      89u
#define CanCclRxHndlCh0_SOD_DOA_warningReqLeftRear_629                  90u
#define CanCclRxHndlCh0_SOD_DOA_warningReqLeftFront_629                 91u
#define CanCclRxHndlCh0_SOD_CTA_warningReqLeft_629                      92u
#define CanCclRxHndlCh0_DOA_OFFTelltale_629                             93u
#define CanCclRxHndlCh0_CTA_OFFTelltale_629                             94u
#define CanCclRxHndlCh0_SODLCA_OFFTelltale_629                          95u
#define CanCclRxHndlCh0_Rolling_counter_0x275_629                       96u
#define CanCclRxHndlCh0_Checksum_0x275_629                              97u
#define CanCclRxHndlCh0_BCM_DoorStsRearRi_784                           98u
#define CanCclRxHndlCh0_BCM_DoorStsRearLe_784                           99u
#define CanCclRxHndlCh0_BCM_DoorStsFrntRi_784                           100u
#define CanCclRxHndlCh0_BCM_DoorStsFrntLe_784                           101u
#define CanCclRxHndlCh0_BCM_DoorLockStsRearRi_784                       102u
#define CanCclRxHndlCh0_BCM_DoorLockStsRearLe_784                       103u
#define CanCclRxHndlCh0_BCM_DoorLockStsFrntRi_784                       104u
#define CanCclRxHndlCh0_BCM_DoorLockStsFrntLe_784                       105u
#define CanCclRxHndlCh0_BCM_AntiTheftSts_784                            106u
#define CanCclRxHndlCh0_BCM_LockUnlockFeedback_784                      107u
#define CanCclRxHndlCh0_BCM_DoorLockStsTrunk_784                        108u
#define CanCclRxHndlCh0_BCM_DoorStsTrunk_784                            109u
#define CanCclRxHndlCh0_BCM_TrunkDoorPreUnlockSts_784                   110u
#define CanCclRxHndlCh0_BCM_EnginehoodSts_784                           111u
#define CanCclRxHndlCh0_BCM_Rolling_counter_0x310_784                   112u
#define CanCclRxHndlCh0_BCM_Checksum_0x310_784                          113u
#define CanCclRxHndlCh0_BCM_OutsideTempDig_787                          114u
#define CanCclRxHndlCh0_BCM_Mechanical_Hand_Brake_787                   115u
#define CanCclRxHndlCh0_BCM_BrakeFluidSts_787                           116u
#define CanCclRxHndlCh0_BCM_OutsideTemp_787                             117u
#define CanCclRxHndlCh0_BCM_PowerMode_787                               118u
#define CanCclRxHndlCh0_BCM_Rollingcounter_0x313_787                    119u
#define CanCclRxHndlCh0_BCM_Checksum_0x313_787                          120u
#define CanCclRxHndlCh0_BCM_BGLLumLvlSettingStatus_789                  121u
#define CanCclRxHndlCh0_BCM_FrntFogLmpSts_792                           122u
#define CanCclRxHndlCh0_BCM_PosLmpSts_792                               123u
#define CanCclRxHndlCh0_BCM_LowBeamSts_792                              124u
#define CanCclRxHndlCh0_BCM_HighBeamSts_792                             125u
#define CanCclRxHndlCh0_BCM_reversinglightSts_792                       126u
#define CanCclRxHndlCh0_BCM_DayRunningLampSts_792                       127u
#define CanCclRxHndlCh0_BCM_TurnIndicatorSts_792                        128u
#define CanCclRxHndlCh0_BCM_RearFogLmpSts_792                           129u
#define CanCclRxHndlCh0_BCM_Highmountedstoplamp_792                     130u
#define CanCclRxHndlCh0_BCM_BrakelightSts_792                           131u
#define CanCclRxHndlCh0_BCM_BackHomeSts_792                             132u
#define CanCclRxHndlCh0_BCM_AutolampSts_792                             133u
#define CanCclRxHndlCh0_BCM_readinglampSts_792                          134u
#define CanCclRxHndlCh0_BCM_DayRunningLampDig_792                       135u
#define CanCclRxHndlCh0_BCM_BrakelightDig_792                           136u
#define CanCclRxHndlCh0_BCM_reversinglightDig_792                       137u
#define CanCclRxHndlCh0_BCM_TurnIndicatorDig_792                        138u
#define CanCclRxHndlCh0_BCM_RearFogLmpDig_792                           139u
#define CanCclRxHndlCh0_BCM_PosLmpDig_792                               140u
#define CanCclRxHndlCh0_BCM_Rollingcounter_0x318_792                    141u
#define CanCclRxHndlCh0_BCM_ImmoSts_792                                 142u
#define CanCclRxHndlCh0_BCM_Checksum_0x318_792                          143u
#define CanCclRxHndlCh0_SWM_Menu_Right_794                              144u
#define CanCclRxHndlCh0_SWM_Menu_Left_794                               145u
#define CanCclRxHndlCh0_SWM_Menu_Down_794                               146u
#define CanCclRxHndlCh0_SWM_Menu_Up_794                                 147u
#define CanCclRxHndlCh0_Rolling_counter_0x31A_794                       148u
#define CanCclRxHndlCh0_SWM_Menu_Confirm_794                            149u
#define CanCclRxHndlCh0_Checksum_0x31A_794                              150u
#define CanCclRxHndlCh0_PEPS_PowerMode_805                              151u
#define CanCclRxHndlCh0_PEPS_IMMO_MatchStatus_805                       152u
#define CanCclRxHndlCh0_PEPS_SystemFailStatus_805                       153u
#define CanCclRxHndlCh0_PEPS_ESCL_FailStatus_805                        154u
#define CanCclRxHndlCh0_PEPS_SSB_FailStatus_805                         155u
#define CanCclRxHndlCh0_PEPS_ClutchIndicationWarning_805                156u
#define CanCclRxHndlCh0_PEPS_BrakeIndicationWarning_805                 157u
#define CanCclRxHndlCh0_PEPS_NotPNWarning_805                           158u
#define CanCclRxHndlCh0_PEPS_NotNWarning_805                            159u
#define CanCclRxHndlCh0_PEPS_DoorOpenNotOFFWarning_805                  160u
#define CanCclRxHndlCh0_PEPS_DoorOpenWarning_805                        161u
#define CanCclRxHndlCh0_PEPS_KeyBatteryWarning_805                      162u
#define CanCclRxHndlCh0_PEPS_KeyOutWarning1_805                         163u
#define CanCclRxHndlCh0_PEPS_NoKeyFoundWarning_805                      164u
#define CanCclRxHndlCh0_PEPS_KeyReminderWarning_805                     165u
#define CanCclRxHndlCh0_PEPS_RollingCounter_0x325_805                   166u
#define CanCclRxHndlCh0_PEPS_RemainingNumSts_805                        167u
#define CanCclRxHndlCh0_PEPS_Checksum_0x325_805                         168u
#define CanCclRxHndlCh0_RF_KeyLowBatteryWarning_806                     169u
#define CanCclRxHndlCh0_SRS_SBR_PAMsenger_848                           170u
#define CanCclRxHndlCh0_SRS_SBR_Driver_848                              171u
#define CanCclRxHndlCh0_SRS_AirBagFailSts_848                           172u
#define CanCclRxHndlCh0_SRS_SBR_SecondRowRight_848                      173u
#define CanCclRxHndlCh0_SRS_SBR_SecondRowMid_848                        174u
#define CanCclRxHndlCh0_SRS_SBR_SecondRowLeft_848                       175u
#define CanCclRxHndlCh0_Rolling_counter_0x350_848                       176u
#define CanCclRxHndlCh0_Checksum_0x350_848                              177u
#define CanCclRxHndlCh0_TPMS_LF_Pressure_865                            178u
#define CanCclRxHndlCh0_TPMS_RF_Pressure_865                            179u
#define CanCclRxHndlCh0_TPMS_RR_Pressure_865                            180u
#define CanCclRxHndlCh0_TPMS_LR_Pressure_865                            181u
#define CanCclRxHndlCh0_TPMS_Tire_Temperature_865                       182u
#define CanCclRxHndlCh0_TPMS_LF_Pressure_Warning_865                    183u
#define CanCclRxHndlCh0_TPMS_RF_Pressure_Warning_865                    184u
#define CanCclRxHndlCh0_TPMS_System_Status_865                          185u
#define CanCclRxHndlCh0_TPMS_RR_Pressure_Warning_865                    186u
#define CanCclRxHndlCh0_TPMS_LR_Pressure_Warning_865                    187u
#define CanCclRxHndlCh0_Temperature_Warning_865                         188u
#define CanCclRxHndlCh0_resrved_865                                     189u
#define CanCclRxHndlCh0_Tire_Position_865                               190u
#define CanCclRxHndlCh0_TPMS_Lamp_Status_865                            191u
#define CanCclRxHndlCh0_RRI_Pressure_866                                192u
#define CanCclRxHndlCh0_LRI_Pressure_866                                193u
#define CanCclRxHndlCh0_RRI_Pressure_Warning_866                        194u
#define CanCclRxHndlCh0_LRI_Pressure_Warning_866                        195u
#define CanCclRxHndlCh0_EMS_Fuelcounter_870                             196u
#define CanCclRxHndlCh0_EMS_RemindShiftGear_MT_870                      197u
#define CanCclRxHndlCh0_EMS_MIL_870                                     198u
#define CanCclRxHndlCh0_EMS_SVS_870                                     199u
#define CanCclRxHndlCh0_EMS_StartStopStatus_870                         200u
#define CanCclRxHndlCh0_EMS_OBDTxt_870                                  201u
#define CanCclRxHndlCh0_EMS_TroqLimtHour_872                            202u
#define CanCclRxHndlCh0_EMS_UreaQty_872                                 203u
#define CanCclRxHndlCh0_EMS_TroqLimtMin_872                             204u
#define CanCclRxHndlCh0_EMS_UreaLvl_872                                 205u
#define CanCclRxHndlCh0_EMS_DieslPrtcWarn_D_Rq_872                      206u
#define CanCclRxHndlCh0_DCM_GlowPlugLmpReq_872                          207u
#define CanCclRxHndlCh0_DCM_WIFFullLmpReq_872                           208u
#define CanCclRxHndlCh0_EMS_SpeedLimtHour_872                           209u
#define CanCclRxHndlCh0_EMS_SpeedLimtMin_872                            210u
#define CanCclRxHndlCh0_EMS_UreaSysWarning_872                          211u
#define CanCclRxHndlCh0_EMS_UreaTxt_872                                 212u
#define CanCclRxHndlCh0_EMS_ManRgenTxt_D_RqDsply_872                    213u
#define CanCclRxHndlCh0_EMS_UreaWarn_872                                214u
#define CanCclRxHndlCh0_EMS_Soot_Pc_Dsply_872                           215u
#define CanCclRxHndlCh0_MP5_BSDSoundSwitchSts_898                       216u
#define CanCclRxHndlCh0_MP5_NavICON_900                                 217u
#define CanCclRxHndlCh0_MP5_ROUTE_REMAIN_DIS_900                        218u
#define CanCclRxHndlCh0_MP5_ALModeSetting_900                           219u
#define CanCclRxHndlCh0_MP5_ALStartAroundSetting_900                    220u
#define CanCclRxHndlCh0_Trans_Limphome_917                              221u
#define CanCclRxHndlCh0_TrnSrvcRqd_B_Rq_917                             222u
#define CanCclRxHndlCh0_TCU_Gearbox_Gonglimp_917                        223u
#define CanCclRxHndlCh0_TCU_Driverwarning_917                           224u
#define CanCclRxHndlCh0_TCU_DrvShiftAllwCondRmd_917                     225u
#define CanCclRxHndlCh0_MP5_AlarmStatus_1328                            226u
#define CanCclRxHndlCh0_MP5_AlarmOrNot_1328                             227u
#define CanCclRxHndlCh0_TBOX_Year_1410                                  228u
#define CanCclRxHndlCh0_TBOX_Month_1410                                 229u
#define CanCclRxHndlCh0_TBOX_Date_1410                                  230u
#define CanCclRxHndlCh0_TBOX_Hour_1410                                  231u
#define CanCclRxHndlCh0_TBOX_Minute_1410                                232u
#define CanCclRxHndlCh0_TBOX_Second_1410                                233u
#define CanCclRxHndlCh0_TBOX_RealTime_UB_Copy_1_1410                    234u
#define CanCclRxHndlCh0_TBOX_RealTime_UB_1410                           235u
#define CanCclRxHndlCh0_BCM_LoadManageLevel_1412                        236u
#define CanCclRxHndlCh0_BCM_CarModeIndicator_1412                       237u
#define CanCclRxHndlCh0_BCM_CarMode_1412                                238u
#define CanCclRxHndlCh0_EMS_BackupOdometer_1590                         239u




/* Handle IDs of handle space CclTxSig [Tx Signals] */
  /*      Symbolic Name                                                                      Value   Active in predefined variant(s) */
#define CanCclTxHndlCh0_IC_BSDSoundSts_902                              0u
#define CanCclTxHndlCh0_IC_Fuel_level_914                               1u
#define CanCclTxHndlCh0_IC_DriveTimehour_914                            2u
#define CanCclTxHndlCh0_IC_DriveTimeMinute_914                          3u
#define CanCclTxHndlCh0_IC_AverageSpeed_914                             4u
#define CanCclTxHndlCh0_IC_ChangOilDistance_914                         5u
#define CanCclTxHndlCh0_IC_OdometerMasterValue_1296                     6u
#define CanCclTxHndlCh0_IC_AirbagTelltaleBehavior_1296                  7u
#define CanCclTxHndlCh0_IC_DISFail_1296                                 8u
#define CanCclTxHndlCh0_IC_QDashACCFail_1296                            9u
#define CanCclTxHndlCh0_IC_VehSpd_HMI_1296                              10u
#define CanCclTxHndlCh0_IC_Rolling_counter_0x510_1296                   11u
#define CanCclTxHndlCh0_IC_Checksum_0x510_1296                          12u
#define CanCclTxHndlCh0_IC_AFC_1317                                     13u
#define CanCclTxHndlCh0_IC_DTE_1317                                     14u
#define CanCclTxHndlCh0_IC_AlarmStatus_1317                             15u
#define CanCclTxHndlCh0_IC_AlarmOrNot_1317                              16u
#define CanCclTxHndlCh0_IC_ManRgen_D_Rq_1317                            17u
#define CanCclTxHndlCh0_IC_AFC_Type_1317                                18u
#define CanCclTxHndlCh0_IC_Fuel_Level_Volumn_1317                       19u
#define CanCclTxHndlCh0_IC_Fuel_Level_VolumnQF_1317                     20u
#define CanCclTxHndlCh0_IC_Rolling_counter_0x525_1317                   21u
#define CanCclTxHndlCh0_IC_Checksum_0x525_1317                          22u
#define CanCclTxHndlCh0_IC_Year_1409                                    23u
#define CanCclTxHndlCh0_IC_Month_1409                                   24u
#define CanCclTxHndlCh0_IC_Date_1409                                    25u
#define CanCclTxHndlCh0_IC_Hour_1409                                    26u
#define CanCclTxHndlCh0_IC_Minute_1409                                  27u
#define CanCclTxHndlCh0_IC_Second_1409                                  28u



typedef CAN_UINT8 Ccl_IpduGroupVector[CCL_MAX_IPDUGROUPVECTOR_BYTES];


  # include "CanCcl_Com.h"


/* ===========================================================================
** G L O B A L   C O N S T A N T   D E C L A R A T I O N S
** =========================================================================*/

extern CONST(Ccl_TxSigInfoType, CCL_CONST) Can_Ccl_TxSigInfo[Can_Ccl_Tx_Signal_Num_Signals];

extern CONST(Ccl_RxAccessInfoType, CCL_CONST) Can_Ccl_RxAccessInfo[Can_Ccl_Rx_Num_Signals];


/**********************************************************************************************************************
  Ccl_CbkRxAckFuncPtr
**********************************************************************************************************************/
/**
  \var    Ccl_CbkRxAckFuncPtr
  \brief  Function pointer table containing configured Rx indication notifications for signals and signal groups.
*/

extern CONST(CclCbkRxAckType, CCL_CONST) Can_Ccl_CbkRxAckFuncPtr[Can_Ccl_Rx_Num_RxAck_Ind];

/**********************************************************************************************************************
  Ccl_CbkRxTOutFuncPtr
**********************************************************************************************************************/
/**
  \var    Ccl_CbkRxTOutFuncPtr
  \brief  Function pointer table containing configured Rx timeout notifications for signals and signal groups.}
*/

extern CONST(CclCbkRxTOutType, CCL_CONST) Can_Ccl_CbkRxTOutFuncPtr[Can_Ccl_Rx_Num_RxTOut_Ind];


/**********************************************************************************************************************
  Ccl_CbkRxInvFuncPtr
**********************************************************************************************************************/
/**
  \var    Ccl_CbkRxInvFuncPtr
  \brief  Function pointer table containing configured Rx invalid notifications for signals and signal groups.
*/

#if (CCL_RX_INVINDICATION_API == STD_ON)
extern CONST(CclCbkInvType, CCL_CONST) Can_Ccl_CbkRxInvFuncPtr[Can_Ccl_Rx_Num_RxInv_Ind];
#endif

/**********************************************************************************************************************
  Ccl_NumOfContainedRxSignals
**********************************************************************************************************************/
/**
  \var    Ccl_NumOfContainedRxSignals
  \brief  Structure describing the starting signal handle and the number of signals contaiend in Rx messages
*/

extern CONST(Ccl_MessageContentInfo, CCL_CONST) Can_Ccl_NumOfContainedRxSignals[Can_Ccl_Rx_Num_Messages];


/**********************************************************************************************************************
  Ccl_NumOfContainedTxSignals
**********************************************************************************************************************/
/**
  \var    Ccl_NumOfContainedTxSignals
  \brief  Structure describing the starting signal handle and the number of signals contaiend in Tx messages
*/

extern CONST(Ccl_MessageContentInfo, CCL_CONST) Can_Ccl_NumOfContainedTxSignals[Can_Ccl_Tx_Num_Messages];


/**********************************************************************************************************************
  Ccl_NumOfContainedTxSignals
**********************************************************************************************************************/
extern CONST(Ccl_RxNotifFuncInfo, CCL_CONST) Can_Ccl_RxNotifFuncPtr[Can_Ccl_Rx_Num_Signals];


/**********************************************************************************************************************
  Ccl_PduGrpVector
**********************************************************************************************************************/
/**
  \var    Ccl_PduGrpVector
  \brief  Contains an I-PDU-Group vector for each I-PDU  mapping the I-PDU to the corresponding I-PDU-Groups.
  */
extern CONST(Ccl_PduGrpVectorType, CCL_CONST) Can_Ccl_PduGrpVector[Can_Ccl_Num_Of_PduGroups];


/**********************************************************************************************************************
  Ccl_TxPduGrpInfo
**********************************************************************************************************************/
/**
  \var    Ccl_TxPduGrpInfo
  \brief  Contains all I-PDU-Group relevant information for Tx I-PDUs.
  \details
  Element                 Description
  PduGrpVectorStartIdx    the start index of the 0:n relation pointing to Ccl_PduGrpVector
*/
extern CONST(Ccl_TxPduGrpInfoType, CCL_CONST) Can_Ccl_TxPduGrpInfo[Can_Ccl_Tx_Num_Messages];


/**********************************************************************************************************************
  Ccl_RxPduGrpInfo
**********************************************************************************************************************/
/**
  \var    Ccl_RxPduGrpInfo
  \brief  Contains all I-PDU-Group relevant information for Rx I-PDUs.
  \details
  Element                 Description
  PduGrpVectorStartIdx    the start index of the 0:n relation pointing to Ccl_PduGrpVector
*/
extern CONST(Ccl_RxPduGrpInfoType, CCL_CONST) Can_Ccl_RxPduGrpInfo[Can_Ccl_Rx_Num_Messages];


/**********************************************************************************************************************
  Ccl_DefRxPduInfo
**********************************************************************************************************************/
/**
  \var    Ccl_DefRxPduInfo
  \brief  Contains all relevant information for deferred Rx I-PDUs.
  \details
  Element                   Description
  RxDefPduBufferUsed        TRUE  if the 0:n relation has 1 relation pointing to ilRxBuffer
*/
extern CONST(Ccl_DefRxPduInfoType, CCL_CONST) Can_Ccl_DefRxPduInfo[Can_Ccl_Rx_Num_Messages];


#endif /* CCL_APP_CFG_H */


/*****************************************************************************
    R E V I S I O N     N O T E S
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  -
For each change to this file  be sure to record:
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
