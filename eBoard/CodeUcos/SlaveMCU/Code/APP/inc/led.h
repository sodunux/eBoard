#ifndef __LED_H__
#define __LED_H__
#include "includes.h"
void LedSiganl(u8 cnt);
void LEDShow(GPIO_TypeDef* GPIOx, u16 GPIOPinx, BitAction BitVal);
void Led_Flag_OK(u8 portn);
void Led_Flag_NoCard(u8 portn);
void Led_Flag_Err(u8 portn);
void Led_Flag_Running(u8 portn);
#endif