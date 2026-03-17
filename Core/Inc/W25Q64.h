#ifndef W25Q64_H
#define W25Q64_H

#ifdef __cplusplus
extern "C" {
#endif

/* 包含必要的头文件 */
#include "main.h"
#include "string.h"
#include "stdio.h"

/* 宏定义 */
#define W25Q64_CS_LOW()  HAL_GPIO_WritePin(W25Q64_CS_GPIO_Port, W25Q64_CS_Pin, GPIO_PIN_RESET)
#define W25Q64_CS_HIGH() HAL_GPIO_WritePin(W25Q64_CS_GPIO_Port, W25Q64_CS_Pin, GPIO_PIN_SET)
// W25Q64基本指令定义
#define ManufactDeviceID_CMD  0x90
#define READ_STATU_REGISTER_1 0x05
#define READ_STATU_REGISTER_2 0x35
#define READ_DATA_CMD         0x03
#define WRITE_ENABLE_CMD      0x06
#define WRITE_DISABLE_CMD     0x04
#define SECTOR_ERASE_CMD      0x20
#define BLOCK_ERASE_64K_CMD   0xD8 
#define CHIP_ERASE_CMD        0xc7
#define PAGE_PROGRAM_CMD      0x02
#define OTA_INFO_ADDR         0
#define OTA_INFO_BLOCK        0
#define CODE_START_BLOCK      1
#define CODE_START_ADDR       CODE_START_BLOCK*64*1024/256
/* 类型定义 */
extern OTA_INFO OTA_info;
/* 函数声明 */

uint16_t W25QXX_ReadID(void);                                           // 读取Flash内部的ID                           // 读取W25QXX的状态寄存器，W25Q64一共有2个状态寄存器
int W25QXX_Read(uint8_t *buffer, uint32_t start_addr, uint16_t nbytes); //
void W25QXX_Write_Enable(void);
void W25QXX_Write_Disable(void);
void W25QXX_Erase_Sector(uint32_t sector_addr);
void W25QXX_Erase_Block(uint32_t block_addr);
void W25QXX_Page_Program(uint8_t *dat, uint32_t WriteAddr, uint16_t nbytes);
void W25Q64_READ_OTA_INFO(OTA_INFO *flag);
void W25Q64_WRITE_OTA_INFO(OTA_INFO *flag);
#ifdef __cplusplus
}
#endif

#endif /* W25Q64_H */