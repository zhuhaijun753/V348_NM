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
#include "Rte_DPFDTEMdl.h"
#include "CmpLib.h"
#include "DpfDteMdl_cfg.h"
//#include "FuelLevelMdl_cfg.h"
//#include "fuel03.h"


/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(DPFDTEMdl)


/*****************************************************************************
* 							   Globally  accessed Variable Declarations    *
*----------------------------------------------------------------------------*
* Declaration shall be followed by a comment that gives the following info.  *
* about the variable. 													   *
* purpose, critical section, unit, and resolution 						   *
******************************************************************************/
#define DPFWARNING_NORMAL       0x7FFE
#define DPFWARING_ABNORMAL      0x7FFF
#define DPFDTE_INVALID          0x0FFF
#define DPFDTE_SIGNAL_LOSS      0x7FFD
#define DPFDTE_DISPLAY_CYCLE    350
#define Max_EMS_Soot_Pc_Dsply	(127*2)
#define INVALID_EMS_Soot_Pc_Dsply 0xFFFF
#define ISENGINE                 0x8FFF

//#define	Rte_Call_rptimer_TmExt_Timer_IsElapsed		TmExt_Timer_IsElapsed
//#define	Rte_Call_rpTimer_TmExt_Timer_Start				TmExt_Timer_Start
//#define	Rte_Call_rptimer_TmExt_Timer_Stop				TmExt_Timer_Stop
//#define	Rte_Call_rpTimer_TmExt_Timer_IsStarted		TmExt_Timer_IsStarted
/*============================================================================
** M E M O R Y   A L L O C A T I O N
**==========================================================================*/

