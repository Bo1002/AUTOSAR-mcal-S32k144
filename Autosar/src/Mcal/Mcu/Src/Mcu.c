/*
 * Mcu.c
 *
 *  Created on: Jun 4, 2026
 *      Author: BO
 */


#include "Mcu.h"
#include <S32K144.h>
#include "Mcu_Cfg.h"
// #include "core_cm4.h"

/* FIRC_CLK, SIRC_CLK, SOSC_CLK, SPLL_CLK*/

/* RCCR[27:24] = SCS: chọn clock source cho Run mode
 *   0x1 = SOSC (external crystal)
 *   0x3 = FIRC (fast internal RC, ~48MHz, không cần crystal)
 *   0x6 = SPLL (PLL output — dùng khi cần tốc độ cao)      */
#define SCG_RCCR_SCS_FIRC           (3UL << 24u)
#define SCG_RCCR_SCS_SPLL           (6UL << 24u)
 

 
/*──────────────────────────────────────────────────────────────────────────────
 * ARM SCB — System Control Block (base: 0xE000ED00, ARM Cortex-M4 core)
 *
 * AIRCR (Application Interrupt and Reset Control Register):
 *   Bits [31:16] = VECTKEY: phải write 0x5FA để lệnh được chấp nhận.
 *                  Đây là cơ chế bảo vệ — tránh vô tình reset.
 *   Bit  [2]     = SYSRESETREQ: 1 = yêu cầu system reset
 *   AIRCR     Appication Interupte and control register
 * Lý do cần VECTKEY: ARM thiết kế AIRCR với vectkey để write được chấp nhận.
 * Nếu không có 0x5FA ở bits [31:16], write vào bit SYSRESETREQ bị ignore.
 *────────────────────────────────────────────────────────────────────────────*/
#define MCU_SCB_AIRCR_REG               (*((volatile uint32*)0xE000ED0CUL))

#define SCB_AIRCR_VECTKEY_POS           16u
#define MCU_SCB_AIRCR_VECTKEY           (0x05FAUL << SCB_AIRCR_VECTKEY_POS)
#define MCU_SCB_AIRCR_SYSRESETREQ_MASK  (1UL << 2u)

#define STANDBY() __asm volatile ("wfi")

/* Data Synchronization Barrier — đảm bảo write hoàn tất trước khi reset */
#define MCU_DSB()   __asm volatile ("dsb" : : : "memory")


/**
 * @brief check if state re on 
 * 
 */
static  uint8 Mcu_InitSate = STD_ON;



const Mcu_ConfigType* Mcu_ConFig = NULL_PTR;

/**
 * @brief to get clock state current 
 * 
 */
const Mcu_ClockTypes* Mcu_ClockCurrentSetting = 0u;


/** 
 * @brief Init 
 */

static void Mcu_EnablePeripClock(uint8 PccIndex)
{  
    IP_PCC->PCCn[PccIndex] |= PCC_PCCn_CGC_MASK;
}

static void Mcu_EnableAllPeripheralClock(void)
{

    #if (MCU_PCC_PORTA_ENABLE == STD_ON)
    Mcu_EnablePeripClock(PCC_PORTA_INDEX);
    #endif
    #if (MCU_PCC_PORTB_ENABLE == STD_ON)
    Mcu_EnablePeripClock(PCC_PORTB_INDEX);
    #endif
    #if (MCU_PCC_PORC_ENABLE == STD_ON)
    Mcu_EnablePeripClock(PCC_PORTB_INDEX);
    #endif
    #if (MCU_PCC_PORTD_ENABLE == STD_ON)
    Mcu_EnablePeripClock(PCC_PORTB_INDEX);
    #endif
    #if (MCU_PCC_PORTE_ENABLE == STD_ON)
    Mcu_EnablePeripClock(PCC_PORTB_INDEX);
    #endif
    /**
     * @brief COMMUNICATION Perip
     * 
     */
    #if (MCU_PCC_FLEXCAN0_ENABLE == STD_ON)
    Mcu_EnablePeripClock(PCC_FlexCAN0_INDEX);
    #endif

    #if (MCU_PCC_ADC0_ENABLE == STD_ON)
    Mcu_EnablePeripClock(PCC_ADC0_INDEX);
    #endif

    /**
     * @brief system management 
     * 
     */
    #if (MCU_PCC_ADC0_ENABLE == STD_ON)
    Mcu_EnablePeripClock(PCC_ADC0_INDEX);
    #endif

    #if (MCU_PCC_LPUART0_ENABLE == STD_ON)
    Mcu_EnablePeripClock(PCC_LPUART0_INDEX);
    #endif
}


