/* ===========================================================================

                     CONFIDENTIAL VISTEON CORPORATION

  This is an unpublished work of authorship, which contains trade secrets,
  created in 2006.  Visteon Corporation owns all rights to this work and
  intends to maintain it in confidence to preserve its trade secret status.
  Visteon Corporation reserves the right, under the copyright laws of the
  United States or those of any other country that may have jurisdiction, to
  protect this work as an unpublished work, in the event of an inadvertent
  or deliberate unauthorized publication.  Visteon Corporation also reserves
  its rights under all copyright laws to protect this work as a published
  work, when appropriate.  Those having access to this work may not copy it,
  use it, modify it or disclose the information contained in it without the
  written authorization of Visteon Corporation.

 =========================================================================*/

/* ===========================================================================

  Name:           Can_Il_Par_Cfg.c

  Description:    CAN Interaction Layer Tx and Rx Parameters Configurations

                  Application Specific Tx and Rx Message and Signal
                  Data Structure Definitions

  Organization:   Multiplex Subsystems

 =========================================================================*/

/* ===========================================================================
  I N C L U D E   F I L E S
 =========================================================================*/ 

//#ifdef CANIL_IFSUPPORT

#include "Can_GeneralTypes.h"
#include <CclStack_Types.h>

//#endif

#include "CanIl_Util.h"
#include "CanIl.h"
#include "CanIl_Par_Cfg.h" 

CAN_UINT16 Can_Ch0_Il_Tx_Periodic_Count[ Can_Ch0_Il_Tx_Num_Periodic ];

#ifdef CAN_IL_TX_BURST_MODE

 #if ( Can_Ch0_Il_Tx_Num_Burst_Periodic > 0 )

CAN_UINT8 Can_Ch0_Il_Tx_Burst_Count[ Can_Ch0_Il_Tx_Num_Burst_Periodic ];

 #endif

#endif

/* ===========================================================================
  M E M O R Y   A L L O C A T I O N
 =========================================================================*/
/* ===========================================================================
  Interaction Layer Transmit Frame Status and Data Storage
 =========================================================================*/
CAN_UINT8 Can_Ch0_Il_Tx_Frame_Data[ Can_Ch0_Il_Tx_Num_Messages ][ CAN_MAX_DATA_LENGTH ];


CAN_UINT8 Can_Ch0_Il_Tx_Frame_Status[ Can_Ch0_Il_Tx_Num_Messages ];


CAN_UINT16 Can_Ch0_Il_Tx_Delay_Count[ Can_Ch0_Il_Tx_Num_Messages ]; 

/* ===========================================================================
  TRANSMIT DATA STRUCTURES
 =========================================================================*/

/* ===========================================================================
  Interaction Layer Transmit Signal Descriptors

 The Following Table Definition Assumes a Motorola (Big Endian) Byte
 Ordering of Bytes within the CAN Frame. As an example, if a 16bit signal
 is defined in Bytes 0 and 1 of the CAN frame, the MSByte is Byte 0 and the
 LSByte is Byte 1.

 =========================================================================*/
CAN_IL_SIGNAL const
Can_Ch0_Il_Tx_Signals[ Can_Ch0_Il_Tx_Num_Signals ] ={\
/*  CAN Frame,                                Num Bytes,       MS Byte,     MS Bit,          LS Byte,       LS Bit   */
{  Can_Ch0_Il_Tx_Msg_IC_0x386_TMH,                1,             0,             7,             0,             7  },  /*IC_BSDSoundSts*/   /*IC_0x386*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x392_TMH,                1,             0,             7,             0,             1  },  /*IC_Fuel_level*/   /*IC_0x392*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x392_TMH,                1,             1,             7,             1,             1  },  /*IC_DriveTimehour*/   /*IC_0x392*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x392_TMH,                1,             2,             7,             2,             2  },  /*IC_DriveTimeMinute*/   /*IC_0x392*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x392_TMH,                1,             3,             7,             3,             0  },  /*IC_AverageSpeed*/   /*IC_0x392*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x392_TMH,                2,             4,             7,             5,             1  },  /*IC_ChangOilDistance*/   /*IC_0x392*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x510_TMH,                3,             0,             7,             2,             0  },  /*IC_OdometerMasterValue*/   /*IC_0x510*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x510_TMH,                1,             3,             0,             4,             6  },  /*IC_AirbagTelltaleBehavior*/   /*IC_0x510*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x510_TMH,                1,             3,             5,             3,             5  },  /*IC_DISFail*/   /*IC_0x510*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x510_TMH,                1,             3,             7,             3,             6  },  /*IC_QDashACCFail*/   /*IC_0x510*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x510_TMH,                2,             4,             5,             5,             5  },  /*IC_VehSpd_HMI*/   /*IC_0x510*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x510_TMH,                1,             6,             3,             6,             0  },  /*IC_Rolling_counter_0x510*/   /*IC_0x510*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x510_TMH,                1,             7,             7,             7,             0  },  /*IC_Checksum_0x510*/   /*IC_0x510*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x525_TMH,                2,             0,             7,             1,             7  },  /*IC_AFC*/   /*IC_0x525*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x525_TMH,                2,             1,             3,             2,             0  },  /*IC_DTE*/   /*IC_0x525*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x525_TMH,                1,             1,             5,             1,             4  },  /*IC_AlarmStatus*/   /*IC_0x525*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x525_TMH,                1,             1,             6,             1,             6  },  /*IC_AlarmOrNot*/   /*IC_0x525*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x525_TMH,                1,             3,             1,             3,             0  },  /*IC_ManRgen_D_Rq*/   /*IC_0x525*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x525_TMH,                1,             3,             4,             3,             2  },  /*IC_AFC_Type*/   /*IC_0x525*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x525_TMH,                2,             4,             7,             5,             3  },  /*IC_Fuel_Level_Volumn*/   /*IC_0x525*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x525_TMH,                1,             5,             2,             5,             0  },  /*IC_Fuel_Level_VolumnQF*/   /*IC_0x525*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x525_TMH,                1,             6,             3,             6,             0  },  /*IC_Rolling_counter_0x525*/   /*IC_0x525*/\
{  Can_Ch0_Il_Tx_Msg_IC_0x525_TMH,                1,             7,             7,             7,             0  },  /*IC_Checksum_0x525*/   /*IC_0x525*/\
{  Can_Ch0_Il_Tx_Msg_IC_time_0x581_TMH,           1,             0,             7,             0,             0  },  /*IC_Year*/   /*IC_time_0x581*/\
{  Can_Ch0_Il_Tx_Msg_IC_time_0x581_TMH,           1,             1,             7,             1,             0  },  /*IC_Month*/   /*IC_time_0x581*/\
{  Can_Ch0_Il_Tx_Msg_IC_time_0x581_TMH,           1,             2,             7,             2,             0  },  /*IC_Date*/   /*IC_time_0x581*/\
{  Can_Ch0_Il_Tx_Msg_IC_time_0x581_TMH,           1,             3,             7,             3,             0  },  /*IC_Hour*/   /*IC_time_0x581*/\
{  Can_Ch0_Il_Tx_Msg_IC_time_0x581_TMH,           1,             4,             7,             4,             0  },  /*IC_Minute*/   /*IC_time_0x581*/\
{  Can_Ch0_Il_Tx_Msg_IC_time_0x581_TMH,           1,             5,             7,             5,             0  },  /*IC_Second*/   /*IC_time_0x581*/\
};
/* ===========================================================================
  Interaction Layer Transmit Message Initialization Arrays

  The following transmit message initialization arrays define the initial
  values for all of the transmitted messages.

 =========================================================================*/

/*
 Sample Message Has 7 Signals, Each 1 Byte, for Initialization
 The order of the bytes in this array is as follows:
 Byte 0 - Signal 0, bit 1.7 Value = 0     C_SAMPLE*/
