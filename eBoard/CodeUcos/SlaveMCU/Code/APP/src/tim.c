#include "tim.h"
u8 uart_tim_flag_1;
u8 uart_tim_flag_2;
u8 uart_tim_flag_3;

void uart_time_config(USART_TypeDef *USARTn)
{	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period=1;
	TIM_TimeBaseStructure.TIM_Prescaler = 10*372-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	if(USARTn==USART1)
	{
		TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);
		TIM_ARRPreloadConfig(TIM2, ENABLE);
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	
	}
	else if(USARTn==USART2)
	{
		TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);
		TIM_ARRPreloadConfig(TIM3, ENABLE);
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	}
	else if (USARTn==USART3)
	{
		TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure);
		TIM_ARRPreloadConfig(TIM4, ENABLE);
		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);		
	}
}


void uart_time_handler(USART_TypeDef *USARTn)
{
	if(USARTn==USART1)
	{
		uart_tim_flag_1=1;
		TIM_ClearFlag(TIM2,TIM_FLAG_Update|TIM_FLAG_CC1|TIM_FLAG_CC2|TIM_FLAG_CC3|TIM_FLAG_CC4);
	}
	else if(USARTn==USART2)
	{
		uart_tim_flag_2=1;
		TIM_ClearFlag(TIM3,TIM_FLAG_Update|TIM_FLAG_CC1|TIM_FLAG_CC2|TIM_FLAG_CC3|TIM_FLAG_CC4);
	}
	else if (USARTn==USART3)
	{
		uart_tim_flag_3=1;
		TIM_ClearFlag(TIM4,TIM_FLAG_Update|TIM_FLAG_CC1|TIM_FLAG_CC2|TIM_FLAG_CC3|TIM_FLAG_CC4);
	}
	
	
}