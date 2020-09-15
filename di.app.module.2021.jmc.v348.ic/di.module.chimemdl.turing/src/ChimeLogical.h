/*************************************************************************************************************************
**  File Name:  ChimeLogical.h
**  Summary:    Manage the chime
**  Create:     2018-06-01
**  Author:     Liu Yang
**  Update history:
**  NO.     Author         Date              Content
**  1.      Liu Yang      2018-06-01        	New

*************************************************************************************************************************/


/* ============================ Version =============================================================================== */

#ifndef CHIMELOGICAL_H
#define CHIMELOGICAL_H

/* ============================ Include Files ========================================================================= */
#include "std_type.h"
#include "Rte_Type.h"


/* ============================ Type Definitions ====================================================================== */
/* Domain status */
typedef struct ChimeIndex_t
{
	UINT8	Request;
	UINT8 	ChimeReqID;
	BOOL	ChimeState;
}ChimeIndex;

typedef enum
{
    CHIME_STATE_IDLE       = 0x00,       /* IDLE status */
    CHIME_STATE_INIT       = 0x01,       /* INIT status */
    CHIME_STATE_ETM        = 0x02,       /* ETM Status */
    CHIME_STATE_EOL        = 0x03,       /* EOL Status */
    CHIME_STATE_IGON       = 0x04,       /* IGON status */
    CHIME_STATE_IGOFF      = 0x05,       /* IGOFF status */
    CHIME_STATE_STOP       = 0x06,       /* STOP status */
    CHIME_STATE_DIAG       = 0x07        /* DIAG status */
}CHIME_STATUS_e;

/* chime process status */
typedef enum
{
    CHIME_PROC_IDLE       = 0x00,       /* IDLE status */
    CHIME_PROC_PREREQ     = 0x01,       /* Process warning Pre-Request */
    CHIME_PROC_DRVSTS     = 0x02,       /* Judge Sound Driver Status */
    CHIME_PROC_NOT_POPUP  = 0x03,       /* Not pop up */
    CHIME_PROC_REQ        = 0x04        /* Process warning Real Request */
}CHIME_PROC_STATUS_e;


typedef struct
{
    UINT8 u8ChimeID;                     /* unique ID for each Chime */
    UINT8 u8ChimeReqType;                /* request type for process */
    UINT8 u8Priority;                    /* priority */
    UINT8 u8ChimeResID;                  /* sound resource */
    UINT8 u8ChimeCycleType;              /* sound cycle type */
    UINT8 u8ChimeInputType;              /* sound request input type */
}CHIME_DATA_t_;

typedef struct
{
    BOOL  bChime_RespCheck;              /* Chime respond check flag */
    UINT8 u8Chime_RespID;                /* Chime ID for respond */
}CHIME_RESP_DATA_t_;

typedef struct
{
    UINT8 u8ChimeReqID;                  /* Chime request ID */
    UINT8 u8ChimeRecover;                /* if need to recover after interrupted */
    UINT8 u8ChimeRepeatTimes;            /* if do not need to repeat, set to 0 */
}CHIME_REQ_DATA_t_;

/* ============================ Defines =============================================================================== */
/* parmeter for chime request */
#define CHIME_REQ_RESP_RECOVER  (0)      /* need to recover after interrupted */
#define CHIME_REQ_RESP_ONCE     (1)      /* don't need to recover after interrupted */

#define CHIME_DONT_REPEAT		(0)		 /* UPA long */
#define CHIME_MUST_REPEAT		(1)      /*  */


#define CHIME_MODE_NORMAL       (0)
#define CHIME_MODE_ETM          (1)
#define CHIME_MODE_IOC          (2)
#define CHIME_MODE_EOL          (3)


/* stop method */
#define CHIME_STOP_IMMI         (5)
#define CHIME_STOP_TONE         (6)
#define CHIME_STOP_SU           (7)

/* chime input type */
#define CHIME_SET_WARN   		(0)
#define CHIME_SET_CHIME  		(1)

