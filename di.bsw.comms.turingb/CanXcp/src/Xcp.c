/* ===========================================================================

                     CONFIDENTIAL VISTEON CORPORATION

  This is an unpublished work of authorship, which contains trade secrets,
  created in 2006.  Visteon Corporation owns all rights to this work and
  intends to maintain it in confidence to preserve its trade secret status.
  Visteon Corporation reserves the right, under the copyright laws of the
  United States or those of any other country that may have jurisdiction, to
  protect this work as an unpublished work, in the event of an inadvertent
  or deliberate unauthorized publication.  Visteon Corporation also reserves
  its rights under all copyright laws to protect this work as a published
  work, when appropriate.  Those having access to this work may not copy it,
  use it, modify it or disclose the information contained in it without the
  written authorization of Visteon Corporation.

 =========================================================================*/

#define XCP_SOURCE

/***************************************************************************/
/* Include files                                                           */
/***************************************************************************/

#include "Xcp.h"
#include "xcp_appl.h"


/* Include AUTOSAR CRC module header file. */
#if defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE )
  #include "Crc.h"
#endif

/* XCP DPRAM Client Mode */


#if defined ( XCP_ENABLE_DEV_ERROR_DETECT )
  #include "Det.h"              /* Include Diagnostic Event Manager */
#endif

#if defined ( XCP_ENABLE_READCCCONFIG )
  #include "FrIf.h"
#endif

#if defined ( XCP_ENABLE_VECTOR_GENERICMEASUREMENT )
  #include "XcpGenMeas.h"
#endif

#if defined ( XCP_ENABLE_AMD_RUNTIME_MEASUREMENT )
  #if ( XCP_AUTOSARVERSION > 3 )
    #include "Rtm.h"
  #else
    #include "AmdRtm.h"
  #endif
#endif

#if defined ( XCP_ENABLE_VX_SUPPORT ) 
  #include "vx1000.h"
#endif

#if defined (XCP_GEN_GENERATOR_MSR)
# include "SchM_Xcp.h"
#endif

/***************************************************************************/
/* Version check                                                           */
/***************************************************************************/
#if ( CP_XCP_VERSION != 0x0206u )
  #error "Source and Header file are inconsistent!"
#endif
#if ( CP_XCP_RELEASE_VERSION != 0x00u )
  #error "Source and Header file are inconsistent!"
#endif

#if defined ( VGEN_GENY )
  #if defined ( CP_XCPDLL_VERSION ) 
    #if ( CP_XCPDLL_VERSION >= 0x0211u )
    #else
      #error "XCP component version and generator version are not consistent!"
    #endif
  #else 
    #if defined ( VGEN_VERSION_CP_XCP_MAIN )
      #if ( ( VGEN_VERSION_CP_XCP_MAIN > 1 ) || ( VGEN_VERSION_CP_XCP_SUB > 5 ) )
      #else
        #error "XCP component version and generator version are not consistent!"
      #endif
    #else
      #error "XCP component version and generator version are not consistent!"
    #endif
  #endif
#endif

/* Check specifcation version of AUTOSAR CRC module. */
#if defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE )
  #if ( CRC_AR_MAJOR_VERSION >= 2 )
  #else
    #error "Compatibility error: Only AUTOSAR XCP module Release 2.x.x and greater is supported!"
  #endif
#endif


/****************************************************************************/
/* Definitions                                                              */
/****************************************************************************/

/* Definition of endianess. */
#if defined ( XCP_CPUTYPE_BIGENDIAN ) || defined ( XCP_CPUTYPE_LITTLEENDIAN )
#else
  #if defined ( C_CPUTYPE_BIGENDIAN )
    #define XCP_CPUTYPE_BIGENDIAN
  #endif
  #if defined ( C_CPUTYPE_LITTLEENDIAN )
    #define XCP_CPUTYPE_LITTLEENDIAN
  #endif
  #if defined ( CPU_BYTE_ORDER )
    #if ( CPU_BYTE_ORDER == HIGH_BYTE_FIRST )
      #define XCP_CPUTYPE_BIGENDIAN
    #endif
    #if ( CPU_BYTE_ORDER == LOW_BYTE_FIRST )
      #define XCP_CPUTYPE_LITTLEENDIAN
    #endif
  #endif
#endif




/****************************************************************************/
/* Macros                                                                   */
/****************************************************************************/

/* Definition of macros that have to be used within the context of Xcp_Command. */
/* They have to be declared global Due to MISRA rule 91. */

/* PRQA S 3458 5 */ /* MD_MSR_19.4 */
#define error(e) { err=(e); goto negative_response; }
#define check_error(e) { err=(e); if (err!=0) { goto negative_response; } }
#define error1(e, b1) { err=(e); CRM_BYTE(2)=(b1); xcp.CrmLen=3; goto negative_response1; }
#define error2(e, b1, b2) { err=(e); CRM_BYTE(2)=(b1); CRM_BYTE(3)=(b2); xcp.CrmLen=4; goto negative_response1; }

#if defined ( XCP_ENABLE_SEED_KEY )
  /* Return CRC_ACCESS_LOCKED if the resource is locked. */
  #define CheckResourceProtection(resource)   if ( (xcp.ProtectionStatus & (resource)) != 0 ) { error(CRC_ACCESS_LOCKED) } /* PRQA S 3412 */ /* MD_MSR_19.4 */
#else
  /* The resource protection is unavailable. */
  #define CheckResourceProtection(resource)
#endif


#if defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE )
  /* Macros for calculation of checksum with AUTOSAR CRC module. */
  #if ( XCP_AUTOSARVERSION == 4 )
    #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC16CCITT )
      #define XcpCalculateCrc(data_ptr, length, start_value)  Crc_CalculateCRC16(data_ptr, length, start_value, FALSE) /* PRQA S 3453 */ /* MD_MSR_19.7 */
    #endif
    #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC32 )
      #define XcpCalculateCrc(data_ptr, length, start_value)  Crc_CalculateCRC32(data_ptr, length, start_value, FALSE) /* PRQA S 3453 */ /* MD_MSR_19.7 */
    #endif
  #else
    #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC16CCITT )
      #define XcpCalculateCrc(data_ptr, length, start_value)  Crc_CalculateCRC16(data_ptr, length, start_value) /* PRQA S 3453 */ /* MD_MSR_19.7 */
    #endif
    #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC32 )
      #define XcpCalculateCrc(data_ptr, length, start_value)  Crc_CalculateCRC32(data_ptr, length, start_value) /* PRQA S 3453 */ /* MD_MSR_19.7 */
    #endif
  #endif
#endif  /* defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE ) */

#if defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL )
  #define XCP_WRITE_BYTE_2_ADDR(addr, data)           XcpAppl_Write( (uint32)(addr), (uint8)(data) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #define XCP_READ_BYTE_FROM_ADDR(addr)               XcpAppl_Read ( (uint32)(addr) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #if defined ( XCP_ENABLE_CHECKSUM )
     #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD22 ) || \
         ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD24 ) || \
         ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD44 )
      #define XCP_READ_CHECKSUMVALUE_FROM_ADDR(addr)  XcpAppl_ReadChecksumValue ( (uint32)(addr) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
    #else
      #define XCP_READ_CHECKSUMVALUE_FROM_ADDR(addr)  XCP_READ_BYTE_FROM_ADDR(addr) /* PRQA S 3453 */ /* MD_MSR_19.7 */
    #endif
  #endif
#else
  #define XCP_WRITE_BYTE_2_ADDR(addr, data)           *(addr) = (data) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #define XCP_READ_BYTE_FROM_ADDR(addr)               *(addr) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #if defined ( XCP_ENABLE_CHECKSUM )
    #if defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL )
      #define XCP_READ_CHECKSUMVALUE_FROM_ADDR(addr)    XcpAppl_ReadChecksumValue ( (uint32)(addr) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
    #else
      #define XCP_READ_CHECKSUMVALUE_FROM_ADDR(addr)    *((P2VAR(tXcpChecksumAddType, AUTOMATIC, XCP_APPL_DATA))(addr)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
    #endif
  #endif
#endif


/***************************************************************************************************/
/* General definition of checking macros */
# define Xcp_CheckErrorReturnVoid(cond, api, err)        \
{ if(!(cond)){                                           \
    Xcp_CallDetReportError((api), (err));                \
    return;                                              \
}} /* PRQA S 3458 */ /* MD_MSR_19.4 */
# define Xcp_CheckErrorReturnValue(cond, api, err, ret)  \
{ if(!(cond)){                                           \
    Xcp_CallDetReportError((api), (err));                \
}} /* PRQA S 3458 */ /* MD_MSR_19.4 */
# define Xcp_CheckErrorContinue(cond, api, err)          \
{ if(!(cond)){                                           \
    Xcp_CallDetReportError((api), (err));                \
}} /* PRQA S 3458 */ /* MD_MSR_19.4 */

/***************************************************************************************************/
/* Macros for DET abstraction */
#if defined ( XCP_ENABLE_DEV_ERROR_DETECT )
# define Xcp_CheckDetErrorReturnVoid(cond, api, err)       Xcp_CheckErrorReturnVoid((cond), (api), (err)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Xcp_CheckDetErrorReturnValue(cond, api, err, ret) Xcp_CheckErrorReturnValue((cond), (api), (err), (ret)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Xcp_CheckDetErrorContinue(cond, api, err)         Xcp_CheckErrorContinue((cond), (api), (err)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Xcp_CallDetReportError(api, err)                  (void)Det_ReportError(XCP_MODULE_ID, 0u, (api), (err)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define Xcp_CheckDetErrorReturnVoid(cond, api, err)
# define Xcp_CheckDetErrorReturnValue(cond, api, err, ret) 
# define Xcp_CheckDetErrorContinue(cond, api, err) 
# define Xcp_CallDetReportError(api, err) 
#endif

/***************************************************************************************************/
/* Macros for SBSW check abstraction */
#if defined ( XCP_ENABLE_SILENT_CHECK )
# define Xcp_SilentBswCheckErrorReturnVoid(cond, api, err)       Xcp_CheckErrorReturnVoid( (cond), (api), (err)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Xcp_SilentBswCheckErrorReturnValue(cond, api, err, ret) Xcp_CheckErrorReturnValue( (cond), (api), (err), (ret)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define Xcp_SilentBswCheckErrorReturnVoid(cond, api, err) Xcp_CheckDetErrorReturnVoid( (cond), (api), (err) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Xcp_SilentBswCheckErrorReturnValue(cond, api, err, ret) Xcp_CheckDetErrorReturnValue( (cond), (api), (err), (ret) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif 

/***************************************************************************************************/
/* Macros for AMD abstraction */
#if defined ( XCP_ENABLE_AMD_RUNTIME_MEASUREMENT ) && ( XCP_AUTOSARVERSION > 3 )
  #define Xcp_Rtm_Start(channel) Rtm_Start(RtmConf_RtmMeasurementPoint_##channel)
  #define Xcp_Rtm_Stop(channel) Rtm_Stop(RtmConf_RtmMeasurementPoint_##channel)
#else
  #define Xcp_Rtm_Start(channel)
  #define Xcp_Rtm_Stop(channel)
#endif

#if defined ( XCP_ENABLE_DEV_ERROR_DETECT )
/* Initialization status of XCP. */
#define XCP_UNINIT   0
#define XCP_INIT     1
#endif


#if defined ( XCP_ENABLE_CONTROL )
  #define Xcp_CheckControlState( )               { if ( Xcp_ControlState == kXcp_Control_Disable ) { return; } } /* PRQA S 3458 */ /* MD_MSR_19.4 */
  #define Xcp_CheckControlStateRet( ret_value )  { if ( Xcp_ControlState == kXcp_Control_Disable ) { return( ret_value ); } } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#else
  #define Xcp_CheckControlState( )
  #define Xcp_CheckControlStateRet( ret_value )
#endif


#define XcpCallTlFunction( function ) ( Xcp_TlApi[xcp.Daq.activeTl].function() ) /* PRQA S 3453 */ /* MD_MSR_19.7 */ /* PRQA S 3410 */ /* MD_Xcp_3410 */
#define XcpCallTlFunction_1_Param( function, param ) ( Xcp_TlApi[xcp.Daq.activeTl].function(param) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */ /* PRQA S 3410 */ /* MD_Xcp_3410 */
#define XcpCallTlFunction_2_Param( function, param1, param2 ) ( Xcp_TlApi[xcp.Daq.activeTl].function(param1, param2) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */ /* PRQA S 3410 */ /* MD_Xcp_3410 */


/****************************************************************************/
/* Constants                                                                */
/****************************************************************************/

/****************************************************************************/
/* Constants with unspecified size for export                               */
/****************************************************************************/
#define XCP_START_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined ( XCP_ENABLE_VECTOR_INFOMEM )
CONST(tInfoMem, XCP_CONST) kInfoMem =
{
  CP_XCP_VERSION,
  CP_XCP_RELEASE_VERSION,
  #if defined ( XCP_ENABLE_DAQ )
    (uint16)kXcpDaqMemSize,
  #else
    (uint16)0u,
  #endif
  #if defined ( XCP_ENABLE_STIM )
    (uint16)kXcpStimOdtCount
  #else
    (uint16)0u
  #endif
};
#endif

#define XCP_STOP_SEC_CONST_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/****************************************************************************/
/* 8 Bit Constants for export                                               */
/****************************************************************************/
#define XCP_START_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Global constants with XCP Protocol Layer main and subversion */
CONST(uint8, XCP_CONST) kXcpMainVersion    = (uint8)(CP_XCP_VERSION >> 8);
CONST(uint8, XCP_CONST) kXcpSubVersion     = (uint8)(CP_XCP_VERSION & 0x00ff);
CONST(uint8, XCP_CONST) kXcpReleaseVersion = (uint8)(CP_XCP_RELEASE_VERSION);

#define XCP_STOP_SEC_CONST_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/****************************************************************************/
/* Local data                                                               */
/****************************************************************************/

/* This section containes all RAM locations needed by the XCP driver */

/******************************************************************************/
/* Local Data definitions with unspecified size                               */
/******************************************************************************/
#define XCP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

VAR(tXcpData, XCP_VAR_NOINIT) xcp; /* PRQA S 3408 */ /* MD_Xcp_3408 */

#define XCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/******************************************************************************/
/* Local Data definitions with 8Bit size                                      */
/******************************************************************************/
#define XCP_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined ( XCP_ENABLE_DEV_ERROR_DETECT )
STATIC VAR(uint8, XCP_VAR_NOINIT) Xcp_InitializationState;
#endif

#if defined ( XCP_ENABLE_CONTROL )
STATIC VAR(uint8, XCP_VAR_NOINIT) Xcp_ControlState;
#endif


#if defined ( XCP_ENABLE_VX_SUPPORT ) 
VAR(uint8, XCP_VAR_NOINIT) Xcp_VxActive;
#endif

STATIC VAR(uint8, XCP_VAR_NOINIT) total_frames;

#define XCP_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/***************************************************************************/
/* Prototypes for local functions                                          */
/***************************************************************************/
#define XCP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined ( XCP_ENABLE_CALIBRATION )
STATIC FUNC(uint8, XCP_CODE) Xcp_WriteMta( uint8 size, ROMBYTEPTR data );
#endif
STATIC FUNC(uint8, XCP_CODE) Xcp_ReadMta( uint8 size, BYTEPTR data );

#if defined ( Xcp_MemClr )
#else
STATIC FUNC(void, XCP_CODE) Xcp_MemClr( BYTEPTR p, uint16 n );
#endif

#if defined ( XCP_ENABLE_SEND_EVENT ) || defined ( XCP_ENABLE_SERV_TEXT )
STATIC FUNC(void, XCP_CODE) Xcp_SendEv( void );
#endif

#if defined ( XCP_ENABLE_SEND_QUEUE )
STATIC FUNC(uint8, XCP_CODE) Xcp_SendDtoFromQueue( void );
STATIC FUNC(void, XCP_CODE) Xcp_QueueInit(void);
#endif

#if defined ( XCP_ENABLE_DAQ )
STATIC FUNC(void, XCP_CODE) Xcp_FreeDaq( void );
STATIC FUNC(uint8, XCP_CODE) Xcp_AllocMemory( void );
STATIC FUNC(uint8, XCP_CODE) Xcp_AllocDaq( uint8 daqCount );
STATIC FUNC(uint8, XCP_CODE) Xcp_AllocOdt( uint8 daq, uint8 odtCount );
STATIC FUNC(uint8, XCP_CODE) Xcp_AllocOdtEntry( uint8 daq, uint8 odt, uint8 odtEntryCount );
STATIC FUNC(void, XCP_CODE) Xcp_StartDaq( uint8 daq );
STATIC FUNC(void, XCP_CODE) Xcp_StartAllSelectedDaq(void);
STATIC FUNC(void, XCP_CODE) Xcp_StopDaq( uint8 daq );
STATIC FUNC(void, XCP_CODE) Xcp_StopAllSelectedDaq(void);
STATIC FUNC(void, XCP_CODE) Xcp_StopAllDaq( void );

  #if defined ( XCP_ENABLE_STIM )
STATIC FUNC(uint8, XCP_CODE) Xcp_AllocStimMemory( void );
  #endif

#endif

#define XCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/******************************************************************************
*
*
*           Code Part for Cp_Xcp
*
*
*******************************************************************************/
#define XCP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/*****************************************************************************
| NAME:             Xcp_MemSet
| CALLED BY:        Xcp_FreeDaq
| PRECONDITIONS:    none
| INPUT PARAMETERS: p : pointer to start address.
|                   n : number of data bytes.
|                   d : data byte to initialize with.
| RETURN VALUES:    none 
| DESCRIPTION:      Initialize n bytes starting from address p with b.
******************************************************************************/

#if defined ( Xcp_MemSet )
 /* Xcp_MemSet is overwritten */
#else
FUNC(void, XCP_CODE) Xcp_MemSet( BYTEPTR p, uint16 n, uint8 b )
{
	for ( ; n > 0; n-- )
	{
		*p = b;
		p++; /* PRQA S 0489 */ /* MD_MSR_17.4 */
	}
}
#endif

/*****************************************************************************
| NAME:             Xcp_MemClr
| CALLED BY:        Xcp_FreeDaq, Xcp_Init
| PRECONDITIONS:    none
| INPUT PARAMETERS: p : pointer to start address.
|                   n : number of data bytes.
| RETURN VALUES:    none 
| DESCRIPTION:      Initialize n bytes starting from address p 0.
******************************************************************************/

#if defined ( Xcp_MemClr )
 /* Xcp_MemClr is overwritten */
#else
/* A macro would be more efficient. Due to MISRA rule 96 violations a function is implemented. */
STATIC FUNC(void, XCP_CODE) Xcp_MemClr( BYTEPTR p, uint16 n )
{
	Xcp_MemSet( p, n, (uint8)0u );
}
#endif

/*****************************************************************************
| NAME:             Xcp_MemCpy
| CALLED BY:        Xcp_Event
| PRECONDITIONS:    none
| INPUT PARAMETERS: dest : pointer to destination address.
|                   src  : pointer to source address.
|                   n    : number of data bytes to copy.
| RETURN VALUES:    none 
| DESCRIPTION:      Copy n bytes from src to dest.
|                   A maximum of 255 Bytes can be copied at once.
******************************************************************************/

/* Optimize this function 
   It is used in the inner loop of Xcp_Event for data acquisition sampling 
*/

#if defined ( Xcp_MemCpy ) 
 /* Xcp_MemCpy is overwritten */
#else
FUNC(void, XCP_CODE) Xcp_MemCpy( DAQBYTEPTR dest, ROMDAQBYTEPTR src, uint8 n )
{
	for ( ; n > 0; n-- )
	{
		XCP_WRITE_BYTE_2_ADDR( dest, *src );
		dest++; /* PRQA S 0489 */ /* MD_MSR_17.4 */
		src++; /* PRQA S 0489 */ /* MD_MSR_17.4 */
	}
}
#endif


