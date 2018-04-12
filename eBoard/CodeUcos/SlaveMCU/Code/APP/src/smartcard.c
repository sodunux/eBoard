#include "smartcard.h"
u8 uartbuff_1[128];
u8 uartbuff_2[128];
u8 uartbuff_3[128];
u8 uart_rx_cnt_1;
u8 uart_tx_cnt_1;
u8 uart_rx_cnt_2;
u8 uart_tx_cnt_2;
u8 uart_rx_cnt_3;
u8 uart_tx_cnt_3;


/*******************************************************************************
* Function Name  : SC_Close
* Description    : Disable the 7816 Port
* Input          : - USARTn  USART1~USART3           
* Output         : None
* Return         : None
* Author     : Yanpengfei 

*******************************************************************************/
void SC_Close(USART_TypeDef* USARTn)
{
  //Close timing
  SC_DirCmd(USARTn,DIR_RECEIVE);
  SC_ResetCmd(USARTn,Bit_RESET);
  SC_ClkCmd(USARTn,DISABLE);
  SC_PowerCmd(USARTn,Bit_RESET);
  

}

/*******************************************************************************
* Function Name  : SC_Delay
* Description    : Delay Function
* Input          : None
* Output         : None
* Return         : None
* Author     : Yanpengfei 
*******************************************************************************/
void SC_Delay(u32 cnt)
{ u32 i;
  for(i=0;i<cnt;i++)
    __nop();
}


/*******************************************************************************
* Function Name  : SC_DirCmd
* Description    : Send(1) or receive(0)
* Input          : DIR_SEND, DIR_RECEIVE
*                : USARTn  USART1~USART3
* Output         : None
* Return         : None
* Author 		 : Yanpengfei
*******************************************************************************/
void SC_DirCmd(USART_TypeDef* USARTn,bool dir)
{	
  if(USARTn==USART1)
	{
    if(dir) //SEND
		{
			if(!GPIO_ReadOutputDataBit(SC_DIR1_PORT,SC_DIR1))
			{
				USART1->CR2&=(~USART_CR2_CLKEN); //disable SC_CLK
				//IO1:GPIOA9,GPIO_Mode_Out_PP
				GPIOA->CRH&=~((1<<6)|(1<<7));
				//GPIOPinxConfig(SC_IO1_PORT, SC_IO1,GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
				GPIO_SetBits(SC_DIR1_PORT, SC_DIR1); //SC_SEND
				USART1->CR2|=USART_CR2_CLKEN; //enable SC_CLK
			}		
		}
   	else
		{
			if(GPIO_ReadOutputDataBit(SC_DIR1_PORT,SC_DIR1))
			{
				USART1->CR2&=(~USART_CR2_CLKEN);
        //IO1:GPIOA9,GPIO_Mode_AF_OD
				GPIOA->CRH|=((1<<6)|(1<<7));
				GPIO_ResetBits(SC_DIR1_PORT,SC_DIR1);
				USART1->CR2|=USART_CR2_CLKEN;					
			}
		}     
	}

  	else if(USARTn==USART2)
  	{
    	if(dir) //SEND
		{
			if(!GPIO_ReadOutputDataBit(SC_DIR2_PORT,SC_DIR2))
			{
				//IO2:GPIOA2,GPIO_Mode_Out_PP
				USART2->CR2&=(~USART_CR2_CLKEN);
				GPIOA->CRL&=~((1<<10)|(1<<11));
				GPIO_SetBits(SC_DIR2_PORT, SC_DIR2);
				USART2->CR2|=USART_CR2_CLKEN;	
			}
			
		}
    	else
		{
			if(GPIO_ReadOutputDataBit(SC_DIR2_PORT,SC_DIR2))
			{
				//IO2:GPIOA2,GPIO_Mode_AF_OD
				USART2->CR2&=(~USART_CR2_CLKEN);
				GPIOA->CRL|=((1<<10)|(1<<11));
				GPIO_ResetBits(SC_DIR2_PORT,SC_DIR2);
				USART2->CR2|=USART_CR2_CLKEN;	
			}			
		}
  	}

  	else if(USARTn==USART3)
  	{
    	if(dir) //SEND
		{
			if(!GPIO_ReadOutputDataBit(SC_DIR3_PORT,SC_DIR3))
			{
				//IO3:GPIOB10,GPIO_Mode_Out_PP
				USART3->CR2&=(~USART_CR2_CLKEN);
				GPIOB->CRH&=~((1<<10)|(1<<11));
				GPIO_SetBits(SC_DIR3_PORT, SC_DIR3);
				USART3->CR2|=USART_CR2_CLKEN;	
			}
			
		}
    	else
		{
			if(GPIO_ReadOutputDataBit(SC_DIR3_PORT,SC_DIR3))
			{
				//IO3:GPIOB10,GPIO_Mode_AF_OD
				USART3->CR2&=(~USART_CR2_CLKEN);
				GPIOB->CRH|=((1<<10)|(1<<11));
				GPIO_ResetBits(SC_DIR3_PORT,SC_DIR3);
				USART3->CR2|=USART_CR2_CLKEN;
			}
			
		}
  	}
}

