#include "OTA.h"

//用来获取是否有OTA任务的标志
void OTA_GET_OTAFlag(void)
{
    char*str1=NULL,*str2=NULL;
    //从W25Q64中获取OTA_INFO
    W25Q64_READ_OTA_INFO(&OTA_info);
    AT_SET_URL(0, 0);
    osDelay(pdMS_TO_TICKS(1000));
    AT_SET_POST(0, "1.34", 0, 0);
    AT_Http_Read();
    str1=AT_Recivejudge("succ");
    str2=AT_Recivejudge("+QHTTPREAD: 0");
    if((str1!=NULL)&&(str2!=NULL))
    {
        HAL_UART_Transmit(&huart1, "上报版本号成功\r\n", 24, 100);
    }
}

void OTA_CHECK_UPDATA(void)
{ 
    char*str1=NULL,*str2=NULL;
    uint8_t* str[250]={0};
    uint16_t size;
    AT_SET_URL(1, 0);
    osDelay(pdMS_TO_TICKS(1000));
    AT_SET_GET(0, TID, 0, 0, "1.34");
    AT_Http_Read();
    PQ_Read(&message,str,&size);
    str1=strstr((const char*)str, "succ");
    str2=AT_Recivejudge("+QHTTPREAD: 0");
    if((str1!=NULL)&&(str2!=NULL))
    {
        HAL_UART_Transmit(&huart1, "检测到OTA任务\r\n", 21, 100);
        AT_GET_TID_VERSION_SIZE(OTA_info.OTA_VERSION, OTA_info.FileLen);
    }
    

}