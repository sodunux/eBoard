#ifndef __CRC_H__
#define __CRC_H__
#include "includes.h"

u32 CRC_Calu(u32 startaddr,u32 endaddr);
u8 BCC_Calu(u8 *buff,u32 len);
#endif
