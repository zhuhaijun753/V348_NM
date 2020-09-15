/*************************************************************************************************************************
**  File Name:  ChimeLogical.c
**  Summary:    Manage the chime
**  Create:     2018-06-01
**  Author:     Liu Yang
**  Update history:
**  NO.     Author         Date              Content
**  1.      Liu Yang      2018-06-01        	New
*************************************************************************************************************************/

/* ============================ Version =============================================================================== */

/* ============================ Include Files ========================================================================= */
#include "Rte_ChimeMdl.h"
#include "Rte_ChimeMdl_Type.h"
#include "ChimeLogical.h"


#define CHIME_TASK_TIME           (100)                                        /* unit 10 ms, chime task schedule time */
#define CHIME_SEATBELT_COUNT      (30)           							   /* Seatbelt timer count, 30 times */
#define CHIME_TPMS_COUNT   	      (1)           							   /* TPMS counter, 1 times */
#define CHIME_DOOROPEN_COUNT   	  (5)           							   /* Door Open counter, 5 times */
#define CHIME_SCR_1HZ_COUNT       (3)
#define CHIME_SCR_2HZ_1_COUNT     (3)
#define CHIME_SCR_2HZ_2_COUNT     (3) 
#define CHIME_ADAS_WA_COUNT       (4)           							   /* ADAS counter, 9 times */
#define CHIME_ADAS_NS_COUNT       (12)           							   /* ADAS counter, 9 times */
#define CHIME_ADAS_HR_COUNT       (1)           							   /* ADAS counter, 9 times */
#define CHIME_ADAS_TO_COUNT       (1)           							   /* ADAS counter, 9 times */
#define CHIME_ADAS_PS_COUNT       (1)           							   /* ADAS counter, 9 times */
//#define CHIME_ADAS_BS_COUNT       (4)           							   /* ADAS counter, 9 times */
//#define CHIME_ADAS_CTA_COUNT      (4)           							   /* ADAS counter, 9 times */
//#define CHIME_ADAS_DOA_COUNT      (4)           							   /* ADAS counter, 9 times */
#define CHIME_POSLMP_COUNT        (30)                                         /* poslmp count, 30 times */
#define CHIME_BKHOME_COUNT        (1)                                          /* BACKHOME count, 1 time*/
#define CHIME_PEPS_SSB_COUNT      (3)                                          /* PEPS counter, 3 times */
//#define CHIME_PEPS_BRK_COUNT      (3)                                          /* PEPS counter, 3 times */
#define CHIME_PEPS_D2_SSB_COUNT   (3)
//#define CHIME_PEPS_D2_BRK_COUNT   (3)
#define CHIME_PEPS_D2_ESCL_COUNT  (1) 
#define CHIME_PEPS_COUNT          (1) 
#define CHIME_NOT_IN_P_COUNT      (5)
//#define CHIME_Over_Spd_COUNT      (10)
#define CHIME_Low_OilPress_COUNT  (6)
#define CHIME_ENG_HIGH_TEMP_COUNT (3)                                          /* Engine high temperature counter, 3 times*/

/* ============================ Global Variables ====================================================================== */
static CHIME_STATUS_e   	gs_eChimeState;                                        /* domain state */
static UINT8            	gs_u8CurChimeSort;                                     /* current Chime ID */
//static UINT8            	l_interruptChime_U8;                                   /* interrupt Chime ID */
static UINT8    			gs_u8Chime_WarnInput[CHIME_REQ_TOTAL_NUM];             /* current input signal */
static UINT8     			gs_u8Chime_WarnInput_Last[CHIME_REQ_TOTAL_NUM];        /* last input signal */
static UINT8     			gs_u8Chime_WarnInput_Req[CHIME_REQ_TOTAL_NUM];         /* request for sound play */
static UINT8     			gs_u8Chime_Warn_ToutSts[CHIME_REQ_TOTAL_NUM];          /* request for sound play */
static UINT8            	gs_u8ChimeFuncState;                                   /* state to judge if chime enabled */
static UINT8            	gs_u8ChimeFuncState_Last;                              /* last state */
static UINT8 				gs_u8Chime_CurMode;                                    /* record last mode, normal/ETM */
static EKeyPos   			gs_eChime_KeySts;                                      /* current power mode */
static EKeyPos   			gs_eChime_KeySts_Last;                                 /* last power mode */
static UINT8            	gs_u8CurChimeResID;                                    /* sound resource ID */
static CHIME_PROC_STATUS_e 	gs_eChimeProcSts;                                      /* state for process pop up warning */
static UINT8 				gs_u8Chime_Respond;
static UINT8            	gs_u8ChimeWarnSts;                                     /* judge if chime is idle for warning to change */
static CHIME_RESP_DATA_t_ 	gs_tChimeRespData[CHIME_RESP_NUM];

static UINT8 gs_u8Chime_ATS_DMC_count;
static UINT8 gs_u8Chime_ATS_count;
static UINT8 gs_u8Chime_LowFule_count;
static UINT8 gs_u8Chime_STT_count;
static UINT8 gs_u8Chime_DoorOpenCount;                                         	   /* timer counter for DoorOpen Warning */
//static UINT8 gs_u8Chime_Over_Spd_Count;
static UINT8 gs_u8Chime_Low_OilPress_Count;
static UINT8 gs_u8Chime_TPMS_Count;
static UINT8 gs_u8Chime_SCR_1HZCount;
static UINT8 gs_u8Chime_SCR_2HZ_1Count;
static UINT8 gs_u8Chime_SCR_2HZ_2Count;
static UINT8 gs_u8Chime_DPF_Count;
static UINT8 gs_u8Chime_ADAS_WarningAudio_Count;
static UINT8 gs_u8Chime_ADAS_NavSts_Count;
static UINT8 gs_u8Chime_ADAS_HandsonReq_Count;
static UINT8 gs_u8Chime_ADAS_TAKE_OVER_Count;
static UINT8 gs_u8Chime_ADAS_PCW_STATE_Count;
//static UINT8 gs_u8Chime_ADAS_BSDSound_Count;
//static UINT8 gs_u8Chime_ADAS_CTA_Count;
//static UINT8 gs_u8Chime_ADAS_DOA_Count;
static UINT8 gs_u8Chime_D2_POSLMP_Count;
static UINT8 gs_u8Chime_D2_BKHOME_Count;
static UINT8 gs_u8Chime_D2_PEPS_REMAIN_NUM_Count;

static UINT8 gs_u8Chime_PEPS_IMMO_Count;
static UINT8 gs_u8Chime_PEPS_SSB_count;
static UINT8 gs_u8Chime_PEPS_ESCL_Count;
static UINT8 gs_u8Chime_PEPS_SYS_count;
static UINT8 gs_u8Chime_PEPS_DOOR_Count;
static UINT8 gs_u8Chime_PEPS_NOTOFF_Count;
static UINT8 gs_u8Chime_PEPS_NOKEY_Count;
static UINT8 gs_u8Chime_PEPS_KEYOUT_Count;
static UINT8 gs_u8Chime_PEPS_BRK_Count;
static UINT8 gs_u8Chime_PEPS_CLUTCH_Count;
static UINT8 gs_u8Chime_PEPS_NOTPN_Count;

static UINT8 gs_u8Chime_PEPS_KEY_BAT_Count;
static UINT16 gs_u16Chime_SeatBelt_Count;                                         /* timer counter for Seatbelt Warning */
static UINT16 gs_u16Chime_PsSeatBelt_Count;   
static UINT16 gs_u16Chime_D2_KEY_IN_ON_Count;   
static UINT16 gs_u16Chime_NOT_IN_P_Count;
static UINT16 gs_u16Chime_D2_NOT_IN_P_Count; 
static UINT8  gs_u8Chime_Eng_High_Temp_Count;                                     /* timer counter for Engine Hight temperature Warning */

static UINT8 gs_u8Chime_D2_PEPS_SSB_count;
static UINT8 gs_u8Chime_D2_PEPS_ESCL_Count;
static UINT8 gs_u8Chime_D2_PEPS_SYS_count;
static UINT8 gs_u8Chime_D2_PEPS_DOOR_Count;
static UINT8 gs_u8Chime_D2_PEPS_NOTOFF_Count;
static UINT8 gs_u8Chime_D2_PEPS_NOKEY_Count;
static UINT8 gs_u8Chime_D2_PEPS_KEYOUT_Count;
static UINT8 gs_u8Chime_D2_PEPS_BRK_Count;
static UINT8 gs_u8Chime_D2_PEPS_CLUTCH_Count;
static UINT8 gs_u8Chime_D2_PEPS_NOTPN_Count;
static UINT8 gs_u8Chime_D2_LIGHT_ON_Count;
// static UINT8 gs_u8NOT_IN_P_D2_CntMax=5;
static UINT8 gs_u8SSB_D2_CntMax=3;
static UINT8 gs_u8SSB_D1_CntMax=3;
static UINT8 gs_u8ESCL_D2_CntMax=1;
static UINT8 gs_u8ESCL_D1_CntMax=1;
static UINT8 gs_u8SYS_D1_CntMax=1;
static UINT8 gs_u8DOOR_D1_CntMax=1;
static UINT8 gs_u8NOTOFF_D1_CntMax=3;
static UINT8 gs_u8NOKEY_D1_CntMax=1;
static UINT8 gs_u8KEYOUT_D1_CntMax=1;
static UINT8 gs_u8BRK_D1_CntMax=3;
static UINT8 gs_u8CLUTCH_D1_CntMax=1;
static UINT8 gs_u8NOTPN_D1_CntMax=1;
static UINT8 gs_u8SYS_D2_CntMax=1;
static UINT8 gs_u8DOOR_D2_CntMax=1;
static UINT8 gs_u8NOTOFF_D2_CntMax=3;
static UINT8 gs_u8NOKEY_D2_CntMax=1;
static UINT8 gs_u8KEYOUT_D2_CntMax=1;
static UINT8 gs_u8BRK_D2_CntMax=1;
static UINT8 gs_u8CLUTCH_D2_CntMax=1;
static UINT8 gs_u8NOTPN_D2_CntMax=1;
static UINT8 gs_u8ChimeDrvSts_Last;                                 /* last sound driver state */
static UINT8 gs_u8ChimeDrvSts;                                      /* sound driver state for current sound */


#if (cUSE_CHM_MANAGER_PROVEOUT != 0)
	//static uint16 u16ChmProveoutTimer;
	boolean    ChmIsProveoutDone;

#endif 

/*--------------------------------------------------------------------------------------------------------------
**  Sound Source        Sound Res
**  Chime1(Tic)         eSnd_N331_Tick
**  Chime2(Toc)         eSnd_N331_Tock
**  Chime7              eSnd_N331_TPMS_1000ms
**  Chime8_1Hz          eSnd_N331_PosLmp_1000ms
**  Chime9              eSnd_N331_ACC_250ms
**  Chime15             eSnd_N331_Radar_250ms
**  Chime17_1Hz         eSnd_N331_Chime_17_1hz
**  Chime17_2Hz         eSnd_N331_Chime_17_2hz
**  Chime17_4Hz         eSnd_N331_Chime_17_4hz
**  Chime17_Long        eSnd_N331_Chime_17_long
**  Chime22             eSnd_N331_ACC_2hz_250ms
**  Chime25_1Hz         eSnd_N331_SeatBelt_1000ms    
**  Chime30             eSnd_N331_SeatBelt_1000ms_05hz
**  Chime45_2Hz         eSnd_N331_BSD_250ms
**  Chime48             eSnd_N331_Radar_500ms


**--------------------------------------------------------------------------------------------------------------*/

