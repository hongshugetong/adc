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
    uint8_t str[250]={0};
    uint16_t size;
    AT_SET_URL(1, 0);
    osDelay(pdMS_TO_TICKS(1000));
    AT_SET_GET(0, TID, 0, 0, "1.34");
    AT_Http_Read();
    PQ_Read(&message,str,&size);
    HAL_UART_Transmit(&huart1, str, strlen(str), 100);
    HAL_UART_Transmit(&huart1, "----------\r\n", 13, 100);
    str1=strstr((const char*)str, "succ");
    osDelay(pdMS_TO_TICKS(100));
    str2=AT_Recivejudge("+QHTTPREAD: 0");
    if((str1!=NULL)&&(str2!=NULL))
    {
        OTA_info.flag=1;
        HAL_UART_Transmit(&huart1, "检测到OTA任务\r\n", 21, 100);
        AT_GET_TID_VERSION_SIZE(OTA_info.OTA_VERSION, &OTA_info.FileLen[0],str);
        W25Q64_WRITE_OTA_INFO(&OTA_info);
    }
    

}

void OTA_DOWNLOAD(uint32_t Range_start,uint32_t Range_end)
{ 
    char*str1=NULL;
    uint8_t str[1500]={0},me[10];//11
    uint16_t size;
    AT_SET_URL(2, 0);
    osDelay(pdMS_TO_TICKS(1000));
    AT_SET_GET(1, TID, Range_start, Range_end, "1.34");
    AT_Http_Read();
    PQ_Read(&message,str,&size);
    HAL_UART_Transmit(&huart1, me, strlen(me), 100);
    HAL_UART_Transmit(&huart1, "----------\r\n", 13, 100);
    str1=AT_Recivejudge("+QHTTPREAD: 0");
    if(str1!=NULL)
    {
        HAL_UART_Transmit(&huart1, "正在下载\r\n", 15, 100);
        for(int i=0;i<4;i++)
        {
            W25QXX_Page_Program(&str[11+256*i], CODE_START_ADDR+i+Range_start/256, 256);
        }
        
    }
}


void OTA_PUT_UPADATE_INformation(uint8_t step)
{ 
    char*str1=NULL,*str2=NULL;
    AT_SET_URL(3, step);
    osDelay(pdMS_TO_TICKS(1000));
    AT_SET_POST(1, "1.34", TID, step);
    AT_Http_Read();
    str1=AT_Recivejudge("succ");
    str2=AT_Recivejudge("+QHTTPREAD: 0");
    if((str1!=NULL)&&(str2!=NULL))
    {
        HAL_UART_Transmit(&huart1, "上报升级进度成功\r\n", 27, 100);
        
    }
}