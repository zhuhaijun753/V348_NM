/*=============================================================================
**              CONFIDENTIAL VISTEON CORPORATION
**
** This is an unpublished work of authorship, which contains trade secrets,
** created in 2000. Visteon Corporation owns all rights to this work and
** intends to maintain it in confidence to preserve its trade secret status.
** Visteon Corporation reserves the right, under the copyright laws of the
** United States or those of any other country that may have  jurisdiction,
** to protect this work as an unpublished work, in the event of an
** inadvertent or deliberate unauthorized publication. Visteon Corporation
** also reserves its rights under all copyright laws to protect this work as
** a published work, when appropriate. Those having access to this work may
** not copy it, use it, modify it or disclose the information contained in
** it without the written authorization of Visteon Corporation
**
**=============================================================================
**  Organization: Driver Information Software Section,
**                DI Core Engineering Department
**
**============================================================================
============================================================================
** T Y P E   D E F I N I T I O N S
**==========================================================================*/

extern boolean CTtMdl_Ignoff_Tt_State(void);

/*
** TELLTALE ATTRIBUTE MACROS...
** DO NOT TOUCH THESE.
** ATTRIBUTE BYTE 1
*/
extern uint8 VeCONFIG_ESPorABS;
extern uint8 VeCONFIG_SDM;
extern uint8 VeCONFIG_ParkingBrakeType;
extern uint8 VeCONFIG_TPMSFailure;


#define cTT_PROVEOUT     	(0x80) /* Proveout Bit, if set in attributes, the tt will proveout.*/
#define cTT_OFF          	(0x40) /* OFF Bit, if set in attributes, the tt will be enabled in OFF.*/
#define cTT_RUN          	(0x20) /* RUN Bit, if set in attributes, the tt will be enabled in RUN.*/
#define cTT_ACC          	(0x10) /* ACC Bit, if set in attributes, the tt will be enabled in ACC.*/
#define cTT_CHECK        	(0x08) /* CHECK Bit, if set in attributes, the tt will be enabled in CHECK.*/
#define cTT_LVI          	(0x04) /* LVI Bit, if set in attrib., the tt will be enabled in low voltage.*/
#define cTT_HVI          	(0x02) /* HVI Bit, if set in attrib., the tt will be enabled in high voltage.*/
#define cTT_CONFIGURED_OFF	(0x01) /* Configured OFF Bit, if set in attrib., the tt will not be enabled.*/
#define cTT_ZERO            (0) /* Null value*/

#define cP   cTT_PROVEOUT
#define cO   cTT_OFF
#define cR   cTT_RUN
#define cA   cTT_ACC
#define cC   cTT_CHECK
#define cL   cTT_LVI
#define cH   cTT_HVI
#define cCO  cTT_CONFIGURED_OFF
#define cZ   cTT_ZERO

typedef struct
{
  uint8 read_NV_Value;
  uint8 Enable_Value;
} type_led_proveout;
/*============================================================================
**============================================================================
**  P V C S    R E V I S I O N    N O T E S
**----------------------------------------------------------------------------
**  For each change to this file, be sure to record:
**  1.  who made the change and when the change was made
**  2.  why the change was made and the intended result
**
**  Date         By      Reason for Change
**  mmm/dd/yy    XXX     -----------------
**----------------------------------------------------------------------------
**---------------------------------------------------------------------------
Date              : 08-September-2016
CDSID             : dthangav
Traceability      : RTC Story 669892
Change Description: Macro definitions related to TtMdl is updated under this file.
**-----------------------------------------------------------------------------*/