static const CHIME_DATA_t_ gs_tChime_D1Data_Tbl[CHIME_IGON_NUM] =              /* look-up table for chime config information */
{
        /* Chime ID */                  /* Request Type */                /* Priority *//* Sound Res */     /* Cycle Type *//* Request Input Type */
/*1*/   {CHIME_NULL                     ,CHIME_REQID_NULL                   ,100,eSnd_NoChimeID     		,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*2*/   {CHIME_TURN_TICK                ,CHIME_REQID_TURN_TICK              ,21 ,eSnd_N331_Tick       		,CHIME_CYCLE_0  ,CHIME_SET_CHIME},
/*3*/   {CHIME_TURN_TOC                 ,CHIME_REQID_TURN_TOC               ,21 ,eSnd_N331_Tock       		,CHIME_CYCLE_0  ,CHIME_SET_CHIME},//continue to use
/*4*/   {CHIME_DOOR_OPEN                ,CHIME_REQID_DOOR_OPEN              ,9  ,eSnd_N331_PosLmp_1000ms    ,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*5*/   {CHIME_SEATBELT                 ,CHIME_REQID_SEATBELT               ,11 ,eSnd_N331_SeatBelt_1000ms  ,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*6*/   {CHIME_TPMS_WARN                ,CHIME_REQID_TPMS_WARN              ,10 ,eSnd_N331_TPMS_1000ms      ,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*7*/   {CHIME_PEPS_ESCL	            ,CHIME_REQID_PEPS_ESCL		        ,95	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*8*/   {CHIME_PEPS_SYS_FAIL            ,CHIME_REQID_PEPS_SYS_FAIL	        ,18	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*9*/   {CHIME_PEPS_REMAIN_NUM          ,CHIME_REQID_PEPS_REMAIN_NUM	    ,96	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*10*/  {CHIME_PEPS_DOOR_OPEN	        ,CHIME_REQID_PEPS_DOOR_OPEN	        ,97	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*11*/  {CHIME_PEPS_OPEN_NOT_OFF        ,CHIME_REQID_PEPS_OPEN_NOT_OFF      ,98	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*12*/  {CHIME_PEPS_IMMO		        ,CHIME_REQID_PEPS_IMMO		        ,14	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*13*/  {CHIME_PEPS_SSB			        ,CHIME_REQID_PEPS_SSB			    ,94 ,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*14*/  {CHIME_PEPS_NO_KEY_FOUND        ,CHIME_REQID_PEPS_NO_KEY_FOUND      ,15	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*15*/  {CHIME_PEPS_KEY_OUT		        ,CHIME_REQID_PEPS_KEY_OUT		    ,16	,eSnd_N331_PosLmp_1000ms    ,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*16*/  {CHIME_PEPS_KEY_BAT		        ,CHIME_REQID_PEPS_KEY_BAT		    ,17	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*17*/  {CHIME_PEPS_BRK_IND		        ,CHIME_REQID_PEPS_BRK_IND		    ,19	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*18*/ 	{CHIME_PEPS_CLUTCH_IND	        ,CHIME_REQID_PEPS_CLUTCH_IND	    ,99	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*19*/  {CHIME_PEPS_NOTPN		        ,CHIME_REQID_PEPS_NOTPN		        ,20	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*20*/  {CHIME_PSNG_SEATBELT	        ,CHIME_REQID_PSNG_SEATBELT	        ,11	,eSnd_N331_SeatBelt_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*21*/  {CHIME_PRK_BRK			        ,CHIME_REQID_PRK_BRK			    ,32	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*22*/  {CHIME_KEY_IN_ON		        ,CHIME_REQID_KEY_IN_ON		        ,31	,eSnd_N331_Radar_500ms		,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*23*/  {CHIME_BK_HOME			        ,CHIME_REQID_BK_HOME			    ,22	,eSnd_N331_ACC_2hz_250ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*24*/  {CHIME_POS_LAMP			        ,CHIME_REQID_POS_LAMP			    ,7	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*25*/  {CHIME_NOT_IN_P			        ,CHIME_REQID_NOT_IN_P			    ,8	,eSnd_N331_NotInP_2000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*26*/  {CHIME_SCR_1HZ                  ,CHIME_REQID_SCR_1HZ	            ,35	,eSnd_N331_Radar_500ms		,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*27*/  {CHIME_SCR_2HZ_1                ,CHIME_REQID_SCR_2HZ_1	            ,33	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*28*/	{CHIME_SCR_2HZ_2                ,CHIME_REQID_SCR_2HZ_2	            ,34	,eSnd_N331_Radar_250ms		,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*29*/  {CHIME_DPF_WARN 		        ,CHIME_REQID_DPF_WARN               ,36 ,eSnd_N331_ACC_250ms  		,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*30*/	{CHIME_LaneAssit_WarningAudio   ,CHIME_REQID_LaneAssit_WarningAudio ,4  ,eSnd_N331_BSD_250ms        ,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*31*/	{CHIME_Nav_Sts 	                ,CHIME_REQID_Nav_Sts                ,5  ,eSnd_N331_ACC_250ms   		,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*32*/	{CHIME_LaneAssit_HandsonReq     ,CHIME_REQID_LaneAssit_HandsonReq   ,3  ,eSnd_N331_Radar_500ms      ,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*33*/	{CHIME_TAKE_OVER_REQ 	        ,CHIME_REQID_TAKE_OVER_REQ          ,2  ,eSnd_N331_ACC_250ms   		,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*34*/	{CHIME_PCW_STATE 	            ,CHIME_REQID_PCW_STATE              ,1  ,eSnd_N331_ACC_2hz_250ms   	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*35*/	{CHIME_BSDSound_SwitchSts       ,CHIME_REQID_BSDSound_SwitchSts     ,5  ,eSnd_N331_Radar_250ms  	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*36*/	{CHIME_CTA_warningReq 	        ,CHIME_REQID_CTA_warningReq         ,5  ,eSnd_N331_Radar_250ms   	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*37*/	{CHIME_DOA_WarningReq 	        ,CHIME_REQID_DOA_WarningReq         ,5  ,eSnd_N331_Radar_250ms   	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*38*/	{CHIME_STT           	        ,CHIME_REQID_STT                    ,37 ,eSnd_N331_Radar_500ms   	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*39*/	{CHIME_LOWFUEL           	    ,CHIME_REQID_LOWFUEL                ,23 ,eSnd_N331_PosLmp_1000ms   	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*40*/	{CHIME_ATS           	        ,CHIME_REQID_ATS                    ,38 ,eSnd_N331_Radar_500ms   	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*41*/	{CHIME_ATS_DMC                  ,CHIME_REQID_ATS_DMC                ,38 ,eSnd_N331_Radar_500ms   	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*42*/	{CHIME_Over_Spd					,CHIME_REQID_Over_Spd				,13 ,eSnd_N331_SeatBelt_1000ms_05hz	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*43*/	{CHIME_Low_OilPress				,CHIME_REQID_Low_OilPress			,12 ,eSnd_N331_ACC_2hz_250ms  	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*44*/	{CHIME_UPA_1HZ				    ,CHIME_REQID_UPA_1HZ				,6  ,eSnd_N331_Chime_17_1hz     ,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*45*/	{CHIME_UPA_2HZ					,CHIME_REQID_UPA_2HZ				,6  ,eSnd_N331_Chime_17_2hz  	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*46*/	{CHIME_UPA_4HZ					,CHIME_REQID_UPA_4HZ				,6  ,eSnd_N331_Chime_17_4hz  	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*47*/	{CHIME_UPA_LONG					,CHIME_REQID_UPA_LONG				,6  ,eSnd_N331_Chime_17_long  	,CHIME_CYCLE_1  ,CHIME_SET_CHIME},
/*48*/  {CHIME_ENG_HIGH_TEMP			,CHIME_REQID_ENG_HIGH_TEMP			,14 ,eSnd_N331_PosLmp_1000ms  	,CHIME_CYCLE_1  ,CHIME_SET_CHIME}
};      
static const CHIME_DATA_t_ gs_tChime_D2Data_Tbl[CHIME_IGOFF_NUM] =              /* look-up table for chime config information */
{
	    /* Chime ID */                  /* Request Type */                 /* Priority *//* Sound Res */    /* Cycle Type *//* Request Input Type */
/*1*/	{CHIME_D2_TICK	                ,CHIME_REQID_D2_TICK	            ,21	,eSnd_N331_Tick	    		,CHIME_CYCLE_0	,CHIME_SET_CHIME},
/*2*/	{CHIME_D2_TOC	                ,CHIME_REQID_D2_TOC	                ,21	,eSnd_N331_Tock	    		,CHIME_CYCLE_0	,CHIME_SET_CHIME},
/*3*/	{CHIME_D2_PEPS_ESCL				,CHIME_REQID_D2_PEPS_ESCL			,95	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*4*/	{CHIME_D2_PEPS_SYS_FAIL			,CHIME_REQID_D2_PEPS_SYS_FAIL		,18	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*5*/	{CHIME_D2_PEPS_REMAIN_NUM		,CHIME_REQID_D2_PEPS_REMAIN_NUM		,96	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*6*/	{CHIME_D2_PEPS_DOOR_OPEN		,CHIME_REQID_D2_PEPS_DOOR_OPEN		,97	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*7*/	{CHIME_D2_PEPS_OPEN_NOT_OFF		,CHIME_REQID_D2_PEPS_OPEN_NOT_OFF	,98	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*8*/	{CHIME_D2_CHIME_PEPS_SSB		,CHIME_REQID_D2_PEPS_SSB			,94	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*9*/	{CHIME_D2_PEPS_NO_KEY_FOUND		,CHIME_REQID_D2_PEPS_NO_KEY_FOUND	,15	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*10*/	{CHIME_D2_PEPS_KEY_OUT			,CHIME_REQID_D2_PEPS_KEY_OUT		,16	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*11*/	{CHIME_D2_PEPS_BRK_IND			,CHIME_REQID_D2_PEPS_BRK_IND		,19	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*12*/	{CHIME_D2_PEPS_CLUTCH_IND		,CHIME_REQID_D2_PEPS_CLUTCH_IND		,99	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*13*/	{CHIME_D2_PEPS_NOTPN		    ,CHIME_REQID_D2_PEPS_NOTPN		    ,20	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*14*/	{CHIME_D2_KEY_IN_ON				,CHIME_REQID_D2_KEY_IN_ON			,31	,eSnd_N331_Radar_500ms		,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*15*/	{CHIME_D2_BK_HOME				,CHIME_REQID_D2_BK_HOME				,100,eSnd_N331_BSD_250ms	    ,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*16*/	{CHIME_D2_POS_LAMP				,CHIME_REQID_D2_POS_LAMP			,7	,eSnd_N331_PosLmp_1000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*17*/	{CHIME_D2_NOT_IN_P				,CHIME_REQID_D2_NOT_IN_P			,8	,eSnd_N331_NotInP_2000ms	,CHIME_CYCLE_1	,CHIME_SET_CHIME},
/*18*/	{CHIME_D2_LIGHT_ON				,CHIME_REQID_D2_LIGHT_ON			,5	,eSnd_N331_BSD_250ms		,CHIME_CYCLE_1	,CHIME_SET_CHIME}
};


static const UINT8 gs_u8Chime_NotPopUp_Tbl[CHIME_REQ_NOT_POPUP_NUM] =          /* chime request which not related to pop up */
{
	CHIME_NULL              	,
	CHIME_TURN_TICK				,
	CHIME_TURN_TOC				,
	CHIME_DOOR_OPEN				,
	CHIME_SEATBELT				,
	CHIME_TPMS_WARN				,
	CHIME_PEPS_ESCL				,
	CHIME_PEPS_SYS_FAIL			,
	CHIME_PEPS_REMAIN_NUM		,
	CHIME_PEPS_DOOR_OPEN		,
	CHIME_PEPS_OPEN_NOT_OFF		,
	CHIME_PEPS_IMMO				,
	CHIME_PEPS_SSB				,
	CHIME_PEPS_NO_KEY_FOUND		,
	CHIME_PEPS_KEY_OUT			,
	CHIME_PEPS_KEY_BAT			,
	CHIME_PEPS_BRK_IND			,
	CHIME_PEPS_CLUTCH_IND		,
	CHIME_PEPS_NOTPN        	,
	CHIME_PSNG_SEATBELT     	,
	CHIME_PRK_BRK	        	,
	CHIME_KEY_IN_ON	        	,
	CHIME_BK_HOME	        	,
	CHIME_POS_LAMP	        	,
	CHIME_NOT_IN_P	        	,
    CHIME_SCR_1HZ           	,
    CHIME_SCR_2HZ_1         	,
    CHIME_SCR_2HZ_2         	,
	CHIME_DPF_WARN				,
    CHIME_LaneAssit_WarningAudio,
    CHIME_Nav_Sts				,
    CHIME_LaneAssit_HandsonReq	,
    CHIME_TAKE_OVER_REQ 		,
    CHIME_PCW_STATE				,
    CHIME_BSDSound_SwitchSts	,
    CHIME_CTA_warningReq		,
    CHIME_DOA_WarningReq        ,
    CHIME_STT					,
    CHIME_LOWFUEL				,
    CHIME_ATS					,
    CHIME_ATS_DMC				,
    CHIME_Over_Spd				,
    CHIME_Low_OilPress			,
	CHIME_UPA_1HZ               ,
	CHIME_UPA_2HZ				,
	CHIME_UPA_4HZ				,
	CHIME_UPA_LONG				,
	CHIME_ENG_HIGH_TEMP         ,
	CHIME_D2_TICK				,
	CHIME_D2_TOC				,
	CHIME_D2_PEPS_ESCL			,
	CHIME_D2_PEPS_SYS_FAIL		,
	CHIME_D2_PEPS_REMAIN_NUM	,
	CHIME_D2_PEPS_DOOR_OPEN		,
	CHIME_D2_PEPS_OPEN_NOT_OFF	,
	CHIME_D2_CHIME_PEPS_SSB		,
	CHIME_D2_PEPS_NO_KEY_FOUND	,
	CHIME_D2_PEPS_KEY_OUT		,
	CHIME_D2_PEPS_BRK_IND		,
	CHIME_D2_PEPS_CLUTCH_IND	,
    CHIME_D2_PEPS_NOTPN			,
	CHIME_D2_KEY_IN_ON			,
	CHIME_D2_BK_HOME			,
	CHIME_D2_POS_LAMP			,
	CHIME_D2_NOT_IN_P			,
	CHIME_D2_LIGHT_ON
};

static const CHIME_REQ_DATA_t_ gs_tChime_ReqType[CHIME_REQ_TOTAL_NUM] =
{
    {CHIME_REQID_NULL                		,CHIME_REQ_RESP_RECOVER    ,CHIME_DONT_REPEAT},
    {CHIME_REQID_TURN_TICK           		,CHIME_REQ_RESP_ONCE       ,CHIME_DONT_REPEAT},
    {CHIME_REQID_TURN_TOC            		,CHIME_REQ_RESP_ONCE       ,CHIME_DONT_REPEAT},
    {CHIME_REQID_DOOR_OPEN           		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_SEATBELT            		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_TPMS_WARN           		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_ESCL			 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_SYS_FAIL		 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_REMAIN_NUM	 		,CHIME_REQ_RESP_RECOVER    ,CHIME_DONT_REPEAT},
	{CHIME_REQID_PEPS_DOOR_OPEN		 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_OPEN_NOT_OFF	 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_IMMO			 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_SSB			 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_NO_KEY_FOUND	 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_KEY_OUT		 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_KEY_BAT		 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_BRK_IND		 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_CLUTCH_IND	 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PEPS_NOTPN			 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_PSNG_SEATBELT		 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_PRK_BRK			 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_KEY_IN_ON			 		,CHIME_REQ_RESP_RECOVER    ,CHIME_DONT_REPEAT},
	{CHIME_REQID_BK_HOME		     		,CHIME_REQ_RESP_RECOVER    ,CHIME_DONT_REPEAT},
	{CHIME_REQID_POS_LAMP			 		,CHIME_REQ_RESP_RECOVER    ,CHIME_DONT_REPEAT},
    {CHIME_REQID_NOT_IN_P			 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_SCR_1HZ             		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_SCR_2HZ_1          	 	,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_SCR_2HZ_2           		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_DPF_WARN            		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_LaneAssit_WarningAudio     ,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_Nav_Sts             		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_LaneAssit_HandsonReq       ,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_TAKE_OVER_REQ       		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_PCW_STATE           		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_BSDSound_SwitchSts  		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_CTA_warningReq      		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_DOA_WarningReq      		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_STT                 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_LOWFUEL             		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_ATS                 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_ATS_DMC             		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_Over_Spd			 		,CHIME_REQ_RESP_RECOVER	   ,CHIME_MUST_REPEAT},
	{CHIME_REQID_Low_OilPress            	,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_UPA_1HZ            		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_UPA_2HZ            		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_UPA_4HZ            		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_UPA_LONG            		,CHIME_REQ_RESP_RECOVER    ,CHIME_DONT_REPEAT},
	{CHIME_REQID_ENG_HIGH_TEMP              ,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_D2_TICK             		,CHIME_REQ_RESP_ONCE       ,CHIME_DONT_REPEAT},
    {CHIME_REQID_D2_TOC 	         		,CHIME_REQ_RESP_ONCE       ,CHIME_DONT_REPEAT},
	{CHIME_REQID_D2_PEPS_ESCL		 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_PEPS_SYS_FAIL	 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_PEPS_REMAIN_NUM	 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_PEPS_DOOR_OPEN	 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_PEPS_OPEN_NOT_OFF 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_PEPS_SSB		 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_PEPS_NO_KEY_FOUND 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_PEPS_KEY_OUT	 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_PEPS_BRK_IND	 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_PEPS_CLUTCH_IND	 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_D2_PEPS_NOTPN       		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_KEY_IN_ON		 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_BK_HOME			 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_POS_LAMP		 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
	{CHIME_REQID_D2_NOT_IN_P		 		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_D2_LIGHT_ON         		,CHIME_REQ_RESP_RECOVER    ,CHIME_MUST_REPEAT},
    {CHIME_REQID_POPUP1              		,CHIME_REQ_RESP_RECOVER    ,CHIME_DONT_REPEAT},
    {CHIME_REQID_POPUP2              		,CHIME_REQ_RESP_RECOVER    ,CHIME_DONT_REPEAT}
};




static void Chime_Mgr_VarInit__(void);
static void Chime_Mgr_StopAll__(void);
static void Chime_Mgr_RunRoutine__(void);
static UINT8 Chime_Mgr_CheckTurnLed__(UINT8 u8NotPopUp_Flag, UINT8 u8NotPopUp_ChimeSort);




/* ====================================================================================================================
**	Function Name	 :	  Chime_Mgr_VarInit__()
**	Process Timing	 :	  -
**	Arguments[Input] :	  -
**	Arguments[Output]:	  -
**	Return Value	 :	  -
==================================================================================================================== */
static void Chime_Mgr_VarInit__(void)
{

    gs_u8CurChimeSort   	 = (UINT8)CHIME_SORT_NULL;
    gs_eChimeState 			 = CHIME_STATE_INIT;
    gs_u8ChimeDrvSts 		 = (UINT8)CHIME_CLEAR;
    gs_u8CurChimeResID 		 = (UINT8)CHIME_NULL;
    gs_u8ChimeFuncState 	 = (UINT8)TRUE;
    gs_u8ChimeFuncState_Last = (UINT8)TRUE;

    gs_u8Chime_ATS_DMC_count = 0;
    gs_u8Chime_ATS_count = 0;
	gs_u8Chime_LowFule_count = 0;
    gs_u8Chime_STT_count = 0;
	gs_u8Chime_TPMS_Count = 0;
	gs_u16Chime_SeatBelt_Count = 0;
	gs_u8Chime_DoorOpenCount = 0;
	//gs_u8Chime_Over_Spd_Count = 0;
	gs_u8Chime_Low_OilPress_Count = 0;
	gs_u8Chime_SCR_1HZCount = 0;
	gs_u8Chime_SCR_2HZ_1Count = 0;
	gs_u8Chime_SCR_2HZ_2Count = 0;
	gs_u8Chime_DPF_Count = 0;
	gs_u8Chime_ADAS_WarningAudio_Count = 0;
	gs_u8Chime_ADAS_NavSts_Count = 0;		
	gs_u8Chime_ADAS_HandsonReq_Count = 0;		
	gs_u8Chime_ADAS_TAKE_OVER_Count = 0;		
	gs_u8Chime_ADAS_PCW_STATE_Count = 0;		
	//gs_u8Chime_ADAS_BSDSound_Count = 0;	
	//gs_u8Chime_ADAS_CTA_Count = 0;
	//gs_u8Chime_ADAS_DOA_Count = 0;
	gs_u8Chime_D2_PEPS_REMAIN_NUM_Count = 0;
	gs_u8Chime_PEPS_IMMO_Count = 0;
	gs_u8Chime_PEPS_KEY_BAT_Count = 0;
	gs_u16Chime_PsSeatBelt_Count = 0;
    gs_u8Chime_PEPS_SSB_count = 0;
    gs_u8Chime_PEPS_ESCL_Count = 0;
    gs_u8Chime_D2_PEPS_ESCL_Count = 0;
    gs_u8Chime_D2_PEPS_SSB_count = 0;
    gs_u8Chime_D2_POSLMP_Count = 0;
    gs_u8Chime_D2_BKHOME_Count = 0;
	gs_u16Chime_D2_KEY_IN_ON_Count = 0;
	gs_u16Chime_NOT_IN_P_Count = 0;
	gs_u16Chime_D2_NOT_IN_P_Count = 0;
	gs_u8Chime_Eng_High_Temp_Count = 0;
    gs_u8Chime_D2_PEPS_SYS_count = 0;
    gs_u8Chime_D2_PEPS_DOOR_Count = 0;
    gs_u8Chime_D2_PEPS_NOTOFF_Count = 0;
    gs_u8Chime_D2_PEPS_NOKEY_Count = 0;
    gs_u8Chime_D2_PEPS_KEYOUT_Count = 0;
    gs_u8Chime_D2_PEPS_BRK_Count = 0;
    gs_u8Chime_D2_PEPS_CLUTCH_Count = 0;
    gs_u8Chime_D2_PEPS_NOTPN_Count = 0;
    gs_u8Chime_PEPS_SYS_count = 0;
    gs_u8Chime_PEPS_DOOR_Count = 0;
    gs_u8Chime_PEPS_NOTOFF_Count = 0;
    gs_u8Chime_PEPS_NOKEY_Count = 0;
    gs_u8Chime_PEPS_KEYOUT_Count = 0;
    gs_u8Chime_PEPS_BRK_Count = 0;
    gs_u8Chime_PEPS_CLUTCH_Count = 0;
    gs_u8Chime_PEPS_NOTPN_Count = 0;

    gs_eChime_KeySts      = eIGN_OFF;
    gs_eChime_KeySts_Last = eIGN_OFF;

    gs_u8ChimeWarnSts     = (UINT8)CHIME_BUSY;
    gs_eChimeProcSts      = CHIME_PROC_PREREQ;

    gs_u8Chime_CurMode    = (UINT8)CHIME_MODE_NORMAL;
    gs_u8ChimeDrvSts_Last = 0;

    gs_u8Chime_Respond    = (UINT8)CHIME_RESP_NONE;
	Chime_Mgr_ClearInputSts__();
	Chime_Mgr_ClearInputLast__();
    Chime_Mgr_ClearInputReq__();
    Chime_Mgr_ClearTOutSts__();

	if(gs_u8ChimeWarnSts == CHIME_BUSY)
	{
	}

#if (cUSE_CHM_MANAGER_PROVEOUT != 0)
	//u16ChmProveoutTimer =0;
	ChmIsProveoutDone = FALSE;
#endif
}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_StopAll__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_StopAll__(void)
{
    UINT8 u8ChimeIndex = (UINT8)0;

    /* stop all chime ID */
    for (u8ChimeIndex = (UINT8)0; u8ChimeIndex < (UINT8)eSnd_MaxNumOfChimes; u8ChimeIndex++)
    {
        (void)Rte_Call_tiCS_TISndCdd_RequestToStop(u8ChimeIndex, (UINT8)CHIME_STOP_SU);
    }

    /* multi-cycle sound witch can recover form voltage abnormal */
    if ((eIGN_RUN == gs_eChime_KeySts)
        && (gs_u8CurChimeSort <= (UINT8)CHIME_IGON_MAXID))
        //&& (CHIME_CYCLE_0 == gs_tChime_D1Data_Tbl[gs_u8CurChimeSort].u8ChimeCycleType))
    {
        gs_u8CurChimeSort = (UINT8)CHIME_SORT_NULL;
    }
    else if ((eIGN_OFF == gs_eChime_KeySts)
        && (gs_u8CurChimeSort <= (UINT8)CHIME_IGOFF_MAXID))
        //&& (CHIME_CYCLE_0 == gs_tChime_D2Data_Tbl[gs_u8CurChimeSort].u8ChimeCycleType))
    {
        gs_u8CurChimeSort = (UINT8)CHIME_SORT_NULL;
    }
    else
    {
        ;
    }
}


/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_InitHW()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
void  Chime_Mgr_InitHW(void)
{
    gs_eChimeState = CHIME_STATE_IDLE;
}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_InitSW()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
void  Chime_Mgr_InitSW(void)
{
    Chime_Mgr_VarInit__();
}


/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_ExitHW()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
void  Chime_Mgr_ExitHW(void)
{
    gs_eChimeState = CHIME_STATE_IDLE;
}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_ExitSW()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
void Chime_Mgr_ExitSW(void)
{

}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_Notify_Shutdown()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
void Chime_Mgr_Notify_Shutdown(void)
{
    Chime_Mgr_StopAll__();
    Chime_Mgr_VarInit__();
    gs_eChimeState = CHIME_STATE_STOP;
}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_Notify_Restore()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
void Chime_Mgr_Notify_Restore(void)
{
    Chime_Mgr_VarInit__();
    gs_eChimeState = CHIME_STATE_INIT;
}

/* ====================================================================================================================
**  Function Name    :    OnTask10ms_Chime_Routine()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
void OnTask10ms_Chime_Routine(void)
{
    switch (gs_eChimeState)
    {
        case CHIME_STATE_IDLE:
            break;

        case CHIME_STATE_INIT:
        case CHIME_STATE_IGON:
        case CHIME_STATE_IGOFF:
        case CHIME_STATE_DIAG:
            Chime_Mgr_RunRoutine__();
            break;

        case CHIME_STATE_STOP:
            break;

        default:
            break;
    }
}

boolean Chime_Mgr_GetTimeout(UINT8 index)
{
    return gs_u8Chime_Warn_ToutSts[index];
}
/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_RunRoutine__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_RunRoutine__(void)
{
    boolean bConfig_Check = FALSE;
    boolean bLVSD_Check = FALSE;
    boolean bHVSD_Check = FALSE;
	// UINT8 u8ChimeId=0;

    Chime_Mgr_GetChimeInput__();                                               /* get input signal, not include input from warning */
    //Chime_Mgr_ChimeLogicProc__();                                              /* process input signal */

    bConfig_Check = Chime_Mgr_CheckConfig__();                                 /* get check flag for chime function */
    bLVSD_Check = Chime_Mgr_CheckLVSD__();
    bHVSD_Check = Chime_Mgr_CheckHVSD__();
    //Chime_Mgr_CheckMode__();/*Always set Normal currently20180522*/

    if ((FALSE != bConfig_Check) && (FALSE != bLVSD_Check) && (FALSE != bHVSD_Check)) /* check if chime function enabled */
    {
        gs_u8ChimeFuncState = (UINT8)TRUE;                                     /* chime enabled */

        switch (gs_u8Chime_CurMode)                                            /* check current mode */
        {
            case CHIME_MODE_ETM:                                               /* ETM mode */
                //Chime_Mgr_EtmRoutine__();
                break;

            case CHIME_MODE_IOC:                                               /* IODID mode */
                //Chime_Mgr_DiagRoutine__();
                break;

            case CHIME_MODE_EOL:                                               /* EOL mode */
                //Chime_Mgr_EolRoutine__();
                break;

            case CHIME_MODE_NORMAL:                                            /* normal process mode */
            default:
                Chime_Mgr_ProcRoutine__();
                break;
        }
    }
    else
    {
        gs_u8ChimeFuncState = (UINT8)FALSE;                                    /* chime disabled */

        if (((UINT8)TRUE == gs_u8ChimeFuncState_Last)                          /* enable -> disable, stop sound, clear input status */
            && ((UINT8)FALSE == gs_u8ChimeFuncState))
        {
            Chime_Mgr_Func_Shutdown__();
        }
    }

    gs_u8ChimeFuncState_Last = gs_u8ChimeFuncState;                            /* keep last chime function state */
    Chime_Mgr_SetOutput__();                                                   /* output process */
	
	/* Timeout State Refreshing process */
    Chime_Mgr_TOutRefresh__();
}


void CChimeMdl_UpdateStatus_Operation(UINT8 index, UINT8 warning_status)
{
    if(index < CHIME_REQ_TOTAL_NUM)
    {
		gs_u8Chime_WarnInput[index] = warning_status;
    }
}

BOOL CChimeMdl_Ignoff_Chime_State__(void)
{
    eIgnState fl_IGN_state;
    BOOL Ignoff_chime_state = FALSE;
    Rte_Read_rpIgnState_IGNState(&fl_IGN_state);
    if(fl_IGN_state == eIGN_OFF)
    {
        if(((gs_u8Chime_WarnInput[CHIME_REQID_D2_PEPS_SSB] == TRUE) && (gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_SSB] == (UINT8)FALSE))
        || ((gs_u8Chime_WarnInput[CHIME_REQID_D2_PEPS_NO_KEY_FOUND] == TRUE) && (gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_NO_KEY_FOUND] == (UINT8)FALSE))
        || ((gs_u8Chime_WarnInput[CHIME_REQID_D2_PEPS_KEY_OUT] == TRUE) && (gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_KEY_OUT] == (UINT8)FALSE))
        || ((gs_u8Chime_WarnInput[CHIME_REQID_D2_PEPS_BRK_IND] == TRUE) && (gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_BRK_IND] == (UINT8)FALSE))
        || ((gs_u8Chime_WarnInput[CHIME_REQID_D2_PEPS_CLUTCH_IND] == TRUE) && (gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_CLUTCH_IND] == (UINT8)FALSE))
        || ((gs_u8Chime_WarnInput[CHIME_REQID_D2_BK_HOME] == TRUE) && (gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_BK_HOME] == (UINT8)FALSE))
        || ((gs_u8Chime_WarnInput[CHIME_REQID_D2_POS_LAMP] == TRUE) && (gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_POS_LAMP] == (UINT8)FALSE))
        || ((gs_u8Chime_WarnInput[CHIME_REQID_D2_NOT_IN_P] == TRUE) && (gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_NOT_IN_P] == (UINT8)FALSE)))
        {
            
            Ignoff_chime_state = TRUE;
        }
        else
        {
            Ignoff_chime_state = FALSE;
        }
    }
    return Ignoff_chime_state;
}

void CChimeMdl_ClearLast_Warning(UINT8 index)
{
	gs_u8Chime_WarnInput[index] = FALSE;
    gs_u8Chime_WarnInput_Last[index] = FALSE;
    gs_u8Chime_WarnInput_Req[index] = FALSE;
	gs_u8Chime_Warn_ToutSts[index] = FALSE;
    switch(index)
    {
    case CHIME_REQID_ATS:
		gs_u8Chime_ATS_count = 0;
		break;
    case CHIME_REQID_ATS_DMC:
        gs_u8Chime_ATS_DMC_count = 0;
        break;
    case CHIME_REQID_LOWFUEL:
		gs_u8Chime_LowFule_count = 0;
		break;
    case CHIME_REQID_STT:
        gs_u8Chime_STT_count = 0;
        break;
    case CHIME_REQID_DOOR_OPEN:
        gs_u8Chime_DoorOpenCount = 0;
        break;
	case CHIME_REQID_Over_Spd:
        //gs_u8Chime_Over_Spd_Count = 0;
        break;
	case CHIME_REQID_Low_OilPress:
        gs_u8Chime_Low_OilPress_Count = 0;
        break;
	case CHIME_REQID_SCR_1HZ:
		gs_u8Chime_SCR_1HZCount = 0;
		break;
	case CHIME_REQID_SCR_2HZ_1:
		gs_u8Chime_SCR_2HZ_1Count = 0;
		break;	
	case CHIME_REQID_SCR_2HZ_2:
		gs_u8Chime_SCR_2HZ_2Count = 0;
		break;	
	case CHIME_REQID_DPF_WARN:
		gs_u8Chime_DPF_Count=0;		
		break;	
	case CHIME_REQID_LaneAssit_WarningAudio:
		gs_u8Chime_ADAS_WarningAudio_Count = 0;
		break;
	case CHIME_REQID_BSDSound_SwitchSts:
		//gs_u8Chime_ADAS_BSDSound_Count = 0;
		break;
	case CHIME_REQID_CTA_warningReq:
		//gs_u8Chime_ADAS_CTA_Count = 0;
		break;
	case CHIME_REQID_DOA_WarningReq:
		//gs_u8Chime_ADAS_DOA_Count = 0;
		break;
	case CHIME_REQID_Nav_Sts:
		gs_u8Chime_ADAS_NavSts_Count= 0;
		break;
	case CHIME_REQID_LaneAssit_HandsonReq:
		gs_u8Chime_ADAS_HandsonReq_Count= 0;
		break;
	case CHIME_REQID_TAKE_OVER_REQ:
		gs_u8Chime_ADAS_TAKE_OVER_Count= 0;
		break;
	case CHIME_REQID_PCW_STATE:
		gs_u8Chime_ADAS_PCW_STATE_Count= 0;
		break;		
	case CHIME_REQID_TPMS_WARN:
        gs_u8Chime_TPMS_Count=0;
        break;		
	case CHIME_REQID_D2_PEPS_REMAIN_NUM:
        gs_u8Chime_D2_PEPS_REMAIN_NUM_Count=0;
        break;
	case CHIME_REQID_PEPS_IMMO:
		gs_u8Chime_PEPS_IMMO_Count=0;
		break;
	case CHIME_REQID_PEPS_KEY_BAT:
		gs_u8Chime_PEPS_KEY_BAT_Count=0;
		break;
	case CHIME_REQID_SEATBELT:
		gs_u16Chime_SeatBelt_Count=0;
		break;
	case CHIME_REQID_PSNG_SEATBELT:
		gs_u16Chime_PsSeatBelt_Count=0;
		break;
	case CHIME_REQID_D2_POS_LAMP:
		gs_u8Chime_D2_POSLMP_Count = 0;
		break;
	case CHIME_REQID_D2_BK_HOME:
		gs_u8Chime_D2_BKHOME_Count = 0;
		break;
	case CHIME_REQID_D2_KEY_IN_ON:
		gs_u16Chime_D2_KEY_IN_ON_Count=0;
		break;		
    case CHIME_REQID_NOT_IN_P:
		gs_u16Chime_NOT_IN_P_Count=0;
		break;	
	case CHIME_REQID_D2_NOT_IN_P:
		gs_u16Chime_D2_NOT_IN_P_Count = 0;
		break;
    case CHIME_REQID_ENG_HIGH_TEMP:
        gs_u8Chime_Eng_High_Temp_Count = 0;
        break;
	case CHIME_REQID_PEPS_SSB:
        gs_u8Chime_PEPS_SSB_count = 0;
        break;
    case CHIME_REQID_D2_PEPS_SSB:
        gs_u8Chime_D2_PEPS_SSB_count = 0;
        break;
    case CHIME_REQID_PEPS_ESCL:
        gs_u8Chime_PEPS_ESCL_Count = 0;
        break;
    case CHIME_REQID_D2_PEPS_ESCL:
        gs_u8Chime_D2_PEPS_ESCL_Count = 0;
        break;
    //st
    case CHIME_REQID_PEPS_SYS_FAIL:
        gs_u8Chime_PEPS_SYS_count=0;
        break;
    case CHIME_REQID_D2_PEPS_SYS_FAIL:
        gs_u8Chime_D2_PEPS_SYS_count=0;
        break;
    case CHIME_REQID_PEPS_DOOR_OPEN:
        gs_u8Chime_PEPS_DOOR_Count = 0;
        break;
    case CHIME_REQID_D2_PEPS_DOOR_OPEN:
        gs_u8Chime_D2_PEPS_DOOR_Count = 0;
        break;
    case CHIME_REQID_PEPS_OPEN_NOT_OFF:
        gs_u8Chime_PEPS_NOTOFF_Count = 0;
        break;
    case CHIME_REQID_D2_PEPS_OPEN_NOT_OFF:
        gs_u8Chime_D2_PEPS_NOTOFF_Count = 0;
        break;
    case CHIME_REQID_PEPS_NO_KEY_FOUND:
        gs_u8Chime_PEPS_NOKEY_Count = 0;
        break;
    case CHIME_REQID_D2_PEPS_NO_KEY_FOUND:
        gs_u8Chime_D2_PEPS_NOKEY_Count = 0;
        break;
    case CHIME_REQID_PEPS_KEY_OUT:
        gs_u8Chime_PEPS_KEYOUT_Count = 0;
        break;
    case CHIME_REQID_D2_PEPS_KEY_OUT:
        gs_u8Chime_D2_PEPS_KEYOUT_Count = 0;
        break;
    case CHIME_REQID_PEPS_BRK_IND:
        gs_u8Chime_PEPS_BRK_Count = 0;
        break;
    case CHIME_REQID_D2_PEPS_BRK_IND:
        gs_u8Chime_D2_PEPS_BRK_Count = 0;
        break;
    case CHIME_REQID_PEPS_CLUTCH_IND:
        gs_u8Chime_PEPS_CLUTCH_Count = 0;
        break;
    case CHIME_REQID_D2_PEPS_CLUTCH_IND:
        gs_u8Chime_D2_PEPS_CLUTCH_Count = 0;
        break;
	case CHIME_REQID_D2_LIGHT_ON:
		gs_u8Chime_D2_LIGHT_ON_Count = 0;
		break;	
    case CHIME_REQID_PEPS_NOTPN:
        gs_u8Chime_PEPS_NOTPN_Count = 0;
        break;
    case CHIME_REQID_D2_PEPS_NOTPN:
        gs_u8Chime_D2_PEPS_NOTPN_Count = 0;
        break;
    //end
    }
}


/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_CheckLVSD__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static boolean Chime_Mgr_CheckLVSD__(void)
{
    boolean bCheckFlag = TRUE;
    UINT8 u8BatSts = (UINT8)eBatteryState_Normal;

    (void)Rte_Read_rpBattState_BatteryState(&u8BatSts);

    if ((UINT8)eBatteryState_UnderVoltage == u8BatSts)
    {
        bCheckFlag = FALSE;
    }

    return (bCheckFlag);
}


/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_CheckHVSD__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static boolean Chime_Mgr_CheckHVSD__(void)
{
	boolean bCheckFlag = TRUE;
	UINT8 u8BatSts = (UINT8)eBatteryState_Normal;

	(void)Rte_Read_rpBattState_BatteryState(&u8BatSts);

	if ((UINT8)eBatteryState_OverVoltage == u8BatSts)
	{
		bCheckFlag = FALSE;
	}

	return (bCheckFlag);
}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_CheckConfig__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static boolean Chime_Mgr_CheckConfig__(void)
{
    boolean bCheckFlag = TRUE;

    return (bCheckFlag);
}

static void Chime_Mgr_D2MaxCntRefresh(void)
{
	// if((gs_u8Chime_WarnInput[CHIME_REQID_NOT_IN_P] == TRUE) 
    // &&(gs_u8Chime_Warn_ToutSts[CHIME_REQID_NOT_IN_P] == (UINT8)FALSE) )
    // {
    //     gs_u8NOT_IN_P_D2_CntMax = (5-gs_u16Chime_NOT_IN_P_Count);
    // }
	
    if((gs_u8Chime_WarnInput[CHIME_REQID_PEPS_SSB] == TRUE) 
    &&(gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_SSB] == (UINT8)FALSE) )
    {
        gs_u8SSB_D2_CntMax = (gs_u8SSB_D1_CntMax-gs_u8Chime_PEPS_SSB_count);
    }

    if((gs_u8Chime_WarnInput[CHIME_REQID_PEPS_ESCL] == TRUE) 
    &&(gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_ESCL] == (UINT8)FALSE) )
    {
        gs_u8ESCL_D2_CntMax = (gs_u8ESCL_D1_CntMax-gs_u8Chime_PEPS_ESCL_Count);
    }

    //st
    if((gs_u8Chime_WarnInput[CHIME_REQID_PEPS_SYS_FAIL] == TRUE) 
    &&(gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_SYS_FAIL] == (UINT8)FALSE) )
    {
        gs_u8SYS_D2_CntMax = (gs_u8SYS_D1_CntMax-gs_u8Chime_PEPS_SYS_count);
    }
    if((gs_u8Chime_WarnInput[CHIME_REQID_PEPS_DOOR_OPEN] == TRUE) 
    &&(gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_DOOR_OPEN] == (UINT8)FALSE) )
    {
        gs_u8DOOR_D2_CntMax = (gs_u8DOOR_D1_CntMax-gs_u8Chime_PEPS_DOOR_Count);
    }
    if((gs_u8Chime_WarnInput[CHIME_REQID_PEPS_OPEN_NOT_OFF] == TRUE) 
    &&(gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_OPEN_NOT_OFF] == (UINT8)FALSE) )
    {
        gs_u8NOTOFF_D2_CntMax = (gs_u8NOTOFF_D1_CntMax-gs_u8Chime_PEPS_NOTOFF_Count);
    }
    if((gs_u8Chime_WarnInput[CHIME_REQID_PEPS_NO_KEY_FOUND] == TRUE) 
    &&(gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_NO_KEY_FOUND] == (UINT8)FALSE) )
    {
        gs_u8NOKEY_D2_CntMax = (gs_u8NOKEY_D1_CntMax-gs_u8Chime_PEPS_NOKEY_Count);
    }
    if((gs_u8Chime_WarnInput[CHIME_REQID_PEPS_KEY_OUT] == TRUE) 
    &&(gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_KEY_OUT] == (UINT8)FALSE) )
    {
        gs_u8KEYOUT_D2_CntMax = (gs_u8KEYOUT_D1_CntMax-gs_u8Chime_PEPS_KEYOUT_Count);
    }
    if((gs_u8Chime_WarnInput[CHIME_REQID_PEPS_BRK_IND] == TRUE) 
    &&(gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_BRK_IND] == (UINT8)FALSE) )
    {
        gs_u8BRK_D2_CntMax = (gs_u8BRK_D1_CntMax-gs_u8Chime_PEPS_BRK_Count);
    }
    if((gs_u8Chime_WarnInput[CHIME_REQID_PEPS_CLUTCH_IND] == TRUE) 
    &&(gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_CLUTCH_IND] == (UINT8)FALSE) )
    {
        gs_u8CLUTCH_D2_CntMax = (gs_u8CLUTCH_D1_CntMax-gs_u8Chime_PEPS_CLUTCH_Count);
    }
    if((gs_u8Chime_WarnInput[CHIME_REQID_PEPS_NOTPN] == TRUE) 
    &&(gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_NOTPN] == (UINT8)FALSE) )
    {
        gs_u8NOTPN_D2_CntMax = (gs_u8NOTPN_D1_CntMax-gs_u8Chime_PEPS_NOTPN_Count);
    }
    //end
   
}
/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_ProcRoutine__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_ProcRoutine__(void)
{
    if (gs_eChime_KeySts_Last != gs_eChime_KeySts)                             /* check if power mode switch */
    {
        if(gs_eChime_KeySts_Last == eIGN_RUN)//IG ON-OFF continue count Yxiong2 added@18 1224
        {
            Chime_Mgr_D2MaxCntRefresh();
        }
        else
        {   
                (void)Rte_Call_tiCS_TISndCdd_RequestToStop(gs_u8CurChimeResID, (UINT8)CHIME_STOP_IMMI);  /* stop current sound */
                gs_u8CurChimeSort = (UINT8)CHIME_SORT_NULL;
                gs_eChimeProcSts = CHIME_PROC_DRVSTS;                                  /* change state to read sound driver status */
        #if 1
                Chime_Mgr_ClearInputSts__();                                           /* clear input status */
                Chime_Mgr_ClearInputLast__();
                Chime_Mgr_ClearInputReq__();
                Chime_Mgr_ClearCounter__();
                Chime_Mgr_ClearTOutSts__();
                //Chime_Mgr_ClearTempSts__();
        #endif	
        }
    }
    else                                                                       /* no power mode switch, normal process */
    {
        Chime_Mgr_Arbitrate__();
    }
		
#if (cUSE_CHM_MANAGER_PROVEOUT != 0)
		uint8 Timer_status = 0;
		
		/*Previous state will be other than RUN state*/
		if((eIGN_RUN == gs_eChime_KeySts) &&
		   (eIGN_RUN != gs_eChime_KeySts_Last))
		{
			/* Proveout Timer is Started  */
			Rte_Call_rptimer_TmExt_Timer_Start(eChimeProveOutTimer);
			//u16ChmProveoutTimer = cCHM_MANAGER_PROVEOUT_TIME_IN_MS+1;
		}
	
		/*Stop the timer and Clear the ProveOut status if the current mode is not Igniiton RUN*/
		if(eIGN_RUN != gs_eChime_KeySts)
		{
			Rte_Call_rptimer_TmExt_Timer_Stop(eChimeProveOutTimer);
			//u16ChmProveoutTimer = 0;
			ChmIsProveoutDone = FALSE;
		}
		
		Rte_Call_rptimer_TmExt_Timer_IsElapsed(eChimeProveOutTimer,cCHM_MANAGER_PROVEOUT_TIME_IN_MS,&Timer_status);
		if((FALSE == ChmIsProveoutDone)&&
			(((uint8)1u) == Timer_status))/*(FALSE != (TmExt_Timer_IsElapsed(eChimeProveOutTimer, 
	cTT_MANAGER_PROVEOUT_TIME_IN_MS)))*/
		{
			/* Proveout Timer is Expired */
			ChmIsProveoutDone = TRUE;
			Rte_Call_rptimer_TmExt_Timer_Stop(eChimeProveOutTimer);
			//u16ChmProveoutTimer = 0;
		}
		// else if((FALSE == ChmIsProveoutDone)&&
			// /*(FALSE != (TmExt_Timer_IsElapsed(eChimeProveOutTimer, cTT_MANAGER_PROVEOUT_TIME_IN_MS)))*/
			// (u16ChmProveoutTimer > 1)
		// )
		// {
			// //u16ChmProveoutTimer--;
		// }
	
#endif 


    gs_eChime_KeySts_Last = gs_eChime_KeySts;                                  /* keep last power mode status */
}

static void Chime_Mgr_ClearCounter__(void)
{
    gs_u8Chime_ATS_DMC_count = 0;
    gs_u8Chime_ATS_count = 0;
	gs_u8Chime_LowFule_count = 0;
    gs_u8Chime_STT_count = 0;
    gs_u8Chime_DoorOpenCount = 0;
	//gs_u8Chime_Over_Spd_Count = 0;
	gs_u8Chime_Low_OilPress_Count = 0;
	gs_u16Chime_SeatBelt_Count = 0;
	gs_u8Chime_SCR_1HZCount = 0;
	gs_u8Chime_SCR_2HZ_1Count = 0;
	gs_u8Chime_SCR_2HZ_2Count = 0;
	gs_u8Chime_DPF_Count = 0;
	gs_u8Chime_ADAS_WarningAudio_Count = 0;
	//gs_u8Chime_ADAS_BSDSound_Count = 0;
	//gs_u8Chime_ADAS_CTA_Count = 0;
	//gs_u8Chime_ADAS_DOA_Count = 0;
	gs_u8Chime_ADAS_NavSts_Count = 0;		
	gs_u8Chime_ADAS_HandsonReq_Count = 0;		
	gs_u8Chime_ADAS_TAKE_OVER_Count = 0;		
	gs_u8Chime_ADAS_PCW_STATE_Count = 0;		
	gs_u8Chime_TPMS_Count = 0;
	gs_u8Chime_D2_PEPS_REMAIN_NUM_Count = 0;
	gs_u8Chime_PEPS_IMMO_Count = 0;
	gs_u8Chime_PEPS_KEY_BAT_Count = 0;
	gs_u16Chime_PsSeatBelt_Count = 0;
    gs_u8Chime_D2_POSLMP_Count = 0;
    gs_u8Chime_D2_BKHOME_Count = 0;
	gs_u16Chime_D2_KEY_IN_ON_Count = 0;
    gs_u16Chime_NOT_IN_P_Count = 0;
	gs_u16Chime_D2_NOT_IN_P_Count = 0;
	gs_u8Chime_Eng_High_Temp_Count = 0;
	gs_u8Chime_D2_LIGHT_ON_Count = 0;
	
	gs_u8Chime_PEPS_SSB_count = 0;
    gs_u8Chime_D2_PEPS_SSB_count = 0;
    gs_u8Chime_PEPS_ESCL_Count = 0;
    gs_u8Chime_D2_PEPS_ESCL_Count = 0;
    //st
    gs_u8Chime_PEPS_SYS_count = 0;
    gs_u8Chime_PEPS_DOOR_Count = 0;
    gs_u8Chime_PEPS_NOTOFF_Count = 0;
    gs_u8Chime_PEPS_NOKEY_Count = 0;
    gs_u8Chime_PEPS_KEYOUT_Count = 0;
    gs_u8Chime_PEPS_BRK_Count = 0;
    gs_u8Chime_PEPS_CLUTCH_Count = 0;
    gs_u8Chime_PEPS_NOTPN_Count = 0;

	gs_u8Chime_D2_PEPS_SYS_count = 0;
    gs_u8Chime_D2_PEPS_DOOR_Count = 0;
    gs_u8Chime_D2_PEPS_NOTOFF_Count = 0;
    gs_u8Chime_D2_PEPS_NOKEY_Count = 0;
    gs_u8Chime_D2_PEPS_KEYOUT_Count = 0;
    gs_u8Chime_D2_PEPS_BRK_Count = 0;
    gs_u8Chime_D2_PEPS_CLUTCH_Count = 0;
    gs_u8Chime_D2_PEPS_NOTPN_Count = 0;
    //end
}
/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_Func_Shutdown__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_Func_Shutdown__(void)
{
    Chime_Mgr_StopAll__();                                                     /* stop sound */
#if 1
    Chime_Mgr_ClearInputSts__();
    Chime_Mgr_ClearInputLast__();
    Chime_Mgr_ClearInputReq__();
    Chime_Mgr_ClearCounter__();
    Chime_Mgr_ClearTOutSts__();
    //Chime_Mgr_ClearTempStsVol__();
#endif
}



/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_Arbitrate__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_Arbitrate__(void)
{
    Chime_Mgr_CheckCplt__();                                                   /* check if current sound complete(cplt), if complete clear the request */
    Chime_Mgr_Input2Req__();                                                   /* convert input to chime request */

	//Chime_Mgr_ClearInputReq__();
	//gs_u8Chime_WarnInput_Req[fl_signal_value_fuel_level_U8] = TRUE;
    switch (gs_eChimeProcSts)
    {
        case CHIME_PROC_PREREQ:                                                /* process pop up Pre-Request */
            Chime_Mgr_Proc_PreReq__(); 
            break;

        case CHIME_PROC_DRVSTS:
            Chime_Mgr_CheckDrvSts__();                                         /* check sound driver state */
            break;

        case CHIME_PROC_REQ:
            Chime_Mgr_Proc_RealReq__();                                        /* process pop up Real Request */
            break;

        default:
            gs_eChimeProcSts = CHIME_PROC_PREREQ;                              /* change state to process warning pop up Pre-Request */
            break;
    }
}

static uint8 f_CheckReqType_ByChimeId(uint8 ChimeId_U8)
{
	UINT8 fl_ChimeReqId_U8 = CHIME_NULL;
	
	switch (gs_eChime_KeySts)
    {
        case eIGN_RUN:                                                      /* D1 mode, check the highest priority request */
            fl_ChimeReqId_U8 = gs_tChime_D1Data_Tbl[ChimeId_U8].u8ChimeReqType;
            break;

        case eIGN_OFF:                                                      /* D2 mode, check the highest priority request */
			fl_ChimeReqId_U8 = gs_tChime_D2Data_Tbl[ChimeId_U8].u8ChimeReqType;
            break;

        default:
            break;
    }	
	return fl_ChimeReqId_U8;
}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_Proc_PreReq__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_Proc_PreReq__(void)
{
    UINT8 u8NewChimeSort = (UINT8)0;
    boolean bNotPopUpCheck = FALSE;
    boolean bTurnCheck = FALSE;
	UINT8 fl_ChimeReqId_U8 = CHIME_NULL;

    u8NewChimeSort = Chime_Mgr_FindHighPrio__();                               /* get highest request of Not Pop Up */
    bNotPopUpCheck = Chime_Mgr_CheckNotPopUp__(u8NewChimeSort);                /* check if Not Pop Up request is valid */

    /* check chime respond type */
    gs_tChimeRespData[CHIME_RESP_NOT_POPUP].bChime_RespCheck = bNotPopUpCheck;
    gs_tChimeRespData[CHIME_RESP_NOT_POPUP].u8Chime_RespID = u8NewChimeSort;

    Chime_Mgr_ComparePrio__();                                                 /* judge highest respond type */
    /* check if can respond to Turn Left/Right */
    bTurnCheck = Chime_Mgr_CheckTurnLed__(bNotPopUpCheck, u8NewChimeSort);
	if(u8NewChimeSort == CHIME_DOOR_OPEN)
	{
		//bTurnCheck = TRUE;
	}

	fl_ChimeReqId_U8 = f_CheckReqType_ByChimeId(u8NewChimeSort);
    /* judge current chime respond type */
    switch (gs_u8Chime_Respond)
    {
        case CHIME_RESP_POPUP_01:
        case CHIME_RESP_POPUP_02:
            break;

        case CHIME_RESP_NOT_POPUP:
			if (u8NewChimeSort != gs_u8CurChimeSort || gs_tChime_ReqType[fl_ChimeReqId_U8].u8ChimeRepeatTimes != CHIME_DONT_REPEAT)                           /* Not pop up warning, New request */
            {
                if (FALSE == bTurnCheck)                                       /* directly respond to Turn Left/Right */
                {
                    (void)Rte_Call_tiCS_TISndCdd_RequestToStop(gs_u8CurChimeResID, (UINT8)CHIME_STOP_SU);
                    gs_eChimeProcSts = CHIME_PROC_DRVSTS;
                }
                else                                                           /* stop sound to synchronize with warning */
                {
                    Chime_Out_StartSound__(u8NewChimeSort);
                }

                gs_u8ChimeWarnSts = (UINT8)CHIME_BUSY;
            }
            else                                                               /* Not pop up warning, Not New request */
            {
            	if (CHIME_CLEAR == gs_u8ChimeDrvSts)                                   /* sound driver state idle */
			    {
                	gs_u8ChimeWarnSts = (UINT8)CHIME_IDLE;			        
			    }
                gs_u8ChimeWarnSts = (UINT8)CHIME_IDLE;
            }
            break;

        case CHIME_RESP_NONE:                                                  /* there is no chime request */
        default:
            if (CHIME_CLEAR != gs_u8ChimeDrvSts)                               /* check if need to stop sound */
            {
                (void)Rte_Call_tiCS_TISndCdd_RequestToStop(gs_u8CurChimeResID, (UINT8)CHIME_STOP_SU);
                gs_eChimeProcSts = CHIME_PROC_DRVSTS;
                gs_u8ChimeWarnSts = (UINT8)CHIME_BUSY;
            }
            else
            {
                gs_u8CurChimeSort = CHIME_SORT_NULL;
                gs_u8ChimeWarnSts = (UINT8)CHIME_IDLE;
            }
            break;
    }
}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_CheckDrvSts__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_CheckDrvSts__(void)
{
    boolean bTurnCheck = FALSE;
    boolean bNotPopUpCheck = FALSE;
    UINT8 u8NewChimeSort = (UINT8)0;
	UINT8 fl_ChimeReqId_U8 = CHIME_NULL;

    u8NewChimeSort = Chime_Mgr_FindHighPrio__();                               /* get highest request of Not Pop Up */
    bNotPopUpCheck = Chime_Mgr_CheckNotPopUp__(u8NewChimeSort);                /* check if Not Pop Up request is valid */
                                                                               /* check if can respond to Turn Left/Right */
    bTurnCheck = Chime_Mgr_CheckTurnLed__(bNotPopUpCheck, u8NewChimeSort);
	
	fl_ChimeReqId_U8 = f_CheckReqType_ByChimeId(u8NewChimeSort);																		   
	if(u8NewChimeSort == CHIME_DOOR_OPEN)
	{
		//bTurnCheck = TRUE;
	}

    if ((TRUE == bTurnCheck) || (gs_tChime_ReqType[fl_ChimeReqId_U8].u8ChimeRepeatTimes != CHIME_DONT_REPEAT))                                                    /* directly respond to Turn Left/Right */
    {
        Chime_Out_StartSound__(u8NewChimeSort);
        gs_u8ChimeWarnSts = (UINT8)CHIME_BUSY;
        gs_eChimeProcSts = CHIME_PROC_PREREQ;
    }
    else                                                                       /* check sound state to synchronize with warning */
    {
        if (CHIME_CLEAR == gs_u8ChimeDrvSts)                                   /* sound driver state idle */
        {
            gs_u8CurChimeSort = CHIME_SORT_NULL;
            gs_u8ChimeWarnSts = (UINT8)CHIME_IDLE;
            gs_eChimeProcSts = CHIME_PROC_REQ;                                 /* change state to process real warning pop up request */
        }
        else                                                                   /* unexpected sound driver state, not allowed to interrupt */
        {
            gs_u8ChimeWarnSts = (UINT8)CHIME_BUSY;
        }
    }
}
/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_Proc_RealReq__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_Proc_RealReq__(void)
{
    UINT8 u8NewChimeSort = (UINT8)0;
    boolean bNotPopUpCheck = FALSE;
	
    u8NewChimeSort = Chime_Mgr_FindHighPrio__();                               /* get highest request of Not Pop Up */
    bNotPopUpCheck = Chime_Mgr_CheckNotPopUp__(u8NewChimeSort);                /* check if Not Pop Up request is valid */

    /* check chime respond type */
    gs_tChimeRespData[CHIME_RESP_NOT_POPUP].bChime_RespCheck = bNotPopUpCheck;
    gs_tChimeRespData[CHIME_RESP_NOT_POPUP].u8Chime_RespID = u8NewChimeSort;

    Chime_Mgr_ComparePrio__();                                                 /* judge highest respond type */

    /* judge current chime respond type */
    switch (gs_u8Chime_Respond)
    {
        case CHIME_RESP_POPUP_01:    
        case CHIME_RESP_POPUP_02:  
            break;

        case CHIME_RESP_NOT_POPUP:                                             /* respond to Not pop up */
            Chime_Out_StartSound__(u8NewChimeSort);
            break;

        case CHIME_RESP_NONE:
        default:
            Chime_Out_StartSound__(CHIME_SORT_NULL);
            break;
    }

    if (CHIME_RESP_NONE != gs_u8Chime_Respond)                                 /* No chime request */
    {
        gs_u8ChimeWarnSts = (UINT8)CHIME_BUSY;
        gs_eChimeProcSts = CHIME_PROC_PREREQ;
    }
    else
    {
        gs_u8ChimeWarnSts = (UINT8)CHIME_IDLE;
    }
}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_ComparePrio__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_ComparePrio__(void)
{
    UINT8 u8LoopCount = (UINT8)STD_INT_ZERO;
    UINT8 u8LoopChimeID = (UINT8)STD_INT_ZERO;
    UINT8 u8TempChimeRespType = (UINT8)CHIME_RESP_NONE;
    UINT8 u8TempPriority = (UINT8)CHIME_SORT_NULL;
    UINT8 u8Chime_MaxID = (UINT8)0;

    switch (gs_eChime_KeySts)
    {
        case eIGN_RUN:                                                      /* D1 mode, check the highest priority request */
            u8Chime_MaxID = (UINT8)CHIME_IGON_MAXID;

            for (u8LoopCount = (UINT8)STD_INT_ZERO; u8LoopCount < (UINT8)CHIME_RESP_NUM; u8LoopCount++)
            {
                if ((UINT8)FALSE != gs_tChimeRespData[u8LoopCount].bChime_RespCheck)
                {
                    u8LoopChimeID = gs_tChimeRespData[u8LoopCount].u8Chime_RespID;

                    if ((u8LoopChimeID <= u8Chime_MaxID)
                        && (gs_tChime_D1Data_Tbl[u8LoopChimeID].u8Priority < u8TempPriority))
                    {
                        u8TempChimeRespType = u8LoopCount;
                        u8TempPriority = gs_tChime_D1Data_Tbl[u8LoopChimeID].u8Priority;
                    }
                }
            }
            break;

        case eIGN_OFF:                                                      /* D2 mode, check the highest priority request */
            u8Chime_MaxID = (UINT8)CHIME_IGOFF_MAXID;

            for (u8LoopCount = (UINT8)STD_INT_ZERO; u8LoopCount < (UINT8)CHIME_RESP_NUM; u8LoopCount++)
            {
                if ((UINT8)FALSE != gs_tChimeRespData[u8LoopCount].bChime_RespCheck)
                {
                    u8LoopChimeID = gs_tChimeRespData[u8LoopCount].u8Chime_RespID;

                    if ((u8LoopChimeID <= u8Chime_MaxID)
                        && (gs_tChime_D2Data_Tbl[u8LoopChimeID].u8Priority < u8TempPriority))
                    {
                        u8TempChimeRespType = u8LoopCount;
                        u8TempPriority = gs_tChime_D2Data_Tbl[u8LoopChimeID].u8Priority;
                    }
                }
            }
            break;

        default:
            break;
    }

    gs_u8Chime_Respond = u8TempChimeRespType;
}


/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_FindHighPrio__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static UINT8 Chime_Mgr_FindHighPrio__(void)
{
    UINT8 u8LoopCount = (UINT8)STD_INT_ZERO;
    UINT8 u8LoopChimeID = (UINT8)STD_INT_ZERO;
    UINT8 u8TempChimeSort = (UINT8)CHIME_SORT_NULL;
    UINT8 u8TempPriority = (UINT8)CHIME_SORT_NULL;
    UINT8 u8Chime_MaxID = (UINT8)0;

    switch (gs_eChime_KeySts)
    {
        case eIGN_RUN:                                                      /* D1 mode, check the highest priority request */
            u8Chime_MaxID = (UINT8)CHIME_IGON_MAXID;

            for (u8LoopCount = (UINT8)CHIME_REQ_D1_MINID; u8LoopCount <= (UINT8)CHIME_REQ_D1_MAXID; u8LoopCount++)
            {
                if ((UINT8)FALSE != gs_u8Chime_WarnInput_Req[u8LoopCount])
                {
                    u8LoopChimeID = gs_u8Chime_NotPopUp_Tbl[u8LoopCount];

                    if ((u8LoopChimeID <= u8Chime_MaxID)
                        && (gs_tChime_D1Data_Tbl[u8LoopChimeID].u8Priority < u8TempPriority))
                    {
                        u8TempChimeSort = u8LoopChimeID;
                        u8TempPriority = gs_tChime_D1Data_Tbl[u8LoopChimeID].u8Priority;
                    }
                }
            }
            break;

        case eIGN_OFF:                                                      /* D2 mode, check the highest priority request */
            u8Chime_MaxID = (UINT8)CHIME_IGOFF_MAXID;

            for (u8LoopCount = (UINT8)CHIME_REQ_D2_MINID; u8LoopCount <= (UINT8)CHIME_REQ_D2_MAXID; u8LoopCount++)
            {
                if ((UINT8)FALSE != gs_u8Chime_WarnInput_Req[u8LoopCount])
                {
                    u8LoopChimeID = gs_u8Chime_NotPopUp_Tbl[u8LoopCount];

                    if ((u8LoopChimeID <= u8Chime_MaxID)
                        && (gs_tChime_D2Data_Tbl[u8LoopChimeID].u8Priority < u8TempPriority))
                    {
                        u8TempChimeSort = u8LoopChimeID;
                        u8TempPriority = gs_tChime_D2Data_Tbl[u8LoopChimeID].u8Priority;
                    }
                }
            }
            break;

        default:
            break;
    }

    return (u8TempChimeSort);                                                  /* return the highest priority request */
}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_CheckNotPopUp__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static boolean Chime_Mgr_CheckNotPopUp__(UINT8 u8ChimeSort)
{
    boolean bRet = FALSE;

    if ((UINT8)CHIME_SORT_NULL != u8ChimeSort)                                 /* check if chime ID is valid */
    {
        bRet = TRUE;
    }

    return (bRet);
}



/* ====================================================================================================================
**  Function Name    :    Chime_Out_StartSound__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Out_StartSound__(    UINT8 a_u8ChimeCtlSort)
{
    UINT8 u8ChimeRes = (UINT8)0;
    UINT8 u8ChimeMaxID;

    switch (gs_eChime_KeySts)
    {
        case eIGN_RUN:                                                      /* D1 mode */
            u8ChimeMaxID = (UINT8)CHIME_IGON_MAXID;

            if ((a_u8ChimeCtlSort <= u8ChimeMaxID)                             /* target sound request ID is valid */
                && (eSnd_NoChimeID != gs_tChime_D1Data_Tbl[a_u8ChimeCtlSort].u8ChimeResID))   /* target sound resource is not NULL */
            {
                                                                               /* sound ID offset from volume */
                u8ChimeRes = gs_tChime_D1Data_Tbl[a_u8ChimeCtlSort].u8ChimeResID;
                (void)Rte_Call_tiCS_TISndCdd_RequestToStart(u8ChimeRes, 0);
                gs_u8CurChimeResID = u8ChimeRes;
            }
            break;

        case eIGN_OFF:                                                      /* D2 mode */
            u8ChimeMaxID = (UINT8)CHIME_IGOFF_MAXID;

            if ((a_u8ChimeCtlSort <= u8ChimeMaxID)                             /* target sound request ID is valid */
                && (eSnd_NoChimeID != gs_tChime_D2Data_Tbl[a_u8ChimeCtlSort].u8ChimeResID))   /* target sound resource is not NULL */
            {
                                                                               /* sound ID offset from volume */
                u8ChimeRes = gs_tChime_D2Data_Tbl[a_u8ChimeCtlSort].u8ChimeResID;
                (void)Rte_Call_tiCS_TISndCdd_RequestToStart(u8ChimeRes, 0);
                gs_u8CurChimeResID = u8ChimeRes;
            }
            break;

        default:
            break;
    }	
    gs_u8CurChimeSort = a_u8ChimeCtlSort;                                      /* keep current request */
	
}


/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_SetOutput__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_SetOutput__(void)
{
    gs_u8ChimeDrvSts_Last = gs_u8ChimeDrvSts;                                  /* record last sound driver state */																   
}


/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_GetChimeInput__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_GetChimeInput__(void)
{
    (void)Rte_Read_rpIgnState_IGNState(&gs_eChime_KeySts);
    (void)Rte_Call_tiCS_TISndCdd_CheckStatus(gs_u8CurChimeResID, &gs_u8ChimeDrvSts);          /* check sound driver status */
}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_ClearInputSts__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_ClearInputSts__(void)
{
    UINT8 u8LoopCount = (UINT8)STD_INT_ZERO;

    for (u8LoopCount = (UINT8)STD_INT_ZERO; u8LoopCount < (UINT8)CHIME_REQ_TOTAL_NUM; u8LoopCount++)
    {
        gs_u8Chime_WarnInput[u8LoopCount] = (UINT8)FALSE;
    }
}

static void Chime_Mgr_ClearTOutSts__(void)
{
    UINT8 u8LoopCount = (UINT8)STD_INT_ZERO;

    for (u8LoopCount = (UINT8)STD_INT_ZERO; u8LoopCount < (UINT8)CHIME_REQ_TOTAL_NUM; u8LoopCount++)
    {
        gs_u8Chime_Warn_ToutSts[u8LoopCount] = (UINT8)FALSE;
    }
}
/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_ClearInputLast__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_ClearInputLast__(void)
{
    UINT8 u8LoopCount = (UINT8)STD_INT_ZERO;

    for (u8LoopCount = (UINT8)STD_INT_ZERO; u8LoopCount < (UINT8)CHIME_REQ_TOTAL_NUM; u8LoopCount++)
    {
        gs_u8Chime_WarnInput_Last[u8LoopCount] = (UINT8)FALSE;
    }
}

/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_ClearInputReq__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_ClearInputReq__(void)
{
    UINT8 u8LoopCount = (UINT8)STD_INT_ZERO;

    for (u8LoopCount = (UINT8)STD_INT_ZERO; u8LoopCount < (UINT8)CHIME_REQ_TOTAL_NUM; u8LoopCount++)
    {
        gs_u8Chime_WarnInput_Req[u8LoopCount] = (UINT8)FALSE;
    }
}


/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_CheckCplt__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_CheckCplt__(void)
{
    UINT8 u8Chime_MaxID = (UINT8)0;

    if (eIGN_RUN == gs_eChime_KeySts)                                       /* D1 mode */
    {
        u8Chime_MaxID = (UINT8)CHIME_IGON_MAXID;

        if ((CHIME_CLEAR == gs_u8ChimeDrvSts)                                  /* sound driver state completed, clear request */
            && (CHIME_CLEAR != gs_u8ChimeDrvSts_Last)                          /* last sound driver state */
            && (gs_u8CurChimeSort <= u8Chime_MaxID))                           /* chime ID is valid */
            
        {
        	if(gs_tChime_ReqType[gs_tChime_D1Data_Tbl[gs_u8CurChimeSort].u8ChimeID].u8ChimeRecover == CHIME_REQ_RESP_ONCE)
        	{
            	gs_u8Chime_WarnInput_Req[gs_tChime_D1Data_Tbl[gs_u8CurChimeSort].u8ChimeReqType] = FALSE;
        	}
        }
    }
    else                                                                       /* D2 mode */
    {
        u8Chime_MaxID = (UINT8)CHIME_IGOFF_MAXID;

        if ((CHIME_CLEAR == gs_u8ChimeDrvSts)                                  /* sound driver state completed, clear request */
            && (CHIME_CLEAR != gs_u8ChimeDrvSts_Last)                          /* last sound driver state */
            && (gs_u8CurChimeSort <= u8Chime_MaxID))                           /* chime ID is valid */
        {
        	if(gs_tChime_ReqType[gs_tChime_D2Data_Tbl[gs_u8CurChimeSort].u8ChimeID].u8ChimeRecover == CHIME_REQ_RESP_ONCE)
        	{
            	gs_u8Chime_WarnInput_Req[gs_tChime_D1Data_Tbl[gs_u8CurChimeSort].u8ChimeReqType] = FALSE;
        	}
#if 0
            if ((CHIME_D2_KEY_OUT == gs_u8CurChimeSort)                        /* defect #801376 */
                || (CHIME_D2_PARK_LIGHT == gs_u8CurChimeSort))
            {
                gs_u8CurChimeSort = (UINT8)CHIME_SORT_NULL;
            }
#endif				
        }
    }
}

void CChimeMdl_DoorOpenCounterAdd(void)
{
	if(eIGN_RUN == gs_eChime_KeySts)
	{
		switch(gs_u8CurChimeSort)
		{
            case CHIME_ATS:
                gs_u8Chime_ATS_count++;
                break;
            case CHIME_ATS_DMC:
                gs_u8Chime_ATS_DMC_count++;
                break;
			case CHIME_LOWFUEL:
                gs_u8Chime_LowFule_count++;
                break;
            case CHIME_STT:
                gs_u8Chime_STT_count++;
                break;
            case CHIME_DOOR_OPEN:
                gs_u8Chime_DoorOpenCount++;
                break;
			case CHIME_Over_Spd:
                //gs_u8Chime_Over_Spd_Count++;
                break;
			case CHIME_Low_OilPress:
                gs_u8Chime_Low_OilPress_Count++;
                break;
			case CHIME_SCR_1HZ:
				gs_u8Chime_SCR_1HZCount++;
				break;
			case CHIME_SCR_2HZ_1:
				gs_u8Chime_SCR_2HZ_1Count++;
				break;
			case CHIME_SCR_2HZ_2:
				gs_u8Chime_SCR_2HZ_2Count++;
				break;	
			case CHIME_DPF_WARN:
				gs_u8Chime_DPF_Count++;
				break;			
			case CHIME_LaneAssit_WarningAudio:
				gs_u8Chime_ADAS_WarningAudio_Count++;
				break;
			case CHIME_BSDSound_SwitchSts:
				//gs_u8Chime_ADAS_BSDSound_Count++;
				break;
			case CHIME_CTA_warningReq:
				//gs_u8Chime_ADAS_CTA_Count++;
				break;
			case CHIME_DOA_WarningReq:
				//gs_u8Chime_ADAS_DOA_Count++;
				break;
			case CHIME_Nav_Sts:
				gs_u8Chime_ADAS_NavSts_Count++;
				break;
			case CHIME_LaneAssit_HandsonReq:
				gs_u8Chime_ADAS_HandsonReq_Count++;
				break;
			case CHIME_TAKE_OVER_REQ:
				gs_u8Chime_ADAS_TAKE_OVER_Count++;
				break;
			case CHIME_PCW_STATE:
				gs_u8Chime_ADAS_PCW_STATE_Count++;
				break;
			case CHIME_TPMS_WARN:
				gs_u8Chime_TPMS_Count++;
				break;
			case CHIME_PEPS_IMMO:
				gs_u8Chime_PEPS_IMMO_Count++;
				break;
			case CHIME_PEPS_KEY_BAT:
				gs_u8Chime_PEPS_KEY_BAT_Count++;
				break;
			case CHIME_SEATBELT:
				gs_u16Chime_SeatBelt_Count++;
				break;
			case CHIME_PSNG_SEATBELT:
				gs_u16Chime_PsSeatBelt_Count++;
				break;
			case CHIME_PEPS_SSB:
	            gs_u8Chime_PEPS_SSB_count++;
	            break;
            case CHIME_PEPS_ESCL:
	            gs_u8Chime_PEPS_ESCL_Count++;
	            break;
            case CHIME_NOT_IN_P:
				gs_u16Chime_NOT_IN_P_Count++;
				break;
            //st
            case CHIME_PEPS_SYS_FAIL:
                gs_u8Chime_PEPS_SYS_count++;
                break;
            case CHIME_PEPS_DOOR_OPEN:
                gs_u8Chime_PEPS_DOOR_Count++;
                break;
            case CHIME_PEPS_OPEN_NOT_OFF:
                gs_u8Chime_PEPS_NOTOFF_Count++;
                break;
            case CHIME_PEPS_NO_KEY_FOUND:
                gs_u8Chime_PEPS_NOKEY_Count++;
                break;
            case CHIME_PEPS_KEY_OUT:
                gs_u8Chime_PEPS_KEYOUT_Count++;
                break;
            case CHIME_PEPS_BRK_IND:
                gs_u8Chime_PEPS_BRK_Count++;
                break;
            case CHIME_PEPS_CLUTCH_IND:
                gs_u8Chime_PEPS_CLUTCH_Count++;
                break;
            case CHIME_PEPS_NOTPN:
                gs_u8Chime_PEPS_NOTPN_Count++;
                break;
            case CHIME_ENG_HIGH_TEMP:
		        gs_u8Chime_Eng_High_Temp_Count++;
		        break;
            //end
			default:
				break;		
		}
	}
	else
	{
		switch(gs_u8CurChimeSort)
		{
			case CHIME_D2_NOT_IN_P:
				gs_u16Chime_D2_NOT_IN_P_Count++;
				break;
			case CHIME_D2_PEPS_REMAIN_NUM:
				gs_u8Chime_D2_PEPS_REMAIN_NUM_Count++;
				break;
			case CHIME_D2_KEY_IN_ON:
				gs_u16Chime_D2_KEY_IN_ON_Count++;
				break;
			case CHIME_D2_BK_HOME:
				gs_u8Chime_D2_BKHOME_Count++;
				break;	
			case CHIME_D2_LIGHT_ON:
				gs_u8Chime_D2_LIGHT_ON_Count++;
				break;
			case CHIME_D2_POS_LAMP:
				gs_u8Chime_D2_POSLMP_Count++;
				break;
			case CHIME_D2_CHIME_PEPS_SSB:
	            gs_u8Chime_D2_PEPS_SSB_count++;
	            break;
            case CHIME_D2_PEPS_ESCL:
	            gs_u8Chime_D2_PEPS_ESCL_Count++;
	            break;
            //st
            case CHIME_D2_PEPS_SYS_FAIL:
                gs_u8Chime_D2_PEPS_SYS_count++;
                break;
            case CHIME_D2_PEPS_DOOR_OPEN:
                gs_u8Chime_D2_PEPS_DOOR_Count++;
                break;
            case CHIME_D2_PEPS_OPEN_NOT_OFF:
                gs_u8Chime_D2_PEPS_NOTOFF_Count++;
                break;
            case CHIME_D2_PEPS_NO_KEY_FOUND:
                gs_u8Chime_D2_PEPS_NOKEY_Count++;
                break;
            case CHIME_D2_PEPS_KEY_OUT:
                gs_u8Chime_D2_PEPS_KEYOUT_Count++;
                break;
            case CHIME_D2_PEPS_BRK_IND:
                gs_u8Chime_D2_PEPS_BRK_Count++;
                break;
            case CHIME_D2_PEPS_CLUTCH_IND:
                gs_u8Chime_D2_PEPS_CLUTCH_Count++;
                break;
            case CHIME_D2_PEPS_NOTPN:
                gs_u8Chime_D2_PEPS_NOTPN_Count++;
                break;
            //end                
			default:
				break;					
		}
	}
}

void CChimeMdl_UpdateDiDa(uint8 ChimeNumber)
{
    switch (gs_eChime_KeySts)
    {
        case eIGN_RUN:                                                      /* D1 mode */
			if(ChimeNumber == (uint8)eSnd_N331_Tick)
			{
				gs_u8Chime_WarnInput[CHIME_REQID_TURN_TICK] = FALSE;
			}
			if(ChimeNumber == (uint8)eSnd_N331_Tock)
			{
				gs_u8Chime_WarnInput[CHIME_REQID_TURN_TOC] = FALSE;
			}
            break;

        case eIGN_OFF:                                                      /* D2 mode */
			if(ChimeNumber == (uint8)eSnd_N331_Tick)
			{
				gs_u8Chime_WarnInput[CHIME_REQID_D2_TICK] = FALSE;
			}
			if(ChimeNumber == (uint8)eSnd_N331_Tock)
			{
				gs_u8Chime_WarnInput[CHIME_REQID_D2_TOC] = FALSE;
			}
            break;

        default:
            break;
    }
}




/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_Special_In2Req__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_Special_In2Req__(void)
{	
	/* Prk_Brk   always keep */
	if(gs_u8Chime_WarnInput[CHIME_REQID_PRK_BRK] == TRUE)
	{
	   gs_u8Chime_WarnInput_Req[CHIME_REQID_PRK_BRK] = TRUE;
	   gs_u8Chime_Warn_ToutSts[CHIME_REQID_PRK_BRK] = (UINT8)FALSE;
	}
	else
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_PRK_BRK] = FALSE;
		gs_u8Chime_Warn_ToutSts[CHIME_REQID_PRK_BRK] = (UINT8)TRUE;
	}

	/*SeatBelt keep 60s */
	 if(gs_u8Chime_WarnInput_Req[CHIME_REQID_SEATBELT] == FALSE)
	 {
	 	gs_u16Chime_SeatBelt_Count = 0;
	 }
	 if(gs_u16Chime_SeatBelt_Count >= CHIME_SEATBELT_COUNT)
	 {
	 	gs_u8Chime_WarnInput_Req[CHIME_REQID_SEATBELT] = (UINT8)FALSE;
	 	gs_u8Chime_Warn_ToutSts[CHIME_REQID_SEATBELT] = (UINT8)TRUE;
	 }
	
	// if(gs_u8CurChimeSort == CHIME_SEATBELT)
	// {
		// gs_u16Chime_SeatBeltCount++;
	// }

	/* not use */
    if(gs_u8Chime_WarnInput_Req[CHIME_REQID_ATS] == (UINT8)FALSE)
	{
		gs_u8Chime_ATS_count = 0;
	}
	if(gs_u8Chime_ATS_count >= 1)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_ATS] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_ATS] = (UINT8)TRUE;
	}
	
	/* not use */
    if(gs_u8Chime_WarnInput_Req[CHIME_REQID_ATS_DMC] == (UINT8)FALSE)
	{
		gs_u8Chime_ATS_DMC_count = 0;
	}
	if(gs_u8Chime_ATS_DMC_count >= 1)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_ATS_DMC] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_ATS_DMC] = (UINT8)TRUE;
	}

	/* Low Fuel 1hz/3s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_LOWFUEL] == (UINT8)FALSE)
	{
		gs_u8Chime_LowFule_count = 0;
	}
	if(gs_u8Chime_LowFule_count >= 3)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_LOWFUEL] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_LOWFUEL] = (UINT8)TRUE;
	}	

	/* not use */
    if(gs_u8Chime_WarnInput_Req[CHIME_REQID_STT] == (UINT8)FALSE)
	{
		gs_u8Chime_STT_count = 0;
	}
	if(gs_u8Chime_STT_count >= 1)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_STT] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_STT] = (UINT8)TRUE;
	}

	/*DoorOpen 1hz/10s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_DOOR_OPEN] == (UINT8)FALSE)
	{
		gs_u8Chime_DoorOpenCount = 0;
	}
	if(gs_u8Chime_DoorOpenCount >= CHIME_DOOROPEN_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_DOOR_OPEN] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_DOOR_OPEN] = (UINT8)TRUE;
	}
#if 0
	/*OverSpd 1hz/10s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_Over_Spd] == (UINT8)FALSE)
	{
		gs_u8Chime_Over_Spd_Count = 0;
	}
	if(gs_u8Chime_Over_Spd_Count >= CHIME_Over_Spd_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_Over_Spd] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_Over_Spd] = (UINT8)TRUE;
	}
#endif

	/* Over_Spd   always keep */
	if(gs_u8Chime_WarnInput[CHIME_REQID_Over_Spd] == TRUE)
	{
	   gs_u8Chime_WarnInput_Req[CHIME_REQID_Over_Spd] = TRUE;
	   gs_u8Chime_Warn_ToutSts[CHIME_REQID_Over_Spd] = (UINT8)FALSE;
	}
	else
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_Over_Spd] = FALSE;
		gs_u8Chime_Warn_ToutSts[CHIME_REQID_Over_Spd] = (UINT8)TRUE;
	}

	/* Low_OilPress 2hz/3s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_Low_OilPress] == (UINT8)FALSE)
	{
		gs_u8Chime_Low_OilPress_Count = 0;
	}
	if(gs_u8Chime_Low_OilPress_Count >= CHIME_Low_OilPress_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_Low_OilPress] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_Low_OilPress] = (UINT8)TRUE;
	}
	
	/* not use */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_SCR_1HZ] == (UINT8)FALSE)
	{
		gs_u8Chime_SCR_1HZCount = 0;
	}
	if(gs_u8Chime_SCR_1HZCount >= CHIME_SCR_1HZ_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_SCR_1HZ] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_SCR_1HZ] = (UINT8)TRUE;
	}
	
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_SCR_2HZ_1] == (UINT8)FALSE)
	{
		gs_u8Chime_SCR_2HZ_1Count = 0;
	}
	if(gs_u8Chime_SCR_2HZ_1Count >= CHIME_SCR_2HZ_1_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_SCR_2HZ_1] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_SCR_2HZ_1] = (UINT8)TRUE;
	}
	
	/* not use */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_SCR_2HZ_2] == 0)
	{
		gs_u8Chime_SCR_2HZ_2Count = 0;
	}
	if(gs_u8Chime_SCR_2HZ_2Count >= CHIME_SCR_2HZ_2_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_SCR_2HZ_2] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_SCR_2HZ_2] = (UINT8)TRUE;
	}

	/* not use */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_DPF_WARN] == 0)
	{
		gs_u8Chime_DPF_Count = 0;
	}
	if(gs_u8Chime_DPF_Count >= 3)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_DPF_WARN] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_DPF_WARN] = (UINT8)TRUE;
	}
	
	/* ADAS LDW 2hz/2s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_LaneAssit_WarningAudio] == (UINT8)FALSE)
	{
		gs_u8Chime_ADAS_WarningAudio_Count = 0;
	}
	if(gs_u8Chime_ADAS_WarningAudio_Count >= CHIME_ADAS_WA_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_LaneAssit_WarningAudio] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_LaneAssit_WarningAudio] = (UINT8)TRUE;
	}
	
#if 0	
	/* ADAS BSD 4hz/1s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_BSDSound_SwitchSts] == (UINT8)FALSE)
	{
		gs_u8Chime_ADAS_BSDSound_Count = 0;
	}
	if(gs_u8Chime_ADAS_BSDSound_Count >= CHIME_ADAS_BS_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_BSDSound_SwitchSts] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_BSDSound_SwitchSts] = (UINT8)TRUE;
	}
#endif

	/* LCA_BSDSound   always keep */
	if(gs_u8Chime_WarnInput[CHIME_REQID_BSDSound_SwitchSts] == TRUE)
	{
	   gs_u8Chime_WarnInput_Req[CHIME_REQID_BSDSound_SwitchSts] = TRUE;
	   gs_u8Chime_Warn_ToutSts[CHIME_REQID_BSDSound_SwitchSts] = (UINT8)FALSE;
	}
	else
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_BSDSound_SwitchSts] = FALSE;
		gs_u8Chime_Warn_ToutSts[CHIME_REQID_BSDSound_SwitchSts] = (UINT8)TRUE;
	}

#if 0
	/* ADAS CTA 4hz/1s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_CTA_warningReq] == (UINT8)FALSE)
	{
		gs_u8Chime_ADAS_CTA_Count = 0;
	}
	if(gs_u8Chime_ADAS_CTA_Count >= CHIME_ADAS_CTA_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_CTA_warningReq] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_CTA_warningReq] = (UINT8)TRUE;
	}
#endif

	/* SOD_CTA   always keep */
	if(gs_u8Chime_WarnInput[CHIME_REQID_CTA_warningReq] == TRUE)
	{
	   gs_u8Chime_WarnInput_Req[CHIME_REQID_CTA_warningReq] = TRUE;
	   gs_u8Chime_Warn_ToutSts[CHIME_REQID_CTA_warningReq] = (UINT8)FALSE;
	}
	else
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_CTA_warningReq] = FALSE;
		gs_u8Chime_Warn_ToutSts[CHIME_REQID_CTA_warningReq] = (UINT8)TRUE;
	}
#if 0
	/* ADAS DOA 4hz/1s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_DOA_WarningReq] == (UINT8)FALSE)
	{
		gs_u8Chime_ADAS_DOA_Count = 0;
	}
	if(gs_u8Chime_ADAS_DOA_Count >= CHIME_ADAS_DOA_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_DOA_WarningReq] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_DOA_WarningReq] = (UINT8)TRUE;
	}
#endif
	/* DOA_CTA   always keep */
	if(gs_u8Chime_WarnInput[CHIME_REQID_DOA_WarningReq] == TRUE)
	{
	   gs_u8Chime_WarnInput_Req[CHIME_REQID_DOA_WarningReq] = TRUE;
	   gs_u8Chime_Warn_ToutSts[CHIME_REQID_DOA_WarningReq] = (UINT8)FALSE;
	}
	else
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_DOA_WarningReq] = FALSE;
		gs_u8Chime_Warn_ToutSts[CHIME_REQID_DOA_WarningReq] = (UINT8)TRUE;
	}

	/* not use */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_Nav_Sts] == (UINT8)FALSE)
	{
		gs_u8Chime_ADAS_NavSts_Count = 0;
	}
	if(gs_u8Chime_ADAS_NavSts_Count >= CHIME_ADAS_NS_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_Nav_Sts] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_Nav_Sts] = (UINT8)TRUE;
	}
	
	/* ADAS LKA 1hz/1s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_LaneAssit_HandsonReq] == (UINT8)FALSE)
	{
		gs_u8Chime_ADAS_HandsonReq_Count = 0;
	}
	if(gs_u8Chime_ADAS_HandsonReq_Count >= CHIME_ADAS_HR_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_LaneAssit_HandsonReq] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_LaneAssit_HandsonReq] = (UINT8)TRUE;
	}

	/* ADAS ACC 4hz/1s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_TAKE_OVER_REQ] == (UINT8)FALSE)
	{
		gs_u8Chime_ADAS_TAKE_OVER_Count = 0;
	}
	if(gs_u8Chime_ADAS_TAKE_OVER_Count >= CHIME_ADAS_TO_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_TAKE_OVER_REQ] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_TAKE_OVER_REQ] = (UINT8)TRUE;
	}
	
	/* ADAS FCW 4hz/1s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PCW_STATE] == (UINT8)FALSE)
	{
		gs_u8Chime_ADAS_PCW_STATE_Count = 0;
	}
	if(gs_u8Chime_ADAS_PCW_STATE_Count >= CHIME_ADAS_PS_COUNT)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_PCW_STATE] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PCW_STATE] = (UINT8)TRUE;
	}
	
	/* TPMS 1hz/1s*/
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_TPMS_WARN] == (UINT8)FALSE)
	{
		gs_u8Chime_TPMS_Count = 0;
	}
	if(gs_u8Chime_TPMS_Count >= 1)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_TPMS_WARN] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_TPMS_WARN] = (UINT8)TRUE;
	}
	
	/* PEPS_REMAIN_NUM not use */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_REMAIN_NUM] == (UINT8)FALSE)
	{
		gs_u8Chime_D2_PEPS_REMAIN_NUM_Count = 0;
	}
	if(gs_u8Chime_D2_PEPS_REMAIN_NUM_Count >= 1)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_REMAIN_NUM] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_REMAIN_NUM] = (UINT8)TRUE;
	}
	
	/* PEPS_IMMO 1hz/1s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_IMMO] == (UINT8)FALSE)
	{
		gs_u8Chime_PEPS_IMMO_Count = 0;
	}
	if(gs_u8Chime_PEPS_IMMO_Count >= 1)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_IMMO] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_IMMO] = (UINT8)TRUE;
	}
	
	/* PEPS_KEY_BAT 1hz/1s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_KEY_BAT] == (UINT8)FALSE)
	{
		gs_u8Chime_PEPS_KEY_BAT_Count = 0;
	}
	if(gs_u8Chime_PEPS_KEY_BAT_Count >= 1)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_KEY_BAT] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_KEY_BAT] = (UINT8)TRUE;
	}
#if 0
    /* DRV_SEATBELT */ 
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_SEATBELT] == (UINT8)FALSE)
	{
		gs_u16Chime_SeatBelt_Count = 0;
	}

	if(gs_u16Chime_SeatBelt_Count >= 30)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_SEATBELT] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_SEATBELT] = (UINT8)TRUE;
	}
	
	/* PSNG_SEATBELT */ 
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PSNG_SEATBELT] == (UINT8)FALSE)
	{
		gs_u16Chime_PsSeatBelt_Count = 0;
	}
	if(gs_u16Chime_PsSeatBelt_Count >= 90)
	{
		gs_u8Chime_WarnInput_Req[CHIME_REQID_PSNG_SEATBELT] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PSNG_SEATBELT] = (UINT8)TRUE;
	}
