/*
 * Dio_Cfg.h
 *
 *  Created on: Jun 17, 2026
 *      Author: BO
 */

#ifndef MCAL_DIO_INCLUDE_DIO_CFG_H_
#define MCAL_DIO_INCLUDE_DIO_CFG_H_

#include "../../../Common/Std_Type.h"


/*Des for DIo port type  */

/*Det switch */
#define DIO_DEV_ERROR_DETECT    STD_ON


/*module version infor */
#define DIO_VENDOR_ID           0x0050u
#define DIO_MODULE_ID           0x0044u   
#define DIO_SW_MAJOR_VERSION    1u
#define DIO_SW_MINOR_VERSION    0u
#define DIO_SW_PATCH_VERSION    0u

/*optional API switch */
#define DIO_VERSION_INFO_API       STD_ON
#define DIO_FLIP_CHANNEL_API       STD_ON
#define DIO_MASKED_WRITE_PORT_API  STD_OFF



#define DIO_CHANNEL_LED_RED    ((Dio_ChannelType)96u)
#define DIO_CHANNEL_LED_BLUE   ((Dio_ChannelType)112u)
#define DIO_PORT_D             ((Dio_PortType)3u)


/*served ID for Det_ReportError() */
#define DIO_SID_READ_CHANNEL          0x00u
#define DIO_SID_WRITE_CHANNEL         0x01u
#define DIO_SID_READ_PORT             0x02u
#define DIO_SID_WRITE_PORT            0x03u
#define DIO_SID_READ_CHANNEL_GROUP    0x04u
#define DIO_SID_WRITE_CHANNEL_GROUP   0x05u
#define DIO_SID_GET_VERSION_INFO      0x06u
#define DIO_SID_FLIP_CHANNEL          0x07u
#define DIO_SID_MASKED_WRITE_PORT     0x08u

/*Error classification sent when DET ERROr*/

#define     DIO_E_PARAM_INVALID_CHANNEL_ID      0x0A
#define     DIO_E_PARAM_INVALID_PORT_ID         0x14
#define     DIO_E_PARAM_INVALID_GROUP           0x1F
#define     DIO_E_PARAM_POINTER                 0x20




#endif /* MCAL_DIO_INCLUDE_DIO_CFG_H_ */