/*****************************************************************************
| NAME:             Xcp_SetActiveTl
| CALLED BY:        Xcp_Init, Tl
| PRECONDITIONS:    none
| INPUT PARAMETERS: MaxCto : Maximum CTO to be used
|                   MaxDto : Maximum DTO to be used
| RETURN VALUES:    none 
| DESCRIPTION:      Set the maximum cto/dto to be used by the XCP.
******************************************************************************/
FUNC(void, XCP_CODE) Xcp_SetActiveTl( uint8 MaxCto, uint8 MaxDto, uint8 ActiveTl )
{
	Xcp_CheckDetErrorReturnVoid( XCP_UNINIT != Xcp_InitializationState,
		XCP_SETACTIVETL_SERVICE_ID, XCP_E_NOT_INITIALIZED )

		xcp.Daq.activeTl  = ActiveTl;
	xcp.Daq.xcpMaxCTO = MaxCto;
	/* SBSW_AsrXcp_020 */
	if( MaxCto >= kXcpMaxCTOMax )
	{
		xcp.Daq.xcpMaxCTO = kXcpMaxCTOMax;
	}
	xcp.Daq.xcpMaxDTO = MaxDto;
	/* SBSW_AsrXcp_020 */
	if( MaxDto >= kXcpMaxDTOMax )
	{
		xcp.Daq.xcpMaxDTO = kXcpMaxDTOMax;
	}
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*****************************************************************************
| NAME:             Xcp_GetActiveTl
| CALLED BY:        Tl
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    Active Tl 
| DESCRIPTION:      Get the currently active TL.
******************************************************************************/
FUNC(uint8, XCP_CODE) Xcp_GetActiveTl( void )
{
	Xcp_CheckDetErrorReturnValue( XCP_UNINIT != Xcp_InitializationState,
		XCP_GETACTIVETL_SERVICE_ID, XCP_E_NOT_INITIALIZED, (uint8)0u )
		return( xcp.Daq.activeTl );
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/****************************************************************************/
/* Transmit                                                                 */
/****************************************************************************/


/*****************************************************************************
| NAME:             Xcp_SendCrm
| CALLED BY:        Xcp_MainFunction, Xcp_Command, Xcp_SendCallBack, application
| PRECONDITIONS:    XCP is initialized and in connected state and 
|                   a command packet (CMD) has been received.
| INPUT PARAMETERS: none
| RETURN VALUES:    none 
| DESCRIPTION:      Transmission of a command response packet (RES), 
|                    or error packet (ERR) if no other packet is pending.
******************************************************************************/
FUNC(void, XCP_CODE) Xcp_SendCrm( void )
{
	Xcp_CheckDetErrorReturnVoid( XCP_UNINIT != Xcp_InitializationState,
		XCP_SENDCRM_SERVICE_ID, XCP_E_NOT_INITIALIZED )

		/* Activation control */
		Xcp_CheckControlState()

#if defined ( XCP_ENABLE_SEND_QUEUE )

	ApplXcpInterruptDisable();

   if ( (xcp.SendStatus & (uint8)XCP_SEND_PENDING) != 0 )
	{
		if ( (xcp.SendStatus & (uint8)XCP_CRM_REQUEST) != 0 )
		{
			XCP_ASSERT(0);
			xcp.SessionStatus |= (SessionStatusType)SS_ERROR;
		}
		xcp.SendStatus |= (uint8)XCP_CRM_REQUEST;
	}
	else
	{	
		xcp.SendStatus |= (uint8)XCP_CRM_PENDING;
		(void)XcpCallTlFunction_2_Param( ApplXcpSend, xcp.CrmLen, &xcp.Crm.b[0] );
	}

	ApplXcpInterruptEnable();

#else

	(void)XcpCallTlFunction_2_Param( ApplXcpSend, xcp.CrmLen, &xcp.Crm.b[0] );

#endif

	XcpCallTlFunction_1_Param( ApplXcpSendFlush, XCP_FLUSH_CTO );
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if defined ( XCP_ENABLE_SEND_EVENT ) || defined ( XCP_ENABLE_SERV_TEXT )
/*****************************************************************************
| NAME:             Xcp_SendEv
| CALLED BY:        Xcp_SendEvent, Xcp_SendCallBack, Xcp_Putchar
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    none 
| DESCRIPTION:      Send a EV or SERV message, if no other message is pending.
******************************************************************************/
STATIC FUNC(void, XCP_CODE) Xcp_SendEv( void )
{
  #if defined ( XCP_ENABLE_SEND_QUEUE )

	ApplXcpInterruptDisable();

	if ( (xcp.SendStatus & (uint8)XCP_SEND_PENDING) != 0 )
	{
		if ( (xcp.SendStatus & (uint8)XCP_EVT_REQUEST) != 0 )
		{
			XCP_ASSERT(0);
			xcp.SessionStatus |= (SessionStatusType)SS_ERROR;
		}
		xcp.SendStatus |= (uint8)XCP_EVT_REQUEST;
	}
	else
	{
		xcp.SendStatus |= (uint8)XCP_EVT_PENDING;
		(void)XcpCallTlFunction_2_Param( ApplXcpSend, xcp.EvLen, xcp.Ev.b );
		xcp.EvLen = 0;
	}

	ApplXcpInterruptEnable();

  #else

	(void)XcpCallTlFunction_2_Param( ApplXcpSend, xcp.EvLen, xcp.Ev.b );
	xcp.EvLen = 0;

  #endif

	XcpCallTlFunction_1_Param( ApplXcpSendFlush, XCP_FLUSH_CTO );
}
#endif /* XCP_ENABLE_SEND_EVENT || XCP_ENABLE_SERV_TEXT */


#if defined ( XCP_ENABLE_DAQ )
/*****************************************************************************
| NAME:             Xcp_SendDto
| CALLED BY:        Xcp_SendDtoFromQueue, Xcp_Event, Xcp_SendCallBack
| PRECONDITIONS:    none
| INPUT PARAMETERS: dto : pointer to XCP packet type definition
| RETURN VALUES:    none 
| DESCRIPTION:      Send a DTO.
******************************************************************************/
  #if defined ( Xcp_SendDto )
  /* Xcp_SendDto is redefined */
  #else
    #define Xcp_SendDto(dto) (void)XcpCallTlFunction_2_Param( ApplXcpSend, (dto)->buffer.l, &(dto)->buffer.b[0] ) /* PRQA S 3453 */ /* MD_MSR_19.7 */ /* PRQA S 3412 */ /* MD_MSR_19.4 */
  #endif
#endif /* XCP_ENABLE_DAQ */


#if defined ( XCP_ENABLE_SEND_QUEUE )
/*****************************************************************************
| NAME:             Xcp_SendDtoFromQueue
| CALLED BY:        Xcp_Event, Xcp_SendCallBack
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    0 : DTO has NOT been transmitted from queue. 
|                   1 : DTO has been transmitted from queue. 
| DESCRIPTION:      Send a DTO from the queue.
******************************************************************************/
STATIC FUNC(uint8, XCP_CODE) Xcp_SendDtoFromQueue( void )
{
	ApplXcpInterruptDisable();
	if ( ( (xcp.SendStatus & (uint8)XCP_SEND_PENDING) == 0 ) && ( xcp.QueueLen != 0 ))
	{
		xcp.SendStatus |= (uint8)XCP_DTO_PENDING;
		Xcp_SendDto(&xcp.pQueue[xcp.QueueRp]);
		xcp.QueueRp++;
		if (xcp.QueueRp>=xcp.QueueSize)
		{
			xcp.QueueRp = (uint16)0u;
		}
		xcp.QueueLen--;
		ApplXcpInterruptEnable();
		return (uint8)1u;
	}
	ApplXcpInterruptEnable();
	return (uint8)0u;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* XCP_ENABLE_SEND_QUEUE */


/****************************************************************************/
/* Transmit Queue */
/****************************************************************************/

#if defined ( XCP_ENABLE_SEND_QUEUE )

/*****************************************************************************
| NAME:             Xcp_QueueInit
| CALLED BY:        Xcp_FreeDaq, Xcp_StopDaq, Xcp_StopAllDaq
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    none 
| DESCRIPTION:      Initialize the transmit queue.
******************************************************************************/
STATIC FUNC(void, XCP_CODE) Xcp_QueueInit(void)
{
	xcp.QueueLen = (uint16)0u;
	xcp.QueueRp = (uint16)0u;
}

#endif /* XCP_ENABLE_SEND_QUEUE */


/****************************************************************************/
/* Handle Mta (Memory-Transfer-Address) */
/****************************************************************************/

/* Assign a pointer to a Mta */
#if defined ( XcpSetMta )
#else
  #if defined ( XCP_ENABLE_READCCCONFIG )
    #define XcpSetMta(p, e) xcp.Mta = (p); xcp.MtaExt = (e) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #else
    #define XcpSetMta(p, e) (xcp.Mta = (p)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  #endif
#endif

/*****************************************************************************
| NAME:             Xcp_WriteMta
| CALLED BY:        Xcp_Command
| PRECONDITIONS:    none
| INPUT PARAMETERS: size : number of data bytes.
|                   data : address of data.
| RETURN VALUES:    XCP_CMD_OK, XCP_CMD_DENIED
| DESCRIPTION:      Write n bytes.
|                   Copying of size bytes from data to xcp.Mta
******************************************************************************/
#if defined ( XCP_ENABLE_CALIBRATION )
STATIC FUNC(uint8, AUTOMATIC) Xcp_WriteMta( uint8 size, ROMBYTEPTR data )
{
#if defined ( XCP_ENABLE_WRITE_EEPROM ) || defined ( XCP_ENABLE_WRITE_PROTECTION ) \
    || ( defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ) ) \
    || defined ( XCP_ENABLE_TYPESAVE_COPY )
	VAR(uint8, AUTOMATIC) r;
#endif

	/* DPRAM Client */

	/* Checked ram memory write access */
#if defined ( XCP_ENABLE_WRITE_PROTECTION )
	r = XcpAppl_CheckWriteAccess(xcp.Mta, size);
	if ( r != (uint8)XCP_CMD_OK )
	{
		xcp.Mta += size; /* PRQA S 0488 */ /* MD_Xcp_0488 */
		return r;
	}
#endif

	/* EEPROM write access */
#if defined ( XCP_ENABLE_WRITE_EEPROM )
	r = XcpAppl_CheckWriteEEPROM(xcp.Mta, size, data);
	if ( r != (uint8)XCP_CMD_DENIED )  /* EEPROM write access */
	{
		xcp.Mta += size; /* PRQA S 0488 */ /* MD_Xcp_0488 */
		return r;
	}
#endif

	/* Standard RAM memory write access */
#if defined ( XCP_ENABLE_TYPESAVE_COPY ) \
  || (defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ))
	r = XcpAppl_CalibrationWrite(xcp.Mta, data, size);
	xcp.Mta += size; /* PRQA S 0488 */ /* MD_Xcp_0488 */
	return r;
#else
	while ( size > (uint8)0u )
	{
		XCP_WRITE_BYTE_2_ADDR( xcp.Mta, *data );
		xcp.Mta++; /* PRQA S 0489 */ /* MD_MSR_17.4 */
		data++; /* PRQA S 0489 */ /* MD_MSR_17.4 */
		size--;
	}
	return (uint8)XCP_CMD_OK;
#endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/*****************************************************************************
| NAME:             Xcp_ReadMta
| CALLED BY:        Xcp_Command
| PRECONDITIONS:    none
| INPUT PARAMETERS: size :
|                   data : address of data
| RETURN VALUES:    XCP_CMD_OK
| DESCRIPTION:      Read n bytes.
|                   Copying of size bytes from data to xcp.Mta
******************************************************************************/
STATIC FUNC(uint8, AUTOMATIC) Xcp_ReadMta( uint8 size, BYTEPTR data )
{
#if defined ( XCP_ENABLE_READ_EEPROM ) || defined ( XCP_ENABLE_READ_PROTECTION ) \
    || ( defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ) ) \
    || defined ( XCP_ENABLE_TYPESAVE_COPY )
	VAR(uint8, AUTOMATIC) r;
#endif

	/* DPRAM Client */

#if defined ( XCP_ENABLE_READCCCONFIG )
	if( (uint8)XCP_ADDR_EXT_READCCCONFIG == xcp.MtaExt )
	{
		/* Read out CC config */
		uint32 regValue;
		if( (0 != (size & 0x03)) || (0 != ((uint32)xcp.Mta & 0x03)) ) /* PRQA S 0306 6 */ /* MD_Xcp_0306 */
		{
			/* we only allow to read whole words */
			return (uint8)XCP_CMD_DENIED;
		}
		while(size > 0)
		{
			if( E_OK == FrIf_ReadCCConfig((uint8)((uint32)xcp.Mta >> 10), (uint8)((uint32)xcp.Mta >> 2), &regValue) ) /* PRQA S 0306 6 */ /* MD_Xcp_0306 */
			{
				data[0] = (uint8)(regValue >>  0);
				data[1] = (uint8)(regValue >>  8);
				data[2] = (uint8)(regValue >> 16);
				data[3] = (uint8)(regValue >> 24);
				xcp.Mta += 4; /* PRQA S 0488 */ /* MD_Xcp_0488 */
				data += 4; /* PRQA S 0488 */ /* MD_Xcp_0488 */
				size -= 4;
			}
			else
			{
				return (uint8)XCP_CMD_DENIED;
			}
		}
		return (uint8)XCP_CMD_OK;
	}
#endif

	/* Checked ram memory read access */
#if defined ( XCP_ENABLE_READ_PROTECTION )
	r = XcpAppl_CheckReadAccess(xcp.Mta, (uint32)size);
	if ( r != (uint8)XCP_CMD_OK )
	{
		xcp.Mta += size; /* PRQA S 0488 */ /* MD_Xcp_0488 */
		return r;
	}
#endif

	/* EEPROM read access */
#if defined ( XCP_ENABLE_READ_EEPROM )
	r = XcpAppl_CheckReadEEPROM(xcp.Mta, size, data);
	if ( r != (uint8)XCP_CMD_DENIED )
	{
		xcp.Mta += size; /* PRQA S 0488 */ /* MD_Xcp_0488 */
		return r; /* ESCAN00014775, ESCAN00016958 */
	}
#endif

  #if defined ( XCP_ENABLE_TYPESAVE_COPY ) \
  || (defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ))
	r = XcpAppl_MeasurementRead(data, xcp.Mta, size);
	xcp.Mta += size; /* PRQA S 0488 */ /* MD_Xcp_0488 */
	return r;
  #else
	/* Standard RAM memory read access */
	while (size > 0)
	{
		/* 
		Compiler bug Tasking
		*(data++) = *(xcp.Mta++);
		*/
		*(data) = XCP_READ_BYTE_FROM_ADDR( xcp.Mta );
		data++; /* PRQA S 0489 */ /* MD_MSR_17.4 */
		xcp.Mta++; /* PRQA S 0489 */ /* MD_MSR_17.4 */
		size--;
	}
	return (uint8)XCP_CMD_OK;
  #endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/****************************************************************************/
/* Data Aquisition Setup                                                    */
/****************************************************************************/


#if defined ( XCP_ENABLE_DAQ )

/*****************************************************************************
| NAME:             Xcp_FreeDaq
| CALLED BY:        Xcp_Command
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Free all dynamic DAQ lists
******************************************************************************/
STATIC FUNC(void, XCP_CODE) Xcp_FreeDaq( void )
{
	xcp.SessionStatus &= (SessionStatusType)(~(SessionStatusType)SS_DAQ); /* PRQA S 0277 */ /* MD_Xcp_0277 */

	xcp.Daq.DaqCount = 0;
	xcp.Daq.OdtCount = 0;
	xcp.Daq.OdtEntryCount = 0;

	xcp.pOdt = (P2VAR(tXcpOdt, AUTOMATIC, XCP_APPL_DATA))0;
	xcp.pOdtEntryAddr = 0;
	xcp.pOdtEntrySize = 0;

	Xcp_MemClr((BYTEPTR)&xcp.Daq.u.b[0], (uint16)kXcpDaqMemSize);
  #if defined ( kXcpMaxEvent ) && ! defined ( XCP_ENABLE_STIM )
	Xcp_MemSet( (BYTEPTR)&xcp.Daq.EventDaq[0], (uint16)sizeof(xcp.Daq.EventDaq), (uint8)0xFFu);
  #endif

  #if defined ( XCP_ENABLE_SEND_QUEUE )
	Xcp_QueueInit();
  #endif
}

/*****************************************************************************
| NAME:             Xcp_AllocMemory
| CALLED BY:        Xcp_AllocDaq, Xcp_AllocOdt, Xcp_AllocOdtEntry, Xcp_Init
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    0, CRC_MEMORY_OVERFLOW
| DESCRIPTION:      Allocate Memory for daq,odt,odtEntries and Queue
|                   according to DaqCount, OdtCount and OdtEntryCount
******************************************************************************/
STATIC FUNC(uint8, XCP_CODE) Xcp_AllocMemory( void )
{
	VAR(uint16, AUTOMATIC) s;
  #if defined ( XCP_ENABLE_NO_P2INT_CAST  )
	P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) p;
	VAR(uint8, AUTOMATIC) i;
  #endif

	/* Check memory overflow */
	s = (uint16)( ( xcp.Daq.DaqCount      *   (uint8)sizeof(tXcpDaqList)                       ) 
		+ ( xcp.Daq.OdtCount      *   (uint16)sizeof(tXcpOdt)                          ) 
		+ ( xcp.Daq.OdtEntryCount * ( (uint8)sizeof(DAQBYTEPTR) + (uint8)sizeof(uint8) ) )
		);
  #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
    #if ( CPU_TYPE == CPU_TYPE_8 ) || ( CPU_TYPE == CPU_TYPE_16 )
	s += 3; /* Worst case 3 bytes needed for alignment */
    #else
	s += 9; /* Worst case 9 bytes needed for alignment */
    #endif
  #endif
	if (s>=(uint16)kXcpDaqMemSize)
	{
		return (uint8)CRC_MEMORY_OVERFLOW;
	}

	/* Force WORD alignment for ODTs */
  #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
    #if defined ( XCP_ENABLE_NO_P2INT_CAST  )
	p = (P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA))&xcp.Daq.u.DaqList[xcp.Daq.DaqCount];
	i = ((uint8)p) & (uint8)0x01u;
	p += i;
	xcp.pOdt = (P2VAR(tXcpOdt, AUTOMATIC, XCP_APPL_DATA))p;
    #else
	/* PRQA S 0306 6 */ /* MD_Xcp_0306 */
      #if ( CPU_TYPE == CPU_TYPE_8 ) || ( CPU_TYPE == CPU_TYPE_16 )
	/* Allign to words */
	xcp.pOdt = (P2VAR(tXcpOdt, AUTOMATIC, XCP_APPL_DATA))((((uint32)(&xcp.Daq.u.DaqList[xcp.Daq.DaqCount])) + (uint32)1u) & (uint32)0xFFFFFFFEu );
      #else
	/* Allign to dwords */
	xcp.pOdt = (P2VAR(tXcpOdt, AUTOMATIC, XCP_APPL_DATA))((((uint32)(&xcp.Daq.u.DaqList[xcp.Daq.DaqCount])) + (uint32)3u) & (uint32)0xFFFFFFFCu );
      #endif
    #endif
  #else
	xcp.pOdt = (P2VAR(tXcpOdt, AUTOMATIC, XCP_APPL_DATA))&xcp.Daq.u.DaqList[xcp.Daq.DaqCount];
  #endif

	/* Force DWORD alignment for ODT entries */
  #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
    #if defined ( XCP_ENABLE_NO_P2INT_CAST  )
	p = (P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA))&xcp.pOdt[xcp.Daq.OdtCount];
	i = ((uint8)p) & (uint8)0x03u;
	i = (uint8)4u - i;
	i &= 0x03;
	p += i;
	xcp.pOdtEntryAddr = (P2VAR(DAQBYTEPTR, AUTOMATIC, XCP_APPL_DATA))p;
    #else
	/* PRQA S 0306 5 */ /* MD_Xcp_0306 */
	xcp.pOdtEntryAddr = (P2VAR(DAQBYTEPTR, AUTOMATIC, XCP_APPL_DATA)) ((((uint32)&xcp.pOdt[xcp.Daq.OdtCount]) + (uint32)3u) & (uint32)0xFFFFFFFCu );
    #endif
  #else
	xcp.pOdtEntryAddr = (P2VAR(DAQBYTEPTR, AUTOMATIC, XCP_APPL_DATA))&xcp.pOdt[xcp.Daq.OdtCount];
  #endif

	/* PRQA S 0310 1 */ /* MD_Xcp_0310_ManualCalc */
	xcp.pOdtEntrySize = (P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA))&xcp.pOdtEntryAddr[xcp.Daq.OdtEntryCount];

  #if defined ( XCP_ENABLE_SEND_QUEUE )
	/* Force WORD alignment for the queue */
    #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
    #if defined ( XCP_ENABLE_NO_P2INT_CAST  )
	p = (P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA))&xcp.pOdtEntrySize[xcp.Daq.OdtEntryCount];
	i = ((uint8)p) & (uint8)0x01u;
	p += i;
	xcp.pQueue = (P2VAR(tXcpDto, AUTOMATIC, XCP_APPL_DATA))p;
    #else
      #if ( CPU_TYPE == CPU_TYPE_8 ) || ( CPU_TYPE == CPU_TYPE_16 )
	/* Allign to words */
	/* PRQA S 0306 4 */ /* MD_Xcp_0306 */
	xcp.pQueue = (P2VAR(tXcpDto, AUTOMATIC, XCP_APPL_DATA))((((uint32)(&xcp.pOdtEntrySize[xcp.Daq.OdtEntryCount])) + (uint32)1u) & (uint32)0xFFFFFFFEu );
      #else
	/* Allign to dwords */
	xcp.pQueue = (P2VAR(tXcpDto, AUTOMATIC, XCP_APPL_DATA))((((uint32)(&xcp.pOdtEntrySize[xcp.Daq.OdtEntryCount])) + (uint32)3u) & (uint32)0xFFFFFFFCu );
      #endif
    #endif
    #else
	xcp.pQueue = (P2VAR(tXcpDto, AUTOMATIC, XCP_APPL_DATA))&xcp.pOdtEntrySize[xcp.Daq.OdtEntryCount];
    #endif

	xcp.QueueSize = ((uint16)kXcpDaqMemSize - s) / sizeof(tXcpDto);

    #if defined ( kXcpSendQueueMinSize )
	if (xcp.QueueSize<(uint16)kXcpSendQueueMinSize)
	{
		return (uint8)CRC_MEMORY_OVERFLOW;
	}
    #else
	/* At least one queue entry per odt */
	if (xcp.QueueSize<xcp.Daq.OdtCount)
	{
		return (uint8)CRC_MEMORY_OVERFLOW;
	}
    #endif
  #endif /* XCP_ENABLE_SEND_QUEUE */


	return (uint8)0u;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

  #if defined ( XCP_ENABLE_STIM )