/* sound cycle type */
#define CHIME_CYCLE_0    		(0)      /* continuous */
#define CHIME_CYCLE_1    		(1)
#define CHIME_CYCLE_3    		(2)
#define CHIME_CYCLE_5    		(3)
#define CHIME_EVENT_REPEAT    	(4)
#define CHIME_CONTINUE 	 		(5)


/* ID define for each Chime */
#define CHIME_SORT_NULL         	 (0xFF)             /* invalid sound request */
#define CHIME_SORT_STOP              (0xFE)             /* pop up sound stop request */
#define CHIME_SORT_EOL               (0xFD)             /* EOL buzzer control */


#define	CHIME_NULL			         (0)/* start of IG ON */
#define	CHIME_TURN_TICK			     (1)//continue to use
#define	CHIME_TURN_TOC			     (2)//continue to use
#define	CHIME_DOOR_OPEN			     (3)//continue to use
#define	CHIME_SEATBELT			     (4)//continue to use as driver seatbelt
#define	CHIME_TPMS_WARN			     (5)//continue to use
#define	CHIME_PEPS_ESCL				 (6)//Yxiong2 added
#define	CHIME_PEPS_SYS_FAIL			 (7)//Yxiong2 added
#define	CHIME_PEPS_REMAIN_NUM		 (8)//Yxiong2 added
#define	CHIME_PEPS_DOOR_OPEN		 (9)//Yxiong2 added
#define	CHIME_PEPS_OPEN_NOT_OFF		 (10)//Yxiong2 added
#define	CHIME_PEPS_IMMO				 (11)//Yxiong2 added
#define	CHIME_PEPS_SSB				 (12)//Yxiong2 added
#define	CHIME_PEPS_NO_KEY_FOUND		 (13)//Yxiong2 added
#define	CHIME_PEPS_KEY_OUT			 (14)//Yxiong2 added
#define	CHIME_PEPS_KEY_BAT			 (15)//Yxiong2 added
#define	CHIME_PEPS_BRK_IND			 (16)//Yxiong2 added
#define	CHIME_PEPS_CLUTCH_IND		 (17)//Yxiong2 added
#define	CHIME_PEPS_NOTPN			 (18)//Yxiong2 added
#define	CHIME_PSNG_SEATBELT			 (19)//Yxiong2 added
#define	CHIME_PRK_BRK				 (20)//Yxiong2 added
#define	CHIME_KEY_IN_ON				 (21)//Yxiong2 added
#define	CHIME_BK_HOME				 (22)//Yxiong2 added
#define	CHIME_POS_LAMP				 (23)//Yxiong2 added
#define	CHIME_NOT_IN_P				 (24)//Yxiong2 added
#define CHIME_SCR_1HZ                (25)//zchen26 added
#define CHIME_SCR_2HZ_1              (26)//Yxiong2 added
#define CHIME_SCR_2HZ_2              (27)//Yxiong2 added
#define	CHIME_DPF_WARN				 (28)//Yxiong2 revised
#define CHIME_LaneAssit_WarningAudio (29)
#define CHIME_Nav_Sts                (30)
#define CHIME_LaneAssit_HandsonReq   (31)
#define CHIME_TAKE_OVER_REQ          (32)
#define CHIME_PCW_STATE              (33)
#define CHIME_BSDSound_SwitchSts     (34)
#define CHIME_CTA_warningReq         (35)
#define CHIME_DOA_WarningReq         (36)
#define CHIME_STT                    (37)
#define CHIME_LOWFUEL                (38)
#define CHIME_ATS                    (39)
#define CHIME_ATS_DMC                (40)
#define CHIME_Over_Spd               (41)//pwu5 add
#define CHIME_Low_OilPress           (42)//pwu5 add
#define CHIME_UPA_1HZ          		 (43)//pwu5 add
#define CHIME_UPA_2HZ          		 (44)//pwu5 add
#define CHIME_UPA_4HZ          		 (45)//pwu5 add
#define CHIME_UPA_LONG         		 (46)//pwu5 add
#define CHIME_ENG_HIGH_TEMP          (47)//cpang1 add

       
#define	CHIME_D2_TICK			     (0)/* start of IG OFF */
#define	CHIME_D2_TOC			     (1)
#define	CHIME_D2_PEPS_ESCL			 (2)
#define	CHIME_D2_PEPS_SYS_FAIL		 (3)
#define	CHIME_D2_PEPS_REMAIN_NUM	 (4)
#define	CHIME_D2_PEPS_DOOR_OPEN		 (5)
#define	CHIME_D2_PEPS_OPEN_NOT_OFF	 (6)
#define	CHIME_D2_CHIME_PEPS_SSB		 (7)
#define	CHIME_D2_PEPS_NO_KEY_FOUND	 (8)
#define	CHIME_D2_PEPS_KEY_OUT		 (9)
#define	CHIME_D2_PEPS_BRK_IND		 (10)
#define	CHIME_D2_PEPS_CLUTCH_IND	 (11)
#define CHIME_D2_PEPS_NOTPN          (12)
#define	CHIME_D2_KEY_IN_ON			 (13)
#define	CHIME_D2_BK_HOME			 (14)
#define	CHIME_D2_POS_LAMP			 (15)
#define	CHIME_D2_NOT_IN_P			 (16)
#define	CHIME_D2_LIGHT_ON			 (17)

