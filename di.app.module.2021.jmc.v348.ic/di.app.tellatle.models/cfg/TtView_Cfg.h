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
#ifndef TTVIEW_CFG_H
#define TTVIEW_CFG_H
#include "TtCfg.h"

//============================================================================================
//NOTE: DONOT DELETE THE BELOW DEFINITIONS-
// USE THE BELOW DEFINITONS TO UPDATE "cTtOutputTableList"
//============================================================================================
#define cTT_HARDWARE_DRIVEN	(0x01)
#define cTT_TFT_DRIVEN		(0x02)
#define cTT_NOT_AVAILABLE	(0x00)

//============================================================================================
// USE THE BELOW DEFINITONS TO ENABLE SYNC BETWEEN SAME FREQUENCY TELLTALES
//============================================================================================
#define TT_SYNC_REQUIRED

/* Total number of TT available*/
#define cTOTAL_TT_CONFIGURED     (61u)
#define cTT_MAX_SUPPORTED_STATES (16u)

#define cTT_MAX_FLASH_NA         ((uint8)0xFFu)
/*Seatbelt flash used TTview Package*/
#define Seatbelt_Reserved 		 (0x00)
//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================
#define cTtView_DOut_SetPin(Byte_Id,Tt_status)     (void)Rte_Call_rpIoHwAbSet_Impl_SetPin(Byte_Id,Tt_status)

//NOTE: TELLTALE SEQUENCE SHOULD BE FOLLOWED AS LIKE IMPLEMENTED IN TtCfg.h
/*{TELLTALE_TYPE, TELLTALE_HW_INDEX_ID_INTERFACE/BYTE ID, BITPOSITION}*/
/*TELLTALE_TYPE - DIRECT/SPI_BASED_SHIFT_REGISTER/TFT*/
/*TELLTALE_HW_INDEX_ID_INTERFACE/BYTE_ID - If it is Direct driven, update the HwID,for shift register update byte name,*/
/*											For TFT, update as None*/
/*BITPOSITION	- 	update during Shift register based Telltales and TFT based telltale, the bit position*/

