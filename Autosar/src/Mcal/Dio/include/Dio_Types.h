/*
 * Dio_Types.h
 *
 *  Created on: Jun 17, 2026
 *      Author: BO
 */

#ifndef MCAL_DIO_INCLUDE_DIO_TYPES_H_
#define MCAL_DIO_INCLUDE_DIO_TYPES_H_

#include "../../../Common/Std_Type.h"



/**
 * @brief Shall cover all available
DIO channels

 * 
 */
typedef        uint8      Dio_ChannelType;



/**
 * @brief  Numeric ID of a Dio Port
 * @Range 0...portID
 */
typedef         uint8      Dio_PortType;


/**
 * @brief type for defiantion of a channel group include channel and port 
 * 
 * @spec: [SWS_Dio_00184]
 */

typedef struct 
{
    /*This element mask which defines the positions of the channel group.*/
   uint32   mask;
   
   /*This element shall be the position of the Channel Group on the port,
   counted from the LSB.*/
   uint8    offset;

   /*This shall be the port on which the Channel group is defined.*/
   Dio_PortType port; 

}Dio_ChannelGroupType;


/**
 * @brief the possible levels a DIO channel (input or output)
 * STD_LOW 0x00 Physical state 0V
 * STD_HIGH 0x01 Physical state 5V or 3.3V
 * @spec: [SWS_Dio_00185]
 */
typedef    uint8     Dio_LevelType;



/**
 * @brief 
 * 
 */
 typedef    uint32        Dio_PortLevelType;



 
#endif /* MCAL_DIO_INCLUDE_DIO_TYPES_H_ */
