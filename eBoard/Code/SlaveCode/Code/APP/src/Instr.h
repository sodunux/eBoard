#ifndef __INSTR__
#define __INSTR__

#include "stm32f10x.h"

//To Slave Instruction
#define WRITECOS 0x53
#define READCOS	0x51
#define WP_COLDRESET 0x01
#define WP_CLOSE		0x02
#define WP_LEDCTRL	0x03

#define WRITEINF	0xA3
#define READINF		0xA1
#define DOWNLOAD_COS 0x04
#define CHANGE_IICADDR 0x05
//Return Code 
//0x6B00 参数是错误的
//0x6D00 指令不被编码或者无效
//0x6700 长度不对
//0x6E00 卡不支持这类指令
//0x6F00 未给出正确的诊断
//0x6A00 传输BCC错误
//0x9000 Right APDU

void INS_WriteCos(void);
void INS_ReadCos(void);
void INS_WpColdReset(void);
void INS_WpClose(void);
void INS_WpLedCtrl(void);
void INS_EnterAuto(void);
void INS_ExitAuto(void);
void INS_WriteInf(void);
void INS_ReadInf(void);
void AutoHandler(void);
void InstrHandler(void);
void INS_DownloadCos(void);
bool FM349_DownloadCos(unsigned char portn,unsigned int startaddr);
bool FMxx_DownloadCos(unsigned char portn,unsigned int startaddr);
void INS_ChangeIicAddr(void);
void Auto_Init(void);
#endif
