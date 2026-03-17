#include "OTA.h"

//用来获取是否有OTA任务的标志
void OTA_GET_OTAFlag(void)
{
    //从W25Q64中获取OTA_INFO
    W25Q64_READ_OTA_INFO(&OTA_info);
    AT_SET_URL(0, 0);
    osDelay(pdMS_TO_TICKS(1000));
    AT_SET_POST(0, OTA_info.OTA_VERSION, 0, 0);
    AT_Http_Read();
    Readystrx=AT_Recivejudge("succ");
    if(Readystrx!=NULL)
    {
        HAL_UART_Transmit(&huart1, "上报版本号成功\r\n", 24, 100);
    }
}