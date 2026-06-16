/*
 *  Port_Cfg.h
 *
 *  Created on: Jun 11, 2026
 *      Author: BO
 */

#ifndef MCAL_PORT_INLCUDE_PORT_CFG_H_
#define MCAL_PORT_INLCUDE_PORT_CFG_H_

#include "../../../Common/Std_Type.h"
#include "Port_Types.h"
/**
 * @brief version check  
 * 
 */
#define   PORT_VENDOR_ID            0x0050u
#define   PORT_MODULE_ID            0x0043u
#define   PORT_SW_MAJOR_VERSION     1u
#define   PORT_SW_MINOR_VERSION     0u
#define   PORT_SW_PATCH_VERSION     0u

/*******************************************************************************
 * 2. DEVELOPMENT ERROR DETECTION (DET)
 *
 * Giống Mcu driver: STD_ON trong development, STD_OFF trong production.
 * Khi bật: Port_Init(), Port_SetPinDirection()... sẽ check NULL pointer,
 * range validity, và báo Det_ReportError() nếu vi phạm.
 ******************************************************************************/
#define PORT_DEV_ERROR_DETECT    STD_ON
 
/*******************************************************************************
 * 3. OPTIONAL API SWITCHES
 *
 * AUTOSAR Port SWS phân chia API:
 *   Bắt buộc  : Port_Init(), Port_RefreshPortDirection()
 *   Tùy chọn  : Port_SetPinDirection(), Port_SetPinMode(),
 *               Port_GetVersionInfo()
 *
 * Tại sao SetPinDirection là tùy chọn?
 *   Nhiều pin không cần đổi hướng lúc runtime (LED luôn OUTPUT,
 *   CAN TX/RX luôn cố định). Nếu không enable API này, code gọi nó
 *   sẽ bị compile error → phát hiện sai sớm.
 *
 * Tại sao SetPinMode là tùy chọn?
 *   Hầu hết pin có mode cố định sau init. Chỉ cần khi có pin dual-function
 *   (ví dụ: pin dùng làm GPIO hoặc UART tùy runtime config).
 ******************************************************************************/
#define PORT_VERSION_INFO_API         STD_ON
#define PORT_SET_PIN_DIRECTION_API    STD_ON   /* Cho phép đổi direction runtime */
#define PORT_SET_PIN_MODE_API         STD_OFF  /* Mode cố định sau init          */

/*******************************************************************************
 * 4. PIN COUNT
 *
 * Tổng số pin được config trong project này.
 * Port_Init() duyệt qua đúng PORT_MAX_PIN_CONFIGS phần tử.
 *
 * S32K144 EVB pins dùng trong project:
 *   1. PTD0  — LED đỏ (OUTPUT, GPIO)
 *   2. PTD15 — LED Red (OUTPUT, GPIO)
 *   3. PTD16 — LED xanh lá (OUTPUT, GPIO)
 *   4. PTC12 — Nút SW2 (INPUT, GPIO, pull-up)
 *   5. PTE4  — CAN0_RX (INPUT,  ALT5)
 *   6. PTE5  — CAN0_TX (OUTPUT, ALT5)
 ******************************************************************************/
#define PORT_MAX_PIN_CONFIGS    6u

/*── LED pins (S32K144 EVB: RGB LED trên PORTD) ───────────────────────────────
 * PTD0  = Pin ID 3×32+0  = 96  → LED đỏ   (active LOW: 0=sáng, 1=tắt)
 * PTD15 = Pin ID 3×32+15 = 111 → LED Red
 * PTD16 = Pin ID 3×32+16 = 112 → LED xanh lá
 *──────────────────────────────────────────────────────────────────────────────*/
#define PORT_PIN_LED_BLUE        ((Port_PinType)96u)
#define PORT_PIN_LED_RED         ((Port_PinType)111u)
#define PORT_PIN_LED_GREEN       ((Port_PinType)112u)
 
/*── Button pins ──────────────────────────────────────────────────────────────
 * PTC12 = Pin ID 2×32+12 = 76  → SW2 (nút nhấn, active LOW)
 *──────────────────────────────────────────────────────────────────────────────*/
#define PORT_PIN_BTN_SW2        ((Port_PinType)76u)
 
/*── CAN pins (FlexCAN0 trên PORTE, ALT5 mode) ───────────────────────────────
 * PTE4 = Pin ID 4×32+4 = 132 → CAN0_RX
 * PTE5 = Pin ID 4×32+5 = 133 → CAN0_TX
 *──────────────────────────────────────────────────────────────────────────────*/
