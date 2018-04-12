#include "dma.h"
void DMA_Uart1_Tx_Config(u8 *mem_addr,u32 len)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr	= (uint32_t) mem_addr;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M	= DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC|DMA_IT_TE,ENABLE);
	DMA_Cmd(DMA1_Channel4,ENABLE);
}


void DMA_Uart1_Rx_Config(u8 *mem_addr,u32 len)
{
	u32 temp = (u32)mem_addr;
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr	= (u32)mem_addr;
	DMA_InitStructure.DMA_MemoryBaseAddr	= (uint32_t)temp;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M	= DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC|DMA_IT_TE,ENABLE);
	DMA_Cmd(DMA1_Channel5,ENABLE);
}
	