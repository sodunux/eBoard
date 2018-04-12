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
#include "stm32f10x.h"
#include "bsp.h"
#include "smartcard.h"



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
volatile ErrorStatus HSEStartUpStatus = SUCCESS;



PortStatus port1_status,port2_status,port3_status;

u8 IIC_StartFlag=0; //IIC开始通讯，地址匹配
u8 IIC_StopFlag=0;	//IIC通讯结束
//u8 IIC_RxOrTX;			//RX=0;TX=1;
u8 IIC_Buff[250];		//IIC缓冲区
u8 IIC_BuffLen;			//IIC缓冲区中记录的长度
u8 usart1_buff_ptr,usart2_buff_ptr,usart3_buff_ptr;
u8 usart1_buff[10],usart2_buff[10],usart3_buff[10];
u8 auto_handler_flag;
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
	RCC_Config();
//	SysTick_Configuration();
	GPIO_Config();
	NVIC_Config();
	IIC_Config();
	TIM_Config();
	SC_Init(USART1);
	SC_Init(USART2);
	SC_Init(USART3);	
	LedSiganl(3);
}



void LedSiganl(u8 cnt)
{
	u32 i,j;
	for(j=0;j<cnt;j++)
	{		
			LEDShow(GREEN1_PORT,GREEN1,LED_ON);
			LEDShow(GREEN2_PORT,GREEN2,LED_ON);
			LEDShow(GREEN3_PORT,GREEN3,LED_ON);
			LEDShow(RED1_PORT,RED1,LED_ON);
			LEDShow(RED2_PORT,RED2,LED_ON);
			LEDShow(RED3_PORT,RED3,LED_ON);		
			for(i=0;i<1000000;i++)
				__nop();
			LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
			LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
			LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
			LEDShow(RED1_PORT,RED1,LED_OFF);
			LEDShow(RED2_PORT,RED2,LED_OFF);
			LEDShow(RED3_PORT,RED3,LED_OFF);
			for(i=0;i<1000000;i++)
				__nop();				
	}
}

void Led_Flag_OK(u8 portn)
{
	switch(portn)
	{
		case 0x01:
			LEDShow(GREEN1_PORT,GREEN1,LED_ON);
			LEDShow(RED1_PORT,RED1,LED_OFF);
			break;
		case 0x02:
			LEDShow(GREEN2_PORT,GREEN2,LED_ON);
			LEDShow(RED2_PORT,RED2,LED_OFF);			
			break;
		case 0x03:
			LEDShow(GREEN3_PORT,GREEN3,LED_ON);
			LEDShow(RED3_PORT,RED3,LED_OFF);
			break;
		default:
			break;
	}
	
}


void Led_Flag_NoCard(u8 portn)
{
	switch(portn)
	{
		case 0x01:
			LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
			LEDShow(RED1_PORT,RED1,LED_ON);
			break;
		case 0x02:
			LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
			LEDShow(RED2_PORT,RED2,LED_ON);			
			break;
		case 0x03:
			LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
			LEDShow(RED3_PORT,RED3,LED_ON);
			break;
		default:
			break;
	}
}

void Led_Flag_Err(u8 portn)
{
	static u8 red_led1=0,red_led2=0,red_led3=0;
	
	LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
	LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
	LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
	switch(portn)
	{
		case 0x01:
			if(red_led1)
			{
				LEDShow(RED1_PORT,RED1,LED_OFF);
				red_led1=0;
			}
			else
			{
				LEDShow(RED1_PORT,RED1,LED_ON);
				red_led1=1;
			}
			break;
		case 0x02:
			if(red_led2)
			{
				LEDShow(RED2_PORT,RED2,LED_OFF);
				red_led1=0;
			}
			else
			{
				LEDShow(RED2_PORT,RED2,LED_ON);
				red_led1=1;
			}	
			break;
		case 0x03:
			if(red_led3)
			{
				LEDShow(RED3_PORT,RED3,LED_OFF);
				red_led3=0;
			}
			else
			{
				LEDShow(RED3_PORT,RED3,LED_ON);
				red_led3=1;
			}	
			break;
		default:
			break;
	}	
}

