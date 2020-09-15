/*****************************************************************************
*                                                                            *
*              CONFIDENTIAL VISTEON CORPORATION                              *
*                                                                            *
* This is an unpublished work of authorship, which contains trade            *
* secrets, created in 2009. Visteon Corporation owns all rights to           *
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

File Name        :  DteMdl_cfg.h
Module Short Name:  foat
VOBName          :  2014_Nissan_P42M_ic
Author           :  jmunisam
Description      :  Handles the AFC application feature

Organization     :  Driver Information Software Section,
                    Visteon Technical and Service Centre, India
                    Visteon Corporation

******************************************************************************/
#ifndef DTEMDL_CFG_H
#define DTEMDL_CFG_H



extern uint32 fdte_get_dte_value(void);
extern void fdte_clear_DteValue(void);



#define Rte_Write_rpSR_DteMdl_Clear_Odo_Value		fdte_clear_DteValue

#endif

