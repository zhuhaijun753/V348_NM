				
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
File Name        :  fcruise_process.c
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
#ifndef FCRUISE_PROCESS_C
#define FCRUISE_PROCESS_C

/*****************************************************************************
*                                 System Includes                            *
******************************************************************************/
//#include "sched.h"
/*****************************************************************************
*                                 Project Includes                           *
******************************************************************************/
#include "Rte_CruiseMdl.h"
#include "fcruise_process.h"
#include "fcruise.h"



/*****************************************************************************
*                                 Macro Definitions                          *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro. Define  the receiving can value of a telltale                                                       *
******************************************************************************/

/*****************************************************************************
*                       Locally used Variable Declarations                   *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/

static UI_CRUISE_CONTROL_STATUS_T   l_curr_cruise_control_status_U8;

/*****************************************************************************
*                               Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall    be followed by a comment that gives the following info.  *
* about    the variable.                                                        *
* purpose, critical    section, unit, and resolution                            *
******************************************************************************/

/*****************************************************************************
*                   Functions                                                *
******************************************************************************/
/****************************************************************************
Function Name     : button_KSColdInit

Description       : DO the ColdInit of cluster

Invocation        : Invoked by the Scheduler when IGN and BAT ON

Parameters        : void.

Return Type       : void.

Critical Section  : None.

******************************************************************************/
void  fcruise_process_KSColdInit (void)
{    
    l_curr_cruise_control_status_U8 = UI_CRUCON_OFF;
}

/*****************************************************************************
Function Name    : cruise_event_handler
Description      : To handle the trip switch user event and take required action
Invocation       : LSH when the trip switch event occurs
Parameters       : p_button_id_U16 - Button ID
                   p_button_event_U8 - Button Event
Return Value     : None
Critical Section : None
*****************************************************************************/
void cruise_event_handler(UINT8 p_button_event_U8)
{
    UINT8 fl_ign_status_U8;
	
	Rte_Read_rpIgnState_IGNState(&fl_ign_status_U8);
    if (eIGN_OFF == fl_ign_status_U8)
    {
        l_curr_cruise_control_status_U8 = UI_CRUCON_OFF;
    }
    else if (eIGN_RUN == fl_ign_status_U8)
    {
         switch(p_button_event_U8)
        {
             /*Cruise on button*/
            case BTN_EV_CRUON_SHORT:
            case BTN_EV_CRUON_LONG_IN:
                l_curr_cruise_control_status_U8 = UI_CRUCON_ON;
                break;
                
            /*Cruise off button*/
            case BTN_EV_CRUOFF_SHORT:
            case BTN_EV_CRUOFF_LONG_IN:
                l_curr_cruise_control_status_U8 = UI_CRUCON_OFF;
                break;
                
            /*RES+ button*/
            case BTN_EV_RES_ADD_SHORT:
                processButton(UI_CRUCON_ADD_SHORT);
                break;
            case BTN_EV_RES_ADD_LONG_IN:
                processButton(UI_CRUCON_ADD_LONG);
                break;
            case BTN_EV_RES_ADD_OUT:
			case BTN_EV_RES_ADD_LONG_OUT:
			case BTN_EV_RES_ADD_SUPER_LONG_OUT:
                processButton(UI_CRUCON_OPTION);
                break;
                
            /*SET- button*/
            case BTN_EV_SET_SUB_SHORT:
                processButton(UI_CRUCON_SUB_SHORT);
                break;
            case BTN_EV_SET_SUB_LONG_IN:
                processButton(UI_CRUCON_SUB_LONG);
                break;
            case BTN_EV_SET_SUB_OUT:
			case BTN_EV_SET_SUB_LONG_OUT:
			case BTN_EV_SET_SUB_SUPER_LONG_OUT:
                processButton(UI_CRUCON_OPTION);
                break;
                
            /*CANCEL- button*/
            case BTN_EV_CANCEL_SHORT:
            case BTN_EV_CANCEL_LONG_IN:
                processButton(UI_CRUCON_CANCEL);
                break;
                
            default:
                break;
        }
    }
    else
    {
        /*do nothing*/
    }
}

/*****************************************************************************
Function Name    : button_cruise_off
Description      : reset the cruise control state default.
Invocation       : Application
Parameters       : None
Return Value     : void
Critical Section : None
*****************************************************************************/
void button_cruise_off(void)
{
    l_curr_cruise_control_status_U8 = UI_CRUCON_OFF;
}

/*****************************************************************************
Function Name    : get_cruise_control_status
Description      : Returns which item to be displayed in the footer.
Invocation       : Application
Parameters       : None
Return Value     : UI_CRUISE_CONTROL_STATUS_T
Critical Section : None
*****************************************************************************/
UI_CRUISE_CONTROL_STATUS_T get_cruise_control_status(void)
{
   return (l_curr_cruise_control_status_U8);
}

#endif

/*End   of File*/
/*****************************************************************************
*       for each change to this file,   be sure to record:                   *
*            1.  who made   the change and when the change was made          *
*            2.  why the change was made and the intended   result           *
*       Following   block   needs   to be   repeated for each   change       *
******************************************************************************/