#endif
	
    /* POSLMP 1hz/30s */
    if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_POS_LAMP] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_POSLMP_Count = 0;
    }
    if(gs_u8Chime_D2_POSLMP_Count >= CHIME_POSLMP_COUNT)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_POS_LAMP] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_POS_LAMP] = (UINT8)TRUE;
    }

#if 1
    /*BKHOME 0.5s*/
    if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_BK_HOME] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_BKHOME_Count = 0;
    }
	if(gs_u8Chime_D2_BKHOME_Count >= CHIME_BKHOME_COUNT)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_BK_HOME] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_BK_HOME] = (UINT8)TRUE;
    }
#endif
    /* ADAS BSD DOA IGN OFF */
    if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_LIGHT_ON] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_LIGHT_ON_Count = 0;
    }
	if(gs_u8Chime_D2_LIGHT_ON_Count >= 4)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_LIGHT_ON] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_LIGHT_ON] = (UINT8)TRUE;
    }
	
	/* D2_KEY_IN_ON  not use */
    if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_KEY_IN_ON] == (UINT8)FALSE)
    {
        gs_u16Chime_D2_KEY_IN_ON_Count = 0;
    }
	if(gs_u16Chime_D2_KEY_IN_ON_Count >= 10)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_KEY_IN_ON] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_KEY_IN_ON] = (UINT8)TRUE;
    }
	
    /*PEPS_SSB not use */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_SSB] == (UINT8)FALSE)
    {
        gs_u8Chime_PEPS_SSB_count = 0;
    }
    if(gs_u8Chime_PEPS_SSB_count >= gs_u8SSB_D1_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_SSB] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_SSB] = (UINT8)TRUE;
        
    }
	
	/*PEPS_D2_SSB not use */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_SSB] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_PEPS_SSB_count = 0;
    }
    if(gs_u8Chime_D2_PEPS_SSB_count >= gs_u8SSB_D2_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_SSB] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_SSB] = (UINT8)TRUE;
        gs_u8SSB_D2_CntMax = CHIME_PEPS_D2_SSB_COUNT;
    }
	
    /*PEPS_ESCL not use */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_ESCL] == (UINT8)FALSE)
    {
        gs_u8Chime_PEPS_ESCL_Count = 0;
    }
    if(gs_u8Chime_PEPS_ESCL_Count >= gs_u8ESCL_D1_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_ESCL] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_ESCL] = (UINT8)TRUE;
        
    }
	
	/*PEPS_D2_ESCL not use */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_ESCL] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_PEPS_ESCL_Count = 0;
    }
    if(gs_u8Chime_D2_PEPS_ESCL_Count >= gs_u8ESCL_D2_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_ESCL] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_ESCL] = (UINT8)TRUE;
        gs_u8ESCL_D2_CntMax = CHIME_PEPS_D2_ESCL_COUNT;
    }

    /* NOT_IN_P 0.5hz/30s */
    if(gs_u8Chime_WarnInput_Req[CHIME_REQID_NOT_IN_P] == (UINT8)FALSE)
    {
        gs_u16Chime_NOT_IN_P_Count = 0;
    }
	if(gs_u16Chime_NOT_IN_P_Count >= 15)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_NOT_IN_P] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_NOT_IN_P] = (UINT8)TRUE;
    }
	
	/* NOT_IN_P_D2 0.5hz/30s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_NOT_IN_P] == (UINT8)FALSE)
    {
        gs_u16Chime_D2_NOT_IN_P_Count = 0;
    }
	if(gs_u16Chime_D2_NOT_IN_P_Count >= 15)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_NOT_IN_P] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_NOT_IN_P] = (UINT8)TRUE;
		// gs_u8NOT_IN_P_D2_CntMax = CHIME_NOT_IN_P_COUNT;
    }

	/* Eng_High_Temp 1Hz,3s*/
    if(gs_u8Chime_WarnInput_Req[CHIME_REQID_ENG_HIGH_TEMP] == (UINT8)FALSE)
    {
        gs_u8Chime_Eng_High_Temp_Count = 0;
    }
    if(gs_u8Chime_Eng_High_Temp_Count >= CHIME_ENG_HIGH_TEMP_COUNT)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_ENG_HIGH_TEMP] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_ENG_HIGH_TEMP] = (UINT8)TRUE;
    }

	
    /* PEPS_SYS_FAIL 1hz/1s*/
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_SYS_FAIL] == (UINT8)FALSE)
    {
        gs_u8Chime_PEPS_SYS_count = 0;
    }
    if(gs_u8Chime_PEPS_SYS_count >= gs_u8SYS_D1_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_SYS_FAIL] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_SYS_FAIL] = (UINT8)TRUE;
        
    }
	
	/* PEPS_SYS_FAIL_D2 1hz/1s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_SYS_FAIL] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_PEPS_SYS_count = 0;
    }
    if(gs_u8Chime_D2_PEPS_SYS_count >= gs_u8SYS_D2_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_SYS_FAIL] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_SYS_FAIL] = (UINT8)TRUE;
        gs_u8SYS_D2_CntMax = CHIME_PEPS_COUNT;
    }
	
    /* PEPS_DOOR_OPEN 1hz/1s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_DOOR_OPEN] == (UINT8)FALSE)
    {
        gs_u8Chime_PEPS_DOOR_Count = 0;
    }
    if(gs_u8Chime_PEPS_DOOR_Count >= gs_u8DOOR_D1_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_DOOR_OPEN] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_DOOR_OPEN] = (UINT8)TRUE;
        
    }
	/* PEPS_DOOR_OPEN_D2 1hz/1s */
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_DOOR_OPEN] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_PEPS_DOOR_Count = 0;
    }
    if(gs_u8Chime_D2_PEPS_DOOR_Count >= gs_u8DOOR_D2_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_DOOR_OPEN] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_DOOR_OPEN] = (UINT8)TRUE;
        gs_u8DOOR_D2_CntMax = CHIME_PEPS_COUNT;
    }
	
    //3
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_OPEN_NOT_OFF] == (UINT8)FALSE)
    {
        gs_u8Chime_PEPS_NOTOFF_Count = 0;
    }
    if(gs_u8Chime_PEPS_NOTOFF_Count >= gs_u8NOTOFF_D1_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_OPEN_NOT_OFF] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_OPEN_NOT_OFF] = (UINT8)TRUE;
        
    }
	
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_OPEN_NOT_OFF] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_PEPS_NOTOFF_Count = 0;
    }
    if(gs_u8Chime_D2_PEPS_NOTOFF_Count >= gs_u8NOTOFF_D2_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_OPEN_NOT_OFF] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_OPEN_NOT_OFF] = (UINT8)TRUE;
        gs_u8NOTOFF_D2_CntMax = 3;
    }
	
    //4
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_NO_KEY_FOUND] == (UINT8)FALSE)
    {
        gs_u8Chime_PEPS_NOKEY_Count = 0;
    }
    if(gs_u8Chime_PEPS_NOKEY_Count >= gs_u8NOKEY_D1_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_NO_KEY_FOUND] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_NO_KEY_FOUND] = (UINT8)TRUE;
        
    }
	
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_NO_KEY_FOUND] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_PEPS_NOKEY_Count = 0;
    }
    if(gs_u8Chime_D2_PEPS_NOKEY_Count >= gs_u8NOKEY_D2_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_NO_KEY_FOUND] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_NO_KEY_FOUND] = (UINT8)TRUE;
       gs_u8NOKEY_D2_CntMax = CHIME_PEPS_COUNT;
    }
	
    //5
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_KEY_OUT] == (UINT8)FALSE)
    {
        gs_u8Chime_PEPS_KEYOUT_Count = 0;
    }
    if(gs_u8Chime_PEPS_KEYOUT_Count >= gs_u8KEYOUT_D1_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_KEY_OUT] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_KEY_OUT] = (UINT8)TRUE;
        
    }
	
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_KEY_OUT] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_PEPS_KEYOUT_Count = 0;
    }
    if(gs_u8Chime_D2_PEPS_KEYOUT_Count >= gs_u8KEYOUT_D2_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_KEY_OUT] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_KEY_OUT] = (UINT8)TRUE;
        gs_u8KEYOUT_D2_CntMax = CHIME_PEPS_COUNT;
    }
    //6
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_BRK_IND] == (UINT8)FALSE)
    {
        gs_u8Chime_PEPS_BRK_Count = 0;
    }
    if(gs_u8Chime_PEPS_BRK_Count >= gs_u8BRK_D1_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_BRK_IND] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_BRK_IND] = (UINT8)TRUE;
        
    }
	
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_BRK_IND] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_PEPS_BRK_Count = 0;
    }
    if(gs_u8Chime_D2_PEPS_BRK_Count >= gs_u8BRK_D2_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_BRK_IND] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_BRK_IND] = (UINT8)TRUE;
        gs_u8BRK_D2_CntMax = CHIME_PEPS_COUNT;
    }
    //7
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_CLUTCH_IND] == (UINT8)FALSE)
    {
        gs_u8Chime_PEPS_CLUTCH_Count = 0;
    }
    if(gs_u8Chime_PEPS_CLUTCH_Count >= gs_u8CLUTCH_D1_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_CLUTCH_IND] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_CLUTCH_IND] = (UINT8)TRUE;
        
    }
	
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_CLUTCH_IND] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_PEPS_CLUTCH_Count = 0;
    }
    if(gs_u8Chime_D2_PEPS_CLUTCH_Count >= gs_u8CLUTCH_D2_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_CLUTCH_IND] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_CLUTCH_IND] = (UINT8)TRUE;
        gs_u8CLUTCH_D2_CntMax = CHIME_PEPS_COUNT;
    }
    //8
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_NOTPN] == (UINT8)FALSE)
    {
        gs_u8Chime_PEPS_NOTPN_Count = 0;
    }
    if(gs_u8Chime_PEPS_NOTPN_Count >= gs_u8NOTPN_D1_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_PEPS_NOTPN] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_PEPS_NOTPN] = (UINT8)TRUE;
        
    }
	
	if(gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_NOTPN] == (UINT8)FALSE)
    {
        gs_u8Chime_D2_PEPS_NOTPN_Count = 0;
    }
    if(gs_u8Chime_D2_PEPS_NOTPN_Count >= gs_u8NOTPN_D2_CntMax)
    {
        gs_u8Chime_WarnInput_Req[CHIME_REQID_D2_PEPS_NOTPN] = (UINT8)FALSE;
        gs_u8Chime_Warn_ToutSts[CHIME_REQID_D2_PEPS_NOTPN] = (UINT8)TRUE;
        gs_u8NOTPN_D2_CntMax = CHIME_PEPS_COUNT;
    }
    //end

}