/* chime sort */
#define CHIME_IGON_MAXID             (CHIME_ENG_HIGH_TEMP)
#define CHIME_IGOFF_MAXID            (CHIME_D2_LIGHT_ON)

#define CHIME_IGON_NUM               (CHIME_IGON_MAXID + 1)     /* Chime of IG ON */
#define CHIME_IGOFF_NUM              (CHIME_IGOFF_MAXID + 1)    /* Chime of IG OFF */


/* chime respond type */
#define CHIME_RESP_POPUP_01     	 (0)                        /* main pop up */
#define CHIME_RESP_POPUP_02     	 (1)                        /* middle pop up */
#define CHIME_RESP_NOT_POPUP    	 (2)
#define CHIME_RESP_NONE         	 (0xFF)

#define CHIME_RESP_NUM          	 (3)                        /* number of valid respond type */

/* sound driver state */
#define CHIME_PLAY_REQUESTED     	 (0x01u)
#define CHIME_STOP_REQUESTED     	 (0x02u)
#define CHIME_ACTIVATED          	 (0x04u)
#define CHIME_QUEUE_TIMEOUT      	 (0x08u)
#define CHIME_RESERVED           	 (0x10u)
#define CHIME_RESERVED1         	 (0x20u)
#define CHIME_COMPLETED         	 (0x40u)
#define CHIME_INTERRUPTED       	 (0x80u)
#define CHIME_CLEAR             	 (0x00u)