FUNC(void,MCU_CODE) Mcu_Init(P2CONST(Mcu_ConfigType, AUTOMATIC, MCU_APPL_CONST) ConfigPtr)
{
    #if (MCU_DEV_ERROR_DETECT == STD_ON)
        if(ConfigPtr == NULL_PTR)
        {
            /**
             * @brief implement later 
             * 
             */

            return;
        }

    #endif
    
    Mcu_ConFig = ConfigPtr;

    Mcu_InitSate = STD_ON;
}



FUNC(Std_ReturnType,MCU_CODE) Mcu_InitClock (VAR(Mcu_ClockTypes,MCU_APPL_DATA) ClockSetting)
{
    const Mcu_ClockConfigType* Mcu_ClkConfig;
    Std_ReturnType retVal = E_OK;
    
    #if (MCU_DEV_ERROR_DETECT == STD_ON)
        if( Mcu_Init == STD_OFF )
        {
            /**
             * @brief implement later 
             * 
             */

            return E_NOT_OK;
        }
        if(ClockSetting >= Mcu_ConFig->numClockConfigs )
        {

            return E_NOT_OK;
        }
    #endif

    Mcu_ClkConfig = &Mcu_ConFig->ClockConfigPtr[ClockSetting];

    if(ClockSetting == MCU_CLOCKSETTING_NORMALRUN)
    {
        /*8Mhz */
        IP_SCG->SOSCCSR &= ~SCG_SOSCCSR_SOSCEN_MASK;

        IP_SCG->SOSCCFG = SCG_SOSCCFG_RANGE(Mcu_ClkConfig->soscRange) 
                          | SCG_SOSCCFG_EREFS(1);

        IP_SCG->SOSCCSR |= SCG_SOSCCSR_SOSCEN_MASK;

        /* wait for it optimise*/
        while(!(IP_SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK)); 
        /*SPLL 112Mhz*/
        IP_SCG->SPLLCSR &= ~SCG_SPLLCSR_SPLLEN_MASK;

        IP_SCG->SPLLCFG = SCG_SPLLCFG_PREDIV(Mcu_ClkConfig->spllPrediv) 
                          | SCG_SPLLCFG_SOURCE(0x01) 
                          | SCG_SPLLCFG_MULT(Mcu_ClkConfig->spllMult);

        IP_SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK;

        /**
         * @brief RCCR 
         * 
         */
        /*RCCR now not select source fre so set sysClk, let ClockPLLdistribute handle it */
        IP_SCG->RCCR = SCG_RCCR_SCS(Mcu_ClkConfig->sysClkSrc)
                       | SCG_RCCR_DIVBUS(Mcu_ClkConfig->busDivider - 1)
                       | SCG_RCCR_DIVCORE(Mcu_ClkConfig->coreDivider -1)
                       | SCG_RCCR_DIVSLOW(Mcu_ClkConfig->flashDivider -1);
    }

    else if(ClockSetting ==  MCU_CLOCKSETTING_LOWPOWER)
    {
        /**
         * @brief for when car in idle mode
         * 
         */
        IP_SCG->RCCR = SCG_RCCR_SCS_FIRC 
                       | SCG_RCCR_DIVBUS(0)
                       | SCG_RCCR_DIVCORE(1)
                       | SCG_RCCR_DIVSLOW(1); 
    }

    else 
    {
        retVal = E_NOT_OK;
    }

    if(retVal == E_OK)
    {

        Mcu_ClockCurrentSetting = ClockSetting;

        Mcu_EnableAllPeripheralClock();
    }
    return retVal;
}



