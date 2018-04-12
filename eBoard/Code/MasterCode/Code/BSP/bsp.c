/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_MODULE
#include "includes.h"
#include "bsp.h"
#include "program.h"


//u32 SystemCoreClock;

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/
volatile ErrorStatus HSEStartUpStatus = SUCCESS;
extern uint8_t bUsbStatus;
u8 t2_count_ms,t3_count_ms;
DpyStruct dpy_show_data;
u8 usart_tx_buff[256],usart_rx_buff[256];
u8 usart_tx_ptr,usart_rx_ptr;
/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void RCC_Config(void);
void SysTick_Configuration(void);
void GPIO_Config(void);
void NVIC_Config(void);
void USART_Config(void);
/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
	CosInfStruct inf;
	RCC_Config();
	//SysTick_Configuration();
	GPIO_Config();
	NVIC_Config();
	USART_Config();
	Tim2Cmd(1,ENABLE);
	program_sel=SwReadByte();
	//初始化dpy_show_data
	dpy_show_data.bit1_flag=0xE;
	dpy_show_data.low_4bit=0;
	//初始化usart buff
	usart_rx_ptr=0;
	usart_tx_ptr=0;
	LedSignal(3);
}



/*
*********************************************************************************************************
*                                            Tim2Start()
*
*********************************************************************************************************
*/
void Tim2Cmd(u16 ms,FunctionalState NewState)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period=ms*10-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,NewState);

}

void Tim3Cmd(u16 ms,FunctionalState NewState)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period=ms*10-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,NewState);

}


/*
*********************************************************************************************************
*                                            SysTick_Configuration()
*
*********************************************************************************************************
*/
void SysTick_Configuration(void)
{
    RCC_ClocksTypeDef  RCC_Clocks;
    u32  cnts;
    RCC_GetClocksFreq(&RCC_Clocks);   
    cnts = (u32)RCC_Clocks.HCLK_Frequency/OS_TICKS_PER_SEC;
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
		SysTick_Config(cnts);
}

/*******************************************************************************
* Function Name  : IIC_Delay
* Description    : IIC Delay time
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */

void IIC_Delay(void)
{
		
   //int i=1000;  
  int i=50; 
	while(i) 
   { 
     i--; 
   }  
}


/*******************************************************************************
* Function Name  : IIC_Start
* Description    : Master Start Simulation IIC Communication
* Input          : 1 or 2
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
bool IIC_Start(u8 IICn)
{

	if(IICn==1)
	{
		SDA1_H;
		SCL1_H;
		IIC_Delay();
		if(!SDA1_READ)return FALSE;	//SDA线为低电平则总线忙,退出
		SDA1_L;
		IIC_Delay();
		if(SDA1_READ) return FALSE;	//SDA线为高电平则总线出错,退出
		SDA1_L;
		IIC_Delay();
		return TRUE;
	}
	else
	{
		SDA2_H;
		SCL2_H;
		IIC_Delay();
		if(!SDA2_READ)return FALSE;	//SDA线为低电平则总线忙,退出
		SDA2_L;
		IIC_Delay();
		if(SDA2_READ) return FALSE;	//SDA线为高电平则总线出错,退出
		SDA2_L;
		IIC_Delay();
		return TRUE;
	}
}


/*******************************************************************************
* Function Name  : IIC_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : 1 or 2
* Output         : None
* Return         : None
****************************************************************************** */
void IIC_Stop(u8 IICn)
{
	if(IICn==1)
	{
		SCL1_L;
		IIC_Delay();
		SDA1_L;
		IIC_Delay();
		SCL1_H;
		IIC_Delay();
		SDA1_H;
		IIC_Delay();
	}
	else 
	{
		SCL2_L;
		IIC_Delay();
		SDA2_L;
		IIC_Delay();
		SCL2_H;
		IIC_Delay();
		SDA2_H;
		IIC_Delay();
	}
} 

/*******************************************************************************
* Function Name  : IIC_Ack
* Description    : Master Send Acknowledge Signal
* Input          : 1 or 2
* Output         : None
* Return         : None
****************************************************************************** */
void IIC_Ack(u8 IICn)
{	
	if(IICn==1)
	{
		SCL1_L;
		IIC_Delay();
		SDA1_L;
		IIC_Delay();
		SCL1_H;
		IIC_Delay();
		SCL1_L;
		IIC_Delay(); 
	}
	else
	{
		SCL2_L;
		IIC_Delay();
		SDA2_L;
		IIC_Delay();
		SCL2_H;
		IIC_Delay();
		SCL2_L;
		IIC_Delay(); 
	}
}
/*******************************************************************************
* Function Name  : IIC_NoAck
* Description    : Master Send No Acknowledge Signal
* Input          : 1 or 2
* Output         : None
* Return         : None
****************************************************************************** */
void IIC_NoAck(u8 IICn)
{
	if(IICn==1)
	{
		SCL1_L;
		IIC_Delay();
		SDA1_H;
		IIC_Delay();
		SCL1_H;
		IIC_Delay();
		SCL1_L;
		IIC_Delay();
	}
	else
	{
		SCL2_L;
		IIC_Delay();
		SDA2_H;
		IIC_Delay();
		SCL2_H;
		IIC_Delay();
		SCL2_L;
		IIC_Delay();
	}
}

 

