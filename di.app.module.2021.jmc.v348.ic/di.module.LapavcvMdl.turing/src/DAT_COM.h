#ifndef I_DAT_COM
#define I_DAT_COM

//------------------------------------------------------------------------------
// Included files to resolve specific definitions in this file
//------------------------------------------------------------------------------
// Reduce to minimum the inclusions!!!
#include "deftypes.h"

//------------------------------------------------------------------------------
// Constant data
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Exported type
//------------------------------------------------------------------------------
//tansmit the times of AVCV message (by shenli April 12)
#define DATACOM_WriteU8BitAVCVTransmitDefault(v)			DATACOM_WriteAVCVTransmitState(v)
#define	DATACOM_ReadU8BitAVCVTransmitDefault()				DATACOM_ReadAVCVTransmitState()

#define DATACOM_WriteU8BitNaviTransmitDefault(v)			DATACOM_WriteNaviTransmitState(v)
#define	DATACOM_ReadU8BitNaviTransmitDefault()				DATACOM_ReadNaviTransmitState()

//transmit the commands from GIP to control the MP5 about audio vedio(by shenli April 16)
#define DATACOM_WriteU8BitClustCtrlDefault(v)				DATACOM_WriteClustCtrlState(v)
#define	DATACOM_ReadU8BitClustCtrlDefault()					DATACOM_ReadClustCtrlState()
#define DATACOM_WriteU1BitICSendDefault(v)  				DATACOM_WriteICSendState(v)
#define	DATACOM_ReadU1BitICSendDefault() 					DATACOM_ReadICSendState()

//Received the commands from GIP to control MP5 make a call.
#define DATACOM_WriteU8BitClustCallUpDefault(v)				DATACOM_WriteClustCallUpState(v)
#define	DATACOM_ReadU8BitClustCallUpDefault()				DATACOM_ReadClustCallUpState()

//------------------------------------------------------------------------------
// Exported functions
//------------------------------------------------------------------------------
//tansmit the times of AVCV message
extern U8	DATACOM_ReadAVCVTransmitState(void);
extern void DATACOM_WriteAVCVTransmitState(const U8 u8InVal);

extern U8	DATACOM_ReadNaviTransmitState(void);
extern void DATACOM_WriteNaviTransmitState(const U8 u8InVal);

//transmit the commands from GIP to control the MP5
extern U8	DATACOM_ReadClustCtrlState(void);
extern void DATACOM_WriteClustCtrlState(const U8 u8InVal);
extern BOOL DATACOM_ReadICSendState(void);
extern void DATACOM_WriteICSendState(const U8 u8InVal);

//Received command from GIP to let MP5 make a call.
extern U8	DATACOM_ReadClustCallUpState(void);
extern void DATACOM_WriteClustCallUpState(const U8 u8InVal);

extern void DATComInit(void);

#endif   /* I_DAT_COM */