/*     IC_0x386   */
static CAN_UINT8 const Can_Ch0_Il_IC_0x386_Message_Init[1 ] = {0x00};
/*     IC_0x392   */
static CAN_UINT8 const Can_Ch0_Il_IC_0x392_Message_Init[6 ] = {0x00,0x00,0x00,0x00,0x00,0x00};
/*     IC_0x510   */
static CAN_UINT8 const Can_Ch0_Il_IC_0x510_Message_Init[10 ] = {0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/*     IC_0x525   */
static CAN_UINT8 const Can_Ch0_Il_IC_0x525_Message_Init[13 ] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/*     IC_time_0x581   */
static CAN_UINT8 const Can_Ch0_Il_IC_time_0x581_Message_Init[6 ] = {0x00,0x00,0x00,0x00,0x00,0x00};
/* ===========================================================================
  Interaction Layer Transmit Message Definition Table

  This table (array of transmit message data structures) defines each
  transmitted message. The data structure defines the number of signals in
  the message, the total number of bytes in the message, a pointer to the
  list of signal handles, and a pointer to the message initialization data
  bytes.

 =========================================================================*/

CAN_IL_TX_MESSAGE const
Can_Ch0_Il_Tx_Messages[ Can_Ch0_Il_Tx_Num_Messages ] =
 {
	/*IC_0x386_TMH*/
	{
        1,                              /* Number of Signals in the Message             */
        0,                               /* Total Number of Signal Bytes in the Message  */
        Can_Ch0_Il_IC_0x386_Message_Init       /* Pointer to the Initialization Data Bytes     */
	},
	/*IC_0x392_TMH*/
	{
        5,                              /* Number of Signals in the Message             */
        0,                               /* Total Number of Signal Bytes in the Message  */
        Can_Ch0_Il_IC_0x392_Message_Init       /* Pointer to the Initialization Data Bytes     */
	},
	/*IC_0x510_TMH*/
	{
        7,                              /* Number of Signals in the Message             */
        0,                               /* Total Number of Signal Bytes in the Message  */
        Can_Ch0_Il_IC_0x510_Message_Init       /* Pointer to the Initialization Data Bytes     */
	},
	/*IC_0x525_TMH*/
	{
        10,                              /* Number of Signals in the Message             */
        0,                               /* Total Number of Signal Bytes in the Message  */
        Can_Ch0_Il_IC_0x525_Message_Init       /* Pointer to the Initialization Data Bytes     */
	},
	/*IC_time_0x581_TMH*/
	{
        6,                              /* Number of Signals in the Message             */
        0,                               /* Total Number of Signal Bytes in the Message  */
        Can_Ch0_Il_IC_time_0x581_Message_Init       /* Pointer to the Initialization Data Bytes     */
	},
};

/* ===========================================================================
  Interaction Layer Transmit Message Data (TMD) Structures (Frame Definition)

  !!! IMPORTANT NOTE !!! The transmit message handles must be specified
  sequentially, starting with 0 (zero). These message handles serve as an
  index to the transmit complete function pointers, so each index must map
  to the correct transmit complete callback function pointer in the lookup
  table (array of function pointers) for servicing transmit complete events.

 =========================================================================*/

/*       IC_0x386     */
 static CAN_IL_TMD const Can_Ch0_Il_Tx_Message_IC_0x386_Tmd =
{
     CAN_GPNUM_8,                            /* CAN message data length  */
     Can_Ch0_Il_Tx_Frame_Data[ 0 ],           /* Pointer to Data          */
     Can_Ch0_Il_Tx_Msg_IC_0x386_TMH           /* Transmit Message Handle  */
};

/*       IC_0x392     */
 static CAN_IL_TMD const Can_Ch0_Il_Tx_Message_IC_0x392_Tmd =
{
     CAN_GPNUM_8,                            /* CAN message data length  */
     Can_Ch0_Il_Tx_Frame_Data[ 1 ],           /* Pointer to Data          */
     Can_Ch0_Il_Tx_Msg_IC_0x392_TMH           /* Transmit Message Handle  */
};

/*       IC_0x510     */
 static CAN_IL_TMD const Can_Ch0_Il_Tx_Message_IC_0x510_Tmd =
{
     CAN_GPNUM_8,                            /* CAN message data length  */
     Can_Ch0_Il_Tx_Frame_Data[ 2 ],           /* Pointer to Data          */
     Can_Ch0_Il_Tx_Msg_IC_0x510_TMH           /* Transmit Message Handle  */
};

/*       IC_0x525     */
 static CAN_IL_TMD const Can_Ch0_Il_Tx_Message_IC_0x525_Tmd =
{
     CAN_GPNUM_8,                            /* CAN message data length  */
     Can_Ch0_Il_Tx_Frame_Data[ 3 ],           /* Pointer to Data          */
     Can_Ch0_Il_Tx_Msg_IC_0x525_TMH           /* Transmit Message Handle  */
};

/*       IC_time_0x581     */
 static CAN_IL_TMD const Can_Ch0_Il_Tx_Message_IC_time_0x581_Tmd =
{
     CAN_GPNUM_8,                            /* CAN message data length  */
     Can_Ch0_Il_Tx_Frame_Data[ 4 ],           /* Pointer to Data          */
     Can_Ch0_Il_Tx_Msg_IC_time_0x581_TMH           /* Transmit Message Handle  */
};
/* ===========================================================================
  Interaction Layer Periodic Transmit Table

  This table is an array of data structures that define the periodic
  transmit characteristics for messages that are transmitted periodically.
  Care must be taken so that the each periodic message defined in the
  Interaction Layer Transmit Frame table map correctly to this table so
  that the correct periodic message attributes, and the pointer to the
  periodic timer, is correctly retrieved.

 =========================================================================*/
static CAN_IL_TX_PERIODIC const
Can_Ch0_Il_Tx_Periodic[ Can_Ch0_Il_Tx_Num_Periodic ] =
{
 /* IC_0x386  Message */
 {
     IL_TIME_IN_TASK_TICS( 100) ,                                   /* Primary Period in Task Tics  */
     IL_TIME_IN_TASK_TICS( 10),                                     /* Offset Delay in Task Tics    */
     &Can_Ch0_Il_Tx_Periodic_Count[ 0 ]                         /* Pointer to Periodic Count    */
 },
 /* IC_0x392  Message */
 {
     IL_TIME_IN_TASK_TICS( 100) ,                                   /* Primary Period in Task Tics  */
     IL_TIME_IN_TASK_TICS( 20),                                     /* Offset Delay in Task Tics    */
     &Can_Ch0_Il_Tx_Periodic_Count[ 1 ]                         /* Pointer to Periodic Count    */
 },
 /* IC_0x510  Message */
 {
     IL_TIME_IN_TASK_TICS( 100) ,                                   /* Primary Period in Task Tics  */
     IL_TIME_IN_TASK_TICS( 30),                                     /* Offset Delay in Task Tics    */
     &Can_Ch0_Il_Tx_Periodic_Count[ 2 ]                         /* Pointer to Periodic Count    */
 },
 /* IC_0x525  Message */
 {
     IL_TIME_IN_TASK_TICS( 100) ,                                   /* Primary Period in Task Tics  */
     IL_TIME_IN_TASK_TICS( 40),                                     /* Offset Delay in Task Tics    */
     &Can_Ch0_Il_Tx_Periodic_Count[ 3 ]                         /* Pointer to Periodic Count    */
 },
 /* IC_time_0x581  Message */
 {
     IL_TIME_IN_TASK_TICS( 500) ,                                   /* Primary Period in Task Tics  */
     IL_TIME_IN_TASK_TICS( 50),                                     /* Offset Delay in Task Tics    */
     &Can_Ch0_Il_Tx_Periodic_Count[ 4 ]                         /* Pointer to Periodic Count    */
 },
};
/* ===========================================================================
  Interaction Layer Burst Periodic Transmit Table
 =========================================================================*/

/* ===========================================================================
  Interaction Layer Transmit Frame Table

  Each entry in this table defines the attributes for a specific transmit
  frame transmitted by the Interaction Layer.

 =========================================================================*/

CAN_IL_TX_FRAME const Can_Ch0_Il_Tx_Frame_Table[ Can_Ch0_Il_Tx_Num_Messages ] = 
{
 {
 /* IC_0x386 Message (CAN periodic, data change or a request ) */
     (IL_TX_ATTR_PERIODIC | IL_TX_ATTR_TXC_NOTIFY),   /* Frame Transmission Attributes                */
     &Can_Ch0_Il_Tx_Frame_Status[ 0 ],       /* Pointer to the Frame Status Variable         */
     Can_Ch0_Il_Tx_Frame_Data[ 0 ],          /* Pointer to the Transmitted Frame Data        */
     &Can_Ch0_Il_Tx_Delay_Count[ 0 ],        /* Pointer to the Transmit Delay Count          */ 
     IL_TIME_IN_TASK_TICS( 0 ),                   /* Minimum Transmit Delay in Timer Tics         */
     &Can_Ch0_Il_Tx_Periodic[ 0 ],           /* Pointer to the Periodic Attributes (or NULL ) */ 
     NULL,                                        /* Ptr to Burst Periodic Attributes (or NULL)   */
     &Can_Ch0_Il_Tx_Message_IC_0x386_Tmd,  /* Pointer to CAN Driver TMD Data Structure     */
 },
 {
 /* IC_0x392 Message (CAN periodic, data change or a request ) */
     (IL_TX_ATTR_PERIODIC | IL_TX_ATTR_TXC_NOTIFY),   /* Frame Transmission Attributes                */
     &Can_Ch0_Il_Tx_Frame_Status[ 1 ],       /* Pointer to the Frame Status Variable         */
     Can_Ch0_Il_Tx_Frame_Data[ 1 ],          /* Pointer to the Transmitted Frame Data        */
     &Can_Ch0_Il_Tx_Delay_Count[ 1 ],        /* Pointer to the Transmit Delay Count          */ 
     IL_TIME_IN_TASK_TICS( 0 ),                   /* Minimum Transmit Delay in Timer Tics         */
     &Can_Ch0_Il_Tx_Periodic[ 1 ],           /* Pointer to the Periodic Attributes (or NULL ) */ 
     NULL,                                        /* Ptr to Burst Periodic Attributes (or NULL)   */
     &Can_Ch0_Il_Tx_Message_IC_0x392_Tmd,  /* Pointer to CAN Driver TMD Data Structure     */
 },
 {
 /* IC_0x510 Message (CAN periodic, data change or a request ) */
     (IL_TX_ATTR_PERIODIC | IL_TX_ATTR_TXC_NOTIFY),   /* Frame Transmission Attributes                */
     &Can_Ch0_Il_Tx_Frame_Status[ 2 ],       /* Pointer to the Frame Status Variable         */
     Can_Ch0_Il_Tx_Frame_Data[ 2 ],          /* Pointer to the Transmitted Frame Data        */
     &Can_Ch0_Il_Tx_Delay_Count[ 2 ],        /* Pointer to the Transmit Delay Count          */ 
     IL_TIME_IN_TASK_TICS( 0 ),                   /* Minimum Transmit Delay in Timer Tics         */
     &Can_Ch0_Il_Tx_Periodic[ 2 ],           /* Pointer to the Periodic Attributes (or NULL ) */ 
     NULL,                                        /* Ptr to Burst Periodic Attributes (or NULL)   */
     &Can_Ch0_Il_Tx_Message_IC_0x510_Tmd,  /* Pointer to CAN Driver TMD Data Structure     */
 },
 {
 /* IC_0x525 Message (CAN periodic, data change or a request ) */
     (IL_TX_ATTR_PERIODIC | IL_TX_ATTR_TXC_NOTIFY),   /* Frame Transmission Attributes                */
     &Can_Ch0_Il_Tx_Frame_Status[ 3 ],       /* Pointer to the Frame Status Variable         */
     Can_Ch0_Il_Tx_Frame_Data[ 3 ],          /* Pointer to the Transmitted Frame Data        */
     &Can_Ch0_Il_Tx_Delay_Count[ 3 ],        /* Pointer to the Transmit Delay Count          */ 
     IL_TIME_IN_TASK_TICS( 0 ),                   /* Minimum Transmit Delay in Timer Tics         */
     &Can_Ch0_Il_Tx_Periodic[ 3 ],           /* Pointer to the Periodic Attributes (or NULL ) */ 
     NULL,                                        /* Ptr to Burst Periodic Attributes (or NULL)   */
     &Can_Ch0_Il_Tx_Message_IC_0x525_Tmd,  /* Pointer to CAN Driver TMD Data Structure     */
 },
 {
 /* IC_time_0x581 Message (CAN periodic, data change or a request ) */
     (IL_TX_ATTR_PERIODIC | IL_TX_ATTR_TXC_NOTIFY),   /* Frame Transmission Attributes                */
     &Can_Ch0_Il_Tx_Frame_Status[ 4 ],       /* Pointer to the Frame Status Variable         */
     Can_Ch0_Il_Tx_Frame_Data[ 4 ],          /* Pointer to the Transmitted Frame Data        */
     &Can_Ch0_Il_Tx_Delay_Count[ 4 ],        /* Pointer to the Transmit Delay Count          */ 
     IL_TIME_IN_TASK_TICS( 0 ),                   /* Minimum Transmit Delay in Timer Tics         */
     &Can_Ch0_Il_Tx_Periodic[ 4 ],           /* Pointer to the Periodic Attributes (or NULL ) */ 
     NULL,                                        /* Ptr to Burst Periodic Attributes (or NULL)   */
     &Can_Ch0_Il_Tx_Message_IC_time_0x581_Tmd,  /* Pointer to CAN Driver TMD Data Structure     */
 },
};
/* ===========================================================================
  R E C E I V E   D A T A   S T R U C T U R E S
 =========================================================================*/
/* ===========================================================================
  Interaction Layer Receive Signal Descriptors

  This data structure defines each received signal, include the specific
  received CAN frame (sequentially enumerated) in which the signal resides,
  and the specific location of the signal within the CAN frame. The
  signal is assumed to span the bits in the frame from the MSByte.MSBit to
  the LSByte.LSBit. As an example, a 16 bit (2 Byte) wide signal that
  occupies the first two bytes (Byte0, Byte 1) of a CAN frame is specified
  as having endpoints at (Byte 0, Bit 7) and (Byte 1, Bit 0). This table
  definition assumes a Motorola (Big Endian) ordering of bytes within the
  CAN Frame.

 =========================================================================*/
/* Receive messages */
CAN_IL_SIGNAL const
Can_Ch0_Il_Rx_Signals[ Can_Ch0_Il_Rx_Num_Signals ] =
{ 
/*  CAN Frame,                                               Num Bytes,      MS Byte,    MS Bit,     LS Byte,    LS Bit */
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x10B,                    1,             0,             7,             0,             6  },/*0*/  /*EMS_BrkPedalStasus*/   /*GW_EMS_Power_0x10B*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x10B,                    1,             1,             7,             1,             0  },/*1*/  /*EMS_EngCoolanTemp*/   /*GW_EMS_Power_0x10B*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x10B,                    1,             3,             2,             3,             0  },/*2*/  /*EMS_EngOperationStatus*/   /*GW_EMS_Power_0x10B*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x10B,                    1,             3,             5,             3,             5  },/*3*/  /*EMS_EngSpeedError*/   /*GW_EMS_Power_0x10B*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x10B,                    1,             3,             7,             3,             7  },/*4*/  /*EMS_BatteryVoltageError*/   /*GW_EMS_Power_0x10B*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x10B,                    2,             4,             7,             5,             1  },/*5*/  /*EMS_EngSpeed*/   /*GW_EMS_Power_0x10B*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x10B,                    1,             6,             7,             6,             6  },/*6*/  /*EMS_CluthPedalStatus*/   /*GW_EMS_Power_0x10B*/
{  Can_Ch0_Il_Rx_Msg_GW_TCU_GearInfo_Power_0x123,           1,             1,             3,             1,             0  },/*7*/  /*TCU_DisplayGear*/   /*GW_TCU_GearInfo_Power_0x123*/
{  Can_Ch0_Il_Rx_Msg_GW_TCU_GearInfo_Power_0x123,           1,             1,             7,             1,             4  },/*8*/  /*TCU_GearSelectorReq*/   /*GW_TCU_GearInfo_Power_0x123*/
{  Can_Ch0_Il_Rx_Msg_GW_TCU_GearInfo_Power_0x123,           1,             4,             7,             4,             4  },/*9*/  /*TCU_StGearMode*/   /*GW_TCU_GearInfo_Power_0x123*/
{  Can_Ch0_Il_Rx_Msg_GW_TCU_GearInfo_Power_0x123,           1,             6,             3,             6,             0  },/*10*/  /*TCU_Status_AliveCounter*/   /*GW_TCU_GearInfo_Power_0x123*/
{  Can_Ch0_Il_Rx_Msg_GW_TCU_GearInfo_Power_0x123,           1,             7,             7,             7,             0  },/*11*/  /*TCU_Status_Checksum*/   /*GW_TCU_GearInfo_Power_0x123*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Sts_Power_0x151,                2,             4,             0,             5,             0  },/*12*/  /*EMS_TargCruiseSpeed*/   /*GW_EMS_Sts_Power_0x151*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Sts_Power_0x151,                1,             6,             3,             6,             0  },/*13*/  /*EMS_Rolling_counter_0x151*/   /*GW_EMS_Sts_Power_0x151*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Sts_Power_0x151,                1,             6,             5,             6,             4  },/*14*/  /*EMS_CruiseControlStatus*/   /*GW_EMS_Sts_Power_0x151*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Sts_Power_0x151,                1,             7,             7,             7,             0  },/*15*/  /*EMS_Checksum_0x151*/   /*GW_EMS_Sts_Power_0x151*/
{  Can_Ch0_Il_Rx_Msg_PEPS_Start_0x1A5,                      1,             0,             7,             0,             6  },/*16*/  /*PEPS_Remotestartstatus*/   /*PEPS_Start_0x1A5*/
{  Can_Ch0_Il_Rx_Msg_PEPS_Start_0x1A5,                      1,             6,             3,             6,             0  },/*17*/  /*PEPS_RollingCounter_0x1A5*/   /*PEPS_Start_0x1A5*/
{  Can_Ch0_Il_Rx_Msg_PEPS_Start_0x1A5,                      1,             7,             7,             7,             0  },/*18*/  /*PEPS_Checksum_0x1A5*/   /*PEPS_Start_0x1A5*/
{  Can_Ch0_Il_Rx_Msg_GW_ABS_Sts_0x221,                      2,             0,             7,             1,             0  },/*19*/  /*ABS_WhlMilgFrntLe*/   /*GW_ABS_Sts_0x221*/
{  Can_Ch0_Il_Rx_Msg_GW_ABS_Sts_0x221,                      2,             2,             7,             3,             0  },/*20*/  /*ABS_WhlMilgFrntRi*/   /*GW_ABS_Sts_0x221*/
{  Can_Ch0_Il_Rx_Msg_GW_ABS_Sts_0x221,                      2,             4,             3,             5,             0  },/*21*/  /*ABS_VehSpdLgt*/   /*GW_ABS_Sts_0x221*/
{  Can_Ch0_Il_Rx_Msg_GW_ABS_Sts_0x221,                      1,             4,             5,             4,             5  },/*22*/  /*ABS_EbdFlgFlt*/   /*GW_ABS_Sts_0x221*/
{  Can_Ch0_Il_Rx_Msg_GW_ABS_Sts_0x221,                      1,             4,             6,             4,             6  },/*23*/  /*ABS_AbsFlgFlt*/   /*GW_ABS_Sts_0x221*/
{  Can_Ch0_Il_Rx_Msg_GW_ABS_Sts_0x221,                      1,             6,             3,             6,             0  },/*24*/  /*ABS_RollingCounter_0x221*/   /*GW_ABS_Sts_0x221*/
{  Can_Ch0_Il_Rx_Msg_GW_ABS_Sts_0x221,                      1,             6,             4,             6,             4  },/*25*/  /*ABS_WhlMilgFrntLeStatus*/   /*GW_ABS_Sts_0x221*/
{  Can_Ch0_Il_Rx_Msg_GW_ABS_Sts_0x221,                      1,             6,             5,             6,             5  },/*26*/  /*ABS_WhlMilgFrntRiStatus*/   /*GW_ABS_Sts_0x221*/
{  Can_Ch0_Il_Rx_Msg_GW_ABS_Sts_0x221,                      1,             7,             7,             7,             0  },/*27*/  /*ABS_Checksum_0x221*/   /*GW_ABS_Sts_0x221*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             0,             5,             0,             3  },/*28*/  /*ESP_EPBStatus*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             0,             7,             0,             6  },/*29*/  /*ESP_EPBErrorStatus*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             1,             6,             1,             6  },/*30*/  /*ESP_CdpAvailable*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             2,             1,             2,             1  },/*31*/  /*ESP_EPBWarningMessage3*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             2,             2,             2,             2  },/*32*/  /*ESP_EPBWarningMessage2*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             2,             3,             2,             3  },/*33*/  /*ESP_EPBWarningMessage1*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             2,             4,             2,             4  },/*34*/  /*ESP_AVHWarningMessage*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             2,             5,             2,             5  },/*35*/  /*ESP_AvhActive*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             2,             7,             2,             7  },/*36*/  /*ESP_AvhAvabliable*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             4,             0,             4,             0  },/*37*/  /*ESP_VdcFlgFlt*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             4,             1,             4,             1  },/*38*/  /*ESP_TcsFlgFlt*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             5,             4,             5,             4  },/*39*/  /*ESP_ESPInfoLamp*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             5,             5,             5,             5  },/*40*/  /*ESP_EspEnableSts*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             6,             3,             6,             0  },/*41*/  /*ESP_RollingCounter_0x243*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_ESP_Status_Chassis_0x243,           1,             7,             7,             7,             0  },/*42*/  /*ESP_Checksum_0x243*/   /*GW_ESP_Status_Chassis_0x243*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             1,             0,             2,             6  },/*43*/  /*IPM_LaneAssitType*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             1,             1,             1,             1  },/*44*/  /*IPM_LaneAssit_WarningVibration*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             2,             1,             2,             1  },/*45*/  /*IPM_LaneAssit_WarningAudio*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             2,             3,             2,             2  },/*46*/  /*IPM_LaneAssit_LeftVisualization*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             2,             5,             2,             4  },/*47*/  /*IPM_LaneAssit_RightVisualization*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             3,             1,             3,             0  },/*48*/  /*IPM_SLASpdlimitUnits*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             3,             4,             3,             2  },/*49*/  /*IPM_SLAState*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             3,             7,             3,             5  },/*50*/  /*IPM_LaneAssit_Status*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             4,             7,             4,             0  },/*51*/  /*IPM_SLASpdlimit*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             5,             2,             5,             0  },/*52*/  /*IPM_HMA_Status*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             5,             4,             5,             3  },/*53*/  /*IPM_DDD_IndexStatus*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             5,             7,             5,             5  },/*54*/  /*IPM_DDD_WarningLevel*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             6,             3,             6,             0  },/*55*/  /*Rolling_counter_0x245*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             6,             6,             6,             6  },/*56*/  /*IPM_LaneAssit_HandsonReq*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_IPM_0x245,                          1,             7,             7,             7,             0  },/*57*/  /*Checksum_0x245*/   /*GW_IPM_0x245*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  2,             0,             7,             1,             7  },/*58*/  /*vSetDis*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             1,             0,             1,             0  },/*59*/  /*MRR_ObjValid*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             1,             3,             1,             1  },/*60*/  /*MRR_TauGapSet*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             1,             6,             1,             4  },/*61*/  /*MRR_dxTarObj*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             2,             2,             2,             0  },/*62*/  /*ACCHMI_Mode*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             2,             3,             2,             3  },/*63*/  /*ACCFailInfo*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             2,             4,             2,             4  },/*64*/  /*MRR_TakeOverReq*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             3,             3,             3,             1  },/*65*/  /*MRR_AEB_STATE*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             3,             5,             3,             4  },/*66*/  /*ACC_Startstop_infor*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             3,             6,             3,             6  },/*67*/  /*MRR_PCW_preWarning*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             3,             7,             3,             7  },/*68*/  /*MRR_PCW_latentWarning*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             4,             3,             4,             3  },/*69*/  /*FCW_preWarning*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             4,             4,             4,             4  },/*70*/  /*FCW_latentWarning*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             4,             7,             4,             5  },/*71*/  /*MRR_PCW_STATE*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             5,             7,             5,             0  },/*72*/  /*Obj_Speed*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             6,             3,             6,             0  },/*73*/  /*Rolling_counter_0x246*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             6,             7,             6,             4  },/*74*/  /*Textinfo*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_MRR_Chassis_0x246,                  1,             7,             7,             7,             0  },/*75*/  /*Checksum_0x246*/   /*GW_MRR_Chassis_0x246*/
{  Can_Ch0_Il_Rx_Msg_GW_EPS_Chassis_0x260,                  1,             0,             6,             0,             6  },/*76*/  /*EPS_S_WarningLampYellow*/   /*GW_EPS_Chassis_0x260*/
{  Can_Ch0_Il_Rx_Msg_GW_EPS_Chassis_0x260,                  1,             0,             7,             0,             7  },/*77*/  /*EPS_S_SafeLampRed*/   /*GW_EPS_Chassis_0x260*/
{  Can_Ch0_Il_Rx_Msg_GW_EPS_Chassis_0x260,                  1,             6,             3,             6,             0  },/*78*/  /*Rolling_counter_0x260*/   /*GW_EPS_Chassis_0x260*/
{  Can_Ch0_Il_Rx_Msg_GW_EPS_Chassis_0x260,                  1,             7,             7,             7,             0  },/*79*/  /*Checksum_0x260*/   /*GW_EPS_Chassis_0x260*/
{  Can_Ch0_Il_Rx_Msg_GW_PAM_0x271,                          1,             6,             1,             6,             0  },/*80*/  /*PAM_ChimeCmdSoundStatus*/   /*GW_PAM_0x271*/
{  Can_Ch0_Il_Rx_Msg_GW_PAM_0x271,                          1,             6,             3,             6,             2  },/*81*/  /*PAM_RearSysTemWorkSts*/   /*GW_PAM_0x271*/
{  Can_Ch0_Il_Rx_Msg_GW_PAM_0x271,                          1,             6,             5,             6,             4  },/*82*/  /*PAM_FrontSysTemWorkSts*/   /*GW_PAM_0x271*/
{  Can_Ch0_Il_Rx_Msg_GW_PAM_0x271,                          1,             7,             3,             7,             0  },/*83*/  /*PAM_ChimeCmdSoundCadence*/   /*GW_PAM_0x271*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             0,             1,             0,             0  },/*84*/  /*SOD_LCA_warningReqRight*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             0,             3,             0,             2  },/*85*/  /*SOD_DOA_warningReqRightRear*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             0,             5,             0,             4  },/*86*/  /*SOD_DOA_warningReqRightFront*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             0,             7,             0,             6  },/*87*/  /*SOD_CTA_warningReqRight*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             1,             7,             1,             4  },/*88*/  /*SOD_BLIS_display*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             2,             1,             2,             0  },/*89*/  /*SOD_LCA_warningReqLeft*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             2,             3,             2,             2  },/*90*/  /*SOD_DOA_warningReqLeftRear*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             2,             5,             2,             4  },/*91*/  /*SOD_DOA_warningReqLeftFront*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             2,             7,             2,             6  },/*92*/  /*SOD_CTA_warningReqLeft*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             3,             5,             3,             5  },/*93*/  /*DOA_OFFTelltale*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             3,             6,             3,             6  },/*94*/  /*CTA_OFFTelltale*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             3,             7,             3,             7  },/*95*/  /*SODLCA_OFFTelltale*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             6,             3,             6,             0  },/*96*/  /*Rolling_counter_0x275*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_GW_SOD_0x275,                          1,             7,             7,             7,             0  },/*97*/  /*Checksum_0x275*/   /*GW_SOD_0x275*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             0,             1,             0,             0  },/*98*/  /*BCM_DoorStsRearRi*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             0,             3,             0,             2  },/*99*/  /*BCM_DoorStsRearLe*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             0,             5,             0,             4  },/*100*/  /*BCM_DoorStsFrntRi*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             0,             7,             0,             6  },/*101*/  /*BCM_DoorStsFrntLe*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             1,             1,             1,             0  },/*102*/  /*BCM_DoorLockStsRearRi*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             1,             3,             1,             2  },/*103*/  /*BCM_DoorLockStsRearLe*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             1,             5,             1,             4  },/*104*/  /*BCM_DoorLockStsFrntRi*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             1,             7,             1,             6  },/*105*/  /*BCM_DoorLockStsFrntLe*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             2,             1,             2,             0  },/*106*/  /*BCM_AntiTheftSts*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             2,             7,             2,             3  },/*107*/  /*BCM_LockUnlockFeedback*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             3,             2,             3,             1  },/*108*/  /*BCM_DoorLockStsTrunk*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             3,             4,             3,             3  },/*109*/  /*BCM_DoorStsTrunk*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             3,             5,             3,             5  },/*110*/  /*BCM_TrunkDoorPreUnlockSts*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             4,             7,             4,             6  },/*111*/  /*BCM_EnginehoodSts*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             6,             3,             6,             0  },/*112*/  /*BCM_Rolling_counter_0x310*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_Door_0x310,                        1,             7,             7,             7,             0  },/*113*/  /*BCM_Checksum_0x310*/   /*BCM_Door_0x310*/
{  Can_Ch0_Il_Rx_Msg_BCM_0x313,                             1,             0,             1,             0,             0  },/*114*/  /*BCM_OutsideTempDig*/   /*BCM_0x313*/
{  Can_Ch0_Il_Rx_Msg_BCM_0x313,                             1,             0,             3,             0,             2  },/*115*/  /*BCM_Mechanical_Hand_Brake*/   /*BCM_0x313*/
{  Can_Ch0_Il_Rx_Msg_BCM_0x313,                             1,             0,             7,             0,             6  },/*116*/  /*BCM_BrakeFluidSts*/   /*BCM_0x313*/
{  Can_Ch0_Il_Rx_Msg_BCM_0x313,                             1,             1,             7,             1,             0  },/*117*/  /*BCM_OutsideTemp*/   /*BCM_0x313*/
{  Can_Ch0_Il_Rx_Msg_BCM_0x313,                             1,             4,             7,             4,             6  },/*118*/  /*BCM_PowerMode*/   /*BCM_0x313*/
{  Can_Ch0_Il_Rx_Msg_BCM_0x313,                             1,             6,             3,             6,             0  },/*119*/  /*BCM_Rollingcounter_0x313*/   /*BCM_0x313*/
{  Can_Ch0_Il_Rx_Msg_BCM_0x313,                             1,             7,             7,             7,             0  },/*120*/  /*BCM_Checksum_0x313*/   /*BCM_0x313*/
{  Can_Ch0_Il_Rx_Msg_BCM_Sts_0x315,                         1,             5,             7,             5,             0  },/*121*/  /*BCM_BGLLumLvlSettingStatus*/   /*BCM_Sts_0x315*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             0,             1,             0,             0  },/*122*/  /*BCM_FrntFogLmpSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             0,             3,             0,             2  },/*123*/  /*BCM_PosLmpSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             0,             5,             0,             4  },/*124*/  /*BCM_LowBeamSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             0,             7,             0,             6  },/*125*/  /*BCM_HighBeamSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             1,             1,             1,             0  },/*126*/  /*BCM_reversinglightSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             1,             3,             1,             2  },/*127*/  /*BCM_DayRunningLampSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             1,             5,             1,             4  },/*128*/  /*BCM_TurnIndicatorSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             1,             7,             1,             6  },/*129*/  /*BCM_RearFogLmpSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             2,             5,             2,             4  },/*130*/  /*BCM_Highmountedstoplamp*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             2,             7,             2,             6  },/*131*/  /*BCM_BrakelightSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             3,             3,             3,             2  },/*132*/  /*BCM_BackHomeSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             3,             5,             3,             4  },/*133*/  /*BCM_AutolampSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             3,             7,             3,             6  },/*134*/  /*BCM_readinglampSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             4,             0,             4,             0  },/*135*/  /*BCM_DayRunningLampDig*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             4,             1,             4,             1  },/*136*/  /*BCM_BrakelightDig*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             4,             2,             4,             2  },/*137*/  /*BCM_reversinglightDig*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             4,             3,             4,             3  },/*138*/  /*BCM_TurnIndicatorDig*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             4,             4,             4,             4  },/*139*/  /*BCM_RearFogLmpDig*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             4,             6,             4,             6  },/*140*/  /*BCM_PosLmpDig*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             6,             3,             6,             0  },/*141*/  /*BCM_Rollingcounter_0x318*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             6,             7,             6,             6  },/*142*/  /*BCM_ImmoSts*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_BCM_Lamp_0x318,                        1,             7,             7,             7,             0  },/*143*/  /*BCM_Checksum_0x318*/   /*BCM_Lamp_0x318*/
{  Can_Ch0_Il_Rx_Msg_GW_SWM_Body_0x31A,                     1,             5,             1,             5,             0  },/*144*/  /*SWM_Menu_Right*/   /*GW_SWM_Body_0x31A*/
{  Can_Ch0_Il_Rx_Msg_GW_SWM_Body_0x31A,                     1,             5,             3,             5,             2  },/*145*/  /*SWM_Menu_Left*/   /*GW_SWM_Body_0x31A*/
{  Can_Ch0_Il_Rx_Msg_GW_SWM_Body_0x31A,                     1,             5,             5,             5,             4  },/*146*/  /*SWM_Menu_Down*/   /*GW_SWM_Body_0x31A*/
{  Can_Ch0_Il_Rx_Msg_GW_SWM_Body_0x31A,                     1,             5,             7,             5,             6  },/*147*/  /*SWM_Menu_Up*/   /*GW_SWM_Body_0x31A*/
{  Can_Ch0_Il_Rx_Msg_GW_SWM_Body_0x31A,                     1,             6,             3,             6,             0  },/*148*/  /*Rolling_counter_0x31A*/   /*GW_SWM_Body_0x31A*/
{  Can_Ch0_Il_Rx_Msg_GW_SWM_Body_0x31A,                     1,             6,             7,             6,             6  },/*149*/  /*SWM_Menu_Confirm*/   /*GW_SWM_Body_0x31A*/
{  Can_Ch0_Il_Rx_Msg_GW_SWM_Body_0x31A,                     1,             7,             7,             7,             0  },/*150*/  /*Checksum_0x31A*/   /*GW_SWM_Body_0x31A*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             0,             7,             0,             6  },/*151*/  /*PEPS_PowerMode*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             2,             3,             2,             2  },/*152*/  /*PEPS_IMMO_MatchStatus*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             3,             5,             3,             5  },/*153*/  /*PEPS_SystemFailStatus*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             3,             6,             3,             6  },/*154*/  /*PEPS_ESCL_FailStatus*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             3,             7,             3,             7  },/*155*/  /*PEPS_SSB_FailStatus*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             4,             0,             4,             0  },/*156*/  /*PEPS_ClutchIndicationWarning*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             4,             1,             4,             1  },/*157*/  /*PEPS_BrakeIndicationWarning*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             4,             2,             4,             2  },/*158*/  /*PEPS_NotPNWarning*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             4,             3,             4,             3  },/*159*/  /*PEPS_NotNWarning*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             5,             2,             5,             2  },/*160*/  /*PEPS_DoorOpenNotOFFWarning*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             5,             3,             5,             3  },/*161*/  /*PEPS_DoorOpenWarning*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             5,             4,             5,             4  },/*162*/  /*PEPS_KeyBatteryWarning*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             5,             5,             5,             5  },/*163*/  /*PEPS_KeyOutWarning1*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             5,             6,             5,             6  },/*164*/  /*PEPS_NoKeyFoundWarning*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             5,             7,             5,             7  },/*165*/  /*PEPS_KeyReminderWarning*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             6,             3,             6,             0  },/*166*/  /*PEPS_RollingCounter_0x325*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             6,             7,             6,             4  },/*167*/  /*PEPS_RemainingNumSts*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_PEPS_0x325,                            1,             7,             7,             7,             0  },/*168*/  /*PEPS_Checksum_0x325*/   /*PEPS_0x325*/
{  Can_Ch0_Il_Rx_Msg_RF_0x326,                              1,             0,             3,             0,             3  },/*169*/  /*RF_KeyLowBatteryWarning*/   /*RF_0x326*/
{  Can_Ch0_Il_Rx_Msg_GW_SRS_0x350,                          1,             0,             4,             0,             3  },/*170*/  /*SRS_SBR_PAMsenger*/   /*GW_SRS_0x350*/
{  Can_Ch0_Il_Rx_Msg_GW_SRS_0x350,                          1,             0,             6,             0,             5  },/*171*/  /*SRS_SBR_Driver*/   /*GW_SRS_0x350*/
{  Can_Ch0_Il_Rx_Msg_GW_SRS_0x350,                          1,             1,             0,             2,             7  },/*172*/  /*SRS_AirBagFailSts*/   /*GW_SRS_0x350*/
{  Can_Ch0_Il_Rx_Msg_GW_SRS_0x350,                          1,             1,             3,             1,             2  },/*173*/  /*SRS_SBR_SecondRowRight*/   /*GW_SRS_0x350*/
{  Can_Ch0_Il_Rx_Msg_GW_SRS_0x350,                          1,             1,             5,             1,             4  },/*174*/  /*SRS_SBR_SecondRowMid*/   /*GW_SRS_0x350*/
{  Can_Ch0_Il_Rx_Msg_GW_SRS_0x350,                          1,             1,             7,             1,             6  },/*175*/  /*SRS_SBR_SecondRowLeft*/   /*GW_SRS_0x350*/
{  Can_Ch0_Il_Rx_Msg_GW_SRS_0x350,                          1,             6,             3,             6,             0  },/*176*/  /*Rolling_counter_0x350*/   /*GW_SRS_0x350*/
{  Can_Ch0_Il_Rx_Msg_GW_SRS_0x350,                          1,             7,             7,             7,             0  },/*177*/  /*Checksum_0x350*/   /*GW_SRS_0x350*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             0,             7,             0,             0  },/*178*/  /*TPMS_LF_Pressure*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             1,             7,             1,             0  },/*179*/  /*TPMS_RF_Pressure*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             2,             7,             2,             0  },/*180*/  /*TPMS_RR_Pressure*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             3,             7,             3,             0  },/*181*/  /*TPMS_LR_Pressure*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             4,             7,             4,             0  },/*182*/  /*TPMS_Tire_Temperature*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             5,             2,             5,             0  },/*183*/  /*TPMS_LF_Pressure_Warning*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             5,             5,             5,             3  },/*184*/  /*TPMS_RF_Pressure_Warning*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             5,             7,             5,             6  },/*185*/  /*TPMS_System_Status*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             6,             2,             6,             0  },/*186*/  /*TPMS_RR_Pressure_Warning*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             6,             5,             6,             3  },/*187*/  /*TPMS_LR_Pressure_Warning*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             6,             6,             6,             6  },/*188*/  /*Temperature_Warning*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             6,             7,             6,             7  },/*189*/  /*resrved*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             7,             2,             7,             0  },/*190*/  /*Tire_Position*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x361,                       1,             7,             4,             7,             3  },/*191*/  /*TPMS_Lamp_Status*/   /*TPMS_Tire_0x361*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x362,                       1,             2,             7,             2,             0  },/*192*/  /*RRI_Pressure*/   /*TPMS_Tire_0x362*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x362,                       1,             3,             7,             3,             0  },/*193*/  /*LRI_Pressure*/   /*TPMS_Tire_0x362*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x362,                       1,             6,             2,             6,             0  },/*194*/  /*RRI_Pressure_Warning*/   /*TPMS_Tire_0x362*/
{  Can_Ch0_Il_Rx_Msg_TPMS_Tire_0x362,                       1,             6,             5,             6,             3  },/*195*/  /*LRI_Pressure_Warning*/   /*TPMS_Tire_0x362*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x366,                    2,             0,             7,             1,             0  },/*196*/  /*EMS_Fuelcounter*/   /*GW_EMS_Power_0x366*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x366,                    1,             5,             1,             5,             0  },/*197*/  /*EMS_RemindShiftGear_MT*/   /*GW_EMS_Power_0x366*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x366,                    1,             5,             2,             5,             2  },/*198*/  /*EMS_MIL*/   /*GW_EMS_Power_0x366*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x366,                    1,             5,             3,             5,             3  },/*199*/  /*EMS_SVS*/   /*GW_EMS_Power_0x366*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x366,                    1,             5,             5,             5,             4  },/*200*/  /*EMS_StartStopStatus*/   /*GW_EMS_Power_0x366*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_Power_0x366,                    1,             5,             7,             5,             6  },/*201*/  /*EMS_OBDTxt*/   /*GW_EMS_Power_0x366*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             0,             3,             1,             6  },/*202*/  /*EMS_TroqLimtHour*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             0,             7,             0,             6  },/*203*/  /*EMS_UreaQty*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             1,             5,             1,             0  },/*204*/  /*EMS_TroqLimtMin*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             2,             7,             2,             0  },/*205*/  /*EMS_UreaLvl*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             3,             3,             3,             0  },/*206*/  /*EMS_DieslPrtcWarn_D_Rq*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             3,             5,             3,             4  },/*207*/  /*DCM_GlowPlugLmpReq*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             3,             7,             3,             6  },/*208*/  /*DCM_WIFFullLmpReq*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             4,             1,             5,             4  },/*209*/  /*EMS_SpeedLimtHour*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             4,             7,             4,             2  },/*210*/  /*EMS_SpeedLimtMin*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             5,             2,             5,             1  },/*211*/  /*EMS_UreaSysWarning*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             6,             1,             7,             7  },/*212*/  /*EMS_UreaTxt*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             6,             4,             6,             2  },/*213*/  /*EMS_ManRgenTxt_D_RqDsply*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             6,             6,             6,             5  },/*214*/  /*EMS_UreaWarn*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x368,                          1,             7,             6,             7,             0  },/*215*/  /*EMS_Soot_Pc_Dsply*/   /*GW_EMS_0x368*/
{  Can_Ch0_Il_Rx_Msg_MP5_0x382,                             1,             0,             6,             0,             6  },/*216*/  /*MP5_BSDSoundSwitchSts*/   /*MP5_0x382*/
{  Can_Ch0_Il_Rx_Msg_MP5_0x384,                             1,             0,             7,             0,             2  },/*217*/  /*MP5_NavICON*/   /*MP5_0x384*/
{  Can_Ch0_Il_Rx_Msg_MP5_0x384,                             3,             1,             7,             3,             0  },/*218*/  /*MP5_ROUTE_REMAIN_DIS*/   /*MP5_0x384*/
{  Can_Ch0_Il_Rx_Msg_MP5_0x384,                             1,             5,             2,             5,             0  },/*219*/  /*MP5_ALModeSetting*/   /*MP5_0x384*/
{  Can_Ch0_Il_Rx_Msg_MP5_0x384,                             1,             5,             3,             5,             3  },/*220*/  /*MP5_ALStartAroundSetting*/   /*MP5_0x384*/
{  Can_Ch0_Il_Rx_Msg_GW_TCU_Info_0x395,                     1,             1,             3,             1,             3  },/*221*/  /*Trans_Limphome*/   /*GW_TCU_Info_0x395*/
{  Can_Ch0_Il_Rx_Msg_GW_TCU_Info_0x395,                     1,             1,             6,             1,             6  },/*222*/  /*TrnSrvcRqd_B_Rq*/   /*GW_TCU_Info_0x395*/
{  Can_Ch0_Il_Rx_Msg_GW_TCU_Info_0x395,                     1,             2,             2,             2,             2  },/*223*/  /*TCU_Gearbox_Gonglimp*/   /*GW_TCU_Info_0x395*/
{  Can_Ch0_Il_Rx_Msg_GW_TCU_Info_0x395,                     1,             3,             2,             3,             2  },/*224*/  /*TCU_Driverwarning*/   /*GW_TCU_Info_0x395*/
{  Can_Ch0_Il_Rx_Msg_GW_TCU_Info_0x395,                     1,             4,             7,             4,             5  },/*225*/  /*TCU_DrvShiftAllwCondRmd*/   /*GW_TCU_Info_0x395*/
{  Can_Ch0_Il_Rx_Msg_MP5_0x530,                             1,             2,             6,             2,             5  },/*226*/  /*MP5_AlarmStatus*/   /*MP5_0x530*/
{  Can_Ch0_Il_Rx_Msg_MP5_0x530,                             1,             2,             7,             2,             7  },/*227*/  /*MP5_AlarmOrNot*/   /*MP5_0x530*/
{  Can_Ch0_Il_Rx_Msg_GW_TBOX_time_0x582,                    1,             0,             7,             0,             0  },/*228*/  /*TBOX_Year*/   /*GW_TBOX_time_0x582*/
{  Can_Ch0_Il_Rx_Msg_GW_TBOX_time_0x582,                    1,             1,             7,             1,             0  },/*229*/  /*TBOX_Month*/   /*GW_TBOX_time_0x582*/
{  Can_Ch0_Il_Rx_Msg_GW_TBOX_time_0x582,                    1,             2,             7,             2,             0  },/*230*/  /*TBOX_Date*/   /*GW_TBOX_time_0x582*/
{  Can_Ch0_Il_Rx_Msg_GW_TBOX_time_0x582,                    1,             3,             7,             3,             0  },/*231*/  /*TBOX_Hour*/   /*GW_TBOX_time_0x582*/
{  Can_Ch0_Il_Rx_Msg_GW_TBOX_time_0x582,                    1,             4,             7,             4,             0  },/*232*/  /*TBOX_Minute*/   /*GW_TBOX_time_0x582*/
{  Can_Ch0_Il_Rx_Msg_GW_TBOX_time_0x582,                    1,             5,             7,             5,             0  },/*233*/  /*TBOX_Second*/   /*GW_TBOX_time_0x582*/
{  Can_Ch0_Il_Rx_Msg_GW_TBOX_time_0x582,                    1,             6,             7,             6,             7  },/*234*/  /*TBOX_RealTime_UB_Copy_1*/   /*GW_TBOX_time_0x582*/
{  Can_Ch0_Il_Rx_Msg_GW_TBOX_time_0x582,                    1,             7,             0,             7,             0  },/*235*/  /*TBOX_RealTime_UB*/   /*GW_TBOX_time_0x582*/
{  Can_Ch0_Il_Rx_Msg_BCM_0x584,                             1,             0,             1,             0,             0  },/*236*/  /*BCM_LoadManageLevel*/   /*BCM_0x584*/
{  Can_Ch0_Il_Rx_Msg_BCM_0x584,                             1,             0,             4,             0,             2  },/*237*/  /*BCM_CarModeIndicator*/   /*BCM_0x584*/
{  Can_Ch0_Il_Rx_Msg_BCM_0x584,                             1,             0,             7,             0,             5  },/*238*/  /*BCM_CarMode*/   /*BCM_0x584*/
{  Can_Ch0_Il_Rx_Msg_GW_EMS_0x636,                          4,             0,             7,             3,             0  },/*239*/  /*EMS_BackupOdometer*/   /*GW_EMS_0x636*/
};
/* ===========================================================================
  Interaction Layer Receive Frame Data Storage and Status
 =========================================================================*/
