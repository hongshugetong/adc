#include  "AT.h"
uint8_t AT_message[310];
uint8_t AT_Json[60];
uint8_t AT_Recive[1200];
uint8_t URL[80];
uint8_t TID[20];
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

void AT_SET_contextid(void)
{
    sprintf(AT_message,"AT+QHTTPCFG=\"contextid\",1\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}

void AT_SET_responseheader(void)
{
    sprintf(AT_message,"AT+QHTTPCFG=\"responseheader\",0\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}

void AT_SET_requestheader(void)
{
    sprintf(AT_message,"AT+QHTTPCFG=\"requestheader\",1\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}

void AT_QIACT(void)
{
    sprintf(AT_message,"AT+QIACT=?\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
    osDelay(pdMS_TO_TICKS(200));
    sprintf(AT_message,"AT+QIACT=1,1,\"CMNET\",\"\",\"\",1\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
    osDelay(pdMS_TO_TICKS(200));
    sprintf(AT_message,"AT+QIACT=1\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
    osDelay(pdMS_TO_TICKS(200));
    sprintf(AT_message,"AT+QIACT=?\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}

void AT_SET_sslctxid(void)
{
    sprintf(AT_message,"AT+QHTTPCFG=\"sslctxid\",1\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}

void AT_SET_seclevel(void)
{
    sprintf(AT_message,"AT+QSSLCFG=\"seclevel\",1,0\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}

void AT_SET_sslversion(void)
{
    sprintf(AT_message,"AT+QSSLCFG=\"sslversion\",1,4\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
}
void AT_CREAT_URL(uint8_t mode,uint8_t* tid)
{
    switch (mode)
    {
        case 0:
        //上报版本号时的URL
        memset(URL, '\0', sizeof(URL));
        sprintf(URL, "https://iot-api.heclouds.com/fuse-ota/fThVVTJwFW/ec200/version");
        break;
        case 1:
        //检测升级任务时的URL
        memset(URL, '\0', sizeof(URL));
        sprintf(URL,"https://iot-api.heclouds.com/fuse-ota/fThVVTJwFW/ec200/check");
        break;
        case 2:
        //下载升级包时的URL
        memset(URL, '\0', sizeof(URL));
        sprintf(URL,"https://iot-api.heclouds.com/fuse-ota/fThVVTJwFW/ec200/%s/download",tid);
        break;
    }
}

void AT_CREAT_GET(uint8_t mode,uint8_t* tid,uint8_t Range_start,uint8_t Range_end,uint8_t*OTA_VERSION)
{
    switch (mode)
    {
        case 0:
        //检测升级任务时的GET
        memset(AT_message, '\0', 310);
        uint8_t GET[120];
        memset(GET, '\0', sizeof(GET));
        sprintf(GET,"GET https://iot-api.heclouds.com/fuse-ota/fThVVTJwFW/ec200/check?type=2&version=%s HTTP/1.1\r\n", OTA_VERSION);
        uint8_t Host[30]="host:iot-api.heclouds.com\r\n";
        uint8_t Content_Type[40]="Content-Type:application/json\r\n";
        uint8_t Authorization[110]="version=2018-10-31&res=products%2FfThVVTJwFW&et=1799825577&method=md5&sign=E0kvULpfTgwdKhkyBCMl7g%3D%3D\r\n";
        uint8_t accpet[20]="accept:*/*\r\n";
        uint8_t Content_length[30]="Content-Length:0\r\n";
        uint8_t End[2]="\r\n";
        strcat(AT_message,GET);
        strcat(AT_message,Content_Type);
        strcat(AT_message,Authorization);
        strcat(AT_message, Host);
        strcat(AT_message,accpet);
        strcat(AT_message,Content_length);
        strcat(AT_message,End);
        break;
        case 1:
        //下载升级包时的GET
        memset(AT_message, '\0', 310);
        uint8_t GET1[120];
        memset(GET1, '\0', sizeof(GET1));
        sprintf(GET1,"GET https://iot-api.heclouds.com/fuse-ota/fThVVTJwFW/ec200/%s/download HTTP/1.1\r\n", tid);
        uint8_t Host1[30]="host:iot-api.heclouds.com\r\n";
        uint8_t Authorization1[110]="version=2018-10-31&res=products%2FfThVVTJwFW&et=1799825577&method=md5&sign=E0kvULpfTgwdKhkyBCMl7g%3D%3D\r\n";
        uint8_t Range[30];
        memset(Range, '\0', 30);
        sprintf(Range,"Range:%d-%d\r\n",Range_start,Range_end);
        uint8_t End1[2]="\r\n";
        strcat(AT_message,GET1);
        strcat(AT_message,Host1);
        strcat(AT_message,Authorization1);
        strcat(AT_message,Range);
        strcat(AT_message,End1);
        break;
    }
}
void AT_SET_URL(uint8_t* url)
{

    sprintf(AT_message,"AT+QHTTPURL=%d,80\r\n",sizeof(url));
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
    osDelay(pdMS_TO_TICKS(500));
    AT_Send(url);

}

void AT_SET_GET(void)
{
    sprintf(AT_message,"AT+QHTTPGET=1\r\n");
    AT_Send(AT_message);
    memset(AT_message, '\0', sizeof(AT_message));
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


void AT_HTTP_Init(void)
{
    AT_SET_contextid();
    osDelay(pdMS_TO_TICKS(500));
    Readystrx=AT_Recivejudge("OK");
    while(Readystrx==NULL)
    {
        AT_SET_contextid();
        osDelay(pdMS_TO_TICKS(500));
        Readystrx=AT_Recivejudge("OK");
    }
    HAL_UART_Transmit(&huart1,"HTTP contextid已设置", 22, 1000);
    osDelay(pdMS_TO_TICKS(500));
    AT_SET_responseheader();
    osDelay(pdMS_TO_TICKS(500));
    AT_SET_requestheader();
    osDelay(pdMS_TO_TICKS(500));
    AT_QIACT();
    Readystrx=AT_Recivejudge("OK");
    while(Readystrx==NULL)
    {
        AT_QIACT();
        osDelay(pdMS_TO_TICKS(500));
        Readystrx=AT_Recivejudge("OK");
    }
    HAL_UART_Transmit(&huart1, "HTTP QIACT已设置", 22, 1000);
    osDelay(pdMS_TO_TICKS(500));
    AT_SET_sslctxid();
    Readystrx=AT_Recivejudge("OK");
    while(Readystrx==NULL)
    {
        AT_SET_sslctxid();
        osDelay(pdMS_TO_TICKS(500));
        Readystrx=AT_Recivejudge("OK");
    }
    HAL_UART_Transmit(&huart1, "HTTP sslctxid已设置", 22, 1000);
    osDelay(pdMS_TO_TICKS(500));
    AT_SET_seclevel();
    Readystrx=AT_Recivejudge("OK");
    while(Readystrx==NULL)
    {
        AT_SET_seclevel();
        osDelay(pdMS_TO_TICKS(500));
        Readystrx=AT_Recivejudge("OK");
    }
    HAL_UART_Transmit(&huart1, "HTTP seclevel已设置", 22, 1000);
    osDelay(pdMS_TO_TICKS(500));
    AT_SET_sslversion();
    Readystrx=AT_Recivejudge("OK");
    while(Readystrx==NULL)
    {
        AT_SET_sslversion();
        osDelay(pdMS_TO_TICKS(500));
        Readystrx=AT_Recivejudge("OK");
    }
    HAL_UART_Transmit(&huart1, "HTTP sslversion已设置", 22, 1000);
    osDelay(pdMS_TO_TICKS(500));
}