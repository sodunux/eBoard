#include "stm32f10x.h"
#include "Instr.h"
#include "bsp.h"
#include "smartcard.h"


/****************************************************************
//										InstrHandler()
//Handler the Instrction from the Master Via IIC bus
*****************************************************************/
void InstrHandler()
{
	u8 CMD_CLA,CMD_INS,CMD_P1,CMD_P2,CMD_LC,CMD_LE;
	u8 i,bccvalue=0;

	INF_STRUCT inf_data;
	InfStruct_Read(&inf_data);
	
	if(IIC_StopFlag) //处理IIC来的指令
	{
		if(IIC_BuffLen)	//Receive data from Master
		{
			//APDU Request:		Header+LC+Data+LE+BCC
			//APDU Response:	ResponseData[LE]+BCC
			//BCC parity,IIC_data+BCC
			for(i=0;i<(IIC_BuffLen-1);i++)
			{
				bccvalue=bccvalue^IIC_Buff[i];
			}
			if(bccvalue!=IIC_Buff[i]) //BCC Parity Failed
			{
				//add code
				IIC_Buff[0]=0x6A;
				IIC_Buff[1]=0x00;
				IIC_Buff[2]=0x6A^0x00;
				IIC_BuffLen=3;
			}
			else  //BCC Parity Succeed
			{
				CMD_CLA=IIC_Buff[0];
				CMD_INS=IIC_Buff[1];
				if(CMD_CLA==0xFF)  //To Slave
				{
					switch(CMD_INS)
					{
						case WRITECOS: //Download COS To Slave 
							INS_WriteCos();
							break;
						case READCOS:		
							INS_ReadCos();
							break;
						case WP_COLDRESET: 
							INS_WpColdReset();
							break;
						case WP_LEDCTRL:
							INS_WpLedCtrl();
							break;
						case WP_CLOSE:
							INS_WpClose();
							break;
						case DOWNLOAD_COS:// Download COS to FM349
							INS_DownloadCos();
							break;
						case WRITEINF: //Write Slave INF 
							INS_WriteInf();
							break;
						case READINF:  //Read Slave INF
							INS_ReadInf();
							break;
						case CHANGE_IICADDR:
							//INS_ChangeIicAddr();
							break;
						default:
							break;
					}	
				}
				else	//To Workpart				
				{
					SC_ApduWrite(IIC_Buff);
					if(inf_data.Port_ID==0x01)//当前操作接口是Port1
					{
						SC_ApduHander(USART1,&apdu_commands,&apdu_responce);
					}
					else if(inf_data.Port_ID==0x02)//当前操作接口是Port2
					{
						SC_ApduHander(USART3,&apdu_commands,&apdu_responce);	
					}
					else if(inf_data.Port_ID==0x03)//当前操作接口是Port3
					{
						SC_ApduHander(USART2,&apdu_commands,&apdu_responce);
					}
				
				IIC_Buff[0]=apdu_responce.SW1;
				IIC_Buff[1]=apdu_responce.SW2;
				for(i=0;i<apdu_commands.LE;i++)
				{
					IIC_Buff[2+i]=apdu_responce.Data[i];
				}
				
				bccvalue=0;
				for(i=0;i<(apdu_commands.LE+2);i++)
				{
					bccvalue^=IIC_Buff[i];
				}
				IIC_Buff[i]=bccvalue;
				IIC_BuffLen=apdu_commands.LE+3;
				}
			}			
		}
		
		IIC_StartFlag=0;	
		IIC_StopFlag=0;
	}
}




//离线模式初始化
void Auto_Init(void)
{
	INF_STRUCT inf_data;
	InfStruct_Read(&inf_data);

	if(inf_data.Auto_Debug==0xAA5555AA)//Enter Auto Mode
	{
		
		if(FM349_DownloadCos(0x01,0x00000000)) //下载成功
		{
			port1_status=Port_Download_Y;
			Led_Flag_OK(0x01);
		}
		else
		{
			port1_status=Port_Download_N;
			Led_Flag_NoCard(0x01);
		}
		
		if(FM349_DownloadCos(0x02,0x00000000)) //下载成功
		{
			port2_status=Port_Download_Y;
			Led_Flag_OK(0x02);
		}
		else
		{
			port2_status=Port_Download_N;
			Led_Flag_NoCard(0x02);
		}
		
		if(FM349_DownloadCos(0x03,0x00000000)) //下载成功
		{
			port3_status=Port_Download_Y;
			Led_Flag_OK(0x03);
		}
		else
		{
			port3_status=Port_Download_N;
			Led_Flag_NoCard(0x03);
		}	
	}
	
}


