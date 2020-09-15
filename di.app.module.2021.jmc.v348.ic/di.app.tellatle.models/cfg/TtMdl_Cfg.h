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
#ifndef TTMDL_CFG_H
#define TTMDL_CFG_H

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

/************************************************************************************************************************
**The PRODUCT_SPECIFIC_TELLTALE_ATTRIB_CONFIG macro is a means for the
**application to populate the telltale attributes.

**Note: The table is limited to TT_MAX_SUPPORTED_CONFIG number of entries.

**The following information needs to be in the table:

**Attributes Byte (UINT8) -
**    Choose the appropriate attributes for each telltale
**    by "OR'ing" the attributes defined in ftt.h.
**    eg. if the BRAKE light should be active only in
**    RUN and CRANK and is proven out, use (P|R|C).
** Note: The order of attributes shall match with the telltale ids given in the above macro.

       cP   TT_PROVEOUT
       cO   TT_OFF
       cR   TT_RUN
       cA   TT_ACC
       cC   TT_CRANK
       cL   TT_LVI
       cH   TT_HVI
	   cCO	cTT_CONFIGURED_OFF
       cZ   NULL_VALE
************************************************************************************************************************
############################
######### EXAMPLE: #########
############################

#define PRODUCT_SPECIFIC_TELLTALE_ATTRIB_CONFIG \
       ATTRIB.
       BYTE
   =============
  { (P|Z|Z|Z|R|C|Z  )  }, \
  { (P|O|A|R|C|Z|Z  )  }, \
  { (Z|Z|Z|Z|Z|R|C  )  }, \
==========================================================================
*/
//#define cPRODUCT_SPECIFIC_TELLTALE_ATTRIB_CONFIG \
///*cTTTURNRIGHT				(0)  */ {(cZ|cO|cR|cA|cC|cL|cH|cZ)},\
///*cTTTURNLEFT				    (1)  */ {(cZ|cO|cR|cA|cC|cL|cH|cZ)},\
///*cTTPosition				    (2)  */ {(cZ|cO|cR|cA|cC|cL|cH|cZ)},\
///*cTTHIGH_BEAM				(3)  */ {(cZ|cO|cR|cA|cC|cL|cH|cZ)},\
///*cTTLOW_BEAM					(4)  */ {(cZ|cO|cR|cA|cC|cL|cH|cZ)},\
///*cTTFRONT_FOG				(5)  */ {(cZ|cO|cR|cA|cC|cL|cH|cZ)},\
///*cTTRearFog					(6)  */ {(cZ|cO|cR|cA|cC|cL|cH|cZ)},\
///*cTTParkingBrakeWarning		(7)  */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTAbswarning 				(8)  */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTLowFuel					(9)  */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTBatteryWarning 			(10) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTEngineCoolant			(11) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTSeatBelt					(12) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTOilPressure				(13) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTTBoxSelfCheck			(14) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTCHK_ENG					(15) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTAIRBAG 					(16) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTePowerSteering 			(17) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTEsc						(18) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTTpms						(19) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTEscoff 					(20) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTStartStopWorkGreen 		(21) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTStartStopWorkYellow		(22) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTAUTOHILD					(23) */ {(cZ|cO|cR|cA|cC|cL|cH|cCO)},\
///*cTTFCW						(24) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTFCWOFF 					(25) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTEpbWarning 				(26) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTHDC						(27) */ {(cZ|cZ|cR|cA|cC|cL|cH|cCO)},\
///*cTTLKAWarningGreen			(28) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTLKAWarningYellow			(29) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTLWDWarningGreen			(30) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTLWDWarningYellow			(31) */ {(cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
///*cTTEpbGreen					(32) */ {(cZ|cO|cR|cA|cC|cL|cH|cZ)},\
///*cTTEpbRed 					(33) */ {(cZ|cO|cR|cA|cC|cL|cH|cZ)}
#define cPRODUCT_SPECIFIC_TELLTALE_ATTRIB_CONFIG \
/*cTTCHARGE (red) 					(0)  */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTPARK_LAMPS (red) 				(1)  */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTAIRBAG (red) 					(2)  */ {(cP|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTABS_SYSTEM_FAILURE (yellow)    (3)  */ {(cP|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTEBD (red)                      (4)  */ {(cP|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTOBD (yellow)					(5)  */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTSVS (yellow)					(6)  */ {(cP|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTESP (yellow)					(7)  */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTESP_OFF (yellow)				(8)  */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTIMMO (red)                     (9)  */ {(cP|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTESCL (red)                     (10) */ {(cZ|cZ|cO|cR|cA|cZ|cL|cH|cZ)},\
/*cTTPEPS_FAULT (red) 				(11) */ {(cP|cZ|cO|cR|cA|cC|cL|cH|cZ)},\
/*cTTTPMS_FAILURE (yellow)          (12) */ {(cP|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTCRUISE_CONTROL (green)         (13) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTLOW_FUEL (yellow)				(14) */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTLOW_OIL_PRESSURE (red)         (15) */ {(cP|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTCOOLANT_TEMP_HOT (red)         (16) */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTDRIVER_SEATBELT_REMIN (red)    (17) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTPASSENGER_SEATBELT_RE (red)    (18) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTLEFT_TURN (green)				(19) */ {(cZ|cZ|cO|cR|cA|cZ|cL|cH|cZ)},\
/*cTTRIGHT_TURN (green)				(20) */ {(cZ|cZ|cO|cR|cA|cZ|cL|cH|cZ)},\
/*cTTFRONT_FOG (green)				(21) */ {(cZ|cZ|cO|cR|cA|cZ|cL|cH|cZ)},\
/*cTTREAR_FOG (yellow)				(22) */ {(cZ|cZ|cO|cR|cA|cZ|cL|cH|cZ)},\
/*cTTHIGH_BEAM (blue)				(23) */ {(cZ|cZ|cO|cR|cA|cZ|cL|cH|cZ)},\
/*cTTEPS_RED (red)					(24) */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTEPS_YLW (yellow)				(25) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTPOSITION_LAMP (green)          (26) */ {(cZ|cZ|cO|cR|cA|cZ|cL|cH|cZ)},\
/*cTTGEAR_SHIFT (delete)            (27) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTOIL_WATER (red)                (28) */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTFOUR_WHEEL_DRIVE_H (green)     (29) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTFOUR_WHEEL_DRIVE_L (green)     (30) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTGLOW (yellow)					(31) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTAVH (green)					(32) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTHDC (green)					(33) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTEPB (yellow)					(34) */ {(cZ|cZ|cO|cR|cA|cZ|cL|cH|cZ)},\
/*cTTHandson (yellow)			    (35) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTSTT_GRAY (gray)                (36) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTSTT_GREEN (green)				(37) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTSTT_YELLOW (yellow)            (38) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTGPF (yellow)					(39) */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTADAS_AUTO (white)				(40) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTAUTO_HOLD (green)				(41) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTGearBox (red)					(42) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTFourWD (yellow)                (43) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTACC_RED (red)					(44) */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTACC_GREEN (green)				(45) */ {(cZ|cZ|cO|cR|cA|cZ|cL|cH|cZ)},\
/*cTTACC_WHITE (white)				(46) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTLDW (yellow)					(47) */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTBSD_SODLCA (yellow)            (48) */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTBSD_CTA (yellow)				(49) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTAEB_YELLOW (yellow)            (50) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTAEB_RED (red)					(51) */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTFCW_YELLOW (yellow)            (52) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTFCW_RED (red)					(53) */ {(cZ|cZ|cZ|cR|cA|cC|cL|cH|cZ)},\
/*cTTAPB (red)                      (54) */ {(cZ|cZ|cO|cR|cA|cC|cL|cH|cZ)},\
/*cTTAPB_FAULT (yellow)				(55) */ {(cZ|cZ|cO|cR|cA|cC|cL|cH|cZ)},\
/*CTT_OVERSPEED (yellow)			(56) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTCRUISE_RED (red)			    (57) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTLOW_FUEL_WHITE (white)			(58) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)},\
/*cTTCOOLANT_TEMP_HOT_WHITE (white) (59) */ {(cZ|cZ|cZ|cR|cA|cZ|cL|cH|cZ)}


#if 1
//#define cPRODUCT_SPECIFIC_TELLTALE_ATTRIB_CONFIG_PROVEOUT \
///*cTTTURNRIGHT			(0)  */ {0x01, 0x01},\
///*cTTTURNLEFT				(1)  */ {0x01, 0x01},\
///*cTTPosition				(2)  */ {0x01, 0x01},\
///*cTTHIGH_BEAM			(3)  */ {0x01, 0x01},\
///*cTTLOW_BEAM				(4)  */ {0x01, 0x01},\
///*cTTFRONT_FOG			(5)  */ {0x01, 0x01},\
///*cTTRearFog				(6)  */ {0x01, 0x01},\
///*cTTParkingBrakeWarning	(7)  */ {0x01, 0x01},\
///*cTTAbswarning 			(8)  */ {0x01, 0x01},\
///*cTTLowFuel				(9)  */ {0x01, 0x01},\
///*cTTBatteryWarning 		(10) */ {0x01, 0x01},\
///*cTTEngineCoolant		(11) */ {0x01, 0x01},\
///*cTTSeatBelt				(12) */ {0x01, 0x01},\
///*cTTOilPressure			(13) */ {0x01, 0x01},\
///*cTTTBoxSelfCheck		(14) */ {0x01, 0x01},\
///*cTTCHK_ENG				(15) */ {0x01, 0x01},\
///*cTTAIRBAG 				(16) */ {0x01, 0x01},\
///*cTTePowerSteering 		(17) */ {0x01, 0x01},\
///*cTTEsc					(18) */ {0x01, 0x01},\
///*cTTTpms					(19) */ {0x01, 0x01},\
///*cTTEscoff 				(20) */ {0x01, 0x01},\
///*cTTStartStopWorkGreen 	(21) */ {0x01, 0x01},\
///*cTTStartStopWorkYellow	(22) */ {0x01, 0x01},\
///*cTTAUTOHILD				(23) */ {0x01, 0x01},\
///*cTTFCW					(24) */ {0x01, 0x01},\
///*cTTFCWOFF 				(25) */ {0x01, 0x01},\
///*cTTEpbWarning 			(26) */ {0x01, 0x01},\
///*cTTHDC					(27) */ {0x01, 0x01},\
///*cTTLKAWarningGreen		(28) */ {0x01, 0x01},\
///*cTTLKAWarningYellow		(29) */ {0x01, 0x01},\
///*cTTLWDWarningGreen		(30) */ {0x01, 0x01},\
///*cTTLWDWarningYellow		(31) */ {0x01, 0x01},\
///*cTTEpbGreen				(32) */ {0x01, 0x01},\
///*cTTEpbRed 				(33) */ {0x01, 0x01}

#define cPRODUCT_SPECIFIC_TELLTALE_ATTRIB_CONFIG_PROVEOUT \
/*cTTCHARGE		  				(0)  */ {0x01, 0x01},\
/*cTTPARK_LAMPS					(1)  */ {0x01, 0x01},\
/*cTTAIRBAG		  				(2)  */ {0x01, 0x01},\
/*cTTABS_SYSTEM_FAILURE			(3)  */ {0x01, 0x01},\
/*cTTEBD						(4)  */ {0x01, 0x01},\
/*cTTOBD						(5)  */ {0x01, 0x01},\
/*cTTSVS						(6)  */ {0x01, 0x01},\
/*cTTESP	   					(7)  */ {0x01, 0x01},\
/*cTTESP_OFF					(8)  */ {0x01, 0x01},\
/*cTTIMMO						(9)  */ {0x01, 0x01},\
/*cTTESCL	   					(10) */ {0x01, 0x01},\
/*cTTPEPS_FAULT					(11) */ {0x01, 0x01},\
/*cTTTPMS_FAILURE				(12) */ {0x01, 0x01},\
/*cTTCRUISE_CONTROL	 			(13) */ {0x01, 0x01},\
/*cTTLOW_FUEL		 			(14) */ {0x01, 0x01},\
/*cTTLOW_OIL_PRESSURE			(15) */ {0x01, 0x01},\
/*cTTCOOLANT_TEMP_HOT			(16) */ {0x01, 0x01},\
/*cTTDRIVER_SEATBELT_REMIN		(17) */ {0x01, 0x01},\
/*cTTPASSENGER_SEATBELT_RE		(18) */ {0x01, 0x01},\
/*cTTLEFT_TURN	  				(19) */ {0x01, 0x01},\
/*cTTRIGHT_TURN	  				(20) */ {0x01, 0x01},\
/*cTTFRONT_FOG					(21) */ {0x01, 0x01},\
/*cTTREAR_FOG					(22) */ {0x01, 0x01},\
/*cTTHIGH_BEAM    				(23) */ {0x01, 0x01},\
/*cTTEPS_RED					(24) */ {0x01, 0x01},\
/*cTTEPS_YLW					(25) */ {0x01, 0x01},\
/*cTTPOSITION_LAMP				(26) */ {0x01, 0x01},\
/*cTTGEAR_SHIFT		   			(27) */ {0x01, 0x01},\
/*cTTOIL_WATER					(28) */ {0x01, 0x01},\
/*cTTFOUR_WHEEL_DRIVE_H			(29) */ {0x01, 0x01},\
/*cTTFOUR_WHEEL_DRIVE_L			(30) */ {0x01, 0x01},\
/*cTTGLOW						(31) */ {0x01, 0x01},\
/*cTTAVH						(32) */ {0x01, 0x01},\
/*cTTHDC						(33) */ {0x01, 0x01},\
/*cTTEPB						(34) */ {0x01, 0x01},\
/*cTTDPF						(35) */ {0x01, 0x01},\
/*cTTSTT_GRAY  					(36) */ {0x01, 0x01},\
/*cTTSTT_GREEN 					(37) */ {0x01, 0x01},\
/*cTTSTT_YELLOW					(38) */ {0x01, 0x01},\
/*cTTGPF      					(39) */ {0x01, 0x01},\
/*cTTADAS_AUTO      			(40) */ {0x01, 0x01},\
/*cTTAUTO_HOLD      			(41) */ {0x01, 0x01},\
/*cTTGear_Box      				(42) */ {0x01, 0x01},\
/*cTTFourWD      				(43) */ {0x01, 0x01},\
/*cTTACC_RED         			(44) */ {0x01, 0x01},\
/*cTTACC_GREEN       			(45) */ {0x01, 0x01},\
/*cTTACC_WHITE       			(46) */ {0x01, 0x01},\
/*cTTLDW             			(47) */ {0x01, 0x01},\
/*cTTBSD_SODLCA      			(48) */ {0x01, 0x01},\
/*cTTBSD_CTA         			(49) */ {0x01, 0x01},\
/*cTTAEB_YELLOW      			(50) */ {0x01, 0x01},\
/*cTTAEB_RED         			(51) */ {0x01, 0x01},\
/*cTTFCW_YELLOW      			(52) */ {0x01, 0x01},\
/*cTTFCW_RED         			(53) */ {0x01, 0x01},\
/*cTTAPB						(54) */ {0x01, 0x01},\
/*cTTAPB_FAULT       			(55) */ {0x01, 0x01},\
/*CTT_OVERSPEED       			(56) */ {0x01, 0x01},\
/*cTTCRUISE_RED       			(57) */ {0x01, 0x01},\
/*cTTLOW_FUEL_WHITE    			(58) */ {0x01, 0x01},\
/*cTTCOOLANT_TEMP_HOT_WHITE		(59) */ {0x01, 0x01}


#endif

/*
** define USE_TT_MANAGER_PROVEOUT as non-zero if you want telltale manager to
** handle proveout (must define TT_MANAGER_PROVEOUT_TIME_IN_MS), otherwise define
** as 0 and define the PROVEOUT_ONGOING() macro to tell telltale manager when
** proveout is happening.
** VALID VALUES: 0 or non-zero.
**
** This #define should be defined as an 8 bit value.
*/
#define cUSE_TT_MANAGER_PROVEOUT (1)
/*
** define TT_MANAGER_PROVEOUT_TIME_IN_MS to the time in milliseconds that proveout
** should last.  This macro has no meaning or effect if USE_TT_MANAGER_PROVEOUT is
** set to 0.
** VALID VALUES: minimum = 0, maximum = 65535 * TELLTALE_EXECUTION_RATE.
**
** This #define should be defined as a 16 bit value.  Use a "u" subscript.
*/
#define cTT_MANAGER_PROVEOUT_TIME_IN_MS (3000)//(3700)//TtMdl task period= 30ms ,9s means 300(=9000/30)


/*
** Define HVI/LVI condition, should return a BOOLEAN
** if TRUE then HVI/LVI condition exists, else FALSE
**
** VALID VALUES:  Must resolve to a BOOLEAN EXPRESSION.
**                For example:   (high_voltage_status != FALSE)
**                A TESTBIT can also be used.
**                In this sample file, the HW_XYZ_BATT macros
**                are written to be BOOLEAN EXPRESSIONS.
*/
#define cPS_VOLTAGE_LVI()  (FALSE)

#define cPS_VOLTAGE_HVI()  (FALSE)

//#endif
#endif