void Led_Flag_Running(u8 portn)
{
	static u8 green_led1=0,green_led2=0,green_led3=0;
	
	LEDShow(RED1_PORT,RED1,LED_OFF);
	LEDShow(RED2_PORT,RED2,LED_OFF);
	LEDShow(RED3_PORT,RED3,LED_OFF);
	switch(portn)
	{
		case 0x01:
			if(green_led1)
			{
				LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
				green_led1=0;
			}
			else
			{
				LEDShow(GREEN1_PORT,GREEN1,LED_ON);
				green_led1=1;
			}
			break;
		case 0x02:
			if(green_led2)
			{
				LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
				green_led1=0;
			}
			else
			{
				LEDShow(GREEN2_PORT,GREEN2,LED_ON);
				green_led1=1;
			}	
			break;
		case 0x03:
			if(green_led3)
			{
				LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
				green_led3=0;
			}
			else
			{
				LEDShow(GREEN3_PORT,GREEN3,LED_ON);
				green_led3=1;
			}
			break;
		default:
			break;
	}	
}


void TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period=1;
	TIM_TimeBaseStructure.TIM_Prescaler = 10*372-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	//TIM_Cmd(TIM2,ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}


/*
*********************************************************************************************************
*                                            IIC_Config()
*
*********************************************************************************************************
*/
void IIC_Config()
{
	INF_STRUCT inf_data;
	I2C_InitTypeDef I2C_InitStructure;
	InfStruct_Read(&inf_data);	
	I2C_StructInit(&I2C_InitStructure);
	I2C_InitStructure.I2C_Mode 			=	I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1	=	IIC_ADDR;
	I2C_InitStructure.I2C_Ack		= 	I2C_Ack_Enable;
	I2C_Init(I2C1,&I2C_InitStructure);
	I2C_StretchClockCmd(I2C1,DISABLE); //Disable Pull Down SCL 
	I2C_ITConfig(I2C1,I2C_IT_EVT|I2C_IT_BUF,ENABLE);
	I2C_Cmd(I2C1,ENABLE);
}

/*
*********************************************************************************************************
*                                            IIC_Hander()
*描述：IIC作为Slave的中断程序
*********************************************************************************************************
*/
void IIC_Hander(void)
{
	static u8 IIC_BuffPtr;
	static u8 tx_or_rx;
	//Clear Error Flag
	//tx_or_rx=I2C_GetFlagStatus(I2C1,I2C_FLAG_TRA);
	I2C_ClearFlag(I2C1,I2C_FLAG_SMBALERT|I2C_FLAG_TIMEOUT|I2C_FLAG_PECERR|I2C_FLAG_OVR|I2C_FLAG_BERR|I2C_FLAG_AF);
	//tx_or_rx=0;
	//检测到地址
	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_ADDR))
	{
		tx_or_rx=I2C_GetFlagStatus(I2C1,I2C_FLAG_TRA);
		(void)(I2C1->SR2);
		 IIC_StartFlag=1;
		 IIC_BuffPtr=0; 
	}
	
