#include "flash.h"

void Flash_Demo()
{
	static u32 addr=0x08010000;
	u32 RecodeData1=0xaa55aa55;
	u32 RecodeData2=0x55aa55aa;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(addr);
	FLASH_ProgramWord(addr,RecodeData1);
	FLASH_ProgramWord(addr,RecodeData2);
	//FLASH_ProgramWord(addr+0x400,RecodeData);
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_Lock();
	//addr+=0x04;
}

void CRC_Demo()
{
	u32 tmp[5]={1,2,3,4,5};
	u32 rtn1,rtn2;
	//Enable CRC Clock
	CRC_ResetDR();
//	rtn1=CRC_CalcCRC(0x32323232);
	rtn2=CRC_CalcBlockCRC(tmp,5);
}

//º∆À„CRC÷µ
u32 CRC_Calu(u32 startaddr,u32 endaddr)
{
	u32 rtn,tmpdat,tmpaddr;
	CRC_ResetDR();
	for(tmpaddr=startaddr;tmpaddr<endaddr;tmpaddr=tmpaddr+4)
	{
		tmpdat=*((u32 *)tmpaddr);
		rtn=CRC_CalcCRC(tmpdat);
	}
	return rtn;
}

void InfStruct_Init(INF_STRUCT *inf)
{
		u8 i;
		inf->cos1_startaddr=COS_START_ADDR;
		inf->cos1_endaddr=COS_START_ADDR;
		inf->cos1_len=0;
		inf->cos1_crc=0;
		
		inf->cos2_startaddr=COS_START_ADDR;
		inf->cos2_endaddr=COS_START_ADDR;
		inf->cos2_len=0;
		inf->cos2_crc=0;
		
		for(i=0;i<16;i++)
		{
			inf->APDU1_Request[i]=0;
			inf->APDU2_Request[i]=0;
			inf->APDU3_Request[i]=0;
			inf->APDU4_Request[i]=0;
			inf->APDU5_Request[i]=0;	
			
			inf->APDU1_Response[i]=0;
			inf->APDU2_Response[i]=0;
			inf->APDU3_Response[i]=0;
			inf->APDU4_Response[i]=0;
			inf->APDU5_Response[i]=0;
		}

		inf->port1_s1=0;
		inf->port1_s2=0;
		inf->port1_s3=0;
		inf->port1_s4=0;
		
		inf->port2_s1=0;
		inf->port2_s2=0;
		inf->port2_s3=0;
		inf->port2_s4=0;
		
		inf->port3_s1=0;
		inf->port3_s2=0;
		inf->port3_s3=0;
		inf->port3_s4=0;		
		
		inf->IIC_addr=0xA5;
		inf->Auto_Debug=0xAA5555AA;
		inf->Port_ID=1;
		
}

void InfStruct_Write(INF_STRUCT *inf)
{
	u32 dest_addr,len;
	u8 *sourdat;
	sourdat=(u8 *)inf;
	dest_addr=INF_START_ADDR;
	len=sizeof(*inf);
	Flash_Erase(INF_START_ADDR);
	Flash_Program(sourdat,dest_addr,len);
}


void InfStruct_Read(INF_STRUCT *inf)
{
	*inf=*((INF_STRUCT *)INF_START_ADDR);
}


bool Flash_Erase(u32 page_addr)
{
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(page_addr);
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);	
	FLASH_Lock();
	return 0;
}



bool Flash_Program(u8 *sour_data,u32 dest_addr,u32 len)
{
	u32 wordlen,leftcnt;
	u32 tailword,tmpword;
	u32 iptr=0;
	u32 tmp;
	u8 b1,b2,b3,b4;
	FLASH_Status status = FLASH_COMPLETE;
	if(dest_addr%4)
		return 1;
	wordlen=len/4;
	leftcnt=len-wordlen*4;
	switch(leftcnt)
	{
		case 0:
			tailword=0xFFFFFFFF;
			break;
		
		case 1:
			b1=*(sour_data+wordlen*4+0);
			tailword=b1|0xFFFFFF00;
			break;
		
		case 2:
			b1=*(sour_data+wordlen*4+0);
			b2=*(sour_data+wordlen*4+1);
			tailword=b1|(b2<<8)|0xFFFF0000;
			break;
		
		case 3:
			b1=*(sour_data+wordlen*4+0);
			b2=*(sour_data+wordlen*4+1);
			b3=*(sour_data+wordlen*4+2);
			tailword=b1|(b2<<8)|(b3<<16)|0xFF000000;
			break;
		
		default:
			break;
	}
	
	//FLASH_Unlock();
	//FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	while(iptr<wordlen)
	{
		b1=*(sour_data+iptr*4+0);
		b2=*(sour_data+iptr*4+1);
		b3=*(sour_data+iptr*4+2);
		b4=*(sour_data+iptr*4+3);
		FLASH_Unlock();
		//FLASH_SetLatency(FLASH_Latency_2);
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
		tmpword=(b4<<24)+(b3<<16)+(b2<<8)+b1;	
		status=FLASH_ProgramWord((dest_addr+iptr*4),tmpword);
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
		FLASH_Lock();
		iptr++;
	}
	
	if(leftcnt)
	{
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
		while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
		FLASH_ProgramWord((dest_addr+iptr*4),tailword);
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
		FLASH_Lock();
	}
	


	return 0;
}