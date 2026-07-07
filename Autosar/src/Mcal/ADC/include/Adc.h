/*
 * Adc.h
 *
 *  Created on: Jun 26, 2026
 *      Author: BO
 */

#ifndef MCAL_ADC_INCLUDE_ADC_H_
#define MCAL_ADC_INCLUDE_ADC_H_

#include "../../../Common/Compiler.h"
#include "../../../Common/Compiler_Cfg.h"
#include "Adc_Types.h"

/**
 * @brief Initializes the ADC hardware units and driver
 * caution: ADC sahll set all group to ADC_IDLE sate
 */
FUNC(void,ADC_CODE) Adc_Init (P2CONST(Adc_ConfigType,AUTOMATIC,ADC_APPL_CONST)ConfigPtr);

/**
 * @brief Initialized ADC driver with the group specific result buffer start 
 * 
 * caution Required after reset
 */
FUNC(Std_ReturnType, ADC_CODE) Adc_SetupResultBuffer(VAR(Adc_GroupType,AUTOMATIC)Group, P2VAR(Adc_ValueGroupType,AUTOMATIC, ADC_APPL_DATA) DataBufferPtr);


/**
 * @brief Returns all ADC HW Units to a state comparable to their power on reset state
 * 
 */
FUNC(void,ADC_CODE) Adc_DeInit (void); 


/**
 * @brief Starts the conversion of all channels of the requested ADC Channel group
 * 
 */
FUNC(void,ADC_CODE) Adc_StartGroupConversion (VAR(Adc_GroupType,AUTOMATIC) Group);



/**
 * @brief Stops the conversion of the requested ADC Channel group.
 * 
 */
FUNC(void,ADC_CODE)  Adc_StopGroupConversion(VAR(Adc_GroupType,AUTOMATIC) Group);


/**
 * @brief eads the group conversion result of the last completed conversion round of the requested
group and stores the channel values starting at the DataBufferPtr address
 * 
 */
FUNC(Std_ReturnType,ADC_CODE) Adc_ReadGroup (VAR(Adc_GroupType,AUTOMATIC) Group, P2VAR(Adc_ValueGroupType, AUTOMATIC,ADC_APPL_DATA) DataBufferPtr);



/**
 * @brief Enables the hardware trigger for the requested ADC Channel group.
 * 
 */
FUNC(void, ADC_CODE) Adc_EnableHardwareTrigger(VAR(Adc_GroupType,AUTOMATIC) Group); 




/**
 * @brief Disables the hardware trigger for the requested ADC Channel group.
 * 
 */
FUNC(void, AUTOMATIC) Adc_DisableHardwareTrigger (VAR(Adc_GroupType,AUTOMATIC) Group);




/**
 * @brief 
 * 
 */
FUNC(void, ADC_CODE) Adc_EnableGroupNotification(VAR(Adc_GroupType,AUTOMATIC) Group);


/**
 * @brief Disable 
 * 
 */
FUNC(void, ADC_CODE) Adc_DisableGroupNotification(VAR(Adc_GroupType,AUTOMATIC) Group);



/**
 * @brief Description Returns the conversion status of the requested ADC Channel group.
 * 
 */
FUNC(Adc_StatusType , ADC_CODE) Adc_GetGroupStatus(VAR(Adc_GroupType,AUTOMATIC) Group);



/**
 * @brief Returns the number of valid samples per channel
 * 
 */
FUNC(Adc_StreamNumSampleType  , ADC_CODE) Adc_GetStreamLastPointer(VAR(Adc_GroupType,AUTOMATIC) Group,P2VAR(Adc_ValueGroupType*,AUTOMATIC,ADC_APPL_DATA) PtrToSamplePtr);


/**
 * @brief Returns the version information of this module.
 * 
 */
FUNC(void,ADC_CODE) Adc_GetVersionInfo(P2VAR(Std_VersionInfoType,AUTOMATIC,ADC_APPL_DATA) versioninfo);



/**
 * @brief This API configures the Adc module so that it enters the already prepared power state, chosen
 * between a predefined set of configured ones
 * 
 */
FUNC(Std_ReturnType,ADC_CODE) Adc_SetPowerState (P2VAR(Adc_PowerStateRequestResultType,AUTOMATIC,ADC_APPL_DATA) Result);




/**
 * @brief This API returns the current power state of the ADC HW unit.
 * 
 */
FUNC(Std_ReturnType,ADC_CODE) Adc_GetCurrentPowerState(P2VAR(Adc_PowerStateType,AUTOMATIC,ADC_APPL_DATA) CurrentPowerState, P2VAR(Adc_PowerStateRequestResultType,AUTOMATIC,ADC_APPL_DATA) Result);



/**
 * @brief This API returns the Target power state of the ADC HW unit.
 * 
 */
FUNC(Std_ReturnType,ADC_CODE) Adc_GetTargetPowerState (P2VAR(Adc_PowerStateType,AUTOMATIC,ADC_APPL_DATA) TargetPowerState, P2VAR(Adc_PowerStateRequestResultType,AUTOMATIC,ADC_APPL_DATA)Result);




/**
 * @brief This API starts the needed process to allow the ADC HW module to enter the requested power state.
 * 
 */
FUNC(Std_ReturnType,ADC_CODE) Adc_PreparePowerState (VAR(Adc_PowerStateType,AUTOMATIC) PowerState, P2VAR(Adc_PowerStateRequestResultType,AUTOMATIC,ADC_APPL_DATA)Result);



/**
 * @brief This API is cyclically called and supervises the power state transitions
 * 
 */
FUNC(void,ADC_CODE) Adc_Main_PowerTransitionManager(void);



/**
 * @brief Will be called by the ADC Driver when a group conversion is completed for group <#groupID>.
 * 
 */
FUNC(void,ADC_CODE) IoHwAb_AdcNotification_Group0 (void);




FUNC(void,ADC_CODE) IoHwAb_AdcNotification_Group1 (void);






#endif /* MCAL_ADC_INCLUDE_ADC_H_ */
