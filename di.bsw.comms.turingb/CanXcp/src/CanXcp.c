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

 
#define CANXCP_SOURCE

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "CanXcp.h"           /* Include header of XCP on CAN Transport Layer */
#if defined (CANXCP_CONFIGURATOR) && (CANXCP_CONFIGURATOR == 5)
  #include "Xcp.h"                    /* Include header of XCP Protocol Layer */
#else
  #include "Xcp.h"                /* Include header of XCP Protocol Layer */
#endif
#include "CanIf.h"                                   /* Include CAN Interface */

#if ( CANXCP_DEV_ERROR_DETECT == STD_ON )
  #include "Det.h"                        /* Include Diagnostic Event Manager */
#endif

#if defined (CANXCP_CONFIGURATOR) && (CANXCP_CONFIGURATOR == 5)
  #include "SchM_Xcp.h"
#endif

#if defined ( CANXCP_AMD_RUNTIME_MEASUREMENT ) && ( CANXCP_AMD_RUNTIME_MEASUREMENT == STD_ON )
  #include "AmdRtm.h"
#endif


/******************************************************************************/
/* Version check                                                              */
/******************************************************************************/
#if ( CP_XCPONCANASR_VERSION != 0x0110u )
  #error "Source and Header file are inconsistent!"
#endif
#if ( CP_XCPONCANASR_RELEASE_VERSION != 0x00u )
  #error "Source and Header file are inconsistent!"
#endif

#if ( CANXCP_TPS_MAJOR_VERSION != 0x01u )
# error "Source and Header file CanXcp_Types.h are inconsistent (CANXCP_TPS_MAJOR_VERSION)!"
#endif
#if ( CANXCP_TPS_MINOR_VERSION != 0x10u )
# error "Source and Header file CanXcp_Types.h are inconsistent (CANXCP_TPS_MINOR_VERSION)!"
#endif
#if ( CANXCP_TPS_PATCH_VERSION != 0x00u )
# error "Source and Header file CanXcp_Types.h are inconsistent (CANXCP_TPS_PATCH_VERSION)!"
#endif

#if defined (CANXCP_AUTOSARVERSION) && (CANXCP_AUTOSARVERSION == 4)
  #if defined ( CANIF_VENDOR_ID )
  # if (CANIF_VENDOR_ID == CANXCP_VENDOR_ID)
  #   if defined(CANIF_AR_MAJOR_VERSION) && defined(CANIF_AR_MINOR_VERSION)
  #     if((CANIF_AR_MAJOR_VERSION != 0x04u) || (CANIF_AR_MINOR_VERSION != 0x00u))
  #      error "Imported module has incompatible AR version - expected CANIF version is 0x04u.0x00u \
                                                        (CANIF_AR_MAJOR_VERSION.CANIF_AR_MINOR_VERSION)."
  #     endif
  #   endif
  # endif
  #endif
#else
  #if defined ( CANIF_VENDOR_ID )
  # if (CANIF_VENDOR_ID == CANXCP_VENDOR_ID)
  #   if defined(CANIF_AR_MAJOR_VERSION) && defined(CANIF_AR_MINOR_VERSION)
  #     if((CANIF_AR_MAJOR_VERSION != 0x03u) || (CANIF_AR_MINOR_VERSION != 0x00u))
  #      error "Imported module has incompatible AR version - expected CANIF version is 0x03u.0x00u \
                                                        (CANIF_AR_MAJOR_VERSION.CANIF_AR_MINOR_VERSION)."
  #     endif
  #   endif
  # endif
  #endif
#endif

/******************************************************************************/
/* Defintions                                                                 */
/******************************************************************************/
/* States of CanXcp_SendWithoutQueueState */
#define CANXCP_MSG_IDLE           0
#define CANXCP_MSG_PENDING        1

