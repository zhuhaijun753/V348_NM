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
typedef enum 
{
    UI_WARNING_HAS_FAULT = 0,
    UI_WARNING_EBS_OFF ,
    UI_WARNING_EBS_FAULT ,
    UI_WARNING_EBS_TMP_UNAVAILABLE ,
    UI_WARNING_FCW_OFF ,
    UI_WARNING_FCW_FAULT ,
    UI_WARNING_EBS_ON ,
    UI_WARNING_FCW_ON ,
    UI_WARNING_LANE_KEEPING_SYS_OFF ,
    UI_WARNING_LANE_KEEPING_SYS_FAULT ,
    UI_WARNING_CAMERA_BLOCKED ,
    UI_WARNING_LANE_KEEPING_SYS_TMP ,
    UI_WARNING_HANDS_ON_STEERING ,
    UI_WARNING_LANE_KEEPING_SYS_ON ,
}UI_WARNING_ID_T;



#define UI_WARNING_TOTAL_AEB 14



#endif


