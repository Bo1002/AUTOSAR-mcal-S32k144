/*
 * Adc.c
 *
 *  Created on: Jun 26, 2026
 *      Author: BO
 */
 
#include <S32K144.h>
#include "Adc.h"
#include "Adc_Cfg.h"
#include "Adc_Types.h"
 
/*==================================================================================================
*  STATIC VARIABLES
==================================================================================================*/
static const Adc_ConfigType        *Adc_pConfig        = NULL_PTR;
 
static uint8                        Adc_initsate        = STD_OFF;
 
/* Khai báo là MẢNG (array), không phải con trỏ — trước đây khai báo là pointer nên crash */
static Adc_StatusType               Adc_GroupStatus[ADC_MAX_GROUPS];
 
/* Buffer pointer từng group — app truyền vào qua SetupResultBuffer */
static Adc_ValueGroupType          *Adc_pGroupValue[ADC_MAX_GROUPS];
 
static uint8                        Adc_CurrentChannelIdx[ADC_MAX_GROUPS];
 
/* Khai báo là MẢNG, không phải con trỏ */
static uint8                        Adc_SampleCount[ADC_MAX_GROUPS];
 
static boolen                       Adc_HwTriggerEnabled[ADC_MAX_GROUPS];
 
static boolen                       Adc_GroupNotification[ADC_MAX_GROUPS];
 
static Adc_PowerStateType           Adc_CurrentPowerState  = ADC_FULL_POWER;
static Adc_PowerStateType           Adc_TargetPowerState   = ADC_FULL_POWER;
static boolen                       Adc_PowerStatePrepared = FALSE;
 