#if 1
//Auto 模式处理程序
void AutoHandler(void)
{
	INF_STRUCT inf_data;
	InfStruct_Read(&inf_data);
	//APDU—Request  采用固定格式
	//APDU-Response 可选择的
	if(inf_data.Auto_Debug==0xAA5555AA)//Enter Auto Mode 
	{
		if(port1_status==Port_Download_Y) //Port1正常
		{
			SC_ColdReset(USART1,SC_ATR_Table,SC_ATR_LEN); 
			if(SC_A2R1.TS!=0x3B)
			{
				port1_status=Port_ATR_N; //ColdReset失败
				Led_Flag_NoCard(0x01);
			}
			else
			{
				port1_status=Port_ATR_Y;
				Led_Flag_OK(0x01);
			}

		}
		
		
		
		//When Running,SC_A2R1、SC_A2R2、SC_A2R3 Works
		 if(SC_A2R1.TS==0x3B) //Port1 USART1
		 {
				SC_ApduWrite(inf_data.APDU1_Request);
				SC_ApduHander(USART1,&apdu_commands,&apdu_responce);
				if((apdu_responce.SW1==0x90)&&(apdu_responce.SW2==0x00))//apdu running OK
				{
					if(apdu_responce.Data[0]==inf_data.APDU1_Response[0]) //闪绿灯
					{
						LEDShow(RED1_PORT,RED1,LED_OFF);	
						green_led_status1=~green_led_status1;
						if(!green_led_status1)
							LEDShow(GREEN1_PORT,GREEN1,LED_ON);
						else
							LEDShow(GREEN1_PORT,GREEN1,LED_OFF);		
					}
					
					else if(apdu_responce.Data[0]==inf_data.APDU1_Response[1]) //闪红灯
					{
						LEDShow(GREEN1_PORT,GREEN1,LED_OFF);		
						red_led_status1=~red_led_status1;
						if(!red_led_status1)
							LEDShow(RED1_PORT,RED1,LED_ON);
						else
							LEDShow(RED1_PORT,RED1,LED_OFF);		
					}
					
					else if(apdu_responce.Data[0]==inf_data.APDU1_Response[2]) //绿灯常亮，红灯常灭
					{
							LEDShow(GREEN1_PORT,GREEN1,LED_ON);
							LEDShow(RED1_PORT,RED1,LED_OFF);		
					}
					
					else if(apdu_responce.Data[0]==inf_data.APDU1_Response[3]) //红灯常亮，绿灯常灭
					{
							LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
							LEDShow(RED1_PORT,RED1,LED_ON);		
					}
					//TODO 其他亮灯情况
					else //指令错误
					{
							LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
							LEDShow(RED1_PORT,RED1,LED_OFF);
					}
				}
				else
				{
					SC_ColdReset(USART1,SC_ATR_Table,SC_ATR_LEN); 
				}
		 }

		 else
		 {
				SC_ColdReset(USART1,SC_ATR_Table,SC_ATR_LEN); 
		 }
		 
		 if(SC_A2R3.TS==0x3B) //Port2 USART3
		 {
				SC_ApduWrite(inf_data.APDU1_Request);
				SC_ApduHander(USART3,&apdu_commands,&apdu_responce);
				if((apdu_responce.SW1==0x90)&&(apdu_responce.SW2==0x00))//apdu running OK
				{
					if(apdu_responce.Data[0]==inf_data.APDU1_Response[0]) //闪绿灯
					{
						LEDShow(RED2_PORT,RED2,LED_OFF);
						green_led_status2=~green_led_status2;
						if(!green_led_status2)
							LEDShow(GREEN2_PORT,GREEN2,LED_ON);
						else
							LEDShow(GREEN2_PORT,GREEN2,LED_OFF);		
					}
					
					else if(apdu_responce.Data[0]==inf_data.APDU1_Response[1]) //闪红灯
					{
						LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
						red_led_status2=~red_led_status2;
						if(!red_led_status2)
							LEDShow(RED2_PORT,RED2,LED_ON);
						else
							LEDShow(RED2_PORT,RED2,LED_OFF);		
					}
					
					else if(apdu_responce.Data[0]==inf_data.APDU1_Response[2]) //绿灯常亮，红灯常灭
					{
							LEDShow(GREEN2_PORT,GREEN2,LED_ON);
							LEDShow(RED2_PORT,RED2,LED_OFF);		
					}
					
					else if(apdu_responce.Data[0]==inf_data.APDU1_Response[3]) //红灯常亮，绿灯常灭
					{
							LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
							LEDShow(RED2_PORT,RED2,LED_ON);		
					}
					//TODO 其他亮灯情况
					else //指令错误
					{
							LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
							LEDShow(RED2_PORT,RED2,LED_OFF);
					}
				}
				else
				{
					SC_ColdReset(USART3,SC_ATR_Table,SC_ATR_LEN);
				}
		 }
		 else
		 {
			 SC_ColdReset(USART3,SC_ATR_Table,SC_ATR_LEN);
		 }
		 
		 if(SC_A2R2.TS==0x3B) //Port2 USART2
		 {
				SC_ApduWrite(inf_data.APDU1_Request);
				SC_ApduHander(USART2,&apdu_commands,&apdu_responce);
				if((apdu_responce.SW1==0x90)&&(apdu_responce.SW2==0x00))//apdu running OK
				{
					if(apdu_responce.Data[0]==inf_data.APDU1_Response[0]) //闪绿灯
					{
						LEDShow(RED3_PORT,RED3,LED_OFF);	
						green_led_status3=~green_led_status3;
						if(!green_led_status3)
							LEDShow(GREEN3_PORT,GREEN3,LED_ON);
						else
							LEDShow(GREEN3_PORT,GREEN3,LED_OFF);		
					}
					
					else if(apdu_responce.Data[0]==inf_data.APDU1_Response[1]) //闪红灯
					{
						LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
						red_led_status3=~red_led_status3;
						if(!red_led_status3)
							LEDShow(RED3_PORT,RED3,LED_ON);
						else
							LEDShow(RED3_PORT,RED3,LED_OFF);		
					}
					
					else if(apdu_responce.Data[0]==inf_data.APDU1_Response[2]) //绿灯常亮，红灯常灭
					{
							LEDShow(GREEN3_PORT,GREEN3,LED_ON);
							LEDShow(RED3_PORT,RED3,LED_OFF);		
					}
					
					else if(apdu_responce.Data[0]==inf_data.APDU1_Response[3]) //红灯常亮，绿灯常灭
					{
							LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
							LEDShow(RED3_PORT,RED3,LED_ON);		
					}
					//TODO 其他亮灯情况
					else //指令错误
					{
							LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
							LEDShow(RED3_PORT,RED3,LED_OFF);
					}		
				}
				else
				{
					SC_ColdReset(USART2,SC_ATR_Table,SC_ATR_LEN);
				}
		 }
		 else
		 {
			 SC_ColdReset(USART2,SC_ATR_Table,SC_ATR_LEN);
		 }			 
		 
	}
}

