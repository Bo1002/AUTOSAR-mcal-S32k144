/*******************************************************************************
 * FILE        : Platform_Types.h
 * DESCRIPTION : AUTOSAR Platform Types Header File
 *               Defines all platform-dependent basic data types.
 *               This file is specific to: ARM Cortex-M4 | NXP S32K144
 *               Compiler: GCC ARM (arm-none-eabi-gcc)
 *
 * SPEC        : AUTOSAR_CP_SWS_PlatformTypes R20-11
 * PLATFORM    : ARM Cortex-M4 (32-bit, Little-Endian)
 *
 * NOTE: This file is the ONLY file allowed to use native C types
 *       (char, int, long...). All other AUTOSAR files must use
 *       types defined here (uint8, uint16, boolean, etc.)
 * AUTHOR      : BO
 ******************************************************************************/

#ifndef COMMON_PLATFORM_TYPES_H_
#define COMMON_PLATFORM_TYPES_H_

/**
 * @brief CPU character
 * 
 */
/*CPU 8-bit*/
#define CPU_TYPE_8   8u
/*CPU 16-bit*/ 
#define CPU_TYPE_16  16u
/*CPU 32-bit*/ 
#define CPU_TYPE_32  32u
/*CPU used S32k144 ARM-cortex M4 32-bit*/
#define CPU_TYPE     CPU_TYPE_32 

/**
 * @brief Bit order within a byte 
 * MSB_FIRST = bit 7 is the most significant bit 
 * LSB_FIRST = Bit 7 is the least significant bit
 * CPU_BIT_ORDER bit in used 
 */
#define MSB_FIRST       0u 
#define LSB_FIRST       1u
#define CPU_BIT_ORDER   LSB_FIRST

/**
 * @brief MSB within 16 bit wide data name byte1, LSB name Byte0
 * HIGH_BYTE_FIRST =  Big Endian byte
 *  LOW_BYTE_FIRST =  Little Endian
 */
#define HIGH_BYTE_FIRST 1u
#define LOW_BYTE_FIRST  0u
#define CPU_BYTE_ORDER  LOW_BYTE_FIRST

/*******************************************************************************
 * BOOLEAN TYPE
 * In AUTOSAR, boolean is explicitly 1 byte (uint8 underneath).
 * TRUE = 1, FALSE = 0. Do NOT use (1==1) style — always compare explicitly.
 *****************************************************************************/
#ifndef DEFINE_TYPEDEF_FOR_BOOOLEN
typedef unsigned char boolen;
#define DEFINE_TYPEDEF_FOR_BOOOLEN
#endif

#ifndef FLASE
#define FLASE ((boolen)0u)
#endif

#ifndef TRUE
#define TRUE ((boolen)1u)
#endif

/**
 * @brief unsigned interger type
 * 
 * ON Arm Cortex M4 with GCC
 * usigned char  = 8bit 
 * usigned short = 16bit
 * usigned int   = 32bit 
 * usinged long  = 64bit  
 * 
 */

 /**
  * @brief 8-bit used 0 ... 255, 0x00..0xFF
  * 
  */
typedef unsigned char           uint8;

/**
 * @brief 16-bit used 0..65535, 0x0000..0xFFFF
 * 
 */
typedef unsigned short          uint16;

/**
 * @brief 32-bit used 0..4294967295, 0x00000000..0xFFFFFFFF
 * 
 */
typedef unsigned int            uint32;

/**
 * @brief 64-bit used 
 * 
 */
typedef unsigned long long      uint64;

/*******************************************************************************
 * SIGNED INTEGER TYPES
 ******************************************************************************/


/** @brief Signed 8-bit integer  [-128 .. 127] */
typedef signed char              sint8;

/** @brief Signed 16-bit integer [-32768 .. 32767] */
typedef signed short             sint16;

/** @brief Signed 32-bit integer [-2147483648 .. 2147483647] */
typedef signed long              sint32;

/** @brief Signed 64-bit integer [-9223372036854775808 .. 9223372036854775807] */
typedef signed long long         sint64;

/*******************************************************************************
 * FLOATING POINT TYPES
 * Use only when absolutely necessary — floating point is discouraged in
 * safety-critical automotive software (ISO 26262).
 ******************************************************************************/
 
/** @brief 32-bit float (IEEE 754 single precision) */
typedef float                   float32;
 
/** @brief 64-bit float (IEEE 754 double precision) */
typedef double                  float64;

/*******************************************************************************
 * "LEAST" TYPES — Efficiency Hint Types
 *
 * Why do these exist?
 * On some CPUs, using a uint8 in a loop counter is SLOWER than uint32
 * because the CPU must sign-extend or mask on every operation.
 * "Least" types tell the compiler: "use AT LEAST this many bits,
 * but feel free to use a wider native type if it's faster."
 *
 * Example: uint8_least counter — might be uint8 or uint32 depending on CPU.
 * On ARM Cortex-M4: uint32 is the native word size, so _least = uint32.
 ******************************************************************************/
 
/** @brief At least 8-bit unsigned — use for loop counters, flags */
typedef unsigned int            uint8_least;
 
/** @brief At least 16-bit unsigned */
typedef unsigned int            uint16_least;
 
/** @brief At least 32-bit unsigned */
typedef unsigned int            uint32_least;
 
/** @brief At least 8-bit signed */
typedef signed int              sint8_least;
 
/** @brief At least 16-bit signed */
typedef signed int              sint16_least;
 
/** @brief At least 32-bit signed */
typedef signed int              sint32_least;

#endif /* COMMON_PLATFORM_TYPES_H_ */

/***********************************************************************************
 * END OF FILE : Platform.h
 * 
 ***********************************************************************************/