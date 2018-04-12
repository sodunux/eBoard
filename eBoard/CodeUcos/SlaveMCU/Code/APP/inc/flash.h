#ifndef __FLASH_H__
#define __FLASH_H__
#include "includes.h"

u32 Flash_Erase(u32 page_addr);
u32 Flash_Program(u8 *sour_data,u32 dest_addr,u32 len);

#endif

