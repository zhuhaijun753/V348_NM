//---------------------------------------------------------------------------------------------------------------------
//
// VISTEON CORPORATION CONFIDENTIAL
// ________________________________
//
// [2015] Visteon Corporation
// All Rights Reserved.
//
// NOTICE: This is an unpublished work of authorship, which contains trade secrets.
// Visteon Corporation owns all rights to this work and intends to maintain it in confidence to preserve
// its trade secret status. Visteon Corporation reserves the right, under the copyright laws of the United States
// or those of any other country that may have jurisdiction, to protect this work as an unpublished work,
// in the event of an inadvertent or deliberate unauthorized publication. Visteon Corporation also reserves its rights
// under all copyright laws to protect this work as a published work, when appropriate.
// Those having access to this work may not copy it, use it, modify it, or disclose the information contained in it
// without the written authorization of Visteon Corporation.
//
//---------------------------------------------------------------------------------------------------------------------
#include "Rte.h"
#include "Rte_type.h"
#include "Rte_SeatBeltMdl.h"
#include "Rte_IoHwAb_Type.h"
#include "CmpLib.h"
#include "TtCfg.h"

/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(SeatBeltMdl)

//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
#if 0
typedef enum {
	SEATBELT_WARNING_FLAG_OFF = 0,
	SEATBELT_WARNING_FLAG_ONE,
	SEATBELT_WARNING_FLAG_TWO,
}SEATBELT_TT_FLASH_FLAG;
#endif
//#define DRIVER_BELT_LAMP_OFF_0 		((uint8)0)
#define DRIVER_BELT_LAMP_OFF_1		((uint8)1)
#define DRIVER_BELT_LAMP_ON_2 		((uint8)2)
//#define DRIVER_BELT_LAMP_OFF_3 		((uint8)3)

//#define PASSENGER_BELT_LAMP_OFF_0  	((uint8)0)
//#define PASSENGER_BELT_LAMP_OFF_1	((uint8)1)
//#define PASSENGER_BELT_LAMP_ON_2 	((uint8)2)
//#define PASSENGER_BELT_LAMP_OFF_3 	((uint8)3)

//#define DOOROPEN_FRNTLE_2			((uint8)2)
//#define DOOROPEN_FRNTRI_2			((uint8)2)

//#define VEHSEPLGT_VALUE_10			((uint8)100)
//#define VEHSEPLGT_VALUE_25			((uint16)2500)
//#define VEHSEPLGT_VALUE_40			((uint16)4000)

//#define VEHICLE_TACH_300			((uint16)600)

//#define DRIVER_TIMER_FLAG_60		((uint16)60000/30)
//#define DRIVER_TIMER_FLAG_90		((uint32)90000/30)
//#define DRIVER_TIMER_FLAG_180		((uint32)180000/30)
//#define PASSENGER_TIMER_FLAG_60		((uint16)60000/30)
//#define PASSENGER_TIMER_FLAG_90		((uint32)90000/30)
//#define PASSENGER_TIMER_FLAG_180	((uint32)180000/30)

//#define STATE_RESET					(0)

//#define VEHICLE_TACH_INVALID		((uint32)0x7FFF)	 // speed Invalid
//#define VEHICLE_SPD_INVALID			((uint16)0xFFF)		 // VehSpdLgt Invalid
//#define INVALID_DISPOSE_FLAG_2S		((uint16)2000/30)

