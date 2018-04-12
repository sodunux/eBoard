#include "led.h"



/*******************************************************************************
* Function Name  : LEDShow.
* Description    : LED pins level output.
* Input          : Index      :   LED Index 1~8.
                   BitAction  : Bit_RESET or Bit_SET.
* Output         : None.
* Return         : None.
*******************************************************************************/
void LEDShow(GPIO_TypeDef* GPIOx, u16 GPIOPinx, BitAction BitVal)
{
	if(BitVal == Bit_SET)  
	  GPIO_SetBits(GPIOx, GPIOPinx);
	else
	  GPIO_ResetBits(GPIOx, GPIOPinx);  	    
}


void LedSiganl(u8 cnt)
{
	u32 i,j;
	for(j=0;j<cnt;j++)
	{		
			LEDShow(GREEN1_PORT,GREEN1,LED_ON);
			LEDShow(GREEN2_PORT,GREEN2,LED_ON);
			LEDShow(GREEN3_PORT,GREEN3,LED_ON);
			LEDShow(RED1_PORT,RED1,LED_ON);
			LEDShow(RED2_PORT,RED2,LED_ON);
			LEDShow(RED3_PORT,RED3,LED_ON);		
			for(i=0;i<1000000;i++)
				__nop();
			LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
			LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
			LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
			LEDShow(RED1_PORT,RED1,LED_OFF);
			LEDShow(RED2_PORT,RED2,LED_OFF);
			LEDShow(RED3_PORT,RED3,LED_OFF);
			for(i=0;i<1000000;i++)
				__nop();				
	}
}




void Led_Flag_OK(u8 portn)
{
	switch(portn)
	{
		case 0x01:
			LEDShow(GREEN1_PORT,GREEN1,LED_ON);
			LEDShow(RED1_PORT,RED1,LED_OFF);
			break;
		case 0x02:
			LEDShow(GREEN2_PORT,GREEN2,LED_ON);
			LEDShow(RED2_PORT,RED2,LED_OFF);			
			break;
		case 0x03:
			LEDShow(GREEN3_PORT,GREEN3,LED_ON);
			LEDShow(RED3_PORT,RED3,LED_OFF);
			break;
		default:
			break;
	}
	
}


void Led_Flag_NoCard(u8 portn)
{
	switch(portn)
	{
		case 0x01:
			LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
			LEDShow(RED1_PORT,RED1,LED_ON);
			break;
		case 0x02:
			LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
			LEDShow(RED2_PORT,RED2,LED_ON);			
			break;
		case 0x03:
			LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
			LEDShow(RED3_PORT,RED3,LED_ON);
			break;
		default:
			break;
	}
}

void Led_Flag_Err(u8 portn)
{
	static u8 red_led1=0,red_led2=0,red_led3=0;
	
	LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
	LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
	LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
	switch(portn)
	{
		case 0x01:
			if(red_led1)
			{
				LEDShow(RED1_PORT,RED1,LED_OFF);
				red_led1=0;
			}
			else
			{
				LEDShow(RED1_PORT,RED1,LED_ON);
				red_led1=1;
			}
			break;
		case 0x02:
			if(red_led2)
			{
				LEDShow(RED2_PORT,RED2,LED_OFF);
				red_led1=0;
			}
			else
			{
				LEDShow(RED2_PORT,RED2,LED_ON);
				red_led1=1;
			}	
			break;
		case 0x03:
			if(red_led3)
			{
				LEDShow(RED3_PORT,RED3,LED_OFF);
				red_led3=0;
			}
			else
			{
				LEDShow(RED3_PORT,RED3,LED_ON);
				red_led3=1;
			}	
			break;
		default:
			break;
	}	
}

void Led_Flag_Running(u8 portn)
{
	static u8 green_led1=0,green_led2=0,green_led3=0;
	
	LEDShow(RED1_PORT,RED1,LED_OFF);
	LEDShow(RED2_PORT,RED2,LED_OFF);
	LEDShow(RED3_PORT,RED3,LED_OFF);
	switch(portn)
	{
		case 0x01:
			if(green_led1)
			{
				LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
				green_led1=0;
			}
			else
			{
				LEDShow(GREEN1_PORT,GREEN1,LED_ON);
				green_led1=1;
			}
			break;
		case 0x02:
			if(green_led2)
			{
				LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
				green_led1=0;
			}
			else
			{
				LEDShow(GREEN2_PORT,GREEN2,LED_ON);
				green_led1=1;
			}	
			break;
		case 0x03:
			if(green_led3)
			{
				LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
				green_led3=0;
			}
			else
			{
				LEDShow(GREEN3_PORT,GREEN3,LED_ON);
				green_led3=1;
			}
			break;
		default:
			break;
	}	
}

