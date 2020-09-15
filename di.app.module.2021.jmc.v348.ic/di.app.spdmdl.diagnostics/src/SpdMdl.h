#ifndef SPDMDL_H
#define SPDMDL_H

#include "TmExt_Cfg.h"
#include "CDD_Test.h"

void TmExt_Timer_IsElapsed(HTimer hTimer, UInt32 Timeout, Boolean* ElapsedStatus); /* PRQA S 0850 */ /* MD_MSR_19.8 */
void TmExt_Timer_IsStarted(HTimer hTimer, Boolean* StartedStatus); /* PRQA S 0850 */ /* MD_MSR_19.8 */
void TmExt_Timer_Start(HTimer hTimer); /* PRQA S 0850 */ /* MD_MSR_19.8 */
void TmExt_Timer_Stop(HTimer hTimer); /* PRQA S 0850 */ /* MD_MSR_19.8 */

#define IS_TIMER_ELAPSED(Id,timeout,status)			Rte_Call_rp_TmExt_Wrap_Timer_IsElapsed(Id,timeout,status)
#define START_TIMER(Id)								Rte_Call_rp_TmExt_Timer_Start(Id)
#define STOP_TIMER(Id)								Rte_Call_rp_TmExt_Timer_Stop(Id)
#define IS_TIMER_STARTED(Id,status)					Rte_Call_rp_TmExt_Wrap_Timer_IsStarted(Id,status)


#define	Rte_Call_rp_TmExt_Wrap_Timer_IsElapsed		TmExt_Timer_IsElapsed
#define	Rte_Call_rp_TmExt_Timer_Start				TmExt_Timer_Start
#define	Rte_Call_rp_TmExt_Timer_Stop				TmExt_Timer_Stop
#define	Rte_Call_rp_TmExt_Wrap_Timer_IsStarted		TmExt_Timer_IsStarted


#endif
