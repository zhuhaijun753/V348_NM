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

File Name        :  AmbientMdl_cfg.h
Module Short Name:  foat
VOBName          :  2014_Nissan_P42M_ic
Author           :  jmunisam
Description      :  Handles the Outside Air temperature application feature

Organization     :  Driver Information Software Section,
                    Visteon Technical and Service Centre, India
                    Visteon Corporation

******************************************************************************/
#ifndef AMBIENTMDL_CFG_H
#define AMBIENTMDL_CFG_H

#include "std_types.h"
#include "TmExt_Cfg.h"


void TmExt_Timer_IsElapsed(HTimer hTimer, UInt32 Timeout, Boolean* ElapsedStatus); /* PRQA S 0850 */ /* MD_MSR_19.8 */
void TmExt_Timer_IsStarted(HTimer hTimer, Boolean* StartedStatus); /* PRQA S 0850 */ /* MD_MSR_19.8 */
void TmExt_Timer_Start(HTimer hTimer); /* PRQA S 0850 */ /* MD_MSR_19.8 */
void TmExt_Timer_Stop(HTimer hTimer); /* PRQA S 0850 */ /* MD_MSR_19.8 */
	
#define	Rte_Call_rp_TmExt_Wrap_Timer_IsElapsed				TmExt_Timer_IsElapsed
#define	Rte_Call_rp_TmExt_Timer_Start						TmExt_Timer_Start
#define	Rte_Call_rp_TmExt_Timer_Stop						TmExt_Timer_Stop
#define	Rte_Call_rp_TmExt_Wrap_Timer_IsStarted				TmExt_Timer_IsStarted

extern boolean HmiGetOutsideTempStatus(void);


//extern Rtc_TimeType Clock_Get_CurrentTime(void);


#endif

