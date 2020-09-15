//******************************************************************************
// Company:      Johnson Controls Inc.
// -----------------------------------------------------------------------------
// Copyright:    This software is JCI property.
//               Duplication or disclosure without JCI written authorization
//               is prohibited.
// -----------------------------------------------------------------------------
// Project:      STK
// Language:     ANSI-C
// -----------------------------------------------------------------------------
// Component:    TOS
//               TOS management
// -----------------------------------------------------------------------------
// $Date:$
// $Archive:$
// $Revision:$
// -----------------------------------------------------------------------------
/* $Log:$
*/
//******************************************************************************

#ifndef I_TOS_H
#define I_TOS_H

//------------------------------------------------------------------------------
// Included files to resolve specific definitions in this file
//------------------------------------------------------------------------------
#include "deftypes.h"

//------------------------------------------------------------------------------
// Exported type
//------------------------------------------------------------------------------
//----- Clock management (must always be an unsigned type) -----
typedef U16 tTOSClock;

typedef tTOSClock tTOSTimer;

#define cTOSTimerTickMs                       5UL

// convert a time duration into a number of clock ticks
#define mTOSConvMsInTimerTick(x)   ((tTOSTimer) ( (x) / cTOSTimerTickMs))

extern void TOSTpoInitialize( void );
extern void TOSStartTimer( tTOSTimer* const pTimer );
extern void TOSStopTimer( tTOSTimer* const pTimer );
extern BOOL TOSIsTimerStarted( const tTOSTimer* const pTimer );
extern BOOL TOSIsTimerElapsed( tTOSTimer* const pTimer, const tTOSTimer Timeout );
extern void TOSClockTask( void );

#endif   // I_TOS_H

