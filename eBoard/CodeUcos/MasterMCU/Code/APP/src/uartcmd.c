#include "uartcmd.h"
u8 uart_tx_buff[128];
u8 uart_rx_buff[128];
u8 uart_tx_cnt=0;
u8 uart_rx_cnt=0;
extern OS_EVENT *uartcmdflag;

void Uart_Buff_Clear(void)
{
	int i;
	for(i=0;i<128;i++)
	{
		uart_tx_buff[i]=0x00;
		uart_rx_buff[i]=0x00;
	}
	uart_tx_cnt=0;
  	uart_rx_cnt=0;
}


void Uart_Cmd_Wait()
{
	Uart_Buff_Clear();
	USART_ClearFlag(USART1,USART_FLAG_ORE|USART_FLAG_TC|USART_FLAG_FE|USART_FLAG_PE);
	DMA_ClearFlag(DMA1_FLAG_TC5|DMA1_FLAG_GL5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);
	DMA_Uart1_Rx_Config(uart_rx_buff,0x05);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
}

/*******************************************************************************
* Function Name  : Uart_Cmd_Valid
* Description    : This function handle cmd in rxbuff
* Input          : None
* Output         : 0,error;1 OK;
* Return         : None
*******************************************************************************/
u8 Uart_Cmd_Valid()
{
	u8 i;
	u8 bccvalue=0;
	if((UART_CMD_CLA==0x55)||(UART_CMD_CLA==0xAA)||(UART_CMD_CLA==0xFF))
	{
		for(i=0;i<uart_rx_cnt;i++)
		{
			bccvalue^=uart_rx_buff[i];
		}
		if(bccvalue==0) 
			return 1;
		else 
			return 0;
	}
	return 0;
}

/*******************************************************************************
* Function Name  : Uart_DMAR_Handler
* Description    : This function handles UartRx DMA interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_DMAR_Handler()
{
	USART_ClearFlag(USART1,USART_FLAG_ORE|USART_FLAG_TC|USART_FLAG_FE|USART_FLAG_PE);
	DMA_ClearFlag(DMA1_FLAG_TC5|DMA1_FLAG_GL5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);
	
	if(UART_CMD_LEN)//exist cmd_data
	{
		if(UART_CMD_LEN>0x80)
		{
				uart_rx_buff[0]=0xEE;
				uart_rx_cnt=5;
				OSSemPost(uartcmdflag);
		}
		else
		{
			if(uart_rx_cnt==0)	
			{
				DMA_Uart1_Rx_Config(UART_CMD_DATAP,UART_CMD_LEN);
				USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
				uart_rx_cnt=5;
			}
			else
			{
				uart_rx_cnt=5+UART_CMD_LEN;
				OSSemPost(uartcmdflag);
			}	
		}
	}
	else
	{	
		uart_rx_cnt=5;
		OSSemPost(uartcmdflag);
	}
	
	
}
/*******************************************************************************
* Function Name  : Uart_DMAT_Handler
* Description    : This function handles UartTx DMA  interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_DMAT_Handler()
{
	USART_ClearFlag(USART1,USART_FLAG_ORE|USART_FLAG_TC|USART_FLAG_FE|USART_FLAG_PE);
	DMA_ClearFlag(DMA1_FLAG_TC4|DMA1_FLAG_GL4|DMA1_FLAG_HT4|DMA1_FLAG_TE4);
	
	
}
/*******************************************************************************
* Function Name  : Uart_Cmd_Parser
* Description    : This function Parser UartCmd from uart_rx_buff.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_Cmd_Parser(void)
{
	if((UART_CMD_CLA==0x55)||(UART_CMD_CLA==0xAA))
	{
		uart_tx_buff[0]=0x90;
		uart_tx_cnt=1;
		if(UART_CMD_CLA==0x55) //PC->Master
		{
			Pc_Master_Handler();
		}
		else //PC->Slave
		{
			Pc_Slave_Handler();
		}
		
	}
	else
	{
		uart_tx_buff[0]=0xE0;
		uart_tx_cnt=1;
	}

}

/*******************************************************************************
* Function Name  : DMA1_Channel5_IRQHandler
* Description    : This function send UartCmd Response to PC.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_Cmd_Resp()
{
	DMA_Uart1_Tx_Config(uart_tx_buff,uart_tx_cnt);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
}

