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
#include "app_cfg.h"
#include "ucos_ii.h"


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
/* LED pins */
#define LED_PORT_1      GPIOB
#define LED_PORT_2			GPIOE
#define LED_PORT_3			GPIOE
#define LED_PORT_4			GPIOE
#define LED_PORT_5			GPIOE
#define LED_PORT_6			GPIOE
#define LED_PORT_7			GPIOE
#define LED_PORT_8			GPIOE

#define LED_PIN_1        	GPIO_Pin_9
#define LED_PIN_2        	GPIO_Pin_0
#define LED_PIN_3        	GPIO_Pin_1
#define LED_PIN_4        	GPIO_Pin_2
#define LED_PIN_5        	GPIO_Pin_3
#define LED_PIN_6        	GPIO_Pin_4
#define LED_PIN_7	      	GPIO_Pin_5
#define LED_PIN_8        	GPIO_Pin_6

/*		USART Port 		*/
#define USART1_PORT		GPIOA
#define USART1_TX		GPIO_Pin_9
#define USART1_RX		GPIO_Pin_10
//IIC Port
#define IIC1_PORT			GPIOB
#define IIC2_PORT 			GPIOB

#define IIC1_SCL			GPIO_Pin_6
#define IIC1_SDA			GPIO_Pin_7
#define IIC2_SCL			GPIO_Pin_10
#define IIC2_SDA			GPIO_Pin_11

#define SCL1_H				IIC1_PORT->BSRR	= IIC1_SCL
#define SCL1_L				IIC1_PORT->BRR 	= IIC1_SCL
#define SCL2_H				IIC2_PORT->BSRR	= IIC2_SCL
#define SCL2_L				IIC2_PORT->BRR 	= IIC2_SCL

#define SDA1_H				IIC1_PORT->BSRR	= IIC1_SDA
#define SDA1_L				IIC1_PORT->BRR 	= IIC1_SDA
#define SDA2_H				IIC2_PORT->BSRR	= IIC2_SDA
#define SDA2_L				IIC2_PORT->BRR 	= IIC2_SDA

#define SCL1_READ			IIC1_PORT->IDR	& IIC1_SCL
#define SCL2_READ			IIC2_PORT->IDR	& IIC2_SCL
#define SDA1_READ			IIC1_PORT->IDR	& IIC1_SDA
#define SDA2_READ			IIC2_PORT->IDR	& IIC2_SDA


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void BSP_Init (void);
int SendChar (int ch);
int GetKey(void);

void SPI_Config(void);
void RCC_Config(void);
void GPIO_Config(void);
void USART_Config(void);
void LedShow(int index,int state);

/*
*********************************************************************************************************
*                                           INTERRUPT SERVICES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     PERIPHERAL POWER/CLOCK SERVICES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              LED SERVICES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               PB SERVICES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                              ADC SERVICES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           JOYSTICK SERVICES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */
