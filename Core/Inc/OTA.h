/**
 ******************************************************************************
 * @file    template.h
 * @author  Your Name
 * @brief   Header file for template functionality
 ******************************************************************************
 * @attention
 *
 * This file contains the declarations for the template module.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OTA_H
#define OTA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "AT.h"
#include "W25Q64.h"
// #include "stm32f4xx_hal.h"  // 根据需要取消注释

/* Exported types ------------------------------------------------------------*/
extern uint8_t URL[80];
extern OTA_INFO OTA_info;
extern uint8_t TID[20];

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void OTA_GET_OTAFlag(void);
void OTA_CHECK_UPDATA(void);
void OTA_DOWNLOAD(uint32_t Range_start,uint32_t Range_end);
void OTA_PUT_UPADATE_INformation(uint8_t step);
#ifdef __cplusplus
}
#endif

#endif /* TEMPLATE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/