/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : smartcard.h
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : This file contains all the functions prototypes for the
*                      Smartcard firmware library.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SMARTCARD_H
#define __SMARTCARD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include 	"bsp.h"
/* Exported constants --------------------------------------------------------*/
#define T0_PROTOCOL        0x00  /* T0 protocol */
#define DIRECT             0x3B  /* Direct bit convention */
#define INDIRECT           0x3F  /* Indirect bit convention */
#define SETUP_LENGTH       20
#define HIST_LENGTH        20
#define LCmax              280
#define SC_Receive_Timeout 0x2000  /* Direction to reader */
#define SC_Reset_Timeout   0x40000
#define SC_ATR_LEN         0x20 
/* SC Tree Structure -----------------------------------------------------------
                              MasterFile
                           ________|___________
                          |        |           |
                        System   UserData     Note
------------------------------------------------------------------------------*/

/*------------------------ Data Area Management Commands ---------------------*/

#define SC_GET_RESPONCE    0xC0

/*-------------------------- Answer to reset Commands ------------------------*/ 
#define SC_GET_A2R         0x00


/* Smartcard Inteface GPIO pins */

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

/* ATR structure - Answer To Reset -------------------------------------------*/
typedef struct
{
  u8 TS;               /* Bit Convention */
  u8 T0;               /* High nibble = Number of setup byte; low nibble = Number of historical byte */
  u8 T[SETUP_LENGTH];  /* Setup array */
  u8 H[HIST_LENGTH];   /* Historical array */
  u8 Tlength;          /* Setup array dimension */
  u8 Hlength;          /* Historical array dimension */
} SC_ATR;

/* ADPU-Header command structure ---------------------------------------------*/
typedef struct
{
  u8 CLA;  /* Command class */
  u8 INS;  /* Operation code */
  u8 P1;   /* Selection Mode */
  u8 P2;   /* Selection Option */
} SC_Header;

/* ADPU-Body command structure -----------------------------------------------*/
typedef struct 
{
  u8 LC;           /* Data field length */
  u8 Data[LCmax];  /* Command parameters */
  u16 LE;           /* Expected length of data to be returned */
} SC_Body;

/* ADPU Command structure ----------------------------------------------------*/
typedef struct
{
  SC_Header Header;
  SC_Body Body;
} SC_ADPU_Commands;

/* SC response structure -----------------------------------------------------*/
typedef struct
{
  u8 Data[LCmax];  /* Data returned from the card */
  u8 SW1;          /* Command Processing status */
  u8 SW2;          /* Command Processing qualification */
} SC_ADPU_Responce;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* APPLICATION LAYER ---------------------------------------------------------*/

//Level 1

void SC_DirCmd(USART_TypeDef* USARTn,bool direction);
void SC_PowerCmd(USART_TypeDef* USARTn,FunctionalState NewState);
void SC_ResetCmd(USART_TypeDef* USARTn,BitAction ResetState);
void SC_ClkCmd(USART_TypeDef* USARTn,FunctionalState NewState);

void SC_ByteReceive(USART_TypeDef* USARTn,*Data,u32 TimeOut);

void SC_Close(USART_TypeDef* USARTn);
void SC_Init(USART_TypeDef* USARTn);
void SC_ColdReset(USART_TypeDef *USARTn);
void SC_WarmReset(USART_TypeDef *USARTn);
void SC_PPS(USART_TypeDef *USARTn,u8 PPS1);
void SC_ApduHander(USART_TypeDef *USARTn,SC_ADPU_Commands *SC_ADPU, SC_ADPU_Responce *SC_ResponceStatus);

ErrorStatus USART_ByteReceive(USART_TypeDef* USARTn,u8 *Data, u32 TimeOut);

/***************************************************************/

//void SC_ColdReset(USART_TypeDef* USARTn,u8 *card,u8 maxlength);
void SC_AnswerReq(USART_TypeDef* USARTn, u8 *card, u8 length);
void SC_Init(USART_TypeDef* USARTn);

ErrorStatus USART_ByteReceive(USART_TypeDef* USARTn,u8 *Data, u32 TimeOut);

extern SC_ATR SC_A2R1,SC_A2R2,SC_A2R3;
extern u8 SC_ATR_Table1[40],SC_ATR_Table2[40],SC_ATR_Table1[40];
//extern SC_State scstate;
extern SC_ADPU_Commands apdu_commands;
extern SC_ADPU_Responce apdu_responce;
#endif /* __SMARTCARD_H */

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
