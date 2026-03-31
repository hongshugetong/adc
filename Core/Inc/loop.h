#ifndef __PACKET_QUEUE_H
#define __PACKET_QUEUE_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stdio.h"
// 配置参数
#define PQ_BUFFER_SIZE      4096       // 共享缓冲区 8KB
#define PQ_MAX_PACKETS      16          // 最多 16 个包
#define PQ_MAX_PACKET_LEN   1500        // 单包最大 1500 字节

// 包描述符
typedef struct {
    uint16_t offset;    // 缓冲区偏移
    uint16_t len;       // 包长度
    bool valid;         // 是否有效
} Packet_t;

// 队列结构
typedef struct {
    uint8_t buffer[PQ_BUFFER_SIZE];     // 共享缓冲区
    Packet_t packets[PQ_MAX_PACKETS];   // 包描述符
    
    uint16_t w_pkt;     // 包写索引（循环）
    uint16_t r_pkt;     // 包读索引（循环）
    uint16_t count;     // 包数量
    uint16_t buf_w;     // 缓冲区写位置
    uint16_t buf_r;     // 缓冲区读位置（已读位置）
} PacketQueue_t;

// 函数声明
void PQ_Init(PacketQueue_t *pq);
bool PQ_Write(PacketQueue_t *pq, uint8_t *data, uint16_t len);
bool PQ_Read(PacketQueue_t *pq, uint8_t *data, uint16_t *len);
uint16_t PQ_Count(PacketQueue_t *pq);
bool PQ_Empty(PacketQueue_t *pq);
uint16_t PQ_FreeSpace(PacketQueue_t *pq);
void PQ_Compress(PacketQueue_t *pq);

extern UART_HandleTypeDef huart1;

#endif