/*******************************************************************************
* Function Name  : IIC_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : 1 or 2
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
bool IIC_WaitAck(u8 IICn) 	 
{
	if(IICn==1)
	{
		SCL1_L;
		IIC_Delay();
		SDA1_H;			
		IIC_Delay();
		SCL1_H;
		IIC_Delay();
		if(SDA1_READ)
		{
     	 	SCL1_L;
	  		IIC_Delay();
     	 	return FALSE;
		}
		SCL1_L;
		IIC_Delay();
		return TRUE;
	}
	else
	{
		SCL2_L;
		IIC_Delay();
		SDA2_H;			
		IIC_Delay();
		SCL2_H;
		IIC_Delay();
		if(SDA1_READ)
		{
     	 	SCL2_L;
	  		IIC_Delay();
     	 	return FALSE;
		}

		SCL2_L;
		IIC_Delay();
		return TRUE;		
	}


}


/*******************************************************************************
* Function Name  : IIC_SendByte
* Description    : Master Send a Byte to Slave
* Input          : IIC1 or IIC2,Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void IIC_SendByte(u8 IICn,u8 sendbyte) //MSB first
{
    u8 i=8;
    if(IICn==1)
    {
    	while(i--)
    	{
      		SCL1_L;
       	 	IIC_Delay();
     		if(sendbyte&0x80)
        		SDA1_H;  
      		else 
       	 		SDA1_L;   
        	sendbyte<<=1;
        	IIC_Delay();
					SCL1_H;
       		IIC_Delay();
    	}
    	SCL1_L;
    }
    else
    {
    	while(i--)
    	{
      		SCL2_L;
       	 	IIC_Delay();
     		if(sendbyte&0x80)
        		SDA2_H;  
      		else 
       	 		SDA2_L;   
        	sendbyte<<=1;
        	IIC_Delay();
			SCL2_H;
       		IIC_Delay();
    	}
    	SCL2_L;
    }
}


/*******************************************************************************
* Function Name  : IIC_ReadByte
* Description    : Master Reserive a Byte From Slave
* Input          : 1 or 2
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
u8 IIC_ReadByte(u8 IICn) 
{ 
    u8 i=8;
    u8 receivebyte=0;

    if(IICn==1)
    {
    	SDA1_H;				
    	while(i--)
    	{
      		receivebyte<<=1;      
      		SCL1_L;
      		IIC_Delay();
	  		SCL1_H;
      		IIC_Delay();	
      		if(SDA1_READ)
      		{
        		receivebyte|=0x01;
      		}
    	}
    	SCL1_L;  	
    }
    else
    {
    	SDA2_H;				
    	while(i--)
    	{
      		receivebyte<<=1;      
      		SCL2_L;
      		IIC_Delay();
	  		SCL2_H;
      		IIC_Delay();	
      		if(SDA2_READ)
      		{
        		receivebyte|=0x01;
      		}
    	}
    	SCL2_L; 
    }
    return receivebyte;
} 



/*******************************************************************************
* Function Name  : IIC_SendData
* Description    : Send Data via IIC
* Input          : addr:0x00~0xFF
									 senddata:data buff address
									 len:senddata length
* Return         : true:
											opertion OK.
									 false:
											opertion failed.
*******************************************************************************/
bool IIC_SendData(u8 addr,u8 *senddata,u8 len)
{
		//1~15 IIC1
		//16~30 IIC2
		bool flag;
		u8 i,bccvalue,IICn;
		u8 *ptr=senddata;
		
		if(((addr>>1)>15)&&((addr>>1)<31))
			IICn=2;
		else
			IICn=1;
		
		IIC_Start(IICn);
		IIC_SendByte(IICn,addr);
		flag=IIC_WaitAck(IICn);
		if(!flag) 
			return 0;
		for(i=0;i<len;i++)
		{
			IIC_SendByte(IICn,*ptr);
			flag=IIC_WaitAck(IICn);
			if(!flag) 
				return 0;
			ptr++;
		}
		
		IIC_Stop(IICn);
		return 1;
	
}

