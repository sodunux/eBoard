/******************** (C) COPYRIGHT 2017 FMSH *********************************
* File Name          : smartcard.h
* Author             : Yanpengfei
* Version            : V1.0
* Date               : 27/04/2017
* Description        : This file contains all the functions prototypes for the
*                      Smartcard firmware library.
********************************************************************************/
#ifndef __SMARTCARD_H
#define __SMARTCARD_H

#include "stm32f10x.h"
#include 	"bsp.h"
/* Exported constants --------------------------------------------------------*/
#define T0_PROTOCOL        0x00
#define DIRECT             0x3B
#define INDIRECT           0x3F
#define SETUP_LENGTH       20
#define HIST_LENGTH        20
#define LCmax              280
#define SC_Receive_Timeout 0x4000 
#define SC_Reset_Timeout  0x40000
#define SC_ATR_LEN         0x10 
#define SC_GET_RESPONCE    0xC0
#define SC_BAUDRATE_F			 372
#define SC_BAUDRATE_D				1
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


typedef struct
{
  u8 TS;               /* Bit Convention */
  u8 T0;               /* High nibble = Number of setup byte; low nibble = Number of historical byte */
  u8 T[SETUP_LENGTH];  /* Setup array */
  u8 H[HIST_LENGTH];   /* Historical array */
  u8 Tlength;          /* Setup array dimension */
  u8 Hlength;          /* Historical array dimension */
} SC_ATR;

typedef struct
{
  u8 CLA;  /* Command class */
  u8 INS;  /* Operation code */
  u8 P1;   /* Selection Mode */
  u8 P2;   /* Selection Option */ 
  u8 LC;           /* Data field length */
  u8 Data[LCmax];  /* Command parameters */
  u16 LE;   /* Expected length of data to be returned */

} SC_ADPU_Commands;

/* SC response structure -----------------------------------------------------*/
typedef struct
{
  u8 Data[LCmax];  /* Data returned from the card */
  u8 SW1;          /* Command Processing status */
  u8 SW2;          /* Command Processing qualification */
} SC_ADPU_Responce;

void SC_DirCmd(USART_TypeDef* USARTn,bool direction);
void SC_PowerCmd(USART_TypeDef* USARTn,BitAction NewState);
void SC_ResetCmd(USART_TypeDef* USARTn,BitAction NewState);
void SC_ClkCmd(USART_TypeDef* USARTn,FunctionalState NewState);

ErrorStatus SC_ByteReceive(USART_TypeDef* USARTn,u8 *Data,u32 TimeOut);

void SC_Close(USART_TypeDef* USARTn);
void SC_Init(USART_TypeDef* USARTn);
void SC_Delay(u32 cnt);
void SC_AnswerReq(USART_TypeDef* USARTn,u8 *card, u8 length);
u8 SC_decode_Answer2reset(u8 *card);
void SC_ColdReset(USART_TypeDef *USARTn,u8 *card,u8 length);
void SC_WarmReset(USART_TypeDef *USARTn,u8 *card,u8 length);
u8 SC_PPS(USART_TypeDef *USARTn,u8 PPS1);
void SC_ApduHander(USART_TypeDef *USARTn,SC_ADPU_Commands *SC_ADPU, SC_ADPU_Responce *SC_ResponceStatus);
void SC_BuffInit(USART_TypeDef *USARTn);
void SC_ApduWrite(u8 * card);
void SC_ByteSend(USART_TypeDef *USARTn,u8 locdata);

extern SC_ATR SC_A2R,SC_A2R1,SC_A2R2,SC_A2R3;
extern u8 SC_ATR_Table[SC_ATR_LEN];
extern SC_ADPU_Commands apdu_commands;
extern SC_ADPU_Responce apdu_responce;

extern u8 ScTimeFlag;

#endif
