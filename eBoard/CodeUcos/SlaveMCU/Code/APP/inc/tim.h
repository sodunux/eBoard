#ifndef __TIM_H__
#define __TIM_H__
#include "includes.h"
extern u8 uart_tim_flag_1;
extern u8 uart_tim_flag_2;
extern u8 uart_tim_flag_3;
void uart_time_config(USART_TypeDef *USARTn);
void uart_time_handler(USART_TypeDef *USARTn);

#endif