/*******************************************************************************
* Function Name  : IIC_SendData
* Description    : Read Data via IIC
* Input          : addr:0x00~0xFF
									 Read:data buff address
									 len:senddata length
* Return         : true:
											opertion OK.
									 false:
											opertion failed.
*******************************************************************************/
bool IIC_ReadData(u8 addr,u8 *readbuf,u8 len)
{
	u8 *ptr=readbuf;
	u8 i,IICn;
	bool flag;

	if(((addr>>1)>15)&&((addr>>1)<31))
		IICn=2;
	else
		IICn=1;		
	IIC_Start(IICn);
	IIC_SendByte(IICn,addr|0x01);
	flag=IIC_WaitAck(IICn);
	if(!flag) 
			return 0;
	for(i=0;i<(len-1);i++)
	{
		*ptr=IIC_ReadByte(IICn);
		IIC_Ack(IICn);
		ptr++;
	}
	*ptr=IIC_ReadByte(IICn);
	IIC_NoAck(IICn);
	IIC_Stop(IICn);
	return 1;
}

/*******************************************************************************
* Function Name  : USART_Config
* Description    : Configures USART
* Input          : None.
* Return         : None.
*******************************************************************************/
void USART_Config(void)
{

	USART_InitTypeDef USART_InitStructure;
	USART_DeInit(USART1);	
//	USART_InitStructure.USART_BaudRate				=	9600;
	USART_InitStructure.USART_BaudRate				=	115200;
	USART_InitStructure.USART_WordLength			=	USART_WordLength_9b;
	USART_InitStructure.USART_StopBits				=	USART_StopBits_1;
	USART_InitStructure.USART_Parity				=	USART_Parity_Odd;
//	USART_InitStructure.USART_Parity				=	USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;		
	USART_InitStructure.USART_Mode						=	USART_Mode_Tx|USART_Mode_Rx;

	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

	USART_Cmd(USART1,ENABLE);

}

/*******************************************************************************
* Function Name  : RCC_Config
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void RCC_Config(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_IIC1|RCC_APB1Periph_IIC2,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE,ENABLE);
  	
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }
}


/*******************************************************************************
* Function Name  : GPIOPinxConfig
* Description    : Configures one GPIO pin used by MCU.
* Input          : GPIOx    : GPIO port.
                   GPIOPinx : GPIO pin.
                   Speed    : GPIO Pin speed.
                   Mode     : GPIO Pin mode.
* Return         : None.
*******************************************************************************/
void GPIOPinxConfig(GPIO_TypeDef* GPIOx, INT16U GPIOPinx, GPIOSpeed_TypeDef Speed, GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIOPinx;
	GPIO_InitStructure.GPIO_Speed = Speed;
	GPIO_InitStructure.GPIO_Mode = Mode;
	GPIO_Init(GPIOx, &GPIO_InitStructure);      
}

