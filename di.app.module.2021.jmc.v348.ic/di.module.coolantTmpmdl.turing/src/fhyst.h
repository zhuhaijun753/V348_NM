/*============================================================================
//
//                     CONFIDENTIAL VISTEON CORPORATION
//
// This is an unpublished work of authorship, which contains trade secrets,
// created in 2000. Visteon Corporation owns all rights to this work and
// intends to maintain it in confidence to preserve its trade secret status.
// Visteon Corporation reserves the right, under the copyright laws of the
// United States or those of any other country that may have  jurisdiction,
// to protect this work as an unpublished work, in the event of an
// inadvertent or deliberate unauthorized publication. Visteon Corporation
// also reserves its rights under all copyright laws to protect this work as
// a published work, when appropriate. Those having access to this work may
// not copy it, use it, modify it or disclose the information contained in
// it without the written authorization of Visteon Corporation.
//
//============================================================================
//
//  Name:               fuel03.c
//
//  Description:        Configurable fuel filter algorithms.
//
//  Organization:       Driver Information Software Section,
//                      DI Core Engineering Department
//
//==========================================================================*/
#ifndef _F_HYST_H_
#define _F_HYST_H_

//#include "basetsd.h"

/*============================================================================
//                         O V E R V I E W
//==========================================================================*/
/*
** The intent of this code is to provide a configurable fuel filter
** algorithm for use in a variety of instrument cluster applications.
**
** This package is a stripped down single sender version of the Fuel02 package.
** Therefore, the requirements for this code package are detailed in the
** Fuel Algorithm Bookshelf Design Specification (BDS) 4FA1-1A306-CA.
*/


/*============================================================================
//                 I N C L U D E   F I L E S
//==========================================================================*/

/*============================================================================
// I N T E R N A L   M A C R O   A N D   T Y P E   D E F I N I T I O N S
//==========================================================================*/
/*
// Define a macro to get past the QAC problem with !TESTBIT
*/
#define UP_HYST    0
#define DOWN_HYST  1
/*
purpose         : This structure will hold the tacho bars OFF and ON Thresholds
critical section: None
unit            : count
resolution      : 1
valid Range     : 0-20
Validity check  : No
*/
typedef struct
{
  uint16 STATE_0_thresh_U16;
  uint16 STATE_1_thresh_U16;
}THRESHOLD;

extern uint8 fhyst_get_index_with_up_hyst(THRESHOLD * hyst_table,uint16 current_value,uint8 current_index);
extern uint8 fhyst_get_index_with_down_hyst(THRESHOLD * hyst_table,uint16 current_value,uint8 current_index);
extern uint8 fhyst_get_index(THRESHOLD * hyst_table,uint16 current_value,uint8 current_index,uint8 up_or_down_hyst);

/*End of File*/
/*****************************************************************************
*   for each change to this file, be sure to record:                         *
*      1.  who made the change and when the change was made                  *
*      2.  why the change was made and the intended result                   *
*   Following block needs to be repeated for each change
******************************************************************************
*   Note: In the traceability column we need to trace back to the Design Doc.*
*   For the initial version it is traced to the Design Document section.     *
*   For further changes it shall trace to the source of the change which may *
*   be SPSS/SCR/Defect details(Defect may be Testing/validation defect)/Any  *
*   other reason                                                            *
******************************************************************************/

#endif

/*---------------------------------------------------------------------------
Date              : 9-Jan-2011
CDSID             : spratap
Traceability      : 
Change Description: Initial Release
-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Date              : 17-Jan-2012
By                : spratap
Traceability      :
Change Description: PR(51121) comments updated
------------------------------------------------------------------------------*/
