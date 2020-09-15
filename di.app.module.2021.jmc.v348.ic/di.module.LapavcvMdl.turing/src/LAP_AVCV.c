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
#include "Rte_LAP_AVCV.h"
#include "CmpLib.h"

#include "DAT_COM.h"
#include "..\..\..\di.bsw.comms.turingb\CanIvi\src\CanIvi.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(LAP_AVCV)

#include "LAP_AVCV.h"

#include "deftypes.h"
#include "lib.h"
#include "tos.h"
#include "LAP_IPCL_adapter.h"

#define  AVCVPBMegNoData	(U8)5
#define  ClearBuffer		(U16)0

#define PHONESIGNAL			2
#define AUDIO				3
#define	PHONEBOOK			5
#define	NAVI				6
#define TIME_INTERVAL      15
extern  U8 DATCplTableTramesPresentes_AVCV[7];
extern  U8 NaviRevFlg;


AVCV_CtrlMeg		AVCtrlMeg;
AVCV_CtrlMeg		AVDataMeg;
AVCV_CtrlMeg		PSCtrlMeg;
AVCV_CtrlMeg		PSDataMeg;
AVCV_PBCtrlMeg		PBCtrlMeg;
AVCV_PBCtrlMeg		PBDataMeg;
AVCV_CtrlMeg		NaCtrlMeg;
AVCV_CtrlMeg		NaDataMeg;


U8 PsErrFlg;
U8 PbErrFlg;
U8 AvErrFlg;
U8 NaErrFlg;


static U8	AV_ByteBuff[8];
static U8	PS_ByteBuff[8];
static U8	PB_ByteBuff[8];
static U8	Na_ByteBuff[8];
static U16	PB_ByteStore[24]; /*Store the data both contact's name and phone number in RAM temporarily*/
static U8	PB_Ctr_RAM[32];
static U16	PB_Dat_RAM[256];
static U16	AV_Dat_RAM[28];
static U16	NumList_RAM[256]; /*Just store the phone numbe in RAM in order.*/
static U16  Na_Data_RAM[12];  /*Just store the navigation road name in RAM.*/
static U16	PS_Name_RAM[8];
static U16	PS_Numb_RAM[8];
static U8   PS_Time_RAM[8];
static U8   AV_Radio_RAM[12];
static U8   AV_Duration_RAM[8];
static U8   AV_PlayTime_RAM[8];
static U8	AV_Source;
static U8	AV_SourceStatus;

IPCL_AVCVType		sAVCV_IPCL;
IPCL_MP5NaviType	sMP5Navi_IPCL;

static	U8	u8AVNmaeLength;
static	U8	u8AVFromLength;
static	U8	u8AVRadioLength;
static	U8	u8AVDurationLength;
static	U8	u8AVPlayTimeLength;
static	U8	u8TransOrder;
static  U8  u8FrameCycle;	/*Need three cycles to sent the phone numbers.*/
static  U8  u8CallPeriod_20ms;	/*The call up task period is 10ms. But the message should be sent 20ms periodly*/
static  U8	u8NaNameLength; /*The length of road name.*/
static  U8  u8PSNameLength;
static  U8  u8PSNumbLength;
static  U8  u8PsTimeLength;
static  U8	u8PSCallSrc;
static  U8	u8PSCallType;
static  U8	u8BTConnectStatus; //0:no blue tooth connected, 1:blue tooth is connecting
static	U8	u8PhoneBookUpdate; //1:update enbale
static	U8	u8PBUpdateEnable;

static	tTOSTimer	tPBClearTimer;
static  tTOSTimer	tAudioTimer;
static  tTOSTimer	tNaviTimer;

static	tTOSTimer	tAVTimer;
static 	tTOSTimer   tPSTimer;
static  tTOSTimer	tPBTimer;
static  tTOSTimer	tNaTimer;

static tTOSTimer PhoneTimer;
static tTOSTimer AudioTimer;
static tTOSTimer NTimer;
static tTOSTimer LTimer;
static tTOSTimer CTimer;

static tTOSTimer tToutAudioTimer;
static tTOSTimer tToutPsTimer;
static tTOSTimer tToutNavTimer;

static U8 u8NaviEndFlg = 1;


static tTOSClock	AVCVCounter;
static tTOSClock	NAVICounter;

static U8 flag;
static U8 flag1;

static U8 BTphoneCmd;
static U8 BTphoneCount;

static U8 AudioChangeCmd;
static U8 AudioChangeCount;

static U8 AudioNextCmd;
static U8 AudioNextCount;

static U8 AudioLastCmd;
static U8 AudioLastCount;

static U8 AudioCenterCmd;
static U8 AudioCenterCount;


typedef enum
{
	TX_Times_Zero,
	TX_Times_First,	
	TX_Times_Second,
	TX_Times_Third,
	TX_Times_Fourth,
	TX_Times_Fifth,
	TX_Times_Sixth,
	TX_Times_Seventh,
	TX_Times_Eighth,
	TX_Times_Ninth
}eDataTX_Times;

typedef enum
{
	Cmd_None,
	Cmd_JoinCall,
	Cmd_MuteMicrophone,
	Cmd_UnMuteMicrophone,
	Cmd_PrivacyMode,
	Cmd_HandfreeMode,
	Cmd_EndCall,
	Cmd_USB,
	Cmd_BT,
	Cmd_Network_radio,
	Cmd_FM,
	Cmd_AM,
	Cmd_Online_musci,
	Cmd_Network_program,
	Cmd_NextItem,
	Cmd_LastItem,
	Cmd_Stop,
	Cmd_Start,
}eClutCtolMP5Cmd;

typedef enum
{
	Cmd_MissedCalls = 0x01,
	Cmd_ReceivedCalls,
	Cmd_DialedCalls,
	Cmd_AllCalls,
	Cmd_Position1 = 0x01,
	Cmd_Position2,
	Cmd_Position3,
	Cmd_Position4,
	Cmd_Position5,
	Cmd_Position6,
	Cmd_Position7,
	Cmd_Position8,
}eClustCallUpCmd;
    
