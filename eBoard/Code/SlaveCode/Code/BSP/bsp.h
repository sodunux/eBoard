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
* Filename      : bsp.h
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_PRESENT
#define  BSP_PRESENT


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/


#include "stm32f10x.h"
#include "smartcard.h"
#include "flash.h"
/*
*********************************************************************************************************
*                                               INT DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             PERIPH DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/
#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)  /*!< JTAG-DP Disabled and SW-DP Enabled */

/* LED Pins*/
#define RED1_PORT		GPIOB
#define GREEN1_PORT 	GPIOB
#define RED2_PORT		GPIOB
#define GREEN2_PORT		GPIOB
#define RED3_PORT		GPIOA
#define GREEN3_PORT		GPIOA

#define RED1 			GPIO_Pin_0
#define GREEN1   		GPIO_Pin_1
#define RED2 			GPIO_Pin_3
#define GREEN2 			GPIO_Pin_4
#define RED3 			GPIO_Pin_6
#define GREEN3 			GPIO_Pin_7

#define LED_ON			Bit_RESET
#define LED_OFF			Bit_SET

#define LED_SLAVE_Y	 0x01
#define LED_SLAVE_N  0x02
#define LED_PORT_Y	0x03
#define LED_PORT_N  0x04
#define LED_SLAVE_ALL 0x05


#define IIC1_PORT			GPIOB
#define IIC1_SCL			GPIO_Pin_6
#define IIC1_SDA			GPIO_Pin_7
#define IIC_ADDR			(14<<1)

typedef enum
{
	Port_Download_Y,
	Port_Download_N,
	Port_ATR_Y,
	Port_ATR_N,
	Port_Init_Y,
	Port_Init_N,
	Port_Done,
	Port_Running,
	Port_Err	
}PortStatus;




extern u8 IIC_StartFlag;
extern u8 IIC_StopFlag;	
		
extern u8 IIC_Buff[250];		
extern u8 IIC_BuffLen;
extern u8 auto_handler_flag;
extern u8 usart1_buff_ptr,usart2_buff_ptr,usart3_buff_ptr;
extern u8 usart1_buff[10],usart2_buff[10],usart3_buff[10];
extern PortStatus port1_status,port2_status,port3_status;
/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void BSP_Init (void);
int SendChar (int ch);
void LEDShow(GPIO_TypeDef* GPIOx, u16 GPIOPinx, BitAction BitVal);
void RCC_Config(void);
void GPIO_Config(void);
void NVIC_Config(void);
void IIC_Config(void);
void IIC_Hander(void);
void TIM_Config(void);
void LedSiganl(u8 cnt);
void Led_Flag_NoCard(u8 portn);//没有卡片
void Led_Flag_OK(u8 portn);//完成擦写
void Led_Flag_Err(u8 portn);//出现错误
void Led_Flag_Running(u8 portn);//正常运行

void OS_CPU_PendSVHandler(void);
/*
*********************************************************************************************************
*                                           INTERRUPT SERVICES
*********************************************************************************************************
*/


#endif                                                          /* End of module include.                               */
