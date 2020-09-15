/************************************************************************************************************************
** File Name      :  app_alarm_manager.c
** Summary        :  Manage The alarm
** Create         :  2016-10-21 
** Author         :  Sun Yuewu
** Update History :
** NO.     Author         Date                  Content
** 001.   Sun Yuewu  2016-10-21      new
************************************************************************************************************************/
#ifndef STD_TYPE_H
#define STD_TYPE_H

/* ============================ Version =============================================================================== */
#define STD_TYPE_H_MAJOR                (1)
#define STD_TYPE_H_MINOR                (0)
#define STD_TYPE_H_PATCH                (0)

/* ============================ Include Files ========================================================================= */
/* ============================ Type Definitions ====================================================================== */
/* data type */
#ifndef __TYPEDEF__
typedef unsigned char   BOOL;               /* Logical Data Type */
typedef signed char     SINT8;              /* Signed Integer[8 bit] */
typedef unsigned char   UINT8;              /* Unsigned Integer[8 bit] */
typedef signed short    SINT16;             /* Signed Integer[16 bit] */
typedef unsigned short  UINT16;             /* Unsigned Integer[16 bit] */
typedef signed long     SINT32;             /* Signed Integer[32 bit] */
typedef unsigned long   UINT32;             /* Unsigned Integer[32 bit] */

typedef unsigned char       bool_t;
typedef signed char         int8_t;
typedef unsigned char       uint8_t;
typedef signed short        int16_t;
typedef unsigned short      uint16_t;
typedef signed long         int32_t;
typedef unsigned long       uint32_t;
typedef unsigned short      MD_STATUS;
#define __TYPEDEF__
#endif

/* Version Information */
typedef struct tagStd_VersionInfoType
{
    UINT16  vendorID;                       /* Vendor Identification */
    UINT16  moduleID;                       /* Module Identification */
    UINT8   sw_major_version;               /* Software Major Version */
    UINT8   sw_minor_version;               /* Software Minor Version */
    UINT8   sw_patch_version;               /* Software Patch Version */
}Std_VersionInfoType_t_;

/* ============================ Defines =============================================================================== */
                                            /* Logical True */
#ifndef TRUE
    #define TRUE    (1)
#endif
                                            /* Logical False */
#ifndef FALSE
    #define FALSE   (0)
#endif
                                            /* Empty Pointer */
#ifndef NULL
    #define NULL    ((void *)0)
#endif

#ifndef STD_INT_ZERO
    #define STD_INT_ZERO            (0)
#endif


#if 0

                                            /* Physical state 5V or 3.3V */
#ifndef STD_HIGH                                           
    #define STD_HIGH    (0x01)
#endif
                                            /* Physical state 0V */
#ifndef STD_LOW                                            
    #define STD_LOW     (0x00)
#endif    
                                            /* Logical state active */
#ifndef STD_ACTIVE
    #define STD_ACTIVE  (0x01)
#endif    
                                            /* Logical state idle */
#ifndef STD_IDLE
    #define STD_IDLE    (0x00)
#endif    
                                            /* Status Definition of On */
#ifndef STD_ON
    #define STD_ON      (0x01)
#endif    
                                            /* Status Definition of Off */
#ifndef STD_OFF
    #define STD_OFF     (0x00)
#endif    
                                            /* Direction of Port[IN] */
#ifndef STD_IN
    #define STD_IN      (0x00)
#endif    
                                            /* Direction of Port[OUT] */
#ifndef STD_OUT
    #define STD_OUT     (0x01)
#endif    
                                            /* Integer Minus One */

                                            
#ifndef STD_INT_MINUS_ONE
    #define STD_INT_MINUS_ONE       (-1)
#endif
                                            /* Integer Positive Zero */

                                            /* Integer Positive One */
#ifndef STD_INT_ONE
    #define STD_INT_ONE             (1)
#endif
                                            /* Integer Positive Two */
#ifndef STD_INT_TWO
    #define STD_INT_TWO             (2)
#endif
                                            /* Integer Positive Three */
#ifndef STD_INT_THREE
    #define STD_INT_THREE           (3)
#endif
                                            /* Integer Positive Three */