/*****************************************************************************
| NAME:             Xcp_AllocStimMemory
| CALLED BY:        Xcp_Command
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    0, CRC_MEMORY_OVERFLOW
| DESCRIPTION:      Allocate Memory for daq,odt,odtEntries and Queue
|                   according to DaqCount, OdtCount and OdtEntryCount
******************************************************************************/
STATIC FUNC(uint8, XCP_CODE) Xcp_AllocStimMemory( void )
{
	VAR(tXcpOdtIdx, AUTOMATIC) odt;
	VAR(tXcpOdtIdx, AUTOMATIC) n;
	VAR(uint8, AUTOMATIC) daq;

	n = 0;
	for (daq=0; daq<xcp.Daq.DaqCount; daq++)
	{
		for (odt=DaqListFirstOdt(daq); odt<=DaqListLastOdt(daq); odt++)
		{
			if ( (DaqListFlags(daq)&(uint8)DAQ_FLAG_DIRECTION) != 0x00u )
			{
				if (n >= (uint8)kXcpStimOdtCount)
				{
					return (uint8)CRC_MEMORY_OVERFLOW;
				}
				xcp.StimBuffer[n].buffer.b[0] = (uint8)0xFFu; /* Invalidate this buffer */
				DaqListOdtStimBuffer(odt) = &xcp.StimBuffer[n];
				n++;
			}
			else
			{
				DaqListOdtStimBuffer(odt) = 0;
			}
		} /* odt */
	} /* daq */

	return (uint8)0u;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
  #endif


/*****************************************************************************
| NAME:             Xcp_AllocDaq
| CALLED BY:        Xcp_Command
| PRECONDITIONS:    none
| INPUT PARAMETERS: daqCount : 
| RETURN VALUES:    return value of Xcp_AllocMemory, CRC_SEQUENCE
| DESCRIPTION:      Allocate DAQ list
******************************************************************************/
STATIC FUNC(uint8, XCP_CODE) Xcp_AllocDaq( uint8 daqCount )
{
	uint8 retValue;

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
	if ( (xcp.Daq.OdtCount!=0) || (xcp.Daq.OdtEntryCount!=0) )
	{
		return (uint8)CRC_SEQUENCE;
	}
  #endif

	xcp.Daq.DaqCount = daqCount;

	retValue = Xcp_AllocMemory();
	if(0u != retValue)
	{
		Xcp_FreeDaq();
	}
	return(retValue);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*****************************************************************************
| NAME:             Xcp_AllocOdt
| CALLED BY:        Xcp_Command
| PRECONDITIONS:    none
| INPUT PARAMETERS: daq      : 
|                   odtCount :
| RETURN VALUES:    return value of Xcp_AllocMemory,
|                   CRC_SEQUENCE, CRC_MEMORY_OVERFLOW
| DESCRIPTION:      Allocate all ODTs in a DAQ list
******************************************************************************/
STATIC FUNC(uint8, XCP_CODE) Xcp_AllocOdt( uint8 daq, uint8 odtCount )
{
	uint8 retValue;

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
	if ( (xcp.Daq.DaqCount==0) || (xcp.Daq.OdtEntryCount!=0) )
	{
		return (uint8)CRC_SEQUENCE;
	}
  #endif

	/* Absolute ODT count must fit in a BYTE */
  #if !defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )
	if (((uint16)xcp.Daq.OdtCount+(uint16)odtCount) > (uint16)0xFBu)
	{
		return (uint8)CRC_MEMORY_OVERFLOW;
	}
  #endif

	xcp.Daq.u.DaqList[daq].firstOdt = xcp.Daq.OdtCount;
	xcp.Daq.OdtCount += (tXcpOdtCnt)odtCount;
	xcp.Daq.u.DaqList[daq].lastOdt = (tXcpOdtIdx)(xcp.Daq.OdtCount-1);

	retValue = Xcp_AllocMemory();
	if(0u != retValue)
	{
		Xcp_FreeDaq();
	}
	return(retValue);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*****************************************************************************
| NAME:             Xcp_AllocOdtEntry
| CALLED BY:        Xcp_Command
| PRECONDITIONS:    none
| INPUT PARAMETERS: daq :
|                   odt :
|                   odtEntryCount :
| RETURN VALUES:    return value of Xcp_AllocMemory
| DESCRIPTION:      Allocate all ODT entries
|                   Parameter odt is relative odt number
******************************************************************************/
STATIC FUNC(uint8, XCP_CODE) Xcp_AllocOdtEntry( uint8 daq, uint8 odt, uint8 odtEntryCount )
{
	tXcpOdtIdx xcpFirstOdt;
	uint8 retValue;

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
	if ( (xcp.Daq.DaqCount==0) || (xcp.Daq.OdtCount==0) )
	{
		return (uint8)CRC_SEQUENCE;
	}
	if (odtEntryCount==0)
	{
		return (uint8)CRC_CMD_SYNTAX;
	}
  #endif

	/* Absolute ODT entry count count must fit in a WORD */
	if (xcp.Daq.OdtEntryCount > (0xFFFFu - odtEntryCount))
	{
		return (uint8)CRC_MEMORY_OVERFLOW;
	}

	xcpFirstOdt = xcp.Daq.u.DaqList[daq].firstOdt;
  #if defined ( XCP_ENABLE_SILENT_CHECK )
	/* SBSW_AsrXcp_027 */
	/* PRQA S 0310 2 */ /* MD_Xcp_0310 */
	if( ((P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA))&xcp.pOdt[xcpFirstOdt+odt].lastOdtEntry >= &xcp.Daq.u.b[kXcpDaqMemSize])
		|| ((P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA))&xcp.pOdt[xcpFirstOdt+odt].firstOdtEntry < &xcp.Daq.u.b[0]) )
	{
		return (uint8)CRC_MEMORY_OVERFLOW;
	}
  #endif
	xcp.pOdt[xcpFirstOdt+odt].firstOdtEntry = xcp.Daq.OdtEntryCount;
	xcp.Daq.OdtEntryCount += (uint16)odtEntryCount;
	xcp.pOdt[xcpFirstOdt+odt].lastOdtEntry = (uint16)(xcp.Daq.OdtEntryCount-1);

	retValue = Xcp_AllocMemory();
	if(0u != retValue)
	{
		Xcp_FreeDaq();
	}
	return(retValue);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*****************************************************************************
| NAME:             Xcp_StartDaq
| CALLED BY:        Xcp_Command, Xcp_StartAllSelectedDaq
| PRECONDITIONS:    none
| INPUT PARAMETERS: daq :
| RETURN VALUES:    none
| DESCRIPTION:      Start DAQ
******************************************************************************/
STATIC FUNC(void, XCP_CODE) Xcp_StartDaq( uint8 daq )
{
	/* Initialize the DAQ list */
	DaqListFlags(daq) |= (uint8)DAQ_FLAG_RUNNING;
  #if defined ( XCP_ENABLE_DAQ_PRESCALER )
	DaqListCycle(daq) = 1;
  #endif

	xcp.SessionStatus |= (SessionStatusType)SS_DAQ;
}

/*****************************************************************************
| NAME:             Xcp_StartAllSelectedDaq
| CALLED BY:        Xcp_Command, Xcp_Init
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Start all selected DAQs
******************************************************************************/
STATIC FUNC(void, XCP_CODE) Xcp_StartAllSelectedDaq(void)
{
	VAR(uint8, AUTOMATIC) daq;

	/* Start all selected DAQs */
	for (daq=0;daq<xcp.Daq.DaqCount;daq++)
	{
		if ( (DaqListFlags(daq) & (uint8)DAQ_FLAG_SELECTED) != 0 )
		{
			Xcp_StartDaq(daq);
			DaqListFlags(daq) &= (uint8)(~DAQ_FLAG_SELECTED & 0x00FFu);
		}
	}
}

/*****************************************************************************
| NAME:             Xcp_StopDaq
| CALLED BY:        Xcp_Command, Xcp_StopAllSelectedDaq
| PRECONDITIONS:    none
| INPUT PARAMETERS: daq : 
| RETURN VALUES:    none
| DESCRIPTION:      Stop DAQ
******************************************************************************/
STATIC FUNC(void, XCP_CODE) Xcp_StopDaq( uint8 daq )
{
	VAR(uint8, AUTOMATIC) i;

	DaqListFlags(daq) &= (uint8)(DAQ_FLAG_DIRECTION|DAQ_FLAG_TIMESTAMP|DAQ_FLAG_NO_PID);

	/* Check if all DAQ lists are stopped */
	for (i=0;i<xcp.Daq.DaqCount;i++)
	{
		if ( (DaqListFlags(i) & (uint8)DAQ_FLAG_RUNNING) != 0 )
		{
			return;
		}
	}

	xcp.SessionStatus &= (SessionStatusType)(~(SessionStatusType)SS_DAQ); /* PRQA S 0277 */ /* MD_Xcp_0277 */

  #if defined ( XCP_ENABLE_SEND_QUEUE )
	Xcp_QueueInit();
  #endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*****************************************************************************
| NAME:             Xcp_StopAllSelectedDaq
| CALLED BY:        Xcp_Command
| PRECONDITIONS:    none
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      Stop all selected DAQs
******************************************************************************/
STATIC FUNC(void, XCP_CODE) Xcp_StopAllSelectedDaq(void)
{
	VAR(uint8, AUTOMATIC) daq;

	for (daq=0;daq<xcp.Daq.DaqCount;daq++)
	{
		if ( (DaqListFlags(daq) & (uint8)DAQ_FLAG_SELECTED) != 0 )
		{
			Xcp_StopDaq(daq);
			DaqListFlags(daq) &= (uint8)(~DAQ_FLAG_SELECTED & 0x00FFu);
		}
	}
}

/*****************************************************************************
| NAME:             Xcp_StopAllDaq
| CALLED BY:        Xcp_Command, Xcp_Disconnect
| PRECONDITIONS:    none
| INPUT PARAMETERS: none 
| RETURN VALUES:    none
| DESCRIPTION:      Stop all DAQs
******************************************************************************/
STATIC FUNC(void, XCP_CODE) Xcp_StopAllDaq( void )
{
	VAR(uint8, AUTOMATIC) daq;

	for (daq=0; daq<xcp.Daq.DaqCount; daq++)
	{
		DaqListFlags(daq) &= (uint8)(DAQ_FLAG_DIRECTION|DAQ_FLAG_TIMESTAMP|DAQ_FLAG_NO_PID);
	}

	xcp.SessionStatus &= (SessionStatusType)(~(SessionStatusType)SS_DAQ); /* PRQA S 0277 */ /* MD_Xcp_0277 */

  #if defined ( XCP_ENABLE_SEND_QUEUE )
	Xcp_QueueInit();
  #endif
}


/****************************************************************************/
/* Data Aquisition Processor                                                */
/****************************************************************************/

/*****************************************************************************
| NAME:             Xcp_StimEventStatus
| CALLED BY:        application
| PRECONDITIONS:    The XCP is initialized and in connected state.
| INPUT PARAMETERS: event : event channel number to process
|                   action : STIM_CHECK_ODT_BUFFER or STIM_RESET_ODT_BUFFER
| RETURN VALUES:    1 (TRUE) if STIM data is available and Xcp_Event() can be called
| DESCRIPTION:      Handling of data acquisition event channel.
******************************************************************************/
  #if defined ( XCP_ENABLE_STIM )
FUNC(uint8, XCP_CODE) Xcp_StimEventStatus( tXcpEventType event, uint8 action )
{
	P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA)  d;
	VAR(uint16, AUTOMATIC)                  e;
	VAR(tXcpOdtIdx, AUTOMATIC)              odt;
	VAR(uint8, AUTOMATIC)                   daq;

	Xcp_CheckDetErrorReturnValue( XCP_UNINIT != Xcp_InitializationState,
		XCP_STIMEVENTSTATUS_SERVICE_ID, XCP_E_NOT_INITIALIZED, (uint8)0u )

		/* Activation control */
		Xcp_CheckControlStateRet((uint8)0u)

		if ( (xcp.SessionStatus & (SessionStatusType)SS_DAQ) == 0 )
	{
		return (uint8)0u;
	}

	for (daq=0; daq<xcp.Daq.DaqCount; daq++)
	{
		if ( (DaqListFlags(daq) & (uint8)DAQ_FLAG_RUNNING) == 0 )
		{
			continue; /* PRQA S 0770 */ /* MD_Xcp_0770 */
		}
		if ( DaqListEventChannel(daq) != event )
		{
			continue; /* PRQA S 0770 */ /* MD_Xcp_0770 */
		}

		if ( (DaqListFlags(daq) & (uint8)DAQ_FLAG_DIRECTION) != 0 ) /* STIM */
		{
			for (odt=DaqListFirstOdt(daq);odt<=DaqListLastOdt(daq);odt++)
			{
				e = DaqListOdtFirstEntry(odt);
				if ( OdtEntrySize(e) == (uint8)0u )
				{
					continue; /* PRQA S 0770 */ /* MD_Xcp_0770 */
				}
				d = (P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA)) DaqListOdtStimBuffer(odt); /* PRQA S 0310 */ /* MD_Xcp_0310_ByteCopy */
				XCP_ASSERT(d!=0);
				if (action == (uint8)STIM_CHECK_ODT_BUFFER)
				{
					if ( *d == (uint8)0xFFu )
					{
						return (uint8)0u; /* No STIM data available */
					}
				}
				else /* STIM_RESET_ODT_BUFFER */
				{
					*d = (uint8)0xFFu;
				}

			} /* odt */

		}

	} /* daq */

	return (uint8)1u;

} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
  #endif


/*****************************************************************************
| NAME:             Xcp_Event
| CALLED BY:        application
| PRECONDITIONS:    The XCP is initialized and in connected state.
| INPUT PARAMETERS: event : event channel number to process
| RETURN VALUES:    status code (XCP_EVENT_xxxx)
| DESCRIPTION:      Handling of data acquisition or stimulation event channel.
******************************************************************************/
FUNC(uint8, XCP_CODE) Xcp_Event( tXcpEventType event )
{
	P2VAR(tXcpDto, AUTOMATIC, XCP_APPL_DATA) dtop;
	BYTEPTR     dst;
	uint16      e, el;
	tXcpOdtIdx  odt;
	uint8       status;
	uint8       n;
	uint8       daq;
	uint8       i;
	uint8 		err;
  #if defined ( XCP_ENABLE_SEND_QUEUE )
  #else
	static tXcpDto dto; /* ESCAN00025020 */ /* PRQA S 0759 */ /* MD_MSR_18.4 */
  #endif
  #if defined ( XCP_ENABLE_DAQ_TIMESTAMP ) && defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
  XcpDaqTimestampType t;
  #endif

#if defined ( XCP_ENABLE_VX_SUPPORT )
	if(0 != Xcp_VxActive)
	{
		VX1000_EVENT(event)
			return (uint8)XCP_EVENT_NOP;
	}
#endif
	if(total_frames > 0)
	{
		if( (xcp.SendStatus & (uint8)XCP_SEND_PENDING) == 0 )
		{
		    		err = Xcp_ReadMta(7, CRM_SHORT_UPLOAD_DATA);
					err = err;/*clear build warning*/
					Xcp_SendCrm();
					total_frames--;
		}  
	}

	status = (uint8)0u;

	if ( (xcp.SessionStatus & (SessionStatusType)SS_DAQ) == 0 )
	{
		return (uint8)XCP_EVENT_NOP;
	}

	/* Activation control */
	Xcp_CheckControlStateRet((uint8)XCP_EVENT_NOP)

  #if defined ( kXcpMaxEvent ) && ! defined( XCP_ENABLE_STIM )

    #if defined ( XCP_ENABLE_PARAMETER_CHECK )
	if (event >= (uint8)kXcpMaxEvent)
	{
		return (uint8)XCP_EVENT_NOP;
	}
    #endif

	Xcp_Rtm_Start(Xcp_Event);
	daq = xcp.Daq.EventDaq[event];
	if ( ((daq<xcp.Daq.DaqCount) && ( (DaqListFlags(daq) & (uint8)DAQ_FLAG_RUNNING) != 0 )) != 0 )
	{

  #else

		Xcp_Rtm_Start(Xcp_Event);
		for (daq=0; daq<xcp.Daq.DaqCount; daq++)
		{
			if ( (DaqListFlags(daq)& (uint8)DAQ_FLAG_RUNNING) == 0 )
			{
				continue; /* PRQA S 0770 */ /* MD_Xcp_0770 */
			}
			if ( DaqListEventChannel(daq) != event )
			{
				continue; /* PRQA S 0770 */ /* MD_Xcp_0770 */
			}

  #endif

  #if defined ( XCP_ENABLE_DAQ_PRESCALER )
			DaqListCycle(daq)--;
			if ( DaqListCycle(daq) == (uint8)0 )
			{
				DaqListCycle(daq) = DaqListPrescaler(daq);
  #endif

				/* Data Stimulation (STIM) */
  #if defined ( XCP_ENABLE_STIM )

				if ( (DaqListFlags(daq)&(uint8)DAQ_FLAG_DIRECTION) != 0 ) /* STIM */
				{
					status |= (uint8)XCP_EVENT_STIM;
					for (odt=DaqListFirstOdt(daq);odt<=DaqListLastOdt(daq);odt++)
					{
						if(0u == DaqListOdtEntryCount(odt))
						{
							break;
						}
						e = DaqListOdtFirstEntry(odt);
						if (OdtEntrySize(e)==(uint8)0u)
						{
							break; /* PRQA S 0771 */ /* MD_Xcp_0771 */
						}
						el = DaqListOdtLastEntry(odt);
						dst = (P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA))DaqListOdtStimBuffer(odt); /* PRQA S 0310 */ /* MD_Xcp_0310 */
						XCP_ASSERT(d!=0);

						if ( *dst == (uint8)0xFFu )
						{
							status |= (uint8)XCP_EVENT_STIM_OVERRUN; /* No STIM data available */
						}
						else
						{
							ApplXcpInterruptDisableStim(); /* The following code is not reentrant */

							/* This is the inner loop, optimize here */
							*dst = (uint8)0xFFu; /* Invalidate STIM data buffer */
    #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )
							if (odt == DaqListFirstOdt(daq))
							{
								/* Time stamp is only applicable in first odt of STIM */
      #if !defined ( XCP_ENABLE_DAQ_TIMESTAMP_FIXED )
								if ( (DaqListFlags(daq) & (uint8)DAQ_FLAG_TIMESTAMP) != 0 )
      #endif
									{
									/* increase pointer to after time stamp */
									dst = dst + sizeof(XcpDaqTimestampType); /* PRQA S 0488 */ /* MD_Xcp_0488 */
								}
							}
    #endif 

							while (e<=el)
							{
								n = OdtEntrySize(e);
								if ( n == (uint8)0u )
								{
									break;
								}
        #if defined ( XCP_ENABLE_TYPESAVE_COPY )
								(void)XcpAppl_CalibrationWrite(OdtEntryAddr(e), dst+1, n); /* PRQA S 0489 */ /* MD_MSR_17.4 */
        #else
								Xcp_MemCpy(OdtEntryAddr(e), dst+1, n); /* PRQA S 0489 */ /* MD_MSR_17.4 */
        #endif
								dst += n; /* PRQA S 0488 */ /* MD_Xcp_0488 */
								e++;
							}

							ApplXcpInterruptEnableStim();
						}

					} /* odt */

				}
				else {

  #endif /* XCP_ENABLE_STIM */

      #if defined ( XCP_ENABLE_DAQ_RUNTIME_SUPERVISION )
					XcpAppl_RtsStart();
      #endif
					for (odt=DaqListFirstOdt(daq);odt<=DaqListLastOdt(daq);odt++)
					{
        #if defined ( XCP_ENABLE_SEND_QUEUE )
        uint16 qs;
        #endif

						status |= (uint8)XCP_EVENT_DAQ;

        #if defined ( XCP_ENABLE_DAQ_RUNTIME_SUPERVISION )
						if(XCP_OK != XcpAppl_RtsSnapshot())
						{
							/* Runtime supervision timeout occured, create overrun condition and exit */
							status |= (uint8)(XCP_EVENT_DAQ_OVERRUN | XCP_EVENT_DAQ_TIMEOUT);
							/* Queue overflow */
							DaqListFlags(daq) |= (uint8)DAQ_FLAG_OVERRUN;
							break;
						}
        #endif

						ApplXcpInterruptDisableDaq(); /* The following code is not reentrant */

  #if defined ( XCP_ENABLE_SEND_QUEUE )
						/* Check if there is space in the queue for this ODT */
  #if defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY ) || defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY_ASR )
						/* avoid overwriting the last element */
						if (xcp.QueueLen>=(xcp.QueueSize-1))
  #else
							if (xcp.QueueLen>=xcp.QueueSize)
  #endif
								{
								status |= (uint8)XCP_EVENT_DAQ_OVERRUN; /* Queue overflow */
								DaqListFlags(daq) |= (uint8)DAQ_FLAG_OVERRUN;

								if( (uint8)1u == xcp.Daq.SendQueueBehaviour )
								{
									/* Ring buffer mode: Throw away oldest element */
									xcp.QueueRp++;
									if (xcp.QueueRp>=xcp.QueueSize)
									{
										xcp.QueueRp = 0;
									}
									xcp.QueueLen--;
								}
								else
								{
									/* Linear mode: Throw away last element */
									goto next_odt; /* PRQA S 2001 */ /* MD_Xcp_2001 */
								}
							}

							qs = (xcp.QueueRp + xcp.QueueLen);
						if(qs >= xcp.QueueSize)
						{
							qs -= xcp.QueueSize;
						}

						dtop = &xcp.pQueue[qs];
    #if defined ( XCP_SEND_QUEUE_SAMPLE_ODT )
						xcp.QueueLen++;
    #endif
  #else
						dtop = &dto;
  #endif /* XCP_ENABLE_SEND_QUEUE */

  #if defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )

						/* ODT,DAQ */
						dtop->buffer.b[0] = odt-DaqListFirstOdt(daq); /* Relative odt number */
						dtop->buffer.b[1] = daq;
						i = 2;

  #else

						/* PID */
						dtop->buffer.b[0] = odt;
						i = 1;

  #endif

						/* Use BIT7 of PID or ODT to indicate overruns */
  #if defined ( XCP_ENABLE_SEND_QUEUE )
    #if defined ( XCP_ENABLE_DAQ_OVERRUN_INDICATION )
						if ( (DaqListFlags(daq) & (uint8)DAQ_FLAG_OVERRUN) != 0 )
						{
							dtop->buffer.b[0] |= (uint8)0x80;
							DaqListFlags(daq) &= (uint8)(~DAQ_FLAG_OVERRUN & 0xFFu);
						}
    #endif
  #endif

						/* Timestamps */
  #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )

    #if !defined ( XCP_ENABLE_DAQ_TIMESTAMP_FIXED )
						if ( (DaqListFlags(daq) & (uint8)DAQ_FLAG_TIMESTAMP) != 0 )
						{
    #endif

							if (odt==DaqListFirstOdt(daq))
							{
    #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
								// t = XcpAppl_GetTimestamp();
    #endif

    #if defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )

								*(P2VAR(XcpDaqTimestampType, AUTOMATIC, XCP_APPL_DATA))&dtop->buffer.b[2] = XcpAppl_GetTimestamp();
								i = 2 + sizeof(XcpDaqTimestampType);

    #else /* XCP_ENABLE_DAQ_HDR_ODT_DAQ */

      #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
        #if defined ( XCP_CPUTYPE_BIGENDIAN )  /* Avoid WORD access */
          #if( kXcpDaqTimestampSize == DAQ_TIMESTAMP_BYTE )
								dtop->buffer.b[i+0u] = (uint8)t;
          #elif( kXcpDaqTimestampSize == DAQ_TIMESTAMP_WORD )
								dtop->buffer.b[i+1u] = (uint8)t;
								dtop->buffer.b[i+0u] = (uint8)(t>>8);
          #elif( kXcpDaqTimestampSize == DAQ_TIMESTAMP_DWORD )
								dtop->buffer.b[i+3u] = (uint8)t;
								dtop->buffer.b[i+2u] = (uint8)(t>>8);
								dtop->buffer.b[i+1u] = (uint8)(t>>16);
								dtop->buffer.b[i+0u] = (uint8)(t>>24);
          #endif
        #else
								dtop->buffer.b[i+0u] = (uint8)t;
            #if( kXcpDaqTimestampSize > DAQ_TIMESTAMP_BYTE )
								dtop->buffer.b[i+1u] = (uint8)(t>>8);
            #endif
            #if( kXcpDaqTimestampSize > DAQ_TIMESTAMP_WORD )
								dtop->buffer.b[i+2u] = (uint8)(t>>16);
								dtop->buffer.b[i+3u] = (uint8)(t>>24);
            #endif
        #endif
      #else
								*(P2VAR(XcpDaqTimestampType, AUTOMATIC, XCP_APPL_DATA))&dtop->buffer.b[i] = XcpAppl_GetTimestamp();
      #endif /* XCP_DISABLE_UNALIGNED_MEM_ACCESS */
								i += sizeof(XcpDaqTimestampType);

    #endif /* XCP_ENABLE_DAQ_HDR_ODT_DAQ */
							}

    #if !defined ( XCP_ENABLE_DAQ_TIMESTAMP_FIXED )
						}
    #endif

  #endif /* XCP_ENABLE_DAQ_TIMESTAMP */

						/* Sample data */
						/* This is the inner loop, optimize here */
						e = DaqListOdtFirstEntry(odt);
						if (OdtEntrySize(e)==0)
						{
							goto next_odt; /* PRQA S 2001 */ /* MD_Xcp_2001 */
						}
						el = DaqListOdtLastEntry(odt);
						dst = (P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA))&dtop->buffer.b[i];

						while (e<=el)
						{
							n = OdtEntrySize(e);
							if (n == 0)
							{
								break;
							}
        #if defined ( XCP_ENABLE_SILENT_CHECK )
							/* SBSW_AsrXcp_008 */
							if((dst+n) < &xcp.Daq.u.b[kXcpDaqMemSize]) /* PRQA S 0488 */ /* MD_Xcp_0488 */
        #endif
								{
        #if defined ( XCP_ENABLE_TYPESAVE_COPY ) \
        || (defined ( XCP_ENABLE_DAQ_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ))
								(void)XcpAppl_MeasurementRead((DAQBYTEPTR)dst, OdtEntryAddr(e), n);
        #else
								Xcp_MemCpy((DAQBYTEPTR)dst, OdtEntryAddr(e), n);
        #endif
								dst = &dst[n]; /* dst += n; Suppress warning for MISRA rule 101 (pointer arithmetic) */
							}
							e++;
						}
						dtop->buffer.l = (uint8)(dst-(&dtop->buffer.b[0]) ); /* PRQA S 0488 */ /* MD_Xcp_0488 */
						XCP_ASSERT(dtop->buffer.l<=xcpMaxDTO);

						/* Queue or transmit the DTO */
  #if defined ( XCP_ENABLE_SEND_QUEUE )
    #if defined ( XCP_SEND_QUEUE_SAMPLE_ODT )
						/* No action yet */
    #else
						if ( (xcp.SendStatus & (uint8)XCP_SEND_PENDING) != 0 )
						{
							xcp.QueueLen++;
						}
						else
						{
							xcp.SendStatus |= (uint8)XCP_DTO_PENDING;
							Xcp_SendDto(dtop);
						}
    #endif
  #else
						Xcp_SendDto(&dto);
  #endif /* XCP_ENABLE_SEND_QUEUE */
next_odt:

						ApplXcpInterruptEnableDaq();

					} /* odt */

  #if defined ( XCP_ENABLE_SEND_QUEUE ) && defined ( XCP_SEND_QUEUE_SAMPLE_ODT )
					(void)Xcp_SendDtoFromQueue();
  #endif

  #if defined ( XCP_ENABLE_STIM )
				}
  #endif

  #if defined ( XCP_ENABLE_DAQ_PRESCALER )
			}
  #endif

		} /* daq */

		Xcp_Rtm_Stop(Xcp_Event);
		return status;
	} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* XCP_ENABLE_DAQ */


	/****************************************************************************/
	/* Background Processing                                                    */
	/* Used for Checksum Calculation                                            */
	/****************************************************************************/

