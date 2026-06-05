
/*******************************************************************************
 * FILE        : Std_Types.h
 * DESCRIPTION : AUTOSAR Standard Types Header File
 *               Contains all types used across BSW modules that are
 *               platform and compiler independent.
 *
 * SPEC        : AUTOSAR_CP_SWS_StandardTypes R24-11 (Document ID 49)
 * VERSION     : R24-11
 * PLATFORM    : NXP S32K144 (ARM Cortex-M4)
 *
 * RULES:
 *   [SWS_Std_00004] - No project/supplier specific extensions allowed
 *   [SWS_Std_00014] - Protected against multiple inclusion
 ******************************************************************************/
#ifndef STD_TYPDE_H
#define STD_TYPDE_H

#include "Platform_Types.h"
#include "Compiler.h"


/*******************************************************************************
 * SECTION 8.1 - TYPE DEFINITIONS
 ******************************************************************************/
 
/**
 * @brief   Standard Return Type
 * @details Used as standard API return type shared between RTE and BSW modules.
 *          Bit[6..7] reserved by RTE. Bit[0..5] available for user errors.
 *
 * @spec    [SWS_Std_00005] - typedef uint8 Std_ReturnType
 * @spec    [SWS_Std_00011] - Layout: 6 user bits + 2 RTE reserved bits
 */
typedef uint8 Std_ReturnType;

/**
 * @brief   Standard Version infor type 
 * @details used to request the version of BSW module using <module name>_GetVersionIfo() 
 * 
 * @spec:   [SWS_Std_00015] - 
 */

typedef struct 
{
    uint8  sw_patch_version; 
    uint8  sw_minor_version;
    uint8  sw_major_version;
    uint16 module;
    uint16 vendorID;
} Std_VersionInfoType;

/**
 * @brief   Transformer Error Code Type
 * @details Underlying type for transformer error codes.
 *          Specific values defined per transformer class in ASWS_TransformerGeneral.
 *
 * @spec    [SWS_Std_00022]
 */
typedef uint8 Std_TransformerErrorCode;
 
/**
 * @brief   Transformer Class Type
 * @details Identifies which transformer class an error belongs to.
 *
 * @spec    [SWS_Std_00023] [SWS_Std_00024]
 */
typedef uint8 Std_TransformerClass;
 
/**
 * @brief   Transformer Error Structure
 * @details Combines error code with the transformer class that generated it.
 *
 * @spec    [SWS_Std_00021]
 */
typedef struct
{
    Std_TransformerErrorCode errorCode;        /**< Error code value           */
    Std_TransformerClass     transformerClass; /**< Class that raised the error */
} Std_TransformerError;


/**
 * @brief Standard transformer status code
 * @spec: [SWS_Std_00028] 
 * 
 */
typedef uint8   Std_TransformerForwardCode;


/**
 * @brief Repesent a forward transformer status in the context of a certain transformer, it seenable by specific transformer chain
 * @spec: [SWS_Std_00027]
 */

typedef struct 
{
    Std_TransformerForwardCode errorCode; 
    Std_TransformerClass       transformerClass; 
}Std_TransformerForward;



typedef uint8 Std_MessageTypeType;
 

/**
 * @brief Standard message result
 * @details Encode dif type for response message.
 * 
 * Range: 
 * STD_MESSAGERRESULT_OK    0X00   -  STD_MESSAGERRESULT_OK
 * STD_MESSAGERESULT_ERROR  0X01   - Messageresult  for an ERROR response   
 * 0x02 - 0x3F - Reservered for future message result
 * 
 * @spec: [SWS_Std_91002]
 */
 typedef uint8      Std_MessageResultType;