#endif

void INS_WriteCos()
{
	//__nop();
	//必须是Word的倍数
	u32 startaddr;
	u8 CMD_P1,CMD_P2,CMD_P3;
	u8 *CMD_DataPtr;
//	INF_STRUCT inf;
//	InfStruct_Read(&inf);
	

	CMD_P1=IIC_Buff[2];
	CMD_P2=IIC_Buff[3];
	CMD_P3=IIC_Buff[4];
	CMD_DataPtr=&IIC_Buff[5];
	startaddr=COS_START_ADDR+(CMD_P1<<8)+CMD_P2;
	
	if(startaddr%0x80)//起始地址不是128的倍数
	{
		IIC_Buff[0]=0x6B;
		IIC_Buff[1]=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=0x03;
	}
	else
	{
		if(CMD_P3>0x80)//长度超过128Bytes
		{
			IIC_Buff[0]	=0x67;
			IIC_Buff[1]	=0x00;
			IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
			IIC_BuffLen=3;
		}
		else
		{
			if(startaddr==(COS_START_ADDR))
			{
				__nop();
			}
			if(startaddr==(COS_START_ADDR+0x80))
			{
				__nop();
			}
			if(startaddr==(COS_START_ADDR+0x100))
			{
				__nop();
			}			
			if(!(startaddr%0x400))//在Page开始才进行页擦，其他地址不进行页擦。所以Master发送数据时，首地址应该是Page的起始地址。
			Flash_Erase(startaddr);
			Flash_Program(CMD_DataPtr,startaddr,CMD_P3);
			IIC_Buff[0]=0x90;
			IIC_Buff[1]=0x00;
			IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
			IIC_BuffLen=3;	
		}
	}
	
}

