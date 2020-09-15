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

File Name        :  linear_interpolation.c
Module Short Name:  LINEINTERPOL
VOBName          :  sw_services_ss
Author           :  aarumuga
Description      :  Linear interpolation for signed and unsigned byte and word 
Organization     :  Visteon Software Operation
                    Visteon Corporation

----------------------------------------------------------------------------
Compiler Name    :  Not compiler dependent
Target Hardware  :  Not hardware dependent
******************************************************************************/

#define LINEINTERPOL_C
/*****************************************************************************
*                                 System Includes                            *
******************************************************************************/
//#include "system.h"
#include "linear_interpolation.h"
/*****************************************************************************
*                                 Project Includes                           *
******************************************************************************/

/*****************************************************************************
*                                 Manifest Constants                         *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/

/*****************************************************************************
*                                 Macro Definitions                          *
*----------------------------------------------------------------------------*
* Definition of macro shall be followed by a comment that explains the       *
* purpose of the macro.                                                      *
******************************************************************************/
#define MIN_VAL_IP     (*(table))
#define MAX_VAL_IP     (table[1])
#define FIRST_VAL_IP   (&(table[2]))
#define CURR_VAL_IP    (*(ptr))
#define CURR_VAL_OP    (ptr[1])
#define NEXT_VAL_IP    (ptr[2])
#define NEXT_VAL_OP    (ptr[3])


/*****************************************************************************
*                                 Type Declarations                          *
******************************************************************************/
/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, and resolution                            *
******************************************************************************/
/*
    The LinearIntepolateXXXXX function makes some important assumptions
    about what is given to it to work on.  They are as follows:
    - A table of uint8/uint16/sint8/sint16 exists that is 2 columns by n rows
    - The first column is the input side (must be ascending order)
    - The second column is the output side (can be ascending or descending order)
    - The first row of the table identifies the lowest and highest value
      found in the input side of the table (not fixed size, # rows!)
      These values must be in the input side, do not put a number in the
      first row and then not put the number in the input side of the table.
      (!!!unpredictable results may occur!!!)
    - Lookup values less than the lowest input val produce an output
      equal to the lowest input value's output value
    - Lookup values greater than the highest input val produces an output
      equal to the highest input value's output value
*/

/*****************************************************************************
*                                 Locally used Variable Declarations         *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, and resolution                            *
******************************************************************************/

/*****************************************************************************
*                              Limited Scope Prototypes                      *
******************************************************************************/



/*****************************************************************************
*                   Functions                                                *
******************************************************************************/
/*============================================================================

  Function Name:      uint16 LinearInterpolateWord(uint16 *table, uint16 lookupVal)

  Description:        Linear interpolation code.

  Input Arguments:    look up table and look up value

  Output Arguments:   interpolated value

==========================================================================*/
uint16 LinearInterpolateWord(uint16 *table, uint16 lookupVal)
{
  uint16 *ptr;

  /*
  ** If the input value is less or greater than the min/max entry, set it
  ** to the min or max entry value for the lookup function.
  */
  if (lookupVal <= MIN_VAL_IP)
  {
    return (table[3]);
  }        
  else if (lookupVal > MAX_VAL_IP)
  {
    lookupVal = MAX_VAL_IP;
  }
  else
  {
    /* to keep QAC happy */
  }

  /*
  ** Search through table until the exact entry is found or bounding entries
  ** are found.  Then interpolate if necessary.
  */
  for (ptr=FIRST_VAL_IP; *ptr<=MAX_VAL_IP; ptr=&ptr[2])
  {
    if (CURR_VAL_IP == lookupVal)
    {
      return (CURR_VAL_OP);
    }
    else
    {
      if (lookupVal < NEXT_VAL_IP)
      {
        if (NEXT_VAL_OP >= CURR_VAL_OP) 
        {
          return (uint16)((((uint32)(lookupVal-CURR_VAL_IP) * (uint32)(NEXT_VAL_OP-CURR_VAL_OP))/((uint32)(NEXT_VAL_IP-CURR_VAL_IP)))+CURR_VAL_OP);
        }
        else
        {
          return (uint16)(CURR_VAL_OP-(((uint32)(lookupVal-CURR_VAL_IP) * (uint32)(CURR_VAL_OP-NEXT_VAL_OP))/((uint32)(NEXT_VAL_IP-CURR_VAL_IP))));
        }
      }
    }
  }

  /*
  ** If we get here, something is wrong with the table, so return 0 as a failure indication.
  */
  return (0);
}


/*============================================================================

  Function Name:      uint8 LinearInterpolateByte(uint8 *table, uint8 lookupVal)

  Description:        Linear interpolation code.

  Input Arguments:    look up table and look up value

  Output Arguments:   interpolated value

==========================================================================*/
uint8 LinearInterpolateByte(uint8 *table, uint8 lookupVal)
{
  uint8 *ptr;

  /*
  ** If the input value is less or greater than the min/max entry, set it
  ** to the min or max entry value for the lookup function.
  */
  if (lookupVal <= MIN_VAL_IP)
  {
    return (table[3]);
  }
  else if (lookupVal > MAX_VAL_IP)
  {
    lookupVal = MAX_VAL_IP;
  }
  else
  {
    /* to keep QAC happy */
  }

  /*
  ** Search through table until the exact entry is found or bounding entries
  ** are found.  Then interpolate if necessary.
  */
  for (ptr=FIRST_VAL_IP; *ptr<=MAX_VAL_IP; ptr=&ptr[2])
  {
    if (CURR_VAL_IP == lookupVal)
    {
      return (CURR_VAL_OP);
    }
    else
    {
      if (lookupVal < NEXT_VAL_IP)
      {
        if (NEXT_VAL_OP >= CURR_VAL_OP) 
        {
          return (uint8)((((uint16)(lookupVal-CURR_VAL_IP) * (uint16)(NEXT_VAL_OP-CURR_VAL_OP))/((uint16)(NEXT_VAL_IP-CURR_VAL_IP)))+CURR_VAL_OP);
        }
        else
        {
          return (uint8)(CURR_VAL_OP-(((uint16)(lookupVal-CURR_VAL_IP) * (uint16)(CURR_VAL_OP-NEXT_VAL_OP))/((uint16)(NEXT_VAL_IP-CURR_VAL_IP))));
        }
      }
    }
  }

  /*
  ** If we get here, something is wrong with the table, so return 0 as a failure indication.
  */
  return (0);
}


