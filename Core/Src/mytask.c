#include "mytask.h"

// 交换两个uint16_t值的函数
void swap_uint16(uint16_t *a, uint16_t *b)
{
    uint16_t temp = *a;
    *a            = *b;
    *b            = temp;
}

// 快速排序的分区函数
uint16_t partition(uint16_t arr[], uint16_t low, uint16_t high)
{
    uint16_t pivot = arr[high];
    int16_t i      = low - 1;

    for (uint16_t j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap_uint16(&arr[i], &arr[j]);
        }
    }
    if (i >= 0) {
        swap_uint16(&arr[i + 1], &arr[high]);
        return i + 1;
    }
    return 0;
}

// 快速排序实现
void quickSort(uint16_t arr[], uint16_t low, uint16_t high)
{
    if (low < high) {
        uint16_t pi = partition(arr, low, high);
        if (pi > 0) {
            quickSort(arr, low, pi - 1);
        }
        quickSort(arr, pi + 1, high);
    }
}

// 处理数组，剔除前1%的极小值和后1%的极大值后求平均
float processArray(uint16_t *arr, uint16_t size)
{
    if (size <= 0) {
        return 0.0;
    }

    // 创建数组副本以避免修改原数组
    uint16_t *tempArr = (uint16_t *)malloc(size * sizeof(uint16_t));
    if (tempArr == NULL) {
        return 0.0; // 内存分配失败
    }

    for (uint16_t i = 0; i < size; i++) {
        tempArr[i] = arr[i];
    }

    // 对数组进行排序
    if (size > 1) {
        quickSort(tempArr, 0, size - 1);
    }

    // 计算需要剔除的元素数量（1%）
    uint16_t removeCount = size / 100;
    if (removeCount == 0 && size > 1) {
        removeCount = 1; // 如果数组长度大于1但1%为0，则至少剔除1个
    }

    // 计算剔除极值后的数组范围
    uint16_t startIndex = removeCount;
    uint16_t endIndex   = size - removeCount - 1;

    // 如果剔除后没有剩余元素，则返回0
    if (startIndex > endIndex) {
        free(tempArr);
        return 0.0;
    }

    // 计算剩余元素的总和
    uint32_t sum = 0;
    for (uint16_t i = startIndex; i <= endIndex; i++) {
        sum += tempArr[i];
    }

    // 计算平均值
    uint16_t remainingCount = endIndex - startIndex + 1;
    float average          = (float)sum / remainingCount;

    free(tempArr);
    return average;
}
// 将12位AD转换数据转换为float类型（0-3.3V范围）
float ad12_to_float(uint16_t ad_value)
{
    // 检查输入值是否在12位范围内（0-4095）
    if (ad_value > 4095) {
        ad_value = 4095; // 限制在最大值
    }
    // 将12位AD值转换为float电压值
    // 12位AD的最大值是4095，对应满量程电压
    // 假设参考电压为3.3V
    float voltage = (float)ad_value * 3.3f / 4095.0f;

    return voltage;
}
void StartDataprocessTask(void *argument)
{
    /* USER CODE BEGIN StartDataprocessTask */
    /* Infinite loop */
    uint16_t average;
    float data;
    for (;;) {
        //osSemaphoreAcquire(processHandle, osWaitForever);
        average = processArray(ADC_data, 10);
        data    = ad12_to_float(average);
        osMessageQueuePut(Adcqueue01Handle, &data, 0, osWaitForever);
    }
    /* USER CODE END StartDataprocessTask */
}

void StartUsart1Task(void *argument)
{
    /* USER CODE BEGIN StartUsart1Task */
    /* Infinite loop */
    // float data          = 30;
    // uint8_t message[20] = {0};
    for (;;) {
    //     osMessageQueueGet(Adcqueue01Handle, &data, 0, osWaitForever);
    //     sprintf(message, "ADC:%0.4f", data);
    //     HAL_UART_Transmit(&huart1, message, sizeof(message), HAL_MAX_DELAY);
        osDelay(1000);
    }
    /* USER CODE END StartUsart1Task */
}
void StartBlueTask(void *argument)
{
    /* USER CODE BEGIN StartBlueTask */
    /* Infinite loop */
    float data;
    uint8_t message[20] = {0};
    for (;;) {

        osMessageQueueGet(Adcqueue01Handle, &data, 0, osWaitForever);
        sprintf(message, "Blue ADC: %0.4f", data);
        HAL_UART_Transmit(&huart4, message, sizeof(message), 100);
        osDelay(1000);
    }
    /* USER CODE END StartBlueTask */
}

void StartEC200task(void *argument)
{
  /* USER CODE BEGIN StartEC200task */
    float adc_value;
    // EC20_Init();
    // MQTT_Init();
    // osDelay( pdMS_TO_TICKS(10000));
    // AT_setMQTT_version(1);
    // osDelay( pdMS_TO_TICKS(1000));
    // AT_OpenMQTT();
    // osDelay( pdMS_TO_TICKS(1000));
    // AT_ConnectMQTT();
    // osDelay( pdMS_TO_TICKS(1000));
//   /* Infinite loop */
  for(;;)
  {
    // osMessageQueueGet(Adcqueue01Handle, &adc_value, 0, osWaitForever);
    // AT_Publish_MQTT(adc_value);
    osDelay(pdMS_TO_TICKS(10000));
  }
  /* USER CODE END StartEC200task */
}