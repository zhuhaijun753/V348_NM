//------------------------------------------------------------------------------
// Included files to resolve specific definitions in this file
//------------------------------------------------------------------------------
#include "DAT_COM.h"

//------------------------------------------------------------------------------
// Local data
//------------------------------------------------------------------------------
U8	u8PBdataPageTX; //totally transmit 10 different data message to GIP roundly.
U8	u8Clust_TX_MP5; //the commands of cluster control MP5
U8	u8Clust_TX_CallUp; //the commands from cluster to let MP5 make a call
BOOL	bNaviTX_Enable;
BOOL	bICSendSucessed;

//==============================================================================
// DESCRIPTION :
//
// PARAMETERS (Type,Name,Min,Max) :   none
//
// RETURN VALUE :   none
//
// DESIGN INFORMATION : (pseudo code, algorithm ... )
//==============================================================================
void DATComInit(void)
{
	/*Some commands should be setted to 0 as default*/
    u8Clust_TX_MP5 = 0;
    u8Clust_TX_CallUp = 0;
	
	u8PBdataPageTX = 0;
	bNaviTX_Enable = 0;
	bICSendSucessed = 0;
}

//==============================================================================
// DESCRIPTION:DATACOM_ReadAVCVTransmitState
//
// PARAMETERS:
//
// RETURN VALUE:
//
// DESIGN INFORMATION: Recoder the order of message which sent to GIP . (Authored by shenli 12 April)
//==============================================================================
U8	DATACOM_ReadAVCVTransmitState(void)
{
	return u8PBdataPageTX;
}

void DATACOM_WriteAVCVTransmitState(const U8 u8InVal)
{
 	u8PBdataPageTX = u8InVal;
}

//==============================================================================
// DESCRIPTION: DATACOM_ReadNaviTransmitState
//
// PARAMETERS:
//
// RETURN VALUE:
//
// DESIGN INFORMATION: Control the road name transmision.)
//==============================================================================
U8	DATACOM_ReadNaviTransmitState(void)
{
	return bNaviTX_Enable;
}

void DATACOM_WriteNaviTransmitState(const U8 u8InVal)
{
 	bNaviTX_Enable = u8InVal;
}

//==============================================================================
// DESCRIPTION:
//
// PARAMETERS:
//
// RETURN VALUE:
//
// DESIGN INFORMATION: Recoder the order of message which sent to GIP . (Authored by shenli 12 April)
//==============================================================================
U8	DATACOM_ReadClustCtrlState(void)
{
	return u8Clust_TX_MP5;
}

void DATACOM_WriteClustCtrlState(const U8 u8InVal)
{
 	u8Clust_TX_MP5 = u8InVal;
 			
	/*	1 -> JoinCall
		2 -> MuteMicrophone
		3 -> UnMuteMicrophone
		4 -> PrivacyMode
		5 -> HandfreeMode
		6 -> EndCall
		7 -> USB
		8 -> BT
		9 -> Network radio
		10 -> FM
		11 -> AM
		12 -> Online musci
		13 -> Network program
		14 -> NextItem
		15 -> LastItem
 	*/
}

BOOL DATACOM_ReadICSendState(void)
{
	return bICSendSucessed;
}

void DATACOM_WriteICSendState(const U8 u8InVal)
{
 	bICSendSucessed = u8InVal;	//1:send sucess, 0:send failed
}

//==============================================================================
// DESCRIPTION:
//
// PARAMETERS:
//
// RETURN VALUE:
//
// DESIGN INFORMATION: Received the commands from GIP. These command will control MP5 make a call.
//==============================================================================
U8	DATACOM_ReadClustCallUpState(void)
{
	return u8Clust_TX_CallUp;
}

void DATACOM_WriteClustCallUpState(const U8 u8InVal)
{
 	u8Clust_TX_CallUp = u8InVal;
 			
	/*	High bits bit4~bit7
		1 -> missed calls
		2 -> received calls
		3 -> dialed calls
		4 -> all calls
		Low bits bit0~bit3
		1-8 position
 	*/
}

