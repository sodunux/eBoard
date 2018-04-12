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
#include "includes.h"
/* Smartcard Inteface GPIO pins */

void SC_DirCmd(USART_TypeDef* USARTn,bool direction);
void SC_PowerCmd(USART_TypeDef* USARTn,BitAction NewState);
void SC_ResetCmd(USART_TypeDef* USARTn,BitAction NewState);
void SC_ClkCmd(USART_TypeDef* USARTn,FunctionalState NewState);

void SC_Close(USART_TypeDef* USARTn);
void SC_Init(USART_TypeDef* USARTn);
void SC_Delay(u32 cnt);
void SC_ColdReset(USART_TypeDef *USARTn);
void SC_Transmit(USART_TypeDef *USARTn,u8 *buff,u8 txlen,u8 rxlen);
void SC_ByteSend(USART_TypeDef *USARTn,u8 locdata);

extern INT8U err;
extern OS_EVENT *uart_receive_1;
extern OS_EVENT *uart_receive_2;
extern OS_EVENT *uart_receive_3;
extern u8 uartbuff_1[128];
extern u8 uartbuff_2[128];
extern u8 uartbuff_3[128];
extern u8 uart_rx_cnt_1;
extern u8 uart_tx_cnt_1;
extern u8 uart_rx_cnt_2;
extern u8 uart_tx_cnt_2;
extern u8 uart_rx_cnt_3;
extern u8 uart_tx_cnt_3;
#endif
