#include "iic.h"

/*******************************************************************************
* Function Name  : IIC_Delay
* Description    : IIC Delay time
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */

void IIC_Delay(void)
{
		
   //int i=1000;  
  int i=50; 
	while(i) 
   { 
     i--; 
   }  
}


/*******************************************************************************
* Function Name  : IIC_Start
* Description    : Master Start Simulation IIC Communication
* Input          : 1 or 2
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
bool IIC_Start(u8 IICn)
{

	if(IICn==1)
	{
		SDA1_H;
		SCL1_H;
		IIC_Delay();
		if(!SDA1_READ)return FALSE;	//SDA?????????,??
		SDA1_L;
		IIC_Delay();
		if(SDA1_READ) return FALSE;	//SDA??????????,??
		SDA1_L;
		IIC_Delay();
		return TRUE;
	}
	else
	{
		SDA2_H;
		SCL2_H;
		IIC_Delay();
		if(!SDA2_READ)return FALSE;	//SDA?????????,??
		SDA2_L;
		IIC_Delay();
		if(SDA2_READ) return FALSE;	//SDA??????????,??
		SDA2_L;
		IIC_Delay();
		return TRUE;
	}
}


/*******************************************************************************
* Function Name  : IIC_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : 1 or 2
* Output         : None
* Return         : None
****************************************************************************** */
void IIC_Stop(u8 IICn)
{
	if(IICn==1)
	{
		SCL1_L;
		IIC_Delay();
		SDA1_L;
		IIC_Delay();
		SCL1_H;
		IIC_Delay();
		SDA1_H;
		IIC_Delay();
	}
	else 
	{
		SCL2_L;
		IIC_Delay();
		SDA2_L;
		IIC_Delay();
		SCL2_H;
		IIC_Delay();
		SDA2_H;
		IIC_Delay();
	}
} 

/*******************************************************************************
* Function Name  : IIC_Ack
* Description    : Master Send Acknowledge Signal
* Input          : 1 or 2
* Output         : None
* Return         : None
****************************************************************************** */
void IIC_Ack(u8 IICn)
{	
	if(IICn==1)
	{
		SCL1_L;
		IIC_Delay();
		SDA1_L;
		IIC_Delay();
		SCL1_H;
		IIC_Delay();
		SCL1_L;
		IIC_Delay(); 
	}
	else
	{
		SCL2_L;
		IIC_Delay();
		SDA2_L;
		IIC_Delay();
		SCL2_H;
		IIC_Delay();
		SCL2_L;
		IIC_Delay(); 
	}
}
/*******************************************************************************
* Function Name  : IIC_NoAck
* Description    : Master Send No Acknowledge Signal
* Input          : 1 or 2
* Output         : None
* Return         : None
****************************************************************************** */
void IIC_NoAck(u8 IICn)
{
	if(IICn==1)
	{
		SCL1_L;
		IIC_Delay();
		SDA1_H;
		IIC_Delay();
		SCL1_H;
		IIC_Delay();
		SCL1_L;
		IIC_Delay();
	}
	else
	{
		SCL2_L;
		IIC_Delay();
		SDA2_H;
		IIC_Delay();
		SCL2_H;
		IIC_Delay();
		SCL2_L;
		IIC_Delay();
	}
}

 

