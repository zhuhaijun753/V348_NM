/* ===========================================================================
**
**                     CONFIDENTIAL VISTEON CORPORATION
**
**  This is an unpublished work of authorship, which contains trade secrets,
**  created in 2007.  Visteon Corporation owns all rights to this work and
**  intends to maintain it in confidence to preserve its trade secret status.
**  Visteon Corporation reserves the right, under the copyright laws of the
**  United States or those of any other country that may have jurisdiction, to
**  protect this work as an unpublished work, in the event of an inadvertent
**  or deliberate unauthorized publication.  Visteon Corporation also reserves
**  its rights under all copyright laws to protect this work as a published
**  work, when appropriate.  Those having access to this work may not copy it,
**  use it, modify it or disclose the information contained in it without the
**  written authorization of Visteon Corporation.
**
** =========================================================================*/

/* ===========================================================================
**
**  Name:           CanCcl_Par_Cfg.c
**
**  Description:    CAN CCL configuration parameter Configuration file for
**                  configured database file
**
**  Organization:   Vehicle Communications
**                  Visteon Corporation
**
** =========================================================================*/

/* ===========================================================================
**  I N C L U D E   F I L E S
** =========================================================================*/

# include "Rte_Cbk.h"
# include "CanCcl_Par_Cfg.h"

/* ===========================================================================
** G L O B A L   C O N S T A N T   D E F I N I T I O N S
** =========================================================================*/

/*******************************************************/
/* DBC Tx & Rx CFG */
/*******************************************************/

/**********************************************************************************************************************
        Can_Ccl_TxSigInfo 
	      **********************************************************************************************************************/
