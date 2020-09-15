/*============================================================================
**
**                     CONFIDENTIAL VISTEON CORPORATION
**
** This is an unpublished work of authorship, which contains trade secrets,
** created in 2005. Visteon Corporation owns all rights to this work and
** intends to maintain it in confidence to preserve its trade secret status.
** Visteon Corporation reserves the right, under the copyright laws of the
** United States or those of any other country that may have jurisdiction,
** to protect this work as an unpublished work, in the event of an
** inadvertent or deliberate unauthorized publication. Visteon Corporation
** also reserves its rights under all copyright laws to protect this work as
** a published work, when appropriate. Those having access to this work may
** not copy it, use it, modify it or disclose the information contained in
** it without the written authorization of Visteon Corporation.
**
**============================================================================
**
** Name:           system.h
**
** Description:    This files contains type definitions and other information 
**                 needed by the DI Kernel for the NIOS II processor. 
**
**============================================================================
**
**  $Archive:   J:/arch/00diaps_/src/template.h_v  $
** $Revision:   1.6  $
**     $Date:   08 May 2003 13:21:28  $
**  $Modtime:   08 May 2003 13:21:18  $
**   $Author:   jkanoza  $
**
**==========================================================================*/
#ifndef SYSTEM_H
#define SYSTEM_H

/*============================================================================
** I N C L U D E   F I L E S
**==========================================================================*/

/*============================================================================
** T Y P E   D E F I N I T I O N S
**==========================================================================*/
#if !defined( FALSE )
    #define FALSE   ((boolean) 0)
#endif

#if !defined( TRUE )
    #define TRUE    ((boolean) 1)
#endif

/*
// Bit manipulation macros.
*/
#define TESTBIT( operand, bit_mask )        (((operand) &  (bit_mask)) != ((bit_mask) - (bit_mask)))
#define SETBIT( operand, bit_mask )         ((operand) |= (bit_mask))
#define CLEARBIT( operand, bit_mask )       ((operand) &= ((uint32)~(bit_mask)))

/*
//  "bit" position mask definitions
*/
#ifndef cBIT_POSITION_0
#define cBIT_POSITION_0    (0x01)
#endif
#ifndef cBIT_POSITION_1
#define cBIT_POSITION_1    (0x02)
#endif
#ifndef cBIT_POSITION_2
#define cBIT_POSITION_2    (0x04)
#endif
#ifndef cBIT_POSITION_3
#define cBIT_POSITION_    (0x08)
#endif
#ifndef cBIT_POSITION_4
#define cBIT_POSITION_4    (0x10)
#endif
#ifndef cBIT_POSITION_5
#define cBIT_POSITION_5    (0x20)
#endif
#ifndef cBIT_POSITION_6
#define cBIT_POSITION_6    (0x40)
#endif
#ifndef cBIT_POSITION_7
#define cBIT_POSITION_7    (0x80)
#endif
#ifndef cBIT_POSITION_8
#define cBIT_POSITION_8    (0x0100u)
#endif
#ifndef cBIT_POSITION_9
#define cBIT_POSITION_9    (0x0200u)
#endif
#ifndef cBIT_POSITION_10
#define cBIT_POSITION_10   (0x0400u)
#endif
#ifndef cBIT_POSITION_11
#define cBIT_POSITION_11   (0x0800u)
#endif
#ifndef cBIT_POSITION_12
#define cBIT_POSITION_12   (0x1000u)
#endif
#ifndef cBIT_POSITION_13
#define cBIT_POSITION_13   (0x2000u)
#endif
#ifndef cBIT_POSITION_14
#define cBIT_POSITION_14   (0x4000u)
#endif
#ifndef cBIT_POSITION_15
#define cBIT_POSITION_15   (0x8000u)
#endif
#ifndef cBIT_POSITION_16
#define cBIT_POSITION_16   (0x00010000u)
#endif
#ifndef cBIT_POSITION_17
#define cBIT_POSITION_17   (0x00020000u)
#endif
#ifndef cBIT_POSITION_18
#define cBIT_POSITION_18   (0x00040000u)
#endif
#ifndef cBIT_POSITION_19
#define cBIT_POSITION_19   (0x00080000u)
#endif
#ifndef cBIT_POSITION_20
#define cBIT_POSITION_20   (0x00100000u)
#endif
#ifndef cBIT_POSITION_21
#define cBIT_POSITION_21   (0x00200000u)
#endif
#ifndef cBIT_POSITION_22
#define cBIT_POSITION_22   (0x00400000u)
#endif
#ifndef cBIT_POSITION_23
#define cBIT_POSITION_23   (0x00800000u)
#endif
#ifndef cBIT_POSITION_24
#define cBIT_POSITION_24   (0x01000000u)
#endif
#ifndef cBIT_POSITION_25
#define cBIT_POSITION_25   (0x02000000u)
#endif
#ifndef cBIT_POSITION_26
#define cBIT_POSITION_26   (0x04000000u)
#endif
#ifndef cBIT_POSITION_27
#define cBIT_POSITION_27   (0x08000000u)
#endif
#ifndef cBIT_POSITION_28
#define cBIT_POSITION_28   (0x10000000u)
#endif
#ifndef cBIT_POSITION_29
#define cBIT_POSITION_29   (0x20000000u)
#endif
#ifndef cBIT_POSITION_30
#define cBIT_POSITION_30   (0x40000000u)
#endif
#ifndef cBIT_POSITION_31
#define cBIT_POSITION_31   (0x80000000u)
#endif

/*============================================================================
** E N T R Y   P O I N T S
**==========================================================================*/


/*============================================================================
** D A T A   A C C E S S   S E R V I C E S
**==========================================================================*/

/* ===========================================================================
//  R E V I S I O N    N O T E S
// ---------------------------------------------------------------------------
//  for each change to this file, record the following:
//
//  1.  who made the change and when the change was made
//  2.  why the change was made and the intended result
//
// ---------------------------------------------------------------------------
//  $Revision:   1.14  $
// ---------------------------------------------------------------------------
//
//  Clearcase main\1 6/11/2007 gpalarsk 
//                   Initial revision 
//
//  Clearcase main\2 6/18/2007 gpalarsk 
//                   Added typedefs for UINT8, SINT8, UINT16, SINT16, UINT32, and SINT32.
//                   Added macros BIT0, BIT1, ... BIT31.
//
// =========================================================================*/
#endif

/* Last line of file */
