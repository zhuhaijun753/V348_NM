/*****************************************************************************
*                                                                            *
*              CONFIDENTIAL VISTEON CORPORATION                              *
*                                                                            *
* This is an unpublished work of authorship, which contains trade            *
* secrets, created in 2014. Visteon Corporation owns all rights to           *
* this work and intends to maintain it in confidence to preserve             *
* its trade secret status. Visteon Corporation reserves the right,           *
* under the copyright laws of the United States or those of any              *
* other country that may have jurisdiction, to protect this work             *
* as an unpublished work, in the event of an inadvertent or                  *
* deliberate unauthorized publication. Visteon Corporation also              *
* reserves its rights under all copyright laws to protect this               *
* work as a published work, when appropriate. Those having access            *
* to this work may not copy it, use it, modify it or disclose the            *
* information contained in it without the written authorization              *
* of Visteon Corporation.                                                    *
*                                                                            *
******************************************************************************/

/*****************************************************************************

File Name        :  RTE_RTE_ADAPTER.H
Module Short Name:  RTE
Author           :  SBAMANE
Description      :  GENERATED BY A2T_VFB_GEN.EXE V1.12
                    GENERATED ON 26 MAY 2017 - 18:14:39 (UTC+05:30)
                    GENERATION OPTIONS : -oSize

Organization     :  Visteon Software Operation
                    Visteon Corporation

******************************************************************************/

#ifndef LAP_IPCL_ADAPTER_H
#define LAP_IPCL_ADAPTER_H


/*------------------------------------------------------------------------------
 Include files 
------------------------------------------------------------------------------*/            
#include "deftypes.h"

/*****************************************************************************   
*                                 Macro Definitions                          *   
*----------------------------------------------------------------------------*   
* Definition of macro shall be followed by a comment that explains the       *   
* purpose of the macro.                                                      *   
******************************************************************************/  

/*****************************************************************************
 *                              IPCL Protocol Struct                         *
 * **************************************************************************/
//PACKID = 0x4007 subid = 26
typedef struct
{
	U16 AVCV_MesgType	:8;
	U16	AVCV_Data_01	:8;
	U16	AVCV_Data_02    :8;
	U16 AVCV_Data_03    :8;
	U16 AVCV_Data_04    :8;
	U16 AVCV_AudioSrc	:8;
	U16 AVCV_AudioStatus:8;
	U16 AVCV_PsCallSrc  :8;
	U16 AVCV_PsCallType :8;
	U16 AVCV_ScreenReq	:8;
    U32 AVCV_PlayTime;
    U32 AVCV_Duration;
	U16 AVCV_Audio_Char[56];
	U16 AVCV_Ps_Char[40];
	U16 AVCV_Pb_Char[32];
}IPCL_AVCVType;

//PACKID = 0x4007 subid = 27
typedef struct
{
	U16	Navi_Switch;
	U16	Navi_Sign;
    U32 Navi_Dist;	
	U16 Navi_Char[16];
}IPCL_MP5NaviType;

//PACKID = 0x4009 subid = 67 vip tx or rx
typedef struct
{
	/* customer's set informations */
	U8 ModeSelection;
    U8 Language;
    U8 OverspeedSwitch;
    U8 OverspeedValue;
    U8 TemperatureUnit;
    U8 PressureUnit;
    U8 MileageUnit;
    U8 FCUnit;
    U8 MenuPosition;
}IPCL_MuenConfigRec;



extern IPCL_AVCVType	  	IPCL_AVCV_Data;				//PACKID = 0x4007 subid = 26
extern IPCL_MP5NaviType  	IPCL_MP5Navi_Data;			//PACKID = 0x4007 subid = 27

extern IPCL_MuenConfigRec 	IPCL_GetMenuConfig_Data;	//PACKID = 0x4009 subid = 67  viprx and tx

extern IPCL_AVCVType	  	HMI_IPCL_AVCV_Data;			//used for gdt phone & multimedia display
extern IPCL_MP5NaviType  	HMI_IPCL_MP5Navi_Data;		//used for gdt navigation display

extern U16 HMI_AVCV_Pb_Char[256];
extern U8 HMI_AVCV_Pl_Char[32];

extern void IPCL_Adapter_DataInit(void);
extern void UpdateAVCVData(uint8* p_signal_pointer);
extern void UpdateMP5NavigationData(uint8* p_signal_pointer);

extern void IPCL_UpdateAVCVStateData(void) ;
extern void IPCL_UpdateMP5NaviStateData(void) ;

#endif //LAP_IPCL_ADAPTER_H