//	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_AF)) //从发送器
//	{
//		if(tx_or_rx)
//		{
//			IIC_BuffLen=0;
//		}
//		I2C_ClearFlag(I2C1,I2C_FLAG_AF);
//	}
	
	//检测到STOP位
	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_STOPF)) //从接收器
	{
		I2C_Cmd(I2C1,ENABLE);
		IIC_StopFlag=1;	
		//接受数据时，记录下接受下多少数据
		IIC_BuffLen=IIC_BuffPtr;
	}
	
	//接受字节
	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE))
	{
		IIC_Buff[IIC_BuffPtr]=I2C_ReceiveData(I2C1);
		IIC_BuffPtr++;
	}
	
	//发送字节
	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE))
	{
		if(IIC_BuffPtr<IIC_BuffLen)
		{
			I2C_SendData(I2C1,IIC_Buff[IIC_BuffPtr]);
			IIC_BuffPtr++;
		}
		else
		{
			I2C_SendData(I2C1,0x55);
			IIC_BuffLen=0;
		}
	}
	
}
/*
*********************************************************************************************************
*                                            SysTick_Configuration()
*
*********************************************************************************************************
*/
//void SysTick_Configuration(void)
//{
//    RCC_ClocksTypeDef  RCC_Clocks;
//    u32  cnts;
//    RCC_GetClocksFreq(&RCC_Clocks);   
//    cnts = (u32)RCC_Clocks.HCLK_Frequency/OS_TICKS_PER_SEC;
//		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
//		SysTick_Config(cnts);
//}

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
    RCC_PCLK2Config(RCC_HCLK_Div2); 

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
	//smartcard port using usart3
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		
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
void GPIOPinxConfig(GPIO_TypeDef* GPIOx, u16 GPIOPinx, GPIOSpeed_TypeDef Speed, GPIOMode_TypeDef Mode)
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
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //disable JTAG
	// Config LED Pins	
	GPIOPinxConfig(RED1_PORT, 	RED1, 	GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(GREEN1_PORT,	GREEN1, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(RED2_PORT, 	RED2, 	GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(GREEN2_PORT, GREEN2, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(RED3_PORT, 	RED3, 	GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(GREEN3_PORT, GREEN3, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);	

	/*	Config SmartCard Port */
	GPIOPinxConfig(SC_VCC1_PORT, SC_VCC1, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_RST1_PORT, SC_RST1, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_DIR1_PORT, SC_DIR1, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	//GPIOPinxConfig(SC_IO1_PORT , SC_IO1,  GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	//GPIOPinxConfig(SC_CLK1_PORT, SC_CLK1,  GPIO_Speed_2MHz, GPIO_Mode_Out_PP);

	GPIOPinxConfig(SC_VCC2_PORT, SC_VCC2, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_RST2_PORT, SC_RST2, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_DIR2_PORT, SC_DIR2, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	//GPIOPinxConfig(SC_IO2_PORT , SC_IO2,  GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
	//GPIOPinxConfig(SC_CLK2_PORT, SC_CLK2, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);

	GPIOPinxConfig(SC_VCC3_PORT, SC_VCC3, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_RST3_PORT, SC_RST3, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_DIR3_PORT, SC_DIR3, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	//GPIOPinxConfig(SC_IO3_PORT , SC_IO3,  GPIO_Speed_2MHz, GPIO_Mode_Out_PP);	
	//GPIOPinxConfig(SC_CLK3_PORT, SC_CLK3, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);

	/* Config IIC Port */
	GPIOPinxConfig(IIC1_PORT,IIC1_SCL,GPIO_Speed_2MHz,GPIO_Mode_AF_OD);
	GPIOPinxConfig(IIC1_PORT,IIC1_SDA,GPIO_Speed_2MHz,GPIO_Mode_AF_OD);

}

/*******************************************************************************
* Function Name  : LEDShow.
* Description    : LED pins level output.
* Input          : Index      :   LED Index 1~8.
                   BitAction  : Bit_RESET or Bit_SET.
* Output         : None.
* Return         : None.
*******************************************************************************/
void LEDShow(GPIO_TypeDef* GPIOx, u16 GPIOPinx, BitAction BitVal)
{
	if(BitVal == Bit_SET)  
	  GPIO_SetBits(GPIOx, GPIOPinx);
	else
	  GPIO_ResetBits(GPIOx, GPIOPinx);  	    
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//NVIC_SetPriority(SysTick_IRQn, 0); 

	//SC_ClkCnt	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	//
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
void OS_CPU_PendSVHandler(void){}
