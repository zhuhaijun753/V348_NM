#ifndef DIMDERATEMDL_CFG_H
#define DIMDERATEMDL_CFG_H

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
	
#include "TmExt_Cfg.h"

void TmExt_Timer_IsElapsed(HTimer hTimer, UInt32 Timeout, Boolean* ElapsedStatus); /* PRQA S 0850 */ /* MD_MSR_19.8 */
void TmExt_Timer_IsStarted(HTimer hTimer, Boolean* StartedStatus); /* PRQA S 0850 */ /* MD_MSR_19.8 */
void TmExt_Timer_Start(HTimer hTimer); /* PRQA S 0850 */ /* MD_MSR_19.8 */
void TmExt_Timer_Stop(HTimer hTimer); /* PRQA S 0850 */ /* MD_MSR_19.8 */
	
#define	Rte_Call_rp_TmExt_Wrap_Timer_IsElapsed				TmExt_Timer_IsElapsed
#define	Rte_Call_rp_TmExt_Timer_Start						TmExt_Timer_Start
#define	Rte_Call_rp_TmExt_Timer_Stop						TmExt_Timer_Stop
#define	Rte_Call_rp_TmExt_Wrap_Timer_IsStarted				TmExt_Timer_IsStarted

#endif