/**
 * @brief Extract Protocol Header Fields 
 * @details type fucntion pointer extract to protocal heade fields of the message and the type of the message result of a transformer
 * 
 * @spec:  [SWS_Std_91003]
 * 
 * @param[in]  buffer        Buffer containing the transformed data
 * @param[in]  bufferLength  Length of the buffer in bytes
 * @param[out] messageType   Extracted message type (REQUEST / RESPONSE)
 * @param[out] messageResult Extracted message result (OK / ERROR)
 * @return     Std_ReturnType E_OK if extraction successful, E_NOT_OK otherwise
 */

typedef Std_ReturnType (*Std_ExtractProtocolHeaderFieldsType)(
    const uint8* buffer, 
    uint32 bufferLength, 
    Std_MessageTypeType* messageType,
    Std_MessageResultType* messageResult
);


/**
 * @brief E_OK, E_NOT_OK 
 * @details ive been defined within OSEK and to avoid  clashes and redefinition problems
 * 
 * @spec:  [SWS_Std_00006]
 */

#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
#define E_OK    0x00u
typedef unsigned char StatusType; /* OSEK compliance */
#endif
#define E_NOT_OK 0x01u


/**
 * @brief STD_HIGH, STD_LOW
 * @details represent differ voltage of GPIO pin 
 *          HIGH 5V or 3.3V
 *          LOW  0V
 * @speec: [SWS_Std_00007]
 */
#define         STD_HIGH        0x01u
#define         STD_LOW         0x00u


/**
 * @brief STD_ACTIVE, STD_IDLE
 * @details Show Pin State  active or idle 
 * @spec: [SWS_Std_00007]
 */

#define         STD_ACTIVE      0x01u
#define         STD_IDLE        0x00u



/**
 * @brief STD_ACTIVE, STD_IDLE
 * @details Show Pin State  active or idle 
 * @spec: [SWS_Std_00013]
 */

#define         STD_ACTIVE      0x01u
#define         STD_IDLE        0x00u

/**
 * @brief STD_OFF, STD_ON
 * @details show the pin on or off 
 * @spec: [SWS_Std_00010]
 */
#define         STD_OFF       0x01u
#define         STD_ON        0x00u


/**
 * @brief NULL_PTR
 * @details 
 */
#ifndef NULL_PTR
#define NULL_PTR      ((void*)0)    
#endif

/*******************************************************************************
 * Std_TransformerClass Enumeration Values
 *
 * @spec [SWS_Std_00024]
 ******************************************************************************/
#define STD_TRANSFORMER_UNSPECIFIED     0x00u  /**< No error / unspecified class  */
#define STD_TRANSFORMER_SERIALIZER      0x01u  /**< Serializer transformer        */
#define STD_TRANSFORMER_SAFETY          0x02u  /**< Safety transformer            */
#define STD_TRANSFORMER_SECURITY        0x03u  /**< Security transformer          */
#define STD_TRANSFORMER_CUSTOM          0xFFu  /**< Custom / non-AUTOSAR class    */
 
/*******************************************************************************
 * Std_TransformerForwardCode Values (DRAFT)
 *
 * @spec [SWS_Std_00028] [SWS_Std_00029]
 ******************************************************************************/
#define E_SAFETY_INVALID_REP            0x01u  /**< Repeat last sequence number   */
#define E_SAFETY_INVALID_SEQ            0x02u  /**< Use wrong sequence number     */
#define E_SAFETY_INVALID_CRC            0x03u  /**< Generate wrong CRC            */
 
/*******************************************************************************
 * Std_MessageTypeType Values
 *
 * @spec [SWS_Std_91001]
 ******************************************************************************/
#define STD_MESSAGETYPE_REQUEST         0x00u  /**< C/S Request message           */
#define STD_MESSAGETYPE_RESPONSE        0x01u  /**< C/S Response message          */
 
/*******************************************************************************
 * Std_MessageResultType Values
 *
 * @spec [SWS_Std_91002]
 ******************************************************************************/
#define STD_MESSAGERESULT_OK            0x00u  /**< Response result OK            */
#define STD_MESSAGERESULT_ERROR         0x01u  /**< Response result ERROR         */
 


#endif