//下载程序到卡
void INS_DownloadCos()
{
	u8 CMD_P1,CMD_P2,CMD_P3;
	u8 *CMD_DataPtr;
	u32 crc_value;
	bool temp_value;
	INF_STRUCT inf_data;
	u32 startaddr;
	
	InfStruct_Read(&inf_data);
	
	crc_value=CRC_Calu(inf_data.cos1_startaddr,inf_data.cos1_endaddr);
	temp_value=(inf_data.cos1_startaddr>=COS_START_ADDR)&&(inf_data.cos1_startaddr<COS_BOUNDADDR);
	temp_value&=(inf_data.cos1_endaddr>=COS_START_ADDR)&&(inf_data.cos1_endaddr<COS_BOUNDADDR);
	temp_value&=(inf_data.cos1_len==(inf_data.cos1_endaddr-inf_data.cos1_startaddr));
	temp_value&=(crc_value==inf_data.cos1_crc);
	
	CMD_P1=IIC_Buff[2];
	CMD_P2=IIC_Buff[3];
	CMD_P3=IIC_Buff[4];
	CMD_DataPtr=&IIC_Buff[5];
	startaddr=(CMD_DataPtr[0]<<24)|(CMD_DataPtr[1]<<16)|(CMD_DataPtr[2]<<8)|(CMD_DataPtr[3]);
	
	if(IIC_BuffLen!=0x0A)
	{
		IIC_Buff[0]=0x67;
		IIC_Buff[1]=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=0x03;
	}
	else if(temp_value==0x00)//COS区数据不正确
	{
		IIC_Buff[0]=0x6B;
		IIC_Buff[1]=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=0x03;
	}
	else
	{
		if(CMD_P1==0x00)//FM349下载程序
		{
			if(FM349_DownloadCos(CMD_P2,startaddr))//下载成功
			{
				IIC_Buff[0]=0x90;
				IIC_Buff[1]=0x00;
				IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
				IIC_BuffLen=0x03;
				LedSiganl(3);
			}
			else
			{
				IIC_Buff[0]=0x6F;
				IIC_Buff[1]=0x00;
				IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
				IIC_BuffLen=0x03;
				LedSiganl(3);
			}
		}
		
		else //FMxx下载程序
		{
			if(FMxx_DownloadCos(CMD_P2,startaddr))//下载成功
			{
				IIC_Buff[0]=0x90;
				IIC_Buff[1]=0x00;
				IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
				IIC_BuffLen=0x03;
				LedSiganl(3);
			}
			else
			{
				IIC_Buff[0]=0x6F;
				IIC_Buff[1]=0x00;
				IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
				IIC_BuffLen=0x03;
				LedSiganl(3);
			}
		}

	}	
	//TODO
}

void INS_ReadCos()
{
	//__nop();
	u32 startaddr;
	u8 CMD_P1,CMD_P2,CMD_P3,i;
	u8 *byteptr;

	
	CMD_P1=IIC_Buff[2];
	CMD_P2=IIC_Buff[3];
	CMD_P3=IIC_Buff[4];
//	CMD_DataPtr=&IIC_Buff[5];
	
	startaddr=COS_START_ADDR+(CMD_P1<<8)+CMD_P2;
	byteptr=(u8 *)startaddr;
	
	if(startaddr%0x80)//起始地址不是128的倍数
	{
		IIC_Buff[0]=0x6B;
		IIC_Buff[1]=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=0x03;
	}
	else
	{
		if(CMD_P3>0x80)//长度超过128Bytes
		{
			IIC_Buff[0]	=0x67;
			IIC_Buff[1]	=0x00;
			IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
			IIC_BuffLen=3;
		}
		else
		{
			for(i=0;i<CMD_P3;i++)
			{
				IIC_Buff[0]=0x90;
				IIC_Buff[1]=0x00;
				IIC_Buff[CMD_P3+2]=IIC_Buff[0]^IIC_Buff[1];
				for(i=0;i<CMD_P3;i++)
				{
					IIC_Buff[2+i]=byteptr[i];
					IIC_Buff[CMD_P3+2]^=IIC_Buff[2+i];
				}
				IIC_BuffLen=CMD_P3+3;

			}
		}
	}	
}


