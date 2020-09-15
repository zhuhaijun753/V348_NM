/******************************************************************************
              CONFIDENTIAL VISTEON CORPORATION

 This is an unpublished work of authorship, which contains trade secrets,
 created in 2012. Visteon Corporation owns all rights to this work and
 intends to maintain it in confidence to preserve its trade secret status.
 Visteon Corporation reserves the right, under the copyright laws of the
 United States or those of any other country that may have  jurisdiction,
 to protect this work as an unpublished work, in the event of an
 inadvertent or deliberate unauthorized publication. Visteon Corporation
 also reserves its rights under all copyright laws to protect this work as
 a published work, when appropriate. Those having access to this work may
 not copy it, use it, modify it or disclose the information contained in
 it without the written authorization of Visteon Corporation
******************************************************************************/
/*****************************************************************************

File Name        :  TPMSRoutineMdl.c
Module Short Name:  TPMSRoutineMdl.c
VOBName          :  
Author           :  
Description      : 

Organization     :  Driver Information Software Section,
                    Visteon Corporation
----------------------------------------------------------------------------
Compiler Name    :  
Target Processor :  
******************************************************************************/
#define TPMSRoutineMdl_C

#include "Rte_TPMSRoutineMdl.h"
#include "CmpLib.h"
#include "Rte.h"
#include "Rte_type.h"

//#include "OdoMdl.h"

/*@brief  Instance of life-cycle state-machine*/

CMPLIB_INSTANCE(TPMSRoutineMdl)

/*****************************************************************************
*                                 Constants                                  *
*----------------------------------------------------------------------------*
* Definition of constant shall be followed by a comment that explains the    *
* purpose of the constant.                                                   *
******************************************************************************/
#if 0
static uint8	l_Tpms_LF_Tire_P_status_U8;
static uint8	l_Tpms_RF_Tire_P_status_U8;
static uint8	l_Tpms_RR_Tire_P_status_U8;
static uint8	l_Tpms_LR_Tire_P_status_U8;
static uint8	l_Tpms_LF_Tire_T_status_U8;
static uint8	l_Tpms_RF_Tire_T_status_U8;
static uint8	l_Tpms_RR_Tire_T_status_U8;
static uint8	l_Tpms_LR_Tire_T_status_U8;
static uint8	l_Tpms_LF_Tire_SYS_status_U8;
static uint8	l_Tpms_RF_Tire_SYS_status_U8;
static uint8	l_Tpms_RR_Tire_SYS_status_U8;
static uint8	l_Tpms_LR_Tire_SYS_status_U8;
static uint8    fl_signal_LF_Pressure_WarningSt_Last_U8 = FALSE;
static uint8    fl_signal_RF_Pressure_WarningSt_Last_U8 = FALSE;
static uint8    fl_signal_RR_Pressure_WarningSt_Last_U8 = FALSE;
static uint8    fl_signal_LR_Pressure_WarningSt_Last_U8 = FALSE;
static uint8    fl_signal_Temperature_WarningSt_Last_U8 = FALSE;
static uint8    fl_signal_TirePosition_WarningSt_Last_U8 = FALSE;
static uint8    fl_signal_SystemSt_Last_U8 = FALSE;

#endif




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
#if 0
#define Tpms_LF_Pressure_Waring_1     0x01
#define Tpms_LF_Pressure_Waring_2     0x02
#define Tpms_LF_Pressure_Waring_4     0x04
#define Tpms_LF_Pressure_Waring_5     0x05
	
#define Tpms_RF_Pressure_Waring_1     0x01
#define Tpms_RF_Pressure_Waring_2     0x02
#define Tpms_RF_Pressure_Waring_4     0x04
#define Tpms_RF_Pressure_Waring_5     0x05
	
#define Tpms_RR_Pressure_Waring_1     0x01
#define Tpms_RR_Pressure_Waring_2     0x02
#define Tpms_RR_Pressure_Waring_4     0x04
#define Tpms_RR_Pressure_Waring_5     0x05
	
#define Tpms_LR_Pressure_Waring_1     0x01
#define Tpms_LR_Pressure_Waring_2     0x02
#define Tpms_LR_Pressure_Waring_4     0x04
#define Tpms_LR_Pressure_Waring_5     0x05
	
#define Temperature_Waring            0x01
	
#define TriePosition_0                0x00
#define TriePosition_1                0x01
#define TriePosition_2                0x02
#define TriePosition_3                0x03
#define TriePosition_4                0x04
	
#define System_Status_1               0x01
#define System_Status_2               0x02
#endif
/*                                 Type Declarations                          *
******************************************************************************/


/*****************************************************************************
*                                Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable.                                                        *
* purpose, critical section, unit, resolution, Valid Range and ValidityCheck *
******************************************************************************/
//=====================================================================================================================
//	FORWARD DECLARATIONS
//=====================================================================================================================
#if 0
static void f_tpmsMdl_initial(void);
static void f_tpmsMdl_Process(void);
static boolean f_tpmsMdl_input_verify(void);
#endif

/*****************************************************************************
*                   Functions                                                *
******************************************************************************/