/* chime request */
#define	CHIME_REQID_NULL					(0)
#define	CHIME_REQID_TURN_TICK				(1)
#define	CHIME_REQID_TURN_TOC				(2)
#define	CHIME_REQID_DOOR_OPEN				(3)
#define	CHIME_REQID_SEATBELT				(4)
#define	CHIME_REQID_TPMS_WARN				(5)
#define	CHIME_REQID_PEPS_ESCL				(6)//Yxiong2 added
#define	CHIME_REQID_PEPS_SYS_FAIL			(7)//Yxiong2 added
#define	CHIME_REQID_PEPS_REMAIN_NUM			(8)//Yxiong2 added
#define	CHIME_REQID_PEPS_DOOR_OPEN			(9)//Yxiong2 added
#define	CHIME_REQID_PEPS_OPEN_NOT_OFF		(10)//Yxiong2 added
#define	CHIME_REQID_PEPS_IMMO				(11)//Yxiong2 added
#define	CHIME_REQID_PEPS_SSB				(12)//Yxiong2 added
#define	CHIME_REQID_PEPS_NO_KEY_FOUND		(13)//Yxiong2 added
#define	CHIME_REQID_PEPS_KEY_OUT			(14)//Yxiong2 added
#define	CHIME_REQID_PEPS_KEY_BAT			(15)//Yxiong2 added
#define	CHIME_REQID_PEPS_BRK_IND			(16)//Yxiong2 added
#define	CHIME_REQID_PEPS_CLUTCH_IND			(17)//Yxiong2 added
#define	CHIME_REQID_PEPS_NOTPN				(18)//Yxiong2 added
#define	CHIME_REQID_PSNG_SEATBELT			(19)//Yxiong2 added
#define	CHIME_REQID_PRK_BRK					(20)//Yxiong2 added
#define	CHIME_REQID_KEY_IN_ON				(21)//Yxiong2 added
#define	CHIME_REQID_BK_HOME					(22)//Yxiong2 added
#define	CHIME_REQID_POS_LAMP				(23)//Yxiong2 added
#define	CHIME_REQID_NOT_IN_P				(24)//Yxiong2 added
#define CHIME_REQID_SCR_1HZ             	(25)//Zchen26 added
#define CHIME_REQID_SCR_2HZ_1           	(26)//Zchen26 added
#define CHIME_REQID_SCR_2HZ_2           	(27)//Zchen26 added
#define	CHIME_REQID_DPF_WARN				(28)//Yxiong2 revised
#define CHIME_REQID_LaneAssit_WarningAudio  (29)
#define CHIME_REQID_Nav_Sts               	(30)
#define CHIME_REQID_LaneAssit_HandsonReq  	(31)
#define CHIME_REQID_TAKE_OVER_REQ         	(32)
#define CHIME_REQID_PCW_STATE             	(33)
#define CHIME_REQID_BSDSound_SwitchSts    	(34)
#define CHIME_REQID_CTA_warningReq        	(35)
#define CHIME_REQID_DOA_WarningReq        	(36)
#define CHIME_REQID_STT                   	(37)
#define CHIME_REQID_LOWFUEL				  	(38)//lfan5
#define CHIME_REQID_ATS                   	(39)
#define CHIME_REQID_ATS_DMC               	(40)
#define CHIME_REQID_Over_Spd              	(41)//pwu5 add
#define CHIME_REQID_Low_OilPress            (42)//pwu5 add
#define CHIME_REQID_UPA_1HZ          		(43)//pwu5 add
#define CHIME_REQID_UPA_2HZ          		(44)//pwu5 add
#define CHIME_REQID_UPA_4HZ          		(45)//pwu5 add
#define CHIME_REQID_UPA_LONG         		(46)//pwu5 add
#define CHIME_REQID_ENG_HIGH_TEMP           (47)//cpang1 Add


#define	CHIME_REQID_D2_TICK				  	(48)//Yxiong2 revised
#define	CHIME_REQID_D2_TOC				  	(49)//Yxiong2 revised
#define	CHIME_REQID_D2_PEPS_ESCL		  	(50)
#define	CHIME_REQID_D2_PEPS_SYS_FAIL	  	(51)
#define	CHIME_REQID_D2_PEPS_REMAIN_NUM	  	(52)
#define	CHIME_REQID_D2_PEPS_DOOR_OPEN	  	(53)
#define	CHIME_REQID_D2_PEPS_OPEN_NOT_OFF  	(54)
#define	CHIME_REQID_D2_PEPS_SSB			  	(55)
#define	CHIME_REQID_D2_PEPS_NO_KEY_FOUND  	(56)
#define	CHIME_REQID_D2_PEPS_KEY_OUT		  	(57)
#define	CHIME_REQID_D2_PEPS_BRK_IND		  	(58)
#define	CHIME_REQID_D2_PEPS_CLUTCH_IND	  	(59)
#define CHIME_REQID_D2_PEPS_NOTPN         	(60)
#define	CHIME_REQID_D2_KEY_IN_ON		  	(61)
#define	CHIME_REQID_D2_BK_HOME			  	(62)
#define	CHIME_REQID_D2_POS_LAMP			  	(63)
#define	CHIME_REQID_D2_NOT_IN_P			  	(64)
#define	CHIME_REQID_D2_LIGHT_ON			  	(65)//Yxiong2 revised

#define	CHIME_REQID_POPUP1			      	(66)//Yxiong2 revised
#define	CHIME_REQID_POPUP2			      	(67)//Yxiong2 revised