CAN_UINT8 Can_Ch0_Il_Rx_Frame_Data[ Can_Ch0_Il_Rx_Num_Messages ][ CAN_MAX_DATA_LENGTH ];

CAN_UINT8 Can_Ch0_Il_Rx_Frame_Status[ Can_Ch0_Il_Rx_Num_Messages ];

CAN_UINT16 Can_Ch0_Il_Rx_Timeout_Count[ Can_Ch0_Il_Rx_Num_Messages ];

Can_Il_Rx_Data_Pointer Can_Ch0_Il_Receive_Ptr[ Can_Ch0_Il_Rx_Num_Messages ][ CAN_MAX_DATA_LENGTH ];

#if (Can_Ch0_Il_Rx_Num_Req_Frames > 0)

#define CAN_CHANNEL0_IL_DATA_REQ_TX_HANDLE
#define CAN_CHANNEL0_IL_REQ_TX_CANID

static CAN_UINT8  reqCounter[ Can_Ch0_Il_Rx_Num_Req_Frames ];
static CAN_UINT16 reqTimer[ Can_Ch0_Il_Rx_Num_Req_Frames ];
static CAN_UINT8  reqStatus[ Can_Ch0_Il_Rx_Num_Req_Frames ];

static const CAN_UINT8 Can_Ch0_Il_Data_Request[2] = {0x00,0x00};

