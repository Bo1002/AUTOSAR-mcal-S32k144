/*******************************************************************************
 * FILE        : Mcu.h
 * DESCRIPTION : AUTOSAR Mcu Driver — Public API Header
 *               API declarations cho tất cả function của Mcu driver.
 *               Đây là file mà các module khác (EcuM, Port, Can...) include
 *               để gọi Mcu services.
 *
 * SPEC        : AUTOSAR_CP_SWS_MicrocontrollerDrivers R24-11
 * PLATFORM    : NXP S32K144 (ARM Cortex-M4)
 ******************************************************************************/
 

#ifndef MCAL_MCU_INCLUDE_MCU_H_
#define MCAL_MCU_INCLUDE_MCU_H_

#include "Mcu_Types.h"
#include "Compiler.h"
#include "Mcu_Cfg.h"


FUNC(void,MCU_CODE) Mcu_Init(P2CONST(Mcu_ClockConfigType, AUTOMATIC, MCU_APPL_CONST) ConfigPtr);



FUNC(Std_ReturnType,MCU_CODE) Mcu_InitRamSection (VAR(Mcu_RamSectionType,MCU_APPL_DATA) RamSection);


FUNC(Std_ReturnType,MCU_CODE) Mcu_InitClock (VAR(Mcu_ClockTypes,MCU_APPL_DATA) ClockSetting);


FUNC(Std_ReturnType,MCU_CODE) Mcu_DistributePllClock (void);


FUNC(Mcu_PllStatusType, MCU_CODE) Mcu_GetPllStatus (void);


FUNC(Mcu_ResetType,MCU_CODE) Mcu_GetResetReson(void); 


FUNC(Mcu_RawResetType,MCU_CODE) Mcu_GetResetRawValue(void); 

#if (MCU_PERFORM_RESET_API == STD_ON)
/**
 * @brief   Thực hiện software reset toàn bộ MCU
 * @details Ghi vào SCB->AIRCR (ARM Cortex-M4 System Reset Request).
 *          Dùng khi phát hiện lỗi nghiêm trọng không thể recover.
 *          Hàm này KHÔNG bao giờ return.
 *
 * @spec [SWS_Mcu_00159]
 */
FUNC(void, MCU_CODE) Mcu_PerformReset(void);
#endif



FUNC(void, MCU_CODE) Mcu_SetMode (VAR(Mcu_ModeType, MCU_APPL_DATA) McuMode); 


#if (MCU_VERSION_INFO_API == STD_ON)
/**
 * @brief   Lấy thông tin version của Mcu module
 * @details Điền vào struct Std_VersionInfoType: vendorID, moduleID,
 *          sw_major/minor/patch_version.
 *          Dùng cho compatibility check khi integrate module.
 *
 * @param[out] versioninfo  Pointer đến struct sẽ được điền version info.
 *                          Không được là NULL_PTR.
 *
 * @spec [SWS_Mcu_00204]
 */
FUNC(void, MCU_CODE) Mcu_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, MCU_APPL_DATA) versioninfo
);
#endif



FUNC(Mcu_RamStateType,MCU_CODE) Mcu_GetRamSate (void);


#endif /* MCAL_MCU_INCLUDE_MCU_H_ */
