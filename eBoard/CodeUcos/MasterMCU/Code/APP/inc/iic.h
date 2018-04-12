#ifndef __IIC_H__
#define __IIC_H__
#include "includes.h"

void IIC_Delay(void);
bool IIC_Start(u8 IICn);
void IIC_Stop(u8 IICn);
void IIC_Ack(u8 IICn);
void IIC_NoAck(u8 IICn);
bool IIC_WaitAck(u8 IICn);
void IIC_SendByte(u8 IICn,u8 sendbyte);
u8 IIC_ReadByte(u8 IICn) ;
u8 IIC_SendData(u8 addr,u8 *senddata,u8 len);
u8 IIC_ReadData(u8 addr,u8 *readbuf,u8 len);

#endif

