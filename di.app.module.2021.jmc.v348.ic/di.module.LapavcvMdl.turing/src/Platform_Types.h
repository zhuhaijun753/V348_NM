#ifndef _QAC_
// *********************************************************************************************************************
// Company:     Johnson Controls Inc.
// ---------------------------------------------------------------------------------------------------------------------
// Copyright:   This software is JCI property.
//              Duplication or disclosure without JCI written authorization is prohibited.
// ---------------------------------------------------------------------------------------------------------------------
// Project:     AUTOSAR standard headers
// Language:    ANSI-C
// ---------------------------------------------------------------------------------------------------------------------
// Component:   Platform Types Header
// ---------------------------------------------------------------------------------------------------------------------
// Reference:   Specification of Platform Types, V2.2.2 R3.1 Rev 0001   --  AUTOSAR_SWS_PlatformTypes.pdf
// ---------------------------------------------------------------------------------------------------------------------
// $Log:   //EL_DI_PSA_X81_IC_SOP_2016/archives/MC/Development/INT/APPLI/L1/BUILD_L1_IT3_TEMP/Sources/Platform_Types.h-arc  $
// 
//    Rev 1.1   09 Jul 2015 11:47:50   ali1653
// Cm084339:L1:DEV If the gauge_mapping_Y >22400,will overflow
// 
//    Rev 1.0   Apr 25 2015 18:15:20   aluant
// Initial revision.
// 
//    Rev 1.0   Mar 06 2015 10:38:12   aluant
// Initial revision.
// 
//    Rev 1.0   14 Mar 2013 12:47:12   agenchr
// Initial revision.
// 
//    Rev 1.4   May 26 2010 18:19:38   asarbip
// Set uint32 to unsigned long
// 
//    Rev 1.3   Dec 18 2009 15:55:26   asarbip
// Put last type edition.
//
//   Rev 1.1   Oct 13 2009 14:46:12   azubria
//Remove non-standard types sint64, uint64
//
//   Rev 1.0   Oct 13 2009 14:39:00   azubria
//Initial revision.
//
// *********************************************************************************************************************
#endif  // _QAC_


#ifndef I_PLATFORM_TYPES_H
#define I_PLATFORM_TYPES_H


//----------------------------------------------------------------------------------------------------------------------
// Included files to resolve specific definitions in this file
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// [BSW00402] Published information
//----------------------------------------------------------------------------------------------------------------------
//-- AUTOSAR Specification Version Information
#define PLATFORM_AR_MAJOR_VERSION   ( 2u )
#define PLATFORM_AR_MINOR_VERSION   ( 2u )
#define PLATFORM_AR_PATCH_VERSION   ( 2u )

//-- Component Version Information
#define PLATFORM_SW_MAJOR_VERSION   ( 1u )
#define PLATFORM_SW_MINOR_VERSION   ( 0u )
#define PLATFORM_SW_PATCH_VERSION   ( 0u )

//-- Component Vendor Id (51 -> JCI)
#define PLATFORM_VENDOR_ID          ( 51u )


//----------------------------------------------------------------------------------------------------------------------
// Constant data
//----------------------------------------------------------------------------------------------------------------------
//-- CPU_TYPE enumeration
#ifndef CPU_TYPE_8
    #define CPU_TYPE_8      (  8 )
#endif  // CPU_TYPE_8
#ifndef CPU_TYPE_16
    #define CPU_TYPE_16     ( 16 )
#endif  // CPU_TYPE_16
#ifndef CPU_TYPE_32
    #define CPU_TYPE_32     ( 32 )
#endif  // CPU_TYPE_32


//-- CPU_BIT_ORDER enumeration
// big endian bit ordering
#ifndef MSB_FIRST
    #define MSB_FIRST       ( 0 )
#endif  // MSB_FIRST
// little endian bit ordering
#ifndef LSB_FIRST
    #define LSB_FIRST       ( 1 )
#endif  // LSB_FIRST

//-- CPU_BYTE_ORDER enumeration
// big endian byte ordering
#ifndef HIGH_BYTE_FIRST
    #define HIGH_BYTE_FIRST ( 0 )
#endif  // HIGH_BYTE_FIRST
// little endian byte ordering
#ifndef LOW_BYTE_FIRST
    #define LOW_BYTE_FIRST  ( 1 )
#endif  // LOW_BYTE_FIRST

//-- Declare boolean constants: TRUE, FALSE
#ifndef TRUE
    #define TRUE    ( (boolean) 1 )
#endif  // TRUE
#ifndef FALSE
   #define FALSE    ( (boolean) 0 )
#endif  // FALSE

//-- Platform specific declarations
#define CPU_TYPE        CPU_TYPE_32
#define CPU_BIT_ORDER   LSB_FIRST
#define CPU_BYTE_ORDER  LOW_BYTE_FIRST


//----------------------------------------------------------------------------------------------------------------------
// Exported type
//----------------------------------------------------------------------------------------------------------------------
//                                     TRUE .. FALSE
typedef unsigned char       boolean;
//                                     -128 .. +127
typedef signed char         sint8;
//                                   -32768 .. +32767
typedef signed short        sint16;
//                              -2147483648 .. +2147483647
typedef signed long        sint32;
//                                        0 .. 255
typedef unsigned char       uint8;
//                                        0 .. 65535
typedef unsigned short      uint16;
//                                        0 .. 4294967295
typedef unsigned long       uint32;

typedef signed int      sint8_least;
typedef signed int      sint16_least;
typedef signed int      sint32_least;
typedef unsigned int    uint8_least;
typedef unsigned int    uint16_least;
typedef unsigned int    uint32_least;

typedef float           float32;
typedef double          float64;


//----------------------------------------------------------------------------------------------------------------------
// Exported data
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// Constant exported data
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// Exported Macros
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// Exported functions
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
#endif  // I_PLATFORM_TYPES_H