#if 0
/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_ShortTimeHandle(void)
* 
*  PARAMETERS:  
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Handle the total time and play time of song or program.
*===============================================================================*/
static void LAP_AVCV_ShortTimeHandle(void)
{
	static	U8	dShortMin_H, dShortMin_L;
	static  U8	dShortSec_H, dShortSec_L;
	static	U8	pShortMin_H, pShortMin_L;
	static  U8	pShortSec_H, pShortSec_L;

	if(AVDataMeg.DataType==DataType_Duration)
	{
		dShortMin_H = *(AV_ByteBuff+2);
		dShortMin_L = *(AV_ByteBuff+3);
		dShortSec_H = *(AV_ByteBuff+5);
		dShortSec_L = *(AV_ByteBuff+6);
		
		if(dShortMin_H<0x30 || dShortMin_L<0x30 || dShortSec_H<0x30 || dShortSec_L<0x30)
		{	//song time duration is invalid	
			sAVCV_IPCL.AVCV_Data_03 = 0xFF;
			sAVCV_IPCL.AVCV_Data_04 = 0xFF;  
			sAVCV_IPCL.AVCV_Data_05 = 0xFF;
		}
		else
		{	//song time duration is valid
			//sAVCV_IPCL.AVCV_Data_03 = 0;
			//sAVCV_IPCL.AVCV_Data_04 = (dShortMin_H & 0x0F)*10 + (dShortMin_L & 0x0F);
			//sAVCV_IPCL.AVCV_Data_05 = (dShortSec_H & 0x0F)*10 + (dShortSec_L & 0x0F);
		}
	}
	else if(AVDataMeg.DataType==DataType_PlayTime)
	{
		pShortMin_H = *(AV_ByteBuff+2);
		pShortMin_L = *(AV_ByteBuff+3);
		pShortSec_H = *(AV_ByteBuff+5);
		pShortSec_L = *(AV_ByteBuff+6);
		
		if(pShortMin_H<0x30 || pShortMin_L<0x30 || pShortSec_H<0x30 || pShortSec_L<0x30)
		{	//song time playing is invalid
			sAVCV_IPCL.AVCV_Data_06 = 0xFF;
			sAVCV_IPCL.AVCV_Data_07 = 0xFF;  
			sAVCV_IPCL.AVCV_Data_08 = 0xFF;
		}
		else
		{	//song time playing is valid
			sAVCV_IPCL.AVCV_Data_06 = 0;
			sAVCV_IPCL.AVCV_Data_07 = (pShortMin_H & 0x0F)*10 + (pShortMin_L & 0x0F);
			sAVCV_IPCL.AVCV_Data_08 = (pShortSec_H & 0x0F)*10 + (pShortSec_L & 0x0F);
		}
	}
	else
	{
		//no operation
	}
	//Because the message of total time is delay to received,
	//During receiving,the total time maybe falsh because of data 0 was sent to GIP.
	//So if receice the total time message firt time ,then send them continually.
	sAVCV_IPCL.AVCV_Data_03 = 0;
	sAVCV_IPCL.AVCV_Data_04 = (dShortMin_H & 0x0F)*10 + (dShortMin_L & 0x0F);
	sAVCV_IPCL.AVCV_Data_05 = (dShortSec_H & 0x0F)*10 + (dShortSec_L & 0x0F);
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_LongTimeHandle(void)
* 
*  PARAMETERS:  
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Handle time of net and online program(long time).
*===============================================================================*/
static void LAP_AVCV_LongTimeHandle(void)
{
	static U8 Time_Hor_H, Time_Hor_L;
	static U8 Time_Min_H, Time_Min_L;
	static U8 Time_Sec_H, Time_Sec_L;
	static U8 Tim_Hor_H, Tim_Hor_L;
	static U8 Tim_Min_H, Tim_Min_L;
	static U8 Tim_Sec_H, Tim_Sec_L;

	if(AVDataMeg.DataType==DataType_Duration)
	{
		if(AVDataMeg.DataNum==MegDataNum21)
		{
			Time_Hor_H = *(AV_ByteBuff+2);
			Time_Hor_L = *(AV_ByteBuff+3);
			Time_Min_H = *(AV_ByteBuff+5);
			Time_Min_L = *(AV_ByteBuff+6);
		}
		else if(AVDataMeg.DataNum==MegDataNum22)
		{
			Time_Sec_H = *(AV_ByteBuff+2);
			Time_Sec_L = *(AV_ByteBuff+3);
		}
		else
		{
			//invalid time data
		}

		if(Time_Hor_H<0x30 || Time_Hor_L<0x30 || Time_Min_H<0x30 || Time_Min_L<0x30 || Time_Sec_H<0x30 || Time_Sec_L<0x30)
		{
			sAVCV_IPCL.AVCV_Data_03 = 0xFF;
			sAVCV_IPCL.AVCV_Data_04 = 0xFF;
			sAVCV_IPCL.AVCV_Data_05 = 0xFF;
		}
		else
		{
			//sAVCV_IPCL.AVCV_Data_03 = (Time_Hor_H & 0x0F)*10 + (Time_Hor_L & 0x0F);
			//sAVCV_IPCL.AVCV_Data_04 = (Time_Min_H & 0x0F)*10 + (Time_Min_L & 0x0F);
			//sAVCV_IPCL.AVCV_Data_05 = (Time_Sec_H & 0x0F)*10 + (Time_Sec_L & 0x0F);
		}		
	}
	else if(AVDataMeg.DataType==DataType_PlayTime)
	{
		if(AVDataMeg.DataNum==MegDataNum21)
		{
			Tim_Hor_H = *(AV_ByteBuff+2);
			Tim_Hor_L = *(AV_ByteBuff+3);
			Tim_Min_H = *(AV_ByteBuff+5);
			Tim_Min_L = *(AV_ByteBuff+6);
		}
		else if(AVDataMeg.DataNum==MegDataNum22)
		{
			Tim_Sec_H = *(AV_ByteBuff+2);
			Tim_Sec_L = *(AV_ByteBuff+3);
		}
		else
		{
			//invalid time data
		}

		if(Tim_Hor_H<0x30 || Tim_Hor_L<0x30 || Tim_Min_H<0x30 || Tim_Min_L<0x30 || Tim_Sec_H<0x30 || Tim_Sec_L<0x30)
		{
			sAVCV_IPCL.AVCV_Data_06 = 0xFF;
			sAVCV_IPCL.AVCV_Data_07 = 0xFF;
			sAVCV_IPCL.AVCV_Data_08 = 0xFF;
		}
		else
		{
			sAVCV_IPCL.AVCV_Data_06 = (Tim_Hor_H & 0x0F)*10 + (Tim_Hor_L & 0x0F);
			sAVCV_IPCL.AVCV_Data_07 = (Tim_Min_H & 0x0F)*10 + (Tim_Min_L & 0x0F);
			sAVCV_IPCL.AVCV_Data_08 = (Tim_Sec_H & 0x0F)*10 + (Tim_Sec_L & 0x0F);
		}
	}
	//Because the message of total time is delay to received,
	//During receiving,the total time maybe falsh because of data 0 was sent to GIP.
	//So if receice the total time message firt time ,then send them continually.
	sAVCV_IPCL.AVCV_Data_03 = (Time_Hor_H & 0x0F)*10 + (Time_Hor_L & 0x0F);
	sAVCV_IPCL.AVCV_Data_04 = (Time_Min_H & 0x0F)*10 + (Time_Min_L & 0x0F);
	sAVCV_IPCL.AVCV_Data_05 = (Time_Sec_H & 0x0F)*10 + (Time_Sec_L & 0x0F);
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_AVTimeHandle(void)
* 
*  PARAMETERS:  
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Handle the total time and play time of song or program.
*===============================================================================*/
static void LAP_AVCV_AVTimeHandle(void)
{
	if(sAVCV_IPCL.AVCV_Data_02==7)
	{
		LAP_AVCV_LongTimeHandle();
	}
	else
	{
		LAP_AVCV_ShortTimeHandle();
	}
}
#endif

/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_AVDurationHandle(void)
* 
*  PARAMETERS:  
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Handle the total time and play time of song or program.
*===============================================================================*/
static void LAP_AVCV_AVDurationHandle(void)
{
    U32 Time_Hor = 0;
	U32 Time_Min = 0;
	U32 Time_Sec = 0;
    
    if(AVCtrlMeg.DataLength <= 10)
	{
		if(AVDataMeg.DataNum==MegDataNum21)
		{
			*AV_Duration_RAM = *(AV_ByteBuff+2);  	  //00 min
			*(AV_Duration_RAM+1) = *(AV_ByteBuff+3);
			*(AV_Duration_RAM+2) = *(AV_ByteBuff+4);  //:
			*(AV_Duration_RAM+3) = *(AV_ByteBuff+5);  //00 sec
			*(AV_Duration_RAM+4) = *(AV_ByteBuff+6);
			*(AV_Duration_RAM+5) = 0;
			*(AV_Duration_RAM+6) = 0;
			*(AV_Duration_RAM+7) = 0;	
		}
		else
		{
			/*no operation*/
		}

        Time_Min = (AV_Duration_RAM[0]&0x0F)*10+(AV_Duration_RAM[1]&0x0F);
        
        Time_Sec = (AV_Duration_RAM[3]&0x0F)*10+(AV_Duration_RAM[4]&0x0F);

        sAVCV_IPCL.AVCV_Duration = Time_Min*60 + Time_Sec;
	}
	else
	{
		if(AVDataMeg.DataNum==MegDataNum21)
		{
			*AV_Duration_RAM = *(AV_ByteBuff+2);  	  //00 hour
			*(AV_Duration_RAM+1) = *(AV_ByteBuff+3);
			*(AV_Duration_RAM+2) = *(AV_ByteBuff+4);  //:
			*(AV_Duration_RAM+3) = *(AV_ByteBuff+5);  //00 min
			*(AV_Duration_RAM+4) = *(AV_ByteBuff+6);
			*(AV_Duration_RAM+5) = *(AV_ByteBuff+7);  //:		
		}
		else if(PSDataMeg.DataNum==MegDataNum22)
		{
			*(AV_Duration_RAM+6) = *(AV_ByteBuff+2);	//00 second
			*(AV_Duration_RAM+7) = *(AV_ByteBuff+3);		
		}
		else
		{
			/*no operation*/
		}

        Time_Hor = (AV_Duration_RAM[0]&0x0F)*10+(AV_Duration_RAM[1]&0x0F);
        
        Time_Min = (AV_Duration_RAM[3]&0x0F)*10+(AV_Duration_RAM[4]&0x0F);

        Time_Sec = (AV_Duration_RAM[6]&0x0F)*10+(AV_Duration_RAM[7]&0x0F);

        sAVCV_IPCL.AVCV_Duration = Time_Hor*60*60 + Time_Min*60 + Time_Sec;
	}
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_AVPlayTimeHandle(void)
* 
*  PARAMETERS:  
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Handle the total time and play time of song or program.
*===============================================================================*/
static void LAP_AVCV_AVPlayTimeHandle(void)
{
    U32 Time_Hor = 0;
	U32 Time_Min = 0;
	U32 Time_Sec = 0;
    
    if(AVCtrlMeg.DataLength <= 10)
	{
		if(AVDataMeg.DataNum==MegDataNum21)
		{
			*AV_PlayTime_RAM = *(AV_ByteBuff+2);  	  //00 min
			*(AV_PlayTime_RAM+1) = *(AV_ByteBuff+3);
			*(AV_PlayTime_RAM+2) = *(AV_ByteBuff+4);  //:
			*(AV_PlayTime_RAM+3) = *(AV_ByteBuff+5);  //00 sec
			*(AV_PlayTime_RAM+4) = *(AV_ByteBuff+6);
			*(AV_PlayTime_RAM+5) = 0;
			*(AV_PlayTime_RAM+6) = 0;
			*(AV_PlayTime_RAM+7) = 0;	
		}
		else
		{
			/*no operation*/
		}

        Time_Min = (AV_PlayTime_RAM[0]&0x0F)*10+(AV_PlayTime_RAM[1]&0x0F);
        
        Time_Sec = (AV_PlayTime_RAM[3]&0x0F)*10+(AV_PlayTime_RAM[4]&0x0F);

        sAVCV_IPCL.AVCV_PlayTime = Time_Min*60 + Time_Sec;
	}
	else
	{
		if(AVDataMeg.DataNum==MegDataNum21)
		{
			*AV_PlayTime_RAM = *(AV_ByteBuff+2);  	  //00 hour
			*(AV_PlayTime_RAM+1) = *(AV_ByteBuff+3);
			*(AV_PlayTime_RAM+2) = *(AV_ByteBuff+4);  //:
			*(AV_PlayTime_RAM+3) = *(AV_ByteBuff+5);  //00 min
			*(AV_PlayTime_RAM+4) = *(AV_ByteBuff+6);
			*(AV_PlayTime_RAM+5) = *(AV_ByteBuff+7);  //:		
		}
		else if(PSDataMeg.DataNum==MegDataNum22)
		{
			*(AV_PlayTime_RAM+6) = *(AV_ByteBuff+2);	//00 second
			*(AV_PlayTime_RAM+7) = *(AV_ByteBuff+3);		
		}
		else
		{
			/*no operation*/
		}
        
        Time_Hor = (AV_PlayTime_RAM[0]&0x0F)*10+(AV_PlayTime_RAM[1]&0x0F);
        
        Time_Min = (AV_PlayTime_RAM[3]&0x0F)*10+(AV_PlayTime_RAM[4]&0x0F);

        Time_Sec = (AV_PlayTime_RAM[6]&0x0F)*10+(AV_PlayTime_RAM[7]&0x0F);

        sAVCV_IPCL.AVCV_PlayTime = Time_Hor*60*60 + Time_Min*60 + Time_Sec;
    }
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_AVRadioProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION :  Transfer the data to buff which will be sent by IPCL.
*===============================================================================*/
static void LAP_AVCV_AVRadioHandle(void)
{
	if(AVDataMeg.DataNum==MegDataNum21)
	{
		*AV_Radio_RAM = *(AV_ByteBuff+2);
		*(AV_Radio_RAM+1)= *(AV_ByteBuff+3);
		*(AV_Radio_RAM+2)= *(AV_ByteBuff+4);
		*(AV_Radio_RAM+3)= *(AV_ByteBuff+5);
		*(AV_Radio_RAM+4)= *(AV_ByteBuff+6);
		*(AV_Radio_RAM+5)= *(AV_ByteBuff+7);
	}
	else if(AVDataMeg.DataNum==MegDataNum22)
	{		
		*(AV_Radio_RAM+6)= *(AV_ByteBuff+2);
		*(AV_Radio_RAM+7)= *(AV_ByteBuff+3);
		*(AV_Radio_RAM+8)= *(AV_ByteBuff+4);
		*(AV_Radio_RAM+9)= *(AV_ByteBuff+5);
		*(AV_Radio_RAM+10)= *(AV_ByteBuff+6);
		*(AV_Radio_RAM+11)= *(AV_ByteBuff+7);
	}
	else
	{
		/*no operation*/
	}
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_AVNameHandle(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION :  Transfer the data to buff which will be sent by IPCL.
*===============================================================================*/
static void LAP_AVCV_NameHandle(void)
{	
	if(AVDataMeg.DataNum==MegDataNum21)
	{	
		AV_Dat_RAM[0] = (U16)*(AV_ByteBuff+2)+(((U16)*(AV_ByteBuff+3))<<8&0xFF00);
		AV_Dat_RAM[1] = (U16)*(AV_ByteBuff+4)+(((U16)*(AV_ByteBuff+5))<<8&0xFF00);
		AV_Dat_RAM[2] = (U16)*(AV_ByteBuff+6)+(((U16)*(AV_ByteBuff+7))<<8&0xFF00);
	}
	else if(AVDataMeg.DataNum==MegDataNum22)
	{
		AV_Dat_RAM[3] = (U16)*(AV_ByteBuff+2)+(((U16)*(AV_ByteBuff+3))<<8&0xFF00);
		AV_Dat_RAM[4] = (U16)*(AV_ByteBuff+4)+(((U16)*(AV_ByteBuff+5))<<8&0xFF00);
		AV_Dat_RAM[5] = (U16)*(AV_ByteBuff+6)+(((U16)*(AV_ByteBuff+7))<<8&0xFF00);
	}
	else if(AVDataMeg.DataNum==MegDataNum23)
	{
		AV_Dat_RAM[6] = (U16)*(AV_ByteBuff+2)+(((U16)*(AV_ByteBuff+3))<<8&0xFF00);
		AV_Dat_RAM[7] = (U16)*(AV_ByteBuff+4)+(((U16)*(AV_ByteBuff+5))<<8&0xFF00);
		AV_Dat_RAM[8] = (U16)*(AV_ByteBuff+6)+(((U16)*(AV_ByteBuff+7))<<8&0xFF00);
	}
	else if(AVDataMeg.DataNum==MegDataNum24)
	{
        AV_Dat_RAM[9] = (U16)*(AV_ByteBuff+2)+(((U16)*(AV_ByteBuff+3))<<8&0xFF00);
		AV_Dat_RAM[10] = (U16)*(AV_ByteBuff+4)+(((U16)*(AV_ByteBuff+5))<<8&0xFF00);
		AV_Dat_RAM[11] = (U16)*(AV_ByteBuff+6)+(((U16)*(AV_ByteBuff+7))<<8&0xFF00);
	}
	else if(AVDataMeg.DataNum==MegDataNum25)
	{
        AV_Dat_RAM[12] = (U16)*(AV_ByteBuff+2)+(((U16)*(AV_ByteBuff+3))<<8&0xFF00);
		AV_Dat_RAM[13] = (U16)*(AV_ByteBuff+4)+(((U16)*(AV_ByteBuff+5))<<8&0xFF00);
    }
    else
	{	// If the length is over to display on LCD ,we shoule fliter the later data message.
		AVCtrlMeg.DataType = DataType_none;  
	}
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_AVFromHandle(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION :  Transfer the data to buff which will be sent by IPCL.
*===============================================================================*/
static void LAP_AVCV_FromHandle(void)
{
	if(AVDataMeg.DataNum==MegDataNum21)
	{
		AV_Dat_RAM[16] = (U16)*(AV_ByteBuff+2)+(((U16)*(AV_ByteBuff+3))<<8&0xFF00);
		AV_Dat_RAM[17] = (U16)*(AV_ByteBuff+4)+(((U16)*(AV_ByteBuff+5))<<8&0xFF00);
		AV_Dat_RAM[18] = (U16)*(AV_ByteBuff+6)+(((U16)*(AV_ByteBuff+7))<<8&0xFF00);
	}
	else if(AVDataMeg.DataNum==MegDataNum22)
	{
		AV_Dat_RAM[19] = (U16)*(AV_ByteBuff+2)+(((U16)*(AV_ByteBuff+3))<<8&0xFF00);
		AV_Dat_RAM[20] = (U16)*(AV_ByteBuff+4)+(((U16)*(AV_ByteBuff+5))<<8&0xFF00);
		AV_Dat_RAM[21] = (U16)*(AV_ByteBuff+6)+(((U16)*(AV_ByteBuff+7))<<8&0xFF00);
	}
	else if(AVDataMeg.DataNum==MegDataNum23)
	{
		AV_Dat_RAM[22] = (U16)*(AV_ByteBuff+2)+(((U16)*(AV_ByteBuff+3))<<8&0xFF00);
		AV_Dat_RAM[23] = (U16)*(AV_ByteBuff+4)+(((U16)*(AV_ByteBuff+5))<<8&0xFF00);
		AV_Dat_RAM[24] = (U16)*(AV_ByteBuff+6)+(((U16)*(AV_ByteBuff+7))<<8&0xFF00);
	}
	else if(AVDataMeg.DataNum==MegDataNum24)
	{
		AV_Dat_RAM[25] = (U16)*(AV_ByteBuff+2)+(((U16)*(AV_ByteBuff+3))<<8&0xFF00);
        AV_Dat_RAM[26] = (U16)*(AV_ByteBuff+4)+(((U16)*(AV_ByteBuff+5))<<8&0xFF00);
		AV_Dat_RAM[27] = (U16)*(AV_ByteBuff+6)+(((U16)*(AV_ByteBuff+7))<<8&0xFF00);
	}
	else
	{
		AVCtrlMeg.DataType = DataType_none;
	}
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_PhoneSignal_NameHandle(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION :  Just to analyze the contacts's name.
*===============================================================================*/
static void LAP_PhoneSignal_NameHandle(void)
{		
	if(PSDataMeg.DataNum==MegDataNum21)
	{			
		*PS_Name_RAM = (U16)*(PS_ByteBuff+2)+(((U16)*(PS_ByteBuff+3))<<8&0xFF00);
		*(PS_Name_RAM+1) = (U16)*(PS_ByteBuff+4)+(((U16)*(PS_ByteBuff+5))<<8&0xFF00);
		*(PS_Name_RAM+2) = (U16)*(PS_ByteBuff+6)+(((U16)*(PS_ByteBuff+7))<<8&0xFF00);
	}
	else if(PSDataMeg.DataNum==MegDataNum22)
	{
		*(PS_Name_RAM+3) = (U16)*(PS_ByteBuff+2)+(((U16)*(PS_ByteBuff+3))<<8&0xFF00);
		*(PS_Name_RAM+4) = (U16)*(PS_ByteBuff+4)+(((U16)*(PS_ByteBuff+5))<<8&0xFF00);
		*(PS_Name_RAM+5) = (U16)*(PS_ByteBuff+6)+(((U16)*(PS_ByteBuff+7))<<8&0xFF00);
	}
	else if(PSDataMeg.DataNum==MegDataNum23)
	{
		*(PS_Name_RAM+6) = (U16)*(PS_ByteBuff+2)+(((U16)*(PS_ByteBuff+3))<<8&0xFF00);
		*(PS_Name_RAM+7) = (U16)*(PS_ByteBuff+4)+(((U16)*(PS_ByteBuff+5))<<8&0xFF00);
	}
	else
	{
		/*no operation*/
	}
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_PSNumHandle(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION :  Just to analyze the contacts's phone number.
*===============================================================================*/
static void LAP_PhoneSignal_NumbHandle(void)
{		
	if(PSDataMeg.DataNum==MegDataNum21) //Phone Numbers do not need to be merged
	{
		*PS_Numb_RAM = (U16)*(PS_ByteBuff+2);
		*(PS_Numb_RAM+1) = (U16)*(PS_ByteBuff+3);
		*(PS_Numb_RAM+2) = (U16)*(PS_ByteBuff+4);
		*(PS_Numb_RAM+3) = (U16)*(PS_ByteBuff+5);
		*(PS_Numb_RAM+4) = (U16)*(PS_ByteBuff+6);
		*(PS_Numb_RAM+5) = (U16)*(PS_ByteBuff+7);
	}
	else if(PSDataMeg.DataNum==MegDataNum22)
	{
		*(PS_Numb_RAM+6) = (U16)*(PS_ByteBuff+2);
		*(PS_Numb_RAM+7) = (U16)*(PS_ByteBuff+3);
	}
	else
	{
		/*no operation*/
	}
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_PSTimeHandle(void)
* 
*  PARAMETERS:  
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Handle the time of calling.
*===============================================================================*/
static void LAP_PhoneSignal_TimeHandle(void)
{
	if(PSCtrlMeg.DataLength <= 9)
	{
		if(PSDataMeg.DataNum==MegDataNum21)
		{
			*PS_Time_RAM = *(PS_ByteBuff+2);  	  //00 min
			*(PS_Time_RAM+1) = *(PS_ByteBuff+3);
			*(PS_Time_RAM+2) = *(PS_ByteBuff+4);  //:
			*(PS_Time_RAM+3) = *(PS_ByteBuff+5);  //00 sec
			*(PS_Time_RAM+4) = *(PS_ByteBuff+6);
			*(PS_Time_RAM+5) = 0;
			*(PS_Time_RAM+6) = 0;
			*(PS_Time_RAM+7) = 0;	
		}
		else
		{
			/*no operation*/
		}
	}
	else
	{
		if(PSDataMeg.DataNum==MegDataNum21)
		{
			*PS_Time_RAM = *(PS_ByteBuff+2);  	  //00 hour
			*(PS_Time_RAM+1) = *(PS_ByteBuff+3);
			*(PS_Time_RAM+2) = *(PS_ByteBuff+4);  //:
			*(PS_Time_RAM+3) = *(PS_ByteBuff+5);  //00 min
			*(PS_Time_RAM+4) = *(PS_ByteBuff+6);
			*(PS_Time_RAM+5) = *(PS_ByteBuff+7);  //:		
		}
		else if(PSDataMeg.DataNum==MegDataNum22)
		{
			*(PS_Time_RAM+6) = *(PS_ByteBuff+2);	//00 second
			*(PS_Time_RAM+7) = *(PS_ByteBuff+3);		
		}
		else
		{
			/*no operation*/
		}
	}
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_PBDataBufferHandle(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION :  Receive the data from message and sotre in PB_ByteBuff.
*===============================================================================*/
static void LAP_AVCV_PBDataBufferHandle(void)
{
	U8	 i;
	
	if(PBDataMeg.DataNum==MegDataNum21)
	{
		for(i=0; i<6; i++)
		{
			*(PB_ByteStore+i) = *(PB_ByteBuff+i+2);
		}		
	}
	else if(PBDataMeg.DataNum==MegDataNum22)
	{
		for(i=0; i<6; i++)
		{
			*(PB_ByteStore+i+6) = *(PB_ByteBuff+i+2);
		}
	}
	else if(PBDataMeg.DataNum==MegDataNum23)
	{
		for(i=0; i<6; i++)
		{
			*(PB_ByteStore+i+12) = *(PB_ByteBuff+i+2);
		}
	}
	else if(PBDataMeg.DataNum==MegDataNum24)
	{
		for(i=0; i<6; i++)
		{
			*(PB_ByteStore+i+18) = *(PB_ByteBuff+i+2);
		}
	}
	else
	{
		//no operation
	}
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_AVCV_AVDataHandle(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION :  According to control message,handle the different tasks of data message.
*===============================================================================*/
static void LAP_AVCV_PSDataHandle(void)
{
	switch(PSDataMeg.DataType)
	{
		case PSDataType_Name:
			 {
				LAP_PhoneSignal_NameHandle();
			 }
			 break;
		case PSDataType_PhoneNum:
		 	 {
				LAP_PhoneSignal_NumbHandle();
		 	 }
		 	 break;
		case PSDataType_CallTime:
			 {
				LAP_PhoneSignal_TimeHandle();
			 }
			 break;
		default:
			 break;
	}
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_AudioVideo_DatMegRead()
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION :  To read the data message data of audio video and stroe them in RAM.
*===============================================================================*/
static void LAP_AudioVideo_DatMegRead()
{
	switch(AVDataMeg.DataType)
	{
		case DataType_Name:	
			 {
			 	LAP_AVCV_NameHandle();
			 }
			 break;
		case DataType_Author:	
			 {
			 	LAP_AVCV_FromHandle();
			 }
			 break;
		case DataType_RadioInfo:	
			 {
			 	LAP_AVCV_AVRadioHandle();
			 }
			 break;
		case DataType_Duration:	
			 {
			 	LAP_AVCV_AVDurationHandle();
			 }
			 break;
		case DataType_PlayTime:	
			 {
			 	LAP_AVCV_AVPlayTimeHandle();
			 }
			 break;
		default:
			 break;
	}
}

/*==============================================================================
*  FUNCTION NAME : LAP_PhoneBook_CtrMegRead(U8	u8Sor, U8 u8ID, U8 u8ContactsNum, U8 u8NumL, U8 u8NamL)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION :  To read the control message data of phone book and stroe data in RAM.
*===============================================================================*/
static void LAP_PhoneBook_CtrMegRead(U8	u8Sor, U8 u8ID, U8 u8ContactsNum, U8 u8NumL, U8 u8NamL)
{
	U8 u8i, u8j, u8m, u8n;

	u8i = u8Sor;
	u8j = u8ID;
	u8m = u8NumL;
	u8n = u8NamL;
	
	if(u8n)
	{
        /*
		if(*(PB_Ctr_RAM+(u8i-1)*8+u8j-1) != 0)
		{
			*(PB_Ctr_RAM+(u8i-1)*8+u8j-1) = 0;
		}
		*/
		*(PB_Ctr_RAM+(u8i-1)*8+u8j-1) = (u8ContactsNum << 4) & 0xF0;
	}
	else
	{
        if (u8m!=0)
        {
            /*
            if(*(PB_Ctr_RAM+(u8i-1)*8+u8j-1) != 0)
    		{
    			*(PB_Ctr_RAM+(u8i-1)*8+u8j-1) = 0;
    		}
    		*/
    		*(PB_Ctr_RAM+(u8i-1)*8+u8j-1) = (u8ContactsNum << 4) | u8m;
        }
	}	
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_PhoneBook_DatMegRead(U8 u8char, U8 u8Order)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION :  Read the data from data message and store them in RAM.
*===============================================================================*/
static void LAP_PhoneBook_DatMegRead(U8 u8Sor, U8 u8ID, U8 u8NumOrder, U8 u8NamOrder)
{
	U8	u8i, u8j, u8k, u8m, u8n, u8s;

	u8i = u8Sor;
	u8j = u8ID;
	u8k = u8NumOrder;
	u8m = u8NamOrder;

	if(u8m)  //Store the contact's name in RAM.
	{		
		for(u8s=0; u8s<(u8m/2); u8s++) //filter unused data through corresponding data length
		{
            /*
			if(u8s == 0 && *(PB_Dat_RAM+(u8i-1)*64+(u8j-1)*8+u8s) != 0)
			{
				mLIBmemset((PB_Dat_RAM+(u8i-1)*64+(u8j-1)*8+u8s), ClearBuffer, sizeof(PB_Dat_RAM[0])*8);
			}
			*/
			*(PB_Dat_RAM+(u8i-1)*64+(u8j-1)*8+u8s) = (U16)*(PB_ByteStore+u8k+u8s*2) + (((U16)*(PB_ByteStore+u8k+u8s*2+1))<<8&0xFF00);
		}		
	}
	else  //Store the contact's data in RAM.
	{
		if(u8i!=0)
		{
			for(u8s=0; u8s<u8k; u8s++) //filter unused data through corresponding data length
			{
                /*
				if(u8s == 0 && *(PB_Dat_RAM+(u8i-1)*64+(u8j-1)*8+u8s) != 0)
				{
					mLIBmemset((PB_Dat_RAM+(u8i-1)*64+(u8j-1)*8+u8s), ClearBuffer, sizeof(PB_Dat_RAM[0])*8);
				}
				*/
				PB_ByteStore[u8k] = 0;	//clear the unused data
				*(PB_Dat_RAM+(u8i-1)*64+(u8j-1)*8+u8s) = (U16)*(PB_ByteStore+u8s);
			}
		}
		else
		{
			//no operation
		}
	}
	/*Read the data in temp RAM PB_ByteStore and store them in RAM of NumList_RAM.*/
	if(u8i==1)
	{
		for(u8n=0; u8n<u8k; u8n++)
		{
			*(NumList_RAM+192+(u8j-1)*8+u8n) = *(PB_ByteStore+u8n);
		}	
	}
	else if(u8i==2)
	{
		for(u8n=0; u8n<u8k; u8n++)
		{
			*(NumList_RAM+64+(u8j-1)*8+u8n) = *(PB_ByteStore+u8n);
		}
	}
	else if(u8i==3)
	{
		for(u8n=0; u8n<u8k; u8n++)
		{
			*(NumList_RAM+128+(u8j-1)*8+u8n) = *(PB_ByteStore+u8n);
		}
	}
	else if(u8i==4)
	{
		for(u8n=0; u8n<u8k; u8n++)
		{
			*(NumList_RAM+0+(u8j-1)*8+u8n) = *(PB_ByteStore+u8n);
		}
	}
	else
	{
		//no state
	}
}

/*==============================================================================
*  FUNCTION NAME : static void LAP_NaviSign_NameHandle()
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION :  Handlding the road name of navigation.At the moment, at most
*						 ten chinses characters or twelve English letters are displayed.					 
*===============================================================================*/
static void LAP_NaviSign_NameHandle(void)
{	
	if(NaDataMeg.DataNum==MegDataNum21)
	{						
		*Na_Data_RAM = (U16)*(Na_ByteBuff+2)+(((U16)*(Na_ByteBuff+3))<<8&0xFF00);
		*(Na_Data_RAM+1) = (U16)*(Na_ByteBuff+4)+(((U16)*(Na_ByteBuff+5))<<8&0xFF00);
		*(Na_Data_RAM+2) = (U16)*(Na_ByteBuff+6)+(((U16)*(Na_ByteBuff+7))<<8&0xFF00);		
	}
	else if(NaDataMeg.DataNum==MegDataNum22)
	{
		*(Na_Data_RAM+3) = (U16)*(Na_ByteBuff+2)+(((U16)*(Na_ByteBuff+3))<<8&0xFF00);
		*(Na_Data_RAM+4) = (U16)*(Na_ByteBuff+4)+(((U16)*(Na_ByteBuff+5))<<8&0xFF00);
		*(Na_Data_RAM+5) = (U16)*(Na_ByteBuff+6)+(((U16)*(Na_ByteBuff+7))<<8&0xFF00);
	}
	else if(NaDataMeg.DataNum==MegDataNum23)
	{
		*(Na_Data_RAM+6) = (U16)*(Na_ByteBuff+2)+(((U16)*(Na_ByteBuff+3))<<8&0xFF00);
		*(Na_Data_RAM+7) = (U16)*(Na_ByteBuff+4)+(((U16)*(Na_ByteBuff+5))<<8&0xFF00);
		*(Na_Data_RAM+8) = (U16)*(Na_ByteBuff+6)+(((U16)*(Na_ByteBuff+7))<<8&0xFF00);
	}
	else if(NaDataMeg.DataNum==MegDataNum24)
	{
		*(Na_Data_RAM+9) = (U16)*(Na_ByteBuff+2)+(((U16)*(Na_ByteBuff+3))<<8&0xFF00);
		*(Na_Data_RAM+10) = (U16)*(Na_ByteBuff+4)+(((U16)*(Na_ByteBuff+5))<<8&0xFF00);
		*(Na_Data_RAM+11) = (U16)*(Na_ByteBuff+6)+(((U16)*(Na_ByteBuff+7))<<8&0xFF00);
	}
	else
	{
		/*no operation*/
	}

}

/*==============================================================================
*  FUNCTION NAME : void LAP_AVCV_AudioVideoProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : The process of handle audio video information.
*===============================================================================*/

void LAP_AVCV_AudioVideoProcess(void)
{

	static Bool u8AvCroFlg = cFalse;
	static U8 U8AvLenErrFlg = 0;

    //Timeout 
	if((TOSIsTimerStarted((&tToutAudioTimer))) && (TOSIsTimerElapsed((&tToutAudioTimer),mTOSConvMsInTimerTick(2000))))
	{
        AV_SourceStatus = 0;
        AV_Source = 0;
        
        mLIBmemset(&AV_Dat_RAM, ClearBuffer, sizeof(AV_Dat_RAM));
        mLIBmemset(&AV_Radio_RAM, ClearBuffer, sizeof(AV_Radio_RAM));
        mLIBmemset(&AV_Duration_RAM, ClearBuffer, sizeof(AV_Duration_RAM));
        mLIBmemset(&AV_PlayTime_RAM, ClearBuffer, sizeof(AV_PlayTime_RAM));
    
		TOSStopTimer((&tToutAudioTimer));
		return;
	}
    
	//Timeout 
	if((TOSIsTimerStarted((&tAVTimer))) && (TOSIsTimerElapsed((&tAVTimer),mTOSConvMsInTimerTick(50))))
	{
		IC_Audio_RX_0x389_Byte_1 = 0x7F;
		IC_Audio_RX_0x389_Byte_2 = 0x37;
		DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_Audio_RX_0x389);
		TOSStopTimer((&tAVTimer));
		AVCtrlMeg.DataCount = 0;//LAPAVCVAvClearBuffer();
		AvErrFlg = 1;
		return;
	}

	if(DATCplTableTramesPresentes_AVCV[AUDIO] == 1 || U8AvLenErrFlg == 1)
	{
		U8AvLenErrFlg = 0;

		DATCplTableTramesPresentes_AVCV[AUDIO] = 0;

		*AV_ByteBuff = MP5_Audio_TX_0x381_Byte_1;
		*(AV_ByteBuff+1) = MP5_Audio_TX_0x381_Byte_2;
		*(AV_ByteBuff+2) = MP5_Audio_TX_0x381_Byte_3;
		*(AV_ByteBuff+3) = MP5_Audio_TX_0x381_Byte_4;
		*(AV_ByteBuff+4) = MP5_Audio_TX_0x381_Byte_5;
		*(AV_ByteBuff+5) = MP5_Audio_TX_0x381_Byte_6;
		*(AV_ByteBuff+6) = MP5_Audio_TX_0x381_Byte_7;
		*(AV_ByteBuff+7) = MP5_Audio_TX_0x381_Byte_8;

		if(((*AV_ByteBuff>=0x80) && (*(AV_ByteBuff+1) != 0)) || ((*AV_ByteBuff>0x80)))  //control meg receive
		{
			/*
			AVCtrlMeg.DataLength = *AV_ByteBuff & 0x7F;	
			sAVCV_IPCL.AVCV_Data_01 = (U16) *(AV_ByteBuff+1);
			sAVCV_IPCL.AVCV_Data_02 = (U16) *(AV_ByteBuff+2);		
			AVCtrlMeg.DataType = (U16) *(AV_ByteBuff+3);
			*/
			AVCtrlMeg.DataLength = (U16) ((U16)(*(AV_ByteBuff+1)) | (((U16)(*AV_ByteBuff & 0x7F)) << 8));
			AV_SourceStatus = *(AV_ByteBuff+2);
			AV_Source = *(AV_ByteBuff+3);
			//IPCL_AVCV_Data.AVCV_Data_01 = (U16) *(AV_ByteBuff+2);
			//IPCL_AVCV_Data.AVCV_Data_02 = (U16) *(AV_ByteBuff+3);		
			AVCtrlMeg.DataType = (U16) *(AV_ByteBuff+4);

			if(AVCtrlMeg.DataCount != 0)
			{
				//Data length mismatch
				IC_Audio_RX_0x389_Byte_1 = 0x7F;
				IC_Audio_RX_0x389_Byte_2 = 0x13;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_Audio_RX_0x389);
				AVCtrlMeg.DataCount = 0;//LAPAVCVAvClearBuffer();
				U8AvLenErrFlg = 1;
				AvErrFlg = 1;
				return;
			}
			
			AVCtrlMeg.DataCount = (AVCtrlMeg.DataLength - 3)/7;
			if((AVCtrlMeg.DataLength - 3)%7 == 0)
			{
				;
			}
			else
			{
				AVCtrlMeg.DataCount += 1;
			}

			/*If there is no valid data in control message, then clear the buffer and executive other task.*/
			if(AVCtrlMeg.DataLength>3)
			{
				u8AvCroFlg = cTrue;
				/*Means that the control message is valid and transmit the respond code */
				IC_Audio_RX_0x389_Byte_1 = 0x30;
				IC_Audio_RX_0x389_Byte_2 = 0x00;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_Audio_RX_0x389);
				if(*(AV_ByteBuff+2) != 0x00)
				{
					if(TOSIsTimerStarted((&tAVTimer)))
					{
						;
					}
					else
					{
						TOSStartTimer((&tAVTimer));
					}
				}
                
                TOSStartTimer((&tToutAudioTimer));
				
				/*Calculate the data length of programe's name and author from bule tooth.*/
				if(AVCtrlMeg.DataType==DataType_Name)
				{
					if((AVCtrlMeg.DataLength-3)%7==0)
					{
						u8AVNmaeLength = AVCtrlMeg.DataLength - (AVCtrlMeg.DataLength-3)/7 - 3;
						u8AVNmaeLength /= 2;
					}
					else
					{
						u8AVNmaeLength = AVCtrlMeg.DataLength - (AVCtrlMeg.DataLength-3)/7 - 4;
						u8AVNmaeLength /= 2;
					}

					if(u8AVNmaeLength>14)
					{
						u8AVNmaeLength = 14;
					}
					else
					{
						//invalid length
					}
				}
				else if(AVCtrlMeg.DataType==DataType_Author)
				{
					if((AVCtrlMeg.DataLength-3)%7==0)
					{
						u8AVFromLength = AVCtrlMeg.DataLength - (AVCtrlMeg.DataLength-3)/7 - 3;
						u8AVFromLength /= 2;
					}
					else
					{
						u8AVFromLength = AVCtrlMeg.DataLength - (AVCtrlMeg.DataLength-3)/7 - 4;
						u8AVFromLength /= 2;
					}

					if(u8AVFromLength>12)
					{
						u8AVFromLength = 12;
					}
					else
					{
						//invalid length
					}
				}
				else if(AVCtrlMeg.DataType==DataType_RadioInfo)
				{
					if((AVCtrlMeg.DataLength-3)%7==0)
					{
						u8AVRadioLength = AVCtrlMeg.DataLength - (AVCtrlMeg.DataLength-3)/7 - 3;
					}
					else
					{
						u8AVRadioLength = AVCtrlMeg.DataLength - (AVCtrlMeg.DataLength-3)/7 - 4;
					}

					if(u8AVRadioLength>8)
					{
						u8AVRadioLength = 8;
					}
					else
					{
						//invalid length
					}
				}
                else if(AVCtrlMeg.DataType==DataType_Duration)
				{
					if((AVCtrlMeg.DataLength-3)%7==0)
					{
						u8AVDurationLength = AVCtrlMeg.DataLength - (AVCtrlMeg.DataLength-3)/7 - 3;
					}
					else
					{
						u8AVDurationLength = AVCtrlMeg.DataLength - (AVCtrlMeg.DataLength-3)/7 - 4;
					}

					if(u8AVDurationLength>8)
					{
						u8AVDurationLength = 8;
					}
					else
					{
						//invalid length
					}
				}
                else if(AVCtrlMeg.DataType==DataType_PlayTime)
				{
					if((AVCtrlMeg.DataLength-3)%7==0)
					{
						u8AVPlayTimeLength = AVCtrlMeg.DataLength - (AVCtrlMeg.DataLength-3)/7 - 3;
					}
					else
					{
						u8AVPlayTimeLength = AVCtrlMeg.DataLength - (AVCtrlMeg.DataLength-3)/7 - 4;
					}

					if(u8AVPlayTimeLength>8)
					{
						u8AVPlayTimeLength = 8;
					}
					else
					{
						//invalid length
					}
				}
				else
				{
					/*no operation*/
				}
			}
			else
			{
				/*no operation*/
			}
		}
		else  //data meg receive
		{
			
			AVDataMeg.DataNum = *AV_ByteBuff;
			AVDataMeg.DataType = *(AV_ByteBuff+1);
			
			if(AVCtrlMeg.DataCount == 0)
			{
				u8AvCroFlg = cFalse;
			}
			else
			{
				AVCtrlMeg.DataCount = AVCtrlMeg.DataCount-1;
			}
			


			if(AVDataMeg.DataType != AVCtrlMeg.DataType)
			{
				
				IC_Audio_RX_0x389_Byte_1 = 0x7F;
				IC_Audio_RX_0x389_Byte_2 = 0x73;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_Audio_RX_0x389);
				AvErrFlg = 1;
			}

			else if((u8AvCroFlg == cFalse) && (AVDataMeg.DataNum != 0))
			{
				IC_Audio_RX_0x389_Byte_1 = 0x7F;
				IC_Audio_RX_0x389_Byte_2 = 0x24;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_Audio_RX_0x389);
				AvErrFlg = 1;
			}
			else
			{
				LAP_AudioVideo_DatMegRead();
			}
			TOSStopTimer(&tAVTimer);

			TOSStartTimer((&tToutAudioTimer));
		}
	}
}


/*==============================================================================
*  FUNCTION NAME : void LAP_AudioVideo_TransmitProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Transfer the data from AV_Dat_RAM to IPCL buffer.
*===============================================================================*/
void LAP_AudioVideo_TransmitProcess(void)
{
	U8	u8AVCount;

	sAVCV_IPCL.AVCV_MesgType = 0x01;

	sAVCV_IPCL.AVCV_AudioStatus = AV_SourceStatus;
	sAVCV_IPCL.AVCV_AudioSrc = AV_Source;

	// name
	for(u8AVCount=0; u8AVCount<u8AVNmaeLength; u8AVCount++) /*Transfer name character data to buffer to wait to be sent.*/
	{
		sAVCV_IPCL.AVCV_Audio_Char[u8AVCount] = *(AV_Dat_RAM+u8AVCount);
	}

	for(u8AVCount=u8AVNmaeLength; u8AVCount<16; u8AVCount++) /*Clear unused data buffer which waiting to be sent.*/
	{
		sAVCV_IPCL.AVCV_Audio_Char[u8AVCount] = 0;
	}

	// author
	for(u8AVCount=0; u8AVCount<u8AVFromLength; u8AVCount++) /*Transfer phone number data to buffer to wait to be sent.*/
	{
		sAVCV_IPCL.AVCV_Audio_Char[16+u8AVCount] = *(AV_Dat_RAM+16+u8AVCount);
	}

	for(u8AVCount=u8AVFromLength; u8AVCount<14; u8AVCount++) /*Clear unused data buffer which waiting to be sent.*/
	{
		sAVCV_IPCL.AVCV_Audio_Char[16+u8AVCount] = 0;
	}

	// radioInfo
	for (u8AVCount = 0; u8AVCount<u8AVRadioLength; u8AVCount++)
	{
		sAVCV_IPCL.AVCV_Audio_Char[30+u8AVCount] = (U16)(*(AV_Radio_RAM+u8AVCount));
	}
	
	for(u8AVCount=u8AVRadioLength; u8AVCount<10; u8AVCount++)	/*filter the invalid data in arrary*/
	{
		sAVCV_IPCL.AVCV_Audio_Char[30+u8AVCount] = 0;		
	}

    // duration
    for (u8AVCount = 0; u8AVCount < u8AVDurationLength; u8AVCount++)
	{
		sAVCV_IPCL.AVCV_Audio_Char[40+u8AVCount] = AV_Duration_RAM[u8AVCount];
	}
    
    // playTime 
    for (u8AVCount = 0; u8AVCount < u8AVPlayTimeLength; u8AVCount++)
	{
		sAVCV_IPCL.AVCV_Audio_Char[48+u8AVCount] = AV_PlayTime_RAM[u8AVCount];
	}
}

/*==============================================================================
*  FUNCTION NAME : void LAP_AVCV_PhoneSignalProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : The process of handle phone signals.
*===============================================================================*/

void LAP_AVCV_PhoneSignalProcess(void)
{
	static Bool u8PsCroFlg = cFalse;
	static U8 U8PsLenErrFlg = 0;

    //Timeout 
	if((TOSIsTimerStarted((&tToutPsTimer))) && (TOSIsTimerElapsed((&tToutPsTimer),mTOSConvMsInTimerTick(2000))))
	{
        u8PSCallSrc = 0;
        u8PSCallType = 0;
        
        mLIBmemset(&PS_Name_RAM, ClearBuffer, sizeof(PS_Name_RAM));
    	mLIBmemset(&PS_Numb_RAM, ClearBuffer, sizeof(PS_Numb_RAM));
    	mLIBmemset(&PS_Time_RAM, ClearBuffer, sizeof(PS_Time_RAM));
    
		TOSStopTimer((&tToutPsTimer));
		return;
	}
    
	if((TOSIsTimerStarted((&tPSTimer))) && (TOSIsTimerElapsed((&tPSTimer),mTOSConvMsInTimerTick(50))))
	{
		//Timeout Res
		IC_Phone_RX_0x388_Byte_1 = 0x7F;
		IC_Phone_RX_0x388_Byte_2 = 0x37;
		DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_Phone_RX_0x388);
		TOSStopTimer((&tPSTimer));	
		PSCtrlMeg.DataCount = 0;//LAPAVCVPsClearBuffer();
		PsErrFlg = 1;
		return;
	}

	if(DATCplTableTramesPresentes_AVCV[PHONESIGNAL] == 1 || U8PsLenErrFlg == 1)
	{

		DATCplTableTramesPresentes_AVCV[PHONESIGNAL] = 0;
		U8PsLenErrFlg = 0;

		*PS_ByteBuff = MP5_Phone_TX_0x380_Byte_1;
		*(PS_ByteBuff+1) = MP5_Phone_TX_0x380_Byte_2;
		*(PS_ByteBuff+2) = MP5_Phone_TX_0x380_Byte_3;
		*(PS_ByteBuff+3) = MP5_Phone_TX_0x380_Byte_4;
		*(PS_ByteBuff+4) = MP5_Phone_TX_0x380_Byte_5;
		*(PS_ByteBuff+5) = MP5_Phone_TX_0x380_Byte_6;
		*(PS_ByteBuff+6) = MP5_Phone_TX_0x380_Byte_7;
		*(PS_ByteBuff+7) = MP5_Phone_TX_0x380_Byte_8;
		
		if(*PS_ByteBuff>0x80)  //control meg receive
		{
			
			PSCtrlMeg.DataLength = *PS_ByteBuff & 0x7F;
			PSCtrlMeg.DataType = *(PS_ByteBuff+2);
			
			//if((PSCtrlMeg.DataCount != 0) && (PSCtrlMeg.OldDataType != PSCtrlMeg.DataType)) 
			if(PSCtrlMeg.DataCount != 0)
			{
				//Data length mismatch
				IC_Phone_RX_0x388_Byte_1 = 0x7F;
				IC_Phone_RX_0x388_Byte_2 = 0x13;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_Phone_RX_0x388);
				PSCtrlMeg.DataCount = 0;//LAPAVCVPsClearBuffer();
				U8PsLenErrFlg = 1;
				PsErrFlg = 1;
				return;
			}
			
			u8PSCallSrc = *(PS_ByteBuff+1); //data source state
			u8BTConnectStatus = *(PS_ByteBuff+1);
			u8PSCallType = *(PS_ByteBuff+2); //data type

			
			PSCtrlMeg.DataCount = (PSCtrlMeg.DataLength - 2)/7;
			if(((PSCtrlMeg.DataLength - 2)%7) == 0)
			{
				;
			}
			else
			{
				PSCtrlMeg.DataCount += 1;
			}

			if(PSCtrlMeg.DataLength>2)
			{

				u8PsCroFlg = cTrue;
			
				/*Means that the control message is valid and transmit the respond code */
				IC_Phone_RX_0x388_Byte_1 = 0x30;
				IC_Phone_RX_0x388_Byte_2 = 0x00;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_Phone_RX_0x388);
				if((*(PS_ByteBuff+1) != 0x01) && (*(PS_ByteBuff+1) != 0x07) && (*(PS_ByteBuff+1) != 0x00))
				{
					if(TOSIsTimerStarted((&tPSTimer)))
					{
						;
					}
					else
					{
						TOSStartTimer((&tPSTimer));
					}
				}

                TOSStartTimer((&tToutPsTimer));

				if(PSCtrlMeg.DataType==PSDataType_Name)
				{
					if((PSCtrlMeg.DataLength-2)%7==0)
					{
						u8PSNameLength = PSCtrlMeg.DataLength - (PSCtrlMeg.DataLength-2)/7 - 2;
						u8PSNameLength /= 2;
					}
					else
					{
						u8PSNameLength = PSCtrlMeg.DataLength - (PSCtrlMeg.DataLength-2)/7 - 3;
						u8PSNameLength /= 2;
					}

					if(u8PSNameLength>8)
					{
						u8PSNameLength = 8;				
					}
					else
					{
						/*keep real length*/
					}
				}
				else if(PSCtrlMeg.DataType==PSDataType_PhoneNum)
				{
					if((PSCtrlMeg.DataLength-2)>14)
					{	//minus 2 means control message unused data length
						//minus 3 means the number of head of frame  
						u8PSNumbLength = PSCtrlMeg.DataLength - 2 - 3;  
					}
					if((PSCtrlMeg.DataLength-2)>7)
					{	//minus 2 means control message unused data length
						//next minus 2 means the number of head of frame
						u8PSNumbLength = PSCtrlMeg.DataLength - 2 - 2;
					}
					else
					{	//minus 2 means control message unused data length
						//minus 1 means the number of head of frame
						u8PSNumbLength = PSCtrlMeg.DataLength - 2 - 1;
					}

					if(u8PSNumbLength>16)
					{
						u8PSNumbLength = 16;
					}
					else
					{
						/*keep real length*/
					}
				}
				else if (PSCtrlMeg.DataType == PSDataType_CallTime)
				{
					if ((PSCtrlMeg.DataLength-2)%7 == 0)
					{
						u8PsTimeLength = PSCtrlMeg.DataLength - (PSCtrlMeg.DataLength-2)/7 - 2;
					}
					else
					{
						u8PsTimeLength = PSCtrlMeg.DataLength - (PSCtrlMeg.DataLength-2)/7 - 3;
					}

					if (u8PsTimeLength > 8)
					{
						u8PsTimeLength = 8;				
					}
					else
					{
						/*keep real length*/
					}
				}
				else
				{
					//u8PSNameLength = 0;
					//u8PSNumbLength = 0;
				}
			}
			else
			{
				/*no operation*/	
			}
		}
		else  //data meg receive
		{
			PSDataMeg.DataNum = *PS_ByteBuff;
			PSDataMeg.DataType = *(PS_ByteBuff+1);


			if(PSCtrlMeg.DataCount == 0)
			{
				u8PsCroFlg = cFalse;
			}
			else
			{
				PSCtrlMeg.DataCount = PSCtrlMeg.DataCount-1;
			}
			
			if(PSDataMeg.DataType != PSCtrlMeg.DataType)
			{
				
				IC_Phone_RX_0x388_Byte_1 = 0x7F;
				IC_Phone_RX_0x388_Byte_2 = 0x73;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_Phone_RX_0x388);
				PsErrFlg = 1;
			}

			else if((u8PsCroFlg == cFalse) && (PSDataMeg.DataNum!=0))
			{
				IC_Phone_RX_0x388_Byte_1 = 0x7F;
				IC_Phone_RX_0x388_Byte_2 = 0x24;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_Phone_RX_0x388);
				PsErrFlg = 1;
				
			}
			else
			{
				LAP_AVCV_PSDataHandle();
				
			}
			TOSStopTimer(&tPSTimer);

            TOSStartTimer((&tToutPsTimer));		
		}
	}

}

/*==============================================================================
*  FUNCTION NAME : void LAP_AVCV_PSTransmitProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Transfer the data from PS_Name_RAM and PS_Numb_RAM to IPCL buffer.
*===============================================================================*/
void LAP_AVCV_PSTransmitProcess(void)
{
	U8  u8PSCount;
	U8  u8PSLength;

	u8PSLength = u8PSNumbLength; 
	//u8PSLength /= 2;
	sAVCV_IPCL.AVCV_MesgType = 0x02; 
	if(u8PSCallSrc<=7)
	{
		sAVCV_IPCL.AVCV_PsCallSrc = u8PSCallSrc; //filter invalid data source
	}
	else
	{
		//no state
	}
	sAVCV_IPCL.AVCV_PsCallType = u8PSCallType;
	
	for(u8PSCount=0; u8PSCount<u8PSNameLength; u8PSCount++)
	{
		sAVCV_IPCL.AVCV_Ps_Char[u8PSCount] = PS_Name_RAM[u8PSCount];
	}
	
	for(u8PSCount=u8PSNameLength; u8PSCount<16; u8PSCount++)
	{
		sAVCV_IPCL.AVCV_Ps_Char[u8PSCount] = 0;
	}

	if(u8PSNumbLength%2==0)
	{	//even number needn't change
		for(u8PSCount=0; u8PSCount<u8PSLength; u8PSCount++)
		{
			sAVCV_IPCL.AVCV_Ps_Char[16+u8PSCount] = PS_Numb_RAM[u8PSCount];
		}	

		for(u8PSCount=u8PSLength; u8PSCount<16; u8PSCount++)
		{
			sAVCV_IPCL.AVCV_Ps_Char[16+u8PSCount] = 0;
		}
	}
	else
	{	//old number should chear unused data
		PS_Numb_RAM[u8PSLength] &= 0x00FF;  
		for(u8PSCount=0; u8PSCount<(u8PSLength+1); u8PSCount++)
		{
			sAVCV_IPCL.AVCV_Ps_Char[16+u8PSCount] = PS_Numb_RAM[u8PSCount];
		}

		for(u8PSCount=(u8PSLength+1); u8PSCount<16; u8PSCount++)
		{
			sAVCV_IPCL.AVCV_Ps_Char[16+u8PSCount] = 0;
		}
	}		
	
	if(sAVCV_IPCL.AVCV_PsCallSrc==4 || sAVCV_IPCL.AVCV_PsCallSrc==5 || sAVCV_IPCL.AVCV_PsCallSrc==6)
	{
		for (u8PSCount = 0; u8PSCount < u8PsTimeLength; u8PSCount++)
		{
			sAVCV_IPCL.AVCV_Ps_Char[32+u8PSCount] = PS_Time_RAM[u8PSCount];
		}
	}
	else
	{	
		mLIBmemset(&sAVCV_IPCL.AVCV_Ps_Char[32], ClearBuffer, 16); /*If there is no call, we should set call time in zero.*/
	}
}

/*==============================================================================
*  FUNCTION NAME : void LAP_AVCV_PhoneBookProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : The process of handle phone signals.
*===============================================================================*/
void LAP_AVCV_PhoneBookProcess(void)
{	
	U8	u8n;
	static Bool u8PbCroFlg = cFalse;
	static U8 U8PbLenErrFlg = 0;
	
	if((TOSIsTimerStarted((&tPBTimer))) && (TOSIsTimerElapsed((&tPBTimer),mTOSConvMsInTimerTick(50))))
	{
		//Timeout Res
		IC_PhoneNum_RX_0x38B_Byte_1 = 0x7F;
		IC_PhoneNum_RX_0x38B_Byte_2 = 0x37;
		DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_PhoneNum_RX_0x38B);
		PBCtrlMeg.DataCount = 0;//LAPAVCVPbClearBuffer();		
		TOSStopTimer((&tPBTimer));
		PbErrFlg = 1;
		return;
	}

	if(DATCplTableTramesPresentes_AVCV[PHONEBOOK] == 1 || U8PbLenErrFlg == 1)
	{
		U8PbLenErrFlg = 0;

		DATCplTableTramesPresentes_AVCV[PHONEBOOK] = 0;
	
		*PB_ByteBuff = MP5_PhoneNum_TX_0x383_Byte_1;
		*(PB_ByteBuff+1) = MP5_PhoneNum_TX_0x383_Byte_2;
		*(PB_ByteBuff+2) = MP5_PhoneNum_TX_0x383_Byte_3;
		*(PB_ByteBuff+3) = MP5_PhoneNum_TX_0x383_Byte_4;
		*(PB_ByteBuff+4) = MP5_PhoneNum_TX_0x383_Byte_5;
		*(PB_ByteBuff+5) = MP5_PhoneNum_TX_0x383_Byte_6;
		*(PB_ByteBuff+6) = MP5_PhoneNum_TX_0x383_Byte_7;
		*(PB_ByteBuff+7) = MP5_PhoneNum_TX_0x383_Byte_8;
		
		if(*PB_ByteBuff>0x80)  //control message receive
		{
			for(u8n=0; u8n<24; u8n++)
			{
				*(PB_ByteStore+u8n) = ClearBuffer;
			}
			
			PBCtrlMeg.DataLength = *PB_ByteBuff & 0x7F;
			PBCtrlMeg.DataSource = *(PB_ByteBuff+1);
			PBCtrlMeg.ContactsID = *(PB_ByteBuff+2);
			PBCtrlMeg.ContactsNum = *(PB_ByteBuff+3);
			PBCtrlMeg.PhNumLength = *(PB_ByteBuff+4);
			PBCtrlMeg.ContactsLength = *(PB_ByteBuff+5);

			if(PBCtrlMeg.DataCount != 0)
			{
				//Data length mismatch
				IC_PhoneNum_RX_0x38B_Byte_1 = 0x7F;
				IC_PhoneNum_RX_0x38B_Byte_2 = 0x13;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_PhoneNum_RX_0x38B);;
				PBCtrlMeg.DataCount = 0;//LAPAVCVPbClearBuffer();
				U8PbLenErrFlg = 1;
				PbErrFlg = 1;
				return;
			}

			PBCtrlMeg.DataCount = (PBCtrlMeg.DataLength - 5)/7;
			if((PBCtrlMeg.DataLength - 5)%7 == 0)
			{
				;
			}
			else
			{
				PBCtrlMeg.DataCount += 1;
			}

			if(PBCtrlMeg.DataSource==1 && PBCtrlMeg.ContactsID==1)
			{
				u8PhoneBookUpdate++;
			}
			else
			{
				//no operation
			}

			if(u8PhoneBookUpdate==1)
			{
				mLIBmemset(&PB_ByteStore, ClearBuffer, sizeof(PB_ByteStore));
				//mLIBmemset(&PB_Ctr_RAM, ClearBuffer, sizeof(PB_Ctr_RAM));
				mLIBmemset(&PB_Dat_RAM, ClearBuffer, sizeof(PB_Dat_RAM));
				mLIBmemset(&NumList_RAM, ClearBuffer, sizeof(NumList_RAM));
				u8PBUpdateEnable = cFalse;
			}
			else
			{
				//Blue tooth is connecting, so needn't to do anything
			}
			
			if(PBCtrlMeg.DataLength>AVCVPBMegNoData)
			{
				u8PbCroFlg = cTrue;
				IC_PhoneNum_RX_0x38B_Byte_1 = 0x30;
				IC_PhoneNum_RX_0x38B_Byte_2 = 0x00;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_PhoneNum_RX_0x38B);	
				if(*(PB_ByteBuff+1) != 0x00)
				{
					if(TOSIsTimerStarted((&tPBTimer)))
					{
						;
					}
					else
					{
						TOSStartTimer((&tPBTimer));
					}
				}
			}
			else
			{
				//no operation
			}	

			if((PBCtrlMeg.DataSource<=0x04)&&(PBCtrlMeg.ContactsID>=0x01)&&(PBCtrlMeg.ContactsID<=0x08)&&
			   (PBCtrlMeg.PhNumLength<=0x08)&&(PBCtrlMeg.ContactsLength<=0x10))
			{
				LAP_PhoneBook_CtrMegRead(PBCtrlMeg.DataSource, PBCtrlMeg.ContactsID, PBCtrlMeg.ContactsNum, PBCtrlMeg.PhNumLength, PBCtrlMeg.ContactsLength);
			}
			else
			{
				//don't processing the invalid data
			}
			TOSStopTimer(&tPBClearTimer);
		}
		else  //data message receive
		{
			PBDataMeg.DataNum = *PB_ByteBuff;
			PBDataMeg.DataSource = *(PB_ByteBuff+1);

			if(PBCtrlMeg.DataCount == 0)
			{
				u8PbCroFlg = cFalse;
			}
			else
			{
				PBCtrlMeg.DataCount = PBCtrlMeg.DataCount-1;
			}
			


			if(PBDataMeg.DataSource!= PBCtrlMeg.DataSource)
			{
				
				IC_PhoneNum_RX_0x38B_Byte_1 = 0x7F;
				IC_PhoneNum_RX_0x38B_Byte_2 = 0x73;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_PhoneNum_RX_0x38B);
				PbErrFlg = 1;
			}

			else if((u8PbCroFlg == cFalse) && (PBDataMeg.DataNum!=0))
			{
				IC_PhoneNum_RX_0x38B_Byte_1 = 0x7F;
				IC_PhoneNum_RX_0x38B_Byte_2 = 0x24;
				DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_PhoneNum_RX_0x38B);
				PbErrFlg = 1;
				
			}
			else
			{
				LAP_AVCV_PBDataBufferHandle();
				if((PBCtrlMeg.DataSource<=0x04)&&(PBCtrlMeg.ContactsID>=0x01)&&(PBCtrlMeg.ContactsID<=0x08)&&
				   (PBCtrlMeg.PhNumLength<=0x08)&&(PBCtrlMeg.ContactsLength<=0x10))
				{
					LAP_PhoneBook_DatMegRead(PBCtrlMeg.DataSource, PBCtrlMeg.ContactsID, PBCtrlMeg.PhNumLength, PBCtrlMeg.ContactsLength);
				}
				else
				{
					//don't processing the invalid data
				}	
/*
				if(u8PBUpdateEnable==cFalse)
				{
					if(TOSIsTimerStarted(&tPBClearTimer))
					{
						if (TOSIsTimerElapsed(&tPBClearTimer,mTOSConvMsInTimerTick(1000)))
						{		
							u8PBUpdateEnable = cTrue;
							u8PhoneBookUpdate = 0;
						} 
						else
						{		
							//no state
						}
					}
					else
					{
						TOSStartTimer(&tPBClearTimer);
					}	
				}
				else
				{
					//no operation
				}
*/
				
			}

			TOSStopTimer(&tPBTimer);

		}
		//If the status of blue tooth from connect to unconnect, clear the phone number and contacts's name in RAM.
		if(u8BTConnectStatus==0)
		{
			mLIBmemset(&PB_ByteStore, ClearBuffer, sizeof(PB_ByteStore));
			mLIBmemset(&PB_Ctr_RAM, ClearBuffer, sizeof(PB_Ctr_RAM));
			mLIBmemset(&PB_Dat_RAM, ClearBuffer, sizeof(PB_Dat_RAM));
			mLIBmemset(&NumList_RAM, ClearBuffer, sizeof(NumList_RAM));
		}
		else
		{
			//Blue tooth is connecting, so needn't to do anything
		}
	}

	if(u8PBUpdateEnable==cFalse)
	{
		if(TOSIsTimerStarted(&tPBClearTimer))
		{
			if (TOSIsTimerElapsed(&tPBClearTimer,mTOSConvMsInTimerTick(4000)))
			{		
				u8PBUpdateEnable = cTrue;
				u8PhoneBookUpdate = 0;
			} 
			else
			{		
				//no state
			}
		}
		else
		{
			TOSStartTimer(&tPBClearTimer);
		}	
	}
	else
	{
		//no operation
	}
}

/*==============================================================================
*  FUNCTION NAME : void LAP_PhoneBook_TransimitProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Transfer the data in PB_Ctr_RAM to IPCL buffer.
*===============================================================================*/
void LAP_PhoneBook_TransmitProcess(void)
{
	U8 u8Num;

	/*Unused data segment, we should set them in zero.*/
    #if 0
	sAVCV_IPCL.AVCV_Data_05 = 0;
	sAVCV_IPCL.AVCV_Data_06 = 0;
	sAVCV_IPCL.AVCV_Data_07 = 0;
	sAVCV_IPCL.AVCV_Data_08 = 0;
	sAVCV_IPCL.AVCV_Data_09 = 0;
    #endif
	if(u8TransOrder==0x02)
	{
		sAVCV_IPCL.AVCV_MesgType = 0x03;  //data source 0x01 of first page
		sAVCV_IPCL.AVCV_Data_01 = *(PB_Ctr_RAM);
		sAVCV_IPCL.AVCV_Data_02 = *(PB_Ctr_RAM+1);
		sAVCV_IPCL.AVCV_Data_03 = *(PB_Ctr_RAM+2);
		sAVCV_IPCL.AVCV_Data_04 = *(PB_Ctr_RAM+3);
		
		for(u8Num=0; u8Num<32; u8Num++)
		{
			sAVCV_IPCL.AVCV_Pb_Char[u8Num] = *(PB_Dat_RAM+u8Num);
		}
	}
	else if(u8TransOrder==0x05)
	{
		sAVCV_IPCL.AVCV_MesgType = 0x13;  //data source 0x01 of second page
		sAVCV_IPCL.AVCV_Data_01 = *(PB_Ctr_RAM+4);
		sAVCV_IPCL.AVCV_Data_02 = *(PB_Ctr_RAM+5);
		sAVCV_IPCL.AVCV_Data_03 = *(PB_Ctr_RAM+6);
		sAVCV_IPCL.AVCV_Data_04 = *(PB_Ctr_RAM+7);
		
		for(u8Num=0; u8Num<32; u8Num++)
		{
			sAVCV_IPCL.AVCV_Pb_Char[u8Num] = *(PB_Dat_RAM+32+u8Num);
		}
	}
	else if(u8TransOrder==0x08)
	{
		sAVCV_IPCL.AVCV_MesgType = 0x04;  //data source 0x02 of first page
		sAVCV_IPCL.AVCV_Data_01 = *(PB_Ctr_RAM+8);
		sAVCV_IPCL.AVCV_Data_02 = *(PB_Ctr_RAM+9);
		sAVCV_IPCL.AVCV_Data_03 = *(PB_Ctr_RAM+10);
		sAVCV_IPCL.AVCV_Data_04 = *(PB_Ctr_RAM+11);
		
		for(u8Num=0; u8Num<32; u8Num++)
		{
			sAVCV_IPCL.AVCV_Pb_Char[u8Num] = *(PB_Dat_RAM+64+u8Num);
		}
	}
	else if(u8TransOrder==0x0B)
	{
		sAVCV_IPCL.AVCV_MesgType = 0x14;  //data source 0x02 of second page
		sAVCV_IPCL.AVCV_Data_01 = *(PB_Ctr_RAM+12);
		sAVCV_IPCL.AVCV_Data_02 = *(PB_Ctr_RAM+13);
		sAVCV_IPCL.AVCV_Data_03 = *(PB_Ctr_RAM+14);
		sAVCV_IPCL.AVCV_Data_04 = *(PB_Ctr_RAM+15);
		
		for(u8Num=0; u8Num<32; u8Num++)
		{
			sAVCV_IPCL.AVCV_Pb_Char[u8Num] = *(PB_Dat_RAM+96+u8Num);
		}
	}
	else if(u8TransOrder==0x0E)
	{
		sAVCV_IPCL.AVCV_MesgType = 0x05;  //data source 0x03 of first page
		sAVCV_IPCL.AVCV_Data_01 = *(PB_Ctr_RAM+16);
		sAVCV_IPCL.AVCV_Data_02 = *(PB_Ctr_RAM+17);
		sAVCV_IPCL.AVCV_Data_03 = *(PB_Ctr_RAM+18);
		sAVCV_IPCL.AVCV_Data_04 = *(PB_Ctr_RAM+19);
		
		for(u8Num=0; u8Num<32; u8Num++)
		{
			sAVCV_IPCL.AVCV_Pb_Char[u8Num] = *(PB_Dat_RAM+128+u8Num);
		}
	}
	else if(u8TransOrder==0x11)
	{
		sAVCV_IPCL.AVCV_MesgType = 0x15;  //data source 0x03 of second page
		sAVCV_IPCL.AVCV_Data_01 = *(PB_Ctr_RAM+20);
		sAVCV_IPCL.AVCV_Data_02 = *(PB_Ctr_RAM+21);
		sAVCV_IPCL.AVCV_Data_03 = *(PB_Ctr_RAM+22);
		sAVCV_IPCL.AVCV_Data_04 = *(PB_Ctr_RAM+23);
		
		for(u8Num=0; u8Num<32; u8Num++)
		{
			sAVCV_IPCL.AVCV_Pb_Char[u8Num] = *(PB_Dat_RAM+160+u8Num);
		}
	}
	else if(u8TransOrder==0x14)
	{
		sAVCV_IPCL.AVCV_MesgType = 0x06;  //data source 0x04 of first page
		sAVCV_IPCL.AVCV_Data_01 = *(PB_Ctr_RAM+24);
		sAVCV_IPCL.AVCV_Data_02 = *(PB_Ctr_RAM+25);
		sAVCV_IPCL.AVCV_Data_03 = *(PB_Ctr_RAM+26);
		sAVCV_IPCL.AVCV_Data_04 = *(PB_Ctr_RAM+27);
		
		for(u8Num=0; u8Num<32; u8Num++)
		{
			sAVCV_IPCL.AVCV_Pb_Char[u8Num] = *(PB_Dat_RAM+192+u8Num);
		}
	}
	else if(u8TransOrder==0x17)
	{
		sAVCV_IPCL.AVCV_MesgType = 0x16;  //data source 0x04 of second page
		sAVCV_IPCL.AVCV_Data_01 = *(PB_Ctr_RAM+28);
		sAVCV_IPCL.AVCV_Data_02 = *(PB_Ctr_RAM+29);
		sAVCV_IPCL.AVCV_Data_03 = *(PB_Ctr_RAM+30);
		sAVCV_IPCL.AVCV_Data_04 = *(PB_Ctr_RAM+31);
		
		for(u8Num=0; u8Num<32; u8Num++)
		{
			sAVCV_IPCL.AVCV_Pb_Char[u8Num] = *(PB_Dat_RAM+224+u8Num);
		}
	}
	else
	{
		//no operation
	}	

	#if 0 /* Store phone book information separately */
	/*Clear the buffer,avoid residual data influencing normal data transmission.*/
	mLIBmemset(&PS_Name_RAM, ClearBuffer, sizeof(PS_Name_RAM));
	mLIBmemset(&PS_Numb_RAM, ClearBuffer, sizeof(PS_Numb_RAM));
	mLIBmemset(&AV_Radio_RAM, ClearBuffer, sizeof(AV_Radio_RAM));
	if(u8PSCallType != 0)
	{
		u8PSCallType = 1;  //defaut no call 
	}
	#endif
}

/*==============================================================================
*  FUNCTION NAME : void LAP_AVCV_NavigationProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : The process of switch on the navigation to display.
*===============================================================================*/
void LAP_AVCV_NavigationProcess(void)
{	
	U8	u8NaviClear;
	static Bool u8NaviCroFlg = cFalse;
	static U8 U8NaLenErrFlg = 0;

    //Timeout 
    if((TOSIsTimerStarted((&tToutNavTimer))) && (TOSIsTimerElapsed((&tToutNavTimer),mTOSConvMsInTimerTick(2000))))
	{
        u8NaviEndFlg = 1;
        
        mLIBmemset(&Na_Data_RAM, ClearBuffer, sizeof(Na_Data_RAM));

		TOSStopTimer((&tToutNavTimer));
		return;
	}
    
	if((TOSIsTimerStarted((&tNaTimer))) && (TOSIsTimerElapsed((&tNaTimer),mTOSConvMsInTimerTick(50))))
	{
		//Timeout Res
		mDATWriteU8BitNavDataReceiveResp_P1Default(0x7F);
		mDATWriteU8BitNavDataReceiveResp_P2Default(0x37);
		DATCilSendMessage(cDATCdlD_UUTxHandleIC_Nav_RX_0x38C);

		NaCtrlMeg.DataCount = 0;//LAPAVCVNaviClearBuffer();
		TOSStopTimer((&tNaTimer));
		NaErrFlg =1;
		return;
	}


	if(NaviRevFlg == 1 || U8NaLenErrFlg == 1)
	{
		U8NaLenErrFlg = 0;
		NaviRevFlg = 0;

        *Na_ByteBuff     = mDATReadU8BitNavDataSendReq_P1Default();
        *(Na_ByteBuff+1) = mDATReadU8BitNavDataSendReq_P2Default();
        *(Na_ByteBuff+2) = mDATReadU8BitNavDataSendReq_P3Default();
        *(Na_ByteBuff+3) = mDATReadU8BitNavDataSendReq_P4Default();
        *(Na_ByteBuff+4) = mDATReadU8BitNavDataSendReq_P5Default();
        *(Na_ByteBuff+5) = mDATReadU8BitNavDataSendReq_P6Default();
        *(Na_ByteBuff+6) = mDATReadU8BitNavDataSendReq_P7Default();
        *(Na_ByteBuff+7) = mDATReadU8BitNavDataSendReq_P8Default();

		if(*Na_ByteBuff>0x80 /*&& (*(Na_ByteBuff+1) == 0x01)*/) //control message
		{
			NaCtrlMeg.DataLength = *Na_ByteBuff & 0x7F;
			NaCtrlMeg.DataSrcState = *(Na_ByteBuff+1);	

			if(/*u8NaviEndFlg == 1 && */NaCtrlMeg.DataSrcState == 1)
			{
				u8NaviEndFlg = 0;
			}
			else // no navi info
			{
				u8NaviEndFlg = 1;
			}

			if(NaCtrlMeg.DataCount != 0)
			{
				//Data length mismatch
				//IC_PhoneNum_RX_0x38C_Byte_1 = 0x7F;
				//IC_PhoneNum_RX_0x38C_Byte_2 = 0x13;
				//DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_PhoneNum_RX_0x38C);
                mDATWriteU8BitNavDataReceiveResp_P1Default(0x7F);
                mDATWriteU8BitNavDataReceiveResp_P2Default(0x13);
                DATCilSendMessage(cDATCdlD_UUTxHandleIC_Nav_RX_0x38C);

				NaCtrlMeg.DataCount = 0;//LAPAVCVNaviClearBuffer();
				U8NaLenErrFlg = 1;
				NaErrFlg = 1;
				return;
			}

			NaCtrlMeg.DataCount = (NaCtrlMeg.DataLength - 1)/7;
			if((NaCtrlMeg.DataLength - 1)%7 == 0)
			{
				;
			}
			else
			{
				NaCtrlMeg.DataCount += 1;
			}

			if(NaCtrlMeg.DataLength>1)
			{			
	 			//IC_PhoneNum_RX_0x38C_Byte_1 = 0x30;
				//IC_PhoneNum_RX_0x38C_Byte_2 = 0x00;
	 			//DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_PhoneNum_RX_0x38C);
                mDATWriteU8BitNavDataReceiveResp_P1Default(0x30);
                mDATWriteU8BitNavDataReceiveResp_P2Default(0x00);
                DATCilSendMessage(cDATCdlD_UUTxHandleIC_Nav_RX_0x38C);

				u8NaviCroFlg = cTrue;

				if(TOSIsTimerStarted((&tNaTimer)))
				{
					;
				}
				else
				{
					TOSStartTimer((&tNaTimer));
				}

                TOSStartTimer((&tToutNavTimer));
				
				/*Calculating the length of road name.*/
				if((NaCtrlMeg.DataLength-1)%7==0)
				{
					u8NaNameLength = NaCtrlMeg.DataLength - (NaCtrlMeg.DataLength-1)/7 - 1;
					u8NaNameLength /= 2;
				}
				else
				{
					u8NaNameLength = NaCtrlMeg.DataLength - (NaCtrlMeg.DataLength-1)/7 - 2;
					u8NaNameLength /= 2;
				}

				if(u8NaNameLength>=12)
				{
					u8NaNameLength = 12;  //display total 6 characters
				}
				else
				{
					//keep real length
				}
				
				if(DATACOM_ReadU8BitNaviTransmitDefault())
				{
					for(u8NaviClear=0; u8NaviClear<16; u8NaviClear++)
					{
						sMP5Navi_IPCL.Navi_Char[u8NaviClear] = ClearBuffer;
					}

					DATACOM_WriteU8BitNaviTransmitDefault(cFalse);
				}
				else
				{
					/*no operation*/
				}
			}
			else
			{
				/*no operation*/
			}	
		}
		else
		{
			NaDataMeg.DataNum = *Na_ByteBuff;
			NaDataMeg.DataSrcState = *(Na_ByteBuff + 1);
/*			
			if(NaDataMeg.DataSrcState == 0)
			{
				u8NaviEndFlg = 1;
			}
*/			
			if(NaCtrlMeg.DataCount == 0)
			{
				u8NaviCroFlg = cFalse;
			}
			else
			{
				NaCtrlMeg.DataCount = NaCtrlMeg.DataCount-1;
			}
			


			if(NaDataMeg.DataSrcState != NaCtrlMeg.DataSrcState)
			{
				
				//IC_PhoneNum_RX_0x38C_Byte_1 = 0x7F;
				//IC_PhoneNum_RX_0x38C_Byte_2 = 0x73;
				//DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_PhoneNum_RX_0x38C);
                mDATWriteU8BitNavDataReceiveResp_P1Default(0x7F);
                mDATWriteU8BitNavDataReceiveResp_P2Default(0x73);
                DATCilSendMessage(cDATCdlD_UUTxHandleIC_Nav_RX_0x38C);

				NaErrFlg = 1;
			}

			else if((u8NaviCroFlg == cFalse) && (NaDataMeg.DataNum!=0))
			{
				//IC_PhoneNum_RX_0x38C_Byte_1 = 0x7F;
				//IC_PhoneNum_RX_0x38C_Byte_2 = 0x24;
				//DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_PhoneNum_RX_0x38C);
                mDATWriteU8BitNavDataReceiveResp_P1Default(0x7F);
                mDATWriteU8BitNavDataReceiveResp_P2Default(0x24);
                DATCilSendMessage(cDATCdlD_UUTxHandleIC_Nav_RX_0x38C);

				NaErrFlg = 1;
				
			}
			else
			{
				LAP_NaviSign_NameHandle();
			}
            TOSStopTimer(&tNaTimer);

            TOSStartTimer((&tToutNavTimer));
		}	
	}
}

/*==============================================================================
*  FUNCTION NAME : void LAP_AVCV_NaviTransmitProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Transfer the data from Na_Data_RAM to IPCL buffer.
*===============================================================================*/
void LAP_AVCV_NaviTransmitProcess(void)
{
	U8  u8Count;

	for(u8Count=0; u8Count<u8NaNameLength; u8Count++)
	{
		sMP5Navi_IPCL.Navi_Char[u8Count] = *(Na_Data_RAM+u8Count);
	}
}

/*==============================================================================
*  FUNCTION NAME : void LAP_AVCV_TurnByTurnProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : The process of turn by turn.
*===============================================================================*/
void LAP_AVCV_TurnByTurnProcess(void)
{	
    U8 u8Nvr_status = 0;
	U8 u8Tout_status = 0;

    U8 u8Navi_sign = 0;
    
    U32 u32Navi_Dist = 0;

    U8 u8Msg384ToutFlg = 0;
    
    #if 0
	U16	cIVI_DistH = 0;
	U16	cIVI_DistL = 0;
	U32 cIVI_Dist = 0;
	
	
	if(!mDATReadU1BitMP5_0x384TimeoutDefault())
	{
		cIVI_Dist = ((((U32)(mDATReadU8BitMP5_ROUTE_REMAIN_DIS_P1Delayed())) << 16) | (((U32)(mDATReadU8BitMP5_ROUTE_REMAIN_DIS_P2Delayed())) << 8) | (U32)(mDATReadU8BitMP5_ROUTE_REMAIN_DIS_P3Delayed()));
		cIVI_DistH = (U16)((cIVI_Dist & 0xFFFF0000) >> 16);
		cIVI_DistL = (U16)(cIVI_Dist & 0x0000FFFF);
		
		sMP5Navi_IPCL.Navi_Sign = mDATReadU8BitMP5_NavICONDelayed();
		//sMP5Navi_IPCL.Navi_RoundNum = mDATReadU8BitMP5_ROUNG_ABOUT_NUMDelayed();
		sMP5Navi_IPCL.Navi_Dist_H = cIVI_DistH;
		sMP5Navi_IPCL.Navi_Dist_L = cIVI_DistL;		
	}
	else
	{
		sMP5Navi_IPCL.Navi_Sign = 0;
		//sMP5Navi_IPCL.Navi_RoundNum = 0;
		sMP5Navi_IPCL.Navi_Dist_H = 0;
		sMP5Navi_IPCL.Navi_Dist_L = 0;
	}

    if(u8NaviEndFlg == 0)
	{
		sMP5Navi_IPCL.Navi_Switch  |= 0x04;
	}
	else
	{
		sMP5Navi_IPCL.Navi_Switch  &= ~0x04;
	}
    #endif

    Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_NR(&u8Nvr_status);
	Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_Tout(&u8Tout_status);

	Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_NavICON(&u8Navi_sign);
    
	Rte_Read_rpSR_CANMSG_GW_MP5_0x384_ComIn_MP5_ROUTE_REMAIN_DIS(&u32Navi_Dist);
	
	if (u8Nvr_status == RTE_E_NEVER_RECEIVED || u8Tout_status == RTE_E_TIMEOUT)
	{
        u8Msg384ToutFlg = 1;
	}
	else 
	{
		u8Msg384ToutFlg = 0; // 0x384 no time out, turn by turn is valid
        
        sMP5Navi_IPCL.Navi_Sign = u8Navi_sign;
        sMP5Navi_IPCL.Navi_Dist = u32Navi_Dist;
	}

    if ((u8NaviEndFlg == 0 && sMP5Navi_IPCL.Navi_Char[0] != 0) && (u8Msg384ToutFlg == 0 && (sMP5Navi_IPCL.Navi_Sign > 0 && sMP5Navi_IPCL.Navi_Sign < 0x52) && sMP5Navi_IPCL.Navi_Dist != 0))
    {
        sMP5Navi_IPCL.Navi_Switch = 0;
    }
    else 
    {
        sMP5Navi_IPCL.Navi_Switch = 1;
    }

    Rte_Write_ppSR_TIGdt_MP5_0x384_IVI_NavICON(sMP5Navi_IPCL.Navi_Sign);
	Rte_Write_ppSR_TIGdt_MP5_0x384_IVI_ROUTE_REMAIN_DIS(sMP5Navi_IPCL.Navi_Dist);
    
	Rte_Write_ppSR_TIGdt_MP5_0x384_IVI_SIGNAL_LOST(sMP5Navi_IPCL.Navi_Switch);
    
/*
	if((sMP5Navi_IPCL.Navi_Sign==0 || sMP5Navi_IPCL.Navi_Sign>0x14) && cIVI_DistH==0xFFFF && cIVI_DistL==0xFF &&  \
	   (sMP5Navi_IPCL.Navi_RoundNum==0 || sMP5Navi_IPCL.Navi_RoundNum>0x10))
	{
		sMP5Navi_IPCL.Navi_Switch &= 0x01;  // turn by tern is invalid
	}
	else
	{
		sMP5Navi_IPCL.Navi_Switch |= 0x02;
	}
*/
}

/*==============================================================================
*  FUNCTION NAME : void LAP_AVCV_ReverseOperationProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Getting the control MP5 commands from GIP and send them 
*						to CAN bus (private CAN).
*===============================================================================*/
void LAP_AVCV_ReverseOperationProcess(void)
{
	U8	u8Command;
	static  Bool	bKeyActived = 0;

	u8Command = DATACOM_ReadU8BitClustCtrlDefault();
	//u8Command &= 0x0F;
	//If VIP get the key actived state(the same command), only operate one time.
	//After send key command(message:ID 0x377) to private CAN, and waiting the ACK is sent to GIP.
	//Once ACK is sent,then clear the key state which from GIP(means end this operation).
	if((u8Command > 0) && bKeyActived==0)
	{
		bKeyActived = 1;
	}
	else
	{
		//no state
	}

	if(bKeyActived==1)
	{
		if((u8Command >= Cmd_JoinCall)&&(u8Command <= Cmd_EndCall))
		{
			IC_0x377_IC_BTphoneCalloptions = u8Command;
			IC_0x377_IC_AudioRequest = 0;
			IC_0x377_IC_NextItem = 0;
			IC_0x377_IC_LastItem = 0;
			IC_0x377_IC_AudioControl = 0;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			TOSStartTimer(&PhoneTimer);
			BTphoneCmd = u8Command;
			BTphoneCount = 0;
		}
		else if((u8Command >= Cmd_USB)&&(u8Command <= Cmd_Network_program))
		{
			IC_0x377_IC_AudioRequest = u8Command - Cmd_EndCall ;
			IC_0x377_IC_BTphoneCalloptions = 0;
			IC_0x377_IC_NextItem = 0;
			IC_0x377_IC_LastItem = 0;
			IC_0x377_IC_AudioControl = 0;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			TOSStartTimer(&AudioTimer);
			AudioChangeCmd = u8Command - Cmd_EndCall ;
			AudioChangeCount = 0;
		}
		else if(u8Command==Cmd_NextItem)
		{
			IC_0x377_IC_NextItem = cTrue;
			IC_0x377_IC_BTphoneCalloptions = 0;
			IC_0x377_IC_AudioRequest = 0;
			IC_0x377_IC_LastItem = 0;
			IC_0x377_IC_AudioControl = 0;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			TOSStartTimer(&NTimer);
			AudioNextCmd = cTrue;
			AudioNextCount = 0;
		}
		else if(u8Command==Cmd_LastItem)
		{
			IC_0x377_IC_LastItem = cTrue;
			IC_0x377_IC_BTphoneCalloptions = 0;
			IC_0x377_IC_AudioRequest = 0;
			IC_0x377_IC_NextItem = 0;
			IC_0x377_IC_AudioControl = 0;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			TOSStartTimer(&LTimer);
			AudioLastCmd = cTrue;
			AudioLastCount = 0;
		}
		else if((u8Command >= Cmd_Stop) && (u8Command <= Cmd_Start))
		{
			IC_0x377_IC_AudioControl = u8Command - Cmd_LastItem;
			IC_0x377_IC_LastItem = 0;
			IC_0x377_IC_BTphoneCalloptions = 0;
			IC_0x377_IC_AudioRequest = 0;
			IC_0x377_IC_NextItem = 0;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			TOSStartTimer(&CTimer);
			AudioCenterCmd = u8Command - Cmd_LastItem;
			AudioCenterCount = 0;
		}
		else
		{
			IC_0x377_IC_BTphoneCalloptions = 0;
			IC_0x377_IC_AudioRequest = 0;
			IC_0x377_IC_NextItem = 0;
			IC_0x377_IC_LastItem = 0;
			IC_0x377_IC_AudioControl = 0;
		}
		
		bKeyActived = 0; //once acticed , operate one time
		DATACOM_WriteU8BitClustCtrlDefault(cFalse);
	}
	else
	{
		//no state
	}

	#if 0 //no gip
	if(DATACOM_ReadU1BitICSendDefault()==1)
	{
		DATACOM_WriteU8BitClustCtrlDefault(cFalse);
		DATACOM_WriteU1BitICSendDefault(cFalse);
		bKeyActived = 0;
	}
	else
	{
		//no state
	}
	#endif

	if(TOSIsTimerStarted(&PhoneTimer) && TOSIsTimerElapsed((&PhoneTimer),mTOSConvMsInTimerTick(TIME_INTERVAL)))
	{
		if(BTphoneCount >= 2)
		{
		    IC_0x377_IC_BTphoneCalloptions = 0;
			BTphoneCount = 0;
		    TOSStopTimer(&PhoneTimer);
		}
		else
		{
			BTphoneCount++;
			IC_0x377_IC_BTphoneCalloptions = BTphoneCmd;
			IC_0x377_IC_AudioRequest = 0;
			IC_0x377_IC_NextItem = 0;
			IC_0x377_IC_LastItem = 0;
			IC_0x377_IC_AudioControl = 0;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			TOSStartTimer(&PhoneTimer);
		}
	}
	if(TOSIsTimerStarted(&AudioTimer) && TOSIsTimerElapsed((&AudioTimer),mTOSConvMsInTimerTick(TIME_INTERVAL)))
	{
		if(AudioChangeCount >= 2)
		{
		    IC_0x377_IC_AudioRequest = 0;
			AudioChangeCount = 0;
			TOSStopTimer(&AudioTimer);			
		}
		else
		{
			AudioChangeCount++;
			IC_0x377_IC_AudioRequest = AudioChangeCmd ;
			IC_0x377_IC_BTphoneCalloptions = 0;
			IC_0x377_IC_NextItem = 0;
			IC_0x377_IC_LastItem = 0;
			IC_0x377_IC_AudioControl = 0;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			TOSStartTimer(&AudioTimer);
		}
	}
	if(TOSIsTimerStarted(&NTimer) && TOSIsTimerElapsed((&NTimer),mTOSConvMsInTimerTick(TIME_INTERVAL)))
	{
		if(AudioNextCount >= 2)
		{
		    IC_0x377_IC_NextItem = 0;
			AudioNextCount = 0;
			TOSStopTimer(&NTimer);	
		}
		else
		{
			AudioNextCount++;
			IC_0x377_IC_NextItem = cTrue;
			IC_0x377_IC_BTphoneCalloptions = 0;
			IC_0x377_IC_AudioRequest = 0;
			IC_0x377_IC_LastItem = 0;
			IC_0x377_IC_AudioControl = 0;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			TOSStartTimer(&NTimer);
		}
	}
	if(TOSIsTimerStarted(&LTimer) && TOSIsTimerElapsed((&LTimer),mTOSConvMsInTimerTick(TIME_INTERVAL)))
	{
		if(AudioLastCount >= 2)
		{
		    IC_0x377_IC_LastItem = 0;
			AudioLastCount = 0;
			TOSStopTimer(&LTimer);	
		}
		else
		{
			AudioLastCount++;
			IC_0x377_IC_LastItem = cTrue;
			IC_0x377_IC_BTphoneCalloptions = 0;
			IC_0x377_IC_AudioRequest = 0;
			IC_0x377_IC_NextItem = 0;
			IC_0x377_IC_AudioControl = 0;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			TOSStartTimer(&LTimer);
		}	
	}
	if(TOSIsTimerStarted(&CTimer) && TOSIsTimerElapsed((&CTimer),mTOSConvMsInTimerTick(TIME_INTERVAL)))
	{
		if(AudioCenterCount >= 2)
		{
		    IC_0x377_IC_AudioControl = 0;
			AudioCenterCount = 0;
		    TOSStopTimer(&CTimer);
		}
		else
		{
			AudioCenterCount++;			
			IC_0x377_IC_AudioControl = AudioCenterCmd;
			IC_0x377_IC_LastItem = 0;
			IC_0x377_IC_BTphoneCalloptions = 0;
			IC_0x377_IC_AudioRequest = 0;
			IC_0x377_IC_NextItem = 0;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			TOSStartTimer(&CTimer);
		}	
	}
}

/*==============================================================================
*  FUNCTION NAME : void LAP_AVCV_ReverseCallUpProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Getting the control MP5 commands from GIP and send them to 
*						CAN bus (private CAN).Letting MP5 to make a call.
*						It need to send complete three cycles messages.Each message 
*						contains three frames.
*===============================================================================*/
void LAP_AVCV_ReverseCallUpProcess(void)
{
	U8	eCmd_Call_H;
	U8	eCmd_Call_L;
	
	eCmd_Call_H = (DATACOM_ReadU8BitClustCallUpDefault()>>4) & 0x0F;
	eCmd_Call_L = DATACOM_ReadU8BitClustCallUpDefault() & 0x0F;
	/*This function is called 10ms each times, means send the phone number 20ms every time.*/
	u8CallPeriod_20ms = ~u8CallPeriod_20ms;

	if(eCmd_Call_H && eCmd_Call_L && u8CallPeriod_20ms)
	{
		IC_PhoneNum_0x376_Byte_1 = *(NumList_RAM+(eCmd_Call_H-1)*64+(eCmd_Call_L-1)*8+0);
		IC_PhoneNum_0x376_Byte_2 = *(NumList_RAM+(eCmd_Call_H-1)*64+(eCmd_Call_L-1)*8+1);
		IC_PhoneNum_0x376_Byte_3 = *(NumList_RAM+(eCmd_Call_H-1)*64+(eCmd_Call_L-1)*8+2);
		IC_PhoneNum_0x376_Byte_4 = *(NumList_RAM+(eCmd_Call_H-1)*64+(eCmd_Call_L-1)*8+3);
		IC_PhoneNum_0x376_Byte_5 = *(NumList_RAM+(eCmd_Call_H-1)*64+(eCmd_Call_L-1)*8+4);
		IC_PhoneNum_0x376_Byte_6 = *(NumList_RAM+(eCmd_Call_H-1)*64+(eCmd_Call_L-1)*8+5);
		IC_PhoneNum_0x376_Byte_7 = *(NumList_RAM+(eCmd_Call_H-1)*64+(eCmd_Call_L-1)*8+6);
		IC_PhoneNum_0x376_Byte_8 = *(NumList_RAM+(eCmd_Call_H-1)*64+(eCmd_Call_L-1)*8+7);

		DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_PhoneNum_0x376);

		u8FrameCycle++;

		if(u8FrameCycle>3)
		{
			u8FrameCycle = 0;
			DATACOM_WriteU8BitClustCallUpDefault(cFalse);
		}
		else
		{
			//send message to MP5 to make a call continually
		}
	}
	else
	{
		//no operation
	}
}

/*==============================================================================
*  FUNCTION NAME : void LAP_AVCV_InformationProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Getting the control MP5 commands from GIP and send them 
*						to CAN bus (private CAN).
*===============================================================================*/
void LAP_AVCV_InformationProcess(void)
{
	U8 u8Command;
	static U8 Navicuont = 0;

	u8Command = LAPAVCVGetMenuStatus();
	if(u8Command != 1 && u8Command !=3 && Navicuont ==0)
	{
		Navicuont = 1;
	}
	
	if(u8Command == 3)//Audio
	{
		//IC_0x377_IC_ScreenRequest = 1;

		if(TOSIsTimerStarted(&tAudioTimer))
		{
			;
		}
		else
		{
			TOSStartTimer(&tAudioTimer);
		}
		
		if(TOSIsTimerStarted(&PhoneTimer) || TOSIsTimerStarted(&AudioTimer) || TOSIsTimerStarted(&NTimer) || TOSIsTimerStarted(&LTimer) || TOSIsTimerStarted(&CTimer))
		{
			;
		}
		else
		{
			if(flag == 0)
			{
				flag = 1;
				AVCVCounter = 0;
			}
			else
			{
				//TOSStartTimer(&tAudioDelayTimer);
			}
			
		}		
		Navicuont = 0;
	}


	else if(u8Command == 1)//Navi
	{
		if(TOSIsTimerStarted(&tNaviTimer))
		{
			;
		}
		else
		{
			TOSStartTimer(&tNaviTimer);
		}
		
		if(TOSIsTimerStarted(&PhoneTimer) || TOSIsTimerStarted(&AudioTimer) || TOSIsTimerStarted(&NTimer) || TOSIsTimerStarted(&LTimer))
		{
			;
		}
		else
		{
			if(flag1 == 0)
			{
				flag1 = 1;
				NAVICounter = 0;
			}
			
		}		
		Navicuont = 0;
	}
	else
	{
		IC_0x377_IC_ScreenRequest = 0;
		if(Navicuont == 1)
		{
			Navicuont++;
		}
	}

}

/*==============================================================================
*  FUNCTION NAME : void LAP_AVCV_InformationTransmitProcess(void)
* 
*  PARAMETERS:   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Getting the control MP5 commands from CAN bus (private CAN) and send them 
*						to GIP.
*===============================================================================*/
void LAP_AVCV_InformationTransmitProcess(void)
{	
	U8 u8Command;

	u8Command = LAPAVCVGetMenuStatus();
	
	if(u8Command == 3)
	{
		if(TOSIsTimerStarted(&tAudioTimer) && (TOSIsTimerElapsed((&tAudioTimer),mTOSConvMsInTimerTick(10))))
		{
			sAVCV_IPCL.AVCV_AudioStatus = (U8)MP5_0x382_MP5_AudioStatus;
			TOSStopTimer(&tAudioTimer);
			sAVCV_IPCL.AVCV_ScreenReq = 0x01;
		}
		
		if(AVCVCounter == 9)
		{
			IC_0x377_IC_ScreenRequest = 1;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			flag = 0;
		}
	}

	if(u8Command == 1)
	{
		if(TOSIsTimerStarted(&tNaviTimer) && (TOSIsTimerElapsed((&tNaviTimer),mTOSConvMsInTimerTick(10))))
		{
			sMP5Navi_IPCL.Navi_Switch = (U8)MP5_0x382_MP5_NavStatus;
			TOSStopTimer(&tNaviTimer);
			sAVCV_IPCL.AVCV_ScreenReq = 0x01;
		}

		if(NAVICounter == 9)
		{
			IC_0x377_IC_ScreenRequest = 2;
			DATCilSendMessage_CH1(cDATCdlD_UUTxHandle_CH1IC_0x377);
			flag1 = 0;			
		}
	}

			
}

/*==============================================================================
*  FUNCTION NAME : U8 LAPAVCVGetMenuStatus(void)
* 
*  PARAMETERS:Menu Status   
*
*  RETURN VALUE :   
* 
*  DESIGN INFORMATION : Setting Menu Status
*	
*===============================================================================*/
U8 LAPAVCVGetMenuStatus(void)
{
	U8 u8MenuPosition;
	
	u8MenuPosition = IPCL_GetMenuConfig_Data.MenuPosition;
	
	return u8MenuPosition;
}


//==============================================================================
// DESCRIPTION : void LAPAVCVInit(void)
//
// PARAMETERS :	
//
// RETURN VALUE :  
//
// DESIGN INFORMATION : Initialize the values of some buffs and variables which 
//						must be initialized at initializaton program.
//==============================================================================
void LAPAVCVInit(void)
{
	mLIBmemset(&AV_ByteBuff, ClearBuffer, sizeof(AV_ByteBuff));
	mLIBmemset(&PS_ByteBuff, ClearBuffer, sizeof(PS_ByteBuff));
	mLIBmemset(&PB_ByteBuff, ClearBuffer, sizeof(PB_ByteBuff));
	mLIBmemset(&sAVCV_IPCL, ClearBuffer, sizeof(IPCL_AVCVType));
	mLIBmemset(&sMP5Navi_IPCL, ClearBuffer, sizeof(IPCL_MP5NaviType));	
	mLIBmemset(&PB_ByteStore, ClearBuffer, sizeof(PB_ByteStore));
	mLIBmemset(&PB_Ctr_RAM, ClearBuffer, sizeof(PB_Ctr_RAM));
	mLIBmemset(&PB_Dat_RAM, ClearBuffer, sizeof(PB_Dat_RAM));
	mLIBmemset(&AV_Dat_RAM, ClearBuffer, sizeof(AV_Dat_RAM));
	mLIBmemset(&NumList_RAM, ClearBuffer, sizeof(NumList_RAM));
	mLIBmemset(&Na_Data_RAM, ClearBuffer, sizeof(Na_Data_RAM));
	mLIBmemset(&PS_Name_RAM, ClearBuffer, sizeof(PS_Name_RAM));
	mLIBmemset(&PS_Numb_RAM, ClearBuffer, sizeof(PS_Numb_RAM));
	mLIBmemset(&PS_Time_RAM, ClearBuffer, sizeof(PS_Time_RAM));
    mLIBmemset(&AV_Duration_RAM, ClearBuffer, sizeof(AV_Duration_RAM));
    mLIBmemset(&AV_PlayTime_RAM, ClearBuffer, sizeof(AV_PlayTime_RAM));
    
	IC_Audio_RX_0x389_Byte_1 = 0x30;
	IC_Phone_RX_0x388_Byte_1 = 0x30;
	IC_PhoneNum_RX_0x38B_Byte_1 = 0x30;
	IC_PhoneNum_RX_0x38C_Byte_1 = 0x30;
	u8TransOrder = 0;
	u8FrameCycle = 0;
	u8CallPeriod_20ms = 0;
	u8PSCallSrc = 0;	
	u8PSCallType = 0;	//defaut no Connect
	AV_Source = 0;
	AV_SourceStatus = 0;
	u8BTConnectStatus = 0;
	PsErrFlg = 0;
	PbErrFlg = 0;
	AvErrFlg = 0;
	NaErrFlg = 0;
	LAPAVCVPsClearBuffer();
	LAPAVCVPbClearBuffer();
	LAPAVCVAvClearBuffer();
	LAPAVCVNaviClearBuffer();
	flag = 0;
	flag1 = 0;
	
}

//==============================================================================
// DESCRIPTION : void LAPAVCVPsClearBuffer(void)
//
// PARAMETERS 
//
// RETURN VALUE :   none
//
// DESIGN INFORMATION : 
//==============================================================================

void LAPAVCVPsClearBuffer(void)
{
	PSCtrlMeg.DataCount = 0;
	PSCtrlMeg.DataNum = 0;
	PSCtrlMeg.DataType = 0;
	PSDataMeg.DataNum = 0;
	PSDataMeg.DataType = 0;
}
//==============================================================================
// DESCRIPTION : void LAPAVCVPbClearBuffer(void)
//
// PARAMETERS 
//
// RETURN VALUE :   none
//
// DESIGN INFORMATION : 
//==============================================================================

void LAPAVCVPbClearBuffer(void)
{
	PBCtrlMeg.DataCount = 0;
	PBCtrlMeg.DataNum = 0;
	PBCtrlMeg.ContactsID = 0;
	PBDataMeg.DataNum = 0;
	PBDataMeg.ContactsID = 0;	
}
//==============================================================================
// DESCRIPTION : void LAPAVCVAvClearBuffer(void)
//
// PARAMETERS 
//
// RETURN VALUE :   none
//
// DESIGN INFORMATION : 
//==============================================================================

void LAPAVCVAvClearBuffer(void)
{
	AVCtrlMeg.DataCount = 0;
	AVCtrlMeg.DataNum = 0;
	AVCtrlMeg.DataType = 0;
	AVDataMeg.DataNum = 0;
	AVDataMeg.DataType = 0;	
}
//==============================================================================
// DESCRIPTION : void LAPAVCVNaviClearBuffer(void)
//
// PARAMETERS 
//
// RETURN VALUE :   none
//
// DESIGN INFORMATION : 
//==============================================================================

void LAPAVCVNaviClearBuffer(void)
{
	NaCtrlMeg.DataCount = 0;
	NaCtrlMeg.DataNum = 0;
	NaCtrlMeg.DataType = 0;
	NaDataMeg.DataNum = 0;
	NaDataMeg.DataType = 0;	
}

//==============================================================================
// DESCRIPTION : void LAPAVCVStop(void)
//
// PARAMETERS 
//
// RETURN VALUE :   none
//
// DESIGN INFORMATION : 
//==============================================================================
void LAPAVCVStop(void)
{
   
}

//==============================================================================
// DESCRIPTION : void LAPAVCVStart(void)
//
// PARAMETERS :   
//
// RETURN VALUE :   none 
//
// DESIGN INFORMATION : 
//==============================================================================
void LAPAVCVStart(void)
{
	mLIBmemset(&AV_ByteBuff, ClearBuffer, sizeof(AV_ByteBuff));
	mLIBmemset(&PS_ByteBuff, ClearBuffer, sizeof(PS_ByteBuff));
	mLIBmemset(&PB_ByteBuff, ClearBuffer, sizeof(PB_ByteBuff));
	mLIBmemset(&sAVCV_IPCL, ClearBuffer, sizeof(IPCL_AVCVType));
	mLIBmemset(&sMP5Navi_IPCL, ClearBuffer, sizeof(IPCL_MP5NaviType));	
	mLIBmemset(&PB_ByteStore, ClearBuffer, sizeof(PB_ByteStore));
	mLIBmemset(&PB_Ctr_RAM, ClearBuffer, sizeof(PB_Ctr_RAM));
	mLIBmemset(&PB_Dat_RAM, ClearBuffer, sizeof(PB_Dat_RAM));
	mLIBmemset(&AV_Dat_RAM, ClearBuffer, sizeof(AV_Dat_RAM));

	
	PsErrFlg = 0;
	PbErrFlg = 0;
	AvErrFlg = 0;
	NaErrFlg = 0;
}

//==============================================================================
// DESCRIPTION : void LAPAVCVTask(void)
//
// PARAMETERS :   none
//
// RETURN VALUE :   none
//
// DESIGN INFORMATION : The strategis of task processing.
//==============================================================================
void LAPAVCVTask(void)
{	
	u8TransOrder = DATACOM_ReadU8BitAVCVTransmitDefault();

	LAP_AVCV_PhoneSignalProcess();
	LAP_AVCV_PhoneBookProcess();
	
	LAP_AVCV_AudioVideoProcess();
	LAP_AVCV_InformationProcess();

	LAP_AVCV_InformationTransmitProcess();

	
	if(u8TransOrder%3==0)
	{
		LAP_AudioVideo_TransmitProcess();
	}
	else if(u8TransOrder%3==0x01)
	{
		LAP_AVCV_PSTransmitProcess();
	}
	else
	{
		LAP_PhoneBook_TransmitProcess();
	}

	LAP_AVCV_NavigationProcess();
	
	if(!DATACOM_ReadU8BitNaviTransmitDefault())
	{
		LAP_AVCV_NaviTransmitProcess();
	}
	else
	{
		/*no operation*/
	}
	
	LAP_AVCV_TurnByTurnProcess();
	LAP_AVCV_ReverseOperationProcess();	
	LAP_AVCV_ReverseCallUpProcess();
	
}

//==============================================================================
// DESCRIPTION : U8 LAPAVCVGetErrorFlg(U8)
//
// PARAMETERS :   MessageType
//
// RETURN VALUE :   ErrorFlg
//
// DESIGN INFORMATION : The strategis of task processing.
//==============================================================================

U8 LAPAVCVGetErrorFlg(U8 MessageType)
{
	U8 u8Error = 2;
	
	switch(MessageType)
	{
		case AUDIO_DATA:
			u8Error = AvErrFlg;
		break;

		case PHONE_DATA:
			u8Error = PsErrFlg;
		break;
		
		case PHONEBOOK_DATA_1_1:
		case PHONEBOOK_DATA_1_2:
		case PHONEBOOK_DATA_2_1:
		case PHONEBOOK_DATA_2_2:
		case PHONEBOOK_DATA_3_1:
		case PHONEBOOK_DATA_3_2:
		case PHONEBOOK_DATA_4_1:
		case PHONEBOOK_DATA_4_2:
			u8Error = PbErrFlg;
		break;
		
		default:
		break;
	}

    return u8Error;
}

//==============================================================================
// DESCRIPTION : void LAPAVCVSetErrorFlg(U8 MessageType, U8 Errflg)
//
// PARAMETERS :   U8 MessageType, U8 Errflg
//
// RETURN VALUE :   void
//
// DESIGN INFORMATION : The strategis of task processing.
//==============================================================================
void LAPAVCVSetErrorFlg(U8 MessageType, U8 Errflg)
{
	switch(MessageType)
	{
		case AUDIO_DATA:
			AvErrFlg  = Errflg;
		break;

		case PHONE_DATA:
			PsErrFlg = Errflg;
		break;
		
		case PHONEBOOK_DATA_1_1:
		case PHONEBOOK_DATA_1_2:
		case PHONEBOOK_DATA_2_1:
		case PHONEBOOK_DATA_2_2:
		case PHONEBOOK_DATA_3_1:
		case PHONEBOOK_DATA_3_2:
		case PHONEBOOK_DATA_4_1:
		case PHONEBOOK_DATA_4_2:
			PbErrFlg = Errflg;
		break;
		
		default:
		break;
	}
}

//==============================================================================
// DESCRIPTION : static void LAP_AVCV_init(void)
//
// PARAMETERS :   void
//
// RETURN VALUE :   void
//
// DESIGN INFORMATION : The strategis of task processing.
//==============================================================================
static void LAP_AVCV_init(void)
{
	LAPAVCVInit();
	
	LAPAVCVStart();
	
	TOSTpoInitialize();
	
	DATComInit();
	
	IPCL_Adapter_DataInit();
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit ( void )
{
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
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{
	eIgnState fl_IGN_state = eIGN_OFF;
	EBatteryState fl_Battery_state = eBatteryState_Normal;
		
	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);

	if ((fl_Battery_state == eBatteryState_UnderVoltage) || (fl_Battery_state == eBatteryState_OverVoltage) || (fl_Battery_state == eBatteryState_OverVoltagePhase1))
	{
		LAP_AVCV_init();
	}
	else
	{
		switch (fl_IGN_state)
		{
			case eIGN_RUN:
				TOSClockTask();
				
				LAPAVCVTask();

				IPCL_UpdateAVCVStateData();

				IPCL_UpdateMP5NaviStateData();

				UpdateAVCVData((uint8*)&HMI_IPCL_AVCV_Data);

				UpdateMP5NavigationData((uint8*)&HMI_IPCL_MP5Navi_Data);
				break;

			case eIGN_OFF:
			default:
				LAP_AVCV_init();
				break;
		}
	}

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

//------------------------------------------------------------------------------
// Local data
//------------------------------------------------------------------------------
static tTOSClock   TOSClock;

//==============================================================================
// DESCRIPTION:         See component header file
//==============================================================================
void TOSTpoInitialize( void )
{
    TOSClock = (tTOSClock) 1;
}

//==============================================================================
// DESCRIPTION:         See component header file
//==============================================================================
void TOSStartTimer( tTOSTimer* const pTimer )
{
    if (pTimer != NULL)
    {
        *pTimer = TOSClock;
    }
}

//==============================================================================
// DESCRIPTION:         See component header file
//==============================================================================
void TOSStopTimer( tTOSTimer* const pTimer )
{
    if (pTimer != NULL)
    {
        *pTimer = 0;
    }
}

//==============================================================================
// DESCRIPTION:         See component header file
//==============================================================================
BOOL TOSIsTimerStarted( const tTOSTimer* const pTimer )
{
    BOOL bTimerIsStarted = cFalse;

    if (pTimer != NULL)
    {
        // A timer is never equal to 0
        // The 0 value is reserved to the timer stoped
        if ( *pTimer != 0 )
        {
            bTimerIsStarted = cTrue;
        }
    }

    return bTimerIsStarted;
}

//==============================================================================
// DESCRIPTION:         See component header file
//==============================================================================
BOOL TOSIsTimerElapsed( tTOSTimer* const pTimer, const tTOSTimer Timeout )
{
    tTOSTimer Delay;
    BOOL bTimerIsElapsed = cFalse;

    if (pTimer != NULL)
    {
        // A timer is never equal to 0
        // The 0 value is reserved to the timer stoped
        if ( *pTimer == 0 )
        {
            bTimerIsElapsed = cTrue;
        }
        else
        {
            Delay = (tTOSTimer)( TOSClock - *pTimer );
            
            if ( TOSClock < *pTimer )
            {
                // The 0 value had been "jump" so we must substact 1 to the delay
                Delay--;
            }
            
            if ( (Delay > Timeout) || (Timeout == 0) )
            {
                // The timer is stoped
                *pTimer = 0;
                bTimerIsElapsed = cTrue;
            }

        }
    }

    return bTimerIsElapsed;
}

//==============================================================================
// DESCRIPTION:         See component header file
//==============================================================================
void TOSClockTask( void )
{
    TOSClock += ((U8)0x01u);

    if ( TOSClock < ((U8)0x01u) )
    {
        TOSClock++;
    }
}