/*******************************************************************************
* Function Name  : SC_PowerCmd
* Description    : Enables or disables the power VCC to the Smartcard.
* Input          : NewState: new state of the Smartcard power supply. 
*                  This parameter can be: ENABLE(VCC=1) or DISABLE(VCC=0).
*                  - USARTn  USART1~USART3
* Output         : None
* Return         : None
* Author 		 : Yanpengfei
*******************************************************************************/
void SC_PowerCmd(USART_TypeDef* USARTn,BitAction NewState)
{
  if(USARTn==USART1)
    GPIO_WriteBit(SC_VCC1_PORT, SC_VCC1, NewState);
  else if(USARTn==USART2)
    GPIO_WriteBit(SC_VCC2_PORT, SC_VCC2, NewState); 
  else if(USARTn==USART3)
    GPIO_WriteBit(SC_VCC3_PORT, SC_VCC3, NewState);   
}

/*******************************************************************************
* Function Name  : SC_ResetCmd
* Description    : Sets or clears the Smartcard reset pin.
* Input          : - ResetState: this parameter specifies the state of the Smartcard 
*                    reset pin.
*                    BitVal must be one of the BitAction enum values:
*                       - Bit_RESET: to clear the port pin.
*                       - Bit_SET: to set the port pin.
*                  - USARTn  USART1~USART3
* Output         : None
* Return         : None
* Author 		 : Yanpengfei
*******************************************************************************/
void SC_ResetCmd(USART_TypeDef* USARTn,BitAction NewState)
{
  if(USARTn==USART1)
    GPIO_WriteBit(SC_RST1_PORT, SC_RST1, NewState);
  else if(USARTn==USART2)
    GPIO_WriteBit(SC_RST2_PORT, SC_RST2, NewState); 
  else if(USARTn==USART3)
    GPIO_WriteBit(SC_RST3_PORT, SC_RST3, NewState);   
}

/*******************************************************************************
* Function Name  : SC_ClkCmd
* Description    : Enable or Disable the 7816 clk
* Input          : - ResetState: this parameter specifies the state of the Smartcard 
*                    reset pin.
*                    BitVal must be one of the BitAction enum values:
*                       - Bit_RESET: to clear the port pin.
*                       - Bit_SET: to set the port pin.
*                  - USARTn  USART1~USART3
* Output         : None
* Return         : None
* Author 		 : Yanpengfei 
*******************************************************************************/

void SC_ClkCmd(USART_TypeDef* USARTn,FunctionalState NewState)
{
	if(USARTn==USART1)
	{
		if(NewState==DISABLE)
			USART1->CR2&=(~USART_CR2_CLKEN);
		else 
			USART1->CR2|=USART_CR2_CLKEN;

	}
	else if(USARTn==USART2)
	{
		if(NewState==DISABLE)
			USART2->CR2&=(~USART_CR2_CLKEN);
		else 
			USART2->CR2|=USART_CR2_CLKEN;		
	}
	else if(USARTn==USART3)
	{
		if(NewState==DISABLE)
			USART3->CR2&=(~USART_CR2_CLKEN);
		else 
			USART3->CR2|=USART_CR2_CLKEN;		
	}
}


