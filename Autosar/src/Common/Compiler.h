/*******************************************************************************
 * FILE        : Compiler.h
 * DESCRIPTION : AUTOSAR Compiler Abstraction Header File
 *               Abstracts compiler-specific keywords so AUTOSAR code is
 *               portable across different compilers without modification.
 *
 * SPEC        : AUTOSAR_CP_SWS_CompilerAbstraction R24-11
 * COMPILER    : GCC ARM (arm-none-eabi-gcc)
 * PLATFORM    : ARM Cortex-M4 | NXP S32K144
 *
 * WHY THIS FILE EXISTS:
 *   Different compilers use different keywords for the same thing:
 *     GCC:          __attribute__((always_inline)) inline
 *     GreenHills:   __inline
 *     IAR:          __inline
 *     TASKING:      inline
 *
 *   This file wraps those differences. AUTOSAR code always uses INLINE,
 *   never writes compiler-specific keywords directly.
 ******************************************************************************/

#ifndef COMPILER_H
#define COMPILER_H

#include "Compiler_Cfg.h"   /* Project-specific memory class definitions */

/*******************************************************************************
 * COMPILER VENDOR ID
 * Used for version checking. GCC = 3 (AUTOSAR vendor ID for GCC).
 ******************************************************************************/
#define COMPILER_VENDOR_ID      3u

/*******************************************************************************
 * INLINE KEYWORDS
 *
 * INLINE:
 *   Suggests the compiler to inline a function (no function call overhead).
 *   Used for small, performance-critical functions called very frequently.
 *   Example: Dio_ReadChannel() is typically INLINE.
 *
 * LOCAL_INLINE:
 *   Same as INLINE but for static functions (internal to a .c file).
 *   Prevents the compiler from generating an external symbol for the function.
 ******************************************************************************/
#define INLINE              inline
#define LOCAL_INLINE        static inline

/*******************************************************************************
 * FUNCTION DECLARATION MACROS
 *
 * Why not just write "uint8 Dio_ReadChannel()" directly?
 * Because some compilers need memory class qualifiers for ROM/RAM placement,
 * interrupt handlers, or DLL export. This macro keeps the syntax portable.
 *
 * Usage:
 *   FUNC(uint8, DIO_CODE) Dio_ReadChannel(Dio_ChannelType ChannelId);
 *
 *   Expands to (GCC):
 *   uint8 Dio_ReadChannel(Dio_ChannelType ChannelId);
 *
 * Parameters:
 *   rettype  = return type of the function (e.g. uint8, void, Std_ReturnType)
 *   memclass = memory class for code placement (e.g. DIO_CODE, MCU_CODE)
 *              Defined in Compiler_Cfg.h — can be empty on most platforms.
 ******************************************************************************/
#define FUNC(rettype, memclass)             rettype

/**
 * @brief Function returning pointer to constant data
 * Example: FUNC_P2CONST(uint8, AUTOMATIC, DIO_CODE) Dio_GetConfigPtr(void);
 */
#define FUNC_P2CONST(rettype, ptrclass, memclass)   const ptrclass rettype *

/*******************************************************************************
 * POINTER DECLARATION MACROS
 *
 * These look intimidating but the pattern is simple:
 *
 *   P2VAR   = Pointer TO a VARiable    (read/write pointer)
 *   P2CONST = Pointer TO a CONSTant    (read-only pointer, like const uint8*)
 *   CONSTP2VAR  = CONST Pointer to a Variable   (pointer itself is const)
 *   CONSTP2CONST = CONST Pointer to CONST data  (fully read-only)
 *
 * Parameters always follow this order:
 *   (pointed-to type,  memory class of the pointed data,  memory class of pointer)
 *
 * Example:
 *   P2VAR(uint8, AUTOMATIC, DIO_APPL_DATA) dataPtr
 *   Expands to: uint8 * dataPtr
 *   Means: pointer to uint8, data in automatic (stack) memory
 ******************************************************************************/

/** Pointer to variable (read/write) */
#define P2VAR(ptrtype, memclass, ptrclass)          ptrtype *

/** Pointer to constant (read-only data) */
#define P2CONST(ptrtype, memclass, ptrclass)        const ptrtype *

/** Constant pointer to variable (pointer address is fixed) */
#define CONSTP2VAR(ptrtype, memclass, ptrclass)     ptrtype * const

/** Constant pointer to constant data (fully immutable) */
#define CONSTP2CONST(ptrtype, memclass, ptrclass)   const ptrtype * const

/** Pointer to function */
#define P2FUNC(rettype, ptrclass, fctname)          rettype (* fctname)

/*******************************************************************************
 * CONST AND VAR DECLARATION MACROS
 *
 * CONST: Declares a constant value placed in ROM (Flash).
 *   Example: CONST(uint8, MCU_CONST) Mcu_MaxModes = 3u;
 *   Expands to: const uint8 Mcu_MaxModes = 3u;
 *
 * VAR: Declares a variable in RAM with a specific memory class.
 *   Example: VAR(uint8, MCU_VAR) Mcu_CurrentMode;
 *   Expands to: uint8 Mcu_CurrentMode;
 *
 * Why memclass? On some ECUs with banked memory, variables might need
 * to be placed in specific RAM sections via compiler pragmas. The memclass
 * handles that in Compiler_Cfg.h — for simple projects it's just empty.
 ******************************************************************************/

/** Constant in ROM */
#define CONST(consttype, memclass)      const consttype

/** Variable in RAM */
#define VAR(vartype, memclass)          vartype

/*******************************************************************************
 * MEMORY CLASS DEFAULTS
 * These are used when no specific placement is needed.
 * Defined here so code compiles even without a full Compiler_Cfg.h.
 ******************************************************************************/
#ifndef AUTOMATIC
#define AUTOMATIC           /* Variable on stack (local variable) */
#endif

#ifndef TYPEDEF
#define TYPEDEF             /* Used in typedef declarations       */
#endif

#endif /* COMPILER_H */

/*******************************************************************************
 * END OF FILE: Compiler.h
 ******************************************************************************/
