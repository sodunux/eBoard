#include "iic.h"

u8 IIC_StartFlag=0; 
u8 IIC_StopFlag=0;	
//u8 IIC_RxOrTX;			//RX=0;TX=1;
u8 IIC_Buff[256];		
u8 IIC_BuffLen;			

/*
*********************************************************************************************************
*                                            IIC_Config()
*
*********************************************************************************************************
*/
void IIC_Config()
{
	I2C_InitTypeDef I2C_InitStructure;
	I2C_StructInit(&I2C_InitStructure);
	I2C_InitStructure.I2C_Mode 			=	I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1	=	IIC_ADDR;
	I2C_InitStructure.I2C_Ack		= 	I2C_Ack_Enable;
	I2C_Init(I2C1,&I2C_InitStructure);
	I2C_StretchClockCmd(I2C1,DISABLE); //Disable Pull Down SCL 
	I2C_ITConfig(I2C1,I2C_IT_EVT|I2C_IT_BUF,ENABLE);
	I2C_Cmd(I2C1,ENABLE);
}

/*
*********************************************************************************************************
*                                            IIC_Hander()
*********************************************************************************************************
*/
void IIC_Hander(void)
{
	static u8 IIC_BuffPtr;
	//Clear Error Flag
	I2C_ClearFlag(I2C1,I2C_FLAG_SMBALERT|I2C_FLAG_TIMEOUT|I2C_FLAG_PECERR|I2C_FLAG_OVR|I2C_FLAG_BERR|I2C_FLAG_AF);
	
	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_ADDR))
	{
		(void)(I2C1->SR2);
		 IIC_StartFlag=1;
		 //IIC_RxOrTX=I2C_GetFlagStatus(I2C1,I2C_FLAG_TRA);
		 IIC_BuffPtr=0;
		 
	}
	
	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_STOPF))
	{
		I2C_Cmd(I2C1,ENABLE);
		IIC_StopFlag=1;	
		
		if(!I2C_GetFlagStatus(I2C1,I2C_FLAG_TRA))
			IIC_BuffLen=IIC_BuffPtr;
		if(I2C_GetFlagStatus(I2C1,I2C_FLAG_TRA))
			IIC_BuffLen=0;
	}
	
	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE))
	{
		IIC_Buff[IIC_BuffPtr]=I2C_ReceiveData(I2C1);
		IIC_BuffPtr++;
	}
	
	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE))
	{
		if(IIC_BuffPtr<IIC_BuffLen)
		{
			I2C_SendData(I2C1,IIC_Buff[IIC_BuffPtr]);
			IIC_BuffPtr++;
		}
		else
		{
			I2C_SendData(I2C1,0x55);
			IIC_BuffLen=0;
		}
	}

}