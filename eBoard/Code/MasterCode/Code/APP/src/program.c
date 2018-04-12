#include "program.h"

u8 program_sel;
void CosInfStruct_Read(CosInfStruct * inf)
{
//	u32 temp32;
//	u8 temp8,i;
//	inf->cos_len
	*inf=*((CosInfStruct *)MASTER_INF_ADDR);
}

void ProgramDelay(u8 ms)
{
	u8 i;
	u32 j;
	for(i=0;i<ms;i++)
	{
		for(j=0;j<100000;j++)
			__nop();
	}
}

void CosInfStruct_Write(CosInfStruct * inf)
{
	u8 buff[40],i;
	buff[0]=(u8)(inf->cos_len);
	buff[1]=(u8)(inf->cos_len>>8);
	buff[2]=(u8)(inf->cos_len>>16);	
	buff[3]=(u8)(inf->cos_len>>24);
	buff[4]=(u8)(inf->cos_crc);
	buff[5]=(u8)(inf->cos_crc>>8);
	buff[6]=(u8)(inf->cos_crc>>16);	
	buff[7]=(u8)(inf->cos_crc>>24);
	for(i=0;i<16;i++)
	{
		buff[i+8]		=	inf->apdu_request[i];
		buff[i+24]	=	inf->apdu_response[i];
	}
	Flash_Erase(MASTER_INF_ADDR);
	Flash_Program((u8 *)buff,MASTER_INF_ADDR,40);
}

