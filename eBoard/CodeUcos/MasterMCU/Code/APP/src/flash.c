#include "flash.h"
/*******************************************************************************
* Function Name  : Flash_Erase
* Description    : None
* Input          : The page addr used to be erase.
* Return         : 0 Okay; 1 Error;
*******************************************************************************/
u32 Flash_Erase(u32 page_addr)
{
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(page_addr);
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);	
	FLASH_Lock();
	return 0;
}

/*******************************************************************************
* Function Name  : Flash_Program
* Description    : None
* Input          : dest_addr align 4bytes.
* Return         : None.
*******************************************************************************/
u32 Flash_Program(u8 *sour_data,u32 dest_addr,u32 len)
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