/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : Configures GPIOs used by MCU.
* Input          : None.
* Return         : None.
*******************************************************************************/
void GPIO_Config(void)
{
	
	/* Configure pins */
	GPIOPinxConfig(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN, GPIO_Speed_10MHz, GPIO_Mode_Out_OD);
	GPIO_SetBits(USB_DISCONNECT_PORT, USB_DISCONNECT_PIN);

	// Config LED Pins	
	GPIOPinxConfig(LED_PORT_1, LED_PIN_1, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(LED_PORT_2, LED_PIN_2, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(LED_PORT_3, LED_PIN_3, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(LED_PORT_4, LED_PIN_4, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(LED_PORT_5, LED_PIN_5, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(LED_PORT_6, LED_PIN_6, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(LED_PORT_7, LED_PIN_7, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(LED_PORT_8, LED_PIN_8, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);

	//Config DIP Switch
	GPIOPinxConfig(SW_PORT, SW_PIN_1, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING);
	GPIOPinxConfig(SW_PORT, SW_PIN_2, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING);
	GPIOPinxConfig(SW_PORT, SW_PIN_3, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING);
	GPIOPinxConfig(SW_PORT, SW_PIN_4, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING);	
	GPIOPinxConfig(SW_PORT, SW_PIN_5, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING);	
	GPIOPinxConfig(SW_PORT, SW_PIN_6, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING);	
	GPIOPinxConfig(SW_PORT, SW_PIN_7, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING);	
	GPIOPinxConfig(SW_PORT, SW_PIN_8, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING);
	
	//Config Dpy Port
	GPIOPinxConfig(DPY_A_PORT, DPY_A, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_B_PORT, DPY_B, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_C_PORT, DPY_C, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_D_PORT, DPY_D, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_E_PORT, DPY_E, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_F_PORT, DPY_F, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_G_PORT, DPY_G, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_DP_PORT, DPY_DP, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);

	
	GPIOPinxConfig(DPY_COM1_PORT, DPY_COM1, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_COM2_PORT, DPY_COM2, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_COM3_PORT, DPY_COM3, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_COM4_PORT, DPY_COM4, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_COM5_PORT, DPY_COM5, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_COM6_PORT, DPY_COM6, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_COM7_PORT, DPY_COM7, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(DPY_COM8_PORT, DPY_COM8, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);

	//Config USART Port 
	
	GPIOPinxConfig(USART1_PORT,USART1_TX,GPIO_Speed_10MHz,GPIO_Mode_AF_PP);
	GPIOPinxConfig(USART1_PORT,USART1_RX,GPIO_Speed_10MHz,GPIO_Mode_IN_FLOATING);

	//Config IIC Ports

	GPIOPinxConfig(IIC1_PORT,IIC1_SCL,GPIO_Speed_2MHz,GPIO_Mode_Out_OD);
	GPIOPinxConfig(IIC1_PORT,IIC1_SDA,GPIO_Speed_2MHz,GPIO_Mode_Out_OD);

}

/*******************************************************************************
* Function Name  : LEDShow.
* Description    : LED pins level output.
* Input          : Index      :   LED Index 1~8.
                   BitAction  : Bit_RESET or Bit_SET.
* Output         : None.
* Return         : None.
*******************************************************************************/
void LEDShow(INT8U Index, BitAction BitVal)
{
	if(BitVal == Bit_SET)  
	{
	if(Index == 1)
	  GPIO_SetBits(LED_PORT_1, LED_PIN_1);
	else if(Index == 2)
	  GPIO_SetBits(LED_PORT_2, LED_PIN_2);      
	else if(Index == 3)
	  GPIO_SetBits(LED_PORT_3, LED_PIN_3);
	else if(Index == 4)
	  GPIO_SetBits(LED_PORT_4, LED_PIN_4);
	else if(Index == 5)
	  GPIO_SetBits(LED_PORT_5, LED_PIN_5);
	else if(Index == 6)
	  GPIO_SetBits(LED_PORT_6, LED_PIN_6);
	else if(Index == 7)
	  GPIO_SetBits(LED_PORT_7, LED_PIN_7);
	else if(Index == 8)
	  GPIO_SetBits(LED_PORT_8, LED_PIN_8);
	}
	else
	{
	if(Index == 1)
	  GPIO_ResetBits(LED_PORT_1, LED_PIN_1);
	else if(Index == 2)
	  GPIO_ResetBits(LED_PORT_2, LED_PIN_2);      
	else if(Index == 3)
	  GPIO_ResetBits(LED_PORT_3, LED_PIN_3);
	else if(Index == 4)
	  GPIO_ResetBits(LED_PORT_4, LED_PIN_4);
	else if(Index == 5)
	  GPIO_ResetBits(LED_PORT_5, LED_PIN_5);
	else if(Index == 6)
	  GPIO_ResetBits(LED_PORT_6, LED_PIN_6);  
	else if(Index == 7)
	  GPIO_ResetBits(LED_PORT_7, LED_PIN_7);
	else if(Index == 8)
	  GPIO_ResetBits(LED_PORT_8, LED_PIN_8);  	    

	}
    
}


u8 SwReadByte()
{
	return (u8)GPIO_ReadInputData(SW_PORT);
}

/*******************************************************************************
* Function Name  : SWRead.
* Description    : Read in SW input.
* Input          : SW index 1~8
* Output         : SW input status.
* Return         : None.
*******************************************************************************/
u8 SwReadBit(u8 Index)
{

	u8 sw_status;
	switch(Index)
	{
		case 1: 
				sw_status=GPIO_ReadInputDataBit(SW_PORT,SW_PIN_1);break;
		case 2: 
				sw_status=GPIO_ReadInputDataBit(SW_PORT,SW_PIN_2);break;
		case 3: 
				sw_status=GPIO_ReadInputDataBit(SW_PORT,SW_PIN_3);break;
		case 4: 
				sw_status=GPIO_ReadInputDataBit(SW_PORT,SW_PIN_4);break;
		case 5: 
				sw_status=GPIO_ReadInputDataBit(SW_PORT,SW_PIN_5);break;
		case 6: 
				sw_status=GPIO_ReadInputDataBit(SW_PORT,SW_PIN_6);break;
		case 7: 
				sw_status=GPIO_ReadInputDataBit(SW_PORT,SW_PIN_7);break;
		case 8: 
				sw_status=GPIO_ReadInputDataBit(SW_PORT,SW_PIN_8);break;
		default:
				sw_status=0;break;


	}
	return sw_status;
}

/*******************************************************************************
* Function Name  : ButtonRead.
* Description    : Read in Buttons status.
* Input          : UP_BUTTON,DOWN_BUTTON,LEFT_BUTTON,RIGHT_BUTTON,ENTER_BUTTON
* Output         : buttons status.
* Return         : None.
*******************************************************************************/
u8 ButtonRead(u16 button)
{
	u8 readvalue;
	switch(button)
	{
		case UP_BUTTON:
			readvalue=GPIO_ReadInputDataBit(BUTTON_PORT_UP,UP_BUTTON);break;
		case LEFT_BUTTON:
			readvalue=GPIO_ReadInputDataBit(BUTTON_PORT_LEFT,LEFT_BUTTON);break;	
		case ENTER_BUTTON:
			readvalue=GPIO_ReadInputDataBit(BUTTON_PORT_ENTER,ENTER_BUTTON);break;					
		case RIGHT_BUTTON:
			readvalue=GPIO_ReadInputDataBit(BUTTON_PORT_RIGHT,RIGHT_BUTTON);break;	
		case DOWN_BUTTON:
			readvalue=GPIO_ReadInputDataBit(BUTTON_PORT_DOWN,DOWN_BUTTON);break;
		default:
			readvalue=0;break;
	}
	return readvalue;
}




u32 CRC_Calu(u32 startaddr,u32 endaddr)
{
	u32 rtn,tmpdat,tmpaddr;
	CRC_ResetDR();
	for(tmpaddr=startaddr;tmpaddr<endaddr;tmpaddr=tmpaddr+4)
	{
		tmpdat=*((u32 *)tmpaddr);
		rtn=CRC_CalcCRC(tmpdat);
	}
	return rtn;
}



bool Flash_Erase(u32 page_addr)
{
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(page_addr);
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);	
	FLASH_Lock();
	return 0;
}



bool Flash_Program(u8 *sour_data,u32 dest_addr,u32 len)
{
	u32 wordlen,leftcnt;
	u32 tailword,tmpword;
	u32 iptr=0;
	u32 tmp;
	u8 b1,b2,b3,b4;
	FLASH_Status status = FLASH_COMPLETE;
	if(dest_addr%4)
		return 1;
	wordlen=len/4;
	leftcnt=len-wordlen*4;
	switch(leftcnt)
	{
		case 0:
			tailword=0xFFFFFFFF;
			break;
		
		case 1:
			b1=*(sour_data+wordlen*4+0);
			tailword=b1|0xFFFFFF00;
			break;
		
		case 2:
			b1=*(sour_data+wordlen*4+0);
			b2=*(sour_data+wordlen*4+1);
			tailword=b1|(b2<<8)|0xFFFF0000;
			break;
		
		case 3:
			b1=*(sour_data+wordlen*4+0);
			b2=*(sour_data+wordlen*4+1);
			b3=*(sour_data+wordlen*4+2);
			tailword=b1|(b2<<8)|(b3<<16)|0xFF000000;
			break;
		
		default:
			break;
	}
	
	//FLASH_Unlock();
	//FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	while(iptr<wordlen)
	{
		b1=*(sour_data+iptr*4+0);
		b2=*(sour_data+iptr*4+1);
		b3=*(sour_data+iptr*4+2);
		b4=*(sour_data+iptr*4+3);
		FLASH_Unlock();
		//FLASH_SetLatency(FLASH_Latency_2);
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
		tmpword=(b4<<24)+(b3<<16)+(b2<<8)+b1;	
		status=FLASH_ProgramWord((dest_addr+iptr*4),tmpword);
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
		FLASH_Lock();
		iptr++;
	}
	
	if(leftcnt)
	{
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
		FLASH_ProgramWord((dest_addr+iptr*4),tailword);
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
		FLASH_Lock();
	}
	


	return 0;
}



/*******************************************************************************
* Function Name  : DpyShowBit.
* Description    : Dpy show information.
* Input          : Dpy index 1~8
				   Dpy showdata 0x00~0x0F,else data:dpy off	
				   dot: 0-none dot; 1-show dot;
* Output         : buttons status.
* Return         : None.
*******************************************************************************/
void DpyShowBit(u8 Index,u8 showdata,u8 dot)
{

		GPIOE->ODR|=0xFF00; //Set all com
	

	if(dot){

		GPIO_SetBits(DPY_DP_PORT,DPY_DP);
	}
	else 
		GPIO_ResetBits(DPY_DP_PORT,DPY_DP);				

	switch(showdata)
	{
		case 0:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_B_PORT,DPY_B);
			GPIO_SetBits(DPY_C_PORT,DPY_C);
			GPIO_SetBits(DPY_D_PORT,DPY_D);
			GPIO_SetBits(DPY_E_PORT,DPY_E);
			GPIO_SetBits(DPY_F_PORT,DPY_F);
			GPIO_ResetBits(DPY_G_PORT,DPY_G);
			GPIO_ResetBits(DPY_DP_PORT,DPY_DP);
			break;
		case 1:
			GPIO_ResetBits(DPY_A_PORT,DPY_A);
//		GPIO_ResetBits(DPY_B_PORT,DPY_B);
//		GPIO_ResetBits(DPY_C_PORT,DPY_C);
			GPIO_ResetBits(DPY_D_PORT,DPY_D);
			GPIO_ResetBits(DPY_E_PORT,DPY_E);
			GPIO_ResetBits(DPY_F_PORT,DPY_F);
			GPIO_ResetBits(DPY_G_PORT,DPY_G);
			GPIO_ResetBits(DPY_DP_PORT,DPY_DP);
			GPIO_SetBits(DPY_B_PORT,DPY_B);
			GPIO_SetBits(DPY_C_PORT,DPY_C);
			break;
		case 2:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_B_PORT,DPY_B);
			GPIO_SetBits(DPY_G_PORT,DPY_G);
			GPIO_SetBits(DPY_E_PORT,DPY_E);
			GPIO_SetBits(DPY_D_PORT,DPY_D);		
//		GPIO_ResetBits(DPY_A_PORT,DPY_A);
//		GPIO_ResetBits(DPY_B_PORT,DPY_B);
			GPIO_ResetBits(DPY_C_PORT,DPY_C);
//		GPIO_ResetBits(DPY_D_PORT,DPY_D);
//		GPIO_ResetBits(DPY_E_PORT,DPY_E);
			GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);

			break;
		case 3:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_B_PORT,DPY_B);
			GPIO_SetBits(DPY_C_PORT,DPY_C);
			GPIO_SetBits(DPY_D_PORT,DPY_D);
			GPIO_SetBits(DPY_G_PORT,DPY_G);		
//		GPIO_ResetBits(DPY_A_PORT,DPY_A);
//		GPIO_ResetBits(DPY_B_PORT,DPY_B);
//		GPIO_ResetBits(DPY_C_PORT,DPY_C);
//		GPIO_ResetBits(DPY_D_PORT,DPY_D);
			GPIO_ResetBits(DPY_E_PORT,DPY_E);
			GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);

			break;
		case 4:
			GPIO_SetBits(DPY_F_PORT,DPY_F);
			GPIO_SetBits(DPY_G_PORT,DPY_G);
			GPIO_SetBits(DPY_B_PORT,DPY_B);
			GPIO_SetBits(DPY_C_PORT,DPY_C);			
			GPIO_ResetBits(DPY_A_PORT,DPY_A);
//		GPIO_ResetBits(DPY_B_PORT,DPY_B);
//		GPIO_ResetBits(DPY_C_PORT,DPY_C);
			GPIO_ResetBits(DPY_D_PORT,DPY_D);
			GPIO_ResetBits(DPY_E_PORT,DPY_E);
//		GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);

		
			break;
		case 5:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_F_PORT,DPY_F);
			GPIO_SetBits(DPY_G_PORT,DPY_G);
			GPIO_SetBits(DPY_C_PORT,DPY_C);
			GPIO_SetBits(DPY_D_PORT,DPY_D);	
//		GPIO_ResetBits(DPY_A_PORT,DPY_A);
			GPIO_ResetBits(DPY_B_PORT,DPY_B);
//		GPIO_ResetBits(DPY_C_PORT,DPY_C);
//		GPIO_ResetBits(DPY_D_PORT,DPY_D);
			GPIO_ResetBits(DPY_E_PORT,DPY_E);
//		GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);

			break;
		case 6:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_F_PORT,DPY_F);
			GPIO_SetBits(DPY_G_PORT,DPY_G);
			GPIO_SetBits(DPY_C_PORT,DPY_C);
			GPIO_SetBits(DPY_D_PORT,DPY_D);
			GPIO_SetBits(DPY_E_PORT,DPY_E);
		
//		GPIO_ResetBits(DPY_A_PORT,DPY_A);
			GPIO_ResetBits(DPY_B_PORT,DPY_B);
//		GPIO_ResetBits(DPY_C_PORT,DPY_C);
//		GPIO_ResetBits(DPY_D_PORT,DPY_D);
//		GPIO_ResetBits(DPY_E_PORT,DPY_E);
//		GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);

			break;
		case 7:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_B_PORT,DPY_B);
			GPIO_SetBits(DPY_C_PORT,DPY_C);
//		GPIO_ResetBits(DPY_A_PORT,DPY_A);
//		GPIO_ResetBits(DPY_B_PORT,DPY_B);
//		GPIO_ResetBits(DPY_C_PORT,DPY_C);
			GPIO_ResetBits(DPY_D_PORT,DPY_D);
			GPIO_ResetBits(DPY_E_PORT,DPY_E);
			GPIO_ResetBits(DPY_F_PORT,DPY_F);
			GPIO_ResetBits(DPY_G_PORT,DPY_G);

			break;
		case 8:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_B_PORT,DPY_B);
			GPIO_SetBits(DPY_C_PORT,DPY_C);
			GPIO_SetBits(DPY_D_PORT,DPY_D);
			GPIO_SetBits(DPY_E_PORT,DPY_E);
			GPIO_SetBits(DPY_F_PORT,DPY_F);
			GPIO_SetBits(DPY_G_PORT,DPY_G);
		
