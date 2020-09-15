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
//  Name:               fhyst.c
//
//  Description:        Configurable fuel filter algorithms.
//
//  Organization:       Driver Information Software Section,
//                      DI Core Engineering Department
//
//==========================================================================*/
#define FHYST_C


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

#include "Rte_IoHwAb.h"
#include "fhyst.h"

/*============================================================================
// I N T E R N A L   M A C R O   A N D   T Y P E   D E F I N I T I O N S
//==========================================================================*/
/*
// Define a macro to get past the QAC problem with !TESTBIT
*/

uint8 fhyst_get_index_with_up_hyst(THRESHOLD * hyst_table,uint16 current_value,uint8 current_index)
{
	uint8 fl_return_value;
	fl_return_value = fhyst_get_index(hyst_table,current_value,current_index,UP_HYST);
	return(fl_return_value);
}




uint8 fhyst_get_index_with_down_hyst(THRESHOLD * hyst_table,uint16 current_value,uint8 current_index)
{
	uint8 fl_return_value;
	fl_return_value = fhyst_get_index(hyst_table,current_value,current_index,DOWN_HYST);
	return(fl_return_value);
}



uint8 fhyst_get_index(THRESHOLD hyst_table[],uint16 current_value,uint8 current_index,uint8 up_or_down_hyst)
{
	uint8 fl_index_U8;
	uint8 fl_return_index_U8;
	uint8 fl_table_size_U8;
	fl_table_size_U8 = (uint8)hyst_table[0].STATE_0_thresh_U16;

	/* check for which bar index it is lying.ie. which bar definitely OFF */
	for(fl_index_U8 = 1; fl_index_U8 <= fl_table_size_U8; fl_index_U8++)
	{
		if(current_value < hyst_table[fl_index_U8].STATE_0_thresh_U16)
		{
			break;
		}
		else
		{
		}
	}
	if((uint8)1 < fl_index_U8)
	{
		fl_index_U8 = fl_index_U8 - 1;
		/* if the value lies between two bars index */
		if(current_value >= hyst_table[fl_index_U8].STATE_1_thresh_U16)
		{
			fl_return_index_U8 = fl_index_U8;
		}
		else /* if not then bar is in hysterisis*/
		{
			if(UP_HYST == up_or_down_hyst)
			{
				if(fl_index_U8 <= current_index)
				{
					fl_return_index_U8 = fl_index_U8;
				}
				else
				{
					fl_return_index_U8 = fl_index_U8-1;
				}
			}
			else
			{
				if(fl_index_U8 <= current_index)
				{
					fl_return_index_U8 = fl_index_U8-1;
				}
				else
				{
					fl_return_index_U8 = fl_index_U8;
				}
			}
		}

	}
	else
	{
		fl_return_index_U8 = 0;
	}
	return(fl_return_index_U8);
}
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
/*---------------------------------------------------------------------------
Date              : 24-Jan-2012
By                : psriniv2
Traceability      :
Change Description: Initial Release
------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Date              : 06-Mar-2013
By                : Ksreenat
Traceability      : QAC - 8.0 fix
Change Description: QAC - 8.0 fix
-----------------------------------------------------------------------------*/