void INS_WpColdReset()
{
	//__nop();
	u8 CMD_P1,CMD_P2,CMD_P3,i,bccvalue;
	CMD_P1=IIC_Buff[2];
	CMD_P2=IIC_Buff[3];
	CMD_P3=IIC_Buff[4];
	
	if(IIC_BuffLen!=0x06)
	{
		IIC_Buff[0]=0x67;
		IIC_Buff[1]=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=0x03;
	}
	else
	{
		if(CMD_P1==0x01)//Port1-USART1
		{
			SC_ColdReset(USART1,SC_ATR_Table,SC_ATR_LEN);
		}
		else if(CMD_P1==0x02)//Port2-USART3
		{
			SC_ColdReset(USART3,SC_ATR_Table,SC_ATR_LEN);
		}
		else if(CMD_P1==0x03)//Port3-USART2
		{
			SC_ColdReset(USART2,SC_ATR_Table,SC_ATR_LEN);
		}
		
		IIC_Buff[0]=0x90;
		IIC_Buff[1]=0x00;

		IIC_Buff[2]=SC_A2R.TS;
		IIC_Buff[3]=SC_A2R.T0;
		for(i=0;i<SC_A2R.Tlength;i++)
		{
			IIC_Buff[4+i]=SC_A2R.T[i];
		}
		for(i=0;i<SC_A2R.Hlength;i++)
		{
			IIC_Buff[4+i+SC_A2R.Tlength]=SC_A2R.H[i];
		}
		bccvalue=0;;//calu bcc
		for(i=0;i<(4+SC_A2R.Hlength+SC_A2R.Tlength);i++)
		{
			bccvalue^=IIC_Buff[i];
		}
		IIC_Buff[i]=bccvalue;
		IIC_BuffLen=5+SC_A2R.Hlength+SC_A2R.Tlength;
	}
	
}

void INS_WpClose()
{
	//__nop();
	u8 CMD_P1,CMD_P2,CMD_P3;
	CMD_P1=IIC_Buff[2];
	CMD_P2=IIC_Buff[3];
	CMD_P3=IIC_Buff[4];
	
	if(IIC_BuffLen!=0x06)
	{
		IIC_Buff[0]=0x67;
		IIC_Buff[1]=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=0x03;
	}
	else
	{
		if(CMD_P1==0x01)//Port1-USART1
		{
			SC_Close(USART1);
		}
		else if(CMD_P1==0x02)//Port2-USART3
		{
			SC_Close(USART3);
		}
		else if(CMD_P1==0x03)//Port3-USART2
		{
			SC_Close(USART2);
		}
		
		IIC_Buff[0]=0x90;
		IIC_Buff[1]=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=0x03;
	}
}

void INS_WpLedCtrl()
{
	//__nop();
	u8 CMD_P1,CMD_P2,CMD_P3;
	
	CMD_P1=IIC_Buff[2];
	CMD_P2=IIC_Buff[3];
	CMD_P3=IIC_Buff[4];
	
	if(IIC_BuffLen!=0x06)
	{
		IIC_Buff[0]=0x67;
		IIC_Buff[1]=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=0x03;
	}
	else
	{
		if(CMD_P1==0x01)
		{
			if(CMD_P2&0xF0)
				LEDShow(RED1_PORT,RED1,LED_ON);
			else
				LEDShow(RED1_PORT,RED1,LED_OFF);
			
			if(CMD_P2&0x0F)
				LEDShow(GREEN1_PORT,GREEN1,LED_ON);
			else
				LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
		}
		if(CMD_P1==0x02)
		{
			if(CMD_P2&0xF0)
				LEDShow(RED2_PORT,RED2,LED_ON);
			else
				LEDShow(RED2_PORT,RED2,LED_OFF);
			
			if(CMD_P2&0x0F)
				LEDShow(GREEN2_PORT,GREEN2,LED_ON);
			else
				LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
		}
		if(CMD_P1==0x03)
		{
			if(CMD_P2&0xF0)
				LEDShow(RED3_PORT,RED3,LED_ON);
			else
				LEDShow(RED3_PORT,RED3,LED_OFF);
			
			if(CMD_P2&0x0F)
				LEDShow(GREEN3_PORT,GREEN3,LED_ON);
			else
				LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
		}
		
		IIC_Buff[0]=0x90;
		IIC_Buff[1]=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=0x03;
		
	}
	
}


void INS_EnterAuto()
{
	__nop();
}

void INS_ExitAuto()
{
	__nop();
}