//1 SUCCESS,0,FAILED
bool DownloadCosToSlave(u8 iic_addr)
{
	CosInfStruct inf;
	u8 debugbuff[100];
	u8 bccvalue,ret;
	u8 ms=2;
	u8 readbuff[0x10];
	u8 instr1[32]={0xFF,0xA3,0x00,0x00,0x04,0x08,0x01,0x08,0x00};	
	u8 instr2[256]={0xFF,0x53,0x00,0x00,0x80};
	u32 temp,i;
	u32 curr_addr;
	CosInfStruct_Read(&inf);
		
	//Send COS_StartAddr
	temp=SLAVE_COS_START_ADDR;
	instr1[5]=(u8)(temp>>24);
	instr1[6]=(u8)(temp>>16);
	instr1[7]=(u8)(temp>>8);
	instr1[8]=(u8)(temp>>0);
	bccvalue=BccCalu(instr1,9);
	instr1[9]=bccvalue;
	IIC_SendData(WP_IICADDR(iic_addr),instr1,10);	
	ProgramDelay(ms);
	IIC_ReadData(WP_IICADDR(iic_addr),readbuff,0x03);
	debugbuff[0]=readbuff[0];
	
	//Send Cos_EndAddr
	instr1[2]=0x01;
	temp=SLAVE_COS_START_ADDR+inf.cos_len;
	instr1[5]=(u8)(temp>>24);
	instr1[6]=(u8)(temp>>16);
	instr1[7]=(u8)(temp>>8);
	instr1[8]=(u8)(temp>>0);
	bccvalue=BccCalu(instr1,9);
	instr1[9]=bccvalue;
	IIC_SendData(WP_IICADDR(iic_addr),instr1,10);
	ProgramDelay(ms);
	IIC_ReadData(WP_IICADDR(iic_addr),readbuff,0x03);
	debugbuff[1]=readbuff[0];

	//Send COS_Len
	instr1[2]=0x02;
	temp=inf.cos_len;
	instr1[5]=(u8)(temp>>24);
	instr1[6]=(u8)(temp>>16);
	instr1[7]=(u8)(temp>>8);
	instr1[8]=(u8)(temp>>0);
	bccvalue=BccCalu(instr1,9);
	instr1[9]=bccvalue;
	IIC_SendData(WP_IICADDR(iic_addr),instr1,10);
	ProgramDelay(ms);
	IIC_ReadData(WP_IICADDR(iic_addr),readbuff,0x03);
	debugbuff[2]=readbuff[0];

	//Send COS_CRC
	instr1[2]=0x03;
	temp=inf.cos_crc;
	instr1[5]=(u8)(temp>>24);
	instr1[6]=(u8)(temp>>16);
	instr1[7]=(u8)(temp>>8);
	instr1[8]=(u8)(temp>>0);
	bccvalue=BccCalu(instr1,9);
	instr1[9]=bccvalue;
	IIC_SendData(WP_IICADDR(iic_addr),instr1,10);	
	ProgramDelay(ms);
	IIC_ReadData(WP_IICADDR(iic_addr),readbuff,0x03);
	debugbuff[3]=readbuff[0];
	
	//Send APDU1 Request
	instr1[2]=0x10;
	instr1[4]=0x10;
	for(i=0;i<16;i++)
		instr1[i+5]	=inf.apdu_request[i];//Apdu1_request
	bccvalue=BccCalu(instr1,21);
	instr1[21]=bccvalue;
	IIC_SendData(WP_IICADDR(iic_addr),instr1,22);
	ProgramDelay(ms);
	IIC_ReadData(WP_IICADDR(iic_addr),readbuff,0x03);
	debugbuff[4]=readbuff[0];
	
	//Send APDU1_Response
	instr1[2]=0x14;
	for(i=0;i<16;i++)
		instr1[i+5]	=inf.apdu_response[i];//Apdu1_response
	bccvalue=BccCalu(instr1,21);
	instr1[21]=bccvalue;
	IIC_SendData(WP_IICADDR(iic_addr),instr1,22);
	ProgramDelay(ms);
	IIC_ReadData(WP_IICADDR(iic_addr),readbuff,0x03);
	debugbuff[5]=readbuff[0];
	
	//Download Cos
	for(curr_addr=0;curr_addr<inf.cos_len;curr_addr=curr_addr+0x80)
	{
		instr2[2]=(u8)(curr_addr>>8);
		instr2[3]=(u8)curr_addr;
		for(i=0;i<0x80;i++)
			instr2[5+i]=*(u8 *)(MASTER_COS_START_ADDR+curr_addr+i);
		bccvalue=BccCalu(instr2,133);
		instr2[133]=bccvalue;
		IIC_SendData(WP_IICADDR(iic_addr),instr2,134);
		ProgramDelay(ms);
		IIC_ReadData(WP_IICADDR(iic_addr),readbuff,0x03);
		debugbuff[6+(curr_addr/0x80)]=readbuff[0];
		
	}
	
	ret=1;
	for(i=0;i<(6+(curr_addr/0x80));i++)
	{
		if(debugbuff[i]!=0x90)
			ret=0;
	}
	
	LedSignal(3);
	return ret;
}




//1 SUCCESS,0,FAILED
bool DownloadCosToFM349(u8 iic_addr,u8 portn,u32 fm349_dest_addr)
{
	u8 instr1[32]={0xFF,0x04,0x00,0x01,0x04,0x00,0xB0,0x00,0x00};
	u8 bccvalue;
	instr1[3]=portn;
	instr1[6]=(u8)(fm349_dest_addr>>16);
	instr1[7]=(u8)(fm349_dest_addr>>8);
	instr1[8]=(u8)(fm349_dest_addr);
	bccvalue=BccCalu(instr1,9);
	instr1[9]=bccvalue;
	IIC_SendData(WP_IICADDR(iic_addr),instr1,10);
	return 1;
	
}

//1 SUCCESS,0,FAILED
bool AutoModeCmd(u8 iic_addr,u8 newstatus)
{
	u8 bccvalue;
	u8 rxbuff[10];
	u8 instr1[32]={0xFF,0xA3,0x75,0x00,0x04,0xAA,0x56,0x55,0xAA};
	if(newstatus)
		instr1[6]=0x55;//EnterMode
	bccvalue=BccCalu(instr1,9);
	instr1[9]=bccvalue;
	IIC_SendData(WP_IICADDR(iic_addr),instr1,10);
	ProgramDelay(2);
	IIC_ReadData(WP_IICADDR(iic_addr),rxbuff,03);
	if((rxbuff[0]!=0x90)||(rxbuff[2]!=0x90))
		return 0;
	else 
		return 1;
}