/* PRQA S 3453 27 */ /*  MD_MSR_19.7 */
#if defined ( CANXCP_MULTI_CHANNEL_ENABLED )
  #define CanXcp_GetTxPduId()     CanXcp_TxPduId
  #define CanXcp_GetRxPduId()     CanXcp_RxPduId
  #if ( CanXcp_IsPostbuild() )
    #define CanXcp_GetTxPduIdIdx(idx)     CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].CanXcpTxPduId)
    #define CanXcp_GetRxPduIdIdx(idx)     CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[idx].CanXcpRxPduId)
    #define CanXcp_GetChHandle(NwH)       CanXcp_ConfigurationAccess(CanXcp_NwHMappingPtr[NwH])
  #else
    #define CanXcp_GetTxPduIdIdx(idx)     CanXcp_PduIdField[idx].CanXcpTxPduId
    #define CanXcp_GetRxPduIdIdx(idx)     CanXcp_PduIdField[idx].CanXcpRxPduId
    #define CanXcp_GetChHandle(NwH)       CanXcp_NwHMapping[NwH]
  #endif
#else
  #if ( CanXcp_IsPostbuild() )
    #define CanXcp_GetTxPduId()     CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[0].CanXcpTxPduId)
    #define CanXcp_GetRxPduId()     CanXcp_ConfigurationAccess(CanXcp_XcpPduPtr[0].CanXcpRxPduId)
    #define CanXcp_GetChHandle(NwH) CanXcp_ConfigurationAccess(CanXcp_NwHMappingPtr[NwH])
  #else
    #define CanXcp_GetTxPduId()     CanXcp_PduIdField[0].CanXcpTxPduId
    #define CanXcp_GetRxPduId()     CanXcp_PduIdField[0].CanXcpRxPduId
    #define CanXcp_GetChHandle(NwH) CanXcp_NwHMapping[NwH]
  #endif
#endif

/******************************************************************************/
/* Macros                                                                     */
/******************************************************************************/
/***************************************************************************************************/
/* General definition of checking macros */
# define CanXcp_CheckErrorReturnVoid(cond, api, err)        \
{ if(!(cond)){                                           \
    CanXcp_CallDetReportError((api), (err));                \
    return;                                              \
}} /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define CanXcp_CheckErrorReturnValue(cond, api, err, ret)  \
{ if(!(cond)){                                           \
    CanXcp_CallDetReportError((api), (err));                \
}} /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define CanXcp_CheckErrorContinue(cond, api, err)          \
{ if(!(cond)){                                           \
    CanXcp_CallDetReportError((api), (err));                \
}} /* PRQA S 3458 */ /* MD_MSR_19.4 */

/***************************************************************************************************/
/* Macros for DET abstraction */
#if(CANXCP_DEV_ERROR_DETECT == STD_ON)
  # define CanXcp_CheckDetErrorReturnVoid(cond, api, err)       CanXcp_CheckErrorReturnVoid((cond), (api), (err)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  # define CanXcp_CheckDetErrorReturnValue(cond, api, err, ret) CanXcp_CheckErrorReturnValue((cond), (api), (err), (ret)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  # define CanXcp_CheckDetErrorContinue(cond, api, err)         CanXcp_CheckErrorContinue((cond), (api), (err)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  # define CanXcp_CallDetReportError(api, err)                  (void)Det_ReportError(CANXCP_MODULE_ID, 0u, (api), (err)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
  # define CanXcp_CheckDetErrorReturnVoid(cond, api, err) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  # define CanXcp_CheckDetErrorReturnValue(cond, api, err, ret) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  # define CanXcp_CheckDetErrorContinue(cond, api, err) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  # define CanXcp_CallDetReportError(api, err) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