void INS_WriteInf()
{
	u8 CMD_P1,CMD_P2,CMD_P3;
	u8 *CMD_DataPtr;
	INF_STRUCT inftest,*inf_ptr;
	u8 i;
	u32 tmp;
	inf_ptr=&inftest;
	CMD_DataPtr=&IIC_Buff[5];
	
	InfStruct_Read(inf_ptr);
	
	CMD_P1=IIC_Buff[2];
	CMD_P2=IIC_Buff[3];
	CMD_P3=IIC_Buff[4];
	
	switch(CMD_P1)
	{
		case 0x00://更改COS1_StartAddr的值
		case 0x01://更改COS1_EndAddr的值
		case 0x02://更改COS1_Len的值
		case 0x03://更改COS1_CRC的值
		case 0x04://更改COS2_StartAddr的值
		case 0x05://更改COS1_EndAddr的值
		case 0x06://更改COS1_Len的值
		case 0x07://更改COS1_CRC的值
		case 0x60://更改Port1Status1
		case 0x61://更改Port1Status2
		case 0x62://更改Port1Status3
		case 0x63://更改Port1Status4
		case 0x64://更改Port2Status1
		case 0x65://更改Port2Status2
		case 0x66://更改Port2Status3
		case 0x67://更改Port2Status4
		case 0x70://更改Port3Status1
		case 0x71://更改Port3Status2
		case 0x72://更改Port3Status3
		case 0x73://更改Port3Status4
		case 0x74://更改IIC_Addr
		case 0x75://更改Auto/Debug
		case 0x76://PortID
		case 0x77://INF_CRC
			
			if(CMD_P3==0x04)
			{
				tmp=(CMD_DataPtr[0]<<24)|(CMD_DataPtr[1]<<16)|(CMD_DataPtr[2]<<8)|(CMD_DataPtr[3]);

				switch(CMD_P1)
				{
					case 0x00:
							inf_ptr->cos1_startaddr=tmp;
						break;
					case 0x01:
							inf_ptr->cos1_endaddr=tmp;
						break;
					case 0x02:
							inf_ptr->cos1_len=tmp;
						break;
					case 0x03:
							inf_ptr->cos1_crc=tmp;
						break;
					case 0x04:
							inf_ptr->cos2_startaddr=tmp;
						break;
					case 0x05:
							inf_ptr->cos2_endaddr=tmp;
						break;	
					case 0x06:
							inf_ptr->cos2_len=tmp;
						break;
					case 0x07:
							inf_ptr->cos2_crc=tmp;
						break;
					case 0x60:
							inf_ptr->port1_s1=tmp;
						break;
					case 0x61:
							inf_ptr->port1_s2=tmp;
						break;
					case 0x62:
							inf_ptr->port1_s3=tmp;
						break;
					case 0x63:
							inf_ptr->port1_s4=tmp;
						break;
					case 0x64:
							inf_ptr->port2_s1=tmp;
						break;
					case 0x65:
							inf_ptr->port2_s2=tmp;
						break;	
					case 0x66:
							inf_ptr->port2_s3=tmp;
						break;
					case 0x67:
							inf_ptr->port2_s4=tmp;
						break;
					case 0x70:
							inf_ptr->port3_s1=tmp;
						break;
					case 0x71:
							inf_ptr->port3_s2=tmp;
						break;
					case 0x72:
							inf_ptr->port3_s3=tmp;
						break;
					case 0x73:
							inf_ptr->port3_s4=tmp;
						break;
					case 0x74:
							inf_ptr->IIC_addr=tmp;
						break;
					case 0x75:
							inf_ptr->Auto_Debug=tmp;
						break;	
					case 0x76:
							inf_ptr->Port_ID=tmp;
						break;
					case 0x77:
							inf_ptr->INF_CRC=tmp;
						break;	
					default:
						break;
				}
				IIC_Buff[0]=0x90;
				IIC_Buff[1]=0x00;
				IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
				IIC_BuffLen=3;

			}
			else
			{
				IIC_Buff[0]=0x6B;
				IIC_Buff[1]=0x00;
				IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
				IIC_BuffLen=3;	
			}
			break;

		
		case 0x10://更改APDU1_Request
		case 0x14:
		case 0x20:
		case 0x24:
		case 0x30:
		case 0x34:
		case 0x40:
		case 0x44:
		case 0x50:
		case 0x54:
			
			//更改APDU_Request/APDU_Response的值
			if(CMD_P3==0x10)
			{
				switch(CMD_P1)
				{
					case 0x10:
						for(i=0;i<0x10;i++)
						inf_ptr->APDU1_Request[i]=CMD_DataPtr[i];
						break;
					case 0x14:
						for(i=0;i<0x10;i++)
						inf_ptr->APDU1_Response[i]=CMD_DataPtr[i];
						break;
					case 0x20:
						for(i=0;i<0x10;i++)
						inf_ptr->APDU2_Request[i]=CMD_DataPtr[i];
						break;
					case 0x24:
						for(i=0;i<0x10;i++)
						inf_ptr->APDU2_Response[i]=CMD_DataPtr[i];
						break;
					case 0x30:
						for(i=0;i<0x10;i++)
						inf_ptr->APDU3_Request[i]=CMD_DataPtr[i];
						break;
					case 0x34:
						for(i=0;i<0x10;i++)
						inf_ptr->APDU3_Response[i]=CMD_DataPtr[i];
						break;
					case 0x40:
						for(i=0;i<0x10;i++)
						inf_ptr->APDU4_Request[i]=CMD_DataPtr[i];
						break;
					case 0x44:
						for(i=0;i<0x10;i++)
						inf_ptr->APDU4_Response[i]=CMD_DataPtr[i];
						break;
					case 0x50:
						for(i=0;i<0x10;i++)
						inf_ptr->APDU5_Request[i]=CMD_DataPtr[i];
						break;
					case 0x54:
						for(i=0;i<0x10;i++)
						inf_ptr->APDU5_Response[i]=CMD_DataPtr[i];
						break;
						
				}
				
				IIC_Buff[0]=0x90;
				IIC_Buff[1]=0x00;
				IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
				IIC_BuffLen=3;

			}
			else
			{
				IIC_Buff[0]=0x6B;
				IIC_Buff[1]=0x00;
				IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
				IIC_BuffLen=3;	
			}
			break;			
	}
	
	InfStruct_Write(inf_ptr);
}
	
