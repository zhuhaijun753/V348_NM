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
#ifndef TT_CFG_H
#define TT_CFG_H

/*
**
** VALID VALUES:  (0 thru (cTT_NUM_ENTRIES_TELLTALE_CONFIG-1))
** cTT_NUM_ENTRIES_TELLTALE_CONFIG is defined in the public header file.
*/

//=====================================================================================================================
// !!!!!!!!!!!!!!!!!!!DO NOT DELETE THE BELOW MENTIONED MACROS !!!!!!!
// USE THIS MACRO FOR CALLING MULTIPLE FREQUENCIES FROM APP LAYER/CTRL LAYER/CTRL
// NOTE: THE SEQUENCE SHOULD NOT BE ALTERED OR MODIFIED.
//=====================================================================================================================
#define cTT_TURN_OFF			(0)
#define cTT_TURN_ON				(1)
#define cTT_FLASH_1HZ           (2)
#define cTT_FLASH_2HZ			(3)
#define cTT_FLASH_HALF_HZ       (4)
#define cTT_FLASH_4HZ			(5)
#define cTT_FLASH_2_5_HZ		(6)
#define cTT_FLASH_1_67_HZ		(7)
#define cTT_FLASH_5HZ			(8)
#define cTT_FLASH_HALF_HZ_75DC	(9)
#define cTT_FLASH_0_66_HZ       (10)
#define cTT_FLASH_HALF_HZ_25DC	(11)
#define cTT_3P33HZ33P33DCFLASH	(12)
#define cTT_FLASH_HALF_600MS_ON	(13)
#define cTT_FLASH_HALF_400MS_ON	(14)
//#define cTT_TURN_INPUT			(15)
#define cTT_FLASH_2_5_HZ_10_TIMES		(15)
#define cTT_FLASH_0p33Hz		(16)
//#define cTT_CONFIGURED_OFF       (0xff)

#define	eTtViewMask_Charge				(uint64)(0x0000000000000001)
// #define	eTtViewMask_AirBag 	  		  	(uint64)(0x0000000000000002)
// #define	eTtViewMask_ABS		    		(uint64)(0x0000000000000004)
#define	eTtViewMask_OBD		    		(uint64)(0x0000000000000008)
// #define	eTtViewMask_SVS	        		(uint64)(0x0000000000000010)
#define	eTtViewMask_ESP_OFF				(uint64)(0x0000000000000020)
// #define	eTtViewMask_IMMO				(uint64)(0x0000000000000040)
#define	eTtViewMask_ESCL				(uint64)(0x0000000000000080)
#define	eTtViewMask_PEPS				(uint64)(0x0000000000000100)
// #define	eTtViewMask_TPMS				(uint64)(0x0000000000000200)
#define	eTtViewMask_CRUISE				(uint64)(0x0000000000000400)
// #define	eTtViewMask_OIL_PRESSURE		(uint64)(0x0000000000000800)
//#define	eTtViewMask_COOLANT_TEMP_HOT	(uint64)(0x0000000000001000)
#define	eTtViewMask_EPS_RED				(uint64)(0x0000000000002000)
#define	eTtViewMask_EPS_YLW				(uint64)(0x0000000000004000)
// #define	eTtViewMask_GEAR_SHIFT			(uint64)(0x0000000000008000)
#define	eTtViewMask_OIL_WATER			(uint64)(0x0000000000010000)
#define	eTtViewMask_FOUR_WHEEL_DRIVE_H	(uint64)(0x0000000000020000)
#define	eTtViewMask_FOUR_WHEEL_DRIVE_L	(uint64)(0x0000000000040000)
#define	eTtViewMask_GLOW				(uint64)(0x0000000000080000)
#define	eTtViewMask_AVH					(uint64)(0x0000000000100000)
#define	eTtViewMask_HDC					(uint64)(0x0000000000200000)
#define	eTtViewMask_EPB					(uint64)(0x0000000000400000)
#define eTtViewMask_Handson             (uint64)(0x0000000000800000)
#define eTtViewMask_STT_GRAY			(uint64)(0x0000000001000000)
#define eTtViewMask_STT_GREEN			(uint64)(0x0000000002000000)
#define eTtViewMask_STT_YELLOW			(uint64)(0x0000000004000000)
#define eTtViewMask_GPF                 (uint64)(0x0000000008000000)
#define eTtViewMask_ADAS_AUTO           (uint64)(0x0000000010000000)
#define eTtViewMask_AUTO_HOLD           (uint64)(0x0000000020000000)
#define eTtViewMask_Gear_Box            (uint64)(0x0000000040000000)
#define eTtViewMask_FOUR_WD             (uint64)(0x0000000080000000)
#define eTtViewMask_ACC_RED             (uint64)(0x0000000100000000)
#define eTtViewMask_ACC_GREEN           (uint64)(0x0000000200000000)
#define eTtViewMask_ACC_WHITE           (uint64)(0x0000000400000000)
#define eTtViewMask_LDW                 (uint64)(0x0000000800000000)
#define eTtViewMask_BSD_SODLCA          (uint64)(0x0000001000000000)
#define eTtViewMask_BSD_CTA             (uint64)(0x0000002000000000)
#define eTtViewMask_AEB_YELLOW          (uint64)(0x0000004000000000)
#define eTtViewMask_AEB_RED             (uint64)(0x0000008000000000)
#define eTtViewMask_FCW_YELLOW          (uint64)(0x0000010000000000)
#define eTtViewMask_FCW_RED             (uint64)(0x0000020000000000)
#define eTtViewMask_APB					(uint64)(0x0000040000000000)
#define eTtViewMask_APB_FAULT           (uint64)(0x0000080000000000)
#define eTtViewMask_OVERSPEED			(uint64)(0x0000100000000000)
#define eTtViewMask_LOW_FUEL            (uint64)(0x0000200000000000)
#define eTtViewMask_COOLANT_TEMP_HOT    (uint64)(0x0000400000000000)
#define	eTtViewMask_CRUISE_RED		    (uint64)(0x0000800000000000)
#define	eTtViewMask_LOW_FUEL_WHITE		(uint64)(0x0001000000000000)
#define	eTtViewMask_COOLANT_TEMP_HOT_WHITE	(uint64)(0x0002000000000000)



