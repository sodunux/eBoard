#include "dma.h"

void DMA_Uart1_Rx_Config(u8 *mem_addr,u32 len)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr	= (u32)mem_addr;
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

void DMA_Uart2_Rx_Config(u8 *mem_addr,u32 len)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr	= (u32)mem_addr;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M	= DMA_M2M_Disable;
	DMA_Init(DMA1_Channel6,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC|DMA_IT_TE,ENABLE);
	DMA_Cmd(DMA1_Channel6,ENABLE);
}


void DMA_Uart3_Rx_Config(u8 *mem_addr,u32 len)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART3->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr	= (u32)mem_addr;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M	= DMA_M2M_Disable;
	DMA_Init(DMA1_Channel3,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel3,DMA_IT_TC|DMA_IT_TE,ENABLE);
	DMA_Cmd(DMA1_Channel3,ENABLE);
}

void DMA_Uart1_Rx_Handler()
{
	USART_ClearFlag(USART1,USART_FLAG_ORE|USART_FLAG_TC|USART_FLAG_FE|USART_FLAG_PE);
	DMA_ClearFlag(DMA1_FLAG_TC5|DMA1_FLAG_GL5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);
	OSSemPost(uart_receive_1);
}
void DMA_Uart2_Rx_Handler()
{
	USART_ClearFlag(USART2,USART_FLAG_ORE|USART_FLAG_TC|USART_FLAG_FE|USART_FLAG_PE);
	DMA_ClearFlag(DMA1_FLAG_TC6|DMA1_FLAG_GL6|DMA1_FLAG_HT6|DMA1_FLAG_TE6);
	OSSemPost(uart_receive_2);	
}
void DMA_Uart3_Rx_Handler()
{
	USART_ClearFlag(USART3,USART_FLAG_ORE|USART_FLAG_TC|USART_FLAG_FE|USART_FLAG_PE);
	DMA_ClearFlag(DMA1_FLAG_TC3|DMA1_FLAG_GL3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);
	OSSemPost(uart_receive_3);
}
