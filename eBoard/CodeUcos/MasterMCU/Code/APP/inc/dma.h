#ifndef __DMA_H__
#define __DMA_H__
#include "includes.h"
void DMA_Uart1_Tx_Config(u8 *mem_addr,u32 len);
void DMA_Uart1_Rx_Config(u8 *mem_addr,u32 len);


#endif