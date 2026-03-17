#include  "AT.h"
uint8_t AT_message[410];
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
void AT_CREAT_URL(uint8_t mode,uint8_t step)
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
        sprintf(URL,"https://iot-api.heclouds.com/fuse-ota/fThVVTJwFW/ec200/%s/download",TID);
        break;
        case 3:
        //上报升级状态时的URL
        memset(URL, '\0', sizeof(URL));
        sprintf(URL,"https://iot-api.heclouds.com/fuse-ota/fThVVTJwFW/ec200/%d/status",step);
        break;
    }
}

void AT_CREAT_GET(uint8_t mode,uint8_t* tid,uint8_t Range_start,uint8_t Range_end,uint8_t*OTA_VERSION)
{
    switch (mode)
    {
        case 0:
        //检测升级任务时的GET
        memset(AT_message, '\0', sizeof(AT_message));
        uint8_t GET[120];
        memset(GET, '\0', sizeof(GET));
        sprintf(GET,"GET https://iot-api.heclouds.com/fuse-ota/fThVVTJwFW/ec200/check?type=2&version=%s HTTP/1.1\r\n", OTA_VERSION);
        uint8_t Host[30]="host:iot-api.heclouds.com\r\n";
        uint8_t Content_Type[40]="Content-Type:application/json\r\n";
        uint8_t Authorization[110]="Authorization:version=2018-10-31&res=products%2FfThVVTJwFW&et=1799825577&method=md5&sign=E0kvULpfTgwdKhkyBCMl7g%3D%3D\r\n";
        uint8_t accpet[20]="accept:*/*\r\n";
        uint8_t Content_length[30]="Content-Length:0\r\n";
        uint8_t End[2]="\r\n";
        strcat(AT_message,GET);
        strcat(AT_message,Content_Type);
        strcat(AT_message,Authorization);
        strcat(AT_message,Host);
        strcat(AT_message,accpet);
        strcat(AT_message,Content_length);
        strcat(AT_message,End);
        break;
        case 1:
        //下载升级包时的GET
        memset(AT_message, '\0', sizeof(AT_message));
        uint8_t GET1[120];
        memset(GET1, '\0', sizeof(GET1));
        sprintf(GET1,"GET https://iot-api.heclouds.com/fuse-ota/fThVVTJwFW/ec200/%s/download HTTP/1.1\r\n", tid);
        uint8_t Host1[30]="host:iot-api.heclouds.com\r\n";
        uint8_t Authorization1[110]="Authorization:version=2018-10-31&res=products%2FfThVVTJwFW&et=1799825577&method=md5&sign=E0kvULpfTgwdKhkyBCMl7g%3D%3D\r\n";
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

void AT_CREAT_POST(uint8_t mode,uint8_t*OTA_VERSION,uint8_t* tid,uint8_t step)
{
    switch (mode)
    {
        case 0:
        //上报版本号时的POST
        memset(AT_message, '\0', sizeof(AT_message));
        uint8_t POST[100]="POST https://iot-api.heclouds.com/fuse-ota/fThVVTJwFW/ec200/version HTTP/1.1\r\n";
        uint8_t Host[30]="host:iot-api.heclouds.com\r\n";
        uint8_t Content_Type[40]="Content-Type: application/json\r\n";
        uint8_t Authorization[130]="Authorization:version=2018-10-31&res=products%2FfThVVTJwFW&et=1799825577&method=md5&sign=E0kvULpfTgwdKhkyBCMl7g%3D%3D\r\n";
        uint8_t Content_length[30];
        memset(Content_length, '\0', 30);
        uint8_t End[3]="\r\n";
        char body[70];
        memset(body, '\0', sizeof(body));
        sprintf(OTA_VERSION,"12.3");
        sprintf(body,"{\"s_version\":\"%s\", \"f_version\": \"0\"}",OTA_VERSION);
        sprintf(Content_length , "Content-Length:%d\r\n", strlen(body));
        strcat(AT_message,POST);
        strcat(AT_message,Host);
        strcat(AT_message,Content_Type);
        strcat(AT_message,Authorization);
        strcat(AT_message,Content_length);
        strcat(AT_message,End);
        strcat(AT_message,body);
        break;
        case 1:
        //上报升级结果时的POST
        memset(AT_message, '\0', sizeof(AT_message));
        uint8_t POST1[120];
        memset(POST1, '\0', sizeof(POST1));
        sprintf(POST1,"POST https://iot-api.heclouds.com/fuse-ota/fThVVTJwFW/ec200/%s/status HTTP/1.1\r\n", tid);
        uint8_t Host1[30]="host:iot-api.heclouds.com\r\n";
        uint8_t Content_Type1[40]="Content-Type: application/json\r\n";
        uint8_t Authorization1[110]="Authorization:version=2018-10-31&res=products%2FfThVVTJwFW&et=1799825577&method=md5&sign=E0kvULpfTgwdKhkyBCMl7g%3D%3D\r\n";
        uint8_t Content_length1[30];
        memset(Content_length1, '\0', 30);
        uint8_t End1[2]="\r\n";
        char body1[50];
        memset(body1, '\0', sizeof(body1));
        sprintf(body1,"{\"step\":%d}",step);
        sprintf(Content_length1,"Content-Length:%d\r\n",sizeof(body1));
        strcat(AT_message,POST1);
        strcat(AT_message,Host1);
        strcat(AT_message,Content_Type1);
        strcat(AT_message,Authorization1);
        strcat(AT_message,Content_length1);
        strcat(AT_message,End1);
        strcat(AT_message,body1);
        break;
    }
}

void AT_GET_TID_VERSION_SIZE(uint8_t*OTA_VERSION,uint32_t FIlelen)
{

    //提取目标版本号，tid和文件大小
    memset(OTA_VERSION, '\0', sizeof(OTA_VERSION));
    memset(TID, '\0', sizeof(TID));
    uint8_t* version_start=strstr(AT_Recive,"\"target\":\"")+strlen("\"target\":\"");
    uint8_t* version_end=strstr(AT_Recive,"\",\"tid\":");
    uint8_t* tid_start=strstr(AT_Recive,"\"tid\":")+strlen("\"tid\":");
    uint8_t* tid_end=strstr(AT_Recive,",\"size\":");
    uint8_t* size_start=strstr(AT_Recive,"\"size\":")+strlen("\"size\":");
    uint8_t* size_end=strstr(AT_Recive,",\"md5\":");
    if(version_start!=NULL&&version_end!=NULL&&tid_start!=NULL&&tid_end!=NULL&&size_start!=NULL&&size_end!=NULL)
    {
        memset(OTA_VERSION, '\0', sizeof(OTA_VERSION));
        memset(TID, '\0', sizeof(TID));
        memcpy(OTA_VERSION,version_start,version_end-version_start);
        memcpy(TID,tid_start,tid_end-tid_start);
        uint8_t FIlelen_size=size_end-size_start;
        FIlelen=0;
        for(int i=0;i<FIlelen_size;i++)
        {
            FIlelen=FIlelen * 10 + (*(size_start+i) - '0');
        }
        uint8_t message[30];
        sprintf(message, "v:%s  tid:%s size:%d\r\n",OTA_VERSION,TID,FIlelen);
        HAL_UART_Transmit(&huart1,"message",28, 1000);
    }
    else 
    {
        HAL_UART_Transmit(&huart1,"服务器回复的任务消息出错\r\n",39, 1000);
    }
}
void AT_SET_URL(uint8_t mode,uint8_t step)
{
    AT_CREAT_URL(mode,step);
    memset(AT_message, '\0', sizeof(AT_message));
    sprintf(AT_message,"AT+QHTTPURL=%d,80\r\n",strlen(URL));
    AT_Send(AT_message);
    osDelay(pdMS_TO_TICKS(1000));
    Readystrx=AT_Recivejudge("CONNECT");
    while(Readystrx==NULL)
    {
        HAL_UART_Transmit(&huart1,"发送设置URL指令失败\r\n", 30, 1000);
        AT_Send(AT_message);
        osDelay(pdMS_TO_TICKS(1000));
        Readystrx=AT_Recivejudge("CONNECT");
    }
    HAL_UART_Transmit(&huart1,"发送设置URL指令成功\r\n", 30, 1000);
    AT_Send(URL);
    osDelay(pdMS_TO_TICKS(1000));
    Readystrx=AT_Recivejudge("OK");
    while(Readystrx==NULL)
    {
        HAL_UART_Transmit(&huart1,"发送URL失败\r\n", 20, 1000);
        AT_Send(URL);
        osDelay(pdMS_TO_TICKS(1000));
        Readystrx=AT_Recivejudge("OK");
    }
    HAL_UART_Transmit(&huart1,"发送URL成功\r\n", 20, 1000);
    memset(AT_message, '\0', sizeof(AT_message));

}
void AT_Http_Read()
{
    memset(AT_message, '\0', sizeof(AT_message));
    sprintf(AT_message, "AT+QHTTPREAD=80\r\n");
    AT_Send(AT_message);
    osDelay(pdMS_TO_TICKS(500));
    memset(AT_message, '\0', sizeof(AT_message));
    
}
void AT_SET_GET(uint8_t mode,uint8_t* tid,uint8_t Range_start,uint8_t Range_end,uint8_t*OTA_VERSION)
{
    uint8_t AT_GET[30];
    memset(AT_GET, '\0', sizeof(AT_GET));
    AT_CREAT_GET(mode,tid,Range_start,Range_end,OTA_VERSION);
    sprintf(AT_GET,"AT+QHTTPGET=80,%d,80\r\n",sizeof(AT_message));
    AT_Send(AT_GET);
    osDelay(pdMS_TO_TICKS(100));
    strx=AT_Recivejudge("CONNECT");
    while(strx==NULL)
    {
        HAL_UART_Transmit(&huart1, "发送GET报文指令失败\r\n", 30, 100);
        sprintf(AT_GET,"AT+QHTTPGET=80,%d,80\r\n",sizeof(AT_message));
        AT_Send(AT_GET);
        osDelay(pdMS_TO_TICKS(500));
        strx=AT_Recivejudge("CONNECT");
    }
    HAL_UART_Transmit(&huart1, "发送GET报文指令成功\r\n", 30, 100);
    AT_Send(AT_message);
    osDelay(pdMS_TO_TICKS(100));
    strx=AT_Recivejudge("0,200,");
    while(strx==NULL)
    {
        HAL_UART_Transmit(&huart1, "发送GET报文失败\r\n", 24, 100);
        AT_Send(AT_message);
        osDelay(pdMS_TO_TICKS(500));
        strx=AT_Recivejudge("0,200,");
    }
    HAL_UART_Transmit(&huart1, "发送GET报文成功\r\n", 24, 100);
    memset(AT_message, '\0', sizeof(AT_message));
}
void AT_SET_POST(uint8_t mode,uint8_t*OTA_VERSION,uint8_t* tid,uint8_t step)
{
    uint8_t AT_POST[30];
    memset(AT_POST, '\0', sizeof(AT_POST));
    memset(AT_message, '\0', sizeof(AT_message));
    AT_CREAT_POST(mode,OTA_VERSION,tid,step);
    sprintf(AT_POST,"AT+QHTTPPOST=%d,80,80\r\n",strlen(AT_message));
    HAL_UART_Transmit(&huart1, AT_POST, sizeof(AT_POST), 100);
    AT_Send(AT_POST);
    HAL_UART_Transmit(&huart1, AT_Recive, strlen(AT_Recive), 100);
    osDelay(pdMS_TO_TICKS(1000));
    HAL_UART_Transmit(&huart1, AT_Recive, strlen(AT_Recive), 100);
    strx=AT_Recivejudge("CONNECT");
    while(strx==NULL)
    {
        HAL_UART_Transmit(&huart1, "发送POST报文指令失败\r\n", 31, 100);
        sprintf(AT_POST,"AT+QHTTPPOST=%d,80,80\r\n",strlen(AT_message));
        AT_Send(AT_POST);
        osDelay(pdMS_TO_TICKS(1000));
        strx=AT_Recivejudge("CONNECT");
    }
    HAL_UART_Transmit(&huart1, "发送POST报文指令成功\r\n", 31, 100);
    AT_CREAT_POST(mode,OTA_VERSION,tid,step);
    AT_Send(AT_message);
    osDelay(pdMS_TO_TICKS(1000));
    strx=AT_Recivejudge("0,200,");
    while(strx==NULL)
    {
        HAL_UART_Transmit(&huart1, "发送POST报文失败\r\n", 25, 100);
        HAL_UART_Transmit(&huart1, AT_Recive, strlen(AT_Recive), 100);
        osDelay(pdMS_TO_TICKS(1000));
        HAL_UART_Transmit(&huart1, AT_message, strlen(AT_message), 100);
        AT_Send(AT_message);
        osDelay(pdMS_TO_TICKS(500));
        strx=AT_Recivejudge("0,200,");
    }
    HAL_UART_Transmit(&huart1, "发送POST报文成功\r\n", 25, 100);
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
        HAL_UART_Transmit(&huart1,"HTTP contextid设置出错\r\n", 29, 1000);
    }
    HAL_UART_Transmit(&huart1,"HTTP contextid已设置\r\n", 26, 1000);
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
    HAL_UART_Transmit(&huart1, "HTTP QIACT已设置\r\n", 22, 1000);
    osDelay(pdMS_TO_TICKS(500));
    AT_SET_sslctxid();
    Readystrx=AT_Recivejudge("OK");
    while(Readystrx==NULL)
    {
        AT_SET_sslctxid();
        osDelay(pdMS_TO_TICKS(500));
        Readystrx=AT_Recivejudge("OK");
    }
    HAL_UART_Transmit(&huart1, "HTTP sslctxid已设置\r\n", 25, 1000);
    osDelay(pdMS_TO_TICKS(500));
    AT_SET_seclevel();
    Readystrx=AT_Recivejudge("OK");
    while(Readystrx==NULL)
    {
        AT_SET_seclevel();
        osDelay(pdMS_TO_TICKS(500));
        Readystrx=AT_Recivejudge("OK");
    }
    HAL_UART_Transmit(&huart1, "HTTP seclevel已设置\r\n", 25, 1000);
    osDelay(pdMS_TO_TICKS(500));
    AT_SET_sslversion();
    Readystrx=AT_Recivejudge("OK");
    while(Readystrx==NULL)
    {
        AT_SET_sslversion();
        osDelay(pdMS_TO_TICKS(500));
        Readystrx=AT_Recivejudge("OK");
    }
    HAL_UART_Transmit(&huart1, "HTTP sslversion已设置\r\n", 27, 1000);
    osDelay(pdMS_TO_TICKS(500));
}