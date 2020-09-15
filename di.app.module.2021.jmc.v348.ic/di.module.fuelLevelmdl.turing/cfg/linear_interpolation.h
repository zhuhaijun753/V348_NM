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

File Name        :  linear_interpolation.c
Module Short Name:  LINEINTERPOL
VOBName          :  sw_services_ss
Author           :  aarumuga
Description      :  Linear interpolation for signed and unsigned byte and word

Organization     :  Visteon Software Operation
                    Visteon Corporation

******************************************************************************/

#ifndef LINEINTERPOL_H
#define LINEINTERPOL_H

#ifndef LINEINTERPOL_C
#define EXTERN  extern
#else
#define EXTERN
#endif

#include "Platform_Types.h"

#if 0
typedef unsigned char       boolean;        /* for use with TRUE/FALSE        */
typedef signed char         sint8;          /*        -128 .. +127            */
typedef unsigned char       uint8;          /*           0 .. 255             */
typedef signed short        sint16;         /*      -32768 .. +32767          */
typedef unsigned short      uint16;         /*           0 .. 65535           */
typedef signed long         sint32;         /* -2147483648 .. +2147483647     */
typedef unsigned long       uint32;         /*           0 .. 4294967295      */
typedef signed long long    sint64;         /* \brief 64-bit unsigned integer */
typedef unsigned long long  uint64;
#endif


//#include "system.h"
/*****************************************************************************
*                                 Global Constant Declarations               *
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

/*****************************************************************************
*                                 Global Variable Declarations               *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, and resolution                            *
******************************************************************************/
/*
    The LinearIntepolateXXXXX functions make some important assumptions
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
*                                 Global Function Prototypes                 *
******************************************************************************/

EXTERN  uint8   LinearInterpolateByte (uint8  *table,  uint8  lookupVal);

EXTERN  uint16  LinearInterpolateWord (uint16 *table,  uint16 lookupVal);

EXTERN  sint8   LinearInterpolateSByte(sint8  *table,  sint8  lookupVal);

EXTERN  sint16  LinearInterpolateSWord(sint16 *table,  sint16 lookupVal);

#undef EXTERN
#endif

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
                    Initial version with source copied from 
                    sw_services_ss\linear_interpolation_byte\03_code\src
                    sw_services_ss\linear_interpolation_word\03_code\src  
Change Description: Linear interpolation for signed / unsigned byte and word.
-----------------------------------------------------------------------------*/