CONST(Ccl_TxSigInfoType, CCL_CONST) Can_Ccl_TxSigInfo[Can_Ccl_Tx_Signal_Num_Signals] = {
	/* Index		CntrId	  InitValue 	SendType   */
	 { /*	  0 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_BSDSoundSts		*/
	 { /*	  1 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Fuel_level		*/
	 { /*	  2 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_DriveTimehour		*/
	 { /*	  3 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_DriveTimeMinute		*/
	 { /*	  4 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_AverageSpeed		*/
	 { /*	  5 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_ChangOilDistance		*/
	 { /*	  6 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_OdometerMasterValue		*/
	 { /*	  7 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_AirbagTelltaleBehavior		*/
	 { /*	  8 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_DISFail		*/
	 { /*	  9 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_QDashACCFail		*/
	 { /*	  10 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_VehSpd_HMI		*/
	 { /*	  11 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Rolling_counter_0x510		*/
	 { /*	  12 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Checksum_0x510		*/
	 { /*	  13 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_AFC		*/
	 { /*	  14 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_DTE		*/
	 { /*	  15 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_AlarmStatus		*/
	 { /*	  16 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_AlarmOrNot		*/
	 { /*	  17 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_ManRgen_D_Rq		*/
	 { /*	  18 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_AFC_Type		*/
	 { /*	  19 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Fuel_Level_Volumn		*/
	 { /*	  20 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Fuel_Level_VolumnQF		*/
	 { /*	  21 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Rolling_counter_0x525		*/
	 { /*	  22 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Checksum_0x525		*/
	 { /*	  23 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Year		*/
	 { /*	  24 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Month		*/
	 { /*	  25 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Date		*/
	 { /*	  26 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Hour		*/
	 { /*	  27 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Minute		*/
	 { /*	  28 */		0 , 	 0x0,	  SIG_NOSENDTYPE	  },		  /* IC_Second		*/
};



/**********************************************************************************************************************
        Can_Ccl_NumOfContainedTxSignals
        **********************************************************************************************************************/
CONST(Ccl_MessageContentInfo, CCL_CONST) Can_Ccl_NumOfContainedTxSignals[Can_Ccl_Tx_Num_Messages] =
{
/* Index      SigStartIndex   SigCount   */
{ /*  0  */          0 ,          1   },/*IC_0x386*/
{ /*  1  */          0 ,          5   },/*IC_0x392*/
{ /*  2  */          0 ,          7   },/*IC_0x510*/
{ /*  3  */          0 ,          10   },/*IC_0x525*/
{ /*  4  */          0 ,          6   },/*IC_time_0x581*/
};

/**********************************************************************************************************************
    Can_Ccl_RxAccessInfo
  **********************************************************************************************************************/
CONST(Ccl_RxAccessInfoType, CCL_CONST) Can_Ccl_RxAccessInfo[Can_Ccl_Rx_Num_Signals] = {
	   /* Index 	 ControllerId */
{ /* 	0  */		   0	   },		/*EMS_BrkPedalStasus                      	*/
{ /* 	1  */		   0	   },		/*EMS_EngCoolanTemp                       	*/
{ /* 	2  */		   0	   },		/*EMS_EngOperationStatus                  	*/
{ /* 	3  */		   0	   },		/*EMS_EngSpeedError                       	*/
{ /* 	4  */		   0	   },		/*EMS_BatteryVoltageError                 	*/
{ /* 	5  */		   0	   },		/*EMS_EngSpeed                            	*/
{ /* 	6  */		   0	   },		/*EMS_CluthPedalStatus                    	*/
{ /* 	7  */		   0	   },		/*TCU_DisplayGear                         	*/
{ /* 	8  */		   0	   },		/*TCU_GearSelectorReq                     	*/
{ /* 	9  */		   0	   },		/*TCU_StGearMode                          	*/
{ /* 	10  */		   0	   },		/*TCU_Status_AliveCounter                 	*/
{ /* 	11  */		   0	   },		/*TCU_Status_Checksum                     	*/
{ /* 	12  */		   0	   },		/*EMS_TargCruiseSpeed                     	*/
{ /* 	13  */		   0	   },		/*EMS_Rolling_counter_0x151               	*/
{ /* 	14  */		   0	   },		/*EMS_CruiseControlStatus                 	*/
{ /* 	15  */		   0	   },		/*EMS_Checksum_0x151                      	*/
{ /* 	16  */		   0	   },		/*PEPS_Remotestartstatus                  	*/
{ /* 	17  */		   0	   },		/*PEPS_RollingCounter_0x1A5               	*/
{ /* 	18  */		   0	   },		/*PEPS_Checksum_0x1A5                     	*/
{ /* 	19  */		   0	   },		/*ABS_WhlMilgFrntLe                       	*/
{ /* 	20  */		   0	   },		/*ABS_WhlMilgFrntRi                       	*/
{ /* 	21  */		   0	   },		/*ABS_VehSpdLgt                           	*/
{ /* 	22  */		   0	   },		/*ABS_EbdFlgFlt                           	*/
{ /* 	23  */		   0	   },		/*ABS_AbsFlgFlt                           	*/
{ /* 	24  */		   0	   },		/*ABS_RollingCounter_0x221                	*/
{ /* 	25  */		   0	   },		/*ABS_WhlMilgFrntLeStatus                 	*/
{ /* 	26  */		   0	   },		/*ABS_WhlMilgFrntRiStatus                 	*/
{ /* 	27  */		   0	   },		/*ABS_Checksum_0x221                      	*/
{ /* 	28  */		   0	   },		/*ESP_EPBStatus                           	*/
{ /* 	29  */		   0	   },		/*ESP_EPBErrorStatus                      	*/
{ /* 	30  */		   0	   },		/*ESP_CdpAvailable                        	*/
{ /* 	31  */		   0	   },		/*ESP_EPBWarningMessage3                  	*/
{ /* 	32  */		   0	   },		/*ESP_EPBWarningMessage2                  	*/
{ /* 	33  */		   0	   },		/*ESP_EPBWarningMessage1                  	*/
{ /* 	34  */		   0	   },		/*ESP_AVHWarningMessage                   	*/
{ /* 	35  */		   0	   },		/*ESP_AvhActive                           	*/
{ /* 	36  */		   0	   },		/*ESP_AvhAvabliable                       	*/
{ /* 	37  */		   0	   },		/*ESP_VdcFlgFlt                           	*/
{ /* 	38  */		   0	   },		/*ESP_TcsFlgFlt                           	*/
{ /* 	39  */		   0	   },		/*ESP_ESPInfoLamp                         	*/
{ /* 	40  */		   0	   },		/*ESP_EspEnableSts                        	*/
{ /* 	41  */		   0	   },		/*ESP_RollingCounter_0x243                	*/
{ /* 	42  */		   0	   },		/*ESP_Checksum_0x243                      	*/
{ /* 	43  */		   0	   },		/*IPM_LaneAssitType                       	*/
{ /* 	44  */		   0	   },		/*IPM_LaneAssit_WarningVibration          	*/
{ /* 	45  */		   0	   },		/*IPM_LaneAssit_WarningAudio              	*/
{ /* 	46  */		   0	   },		/*IPM_LaneAssit_LeftVisualization         	*/
{ /* 	47  */		   0	   },		/*IPM_LaneAssit_RightVisualization        	*/
{ /* 	48  */		   0	   },		/*IPM_SLASpdlimitUnits                    	*/
{ /* 	49  */		   0	   },		/*IPM_SLAState                            	*/
{ /* 	50  */		   0	   },		/*IPM_LaneAssit_Status                    	*/
{ /* 	51  */		   0	   },		/*IPM_SLASpdlimit                         	*/
{ /* 	52  */		   0	   },		/*IPM_HMA_Status                          	*/
{ /* 	53  */		   0	   },		/*IPM_DDD_IndexStatus                     	*/
{ /* 	54  */		   0	   },		/*IPM_DDD_WarningLevel                    	*/
{ /* 	55  */		   0	   },		/*Rolling_counter_0x245                   	*/
{ /* 	56  */		   0	   },		/*IPM_LaneAssit_HandsonReq                	*/
{ /* 	57  */		   0	   },		/*Checksum_0x245                          	*/
{ /* 	58  */		   0	   },		/*vSetDis                                 	*/
{ /* 	59  */		   0	   },		/*MRR_ObjValid                            	*/
{ /* 	60  */		   0	   },		/*MRR_TauGapSet                           	*/
{ /* 	61  */		   0	   },		/*MRR_dxTarObj                            	*/
{ /* 	62  */		   0	   },		/*ACCHMI_Mode                             	*/
{ /* 	63  */		   0	   },		/*ACCFailInfo                             	*/
{ /* 	64  */		   0	   },		/*MRR_TakeOverReq                         	*/
{ /* 	65  */		   0	   },		/*MRR_AEB_STATE                           	*/
{ /* 	66  */		   0	   },		/*ACC_Startstop_infor                     	*/
{ /* 	67  */		   0	   },		/*MRR_PCW_preWarning                      	*/
{ /* 	68  */		   0	   },		/*MRR_PCW_latentWarning                   	*/
{ /* 	69  */		   0	   },		/*FCW_preWarning                          	*/
{ /* 	70  */		   0	   },		/*FCW_latentWarning                       	*/
{ /* 	71  */		   0	   },		/*MRR_PCW_STATE                           	*/
{ /* 	72  */		   0	   },		/*Obj_Speed                               	*/
{ /* 	73  */		   0	   },		/*Rolling_counter_0x246                   	*/
{ /* 	74  */		   0	   },		/*Textinfo                                	*/
{ /* 	75  */		   0	   },		/*Checksum_0x246                          	*/
{ /* 	76  */		   0	   },		/*EPS_S_WarningLampYellow                 	*/
{ /* 	77  */		   0	   },		/*EPS_S_SafeLampRed                       	*/
{ /* 	78  */		   0	   },		/*Rolling_counter_0x260                   	*/
{ /* 	79  */		   0	   },		/*Checksum_0x260                          	*/
{ /* 	80  */		   0	   },		/*PAM_ChimeCmdSoundStatus                 	*/
{ /* 	81  */		   0	   },		/*PAM_RearSysTemWorkSts                   	*/
{ /* 	82  */		   0	   },		/*PAM_FrontSysTemWorkSts                  	*/
{ /* 	83  */		   0	   },		/*PAM_ChimeCmdSoundCadence                	*/
{ /* 	84  */		   0	   },		/*SOD_LCA_warningReqRight                 	*/
{ /* 	85  */		   0	   },		/*SOD_DOA_warningReqRightRear             	*/
{ /* 	86  */		   0	   },		/*SOD_DOA_warningReqRightFront            	*/
{ /* 	87  */		   0	   },		/*SOD_CTA_warningReqRight                 	*/
{ /* 	88  */		   0	   },		/*SOD_BLIS_display                        	*/
{ /* 	89  */		   0	   },		/*SOD_LCA_warningReqLeft                  	*/
{ /* 	90  */		   0	   },		/*SOD_DOA_warningReqLeftRear              	*/
{ /* 	91  */		   0	   },		/*SOD_DOA_warningReqLeftFront             	*/
{ /* 	92  */		   0	   },		/*SOD_CTA_warningReqLeft                  	*/
{ /* 	93  */		   0	   },		/*DOA_OFFTelltale                         	*/
{ /* 	94  */		   0	   },		/*CTA_OFFTelltale                         	*/
{ /* 	95  */		   0	   },		/*SODLCA_OFFTelltale                      	*/
{ /* 	96  */		   0	   },		/*Rolling_counter_0x275                   	*/
{ /* 	97  */		   0	   },		/*Checksum_0x275                          	*/
{ /* 	98  */		   0	   },		/*BCM_DoorStsRearRi                       	*/
{ /* 	99  */		   0	   },		/*BCM_DoorStsRearLe                       	*/
{ /* 	100  */		   0	   },		/*BCM_DoorStsFrntRi                       	*/
{ /* 	101  */		   0	   },		/*BCM_DoorStsFrntLe                       	*/
{ /* 	102  */		   0	   },		/*BCM_DoorLockStsRearRi                   	*/
{ /* 	103  */		   0	   },		/*BCM_DoorLockStsRearLe                   	*/
{ /* 	104  */		   0	   },		/*BCM_DoorLockStsFrntRi                   	*/
{ /* 	105  */		   0	   },		/*BCM_DoorLockStsFrntLe                   	*/
{ /* 	106  */		   0	   },		/*BCM_AntiTheftSts                        	*/
{ /* 	107  */		   0	   },		/*BCM_LockUnlockFeedback                  	*/
{ /* 	108  */		   0	   },		/*BCM_DoorLockStsTrunk                    	*/
{ /* 	109  */		   0	   },		/*BCM_DoorStsTrunk                        	*/
{ /* 	110  */		   0	   },		/*BCM_TrunkDoorPreUnlockSts               	*/
{ /* 	111  */		   0	   },		/*BCM_EnginehoodSts                       	*/
{ /* 	112  */		   0	   },		/*BCM_Rolling_counter_0x310               	*/
{ /* 	113  */		   0	   },		/*BCM_Checksum_0x310                      	*/
{ /* 	114  */		   0	   },		/*BCM_OutsideTempDig                      	*/
{ /* 	115  */		   0	   },		/*BCM_Mechanical_Hand_Brake               	*/
{ /* 	116  */		   0	   },		/*BCM_BrakeFluidSts                       	*/
{ /* 	117  */		   0	   },		/*BCM_OutsideTemp                         	*/
{ /* 	118  */		   0	   },		/*BCM_PowerMode                           	*/
{ /* 	119  */		   0	   },		/*BCM_Rollingcounter_0x313                	*/
{ /* 	120  */		   0	   },		/*BCM_Checksum_0x313                      	*/
{ /* 	121  */		   0	   },		/*BCM_BGLLumLvlSettingStatus              	*/
{ /* 	122  */		   0	   },		/*BCM_FrntFogLmpSts                       	*/
{ /* 	123  */		   0	   },		/*BCM_PosLmpSts                           	*/
{ /* 	124  */		   0	   },		/*BCM_LowBeamSts                          	*/
{ /* 	125  */		   0	   },		/*BCM_HighBeamSts                         	*/
{ /* 	126  */		   0	   },		/*BCM_reversinglightSts                   	*/
{ /* 	127  */		   0	   },		/*BCM_DayRunningLampSts                   	*/
{ /* 	128  */		   0	   },		/*BCM_TurnIndicatorSts                    	*/
{ /* 	129  */		   0	   },		/*BCM_RearFogLmpSts                       	*/
{ /* 	130  */		   0	   },		/*BCM_Highmountedstoplamp                 	*/
{ /* 	131  */		   0	   },		/*BCM_BrakelightSts                       	*/
{ /* 	132  */		   0	   },		/*BCM_BackHomeSts                         	*/
{ /* 	133  */		   0	   },		/*BCM_AutolampSts                         	*/
{ /* 	134  */		   0	   },		/*BCM_readinglampSts                      	*/
{ /* 	135  */		   0	   },		/*BCM_DayRunningLampDig                   	*/
{ /* 	136  */		   0	   },		/*BCM_BrakelightDig                       	*/
{ /* 	137  */		   0	   },		/*BCM_reversinglightDig                   	*/
{ /* 	138  */		   0	   },		/*BCM_TurnIndicatorDig                    	*/
{ /* 	139  */		   0	   },		/*BCM_RearFogLmpDig                       	*/
{ /* 	140  */		   0	   },		/*BCM_PosLmpDig                           	*/
{ /* 	141  */		   0	   },		/*BCM_Rollingcounter_0x318                	*/
{ /* 	142  */		   0	   },		/*BCM_ImmoSts                             	*/
{ /* 	143  */		   0	   },		/*BCM_Checksum_0x318                      	*/
{ /* 	144  */		   0	   },		/*SWM_Menu_Right                          	*/
{ /* 	145  */		   0	   },		/*SWM_Menu_Left                           	*/
{ /* 	146  */		   0	   },		/*SWM_Menu_Down                           	*/
{ /* 	147  */		   0	   },		/*SWM_Menu_Up                             	*/
{ /* 	148  */		   0	   },		/*Rolling_counter_0x31A                   	*/
{ /* 	149  */		   0	   },		/*SWM_Menu_Confirm                        	*/
{ /* 	150  */		   0	   },		/*Checksum_0x31A                          	*/
{ /* 	151  */		   0	   },		/*PEPS_PowerMode                          	*/
{ /* 	152  */		   0	   },		/*PEPS_IMMO_MatchStatus                   	*/
{ /* 	153  */		   0	   },		/*PEPS_SystemFailStatus                   	*/
{ /* 	154  */		   0	   },		/*PEPS_ESCL_FailStatus                    	*/
{ /* 	155  */		   0	   },		/*PEPS_SSB_FailStatus                     	*/
{ /* 	156  */		   0	   },		/*PEPS_ClutchIndicationWarning            	*/
{ /* 	157  */		   0	   },		/*PEPS_BrakeIndicationWarning             	*/
{ /* 	158  */		   0	   },		/*PEPS_NotPNWarning                       	*/
{ /* 	159  */		   0	   },		/*PEPS_NotNWarning                        	*/
{ /* 	160  */		   0	   },		/*PEPS_DoorOpenNotOFFWarning              	*/
{ /* 	161  */		   0	   },		/*PEPS_DoorOpenWarning                    	*/
{ /* 	162  */		   0	   },		/*PEPS_KeyBatteryWarning                  	*/
{ /* 	163  */		   0	   },		/*PEPS_KeyOutWarning1                     	*/
{ /* 	164  */		   0	   },		/*PEPS_NoKeyFoundWarning                  	*/
{ /* 	165  */		   0	   },		/*PEPS_KeyReminderWarning                 	*/
{ /* 	166  */		   0	   },		/*PEPS_RollingCounter_0x325               	*/
{ /* 	167  */		   0	   },		/*PEPS_RemainingNumSts                    	*/
{ /* 	168  */		   0	   },		/*PEPS_Checksum_0x325                     	*/
{ /* 	169  */		   0	   },		/*RF_KeyLowBatteryWarning                 	*/
{ /* 	170  */		   0	   },		/*SRS_SBR_PAMsenger                       	*/
{ /* 	171  */		   0	   },		/*SRS_SBR_Driver                          	*/
{ /* 	172  */		   0	   },		/*SRS_AirBagFailSts                       	*/
{ /* 	173  */		   0	   },		/*SRS_SBR_SecondRowRight                  	*/
{ /* 	174  */		   0	   },		/*SRS_SBR_SecondRowMid                    	*/
{ /* 	175  */		   0	   },		/*SRS_SBR_SecondRowLeft                   	*/
{ /* 	176  */		   0	   },		/*Rolling_counter_0x350                   	*/
{ /* 	177  */		   0	   },		/*Checksum_0x350                          	*/
{ /* 	178  */		   0	   },		/*TPMS_LF_Pressure                        	*/
{ /* 	179  */		   0	   },		/*TPMS_RF_Pressure                        	*/
{ /* 	180  */		   0	   },		/*TPMS_RR_Pressure                        	*/
{ /* 	181  */		   0	   },		/*TPMS_LR_Pressure                        	*/
{ /* 	182  */		   0	   },		/*TPMS_Tire_Temperature                   	*/
{ /* 	183  */		   0	   },		/*TPMS_LF_Pressure_Warning                	*/
{ /* 	184  */		   0	   },		/*TPMS_RF_Pressure_Warning                	*/
{ /* 	185  */		   0	   },		/*TPMS_System_Status                      	*/
{ /* 	186  */		   0	   },		/*TPMS_RR_Pressure_Warning                	*/
{ /* 	187  */		   0	   },		/*TPMS_LR_Pressure_Warning                	*/
{ /* 	188  */		   0	   },		/*Temperature_Warning                     	*/
{ /* 	189  */		   0	   },		/*resrved                                 	*/
{ /* 	190  */		   0	   },		/*Tire_Position                           	*/
{ /* 	191  */		   0	   },		/*TPMS_Lamp_Status                        	*/
{ /* 	192  */		   0	   },		/*RRI_Pressure                            	*/
{ /* 	193  */		   0	   },		/*LRI_Pressure                            	*/
{ /* 	194  */		   0	   },		/*RRI_Pressure_Warning                    	*/
{ /* 	195  */		   0	   },		/*LRI_Pressure_Warning                    	*/
{ /* 	196  */		   0	   },		/*EMS_Fuelcounter                         	*/
{ /* 	197  */		   0	   },		/*EMS_RemindShiftGear_MT                  	*/
{ /* 	198  */		   0	   },		/*EMS_MIL                                 	*/
{ /* 	199  */		   0	   },		/*EMS_SVS                                 	*/
{ /* 	200  */		   0	   },		/*EMS_StartStopStatus                     	*/
{ /* 	201  */		   0	   },		/*EMS_OBDTxt                              	*/
{ /* 	202  */		   0	   },		/*EMS_TroqLimtHour                        	*/
{ /* 	203  */		   0	   },		/*EMS_UreaQty                             	*/
{ /* 	204  */		   0	   },		/*EMS_TroqLimtMin                         	*/
{ /* 	205  */		   0	   },		/*EMS_UreaLvl                             	*/
{ /* 	206  */		   0	   },		/*EMS_DieslPrtcWarn_D_Rq                  	*/
{ /* 	207  */		   0	   },		/*DCM_GlowPlugLmpReq                      	*/
{ /* 	208  */		   0	   },		/*DCM_WIFFullLmpReq                       	*/
{ /* 	209  */		   0	   },		/*EMS_SpeedLimtHour                       	*/
{ /* 	210  */		   0	   },		/*EMS_SpeedLimtMin                        	*/
{ /* 	211  */		   0	   },		/*EMS_UreaSysWarning                      	*/
{ /* 	212  */		   0	   },		/*EMS_UreaTxt                             	*/
{ /* 	213  */		   0	   },		/*EMS_ManRgenTxt_D_RqDsply                	*/
{ /* 	214  */		   0	   },		/*EMS_UreaWarn                            	*/
{ /* 	215  */		   0	   },		/*EMS_Soot_Pc_Dsply                       	*/
{ /* 	216  */		   0	   },		/*MP5_BSDSoundSwitchSts                   	*/
{ /* 	217  */		   0	   },		/*MP5_NavICON                             	*/
{ /* 	218  */		   0	   },		/*MP5_ROUTE_REMAIN_DIS                    	*/
{ /* 	219  */		   0	   },		/*MP5_ALModeSetting                       	*/
{ /* 	220  */		   0	   },		/*MP5_ALStartAroundSetting                	*/
{ /* 	221  */		   0	   },		/*Trans_Limphome                          	*/
{ /* 	222  */		   0	   },		/*TrnSrvcRqd_B_Rq                         	*/
{ /* 	223  */		   0	   },		/*TCU_Gearbox_Gonglimp                    	*/
{ /* 	224  */		   0	   },		/*TCU_Driverwarning                       	*/
{ /* 	225  */		   0	   },		/*TCU_DrvShiftAllwCondRmd                 	*/
{ /* 	226  */		   0	   },		/*MP5_AlarmStatus                         	*/
{ /* 	227  */		   0	   },		/*MP5_AlarmOrNot                          	*/
{ /* 	228  */		   0	   },		/*TBOX_Year                               	*/
{ /* 	229  */		   0	   },		/*TBOX_Month                              	*/
{ /* 	230  */		   0	   },		/*TBOX_Date                               	*/
{ /* 	231  */		   0	   },		/*TBOX_Hour                               	*/
{ /* 	232  */		   0	   },		/*TBOX_Minute                             	*/
{ /* 	233  */		   0	   },		/*TBOX_Second                             	*/
{ /* 	234  */		   0	   },		/*TBOX_RealTime_UB_Copy_1                 	*/
{ /* 	235  */		   0	   },		/*TBOX_RealTime_UB                        	*/
{ /* 	236  */		   0	   },		/*BCM_LoadManageLevel                     	*/
{ /* 	237  */		   0	   },		/*BCM_CarModeIndicator                    	*/
{ /* 	238  */		   0	   },		/*BCM_CarMode                             	*/
{ /* 	239  */		   0	   },		/*EMS_BackupOdometer                      	*/
};



/**********************************************************************************************************************
    Can_Ccl_NumOfContainedRxSignals
  **********************************************************************************************************************/
  CONST(Ccl_MessageContentInfo, CCL_CONST) Can_Ccl_NumOfContainedRxSignals[Can_Ccl_Rx_Num_Messages] =
  {
     /* Index      SigStartIndex   SigCount   */
     { /*  0  */            0,       7        },
     { /*  1  */            7,       5        },
     { /*  2  */            12,       4        },
     { /*  3  */            16,       3        },
     { /*  4  */            19,       9        },
     { /*  5  */            28,       15        },
     { /*  6  */            43,       15        },
     { /*  7  */            58,       18        },
     { /*  8  */            76,       4        },
     { /*  9  */            80,       4        },
     { /*  10  */            84,       14        },
     { /*  11  */            98,       16        },
     { /*  12  */            114,       7        },
     { /*  13  */            121,       1        },
     { /*  14  */            122,       22        },
     { /*  15  */            144,       7        },
     { /*  16  */            151,       18        },
     { /*  17  */            169,       1        },
     { /*  18  */            170,       8        },
     { /*  19  */            178,       14        },
     { /*  20  */            192,       4        },
     { /*  21  */            196,       6        },
     { /*  22  */            202,       14        },
     { /*  23  */            216,       1        },
     { /*  24  */            217,       4        },
     { /*  25  */            221,       5        },
     { /*  26  */            226,       2        },
     { /*  27  */            228,       8        },
     { /*  28  */            236,       3        },
     { /*  29  */            239,       1        },
};



  #if (CCL_RX_INDICATION_API == STD_ON)
  /**********************************************************************************************************************
    Ccl_CbkRxAckFuncPtr
  **********************************************************************************************************************/
  /**
    \var    Ccl_CbkRxAckFuncPtr
    \brief  Function pointer table containing configured Rx Indication notifications for signals and signal groups.*/

  CONST(CclCbkRxAckType, CCL_CONST) Can_Ccl_CbkRxAckFuncPtr[Can_Ccl_Rx_Num_RxAck_Ind] = {
    /* Index    CbkRxAckFuncPtr                              */
	/*	0	*/	NULL	,/*EMS_BrkPedalStasus*/
	/*	1	*/	Rte_COMCbk_EMS_EngCoolanTemp_85187d93	,/*EMS_EngCoolanTemp*/
	/*	2	*/	NULL	,/*EMS_EngOperationStatus*/
	/*	3	*/	NULL	,/*EMS_EngSpeedError*/
	/*	4	*/	Rte_COMCbk_EMS_BatteryVoltageError_85187d93	,/*EMS_BatteryVoltageError*/
	/*	5	*/	Rte_COMCbk_EMS_EngSpeed_85187d93	,/*EMS_EngSpeed*/
	/*	6	*/	Rte_COMCbk_EMS_CluthPedalStatus_85187d93	,/*EMS_CluthPedalStatus*/
	/*	7	*/	Rte_COMCbk_TCU_DisplayGear_cb5c6a72	,/*TCU_DisplayGear*/
	/*	8	*/	Rte_COMCbk_TCU_GearSelectorReq_cb5c6a72	,/*TCU_GearSelectorReq*/
	/*	9	*/	NULL	,/*TCU_StGearMode*/
	/*	10	*/	NULL	,/*TCU_Status_AliveCounter*/
	/*	11	*/	NULL	,/*TCU_Status_Checksum*/
	/*	12	*/	Rte_COMCbk_EMS_TargCruiseSpeed_341d7c91	,/*EMS_TargCruiseSpeed*/
	/*	13	*/	NULL	,/*EMS_Rolling_counter_0x151*/
	/*	14	*/	Rte_COMCbk_EMS_CruiseControlStatus_341d7c91	,/*EMS_CruiseControlStatus*/
	/*	15	*/	NULL	,/*EMS_Checksum_0x151*/
	/*	16	*/	NULL	,/*PEPS_Remotestartstatus*/
	/*	17	*/	NULL	,/*PEPS_RollingCounter_0x1A5*/
	/*	18	*/	NULL	,/*PEPS_Checksum_0x1A5*/
	/*	19	*/	Rte_COMCbk_ABS_WhlMilgFrntLe_2356a03c	,/*ABS_WhlMilgFrntLe*/
	/*	20	*/	Rte_COMCbk_ABS_WhlMilgFrntRi_2356a03c	,/*ABS_WhlMilgFrntRi*/
	/*	21	*/	Rte_COMCbk_ABS_VehSpdLgt_2356a03c	,/*ABS_VehSpdLgt*/
	/*	22	*/	Rte_COMCbk_ABS_EbdFlgFlt_2356a03c	,/*ABS_EbdFlgFlt*/
	/*	23	*/	Rte_COMCbk_ABS_AbsFlgFlt_2356a03c	,/*ABS_AbsFlgFlt*/
	/*	24	*/	NULL	,/*ABS_RollingCounter_0x221*/
	/*	25	*/	Rte_COMCbk_ABS_WhlMilgFrntLeStatus_2356a03c	,/*ABS_WhlMilgFrntLeStatus*/
	/*	26	*/	Rte_COMCbk_ABS_WhlMilgFrntRiStatus_2356a03c	,/*ABS_WhlMilgFrntRiStatus*/
	/*	27	*/	NULL	,/*ABS_Checksum_0x221*/
	/*	28	*/	Rte_COMCbk_ESP_EPBStatus_af66671b	,/*ESP_EPBStatus*/
	/*	29	*/	Rte_COMCbk_ESP_EPBErrorStatus_af66671b	,/*ESP_EPBErrorStatus*/
	/*	30	*/	Rte_COMCbk_ESP_CdpAvailable_af66671b	,/*ESP_CdpAvailable*/
	/*	31	*/	Rte_COMCbk_ESP_EPBWarningMessage3_af66671b	,/*ESP_EPBWarningMessage3*/
	/*	32	*/	Rte_COMCbk_ESP_EPBWarningMessage2_af66671b	,/*ESP_EPBWarningMessage2*/
	/*	33	*/	Rte_COMCbk_ESP_EPBWarningMessage1_af66671b	,/*ESP_EPBWarningMessage1*/
	/*	34	*/	Rte_COMCbk_ESP_AVHWarningMessage_af66671b	,/*ESP_AVHWarningMessage*/
	/*	35	*/	Rte_COMCbk_ESP_AvhActive_af66671b	,/*ESP_AvhActive*/
	/*	36	*/	Rte_COMCbk_ESP_AvhAvabliable_af66671b	,/*ESP_AvhAvabliable*/
	/*	37	*/	Rte_COMCbk_ESP_VdcFlgFlt_af66671b	,/*ESP_VdcFlgFlt*/
	/*	38	*/	Rte_COMCbk_ESP_TcsFlgFlt_af66671b	,/*ESP_TcsFlgFlt*/
	/*	39	*/	Rte_COMCbk_ESP_ESPInfoLamp_af66671b	,/*ESP_ESPInfoLamp*/
	/*	40	*/	Rte_COMCbk_ESP_EspEnableSts_af66671b	,/*ESP_EspEnableSts*/
	/*	41	*/	NULL	,/*ESP_RollingCounter_0x243*/
	/*	42	*/	NULL	,/*ESP_Checksum_0x243*/
	/*	43	*/	Rte_COMCbk_IPM_LaneAssitType_32f79b14	,/*IPM_LaneAssitType*/
	/*	44	*/	NULL	,/*IPM_LaneAssit_WarningVibration*/
	/*	45	*/	Rte_COMCbk_IPM_LaneAssit_WarningAudio_32f79b14	,/*IPM_LaneAssit_WarningAudio*/
	/*	46	*/	Rte_COMCbk_IPM_LaneAssit_LeftVisualization_32f79b14	,/*IPM_LaneAssit_LeftVisualization*/
	/*	47	*/	Rte_COMCbk_IPM_LaneAssit_RightVisualization_32f79b14	,/*IPM_LaneAssit_RightVisualization*/
	/*	48	*/	Rte_COMCbk_IPM_SLASpdlimitUnits_32f79b14	,/*IPM_SLASpdlimitUnits*/
	/*	49	*/	Rte_COMCbk_IPM_SLAState_32f79b14	,/*IPM_SLAState*/
	/*	50	*/	Rte_COMCbk_IPM_LaneAssit_Status_32f79b14	,/*IPM_LaneAssit_Status*/
	/*	51	*/	Rte_COMCbk_IPM_SLASpdlimit_32f79b14	,/*IPM_SLASpdlimit*/
	/*	52	*/	Rte_COMCbk_IPM_HMA_Status_32f79b14	,/*IPM_HMA_Status*/
	/*	53	*/	Rte_COMCbk_IPM_DDD_IndexStatus_32f79b14	,/*IPM_DDD_IndexStatus*/
	/*	54	*/	Rte_COMCbk_IPM_DDD_WarningLevel_32f79b14	,/*IPM_DDD_WarningLevel*/
	/*	55	*/	NULL	,/*Rolling_counter_0x245*/
	/*	56	*/	Rte_COMCbk_IPM_LaneAssit_HandsonReq_32f79b14	,/*IPM_LaneAssit_HandsonReq*/
	/*	57	*/	NULL	,/*Checksum_0x245*/
	/*	58	*/	Rte_COMCbk_vSetDis_255e8a04	,/*vSetDis*/
	/*	59	*/	Rte_COMCbk_MRR_ObjValid_255e8a04	,/*MRR_ObjValid*/
	/*	60	*/	Rte_COMCbk_MRR_TauGapSet_255e8a04	,/*MRR_TauGapSet*/
	/*	61	*/	Rte_COMCbk_MRR_dxTarObj_255e8a04	,/*MRR_dxTarObj*/
	/*	62	*/	Rte_COMCbk_ACCHMI_Mode_255e8a04	,/*ACCHMI_Mode*/
	/*	63	*/	NULL	,/*ACCFailInfo*/
	/*	64	*/	Rte_COMCbk_MRR_TakeOverReq_255e8a04	,/*MRR_TakeOverReq*/
	/*	65	*/	Rte_COMCbk_MRR_AEB_STATE_255e8a04	,/*MRR_AEB_STATE*/
	/*	66	*/	NULL	,/*ACC_Startstop_infor*/
	/*	67	*/	Rte_COMCbk_MRR_PCW_preWarning_255e8a04	,/*MRR_PCW_preWarning*/
	/*	68	*/	Rte_COMCbk_MRR_PCW_latentWarning_255e8a04	,/*MRR_PCW_latentWarning*/
	/*	69	*/	Rte_COMCbk_FCW_preWarning_255e8a04	,/*FCW_preWarning*/
	/*	70	*/	Rte_COMCbk_FCW_latentWarning_255e8a04	,/*FCW_latentWarning*/
	/*	71	*/	Rte_COMCbk_MRR_PCW_STATE_255e8a04	,/*MRR_PCW_STATE*/
	/*	72	*/	NULL	,/*Obj_Speed*/
	/*	73	*/	NULL	,/*Rolling_counter_0x246*/
	/*	74	*/	Rte_COMCbk_Textinfo_255e8a04	,/*Textinfo*/
	/*	75	*/	NULL	,/*Checksum_0x246*/
	/*	76	*/	Rte_COMCbk_EPS_S_WarningLampYellow_e5742092	,/*EPS_S_WarningLampYellow*/
	/*	77	*/	Rte_COMCbk_EPS_S_SafeLampRed_e5742092	,/*EPS_S_SafeLampRed*/
	/*	78	*/	NULL	,/*Rolling_counter_0x260*/
	/*	79	*/	NULL	,/*Checksum_0x260*/
	/*	80	*/	Rte_COMCbk_PAM_ChimeCmdSoundStatus_49f5ddcc	,/*PAM_ChimeCmdSoundStatus*/
	/*	81	*/	Rte_COMCbk_PAM_RearSysTemWorkSts_49f5ddcc	,/*PAM_RearSysTemWorkSts*/
	/*	82	*/	Rte_COMCbk_PAM_FrontSysTemWorkSts_49f5ddcc	,/*PAM_FrontSysTemWorkSts*/
	/*	83	*/	Rte_COMCbk_PAM_ChimeCmdSoundCadence_49f5ddcc	,/*PAM_ChimeCmdSoundCadence*/
	/*	84	*/	Rte_COMCbk_SOD_LCA_warningReqRight_a75280f1	,/*SOD_LCA_warningReqRight*/
	/*	85	*/	Rte_COMCbk_SOD_DOA_warningReqRightRear_a75280f1	,/*SOD_DOA_warningReqRightRear*/
	/*	86	*/	Rte_COMCbk_SOD_DOA_warningReqRightFront_a75280f1	,/*SOD_DOA_warningReqRightFront*/
	/*	87	*/	Rte_COMCbk_SOD_CTA_warningReqRight_a75280f1	,/*SOD_CTA_warningReqRight*/
	/*	88	*/	Rte_COMCbk_SOD_BLIS_display_a75280f1	,/*SOD_BLIS_display*/
	/*	89	*/	Rte_COMCbk_SOD_LCA_warningReqLeft_a75280f1	,/*SOD_LCA_warningReqLeft*/
	/*	90	*/	Rte_COMCbk_SOD_DOA_warningReqLeftRear_a75280f1	,/*SOD_DOA_warningReqLeftRear*/
	/*	91	*/	Rte_COMCbk_SOD_DOA_warningReqLeftFront_a75280f1	,/*SOD_DOA_warningReqLeftFront*/
	/*	92	*/	Rte_COMCbk_SOD_CTA_warningReqLeft_a75280f1	,/*SOD_CTA_warningReqLeft*/
	/*	93	*/	Rte_COMCbk_DOA_OFFTelltale_a75280f1	,/*DOA_OFFTelltale*/
	/*	94	*/	Rte_COMCbk_CTA_OFFTelltale_a75280f1	,/*CTA_OFFTelltale*/
	/*	95	*/	Rte_COMCbk_SODLCA_OFFTelltale_a75280f1	,/*SODLCA_OFFTelltale*/
	/*	96	*/	NULL	,/*Rolling_counter_0x275*/
	/*	97	*/	NULL	,/*Checksum_0x275*/
	/*	98	*/	Rte_COMCbk_BCM_DoorStsRearRi_0edb943f	,/*BCM_DoorStsRearRi*/
	/*	99	*/	Rte_COMCbk_BCM_DoorStsRearLe_0edb943f	,/*BCM_DoorStsRearLe*/
	/*	100	*/	Rte_COMCbk_BCM_DoorStsFrntRi_0edb943f	,/*BCM_DoorStsFrntRi*/
	/*	101	*/	Rte_COMCbk_BCM_DoorStsFrntLe_0edb943f	,/*BCM_DoorStsFrntLe*/
	/*	102	*/	NULL	,/*BCM_DoorLockStsRearRi*/
	/*	103	*/	NULL	,/*BCM_DoorLockStsRearLe*/
	/*	104	*/	NULL	,/*BCM_DoorLockStsFrntRi*/
	/*	105	*/	NULL	,/*BCM_DoorLockStsFrntLe*/
	/*	106	*/	NULL	,/*BCM_AntiTheftSts*/
	/*	107	*/	NULL	,/*BCM_LockUnlockFeedback*/
	/*	108	*/	NULL	,/*BCM_DoorLockStsTrunk*/
	/*	109	*/	Rte_COMCbk_BCM_DoorStsTrunk_0edb943f	,/*BCM_DoorStsTrunk*/
	/*	110	*/	NULL	,/*BCM_TrunkDoorPreUnlockSts*/
	/*	111	*/	Rte_COMCbk_BCM_EnginehoodSts_0edb943f	,/*BCM_EnginehoodSts*/
	/*	112	*/	NULL	,/*BCM_Rolling_counter_0x310*/
	/*	113	*/	NULL	,/*BCM_Checksum_0x310*/
	/*	114	*/	NULL	,/*BCM_OutsideTempDig*/
	/*	115	*/	Rte_COMCbk_BCM_Mechanical_Hand_Brake_abc8cd44	,/*BCM_Mechanical_Hand_Brake*/
	/*	116	*/	Rte_COMCbk_BCM_BrakeFluidSts_abc8cd44	,/*BCM_BrakeFluidSts*/
	/*	117	*/	Rte_COMCbk_BCM_OutsideTemp_abc8cd44	,/*BCM_OutsideTemp*/
	/*	118	*/	Rte_COMCbk_BCM_PowerMode_abc8cd44	,/*BCM_PowerMode*/
	/*	119	*/	NULL	,/*BCM_Rollingcounter_0x313*/
	/*	120	*/	NULL	,/*BCM_Checksum_0x313*/
	/*	121	*/	Rte_COMCbk_BCM_BGLLumLvlSettingStatus_a1dcf71c	,/*BCM_BGLLumLvlSettingStatus*/
	/*	122	*/	Rte_COMCbk_BCM_FrntFogLmpSts_52444383	,/*BCM_FrntFogLmpSts*/
	/*	123	*/	Rte_COMCbk_BCM_PosLmpSts_52444383	,/*BCM_PosLmpSts*/
	/*	124	*/	Rte_COMCbk_BCM_LowBeamSts_52444383	,/*BCM_LowBeamSts*/
	/*	125	*/	Rte_COMCbk_BCM_HighBeamSts_52444383	,/*BCM_HighBeamSts*/
	/*	126	*/	NULL	,/*BCM_reversinglightSts*/
	/*	127	*/	NULL	,/*BCM_DayRunningLampSts*/
	/*	128	*/	Rte_COMCbk_BCM_TurnIndicatorSts_52444383	,/*BCM_TurnIndicatorSts*/
	/*	129	*/	Rte_COMCbk_BCM_RearFogLmpSts_52444383	,/*BCM_RearFogLmpSts*/
	/*	130	*/	NULL	,/*BCM_Highmountedstoplamp*/
	/*	131	*/	NULL	,/*BCM_BrakelightSts*/
	/*	132	*/	Rte_COMCbk_BCM_BackHomeSts_52444383	,/*BCM_BackHomeSts*/
	/*	133	*/	NULL	,/*BCM_AutolampSts*/
	/*	134	*/	NULL	,/*BCM_readinglampSts*/
	/*	135	*/	Rte_COMCbk_BCM_DayRunningLampDig_52444383	,/*BCM_DayRunningLampDig*/
	/*	136	*/	Rte_COMCbk_BCM_BrakelightDig_52444383	,/*BCM_BrakelightDig*/
	/*	137	*/	Rte_COMCbk_BCM_reversinglightDig_52444383	,/*BCM_reversinglightDig*/
	/*	138	*/	Rte_COMCbk_BCM_TurnIndicatorDig_52444383	,/*BCM_TurnIndicatorDig*/
	/*	139	*/	Rte_COMCbk_BCM_RearFogLmpDig_52444383	,/*BCM_RearFogLmpDig*/
	/*	140	*/	Rte_COMCbk_BCM_PosLmpDig_52444383	,/*BCM_PosLmpDig*/
	/*	141	*/	NULL	,/*BCM_Rollingcounter_0x318*/
	/*	142	*/	Rte_COMCbk_BCM_ImmoSts_52444383	,/*BCM_ImmoSts*/
	/*	143	*/	NULL	,/*BCM_Checksum_0x318*/
	/*	144	*/	Rte_COMCbk_SWM_Menu_Right_f733f54d	,/*SWM_Menu_Right*/
	/*	145	*/	Rte_COMCbk_SWM_Menu_Left_f733f54d	,/*SWM_Menu_Left*/
	/*	146	*/	Rte_COMCbk_SWM_Menu_Down_f733f54d	,/*SWM_Menu_Down*/
	/*	147	*/	Rte_COMCbk_SWM_Menu_Up_f733f54d	,/*SWM_Menu_Up*/
	/*	148	*/	NULL	,/*Rolling_counter_0x31A*/
	/*	149	*/	Rte_COMCbk_SWM_Menu_Confirm_f733f54d	,/*SWM_Menu_Confirm*/
	/*	150	*/	NULL	,/*Checksum_0x31A*/
	/*	151	*/	Rte_COMCbk_PEPS_PowerMode_9b45c0e5	,/*PEPS_PowerMode*/
	/*	152	*/	Rte_COMCbk_PEPS_IMMO_MatchStatus_9b45c0e5	,/*PEPS_IMMO_MatchStatus*/
	/*	153	*/	Rte_COMCbk_PEPS_SystemFailStatus_9b45c0e5	,/*PEPS_SystemFailStatus*/
	/*	154	*/	Rte_COMCbk_PEPS_ESCL_FailStatus_9b45c0e5	,/*PEPS_ESCL_FailStatus*/
	/*	155	*/	Rte_COMCbk_PEPS_SSB_FailStatus_9b45c0e5	,/*PEPS_SSB_FailStatus*/
	/*	156	*/	Rte_COMCbk_PEPS_ClutchIndicationWarning_9b45c0e5	,/*PEPS_ClutchIndicationWarning*/
	/*	157	*/	Rte_COMCbk_PEPS_BrakeIndicationWarning_9b45c0e5	,/*PEPS_BrakeIndicationWarning*/
	/*	158	*/	Rte_COMCbk_PEPS_NotPNWarning_9b45c0e5	,/*PEPS_NotPNWarning*/
	/*	159	*/	Rte_COMCbk_PEPS_NotNWarning_9b45c0e5	,/*PEPS_NotNWarning*/
	/*	160	*/	Rte_COMCbk_PEPS_DoorOpenNotOFFWarning_9b45c0e5	,/*PEPS_DoorOpenNotOFFWarning*/
	/*	161	*/	Rte_COMCbk_PEPS_DoorOpenWarning_9b45c0e5	,/*PEPS_DoorOpenWarning*/
	/*	162	*/	Rte_COMCbk_PEPS_KeyBatteryWarning_9b45c0e5	,/*PEPS_KeyBatteryWarning*/
	/*	163	*/	Rte_COMCbk_PEPS_KeyOutWarning1_9b45c0e5	,/*PEPS_KeyOutWarning1*/
	/*	164	*/	Rte_COMCbk_PEPS_NoKeyFoundWarning_9b45c0e5	,/*PEPS_NoKeyFoundWarning*/
	/*	165	*/	Rte_COMCbk_PEPS_KeyReminderWarning_9b45c0e5	,/*PEPS_KeyReminderWarning*/
	/*	166	*/	NULL	,/*PEPS_RollingCounter_0x325*/
	/*	167	*/	Rte_COMCbk_PEPS_RemainingNumSts_9b45c0e5	,/*PEPS_RemainingNumSts*/
	/*	168	*/	NULL	,/*PEPS_Checksum_0x325*/
	/*	169	*/	Rte_COMCbk_RF_KeyLowBatteryWarning_131d5da3	,/*RF_KeyLowBatteryWarning*/
	/*	170	*/	Rte_COMCbk_SRS_SBR_PAMsenger_9595b6c2	,/*SRS_SBR_PAMsenger*/
	/*	171	*/	Rte_COMCbk_SRS_SBR_Driver_9595b6c2	,/*SRS_SBR_Driver*/
	/*	172	*/	Rte_COMCbk_SRS_AirBagFailSts_9595b6c2	,/*SRS_AirBagFailSts*/
	/*	173	*/	NULL	,/*SRS_SBR_SecondRowRight*/
	/*	174	*/	NULL	,/*SRS_SBR_SecondRowMid*/
	/*	175	*/	NULL	,/*SRS_SBR_SecondRowLeft*/
	/*	176	*/	NULL	,/*Rolling_counter_0x350*/
	/*	177	*/	NULL	,/*Checksum_0x350*/
	/*	178	*/	Rte_COMCbk_TPMS_LF_Pressure_c030f382	,/*TPMS_LF_Pressure*/
	/*	179	*/	Rte_COMCbk_TPMS_RF_Pressure_c030f382	,/*TPMS_RF_Pressure*/
	/*	180	*/	Rte_COMCbk_TPMS_RR_Pressure_c030f382	,/*TPMS_RR_Pressure*/
	/*	181	*/	Rte_COMCbk_TPMS_LR_Pressure_c030f382	,/*TPMS_LR_Pressure*/
	/*	182	*/	Rte_COMCbk_TPMS_Tire_Temperature_c030f382	,/*TPMS_Tire_Temperature*/
	/*	183	*/	Rte_COMCbk_TPMS_LF_Pressure_Warning_c030f382	,/*TPMS_LF_Pressure_Warning*/
	/*	184	*/	Rte_COMCbk_TPMS_RF_Pressure_Warning_c030f382	,/*TPMS_RF_Pressure_Warning*/
	/*	185	*/	Rte_COMCbk_TPMS_System_Status_c030f382	,/*TPMS_System_Status*/
	/*	186	*/	Rte_COMCbk_TPMS_RR_Pressure_Warning_c030f382	,/*TPMS_RR_Pressure_Warning*/
	/*	187	*/	Rte_COMCbk_TPMS_LR_Pressure_Warning_c030f382	,/*TPMS_LR_Pressure_Warning*/
	/*	188	*/	Rte_COMCbk_Temperature_Warning_c030f382	,/*Temperature_Warning*/
	/*	189	*/	NULL	,/*resrved*/
	/*	190	*/	Rte_COMCbk_Tire_Position_c030f382	,/*Tire_Position*/
	/*	191	*/	NULL	,/*TPMS_Lamp_Status*/
	/*	192	*/	NULL	,/*RRI_Pressure*/
	/*	193	*/	NULL	,/*LRI_Pressure*/
	/*	194	*/	NULL	,/*RRI_Pressure_Warning*/
	/*	195	*/	NULL	,/*LRI_Pressure_Warning*/
	/*	196	*/	Rte_COMCbk_EMS_Fuelcounter_daed4b4e	,/*EMS_Fuelcounter*/
	/*	197	*/	Rte_COMCbk_EMS_RemindShiftGear_MT_daed4b4e	,/*EMS_RemindShiftGear_MT*/
	/*	198	*/	Rte_COMCbk_EMS_MIL_daed4b4e	,/*EMS_MIL*/
	/*	199	*/	Rte_COMCbk_EMS_SVS_daed4b4e	,/*EMS_SVS*/
	/*	200	*/	Rte_COMCbk_EMS_StartStopStatus_daed4b4e	,/*EMS_StartStopStatus*/
	/*	201	*/	Rte_COMCbk_EMS_OBDTxt_daed4b4e	,/*EMS_OBDTxt*/
	/*	202	*/	Rte_COMCbk_EMS_TroqLimtHour_daa19a1a	,/*EMS_TroqLimtHour*/
	/*	203	*/	Rte_COMCbk_EMS_UreaQty_daa19a1a	,/*EMS_UreaQty*/
	/*	204	*/	Rte_COMCbk_EMS_TroqLimtMin_daa19a1a	,/*EMS_TroqLimtMin*/
	/*	205	*/	Rte_COMCbk_EMS_UreaLvl_daa19a1a	,/*EMS_UreaLvl*/
	/*	206	*/	Rte_COMCbk_EMS_DieslPrtcWarn_D_Rq_daa19a1a	,/*EMS_DieslPrtcWarn_D_Rq*/
	/*	207	*/	Rte_COMCbk_DCM_GlowPlugLmpReq_daa19a1a	,/*DCM_GlowPlugLmpReq*/
	/*	208	*/	Rte_COMCbk_DCM_WIFFullLmpReq_daa19a1a	,/*DCM_WIFFullLmpReq*/
	/*	209	*/	Rte_COMCbk_EMS_SpeedLimtHour_daa19a1a	,/*EMS_SpeedLimtHour*/
	/*	210	*/	Rte_COMCbk_EMS_SpeedLimtMin_daa19a1a	,/*EMS_SpeedLimtMin*/
	/*	211	*/	Rte_COMCbk_EMS_UreaSysWarning_daa19a1a	,/*EMS_UreaSysWarning*/
	/*	212	*/	Rte_COMCbk_EMS_UreaTxt_daa19a1a	,/*EMS_UreaTxt*/
	/*	213	*/	Rte_COMCbk_EMS_ManRgenTxt_D_RqDsply_daa19a1a	,/*EMS_ManRgenTxt_D_RqDsply*/
	/*	214	*/	Rte_COMCbk_EMS_UreaWarn_daa19a1a	,/*EMS_UreaWarn*/
	/*	215	*/	Rte_COMCbk_EMS_Soot_Pc_Dsply_daa19a1a	,/*EMS_Soot_Pc_Dsply*/
	/*	216	*/	Rte_COMCbk_MP5_BSDSoundSwitchSts_c90244fe	,/*MP5_BSDSoundSwitchSts*/
	/*	217	*/	Rte_COMCbk_MP5_NavICON_0f6d4d79	,/*MP5_NavICON*/
	/*	218	*/	Rte_COMCbk_MP5_ROUTE_REMAIN_DIS_0f6d4d79	,/*MP5_ROUTE_REMAIN_DIS*/
	/*	219	*/	NULL	,/*MP5_ALModeSetting*/
	/*	220	*/	NULL	,/*MP5_ALStartAroundSetting*/
	/*	221	*/	NULL	,/*Trans_Limphome*/
	/*	222	*/	Rte_COMCbk_TrnSrvcRqd_B_Rq_94ca6b54	,/*TrnSrvcRqd_B_Rq*/
	/*	223	*/	NULL	,/*TCU_Gearbox_Gonglimp*/
	/*	224	*/	Rte_COMCbk_TCU_Driverwarning_94ca6b54	,/*TCU_Driverwarning*/
	/*	225	*/	Rte_COMCbk_TCU_DrvShiftAllwCondRmd_94ca6b54	,/*TCU_DrvShiftAllwCondRmd*/
	/*	226	*/	Rte_COMCbk_MP5_AlarmStatus_046cad19	,/*MP5_AlarmStatus*/
	/*	227	*/	Rte_COMCbk_MP5_AlarmOrNot_046cad19	,/*MP5_AlarmOrNot*/
	/*	228	*/	Rte_COMCbk_TBOX_Year_4e88e95a	,/*TBOX_Year*/
	/*	229	*/	Rte_COMCbk_TBOX_Month_4e88e95a	,/*TBOX_Month*/
	/*	230	*/	Rte_COMCbk_TBOX_Date_4e88e95a	,/*TBOX_Date*/
	/*	231	*/	Rte_COMCbk_TBOX_Hour_4e88e95a	,/*TBOX_Hour*/
	/*	232	*/	Rte_COMCbk_TBOX_Minute_4e88e95a	,/*TBOX_Minute*/
	/*	233	*/	Rte_COMCbk_TBOX_Second_4e88e95a	,/*TBOX_Second*/
	/*	234	*/	Rte_COMCbk_TBOX_RealTime_UB_Copy_1_4e88e95a	,/*TBOX_RealTime_UB_Copy_1*/
	/*	235	*/	NULL	,/*TBOX_RealTime_UB*/
	/*	236	*/	Rte_COMCbk_BCM_LoadManageLevel_00b00241	,/*BCM_LoadManageLevel*/
	/*	237	*/	Rte_COMCbk_BCM_CarModeIndicator_00b00241	,/*BCM_CarModeIndicator*/
	/*	238	*/	NULL	,/*BCM_CarMode*/
	/*	239	*/	Rte_COMCbk_EMS_BackupOdometer_63950623	,/*EMS_BackupOdometer*/
  };
  #endif


  #if (CCL_RX_TOUTINDICATION_API == STD_ON)
  /**********************************************************************************************************************
    Ccl_CbkRxTOutFuncPtr
  **********************************************************************************************************************/
  /**
    \var    Ccl_CbkRxTOutFuncPtr
    \brief  Function pointer table containing configured Rx timeout notifications for signals and signal groups. ea:{71D75BEB-245A-468b-96DB-A4F1362E927C}
  */
CONST(CclCbkRxTOutType, CCL_CONST) Can_Ccl_CbkRxTOutFuncPtr[Can_Ccl_Rx_Num_RxTOut_Ind] = {
/* Index	  CbkRxTOutFuncPtr	   */
	/*	0	*/	NULL	,/*EMS_BrkPedalStasus*/
	/*	1	*/	Rte_COMCbkRxTOut_EMS_EngCoolanTemp_85187d93	,/*EMS_EngCoolanTemp*/
	/*	2	*/	NULL	,/*EMS_EngOperationStatus*/
	/*	3	*/	NULL	,/*EMS_EngSpeedError*/
	/*	4	*/	Rte_COMCbkRxTOut_EMS_BatteryVoltageError_85187d93	,/*EMS_BatteryVoltageError*/
	/*	5	*/	Rte_COMCbkRxTOut_EMS_EngSpeed_85187d93	,/*EMS_EngSpeed*/
	/*	6	*/	Rte_COMCbkRxTOut_EMS_CluthPedalStatus_85187d93	,/*EMS_CluthPedalStatus*/
	/*	7	*/	Rte_COMCbkRxTOut_TCU_DisplayGear_cb5c6a72	,/*TCU_DisplayGear*/
	/*	8	*/	Rte_COMCbkRxTOut_TCU_GearSelectorReq_cb5c6a72	,/*TCU_GearSelectorReq*/
	/*	9	*/	NULL	,/*TCU_StGearMode*/
	/*	10	*/	NULL	,/*TCU_Status_AliveCounter*/
	/*	11	*/	NULL	,/*TCU_Status_Checksum*/
	/*	12	*/	Rte_COMCbkRxTOut_EMS_TargCruiseSpeed_341d7c91	,/*EMS_TargCruiseSpeed*/
	/*	13	*/	NULL	,/*EMS_Rolling_counter_0x151*/
	/*	14	*/	Rte_COMCbkRxTOut_EMS_CruiseControlStatus_341d7c91	,/*EMS_CruiseControlStatus*/
	/*	15	*/	NULL	,/*EMS_Checksum_0x151*/
	/*	16	*/	NULL	,/*PEPS_Remotestartstatus*/
	/*	17	*/	NULL	,/*PEPS_RollingCounter_0x1A5*/
	/*	18	*/	NULL	,/*PEPS_Checksum_0x1A5*/
	/*	19	*/	Rte_COMCbkRxTOut_ABS_WhlMilgFrntLe_2356a03c	,/*ABS_WhlMilgFrntLe*/
	/*	20	*/	Rte_COMCbkRxTOut_ABS_WhlMilgFrntRi_2356a03c	,/*ABS_WhlMilgFrntRi*/
	/*	21	*/	Rte_COMCbkRxTOut_ABS_VehSpdLgt_2356a03c	,/*ABS_VehSpdLgt*/
	/*	22	*/	Rte_COMCbkRxTOut_ABS_EbdFlgFlt_2356a03c	,/*ABS_EbdFlgFlt*/
	/*	23	*/	Rte_COMCbkRxTOut_ABS_AbsFlgFlt_2356a03c	,/*ABS_AbsFlgFlt*/
	/*	24	*/	NULL	,/*ABS_RollingCounter_0x221*/
	/*	25	*/	Rte_COMCbkRxTOut_ABS_WhlMilgFrntLeStatus_2356a03c	,/*ABS_WhlMilgFrntLeStatus*/
	/*	26	*/	Rte_COMCbkRxTOut_ABS_WhlMilgFrntRiStatus_2356a03c	,/*ABS_WhlMilgFrntRiStatus*/
	/*	27	*/	NULL	,/*ABS_Checksum_0x221*/
	/*	28	*/	Rte_COMCbkRxTOut_ESP_EPBStatus_af66671b	,/*ESP_EPBStatus*/
	/*	29	*/	Rte_COMCbkRxTOut_ESP_EPBErrorStatus_af66671b	,/*ESP_EPBErrorStatus*/
	/*	30	*/	Rte_COMCbkRxTOut_ESP_CdpAvailable_af66671b	,/*ESP_CdpAvailable*/
	/*	31	*/	Rte_COMCbkRxTOut_ESP_EPBWarningMessage3_af66671b	,/*ESP_EPBWarningMessage3*/
	/*	32	*/	Rte_COMCbkRxTOut_ESP_EPBWarningMessage2_af66671b	,/*ESP_EPBWarningMessage2*/
	/*	33	*/	Rte_COMCbkRxTOut_ESP_EPBWarningMessage1_af66671b	,/*ESP_EPBWarningMessage1*/
	/*	34	*/	Rte_COMCbkRxTOut_ESP_AVHWarningMessage_af66671b	,/*ESP_AVHWarningMessage*/
	/*	35	*/	Rte_COMCbkRxTOut_ESP_AvhActive_af66671b	,/*ESP_AvhActive*/
	/*	36	*/	Rte_COMCbkRxTOut_ESP_AvhAvabliable_af66671b	,/*ESP_AvhAvabliable*/
	/*	37	*/	Rte_COMCbkRxTOut_ESP_VdcFlgFlt_af66671b	,/*ESP_VdcFlgFlt*/
	/*	38	*/	Rte_COMCbkRxTOut_ESP_TcsFlgFlt_af66671b	,/*ESP_TcsFlgFlt*/
	/*	39	*/	Rte_COMCbkRxTOut_ESP_ESPInfoLamp_af66671b	,/*ESP_ESPInfoLamp*/
	/*	40	*/	Rte_COMCbkRxTOut_ESP_EspEnableSts_af66671b	,/*ESP_EspEnableSts*/
	/*	41	*/	NULL	,/*ESP_RollingCounter_0x243*/
	/*	42	*/	NULL	,/*ESP_Checksum_0x243*/
	/*	43	*/	Rte_COMCbkRxTOut_IPM_LaneAssitType_32f79b14	,/*IPM_LaneAssitType*/
	/*	44	*/	NULL	,/*IPM_LaneAssit_WarningVibration*/
	/*	45	*/	Rte_COMCbkRxTOut_IPM_LaneAssit_WarningAudio_32f79b14	,/*IPM_LaneAssit_WarningAudio*/
	/*	46	*/	Rte_COMCbkRxTOut_IPM_LaneAssit_LeftVisualization_32f79b14	,/*IPM_LaneAssit_LeftVisualization*/
	/*	47	*/	Rte_COMCbkRxTOut_IPM_LaneAssit_RightVisualization_32f79b14	,/*IPM_LaneAssit_RightVisualization*/
	/*	48	*/	Rte_COMCbkRxTOut_IPM_SLASpdlimitUnits_32f79b14	,/*IPM_SLASpdlimitUnits*/
	/*	49	*/	Rte_COMCbkRxTOut_IPM_SLAState_32f79b14	,/*IPM_SLAState*/
	/*	50	*/	Rte_COMCbkRxTOut_IPM_LaneAssit_Status_32f79b14	,/*IPM_LaneAssit_Status*/
	/*	51	*/	Rte_COMCbkRxTOut_IPM_SLASpdlimit_32f79b14	,/*IPM_SLASpdlimit*/
	/*	52	*/	Rte_COMCbkRxTOut_IPM_HMA_Status_32f79b14	,/*IPM_HMA_Status*/
	/*	53	*/	Rte_COMCbkRxTOut_IPM_DDD_IndexStatus_32f79b14	,/*IPM_DDD_IndexStatus*/
	/*	54	*/	Rte_COMCbkRxTOut_IPM_DDD_WarningLevel_32f79b14	,/*IPM_DDD_WarningLevel*/
	/*	55	*/	NULL	,/*Rolling_counter_0x245*/
	/*	56	*/	Rte_COMCbkRxTOut_IPM_LaneAssit_HandsonReq_32f79b14	,/*IPM_LaneAssit_HandsonReq*/
	/*	57	*/	NULL	,/*Checksum_0x245*/
	/*	58	*/	Rte_COMCbkRxTOut_vSetDis_255e8a04	,/*vSetDis*/
	/*	59	*/	Rte_COMCbkRxTOut_MRR_ObjValid_255e8a04	,/*MRR_ObjValid*/
	/*	60	*/	Rte_COMCbkRxTOut_MRR_TauGapSet_255e8a04	,/*MRR_TauGapSet*/
	/*	61	*/	Rte_COMCbkRxTOut_MRR_dxTarObj_255e8a04	,/*MRR_dxTarObj*/
	/*	62	*/	Rte_COMCbkRxTOut_ACCHMI_Mode_255e8a04	,/*ACCHMI_Mode*/
	/*	63	*/	NULL	,/*ACCFailInfo*/
	/*	64	*/	Rte_COMCbkRxTOut_MRR_TakeOverReq_255e8a04	,/*MRR_TakeOverReq*/
	/*	65	*/	Rte_COMCbkRxTOut_MRR_AEB_STATE_255e8a04	,/*MRR_AEB_STATE*/
	/*	66	*/	NULL	,/*ACC_Startstop_infor*/
	/*	67	*/	Rte_COMCbkRxTOut_MRR_PCW_preWarning_255e8a04	,/*MRR_PCW_preWarning*/
	/*	68	*/	Rte_COMCbkRxTOut_MRR_PCW_latentWarning_255e8a04	,/*MRR_PCW_latentWarning*/
	/*	69	*/	Rte_COMCbkRxTOut_FCW_preWarning_255e8a04	,/*FCW_preWarning*/
	/*	70	*/	Rte_COMCbkRxTOut_FCW_latentWarning_255e8a04	,/*FCW_latentWarning*/
	/*	71	*/	Rte_COMCbkRxTOut_MRR_PCW_STATE_255e8a04	,/*MRR_PCW_STATE*/
	/*	72	*/	NULL	,/*Obj_Speed*/
	/*	73	*/	NULL	,/*Rolling_counter_0x246*/
	/*	74	*/	Rte_COMCbkRxTOut_Textinfo_255e8a04	,/*Textinfo*/
	/*	75	*/	NULL	,/*Checksum_0x246*/
	/*	76	*/	Rte_COMCbkRxTOut_EPS_S_WarningLampYellow_e5742092	,/*EPS_S_WarningLampYellow*/
	/*	77	*/	Rte_COMCbkRxTOut_EPS_S_SafeLampRed_e5742092	,/*EPS_S_SafeLampRed*/
	/*	78	*/	NULL	,/*Rolling_counter_0x260*/
	/*	79	*/	NULL	,/*Checksum_0x260*/
	/*	80	*/	Rte_COMCbkRxTOut_PAM_ChimeCmdSoundStatus_49f5ddcc	,/*PAM_ChimeCmdSoundStatus*/
	/*	81	*/	Rte_COMCbkRxTOut_PAM_RearSysTemWorkSts_49f5ddcc	,/*PAM_RearSysTemWorkSts*/
	/*	82	*/	Rte_COMCbkRxTOut_PAM_FrontSysTemWorkSts_49f5ddcc	,/*PAM_FrontSysTemWorkSts*/
	/*	83	*/	Rte_COMCbkRxTOut_PAM_ChimeCmdSoundCadence_49f5ddcc	,/*PAM_ChimeCmdSoundCadence*/
	/*	84	*/	Rte_COMCbkRxTOut_SOD_LCA_warningReqRight_a75280f1	,/*SOD_LCA_warningReqRight*/
	/*	85	*/	Rte_COMCbkRxTOut_SOD_DOA_warningReqRightRear_a75280f1	,/*SOD_DOA_warningReqRightRear*/
	/*	86	*/	Rte_COMCbkRxTOut_SOD_DOA_warningReqRightFront_a75280f1	,/*SOD_DOA_warningReqRightFront*/
	/*	87	*/	Rte_COMCbkRxTOut_SOD_CTA_warningReqRight_a75280f1	,/*SOD_CTA_warningReqRight*/
	/*	88	*/	Rte_COMCbkRxTOut_SOD_BLIS_display_a75280f1	,/*SOD_BLIS_display*/
	/*	89	*/	Rte_COMCbkRxTOut_SOD_LCA_warningReqLeft_a75280f1	,/*SOD_LCA_warningReqLeft*/
	/*	90	*/	Rte_COMCbkRxTOut_SOD_DOA_warningReqLeftRear_a75280f1	,/*SOD_DOA_warningReqLeftRear*/
	/*	91	*/	Rte_COMCbkRxTOut_SOD_DOA_warningReqLeftFront_a75280f1	,/*SOD_DOA_warningReqLeftFront*/
	/*	92	*/	Rte_COMCbkRxTOut_SOD_CTA_warningReqLeft_a75280f1	,/*SOD_CTA_warningReqLeft*/
	/*	93	*/	Rte_COMCbkRxTOut_DOA_OFFTelltale_a75280f1	,/*DOA_OFFTelltale*/
	/*	94	*/	Rte_COMCbkRxTOut_CTA_OFFTelltale_a75280f1	,/*CTA_OFFTelltale*/
	/*	95	*/	Rte_COMCbkRxTOut_SODLCA_OFFTelltale_a75280f1	,/*SODLCA_OFFTelltale*/
	/*	96	*/	NULL	,/*Rolling_counter_0x275*/
	/*	97	*/	NULL	,/*Checksum_0x275*/
	/*	98	*/	Rte_COMCbkRxTOut_BCM_DoorStsRearRi_0edb943f	,/*BCM_DoorStsRearRi*/
	/*	99	*/	Rte_COMCbkRxTOut_BCM_DoorStsRearLe_0edb943f	,/*BCM_DoorStsRearLe*/
	/*	100	*/	Rte_COMCbkRxTOut_BCM_DoorStsFrntRi_0edb943f	,/*BCM_DoorStsFrntRi*/
	/*	101	*/	Rte_COMCbkRxTOut_BCM_DoorStsFrntLe_0edb943f	,/*BCM_DoorStsFrntLe*/
	/*	102	*/	NULL	,/*BCM_DoorLockStsRearRi*/
	/*	103	*/	NULL	,/*BCM_DoorLockStsRearLe*/
	/*	104	*/	NULL	,/*BCM_DoorLockStsFrntRi*/
	/*	105	*/	NULL	,/*BCM_DoorLockStsFrntLe*/
	/*	106	*/	NULL	,/*BCM_AntiTheftSts*/
	/*	107	*/	NULL	,/*BCM_LockUnlockFeedback*/
	/*	108	*/	NULL	,/*BCM_DoorLockStsTrunk*/
	/*	109	*/	Rte_COMCbkRxTOut_BCM_DoorStsTrunk_0edb943f	,/*BCM_DoorStsTrunk*/
	/*	110	*/	NULL	,/*BCM_TrunkDoorPreUnlockSts*/
	/*	111	*/	Rte_COMCbkRxTOut_BCM_EnginehoodSts_0edb943f	,/*BCM_EnginehoodSts*/
	/*	112	*/	NULL	,/*BCM_Rolling_counter_0x310*/
	/*	113	*/	NULL	,/*BCM_Checksum_0x310*/
	/*	114	*/	NULL	,/*BCM_OutsideTempDig*/
	/*	115	*/	Rte_COMCbkRxTOut_BCM_Mechanical_Hand_Brake_abc8cd44	,/*BCM_Mechanical_Hand_Brake*/
	/*	116	*/	Rte_COMCbkRxTOut_BCM_BrakeFluidSts_abc8cd44	,/*BCM_BrakeFluidSts*/
	/*	117	*/	Rte_COMCbkRxTOut_BCM_OutsideTemp_abc8cd44	,/*BCM_OutsideTemp*/
	/*	118	*/	Rte_COMCbkRxTOut_BCM_PowerMode_abc8cd44	,/*BCM_PowerMode*/
	/*	119	*/	NULL	,/*BCM_Rollingcounter_0x313*/
	/*	120	*/	NULL	,/*BCM_Checksum_0x313*/
	/*	121	*/	Rte_COMCbkRxTOut_BCM_BGLLumLvlSettingStatus_a1dcf71c	,/*BCM_BGLLumLvlSettingStatus*/
	/*	122	*/	Rte_COMCbkRxTOut_BCM_FrntFogLmpSts_52444383	,/*BCM_FrntFogLmpSts*/
	/*	123	*/	Rte_COMCbkRxTOut_BCM_PosLmpSts_52444383	,/*BCM_PosLmpSts*/
	/*	124	*/	Rte_COMCbkRxTOut_BCM_LowBeamSts_52444383	,/*BCM_LowBeamSts*/
	/*	125	*/	Rte_COMCbkRxTOut_BCM_HighBeamSts_52444383	,/*BCM_HighBeamSts*/
	/*	126	*/	NULL	,/*BCM_reversinglightSts*/
	/*	127	*/	NULL	,/*BCM_DayRunningLampSts*/
	/*	128	*/	Rte_COMCbkRxTOut_BCM_TurnIndicatorSts_52444383	,/*BCM_TurnIndicatorSts*/
	/*	129	*/	Rte_COMCbkRxTOut_BCM_RearFogLmpSts_52444383	,/*BCM_RearFogLmpSts*/
	/*	130	*/	NULL	,/*BCM_Highmountedstoplamp*/
	/*	131	*/	NULL	,/*BCM_BrakelightSts*/
	/*	132	*/	Rte_COMCbkRxTOut_BCM_BackHomeSts_52444383	,/*BCM_BackHomeSts*/
	/*	133	*/	NULL	,/*BCM_AutolampSts*/
	/*	134	*/	NULL	,/*BCM_readinglampSts*/
	/*	135	*/	Rte_COMCbkRxTOut_BCM_DayRunningLampDig_52444383	,/*BCM_DayRunningLampDig*/
	/*	136	*/	Rte_COMCbkRxTOut_BCM_BrakelightDig_52444383	,/*BCM_BrakelightDig*/
	/*	137	*/	Rte_COMCbkRxTOut_BCM_reversinglightDig_52444383	,/*BCM_reversinglightDig*/
	/*	138	*/	Rte_COMCbkRxTOut_BCM_TurnIndicatorDig_52444383	,/*BCM_TurnIndicatorDig*/
	/*	139	*/	Rte_COMCbkRxTOut_BCM_RearFogLmpDig_52444383	,/*BCM_RearFogLmpDig*/
	/*	140	*/	Rte_COMCbkRxTOut_BCM_PosLmpDig_52444383	,/*BCM_PosLmpDig*/
	/*	141	*/	NULL	,/*BCM_Rollingcounter_0x318*/
	/*	142	*/	Rte_COMCbkRxTOut_BCM_ImmoSts_52444383	,/*BCM_ImmoSts*/
	/*	143	*/	NULL	,/*BCM_Checksum_0x318*/
	/*	144	*/	Rte_COMCbkRxTOut_SWM_Menu_Right_f733f54d	,/*SWM_Menu_Right*/
	/*	145	*/	Rte_COMCbkRxTOut_SWM_Menu_Left_f733f54d	,/*SWM_Menu_Left*/
	/*	146	*/	Rte_COMCbkRxTOut_SWM_Menu_Down_f733f54d	,/*SWM_Menu_Down*/
	/*	147	*/	Rte_COMCbkRxTOut_SWM_Menu_Up_f733f54d	,/*SWM_Menu_Up*/
	/*	148	*/	NULL	,/*Rolling_counter_0x31A*/
	/*	149	*/	Rte_COMCbkRxTOut_SWM_Menu_Confirm_f733f54d	,/*SWM_Menu_Confirm*/
	/*	150	*/	NULL	,/*Checksum_0x31A*/
	/*	151	*/	Rte_COMCbkRxTOut_PEPS_PowerMode_9b45c0e5	,/*PEPS_PowerMode*/
	/*	152	*/	Rte_COMCbkRxTOut_PEPS_IMMO_MatchStatus_9b45c0e5	,/*PEPS_IMMO_MatchStatus*/
	/*	153	*/	Rte_COMCbkRxTOut_PEPS_SystemFailStatus_9b45c0e5	,/*PEPS_SystemFailStatus*/
	/*	154	*/	Rte_COMCbkRxTOut_PEPS_ESCL_FailStatus_9b45c0e5	,/*PEPS_ESCL_FailStatus*/
	/*	155	*/	Rte_COMCbkRxTOut_PEPS_SSB_FailStatus_9b45c0e5	,/*PEPS_SSB_FailStatus*/
	/*	156	*/	Rte_COMCbkRxTOut_PEPS_ClutchIndicationWarning_9b45c0e5	,/*PEPS_ClutchIndicationWarning*/
	/*	157	*/	Rte_COMCbkRxTOut_PEPS_BrakeIndicationWarning_9b45c0e5	,/*PEPS_BrakeIndicationWarning*/
	/*	158	*/	Rte_COMCbkRxTOut_PEPS_NotPNWarning_9b45c0e5	,/*PEPS_NotPNWarning*/
	/*	159	*/	Rte_COMCbkRxTOut_PEPS_NotNWarning_9b45c0e5	,/*PEPS_NotNWarning*/
	/*	160	*/	Rte_COMCbkRxTOut_PEPS_DoorOpenNotOFFWarning_9b45c0e5	,/*PEPS_DoorOpenNotOFFWarning*/
	/*	161	*/	Rte_COMCbkRxTOut_PEPS_DoorOpenWarning_9b45c0e5	,/*PEPS_DoorOpenWarning*/
	/*	162	*/	Rte_COMCbkRxTOut_PEPS_KeyBatteryWarning_9b45c0e5	,/*PEPS_KeyBatteryWarning*/
	/*	163	*/	Rte_COMCbkRxTOut_PEPS_KeyOutWarning1_9b45c0e5	,/*PEPS_KeyOutWarning1*/
	/*	164	*/	Rte_COMCbkRxTOut_PEPS_NoKeyFoundWarning_9b45c0e5	,/*PEPS_NoKeyFoundWarning*/
	/*	165	*/	Rte_COMCbkRxTOut_PEPS_KeyReminderWarning_9b45c0e5	,/*PEPS_KeyReminderWarning*/
	/*	166	*/	NULL	,/*PEPS_RollingCounter_0x325*/
	/*	167	*/	Rte_COMCbkRxTOut_PEPS_RemainingNumSts_9b45c0e5	,/*PEPS_RemainingNumSts*/
	/*	168	*/	NULL	,/*PEPS_Checksum_0x325*/
	/*	169	*/	Rte_COMCbkRxTOut_RF_KeyLowBatteryWarning_131d5da3	,/*RF_KeyLowBatteryWarning*/
	/*	170	*/	Rte_COMCbkRxTOut_SRS_SBR_PAMsenger_9595b6c2	,/*SRS_SBR_PAMsenger*/
	/*	171	*/	Rte_COMCbkRxTOut_SRS_SBR_Driver_9595b6c2	,/*SRS_SBR_Driver*/
	/*	172	*/	Rte_COMCbkRxTOut_SRS_AirBagFailSts_9595b6c2	,/*SRS_AirBagFailSts*/
	/*	173	*/	NULL	,/*SRS_SBR_SecondRowRight*/
	/*	174	*/	NULL	,/*SRS_SBR_SecondRowMid*/
	/*	175	*/	NULL	,/*SRS_SBR_SecondRowLeft*/
	/*	176	*/	NULL	,/*Rolling_counter_0x350*/
	/*	177	*/	NULL	,/*Checksum_0x350*/
	/*	178	*/	Rte_COMCbkRxTOut_TPMS_LF_Pressure_c030f382	,/*TPMS_LF_Pressure*/
	/*	179	*/	Rte_COMCbkRxTOut_TPMS_RF_Pressure_c030f382	,/*TPMS_RF_Pressure*/
	/*	180	*/	Rte_COMCbkRxTOut_TPMS_RR_Pressure_c030f382	,/*TPMS_RR_Pressure*/
	/*	181	*/	Rte_COMCbkRxTOut_TPMS_LR_Pressure_c030f382	,/*TPMS_LR_Pressure*/
	/*	182	*/	Rte_COMCbkRxTOut_TPMS_Tire_Temperature_c030f382	,/*TPMS_Tire_Temperature*/
	/*	183	*/	Rte_COMCbkRxTOut_TPMS_LF_Pressure_Warning_c030f382	,/*TPMS_LF_Pressure_Warning*/
	/*	184	*/	Rte_COMCbkRxTOut_TPMS_RF_Pressure_Warning_c030f382	,/*TPMS_RF_Pressure_Warning*/
	/*	185	*/	Rte_COMCbkRxTOut_TPMS_System_Status_c030f382	,/*TPMS_System_Status*/
	/*	186	*/	Rte_COMCbkRxTOut_TPMS_RR_Pressure_Warning_c030f382	,/*TPMS_RR_Pressure_Warning*/
	/*	187	*/	Rte_COMCbkRxTOut_TPMS_LR_Pressure_Warning_c030f382	,/*TPMS_LR_Pressure_Warning*/
	/*	188	*/	Rte_COMCbkRxTOut_Temperature_Warning_c030f382	,/*Temperature_Warning*/
	/*	189	*/	NULL	,/*resrved*/
	/*	190	*/	Rte_COMCbkRxTOut_Tire_Position_c030f382	,/*Tire_Position*/
	/*	191	*/	NULL	,/*TPMS_Lamp_Status*/
	/*	192	*/	NULL	,/*RRI_Pressure*/
	/*	193	*/	NULL	,/*LRI_Pressure*/
	/*	194	*/	NULL	,/*RRI_Pressure_Warning*/
	/*	195	*/	NULL	,/*LRI_Pressure_Warning*/
	/*	196	*/	Rte_COMCbkRxTOut_EMS_Fuelcounter_daed4b4e	,/*EMS_Fuelcounter*/
	/*	197	*/	Rte_COMCbkRxTOut_EMS_RemindShiftGear_MT_daed4b4e	,/*EMS_RemindShiftGear_MT*/
	/*	198	*/	Rte_COMCbkRxTOut_EMS_MIL_daed4b4e	,/*EMS_MIL*/
	/*	199	*/	Rte_COMCbkRxTOut_EMS_SVS_daed4b4e	,/*EMS_SVS*/
	/*	200	*/	Rte_COMCbkRxTOut_EMS_StartStopStatus_daed4b4e	,/*EMS_StartStopStatus*/
	/*	201	*/	Rte_COMCbkRxTOut_EMS_OBDTxt_daed4b4e	,/*EMS_OBDTxt*/
	/*	202	*/	Rte_COMCbkRxTOut_EMS_TroqLimtHour_daa19a1a	,/*EMS_TroqLimtHour*/
	/*	203	*/	Rte_COMCbkRxTOut_EMS_UreaQty_daa19a1a	,/*EMS_UreaQty*/
	/*	204	*/	Rte_COMCbkRxTOut_EMS_TroqLimtMin_daa19a1a	,/*EMS_TroqLimtMin*/
	/*	205	*/	Rte_COMCbkRxTOut_EMS_UreaLvl_daa19a1a	,/*EMS_UreaLvl*/
	/*	206	*/	Rte_COMCbkRxTOut_EMS_DieslPrtcWarn_D_Rq_daa19a1a	,/*EMS_DieslPrtcWarn_D_Rq*/
	/*	207	*/	Rte_COMCbkRxTOut_DCM_GlowPlugLmpReq_daa19a1a	,/*DCM_GlowPlugLmpReq*/
	/*	208	*/	Rte_COMCbkRxTOut_DCM_WIFFullLmpReq_daa19a1a	,/*DCM_WIFFullLmpReq*/
	/*	209	*/	Rte_COMCbkRxTOut_EMS_SpeedLimtHour_daa19a1a	,/*EMS_SpeedLimtHour*/
	/*	210	*/	Rte_COMCbkRxTOut_EMS_SpeedLimtMin_daa19a1a	,/*EMS_SpeedLimtMin*/
	/*	211	*/	Rte_COMCbkRxTOut_EMS_UreaSysWarning_daa19a1a	,/*EMS_UreaSysWarning*/
	/*	212	*/	Rte_COMCbkRxTOut_EMS_UreaTxt_daa19a1a	,/*EMS_UreaTxt*/
	/*	213	*/	Rte_COMCbkRxTOut_EMS_ManRgenTxt_D_RqDsply_daa19a1a	,/*EMS_ManRgenTxt_D_RqDsply*/
	/*	214	*/	Rte_COMCbkRxTOut_EMS_UreaWarn_daa19a1a	,/*EMS_UreaWarn*/
	/*	215	*/	Rte_COMCbkRxTOut_EMS_Soot_Pc_Dsply_daa19a1a	,/*EMS_Soot_Pc_Dsply*/
	/*	216	*/	Rte_COMCbkRxTOut_MP5_BSDSoundSwitchSts_c90244fe	,/*MP5_BSDSoundSwitchSts*/
	/*	217	*/	Rte_COMCbkRxTOut_MP5_NavICON_0f6d4d79	,/*MP5_NavICON*/
	/*	218	*/	Rte_COMCbkRxTOut_MP5_ROUTE_REMAIN_DIS_0f6d4d79	,/*MP5_ROUTE_REMAIN_DIS*/
	/*	219	*/	NULL	,/*MP5_ALModeSetting*/
	/*	220	*/	NULL	,/*MP5_ALStartAroundSetting*/
	/*	221	*/	NULL	,/*Trans_Limphome*/
	/*	222	*/	Rte_COMCbkRxTOut_TrnSrvcRqd_B_Rq_94ca6b54	,/*TrnSrvcRqd_B_Rq*/
	/*	223	*/	NULL	,/*TCU_Gearbox_Gonglimp*/
	/*	224	*/	Rte_COMCbkRxTOut_TCU_Driverwarning_94ca6b54	,/*TCU_Driverwarning*/
	/*	225	*/	Rte_COMCbkRxTOut_TCU_DrvShiftAllwCondRmd_94ca6b54	,/*TCU_DrvShiftAllwCondRmd*/
	/*	226	*/	Rte_COMCbkRxTOut_MP5_AlarmStatus_046cad19	,/*MP5_AlarmStatus*/
	/*	227	*/	Rte_COMCbkRxTOut_MP5_AlarmOrNot_046cad19	,/*MP5_AlarmOrNot*/
	/*	228	*/	Rte_COMCbkRxTOut_TBOX_Year_4e88e95a	,/*TBOX_Year*/
	/*	229	*/	Rte_COMCbkRxTOut_TBOX_Month_4e88e95a	,/*TBOX_Month*/
	/*	230	*/	Rte_COMCbkRxTOut_TBOX_Date_4e88e95a	,/*TBOX_Date*/
	/*	231	*/	Rte_COMCbkRxTOut_TBOX_Hour_4e88e95a	,/*TBOX_Hour*/
	/*	232	*/	Rte_COMCbkRxTOut_TBOX_Minute_4e88e95a	,/*TBOX_Minute*/
	/*	233	*/	Rte_COMCbkRxTOut_TBOX_Second_4e88e95a	,/*TBOX_Second*/
	/*	234	*/	Rte_COMCbkRxTOut_TBOX_RealTime_UB_Copy_1_4e88e95a	,/*TBOX_RealTime_UB_Copy_1*/
	/*	235	*/	NULL	,/*TBOX_RealTime_UB*/
	/*	236	*/	Rte_COMCbkRxTOut_BCM_LoadManageLevel_00b00241	,/*BCM_LoadManageLevel*/
	/*	237	*/	Rte_COMCbkRxTOut_BCM_CarModeIndicator_00b00241	,/*BCM_CarModeIndicator*/
	/*	238	*/	NULL	,/*BCM_CarMode*/
	/*	239	*/	Rte_COMCbkRxTOut_EMS_BackupOdometer_63950623	,/*EMS_BackupOdometer*/
  };
  #endif


  #if (CCL_RX_INVINDICATION_API == STD_ON)
  /**********************************************************************************************************************
    Ccl_CbkRxInvFuncPtr
  **********************************************************************************************************************/
  /**
    \var    Ccl_CbkRxInvFuncPtr
    \brief  Function pointer table containing configured Rx Indication notifications for signals and signal groups.*/

  CONST(CclCbkInvType, CCL_CONST) Can_Ccl_CbkRxInvFuncPtr[Can_Ccl_Rx_Num_RxInv_Ind] = {
    /* Index    CbkRxAckFuncPtr                        */
    /*     0 */ NULL
  };
  #endif


  /**********************************************************************************************************************
    Can_Ccl_Channel0_RxNotifFuncPtr
  **********************************************************************************************************************/
  CONST(Ccl_RxNotifFuncInfo, CCL_CONST) Can_Ccl_RxNotifFuncPtr[Can_Ccl_Rx_Num_Signals] = {
        /* Index      RxAckIndex          RxToutIndex         RxInvIndex       */
  { /*  0  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*EMS_BrkPedalStasus*/
  { /*  1  */    CanCclRxHndlCh0_EMS_EngCoolanTemp_267,  CanCclRxHndlCh0_EMS_EngCoolanTemp_267, CanCclRxHndlCh0_EMS_EngCoolanTemp_267   },/*EMS_EngCoolanTemp*/
  { /*  2  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*EMS_EngOperationStatus*/
  { /*  3  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*EMS_EngSpeedError*/
  { /*  4  */    CanCclRxHndlCh0_EMS_BatteryVoltageError_267,  CanCclRxHndlCh0_EMS_BatteryVoltageError_267, CanCclRxHndlCh0_EMS_BatteryVoltageError_267   },/*EMS_BatteryVoltageError*/
  { /*  5  */    CanCclRxHndlCh0_EMS_EngSpeed_267,  CanCclRxHndlCh0_EMS_EngSpeed_267, CanCclRxHndlCh0_EMS_EngSpeed_267   },/*EMS_EngSpeed*/
  { /*  6  */    CanCclRxHndlCh0_EMS_CluthPedalStatus_267,  CanCclRxHndlCh0_EMS_CluthPedalStatus_267, CanCclRxHndlCh0_EMS_CluthPedalStatus_267   },/*EMS_CluthPedalStatus*/
  { /*  7  */    CanCclRxHndlCh0_TCU_DisplayGear_291,  CanCclRxHndlCh0_TCU_DisplayGear_291, CanCclRxHndlCh0_TCU_DisplayGear_291   },/*TCU_DisplayGear*/
  { /*  8  */    CanCclRxHndlCh0_TCU_GearSelectorReq_291,  CanCclRxHndlCh0_TCU_GearSelectorReq_291, CanCclRxHndlCh0_TCU_GearSelectorReq_291   },/*TCU_GearSelectorReq*/
  { /*  9  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*TCU_StGearMode*/
  { /*  10  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*TCU_Status_AliveCounter*/
  { /*  11  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*TCU_Status_Checksum*/
  { /*  12  */    CanCclRxHndlCh0_EMS_TargCruiseSpeed_337,  CanCclRxHndlCh0_EMS_TargCruiseSpeed_337, CanCclRxHndlCh0_EMS_TargCruiseSpeed_337   },/*EMS_TargCruiseSpeed*/
  { /*  13  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*EMS_Rolling_counter_0x151*/
  { /*  14  */    CanCclRxHndlCh0_EMS_CruiseControlStatus_337,  CanCclRxHndlCh0_EMS_CruiseControlStatus_337, CanCclRxHndlCh0_EMS_CruiseControlStatus_337   },/*EMS_CruiseControlStatus*/
  { /*  15  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*EMS_Checksum_0x151*/
  { /*  16  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*PEPS_Remotestartstatus*/
  { /*  17  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*PEPS_RollingCounter_0x1A5*/
  { /*  18  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*PEPS_Checksum_0x1A5*/
  { /*  19  */    CanCclRxHndlCh0_ABS_WhlMilgFrntLe_545,  CanCclRxHndlCh0_ABS_WhlMilgFrntLe_545, CanCclRxHndlCh0_ABS_WhlMilgFrntLe_545   },/*ABS_WhlMilgFrntLe*/
  { /*  20  */    CanCclRxHndlCh0_ABS_WhlMilgFrntRi_545,  CanCclRxHndlCh0_ABS_WhlMilgFrntRi_545, CanCclRxHndlCh0_ABS_WhlMilgFrntRi_545   },/*ABS_WhlMilgFrntRi*/
  { /*  21  */    CanCclRxHndlCh0_ABS_VehSpdLgt_545,  CanCclRxHndlCh0_ABS_VehSpdLgt_545, CanCclRxHndlCh0_ABS_VehSpdLgt_545   },/*ABS_VehSpdLgt*/
  { /*  22  */    CanCclRxHndlCh0_ABS_EbdFlgFlt_545,  CanCclRxHndlCh0_ABS_EbdFlgFlt_545, CanCclRxHndlCh0_ABS_EbdFlgFlt_545   },/*ABS_EbdFlgFlt*/
  { /*  23  */    CanCclRxHndlCh0_ABS_AbsFlgFlt_545,  CanCclRxHndlCh0_ABS_AbsFlgFlt_545, CanCclRxHndlCh0_ABS_AbsFlgFlt_545   },/*ABS_AbsFlgFlt*/
  { /*  24  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*ABS_RollingCounter_0x221*/
  { /*  25  */    CanCclRxHndlCh0_ABS_WhlMilgFrntLeStatus_545,  CanCclRxHndlCh0_ABS_WhlMilgFrntLeStatus_545, CanCclRxHndlCh0_ABS_WhlMilgFrntLeStatus_545   },/*ABS_WhlMilgFrntLeStatus*/
  { /*  26  */    CanCclRxHndlCh0_ABS_WhlMilgFrntRiStatus_545,  CanCclRxHndlCh0_ABS_WhlMilgFrntRiStatus_545, CanCclRxHndlCh0_ABS_WhlMilgFrntRiStatus_545   },/*ABS_WhlMilgFrntRiStatus*/
  { /*  27  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*ABS_Checksum_0x221*/
  { /*  28  */    CanCclRxHndlCh0_ESP_EPBStatus_579,  CanCclRxHndlCh0_ESP_EPBStatus_579, CanCclRxHndlCh0_ESP_EPBStatus_579   },/*ESP_EPBStatus*/
  { /*  29  */    CanCclRxHndlCh0_ESP_EPBErrorStatus_579,  CanCclRxHndlCh0_ESP_EPBErrorStatus_579, CanCclRxHndlCh0_ESP_EPBErrorStatus_579   },/*ESP_EPBErrorStatus*/
  { /*  30  */    CanCclRxHndlCh0_ESP_CdpAvailable_579,  CanCclRxHndlCh0_ESP_CdpAvailable_579, CanCclRxHndlCh0_ESP_CdpAvailable_579   },/*ESP_CdpAvailable*/
  { /*  31  */    CanCclRxHndlCh0_ESP_EPBWarningMessage3_579,  CanCclRxHndlCh0_ESP_EPBWarningMessage3_579, CanCclRxHndlCh0_ESP_EPBWarningMessage3_579   },/*ESP_EPBWarningMessage3*/
  { /*  32  */    CanCclRxHndlCh0_ESP_EPBWarningMessage2_579,  CanCclRxHndlCh0_ESP_EPBWarningMessage2_579, CanCclRxHndlCh0_ESP_EPBWarningMessage2_579   },/*ESP_EPBWarningMessage2*/
  { /*  33  */    CanCclRxHndlCh0_ESP_EPBWarningMessage1_579,  CanCclRxHndlCh0_ESP_EPBWarningMessage1_579, CanCclRxHndlCh0_ESP_EPBWarningMessage1_579   },/*ESP_EPBWarningMessage1*/
  { /*  34  */    CanCclRxHndlCh0_ESP_AVHWarningMessage_579,  CanCclRxHndlCh0_ESP_AVHWarningMessage_579, CanCclRxHndlCh0_ESP_AVHWarningMessage_579   },/*ESP_AVHWarningMessage*/
  { /*  35  */    CanCclRxHndlCh0_ESP_AvhActive_579,  CanCclRxHndlCh0_ESP_AvhActive_579, CanCclRxHndlCh0_ESP_AvhActive_579   },/*ESP_AvhActive*/
  { /*  36  */    CanCclRxHndlCh0_ESP_AvhAvabliable_579,  CanCclRxHndlCh0_ESP_AvhAvabliable_579, CanCclRxHndlCh0_ESP_AvhAvabliable_579   },/*ESP_AvhAvabliable*/
  { /*  37  */    CanCclRxHndlCh0_ESP_VdcFlgFlt_579,  CanCclRxHndlCh0_ESP_VdcFlgFlt_579, CanCclRxHndlCh0_ESP_VdcFlgFlt_579   },/*ESP_VdcFlgFlt*/
  { /*  38  */    CanCclRxHndlCh0_ESP_TcsFlgFlt_579,  CanCclRxHndlCh0_ESP_TcsFlgFlt_579, CanCclRxHndlCh0_ESP_TcsFlgFlt_579   },/*ESP_TcsFlgFlt*/
  { /*  39  */    CanCclRxHndlCh0_ESP_ESPInfoLamp_579,  CanCclRxHndlCh0_ESP_ESPInfoLamp_579, CanCclRxHndlCh0_ESP_ESPInfoLamp_579   },/*ESP_ESPInfoLamp*/
  { /*  40  */    CanCclRxHndlCh0_ESP_EspEnableSts_579,  CanCclRxHndlCh0_ESP_EspEnableSts_579, CanCclRxHndlCh0_ESP_EspEnableSts_579   },/*ESP_EspEnableSts*/
  { /*  41  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*ESP_RollingCounter_0x243*/
  { /*  42  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*ESP_Checksum_0x243*/
  { /*  43  */    CanCclRxHndlCh0_IPM_LaneAssitType_581,  CanCclRxHndlCh0_IPM_LaneAssitType_581, CanCclRxHndlCh0_IPM_LaneAssitType_581   },/*IPM_LaneAssitType*/
  { /*  44  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*IPM_LaneAssit_WarningVibration*/
  { /*  45  */    CanCclRxHndlCh0_IPM_LaneAssit_WarningAudio_581,  CanCclRxHndlCh0_IPM_LaneAssit_WarningAudio_581, CanCclRxHndlCh0_IPM_LaneAssit_WarningAudio_581   },/*IPM_LaneAssit_WarningAudio*/
  { /*  46  */    CanCclRxHndlCh0_IPM_LaneAssit_LeftVisualization_581,  CanCclRxHndlCh0_IPM_LaneAssit_LeftVisualization_581, CanCclRxHndlCh0_IPM_LaneAssit_LeftVisualization_581   },/*IPM_LaneAssit_LeftVisualization*/
  { /*  47  */    CanCclRxHndlCh0_IPM_LaneAssit_RightVisualization_581,  CanCclRxHndlCh0_IPM_LaneAssit_RightVisualization_581, CanCclRxHndlCh0_IPM_LaneAssit_RightVisualization_581   },/*IPM_LaneAssit_RightVisualization*/
  { /*  48  */    CanCclRxHndlCh0_IPM_SLASpdlimitUnits_581,  CanCclRxHndlCh0_IPM_SLASpdlimitUnits_581, CanCclRxHndlCh0_IPM_SLASpdlimitUnits_581   },/*IPM_SLASpdlimitUnits*/
  { /*  49  */    CanCclRxHndlCh0_IPM_SLAState_581,  CanCclRxHndlCh0_IPM_SLAState_581, CanCclRxHndlCh0_IPM_SLAState_581   },/*IPM_SLAState*/
  { /*  50  */    CanCclRxHndlCh0_IPM_LaneAssit_Status_581,  CanCclRxHndlCh0_IPM_LaneAssit_Status_581, CanCclRxHndlCh0_IPM_LaneAssit_Status_581   },/*IPM_LaneAssit_Status*/
  { /*  51  */    CanCclRxHndlCh0_IPM_SLASpdlimit_581,  CanCclRxHndlCh0_IPM_SLASpdlimit_581, CanCclRxHndlCh0_IPM_SLASpdlimit_581   },/*IPM_SLASpdlimit*/
  { /*  52  */    CanCclRxHndlCh0_IPM_HMA_Status_581,  CanCclRxHndlCh0_IPM_HMA_Status_581, CanCclRxHndlCh0_IPM_HMA_Status_581   },/*IPM_HMA_Status*/
  { /*  53  */    CanCclRxHndlCh0_IPM_DDD_IndexStatus_581,  CanCclRxHndlCh0_IPM_DDD_IndexStatus_581, CanCclRxHndlCh0_IPM_DDD_IndexStatus_581   },/*IPM_DDD_IndexStatus*/
  { /*  54  */    CanCclRxHndlCh0_IPM_DDD_WarningLevel_581,  CanCclRxHndlCh0_IPM_DDD_WarningLevel_581, CanCclRxHndlCh0_IPM_DDD_WarningLevel_581   },/*IPM_DDD_WarningLevel*/
  { /*  55  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Rolling_counter_0x245*/
  { /*  56  */    CanCclRxHndlCh0_IPM_LaneAssit_HandsonReq_581,  CanCclRxHndlCh0_IPM_LaneAssit_HandsonReq_581, CanCclRxHndlCh0_IPM_LaneAssit_HandsonReq_581   },/*IPM_LaneAssit_HandsonReq*/
  { /*  57  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Checksum_0x245*/
  { /*  58  */    CanCclRxHndlCh0_vSetDis_582,  CanCclRxHndlCh0_vSetDis_582, CanCclRxHndlCh0_vSetDis_582   },/*vSetDis*/
  { /*  59  */    CanCclRxHndlCh0_MRR_ObjValid_582,  CanCclRxHndlCh0_MRR_ObjValid_582, CanCclRxHndlCh0_MRR_ObjValid_582   },/*MRR_ObjValid*/
  { /*  60  */    CanCclRxHndlCh0_MRR_TauGapSet_582,  CanCclRxHndlCh0_MRR_TauGapSet_582, CanCclRxHndlCh0_MRR_TauGapSet_582   },/*MRR_TauGapSet*/
  { /*  61  */    CanCclRxHndlCh0_MRR_dxTarObj_582,  CanCclRxHndlCh0_MRR_dxTarObj_582, CanCclRxHndlCh0_MRR_dxTarObj_582   },/*MRR_dxTarObj*/
  { /*  62  */    CanCclRxHndlCh0_ACCHMI_Mode_582,  CanCclRxHndlCh0_ACCHMI_Mode_582, CanCclRxHndlCh0_ACCHMI_Mode_582   },/*ACCHMI_Mode*/
  { /*  63  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*ACCFailInfo*/
  { /*  64  */    CanCclRxHndlCh0_MRR_TakeOverReq_582,  CanCclRxHndlCh0_MRR_TakeOverReq_582, CanCclRxHndlCh0_MRR_TakeOverReq_582   },/*MRR_TakeOverReq*/
  { /*  65  */    CanCclRxHndlCh0_MRR_AEB_STATE_582,  CanCclRxHndlCh0_MRR_AEB_STATE_582, CanCclRxHndlCh0_MRR_AEB_STATE_582   },/*MRR_AEB_STATE*/
  { /*  66  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*ACC_Startstop_infor*/
  { /*  67  */    CanCclRxHndlCh0_MRR_PCW_preWarning_582,  CanCclRxHndlCh0_MRR_PCW_preWarning_582, CanCclRxHndlCh0_MRR_PCW_preWarning_582   },/*MRR_PCW_preWarning*/
  { /*  68  */    CanCclRxHndlCh0_MRR_PCW_latentWarning_582,  CanCclRxHndlCh0_MRR_PCW_latentWarning_582, CanCclRxHndlCh0_MRR_PCW_latentWarning_582   },/*MRR_PCW_latentWarning*/
  { /*  69  */    CanCclRxHndlCh0_FCW_preWarning_582,  CanCclRxHndlCh0_FCW_preWarning_582, CanCclRxHndlCh0_FCW_preWarning_582   },/*FCW_preWarning*/
  { /*  70  */    CanCclRxHndlCh0_FCW_latentWarning_582,  CanCclRxHndlCh0_FCW_latentWarning_582, CanCclRxHndlCh0_FCW_latentWarning_582   },/*FCW_latentWarning*/
  { /*  71  */    CanCclRxHndlCh0_MRR_PCW_STATE_582,  CanCclRxHndlCh0_MRR_PCW_STATE_582, CanCclRxHndlCh0_MRR_PCW_STATE_582   },/*MRR_PCW_STATE*/
  { /*  72  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Obj_Speed*/
  { /*  73  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Rolling_counter_0x246*/
  { /*  74  */    CanCclRxHndlCh0_Textinfo_582,  CanCclRxHndlCh0_Textinfo_582, CanCclRxHndlCh0_Textinfo_582   },/*Textinfo*/
  { /*  75  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Checksum_0x246*/
  { /*  76  */    CanCclRxHndlCh0_EPS_S_WarningLampYellow_608,  CanCclRxHndlCh0_EPS_S_WarningLampYellow_608, CanCclRxHndlCh0_EPS_S_WarningLampYellow_608   },/*EPS_S_WarningLampYellow*/
  { /*  77  */    CanCclRxHndlCh0_EPS_S_SafeLampRed_608,  CanCclRxHndlCh0_EPS_S_SafeLampRed_608, CanCclRxHndlCh0_EPS_S_SafeLampRed_608   },/*EPS_S_SafeLampRed*/
  { /*  78  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Rolling_counter_0x260*/
  { /*  79  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Checksum_0x260*/
  { /*  80  */    CanCclRxHndlCh0_PAM_ChimeCmdSoundStatus_625,  CanCclRxHndlCh0_PAM_ChimeCmdSoundStatus_625, CanCclRxHndlCh0_PAM_ChimeCmdSoundStatus_625   },/*PAM_ChimeCmdSoundStatus*/
  { /*  81  */    CanCclRxHndlCh0_PAM_RearSysTemWorkSts_625,  CanCclRxHndlCh0_PAM_RearSysTemWorkSts_625, CanCclRxHndlCh0_PAM_RearSysTemWorkSts_625   },/*PAM_RearSysTemWorkSts*/
  { /*  82  */    CanCclRxHndlCh0_PAM_FrontSysTemWorkSts_625,  CanCclRxHndlCh0_PAM_FrontSysTemWorkSts_625, CanCclRxHndlCh0_PAM_FrontSysTemWorkSts_625   },/*PAM_FrontSysTemWorkSts*/
  { /*  83  */    CanCclRxHndlCh0_PAM_ChimeCmdSoundCadence_625,  CanCclRxHndlCh0_PAM_ChimeCmdSoundCadence_625, CanCclRxHndlCh0_PAM_ChimeCmdSoundCadence_625   },/*PAM_ChimeCmdSoundCadence*/
  { /*  84  */    CanCclRxHndlCh0_SOD_LCA_warningReqRight_629,  CanCclRxHndlCh0_SOD_LCA_warningReqRight_629, CanCclRxHndlCh0_SOD_LCA_warningReqRight_629   },/*SOD_LCA_warningReqRight*/
  { /*  85  */    CanCclRxHndlCh0_SOD_DOA_warningReqRightRear_629,  CanCclRxHndlCh0_SOD_DOA_warningReqRightRear_629, CanCclRxHndlCh0_SOD_DOA_warningReqRightRear_629   },/*SOD_DOA_warningReqRightRear*/
  { /*  86  */    CanCclRxHndlCh0_SOD_DOA_warningReqRightFront_629,  CanCclRxHndlCh0_SOD_DOA_warningReqRightFront_629, CanCclRxHndlCh0_SOD_DOA_warningReqRightFront_629   },/*SOD_DOA_warningReqRightFront*/
  { /*  87  */    CanCclRxHndlCh0_SOD_CTA_warningReqRight_629,  CanCclRxHndlCh0_SOD_CTA_warningReqRight_629, CanCclRxHndlCh0_SOD_CTA_warningReqRight_629   },/*SOD_CTA_warningReqRight*/
  { /*  88  */    CanCclRxHndlCh0_SOD_BLIS_display_629,  CanCclRxHndlCh0_SOD_BLIS_display_629, CanCclRxHndlCh0_SOD_BLIS_display_629   },/*SOD_BLIS_display*/
  { /*  89  */    CanCclRxHndlCh0_SOD_LCA_warningReqLeft_629,  CanCclRxHndlCh0_SOD_LCA_warningReqLeft_629, CanCclRxHndlCh0_SOD_LCA_warningReqLeft_629   },/*SOD_LCA_warningReqLeft*/
  { /*  90  */    CanCclRxHndlCh0_SOD_DOA_warningReqLeftRear_629,  CanCclRxHndlCh0_SOD_DOA_warningReqLeftRear_629, CanCclRxHndlCh0_SOD_DOA_warningReqLeftRear_629   },/*SOD_DOA_warningReqLeftRear*/
  { /*  91  */    CanCclRxHndlCh0_SOD_DOA_warningReqLeftFront_629,  CanCclRxHndlCh0_SOD_DOA_warningReqLeftFront_629, CanCclRxHndlCh0_SOD_DOA_warningReqLeftFront_629   },/*SOD_DOA_warningReqLeftFront*/
  { /*  92  */    CanCclRxHndlCh0_SOD_CTA_warningReqLeft_629,  CanCclRxHndlCh0_SOD_CTA_warningReqLeft_629, CanCclRxHndlCh0_SOD_CTA_warningReqLeft_629   },/*SOD_CTA_warningReqLeft*/
  { /*  93  */    CanCclRxHndlCh0_DOA_OFFTelltale_629,  CanCclRxHndlCh0_DOA_OFFTelltale_629, CanCclRxHndlCh0_DOA_OFFTelltale_629   },/*DOA_OFFTelltale*/
  { /*  94  */    CanCclRxHndlCh0_CTA_OFFTelltale_629,  CanCclRxHndlCh0_CTA_OFFTelltale_629, CanCclRxHndlCh0_CTA_OFFTelltale_629   },/*CTA_OFFTelltale*/
  { /*  95  */    CanCclRxHndlCh0_SODLCA_OFFTelltale_629,  CanCclRxHndlCh0_SODLCA_OFFTelltale_629, CanCclRxHndlCh0_SODLCA_OFFTelltale_629   },/*SODLCA_OFFTelltale*/
  { /*  96  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Rolling_counter_0x275*/
  { /*  97  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Checksum_0x275*/
  { /*  98  */    CanCclRxHndlCh0_BCM_DoorStsRearRi_784,  CanCclRxHndlCh0_BCM_DoorStsRearRi_784, CanCclRxHndlCh0_BCM_DoorStsRearRi_784   },/*BCM_DoorStsRearRi*/
  { /*  99  */    CanCclRxHndlCh0_BCM_DoorStsRearLe_784,  CanCclRxHndlCh0_BCM_DoorStsRearLe_784, CanCclRxHndlCh0_BCM_DoorStsRearLe_784   },/*BCM_DoorStsRearLe*/
  { /*  100  */    CanCclRxHndlCh0_BCM_DoorStsFrntRi_784,  CanCclRxHndlCh0_BCM_DoorStsFrntRi_784, CanCclRxHndlCh0_BCM_DoorStsFrntRi_784   },/*BCM_DoorStsFrntRi*/
  { /*  101  */    CanCclRxHndlCh0_BCM_DoorStsFrntLe_784,  CanCclRxHndlCh0_BCM_DoorStsFrntLe_784, CanCclRxHndlCh0_BCM_DoorStsFrntLe_784   },/*BCM_DoorStsFrntLe*/
  { /*  102  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_DoorLockStsRearRi*/
  { /*  103  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_DoorLockStsRearLe*/
  { /*  104  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_DoorLockStsFrntRi*/
  { /*  105  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_DoorLockStsFrntLe*/
  { /*  106  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_AntiTheftSts*/
  { /*  107  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_LockUnlockFeedback*/
  { /*  108  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_DoorLockStsTrunk*/
  { /*  109  */    CanCclRxHndlCh0_BCM_DoorStsTrunk_784,  CanCclRxHndlCh0_BCM_DoorStsTrunk_784, CanCclRxHndlCh0_BCM_DoorStsTrunk_784   },/*BCM_DoorStsTrunk*/
  { /*  110  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_TrunkDoorPreUnlockSts*/
  { /*  111  */    CanCclRxHndlCh0_BCM_EnginehoodSts_784,  CanCclRxHndlCh0_BCM_EnginehoodSts_784, CanCclRxHndlCh0_BCM_EnginehoodSts_784   },/*BCM_EnginehoodSts*/
  { /*  112  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_Rolling_counter_0x310*/
  { /*  113  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_Checksum_0x310*/
  { /*  114  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_OutsideTempDig*/
  { /*  115  */    CanCclRxHndlCh0_BCM_Mechanical_Hand_Brake_787,  CanCclRxHndlCh0_BCM_Mechanical_Hand_Brake_787, CanCclRxHndlCh0_BCM_Mechanical_Hand_Brake_787   },/*BCM_Mechanical_Hand_Brake*/
  { /*  116  */    CanCclRxHndlCh0_BCM_BrakeFluidSts_787,  CanCclRxHndlCh0_BCM_BrakeFluidSts_787, CanCclRxHndlCh0_BCM_BrakeFluidSts_787   },/*BCM_BrakeFluidSts*/
  { /*  117  */    CanCclRxHndlCh0_BCM_OutsideTemp_787,  CanCclRxHndlCh0_BCM_OutsideTemp_787, CanCclRxHndlCh0_BCM_OutsideTemp_787   },/*BCM_OutsideTemp*/
  { /*  118  */    CanCclRxHndlCh0_BCM_PowerMode_787,  CanCclRxHndlCh0_BCM_PowerMode_787, CanCclRxHndlCh0_BCM_PowerMode_787   },/*BCM_PowerMode*/
  { /*  119  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_Rollingcounter_0x313*/
  { /*  120  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_Checksum_0x313*/
  { /*  121  */    CanCclRxHndlCh0_BCM_BGLLumLvlSettingStatus_789,  CanCclRxHndlCh0_BCM_BGLLumLvlSettingStatus_789, CanCclRxHndlCh0_BCM_BGLLumLvlSettingStatus_789   },/*BCM_BGLLumLvlSettingStatus*/
  { /*  122  */    CanCclRxHndlCh0_BCM_FrntFogLmpSts_792,  CanCclRxHndlCh0_BCM_FrntFogLmpSts_792, CanCclRxHndlCh0_BCM_FrntFogLmpSts_792   },/*BCM_FrntFogLmpSts*/
  { /*  123  */    CanCclRxHndlCh0_BCM_PosLmpSts_792,  CanCclRxHndlCh0_BCM_PosLmpSts_792, CanCclRxHndlCh0_BCM_PosLmpSts_792   },/*BCM_PosLmpSts*/
  { /*  124  */    CanCclRxHndlCh0_BCM_LowBeamSts_792,  CanCclRxHndlCh0_BCM_LowBeamSts_792, CanCclRxHndlCh0_BCM_LowBeamSts_792   },/*BCM_LowBeamSts*/
  { /*  125  */    CanCclRxHndlCh0_BCM_HighBeamSts_792,  CanCclRxHndlCh0_BCM_HighBeamSts_792, CanCclRxHndlCh0_BCM_HighBeamSts_792   },/*BCM_HighBeamSts*/
  { /*  126  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_reversinglightSts*/
  { /*  127  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_DayRunningLampSts*/
  { /*  128  */    CanCclRxHndlCh0_BCM_TurnIndicatorSts_792,  CanCclRxHndlCh0_BCM_TurnIndicatorSts_792, CanCclRxHndlCh0_BCM_TurnIndicatorSts_792   },/*BCM_TurnIndicatorSts*/
  { /*  129  */    CanCclRxHndlCh0_BCM_RearFogLmpSts_792,  CanCclRxHndlCh0_BCM_RearFogLmpSts_792, CanCclRxHndlCh0_BCM_RearFogLmpSts_792   },/*BCM_RearFogLmpSts*/
  { /*  130  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_Highmountedstoplamp*/
  { /*  131  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_BrakelightSts*/
  { /*  132  */    CanCclRxHndlCh0_BCM_BackHomeSts_792,  CanCclRxHndlCh0_BCM_BackHomeSts_792, CanCclRxHndlCh0_BCM_BackHomeSts_792   },/*BCM_BackHomeSts*/
  { /*  133  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_AutolampSts*/
  { /*  134  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_readinglampSts*/
  { /*  135  */    CanCclRxHndlCh0_BCM_DayRunningLampDig_792,  CanCclRxHndlCh0_BCM_DayRunningLampDig_792, CanCclRxHndlCh0_BCM_DayRunningLampDig_792   },/*BCM_DayRunningLampDig*/
  { /*  136  */    CanCclRxHndlCh0_BCM_BrakelightDig_792,  CanCclRxHndlCh0_BCM_BrakelightDig_792, CanCclRxHndlCh0_BCM_BrakelightDig_792   },/*BCM_BrakelightDig*/
  { /*  137  */    CanCclRxHndlCh0_BCM_reversinglightDig_792,  CanCclRxHndlCh0_BCM_reversinglightDig_792, CanCclRxHndlCh0_BCM_reversinglightDig_792   },/*BCM_reversinglightDig*/
  { /*  138  */    CanCclRxHndlCh0_BCM_TurnIndicatorDig_792,  CanCclRxHndlCh0_BCM_TurnIndicatorDig_792, CanCclRxHndlCh0_BCM_TurnIndicatorDig_792   },/*BCM_TurnIndicatorDig*/
  { /*  139  */    CanCclRxHndlCh0_BCM_RearFogLmpDig_792,  CanCclRxHndlCh0_BCM_RearFogLmpDig_792, CanCclRxHndlCh0_BCM_RearFogLmpDig_792   },/*BCM_RearFogLmpDig*/
  { /*  140  */    CanCclRxHndlCh0_BCM_PosLmpDig_792,  CanCclRxHndlCh0_BCM_PosLmpDig_792, CanCclRxHndlCh0_BCM_PosLmpDig_792   },/*BCM_PosLmpDig*/
  { /*  141  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_Rollingcounter_0x318*/
  { /*  142  */    CanCclRxHndlCh0_BCM_ImmoSts_792,  CanCclRxHndlCh0_BCM_ImmoSts_792, CanCclRxHndlCh0_BCM_ImmoSts_792   },/*BCM_ImmoSts*/
  { /*  143  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_Checksum_0x318*/
  { /*  144  */    CanCclRxHndlCh0_SWM_Menu_Right_794,  CanCclRxHndlCh0_SWM_Menu_Right_794, CanCclRxHndlCh0_SWM_Menu_Right_794   },/*SWM_Menu_Right*/
  { /*  145  */    CanCclRxHndlCh0_SWM_Menu_Left_794,  CanCclRxHndlCh0_SWM_Menu_Left_794, CanCclRxHndlCh0_SWM_Menu_Left_794   },/*SWM_Menu_Left*/
  { /*  146  */    CanCclRxHndlCh0_SWM_Menu_Down_794,  CanCclRxHndlCh0_SWM_Menu_Down_794, CanCclRxHndlCh0_SWM_Menu_Down_794   },/*SWM_Menu_Down*/
  { /*  147  */    CanCclRxHndlCh0_SWM_Menu_Up_794,  CanCclRxHndlCh0_SWM_Menu_Up_794, CanCclRxHndlCh0_SWM_Menu_Up_794   },/*SWM_Menu_Up*/
  { /*  148  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Rolling_counter_0x31A*/
  { /*  149  */    CanCclRxHndlCh0_SWM_Menu_Confirm_794,  CanCclRxHndlCh0_SWM_Menu_Confirm_794, CanCclRxHndlCh0_SWM_Menu_Confirm_794   },/*SWM_Menu_Confirm*/
  { /*  150  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Checksum_0x31A*/
  { /*  151  */    CanCclRxHndlCh0_PEPS_PowerMode_805,  CanCclRxHndlCh0_PEPS_PowerMode_805, CanCclRxHndlCh0_PEPS_PowerMode_805   },/*PEPS_PowerMode*/
  { /*  152  */    CanCclRxHndlCh0_PEPS_IMMO_MatchStatus_805,  CanCclRxHndlCh0_PEPS_IMMO_MatchStatus_805, CanCclRxHndlCh0_PEPS_IMMO_MatchStatus_805   },/*PEPS_IMMO_MatchStatus*/
  { /*  153  */    CanCclRxHndlCh0_PEPS_SystemFailStatus_805,  CanCclRxHndlCh0_PEPS_SystemFailStatus_805, CanCclRxHndlCh0_PEPS_SystemFailStatus_805   },/*PEPS_SystemFailStatus*/
  { /*  154  */    CanCclRxHndlCh0_PEPS_ESCL_FailStatus_805,  CanCclRxHndlCh0_PEPS_ESCL_FailStatus_805, CanCclRxHndlCh0_PEPS_ESCL_FailStatus_805   },/*PEPS_ESCL_FailStatus*/
  { /*  155  */    CanCclRxHndlCh0_PEPS_SSB_FailStatus_805,  CanCclRxHndlCh0_PEPS_SSB_FailStatus_805, CanCclRxHndlCh0_PEPS_SSB_FailStatus_805   },/*PEPS_SSB_FailStatus*/
  { /*  156  */    CanCclRxHndlCh0_PEPS_ClutchIndicationWarning_805,  CanCclRxHndlCh0_PEPS_ClutchIndicationWarning_805, CanCclRxHndlCh0_PEPS_ClutchIndicationWarning_805   },/*PEPS_ClutchIndicationWarning*/
  { /*  157  */    CanCclRxHndlCh0_PEPS_BrakeIndicationWarning_805,  CanCclRxHndlCh0_PEPS_BrakeIndicationWarning_805, CanCclRxHndlCh0_PEPS_BrakeIndicationWarning_805   },/*PEPS_BrakeIndicationWarning*/
  { /*  158  */    CanCclRxHndlCh0_PEPS_NotPNWarning_805,  CanCclRxHndlCh0_PEPS_NotPNWarning_805, CanCclRxHndlCh0_PEPS_NotPNWarning_805   },/*PEPS_NotPNWarning*/
  { /*  159  */    CanCclRxHndlCh0_PEPS_NotNWarning_805,  CanCclRxHndlCh0_PEPS_NotNWarning_805, CanCclRxHndlCh0_PEPS_NotNWarning_805   },/*PEPS_NotNWarning*/
  { /*  160  */    CanCclRxHndlCh0_PEPS_DoorOpenNotOFFWarning_805,  CanCclRxHndlCh0_PEPS_DoorOpenNotOFFWarning_805, CanCclRxHndlCh0_PEPS_DoorOpenNotOFFWarning_805   },/*PEPS_DoorOpenNotOFFWarning*/
  { /*  161  */    CanCclRxHndlCh0_PEPS_DoorOpenWarning_805,  CanCclRxHndlCh0_PEPS_DoorOpenWarning_805, CanCclRxHndlCh0_PEPS_DoorOpenWarning_805   },/*PEPS_DoorOpenWarning*/
  { /*  162  */    CanCclRxHndlCh0_PEPS_KeyBatteryWarning_805,  CanCclRxHndlCh0_PEPS_KeyBatteryWarning_805, CanCclRxHndlCh0_PEPS_KeyBatteryWarning_805   },/*PEPS_KeyBatteryWarning*/
  { /*  163  */    CanCclRxHndlCh0_PEPS_KeyOutWarning1_805,  CanCclRxHndlCh0_PEPS_KeyOutWarning1_805, CanCclRxHndlCh0_PEPS_KeyOutWarning1_805   },/*PEPS_KeyOutWarning1*/
  { /*  164  */    CanCclRxHndlCh0_PEPS_NoKeyFoundWarning_805,  CanCclRxHndlCh0_PEPS_NoKeyFoundWarning_805, CanCclRxHndlCh0_PEPS_NoKeyFoundWarning_805   },/*PEPS_NoKeyFoundWarning*/
  { /*  165  */    CanCclRxHndlCh0_PEPS_KeyReminderWarning_805,  CanCclRxHndlCh0_PEPS_KeyReminderWarning_805, CanCclRxHndlCh0_PEPS_KeyReminderWarning_805   },/*PEPS_KeyReminderWarning*/
  { /*  166  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*PEPS_RollingCounter_0x325*/
  { /*  167  */    CanCclRxHndlCh0_PEPS_RemainingNumSts_805,  CanCclRxHndlCh0_PEPS_RemainingNumSts_805, CanCclRxHndlCh0_PEPS_RemainingNumSts_805   },/*PEPS_RemainingNumSts*/
  { /*  168  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*PEPS_Checksum_0x325*/
  { /*  169  */    CanCclRxHndlCh0_RF_KeyLowBatteryWarning_806,  CanCclRxHndlCh0_RF_KeyLowBatteryWarning_806, CanCclRxHndlCh0_RF_KeyLowBatteryWarning_806   },/*RF_KeyLowBatteryWarning*/
  { /*  170  */    CanCclRxHndlCh0_SRS_SBR_PAMsenger_848,  CanCclRxHndlCh0_SRS_SBR_PAMsenger_848, CanCclRxHndlCh0_SRS_SBR_PAMsenger_848   },/*SRS_SBR_PAMsenger*/
  { /*  171  */    CanCclRxHndlCh0_SRS_SBR_Driver_848,  CanCclRxHndlCh0_SRS_SBR_Driver_848, CanCclRxHndlCh0_SRS_SBR_Driver_848   },/*SRS_SBR_Driver*/
  { /*  172  */    CanCclRxHndlCh0_SRS_AirBagFailSts_848,  CanCclRxHndlCh0_SRS_AirBagFailSts_848, CanCclRxHndlCh0_SRS_AirBagFailSts_848   },/*SRS_AirBagFailSts*/
  { /*  173  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*SRS_SBR_SecondRowRight*/
  { /*  174  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*SRS_SBR_SecondRowMid*/
  { /*  175  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*SRS_SBR_SecondRowLeft*/
  { /*  176  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Rolling_counter_0x350*/
  { /*  177  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Checksum_0x350*/
  { /*  178  */    CanCclRxHndlCh0_TPMS_LF_Pressure_865,  CanCclRxHndlCh0_TPMS_LF_Pressure_865, CanCclRxHndlCh0_TPMS_LF_Pressure_865   },/*TPMS_LF_Pressure*/
  { /*  179  */    CanCclRxHndlCh0_TPMS_RF_Pressure_865,  CanCclRxHndlCh0_TPMS_RF_Pressure_865, CanCclRxHndlCh0_TPMS_RF_Pressure_865   },/*TPMS_RF_Pressure*/
  { /*  180  */    CanCclRxHndlCh0_TPMS_RR_Pressure_865,  CanCclRxHndlCh0_TPMS_RR_Pressure_865, CanCclRxHndlCh0_TPMS_RR_Pressure_865   },/*TPMS_RR_Pressure*/
  { /*  181  */    CanCclRxHndlCh0_TPMS_LR_Pressure_865,  CanCclRxHndlCh0_TPMS_LR_Pressure_865, CanCclRxHndlCh0_TPMS_LR_Pressure_865   },/*TPMS_LR_Pressure*/
  { /*  182  */    CanCclRxHndlCh0_TPMS_Tire_Temperature_865,  CanCclRxHndlCh0_TPMS_Tire_Temperature_865, CanCclRxHndlCh0_TPMS_Tire_Temperature_865   },/*TPMS_Tire_Temperature*/
  { /*  183  */    CanCclRxHndlCh0_TPMS_LF_Pressure_Warning_865,  CanCclRxHndlCh0_TPMS_LF_Pressure_Warning_865, CanCclRxHndlCh0_TPMS_LF_Pressure_Warning_865   },/*TPMS_LF_Pressure_Warning*/
  { /*  184  */    CanCclRxHndlCh0_TPMS_RF_Pressure_Warning_865,  CanCclRxHndlCh0_TPMS_RF_Pressure_Warning_865, CanCclRxHndlCh0_TPMS_RF_Pressure_Warning_865   },/*TPMS_RF_Pressure_Warning*/
  { /*  185  */    CanCclRxHndlCh0_TPMS_System_Status_865,  CanCclRxHndlCh0_TPMS_System_Status_865, CanCclRxHndlCh0_TPMS_System_Status_865   },/*TPMS_System_Status*/
  { /*  186  */    CanCclRxHndlCh0_TPMS_RR_Pressure_Warning_865,  CanCclRxHndlCh0_TPMS_RR_Pressure_Warning_865, CanCclRxHndlCh0_TPMS_RR_Pressure_Warning_865   },/*TPMS_RR_Pressure_Warning*/
  { /*  187  */    CanCclRxHndlCh0_TPMS_LR_Pressure_Warning_865,  CanCclRxHndlCh0_TPMS_LR_Pressure_Warning_865, CanCclRxHndlCh0_TPMS_LR_Pressure_Warning_865   },/*TPMS_LR_Pressure_Warning*/
  { /*  188  */    CanCclRxHndlCh0_Temperature_Warning_865,  CanCclRxHndlCh0_Temperature_Warning_865, CanCclRxHndlCh0_Temperature_Warning_865   },/*Temperature_Warning*/
  { /*  189  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*resrved*/
  { /*  190  */    CanCclRxHndlCh0_Tire_Position_865,  CanCclRxHndlCh0_Tire_Position_865, CanCclRxHndlCh0_Tire_Position_865   },/*Tire_Position*/
  { /*  191  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*TPMS_Lamp_Status*/
  { /*  192  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*RRI_Pressure*/
  { /*  193  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*LRI_Pressure*/
  { /*  194  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*RRI_Pressure_Warning*/
  { /*  195  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*LRI_Pressure_Warning*/
  { /*  196  */    CanCclRxHndlCh0_EMS_Fuelcounter_870,  CanCclRxHndlCh0_EMS_Fuelcounter_870, CanCclRxHndlCh0_EMS_Fuelcounter_870   },/*EMS_Fuelcounter*/
  { /*  197  */    CanCclRxHndlCh0_EMS_RemindShiftGear_MT_870,  CanCclRxHndlCh0_EMS_RemindShiftGear_MT_870, CanCclRxHndlCh0_EMS_RemindShiftGear_MT_870   },/*EMS_RemindShiftGear_MT*/
  { /*  198  */    CanCclRxHndlCh0_EMS_MIL_870,  CanCclRxHndlCh0_EMS_MIL_870, CanCclRxHndlCh0_EMS_MIL_870   },/*EMS_MIL*/
  { /*  199  */    CanCclRxHndlCh0_EMS_SVS_870,  CanCclRxHndlCh0_EMS_SVS_870, CanCclRxHndlCh0_EMS_SVS_870   },/*EMS_SVS*/
  { /*  200  */    CanCclRxHndlCh0_EMS_StartStopStatus_870,  CanCclRxHndlCh0_EMS_StartStopStatus_870, CanCclRxHndlCh0_EMS_StartStopStatus_870   },/*EMS_StartStopStatus*/
  { /*  201  */    CanCclRxHndlCh0_EMS_OBDTxt_870,  CanCclRxHndlCh0_EMS_OBDTxt_870, CanCclRxHndlCh0_EMS_OBDTxt_870   },/*EMS_OBDTxt*/
  { /*  202  */    CanCclRxHndlCh0_EMS_TroqLimtHour_872,  CanCclRxHndlCh0_EMS_TroqLimtHour_872, CanCclRxHndlCh0_EMS_TroqLimtHour_872   },/*EMS_TroqLimtHour*/
  { /*  203  */    CanCclRxHndlCh0_EMS_UreaQty_872,  CanCclRxHndlCh0_EMS_UreaQty_872, CanCclRxHndlCh0_EMS_UreaQty_872   },/*EMS_UreaQty*/
  { /*  204  */    CanCclRxHndlCh0_EMS_TroqLimtMin_872,  CanCclRxHndlCh0_EMS_TroqLimtMin_872, CanCclRxHndlCh0_EMS_TroqLimtMin_872   },/*EMS_TroqLimtMin*/
  { /*  205  */    CanCclRxHndlCh0_EMS_UreaLvl_872,  CanCclRxHndlCh0_EMS_UreaLvl_872, CanCclRxHndlCh0_EMS_UreaLvl_872   },/*EMS_UreaLvl*/
  { /*  206  */    CanCclRxHndlCh0_EMS_DieslPrtcWarn_D_Rq_872,  CanCclRxHndlCh0_EMS_DieslPrtcWarn_D_Rq_872, CanCclRxHndlCh0_EMS_DieslPrtcWarn_D_Rq_872   },/*EMS_DieslPrtcWarn_D_Rq*/
  { /*  207  */    CanCclRxHndlCh0_DCM_GlowPlugLmpReq_872,  CanCclRxHndlCh0_DCM_GlowPlugLmpReq_872, CanCclRxHndlCh0_DCM_GlowPlugLmpReq_872   },/*DCM_GlowPlugLmpReq*/
  { /*  208  */    CanCclRxHndlCh0_DCM_WIFFullLmpReq_872,  CanCclRxHndlCh0_DCM_WIFFullLmpReq_872, CanCclRxHndlCh0_DCM_WIFFullLmpReq_872   },/*DCM_WIFFullLmpReq*/
  { /*  209  */    CanCclRxHndlCh0_EMS_SpeedLimtHour_872,  CanCclRxHndlCh0_EMS_SpeedLimtHour_872, CanCclRxHndlCh0_EMS_SpeedLimtHour_872   },/*EMS_SpeedLimtHour*/
  { /*  210  */    CanCclRxHndlCh0_EMS_SpeedLimtMin_872,  CanCclRxHndlCh0_EMS_SpeedLimtMin_872, CanCclRxHndlCh0_EMS_SpeedLimtMin_872   },/*EMS_SpeedLimtMin*/
  { /*  211  */    CanCclRxHndlCh0_EMS_UreaSysWarning_872,  CanCclRxHndlCh0_EMS_UreaSysWarning_872, CanCclRxHndlCh0_EMS_UreaSysWarning_872   },/*EMS_UreaSysWarning*/
  { /*  212  */    CanCclRxHndlCh0_EMS_UreaTxt_872,  CanCclRxHndlCh0_EMS_UreaTxt_872, CanCclRxHndlCh0_EMS_UreaTxt_872   },/*EMS_UreaTxt*/
  { /*  213  */    CanCclRxHndlCh0_EMS_ManRgenTxt_D_RqDsply_872,  CanCclRxHndlCh0_EMS_ManRgenTxt_D_RqDsply_872, CanCclRxHndlCh0_EMS_ManRgenTxt_D_RqDsply_872   },/*EMS_ManRgenTxt_D_RqDsply*/
  { /*  214  */    CanCclRxHndlCh0_EMS_UreaWarn_872,  CanCclRxHndlCh0_EMS_UreaWarn_872, CanCclRxHndlCh0_EMS_UreaWarn_872   },/*EMS_UreaWarn*/
  { /*  215  */    CanCclRxHndlCh0_EMS_Soot_Pc_Dsply_872,  CanCclRxHndlCh0_EMS_Soot_Pc_Dsply_872, CanCclRxHndlCh0_EMS_Soot_Pc_Dsply_872   },/*EMS_Soot_Pc_Dsply*/
  { /*  216  */    CanCclRxHndlCh0_MP5_BSDSoundSwitchSts_898,  CanCclRxHndlCh0_MP5_BSDSoundSwitchSts_898, CanCclRxHndlCh0_MP5_BSDSoundSwitchSts_898   },/*MP5_BSDSoundSwitchSts*/
  { /*  217  */    CanCclRxHndlCh0_MP5_NavICON_900,  CanCclRxHndlCh0_MP5_NavICON_900, CanCclRxHndlCh0_MP5_NavICON_900   },/*MP5_NavICON*/
  { /*  218  */    CanCclRxHndlCh0_MP5_ROUTE_REMAIN_DIS_900,  CanCclRxHndlCh0_MP5_ROUTE_REMAIN_DIS_900, CanCclRxHndlCh0_MP5_ROUTE_REMAIN_DIS_900   },/*MP5_ROUTE_REMAIN_DIS*/
  { /*  219  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*MP5_ALModeSetting*/
  { /*  220  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*MP5_ALStartAroundSetting*/
  { /*  221  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*Trans_Limphome*/
  { /*  222  */    CanCclRxHndlCh0_TrnSrvcRqd_B_Rq_917,  CanCclRxHndlCh0_TrnSrvcRqd_B_Rq_917, CanCclRxHndlCh0_TrnSrvcRqd_B_Rq_917   },/*TrnSrvcRqd_B_Rq*/
  { /*  223  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*TCU_Gearbox_Gonglimp*/
  { /*  224  */    CanCclRxHndlCh0_TCU_Driverwarning_917,  CanCclRxHndlCh0_TCU_Driverwarning_917, CanCclRxHndlCh0_TCU_Driverwarning_917   },/*TCU_Driverwarning*/
  { /*  225  */    CanCclRxHndlCh0_TCU_DrvShiftAllwCondRmd_917,  CanCclRxHndlCh0_TCU_DrvShiftAllwCondRmd_917, CanCclRxHndlCh0_TCU_DrvShiftAllwCondRmd_917   },/*TCU_DrvShiftAllwCondRmd*/
  { /*  226  */    CanCclRxHndlCh0_MP5_AlarmStatus_1328,  CanCclRxHndlCh0_MP5_AlarmStatus_1328, CanCclRxHndlCh0_MP5_AlarmStatus_1328   },/*MP5_AlarmStatus*/
  { /*  227  */    CanCclRxHndlCh0_MP5_AlarmOrNot_1328,  CanCclRxHndlCh0_MP5_AlarmOrNot_1328, CanCclRxHndlCh0_MP5_AlarmOrNot_1328   },/*MP5_AlarmOrNot*/
  { /*  228  */    CanCclRxHndlCh0_TBOX_Year_1410,  CanCclRxHndlCh0_TBOX_Year_1410, CanCclRxHndlCh0_TBOX_Year_1410   },/*TBOX_Year*/
  { /*  229  */    CanCclRxHndlCh0_TBOX_Month_1410,  CanCclRxHndlCh0_TBOX_Month_1410, CanCclRxHndlCh0_TBOX_Month_1410   },/*TBOX_Month*/
  { /*  230  */    CanCclRxHndlCh0_TBOX_Date_1410,  CanCclRxHndlCh0_TBOX_Date_1410, CanCclRxHndlCh0_TBOX_Date_1410   },/*TBOX_Date*/
  { /*  231  */    CanCclRxHndlCh0_TBOX_Hour_1410,  CanCclRxHndlCh0_TBOX_Hour_1410, CanCclRxHndlCh0_TBOX_Hour_1410   },/*TBOX_Hour*/
  { /*  232  */    CanCclRxHndlCh0_TBOX_Minute_1410,  CanCclRxHndlCh0_TBOX_Minute_1410, CanCclRxHndlCh0_TBOX_Minute_1410   },/*TBOX_Minute*/
  { /*  233  */    CanCclRxHndlCh0_TBOX_Second_1410,  CanCclRxHndlCh0_TBOX_Second_1410, CanCclRxHndlCh0_TBOX_Second_1410   },/*TBOX_Second*/
  { /*  234  */    CanCclRxHndlCh0_TBOX_RealTime_UB_Copy_1_1410,  CanCclRxHndlCh0_TBOX_RealTime_UB_Copy_1_1410, CanCclRxHndlCh0_TBOX_RealTime_UB_Copy_1_1410   },/*TBOX_RealTime_UB_Copy_1*/
  { /*  235  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*TBOX_RealTime_UB*/
  { /*  236  */    CanCclRxHndlCh0_BCM_LoadManageLevel_1412,  CanCclRxHndlCh0_BCM_LoadManageLevel_1412, CanCclRxHndlCh0_BCM_LoadManageLevel_1412   },/*BCM_LoadManageLevel*/
  { /*  237  */    CanCclRxHndlCh0_BCM_CarModeIndicator_1412,  CanCclRxHndlCh0_BCM_CarModeIndicator_1412, CanCclRxHndlCh0_BCM_CarModeIndicator_1412   },/*BCM_CarModeIndicator*/
  { /*  238  */    CCL_NO_CBKRXACK,  CCL_NO_CBKRXTOUT, CCL_NO_CBKRXINV   },/*BCM_CarMode*/
  { /*  239  */    CanCclRxHndlCh0_EMS_BackupOdometer_1590,  CanCclRxHndlCh0_EMS_BackupOdometer_1590, CanCclRxHndlCh0_EMS_BackupOdometer_1590   },/*EMS_BackupOdometer*/
};


  /**********************************************************************************************************************
    Ccl_PduGrpVector
  **********************************************************************************************************************/
  /**
    \var    Ccl_PduGrpVector
    \brief  Contains an I-PDU-Group vector for each I-PDU, mapping the I-PDU to the corresponding I-PDU-Groups.
    */
  CONST(Ccl_PduGrpVectorType, CCL_CONST) Can_Ccl_PduGrpVector[Can_Ccl_Num_Of_PduGroups] = {
    /* Index    PduGrpVector   */
    /*     0 */        0x02,
    /*     1 */        0x01
  };

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
  CONST(Ccl_TxPduGrpInfoType, CCL_CONST) Can_Ccl_TxPduGrpInfo[Can_Ccl_Tx_Num_Messages] = {
    /* Index     PduGrpVectorStartIdx */ 
  { /*  0  */           0U  },
  { /*  1  */           0U  },
  { /*  2  */           0U  },
  { /*  3  */           0U  },
  { /*  4  */           0U  },
};

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
  CONST(Ccl_RxPduGrpInfoType, CCL_CONST) Can_Ccl_RxPduGrpInfo[Can_Ccl_Rx_Num_Messages] = {
    /* Index     PduGrpVectorStartIdx */ 
  { /*     0 */           1U  },
  { /*     1 */           1U  },
  { /*     2 */           1U  },
  { /*     3 */           1U  },
  { /*     4 */           1U  },
  { /*     5 */           1U  },
  { /*     6 */           1U  },
  { /*     7 */           1U  },
  { /*     8 */           1U  },
  { /*     9 */           1U  },
  { /*     10 */           1U  },
  { /*     11 */           1U  },
  { /*     12 */           1U  },
  { /*     13 */           1U  },
  { /*     14 */           1U  },
  { /*     15 */           1U  },
  { /*     16 */           1U  },
  { /*     17 */           1U  },
  { /*     18 */           1U  },
  { /*     19 */           1U  },
  { /*     20 */           1U  },
  { /*     21 */           1U  },
  { /*     22 */           1U  },
  { /*     23 */           1U  },
  { /*     24 */           1U  },
  { /*     25 */           1U  },
  { /*     26 */           1U  },
  { /*     27 */           1U  },
  { /*     28 */           1U  },
  { /*     29 */           1U  },
};

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