/*******************************************************************************
* Function Name  : SC_Init
* Description    : Initializes all peripheral used for Smartcard interface.
* Input          : USARTn: USART1~USART3
* Output         : None
* Return         : None
* Author     : Yanpengfei 
*******************************************************************************/
void SC_Init(USART_TypeDef* USARTn)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
		USART_StructInit(&USART_InitStructure);
    //USART_InitStructure.USART_Mode=USART_Mode_Tx;
    USART_InitStructure.USART_Mode=USART_Mode_Rx;
    USART_InitStructure.USART_BaudRate =9677;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_Clock = USART_Clock_Enable;   
    SC_PowerCmd(USARTn,Bit_RESET);//VCC=0
    SC_ResetCmd(USARTn,Bit_RESET);//RST=0;
    SC_DirCmd(USARTn,DIR_RECEIVE); //IO receive mode 
		if(USARTn==USART1)
			USART_SetPrescaler(USARTn, 0x0A);
		else
			USART_SetPrescaler(USARTn, 0x05);
    USART_SetGuardTime(USARTn, 0);
    USART_Init(USARTn, &USART_InitStructure);
    USART_Cmd(USARTn, ENABLE);	
    USART_SmartCardNACKCmd(USARTn, DISABLE);     
		USART_SmartCardCmd(USARTn, ENABLE);
    uart_time_config(USARTn); 
}

/*******************************************************************************
* Function Name  : SC_ByteSend
* Description    : Send a Byte to Smartcard via USARTn.
* Input          : - A byte Data
                   - USARTn 1~3
* Output         : None
* Return         : None
* Author         : Yanpengfei
*******************************************************************************/
void SC_ByteSend(USART_TypeDef *USARTn,u8 locdata)
{
  u8 send_bit,send_even_parity,i;
  send_even_parity=0;
  for(i=0;i<8;i++)
    send_even_parity^=(locdata<<i)&0x80;
  
  if(USARTn==USART1)
  {
    SC_DirCmd(USART1,DIR_SEND);
    //Send StartBit
    uart_tim_flag_1=0;
    TIM_Cmd(TIM2,ENABLE);
    GPIO_WriteBit(SC_IO1_PORT,SC_IO1,Bit_RESET);
    while(uart_tim_flag_1==0);

    for(i=0;i<8;i++)//send bit0~bit7
    {
    uart_tim_flag_1=0;
    send_bit=locdata&(1<<i);//Calu Bit(i)
    GPIO_WriteBit(SC_IO1_PORT,SC_IO1,send_bit);
    while(uart_tim_flag_1==0);    
    }

    uart_tim_flag_1=0;//send parity bit 
    GPIO_WriteBit(SC_IO1_PORT,SC_IO1,send_even_parity);
    while(uart_tim_flag_1==0);

    for(i=0;i<2;i++)
    {
      uart_tim_flag_1=0;
      GPIO_WriteBit(SC_IO1_PORT,SC_IO1,Bit_SET);//send stop bit
      while(uart_tim_flag_1==0);
    }
    TIM_Cmd(TIM2,DISABLE);  
    SC_DirCmd(USART1,DIR_RECEIVE);
  }
  else if(USARTn==USART2)
  {
    SC_DirCmd(USART2,DIR_SEND);
    //GPIO_SetBits(SC_DIR1_PORT, SC_DIR1);
    //Send StartBit
    uart_tim_flag_2=0;
    TIM_Cmd(TIM3,ENABLE);
    GPIO_WriteBit(SC_IO2_PORT,SC_IO2,Bit_RESET);
    while(uart_tim_flag_2==0);

    for(i=0;i<8;i++)//send bit0~bit7
    {
    uart_tim_flag_2=0;
    send_bit=locdata&(1<<i);//Calu Bit(i)
    GPIO_WriteBit(SC_IO2_PORT,SC_IO2,send_bit);
    while(uart_tim_flag_2==0);    
    }

    uart_tim_flag_2=0;//send parity bit 
    GPIO_WriteBit(SC_IO2_PORT,SC_IO2,send_even_parity);
    while(uart_tim_flag_2==0);
    for(i=0;i<2;i++)
    {
      uart_tim_flag_2=0;
      GPIO_WriteBit(SC_IO2_PORT,SC_IO2,Bit_SET);//send stop bit
      while(uart_tim_flag_2==0);
    }
    TIM_Cmd(TIM3,DISABLE);  
    SC_DirCmd(USART2,DIR_RECEIVE);
  }
else if(USARTn==USART3)
  {
    SC_DirCmd(USART3,DIR_SEND);
    //GPIO_SetBits(SC_DIR1_PORT, SC_DIR1);
    //Send StartBit
    uart_tim_flag_3=0;
    TIM_Cmd(TIM4,ENABLE);
    GPIO_WriteBit(SC_IO3_PORT,SC_IO3,Bit_RESET);
    while(uart_tim_flag_3==0);

    for(i=0;i<8;i++)//send bit0~bit7
    {
    uart_tim_flag_3=0;
    send_bit=locdata&(1<<i);//Calu Bit(i)
    GPIO_WriteBit(SC_IO3_PORT,SC_IO3,send_bit);
    while(uart_tim_flag_3==0);    
    }

    uart_tim_flag_3=0;//send parity bit 
    GPIO_WriteBit(SC_IO3_PORT,SC_IO3,send_even_parity);
    while(uart_tim_flag_3==0);
    for(i=0;i<2;i++)
    {
      uart_tim_flag_3=0;
      GPIO_WriteBit(SC_IO3_PORT,SC_IO3,Bit_SET);//send stop bit
      while(uart_tim_flag_3==0);
    }
    TIM_Cmd(TIM4,DISABLE);  
    SC_DirCmd(USART3,DIR_RECEIVE);
  }  
}