//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{

	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional de-initialization state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{

	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional activation state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress. Keeps the component in this state.
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActivation( void )
{

	return E_OK;
}
//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional de-activation state
///
/// @return E_OK:			Succeed
///<br> 	E_NOT_READY:	In progress
///<br> 	E_NOT_OK:		Failed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeActivation( void )
{

	return E_OK;
}


//---------------------------------------------------------------------------------------------------------------------
/// @brief	State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive( void )
{	
#if 0
	eIgnState fl_IGN_state;
	EBatteryState fl_Battery_state;
    boolean	f_ret = FALSE;

	Rte_Read_rpBattState_BatteryState(&fl_Battery_state);
	Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
	
	if(fl_Battery_state == eBatteryState_UnderVoltage || fl_Battery_state == eBatteryState_OverVoltage)
	{
		f_tpmsMdl_initial();
	}
	else
	{
		f_ret = f_tpmsMdl_input_verify();
		if(f_ret == TRUE)
        {
	    	switch(fl_IGN_state)
	    	{
		    	case eIGN_RUN:
		    		f_tpmsMdl_Process();
			    	break;
		    	case eIGN_OFF:					
		    		f_tpmsMdl_initial();
		    	default:
		    		break;
		    }
        }
        else
        {
           	f_tpmsMdl_initial();
        }
	}
#endif
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Diagnostic state
///
/// @return E_OK:			Diagnostic has completed. Go to DiagReturn state.
///<br> 	E_NOT_READY:	Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiag( void )
{
	return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief	Transitional state to clean up after diagnostic state. Component behaviour to be restored.
///
/// @return E_OK:			Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br> 	E_NOT_READY:	Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
	return E_OK;
}

#if 0
static void f_tpmsMdl_initial(void)
{
	l_Tpms_LF_Tire_P_status_U8 = FALSE;
	l_Tpms_RF_Tire_P_status_U8 = FALSE;
	l_Tpms_RR_Tire_P_status_U8 = FALSE;
	l_Tpms_LR_Tire_P_status_U8 = FALSE;
	l_Tpms_LF_Tire_T_status_U8 = FALSE;
	l_Tpms_RF_Tire_T_status_U8 = FALSE;
	l_Tpms_RR_Tire_T_status_U8 = FALSE;
	l_Tpms_LR_Tire_T_status_U8 = FALSE;
	l_Tpms_LF_Tire_SYS_status_U8 = FALSE;
	l_Tpms_RF_Tire_SYS_status_U8 = FALSE;
	l_Tpms_RR_Tire_SYS_status_U8 = FALSE;
	l_Tpms_LR_Tire_SYS_status_U8 = FALSE;
    fl_signal_LF_Pressure_WarningSt_Last_U8 = FALSE;
    fl_signal_RF_Pressure_WarningSt_Last_U8 = FALSE;
    fl_signal_RR_Pressure_WarningSt_Last_U8 = FALSE;
    fl_signal_LR_Pressure_WarningSt_Last_U8 = FALSE;
    fl_signal_Temperature_WarningSt_Last_U8 = FALSE;
    fl_signal_TirePosition_WarningSt_Last_U8 = FALSE;
    fl_signal_SystemSt_Last_U8 = FALSE;
}

static boolean f_tpmsMdl_input_verify(void)
{
	boolean	f_ret = TRUE;
	uint8 	fl_can_nvr_status_U8 = 0;
	uint8 	fl_can_msg_status_U8 = 0;
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_NR(&fl_can_nvr_status_U8);
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tout(&fl_can_msg_status_U8);
	if(fl_can_nvr_status_U8 == RTE_E_NEVER_RECEIVED || fl_can_msg_status_U8 == RTE_E_TIMEOUT)
	{
		f_ret = FALSE;
	}
	
	return f_ret;
}


static void f_tpmsMdl_Process(void)
{
	uint8    fl_signal_LF_Pressure_WarningSt_U8 = 0;
    uint8    fl_signal_RF_Pressure_WarningSt_U8 = 0;
    uint8    fl_signal_RR_Pressure_WarningSt_U8 = 0;
    uint8    fl_signal_LR_Pressure_WarningSt_U8 = 0;
    uint8    fl_signal_Temperature_WarningSt_U8 = 0;
    uint8    fl_signal_TirePosition_WarningSt_U8 = 0;
    uint8    fl_signal_SystemSt_U8 = 0;

	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LF_Pressure_Warning(&fl_signal_LF_Pressure_WarningSt_U8); 
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RF_Pressure_Warning(&fl_signal_RF_Pressure_WarningSt_U8); 	
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_RR_Pressure_Warning(&fl_signal_RR_Pressure_WarningSt_U8); 
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_LR_Pressure_Warning(&fl_signal_LR_Pressure_WarningSt_U8); 
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Temperature_Warning(&fl_signal_Temperature_WarningSt_U8); 
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_Tire_Position(&fl_signal_TirePosition_WarningSt_U8); 
	Rte_Read_rpSR_CANMSG_TPMS_Tire_0x361_ComIn_TPMS_System_Status(&fl_signal_SystemSt_U8);

	if((fl_signal_LF_Pressure_WarningSt_U8 == Tpms_LF_Pressure_Waring_1)
	|| (fl_signal_LF_Pressure_WarningSt_U8 == Tpms_LF_Pressure_Waring_2)
	|| (fl_signal_LF_Pressure_WarningSt_U8 == Tpms_LF_Pressure_Waring_4)
	|| (fl_signal_LF_Pressure_WarningSt_U8 == Tpms_LF_Pressure_Waring_5))
	{
		if(fl_signal_LF_Pressure_WarningSt_Last_U8 != fl_signal_LF_Pressure_WarningSt_U8)
		{
			l_Tpms_LF_Tire_P_status_U8 = TRUE;
		}
	}
	
}
#endif