FUNC(Std_ReturnType,MCU_CODE) Mcu_DistributePllClock (void)
{
    #if(MCU_DEV_ERROR_DETECT  == STD_ON)
        if(Mcu_Init == STD_OFF)
        {
            return;
        }
        
        if(!(IP_SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK))
        {
            return;
        }
    #endif
    
    IP_SCG->RCCR = (IP_SCG->RCCR & ~SCG_RCCR_SCS_MASK)
                    | SCG_RCCR_SCS_SPLL;
}



FUNC(Mcu_PllStatusType, MCU_CODE) Mcu_GetPllStatus (void)
{
    Mcu_PllStatusType PLLtype;

    if(!(IP_SCG->SPLLCSR & SCG_SPLLCSR_SPLLEN_MASK))
    {
        PLLtype = MCU_PLL_STATUS_UNDEFINED;
    }

    else if (!(IP_SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK))
    {
        PLLtype = MCU_PLL_LOCKED;
    }

    else 
    {
        PLLtype = MCU_PLL_UNLOCKED;
    }

    return PLLtype;
}



FUNC(Mcu_ResetType,MCU_CODE) Mcu_GetResetReson(void)
{   
    Mcu_ResetType reason; 
    Mcu_RawResetType srs = IP_RCM->SRS;

    /*power on reset*/
    if((srs & RCM_SRS_POR_MASK))
    {
        
        reason = MCU_POWER_ON_RESET;
    }
    /*RESET by external pin, ex reset pin*/
    else if ((srs & RCM_SRS_PIN_MASK))
    {
        reason = MCU_EXT_RESET;
    }

    else if(srs & RCM_SRS_WDOG_MASK)
    {
        reason = MCU_WATCHDOG_RESET;
    }

    else if(srs & RCM_SRS_LOCKUP_MASK)
    {
        reason = MCU_LOCKUP_RESET; /* ARM Cortex-M4 CPU lockup   [S32K144]*/
    }

    else if (srs & RCM_SRS_SW_MASK)
    {
        reason = MCU_SW_RESET;
    }

    else 
    {
        reason = MCU_RESET_UNDEFINED;
    }

    return reason;
}

FUNC(Mcu_RawResetType,MCU_CODE) Mcu_GetResetRawValue(void)
{
    return (Mcu_RawResetType)IP_RCM->SRS;
}

#if(MCU_PERFORM_RESET_API  == STD_ON)


FUNC(void, MCU_CODE) Mcu_PerformReset(void)
{
    /* Ghi VECTKEY | SYSRESETREQ vào AIRCR
     * Direct register access — không cần CMSIS SCB struct*/
    MCU_SCB_AIRCR_REG = MCU_SCB_AIRCR_VECTKEY | MCU_SCB_AIRCR_SYSRESETREQ_MASK;

    /* DSB — đảm bảo write hoàn tất trước khi pipeline fetch tiếp*/
    MCU_DSB();

    /* Infinite loop — CPU reset trước khi đến đây */
    while (1u) {}     
}

#endif