#define	CHIME_REQ_NOT_POPUP_NUM				(CHIME_REQID_POPUP1)
#define	CHIME_REQ_TOTAL_NUM					(CHIME_REQID_POPUP2+1)             /* number of chime which don't relate to pop up */
#define	CHIME_REQ_D1_MINID					(CHIME_REQID_NULL)                 /* scope of D1 mode Request ID */
#define	CHIME_REQ_D1_MAXID					(CHIME_REQID_ENG_HIGH_TEMP)
#define	CHIME_REQ_D2_MINID					(CHIME_REQID_D2_TICK)              /* scope of D2 mode Request ID */
#define	CHIME_REQ_D2_MAXID					(CHIME_REQID_D2_LIGHT_ON)

#define cUSE_CHM_MANAGER_PROVEOUT 			(1)
#define cCHM_MANAGER_PROVEOUT_TIME_IN_MS 	(3000)							   /* ChimeMld is 100ms task，30X100ms=3s */


/* function declare for system call */
static void Chime_Mgr_VarInit__(void);
static void Chime_Mgr_StopAll__(void);
void  Chime_Mgr_InitHW(void);
void  Chime_Mgr_InitSW(void);
void  Chime_Mgr_ExitHW(void);
void Chime_Mgr_ExitSW(void);
void Chime_Mgr_Notify_Shutdown(void);
void Chime_Mgr_Notify_Restore(void);
void OnTask10ms_Chime_Routine(void);
static void Chime_Mgr_RunRoutine__(void);
void CChimeMdl_UpdateStatus_Operation(UINT8 index, UINT8 warning_status);
static BOOL Chime_Mgr_CheckLVSD__(void);
static BOOL Chime_Mgr_CheckHVSD__(void);
static BOOL Chime_Mgr_CheckConfig__(void);
static void Chime_Mgr_ProcRoutine__(void);
static void Chime_Mgr_Func_Shutdown__(void);
static void Chime_Mgr_Arbitrate__(void);
static void Chime_Out_StartSound__(    UINT8 a_u8ChimeCtlSort);
static void Chime_Mgr_Proc_PreReq__(void);
static void Chime_Mgr_CheckDrvSts__(void);
static void Chime_Mgr_Proc_RealReq__(void);
static void Chime_Mgr_ComparePrio__(void);
static UINT8 Chime_Mgr_FindHighPrio__(void);
static BOOL Chime_Mgr_CheckNotPopUp__(UINT8 u8ChimeSort);
static void Chime_Mgr_SetOutput__(void);
static void Chime_Mgr_CheckCplt__(void);
static void Chime_Mgr_GetChimeInput__(void);
static void Chime_Mgr_Special_In2Req__(void);
static void Chime_Mgr_Input2Req__(void);
static void Chime_Mgr_ClearInputReq__(void);
static void Chime_Mgr_ClearInputLast__(void);
static void Chime_Mgr_ClearInputSts__(void);
static void Chime_Mgr_ClearTOutSts__(void);
static UINT8 f_CheckReqType_ByChimeId(UINT8 ChimeId_U8);

static void Chime_Mgr_ClearCounter__(void);
static void Chime_Mgr_TOutRefresh__(void);

BOOL Chime_Mgr_GetTimeout(UINT8 index);

extern BOOL CChimeMdl_Ignoff_Chime_State__(void);
extern Std_ReturnType CSndCdd_RequestToStop(uint8 ChimeId, uint8 StopMethod);



#  ifndef CHIME_IDLE
#   define CHIME_IDLE (0U)
#  endif

#  ifndef CHIME_BUSY
#   define CHIME_BUSY (1U)
#  endif

#  ifndef eChimeStat_Idle
#   define eChimeStat_Idle (0U)
#  endif

#  ifndef eChimeStat_Queued
#   define eChimeStat_Queued (1U)
#  endif

#  ifndef eChimeStat_TimedOut
#   define eChimeStat_TimedOut (2U)
#  endif

#  ifndef eChimeStat_Playing
#   define eChimeStat_Playing (3U)
#  endif

#  ifndef eChimeStat_Interrupted
#   define eChimeStat_Interrupted (4U)
#  endif

#  ifndef eChimeStat_Completed
#   define eChimeStat_Completed (5U)
#  endif

#  ifndef eChimeStat_Count
#   define eChimeStat_Count (6U)
#  endif

#endif    /* CHIME_H */