//		GPIO_ResetBits(DPY_A_PORT,DPY_A);
//		GPIO_ResetBits(DPY_B_PORT,DPY_B);
//		GPIO_ResetBits(DPY_C_PORT,DPY_C);
//		GPIO_ResetBits(DPY_D_PORT,DPY_D);
//		GPIO_ResetBits(DPY_E_PORT,DPY_E);
//		GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);
			break;
		case 9:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_B_PORT,DPY_B);
			GPIO_SetBits(DPY_C_PORT,DPY_C);
			GPIO_SetBits(DPY_D_PORT,DPY_D);
			GPIO_SetBits(DPY_F_PORT,DPY_F);
			GPIO_SetBits(DPY_G_PORT,DPY_G);
		
//		GPIO_ResetBits(DPY_A_PORT,DPY_A);
//		GPIO_ResetBits(DPY_B_PORT,DPY_B);
//		GPIO_ResetBits(DPY_C_PORT,DPY_C);
//		GPIO_ResetBits(DPY_D_PORT,DPY_D);
			GPIO_ResetBits(DPY_E_PORT,DPY_E);
//		GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);
			break;	
		case 0x0A:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_B_PORT,DPY_B);
			GPIO_SetBits(DPY_C_PORT,DPY_C);
			GPIO_SetBits(DPY_E_PORT,DPY_E);
			GPIO_SetBits(DPY_F_PORT,DPY_F);
			GPIO_SetBits(DPY_G_PORT,DPY_G);
	