#if defined ( XCP_ENABLE_CHECKSUM )
  #if defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE )
  #else
	/* Table for CCITT checksum calculation */
    #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC16CCITT )

#define XCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define XCP_START_SEC_CONST_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

	CONST(uint16, AUTOMATIC) CRC16CCITTtab[256] = {
      #if defined ( XCP_ENABLE_CRC16CCITT_REFLECTED )
    0x0000,0x1189,0x2312,0x329B,0x4624,0x57AD,0x6536,0x74BF,
    0x8C48,0x9DC1,0xAF5A,0xBED3,0xCA6C,0xDBE5,0xE97E,0xF8F7,
    0x1081,0x0108,0x3393,0x221A,0x56A5,0x472C,0x75B7,0x643E,
    0x9CC9,0x8D40,0xBFDB,0xAE52,0xDAED,0xCB64,0xF9FF,0xE876,
    0x2102,0x308B,0x0210,0x1399,0x6726,0x76AF,0x4434,0x55BD,
    0xAD4A,0xBCC3,0x8E58,0x9FD1,0xEB6E,0xFAE7,0xC87C,0xD9F5,
    0x3183,0x200A,0x1291,0x0318,0x77A7,0x662E,0x54B5,0x453C,
    0xBDCB,0xAC42,0x9ED9,0x8F50,0xFBEF,0xEA66,0xD8FD,0xC974,
    0x4204,0x538D,0x6116,0x709F,0x0420,0x15A9,0x2732,0x36BB,
    0xCE4C,0xDFC5,0xED5E,0xFCD7,0x8868,0x99E1,0xAB7A,0xBAF3,
    0x5285,0x430C,0x7197,0x601E,0x14A1,0x0528,0x37B3,0x263A,
    0xDECD,0xCF44,0xFDDF,0xEC56,0x98E9,0x8960,0xBBFB,0xAA72,
    0x6306,0x728F,0x4014,0x519D,0x2522,0x34AB,0x0630,0x17B9,
    0xEF4E,0xFEC7,0xCC5C,0xDDD5,0xA96A,0xB8E3,0x8A78,0x9BF1,
    0x7387,0x620E,0x5095,0x411C,0x35A3,0x242A,0x16B1,0x0738,
    0xFFCF,0xEE46,0xDCDD,0xCD54,0xB9EB,0xA862,0x9AF9,0x8B70,
    0x8408,0x9581,0xA71A,0xB693,0xC22C,0xD3A5,0xE13E,0xF0B7,
    0x0840,0x19C9,0x2B52,0x3ADB,0x4E64,0x5FED,0x6D76,0x7CFF,
    0x9489,0x8500,0xB79B,0xA612,0xD2AD,0xC324,0xF1BF,0xE036,
    0x18C1,0x0948,0x3BD3,0x2A5A,0x5EE5,0x4F6C,0x7DF7,0x6C7E,
    0xA50A,0xB483,0x8618,0x9791,0xE32E,0xF2A7,0xC03C,0xD1B5,
    0x2942,0x38CB,0x0A50,0x1BD9,0x6F66,0x7EEF,0x4C74,0x5DFD,
    0xB58B,0xA402,0x9699,0x8710,0xF3AF,0xE226,0xD0BD,0xC134,
    0x39C3,0x284A,0x1AD1,0x0B58,0x7FE7,0x6E6E,0x5CF5,0x4D7C,
    0xC60C,0xD785,0xE51E,0xF497,0x8028,0x91A1,0xA33A,0xB2B3,
    0x4A44,0x5BCD,0x6956,0x78DF,0x0C60,0x1DE9,0x2F72,0x3EFB,
    0xD68D,0xC704,0xF59F,0xE416,0x90A9,0x8120,0xB3BB,0xA232,
    0x5AC5,0x4B4C,0x79D7,0x685E,0x1CE1,0x0D68,0x3FF3,0x2E7A,
    0xE70E,0xF687,0xC41C,0xD595,0xA12A,0xB0A3,0x8238,0x93B1,
    0x6B46,0x7ACF,0x4854,0x59DD,0x2D62,0x3CEB,0x0E70,0x1FF9,
    0xF78F,0xE606,0xD49D,0xC514,0xB1AB,0xA022,0x92B9,0x8330,
    0x7BC7,0x6A4E,0x58D5,0x495C,0x3DE3,0x2C6A,0x1EF1,0x0F78
      #else
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7u,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1efu,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6u,
    0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3deu,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485u,
    0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58du,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4u,
    0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bcu,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823u,
    0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92bu,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12u,
    0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1au,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41u,
    0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49u,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70u,
    0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78u,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16fu,
    0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067u,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35eu,
    0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256u,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50du,
    0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405u,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73cu,
    0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634u,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9abu,
    0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3u,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9au,
    0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92u,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9u,
    0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1u,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8u,
    0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0u
      #endif /* defined ( XCP_ENABLE_CRC16CCITT_REFLECTED ) */
  };

#define XCP_STOP_SEC_CONST_16BIT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define XCP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

    #endif /* ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC16CCITT ) */

  #endif /*  defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE ) */

  #if !defined ( kXcpChecksumBlockSize )
    #define kXcpChecksumBlockSize 256
  #endif

#endif /* defined ( XCP_ENABLE_CHECKSUM ) */


	/*****************************************************************************
	| NAME:             Xcp_MainFunction
	| CALLED BY:        application
	| PRECONDITIONS:    none
	| INPUT PARAMETERS: none 
	| RETURN VALUES:    none
	| DESCRIPTION:      perform background calculation of checksum
	******************************************************************************/
	FUNC(void, XCP_CODE) Xcp_MainFunction( void )
	{
#if defined ( XCP_ENABLE_VX_SUPPORT )
  #ifdef VX1000_DETECT_CONTROL
		VX1000_DETECT_CONTROL()
  #endif
  #ifdef VX1000_STIM
		VX1000_STIM_CONTROL()
  #endif
  #ifdef VX1000_MAILBOX
		VX1000_MAILBOX_CONTROL()
  #endif
#endif

		/* Activation control */
		Xcp_CheckControlState()
			Xcp_Rtm_Start(Xcp_MainFunction);

		/* STORE_DAQ_REQ or CLEAR_DAQ_REQ pending */
#if defined ( XCP_ENABLE_DAQ )
  #if defined ( XCP_ENABLE_DAQ_RESUME )
		if ( (xcp.SessionStatus & (SessionStatusType)SS_STORE_DAQ_REQ) != 0 )
		{
			uint16 size;
			/* Check memory overflow */
			size = (uint16)( ( xcp.Daq.DaqCount      *   (uint8)sizeof(tXcpDaqList)                       ) 
				+ ( xcp.Daq.OdtCount      *   (uint16)sizeof(tXcpOdt)                          ) 
				+ ( xcp.Daq.OdtEntryCount * ( (uint8)sizeof(DAQBYTEPTR) + (uint8)sizeof(uint8) ) )
				);
    #if defined ( XCP_DISABLE_UNALIGNED_MEM_ACCESS )
      #if ( CPU_TYPE == CPU_TYPE_8 ) || ( CPU_TYPE == CPU_TYPE_16 )
			size += 3; /* Worst case 3 bytes needed for alignment */
      #else
			size += 9; /* Worst case 9 bytes needed for alignment */
      #endif
    #endif
    #if defined ( XCP_ENABLE_SILENT_CHECK )
			if(size >= kXcpDaqMemSize) /* SBSW_AsrXcp_009 */
			{
				size = kXcpDaqMemSize;
			}
    #endif
			/* Add size of remaining variables */
			size += (uint16)((&xcp.Daq.u.b[0] - &xcp.Daq.activeTl) + 1); /* PRQA S 0488 */ /* MD_Xcp_0488 */

			if( 0 != (xcp.SessionStatus & (SessionStatusType)SS_STORE_DAQ_REQ_NO_RESUME_INTERNAL) )
			{
				XcpAppl_DaqResumeStore( &xcp.Daq, size, 0 );
			}
			else
			{
				XcpAppl_DaqResumeStore( &xcp.Daq, size, 1 );
			}
  #if defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY_ASR )
			FrXcp_DaqResumeStore();
  #endif
			xcp.SessionStatus &= (SessionStatusType)(~SS_STORE_DAQ_REQ); /* PRQA S 0277 */ /* MD_Xcp_0277 */
			/* #### Send an event message */
		}
		if ( (xcp.SessionStatus & (SessionStatusType)SS_CLEAR_DAQ_REQ) != 0 )
		{
			XcpAppl_DaqResumeClear();
  #if defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY_ASR )
			FrXcp_DaqResumeClear();
  #endif
			xcp.SessionStatus &= (SessionStatusType)(~SS_CLEAR_DAQ_REQ); /* PRQA S 0277 */ /* MD_Xcp_0277 */
			xcp.Daq.DaqConfigId = 0x00;
			/* #### Send an event message */
		}
		if ( (xcp.SessionStatus & (SessionStatusType)SS_STORE_CAL_REQ) != 0 )
		{
			VAR(uint8, AUTOMATIC) ret;
			/* report application to store calibration data */
			ret = XcpAppl_CalResumeStore();
			if ( ret != 0 )
			{
				/* Application finished with storing, clear bit */
				xcp.SessionStatus &= (SessionStatusType)(~SS_STORE_CAL_REQ); /* PRQA S 0277 */ /* MD_Xcp_0277 */
				/* #### Send an event message */
			}
		}
  #endif
#endif /* XCP_ENABLE_DAQ */

		/* XCP checksum calculation */
#if defined ( XCP_ENABLE_CHECKSUM ) && !defined ( XCP_ENABLE_CUSTOM_CRC )
		/*
		Checksum algorithm is not defined by the standard
		Type is defined by tXcpChecksumSumType, the maximum blocksize is 64K
		*/

		/* Checksum calculation in progress */
		if ( (xcp.CheckSumSize) != (tXcpChecksumSumType)0u )
		{
			VAR(uint16, AUTOMATIC) n;

			if (xcp.CheckSumSize<=(kXcpChecksumBlockSize-1))
			{
				n = xcp.CheckSumSize;
				xcp.CheckSumSize = 0;
			}
			else
			{
				n = (uint16)kXcpChecksumBlockSize;
				xcp.CheckSumSize -= kXcpChecksumBlockSize;
			}

  #if defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE )

			/* Calculate CRC with AUTOSAR CRC module. */
			xcp.CheckSum = XcpCalculateCrc((P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA))xcp.Mta, n, xcp.CheckSum);
			xcp.Mta += n; /* PRQA S 0488 */ /* MD_Xcp_0488 */

  #else

			do
			{
    #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC16CCITT )

      #if defined ( XCP_ENABLE_CRC16CCITT_REFLECTED )
				/* CRC16 CCITT Reflected: Refin = true and refout = true. */
				xcp.CheckSum = CRC16CCITTtab[((uint8)(xcp.CheckSum&0xFF)) ^ XCP_READ_CHECKSUMVALUE_FROM_ADDR( xcp.Mta )] ^ ((uint8)((xcp.CheckSum>>8)&0xFF));
      #else
				/* CRC16 CCITT */
				xcp.CheckSum = CRC16CCITTtab[((uint8)((xcp.CheckSum >> 8)&0xFF)) ^ XCP_READ_CHECKSUMVALUE_FROM_ADDR( xcp.Mta )] ^ (xcp.CheckSum << 8);
      #endif

				xcp.Mta++;
				n--;

    #else

				/* Checksum calculation method: XCP_ADD_xx */
				xcp.CheckSum += (tXcpChecksumSumType)(XCP_READ_CHECKSUMVALUE_FROM_ADDR( xcp.Mta ));
				xcp.Mta      += (uint8)sizeof(tXcpChecksumAddType); /* PRQA S 0488 */ /* MD_Xcp_0488 */
				n            -= (uint8)sizeof(tXcpChecksumAddType);

    #endif
			}
			while (n!=0);

  #endif  /* defined ( XCP_ENABLE_AUTOSAR_CRC_MODULE ) */

			/* Checksum calculation finished ? */
			if ( (xcp.CheckSumSize) != 0 )
			{
				Xcp_Rtm_Stop(Xcp_MainFunction);
				return;
			}

			CRM_BUILD_CHECKSUM_TYPE = kXcpChecksumMethod;
			CRM_BUILD_CHECKSUM_RESULT = xcp.CheckSum;
			xcp.CrmLen = CRM_BUILD_CHECKSUM_LEN;


			Xcp_SendCrm();
		} /* xcp.CheckSumSize */