static uint8 message_invalid(void);
//static void Calculation_DpfDte_process(void);
//static void DpfDte_init(void);
static void Dpf_ManRGen_CanMessageOutput(void);
static void EMS_Soot_Pc_Dsply_Process(void);
//static boolean Message_0x368_invild_exe(void);
static void EMS_UreaLvl_Dsply_Process(void);

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional initialization state
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpInit( void )
{
//	DpfDte_init();
	return E_OK;	
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional de-initialization state
** 
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDeInit( void )
{
//    DpfDte_init();
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional activation state
** 
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress. Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpActivation( void )
{	
 //   DpfDte_init();
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional de-activation state
** 
** @return E_OK:           Succeed
**         E_NOT_READY:    In progress
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDeActivation( void )
{	
//    DpfDte_init();
    return  E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  State for normal operations
** 
** @return The return value is disregarded. Say E_OK
**---------------------------------------------------------------------------------------------------------------------*/
//#define eeprom_rte
static Std_ReturnType CmpActive( void )
{			
	//eIgnState fl_ign;
    EBatteryState fl_batt;
	boolean IsEngineCfg = FALSE;
//	Rte_Call_GetVehicleCfg_Operation(VEHICLE_CONFIGURATION_Engine,&IsEngineCfg);
	Rte_Read_rpBattState_BatteryState(&fl_batt);
	if(FALSE==IsEngineCfg)
	{
		if ((fl_batt != eBatteryState_OverVoltage) && (fl_batt != eBatteryState_UnderVoltage)) /*overPh1,normal,underPh1*/
		{

		#if 0
		   if(FALSE == message_invalid())
		   {
		       Calculation_DpfDte_process();
		   }
		   else
		   {
		       Rte_Write_ppSR_DPFDTEMdl_DteVal_Value(DPFWARING_ABNORMAL);/*HMI diaplay --- */
		   }
		#endif
		   EMS_Soot_Pc_Dsply_Process();
		   EMS_UreaLvl_Dsply_Process();
//		   Message_0x368_invild_exe();

		 }
		else
		{
			 //  DpfDte_init();
			   //Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(INVALID_EMS_Soot_Pc_Dsply);/*voltage fault*/
			   Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(INVALID_EMS_Soot_Pc_Dsply);
		}
	}
	else
	{
	//	Rte_Write_ppSR_DPFDTEMdl_DteVal_Value(DPFDTE_INVALID);
	//	Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(ISENGINE);/*Diesel car*/
	  Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(INVALID_EMS_Soot_Pc_Dsply);
	}
	Dpf_ManRGen_CanMessageOutput();//add by lhan4
	return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Diagnostic state
** 
** @return E_OK:           Diagnostic has completed. Go to DiagReturn state.
**         E_NOT_READY:    Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDiag( void )
{
    return E_OK;
}

/*---------------------------------------------------------------------------------------------------------------------
** @brief  Transitional state to clean up after diagnostic state. Component behaviour to be restored.
** 
** @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
**         E_NOT_READY:    Keeps the component in this state.
**         E_NOT_OK:       Failed
**---------------------------------------------------------------------------------------------------------------------*/
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}

static uint8 message_invalid(void)
{
   uint8 u8MsgNR = 0;
   uint8 u8MsgTout = 0;

   Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_NR(&u8MsgNR);
   Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_Tout(&u8MsgTout);
   
   
   if ((RTE_E_TIMEOUT == u8MsgTout) || (RTE_E_NEVER_RECEIVED == u8MsgNR))
   {
       return TRUE;
   }
   else
   {
       return FALSE;
   }
}

#if 0
static void Calculation_DpfDte_process(void)
{
    uint16 EMS_UreaDTE_Value = 0;
    boolean  dpfdiaplay_cycle;
    boolean  dpfdiaplay_cycle_already;
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaDTE(&EMS_UreaDTE_Value);
    Rte_Call_rpTimer_TmExt_Timer_IsStarted(eTimerHandleDPFDTEWait,&dpfdiaplay_cycle);
    if((0xFA1 <= EMS_UreaDTE_Value) && (EMS_UreaDTE_Value <= 0XFFF))
    {
       Rte_Write_ppSR_DPFDTEMdl_DteVal_Value(DPFWARNING_NORMAL);/*HMI diaplay --- KM*/
    }
    else
    {
       if(FALSE == dpfdiaplay_cycle)
       {
            Rte_Call_rpTimer_TmExt_Timer_Start(eTimerHandleDPFDTEWait);
       }
            Rte_Call_rpTimer_TmExt_Timer_IsElapsed(eTimerHandleDPFDTEWait,DPFDTE_DISPLAY_CYCLE,&dpfdiaplay_cycle_already);
       if(TRUE==dpfdiaplay_cycle_already)
       {
            Rte_Write_ppSR_DPFDTEMdl_DteVal_Value(EMS_UreaDTE_Value);  /*HMI display XXX KM*/
            Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleDPFDTEWait);
       }
    }
}


static void DpfDte_init(void)
{
    Rte_Call_rpTimer_TmExt_Timer_Stop(eTimerHandleDPFDTEWait);
}
#endif
static void Dpf_ManRGen_CanMessageOutput(void)
{
	uint8 dpf_generation_status = 0;
	uint8 DieslPrtcWarn_D_Rq = 0;
	
	Rte_Read_rpSR_DPF_Generation_Status(&dpf_generation_status);
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq(&DieslPrtcWarn_D_Rq);
	if((DieslPrtcWarn_D_Rq == 0x06) || (DieslPrtcWarn_D_Rq == 0x07))
	{
		dpf_generation_status = 0;
		Rte_Write_ppSR_DPF_Generation_Status(dpf_generation_status);
		Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_ManRgen_D_Rq(dpf_generation_status);
	}
	else
	{
		Rte_Write_ppSR_CANMSG_IC_0x525_ComOut_IC_ManRgen_D_Rq(dpf_generation_status);
	}

}

static void EMS_UreaLvl_Dsply_Process(void)
{
	#if 0

	#else
	uint8 ems_urealvl;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_UreaLvl(&ems_urealvl);


	if(FALSE == message_invalid())
	{
		if (ems_urealvl == 0xFF)
		{
			Rte_Write_ppSR_DPFDTEMdl_DteVal_EMS_UreaLvl_Dsply(0xFF);
		}
		else
		{
			ems_urealvl = (4*ems_urealvl)/10;
			if (ems_urealvl > 100)
			{
				ems_urealvl = 100;
			}
			Rte_Write_ppSR_DPFDTEMdl_DteVal_EMS_UreaLvl_Dsply(ems_urealvl);
		}
	}
	else
	{

		Rte_Write_ppSR_DPFDTEMdl_DteVal_EMS_UreaLvl_Dsply(0xFF);
	}
	#endif
	
}

static void EMS_Soot_Pc_Dsply_Process(void)
{
	uint8   EMS_Soot_Pc_Dsply;
	uint8 	ManRgenTxt_D_RqDsply;
	uint8 	DieslPrtcWarn_D_Rq;
	uint16  EMS_Soot_Pc_Dsply_u16;
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_Soot_Pc_Dsply(&EMS_Soot_Pc_Dsply);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_ManRgenTxt_D_RqDsply(&ManRgenTxt_D_RqDsply);
	Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_DieslPrtcWarn_D_Rq(&DieslPrtcWarn_D_Rq);
	EMS_Soot_Pc_Dsply_u16=EMS_Soot_Pc_Dsply*2;
	if(FALSE == message_invalid())
	{
		if(EMS_Soot_Pc_Dsply_u16 != (0x7F*2))
		{
			if((ManRgenTxt_D_RqDsply == 0) && (DieslPrtcWarn_D_Rq == 2))
			{
				Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(0x7FFF);						
			}
		    else if((ManRgenTxt_D_RqDsply == 1) && (DieslPrtcWarn_D_Rq == 2))
		    {
				Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(0x7FFE);						
			}
			else if (DieslPrtcWarn_D_Rq == 1)
			{
				Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(0x62);
			}
			else if(DieslPrtcWarn_D_Rq == 3)
			{
				Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(0x7FFD);							
			}
			else if(DieslPrtcWarn_D_Rq == 4)
			{
				Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(0x7FFC);							
			}
			else if(DieslPrtcWarn_D_Rq == 5)
			{
				Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(0x7FFB);							
			}
			else if(DieslPrtcWarn_D_Rq == 6)
			{
				Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(0x7FFA);							
			}
			else if(DieslPrtcWarn_D_Rq == 7)
			{
				Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(0x7FF9);
			}
			else if (DieslPrtcWarn_D_Rq == 0)
			{
				if(EMS_Soot_Pc_Dsply_u16 >= 98)
				{
					EMS_Soot_Pc_Dsply_u16 = 98;
				}		
				Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(EMS_Soot_Pc_Dsply_u16);			
			}
			else
			{
				Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(INVALID_EMS_Soot_Pc_Dsply);
			}
		}
		else 
		{
	//		if(FALSE == Message_0x368_invild_exe()) /*invild*/
		//	{
				//EMS_Soot_Pc_Dsply_u16 = INVALID_EMS_Soot_Pc_Dsply;
				Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(INVALID_EMS_Soot_Pc_Dsply);

		//	}
		}
    }
	else
	{
		Rte_Write_DPFDTEMdl_ppsr_EMS_Soot_Pc_Dsply_EMS_Soot_Pc_Dsply(INVALID_EMS_Soot_Pc_Dsply);/*signal error*/
	}
}

#if 0
static boolean Message_0x368_invild_exe(void)
{
	static uint8 count=0;
	uint8  EMS_Soot_Pc_Dsply;
	uint16  EMS_Soot_Pc_Dsply_u16;
    Rte_Read_rpSR_CANMSG_GW_EMS_0x368_ComIn_EMS_Soot_Pc_Dsply(&EMS_Soot_Pc_Dsply); 
	EMS_Soot_Pc_Dsply_u16 = EMS_Soot_Pc_Dsply * 2;
	if(EMS_Soot_Pc_Dsply_u16 == 254)
	{
		count++;
		if(count <= 28) /*2S timer*/
		{
			return TRUE;	
		}
		else
		{
			if(count == 255)
			{
				count = 29;
			}
			return FALSE;
		}
	}
	else 
	{
		count = 0;
		return TRUE;
	}

}
#endif
