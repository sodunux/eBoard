#include "crc.h"

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

u8 BCC_Calu(u8 *buff,u32 len)
{
	int i;
	u8 bccvalue=0;
	for(i=0;i<len;i++)
	{
		bccvalue ^= buff[i];
	}
	return bccvalue;
}