/***************************************************************************************************/
/* Macros for SBSW check abstraction */
#if(CANXCP_SILENT_CHECK == STD_ON)
  # define CanXcp_SilentBswCheckErrorReturnVoid(cond, api, err)       CanXcp_CheckErrorReturnVoid( (cond), (api), (err)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  # define CanXcp_SilentBswCheckErrorReturnValue(cond, api, err, ret) CanXcp_CheckErrorReturnValue( (cond), (api), (err), (ret)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
  # define CanXcp_SilentBswCheckErrorReturnVoid(cond, api, err)                \
  CanXcp_CheckDetErrorReturnVoid( (cond), (api), (err) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
  # define CanXcp_SilentBswCheckErrorReturnValue(cond, api, err, ret)          \
  CanXcp_CheckDetErrorReturnValue( (cond), (api), (err), (ret) ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif 


/* Activation control macros */
/* PRQA S 3458 7 */ /* MD_MSR_19.4 */
#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  #define CanXcp_CheckControlState( )              { if ( CanXcp_ControlState == (uint8)kXcponCan_Control_Disable ) { return; } }
  #define CanXcp_CheckControlStateRet( ret_value ) { if ( CanXcp_ControlState == (uint8)kXcponCan_Control_Disable ) { return( ret_value ); } }
#else
  #define CanXcp_CheckControlState( )
  #define CanXcp_CheckControlStateRet( ret_value )
#endif


/*******************************************************************************
 * Types
*******************************************************************************/
#define CANXCP_MAX(a, b)   (((a) > (b)) ? (a) : (b)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

typedef union { /* PRQA S 0750 */ /* MD_MSR_18.4 */
  uint8  b[CANXCP_MAX(kCanXcpMaxCTO, kCanXcpMaxDTO)];
  uint16 w[CANXCP_MAX(kCanXcpMaxCTO, kCanXcpMaxDTO)/2];
  uint32 dw[CANXCP_MAX(kCanXcpMaxCTO, kCanXcpMaxDTO)/4];
} tCanXcpXto; /* PRQA S 3448 */ /* MD_CanXcp_3448 */

/* Define AUTOSAR types if not defined yet */
#if defined ( STATIC )
#else
  #define STATIC static
#endif

/*******************************************************************************
 * Data definitions
*******************************************************************************/
/* 
  State of CAN message, required to send without any queue.
  (requested to send via CanXcp_Send).
  These variables are initialized inCanXcp_Init().
*/
#define CANXCP_START_SEC_VAR_NOINIT_8BIT
//#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC VAR(uint8, CANXCP_VAR_NOINIT) CanXcp_SendWithoutQueueState;
#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
STATIC VAR(uint8, CANXCP_VAR_NOINIT) CanXcp_ChannelIdx;
#endif

#if ( CANXCP_DEV_ERROR_DETECT == STD_ON )
STATIC volatile VAR(uint8, CANXCP_VAR_NOINIT) CanXcp_InitializationState;
#endif

/* TL Enable/Disable state */
#if ( CANXCP_ENABLE_CONTROL == STD_ON )
STATIC VAR(uint8, CANXCP_VAR_NOINIT) CanXcp_ControlState;
#endif

#define CANXCP_STOP_SEC_VAR_NOINIT_8BIT
//#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#define CANXCP_START_SEC_VAR_NOINIT_UNSPECIFIED
//#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC VAR(PduInfoType, CANXCP_VAR_NOINIT) CanXcp_QueuedPduInfo;
#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
STATIC VAR(CanXcp_PduSetModeType, CANXCP_VAR_NOINIT) CanXcp_PduState[CANXCP_NUMBER_OF_CHANNELS];
#endif
STATIC VAR(CanXcp_TimeoutTimerType, CANXCP_VAR_NOINIT) CanXcp_ConfTimeoutTimer;

STATIC tCanXcpXto CanXcp_PduBuffer; /* PRQA S 0759 */ /* MD_MSR_18.4 */ /* PRQA S 3218 */ /* MD_CanXcp_3218 */

#define CANXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
//#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


#if defined ( CANXCP_MULTI_CHANNEL_ENABLED )
/* global variables */
  #define CANXCP_START_SEC_VAR_NOINIT_UNSPECIFIED
//  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
STATIC VAR(PduIdType, CANXCP_VAR_NOINIT) CanXcp_RxPduId;
STATIC VAR(PduIdType, CANXCP_VAR_NOINIT) CanXcp_TxPduId;
  #define CANXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
//  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

#if ( CanXcp_IsPostbuild() )
  #define CANXCP_START_SEC_VAR_NOINIT_UNSPECIFIED
//  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
STATIC P2CONST(CanXcp_ConfigType, CANXCP_VAR_NOINIT, CANXCP_PBCFG) CanXcp_Configuration;
  #define CANXCP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
//  #include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
# define CanXcp_ConfigurationAccess(a)  (CanXcp_Configuration->a) /* PRQA S 3410 */ /* MD_CanXcp_3410 */ /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define CanXcp_ConfigurationAccess(a)  (CanXcp_Config.a) /* PRQA S 3410 */ /* MD_CanXcp_3410 */ /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

/****************************************************************************/
/* External ROM Data                                                        */
/****************************************************************************/

#define CANXCP_START_SEC_CONST_8BIT
//#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Global constants with XCP Transport Layer for CAN main and subversion */
CONST(uint8, CANXCP_CONST) kXcpOnCanAsrMainVersion    = (uint8)(CP_XCPONCANASR_VERSION >> 8);
CONST(uint8, CANXCP_CONST) kXcpOnCanSubAsrVersion     = (uint8)(CP_XCPONCANASR_VERSION & 0x00ff);
CONST(uint8, CANXCP_CONST) kXcpOnCanAsrReleaseVersion = (uint8)(CP_XCPONCANASR_RELEASE_VERSION);

#define CANXCP_STOP_SEC_CONST_8BIT
//#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/******************************************************************************
*
*
*           Code Part for Cp_XcpOnCanAsr
*
*
*******************************************************************************/

#define CANXCP_START_SEC_CODE
//#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/****************************************************************************
| NAME:             CanXcp_InitMemory()
| SID:              none
| CALLED BY:        Upper layer
| PRECONDITIONS:    
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      Initializes global variables. This function has to be called
|                   before any other calls to the CanXcp API
****************************************************************************/
FUNC(void, CANXCP_CODE) CanXcp_InitMemory
(
  void
)
{
  /* Initialize this variable here in case the Init function is not called */
  CanXcp_SendWithoutQueueState = (uint8)CANXCP_MSG_IDLE;
#if ( CANXCP_DEV_ERROR_DETECT == STD_ON )
  CanXcp_InitializationState = CANXCP_UNINIT;
#endif
}

#if ( CANXCP_VERSION_INFO_API == STD_ON )
/****************************************************************************
| NAME:             CanXcp_GetVersionInfo()
| SID:              0x0A
| CALLED BY:        Upper layer
| PRECONDITIONS:
| INPUT PARAMETERS: versioninfo - buffer to copy version info to
| RETURN VALUES:    Std_ReturnType
| DESCRIPTION:      Provide DLC and data of the specified Pdu to upper layers
****************************************************************************/
FUNC(void, CANXCP_CODE) CanXcp_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CANXCP_APPL_VAR) Versioninfo
)
{
  CanXcp_SilentBswCheckErrorReturnVoid( Versioninfo != (P2VAR(Std_VersionInfoType, AUTOMATIC, CANXCP_APPL_VAR))NULL_PTR,
                                        CANXCP_GETVERSIONINFO_ID, CANXCP_E_NULL_POINTER )

  Versioninfo->vendorID         = CANXCP_VENDOR_ID;
  Versioninfo->moduleID         = CANXCP_MODULE_ID;
  #if ( defined CANXCP_AUTOSARVERSION ) && ( CANXCP_AUTOSARVERSION == 4 )
  #else
  Versioninfo->instanceID       = 0x00u;
  #endif
  Versioninfo->sw_major_version = CANXCP_SW_MAJOR_VERSION;
  Versioninfo->sw_minor_version = CANXCP_SW_MINOR_VERSION;
  Versioninfo->sw_patch_version = CANXCP_SW_PATCH_VERSION;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif

/*******************************************************************************
** Function:    CanXcp_Init
**
** Purpose:     Initializations of the XCP on CAN Transport Layer.
**
**  Called by:  Xcp_SendCrm, Xcp_SendEv, Xcp_SendDto
**
**  Parameter:  - 
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CANXCP_CODE) CanXcp_Init
(
  P2CONST(CanXcp_ConfigType, AUTOMATIC, CANXCP_PBCFG) ConfigPtr
)
{
#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
  uint8_least i;
#endif
#if ( CanXcp_IsPostbuild() )
  CanXcp_Configuration = ConfigPtr;
#else
  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  /* avoid compiler warning due to unused parameters */
  ConfigPtr            = ConfigPtr;
  #endif
#endif

#if ( CANXCP_DEV_ERROR_DETECT == STD_ON )
  CanXcp_InitializationState = CANXCP_UNINIT;
#endif

  /* Initialize variables for transmisson without any queue. */
  CanXcp_SendWithoutQueueState    = (uint8)CANXCP_MSG_IDLE;
  CanXcp_QueuedPduInfo.SduDataPtr = (P2VAR(uint8, AUTOMATIC, CANXCP_APPL_VAR))NULL_PTR;
  CanXcp_QueuedPduInfo.SduLength  = (PduLengthType)8u;

#if defined ( CANXCP_MULTI_CHANNEL_ENABLED )
  /* Initialize static variables. */
  CanXcp_TxPduId = CanXcp_GetTxPduIdIdx(0);
  CanXcp_RxPduId = CanXcp_GetRxPduIdIdx(0);
#endif
  CanXcp_ConfTimeoutTimer = (CanXcp_TimeoutTimerType)0u;

  /* Set all channels to offline by default */
#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
  CanXcp_ChannelIdx = (uint8)0u;
  for(i=0; i < CANXCP_NUMBER_OF_CHANNELS; i++)
  {
    CanXcp_PduState[i] = CANXCP_SET_OFFLINE;
  }
#endif

#if ( CANXCP_ENABLE_CONTROL == STD_ON )
  CanXcp_ControlState = (uint8)kXcponCan_Control_Enable;
#endif

#if ( CANXCP_DEV_ERROR_DETECT == STD_ON )
  CanXcp_InitializationState = CANXCP_INIT;
#endif
}