// void CChime_SeatbeltSndState(boolean* seatBeltSndSt_U8)
// {
// 	if(gs_u8CurChimeSort == CHIME_SEATBELT)
// 	{
// 		*seatBeltSndSt_U8 = TRUE;
// 	}
// 	else 
// 	{
// 		*seatBeltSndSt_U8 = FALSE;
// 	}
// }

void CChime_DoorOpenSndState(boolean* doorOpenSndSt_U8)
{
	if(gs_u8CurChimeSort == CHIME_DOOR_OPEN)
	{
		*doorOpenSndSt_U8 = TRUE;
	}
	else 
	{
		*doorOpenSndSt_U8 = FALSE;
	}
}


/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_CheckDrvSts__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static void Chime_Mgr_Input2Req__(void)
{
    UINT8 u8LoopCount = (UINT8)STD_INT_ZERO;
    //UINT8 u8RecoverType;

    /* Special chime request processing */
    Chime_Mgr_Special_In2Req__();

    /* loop request */
    for(u8LoopCount = (UINT8)STD_INT_ZERO; u8LoopCount < (UINT8)CHIME_REQ_NOT_POPUP_NUM; u8LoopCount++)
    {
    	#if 0
        u8RecoverType = gs_tChime_ReqType[u8LoopCount].u8ChimeRecover;

        if (CHIME_REQ_RESP_ONCE == u8RecoverType)                              /* respond once, can be interrupted */
        {
        }
		#endif

		
        if (((UINT8)FALSE == gs_u8Chime_WarnInput_Last[u8LoopCount])
            && ((UINT8)TRUE == gs_u8Chime_WarnInput[u8LoopCount]))
        {
            gs_u8Chime_WarnInput_Req[u8LoopCount] = (UINT8)TRUE;           /* input OFF -> ON, set request */
		}
        else if (((UINT8)TRUE == gs_u8Chime_WarnInput_Last[u8LoopCount])
            && ((UINT8)FALSE == gs_u8Chime_WarnInput[u8LoopCount]))
        {
            gs_u8Chime_WarnInput_Req[u8LoopCount] = (UINT8)FALSE;          /* input ON -> OFF, clear request */
            gs_u8Chime_Warn_ToutSts[u8LoopCount] = (UINT8)FALSE;
        }
        else
        {
            ;
        }
                                                                             /* keep last input status */
        gs_u8Chime_WarnInput_Last[u8LoopCount] =  gs_u8Chime_WarnInput[u8LoopCount];
    }

	//Precheck
	//if(FALSE==ChmIsProveoutDone)
	//{
	// 	 gs_u8Chime_WarnInput_Req[CHIME_REQID_SEATBELT] = (UINT8)FALSE;  
	// 	 gs_u8Chime_WarnInput_Req[CHIME_REQID_PSNG_SEATBELT] = (UINT8)FALSE; 
	//	 gs_u8Chime_WarnInput_Req[CHIME_REQID_LOWFUEL] = (UINT8)FALSE; 
	//}
	
}