static const CAN_IL_TMD Can_Ch0_Il_Data_Request_Frame_Tmd[ Can_Ch0_Il_Rx_Num_Req_Frames ] =
{
};

static const Can_Ch0_Il_Rx_Frame_Request Can_Ch0_Il_Rx_Request_Frame_Data[ Can_Ch0_Il_Rx_Num_Req_Frames ] =
{
};

#endif


/* ===========================================================================
  Received Frame Attributes Lookup Table

  This table includes the attributes for all of the received frames.
  If a received frame is periodic, this table also includes pointers to the
  received frame status and to the receive timeout counter for message
  gain and loss indication. If a transmitted frame request can be issued
  for a received frame, this table includes the pointer to the receive
  frame request attributes.

 =========================================================================*/
CAN_IL_RX_FRAME const
Can_Ch0_Il_Rx_Frame_Table[ Can_Ch0_Il_Rx_Num_Messages ] =
{
/* GW_EMS_Power_0x10B */ /*0*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     0,      /* Signal Start Index                   */
     7,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 0],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 0],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[0],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_TCU_GearInfo_Power_0x123 */ /*1*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     7,      /* Signal Start Index                   */
     5,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 1],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 1],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[1],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_EMS_Sts_Power_0x151 */ /*2*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     12,      /* Signal Start Index                   */
     4,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 2],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 2],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[2],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* PEPS_Start_0x1A5 */ /*3*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     16,      /* Signal Start Index                   */
     3,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 3],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 3],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[3],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_ABS_Sts_0x221 */ /*4*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     19,      /* Signal Start Index                   */
     9,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 4],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 4],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[4],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_ESP_Status_Chassis_0x243 */ /*5*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     28,      /* Signal Start Index                   */
     15,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 5],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 5],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[5],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_IPM_0x245 */ /*6*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     43,      /* Signal Start Index                   */
     15,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 6],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 6],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[6],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_MRR_Chassis_0x246 */ /*7*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     58,      /* Signal Start Index                   */
     18,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 7],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 7],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[7],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_EPS_Chassis_0x260 */ /*8*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     76,      /* Signal Start Index                   */
     4,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 8],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 8],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[8],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_PAM_0x271 */ /*9*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     80,      /* Signal Start Index                   */
     4,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 9],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 9],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[9],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_SOD_0x275 */ /*10*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     84,      /* Signal Start Index                   */
     14,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 10],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 10],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[10],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* BCM_Door_0x310 */ /*11*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     98,      /* Signal Start Index                   */
     16,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 11],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 11],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[11],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* BCM_0x313 */ /*12*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     114,      /* Signal Start Index                   */
     7,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 12],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 12],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[12],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* BCM_Sts_0x315 */ /*13*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     121,      /* Signal Start Index                   */
     1,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 13],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 13],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[13],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* BCM_Lamp_0x318 */ /*14*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     122,      /* Signal Start Index                   */
     22,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 14],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 14],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[14],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_SWM_Body_0x31A */ /*15*/
 {
     (IL_RX_ATTR_EVENT | IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     144,      /* Signal Start Index                   */
     7,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 15],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 15],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[15],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* PEPS_0x325 */ /*16*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     151,      /* Signal Start Index                   */
     18,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 16],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 16],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[16],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* RF_0x326 */ /*17*/
 {
     (IL_RX_ATTR_EVENT | IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     169,      /* Signal Start Index                   */
     1,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 17],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 17],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[17],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_SRS_0x350 */ /*18*/
 {
     (IL_RX_ATTR_EVENT | IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     170,      /* Signal Start Index                   */
     8,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 18],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 18],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[18],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* TPMS_Tire_0x361 */ /*19*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     178,      /* Signal Start Index                   */
     14,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 19],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 19],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[19],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* TPMS_Tire_0x362 */ /*20*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     192,      /* Signal Start Index                   */
     4,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 20],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 20],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[20],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_EMS_Power_0x366 */ /*21*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     196,      /* Signal Start Index                   */
     6,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 21],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 21],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[21],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_EMS_0x368 */ /*22*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     202,      /* Signal Start Index                   */
     14,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 22],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 22],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[22],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* MP5_0x382 */ /*23*/
 {
     (IL_RX_ATTR_EVENT | IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     216,      /* Signal Start Index                   */
     1,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 23],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 23],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[23],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* MP5_0x384 */ /*24*/
 {
     (IL_RX_ATTR_EVENT | IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     217,      /* Signal Start Index                   */
     4,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 24],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 24],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[24],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(5000),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_TCU_Info_0x395 */ /*25*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     221,      /* Signal Start Index                   */
     5,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 25],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 25],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[25],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* MP5_0x530 */ /*26*/
 {
     (IL_RX_ATTR_EVENT | IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     226,      /* Signal Start Index                   */
     2,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 26],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 26],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[26],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_TBOX_time_0x582 */ /*27*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     228,      /* Signal Start Index                   */
     8,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 27],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 27],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[27],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* BCM_0x584 */ /*28*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     236,      /* Signal Start Index                   */
     3,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 28],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 28],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[28],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