#ifndef STD_INT_FOUR
    #define STD_INT_FOUR           (4)
#endif
                                            /* Integer Positive Four */
#ifndef STD_INT_SIX
    #define STD_INT_SIX           (6)
#endif
                                            /* Integer Positive Four */
#ifndef STD_INT_EIGHT
    #define STD_INT_EIGHT           (8)
#endif

#ifndef STD_INT_TEN
    #define STD_INT_TEN           (10)
#endif


#ifndef STD_INT_FF
    #define STD_INT_FF              (0x55)
#endif

#endif

#if 0

                                            /* Should Be Considered */
#define STD_SHOULD_BE_CONSIDERED(v)
                                            /* Input Argument Prefix */
#define STD_ARG_IN
                                            /* Output Argument Prefix */
#define STD_ARG_OUT
                                            /* Public IF Declare Area */
#define STD_PUBLIC_IF_DECLARE
                                            /* Private IF Declare Area */
#define STD_PRIVATE_IF_DECLARE
                                            /* Public IF Implement Area */
#define STD_PUBLIC_IF_IMPLEMENT
                                            /* Private IF Implement Area */
#define STD_PRIVATE_IF_IMPLEMENT
                                            /* Default Value Should Be v */
#define STD_DEFAULT_VALUE(v)

#define STD_BOOL_MIN    (0)
#define STD_BOOL_MAX    (1)

#define STD_UINT8_MIN   (0)
#define STD_UINT8_MAX   (255)

#define STD_UINT16_MIN   (0)
#define STD_UINT16_MAX   (65535)

#define STD_UINT32_MIN   (0)
#define STD_UINT32_MAX   (4294967295UL)

#define STD_SINT8_MIN   (-128)
#define STD_SINT8_MAX   (127)

#define STD_SINT16_MIN   (-32768)
#define STD_SINT16_MAX   (32767)

#define STD_SINT32_MIN   (-2147483648L)
#define STD_SINT32_MAX   (2147483647L)

#define STD_SHIFT_00    (0)
#define STD_SHIFT_01    (1)
#define STD_SHIFT_02    (2)
#define STD_SHIFT_03    (3)
#define STD_SHIFT_04    (4)
#define STD_SHIFT_05    (5)
#define STD_SHIFT_06    (6)
#define STD_SHIFT_07    (7)
#define STD_SHIFT_08    (8)
#define STD_SHIFT_09    (9)
#define STD_SHIFT_10    (10)
#define STD_SHIFT_11    (11)
#define STD_SHIFT_12    (12)
#define STD_SHIFT_13    (13)
#define STD_SHIFT_14    (14)
#define STD_SHIFT_15    (15)
#define STD_SHIFT_16    (16)
#define STD_SHIFT_17    (17)
#define STD_SHIFT_18    (18)
#define STD_SHIFT_19    (19)
#define STD_SHIFT_20    (20)
#define STD_SHIFT_21    (21)
#define STD_SHIFT_22    (22)
#define STD_SHIFT_23    (23)
#define STD_SHIFT_24    (24)
#define STD_SHIFT_25    (25)
#define STD_SHIFT_26    (26)
#define STD_SHIFT_27    (27)
#define STD_SHIFT_28    (28)
#define STD_SHIFT_29    (29)
#define STD_SHIFT_30    (30)
#define STD_SHIFT_31    (31)
#define STD_SHIFT_32    (32)

/* ============================ Macro ================================================================================= */
/* Constraint */
#define STD_CONSTRAINT(v, vmin, vmax) (((v) <= (vmin)) ? (vmin) : (((v) >= (vmax)) ? (vmax) : (v)))
/* Maximum of Two Value */
#define STD_MAX(v1, v2) (((v1) <= (v2)) ? (v2) : (v1))
/* Minimum of Two Value */
#define STD_MIN(v1, v2) (((v1) >= (v2)) ? (v2) : (v1))

#endif

#ifndef REG_RPOCESS
#define REG_PROCESS
#define REG_READ(REG)           (REG)
#define REG_WRITE(REG, NUM)     ((REG) = (NUM))
#endif

#define DEBUG_OUT(info)

#endif /* STD_TYPE_H */
