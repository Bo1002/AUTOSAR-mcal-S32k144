#ifndef MCAL_ADC_INCLUDE_ADC_CFG_H_
#define MCAL_ADC_INCLUDE_ADC_CFG_H_

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "../../../Common/Std_Type.h"
#include "Adc_Types.h"

/*==================================================================================================
*                              VENDOR / MODULE ID / SW VERSION
==================================================================================================*/
#define ADC_VENDOR_ID                          (0x0050u)
#define ADC_MODULE_ID                          (123u)      /* theo AUTOSAR BSW Module List */

#define ADC_SW_MAJOR_VERSION                   (1u)
#define ADC_SW_MINOR_VERSION                   (0u)
#define ADC_SW_PATCH_VERSION                   (0u)

#define ADC_AR_RELEASE_MAJOR_VERSION           (4u)
#define ADC_AR_RELEASE_MINOR_VERSION           (4u)
#define ADC_AR_RELEASE_REVISION_VERSION        (0u)

/*==================================================================================================
*                              DEVELOPMENT ERROR DETECTION
==================================================================================================*/
#define ADC_DEV_ERROR_DETECT                   (STD_ON)

/*==================================================================================================
*                              OPTIONAL API SWITCHES
==================================================================================================*/
#define ADC_VERSION_INFO_API                   (STD_ON)
#define ADC_DEINIT_API                         (STD_ON)
#define ADC_READ_GROUP_API                     (STD_ON)
#define ADC_ENABLE_START_STOP_GROUP_API        (STD_ON)
#define ADC_HW_TRIGGER_API                     (STD_ON)
#define ADC_GRP_NOTIF_CAPABILITY               (STD_ON)
#define ADC_GET_GROUP_STATUS_API               (STD_ON)
#define ADC_SET_HW_UNIT_POWER_MODE_API         (STD_OFF)   /* mini stack, chưa làm power mode */

/*==================================================================================================
*                              SERVICE ID (SID) — báo DET biết lỗi xảy ra ở API nào
==================================================================================================*/
#define ADC_SID_INIT                           (0x00u)
#define ADC_SID_DEINIT                         (0x01u)
#define ADC_SID_START_GROUP_CONVERSION         (0x02u)
#define ADC_SID_STOP_GROUP_CONVERSION          (0x03u)
#define ADC_SID_READ_GROUP                     (0x04u)
#define ADC_SID_ENABLE_HARDWARE_TRIGGER        (0x05u)
#define ADC_SID_DISABLE_HARDWARE_TRIGGER       (0x06u)
#define ADC_SID_ENABLE_GROUP_NOTIFICATION      (0x07u)
#define ADC_SID_DISABLE_GROUP_NOTIFICATION     (0x08u)
#define ADC_SID_GET_GROUP_STATUS               (0x09u)
#define ADC_SID_GET_STREAM_LAST_POINTER        (0x0Au)
#define ADC_SID_GET_VERSION_INFO               (0x0Bu)

/*==================================================================================================
*                              DET ERROR CODE (theo SWS_Adc — Error Classification)
==================================================================================================*/
#define ADC_E_PARAM_POINTER                    (0x0Au)
#define ADC_E_PARAM_GROUP                      (0x0Bu)
#define ADC_E_WRONG_CONV_MODE                  (0x0Cu)
#define ADC_E_WRONG_TRIGG_SRC                  (0x0Du)
#define ADC_E_NOTIF_CAPABILITY                 (0x0Eu)
#define ADC_E_BUFFER_UNINIT                    (0x0Fu)
#define ADC_E_ALREADY_INITIALIZED              (0x10u)
#define ADC_E_PARAM_CONFIG                     (0x11u)
#define ADC_E_UNINIT                           (0x12u)
#define ADC_E_BUSY                             (0x13u)
#define ADC_E_IDLE                             (0x14u)
#define ADC_E_NOT_DISENGAGED                   (0x15u)

/*==================================================================================================
*                              PHẦN CỨNG ADC TRÊN S32K144
*   S32K144 có 2 instance vật lý: ADC0, ADC1 — mỗi cái tối đa 16 channel (SE0..SE15)
==================================================================================================*/
#define ADC_HW_UNIT_0                          (0u)   /* ADC0 */
#define ADC_HW_UNIT_1                          (1u)   /* ADC1 */
#define ADC_MAX_HW_UNIT                        (2u)

#define ADC_MAX_GROUPS                         (1u)   /* tăng dần theo số group bạn thực sự cấu hình */
#define ADC_FULL_POWER                         (uint8)(1U)
/*==================================================================================================
*                              SYMBOLIC NAME — CHANNEL
*   TODO: thay số channel thật theo chân bạn nối (tra bảng SE_x trong S32K144 Reference Manual)
==================================================================================================*/
#define ADC_CHANNEL_POTENTIOMETER              (12u)  /* ví dụ: PTC14 = ADC0_SE12 */

/*==================================================================================================
*                              SYMBOLIC NAME — GROUP
==================================================================================================*/
#define ADC_GROUP_POTENTIOMETER                (0u)
#define ADC_GROUP_0                            (0u)
#define ADC_GROUP_1                            (1u)
#endif /* MCAL_ADC_INCLUDE_ADC_CFG_H_ */