/* GW_EMS_0x636 */ /*29*/
 {
     (IL_RX_ATTR_PERIODIC | IL_RX_ATTR_TIMEOUT_MONITOR),
     239,      /* Signal Start Index                   */
     1,      /* Number of Signals in Message         */
     CAN_GPNUM_8,           /* Minimum Data Length Code             */
     &Can_Ch0_Il_Rx_Frame_Status[ 29],      /* Pointer to Receive Status            */
     Can_Ch0_Il_Rx_Frame_Data[ 29],         /* Pointer to Received Frame Data       */
     &Can_Ch0_Il_Rx_Timeout_Count[29],         /* Pointer to the Timeout Counter       */
     IL_TIME_IN_TASK_TICS(1900),     /* Timeout Count Value                  */
     NULL,                                       /* &Can_Ch0_Il_Rx_Frame_Request_Table[ 0 ] */  /* Ptr to Receive Request Attributes    */ 
},
};
pTxPrecopyfn const Can_Ch0_Il_Tx_Precopy_Function_Table[ Can_Ch0_Il_Tx_Num_Messages ] =
  {
     NULL  ,
     NULL  ,
     IC_0x510_RollingCounter_Checksum  ,
     IC_0x525_RollingCounter_Checksum  ,
     NULL  ,
};
/*****************************************************************************
    R E V I S I O N     N O T E S
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  -
For each change to this file, be sure to record:
1.   Who made the change and when the change was made.
2.   Why the change was made and the intended result.

Date       By         Reason For Change
------------------------------------------------------------------------------

******************************************************************************/
/*****************************************************************************
Date          : 2020-08-27 15:12:52
By              : ChengJia
Traceability    : JMC_SUV_Body_CAN_Matrix_20200827.dbc
Change Description  : Tool Generated code
*****************************************************************************/
