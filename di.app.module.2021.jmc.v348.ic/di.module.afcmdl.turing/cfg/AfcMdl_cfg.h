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

File Name        :  AFCMdl_cfg.h
Module Short Name:  foat
VOBName          :  2014_Nissan_P42M_ic
Author           :  jmunisam
Description      :  Handles the AFC application feature

Organization     :  Driver Information Software Section,
                    Visteon Technical and Service Centre, India
                    Visteon Corporation

******************************************************************************/
#ifndef AFCMDL_CFG_H
#define AFCMDL_CFG_H



typedef enum eODO_UINT
{
    ODO_1000M = 0,
    ODO_100M  = 1,
    ODO_10M   = 2,
    ODO_1M    = 3
} tODO_UINT;

extern uint32 OdoLogicalMdl_get_main(tODO_UINT uint);
extern uint32 fafc_get_single_value(void);
extern uint32 fafc_get_accumulate_value(void);
extern uint32 fafc_get_refuel_value(void);
extern void fafc_clear_single_value(void);
extern void fafc_clear_acc_value(void);
extern void fuel03_set_refuel_flag(boolean flag);
extern boolean fuel03_get_refuel_flag(void);


/*****************************************************************************
*                                 Type Decleration                           *
*----------------------------------------------------------------------------*
* Decleration of type shall be accompanied by a comment that explains the    *
* purpose and usage of the type.                                             *
******************************************************************************/

#define AFC_TACH_300RPM                			(uint16)(30000)			//300rpm
#define AFC_SPEED_3KM							(uint16)(300)			//3km/h
#define AFC_TimeOut_300S            			(uint32)(300000)		//300s
#define AFC_IDLE_STATUS							(uint8)(0)
#define AFC_NO_IDLE_LOOP_NUMBER                 (uint16)(1000)			//10s
#define AFC_IDLE_LOOP_NUMBER                    (uint16)(3000)			//30s
#define ENG_SPD_RESOLUTION          			(uint16)2
#define MAX_A_VALUE                 			(uint32)1000
#define MIN_B_VALUE                 			(uint16)0
#define AFC_INVALID_VALUE             			(uint32)(0xFFFF)
#define AFC_DEFAULT_VALUE             			(uint8)(65)
#define AFC_CALC_CONST_VALUE        			(uint32)(131)
#define AFC_CALC_FACTOR							(uint8)2
#define AFC_CALC_MOVE_VALUE         			(uint16)16
#define AFC_MAX_OF_VALUE            			(uint32)(0xFFFFFFFF)
#define AFC_DISTANCE_300M            			(uint32)300
#define AFC_MAX_CALC_VALUE          			(uint32)300
#define AFC_DISPLAY_PERCENT         			(uint32)110
#define FODO_ODO_NVM_ERROR      				(uint32)(0xFFFFFFFFul)
#define RAFC_VEHICLE_TYPE_15GL_2WD_AT			(uint32)(790)
#define RAFC_VEHICLE_TYPE_15GLS_2WD_AT			(uint32)(790)
#define RAFC_VEHICLE_TYPE_15SLX_2WD_AT			(uint32)(790)
#define RAFC_VEHICLE_TYPE_15GLS_4WD_AT			(uint32)(810)
#define RAFC_VEHICLE_TYPE_15SLX_4WD_AT			(uint32)(810)
#define RAFC_VEHICLE_TYPE_15GLS_2WD_MT			(uint32)(790)
#define RAFC_VEHICLE_TYPE_JMC_N356  			(uint32)(860)
#define NVM_AFC_C_VALUE							(uint8)2		/* zoom 1000 , raw value is 0.2%*/
/*max F(k) value, unit is mL*/
#define MAX_F_VALUE   							(uint32)8589900
#define MAX_D_VALUE   							(uint32)53129200
#define AFC_INVALID_STATUS						(uint8)1
#define AFC_COMPUTER1_DISPLAY					(uint8)3
#define AFC_COMPUTER2_DISPLAY					(uint8)0
#define AFC_MAX_VALUE		  					(uint16)(300)
#define AFC_MIN_VALUE 		  					(uint16)(65)
#define AFC_BATTARY_FIRST_ON					(uint8)1
#define AFC_GAS_RAFC							(uint16)810
#define AFC_DIESEL_RAFC							(uint16)720

#define Rte_Read_rpSR_AfcMdl_OdoValue(x)  			OdoLogicalMdl_get_main(x)
#define Rte_Read_rpSR_AfcMdl_Clear_Single_Value		fafc_clear_single_value
#define Rte_Read_rpSR_AfcMdl_Clear_Acc_Value		fafc_clear_acc_value


static const uint32 NVM_AFC_AFE_VALUE[7] = { RAFC_VEHICLE_TYPE_JMC_N356,
									  RAFC_VEHICLE_TYPE_15GLS_2WD_MT,
                                      RAFC_VEHICLE_TYPE_15GL_2WD_AT,
                                      RAFC_VEHICLE_TYPE_15GLS_2WD_AT,
                                      RAFC_VEHICLE_TYPE_15GLS_4WD_AT, 
                                      RAFC_VEHICLE_TYPE_15SLX_2WD_AT, 
                                      RAFC_VEHICLE_TYPE_15SLX_4WD_AT };

#endif