//读的长度不要超过128Byte
void INS_ReadInf()
{
	u8 CMD_P1,CMD_P2,CMD_P3;

	u8 *byteptr,i;
	u32 startaddr;
	
	INF_STRUCT inftest,*inf_ptr;
	inf_ptr=&inftest;


	InfStruct_Read(inf_ptr);
	
	CMD_P1=IIC_Buff[2];
	CMD_P2=IIC_Buff[3];
	CMD_P3=IIC_Buff[4];
	
	byteptr=(u8 *)inf_ptr;
	startaddr=(CMD_P1<<8)+CMD_P2;
	
	if(CMD_P3>0x80)
	{
		IIC_Buff[0]	=0x67;
		IIC_Buff[1]	=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=3;
		
	}
	else
	{
		IIC_Buff[CMD_P3+2]=0;
		IIC_Buff[0]	=0x90;
		IIC_Buff[1]=0x00;
		
		for(i=0;i<CMD_P3;i++)
		{
			IIC_Buff[i+2]=byteptr[startaddr+i];	
		}
		
		for(i=0;i<(CMD_P3+2);i++)
		{
			IIC_Buff[CMD_P3+2]^=IIC_Buff[i];//BCC Parity
		}
		
		IIC_BuffLen=CMD_P3+3;
	}
	
}

void INS_ChangeIicAddr(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	INF_STRUCT inf_data;
	InfStruct_Read(&inf_data);
	
	if(IIC_BuffLen!=0x06)
	{
		IIC_Buff[0]=0x67;
		IIC_Buff[1]=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=0x03;
	}
	else
	{
		if((inf_data.IIC_addr>>8)==0x00AA55AA)//更改设备IIC地址,高3Byte为0xAA,0x55,0xAA时，低Byte作为Slave的IIC地址
		{		
			I2C_StructInit(&I2C_InitStructure);
			I2C_InitStructure.I2C_Mode 			=	I2C_Mode_I2C;
			I2C_InitStructure.I2C_OwnAddress1	=	(u8)inf_data.IIC_addr;
			I2C_InitStructure.I2C_Ack		= 	I2C_Ack_Enable;
			I2C_Init(I2C1,&I2C_InitStructure);
			I2C_StretchClockCmd(I2C1,DISABLE); //Disable Pull Down SCL 
			I2C_ITConfig(I2C1,I2C_IT_EVT|I2C_IT_BUF,ENABLE);
			I2C_Cmd(I2C1,ENABLE);
		}
		IIC_Buff[0]=0x90;
		IIC_Buff[1]=0x00;
		IIC_Buff[2]=IIC_Buff[0]^IIC_Buff[1];
		IIC_BuffLen=0x03;
	}
	
}