/*******************************************************************************
* Function Name  : SC_ColdReset
* Description    : Operate a ColdRest action.
* Input          : - USARTn: USART1~USART3
				   - card:save the ATR to *card
				   - length: the max length of *card
* Output         : None
* Return         : None
* Author 		 : Yanpengfei 
*******************************************************************************/

void SC_ColdReset(USART_TypeDef *USARTn){
		//SC_Init(USARTn);
		SC_Close(USARTn);
		SC_Delay(1000);
		SC_PowerCmd(USARTn,Bit_SET);
		SC_ClkCmd(USARTn,ENABLE);
		SC_Delay(2000);
    SC_ResetCmd(USARTn,Bit_SET);
    if(USARTn==USART1)  
      SC_Transmit(USARTn,uartbuff_1,0,0x10);
    else if(USARTn==USART2)
      SC_Transmit(USARTn,uartbuff_2,0,0x10);
    else 
      SC_Transmit(USARTn,uartbuff_3,0,0x10);
}

void SC_Transmit(USART_TypeDef *USARTn,u8 *buff,u8 txlen,u8 rxlen)
{
  u8 i;
  for(i=0;i<txlen;i++)
  {
    SC_ByteSend(USARTn,buff[i]);
  }

  (void)USART_GetFlagStatus(USARTn,USART_FLAG_RXNE);//Clear USART_SR ORE NE FE PE
  (void)USART_ReceiveData(USARTn);//Clear DR
  USART_ClearFlag(USARTn,USART_FLAG_ORE|USART_FLAG_TC|USART_FLAG_FE|USART_FLAG_PE);
  if(USARTn==USART1)
  {
    DMA_ClearFlag(DMA1_FLAG_TC5|DMA1_FLAG_GL5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);
		SC_Delay(1000);
    DMA_Uart1_Rx_Config(buff,rxlen);
    //OSSemPend(uart_receive_1, 0, &err);
  }    
  else if(USARTn==USART2)
  {
    DMA_ClearFlag(DMA1_FLAG_TC6|DMA1_FLAG_GL6|DMA1_FLAG_HT6|DMA1_FLAG_TE6);
		SC_Delay(1000);
    DMA_Uart2_Rx_Config(buff,rxlen);
    //OSSemPend(uart_receive_2, 0, &err);
  }
  else {
    DMA_ClearFlag(DMA1_FLAG_TC3|DMA1_FLAG_GL3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);
		SC_Delay(1000);
    DMA_Uart3_Rx_Config(buff,rxlen);
    //OSSemPend(uart_receive_3, 0, &err);
  }
  USART_DMACmd(USARTn,USART_DMAReq_Rx,ENABLE);
}