/*******************************************************************************
* Function Name  : IIC_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : 1 or 2
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
bool IIC_WaitAck(u8 IICn) 	 
{
	if(IICn==1)
	{
		SCL1_L;
		IIC_Delay();
		SDA1_H;			
		IIC_Delay();
		SCL1_H;
		IIC_Delay();
		if(SDA1_READ)
		{
     	 	SCL1_L;
	  		IIC_Delay();
     	 	return FALSE;
		}
		SCL1_L;
		IIC_Delay();
		return TRUE;
	}
	else
	{
		SCL2_L;
		IIC_Delay();
		SDA2_H;			
		IIC_Delay();
		SCL2_H;
		IIC_Delay();
		if(SDA1_READ)
		{
     	 	SCL2_L;
	  		IIC_Delay();
     	 	return FALSE;
		}

		SCL2_L;
		IIC_Delay();
		return TRUE;		
	}


}


/*******************************************************************************
* Function Name  : IIC_SendByte
* Description    : Master Send a Byte to Slave
* Input          : IIC1 or IIC2,Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void IIC_SendByte(u8 IICn,u8 sendbyte) //MSB first
{
    u8 i=8;
    if(IICn==1)
    {
    	while(i--)
    	{
      		SCL1_L;
       	 	IIC_Delay();
     		if(sendbyte&0x80)
        		SDA1_H;  
      		else 
       	 		SDA1_L;   
        	sendbyte<<=1;
        	IIC_Delay();
					SCL1_H;
       		IIC_Delay();
    	}
    	SCL1_L;
    }
    else
    {
    	while(i--)
    	{
      		SCL2_L;
       	 	IIC_Delay();
     		if(sendbyte&0x80)
        		SDA2_H;  
      		else 
       	 		SDA2_L;   
        	sendbyte<<=1;
        	IIC_Delay();
			SCL2_H;
       		IIC_Delay();
    	}
    	SCL2_L;
    }
}


/*******************************************************************************
* Function Name  : IIC_ReadByte
* Description    : Master Reserive a Byte From Slave
* Input          : 1 or 2
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
u8 IIC_ReadByte(u8 IICn) 
{ 
    u8 i=8;
    u8 receivebyte=0;

    if(IICn==1)
    {
    	SDA1_H;				
    	while(i--)
    	{
      		receivebyte<<=1;      
      		SCL1_L;
      		IIC_Delay();
	  		SCL1_H;
      		IIC_Delay();	
      		if(SDA1_READ)
      		{
        		receivebyte|=0x01;
      		}
    	}
    	SCL1_L;  	
    }
    else
    {
    	SDA2_H;				
    	while(i--)
    	{
      		receivebyte<<=1;      
      		SCL2_L;
      		IIC_Delay();
	  		SCL2_H;
      		IIC_Delay();	
      		if(SDA2_READ)
      		{
        		receivebyte|=0x01;
      		}
    	}
    	SCL2_L; 
    }
    return receivebyte;
} 



/*******************************************************************************
* Function Name  : IIC_SendData
* Description    : Send Data via IIC
* Input          : addr:0x00~0x80
									 senddata:data buff address
									 len:senddata length
* Return         : 1,OKay;0,Failed;
*******************************************************************************/
u8 IIC_SendData(u8 addr,u8 *senddata,u8 len)
{
		//1~15 IIC1
		//16~30 IIC2
		u8 i,bccvalue,IICn;
		u8 *ptr=senddata;
		if((addr>15)&&(addr<31))
			IICn=2;
		else
			IICn=1;
		
		IIC_Start(IICn);
		IIC_SendByte(IICn,addr<<1);
		flag=IIC_WaitAck(IICn);
		if(!flag) 
			return 0;
		for(i=0;i<len;i++)
		{
			IIC_SendByte(IICn,*ptr);
			flag=IIC_WaitAck(IICn);
			if(!flag) 
				return 0;
			ptr++;
		}
		
		IIC_Stop(IICn);
		return 1;
	
}

/*******************************************************************************
* Function Name  : IIC_SendData
* Description    : Read Data via IIC
* Input          : addr:0x00~0x80
									 Read:data buff address
									 len:senddata length
* Return         : true:
											opertion OK.
									 false:
											opertion failed.
*******************************************************************************/
u8 IIC_ReadData(u8 addr,u8 *readbuf,u8 len)
{
	u8 *ptr=readbuf;
	u8 i,IICn;

	if((addr>15)&&(addr<31))
		IICn=2;
	else
		IICn=1;
		
	IIC_Start(IICn);
	IIC_SendByte(IICn,(addr<<1)|0x01);
	flag=IIC_WaitAck(IICn);
	if(!flag) 
			return 0;
	for(i=0;i<(len-1);i++)
	{
		*ptr=IIC_ReadByte(IICn);
		IIC_Ack(IICn);
		ptr++;
	}
	*ptr=IIC_ReadByte(IICn);
	IIC_NoAck(IICn);
	IIC_Stop(IICn);
	return 1;
}