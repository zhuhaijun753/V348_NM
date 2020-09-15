//******************************************************************************
// Company:      Johnson Controls Inc.
// -----------------------------------------------------------------------------
// Copyright:    This software is JCI property.
//               Duplication or disclosure without JCI written authorization
//               is prohibited.
// -----------------------------------------------------------------------------
// Project:      <PROJECT>
// Language:     C
// -----------------------------------------------------------------------------
// Component:    <Comments about the component (role, algorithm, structure, 
//               limitations, defined tasks and events...)>
// -----------------------------------------------------------------------------
// $Date    :   $
// $Archive :   $
// $Revision:   1.1  $
// -----------------------------------------------------------------------------
// $Log     :   $
//******************************************************************************

//------------------------------------------------------------------------------
// Included files to resolve specific definitions in this file
//
// #include <system_file_name.h>
// #include "project_file_name.h"
//------------------------------------------------------------------------------
#include "deftypes.h"
#include "LAP_IPCL_adapter.h"

typedef struct
{
	U16	DataLength;	  
	U8	DataSrcState  :2;
	U8	DataSource	  :3;
	U8	DataType	  :3;	
	U8  DataNum;
	U8	DataCount;
}AVCV_CtrlMeg;

typedef struct
{
	U8	DataLength	  :6;
	U8	DataSource	  :3;
	U8	ContactsID    :4;	//maximum 8 contacts
	U8	ContactsNum   :4;	//maximum 8 contacts
	U8 	PhNumLength;
	U8	ContactsLength;
	U8  DataNum;
	U8  DataCount;
}AVCV_PBCtrlMeg;

typedef enum
{
	MegDataNum21=0x21,	
	MegDataNum22,
	MegDataNum23,
	MegDataNum24,
	MegDataNum25,
	MegDataNum26,
	MegDataNum27,
	MegDataNum28,
	MegDataNum29,
	MegDataNum2A,
	MegDataNum2B,
	MegDataNum2C,
	MegDataNum2D,
	MegDataNum2E,
	MegDataNum2F
}AVCVMegData;

typedef enum
{
	SoundSrc_none,
	SoundSrc_play,
	SoundSrc_stop
}AVDataSrcState;

typedef enum
{
	DataType_none,
	DataType_Name,
	DataType_Author,
	DataType_RadioInfo = 0x04,
	DataType_Duration,
	DataType_PlayTime
}AVDataType;

typedef enum
{
	PSDataType_None,
	PSDataType_Name,
	PSDataType_PhoneNum,
	PSDataType_CallTime,
}PSDataType;

extern IPCL_AVCVType		sAVCV_IPCL;
extern IPCL_MP5NaviType		sMP5Navi_IPCL;

#define AUDIO_DATA			((U8)0x01)
#define PHONE_DATA			((U8)0x02)
#define PHONEBOOK_DATA_1_1	((U8)0x03)
#define PHONEBOOK_DATA_1_2	((U8)0x13)
#define PHONEBOOK_DATA_2_1	((U8)0x04)
#define PHONEBOOK_DATA_2_2	((U8)0x14)
#define PHONEBOOK_DATA_3_1	((U8)0x05)
#define PHONEBOOK_DATA_3_2	((U8)0x15)
#define PHONEBOOK_DATA_4_1	((U8)0x06)
#define PHONEBOOK_DATA_4_2	((U8)0x16)

extern  U8 AvErrFlg;
extern  U8 PsErrFlg;
extern  U8 PbErrFlg;
extern  U8 NaErrFlg;


extern void LAPAVCVInit(void);
extern void LAPAVCVStart(void);
extern void LAPAVCVStop(void);
extern void LAPAVCVTask(void);

extern U8 LAPAVCVGetMenuStatus(void);
extern U8 LAPAVCVGetErrorFlg(U8 MessageType);
extern void LAPAVCVSetErrorFlg(U8 MessageType, U8 Errflg);

extern void LAPAVCVPsClearBuffer(void);
extern void LAPAVCVPbClearBuffer(void);
extern void LAPAVCVAvClearBuffer(void);
extern void LAPAVCVNaviClearBuffer(void);




