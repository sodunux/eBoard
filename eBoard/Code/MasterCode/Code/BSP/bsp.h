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

  /* USB disconnect pin */
#define USB_DISCONNECT_PORT GPIOA
#define USB_DISCONNECT_PIN  GPIO_Pin_8

/* LED pins */
#define LED_PORT_1         	GPIOB
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

#define LED_ON				Bit_RESET
#define LED_OFF				Bit_SET

/* 		Buttons		 */
#define BUTTON_PORT_UP		GPIOD
#define BUTTON_PORT_LEFT	GPIOD
#define BUTTON_PORT_ENTER	GPIOD
#define BUTTON_PORT_RIGHT	GPIOD
#define BUTTON_PORT_DOWN	GPIOD

#define UP_BUTTON			GPIO_Pin_8
#define LEFT_BUTTON			GPIO_Pin_9
#define ENTER_BUTTON		GPIO_Pin_10
#define RIGHT_BUTTON		GPIO_Pin_11
#define DOWN_BUTTON			GPIO_Pin_12

/*		 DIP Switch 		*/
#define SW_PORT				GPIOD

#define SW_PIN_1			GPIO_Pin_0
#define SW_PIN_2			GPIO_Pin_1
#define SW_PIN_3			GPIO_Pin_2
#define SW_PIN_4			GPIO_Pin_3
#define SW_PIN_5			GPIO_Pin_4
#define SW_PIN_6			GPIO_Pin_5
#define SW_PIN_7			GPIO_Pin_6
#define SW_PIN_8			GPIO_Pin_7

/*     LCD Display    */
#define LCD_CS_PORT 		GPIO_C
#define LCD_SID_PORT 		GPIO_D
#define LCD_SCLK_PORT		GPIO_D
#define LCD_DIR_PORT 		GPIO_D

#define LCD_CS 				GPIO_Pin_6
#define LCD_SID 			GPIO_Pin_13
#define LCD_SCLK 			GPIO_Pin_14
#define LCD_DIR 			GPIO_Pin_15

/*		IIC Port 	*/

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

/*		LED Display		*/			
#define DPY_A_PORT 			GPIOC
#define DPY_B_PORT 			GPIOC
#define DPY_C_PORT 			GPIOC
#define DPY_D_PORT 			GPIOC
#define DPY_E_PORT 			GPIOC
#define DPY_F_PORT 			GPIOC
#define DPY_G_PORT 			GPIOB
#define DPY_DP_PORT 		GPIOB

#define DPY_A 				GPIO_Pin_0
#define DPY_B 				GPIO_Pin_1
#define DPY_C  				GPIO_Pin_2
#define DPY_D  				GPIO_Pin_3
#define DPY_E 				GPIO_Pin_4
#define DPY_F  				GPIO_Pin_5
#define DPY_G  				GPIO_Pin_0
#define DPY_DP 				GPIO_Pin_1

#define DPY_COM1_PORT 		GPIOE
#define DPY_COM2_PORT		GPIOE
#define DPY_COM3_PORT 		GPIOE	
#define DPY_COM4_PORT 		GPIOE	
#define DPY_COM5_PORT 		GPIOE	
#define DPY_COM6_PORT 		GPIOE	
#define DPY_COM7_PORT 		GPIOE	
#define DPY_COM8_PORT 		GPIOE	

#define DPY_COM1 			GPIO_Pin_15
#define DPY_COM2 			GPIO_Pin_14
#define DPY_COM3 			GPIO_Pin_13
#define DPY_COM4 			GPIO_Pin_12
#define DPY_COM5 			GPIO_Pin_11
#define DPY_COM6 			GPIO_Pin_10
#define DPY_COM7 			GPIO_Pin_9			
#define DPY_COM8 			GPIO_Pin_8

/*		USART Port 		*/
#define USART1_PORT		GPIOA
#define USART2_PORT 	GPIOA

#define USART1_TX		GPIO_Pin_9
#define USART1_RX		GPIO_Pin_10

#define USART2_TX		GPIO_Pin_2
#define USART2_RX		GPIO_Pin_3
#define USART2_CK		GPIO_Pin_4


#define WP_IICADDR(x) (x<<1)
//#define WP_DEBUGIICADDR1 0xF0
//#define WP_DEBUGIICADDR2 0xF1
/* SPI Port */


typedef struct
{
	u8 	bit1_flag;
	u32 low_4bit;
}
DpyStruct;
extern DpyStruct dpy_show_data;
extern u8 program_sel;
extern u8 usart_tx_buff[256],usart_rx_buff[256];
extern u8 usart_tx_ptr,usart_rx_ptr;
/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void BSP_Init (void);
int SendChar (int ch);
int GetKey(void);
void LEDShow(INT8U Index, BitAction BitVal);
u8 SwReadBit(u8 Index);
u8 SwReadByte();
u8  ButtonRead(u16 button);
void DpyShowBit(u8 Index,u8 showdata,u8 dot);
void SPI_Config(void);
void RCC_Config(void);
void GPIO_Config(void);
void USART_Config(void);
void NVIC_Config(void);
void Tim2Cmd(u16 ms,FunctionalState NewState);
void Tim3Cmd(u16 ms,FunctionalState NewState);
//IIC Functions;
void IIC_Delay(void);
bool IIC_Start(u8 IICn);
void IIC_Stop(u8 IICn);
void IIC_Ack(u8 IICn);
void IIC_NoAck(u8 IICn);
bool IIC_WaitAck(u8 IICn);
void IIC_SendByte(u8 IICn,u8 sendbyte);
u8 	 IIC_ReadByte(u8 IICn);
bool IIC_SendData(u8 addr,u8 *senddata,u8 len);
bool IIC_ReadData(u8 addr,u8 *readbuf,u8 len);
void OS_CPU_PendSVHandler(void);
void LedSignal(u8 cnt);
u32 CRC_Calu(u32 startaddr,u32 endaddr);
bool Flash_Erase(u32 page_addr);
bool Flash_Program(u8 *sour_data,u32 dest_addr,u32 len);

extern u8 t2_count_ms,t3_count_ms;
extern DpyStruct dpy_show_data;

#endif                                                                        