#endif /* XCP_ENABLE_CHECKSUM && !XCP_ENABLE_CUSTOM_CRC */

		Xcp_Rtm_Stop(Xcp_MainFunction);
	} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6030 */ /* MD_MSR_STCYC */


	/****************************************************************************/
	/* Command Processor                                                        */
	/****************************************************************************/


	/*****************************************************************************
	| NAME:             Xcp_Disconnect
	| CALLED BY:        Xcp_Command
	| PRECONDITIONS:    XCP is initialized and in connected state.
	| INPUT PARAMETERS: none 
	| RETURN VALUES:    none
	| DESCRIPTION:      If the XCP slave is connected to a XCP master a call of this
	|                   function discontinues the connection (transition to disconnected state).
	|                   If the XCP slave is not connected this function performs no action.
	******************************************************************************/
	FUNC(void, XCP_CODE) Xcp_Disconnect( void )
	{
		Xcp_CheckDetErrorReturnVoid( XCP_UNINIT != Xcp_InitializationState,
			XCP_DISCONNECT_SERVICE_ID, XCP_E_NOT_INITIALIZED )

			/* Activation control */
			Xcp_CheckControlState()

			/* PRQA S 0277 1 */ /* MD_Xcp_0277 */
			xcp.SessionStatus &= (SessionStatusType)(~(SessionStatusType)SS_CONNECTED); /* PRQA S 0277 */ /* MD_Xcp_0277 */

#if defined ( XCP_ENABLE_DAQ )
		Xcp_StopAllDaq();
#endif
#if defined ( XCP_ENABLE_SEED_KEY )
		/* Lock all resources */
		xcp.ProtectionStatus = (uint8)RM_CAL_PAG|RM_DAQ|RM_PGM|RM_STIM;
#endif

	} /* PRQA S 2006 */ /* MD_MSR_14.7 */


	/*****************************************************************************
	| NAME:             Xcp_Command
	| CALLED BY:        Xcp_SendCallBack, XCP Transport Layer
	| PRECONDITIONS:    none
	| INPUT PARAMETERS: pCmd : data of received CTO message.
	| RETURN VALUES:    none
	| DESCRIPTION:      
	******************************************************************************/
	FUNC(void, XCP_CODE) Xcp_Command( P2CONST(uint32, AUTOMATIC, XCP_APPL_DATA) pCommand )
	{
		P2CONST(tXcpCto, AUTOMATIC, XCP_APPL_DATA) pCmd = (P2CONST(tXcpCto, AUTOMATIC, XCP_APPL_DATA)) pCommand; /* PRQA S 0310 */ /* MD_Xcp_0310 */
		uint8 err;
		uint8 frames_with_7_bytes;
		//uint8 frames_less_7_bytes;
		//uint8 total_frames;
		//uint8 frames;


  #if defined ( XCP_ENABLE_STIM )
    #if !defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )
  uint8 pid;
    #endif
  #endif

		Xcp_CheckDetErrorReturnVoid( XCP_UNINIT != Xcp_InitializationState,
			XCP_COMMAND_SERVICE_ID, XCP_E_NOT_INITIALIZED )
			/* SBSW_AsrXcp_028 */
			Xcp_SilentBswCheckErrorReturnVoid( (P2CONST(uint32, AUTOMATIC, XCP_APPL_DATA))NULL_PTR != pCommand,
			XCP_COMMAND_SERVICE_ID, XCP_E_NULL_POINTER )

			Xcp_Rtm_Start(Xcp_Command);
		/* Activation control */
		Xcp_CheckControlState()

			/* XCP Data Stimulation Handler */

  #if defined ( XCP_ENABLE_STIM )

    #if defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )
      #error "Data stimulation not implemented for XCP_ENABLE_DAQ_HDR_ODT_DAQ"
    #else
		pid = (uint8)CRO_BYTE(0);

		if (pid < (uint8)0xC0u)
		{
			P2VAR(tXcpDto, AUTOMATIC, XCP_APPL_DATA) pStimBuffer;
			/* Check whether the resource STIM is locked. */
			CheckResourceProtection( RM_STIM ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

			if (pid<xcp.Daq.OdtCount)
			{

				pStimBuffer = DaqListOdtStimBuffer(pid);
				if (pStimBuffer != NULL_PTR)
				{ /* Copy to STIM buffer, this validated the buffer */
					/* PRQA S 0310 2 */ /* MD_Xcp_0310_StimBuffer */
					/* PRQA S 0311 2 */ /* MD_Xcp_0311_StimBuffer */
					*pStimBuffer = *(P2VAR(tXcpDto, AUTOMATIC, XCP_APPL_DATA))pCmd;
					/* No response, if there is a valid STIM buffer */
					Xcp_Rtm_Stop(Xcp_Command);
					return;
				}
			}

			/* Error */
			/* pid out of range or DAQ list not configured for STIM */
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}

    #endif

  #endif /* XCP_ENABLE_STIM */


		/* XCP Command Handler */

		/* CONNECT */
		if (CRO_CMD==CC_CONNECT)
		{
    #if defined ( XCP_ENABLE_VX_SUPPORT ) 
			if(0 != Xcp_VxActive)
			{
				error(CRC_RESOURCE_TEMP_NOT_ACCESSIBLE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
			}
    #endif

			/* Prepare the default response */
			/* ESCAN00023570 */
			CRM_CMD = 0xFF; /* No Error */
			xcp.CrmLen = 1; /* Length = 1 */


			/* DPRAM */
			/* DPRAM Client */

#if defined ( XCP_ENABLE_SEED_KEY )
			/* Lock all resources. */
			xcp.ProtectionStatus = (uint8)RM_CAL_PAG|RM_DAQ|RM_PGM|RM_STIM;
#endif

			/* Reset Session Status */
			xcp.SessionStatus = (SessionStatusType)SS_CONNECTED;

			xcp.CrmLen = CRM_CONNECT_LEN;

			/* Versions of the XCP Protocol Layer and Transport Layer Specifications. */
    #if defined ( XCP_TRANSPORT_LAYER_TYPE_CAN_ASR )
			CRM_CONNECT_TRANSPORT_VERSION = (uint8)( (uint16)CANXCP_TRANSPORT_LAYER_VERSION >> 8 );
    #endif
    #if defined ( XCP_TRANSPORT_LAYER_TYPE_FLEXRAY_ASR )
			if( XCP_TRANSPORT_LAYER_FR == xcp.Daq.activeTl )
			{
				CRM_CONNECT_TRANSPORT_VERSION = (uint8)( (uint16)FRXCP_TRANSPORT_LAYER_VERSION >> 8 );
			}
    #endif
    #if defined ( XCP_TRANSPORT_LAYER_TYPE_TCPIP_ASR )
			if( XCP_TRANSPORT_LAYER_TCPIP == xcp.Daq.activeTl )
			{
				CRM_CONNECT_TRANSPORT_VERSION = (uint8)( (uint16)TCPIPXCP_TRANSPORT_LAYER_VERSION >> 8 );
			}
    #endif
			CRM_CONNECT_PROTOCOL_VERSION =  (uint8)( (uint16)XCP_VERSION >> 8 );

			CRM_CONNECT_MAX_CTO_SIZE = xcp.Daq.xcpMaxCTO;
			CRM_CONNECT_MAX_DTO_SIZE = xcp.Daq.xcpMaxDTO;

#if defined ( XCP_ENABLE_CALIBRATION_PAGE )
			CRM_CONNECT_RESOURCE = RM_CAL_PAG;           /* Calibration */
#else
			CRM_CONNECT_RESOURCE = 0x00;                 /* Reset resource mask */
#endif
#if defined ( XCP_ENABLE_DAQ )
			CRM_CONNECT_RESOURCE |= (uint8)RM_DAQ;       /* Data Acquisition */
#endif
#if defined ( XCP_ENABLE_STIM )
			CRM_CONNECT_RESOURCE |= (uint8)RM_STIM;      /* Stimulation */
#endif
#if defined ( XCP_ENABLE_PROGRAM )
			CRM_CONNECT_RESOURCE |= (uint8)RM_PGM;       /* Flash Programming */
#endif

			CRM_CONNECT_COMM_BASIC = 0;
#if defined ( XCP_ENABLE_COMM_MODE_INFO )
			CRM_CONNECT_COMM_BASIC |= (uint8)CMB_OPTIONAL;
#endif
#if defined ( XCP_ENABLE_BLOCK_UPLOAD )
			CRM_CONNECT_COMM_BASIC |= (uint8)CMB_SLAVE_BLOCK_MODE;
#endif
#if defined ( XCP_CPUTYPE_BIGENDIAN )
			CRM_CONNECT_COMM_BASIC |= (uint8)PI_MOTOROLA;
#endif

			XCP_PRINT(("<- 0xFF version=%02Xh/%02Xh, maxcro=%02Xh, maxdto=%02Xh, resource=%02X, mode=%02X\n",
				CRM_CONNECT_PROTOCOL_VERSION,
				CRM_CONNECT_TRANSPORT_VERSION,
				CRM_CONNECT_MAX_CTO_SIZE,
				CRM_CONNECT_MAX_DTO_SIZE,
				CRM_CONNECT_RESOURCE,
				CRM_CONNECT_COMM_BASIC));

			goto positive_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */

		}

		/* Handle other commands only if connected */
		else /* CC_CONNECT */
		{
			if ( (xcp.SessionStatus & (SessionStatusType)SS_CONNECTED) != 0 )
			{
				/* Ignore commands if the previous command sequence has not been completed */
#if defined ( XCP_ENABLE_SEND_QUEUE ) /* ESCAN00081696 */
				if ( ((xcp.SendStatus & (uint8)(XCP_CRM_PENDING)) != 0 )
					&& ((xcp.SendStatus & (uint8)(XCP_CRM_REQUEST)) != 0 ) )
				{
					xcp.SessionStatus |= (SessionStatusType)SS_ERROR;
					Xcp_Rtm_Stop(Xcp_Command);

					/* No response */
					return;
				}
#endif

      #if defined ( XCP_ENABLE_GET_SESSION_STATUS_API )
				xcp.SessionStatus |= (SessionStatusType)SS_POLLING;
      #endif
				/* Prepare the default response */
				/* ESCAN00023570 */
				CRM_CMD = 0xFF; /* No Error */
				xcp.CrmLen = 1; /* Length = 1 */

				switch (CRO_CMD)
				{

				case CC_SYNC:
					{
						/* Always return a negative response with the error code ERR_CMD_SYNCH. */
						xcp.CrmLen = CRM_SYNCH_LEN;
						CRM_CMD    = PID_ERR;
						CRM_ERR    = CRC_CMD_SYNCH;

					}
					break;


#if defined ( XCP_ENABLE_COMM_MODE_INFO )
				case CC_GET_COMM_MODE_INFO:
					{
						xcp.CrmLen = CRM_GET_COMM_MODE_INFO_LEN;
						/* Transmit the version of the XCP Protocol Layer implementation.    */
						/* The higher nibble is the main version, the lower the sub version. */
						/* The lower nibble overflows, if the sub version is greater than 15.*/
						CRM_GET_COMM_MODE_INFO_DRIVER_VERSION = (uint8)( ((CP_XCP_VERSION & 0x0F00) >> 4) |
							(CP_XCP_VERSION & 0x000F)         );
						CRM_GET_COMM_MODE_INFO_COMM_OPTIONAL = 0;
						CRM_GET_COMM_MODE_INFO_QUEUE_SIZE = 0;
  #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
						CRM_GET_COMM_MODE_INFO_COMM_OPTIONAL |= (uint8)CMO_MASTER_BLOCK_MODE;
						CRM_GET_COMM_MODE_INFO_MAX_BS = (uint8)((254 / CRO_DOWNLOAD_MAX_SIZE) + 1);
    #if defined (kXcpBlockDownloadMinSt)
						CRM_GET_COMM_MODE_INFO_MIN_ST = (uint8)kXcpBlockDownloadMinSt;
    #else
						CRM_GET_COMM_MODE_INFO_MIN_ST = 0;
    #endif
  #else
						CRM_GET_COMM_MODE_INFO_MAX_BS = 0;
						CRM_GET_COMM_MODE_INFO_MIN_ST = 0;
  #endif


					}
					break;
#endif /* XCP_ENABLE_COMM_MODE_INFO */


				case CC_DISCONNECT:
					{
						xcp.CrmLen = CRM_DISCONNECT_LEN;
						Xcp_Disconnect();

					}
					break;


#if defined ( kXcpStationIdLength ) || defined ( XCP_ENABLE_GET_ID_GENERIC ) || defined ( XCP_ENABLE_VECTOR_INFOMEM ) || defined ( XCP_ENABLE_VECTOR_GENERICMEASUREMENT )
				case CC_GET_ID:
					{
						xcp.CrmLen = CRM_GET_ID_LEN;
						CRM_GET_ID_MODE = 0;
						CRM_GET_ID_LENGTH = 0;

  #if defined ( kXcpStationIdLength )
						if ( CRO_GET_ID_TYPE == IDT_ASAM_NAME )   /* Type = ASAM MC2 */
						{
							CRM_GET_ID_LENGTH = (uint32)kXcpStationIdLength;
							XcpSetMta( XcpAppl_GetPointer(0x00, (uint32)(&kXcpStationId[0])), 0x00); /* PRQA S 0306 */ /* MD_Xcp_0306 */
						}
						else
  #endif
  #if defined ( XCP_ENABLE_VECTOR_INFOMEM )
						if ( CRO_GET_ID_TYPE == IDT_VECTOR_INFOMEM )   /* Type = Vector Type */
						{
							CRM_GET_ID_LENGTH = sizeof(kInfoMem);
							XcpSetMta(XcpAppl_GetPointer(0x00, (uint32)&kInfoMem), 0x00); /* PRQA S 0306 */ /* MD_Xcp_0306 */
						}
						else
  #endif
  #if defined ( XCP_ENABLE_VECTOR_GENERICMEASUREMENT )
						if ( CRO_GET_ID_TYPE == IDT_VECTOR_GENMEAS )   /* Type = Vector Type */
						{
							CRM_GET_ID_LENGTH = sizeof(GenMeasInfoTable);
							XcpSetMta(XcpAppl_GetPointer(0x00, (uint32)&GenMeasInfoTable), 0x00); /* PRQA S 0306 */ /* MD_Xcp_0306 */
						}
						else
  #endif
							{
  #if defined ( XCP_ENABLE_GET_ID_GENERIC )
							MTABYTEPTR pData;
							CRM_GET_ID_LENGTH = XcpAppl_GetIdData(&pData, CRO_GET_ID_TYPE);
							if( CRM_GET_ID_LENGTH > 0u )
							{
								XcpSetMta(XcpAppl_GetPointer(0x00, (uint32)pData), 0x00); /* PRQA S 0306 */ /* MD_Xcp_0306 */
							}
  #endif
						}

					}
					break;
#endif


				case CC_GET_STATUS:
					{
						xcp.CrmLen = CRM_GET_STATUS_LEN;
						CRM_GET_STATUS_STATUS = (uint8)xcp.SessionStatus;
#if defined ( XCP_ENABLE_SEED_KEY )
						/* Return current resource protection status. If a bit is one, the associated resource is locked. */
						CRM_GET_STATUS_PROTECTION = xcp.ProtectionStatus;
#else
						CRM_GET_STATUS_PROTECTION = 0;
#endif

#if defined ( XCP_ENABLE_DAQ ) && defined ( XCP_ENABLE_DAQ_RESUME )
						/* Return the session configuration ID. */
						CRM_GET_STATUS_CONFIG_ID = xcp.Daq.DaqConfigId;
#else
						/* Session configuration ID not available. */
						CRM_GET_STATUS_CONFIG_ID = 0x00;
#endif

					}
					break;


#if defined ( XCP_ENABLE_SEED_KEY )

				case CC_GET_SEED:
					{
						/* Only seeds with a maximum length of MAX_CTO-2 are supported so far. */

						/* Check whether the first part or a remaining part of the seed is requested. */
						if (CRO_GET_SEED_MODE == 0x01)
						{
							/* Remaining parts of seeds are not supported so far. */
							error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
						}
						else
						{
  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
							/* Only one resource may be requested at one time. */
							switch (CRO_GET_SEED_RESOURCE)
							{
							case RM_CAL_PAG:
								break;
							case RM_PGM:
								break;
							case RM_DAQ:
								break;
							case RM_STIM:
								break;
							default:
								error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
							}

  #endif                
							if ((xcp.ProtectionStatus & CRO_GET_SEED_RESOURCE) != 0)   /* locked */
							{
								uint8 seedLength;
								seedLength = XcpAppl_GetSeed(CRO_GET_SEED_RESOURCE, CRM_GET_SEED_DATA);
								if (seedLength > (uint8)(xcp.Daq.xcpMaxCTO-2))
								{
									/* A maximum seed length of MAX_CTO-2 is supported. */
									error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
								}
								CRM_GET_SEED_LENGTH = seedLength;
							}
							else                       /* Unlocked */
							{
								/* return 0 if the resource is unprotected. */
								CRM_GET_SEED_LENGTH = 0;
							}
							xcp.CrmLen = (uint8)CRM_GET_SEED_LEN;
						}

					}
					break;

				case CC_UNLOCK:
					{
						uint8 resource;


						/* Only keys with a maximum length of MAX_CTO-2 are supported so far. */
						if (CRO_UNLOCK_LENGTH > (uint8)(xcp.Daq.xcpMaxCTO-2))
						{
							error(CRC_SEQUENCE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
						}
						else
						{
							resource = XcpAppl_Unlock(CRO_UNLOCK_KEY, CRO_UNLOCK_LENGTH);
							if ( resource == (uint8)0x00u )
							{
								/* Key wrong ! */
								/* Send ERR_ACCESS_LOCKED and go to disconnected state. */
								Xcp_Disconnect();
								error(CRC_ACCESS_LOCKED) /* PRQA S 2001 */ /* MD_Xcp_2001 */
							}
							else
							{
								/* unlock (reset) the appropriate resource protection mask bit.. */
								xcp.ProtectionStatus &= (uint8)~resource;

								/* ..and return the current resource protection status. */
								CRM_UNLOCK_PROTECTION = xcp.ProtectionStatus;
								xcp.CrmLen = CRM_UNLOCK_LEN;
							}
						}

					}
					break;

#endif /* XCP_ENABLE_SEED_KEY */


#if defined ( XCP_ENABLE_CALIBRATION_PAGE )

				case CC_SET_CAL_PAGE:
					{
						err = XcpAppl_SetCalPage(CRO_SET_CAL_PAGE_SEGMENT, CRO_SET_CAL_PAGE_PAGE, CRO_SET_CAL_PAGE_MODE);

						if (err==(uint8)XCP_CMD_PENDING)
						{
							goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
						}

						check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

					}
					break;

				case CC_GET_CAL_PAGE:
					{


						xcp.CrmLen = CRM_GET_CAL_PAGE_LEN;
						CRM_GET_CAL_PAGE_PAGE = XcpAppl_GetCalPage(CRO_GET_CAL_PAGE_SEGMENT, CRO_GET_CAL_PAGE_MODE);

					}
					break;

#endif /* XCP_ENABLE_CALIBRATION_PAGE */


#if defined ( XCP_ENABLE_PAGE_INFO )
				/* Paging Information optional */
				case CC_GET_PAG_PROCESSOR_INFO:
					{

						xcp.CrmLen = CRM_GET_PAG_PROCESSOR_INFO_LEN;

						CRM_GET_PAG_PROCESSOR_INFO_MAX_SEGMENT = (uint8)kXcpMaxSegment;
  #if defined ( XCP_ENABLE_PAGE_FREEZE )
						CRM_GET_PAG_PROCESSOR_INFO_PROPERTIES  = 1; /* FREEZE_SUPPORTED = 1 */
  #else
						CRM_GET_PAG_PROCESSOR_INFO_PROPERTIES  = 0; /* FREEZE_SUPPORTED = 0 */
  #endif


					}
					break;


#endif /* defined ( XCP_BASIC_VERSION_COMMENT ) */

					/* Paging freeze mode support */
#if defined ( XCP_ENABLE_PAGE_FREEZE )
				case CC_SET_SEGMENT_MODE:
					{

						if (CRO_SET_SEGMENT_MODE_SEGMENT > ((uint8)(kXcpMaxSegment-1)))
						{
							error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
						}

						/* inform application about Set Segment Mode command */
						XcpAppl_SetFreezeMode(CRO_SET_SEGMENT_MODE_SEGMENT, CRO_SET_SEGMENT_MODE_MODE);
						/* CRO_SET_SEGMENT_MODE_MODE;*/
						xcp.CrmLen = CRM_SET_SEGMENT_MODE_LEN;

					}
					break;

				case CC_GET_SEGMENT_MODE:
					{

						if (CRO_GET_SEGMENT_MODE_SEGMENT > ((uint8)(kXcpMaxSegment-1)))
						{
							error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
						}

						/* request current freeze mode information from application */
						CRM_GET_SEGMENT_MODE_MODE = XcpAppl_GetFreezeMode(CRO_GET_SEGMENT_MODE_SEGMENT);

						xcp.CrmLen = CRM_GET_SEGMENT_MODE_LEN;

					}
					break;
#endif /* XCP_ENABLE_PAGE_FREEZE */

					/* Copy cal page support */
#if defined ( XCP_ENABLE_PAGE_COPY )
				case CC_COPY_CAL_PAGE:
					{

						xcp.CrmLen = CRM_COPY_CAL_PAGE_LEN;

						err = XcpAppl_CopyCalPage(CRO_COPY_CAL_PAGE_SRC_SEGMENT, CRO_COPY_CAL_PAGE_SRC_PAGE, CRO_COPY_CAL_PAGE_DEST_SEGMENT, CRO_COPY_CAL_PAGE_DEST_PAGE);

						if (err==(uint8)XCP_CMD_PENDING)
						{
							goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
						}

						check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

					}
					break;
#endif /* XCP_ENABLE_PAGE_COPY */


				case CC_SET_MTA:
					{
						XcpSetMta(XcpAppl_GetPointer(CRO_SET_MTA_EXT, CRO_SET_MTA_ADDR), CRO_SET_MTA_EXT);

					}
					break;

#if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
				case CC_DOWNLOAD_NEXT:

#if defined ( XCP_ENABLE_CALIBRATION )  

					if (CRO_DOWNLOAD_SIZE!=xcp.NextDataBlockSize)
					{
						/* xcp.NextDataBlockSize = 0;*/
						error1(CRC_SEQUENCE, xcp.NextDataBlockSize) /* PRQA S 2001 */ /* MD_Xcp_2001 */
					}
#else
					error(CRC_CMD_UNKNOWN) /* PRQA S 2001 */ /* MD_Xcp_2001 */
#endif
					/* |||| */

#endif /* XCP_ENABLE_BLOCK_DOWNLOAD */

				case CC_DOWNLOAD: /* PRQA S 2003 */ /* MD_Xcp_2003 */
					{
#if defined ( XCP_ENABLE_CALIBRATION )
						uint8 size;


						CheckResourceProtection( RM_CAL_PAG ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

							size = CRO_DOWNLOAD_SIZE;
#if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
						if (size>CRO_DOWNLOAD_MAX_SIZE)
						{
							size = CRO_DOWNLOAD_MAX_SIZE;
						}
						xcp.NextDataBlockSize = (uint8)(CRO_DOWNLOAD_SIZE-size);
#else
						if (size>CRO_DOWNLOAD_MAX_SIZE)
						{
							error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
						}
#endif

						err = Xcp_WriteMta(size, CRO_DOWNLOAD_DATA);
              #if defined ( XCP_ENABLE_WRITE_EEPROM ) || defined ( XCP_ENABLE_WRITE_PROTECTION ) \
                  || ( defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ) ) \
                  || defined ( XCP_ENABLE_TYPESAVE_COPY )
						if (err==(uint8)XCP_CMD_PENDING)
						{
							goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
						}

						check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
              #endif

#if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
						if ( xcp.NextDataBlockSize != 0)
						{
							goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
						}
#endif

					}
					break;
#else
					error(CRC_CMD_UNKNOWN) /* PRQA S 2001 */ /* MD_Xcp_2001 */
				}
#endif /* !defined ( XCP_ENABLE_CALIBRATION ) */

#if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
#else
case CC_DOWNLOAD_MAX:
				{
#if defined ( XCP_ENABLE_CALIBRATION )

					CheckResourceProtection( RM_CAL_PAG ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

						err = Xcp_WriteMta(CRO_DOWNLOAD_MAX_MAX_SIZE, CRO_DOWNLOAD_MAX_DATA);
              #if defined ( XCP_ENABLE_WRITE_EEPROM ) || defined ( XCP_ENABLE_WRITE_PROTECTION ) \
                  || ( defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ) ) \
                  || defined ( XCP_ENABLE_TYPESAVE_COPY )
					if (err==(uint8)XCP_CMD_PENDING)
					{
						return;
					}

					check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
              #endif

				}
				break;
#else
				error(CRC_ACCESS_DENIED) /* PRQA S 2001 */ /* MD_Xcp_2001 */
			}
#endif /* !defined ( XCP_ENABLE_CALIBRATION ) */
#endif /* !defined ( XCP_ENABLE_BLOCK_DOWNLOAD ) */

#if defined ( XCP_ENABLE_SHORT_DOWNLOAD ) 
case CC_SHORT_DOWNLOAD:
			{
#if defined ( XCP_ENABLE_CALIBRATION )
  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
				if (CRO_SHORT_DOWNLOAD_SIZE > (uint8)CRM_SHORT_DOWNLOAD_MAX_SIZE)
				{
					error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
				}
  #endif
				CheckResourceProtection( RM_CAL_PAG ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

					XcpSetMta(XcpAppl_GetPointer(CRO_SHORT_DOWNLOAD_EXT, CRO_SHORT_DOWNLOAD_ADDR), CRO_SHORT_DOWNLOAD_EXT);
				err = Xcp_WriteMta(CRO_SHORT_DOWNLOAD_SIZE, CRO_SHORT_DOWNLOAD_DATA);
              #if defined ( XCP_ENABLE_WRITE_EEPROM ) || defined ( XCP_ENABLE_WRITE_PROTECTION ) \
                  || ( defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ) ) \
                  || defined ( XCP_ENABLE_TYPESAVE_COPY )
				if (err==(uint8)XCP_CMD_PENDING)
				{
					goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
				}

				check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
              #endif
			}
			break;
#else
			error(CRC_CMD_UNKNOWN) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
#endif /* !defined ( XCP_ENABLE_CALIBRATION ) */
#endif /* defined ( XCP_ENABLE_SHORT_DOWNLOAD ) */

#if defined ( XCP_ENABLE_MODIFY_BITS ) 
case CC_MODIFY_BITS:
		{
#if defined ( XCP_ENABLE_CALIBRATION )
			uint32 data;
			MTABYTEPTR Mta_backup;

			CheckResourceProtection( RM_CAL_PAG ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
				Mta_backup = xcp.Mta;
			err = Xcp_ReadMta(sizeof(data), (BYTEPTR)&data); /* PRQA S 0310 */ /* MD_Xcp_0310 */
			xcp.Mta = Mta_backup;
              #if defined ( XCP_ENABLE_READ_EEPROM ) || defined ( XCP_ENABLE_READ_PROTECTION ) \
                  || ( defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ) ) \
                  || defined ( XCP_ENABLE_TYPESAVE_COPY )
			if (err==(uint8)XCP_CMD_PENDING)
			{
				goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
			}

			check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
              #endif
			/* PRQA S 3397 2 */ /* MD_Xcp_3397 */
			data = ((data) & ((~((uint32)(((uint16)~CRO_MODIFY_BITS_AND)<<CRO_MODIFY_BITS_SHIFT))))
				^((uint32)(CRO_MODIFY_BITS_XOR<<CRO_MODIFY_BITS_SHIFT)));

			err = Xcp_WriteMta(sizeof(data), (ROMBYTEPTR)&data); /* PRQA S 0310 */ /* MD_Xcp_0310 */
			xcp.Mta = Mta_backup;
              #if defined ( XCP_ENABLE_WRITE_EEPROM ) || defined ( XCP_ENABLE_WRITE_PROTECTION ) \
                  || ( defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ) ) \
                  || defined ( XCP_ENABLE_TYPESAVE_COPY )
			if (err==(uint8)XCP_CMD_PENDING)
			{
				goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
			}

			check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
              #endif
		}
		break;
#else
		error(CRC_CMD_UNKNOWN) /* PRQA S 2001 */ /* MD_Xcp_2001 */
	}
#endif /* !defined ( XCP_ENABLE_CALIBRATION ) */
#endif /* defined ( XCP_ENABLE_MODIFY_BITS ) */

case CC_UPLOAD:
	{
		uint8 size = CRO_UPLOAD_SIZE;


#if defined ( XCP_ENABLE_BLOCK_UPLOAD )
		if ( size > (uint8)CRM_UPLOAD_MAX_SIZE )
		{
			xcp.SessionStatus |= (SessionStatusType)SS_BLOCK_UPLOAD;
			xcp.NextDataBlockSize = (uint8)(size - (uint8)CRM_UPLOAD_MAX_SIZE);
			size = CRM_UPLOAD_MAX_SIZE;
		}
#else
		if ( size > (uint8)CRM_UPLOAD_MAX_SIZE )
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
#endif
		err = Xcp_ReadMta(size, CRM_UPLOAD_DATA);
		xcp.CrmLen = (uint8)(CRM_UPLOAD_LEN + size);
              #if defined ( XCP_ENABLE_READ_EEPROM ) || defined ( XCP_ENABLE_READ_PROTECTION ) \
                  || ( defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ) ) \
                  || defined ( XCP_ENABLE_TYPESAVE_COPY )
		if (err==(uint8)XCP_CMD_PENDING)
		{
			goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}

		check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
              #endif

	}
	break;

case CC_SHORT_UPLOAD:
	{
	  total_frames = 0;
#if defined ( XCP_ENABLE_PARAMETER_CHECK )
		//if (CRO_SHORT_UPLOAD_SIZE > (uint8)CRM_SHORT_UPLOAD_MAX_SIZE)
		//{
		//	error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		//}
#endif
		XcpSetMta(XcpAppl_GetPointer(CRO_SHORT_UPLOAD_EXT, CRO_SHORT_UPLOAD_ADDR), CRO_SHORT_UPLOAD_EXT);

		if (CRO_SHORT_UPLOAD_SIZE > (uint8)CRM_SHORT_UPLOAD_MAX_SIZE)
		{
			/*calculate no.of CAN frames*/
			frames_with_7_bytes = (CRO_SHORT_UPLOAD_SIZE/7);
			//frames_less_7_bytes = (CRO_SHORT_UPLOAD_SIZE%7);

			if((CRO_SHORT_UPLOAD_SIZE%7) == 0)
			{
				total_frames =  frames_with_7_bytes;
			}
			else
			{
				total_frames =  frames_with_7_bytes + 1;
			}

		   	err = Xcp_ReadMta(7, CRM_SHORT_UPLOAD_DATA);
			//Xcp_SendCrm();
			total_frames --;
			
			xcp.CrmLen = (uint8)(CRM_SHORT_UPLOAD_LEN + CRO_SHORT_UPLOAD_SIZE);
		}
		else
		{
			err = Xcp_ReadMta(CRO_SHORT_UPLOAD_SIZE, CRM_SHORT_UPLOAD_DATA);
			xcp.CrmLen = (uint8)(CRM_SHORT_UPLOAD_LEN + CRO_SHORT_UPLOAD_SIZE);
              #if defined ( XCP_ENABLE_READ_EEPROM ) || defined ( XCP_ENABLE_READ_PROTECTION ) \
                  || ( defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL ) ) \
                  || defined ( XCP_ENABLE_TYPESAVE_COPY )
			if (err==(uint8)XCP_CMD_PENDING)
			{ /* ESCAN00014775 */
				goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
			}

			check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
              #endif
		}
	}
	break;


#if defined ( XCP_ENABLE_CHECKSUM )

case CC_BUILD_CHECKSUM: /* Build Checksum */
	{
		uint32 s;

		s = CRO_BUILD_CHECKSUM_SIZE;

  #if !defined ( XCP_ENABLE_CUSTOM_CRC )
		/* Initialization of checksum calculation. */
    #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC16CCITT ) || \
        ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_CRC32 )
		xcp.CheckSum = (tXcpChecksumSumType)0xFFFFFFFFu;
    #else
		xcp.CheckSum = (tXcpChecksumSumType)0u;
    #endif
		/* The blocksize is limited to WORD length. */
		/* If the blocksize exceeds the allowed maximum transmit negative response. */
		if ( (s & (uint32)0xffff0000u) != (uint32)0u )
		{
			CRM_BUILD_CHECKSUM_RESULT = 0xFFFFu; /* Range, max. 64K-1 */
			err = CRC_OUT_OF_RANGE;
			xcp.CrmLen = CRM_BUILD_CHECKSUM_LEN;
			/* Response length is arbitrary */
			goto negative_response1; /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
		else
  #endif
			{

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
			/* Parameter check whether the block size is modulo 2 for ADD_22, ADD_24 */
			/* and ADD_44 (modulo 4).                                                */
    #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD22 ) || \
        ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD24 ) || \
        ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD44 )        
			if ( (s % (uint32)sizeof(tXcpChecksumAddType)) != (uint32)0u )
			{
				error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
			}
			else
    #endif    
  #endif
				{
  #if defined ( XCP_ENABLE_READ_PROTECTION )
				if( (uint8)XCP_CMD_DENIED == XcpAppl_CheckReadAccess( xcp.Mta, s ) )
				{
					xcp.Mta += s; /* PRQA S 0488 */ /* MD_Xcp_0488 */
					CRM_BUILD_CHECKSUM_RESULT = 0xFFFFu; /* Range, max. 64K-1 */
					err = CRC_ACCESS_DENIED;
					xcp.CrmLen = CRM_BUILD_CHECKSUM_LEN;
					/* Response length is arbitrary */
					goto negative_response1; /* PRQA S 2001 */ /* MD_Xcp_2001 */
				}
  #endif

  #if defined ( XCP_ENABLE_CUSTOM_CRC )
				err = XcpAppl_CalculateChecksum( xcp.Mta, xcp.Crm.b, s );
				xcp.Mta += s; /* PRQA S 0488 */ /* MD_Xcp_0488 */
				if (err==(uint8)XCP_CMD_PENDING)
				{
					goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
				}

				check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
  #else
				xcp.CheckSumSize = (uint16)s;
				/* Checksum calculation will be performed by XcpBackground() */
				goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
  #endif
			}
		}
	}
  #if defined ( XCP_ENABLE_CUSTOM_CRC )
	break;
  #endif

