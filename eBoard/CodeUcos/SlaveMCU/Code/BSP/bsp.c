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
	RCC_Config();
	SysTick_Configuration();
	GPIO_Config();
	NVIC_Config();
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
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);		
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
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //disable JTAG
	
	// Config LED Pins	
	GPIOPinxConfig(RED1_PORT, 	RED1, 	GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(GREEN1_PORT,	GREEN1, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(RED2_PORT, 	RED2, 	GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(GREEN2_PORT, GREEN2, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(RED3_PORT, 	RED3, 	GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(GREEN3_PORT, GREEN3, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	

	/*	Config SmartCard Port */
	GPIOPinxConfig(SC_VCC1_PORT, SC_VCC1, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_RST1_PORT, SC_RST1, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_DIR1_PORT, SC_DIR1, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(SC_IO1_PORT , SC_IO1,  GPIO_Speed_50MHz, GPIO_Mode_AF_PP); //the main State
	GPIOPinxConfig(SC_CLK1_PORT, SC_CLK1, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);

	GPIOPinxConfig(SC_VCC2_PORT, SC_VCC2, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_RST2_PORT, SC_RST2, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_DIR2_PORT, SC_DIR2, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(SC_IO2_PORT , SC_IO2,  GPIO_Speed_50MHz, GPIO_Mode_AF_PP); //the main state
	GPIOPinxConfig(SC_CLK2_PORT, SC_CLK2, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);

	GPIOPinxConfig(SC_VCC3_PORT, SC_VCC3, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_RST3_PORT, SC_RST3, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);	
	GPIOPinxConfig(SC_DIR3_PORT, SC_DIR3, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	GPIOPinxConfig(SC_IO3_PORT , SC_IO3,  GPIO_Speed_50MHz, GPIO_Mode_AF_PP); //the main state
	GPIOPinxConfig(SC_CLK3_PORT, SC_CLK3, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);

	/* Config IIC Port */
	GPIOPinxConfig(IIC1_PORT,IIC1_SCL,GPIO_Speed_50MHz,GPIO_Mode_AF_OD);
	GPIOPinxConfig(IIC1_PORT,IIC1_SDA,GPIO_Speed_50MHz,GPIO_Mode_AF_OD);	
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
	
	//SC_BitCnt	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//SC_BitCnt
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//SC_BitCnt
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    //Uart1 DMA RX
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;             
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 

    //Uart2 DMA RX
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;             
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
	
    //Uart3 DMA RX
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;             
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 

    //IIC1 
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	NVIC_SetPriority(SysTick_IRQn, 0);  
}


int SendChar (int ch)  //used in retarget.c
{
	//while (!(USART3->SR & USART_FLAG_TXE)); // USART1 可换成你程序中通信的串口
	//USART3->DR = (ch & 0x1FF);
	//return (ch);
	return 0;
}

int GetKey (void)			//used in retarget.c
{
	//while (!(USART3->SR & USART_FLAG_RXNE));
	//return ((int)(USART3->DR & 0x1FF));
	return 0;
}


