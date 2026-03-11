#include  "AT.h"
uint8_t AT_message[150];
uint8_t AT_Json[60];
uint8_t AT_Recive[50];
char *strx,*extstrx,*Readystrx;


char * AT_Recivejudge(char*cmd)
{
    return strstr((const char*)AT_Recive, cmd);
}
void AT_Print(uint8_t*data)
{
    HAL_UART_Transmit(&huart1,data, strlen(data), 1000);
    memset(data, '\0', sizeof(data));
}
void AT_Send(uint8_t *cmd)
{
    HAL_UART_Transmit(&huart3, cmd, strlen(cmd), 1000);
}
void AT_ATE0(void)
{
    sprintf(AT_message,"ATE0\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}
void AT_SIMReady(void)
{
    sprintf(AT_message,"AT+CPIN?\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}
void AT_GSM()
{
    sprintf(AT_message,"AT+CREG?\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}

void AT_GPRS(void)
{
    sprintf(AT_message,"AT+CGREG?\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}
void AT_setMQTT_version(uint8_t version)
{
    //如果version=0:设置mqtt版本为3.1
    //如果version=1:设置mqtt版本为3.1.1
    if(version)
    {
        sprintf(AT_message,"AT+QMTCFG=\"version\",0,4\r\n");
    }
    else
    {
        sprintf(AT_message,"AT+QMTCFG=\"version\",0,3\r\n");
    }
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}

void AT_OpenMQTT(void)
{
    sprintf(AT_message,"AT+QMTOPEN=0,\"%s\",%s\r\n",AT_MQTT_URL,AT_MQTT_Port);
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}

void AT_ConnectMQTT(void)
{
    sprintf(AT_message,"AT+QMTCONN=0,\"%s\",\"%s\",\"%s\"\r\n",AT_DeviceID,AT_ProjectID,AT_TOKEN);
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}
void AT_BreakMQTT(void)
{
    sprintf(AT_message,"AT+QMTDISC=0\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}
void create_adc_json(float value, uint8_t*json_str)
{
    sprintf(json_str, "{\"id\":3,\"dp\":{\"ADC\":[{\"v\":%0.3f}]}}", value);
}

void AT_Publish_MQTT(float adc_value)
{
    create_adc_json(adc_value, AT_Json);
    sprintf(AT_message,"AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n",AT_MQTT_Publish,strlen(AT_Json));
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
    osDelay(pdMS_TO_TICKS(1000));
    AT_Send(AT_Json);
    memset(AT_message, '\0', sizeof(AT_Json));
}

void EC20_Init(void)
{ 
    AT_Send("AT\r\n");
    osDelay(pdMS_TO_TICKS(500));
    strx=AT_Recivejudge("OK");
    while(strx==NULL)
    {
        AT_Send("AT\r\n");
        osDelay(pdMS_TO_TICKS(500));
        strx=AT_Recivejudge("OK");
    }
    HAL_UART_Transmit(&huart1,"EC200已经启动", 18, 1000);
    AT_BreakMQTT();
    osDelay(pdMS_TO_TICKS(500));
    AT_ATE0();
    osDelay(pdMS_TO_TICKS(500));
    AT_SIMReady();
    osDelay(pdMS_TO_TICKS(500));
    Readystrx=AT_Recivejudge("READY");
    while(Readystrx==NULL)
    {
        AT_SIMReady();
        osDelay(pdMS_TO_TICKS(500));
        Readystrx=AT_Recivejudge("READY");
    }
    HAL_UART_Transmit(&huart1,"SIM卡已准备好", 19, 1000);
    osDelay(pdMS_TO_TICKS(500));
    AT_GSM();
    osDelay(pdMS_TO_TICKS(500));
    strx=AT_Recivejudge("+CREG: 0,1");
    extstrx=AT_Recivejudge("+CREG: 0,5");
    while((strx==NULL)&&(extstrx==NULL))
    {
        AT_GSM();
        osDelay(pdMS_TO_TICKS(500));
        strx=AT_Recivejudge("+CREG: 0,1");
        extstrx=AT_Recivejudge("+CREG: 0,5");
    }
    HAL_UART_Transmit(&huart1,"GSM网络已注册", 19, 1000);
    osDelay(pdMS_TO_TICKS(500));
    AT_GPRS();
    osDelay(pdMS_TO_TICKS(500));
    strx=AT_Recivejudge("+CGREG: 0,1");
    extstrx=AT_Recivejudge("+CGREG: 0,5");
    while((strx==NULL)&&(extstrx==NULL))
    {
        AT_GPRS();
        osDelay(pdMS_TO_TICKS(500));
        strx=AT_Recivejudge("+CGREG: 0,1");
        extstrx=AT_Recivejudge("+CGREG: 0,5");
    }
    HAL_UART_Transmit(&huart1,"GPRS网络已注册", 20, 1000);
}

void MQTT_Init(void)
{
    AT_setMQTT_version(1);
    osDelay(pdMS_TO_TICKS(500));
    AT_OpenMQTT();
    osDelay(pdMS_TO_TICKS(500));
    strx=AT_Recivejudge("+QMTCONN: 0,0");
    while(strx==NULL)
    {
        AT_ConnectMQTT();
        osDelay(pdMS_TO_TICKS(500));
        strx=AT_Recivejudge("+QMTCONN: 0,0");
    }
    HAL_UART_Transmit(&huart1,"MQTT已连接", 15, 1000);    
    AT_ConnectMQTT();
    osDelay(pdMS_TO_TICKS(500));
    strx=AT_Recivejudge("+QMTCONN: 0,0,0");
    while(strx==NULL)
    {
        AT_ConnectMQTT();
        osDelay(pdMS_TO_TICKS(500));
        strx=AT_Recivejudge("+QMTCONN: 0,0,0");
    }
    HAL_UART_Transmit(&huart1,"MQTT已登入", 14, 1000);


}