/*============================================================================

  Function Name:      sint8 LinearInterpolateSByte(sint8 *table, sint8 lookupVal)

  Description:        Linear interpolation code.

  Input Arguments:    look up table and look up value

  Output Arguments:   interpolated value

==========================================================================*/
sint8 LinearInterpolateSByte(sint8 *table, sint8 lookupVal)
{
  sint8 *ptr;

  /*
  ** If the input value is less or greater than the min/max entry, set it
  ** to the min or max entry value for the lookup function.
  */
  if (lookupVal <= MIN_VAL_IP)
  {
    return (table[3]);
  }
  else if (lookupVal > MAX_VAL_IP)
  {
    lookupVal = MAX_VAL_IP;
  }
  else
  {
    /* to keep QAC happy */
  }

  /*
  ** Search through table until the exact entry is found or bounding entries
  ** are found.  Then interpolate if necessary.
  */
  for (ptr=FIRST_VAL_IP; *ptr<=MAX_VAL_IP; ptr=&ptr[2])
  {
    if (CURR_VAL_IP == lookupVal)
    {
      return (CURR_VAL_OP);
    }
    else
    {
      if (lookupVal < NEXT_VAL_IP)
      {
        if (NEXT_VAL_OP >= CURR_VAL_OP) 
        {
          return (sint8)((((sint16)(lookupVal-CURR_VAL_IP) * (sint16)(NEXT_VAL_OP-CURR_VAL_OP))/((sint16)(NEXT_VAL_IP-CURR_VAL_IP)))+CURR_VAL_OP);
        }
        else
        {
          return (sint8)(CURR_VAL_OP-(((sint16)(lookupVal-CURR_VAL_IP) * (sint16)(CURR_VAL_OP-NEXT_VAL_OP))/((sint16)(NEXT_VAL_IP-CURR_VAL_IP))));

        }
      }
    }
  }

  /*
  ** If we get here, something is wrong with the table, so return 0 as a failure indication.
  */
  return (0);
}

 /*============================================================================

  Function Name:      sint16 LinearInterpolateSWord(sint16 *table, sint16 lookupVal)

  Description:        Linear interpolation code.

  Input Arguments:    look up table and look up value

  Output Arguments:   interpolated value

==========================================================================*/
sint16 LinearInterpolateSWord(sint16 *table, sint16 lookupVal)
{
  sint16 *ptr;

  /*
  ** If the input value is less or greater than the min/max entry, set it
  ** to the min or max entry value for the lookup function.
  */
  if (lookupVal <= MIN_VAL_IP)
  {
    return (table[3]);
  }        
  else if (lookupVal > MAX_VAL_IP)
  {
    lookupVal = MAX_VAL_IP;
  }
  else
  {
    /* to keep QAC happy */
  }

  /*
  ** Search through table until the exact entry is found or bounding entries
  ** are found.  Then interpolate if necessary.
  */
  for (ptr=FIRST_VAL_IP; *ptr<=MAX_VAL_IP; ptr=&ptr[2])
  {
    if (CURR_VAL_IP == lookupVal)
    {
      return (CURR_VAL_OP);
    }
    else
    {
      if (lookupVal < NEXT_VAL_IP)
      {
        if (NEXT_VAL_OP >= CURR_VAL_OP) 
        {
          return (sint16)((((sint32)(lookupVal-CURR_VAL_IP) * (sint32)(NEXT_VAL_OP-CURR_VAL_OP))/((sint32)(NEXT_VAL_IP-CURR_VAL_IP)))+CURR_VAL_OP);
        }
        else
        {
          return (sint16)(CURR_VAL_OP-(((sint32)(lookupVal-CURR_VAL_IP) * (sint32)(CURR_VAL_OP-NEXT_VAL_OP))/((sint32)(NEXT_VAL_IP-CURR_VAL_IP))));
        }
      }
    }
  }

  /*
  ** If we get here, something is wrong with the table, so return 0 as a failure indication.
  */
  return (0);
}



/*End of File*/
/****************************************************************************
*   for each change to this file, be sure to record:                        *
*      1.  who made the change and when the change was made                 *
*      2.  why the change was made and the intended result                  *
*   Following block needs to be repeated for each change                    *
*****************************************************************************/
/*---------------------------------------------------------------------------
Release Label     : SERVICES_LINPOL 1.0
Date              : 18-01-2010
By                : aarumuga
Traceability      : CQUCM00003989
                    Initial version Source copied from 
                    sw_services_ss\linear_interpolation_byte\03_code\src
                    sw_services_ss\linear_interpolation_word\03_code\src  
Change Description: Linear interpolation for signed / unsigned byte and word.
-----------------------------------------------------------------------------*/
