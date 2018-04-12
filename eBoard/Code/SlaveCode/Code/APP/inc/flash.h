#include "stm32f10x.h"
#include "smartcard.h"

/*
//分机主要的工作：
//								1.下载程序到分机
//								2.下载分机程序到工位
//								3.分机发送APDU指令到工位
//								4.获取分机或者工位的一些状态信息
//								5.主机发送一些控制信号到分机的BUFF。
//									(1)分机的一个任务就是每次查询BUFF的各个数值。
//									(2)然后根据BUFF的不同值进行相应的Case跳转
//									(3)编程的关键在于此BUFF的数据结构的划分。
//									(4)大体上存储空间划分为INF区和COS区。
//存储器的划分：											
//									|COS2	| COS2_START_ADDR
//									|COS1	|	COS1_START_ADDR
//									|INF  | INF_START_ADDR

//INF区需要存储哪些信息：
//												1.COS1的起始地址
//												2.COS1结束地址
//												3.COS1的CRC
//												4.COS2的起始地址
//												5.COS2的结束地址
//												6.COS2的CRC
//								
*/
#ifndef __FLASH__
#define __FLASH__

#define INF_START_ADDR 		0x08010400
#define COS_START_ADDR		(INF_START_ADDR+0x400)
#define COS_PAGE_ADDR(x)	(COS_STARTADDR+0x400*x)  
#define COS_BOUNDADDR 		0x08020000



typedef struct 
{
	//cos1
	u32 cos1_startaddr;
	u32 cos1_endaddr;
	u32 cos1_len;
	u32 cos1_crc;
	
	//cos2
	u32 cos2_startaddr;
	u32 cos2_endaddr;
	u32 cos2_len;
	u32 cos2_crc;
	
	//APDU
	u8 APDU1_Request[16];
	u8 APDU1_Response[16];
	u8 APDU2_Request[16];
	u8 APDU2_Response[16];	
	u8 APDU3_Request[16];
	u8 APDU3_Response[16];
	u8 APDU4_Request[16];
	u8 APDU4_Response[16];	
	u8 APDU5_Request[16];
	u8 APDU5_Response[16];		
	
	//PortStatus
	u32 port1_s1;
	u32 port1_s2;
	u32 port1_s3;
	u32 port1_s4;
	
	u32 port2_s1;
	u32 port2_s2;
	u32 port2_s3;
	u32 port2_s4;

	u32 port3_s1;
	u32 port3_s2;
	u32 port3_s3;
	
	u32 port3_s4;
	
	//Other
	u32 IIC_addr;
	u32 Auto_Debug;
	u32 Port_ID;
	u32 INF_CRC;
}INF_STRUCT;

void InfStruct_Init(INF_STRUCT *inf);
void InfStruct_Write(INF_STRUCT *inf);
void InfStruct_Read(INF_STRUCT *inf);

bool Flash_Erase(u32 page_addr);
bool Flash_Program(u8 *sour_data,u32 dest_addr,u32 len);
u32 CRC_Calu(u32 startaddr,u32 endaddr);
void Flash_Demo(void);

#endif