//#define cTtOutputTableList  {\
///*cTTTURNRIGHT				(0)  */{cTT_HARDWARE_DRIVEN,			eIOPinOutId_TT_TURN_RIGHT,			cTT_NOT_AVAILABLE					,NULL},\
///*cTTTURNLEFT				    (1)  */{cTT_HARDWARE_DRIVEN,			eIOPinOutId_TT_TURN_LEFT,			cTT_NOT_AVAILABLE					,NULL},\
///*cTTPosition				    (2)  */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_POS_LIGHT_BIT_POS				,NULL},\
///*cTTHIGH_BEAM				(3)  */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_HIGH_BEAM_BIT_POS				,NULL},\
///*cTTLOW_BEAM				    (4)  */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_LOW_BEAM_BIT_POS				,NULL},\
///*cTTFRONT_FOG				(5)  */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_FRONT_FOG_BIT_POS				,NULL},\
///*cTTRearFog				    (6)  */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_REAR_FOG_BIT_POS				,NULL},\
///*cTTParkingBrakeWarning	    (7)  */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_PBK_WARN_BIT_POS				,NULL},\
///*cTTAbswarning 			    (8)  */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_ABS_WARN_BIT_POS				,NULL},\
///*cTTLowFuel				    (9)  */{cTT_HARDWARE_DRIVEN,			eIOPinOutId_TT_FUEL_LOW,			cTT_NOT_AVAILABLE					,NULL},\
///*cTTBatteryWarning 		    (10) */{cTT_HARDWARE_DRIVEN,			eIOPinOutId_TT_CHARGE,				cTT_NOT_AVAILABLE					,NULL},\
///*cTTEngineCoolant			(11) */{cTT_HARDWARE_DRIVEN,			eIOPinOutId_TT_WATER_TEMPERATURE,	cTT_NOT_AVAILABLE					,NULL},\
///*cTTSeatBelt				    (12) */{cTT_HARDWARE_DRIVEN,			eIOPinOutId_TT_SEATBELT,			cTT_NOT_AVAILABLE					,NULL},\
///*cTTOilPressure			    (13) */{cTT_HARDWARE_DRIVEN,			eIOPinOutId_TT_OIL_PRESSURE,		cTT_NOT_AVAILABLE					,NULL},\
///*cTTTBoxSelfCheck			(14) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_TBOX_BIT_POS					,NULL},\
///*cTTCHK_ENG				    (15) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_CHECK_ENGINE_BIT_POS			,NULL},\
///*cTTAIRBAG 				    (16) */{cTT_HARDWARE_DRIVEN,			eIOPinOutId_TT_AIRBAG,				cTT_NOT_AVAILABLE					,NULL},\
///*cTTePowerSteering 	    	(17) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_EPS_BIT_POS					,NULL},\
///*cTTEsc				    	(18) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_ESC_BIT_POS					,NULL},\
///*cTTTpms				    	(19) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_TPMS_BIT_POS					,NULL},\
///*cTTEscoff 				    (20) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_ESC_OFF_BIT_POS				,NULL},\
///*cTTStartStopWorkGreen     	(21) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_STT_GREEN_BIT_POS				,NULL},\
///*cTTStartStopWorkYellow	    (22) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_STT_YELLOW_BIT_POS 			,NULL},\
///*cTTAUTOHILD				    (23) */{cTT_HARDWARE_DRIVEN,			cTT_NOT_AVAILABLE,					cTT_NOT_AVAILABLE					,NULL},\
///*cTTFCW					    (24) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_FCW_BIT_POS					,NULL},\
///*cTTFCWOFF 				    (25) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_FCW_OFF_BIT_POS				,NULL},\
///*cTTEpbWarning 			    (26) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_EPB_WARN_BIT_POS				,NULL},\
///*cTTHDC					    (27) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_HDC_BIT_POS					,NULL},\
///*cTTLKAWarningGreen	    	(28) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_LKA_WARN_GREEN_BIT_POS 		,NULL},\
///*cTTLKAWarningYellow	    	(29) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_LKA_WARN_YELLOW_BIT_POS		,NULL},\
///*cTTLWDWarningGreen		    (30) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_LWD_WARN_GREEN_BIT_POS 		,NULL},\
///*cTTLWDWarningYellow	    	(31) */{cTT_TFT_DRIVEN, 				cTT_NOT_AVAILABLE,					cTFT_LWD_WARN_YELLOW_BIT_POS		,NULL},\
///*cTTEpbGreen			    	(32) */{cTT_HARDWARE_DRIVEN,			eIOPinOutId_TT_PARK_GREEN,			cTT_NOT_AVAILABLE					,NULL},\
///*cTTEpbRed 			    	(33) */{cTT_HARDWARE_DRIVEN,			eIOPinOutId_TT_PARK_RED,			cTT_NOT_AVAILABLE					,NULL},\
//}
#define cTtOutputTableList \
{\
	/*cTTCHARGE		   					*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_Charge},\
	/*cTTPARK_LAMPS		    			*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/0},\
	/*cTTAIRBAG		   					*/ {cTT_HARDWARE_DRIVEN,				    8,				/*1*/cTT_NOT_AVAILABLE},\
	/*cTTABS_SYSTEM_FAILURE		   		*/ {cTT_HARDWARE_DRIVEN,				    6,				/*1*/cTT_NOT_AVAILABLE},\
	/*cTTEBD		   					*/ {cTT_HARDWARE_DRIVEN,				   12,				/*1*/cTT_NOT_AVAILABLE},\
	/*cTTOBD		   					*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_OBD},\
	/*cTTSVS		   					*/ {cTT_HARDWARE_DRIVEN,				   10,				/*1*/cTT_NOT_AVAILABLE},\
	/*cTTESP	   						*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_ESP},\
	/*cTTESP_OFF	   					*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_ESP_OFF},\
	/*cTTIMMO		   					*/ {cTT_HARDWARE_DRIVEN, 				    7, 				/*1*/cTT_NOT_AVAILABLE},\
	/*cTTESCL	   						*/ {cTT_TFT_DRIVEN, 		cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_ESCL},\
	/*cTTPEPS_FAULT		   				*/ {cTT_HARDWARE_DRIVEN, 				    9, 				/*1*/eTtViewMask_PEPS},\
	/*cTTTPMS_FAILURE		   			*/ {cTT_HARDWARE_DRIVEN, 				   11,				/*1*/cTT_NOT_AVAILABLE},\
	/*cTTCRUISE_CONTROL	   				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_CRUISE},\
	/*cTTLOW_FUEL		   				*/ {cTT_TFT_DRIVEN, 		cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_LOW_FUEL},\
	/*cTTLOW_OIL_PRESSURE		   		*/ {cTT_HARDWARE_DRIVEN, 				    5,				/*1*/cTT_NOT_AVAILABLE},\
	/*cTTCOOLANT_TEMP_HOT		   		*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_COOLANT_TEMP_HOT},\
	/*cTTDRIVER_SEATBELT_REMINDER	    */ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE, 				/*1*/eTtViewMask_DRIVER_SEATBELT},\
	/*cTTPASSENGER_SEATBELT_REMINDER	*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,	 			/*1*/eTtViewMask_PASSENGER_SEATBELT},\
	/*cTTLEFT_TURN	   					*/ {cTT_TFT_DRIVEN, 		cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_LEFT_TURN},\
	/*cTTRIGHT_TURN	   					*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_RIGHT_TURN},\
	/*cTTFRONT_FOG			  		 	*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_FRONT_FOG},\
	/*cTTREAR_FOG		   				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_REAR_FOG},\
	/*cTTHIGH_BEAM    					*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_HIGH_BEAM},\
	/*cTTEPS_RED		   				*/ {cTT_TFT_DRIVEN, 		cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_EPS_RED},\
	/*cTTEPS_YLW		   				*/ {cTT_TFT_DRIVEN, 		cTT_NOT_AVAILABLE, 				/*1*/eTtViewMask_EPS_YLW},\
	/*cTTPOSITION_LAMP			   		*/ {cTT_TFT_DRIVEN, 		cTT_NOT_AVAILABLE, 	 			/*1*/eTtViewMask_POSITION_LAMP},\
	/*cTTGEAR_SHIFT		   				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/cTT_NOT_AVAILABLE},\
	/*cTTOIL_WATER			   			*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_OIL_WATER},\
	/*cTTFOUR_WHEEL_DRIVE_H		   		*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/0},\
	/*cTTFOUR_WHEEL_DRIVE_L	   			*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/0},\
	/*cTTGLOW		   					*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_GLOW},\
	/*cTTAVH	  	 					*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_AVH},\
	/*cTTHDC		   					*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_HDC},\
	/*cTTEPB		   					*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_EPB},\
	/*cTTDPF							*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_Handson},\
	/*cTTSTT_GRAY						*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_STT_GRAY},\
	/*cTTSTT_GREEN						*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_STT_GREEN},\
	/*cTTSTT_YELLOW						*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_STT_YELLOW},\
	/*cTTGPF 							*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_GPF},\
	/*cTTADAS_AUTO 	    				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_ADAS_AUTO},\
	/*cTTAUTO_HOLD 	   				 	*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/0},\
	/*cTTGear_Box 	    				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_Gear_Box},\
	/*cTTFour_WD 	    				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/0},\
	/*cTTACC_RED        				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_ACC_RED},\
	/*cTTACC_GREEN      				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_ACC_GREEN},\
	/*cTTACC_WHITE 						*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_ACC_WHITE},\
	/*cTTLDW       						*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_LDW},\
	/*cTTBSD_SODLCA						*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_BSD_SODLCA},\
	/*cTTBSD_CTA   						*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_BSD_CTA},\
	/*cTTAEB_YELLOW						*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_AEB_YELLOW},\
	/*cTTAEB_RED        				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_AEB_RED},\
	/*cTTFCW_YELLOW     				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_FCW_YELLOW},\
	/*cTTFCW_RED        				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_FCW_RED},\
	/*cTTAPB	        				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_APB},\
	/*cTTAPB_FAULT      				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_APB_FAULT},\
	/*CTT_OVERSPEED      				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_OVERSPEED},\
	/*cTTCRUISE_RED      				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_CRUISE_RED},\
	/*cTTLOW_FUEL_WHITE  				*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_LOW_FUEL_WHITE},\
	/*cTTCOOLANT_TEMP_HOT_WHITE		   	*/ {cTT_TFT_DRIVEN,			cTT_NOT_AVAILABLE,				/*1*/eTtViewMask_COOLANT_TEMP_HOT_WHITE},\
}