//		GPIO_ResetBits(DPY_A_PORT,DPY_A);
//		GPIO_ResetBits(DPY_B_PORT,DPY_B);
//		GPIO_ResetBits(DPY_C_PORT,DPY_C);
			GPIO_ResetBits(DPY_D_PORT,DPY_D);
//		GPIO_ResetBits(DPY_E_PORT,DPY_E);
//		GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);
			break;
		case 0x0B:
			GPIO_SetBits(DPY_C_PORT,DPY_C);
			GPIO_SetBits(DPY_D_PORT,DPY_D);
			GPIO_SetBits(DPY_E_PORT,DPY_E);
			GPIO_SetBits(DPY_F_PORT,DPY_F);
			GPIO_SetBits(DPY_G_PORT,DPY_G);
		
			GPIO_ResetBits(DPY_A_PORT,DPY_A);
			GPIO_ResetBits(DPY_B_PORT,DPY_B);
//		GPIO_ResetBits(DPY_C_PORT,DPY_C);
//		GPIO_ResetBits(DPY_D_PORT,DPY_D);
//		GPIO_ResetBits(DPY_E_PORT,DPY_E);
//		GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);
			break;		
		case 0x0C:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_D_PORT,DPY_D);
			GPIO_SetBits(DPY_E_PORT,DPY_E);
			GPIO_SetBits(DPY_F_PORT,DPY_F);