/* Group đang được convert — ISR dùng biến này để biết group nào đang chạy */
static Adc_GroupType                Adc_ActiveGroup        = 0u;
 
 
/*==================================================================================================
*  Adc_Init
==================================================================================================*/
FUNC(void, ADC_CODE) Adc_Init(P2CONST(Adc_ConfigType, AUTOMATIC, ADC_APPL_CONST) ConfigPtr)
{
    uint8 i;
    uint8 j;
 
    /* Validate TRƯỚC khi dùng ConfigPtr */
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (ConfigPtr == NULL_PTR)
    {
        return;
    }
    if (Adc_initsate == STD_ON)
    {
        return;
    }
    #endif
 
    /* Lưu config SAU khi validate */
    Adc_pConfig = ConfigPtr;
 
    for (i = 0u; i < ConfigPtr->numGroups; i++)
    {
        const Adc_GroupConfigType* pGroup = &ConfigPtr->Configtool[i];
 
        /* Init runtime state cho group — đặt ở outer loop, không phải inner */
        Adc_GroupStatus[pGroup->groupId]    = ADC_IDLE;
        Adc_HwTriggerEnabled[i]             = FALSE;
        Adc_GroupNotification[i]            = FALSE;
        Adc_CurrentChannelIdx[i]            = 0u;
        Adc_SampleCount[i]                  = 0u;
        Adc_pGroupValue[i]                  = NULL_PTR;
 
        for (j = 0u; j < pGroup->numChannels; j++)
        {
            const Adc_ChannelConfigType* pChannel = &pGroup->channelConfigPtr[j];
 
            switch (pChannel->resolution)
            {
                case 8u:
                    IP_ADC0->CFG1 = (0x00u << 2u) | (0x00u << 0u) | (0x01u << 5u);
                    break;
                case 12u:
                    IP_ADC0->CFG1 = (0x01u << 2u) | (0x00u << 0u) | (0x01u << 5u);
                    break;
                case 10u:
                    IP_ADC0->CFG1 = (0x02u << 2u) | (0x00u << 0u) | (0x01u << 5u);
                    break;
                default:
                    break;
            }
 
            IP_ADC0->CFG2 = ADC_CFG2_SMPLTS(pChannel->samplingTime);
        }
    }
 
    /* Enable ADC0 interrupt trong NVIC để ISR có thể chạy */
    NVIC_EnableIRQ(ADC0_IRQn);
 
    Adc_initsate = STD_ON;
}
 
 
/*==================================================================================================
*  Adc_SetupResultBuffer
==================================================================================================*/
FUNC(Std_ReturnType, ADC_CODE) Adc_SetupResultBuffer(
    VAR(Adc_GroupType, AUTOMATIC) Group,
    P2VAR(Adc_ValueGroupType, AUTOMATIC, ADC_APPL_DATA) DataBufferPtr)
{
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        return E_NOT_OK;   /* void function không return giá trị được */
    }
    if (Group >= ADC_MAX_GROUPS)
    {
        return E_NOT_OK;
    }
    if (DataBufferPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }
    #endif
 
    Adc_pGroupValue[Group] = DataBufferPtr;
 
    return E_OK;
}
 
 
/*==================================================================================================
*  Adc_DeInit
==================================================================================================*/
FUNC(void, ADC_CODE) Adc_DeInit(void)
{
    uint8 i;
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        return;
    }
    /* Điều kiện loop phải có i < numGroups */
    for (i = 0u; i < Adc_pConfig->numGroups; i++)
    {
        if (Adc_GroupStatus[i] == ADC_BUSY)
        {
            return;
        }
    }
    #endif
 
    IP_ADC0->SC1[0] = 0x1Fu;
    IP_ADC0->CFG1   = 0x00u;
    IP_ADC0->CFG2   = 0x0Cu;
    IP_ADC0->SC2    = 0x00u;
    IP_ADC0->SC3    = 0x00u;
 
    /* Điều kiện loop phải có i < numGroups */
    for (i = 0u; i < Adc_pConfig->numGroups; i++)
    {
        Adc_GroupStatus[i] = ADC_IDLE;
        Adc_pGroupValue[i] = NULL_PTR;
    }
 
    Adc_pConfig  = NULL_PTR;
    Adc_initsate = STD_OFF;
}
 
 
/*==================================================================================================
*  Adc_StartGroupConversion
==================================================================================================*/
FUNC(void, ADC_CODE) Adc_StartGroupConversion(VAR(Adc_GroupType, AUTOMATIC) Group)
{
    uint8 i;
    const Adc_GroupConfigType *pGroup = NULL_PTR;
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        return;
    }
    if (Group >= ADC_MAX_GROUPS)   /* >= không phải > */
    {
        return;
    }
    /* Check đúng: group ĐANG BUSY thì không start lại được */
    if (Adc_GroupStatus[Group] == ADC_BUSY)
    {
        return;
    }
    #endif
 
    /* Tìm group config theo groupId */
    for (i = 0u; i < Adc_pConfig->numGroups; i++)
    {
        if (Adc_pConfig->Configtool[i].groupId == Group)   /* phải index [i] */
        {
            pGroup = &Adc_pConfig->Configtool[i];
            break;
        }
    }
 
    /* Kiểm tra tìm thấy group chưa */
    if (pGroup == NULL_PTR)
    {
        return;
    }
 
    Adc_ActiveGroup              = Group;   /* ISR cần biết group nào đang convert */
    Adc_GroupStatus[Group]       = ADC_BUSY;
    Adc_CurrentChannelIdx[Group] = 0u;
 
    /* Dùng runtime flag Adc_GroupNotification (không phải config flag) */
    if (Adc_GroupNotification[Group] == TRUE)
    {
        IP_ADC0->SC1[0] = ADC_SC1_ADCH(pGroup->channelConfigPtr[0].channelId)
                        | ADC_SC1_AIEN_MASK;
    }
    else
    {
        IP_ADC0->SC1[0] = ADC_SC1_ADCH(pGroup->channelConfigPtr[0].channelId);
    }
}
 
 
/*==================================================================================================
*  Adc_StopGroupConversion
==================================================================================================*/
FUNC(void, ADC_CODE) Adc_StopGroupConversion(VAR(Adc_GroupType, AUTOMATIC) Group)
{
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Group >= ADC_MAX_GROUPS)   /* >= không phải > */
    {
        return;   /* void function không return giá trị */
    }
    if (Adc_GroupStatus[Group] == ADC_IDLE)
    {
        return;
    }
    #endif
 
    IP_ADC0->SC1[0] = 0x1Fu;                  /* SC1[0] không phải SC1[Group] */
    Adc_GroupStatus[Group] = ADC_IDLE;         /* set GroupStatus, không phải Adc_initsate */
    Adc_CurrentChannelIdx[Group] = 0u;
}
 
 
/*==================================================================================================
*  Adc_ReadGroup
==================================================================================================*/
FUNC(Std_ReturnType, ADC_CODE) Adc_ReadGroup(
    VAR(Adc_GroupType, AUTOMATIC) Group,
    P2VAR(Adc_ValueGroupType, AUTOMATIC, ADC_APPL_DATA) DataBufferPtr)
{
    const Adc_GroupConfigType *pGroup = NULL_PTR;
    uint8 j;
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        return E_NOT_OK;
    }
    if (Group >= ADC_MAX_GROUPS)
    {
        return E_NOT_OK;
    }
    if (DataBufferPtr == NULL_PTR)
    {
        return E_NOT_OK;
    }
    if ((Adc_GroupStatus[Group] != ADC_COMPLETED) &&
        (Adc_GroupStatus[Group] != ADC_STREAM_COMPLETED))
    {
        return E_NOT_OK;
    }
    #endif
 
    if (Adc_pGroupValue[Group] == NULL_PTR)
    {
        return E_NOT_OK;
    }
 
    pGroup = &Adc_pConfig->Configtool[Group];
 
    for (j = 0u; j < pGroup->numChannels; j++)
    {
        DataBufferPtr[j] = Adc_pGroupValue[Group][j];
    }
 
    if (pGroup->accessMode == ADC_ACCESS_MODE_SINGLE)
    {
        Adc_GroupStatus[Group] = ADC_IDLE;
    }
 
    return E_OK;
}
 
 
/*==================================================================================================
*  Adc_EnableHardwareTrigger
==================================================================================================*/
FUNC(void, ADC_CODE) Adc_EnableHardwareTrigger(VAR(Adc_GroupType, AUTOMATIC) Group)
{
    const Adc_GroupConfigType *pGroup = NULL_PTR;
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        return;
    }
    if (Group >= ADC_MAX_GROUPS)
    {
        return;
    }
    if (Adc_GroupStatus[Group] != ADC_IDLE)
    {
        return;
    }
    #endif
 
    /* Lấy pGroup TRƯỚC khi dùng để check trigger — tránh NULL dereference */
    pGroup = &Adc_pConfig->Configtool[Group];
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (pGroup->trigger == ADC_TRIGG_SRC_SW)
    {
        return;
    }
    #endif
 
    Adc_HwTriggerEnabled[Group] = TRUE;
    IP_ADC0->SC2 |= ADC_SC2_ADTRG_MASK;   /* |= không phải = để giữ các bit khác */
}
 
 
/*==================================================================================================
*  Adc_DisableHardwareTrigger
==================================================================================================*/
FUNC(void, ADC_CODE) Adc_DisableHardwareTrigger(VAR(Adc_GroupType, AUTOMATIC) Group)
{
    const Adc_GroupConfigType *pGroup = NULL_PTR;
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        return;
    }
    if (Group >= ADC_MAX_GROUPS)
    {
        return;
    }
    #endif
 
    /* Lấy pGroup TRƯỚC khi dùng để check trigger */
    pGroup = &Adc_pConfig->Configtool[Group];
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (pGroup->trigger == ADC_TRIGG_SRC_SW)
    {
        return;
    }
    #endif
 
    IP_ADC0->SC1[0]  = ADC_SC1_ADCH(0x1Fu);    /* dừng convert trước */
    IP_ADC0->SC2    &= ~ADC_SC2_ADTRG_MASK;
 
    Adc_HwTriggerEnabled[Group] = FALSE;
    Adc_GroupStatus[Group]      = ADC_IDLE;
}
 
 
/*==================================================================================================
*  Adc_EnableGroupNotification
==================================================================================================*/
FUNC(void, ADC_CODE) Adc_EnableGroupNotification(VAR(Adc_GroupType, AUTOMATIC) Group)
{
    const Adc_GroupConfigType *pGroup = NULL_PTR;
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        return;
    }
    if (Group >= ADC_MAX_GROUPS)
    {
        return;
    }
    #endif
 
    pGroup = &Adc_pConfig->Configtool[Group];
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (pGroup->NotificationPtr == NULL_PTR)
    {
        return;
    }
    #endif
 
    Adc_GroupNotification[Group] = TRUE;
}
 
 
/*==================================================================================================
*  Adc_DisableGroupNotification
==================================================================================================*/
FUNC(void, ADC_CODE) Adc_DisableGroupNotification(VAR(Adc_GroupType, AUTOMATIC) Group)
{
    const Adc_GroupConfigType *pGroup = NULL_PTR;
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        return;
    }
    if (Group >= ADC_MAX_GROUPS)
    {
        return;
    }
    #endif
 
    pGroup = &Adc_pConfig->Configtool[Group];
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (pGroup->NotificationPtr == NULL_PTR)
    {
        return;
    }
    #endif
 
    Adc_GroupNotification[Group] = FALSE;
}
 
 
/*==================================================================================================
*  Adc_GetGroupStatus
==================================================================================================*/
FUNC(Adc_StatusType, ADC_CODE) Adc_GetGroupStatus(VAR(Adc_GroupType, AUTOMATIC) Group)
{
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        return ADC_IDLE;   /* phải return giá trị, không phải return; */
    }
    if (Group >= ADC_MAX_GROUPS)
    {
        return ADC_IDLE;   /* phải return giá trị */
    }
    #endif
 
    return Adc_GroupStatus[Group];
}
 
 
/*==================================================================================================
*  Adc_GetVersionInfo
==================================================================================================*/
FUNC(void, ADC_CODE) Adc_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, ADC_APPL_DATA) versioninfo)
{
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (versioninfo == NULL_PTR)
    {
        return;
    }
    #endif
 
    versioninfo->module         = ADC_MODULE_ID;   /* moduleID không phải module */
    versioninfo->vendorID         = ADC_VENDOR_ID;
    versioninfo->sw_major_version = ADC_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = ADC_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = ADC_SW_PATCH_VERSION;
}
 
 
/*==================================================================================================
*  Adc_PreparePowerState
==================================================================================================*/
FUNC(Std_ReturnType, ADC_CODE) Adc_PreparePowerState(
    VAR(Adc_PowerStateType, AUTOMATIC) PowerState,
    P2VAR(Adc_PowerStateRequestResultType, AUTOMATIC, ADC_APPL_DATA) Result)
{
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        *Result = ADC_NOT_INIT;
        return E_NOT_OK;
    }
    if (PowerState != ADC_FULL_POWER)
    {
        *Result = ADC_POWER_STATE_NOT_SUPP;
        return E_NOT_OK;
    }
    #endif
 
    Adc_TargetPowerState   = PowerState;
    Adc_PowerStatePrepared = TRUE;
 
    *Result = ADC_SERVICE_ACCEPTED;
    return E_OK;
}
 
 
/*==================================================================================================
*  Adc_SetPowerState
==================================================================================================*/
FUNC(Std_ReturnType, ADC_CODE) Adc_SetPowerState(
    P2VAR(Adc_PowerStateRequestResultType, AUTOMATIC, ADC_APPL_DATA) Result)
{
    uint8 i;
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        *Result = ADC_NOT_INIT;
        return E_NOT_OK;
    }
    for (i = 0u; i < ADC_MAX_GROUPS; i++)
    {
        if (Adc_GroupStatus[i] == ADC_BUSY)
        {
            *Result = ADC_SEQUENCE_ERROR;
            return E_NOT_OK;
        }
    }
    if (Adc_PowerStatePrepared == FALSE)
    {
        *Result = ADC_SEQUENCE_ERROR;
        return E_NOT_OK;
    }
    #endif
 
    /* Dùng Adc_TargetPowerState (đích đến), không phải CurrentPowerState */
    if (Adc_TargetPowerState == ADC_FULL_POWER)
    {
        IP_PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_CGC_MASK;
    }
    else
    {
        IP_ADC0->SC1[0] = ADC_SC1_ADCH(0x1Fu);
        IP_PCC->PCCn[PCC_ADC0_INDEX] &= ~PCC_PCCn_CGC_MASK;
    }
 
    Adc_CurrentPowerState  = Adc_TargetPowerState;
    Adc_PowerStatePrepared = FALSE;
 
    *Result = ADC_SERVICE_ACCEPTED;
    return E_OK;
}
 
 
/*==================================================================================================
*  Adc_GetCurrentPowerState
==================================================================================================*/
FUNC(Std_ReturnType, ADC_CODE) Adc_GetCurrentPowerState(
    P2VAR(Adc_PowerStateType, AUTOMATIC, ADC_APPL_DATA) CurrentPowerState,
    P2VAR(Adc_PowerStateRequestResultType, AUTOMATIC, ADC_APPL_DATA) Result)
{
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        *Result = ADC_NOT_INIT;
        return E_NOT_OK;
    }
    #endif
 
    *CurrentPowerState = Adc_CurrentPowerState;
    *Result            = ADC_SERVICE_ACCEPTED;
    return E_OK;
}
 
 
/*==================================================================================================
*  Adc_GetTargetPowerState
==================================================================================================*/
FUNC(Std_ReturnType, ADC_CODE) Adc_GetTargetPowerState(
    P2VAR(Adc_PowerStateType, AUTOMATIC, ADC_APPL_DATA) TargetPowerState,
    P2VAR(Adc_PowerStateRequestResultType, AUTOMATIC, ADC_APPL_DATA) Result)
{
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        *Result = ADC_NOT_INIT;
        return E_NOT_OK;
    }
    #endif
 
    *TargetPowerState = Adc_TargetPowerState;
    *Result           = ADC_SERVICE_ACCEPTED;
    return E_OK;
}
 
 
/*==================================================================================================
*  Adc_GetStreamLastPointer
==================================================================================================*/
FUNC(Adc_StreamNumSampleType, ADC_CODE) Adc_GetStreamLastPointer(
    VAR(Adc_GroupType, AUTOMATIC) Group,
    P2VAR(Adc_ValueGroupType*, AUTOMATIC, ADC_APPL_DATA) PtrToSamplePtr)
{
    const Adc_GroupConfigType  *pGroup     = NULL_PTR;
    Adc_StreamNumSampleType     numSamples = 0u;
 
    #if(ADC_DEV_ERROR_DETECT == STD_ON)
    if (Adc_initsate == STD_OFF)
    {
        return 0u;
    }
    if (Group >= ADC_MAX_GROUPS)
    {
        return 0u;
    }
    if (PtrToSamplePtr == NULL_PTR)
    {
        return 0u;
    }
    if (Adc_GroupStatus[Group] != ADC_STREAM_COMPLETED)
    {
        *PtrToSamplePtr = NULL_PTR;
        return 0u;
    }
    #endif
 
    pGroup     = &Adc_pConfig->Configtool[Group];
    numSamples = (Adc_StreamNumSampleType)Adc_SampleCount[Group];
 
    if (numSamples > 0u)
    {
        *PtrToSamplePtr = &Adc_pGroupValue[Group][(numSamples - 1u) * pGroup->numChannels];
    }
    else
    {
        *PtrToSamplePtr = NULL_PTR;
    }
 
    return numSamples;
}
 
 
/*==================================================================================================
*  ADC0_IRQHandler
*  Chạy khi 1 channel convert xong (COCO flag set)
==================================================================================================*/
void ADC0_IRQHandler(void)
{
    const Adc_GroupConfigType *pGroup = NULL_PTR;
    Adc_ValueGroupType         result;
    uint8                      chIdx;
    uint8                      bufIdx;
 
    if (Adc_pConfig == NULL_PTR) { return; }
 
    pGroup = &Adc_pConfig->Configtool[Adc_ActiveGroup];   /* Configtool không phải co */
    chIdx  = Adc_CurrentChannelIdx[Adc_ActiveGroup];
 
    /* Đọc R[0] → tự clear COCO flag */
    result = (Adc_ValueGroupType)IP_ADC0->R[0];
 
    if (pGroup->resultAlignment == ADC_ALIGN_LEFT)
    {
        result = (Adc_ValueGroupType)(result << 4u);
    }
 
    /* Lưu kết quả */
    if (pGroup->accessMode == ADC_ACCESS_MODE_SINGLE)
    {
        if (Adc_pGroupValue[Adc_ActiveGroup] != NULL_PTR)   /* Adc_pGroupValue không phải Adc_ResultBufferPtr */
        {
            Adc_pGroupValue[Adc_ActiveGroup][chIdx] = result;
        }
    }
    else
    {
        bufIdx = (uint8)(Adc_SampleCount[Adc_ActiveGroup] * pGroup->numChannels) + chIdx;
        if (Adc_pGroupValue[Adc_ActiveGroup] != NULL_PTR)
        {
            Adc_pGroupValue[Adc_ActiveGroup][bufIdx] = result;
        }
    }
 
    Adc_CurrentChannelIdx[Adc_ActiveGroup]++;
 
    if (Adc_CurrentChannelIdx[Adc_ActiveGroup] < pGroup->numChannels)
    {
        /* Còn channel tiếp → start channel tiếp */
        chIdx = Adc_CurrentChannelIdx[Adc_ActiveGroup];
        IP_ADC0->SC1[0] = ADC_SC1_ADCH(pGroup->channelConfigPtr[chIdx].channelId)
                        | ADC_SC1_AIEN_MASK;
    }
    else
    {
        /* Hết channel trong group */
        Adc_CurrentChannelIdx[Adc_ActiveGroup] = 0u;
 
        if (pGroup->accessMode == ADC_ACCESS_MODE_STREAMING)
        {
            Adc_SampleCount[Adc_ActiveGroup]++;
            Adc_GroupStatus[Adc_ActiveGroup] = ADC_STREAM_COMPLETED;
        }
        else
        {
            Adc_GroupStatus[Adc_ActiveGroup] = ADC_COMPLETED;
        }
 
        /* Gọi notification — dùng Adc_GroupNotification (runtime flag) */
        if ((Adc_GroupNotification[Adc_ActiveGroup] == TRUE) &&
            (pGroup->NotificationPtr != NULL_PTR))
        {
            pGroup->NotificationPtr();
        }
 
        /* Continuous mode → restart */
        if (pGroup->convMode == ADC_CONV_MODE_CONTINUOUS)
        {
            Adc_GroupStatus[Adc_ActiveGroup] = ADC_BUSY;
            IP_ADC0->SC1[0] = ADC_SC1_ADCH(pGroup->channelConfigPtr[0].channelId)
                            | ADC_SC1_AIEN_MASK;
        }
    }
}
 