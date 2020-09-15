#ifndef FUELLEVELMDL_CFG_H
#define FUELLEVELMDL_CFG_H

/*****************************************************************************
*                                 Type Decleration                           *
*----------------------------------------------------------------------------*
* Decleration of type shall be accompanied by a comment that explains the    *
* purpose and usage of the type.                                             *
******************************************************************************/
	
#include "TmExt_Cfg.h"

#define EXTERN  extern

#ifndef MS_8_BITS
#define MS_8_BITS( w )                      ((uint8)((w) >> (8)))
#endif



/*****************************************************************************
*                                 Global Macro Definitions                   *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/
#define Rte_Read_tiSR_IoHwAb_Impl_GetVoltage                CIoHwAb_Impl_GetVoltage
#define	Rte_Call_rp_TmExt_Wrap_Timer_IsElapsed				TmExt_Timer_IsElapsed
#define	Rte_Call_rp_TmExt_Timer_Start						TmExt_Timer_Start
#define	Rte_Call_rp_TmExt_Timer_Stop						TmExt_Timer_Stop
#define	Rte_Call_rp_TmExt_Wrap_Timer_IsStarted				TmExt_Timer_IsStarted



/*****************************************************************************
*                                 Global Function Prototypes                 *
******************************************************************************/
uint8 CIoHwAb_Impl_GetVoltage(sint32 Id, sint32* Voltage);

EXTERN boolean ffuel_FastFill_Entry_Conditions(void);

EXTERN boolean ffuel_valid_condition(void);

EXTERN uint16 ffuel_get_calculate_fuel(void);

EXTERN uint16 ffuel_get_parameter_C_fuel(void);

EXTERN uint16 ffuel_get_parameter_A_fuel(void);

EXTERN uint16 ffuel_get_parameter_display_fuel(void);


#endif


