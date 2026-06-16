/*
 * Port_Types.h
 *  
 *  Created on: Jun 11, 2026
 *      Author: BO
 */

#ifndef MCAL_PORT_INLCUDE_PORT_TYPES_H_
#define MCAL_PORT_INLCUDE_PORT_TYPES_H_

/**********************INCLUDE *************************/
#include "../../../Common/Std_Type.h"

/**
 * @brief Port Pin types 
 * @details data type for the symbolic name of a port pin 
 * 
 * @Range: 0 - <PinID>
 * @spec: [SWS_Port_00229]
 */
typedef     uint16      Port_PinType;    


/**
 * @brief Pin Direction 
 * @details Set directions for port pin 
 * 
 * @spec: [SWS_Port_00230]
 */
typedef enum
{
    PORT_PIN_IN  = 0x00,
    PORT_PIN_OUT = 0x01
}Port_PinDirectionType;


/**
 * @brief Dif Port Pin mode
 * 
 * @spec: [SWS_Port_00231]
 */
typedef     uint8           Port_PinModeType;


/**
 * @brief config for all Pin 
 * @detail tool config Pin and mode 
 */

typedef struct 
{
    /*ID Pin physic */
    Port_PinType                pinId;
    /*Input or out put*/ 
    Port_PinDirectionType       direction;
    /* 1=GPIO, 2=ALT2, 3=ALT3... */
    Port_PinModeType            pinMode;
    /* turn on resistor ? */
    boolen                      pullEnable;
    /* TRUE=pull-up, FALSE=pull-down */ 
    boolen                      pullUp;
    /* TRUE=high drive strength */
    boolen                      driveStrength;
    /* allow change direction when runtime? */
    boolen                      dirChangeable;
    /* allow change mode when runtime? */
    boolen                      modeChangeable;
    /* OUTPUT (0 hoặc 1) */
    uint8                       initValue;
}Port_PinConfigType;


/**
 * @brief container pass Port_init
 * 
 */
typedef struct
{
    const Port_PinConfigType* pinConfigPtr; /* Pointer đến array pin configs */
    uint8                     numPins;      /* Số pin được config */
} Port_ConfigType;



#endif /* MCAL_DIO_INLCUDE_PORT_TYPES_H_ */