/*This macro is used as stub instead of CAN dependency ,Have to Remove this macro */

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//=====================================================================================================================
//  PRIVATE
//=====================================================================================================================
static uint8   l_dr_seatblt_tt_status_u8   	= cTT_TURN_OFF;
//static uint8   l_ps_seatblt_tt_status_u8   	= cTT_TURN_OFF;
//static boolean l_dr_speed_meet_warning_flag = FALSE;
//static boolean l_ps_speed_meet_warning_flag = FALSE;
//static uint16  l_driver_falsh_count		 	= STATE_RESET;
//static uint16  l_passenger_falsh_count		= STATE_RESET;
//static uint32  l_Speed_Last_u32			    = STATE_RESET;
//static uint16  l_VehSpdLgt_Last_u16			= STATE_RESET;
//static uint16  l_Speed_Invalid_dr_count	    = STATE_RESET;
//static uint16  l_Speed_Invalid_ps_count	    = STATE_RESET;
//static uint16  l_VehSpdLgt_Invalid_dr_count	= STATE_RESET;
//static uint16  l_VehSpdLgt_Invalid_ps_count	= STATE_RESET;
//static SEATBELT_TT_FLASH_FLAG l_dr_spd_warning_flag	   = SEATBELT_WARNING_FLAG_OFF;
//static SEATBELT_TT_FLASH_FLAG l_ps_spd_warning_flag	   = SEATBELT_WARNING_FLAG_OFF;
//static SEATBELT_TT_FLASH_FLAG l_driver_warning_flag    = SEATBELT_WARNING_FLAG_OFF;
//static SEATBELT_TT_FLASH_FLAG l_passenger_warning_flag = SEATBELT_WARNING_FLAG_OFF;

