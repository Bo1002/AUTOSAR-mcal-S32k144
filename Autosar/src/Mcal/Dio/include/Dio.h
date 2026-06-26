/*
 * Dio.h
 *
 *  Created on: Jun 17, 2026
 *      Author: BO
 */

#ifndef MCAL_DIO_INCLUDE_DIO_H_
#define MCAL_DIO_INCLUDE_DIO_H_

#include "Dio_Types.h"
#include "../../../Common/Compiler_Cfg.h"
#include "Dio_Cfg.h"
/**
 * @brief Dio read channel
 * 
 * @param ChannelId : ID of channel 
 * 
 * require: SWS_Dio_00005 and
            SWS_Dio_00026,83,84
 * @erro check ChannelId param if it valid within the configuration
 */
FUNC(Dio_LevelType,DIO_CODE)   Dio_ReadChannel (VAR(Dio_ChannelType,AUTOMATIC) ChannelId);


/**
 * @brief function write channel 
 * 
 * @param ChannelID : ID of Dio channel 
 * @param Level     : Value to be written
 * 
 *  SWS_Dio_00005, SWS_Dio_00119 and
 *  SWS_Dio_00026
 */
FUNC(void, DIO_CODE)    Dio_WriteChannel (VAR(Dio_ChannelType, AUTOMATIC) ChannelId, VAR(Dio_LevelType,AUTOMATIC) Level);


/**
 * @brief 
 * 
 */
FUNC(void, DIO_CODE)    Dio_WritePort (VAR(Dio_PortType,AUTOMATIC) PortId, VAR(Dio_PortLevelType, AUTOMATIC) level);



/**
 * @brief Read port
 * 
 */
FUNC(Dio_PortLevelType, DIO_CODE)  Dio_ReadPort (VAR(Dio_PortType,AUTOMATIC) PortId); 


/**
 * @brief read channel group 
 * 
 */

FUNC(Dio_PortLevelType,DIO_CODE) Dio_ReadChannelGroup (P2CONST(Dio_ChannelGroupType,AUTOMATIC,DIO_APPL_CONST) ChannelGroupIdPtr);



/**
 * @brief write group channel
 * 
 */
FUNC(void, DIO_CODE)    Dio_WriteChannelGroup (P2CONST(Dio_ChannelGroupType,AUTOMATIC,DIO_APPL_CONST) ChannelGroupIdPtr, VAR(Dio_PortLevelType, AUTOMATIC) Level);




#if (DIO_VERSION_INFO_API == STD_ON)
/**
 * @brief Get version
 * 
 */
FUNC(void, DIO_CODE) Dio_GetVersionInfo (P2VAR(Std_VersionInfoType,AUTOMATIC,DIO_APPL_DATA) VersionInfo);

#endif



#if (DIO_FLIP_CHANNEL_API == STD_ON)
/**
 * @brief 
 * 
 */
FUNC(Dio_LevelType,DIO_CODE) Dio_FlipChannel (VAR(Dio_ChannelType,AUTOMATIC) ChannelId);
#endif




#if(DIO_MASKED_WRITE_PORT_API == STD_ON)
/**
 * @brief 
 * 
 */
FUNC(void, DIO_CODE) Dio_MaskedWritePort(VAR(Dio_PortType, AUTOMATIC) PortId, VAR(Dio_PortLevelType, AUTOMATIC) level, VAR(Dio_PortLevelType,AUTOMATIC) Mask);
#endif

#endif /* MCAL_DIO_INCLUDE_DIO_H_ */
