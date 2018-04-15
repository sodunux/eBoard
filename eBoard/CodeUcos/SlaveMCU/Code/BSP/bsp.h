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



/* LED Pins*/
#define RED1_PORT		GPIOB
#define GREEN1_PORT 	GPIOB
#define RED3_PORT		GPIOB
#define GREEN3_PORT		GPIOB
#define RED2_PORT		GPIOA
#define GREEN2_PORT		GPIOA

#define RED1 			GPIO_Pin_0
#define GREEN1   		GPIO_Pin_1
#define RED3 			GPIO_Pin_3
#define GREEN3 			GPIO_Pin_4
#define RED2 			GPIO_Pin_6
#define GREEN2 			GPIO_Pin_7
#define LED_ON			Bit_RESET
#define LED_OFF			Bit_SET

#define IIC1_PORT			GPIOB
#define IIC1_SCL			GPIO_Pin_6
#define IIC1_SDA			GPIO_Pin_7
#define IIC_ADDR			(14<<1)

/* SmartCard Ports */
#define SC_CLK1_PORT  GPIOA
#define SC_IO1_PORT   GPIOA
#define SC_DIR1_PORT  GPIOA
#define SC_RST1_PORT  GPIOA
#define SC_VCC1_PORT  GPIOA

#define SC_CLK2_PORT  GPIOA
#define SC_IO2_PORT   GPIOA
#define SC_DIR2_PORT  GPIOA
#define SC_RST2_PORT  GPIOA
#define SC_VCC2_PORT  GPIOA

#define SC_CLK3_PORT  GPIOB
#define SC_IO3_PORT   GPIOB
#define SC_DIR3_PORT  GPIOB
#define SC_RST3_PORT  GPIOB
#define SC_VCC3_PORT  GPIOB

#define SC_CLK1     GPIO_Pin_8
#define SC_IO1      GPIO_Pin_9
#define SC_DIR1     GPIO_Pin_10
#define SC_RST1     GPIO_Pin_11
#define SC_VCC1     GPIO_Pin_12

#define SC_CLK2     GPIO_Pin_4
#define SC_IO2      GPIO_Pin_2
#define SC_DIR2     GPIO_Pin_1
#define SC_RST2     GPIO_Pin_3
#define SC_VCC2     GPIO_Pin_5  

#define SC_CLK3     GPIO_Pin_12
#define SC_IO3      GPIO_Pin_10
#define SC_DIR3     GPIO_Pin_11
#define SC_RST3     GPIO_Pin_13
#define SC_VCC3     GPIO_Pin_9

#define DIR_SEND    1
#define DIR_RECEIVE 0

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
void GPIOPinxConfig(GPIO_TypeDef* GPIOx, INT16U GPIOPinx, GPIOSpeed_TypeDef Speed, GPIOMode_TypeDef Mode);
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