#define eTtViewMask_ESP 	  		  	(uint64)(0x0000000000000002)
#define	eTtViewMask_DRIVER_SEATBELT		(uint64)(0x0000000000000004)
#define	eTtViewMask_PASSENGER_SEATBELT	(uint64)(0x0000000000000010)
#define	eTtViewMask_LEFT_TURN		    (uint64)(0x0000000000000040)
#define	eTtViewMask_RIGHT_TURN			(uint64)(0x0000000000000100)
#define	eTtViewMask_FRONT_FOG			(uint64)(0x0000000000000200)
#define	eTtViewMask_REAR_FOG    		(uint64)(0x0000000000000800)
#define	eTtViewMask_HIGH_BEAM       	(uint64)(0x0000000000001000)
#define	eTtViewMask_POSITION_LAMP		(uint64)(0x0000000000008000)



//=====================================================================================================================
//  Create the definitions for the required number of telltales as like below and map the macro name under the second
//  macro definition list.
// NOTE: Individual Telltale Macro Definitions should hold the value which is defined under Port and Pin Configuration
// definitions.
// Start the list from '0' always. It is mandatory!!!
//=====================================================================================================================
#define cTTCHARGE		  			(0)
#define cTTPARK_LAMPS				(1)
#define cTTAIRBAG		  			(2)
#define cTTABS_SYSTEM_FAILURE		(3)
#define cTTEBD						(4)
#define cTTOBD						(5)
#define cTTSVS						(6)
#define cTTESP	   					(7)
#define cTTESP_OFF					(8)
#define cTTIMMO						(9)
#define cTTESCL	   					(10)
#define cTTPEPS_FAULT				(11)
#define cTTTPMS_FAILURE				(12)
#define cTTCRUISE_CONTROL	 		(13)
#define cTTLOW_FUEL		 			(14)
#define cTTLOW_OIL_PRESSURE			(15)
#define cTTCOOLANT_TEMP_HOT			(16)
#define cTTDRIVER_SEATBELT_REMINDER	(17)
#define cTTPASSENGER_SEATBELT_REMINDER (18)
#define cTTLEFT_TURN	  			(19)
#define cTTRIGHT_TURN	  			(20)
#define cTTFRONT_FOG				(21)
#define cTTREAR_FOG					(22)
#define cTTHIGH_BEAM    			(23)
#define cTTEPS_RED					(24)
#define cTTEPS_YLW					(25)
#define cTTPOSITION_LAMP			(26)
#define cTTGEAR_SHIFT		   		(27)
#define cTTOIL_WATER				(28)
#define cTTFOUR_WHEEL_DRIVE_H		(29)
#define cTTFOUR_WHEEL_DRIVE_L		(30)
#define cTTGLOW						(31)
#define cTTAVH						(32)
#define cTTHDC						(33)
#define cTTEPB						(34)
#define cTTHandson                  (35)
#define cTTSTT_GRAY                 (36)
#define cTTSTT_GREEN                (37)
#define cTTSTT_YELLOW               (38)
#define cTTGPF                      (39)
#define cTTADAS_AUTO                (40)
#define cTTAUTO_HOLD                (41)
#define cTTGear_Box                 (42)
#define cTTFour_WD                  (43)
#define cTTACC_RED                  (44)
#define cTTACC_GREEN                (45)
#define cTTACC_WHITE                (46)
#define cTTLDW                      (47)
#define cTTBSD_SODLCA               (48)
#define cTTBSD_CTA                  (49) 
#define cTTAEB_YELLOW               (50)
#define cTTAEB_RED                  (51)
#define cTTFCW_YELLOW               (52)
#define cTTFCW_RED                  (53)
#define cTTAPB		                (54)
#define cTTAPB_FAULT                (55)
#define CTT_OVERSPEED				(56)           
#define cTTCRUISE_RED               (57)
#define cTTLOW_FUEL_WHITE           (58)
#define cTTCOOLANT_TEMP_HOT_WHITE   (59)




//====================================================================================================================
// Assign the number of Telltales count in the below macro
//====================================================================================================================
#define cTT_NUM_ENTRIES_TELLTALE_CONFIG (60)


/* ============Telltale Key Value============ */
#define FRONTFOG_IND_ON		((uint8)0x01)
#define TURN_ON_REARFOG		((uint8)0x01)
#define TURN_ON_LOWBEAM		((uint8)0x01)
#define TURN_ON_HIGHBEAM	((uint8)0x01)
#define ABSALARM_ON			((uint8)0x01)
#define EBDALARM_ON			((uint8)0x01)
#define BRAKE_FLUID_LOW		((uint8)0x00)
#define AIRBAG_LAMP_ON		((uint8)0x01)
#define CHARGE_WARN_OFF		((uint8)0x00)
#define COOLANT_WARNING_ON	((uint8)0x01)
#define STT_SYS_NO_FAULT	((uint8)0x00)
#define STT_SYS_STOP   		((uint8)0x01)
#define STT_SYS_FAULT   	((uint8)0x02)
#define AUTOHOLD_OFF						((uint8)0x00)
#define AUTOHOLD_ON   						((uint8)0x01)
#define HDC_DISPLAY_MSG_REQ_OFF				((uint8)0x00)
#define HDC_DISPLAY_MSG_REQ_ON  			((uint8)0x01)
#define EPB_SYSTEM_ST_INDICATION_OFF		((uint8)0x00)
#define EPB_SYSTEM_ST_INDICATION_ON  		((uint8)0x01)
#define EPB_SYSTEM_ST_INDICATION_FLASH_2HZ	((uint8)0x02)
#define EPB_SYSTEM_ST_INDICATION_FLASH_4HZ  ((uint8)0x03)
#define EPB_WARNING_INDICATION_OFF			((uint8)0x00)
#define EPB_WARNING_INDICATION_ON  			((uint8)0x01)
#define EPB_WARNING_INDICATION_FLASH_2HZ	((uint8)0x02)
#define EPB_WARNING_INDICATION_FLASH_4HZ  	((uint8)0x03)
#define AEB_FCW_INIT						((uint8)0x00)
#define AEB_FCW_FUNCTION_OK  				((uint8)0x01)
#define AEB_FCW_TEMPORARY_ERROR				((uint8)0x02)
#define AEB_FCW_PERMANENT_ERROR  			((uint8)0x03)
#define AEB_EBA_INIT						((uint8)0x00)
#define AEB_EBA_FUNCTION_OK  				((uint8)0x01)
#define AEB_EBA_TEMPORARY_ERROR				((uint8)0x02)
#define AEB_EBA_PERMANENT_ERROR  			((uint8)0x03)
#define AEB_FCW_RQ_ENA_OFF					((uint8)0x00)
#define AEB_FCW_RQ_ENA_ON  					((uint8)0x01)
#define AEB_FCW_RQ_ENA_RESERVED				((uint8)0x02)
#define AEB_FCW_RQ_ENA_INVALID  			((uint8)0x03)
#define AEB_EBA_RQ_ENA_OFF					((uint8)0x00)
#define AEB_EBA_RQ_ENA_ON  					((uint8)0x01)
#define AEB_EBA_RQ_ENA_RESERVED				((uint8)0x02)
#define AEB_EBA_RQ_ENA_INVALID  			((uint8)0x03)



#define LEFT_TURN_IND_ON 	((uint8)0x01)
#define RIGHT_TURN_IND_ON 	((uint8)0x01)







#endif
