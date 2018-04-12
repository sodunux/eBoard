#include "IIC.h"

u8 IIC_StartFlag=0; //IIC????,????
u8 IIC_StopFlag=0;	//IIC????
//u8 IIC_RxOrTX;			//RX=0;TX=1;
u8 IIC_Buff[256];		//IIC???
u8 IIC_BuffLen;			//IIC?????????


/*
*********************************************************************************************************
*                                            IIC_Hander()
*??:IIC??Slave?????
*********************************************************************************************************
*/
void IIC_Hander(void)
{
	static u8 IIC_BuffPtr;
	//Clear Error Flag
	I2C_ClearFlag(I2C1,I2C_FLAG_SMBALERT|I2C_FLAG_TIMEOUT|I2C_FLAG_PECERR|I2C_FLAG_OVR|I2C_FLAG_BERR|I2C_FLAG_AF);
	
	//?????
	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_ADDR))
	{
		(void)(I2C1->SR2);
		 IIC_StartFlag=1;
		 //IIC_RxOrTX=I2C_GetFlagStatus(I2C1,I2C_FLAG_TRA);
		 IIC_BuffPtr=0;
		 
	}
	
	//???STOP?
	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_STOPF))
	{
		I2C_Cmd(I2C1,ENABLE);
		IIC_StopFlag=1;	
		
		//?????,??????????
		if(!I2C_GetFlagStatus(I2C1,I2C_FLAG_TRA))
			IIC_BuffLen=IIC_BuffPtr;
		//?????,??BuffLen?0
		if(I2C_GetFlagStatus(I2C1,I2C_FLAG_TRA))
			IIC_BuffLen=0;
	}
	
	//????
	if(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE))
	{
		IIC_Buff[IIC_BuffPtr]=I2C_ReceiveData(I2C1);
		IIC_BuffPtr++;
	}
	
	//????
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