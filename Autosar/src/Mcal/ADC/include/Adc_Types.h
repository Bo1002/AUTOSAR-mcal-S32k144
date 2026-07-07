/*
 * Adc_Types.h
 *
 *  Created on: Jun 26, 2026
 *      Author: BO
 */

#ifndef MCAL_ADC_INCLUDE_ADC_TYPES_H_
#define MCAL_ADC_INCLUDE_ADC_TYPES_H_

#include "../../../Common/Std_Type.h"
#include "../../../Common/Platform_Types.h"
#include "../../../Common/Compiler.h"

/**
 * @brief Numeric ID of an ADC channel
 * 
 */
typedef     uint8       Adc_ChannelType;



/**
 * @brief Numeric ID of an ADC channel group.

 * 
 */
typedef     uint8       Adc_GroupType;



/**
 * @brief Type for reading the converted values of a channel group
 * 
 */
typedef     uint16_least       Adc_ValueGroupType; 



/**
 * @brief Type of clock prescaler factor. This is not an API type
 * 
 */
typedef      uint8              Adc_PrescaleType;




/** @brief Type of conversion time, i.e. the time during which the sampled analogue value is converted into
digital representation
 * 
*/
typedef     uint8               Adc_ConversionTimeType;



/**
 * @brief Type of sampling time, i.e. the time during which the value is sampled, (in clock-cycles). (This is
not an API type).
 * 
 */
typedef      uint8              Adc_SamplingTimeType;



/**
 * @brief Type of channel resolution in number of bits. (This is not an API type).

 * 
 */

typedef      uint8                Adc_ResolutionType;


/**
 * @brief Description Current status of the conversion of the requested ADC Channel group.

 * 
 */

typedef enum
{
    /*no result avail, the conversin has not been started */
    ADC_IDLE                =  0X00,
    /*conversion has bene started and is still going on, no result*/
    ADC_BUSY                =  0x01,
    /*conversion round, result is avail for all channels of the group*/
    ADC_COMPLETED           =  0x02,
    /*the result buffer is completely filled*/
    ADC_STREAM_COMPLETED    =  0x03
}Adc_StatusType;


/**
 * @brief type of config trigger source of an adc channel group
 * 
 */

typedef enum 
{
    ADC_TRIGG_SRC_SW        =  0x01,
    ADC_TRIGG_SRC_HW        =  0x02
}Adc_TriggerSourceType;


/**
 * @brief Type for configuring the conversion mode of an ADC Channel group.
 * 
 * for MODE_ONESHOT: one conversion per time in an ADC channel group
 * if perform but AFTER the configured trigger event.
 * Caution: if "Group trigger source software" a started One-shot conversion can be stopped by software API 
 * both can be stopped by software APIN call 
 * but continuos is immitates with "Group trigger source software"
 */
typedef enum
{
    ADC_CONV_MODE_ONESHOT       =  0x00,
    ADC_CONV_MODE_CONTINUOUS    =  0x01
}Adc_GroupConvModeType;



/**
 * @brief Priority level of the channel. Lowest priority is 0.
 * 
 * RANGE: 0_255
 */
typedef        uint8            Adc_GroupPriorityType;




/**
 * @brief Type for configuring the number of group conversions in streaming access mode (in single access
 * mode, parameter is 1).
 * 
 */
typedef         uint8           Adc_GroupDefType;





/**
 * @brief Type for configuring the number of group conversions in streaming access mode (in single access
mode, parameter is 1).
 * 
 */
typedef         uint8           Adc_StreamNumSampleType;




/**
 * @brief Type for configuring the streaming access mode buffer type.
 * 
 */
typedef enum
{
    ADC_STREAM_BUFFER_LINEAR    =  0x00,
    ADC_STREAM_BUFFER_CIRCULAR  =  0x01
}Adc_StreamBufferModeType;





typedef enum
{
    ADC_ACCESS_MODE_SINGLE      =  0x00,
    ADC_ACCESS_MODE_STREAMING   =  0x01
}Adc_GroupAccessModeType;





typedef enum
{
    ADC_HW_TRIG_RISING_EDGE     =  0x00,
    ADC_HW_TRIG_FALLING_EDGE    =  0x01,
    ADC_HW_TRIG_BOTH_EDGE       =  0x02
}Adc_HwTriggerSignalType;



typedef         uint8           Adc_HwTriggerTimerType;




typedef enum
{
    ADC_PRIORITY_NONE        =  0x00,
    ADC_PRIORITY_HW          =  0x01,
    ADC_PRIORITY_HW_SW       =  0x02
}Adc_PriorityImplementationType;






typedef enum
{
    ADC_GROUP_REPL_ABORT_RESTART      =  0x00,
    ADC_GROUP_REPL_SUSPEND_RESTART    =  0x01
}Adc_GroupReplacementType;



typedef enum
{
    ADC_RANGE_UNDER_LOW        =  0x00,
    ADC_RANGE_BETWEEN          =  0x01,
    ADC_RANGE_OVER_HIGH        =  0x02,
    ADC_RANGE_ALWAYS           =  0x03,
    ADC_RANGE_NOT_UNDER_LOW    =  0x04,
    ADC_RANGE_NOT_BETWEEN      =  0x05,
    ADC_RANGE_NOT_OVER_HIGH    =  0x06
}Adc_ChannelRangeSelectType;



typedef enum
{
    ADC_ALIGN_LEFT      =  0x00,
    ADC_ALIGN_RIGHT     =  0x01
}Adc_ResultAlignmentType;



typedef         uint8           Adc_PowerStateType;




typedef enum
{
    ADC_SERVICE_ACCEPTED        =  0x00,
    ADC_NOT_INIT                =  0x01,
    ADC_SEQUENCE_ERROR          =  0x02,
    ADC_HW_FAILURE              =  0x03,
    ADC_POWER_STATE_NOT_SUPP    =  0x04,
    ADC_TRANS_NOT_POSSIBLE      =  0x05
}Adc_PowerStateRequestResultType;



typedef struct
{
    Adc_ChannelType        channelId;
    Adc_SamplingTimeType   samplingTime;
    Adc_ResolutionType     resolution;
} Adc_ChannelConfigType;



typedef struct
{
    Adc_GroupType                groupId;
    const Adc_ChannelConfigType* channelConfigPtr;
    uint8                        numChannels;
    Adc_TriggerSourceType        trigger;
    Adc_GroupConvModeType        convMode;
    Adc_GroupPriorityType        priority;
    Adc_GroupAccessModeType      accessMode;
    Adc_ResultAlignmentType      resultAlignment;
    Adc_ValueGroupType*          resultBufferPtr;       /* RAM chứa kết quả conversion */
    boolen                       notificationEnable;    /* group này có cần gọi callback không */
    P2FUNC(void, ADC_APPL_CODE, NotificationPtr)(void); /* callback gọi khi group conversion xong */
} Adc_GroupConfigType;



typedef struct 
{
    const Adc_GroupConfigType* Configtool;
    uint8                 numGroups;
}Adc_ConfigType;


#endif /* MCAL_ADC_INCLUDE_ADC_TYPES_H_ */