//		GPIO_ResetBits(DPY_A_PORT,DPY_A);
			GPIO_ResetBits(DPY_B_PORT,DPY_B);
			GPIO_ResetBits(DPY_C_PORT,DPY_C);
//		GPIO_ResetBits(DPY_D_PORT,DPY_D);
//		GPIO_ResetBits(DPY_E_PORT,DPY_E);
//		GPIO_ResetBits(DPY_F_PORT,DPY_F);
			GPIO_ResetBits(DPY_G_PORT,DPY_G);
			break;
		case 0x0D:
			
			GPIO_SetBits(DPY_B_PORT,DPY_B);
			GPIO_SetBits(DPY_C_PORT,DPY_C);
			GPIO_SetBits(DPY_D_PORT,DPY_D);
			GPIO_SetBits(DPY_E_PORT,DPY_E);
			GPIO_SetBits(DPY_G_PORT,DPY_G);
		
			GPIO_ResetBits(DPY_A_PORT,DPY_A);
//		GPIO_ResetBits(DPY_B_PORT,DPY_B);
//		GPIO_ResetBits(DPY_C_PORT,DPY_C);
//		GPIO_ResetBits(DPY_D_PORT,DPY_D);
//		GPIO_ResetBits(DPY_E_PORT,DPY_E);
			GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);
			break;
		case 0x0E:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_D_PORT,DPY_D);
			GPIO_SetBits(DPY_E_PORT,DPY_E);
			GPIO_SetBits(DPY_F_PORT,DPY_F);
			GPIO_SetBits(DPY_G_PORT,DPY_G);
