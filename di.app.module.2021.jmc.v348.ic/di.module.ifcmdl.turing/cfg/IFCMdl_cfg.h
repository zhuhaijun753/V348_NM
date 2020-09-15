/*****************************************************************************
*                                                                            *
*              CONFIDENTIAL VISTEON CORPORATION                              *
*                                                                            *
* This is an unpublished work of authorship, which contains trade            *
* secrets, created in 2003. Visteon Corporation owns all rights to           *
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

File Name        :  fifc.h
Module Short Name:  fifc
VOBName          :  \sw_di_int\2010_nissan_x32l_ic
Author           :  lizhao
Description      :  This module drives the  Instantaneous Fuel Consumption
					as per the specific requirements of the Program

Organization     :  Driver Information Software Section,
                    Visteon Software Operation, India
                    Visteon Corporation

----------------------------------------------------------------------------
Compiler Name    : cosmic.v477 
******************************************************************************/

#ifndef IFCMDL_CFG_H
#define IFCMDL_CFG_H


typedef enum eODO_UINT
{
    ODO_1000M = 0,
    ODO_100M  = 1,
    ODO_10M   = 2,
    ODO_1M    = 3
} tODO_UINT;


extern uint32 OdoLogicalMdl_get_main(tODO_UINT uint);
extern uint32 OdoLogicalMdl_get_odo_for_ifc(void);


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

#define VEHICLE_STATUS_IDLE         (uint8)(0)
#define VEHICLE_STATUS_NON_IDLE     (uint8)(1)
#define MAX_FUEL_IFC_COUNT          (uint8)(2)
#define IFC_INVALID_VALUE           (uint32)(0xFFFF)
#define IFC_LOOP_NUMBER             (uint8)(10)
#define FSPEED_3KMH                 (uint16)(300)
#define DT_CALC_VALUE               (uint64)214489588
#define IFC_CALC_TIME_DIFF          (uint8)1
#define WHLMILG_COMPARE_VALUE       (uint8)151
#define IFC_2S_TIME_OUT_COUNT       (uint16)1000
#define IFC_CALC_CONST_VALUE        (uint32)(131070)
#define IFC_CALC_MOVE_VALUE         (uint8)16
#define IFC_CALC_FACTOR_Diesel      (uint8)2
#define IFC_CALC_FACTOR_Gasoline    (uint8)5
#define IFC_CALC_OF_CONST_VALUE     (uint32)65535
#define IFC_CALC_DT_D_CONST_VALUE   (uint32)65535
#define IFC_FUEL_INVALID_VALUE      (uint32)0xFFFF
#define IDC_FUEL_MAX_VALUE          (uint32)0xFFFF
#define IFC_INVALID_DISPLAY         (uint8)0x00
#define IFC_IDLE_DISPLAY            (uint8)0x01
#define IFC_NONE_IDLE_DISPLAY       (uint8)0x02
#define FODO_ODO_NVM_ERROR      	(uint32)(0xFFFFFFFFul)
#define ENG_SPD_RESOLUTION          (uint8)2
#define FENGSPEED_300RPM            (uint16)(300)


#define Rte_Read_rpSR_AfcMdl_OdoValue(x)  			OdoLogicalMdl_get_main(x)
#define Rte_read_rpSR_IFCMdl_OdoValue				OdoLogicalMdl_get_odo_for_ifc


#endif
/*End of File*/
/****************************************************************************
*   for each change to this file, be sure to record:                        *
*      1.  who made the change and when the change was made                 *
*      2.  why the change was made and the intended result                  *
*   Following block needs to be repeated for each change                    *
*****************************************************************************/
/*---------------------------------------------------------------------------
Release Label     : Initial Version
Date              : 01-Nov-09
By                : lizhao
Traceability      : Leveraged from 2010 Nissan X61B IC
Change Description: Initial version.
-----------------------------------------------------------------------------*/