/****************************************************************************************************************************/
/* TELLTALE_PROCESS_TYPE, TELLTALE_ON_COUNTER, TELLTALE_OFF_COUNTER, TELLTALE_MIN_FLASH_COUNTER, TELLTALE_MAX_FLASH_COUNTER */
/* TELLTALE_PROCESS_TYPE - OFF, ON , FLASH                                                                                  */
/* TELLTALE_ON_COUNTER - ON TIME interms of Counts                                                                          */
/* TELLTALE_ON_COUNTER - OFF TIME interms of Counts                                                                         */
/* TELLTALE_MIN_FLASH_COUNTER - MINIMUM FLASH                                                                               */
/* TELLTALE_MAX_FLASH_COUNTER - MAXIMUM FLASH                                                                               */
/****************************************************************************************************************************/
#define cTtConfigTableList {\
 /*eTtViewInputState_Off                */ {0,    0,    0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_On                 */ {0,    0,    0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_1HzFlash           */ {16,   16,   0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_2HzFlash           */ {8,    8,    0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_0p5HzFlash         */ {31,   31,   0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_4HzFlash           */ {4,    4,    0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_0p25HzFlash        */ {6,    6,    0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_1p67HzFlash        */ {9,    9,    0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_5HzFlash           */ {3,    3,    0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_0p5Hz75DcFlash     */ {47,   16,   0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_1p33HzFlash        */ {12,   12,   0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_0p5Hz25DcFlash     */ {16,   47,   0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_3p33Hz33p33DcFlash */ {3,     6,   0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_60MSON_400MS_OFF   */ {19,   13,   0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_400MSON_400MS_OFF  */ {13,   13,   0, cTT_MAX_FLASH_NA},\
 /*eTtViewInputState_0p33Hz 			*/ {50,   50,   0, cTT_MAX_FLASH_NA}\
}
#endif //TTVIEW_CFG_H