//		GPIO_ResetBits(DPY_A_PORT,DPY_A);
			GPIO_ResetBits(DPY_B_PORT,DPY_B);
			GPIO_ResetBits(DPY_C_PORT,DPY_C);
//		GPIO_ResetBits(DPY_D_PORT,DPY_D);
//		GPIO_ResetBits(DPY_E_PORT,DPY_E);
//		GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);
			break;
		case 0x0F:
			GPIO_SetBits(DPY_A_PORT,DPY_A);
			GPIO_SetBits(DPY_E_PORT,DPY_E);
			GPIO_SetBits(DPY_F_PORT,DPY_F);
			GPIO_SetBits(DPY_G_PORT,DPY_G);
//		GPIO_ResetBits(DPY_A_PORT,DPY_A);
			GPIO_ResetBits(DPY_B_PORT,DPY_B);
			GPIO_ResetBits(DPY_C_PORT,DPY_C);
			GPIO_ResetBits(DPY_D_PORT,DPY_D);
//		GPIO_ResetBits(DPY_E_PORT,DPY_E);
//		GPIO_ResetBits(DPY_F_PORT,DPY_F);
//		GPIO_ResetBits(DPY_G_PORT,DPY_G);
			break;
		default:
			break;
	}
	
	switch(Index)
	{
		case 1:
			GPIO_ResetBits(DPY_COM1_PORT,DPY_COM1);
			break;
		case 2: 
			GPIO_ResetBits(DPY_COM2_PORT,DPY_COM2);
			break;
		case 3: 
			GPIO_ResetBits(DPY_COM3_PORT,DPY_COM3);
			break;
		case 4:
	
			GPIO_ResetBits(DPY_COM4_PORT,DPY_COM4);
			break;
		case 5: 
			GPIO_ResetBits(DPY_COM5_PORT,DPY_COM5);
			break;
		case 6: 
			GPIO_ResetBits(DPY_COM6_PORT,DPY_COM6);
			break;
		case 7: 
			GPIO_ResetBits(DPY_COM7_PORT,DPY_COM7);
			break;
		case 8: 
			GPIO_ResetBits(DPY_COM8_PORT,DPY_COM8);
			break;
		default: 
			GPIOE->ODR|=0xFF00; //Set all com
			break;
	}

}


/*******************************************************************************
* Function Name  : NVIC_Config
* Description    : Configures the interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	// USART
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// TIM2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	//TIM3
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
}

void LedSignal(u8 cnt)
{
	u32 i,j;
	for(i=0;i<cnt;i++)
	{
		LEDShow(1,Bit_RESET);
		LEDShow(2,Bit_RESET);
		LEDShow(3,Bit_RESET);
		LEDShow(4,Bit_RESET);
		LEDShow(5,Bit_RESET);
		LEDShow(6,Bit_RESET);
		LEDShow(7,Bit_RESET);
		LEDShow(8,Bit_RESET);
		for(j=0;j<1000000;j++)
			__nop();
		LEDShow(1,Bit_SET);
		LEDShow(2,Bit_SET);
		LEDShow(3,Bit_SET);
		LEDShow(4,Bit_SET);
		LEDShow(5,Bit_SET);
		LEDShow(6,Bit_SET);
		LEDShow(7,Bit_SET);
		LEDShow(8,Bit_SET);
		for(j=0;j<1000000;j++)
			__nop();		
	}
}

int SendChar (int ch)  //used in retarget.c
{
	while (!(USART3->SR & USART_FLAG_TXE)); // USART1 ¿É»»³ÉÄã³ÌÐòÖÐÍ¨ÐÅµÄ´®¿Ú
	USART3->DR = (ch & 0x1FF);
	return (ch);
}

int GetKey (void)			//used in retarget.c
{
	while (!(USART3->SR & USART_FLAG_RXNE));
	return ((int)(USART3->DR & 0x1FF));
}

void OS_CPU_PendSVHandler(){}