#define PORT_PIN_CAN0_RX        ((Port_PinType)132u)
#define PORT_PIN_CAN0_TX        ((Port_PinType)133u)


/*******************************************************************************
 * 6. PIN MODE DEFINES (MUX VALUES)
 *
 *   Port_PinModeType là uint8 — giá trị MUX ghi vào PCR[MUX] field (bits 10:8).
 *   STD_ON (=1) và GPIO (=1) trùng nhau về số nhưng hoàn toàn khác ngữ nghĩa:
 *     STD_ON = "tính năng này có bật không?"
 *     MUX=1  = "pin kết nối với GPIO buffer của chip"
 *
 * S32K144 PCR[MUX] mapping:
 *   0 = Disabled / Analog  → pin không có digital buffer (dùng cho ADC)
 *   1 = GPIO               → digital I/O thông thường
 *   2 = ALT2               → peripheral function 2 (tùy từng pin)
 *   3 = ALT3               → peripheral function 3
 *   4 = ALT4               → peripheral function 4
 *   5 = ALT5               → ví dụ: FlexCAN0 RX/TX trên PORTE4/5
 *   6 = ALT6               → peripheral function 6
 *   7 = ALT7               → peripheral function 7
 ******************************************************************************/
 
/* Generic MUX values */
#define PORT_PIN_MODE_ANALOG    ((Port_PinModeType)0u)  /* Analog, no digital buffer */
#define PORT_PIN_MODE_GPIO      ((Port_PinModeType)1u)  /* GPIO digital I/O          */
#define PORT_PIN_MODE_ALT2      ((Port_PinModeType)2u)  /* Alternate function 2      */
#define PORT_PIN_MODE_ALT3      ((Port_PinModeType)3u)  /* Alternate function 3      */
#define PORT_PIN_MODE_ALT4      ((Port_PinModeType)4u)  /* Alternate function 4      */
#define PORT_PIN_MODE_ALT5      ((Port_PinModeType)5u)  /* Alternate function 5      */
#define PORT_PIN_MODE_ALT6      ((Port_PinModeType)6u)  /* Alternate function 6      */
#define PORT_PIN_MODE_ALT7      ((Port_PinModeType)7u)  /* Alternate function 7      */
 
/* Alias theo chức năng — dễ đọc hơn khi config pin */
#define PORT_PIN_MODE_CAN       PORT_PIN_MODE_ALT5  /* FlexCAN0 trên PTE4/PTE5  */
#define PORT_PIN_MODE_ADC       PORT_PIN_MODE_ANALOG /* ADC input                */

/*******************************************************************************
 * 7. DET ERROR IDs
 *
 * Giống Mcu driver — giúp DET log biết lỗi gì, ở hàm nào.
 * Giá trị lấy từ AUTOSAR SWS Port Driver spec.
 ******************************************************************************/
#define PORT_E_PARAM_PIN                 0x0Au  /* Pin ID không hợp lệ             */
#define PORT_E_PARAM_CONFIG              0x0Bu  /* NULL config pointer             */
#define PORT_E_PARAM_INVALID_MODE        0x0Du  /* Mode value không hợp lệ        */
#define PORT_E_PARAM_INVALID_VALUE       0x0Eu  /* Giá trị direction không hợp lệ */
#define PORT_E_UNINIT                    0x0Fu  /* Gọi API trước Port_Init()       */
#define PORT_E_MODE_UNCHANGEABLE         0x03u  /* Pin không cho phép đổi mode     */
#define PORT_E_DIRECTION_UNCHANGEABLE    0x0Cu  /* Pin không cho phép đổi direction*/
 
/*******************************************************************************
 * 8. SERVICE IDs (cho Det_ReportError)
 ******************************************************************************/
#define PORT_SID_INIT                    0x00u  /* Port_Init()                */
#define PORT_SID_SET_PIN_DIRECTION       0x01u  /* Port_SetPinDirection()     */
#define PORT_SID_REFRESH_PORT_DIRECTION  0x02u  /* Port_RefreshPortDirection()*/
#define PORT_SID_GET_VERSION_INFO        0x03u  /* Port_GetVersionInfo()      */
#define PORT_SID_SET_PIN_MODE            0x04u  /* Port_SetPinMode()          */

#endif /* MCAL_DIO_INLCUDE_PORT_CFG_H_ */