/*******************************************************************************
* Function Name  : BccCalu
* Description    : Calculate the data in buff(len) bccvalue
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 BccCalu(u8 *buff,u8 len)
{
	u8 i,bccvalue;
	bccvalue=0;
	for(i=0;i<len;i++)
		bccvalue^=buff[i];
	return bccvalue;
}



void Program1()
{
	dpy_show_data.bit1_flag=1;
	//dpy_show_data.low_4bit=0;
}
/*******************************************************************************
* Function Name  : ProgramHandler1
* Description    : PC Download COS to Master via USART  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ProgramHandler1()
{
	 u8 readvalue;
	 
	 static CosInfStruct cosinf;
	 static u32 cos_addr=MASTER_INF_ADDR;	
	 readvalue=USART_ReceiveData(USART1);
	if(usart_rx_ptr==0x7F)
	{			
		usart_rx_buff[usart_rx_ptr]=readvalue;
		if(!(cos_addr%0x400))
			Flash_Erase(cos_addr);
		Flash_Program(usart_rx_buff,cos_addr,0x80);
		
		if(cos_addr<MASTER_COS_START_ADDR)
		{
			dpy_show_data.low_4bit=0;
			//CosInfStruct_Read(&inf);
		}
		else if(cos_addr==MASTER_COS_START_ADDR)
		{
			CosInfStruct_Read(&cosinf);
			cosinf.cos_len=0x80;
			cosinf.cos_crc=CRC_Calu(MASTER_COS_START_ADDR,MASTER_COS_START_ADDR+cosinf.cos_len);
			CosInfStruct_Write(&cosinf);
		}
		else
		{
			CosInfStruct_Read(&cosinf);
			cosinf.cos_len+=0x80;
			cosinf.cos_crc=CRC_Calu(MASTER_COS_START_ADDR,MASTER_COS_START_ADDR+cosinf.cos_len);
			CosInfStruct_Write(&cosinf);		
		}
		if(cos_addr==MASTER_INF_ADDR)
			cos_addr+=0x400;
		else
			cos_addr+=0x80;
	  usart_rx_ptr=0;
		dpy_show_data.low_4bit=(cosinf.cos_len/0x80);
		//LedSignal(3);
	}
 else
 {
	usart_rx_buff[usart_rx_ptr]=readvalue;
	usart_rx_ptr++;
 }	 
	//USART_ClearFlag();
}

/*******************************************************************************
* Function Name  : Program2
* Description    : Master Download Cos to Slave Via IIC 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void Program2()
{	
	dpy_show_data.bit1_flag=2;
	//dpy_show_data.low_4bit=0;
	
}
/*******************************************************************************
* Function Name  : ProgramHandler2
* Description    : Download COS From Master to Slave via IIC bus
* Input          :  COS_Startadd,COS_Endaddr,COS_len,COS_CRC,
					APDU1-Response,APDU1-Request
					COS
* Output         : None
* Return         : None
*******************************************************************************/
bool ProgramHandler2()
{
	u32 temp;
	u8 bccvalue,readvalue,validcmd;
	u8 i,j,iic_addr;
	u32 delay;
	u32 fm349_dest_addr;
	CosInfStruct inf;
	static u8 rxbuff[16];
	static u8 rx_ptr=0;
	u8 instr1[32]={0xFF,0xA3,0x00,0x00,0x04,0x08,0x01,0x08,0x00};	//FFA3740004FFFF0102 COS_StartAddr
	u8 instr2[256]={0xFF,0x53,0x00,0x00,0x80};
	u8 instr3[32]={0xFF,0x04,0x00,0x01,0x04,0x00,0xB0,0x00,0x00};
	u8 instr4[32]={0xFF,0xA3,0x75,0x00,0x04,0xAA,0x56,0x55,0xAA};
	readvalue=USART_ReceiveData(USART1);
	
	//rxbuff[0]==0x01 DownLoad Cos To Slave  0x01+Slave_IIC_Addr
	//rxbuff[0]==0x02 DownLoad Cos To FM349  0x02+Slave_IIC_Addr+Workpart+FM349_DestAddr
	//rxbuff[0]==0x03 Enter AutoMode         0x03+Slave_IIC_Addr+EnterOrExit

	
	if(rx_ptr==0x0F) //16Byte
	{
		rx_ptr=0;
		rxbuff[0x0F]=readvalue;
		//Varity Data Valid
		validcmd=0;
		for(i=0;i<8;i++)
		{
			if(rxbuff[i]!=rxbuff[15-i])
				validcmd++;
		}
		if(validcmd)//command invalid
		{
			dpy_show_data.low_4bit=1111;
			//return 0;
		}
		else
		{ 
			dpy_show_data.low_4bit=0;
			switch(rxbuff[0])
			{
				case 0x01:
					if(DownloadCosToSlave(rxbuff[1]))  //下载成功 010E0000000000000000000000000E01
						dpy_show_data.low_4bit=0;
					else
						dpy_show_data.low_4bit=1111;			
					break;
				case 0x02: //下载程序到FM349 
					fm349_dest_addr=(rxbuff[3]<<16)+(rxbuff[4]<<8)+rxbuff[5];
					DownloadCosToFM349(rxbuff[1],rxbuff[2],fm349_dest_addr);
					dpy_show_data.low_4bit=0;
					break;
				case 0x03:
					if(AutoModeCmd(rxbuff[1],rxbuff[2]))
						dpy_show_data.low_4bit=0;
					else
						dpy_show_data.low_4bit=1111;				
					break;
				default:
					break;
			}		
		}
		
	}
	
	else
	{
		rxbuff[rx_ptr]=readvalue;
		rx_ptr++;
		dpy_show_data.low_4bit=0;
	}	
	return 0;
	
}
/*******************************************************************************
* Function Name  : Program3
* Description    : Enter Auto Mode
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Program3()
{
	dpy_show_data.bit1_flag=3;
	//dpy_show_data.low_4bit=0;
}
/*******************************************************************************
* Function Name  : ProgramHandler3
* Description    : Enter Auto Mode
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool ProgramHandler3()
{
//	u8 bccvalue,readvalue,i;
//	u32 delay;
//	static u8 rxbuff[16];
//	static u8 rx_ptr=0;
//	u8 iicbuff[16];
//	u8 instr1[32]={0xFF,0xA3,0x75,0x00,0x04,0xAA,0x56,0x55,0xAA};
//	//u8 instr2[32]={0xFF,0x04,0x00,0x01,0x04,0x00,0xB0,0x00,0x00};
//	readvalue=USART_ReceiveData(USART1);
//	if(rx_ptr<16)
//	{
//		rxbuff[rx_ptr]=readvalue;
//		rx_ptr++;
//	}
	
//	if(rx_ptr==16)
//	{
//		//SLAVE_ADDR(1byte),EnterMode(1byte,YES/NO)
//		rx_ptr=0;
//		
//		if(rxbuff[0]==rxbuff[15])//地址正确
//		{
//			if(rxbuff[1]==0xAA)
//			{
//				instr1[6]=0x55;//EnterMode
//				bccvalue=BccCalu(instr1,9);
//				instr1[9]=bccvalue;
//				IIC_SendData(WP_IICADDR(rxbuff[0]),instr1,10);//第一个字节为SLAVE地址
//				for(delay=0;delay<1000;delay++)
//					__nop();		
//				IIC_ReadData(WP_IICADDR(rxbuff[0]),iicbuff,16);
//				if((iicbuff[0]!=0x90)||(iicbuff[2]!=0x90))
//				{
//					dpy_show_data.low_4bit=0x1;
//					return 0;
//				}
//			}
//			else
//			{
//				instr1[6]=0x56;//EnterMode
//				bccvalue=BccCalu(instr1,9);
//				instr1[9]=bccvalue;
//				IIC_SendData(WP_IICADDR(rxbuff[0]),instr1,10);//第一个字节为SLAVE地址
//				for(delay=0;delay<1000;delay++)
//					__nop();		
//				IIC_ReadData(WP_IICADDR(rxbuff[0]),iicbuff,16);
//				if((iicbuff[0]!=0x90)||(iicbuff[2]!=0x90))
//				{
//					dpy_show_data.low_4bit=0x1;
//					return 0;
//				}				
//			}
//		}

		
//		if(rxbuff[5]==0xAA)
//		{
//			instr1[6]=0x55;//EnterMode
//			bccvalue=BccCalu(instr1,9);
//			instr1[9]=bccvalue;
//			IIC_SendData(WP_IICADDR(rxbuff[0]),instr1,10);//第一个字节为SLAVE地址
//		
//						
//		}
//		else
//		{
//			instr1[6]=0x56;//ExitMode
//			bccvalue=BccCalu(instr1,9);
//			instr1[9]=bccvalue;
//			IIC_SendData(WP_IICADDR(rxbuff[0]),instr1,10);//第一个字节为SLAVE地址
//			//IIC_ReadData(WP_IICADDR(rxbuff[0]),rxbuff,16);	
//			rx_ptr=0;			
//		}
//	}
	
//	IIC_SendData(readvalue,instr1,10);
//	IIC_ReadData(readvalue,rxbuff,16);	
	
}

/*******************************************************************************
* Function Name  : Program4
* Description    : Show Dpy 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ProgramDebug()
{
	dpy_show_data.bit1_flag=0x0D;
	dpy_show_data.low_4bit=0;
}

/*******************************************************************************
* Function Name  : ProgramHandlerDebug
* Description    : PC Debug Slave via USART->IIC 
* Input          : iic_addr,apdu_header,adpu_response
* Output         : None
* Return         : None
*******************************************************************************/
void ProgramHandlerDebug()
{
	u8 readvalue;
	static u8 Txbuff[256],Rxbuff[16];
	static u8 txptr=0,txlen;
	static u8 iic_addr;
	static u8 tx_enable_flag=0;//0 IDLE,1 Busy
	u32 i;
	readvalue=USART_ReceiveData(USART1);
	dpy_show_data.low_4bit=readvalue;
	//0xF0-iic_addr-apdu.requeset-apdu.response
		
	if(tx_enable_flag==1)
	{
		if(txptr==0) //The first byte received
		{
			Txbuff[txptr]=readvalue;
			txlen=readvalue;
			txptr++;
		}
		else
		{
			if(txptr<txlen)
			{
				Txbuff[txptr]=readvalue;
				txptr++;
			}
			else if(txptr==txlen)
			{
				Txbuff[txptr]=readvalue;
				txptr=0;
				tx_enable_flag=0;
				iic_addr=Txbuff[1];
				Txbuff[txlen+1]=BccCalu(&Txbuff[2],(txlen-1));
				IIC_SendData(WP_IICADDR(iic_addr),&Txbuff[2],txlen);
			}
			else
			{
				(void)readvalue; 
			}
		}
			
	}
	else if((tx_enable_flag==0)&&(readvalue==0xF0))
	{
		tx_enable_flag=1;
	}
	else 
	{
		IIC_ReadData(WP_IICADDR(iic_addr),Rxbuff,readvalue);
		for(i=0;i<readvalue;i++)
		{
			USART_SendData(USART1,Rxbuff[i]);
			while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
		}
	}

}
/*********************************************************
Name	:						Program5
Descript:   (PC->Master)
*********************************************************/


void Program4(){}
void ProgramHandler4(){}
void Program5(){}
void ProgramHandler5(){}
void Program6(){}
void ProgramHandler6(){}
void Program7(){}
void ProgramHandler7(){}
void Program8(){}
void ProgramHandler8(){}