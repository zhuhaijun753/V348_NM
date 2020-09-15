#ifndef PEPS_WRN_CFG_H
#define PEPS_WRN_CFG_H
/*****************************************************************************
*                                 Constant Declarations                      *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the constant.                                                        *
* purpose, unit, and resolution                                              *
******************************************************************************/

/*****************************************************************************
*                                 Global Macro Definitions                   *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/

/*****************************************************************************
*                                 Type Decleration                           *
*----------------------------------------------------------------------------*
* Decleration of type shall be accompanied by a comment that explains the    *
* purpose and usage of the type.                                             *
******************************************************************************/
#define cPEPS_WARNING_VALUE_0                          (uint8)(0x0)
#define cPEPS_WARNING_ACTIVE                           (uint8)(0x1)
#define cPEPS_IMMO_MATCH_WARNING_ACTIVE                (uint8)(0x2)
#define cTIMEOUT_10S                                   (uint32)10000

typedef enum 
{
  WARN_PEPS_ESCL_FAILSTATUS            ,
  WARN_PEPS_ESCL_FAILSTATUS_ACTION     ,
  WARN_PEPS_IMMO_MATCH_STATUS          ,
  WARN_PEPS_NO_KEY_FOUND_WARNING       ,
  WARN_PEPS_KEY_OUT_WARNING            ,
  WARN_PEPS_KEY_BATTERY_WARNING        ,
  WARN_PEPS_BRAKE_INDICATION_WARNING   ,
  WARN_PEPS_BRAKE_INDICATION_WARNING_ACTION,
  WARN_PEPS_NOT_N_WARNING              ,
  WARN_PEPS_SSB_FAILSTATUS             ,
  WARN_PEPS_SSB_FAILSTATUS_ACTION      ,
  WARN_PEPS_CLUTCH_INDICATION_WARNING  ,
  WARN_PEPS_CLUTCH_INDICATION_WARNING_ACTION,
  PEPS_WARNING_TOTAL
}UI_WARNING_ID_T;


#endif


