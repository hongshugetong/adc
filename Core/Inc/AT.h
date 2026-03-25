#ifndef __AT_H
#define __AT_H

#include "main.h"
#include "stdio.h"
#include <stdbool.h>
#include <string.h>
#include "math.h"
#include "loop.h"
/* AT命令处理状态 */

#define AT_TOKEN "version=2018-10-31&res=products%2FfThVVTJwFW&et=1799825577&method=md5&sign=E0kvULpfTgwdKhkyBCMl7g%3D%3D"
#define AT_ProjectID "fThVVTJwFW"
#define AT_DeviceID "ec200"
#define AT_MQTT_URL "mqtts.heclouds.com"
#define AT_MQTT_Port "1883"
#define AT_MQTT_Publish "$sys/fThVVTJwFW/ec200/dp/post/json"
/* AT命令结构体 */


/* 函数声明 */
char * AT_Recivejudge(char*cmd);
void AT_Send(uint8_t *cmd);
void AT_setMQTT_version(uint8_t version);
void AT_OpenMQTT(void);
void AT_ConnectMQTT(void);
void create_adc_json(float value, uint8_t* json_str);
void AT_Publish_MQTT(float adc_value);
void AT_ATE0(void);
void EC20_Init(void);
void MQTT_Init(void);
void AT_CREAT_URL(uint8_t mode,uint8_t step);
void AT_CREAT_GET(uint8_t mode,uint8_t* tid,uint8_t Range_start,uint8_t Range_end,uint8_t*OTA_VERSION);
void AT_CREAT_POST(uint8_t mode,uint8_t*OTA_VERSION,uint8_t* tid,uint8_t step);
void AT_GET_TID_VERSION_SIZE(uint8_t*OTA_VERSION,uint32_t*FIlelen,uint8_t*str);
void AT_SET_URL(uint8_t mode,uint8_t step);
void AT_Http_Read();
void AT_SET_GET(uint8_t mode,uint8_t* tid,uint8_t Range_start,uint8_t Range_end,uint8_t*OTA_VERSION);
void AT_SET_POST(uint8_t mode,uint8_t*OTA_VERSION,uint8_t* tid,uint8_t step);
void AT_HTTP_Init(void);
#endif /* __AT_H */