static void fttmdl_N356Seatbelt_process(void);
//static void fttmdl_Driver_Seatbelt_falsh_Dispose(void);
//static void fttmdl_passenger_Seatbelt_falsh_Dispose(void);
static void Seatbelt_Init_Variable(void);
//static void Seatbelt_Falsh_Sync(void);
	
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
	Seatbelt_Init_Variable();

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
	Seatbelt_Init_Variable();

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress. Keeps the component in this state.
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation( void )
{
	Seatbelt_Init_Variable();

    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-activation state
///
/// @return E_OK:           Succeed
///<br>     E_NOT_READY:    In progress
///<br>     E_NOT_OK:       Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation( void )
{
	Seatbelt_Init_Variable();

	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
/****************************************************************************
Function Name     	: CmpActive

Description         : This function process the oilpressure TT ON/OFF for
					  ISG /NON ISG Variant.

Invocation          : called in cmpActive.

Parameters          : None

Return Value        : None

Critical Section    : None

******************************************************************************/
static Std_ReturnType CmpActive( void )
{
	fttmdl_N356Seatbelt_process();
	
	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief  Diagnostic state
///
/// @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}

/*********************************************************************************

Function Name	  :  Seatbelt_Falsh_Sync (void)

Description 	  : SeatBelt Telltale Machine Logical

Parameters		  : None

Return Value	  : None

Critical Section  : None

***********************************************************************************/
#if 0
static void Seatbelt_Falsh_Sync(void)
{
	if(((STATE_RESET != l_driver_falsh_count) && (DRIVER_TIMER_FLAG_180 > l_driver_falsh_count))
	|| ((STATE_RESET != l_passenger_falsh_count) && (PASSENGER_TIMER_FLAG_180 > l_passenger_falsh_count)))
	{
		if(l_driver_falsh_count < l_passenger_falsh_count)
		{
			l_driver_falsh_count = l_passenger_falsh_count;
		}
		else
		{
			l_passenger_falsh_count = l_driver_falsh_count;
		}
	}


}
#endif

/*********************************************************************************

Function Name	  :  Seatbelt_Init_Variable (void)

Description 	  : SeatBelt Telltale Machine Logical

Parameters		  : None

Return Value	  : None

Critical Section  : None

***********************************************************************************/
static void Seatbelt_Init_Variable(void)
{
	l_dr_seatblt_tt_status_u8 	 = cTT_TURN_OFF;
	//l_ps_seatblt_tt_status_u8 	 = cTT_TURN_OFF;
	//l_dr_speed_meet_warning_flag = FALSE;
	//l_ps_speed_meet_warning_flag = FALSE;
	//l_driver_falsh_count 	  	   = STATE_RESET;
	//l_passenger_falsh_count      = STATE_RESET;
	//l_Speed_Last_u32			   = STATE_RESET;
	//l_VehSpdLgt_Last_u16		   = STATE_RESET;
	//l_Speed_Invalid_dr_count	   = STATE_RESET;
	//l_Speed_Invalid_ps_count	   = STATE_RESET;
 	//l_VehSpdLgt_Invalid_dr_count = STATE_RESET;
	//l_VehSpdLgt_Invalid_ps_count = STATE_RESET;
	//l_dr_spd_warning_flag		   = SEATBELT_WARNING_FLAG_OFF;
	//l_ps_spd_warning_flag		   = SEATBELT_WARNING_FLAG_OFF;
    //l_driver_warning_flag    	   = SEATBELT_WARNING_FLAG_OFF;
    //l_passenger_warning_flag 	   = SEATBELT_WARNING_FLAG_OFF;

	Rte_Call_rpCS_SeatBeltMdl_TIMdlUpdateTtStatus_Operation(cTTDRIVER_SEATBELT_REMINDER,l_dr_seatblt_tt_status_u8);
	//Rte_Call_rpCS_SeatBeltMdl_TIMdlUpdateTtStatus_Operation(cTTPASSENGER_SEATBELT_REMINDER,l_ps_seatblt_tt_status_u8);
}
/*********************************************************************************

Function Name	  :  fttmdl_Driver_Seatbelt_falsh_Dispose (void)

Description 	  : SeatBelt Telltale Machine Logical

Parameters		  : None

Return Value	  : None

Critical Section  : None

***********************************************************************************/
#if 0
static void fttmdl_Driver_Seatbelt_falsh_Dispose(void)
{
	//uint8 u8Msg_0x10B_NR   = 0;
	//uint8 u8Msg_0x10B_Tout = 0;
	uint8 u8Msg_0x221_NR   = 0;
	uint8 u8Msg_0x221_Tout = 0;
	uint8 u8Msg_0x310_NR   = 0;
	uint8 u8Msg_0x310_Tout = 0;

	//uint16 fl_EngSpeed_u8	  = 0;
	uint16 fl_VehSpdLgt_u8	  = 0;
	uint8 fl_DoorStsFrntLe_u8 = 0;
	//uint8 fl_DoorStsFrntRi_u8 = 0;
	//uint8 fl_ign_substate	  = eIGN_OFF;

	//Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&u8Msg_0x10B_NR);
	//Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&u8Msg_0x10B_Tout);
	//Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&fl_EngSpeed_u8);

	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8Msg_0x221_NR);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8Msg_0x221_Tout);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_VehSpdLgt_u8);

	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&u8Msg_0x310_NR);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&u8Msg_0x310_Tout);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&fl_DoorStsFrntLe_u8);
	//Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi(&fl_DoorStsFrntRi_u8);

	if(//(RTE_E_NEVER_RECEIVED == u8Msg_0x10B_NR) ||
	   (RTE_E_NEVER_RECEIVED == u8Msg_0x221_NR)
	|| (RTE_E_NEVER_RECEIVED == u8Msg_0x310_NR))
	{
		//l_dr_seatblt_tt_status_u8 = cTT_TURN_OFF;
		//l_ps_seatblt_tt_status_u8 = cTT_TURN_OFF;
	}
	else
	{
		//if(RTE_E_TIMEOUT == u8Msg_0x10B_Tout)
		//{
			//fl_EngSpeed_u8 = 0;
		//}

		if(RTE_E_TIMEOUT == u8Msg_0x221_Tout)
		{
			fl_VehSpdLgt_u8 = 0;
		}

		if(RTE_E_TIMEOUT == u8Msg_0x310_Tout)
		{
			fl_DoorStsFrntLe_u8 = 0;
			//fl_DoorStsFrntRi_u8 = 0;
		}

		#if 0
		if(VEHICLE_TACH_INVALID == fl_EngSpeed_u8)
		{
			if(INVALID_DISPOSE_FLAG_2S > l_Speed_Invalid_dr_count)
			{
				l_Speed_Invalid_dr_count++;
				fl_EngSpeed_u8 = l_Speed_Last_u32;
			}
			else
			{
				fl_EngSpeed_u8   = 0;
				l_Speed_Last_u32 = fl_EngSpeed_u8;
			}
		}
		else
		{
			l_Speed_Invalid_dr_count = STATE_RESET;
			l_Speed_Last_u32	  	 = fl_EngSpeed_u8;
		}
		#endif

		if(VEHICLE_SPD_INVALID == fl_VehSpdLgt_u8)
		{
			if(INVALID_DISPOSE_FLAG_2S > l_VehSpdLgt_Invalid_dr_count)
			{
				l_VehSpdLgt_Invalid_dr_count++;
				fl_VehSpdLgt_u8 = l_VehSpdLgt_Last_u16;
			}
			else
			{
				fl_VehSpdLgt_u8		 = 0;
				l_VehSpdLgt_Last_u16 = fl_VehSpdLgt_u8;
			}
		}
		else
		{
			l_VehSpdLgt_Invalid_dr_count = STATE_RESET;

			Rte_Read_rpSR_SpdMdl_VehicleIndicatedspeed_value(&fl_VehSpdLgt_u8);
			
			l_VehSpdLgt_Last_u16 = fl_VehSpdLgt_u8;
		}

		if(DOOROPEN_FRNTLE_2 != fl_DoorStsFrntLe_u8)
		{
			if(VEHSEPLGT_VALUE_10 > fl_VehSpdLgt_u8)
			{
				l_driver_falsh_count = STATE_RESET;
				//l_dr_spd_warning_flag = SEATBELT_WARNING_FLAG_OFF;
				//l_driver_warning_flag = SEATBELT_WARNING_FLAG_OFF;
				//l_dr_speed_meet_warning_flag = FALSE;
			}
			else if(VEHSEPLGT_VALUE_25 > fl_VehSpdLgt_u8)
			{
				if(DRIVER_TIMER_FLAG_60 > l_driver_falsh_count)
				{
					l_driver_falsh_count++;
				}
			}

			if(VEHSEPLGT_VALUE_25 <= fl_VehSpdLgt_u8)
			{
				//l_speed_meet_warning_flag = TRUE;
				if(DRIVER_TIMER_FLAG_60 > l_driver_falsh_count)
				{
					l_driver_falsh_count = DRIVER_TIMER_FLAG_60;
				}

				if((SEATBELT_WARNING_FLAG_ONE == l_dr_spd_warning_flag)
				&& (SEATBELT_WARNING_FLAG_TWO == l_driver_warning_flag))
				{
					if(DRIVER_TIMER_FLAG_180 <= l_driver_falsh_count)
					{
						l_driver_falsh_count  = DRIVER_TIMER_FLAG_60;
						l_dr_spd_warning_flag = SEATBELT_WARNING_FLAG_OFF;
						l_driver_warning_flag = SEATBELT_WARNING_FLAG_OFF;
					}
				}
			}
			else
			{
				if((DRIVER_TIMER_FLAG_180 == l_driver_falsh_count)
				&& (SEATBELT_WARNING_FLAG_TWO == l_driver_warning_flag))
				{
					l_dr_spd_warning_flag = SEATBELT_WARNING_FLAG_ONE;
					//l_driver_warning_flag = SEATBELT_WARNING_FLAG_OFF;
				}
			}

			if(VEHSEPLGT_VALUE_40 <= fl_VehSpdLgt_u8)
			{
				if(DRIVER_TIMER_FLAG_90 > l_driver_falsh_count)
				{
					l_driver_falsh_count = DRIVER_TIMER_FLAG_90;
				}
				//l_dr_speed_meet_warning_flag = TRUE;

				if((SEATBELT_WARNING_FLAG_TWO == l_dr_spd_warning_flag)
				&& (SEATBELT_WARNING_FLAG_TWO == l_driver_warning_flag))
				{
					l_driver_falsh_count  = DRIVER_TIMER_FLAG_90;
					l_dr_spd_warning_flag = SEATBELT_WARNING_FLAG_OFF;
					l_driver_warning_flag = SEATBELT_WARNING_FLAG_ONE;
				}
			}
			else
			{
				if((DRIVER_TIMER_FLAG_180 == l_driver_falsh_count)
				&& (SEATBELT_WARNING_FLAG_TWO == l_driver_warning_flag))
				{
					l_dr_spd_warning_flag = SEATBELT_WARNING_FLAG_TWO;
					//l_driver_warning_flag = SEATBELT_WARNING_FLAG_ONE;
				}
			}

			if(DRIVER_TIMER_FLAG_60 <= l_driver_falsh_count)
			{
				if(DRIVER_TIMER_FLAG_90 > l_driver_falsh_count)
				{
					l_driver_falsh_count++;
					l_dr_speed_meet_warning_flag = TRUE;
				}
				else if(DRIVER_TIMER_FLAG_180 > l_driver_falsh_count)
				{
					l_driver_falsh_count++;
					l_dr_speed_meet_warning_flag = TRUE;
					if(SEATBELT_WARNING_FLAG_OFF == l_dr_spd_warning_flag)
					{
						l_driver_warning_flag = SEATBELT_WARNING_FLAG_ONE;
					}
					  //l_passenger_warning_flag  = SEATBELT_WARNING_FLAG_ONE;
				}
				else
				{
					l_dr_speed_meet_warning_flag = FALSE;
					if(SEATBELT_WARNING_FLAG_ONE == l_dr_spd_warning_flag)
					{
						l_driver_warning_flag = SEATBELT_WARNING_FLAG_TWO;
					}
    				//l_passenger_warning_flag  = SEATBELT_WARNING_FLAG_TWO;
				}
			}
			else
			{
				l_dr_speed_meet_warning_flag = FALSE;
			}

			if(l_dr_speed_meet_warning_flag == TRUE)
			{
				l_dr_seatblt_tt_status_u8 = cTT_FLASH_1HZ;
			}
			else
			{
				//l_dr_seatblt_tt_status_u8 = cTT_TURN_ON;
			}
		}
		else
		{
			//l_dr_seatblt_tt_status_u8 = cTT_TURN_ON;
			l_driver_falsh_count = STATE_RESET;
		}
	}
}
#endif
/*********************************************************************************

Function Name	  :  fttmdl_passenger_Seatbelt_falsh_Dispose (void)

Description 	  : SeatBelt Telltale Machine Logical

Parameters		  : None

Return Value	  : None

Critical Section  : None

***********************************************************************************/
#if 0
static void fttmdl_passenger_Seatbelt_falsh_Dispose(void)
{
	//uint8 u8Msg_0x10B_NR   = 0;
	//uint8 u8Msg_0x10B_Tout = 0;
	uint8 u8Msg_0x221_NR   = 0;
	uint8 u8Msg_0x221_Tout = 0;
	uint8 u8Msg_0x310_NR   = 0;
	uint8 u8Msg_0x310_Tout = 0;

	//uint16 fl_EngSpeed_u8	  = 0;
	uint16 fl_VehSpdLgt_u8	  = 0;
	//uint8 fl_DoorStsFrntLe_u8 = 0;
	uint8 fl_DoorStsFrntRi_u8 = 0;
	//uint8 fl_ign_substate	  = eIGN_OFF;

	//Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_NR(&u8Msg_0x10B_NR);
	//Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_Tout(&u8Msg_0x10B_Tout);
	//Rte_Read_rpSR_CANMSG_GW_EMS_Power_0x10B_ComIn_EMS_EngSpeed(&fl_EngSpeed_u8);

	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_NR(&u8Msg_0x221_NR);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_Tout(&u8Msg_0x221_Tout);
	Rte_Read_rpSR_CANMSG_GW_ABS_Sts_0x221_ComIn_ABS_VehSpdLgt(&fl_VehSpdLgt_u8);

	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_NR(&u8Msg_0x310_NR);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_Tout(&u8Msg_0x310_Tout);
	//Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntLe(&fl_DoorStsFrntLe_u8);
	Rte_Read_rpSR_CANMSG_BCM_Door_0x310_ComIn_BCM_DoorStsFrntRi(&fl_DoorStsFrntRi_u8);

	if(//(RTE_E_NEVER_RECEIVED == u8Msg_0x10B_NR) ||
		(RTE_E_NEVER_RECEIVED == u8Msg_0x221_NR)
	|| (RTE_E_NEVER_RECEIVED == u8Msg_0x310_NR))
	{
		//l_dr_seatblt_tt_status_u8 = cTT_TURN_OFF;
		//l_ps_seatblt_tt_status_u8 = cTT_TURN_OFF;
	}
	else
	{
		//if(RTE_E_TIMEOUT == u8Msg_0x10B_Tout)
		//{
			//fl_EngSpeed_u8 = 0;
		//}

		if(RTE_E_TIMEOUT == u8Msg_0x221_Tout)
		{
			fl_VehSpdLgt_u8 = 0;
		}

		if(RTE_E_TIMEOUT == u8Msg_0x310_Tout)
		{
			//fl_DoorStsFrntLe_u8 = 0;
			fl_DoorStsFrntRi_u8 = 0;
		}

		#if 0
		if(VEHICLE_TACH_INVALID == fl_EngSpeed_u8)
		{
			if(INVALID_DISPOSE_FLAG_2S > l_Speed_Invalid_ps_count)
			{
				l_Speed_Invalid_ps_count++;
				fl_EngSpeed_u8 = l_Speed_Last_u32;
			}
			else
			{
				fl_EngSpeed_u8   = 0;
				l_Speed_Last_u32 = fl_EngSpeed_u8;
			}
		}
		else
		{
			l_Speed_Invalid_ps_count = STATE_RESET;
			l_Speed_Last_u32	     = fl_EngSpeed_u8;
		}
		#endif

		if(VEHICLE_SPD_INVALID == fl_VehSpdLgt_u8)
		{
			if(INVALID_DISPOSE_FLAG_2S > l_VehSpdLgt_Invalid_ps_count)
			{
				l_VehSpdLgt_Invalid_ps_count++;
				fl_VehSpdLgt_u8 = l_VehSpdLgt_Last_u16;
			}
			else
			{
				fl_VehSpdLgt_u8 	 = 0;
				l_VehSpdLgt_Last_u16 = fl_VehSpdLgt_u8;
			}
		}
		else
		{
			l_VehSpdLgt_Invalid_ps_count = STATE_RESET;

			Rte_Read_rpSR_SpdMdl_VehicleIndicatedspeed_value(&fl_VehSpdLgt_u8);
			
			l_VehSpdLgt_Last_u16 = fl_VehSpdLgt_u8;
		}

		if(DOOROPEN_FRNTLE_2 != fl_DoorStsFrntRi_u8)
		{
			if(VEHSEPLGT_VALUE_10 > fl_VehSpdLgt_u8)
			{
				l_passenger_falsh_count = STATE_RESET;
				//l_dr_spd_warning_flag = SEATBELT_WARNING_FLAG_OFF;
				//l_driver_warning_flag = SEATBELT_WARNING_FLAG_OFF;
				//l_ps_speed_meet_warning_flag = FALSE;
			}
			else if(VEHSEPLGT_VALUE_25 > fl_VehSpdLgt_u8)
			{
				if(PASSENGER_TIMER_FLAG_60 > l_passenger_falsh_count)
				{
					l_passenger_falsh_count++;
				}
			}

			if(VEHSEPLGT_VALUE_25 <= fl_VehSpdLgt_u8)
			{
				//l_speed_meet_warning_flag = TRUE;
				if(PASSENGER_TIMER_FLAG_60 > l_passenger_falsh_count)
				{
					l_passenger_falsh_count = PASSENGER_TIMER_FLAG_60;
				}

				if((SEATBELT_WARNING_FLAG_ONE == l_ps_spd_warning_flag)
				&& (SEATBELT_WARNING_FLAG_TWO == l_passenger_warning_flag))
				{
					if(PASSENGER_TIMER_FLAG_90 <= l_passenger_falsh_count)
					{
						l_passenger_falsh_count  = PASSENGER_TIMER_FLAG_60;
						l_ps_spd_warning_flag 	 = SEATBELT_WARNING_FLAG_OFF;
						l_passenger_warning_flag = SEATBELT_WARNING_FLAG_OFF;
					}
				}
			}
			else
			{
				if((PASSENGER_TIMER_FLAG_180 == l_passenger_falsh_count)
				&& (SEATBELT_WARNING_FLAG_TWO == l_passenger_warning_flag))
				{
					l_ps_spd_warning_flag	 = SEATBELT_WARNING_FLAG_ONE;
					//l_passenger_warning_flag = SEATBELT_WARNING_FLAG_OFF;
				}
			}

			if(VEHSEPLGT_VALUE_40 <= fl_VehSpdLgt_u8)
			{
				//l_dr_speed_meet_warning_flag = TRUE;
				if(PASSENGER_TIMER_FLAG_90 > l_passenger_falsh_count)
				{
					l_passenger_falsh_count = PASSENGER_TIMER_FLAG_90;
				}

				if((SEATBELT_WARNING_FLAG_TWO == l_ps_spd_warning_flag)
				&& (SEATBELT_WARNING_FLAG_TWO == l_passenger_warning_flag))
				{
					l_passenger_falsh_count  = PASSENGER_TIMER_FLAG_90;
					l_ps_spd_warning_flag 	 = SEATBELT_WARNING_FLAG_OFF;
					l_passenger_warning_flag = SEATBELT_WARNING_FLAG_ONE;
				}
			}
			else
			{
				if((PASSENGER_TIMER_FLAG_180 == l_passenger_falsh_count)
				&& (SEATBELT_WARNING_FLAG_TWO == l_passenger_warning_flag))
				{
					l_ps_spd_warning_flag = SEATBELT_WARNING_FLAG_TWO;
					//l_passenger_warning_flag = SEATBELT_WARNING_FLAG_ONE;
				}
			}

			if(PASSENGER_TIMER_FLAG_60 <= l_passenger_falsh_count)
			{
				if(PASSENGER_TIMER_FLAG_90 > l_passenger_falsh_count)
				{
					l_passenger_falsh_count++;
					l_ps_speed_meet_warning_flag = TRUE;
				}
				else if(PASSENGER_TIMER_FLAG_180 > l_passenger_falsh_count)
				{
					l_passenger_falsh_count++;
					l_ps_speed_meet_warning_flag = TRUE;
					//l_driver_warning_flag     = SEATBELT_WARNING_FLAG_ONE;
					if(SEATBELT_WARNING_FLAG_OFF == l_ps_spd_warning_flag)
					{
						l_passenger_warning_flag = SEATBELT_WARNING_FLAG_ONE;
					}
					  //l_passenger_warning_flag  = SEATBELT_WARNING_FLAG_ONE;
				}
				else
				{
					l_ps_speed_meet_warning_flag = FALSE;
					if(SEATBELT_WARNING_FLAG_ONE == l_ps_spd_warning_flag)
					{
						l_passenger_warning_flag = SEATBELT_WARNING_FLAG_TWO;
					}
				}
			}
			else
			{
				l_ps_speed_meet_warning_flag = FALSE;
			}

			if(l_ps_speed_meet_warning_flag == TRUE)
			{
				l_ps_seatblt_tt_status_u8 = cTT_FLASH_1HZ;
			}
			else
			{
				//l_ps_seatblt_tt_status_u8 = cTT_TURN_ON;
			}
		}
		else
		{
			//l_ps_seatblt_tt_status_u8 = cTT_TURN_ON;
			l_passenger_falsh_count = STATE_RESET;
		}
	}
}
#endif
/*********************************************************************************
Function Name	  :  fttmdl_N356Seatbelt_process (void)

Description 	  : SeatBelt Telltale Machine Logical

Parameters		  : None

Return Value	  : None

Critical Section  : None
***********************************************************************************/
static void fttmdl_N356Seatbelt_process(void)
{
#ifndef TT_ONOFF_TEST

	uint8 u8MsgNR   = 0;
	uint8 u8MsgTout = 0;

	uint8 fl_SBR_Driver_sig_val_U8    = cTT_TURN_OFF;
	//uint8 fl_SBR_Passenger_sig_val_U8 = cTT_TURN_OFF;
	uint8 fl_ign_substate			  = eIGN_OFF;

	//boolean IsEngineCfg = FALSE;

	/* TRUE:PASSENGER_BELT/Not PK,FALSE:no PASSENGER_BELT/PK */
	//Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_PASSENGER_BELT,&IsEngineCfg);

	Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_NR(&u8MsgNR);
	Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_Tout(&u8MsgTout);
	Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_Driver(&fl_SBR_Driver_sig_val_U8);
	//Rte_Read_rpSR_CANMSG_GW_SRS_0x350_ComIn_SRS_SBR_PAMsenger(&fl_SBR_Passenger_sig_val_U8);

	Rte_Read_rpIgnState_IGNState(&fl_ign_substate);

	if( eIGN_RUN == fl_ign_substate )
	{
		if((RTE_E_NEVER_RECEIVED == u8MsgNR) || (RTE_E_TIMEOUT == u8MsgTout))
		{
			l_dr_seatblt_tt_status_u8 = cTT_TURN_OFF;
			//l_ps_seatblt_tt_status_u8 = cTT_TURN_OFF;
		}
		else
		{	
			//Seatbelt_Falsh_Sync();
			
			//driver seatbelt
			switch(fl_SBR_Driver_sig_val_U8)
			{
				case DRIVER_BELT_LAMP_ON_2:
					l_dr_seatblt_tt_status_u8 =	cTT_TURN_ON;
					//fttmdl_Driver_Seatbelt_falsh_Dispose();
					break;
				case DRIVER_BELT_LAMP_OFF_1:
					l_dr_seatblt_tt_status_u8 =	cTT_FLASH_1HZ;					
					break;
				default:
					l_dr_seatblt_tt_status_u8 = cTT_TURN_OFF;
					//l_driver_falsh_count = STATE_RESET;
					break;
			}
			#if 0
			//passenger seatbelt
			switch( fl_SBR_Passenger_sig_val_U8 )
			{
				case PASSENGER_BELT_LAMP_ON_2:
					l_ps_seatblt_tt_status_u8 =	cTT_TURN_ON;
					fttmdl_passenger_Seatbelt_falsh_Dispose();
					break;
				default:
					l_ps_seatblt_tt_status_u8 =	cTT_TURN_OFF;
					l_passenger_falsh_count = STATE_RESET;
					break;
			}
			#endif
		}
	}
	else
	{
		l_dr_seatblt_tt_status_u8 =	cTT_TURN_OFF;
		//l_ps_seatblt_tt_status_u8 = cTT_TURN_OFF;
	}

	Rte_Write_pp_Seatbelt_TT_Status_Drv_Status(l_dr_seatblt_tt_status_u8);
	//Rte_Write_pp_Seatbelt_TT_Status_Pas_Status(l_ps_seatblt_tt_status_u8);

	Rte_Call_rpCS_SeatBeltMdl_TIMdlUpdateTtStatus_Operation(cTTDRIVER_SEATBELT_REMINDER,l_dr_seatblt_tt_status_u8);
	//Rte_Call_rpCS_SeatBeltMdl_TIMdlUpdateTtStatus_Operation(cTTPASSENGER_SEATBELT_REMINDER,l_ps_seatblt_tt_status_u8);

#else

	static unsigned int u32TimeCnt = 0;
	if(u32TimeCnt < 50)
	{
		l_dr_seatblt_tt_status_u8 = cTT_TURN_ON;
		Rte_Call_rpCS_SeatBeltMdl_TIMdlUpdateTtStatus_Operation(cTTDRIVER_SEATBELT_REMINDER,cTT_TURN_OFF);
		//l_ps_seatblt_tt_status_u8 = cTT_TURN_ON;
		//Rte_Call_rpCS_SeatBeltMdl_TIMdlUpdateTtStatus_Operation(cTTPASSENGER_SEATBELT_REMINDER,cTT_TURN_OFF);

	}
	else if(u32TimeCnt < 100)
	{
		l_dr_seatblt_tt_status_u8 = cTT_TURN_OFF;
		Rte_Call_rpCS_SeatBeltMdl_TIMdlUpdateTtStatus_Operation(cTTDRIVER_SEATBELT_REMINDER,cTT_TURN_OFF);
		//l_ps_seatblt_tt_status_u8 = cTT_TURN_OFF;
		//Rte_Call_rpCS_SeatBeltMdl_TIMdlUpdateTtStatus_Operation(cTTPASSENGER_SEATBELT_REMINDER,cTT_TURN_OFF);
	}
	else
	{
		u32TimeCnt = 0;
	}
	u32TimeCnt++;

#endif
}