/*******************************************************************************
** Function:    CanXcp_MainFunction
**
** Purpose:     Main function of XCP on CAN Transport Layer
**
**  Called by:  BSW Scheduler, OS
**
**  Parameter:  - 
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CANXCP_CODE) CanXcp_MainFunction
(
  void
)
{
  /* Activation control */
  CanXcp_CheckControlState()

#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
  if ( CANXCP_SET_ONLINE == CanXcp_PduState[CanXcp_ChannelIdx] )
#endif
  {
    /* If a CAN message is pending, try to send this */
    if ( CanXcp_SendWithoutQueueState == (uint8)CANXCP_MSG_PENDING )
    {
      /* Try to transmit the XCP Packet */
      if ( CanIf_Transmit(0, CanXcp_GetTxPduId(), &CanXcp_QueuedPduInfo ) == (Std_ReturnType)E_OK )
      {
        CanXcp_ConfTimeoutTimer = (CanXcp_TimeoutTimerType)CANXCP_CONF_TIMEOUT;
        /* The message was sent successfully. Clear the 'message pending' bit. */
        CanXcp_SendWithoutQueueState = (uint8)CANXCP_MSG_IDLE;
      }
      else
      {
        /* The message is still pending. */
      }
    }
  }

  CanXcp_EnterCritical();
  if( CanXcp_ConfTimeoutTimer > 0u)
  {
    CanXcp_ConfTimeoutTimer--;
    CanXcp_LeaveCritical();
    if( 0u == CanXcp_ConfTimeoutTimer )
    {
      /* Timeout happened, release PDU */
      (void)Xcp_SendCallBack();
    }
  }
  else
  {
    CanXcp_LeaveCritical();
  }

} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*******************************************************************************
** Function:    CanXcp_RxIndication
**
** Purpose:     Rx Indication for reception of CTO and DTO Packets
**              This function is configured in the generation tool.
**
**  Called by:  CanIf
**
**  Parameter:  
**              CanCanXcpRxPduId : Target PDU ID of CAN L-PDU that has been received.
**              CanSduPtr        : Pointer to received L-SDU (payload).
**
**  Returncode: -
*******************************************************************************/
//#if defined (CANXCP_AUTOSARVERSION) && (CANXCP_AUTOSARVERSION == 4)
//FUNC(void, CANXCP_CODE) Xcp_CanIfRxIndication
//(
//  VAR(PduIdType, AUTOMATIC) CanCanXcpRxPduId,
//  P2CONST(PduInfoType, AUTOMATIC, CANXCP_APPL_VAR) PduInfoPtr
//)
//#else
#if ( CANXCP_USE_PDUINFOTYPE == STD_ON )
FUNC(void, CANXCP_CODE) CanXcp_RxIndication
(
  CAN_HWINST const hwInst,
  VAR(PduIdType, AUTOMATIC) CanCanXcpRxPduId,
  CAN_RMD const * const pRmd
)
#else
FUNC(void, CANXCP_CODE) CanXcp_RxIndication
(
  VAR(PduIdType, AUTOMATIC) CanCanXcpRxPduId,
  P2CONST(uint8, AUTOMATIC, CANXCP_APPL_VAR) CanSduPtr
)
#endif
//#endif
{
  uint8_least i;
#if ( CANXCP_USE_PDUINFOTYPE == STD_ON )
  uint8 *CanSduPtr;
#endif

  CanXcp_CheckDetErrorReturnVoid( CANXCP_UNINIT != CanXcp_InitializationState,
                                  CANXCP_RXINDICATION_SERVICE_ID, CANXCP_E_NOT_INITIALIZED )

#if ( CANXCP_USE_PDUINFOTYPE == STD_ON )
  CanXcp_SilentBswCheckErrorReturnVoid( pRmd != (P2CONST(CAN_RMD, AUTOMATIC, CANXCP_APPL_VAR))NULL_PTR,
                                        CANXCP_RXINDICATION_SERVICE_ID, CANXCP_E_NULL_POINTER )
  CanSduPtr = (CAN_UINT8 *)pRmd->Data;
#else
  CanXcp_SilentBswCheckErrorReturnVoid( CanSduPtr != (P2CONST(uint8, AUTOMATIC, CANXCP_APPL_VAR))NULL_PTR,
                                        CANXCP_RXINDICATION_SERVICE_ID, CANXCP_E_NULL_POINTER )
#endif

  /* Activation control */
  CanXcp_CheckControlState()

#if defined ( CANXCP_MULTI_CHANNEL_ENABLED )
  if ( CanXcp_RxPduId != CanCanXcpRxPduId )
  {
  #if defined ( CANXCP_MULTI_CONN_PROT_ENABLED )
    /* Ensure that no XCP connection is interrupt via another channel. */
    if ( 0 != (Xcp_GetSessionStatus() & (SessionStatusType)SS_CONNECTED) )
    {
      /* Do not transmit an error packet due to XcpConfirmation() will be called on the wrong channel. */
      return;
    }
    else
  #endif
    {
      /* Store the Rx PDU Id for the reception. */
      CanXcp_RxPduId = CanCanXcpRxPduId;

      /* Store the Tx PDU Id for the response. */
      i = 0;
      while ( (i < CANXCP_NUMBER_OF_CHANNELS) && (CanXcp_GetRxPduIdIdx(i) != CanCanXcpRxPduId) )
      {
        i++;
      }
      CanXcp_TxPduId = CanXcp_GetTxPduIdIdx(i); /* PRQA S 3689 */ /* MD_CanXcp_3689 */
      #if ( CANXCP_ENABLE_PDUMODE == STD_ON )
        CanXcp_ChannelIdx = (uint8)i;
      #endif
    }
  }
#else
  /* Verifiy RX Pdu Id */
  if(CanCanXcpRxPduId == CanXcp_GetRxPduId())
  {
#endif

    if(0xff == CanSduPtr[0])
    {
      /* In case of connect we set this as active TL */
      Xcp_SetActiveTl(kCanXcpMaxCTO, kCanXcpMaxDTO, XCP_TRANSPORT_LAYER_CAN);
    }

    if(XCP_TRANSPORT_LAYER_CAN == Xcp_GetActiveTl())
    {
      /*
        A XCP Packet has been received.
        Copy the whole received SDU to an 32-Bit aligned buffer.
      */
      for(i=0; i<CANXCP_MAX(kCanXcpMaxCTO, kCanXcpMaxDTO); i++)
      {
        CanXcp_PduBuffer.b[i] = CanSduPtr[i];
      }
      /* Execution of Command Interpreter (evaluation of the XCP Packet) */
      Xcp_Command( CanXcp_PduBuffer.dw );
    }
#if defined ( CANXCP_MULTI_CHANNEL_ENABLED )
#else
  }
#endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */ /* PRQA S 6010 */ /* MD_MSR_STPTH */

/*******************************************************************************
** Function:    CanXcp_TxConfirmation
**
** Purpose:     Tx Confirmation for successful transmission of CTO and DTO Packets
**              This function is configured in the generation tool.
**
**  Called by:  CanIf
**
**  Parameter:  
**              CanCanXcpTxPduId : PDU ID of CAN L-PDU transmitted successfully.
**
**  Returncode: -
*******************************************************************************/
//#if defined (CANXCP_AUTOSARVERSION) && (CANXCP_AUTOSARVERSION == 4)
//FUNC(void, CANXCP_CODE) Xcp_CanIfTxConfirmation( VAR(PduIdType, AUTOMATIC) CanTxPduId )
//#else
FUNC(void, CANXCP_CODE) CanXcp_TxConfirmation(CAN_HWINST const hwInst, VAR(PduIdType, AUTOMATIC) CanTxPduId )
//#endif
{
#if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  /* avoid compiler warning due to unused parameters */
  CanTxPduId = CanTxPduId;
#endif

  CanXcp_CheckDetErrorReturnVoid( CANXCP_UNINIT != CanXcp_InitializationState,
                                  CANXCP_TXCONFIRMATION_SERVICE_ID, CANXCP_E_NOT_INITIALIZED )

  /* Activation control */
  CanXcp_CheckControlState()

  CanXcp_ConfTimeoutTimer = (CanXcp_TimeoutTimerType)0u;
  /*
    Notify the XCP protocol driver of the successful transmission of a message.
    The XCP driver will not call ApplXcpSend() again, until Xcp_SendCallBack has
    indicated the successful transmission of the previous message.
  */
  (void)Xcp_SendCallBack();
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*******************************************************************************
** Function:    CanXcp_Send
**
** Purpose:     Request for the transmission of a DTO or CTO message
**
**  Called by:  Xcp_SendCrm, Xcp_SendEv, Xcp_SendDto
**
**  Parameter:  
**              len : length of PDU data
**              msg : pointer to PDU data
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CANXCP_CODE) CanXcp_Send
(
  VAR(uint8, AUTOMATIC) len,
  P2CONST(uint8, AUTOMATIC, CANXCP_APPL_VAR) msg
) /* ESCAN00071289 */
{
  PduInfoType PduInfo;

  CanXcp_CheckDetErrorReturnVoid( CANXCP_UNINIT != CanXcp_InitializationState,
                                  CANXCP_SEND_ID, CANXCP_E_NOT_INITIALIZED )
  CanXcp_SilentBswCheckErrorReturnVoid( msg != (P2CONST(uint8, AUTOMATIC, CANXCP_APPL_VAR))NULL_PTR,
                                        CANXCP_SEND_ID, CANXCP_E_NULL_POINTER )
  /* Activation control */
  CanXcp_CheckControlState()

  PduInfo.SduDataPtr = (P2VAR(uint8, AUTOMATIC, CANXCP_APPL_VAR))msg; /* PRQA S 0311 */ /* MD_CanXcp_0311 */
#if defined ( CANXCP_VARIABLE_DLC_ENABLED )
  PduInfo.SduLength  = (PduLengthType)len;
#else
  PduInfo.SduLength  = (PduLengthType)CANXCP_PDU_SIZE;
  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  /* avoid compiler warning due to unused parameters */
  len = len;
  #endif
#endif

  /* Transmit the XCP Packet */
#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
  if ( CANXCP_SET_ONLINE == CanXcp_PduState[CanXcp_ChannelIdx] )
  {
#endif
    if ( CanIf_Transmit( 0, CanXcp_GetTxPduId(), &PduInfo ) != (Std_ReturnType)E_OK )
    {
#if ( CANIF_TRANSMIT_BUFFER == STD_ON )

      /* Fatal Error. XCP Packet cannot be transmitted. */
      Xcp_Init();

#else /* if ( CANIF_TRANSMIT_BUFFER == STD_ON ) */
      /*
        Transmission via CanIf_Transmit has failed.
        Set a flag in order to re-start transmission of the XCP Packet in XcpCan_MainFunction
      */
      CanXcp_SendWithoutQueueState = (uint8)CANXCP_MSG_PENDING;
      CanXcp_QueuedPduInfo         = PduInfo;
#endif /* if ( CANIF_TRANSMIT_BUFFER == STD_ON ) */
    }
    else
    {
      CanXcp_ConfTimeoutTimer = (CanXcp_TimeoutTimerType)CANXCP_CONF_TIMEOUT;
    }
#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
  }
  else
  {
    CanXcp_SendWithoutQueueState = (uint8)CANXCP_MSG_PENDING;
    CanXcp_QueuedPduInfo         = PduInfo;
  }
#endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*******************************************************************************
** Function:    CanXcp_SendFlush
**
** Purpose:     Flush function of XCP on CAN Transport Layer
**
**  Called by:  Upper Layer
**
**  Parameter:  FlushType: 
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CANXCP_CODE) CanXcp_SendFlush( uint8 FlushType )
{
  CanXcp_CheckDetErrorReturnVoid( CANXCP_UNINIT != CanXcp_InitializationState,
                                  CANXCP_SENDFLUSH_ID, CANXCP_E_NOT_INITIALIZED )

  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  /* avoid compiler warning due to unused parameters */
  FlushType = FlushType;
  #endif
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

/*******************************************************************************
** Function:    CanXcp_TLService
**
** Purpose:     CAN Transport Layer Command
**
**  Called by:  Upper Layer
**
**  Parameter:  pCmd: pointer to cmd string
**
**  Returncode: -
*******************************************************************************/
FUNC(uint8, CANXCP_CODE) CanXcp_TLService( P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) pCmd )
{
  CanXcp_CheckDetErrorReturnValue( CANXCP_UNINIT != CanXcp_InitializationState,
                                   CANXCP_TLSERVICE_ID, CANXCP_E_NOT_INITIALIZED, XCP_CMD_UNKNOWN )

  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  /* avoid compiler warning due to unused parameters */
  pCmd = pCmd;
  #endif
  return(XCP_CMD_UNKNOWN);
} /* PRQA S 2006 */ /* MD_MSR_14.7 */

#if ( CANXCP_ENABLE_CONTROL == STD_ON )
/*******************************************************************************
** Function:    CanXcp_Control
**
** Purpose:     Enable or disable the component
**
**  Called by:  user
**
**  Parameter:  command : enable/disable
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CANXCP_CODE) CanXcp_Control( VAR(uint8, AUTOMATIC) command )
{
  CanXcp_CheckDetErrorReturnVoid( CANXCP_UNINIT != CanXcp_InitializationState,
                                  CANXCP_CONTROL_ID, CANXCP_E_NOT_INITIALIZED )

#if ( CANXCP_DEV_ERROR_DETECT == STD_ON )
  if( ((uint8)kXcponCan_Control_Disable != command) && ((uint8)kXcponCan_Control_Enable != command) )
  {
    (void)Det_ReportError( CANXCP_MODULE_ID, 0, CANXCP_CONTROL_ID, CANXCP_E_WRONG_CONTROL_COMMAND); /* PRQA S 3458 */ /* MD_MSR_19.4 */
    return;
  }