#endif /* XCP_ENABLE_CHECKSUM */


#if defined ( XCP_ENABLE_DAQ )

  #if defined ( XCP_ENABLE_DAQ_PROCESSOR_INFO )

case CC_GET_DAQ_PROCESSOR_INFO: /* PRQA S 2003 */ /* MD_Xcp_2003 */
	{

		xcp.CrmLen = CRM_GET_DAQ_PROCESSOR_INFO_LEN;
		CRM_GET_DAQ_PROCESSOR_INFO_MIN_DAQ = 0;
		CRM_GET_DAQ_PROCESSOR_INFO_MAX_DAQ = xcp.Daq.DaqCount; /* dynamic or static */
    #if defined ( kXcpMaxEvent )
		CRM_GET_DAQ_PROCESSOR_INFO_MAX_EVENT = kXcpMaxEvent;
    #else
		CRM_GET_DAQ_PROCESSOR_INFO_MAX_EVENT = 0; /* Unknown */
    #endif
    #if defined ( XCP_ENABLE_DAQ_HDR_ODT_DAQ )
		/* DTO identification field type: Relative ODT number, absolute list number (BYTE) */
		CRM_GET_DAQ_PROCESSOR_INFO_DAQ_KEY_BYTE = (uint8)DAQ_HDR_ODT_DAQB;
    #else
		/* DTO identification field type: Absolute ODT number */
		CRM_GET_DAQ_PROCESSOR_INFO_DAQ_KEY_BYTE = (uint8)DAQ_HDR_PID;
    #endif
		CRM_GET_DAQ_PROCESSOR_INFO_PROPERTIES = (uint8)( DAQ_PROPERTY_CONFIG_TYPE
    #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )
		| DAQ_PROPERTY_TIMESTAMP
    #endif
    #if defined ( XCP_ENABLE_DAQ_PRESCALER )
		| DAQ_PROPERTY_PRESCALER
    #endif
    #if defined ( XCP_ENABLE_DAQ_RESUME )
		| DAQ_PROPERTY_RESUME
    #endif
    #if defined ( XCP_ENABLE_DAQ_OVERRUN_INDICATION ) /* DAQ_PROPERTY_OVERLOAD_INDICATION */
		| DAQ_OVERLOAD_INDICATION_PID
    #endif
		);

	}
	break;

  #endif /* XCP_ENABLE_DAQ_PROCESSOR_INFO */

  #if defined ( XCP_ENABLE_DAQ_RESOLUTION_INFO )
case CC_GET_DAQ_RESOLUTION_INFO:
	{

		xcp.CrmLen = CRM_GET_DAQ_RESOLUTION_INFO_LEN;
		CRM_GET_DAQ_RESOLUTION_INFO_GRANULARITY_DAQ = 1;
		CRM_GET_DAQ_RESOLUTION_INFO_GRANULARITY_STIM = 1;
		CRM_GET_DAQ_RESOLUTION_INFO_MAX_SIZE_DAQ  = (uint8)XCP_MAX_ODT_ENTRY_SIZE;
		CRM_GET_DAQ_RESOLUTION_INFO_MAX_SIZE_STIM = (uint8)XCP_MAX_ODT_ENTRY_SIZE;
    #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )
		CRM_GET_DAQ_RESOLUTION_INFO_TIMESTAMP_MODE = kXcpDaqTimestampUnit | (uint8)sizeof(XcpDaqTimestampType)
      #if defined ( XCP_ENABLE_DAQ_TIMESTAMP_FIXED )
		| DAQ_TIMESTAMP_FIXED
      #endif
		;
		CRM_GET_DAQ_RESOLUTION_INFO_TIMESTAMP_TICKS = kXcpDaqTimestampTicksPerUnit;  /* BCD coded */
    #else
		CRM_GET_DAQ_RESOLUTION_INFO_TIMESTAMP_MODE = 0;
		CRM_GET_DAQ_RESOLUTION_INFO_TIMESTAMP_TICKS = 0;
    #endif /* XCP_ENABLE_DAQ_TIMESTAMP */

	}
	break;
  #endif /* XCP_ENABLE_DAQ_RESOLUTION_INFO */

  #if defined ( XCP_ENABLE_DAQ_EVENT_INFO ) && defined ( kXcpMaxEvent )
case CC_GET_DAQ_EVENT_INFO:
	{
		tXcpEventType event = (tXcpEventType)CRO_GET_DAQ_EVENT_INFO_EVENT;


    #if defined ( XCP_ENABLE_PARAMETER_CHECK )
		if (event >= (uint8)kXcpMaxEvent )
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
    #endif

		xcp.CrmLen = CRM_GET_DAQ_EVENT_INFO_LEN;
		CRM_GET_DAQ_EVENT_INFO_PROPERTIES = kXcpEventDirection[event];
		CRM_GET_DAQ_EVENT_INFO_MAX_DAQ_LIST = 1; /* Only one DAQ-List available per event channel */
		CRM_GET_DAQ_EVENT_INFO_NAME_LENGTH = kXcpEventNameLength[event];
		CRM_GET_DAQ_EVENT_INFO_TIME_CYCLE = kXcpEventCycle[event];
    #if defined ( XCP_ENABLE_CANAPE_5_5_X_SUPPORT )
		CRM_GET_DAQ_EVENT_INFO_TIME_UNIT = kXcpEventUnit[event];
    #else
		CRM_GET_DAQ_EVENT_INFO_TIME_UNIT = kXcpEventUnit[event]>>4;
    #endif
		CRM_GET_DAQ_EVENT_INFO_PRIORITY = 0;     /* Event channel prioritization is not supported. */
		XcpSetMta( XcpAppl_GetPointer( 0x00, (uint32)kXcpEventName[event]), 0x00 ); /* PRQA S 0306 */ /* MD_Xcp_0306 */

	}
	break;
  #endif /* XCP_ENABLE_DAQ_EVENT_INFO && kXcpMaxEvent */

  #if defined ( XCP_ENABLE_DAQ_RESUME )
case CC_SET_REQUEST:
	{

		if ( (CRO_SET_REQUEST_MODE & (SS_STORE_DAQ_REQ | SS_CLEAR_DAQ_REQ | SS_STORE_CAL_REQ | SS_STORE_DAQ_REQ_NO_RESUME)) != 0 )
		{
			if ( (CRO_SET_REQUEST_MODE & (SS_STORE_DAQ_REQ | SS_STORE_DAQ_REQ_NO_RESUME)) != 0 )
			{
				xcp.Daq.DaqConfigId = CRO_SET_REQUEST_CONFIG_ID;
				xcp.SessionStatus |= (SessionStatusType)SS_STORE_DAQ_REQ;
				if((CRO_SET_REQUEST_MODE & SS_STORE_DAQ_REQ_NO_RESUME) != 0 )
				{
					xcp.SessionStatus |= (SessionStatusType)SS_STORE_DAQ_REQ_NO_RESUME_INTERNAL;
				}
			}
			if ( (CRO_SET_REQUEST_MODE & SS_CLEAR_DAQ_REQ) != 0 )
			{
				xcp.SessionStatus |= (SessionStatusType)SS_CLEAR_DAQ_REQ;
			}
			if ( (CRO_SET_REQUEST_MODE & SS_STORE_CAL_REQ) != 0 )
			{
				xcp.SessionStatus |= (SessionStatusType)SS_STORE_CAL_REQ;
			}
		}
		else
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}

		xcp.CrmLen = CRM_SET_REQUEST_LEN;


	}
	break;
  #endif /* XCP_ENABLE_DAQ_RESUME */

