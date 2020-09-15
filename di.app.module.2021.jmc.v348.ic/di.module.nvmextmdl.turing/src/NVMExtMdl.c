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

#include "Rte_NVMExtMdl.h"
#include "CmpLib.h"
#include "Dem.h"
#include "NvM.h"
/// @brief  Instance of life-cycle state-machine
CMPLIB_INSTANCE(NVMExtMdl)

/* ADC channel number to be passed to IoHwAb */


//=====================================================================================================================
//  CONSTANTS & TYPES
//=====================================================================================================================
static void fnvmextmdl_cluster_optional_byte8(void);

static NV_Data_Cluster_Optional_Feature_Configure l_cluster_optional_feature_config;
#define VEHICLE_MODELS_BIT		0x03
#define LANGUAGE_BIT			0x1C
#define MAINTENANCE_INT_BIT		0xE0

//=====================================================================================================================
//  FORWARD DECLARATIONS
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional initialization state
///
/// @return E_OK:           Succeed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpInit( void )
{
   
    return E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  Transitional de-initialization state
///
/// @return E_OK:           Succeed
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDeInit( void )
{
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
    return  E_OK;
}

//---------------------------------------------------------------------------------------------------------------------
/// @brief  State for normal operations
///
/// @return The return value is disregarded. Say E_OK
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpActive(void)
{
	
 	(void)Rte_Read_NV_Cluster_Optional_Feature_Configure_Cluster_Feature_Configure_Value(&l_cluster_optional_feature_config);
	fnvmextmdl_cluster_optional_byte8();
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
/// @brief  Transitional state to clean up after diagnostic state. Component behavior to be restored.
///
/// @return E_OK:           Diagnostic clean-up has completed. Return to the previous state (Active or Inactive).
///<br>     E_NOT_READY:    Keeps the component in this state.
//---------------------------------------------------------------------------------------------------------------------
static Std_ReturnType CmpDiagReturn( void )
{
    return E_OK;
}


static void fnvmextmdl_cluster_optional_byte8(void)
{
	if(((l_cluster_optional_feature_config.Option_Configure_Byte8 & MAINTENANCE_INT_BIT) >> 5) == MAINTENANCE_INTERVAL_SET_5000KM)
	{
		Rte_Write_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_MAINTENANCE_INTERVAL_VARIETY(MAINTENANCE_INTERVAL_SET_5000KM);
	}
	else if(((l_cluster_optional_feature_config.Option_Configure_Byte8 & MAINTENANCE_INT_BIT) >> 5) == MAINTENANCE_INTERVAL_SET_7500KM)
	{
		Rte_Write_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_MAINTENANCE_INTERVAL_VARIETY(MAINTENANCE_INTERVAL_SET_7500KM);
	}
	else
	{
		Rte_Write_ppSR_pNVMExtMdl_TINVMExtMdl_Cluster_Optional_Configure_MAINTENANCE_INTERVAL_VARIETY(MAINTENANCE_INTERVAL_SET_RESERVED);
	}
}


void Sp_NvMNotifyJobFinished_Cluster_SIAResetOffset(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	Rte_Call_rpCS_TINVMMdl_SIA_Retset_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);

	#if 0
	if(eol_nvmwrite_status() == 0x02)
	{
		if(eol_write_block_id() == NvMConf_NvMBlockDescriptor_ClusterSIAResetOffset)
		{
			eol_nvmwrite_callback(TRUE);
		}
	}
	#endif
}

void Sp_NvMNotifyJobFinished_Cluster_Optional_Configure(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	Rte_Call_rpCS_TINVMMdl_SIA_Optional_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
}

void Sp_NvMNotifyJobFinished_JobFinished_Dim(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	Rte_Call_rpCS_Dim_TINVMMdl_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
}

void Sp_NvMNotifyJobFinished_MPCDateTime(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
    Rte_Call_rpCS_TIAmbientMdl_MPCDateTime_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
}

void Sp_NvMNotifyJobFinished_FuelLevelValue(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	Rte_Call_rpCS_TIFuelLevelMdl_FuelLevel_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
}



/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :
** @Function Description :
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
void Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer0(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	Rte_Call_rpCS_TIOdoMdl_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
	#if 0
	if(eol_nvmwrite_status() == 0x02)
	{
		if(eol_write_block_id() == NvMConf_NvMBlockDescriptor_OdometerBuffer0)
		{
			eol_nvmwrite_callback(TRUE);
		}
	}
	#endif
}
/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :
** @Function Description :
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
void Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer1(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	Rte_Call_rpCS_TIOdoMdl_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
	#if 0
	if(eol_nvmwrite_status() == 0x02)
	{
		if(eol_write_block_id() == NvMConf_NvMBlockDescriptor_OdometerBuffer1)
		{
			eol_nvmwrite_callback(TRUE);
		}
	}
	#endif
}
/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :
** @Function Description :
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
void Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer2(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	Rte_Call_rpCS_TIOdoMdl_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
	#if 0
	if(eol_nvmwrite_status() == 0x02)
	{
		if(eol_write_block_id() == NvMConf_NvMBlockDescriptor_OdometerBuffer2)
		{
			eol_nvmwrite_callback(TRUE);
		}
	}
	#endif
}
/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :
** @Function Description :
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
void Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer3(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	Rte_Call_rpCS_TIOdoMdl_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
	#if 0
	if(eol_nvmwrite_status() == 0x02)
	{
		if(eol_write_block_id() == NvMConf_NvMBlockDescriptor_OdometerBuffer3)
		{
			eol_nvmwrite_callback(TRUE);
		}
	}
	#endif
}
/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :
** @Function Description :
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
void Sp_NvMNotifyJobFinished_JobFinished_OdoMdl_Buffer4(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	Rte_Call_rpCS_TIOdoMdl_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
	#if 0
	if(eol_nvmwrite_status() == 0x02)
	{
		if(eol_write_block_id() == NvMConf_NvMBlockDescriptor_OdometerBuffer4)
		{
			eol_nvmwrite_callback(TRUE);
		}
	}
	#endif
}
/*---------------------------------------------------------------------------------------------------------------------
** @Function name        :
** @Function Description :
** @return value         :
--------------------------------------------------------------------------------------------------------------------
** @Record
** @modify reason        :
** @modify version       :
** @author               :
** @data                 :
**---------------------------------------------------------------------------------------------------------------------*/
void Sp_OdoResetOffSet_NvMNotifyJobFinished_JobFinished(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	Rte_Call_rpCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
	#if 0
	if(eol_nvmwrite_status() == 0x02)
	{
		if(eol_write_block_id() == NvMConf_NvMBlockDescriptor_ClusterOdoResetOffset)
		{
			eol_nvmwrite_callback(TRUE);
		}
	}
	#endif 
}

void Sp_NvMNotifyJobFinished_Cluster_ODO_Backup_Counter(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	//Rte_Call_rpCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
	#if 0
	if(eol_nvmwrite_status() == 0x02)
	{
		if(eol_write_block_id() == NvMConf_NvMBlockDescriptor_ClusterOdoResetOffset)
		{
			eol_nvmwrite_callback(TRUE);
		}
	}
	#endif 
}
void Sp_NvMNotifyJobFinished_Cluster_Variable_Block(NvM_ServiceIdType ServiceId, NvM_RequestResultType JobResult)
{
	//Rte_Call_rpCS_TIOdoLogicalMdl_OdoResetOffSet_NvMNotifyJobFinished_NvMNotifyJobFinished(ServiceId,JobResult);
	#if 0
	if(eol_nvmwrite_status() == 0x02)
	{
		if(eol_write_block_id() == NvMConf_NvMBlockDescriptor_ClusterOdoResetOffset)
		{
			eol_nvmwrite_callback(TRUE);
		}
	}
	#endif 
}