/* ====================================================================================================================
**  Function Name    :    Chime_Mgr_CheckTurnLed__()
**  Process Timing   :    -
**  Arguments[Input] :    -
**  Arguments[Output]:    -
**  Return Value     :    -
==================================================================================================================== */
static boolean Chime_Mgr_CheckTurnLed__(boolean u8NotPopUp_Flag, UINT8 u8NotPopUp_ChimeSort)
{
    boolean bRet = FALSE;
    boolean bTurnReq = FALSE;
    UINT8 u8TickDrvSts = (UINT8)CHIME_CLEAR;
    UINT8 u8TocDrvSts = (UINT8)CHIME_CLEAR;
                                                                               /* check Tick/Toc sound state */
    (void)Rte_Call_tiCS_TISndCdd_CheckStatus(eSnd_N331_Tick, &u8TickDrvSts);
    (void)Rte_Call_tiCS_TISndCdd_CheckStatus(eSnd_N331_Tock, &u8TocDrvSts);
                                                                               /* check if there is Turn Left/Right request */
    if (((CHIME_TURN_TICK == u8NotPopUp_ChimeSort)
        || (CHIME_TURN_TOC == u8NotPopUp_ChimeSort))
        && (eIGN_RUN == gs_eChime_KeySts))
    {
        bTurnReq = TRUE;                                                       /* Turn Left/Right request exist */
    }
    else if (((CHIME_D2_TICK == u8NotPopUp_ChimeSort)
        || (CHIME_D2_TOC == u8NotPopUp_ChimeSort))
        && (eIGN_OFF == gs_eChime_KeySts))
    {
        bTurnReq = TRUE;                                                       /* Turn Left/Right request exist */
    }
    else
    {
        ;
    }
                                                                              /* check if can respond to Turn Left/Right */
    if ((TRUE == u8NotPopUp_Flag)
        && (TRUE == bTurnReq)                                                  /* Turn Left/Right request exist */
        && (CHIME_CLEAR == gs_u8ChimeDrvSts)                                   /* current sound state idle */
        && (CHIME_CLEAR == u8TickDrvSts)                                       /* Tick/Toc sound state idle */
        && (CHIME_CLEAR == u8TocDrvSts))
    {
        bRet = TRUE;
    }

    return (bRet);
}