case CC_FREE_DAQ:
	{

		CheckResourceProtection( RM_DAQ ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

			Xcp_FreeDaq();

	}
	break;

case CC_ALLOC_DAQ:
	{
		uint8 count = (uint8)CRO_ALLOC_DAQ_COUNT;

		check_error( Xcp_AllocDaq(count) ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

	}
	break;

case CC_ALLOC_ODT:
	{
		uint8 daq = (uint8)CRO_ALLOC_ODT_DAQ;
		uint8 count = CRO_ALLOC_ODT_COUNT;


  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
		if (daq>=xcp.Daq.DaqCount)
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
  #endif
		check_error( Xcp_AllocOdt(daq, count) ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

	}
	break;

case CC_ALLOC_ODT_ENTRY:
	{
		uint8 daq = (uint8)CRO_ALLOC_ODT_ENTRY_DAQ;
		uint8 odt = CRO_ALLOC_ODT_ENTRY_ODT;
		uint8 count = CRO_ALLOC_ODT_ENTRY_COUNT;


  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
		if ( (daq>=xcp.Daq.DaqCount) || (odt>=(uint8)DaqListOdtCount(daq)) )
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
  #endif
		check_error( Xcp_AllocOdtEntry(daq, odt, count) ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

	}
	break;

case CC_GET_DAQ_LIST_MODE:
	{
		uint8 daq = (uint8)CRO_GET_DAQ_LIST_MODE_DAQ;


  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
		if (daq>=xcp.Daq.DaqCount)
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
  #endif

		xcp.CrmLen = CRM_GET_DAQ_LIST_MODE_LEN;
		CRM_GET_DAQ_LIST_MODE_MODE = DaqListFlags(daq);
  #if defined ( XCP_ENABLE_DAQ_PRESCALER )
		CRM_GET_DAQ_LIST_MODE_PRESCALER = DaqListPrescaler(daq);
  #else
		CRM_GET_DAQ_LIST_MODE_PRESCALER = 1;
  #endif
  #if defined ( kXcpMaxEvent )
		CRM_GET_DAQ_LIST_MODE_EVENTCHANNEL = 0; /* #### Lookup in EventDaq[] */
  #else
		CRM_GET_DAQ_LIST_MODE_EVENTCHANNEL = DaqListEventChannel(daq);
  #endif
		CRM_GET_DAQ_LIST_MODE_PRIORITY = 0;  /* DAQ-list prioritization is not supported. */

	}
	break;

case CC_SET_DAQ_LIST_MODE:
	{
		uint8 daq = (uint8)CRO_SET_DAQ_LIST_MODE_DAQ;
  #if defined ( XCP_ENABLE_DAQ_PRESCALER ) || ( !defined ( XCP_ENABLE_DAQ_PRESCALER ) && defined ( XCP_ENABLE_PARAMETER_CHECK ) )
		uint8 xcpPrescaler = CRO_SET_DAQ_LIST_MODE_PRESCALER;
  #endif
		tXcpEventType event = (tXcpEventType)(CRO_SET_DAQ_LIST_MODE_EVENTCHANNEL);


  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
		if (daq>=xcp.Daq.DaqCount)
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
    #if defined ( kXcpMaxEvent )
		if (event >= (tXcpEventType)kXcpMaxEvent)
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
    #endif
    #if !defined ( XCP_ENABLE_DAQ_PRESCALER )
		if (xcpPrescaler!=1)
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
    #endif
		if (CRO_SET_DAQ_LIST_MODE_PRIORITY!=0)   /* Priorization is not supported */
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
  #endif

  #if defined ( XCP_ENABLE_DAQ_PRESCALER )
		if (xcpPrescaler==0)
		{
			xcpPrescaler = 1;
		}
		DaqListPrescaler(daq) = xcpPrescaler;
  #endif
  #if defined ( kXcpMaxEvent ) && ! defined ( XCP_ENABLE_STIM )
		xcp.Daq.EventDaq[event] = daq;
  #else
		DaqListEventChannel(daq) = event;
  #endif
		DaqListFlags(daq) = CRO_SET_DAQ_LIST_MODE_MODE;

  #if defined ( XCP_ENABLE_STIM )
		check_error( Xcp_AllocStimMemory() ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
  #endif

		break;
	}


case CC_SET_DAQ_PTR:
	{
		uint8 daq = (uint8)(CRO_SET_DAQ_PTR_DAQ);
		uint8 odt = CRO_SET_DAQ_PTR_ODT;
		uint8 idx = CRO_SET_DAQ_PTR_IDX;
		tXcpOdtIdx odt0 = (tXcpOdtIdx)(DaqListFirstOdt(daq)+odt); /* Absolute odt number */


  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
		if ( (daq>=xcp.Daq.DaqCount) || (odt>=(uint8)DaqListOdtCount(daq)) || (idx>=(uint8)DaqListOdtEntryCount(odt0)) )
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
  #endif

		xcp.CrmLen = CRM_SET_DAQ_PTR_LEN;
		xcp.DaqListPtr = DaqListOdtFirstEntry(odt0)+idx;
#if defined ( XCP_ENABLE_SILENT_CHECK )
		/* SBSW_AsrXcp_010 */
		if(&OdtEntrySize(xcp.DaqListPtr) >= &xcp.Daq.u.b[kXcpDaqMemSize])
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
#endif
	}
	break;

case CC_WRITE_DAQ: /* Write DAQ entry */
	{
		DAQBYTEPTR addr;

		CheckResourceProtection( RM_DAQ ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
		if ( ((uint8)CRO_WRITE_DAQ_SIZE==(uint8)0u ) || (CRO_WRITE_DAQ_SIZE > (uint8)XCP_MAX_ODT_ENTRY_SIZE) )
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
		if ( (0u == xcp.Daq.DaqCount) || (0u == xcp.Daq.OdtCount) || (0u == xcp.Daq.OdtEntryCount) )
		{
			error(CRC_DAQ_CONDIF) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
  #endif
		addr = (DAQBYTEPTR)XcpAppl_GetPointer(CRO_WRITE_DAQ_EXT, CRO_WRITE_DAQ_ADDR);
              #if defined ( XCP_ENABLE_READ_PROTECTION ) || defined ( XCP_ENABLE_WRITE_PROTECTION )
		if( XCP_CMD_DENIED == XcpAppl_CheckDAQAccess(addr, CRO_WRITE_DAQ_SIZE) )
		{
			error(CRC_ACCESS_DENIED) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
              #endif

		xcp.CrmLen = CRM_WRITE_DAQ_LEN;
              #if defined ( XCP_ENABLE_SILENT_CHECK )
		/* SBSW_AsrXcp_010 */
		if(&OdtEntrySize(xcp.DaqListPtr) >= &xcp.Daq.u.b[kXcpDaqMemSize])
		{
			error(CRC_DAQ_CONDIF) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
              #endif
		OdtEntrySize(xcp.DaqListPtr) = CRO_WRITE_DAQ_SIZE;
		OdtEntryAddr(xcp.DaqListPtr) = addr;
		xcp.DaqListPtr++; /* Autoincrement */

	}
	break;

case CC_START_STOP_DAQ_LIST:
	{
		uint8 daq = (uint8)(CRO_START_STOP_DAQ);

		CheckResourceProtection( RM_DAQ ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

  #if defined ( XCP_ENABLE_PARAMETER_CHECK )
		if (daq>=xcp.Daq.DaqCount)
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
  #endif

		if ( (CRO_START_STOP_MODE==1 ) || (CRO_START_STOP_MODE==2) )
		{
			DaqListFlags(daq) |= (uint8)DAQ_FLAG_SELECTED;
			if ( CRO_START_STOP_MODE == (uint8)1u )
			{
				Xcp_StartDaq(daq);
			}
			xcp.CrmLen = CRM_START_STOP_LEN;
			CRM_START_STOP_FIRST_PID = DaqListFirstPid(daq);
		}
		else
		{
			Xcp_StopDaq(daq);
		}

	}
	break;

case CC_START_STOP_SYNCH:
	{

		CheckResourceProtection( RM_DAQ ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

		if( (0 == xcp.Daq.DaqCount) || (0 == xcp.Daq.OdtCount) || (0 == xcp.Daq.OdtEntryCount) )
		{
			error(CRC_DAQ_CONDIF) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}

		if (CRO_START_STOP_MODE==2) /* stop selected */
		{
			Xcp_StopAllSelectedDaq();
		}
		else
		{
			if (CRO_START_STOP_MODE==1) /* start selected */
			{
				Xcp_StartAllSelectedDaq();
			}
			else
			{
				/* CRO_START_STOP_MODE==0 : stop all */
				Xcp_StopAllDaq();
			}
		}
	}
	break;

  #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )
case CC_GET_DAQ_CLOCK:
	{
		xcp.CrmLen = CRM_GET_DAQ_CLOCK_LEN;
		CRM_GET_DAQ_CLOCK_TIME = (uint32)XcpAppl_GetTimestamp();

	}
	break;
  #endif

#endif /* XCP_ENABLE_DAQ */


	/* Flash Programming Kernel Download */
#if defined ( XCP_ENABLE_BOOTLOADER_DOWNLOAD )

case CC_PROGRAM_PREPARE: /* Prepare flash kernel download */
	{
		XCP_PRINT(("-> PROGRAM_PREPARE\n"));

		if ( XcpAppl_DisableNormalOperation(xcp.Mta, CRO_PROGRAM_PREPARE_SIZE) == (uint8)XCP_CMD_DENIED )
		{ /* ESCAN00009199 */
			error(CRC_GENERIC) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
		xcp.CrmLen = CRM_PROGRAM_PREPARE_LEN;

		XCP_PRINT(("<- 0xFF\n"));
	}
	break;

#endif /* XCP_ENABLE_BOOTLOADER_DOWNLOAD */


	/* Flash Programming direct and Kernel */
#if defined ( XCP_ENABLE_PROGRAM ) || defined ( XCP_ENABLE_BOOTLOADER_DOWNLOAD )

case CC_PROGRAM_START:   /* Start flash kernel */
	{
		XCP_PRINT(("-> PROGRAM_START\n"));

		CheckResourceProtection( RM_PGM ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

			xcp.CrmLen = CRM_PROGRAM_START_LEN;
		CRM_PROGRAM_MAX_CTO_PGM = xcp.Daq.xcpMaxCTO;
		CRM_PROGRAM_COMM_MODE_PGM = (uint8)0u;
               #if defined ( XCP_ENABLE_BLOCK_UPLOAD )
		CRM_PROGRAM_COMM_MODE_PGM |= (uint8)CMB_SLAVE_BLOCK_MODE;
               #endif
               #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
		CRM_PROGRAM_COMM_MODE_PGM |= (uint8)CMO_MASTER_BLOCK_MODE;
		CRM_PROGRAM_MAX_BS_PGM = (uint8)((254 / CRO_PROGRAM_MAX_SIZE) + (uint8)1u);
		CRM_PROGRAM_MIN_ST_PGM = (uint8)kXcpProgramMinStPgm;
               #else
		CRM_PROGRAM_MAX_BS_PGM = (uint8)0u;
		CRM_PROGRAM_MIN_ST_PGM = (uint8)0u;
               #endif 
		CRM_PROGRAM_QUEUE_SIZE_PGM = (uint8)0u; /* Interleaved mode is not implemented */

  #if defined ( XCP_ENABLE_BOOTLOADER_DOWNLOAD )
		if ( XcpAppl_StartBootLoader() != (uint8)XCP_CMD_OK ) /* Never returns */
		{
			error(CRC_GENERIC) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
  #else
		err = XcpAppl_ProgramStart();
		if (err == XCP_CMD_PENDING)
		{
			goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
		if ( err != XCP_CMD_OK )
		{
			error(CRC_GENERIC) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
  #endif

		XCP_PRINT(("<- 0xFF\n"));
	}
	break;
#endif

	/* Flash Programming  */
#if defined ( XCP_ENABLE_PROGRAM )

	/* Sector info*/
  #if defined ( XCP_ENABLE_PROGRAM_INFO )
case CC_GET_SECTOR_INFO:
	{
		uint8 sector = CRO_PROGRAM_GET_SECTOR_INFO_NUMBER;

		XCP_PRINT(("-> GET_SECTOR_INFO sector=%u\n", sector));

    #if defined ( XCP_ENABLE_PARAMETER_CHECK )
		if (sector>=kXcpProgramMaxSector)
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
    #endif


		/* The programming method is per default Sequential Programming (ESCAN00009202) */
		CRM_PROGRAM_GET_SECTOR_PGM_METHOD = (uint8)0x00;

		/* Mode 0: get start address of sector */
		if (CRO_PROGRAM_GET_SECTOR_INFO_MODE == 0)
		{
			CRM_PROGRAM_GET_SECTOR_SECTOR_INFO = kXcpProgramSectorStart[sector];
			XCP_PRINT(("<- 0xFF address=%u\n", CRM_PROGRAM_GET_SECTOR_SECTOR_INFO));
		}
		else
		/* Mode 1: get length of sector */
		if (CRO_PROGRAM_GET_SECTOR_INFO_MODE == 1)
		{
			CRM_PROGRAM_GET_SECTOR_SECTOR_INFO = (kXcpProgramSectorEnd[sector]-kXcpProgramSectorStart[sector])+(uint32)1u;
			XCP_PRINT(("<- 0xFF length=%u\n", CRM_PROGRAM_GET_SECTOR_SECTOR_INFO));
		}
		else
		/* Mode 2: get name length of this sector */
		if (CRO_PROGRAM_GET_SECTOR_INFO_MODE == 2)
		{
			CRM_PROGRAM_GET_SECTOR_NAME_LENGTH = kXcpSectorNameLength[sector];
			XcpSetMta(XcpAppl_GetPointer(0x00, (uint32)(kXcpSectorName[sector])), 0x00) /* PRQA S 0306 */ /* MD_Xcp_0306 */
				XCP_PRINT(("<- 0xFF length=%u\n", CRM_PROGRAM_GET_SECTOR_NAME_LENGTH));
		}
		else
		{
			error(CRC_CMD_SYNTAX) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}

		xcp.CrmLen = CRM_PROGRAM_GET_SECTOR_INFO_LEN;
	}
	break;


case CC_GET_PGM_PROCESSOR_INFO:
	{
		XCP_PRINT(("-> GET_PGM_PROCESSOR_INFO\n"));

		xcp.CrmLen = CRM_GET_PGM_PROCESSOR_INFO_LEN;
		/* PGM_PROPERTIES: only the default programming format is supported (ESCAN00009202) */
		CRM_GET_PGM_PROCESSOR_INFO_PROPERTIES = (uint8)( PGM_ACCESS_ABSOLUTE | PGM_COMPRESSION_NONE | PGM_ENCRYPTION_NONE | PGM_NON_SEQ_NONE );
		/* MAX_SECTOR */
		CRM_GET_PGM_PROCESSOR_INFO_MAX_SECTOR = kXcpProgramMaxSector;

		XCP_PRINT(("<- 0xFF MAX_SECTOR =%u\n", CRM_GET_PGM_PROCESSOR_INFO_MAX_SECTOR));
	}
	break;

  #endif /* XCP_ENABLE_PROGRAM_INFO */

case CC_PROGRAM_RESET: /* disconnect without response */
	{
		XCP_PRINT(("-> PROGRAM_RESET\n"));
		Xcp_Disconnect();
		XcpAppl_Reset(); /* May not return */
		XCP_PRINT(("<- 0xFF\n"));
	}
	break;


case CC_PROGRAM_CLEAR:
	{
		XCP_PRINT(("-> PROGRAM_CLEAR\n"));

		CheckResourceProtection( RM_PGM ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

#if defined ( XCP_ENABLE_PROGRAMMING_WRITE_PROTECTION )
		err = XcpAppl_CheckProgramAccess(xcp.Mta, CRO_PROGRAM_CLEAR_SIZE);
		if ( err == (uint8)XCP_CMD_DENIED )
		{
			error(CRC_ACCESS_DENIED) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
#endif

		/* Only the absolute access mode (default access mode) is available. */
		xcp.CrmLen = CRM_PROGRAM_CLEAR_LEN;

		err = XcpAppl_FlashClear(xcp.Mta, CRO_PROGRAM_CLEAR_SIZE);
		if (err == XCP_CMD_PENDING)
		{ /* FLASH clear pending */
			goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
		if (err != XCP_CMD_OK)
		{
			error(CRC_GENERIC) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}

		XCP_PRINT(("<- 0xFF\n"));
	}
	break;

  #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
case CC_PROGRAM_NEXT:


	if (CRO_PROGRAM_NEXT_SIZE != xcp.NextDataBlockSize)
	{
		error1(CRC_SEQUENCE, xcp.NextDataBlockSize) /* PRQA S 2001 */ /* MD_Xcp_2001 */
	}
	/* |||| */
  #endif

case CC_PROGRAM: /* PRQA S 2003 */ /* MD_Xcp_2003 */
	{
		uint8 size = CRO_PROGRAM_SIZE;
		uint8 r;


		CheckResourceProtection( RM_PGM ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

  #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
		xcp.NextDataBlockSize = size;
		if (size>(uint8)CRO_PROGRAM_MAX_SIZE)
		{
			size = CRO_PROGRAM_MAX_SIZE;
		}
		xcp.NextDataBlockSize -= size;
  #else
		if (size>(uint8)CRO_PROGRAM_MAX_SIZE)
		{
			error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
  #endif

#if defined ( XCP_ENABLE_PROGRAMMING_WRITE_PROTECTION )
		r = XcpAppl_CheckProgramAccess(xcp.Mta, size);
		if ( r == (uint8)XCP_CMD_DENIED )
		{
			xcp.Mta += size; /* PRQA S 0488 */ /* MD_Xcp_0488 */
			error(CRC_ACCESS_DENIED) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
#endif
		r = XcpAppl_FlashProgram(CRO_PROGRAM_DATA, xcp.Mta, size);
		xcp.Mta += size; /* PRQA S 0488 */ /* MD_Xcp_0488 */
		xcp.CrmLen = CRM_PROGRAM_LEN;
		if (r==XCP_CMD_PENDING)
		{ /* FLASH write pending */
			goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
		if (r!=XCP_CMD_OK)
		{
			error(CRC_GENERIC) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}

  #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
		if ( xcp.NextDataBlockSize != 0 )
		{
			goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
  #endif
		XCP_PRINT(("<- 0xFF\n"));
	}
	break;

  #if defined ( XCP_ENABLE_BLOCK_DOWNLOAD )
  #else

case CC_PROGRAM_MAX:
	{
		uint8 r;


		CheckResourceProtection( RM_PGM ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

#if defined ( XCP_ENABLE_PROGRAMMING_WRITE_PROTECTION )
		r = XcpAppl_CheckProgramAccess(xcp.Mta, CRO_PROGRAM_MAX_MAX_SIZE);
		if ( r == (uint8)XCP_CMD_DENIED )
		{
			xcp.Mta += CRO_PROGRAM_MAX_MAX_SIZE; /* PRQA S 0488 */ /* MD_Xcp_0488 */
			error(CRC_ACCESS_DENIED) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
#endif
		r = XcpAppl_FlashProgram(CRO_PROGRAM_MAX_DATA, xcp.Mta, CRO_PROGRAM_MAX_MAX_SIZE);
		xcp.Mta += CRO_PROGRAM_MAX_MAX_SIZE; /* PRQA S 0488 */ /* MD_Xcp_0488 */
		if (r==XCP_CMD_PENDING)
		{ /* FLASH write pending */
			goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}
		if (r!=XCP_CMD_OK)
		{
			error(CRC_GENERIC) /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}

		XCP_PRINT(("<- 0xFF\n"));
	}
	break;

  #endif /* !defined ( XCP_ENABLE_BLOCK_DOWNLOAD ) */ 
#endif /* XCP_ENABLE_PROGRAM */

#if defined ( XCP_ENABLE_WRITE_DAQ_MULTIPLE )
case CC_WRITE_DAQ_MULTIPLE:
	{
		uint8 NoDAQ;

		CheckResourceProtection( RM_DAQ ) /* PRQA S 2001 */ /* MD_Xcp_2001 */

		for(NoDAQ = 0; NoDAQ < CRO_WRITE_DAQ_MULTIPLE_NODAQ; NoDAQ++)
		{
			DAQBYTEPTR addr;

    #if defined ( XCP_ENABLE_PARAMETER_CHECK )
			if ( ((uint8)CRO_WRITE_DAQ_MULTIPLE_SIZE(NoDAQ)==(uint8)0u )
				|| (CRO_WRITE_DAQ_MULTIPLE_SIZE(NoDAQ) > (uint8)XCP_MAX_ODT_ENTRY_SIZE)
				|| (CRO_WRITE_DAQ_MULTIPLE_NODAQ > (uint8)((xcp.Daq.xcpMaxCTO-3u) / 8u)) )
			{
				error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
			}
    #endif
			addr = (DAQBYTEPTR)XcpAppl_GetPointer(CRO_WRITE_DAQ_MULTIPLE_EXT(NoDAQ), CRO_WRITE_DAQ_MULTIPLE_ADDR(NoDAQ));
                #if defined ( XCP_ENABLE_READ_PROTECTION ) || defined ( XCP_ENABLE_WRITE_PROTECTION )
			if( XCP_CMD_DENIED == XcpAppl_CheckDAQAccess(addr, CRO_WRITE_DAQ_MULTIPLE_SIZE(NoDAQ)) )
			{
				error(CRC_ACCESS_DENIED) /* PRQA S 2001 */ /* MD_Xcp_2001 */
			}
                #endif

                #if defined ( XCP_ENABLE_SILENT_CHECK )
			/* SBSW_AsrXcp_010 */
			if(&OdtEntrySize(xcp.DaqListPtr) > &xcp.Daq.u.b[kXcpDaqMemSize])
			{
				error(CRC_OUT_OF_RANGE) /* PRQA S 2001 */ /* MD_Xcp_2001 */
			}
                #endif
			OdtEntrySize(xcp.DaqListPtr) = CRO_WRITE_DAQ_MULTIPLE_SIZE(NoDAQ);
			OdtEntryAddr(xcp.DaqListPtr) = addr;
			xcp.DaqListPtr++; /* Autoincrement */
		}
		xcp.CrmLen = CRM_WRITE_DAQ_LEN;
	}
	break;
#endif /* XCP_ENABLE_WRITE_DAQ_MULTIPLE */


#if defined ( XCP_ENABLE_USER_COMMAND )
case CC_USER_CMD:
	{


  #if defined ( XCP_ENABLE_USER_COMMAND )
		err = XcpAppl_UserService( (ROMBYTEPTR) &CRO_WORD(0) ); /* PRQA S 0310 */ /* MD_Xcp_0310 */

		if (err==(uint8)XCP_CMD_PENDING)
		{
			goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}

		check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
  #endif

	}
	break;
#endif


#if defined ( XCP_ENABLE_TL_COMMAND )
case CC_TRANSPORT_LAYER_CMD:
	{
		err = XcpCallTlFunction_1_Param( ApplXcpTLService, &CRO_BYTE(0) );
		if (err==(uint8)XCP_CMD_PENDING)
		{
			goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}

		check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
	}
	break;
#endif


default: /* unknown */
	{
#if defined ( XCP_ENABLE_OPENCMDIF )
		err = XcpAppl_OpenCmdIf( pCmd->b, xcp.Crm.b, &xcp.CrmLen );
		if (err==(uint8)XCP_CMD_PENDING)
		{
			goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
		}

		check_error( err ) /* PRQA S 2001 */ /* MD_Xcp_2001 */
#else
		error(CRC_CMD_UNKNOWN) /* PRQA S 2001 */ /* MD_Xcp_2001 */
#endif
	}

} /* switch */

goto positive_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
}

    /* Not connected */
else
{
	goto no_response; /* PRQA S 2001 */ /* MD_Xcp_2001 */
}
} /* CC_CONNECT */

negative_response:
xcp.CrmLen = 2;

#if defined ( XCP_ENABLE_CHECKSUM ) || \
    (defined ( XCP_ENABLE_BLOCK_DOWNLOAD ) && defined ( XCP_ENABLE_CALIBRATION )) || \
    (defined ( XCP_ENABLE_PROGRAM ) && defined ( XCP_ENABLE_BLOCK_DOWNLOAD ))
negative_response1:
#endif
CRM_CMD = (uint8)PID_ERR;
CRM_ERR = (uint8)err;

positive_response:
Xcp_SendCrm();

no_response:
Xcp_Rtm_Stop(Xcp_Command);
return;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6010 */ /* MD_MSR_STPTH */ /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* PRQA S 6050 */ /* MD_MSR_STCAL */

/****************************************************************************/
/* Send notification callback                                               */
/****************************************************************************/


/*****************************************************************************
| NAME:             Xcp_SendCallBack
| CALLED BY:        XCP Transport Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    0 : if the XCP Protocol Layer is idle (no transmit messages are pending)
| DESCRIPTION:      Notifies the XCP Protocol Layer about the successful
|                   transmission of a XCP packet.
******************************************************************************/
FUNC(uint8, XCP_CODE) Xcp_SendCallBack( void )
{
	Xcp_CheckDetErrorReturnValue( XCP_UNINIT != Xcp_InitializationState,
		XCP_SENDCALLBACK_SERVICE_ID, XCP_E_NOT_INITIALIZED, (uint8)1u )

		/* Activation control */
		Xcp_CheckControlStateRet((uint8)1u)

#if defined ( XCP_ENABLE_DAQ ) && defined ( XCP_ENABLE_SEND_QUEUE )

	/* Clear all pending flags */
	/* A pending flag indicates that ApplXcpSend() is in progress */
	xcp.SendStatus &= (uint8)(~XCP_SEND_PENDING & 0xFFu);

	/* Now check if there is another transmit request */

	/* Send a RES or ERR (CRM) message */
	if ( (xcp.SendStatus & (uint8)XCP_CRM_REQUEST) != 0 )
	{
		xcp.SendStatus &= (uint8)(~XCP_CRM_REQUEST & 0xFFu);
		Xcp_SendCrm();
		return (uint8)0x01u;
	}

	/* Send a EV or SERV message */
  #if defined ( XCP_ENABLE_SEND_EVENT ) || defined ( XCP_ENABLE_SERV_TEXT )
	if ( (xcp.SendStatus & (uint8)XCP_EVT_REQUEST) != 0 )
	{
		xcp.SendStatus &= (uint8)(~XCP_EVT_REQUEST & 0xFFu);
		Xcp_SendEv();
		return (uint8)0x01u;
	}
  #endif

	/* Send a DAQ message from the queue or from the buffer */
	if ( (xcp.SessionStatus & (SessionStatusType)SS_DAQ) != 0 )
	{
		if ( Xcp_SendDtoFromQueue() != 0 )
		{
			return (uint8)0x01u;
		}
	}
#endif /* XCP_ENABLE_DAQ && XCP_ENABLE_SEND_QUEUE */

	/* Continue a pending block upload command */
#if defined ( XCP_ENABLE_BLOCK_UPLOAD )
	if ( (xcp.SessionStatus & (SessionStatusType)SS_BLOCK_UPLOAD) != 0 )
	{
		union { /* PRQA S 0750 */ /* MD_MSR_18.4 */
			uint8  b[4];
			uint32 dw;
		} cmd; /* PRQA S 0759 */ /* MD_MSR_18.4 */
		cmd.b[0] = CC_UPLOAD;
		cmd.b[1] = xcp.NextDataBlockSize;
		xcp.SessionStatus &= (SessionStatusType)(~(SessionStatusType)SS_BLOCK_UPLOAD); /* PRQA S 0277 */ /* MD_Xcp_0277 */
		Xcp_Command( &cmd.dw );

		return (uint8)0x01u;
	}
#endif

	return (uint8)0x00u;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH */


/****************************************************************************/
/* Initialization / de-initialization                                       */
/****************************************************************************/

/*****************************************************************************
| NAME:             Xcp_InitMemory
| CALLED BY:        application
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Initialization of the XCP Protocol Layer memory
******************************************************************************/
FUNC(void, XCP_CODE) Xcp_InitMemory( void )
{
	/* Initialize all XCP variables to zero */
	Xcp_MemSet( (BYTEPTR)&xcp, (uint16)sizeof(xcp), (uint8)0 ); /* PRQA S 0310 */ /* MD_Xcp_0310_ByteCopy */

#if defined ( XCP_ENABLE_DEV_ERROR_DETECT )
	Xcp_InitializationState = XCP_UNINIT;
#endif

#if defined ( XCP_ENABLE_CONTROL )
	Xcp_ControlState = kXcp_Control_Disable;
#endif
}


/*****************************************************************************
| NAME:             Xcp_Init
| CALLED BY:        application
| PRECONDITIONS:    the data link layer has to be initialized.
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Initialization of the XCP Protocol Layer
|                   Application specific initialization
|                    ( e.g. Vector XCP on CAN Transport Layer )
******************************************************************************/
FUNC(void, XCP_CODE) Xcp_Init( void )
{

#if defined ( XCP_ENABLE_DEV_ERROR_DETECT )
	Xcp_InitializationState = XCP_UNINIT;
#endif

#if defined ( XCP_ENABLE_CONTROL )
	Xcp_ControlState = kXcp_Control_Enable;
#endif
	total_frames = (uint8)0u;
	/* Initialize the RAM interface */

	/* Initialize the session status (ESCAN00013899) */
	xcp.SessionStatus = (SessionStatusType)0u;

  #if defined ( XCP_ENABLE_SEND_QUEUE )
	/* Initialize the transmit queue (ESCAN00013899) */
	xcp.SendStatus = (uint8)0u;
  #if defined ( XCP_SEND_QUEUE_OVERFLOW_FIRST )
	xcp.Daq.SendQueueBehaviour = (uint8)1u;
  #else
	xcp.Daq.SendQueueBehaviour = (uint8)0u;
  #endif
  #endif

  #if defined ( XCP_ENABLE_DEV_ERROR_DETECT )
	Xcp_InitializationState = XCP_INIT;
  #endif

	/* We set 8 bytes as default for CAN */
	Xcp_SetActiveTl(8, 8, 0);

#if defined ( XCP_ENABLE_VX_SUPPORT )
	/* Initialize the VX1000 system
	Must be called before any other VX1000_xxx function is called */
	VX1000_INIT()

		/* Check if the VX1000 is connected and powered
		Confirm that the VX1000 has been detected
		Take any actions needed to enable a cold start data acquisition */
  #if defined ( XCP_ENABLE_VX1000_TRICORE )
	VX1000_DETECT()
	{
		/* VX1000 will take over control for EDRAM and OVRCON 
		It may be nessesary to avoid resource conflicts
		For instance, disable the XCP on CAN driver which uses the overlay feature for calibration */
	}
  #endif
	if( 0 != VX1000_DETECTED() )
	{
		/* Shut down XCP driver operation, from now on we use VX1000 */
		Xcp_Disconnect();
		Xcp_VxActive = 1;
		/* We do not use resume mode */
		return;
	}
	else
	{
		Xcp_VxActive = 0;
	}

#endif

	/* Resume DAQ */
#if defined ( XCP_ENABLE_DAQ )
  #if defined ( XCP_ENABLE_DAQ_RESUME )
	if ( XcpAppl_DaqResume( &xcp.Daq ) != 0 )
	{
		/* Adjust the pointers */
		(void)Xcp_AllocMemory();
		/* Goto temporary disconnected mode and start all selected DAQ lists */
		xcp.SessionStatus |= (SessionStatusType)SS_RESUME;
		/* Select all DAQ lists. */
		Xcp_StartAllSelectedDaq();

		/* Send an event message to indicate resume mode */
		/* #### */


	}
  #endif /* XCP_ENABLE_DAQ_RESUME */
#endif /* XCP_ENABLE_DAQ */
}

/*****************************************************************************
| NAME:             Xcp_Exit
| CALLED BY:        application
| PRECONDITIONS:    The XCP Protocol Layer has to be initialized.
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      De-initialization of the XCP Protocol Layer.
******************************************************************************/
FUNC(void, XCP_CODE) Xcp_Exit( void )
{
	Xcp_CheckDetErrorReturnVoid( XCP_UNINIT != Xcp_InitializationState,
		XCP_EXIT_SERVICE_ID, XCP_E_NOT_INITIALIZED )

		/* Activation control */
		Xcp_CheckControlState()

		/* Deinitialize the RAM interface */
} /* PRQA S 2006 */ /* MD_MSR_14.7 */


/****************************************************************************/
/* Print via SERV/SERV_TEXT                                                */
/****************************************************************************/


#if defined ( XCP_ENABLE_SERV_TEXT )
  #if defined ( XCP_ENABLE_SERV_TEXT_PUTCHAR )

/*****************************************************************************
| NAME:             Xcp_PutChar
| CALLED BY:        application, Xcp_Print
| PRECONDITIONS:    XCP is initialized and in connected state.
| INPUT PARAMETERS: c : character
| RETURN VALUES:    none
| DESCRIPTION:      Put a char into a service request packet (SERV).
******************************************************************************/
FUNC(void, XCP_CODE) Xcp_Putchar( CONST(uint8, AUTOMATIC) c )
{
	Xcp_CheckDetErrorReturnVoid( XCP_UNINIT != Xcp_InitializationState,
		XCP_PUTCHAR_SERVICE_ID, XCP_E_NOT_INITIALIZED )

		/* Activation control */
		Xcp_CheckControlState()

		/* Check for stall condition */
    #if defined ( XCP_ENABLE_SEND_QUEUE )

	while ( (xcp.SendStatus & (uint8)XCP_EVT_REQUEST) != 0 )
	{
		if ( XcpAppl_SendStall() == 0 )
		{
			return; /* Abort */
		}
	}

    #endif

	/* If xcp.EvLen!=0 there is a pending text message*/
	if (xcp.EvLen<2)
	{
		xcp.EvLen = 2;
	}

	xcp.Ev.b[xcp.EvLen] = c;
	xcp.EvLen++;

	/* SBSW_AsrXcp_026 */
	if ( (xcp.EvLen>=(uint8)xcp.Daq.xcpMaxCTO) || (c==(uint8)0x00u) )  /* Flush */
	{
		EV_BYTE(0) = 0xFC; /* SERV */
		EV_BYTE(1) = 0x01; /* SERV_TEXT*/
		Xcp_SendEv();
	}
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

    #if defined ( XCP_ENABLE_SERV_TEXT_PRINT )

/*****************************************************************************
| NAME:             Xcp_Print
| CALLED BY:        application
| PRECONDITIONS:    XCP is initialized and in connected state.
| INPUT PARAMETERS: *str : pointer to a string
| RETURN VALUES:    none
| DESCRIPTION:      Transmission of a service request packet (SERV).
******************************************************************************/
FUNC(void, XCP_CODE) Xcp_Print( P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) str )
{
	Xcp_CheckDetErrorReturnVoid( XCP_UNINIT != Xcp_InitializationState,
		XCP_PRINT_SERVICE_ID, XCP_E_NOT_INITIALIZED )
		/* SBSW_AsrXcp_028 */
		Xcp_SilentBswCheckErrorReturnVoid( (P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA))NULL_PTR != str,
		XCP_PRINT_SERVICE_ID, XCP_E_NULL_POINTER )

		/* Activation control */
		Xcp_CheckControlState()

	/* Transmit the text message. */
	while ( *str != 0x00 )
	{
		Xcp_Putchar(*str);
		str++; /* PRQA S 0489 */ /* MD_MSR_17.4 */
	}
	/* Transmit the terminating 0x00. */
	Xcp_Putchar( (uint8)0x00u );
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

    #endif /* XCP_ENABLE_SERV_TEXT_PRINT */
    #if defined ( XCP_ENABLE_SERV_TEXT_PRINTF )

/*****************************************************************************
| NAME:             Xcp_Printf
| CALLED BY:        application
| PRECONDITIONS:    none
| INPUT PARAMETERS: *str : pointer to a string
|                   ...  : varaibale number of parameters (see printf)
| RETURN VALUES:    none
| DESCRIPTION:      Printf into a SERV_TEXT message
******************************************************************************/
FUNC(void, XCP_CODE) Xcp_Printf( P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) str, ... )
{
	va_list argptr;
	uint8 buf[128];

	Xcp_CheckDetErrorReturnVoid( XCP_UNINIT != Xcp_InitializationState,
		XCP_PRINTF_SERVICE_ID, XCP_E_NOT_INITIALIZED )
		/* SBSW_AsrXcp_028 */
		Xcp_SilentBswCheckErrorReturnVoid( (P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA))NULL_PTR != str,
		XCP_PRINTF_SERVICE_ID, XCP_E_NULL_POINTER )

		/* Activation control */
		Xcp_CheckControlState()

		va_start(argptr, str);
	vsprintf((P2VAR(int8, AUTOMATIC, XCP_APPL_DATA))buf, (P2CONST(int8, AUTOMATIC, XCP_APPL_DATA))str, argptr);
	va_end(argptr);

	/* Transmit the text message*/
	{
		P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) p = buf;
		while (*p != 0)
		{
			Xcp_Putchar(*p);
			p++;
		}
	}
	/* Transmit the terminating 0x00. */
	Xcp_Putchar( 0x00 );
}

    #endif /* XCP_ENABLE_SERV_TEXT_PRINTF */
  #endif /* XCP_ENABLE_SERV_TEXT_PUTCHAR */
#endif /* XCP_ENABLE_SERV_TEXT */


#if defined ( XCP_ENABLE_SEND_EVENT )

/*****************************************************************************
| NAME:             Xcp_SendEvent
| CALLED BY:        application
| PRECONDITIONS:    none
| INPUT PARAMETERS: evc : event code 
|                   c   : pointer to event data
|                   len : event data length
| RETURN VALUES:    none
| DESCRIPTION:      Transmission of an event packet (EV).
******************************************************************************/
FUNC(void, XCP_CODE) Xcp_SendEvent( uint8 evc, ROMBYTEPTR c, uint8 len)
{
	uint8 i;

	Xcp_CheckDetErrorReturnVoid( XCP_UNINIT != Xcp_InitializationState,
		XCP_SENDEVENT_SERVICE_ID, XCP_E_NOT_INITIALIZED )
		/* SBSW_AsrXcp_028 */
		Xcp_SilentBswCheckErrorReturnVoid( (ROMBYTEPTR)NULL_PTR != c,
		XCP_SENDEVENT_SERVICE_ID, XCP_E_NULL_POINTER )

		/* Activation control */
		Xcp_CheckControlState()

		/* Check for stall condition */
#if defined ( XCP_ENABLE_SEND_QUEUE )
	while ( (xcp.SendStatus & (uint8)XCP_EVT_REQUEST) != (uint8)0u )
	{
		if (!XcpAppl_SendStall())
		{
			return; /* Abort */
		}
	}
#endif

	EV_BYTE(0)  = PID_EV; /* Event*/
	EV_BYTE(1)  = evc;  /* Event Code*/
	xcp.EvLen   = 2;
	/* SBSW_AsrXcp_026 */
	if (len <= (uint8)(xcp.Daq.xcpMaxCTO-2) )
	{
		for (i = 0; i < len; i++)
		{
			xcp.Ev.b[xcp.EvLen] = c[i];
			xcp.EvLen++;
		}
	}


	Xcp_SendEv();
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#endif /* XCP_ENABLE_SEND_EVENT */

#if defined ( XCP_ENABLE_GET_SESSION_STATUS_API )
/*****************************************************************************
| NAME:             Xcp_GetSessionStatus
| CALLED BY:        XcpPreCopy
| PRECONDITIONS:    none
| INPUT PARAMETERS: none
| RETURN VALUES:    Xcp session state
| DESCRIPTION:      Get the session state of the XCP Protocol Layer
******************************************************************************/
FUNC(SessionStatusType, XCP_CODE) Xcp_GetSessionStatus( void )
{
	SessionStatusType result;
	Xcp_CheckDetErrorReturnValue( XCP_UNINIT != Xcp_InitializationState,
		XCP_GETSTATE_SERVICE_ID, XCP_E_NOT_INITIALIZED, (uint8)XCP_DISCONNECTED )
		result = xcp.SessionStatus;
	/* Reset the polling state */
	xcp.SessionStatus &= (SessionStatusType)(~(SessionStatusType)SS_POLLING); /* PRQA S 0277 */ /* MD_Xcp_0277 */
	return(result);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

#if defined ( XCP_ENABLE_GET_XCP_DATA_POINTER )
/*****************************************************************************
| NAME:             Xcp_GetXcpDataPointer
| CALLED BY:        Application
| PRECONDITIONS:    none
| INPUT PARAMETERS: tXcpData ** pXcpData: Pointer to Pointer that is set to xcp
| RETURN VALUES:    none
| DESCRIPTION:      Get the pointer to the internal xcp structure
******************************************************************************/
FUNC(void, XCP_CODE) Xcp_GetXcpDataPointer( P2VAR(tXcpDataPtr, AUTOMATIC, XCP_APPL_DATA) pXcpData )
{
	Xcp_CheckDetErrorReturnVoid( XCP_UNINIT != Xcp_InitializationState,
		XCP_GETXCPDATAPOINTER_SERVICE_ID, XCP_E_NOT_INITIALIZED )
		/* SBSW_AsrXcp_028 */
		Xcp_SilentBswCheckErrorReturnVoid( (P2VAR(tXcpDataPtr, AUTOMATIC, XCP_APPL_DATA))NULL_PTR != pXcpData,
		XCP_GETXCPDATAPOINTER_SERVICE_ID, XCP_E_NULL_POINTER )

		*pXcpData = &xcp;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

#if defined ( XCP_ENABLE_CONTROL )
/*****************************************************************************
| NAME:             Xcp_Control
| CALLED BY:        Application
| PRECONDITIONS:    none
| INPUT PARAMETERS: command: en- or disable the modul
| RETURN VALUES:    none
|                   none
| DESCRIPTION:      En- or disable the Xcp Protocol Layer globally
******************************************************************************/
FUNC(void, XCP_CODE) Xcp_Control( uint8 command )
{
	Xcp_CheckDetErrorReturnVoid( XCP_UNINIT != Xcp_InitializationState,
		XCP_CONTROL_SERVICE_ID, XCP_E_NOT_INITIALIZED )

		Xcp_ControlState = command;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

#if defined ( XCP_ENABLE_VERSION_INFO_API )
/**************************************************************************************************
   Function name    : Xcp_GetVersionInfo
   ------------------------------------------------------------------------------------------------
   Description      : Returns version information of module
   ------------------------------------------------------------------------------------------------
   Called by        : -
   ------------------------------------------------------------------------------------------------
   Parameter        : Pointer to location at which version information shall be stored at
   ------------------------------------------------------------------------------------------------
   Returncode       : void
   ------------------------------------------------------------------------------------------------
   Misc             : -
**************************************************************************************************/
FUNC(void, XCP_CODE) Xcp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, XCP_APPL_DATA) XcpVerInfoPtr)
{
	/* SBSW_AsrXcp_028 */
	Xcp_SilentBswCheckErrorReturnVoid( (P2VAR(Std_VersionInfoType, AUTOMATIC, XCP_APPL_DATA))NULL_PTR != XcpVerInfoPtr,
		XCP_GETVERSIONINFO_SERVICE_ID, XCP_E_NULL_POINTER )

		/* Since this service only access non-volatile data and no channel parameter is passed,
		checking of the channel handle and initialization is omitted. */
		XcpVerInfoPtr->vendorID = XCP_VENDOR_ID;
	XcpVerInfoPtr->moduleID = XCP_MODULE_ID;
	XcpVerInfoPtr->sw_major_version = (CP_XCP_VERSION >> 8u);
	XcpVerInfoPtr->sw_minor_version = (CP_XCP_VERSION & 0xff);
	XcpVerInfoPtr->sw_patch_version = CP_XCP_RELEASE_VERSION;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif /* XCP_ENABLE_VERSION_INFO_API */


/****************************************************************************/
/* Test                                                                     */
/* Some screen output functions for test and diagnostics                    */
/****************************************************************************/



#define XCP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/*******************************************************************************
  Consistency checks
*******************************************************************************/

/* Check definition of endianess of CPU */

#if defined ( XCP_CPUTYPE_LITTLEENDIAN ) || defined ( XCP_CPUTYPE_BIGENDIAN )
#else
  #error "Please define XCP_CPUTYPE_LITTLEENDIAN or XCP_CPUTYPE_BIGENDIAN."
#endif

/****************************HISTORY*****************************************/
/****************************************************************************
Date              : 11/21/2016
By                : cmurali1
Change Description: Migrated to BAIC requirements
*****************************************************************************/
