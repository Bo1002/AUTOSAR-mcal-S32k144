/*******************************************************************************
 * FILE        : Compiler_Cfg.h
 * DESCRIPTION : Compiler Configuration — Memory Class Definitions
 *               Defines memory class qualifiers for each BSW module.
 *               In simple projects these are empty (no special placement).
 *               In production, they map to linker section pragmas.
 *
 * SPEC        : AUTOSAR_CP_SWS_CompilerAbstraction R24-11
 * PLATFORM    : NXP S32K144
 *
 * HOW IT WORKS:
 *   Each module has its own set of memory classes, e.g. for Mcu driver:
 *     MCU_CODE     = code (functions) placed in Flash
 *     MCU_CONST    = constants placed in Flash (read-only data)
 *     MCU_VAR      = variables placed in RAM
 *     MCU_VAR_INIT = variables placed in initialized RAM section
 *
 *   For S32K144 with simple linker script, these are all empty —
 *   the linker puts everything in the right place automatically.
 *   In a production ECU with multiple memory banks, you would add
 *   __attribute__((section(".mcucode"))) etc. here.
 ******************************************************************************/

#ifndef COMPILER_CFG_H
#define COMPILER_CFG_H

/*******************************************************************************
 * MCU DRIVER MEMORY CLASSES
 ******************************************************************************/
#define MCU_CODE            /* Function code section    */
#define MCU_CONST           /* Constant data in Flash   */
#define MCU_VAR             /* Variable data in RAM     */
#define MCU_VAR_INIT        /* Initialized RAM variable */
#define MCU_VAR_ZERO_INIT   /* Zero-initialized RAM var */
#define MCU_APPL_DATA       /* Application data pointer */
#define MCU_APPL_CONST      /* Application const pointer*/
#define MCU_APPL_CODE       /* Application code pointer */

/*******************************************************************************
 * PORT DRIVER MEMORY CLASSES
 ******************************************************************************/
#define PORT_CODE
#define PORT_CONST
#define PORT_VAR
#define PORT_VAR_INIT
#define PORT_VAR_ZERO_INIT
#define PORT_APPL_DATA
#define PORT_APPL_CONST
#define PORT_APPL_CODE

/*******************************************************************************
 * DIO DRIVER MEMORY CLASSES
 ******************************************************************************/
#define DIO_CODE
#define DIO_CONST
#define DIO_VAR
#define DIO_VAR_INIT
#define DIO_VAR_ZERO_INIT
#define DIO_APPL_DATA
#define DIO_APPL_CONST
#define DIO_APPL_CODE

/*******************************************************************************
 * CAN DRIVER MEMORY CLASSES
 ******************************************************************************/
#define CAN_CODE
#define CAN_CONST
#define CAN_VAR
#define CAN_VAR_INIT
#define CAN_VAR_ZERO_INIT
#define CAN_APPL_DATA
#define CAN_APPL_CONST
#define CAN_APPL_CODE

/*******************************************************************************
 * ADC DRIVER MEMORY CLASSES
 ******************************************************************************/
#define ADC_CODE
#define ADC_CONST
#define ADC_VAR
#define ADC_VAR_INIT
#define ADC_VAR_ZERO_INIT
#define ADC_APPL_DATA
#define ADC_APPL_CONST
#define ADC_APPL_CODE


/*******************************************************************************
 * SPI DRIVER MEMORY CLASSES
 ******************************************************************************/

#define SPI_CODE
#define SPI_CONST
#define SPI_VAR
#define SPI_VAR_INIT
#define SPI_VAR_ZERO_INIT
#define SPI_APPL_DATA
#define SPI_APPL_CONST
#define SPI_APPL_CODE

#endif /* COMPILER_CFG_H */

/*******************************************************************************
 * END OF FILE: Compiler_Cfg.h
 ******************************************************************************/