static void Chime_Mgr_TOutRefresh__(void)
{
    UINT8 u8Id;

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_IPM_LaneAssit_HandsonReq(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_IPM_LaneAssit_HandsonReq(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_IPM_LaneAssit_WarningAudio(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_IPM_LaneAssit_WarningAudio(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_MRR_PCW_STATE(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_MRR_PCW_STATE(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_MRR_TakeOverReq(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_MRR_TakeOverReq(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_Nav_Sts(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_Nav_Sts(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_PEPSChime(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_PEPSChime(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_SCRChime_2hz_1(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_SCRChime_2hz_1(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_TPMSChime(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_TPMSChime(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_DPFChime(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_DPFChime(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_DrvSeatbelt(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_DrvSeatbelt(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_PasSeatbelt(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_PasSeatbelt(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_NotInPChime(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_NotInPChime(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_DoorOpenChime(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_DoorOpenChime(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_NotInPChimeIgnoff(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_NotInPChimeIgnoff(Chime_Mgr_GetTimeout(u8Id));

    Rte_Read_rpSR_TIChimeMdl_Recv_ChimeId_ATSChime(&u8Id);
    Rte_Write_ppSR_TIChimeMdl_Send_ToutValue_ATSChime(Chime_Mgr_GetTimeout(u8Id));

}
