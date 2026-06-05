/*FILE: Mcu_Types.h
 *DESCRIPTION: AUTOSAR Mcu Driver — Type Definitions
 *             All data types specific to the Mcu module.
 *             No API declarations here — only types.
 * SPEC        : AUTOSAR_CP_SWS_MicrocontrollerDrivers R24-11
 * PLATFORM    : NXP S32K144 (ARM Cortex-M4)
 *  Created on: Jun 4, 2026
 *  Author: BO
 */

#ifndef MCAL_MCU_INCLUDE_MCU_TYPES_H_
#define MCAL_MCU_INCLUDE_MCU_TYPES_H_

/***************************************************
 * *********         INCLUDE 
 ***************************************************/
#include "Std_Type.h"

/***********************************************************************************************************
 * @brief Mcu clock type 
 * @details  The type shall be chosen depending on MCU platform (ID) for best performance in Mcu_ConfigType
 * 
 * @Range: 0.. (MCU_MAX_CLOCK_SETTINGS - 1)
 * @spec: [SWS_Mcu_00251]
 **********************************************************************************************************/

typedef  uint8     Mcu_ClockTypes;

/***********************************************************************************************************
 * @brief Mcu reset type 
 * @details containing the subset of reset type not required that
            all reset types are supported by hardware
 *  @Spec: [SWS_Mcu_00252]
 **********************************************************************************************************/
typedef enum
{
    MCU_POWER_ON_RESET  = 0x00u, /*(0x00) Power On Reset (default) */
    MCU_WATCHDOG_RESET  = 0x01u, /*(0x01) Internal Watchdog Timer Reset*/
    MCU_SW_RESET        = 0x02u, /*(0x02) Software ResetT*/    
    MCU_RESET_UNDEFINED = 0x03u, /*(0x03) Reset is undefined*/
    MCU_EXT_RESET       = 0x04u, /* External RESET pin         [S32K144]*/
    MCU_LOCKUP_RESET    = 0x05u  /* ARM Cortex-M4 CPU lockup   [S32K144]*/
}Mcu_ResetType;



/*******************************************************************************
 * Mcu_PllStatusType
 *
 * WHY: PLL cần thời gian để lock sau khi được cấu hình.
 *      Mcu_GetPllStatus() trả về type này để caller biết
 *      PLL đã sẵn sàng chưa trước khi gọi Mcu_DistributePllClock().
 *
 * Trên S32K144: check bit SPLLVLD trong SCG->SPLLCSR
 ******************************************************************************/
typedef enum
{
    MCU_PLL_LOCKED            = 0x00u, /* PLL stable, ready to distribute  */
    MCU_PLL_UNLOCKED          = 0x01u, /* PLL still locking, wait more     */
    MCU_PLL_STATUS_UNDEFINED  = 0x02u  /* PLL not configured or no PLL HW  */
} Mcu_PllStatusType;





/***********************************************************************************************************
 * @brief Mcu raw reset 
 * @details The type shall be chosen depending on MCU platform for best performance.
 *
 *  @Spec: [SWS_Mcu_00253]
 **********************************************************************************************************/
typedef        uint8   Mcu_RawResetType;


/**************************************************************
 * @brief  Mcu mode type
 * @details specifies the identification (ID) for a MCU mode
 * @Range: 0... (ID-1)
 * @spec: [SWS_Mcu_00254]  
 */
typedef         uint8        Mcu_ModeType;


/************************************************************************************************************
 * @brief Ram section
 * @details chose ID suitable for MCU platform 
 * @Range: 0... (ID-1)
 * @spec: [SWS_Mcu_00255] 
 *************************************************************************************************************/

typedef         uint8    Mcu_RamSectionType;  



/**
 * @brief Ram data return 
 * @details  Ram State data type returned by the function Mcu_GetRamState of the Mcu module
 * @spec: [SWS_Mcu_00256]
 */

typedef enum
{
    MCU_RAMSTATE_INVALID, /*(0x00) Ram content is not valid or unknown (default */
    MCU_RAMSTATE_VALID    /*(0x01) Ram content is valid*/
}Mcu_RamStateType;



/*******************************************************************************
 * Mcu_ClockConfigType
 *
 * WHY: Chứa toàn bộ tham số để cấu hình clock tree của S32K144.
 *      Trong dự án thật, struct này được tool generate từ ARXML.
 *      Ở đây mình define thủ công cho S32K144.
 *
 * S32K144 Clock Tree:
 *   SOSC (8MHz external) → SPLL → System Clock (112MHz max)
 *   Core Clock  = System Clock / coreDivider
 *   Bus Clock   = System Clock / busDivider
 *   Flash Clock = System Clock / flashDivider
 ******************************************************************************/
typedef struct
{
    /* SOSC (System Oscillator) — thạch anh ngoài */
    uint8  soscFreqMHz;      /* Tần số thạch anh: thường 8MHz trên S32K144  */
    uint8  soscRange;        /* SCG_SOSCCFG_RANGE: 1=Low, 2=Mid, 3=High     */
 
    /* SPLL (System PLL) */
    uint8  spllPrediv;       /* PLL Pre-divider:  SCG_SPLLCFG_PREDIV field  */
    uint8  spllMult;         /* PLL Multiplier:   SCG_SPLLCFG_MULT  field   */
    /* VCO_CLK = SOSC / (prediv+1) * (mult+16)                              */
    /* SPLL_CLK = VCO_CLK / 2                                               */
    /* Example: 8MHz / 1 * 28 / 2 = 112MHz                                  */
 
    /* System Clock Dividers */
    uint8  coreDivider;      /* Core clock divider (DIVCORE): 1..16          */
    uint8  busDivider;       /* Bus clock divider  (DIVBUS):  1..16          */
    uint8  flashDivider;     /* Flash clock divider(DIVFLASH):1..16          */
 
    /* Clock source selection for Run Mode */
    uint8  sysClkSrc;        /* SCG_RCCR_SCS: 1=SOSC, 3=FIRC, 6=SPLL       */
} Mcu_ClockConfigType;




/*******************************************************************************
 * Mcu_ModeConfigType
 *
 * WHY: Bảng cấu hình cho từng power mode.
 *      Mcu_SetMode(modeId) sẽ dùng modeId làm index vào bảng này
 *      để biết cần set SMC register thành giá trị nào.
 *
 *      hwModeValue map Mcu_ModeType ID → giá trị thật của SMC_PMCTRL register.
 *      Ví dụ: modeId=0 → hwModeValue=0x00 (RUN)
 *             modeId=1 → hwModeValue=0x02 (STOP)
 *             modeId=2 → hwModeValue=0x06 (VLPS)
 ******************************************************************************/
typedef struct
{
    Mcu_ModeType mcuModeId;   /* ID của mode này (index trong bảng config) */
    uint8        hwModeValue; /* Giá trị tương ứng ghi vào SMC_PMCTRL      */
} Mcu_ModeConfigType;




/***********************************************************************************************************
 * @brief Mcu configuration 
 * @details: A pointer to such a structure is provided to the MCU initialization routines for configuration
 * 
 * @spec: [SWS_Mcu_00249]
 **********************************************************************************************************/
typedef struct
{
    const Mcu_ClockConfigType* ClockConfigPtr;
    /*amount of clock congfig*/
    uint8 numClockConfigs;

    const Mcu_ModeConfigType* Mode;

    uint8 numModeConfigs;
}Mcu_ConfigType; 


#endif /* MCAL_MCU_INCLUDE_MCU_TYPES_H_ */