FUNC(void, MCU_CODE) Mcu_SetMode(
    VAR(Mcu_ModeType, AUTOMATIC) McuMode)
{
    uint8 hwMode;
 
#if (MCU_DEV_ERROR_DETECT == STD_ON)
    if (Mcu_InitSate == STD_OFF)
    {
        /* Det_ReportError(MCU_MODULE_ID, 0u, MCU_SID_SET_MODE, MCU_E_UNINIT); */
        return;
    }
#endif
 
    /* Validate range: McuMode phải < số mode trong config */
    if (McuMode >= Mcu_ConFig->numModeConfigs)
    {
#if (MCU_DEV_ERROR_DETECT == STD_ON)
        /* Det_ReportError(MCU_MODULE_ID, 0u, MCU_SID_SET_MODE, MCU_E_PARAM_MODE); */
#endif
        return;
    }
 
    /* Lookup hardware value từ config table.
     *
     * McuMode (ví dụ: 1) → Mcu_pConfig->modeConfigPtr[1].hwModeValue
     * → hwMode = 0x02 (STOP mode trên SMC register)
     *
     * WHY không hardcode: config table trong Mcu_Cfg.c quyết định mapping.
     * Để thay đổi mapping, chỉ cần sửa config, không sửa code ở đây.    */
    hwMode = Mcu_ConFig->Mode[McuMode].hwModeValue;
 
    if (McuMode == MCU_MODE_ID_RUN)
    {
        /* RUN mode: thay đổi RUNM field trong SMC_PMCTRL
         * RUNM[23:22]: 00=Normal RUN, 10=VLPR (Very Low Power Run)
         * Clear RUNM bits cũ, sau đó OR với hwMode mới              */
        IP_SMC->PMCTRL = (IP_SMC->PMCTRL & ~SMC_PMCTRL_RUNM_MASK)
                    | SMC_PMCTRL_RUNM(hwMode);
    }
    else
    {
        /* STOP hoặc VLPS: thay đổi STOPM field
         * STOPM[2:0]: 000=Normal STOP, 010=STOP (full), 110=VLPS
         * Sau khi set, thực thi WFI để CPU thực sự vào sleep mode   */
        IP_SMC->PMCTRL = (IP_SMC->PMCTRL & ~SMC_PMCTRL_STOPM_MASK)
                    | SMC_PMCTRL_STOPM(hwMode);
 
        /* WFI — Wait For Interrupt
         * ARM Cortex-M4 instruction: CPU dừng execute, clock bị gate.
         * CPU tỉnh lại khi: interrupt pending, debug event, hoặc NMI.
         * volatile đảm bảo compiler không optimize bỏ lệnh này.     */
        __asm volatile ("wfi");
 
        /* Code tiếp tục ở đây sau khi CPU wakeup từ interrupt.
         * Caller cần setup interrupt source trước khi gọi SetMode.  */
    }
}


#if (MCU_VERSION_INFO_API == STD_ON)
/**
 * @brief   Mcu_GetVersionInfo — Trả về thông tin version module [SWS_Mcu_00204]
 *
 * @details Điền các field version vào struct do caller cung cấp.
 *          Dùng để kiểm tra compatibility khi tích hợp nhiều module cùng nhau.
 *
 *
 * @param[out] versioninfo   Caller cấp phát struct, hàm này điền vào.
 *             P2VAR(..., AUTOMATIC, MCU_APPL_DATA):
 *               AUTOMATIC = pointer nằm trên stack caller
 *               MCU_APPL_DATA = struct nằm trong RAM của caller
 */
FUNC(void, MCU_CODE) Mcu_GetVersionInfo(
    P2VAR(Std_VersionInfoType, AUTOMATIC, MCU_APPL_DATA) versioninfo)
{
#if (MCU_DEV_ERROR_DETECT == STD_ON)
    if (versioninfo == NULL_PTR)
    {
        /* Det_ReportError(MCU_MODULE_ID, 0u, MCU_SID_GET_VERSION_INFO, MCU_E_PARAM_CONFIG); */
        return;
    }
#endif
 
    /* Điền từng field từ constant defines trong Mcu_Cfg.h */
    versioninfo->vendorID         = MCU_VENDOR_ID;
    versioninfo->module           = MCU_MODULE_ID;
    versioninfo->sw_major_version = MCU_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = MCU_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = MCU_SW_PATCH_VERSION;
}
#endif /* MCU_VERSION_INFO_API */
