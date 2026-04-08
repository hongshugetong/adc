#include "loop.h"
PacketQueue_t message;
// 初始化
void PQ_Init(PacketQueue_t *pq)
{
    pq->w_pkt = 0;
    pq->r_pkt = 0;
    pq->count = 0;
    pq->buf_w = 0;
    pq->buf_r = 0;
    memset(pq->buffer, 0, PQ_BUFFER_SIZE);
    
    for (int i = 0; i < PQ_MAX_PACKETS; i++) {
        pq->packets[i].valid = false;
        pq->packets[i].len = 0;
    }
}

// 获取缓冲区可用空间
uint16_t PQ_FreeSpace(PacketQueue_t *pq)
{
    if (pq->buf_w >= pq->buf_r) {
        return PQ_BUFFER_SIZE - pq->buf_w;
    } else {
        return pq->buf_r - pq->buf_w;
    }
}

// 写入一包（中断调用）
bool PQ_Write(PacketQueue_t *pq, uint8_t *data, uint16_t len)
{
    // HAL_UART_Transmit(&huart1, data, len, HAL_MAX_DELAY);
    // HAL_UART_Transmit(&huart1, "*******************\r\n", 24, HAL_MAX_DELAY);
    if (pq->count >= PQ_MAX_PACKETS)
    {
        HAL_UART_Transmit(&huart1, "包数量已满\r\n",18 , 100);
        return false;
    }      // 包数量满
    if (len == 0 || len > PQ_MAX_PACKET_LEN) 
    {
        HAL_UART_Transmit(&huart1, "写入数据大小为0\r\n",25 , 100);
    }
    
    // 检查缓冲区空间
    if (len > PQ_FreeSpace(pq))
    {
        HAL_UART_Transmit(&huart1, "缓冲区空间不足\r\n",24 , 100);
        return false;  // 空间不足，丢弃
    }
    
    // 记录包信息
    Packet_t *pkt = &pq->packets[pq->w_pkt];
    pkt->offset = pq->buf_w;
    pkt->len = len;
    pkt->valid = true;
    
    // 复制数据
    memcpy(&pq->buffer[pq->buf_w], data, len);
    pq->buf_w += len;
    
    // 包索引循环
    pq->w_pkt = (pq->w_pkt + 1) % PQ_MAX_PACKETS;
    pq->count++;
    
    return true;
}

// 读取一包（任务调用）
bool PQ_Read(PacketQueue_t *pq, uint8_t *data, uint16_t *len)
{
    if (pq->count == 0)
    {
        //HAL_UART_Transmit(&huart1, "包数量为0\r\n",13 , 100);
        return false;
    } 
    Packet_t *pkt = &pq->packets[pq->r_pkt];
    if (!pkt->valid)
    {
        HAL_UART_Transmit(&huart1, "包无效\r\n",12 , 100);
        return false;
    } 
    
    // 复制数据
    memcpy(data, &pq->buffer[pkt->offset], pkt->len);
    *len = pkt->len;
    pkt->valid = false;
    pq->r_pkt = (pq->r_pkt + 1) % PQ_MAX_PACKETS;
    pq->count--;
    // 释放缓冲区空间（只有读取最老的包才能释放）
    if (pkt->offset == pq->buf_r) 
    {
        
        pq->buf_r += pkt->len;
        
        // 如果缓冲区已读超过一半，重置指针（回收空间）
        if (pq->buf_r > PQ_BUFFER_SIZE / 2)
        {
            HAL_UART_Transmit(&huart1, "正在释放缓冲区\r\n", 24, 100);
            PQ_Compress(pq);
        }
    }
    
    // 清除包描述符

    
    return true;
}

// 压缩缓冲区（回收已读空间）
void PQ_Compress(PacketQueue_t *pq)
{
    if (pq->count == 0) {
        // 没有包了，重置所有指针
        pq->buf_w = 0;
        pq->buf_r = 0;
        return;
    }
    
    // 获取第一个有效包的偏移
    uint16_t first_offset = pq->packets[pq->r_pkt].offset;
    
    if (first_offset > 0) {
        // 移动剩余有效数据到开头
        uint16_t move_len = pq->buf_w - first_offset;
        memmove(&pq->buffer[0], &pq->buffer[first_offset], move_len);
        
        // 更新所有有效包的偏移
        for (int i = 0; i < PQ_MAX_PACKETS; i++) {
            if (pq->packets[i].valid && pq->packets[i].offset >= first_offset) {
                pq->packets[i].offset -= first_offset;
            }
        }
        
        // 重置缓冲区指针
        pq->buf_w = move_len;
        pq->buf_r = 0;
    }
}

// 获取包数量
uint16_t PQ_Count(PacketQueue_t *pq)
{
    return pq->count;
}

// 判断是否为空
bool PQ_Empty(PacketQueue_t *pq)
{
    return (pq->count == 0);
}