bool FM349_DownloadCos(unsigned char portn,unsigned int destaddr)
{

		INF_STRUCT inf_data;
		USART_TypeDef *USARTn;
		u32 pagesize;
		u8 send_data,i;
	
		u32 cos_ptr;
		u32 crc_value;
		bool temp_value;
		//CLA-INS-P1-P2-LC-LE-Data[LC]
		u8 apdu01[]={0x00,0x01,0x00,0x00,0x00,0x00};//初始化开始指令
		u8 apdu02[]={0x00,0x02,0x03,0x00,0x00,0x00};//带页擦编程模式设置
		u8 apdu03[]={0x00,0x02,0x01,0x00,0x00,0x00};//不带页擦的编程模式
		
		InfStruct_Read(&inf_data);
	
		crc_value=CRC_Calu(inf_data.cos1_startaddr,inf_data.cos1_endaddr);
		temp_value=(inf_data.cos1_startaddr>=COS_START_ADDR)&&(inf_data.cos1_startaddr<COS_BOUNDADDR);
		temp_value&=(inf_data.cos1_endaddr>=COS_START_ADDR)&&(inf_data.cos1_endaddr<COS_BOUNDADDR);
		temp_value&=(inf_data.cos1_len==(inf_data.cos1_endaddr-inf_data.cos1_startaddr));
		temp_value&=(crc_value==inf_data.cos1_crc);
		if(temp_value==0x00)
			return 0;
		
		if((inf_data.cos1_startaddr%0x80)||(inf_data.cos1_endaddr%0x80)||(destaddr%0x80))
		{
			return 0;
		}
		//Flash 	512Byte
		//EEProm 	128Byte
		if(destaddr<0x60000)
			pagesize=0x200;
		else if((destaddr>=0xB00000)&&(destaddr<0xB28000))
			pagesize=0x80;
		else 
			pagesize=0x00;
		
		if(portn==0x01)//port1
			USARTn=USART1;
		else if(portn==0x02)
			USARTn=USART3;
		else 
			USARTn=USART2;
		
		SC_ColdReset(USARTn,SC_ATR_Table,SC_ATR_LEN);
		SC_ApduWrite(apdu01);
		SC_ApduHander(USARTn,&apdu_commands,&apdu_responce);//0x01指令
		if((apdu_responce.SW1!=0x90)||(apdu_responce.SW2!=0x00))
			return 0;
			
		for(cos_ptr=0;cos_ptr<inf_data.cos1_len;cos_ptr=cos_ptr+0x80)
		{

			if((cos_ptr==0)||((destaddr+cos_ptr)%pagesize==0))
			{
				SC_ApduWrite(apdu02);
				SC_ApduHander(USART1,&apdu_commands,&apdu_responce);
				if((apdu_responce.SW1!=0x90)||(apdu_responce.SW2!=0x00))
					return 0;				
			}
			else
			{
				SC_ApduWrite(apdu03);
				SC_ApduHander(USART1,&apdu_commands,&apdu_responce);
				if((apdu_responce.SW1!=0x90)||(apdu_responce.SW2!=0x00))
					return 0;
			}
			/* Send header -------------------------------------------------------------*/
			send_data = 0x00;
			SC_ByteSend(USARTn, send_data);   
			send_data = 0x00;
			SC_ByteSend(USARTn, send_data); 
			send_data = (u8)((destaddr+cos_ptr)>>16);
			SC_ByteSend(USARTn, send_data);
			send_data = (u8)((destaddr+cos_ptr)>>8);
			SC_ByteSend(USARTn, send_data);
			send_data = (u8)(0x80&(destaddr+cos_ptr));
			SC_ByteSend(USARTn, send_data); 
			(void)USART_GetFlagStatus(USARTn,USART_FLAG_RXNE);//Clear USART_SR ORE NE FE PE
			(void)USART_ReceiveData(USARTn);//Clear DR						
			if((SC_ByteReceive(USARTn,&send_data,SC_Receive_Timeout)) == SUCCESS)
			{
				if(send_data!=0x00)
					return 0;
				else
				{
					for(i=0;i<0x80;i++)
					{
						send_data=*((u8*)(inf_data.cos1_startaddr+cos_ptr+i));
						SC_ByteSend(USARTn, send_data);
					}
					(void)USART_GetFlagStatus(USARTn,USART_FLAG_RXNE);//Clear USART_SR ORE NE FE PE
					(void)USART_ReceiveData(USARTn);//Clear DR
					if((SC_ByteReceive(USARTn,&send_data,SC_Receive_Timeout)) == SUCCESS)
					{
						if(send_data!=0x90)
							return 0;
					}
					else 
						return 0;
					
					if((SC_ByteReceive(USARTn,&send_data,SC_Receive_Timeout)) == SUCCESS)
					{
						if(send_data!=0x00)
							return 0;
					}
					else 
						return 0;					
				}	
			}	
			else 
				return 0;
		}
	//TODO
	return 1;
}
	
bool FMxx_DownloadCos(unsigned char portn,unsigned int startaddr)
{
	//TODO
	return 1;
}
