
/*****************************************************************************
*                                                                            *
*              CONFIDENTIAL VISTEON CORPORATION                              *
*                                                                            *
* This is an unpublished work of authorship, which contains trade            *
* secrets, created in 2011. Visteon Corporation owns all rights to           *
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
File Name        :  fcruise.h
Module Short Name:  Common
VOBName          :  \sw_di_int\2014_nissan_p42m_ic
Author           :  sun.liyuan
Description      :  process button state, include short press and long press
Organization     :  Driver Information Software Section,
                    Visteon Software Operation, India
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    : cosmic.v477
Target Processor :  Freescale MPC5604S/MPC5645S
******************************************************************************/

#ifndef FCRUISE_H
#define FCRUISE_H

/*============================================================================
** I N C L U D E   F I L E S
**==========================================================================*/
#include "fcruise_process.h"

/*==============================================================
** T Y P E   D E F I N I T I O N S
**=============================================================*/
#define WARNING_DISPLAY          1
#define WARNING_NOT_DISPLAY      0

typedef enum
{
   CRUISE_ICON_HIDE = 0,
   CRUISE_ICON_GRAY ,
   CRUISE_ICON_GREEN ,
}FCRUISE_TITLE_T;

typedef enum
{
   CRUISE_SPEED_NOT_DISPLAY = 0,
   CRUISE_SPEED_DISPLAY ,
   CRUISE_SPEED_FLASH_1HZ ,
}FCRUISE_SPEED_T;

/***************************************************************************/
/***************************************************************************/
extern void processButton(UI_CRUISE_CONTROL_OPTION_T buttonType);
extern void fcruise_trans_Outof_Normal (void);


#endif

/*End   of File*/