#endif

  CanXcp_ControlState = command;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif


#if ( CANXCP_ENABLE_PDUMODE == STD_ON )
/*******************************************************************************
** Function:    CanXcp_SetPduMode
**
** Purpose:     Enable or disable the component
**
**  Called by:  State Manager
**
**  Parameter:  command : enable/disable
**
**  Returncode: -
*******************************************************************************/
FUNC(void, CANXCP_CODE) CanXcp_SetPduMode( NetworkHandleType XcpNwH, CanXcp_PduSetModeType PduMode )
{
  CanXcp_CheckDetErrorReturnVoid( CANXCP_UNINIT != CanXcp_InitializationState,
                                  CANXCP_SETPDUMODE_ID, CANXCP_E_NOT_INITIALIZED )

  CanXcp_SilentBswCheckErrorReturnVoid(XcpNwH < CANXCP_NUMBER_OF_CHANNELS, CANXCP_SETPDUMODE_ID,
                                       CANXCP_E_INVALID_NW_HANDLE);

  CanXcp_PduState[XcpNwH] = PduMode;
} /* PRQA S 2006 */ /* MD_MSR_14.7 */
#endif



#define CANXCP_STOP_SEC_CODE
//#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*******************************************************************************
  Consistency checks
*******************************************************************************/

/* Check configuration of Transport Layer */
#if defined ( XCP_TRANSPORT_LAYER_TYPE_CAN_ASR )
#else
  #error "Configuration for XCP Transport Layer for AUTOSAR CAN Interface doesn't exist!"
#endif

/* Check range of kXcpMaxCTO and kXcpMaxDTO */
#if ( kCanXcpMaxCTO <= CANXCP_PDU_SIZE )
#else
  #error "XCP consistency error: MAX_CTO must be less or equal to the PDU size on CAN."
#endif
#if ( kCanXcpMaxDTO <= CANXCP_PDU_SIZE )
#else
  #error "XCP consistency error: MAX_DTO must be less or equal to the PDU size on CAN."
#endif

/****************************HISTORY*****************************************/
/****************************************************************************
Date              : 11/21/2016
By                : cmurali1
Change Description: Migrated to BAIC requirements
*****************************************************************************/

