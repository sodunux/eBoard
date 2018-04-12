#ifndef __PROGRAM_H
#define __PROGRAM_H
#include "bsp.h"

#define MASTER_INF_ADDR 0x8010000
#define MASTER_COS_START_ADDR 0x8010400
#define SLAVE_COS_START_ADDR 0x8010800
#define SLAVE_INF_START_ADDR 0x8010400
//00000400-00000000-55aa55aa55aa55aa55aa55aa55aa55aa-0a0b0c0d0a0b0c0d0a0b0c0d0a0b0c0d

extern u8 program_sel;

typedef struct 
{
	u32 cos_len;
	u32 cos_crc;
	u8 apdu_request[16];
	u8 apdu_response[16];
}CosInfStruct;

void CosInfStruct_Read(CosInfStruct * inf);
void CosInfStruct_Write(CosInfStruct * inf);

u8 BccCalu(u8 *buff,u8 len);
void Program1();//PC Download Cos to Master
void Program2();//Master Download Cos to Slave
void Program3();//Enter Auto
void Program4();
void Program5();
void Program6();
void Program7();
void Program8();
void ProgramDebug();//PC Debug Slave
void ProgramHandler1();
bool ProgramHandler2();
bool ProgramHandler3();
void ProgramHandler4();
void ProgramHandler5();
void ProgramHandler6();
void ProgramHandler7();
void ProgramHandler8();
void ProgramHandlerDebug();

#endif