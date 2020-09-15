
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
File Name        :  fcruise_process.h
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

#ifndef FCRUISE_PROCESS_H
#define FCRUISE_PROCESS_H

/*============================================================================
** I N C L U D E   F I L E S
**==========================================================================*/
#include "system.h"


/*==============================================================
** MACRO DEFINITIONS
**=============================================================*/


/*============================================================================
** T Y P E   D E F I N I T I O N S
**==========================================================================*/

/* BUTTON TYPES */
#define HMI_BTN_CRUISEON            (1)   /* CRUISE Button */
#define HMI_BTN_CRUISEOFF           (2)   /* CRUISE OFF Button */
#define HMI_BTN_CANCEL              (3)   /* CANCEL Button */
#define HMI_BTN_RES_ADD             (4)   /* RES/ADD Recognition Button */
#define HMI_BTN_SET_SUB             (5)   /* SET/SUB Button */


/* BUTTON EVENT TYPES */
#define HMI_BTN_OUT                 (0)   /* Out Event */
#define HMI_BTN_IN                  (1)   /* In Event */
#define HMI_BTN_SHORT               (2)   /* Short Event */
#define HMI_BTN_LONG_IN             (3)   /* Long Event */
#define HMI_BTN_LONG_OUT			(4)   /* Long Out Event */
#define HMI_BTN_SUPER_LONG_IN		(5)
#define HMI_BTN_SUPER_LONG_OUT		(6)


/* Cruise On Button Events */
#define   BTN_EV_CRUON_OUT                      ((HMI_BTN_CRUISEON << 4) + HMI_BTN_OUT)
#define   BTN_EV_CRUON_IN                       ((HMI_BTN_CRUISEON << 4) + HMI_BTN_IN)
#define   BTN_EV_CRUON_SHORT                    ((HMI_BTN_CRUISEON << 4) + HMI_BTN_SHORT)
#define   BTN_EV_CRUON_LONG_IN                  ((HMI_BTN_CRUISEON << 4) + HMI_BTN_LONG_IN)
#define   BTN_EV_CRUON_LONG_OUT                 ((HMI_BTN_CRUISEON << 4) + HMI_BTN_LONG_OUT)
#define   BTN_EV_CRUON_SUPER_LONG_OUT			((HMI_BTN_CRUISEON << 4) + HMI_BTN_SUPER_LONG_OUT)

/* Cruise Off Button Events */
#define   BTN_EV_CRUOFF_OUT                     ((HMI_BTN_CRUISEOFF << 4) + HMI_BTN_OUT)
#define   BTN_EV_CRUOFF_IN                      ((HMI_BTN_CRUISEOFF << 4) + HMI_BTN_IN)
#define   BTN_EV_CRUOFF_SHORT                   ((HMI_BTN_CRUISEOFF << 4) + HMI_BTN_SHORT)
#define   BTN_EV_CRUOFF_LONG_IN                 ((HMI_BTN_CRUISEOFF << 4) + HMI_BTN_LONG_IN)
#define   BTN_EV_CRUOFF_LONG_OUT                ((HMI_BTN_CRUISEOFF << 4) + HMI_BTN_LONG_OUT)
#define   BTN_EV_CRUOFF_SUPER_LONG_OUT			((HMI_BTN_CRUISEOFF << 4) + HMI_BTN_SUPER_LONG_OUT)

/* Cancel Button Events */
#define   BTN_EV_CANCEL_OUT                     ((HMI_BTN_CANCEL << 4) + HMI_BTN_OUT)
#define   BTN_EV_CANCEL_IN                      ((HMI_BTN_CANCEL << 4) + HMI_BTN_IN)
#define   BTN_EV_CANCEL_SHORT                   ((HMI_BTN_CANCEL << 4) + HMI_BTN_SHORT)
#define   BTN_EV_CANCEL_LONG_IN                 ((HMI_BTN_CANCEL << 4) + HMI_BTN_LONG_IN)
#define   BTN_EV_CANCEL_LONG_OUT                 ((HMI_BTN_CANCEL << 4) + HMI_BTN_LONG_OUT)
#define   BTN_EV_CANCEL_SUPER_LONG_OUT			((HMI_BTN_CANCEL << 4) + HMI_BTN_SUPER_LONG_OUT)

/* Res/+ Button Events */
#define   BTN_EV_RES_ADD_OUT                    ((HMI_BTN_RES_ADD << 4) + HMI_BTN_OUT)
#define   BTN_EV_RES_ADD_IN                     ((HMI_BTN_RES_ADD << 4) + HMI_BTN_IN)
#define   BTN_EV_RES_ADD_SHORT                  ((HMI_BTN_RES_ADD << 4) + HMI_BTN_SHORT)
#define   BTN_EV_RES_ADD_LONG_IN                ((HMI_BTN_RES_ADD << 4) + HMI_BTN_LONG_IN)
#define   BTN_EV_RES_ADD_LONG_OUT               ((HMI_BTN_RES_ADD << 4) + HMI_BTN_LONG_OUT)
#define   BTN_EV_RES_ADD_SUPER_LONG_OUT			((HMI_BTN_RES_ADD << 4) + HMI_BTN_SUPER_LONG_OUT)

/* Set/- Button Events */
#define   BTN_EV_SET_SUB_OUT                    ((HMI_BTN_SET_SUB << 4) + HMI_BTN_OUT)
#define   BTN_EV_SET_SUB_IN                     ((HMI_BTN_SET_SUB << 4) + HMI_BTN_IN)
#define   BTN_EV_SET_SUB_SHORT                  ((HMI_BTN_SET_SUB << 4) + HMI_BTN_SHORT)
#define   BTN_EV_SET_SUB_LONG_IN                ((HMI_BTN_SET_SUB << 4) + HMI_BTN_LONG_IN)
#define   BTN_EV_SET_SUB_LONG_OUT               ((HMI_BTN_SET_SUB << 4) + HMI_BTN_LONG_OUT)
#define   BTN_EV_SET_SUB_SUPER_LONG_OUT			((HMI_BTN_SET_SUB << 4) + HMI_BTN_SUPER_LONG_OUT)



#  ifndef eIGN_OFF
#   define eIGN_OFF (1U)
#  endif

#  ifndef eIGN_RUN
#   define eIGN_RUN (2U)
#  endif

#  ifndef eIGN_COUNT
#   define eIGN_COUNT (2U)
#  endif



typedef enum
{
   UI_CRUCON_OFF,
   UI_CRUCON_ON,
}UI_CRUISE_CONTROL_STATUS_T;

typedef enum
{
   UI_CRUCON_OPTION,
   UI_CRUCON_SUB_SHORT,
   UI_CRUCON_ADD_SHORT,
   UI_CRUCON_SUB_LONG,
   UI_CRUCON_ADD_LONG,
   UI_CRUCON_CANCEL,
}UI_CRUISE_CONTROL_OPTION_T;

/***************************************************************************/
/***************************************************************************/

extern void fcruise_process_KSColdInit (void);
extern void cruise_event_handler(UINT8 p_button_event_U8);
extern UI_CRUISE_CONTROL_STATUS_T get_cruise_control_status(void);
extern void button_cruise_off(void);

#endif

/*End   of File*/

