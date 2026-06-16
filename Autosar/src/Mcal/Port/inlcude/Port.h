/*
 * Port.h
 *
 *  Created on: Jun 11, 2026
 *      Author: BO
 */

#ifndef MCAL_PORT_INLCUDE_PORT_H_
#define MCAL_PORT_INLCUDE_PORT_H_

/*
 *****INCLUE 
 */
#include "Port_Cfg.h"
#include "Port_Types.h"
#include "../../../common/Compiler.h"

/**
 * @brief 
 * 
 */
FUNC(void, PORT_CODE) Port_Init(P2CONST(Port_ConfigType,AUTOMATIC,PORT_APPL_CONST)  ConfigPt);



/**
 * @brief Port set pin direction 
 *
 * @details to set port pin direction
 * @param Pin
 * @param Direction 
 * @spec [SWS_Port_00141] 
 */
FUNC(void,PORT_CODE) Port_SetPinDirection(VAR(Port_PinType,AUTOMATIC) Pin, VAR(Port_PinDirectionType, AUTOMATIC) Direction);


/**
 * @brief Refreshes port direction
 * @details to resfresh port
 * 
 * @spec [SWS_Port_00142]
 */
FUNC(void, PORT_CODE) Port_RefreshPortDirection(void); 

/**
 * @brief Get version 
 * @details Return the version information of the module
 * @param Versioninfo , A pointer to sore version infor of this module
 * 
 * @spec: [SWS_Port_00143] 
 */
FUNC(void, PORT_CODE) Port_GetVersionInfo (P2VAR(Std_VersionInfoType,AUTOMATIC,PORT_APPL_DATA) versioninfo);



/**
 * @brief Set PIN MODE 
 * @details Use in set Pin mode 
 * 
 * @param Pin., Port ID num
 * @param Mode, port pin mode to be set on port pin
 * @spec: [SWS_Port_00145] 
 */

FUNC(void, PORT_CODE) Port_SetPinMode(VAR(Port_PinType,AUTOMATIC) Pin, VAR(Port_PinModeType, AUTOMATIC) Mode);



#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
FUNC(void, PORT_CODE) Port_SetPinDirection(VAR(Port_PinType,AUTOMATIC) Pin, VAR(Port_PinDirectionType, AUTOMATIC) Direction);
#endif

#if (PORT_SET_PIN_MODE_API == STD_ON)
FUNC(void, PORT_CODE) Port_SetPinMode(VAR(Port_PinType,AUTOMATIC) Pin, VAR(Port_PinModeType, AUTOMATIC) Mode);
#endif

#if (PORT_VERSION_INFO_API == STD_ON)
FUNC(void, PORT_CODE) Port_GetVersionInfo (P2VAR(Std_VersionInfoType,AUTOMATIC,PORT_APPL_DATA) versioninfo);
#endif


#endif /* MCAL_DIO_INLCUDE_PORT_H_ */
