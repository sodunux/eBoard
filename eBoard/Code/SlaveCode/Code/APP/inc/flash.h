#include "stm32f10x.h"
#include "smartcard.h"

/*
//�ֻ���Ҫ�Ĺ�����
//								1.���س��򵽷ֻ�
//								2.���طֻ����򵽹�λ
//								3.�ֻ�����APDUָ���λ
//								4.��ȡ�ֻ����߹�λ��һЩ״̬��Ϣ
//								5.��������һЩ�����źŵ��ֻ���BUFF��
//									(1)�ֻ���һ���������ÿ�β�ѯBUFF�ĸ�����ֵ��
//									(2)Ȼ�����BUFF�Ĳ�ֵͬ������Ӧ��Case��ת
//									(3)��̵Ĺؼ����ڴ�BUFF�����ݽṹ�Ļ��֡�
//									(4)�����ϴ洢�ռ仮��ΪINF����COS����
//�洢���Ļ��֣�											
//									|COS2	| COS2_START_ADDR
//									|COS1	|	COS1_START_ADDR
//									|INF  | INF_START_ADDR

//INF����Ҫ�洢��Щ��Ϣ��
//												1.COS1����ʼ��ַ
//												2.COS1������ַ
//												3.COS1��CRC
//												4.COS2����ʼ��ַ
//												5.COS2�Ľ�����ַ
//												6.COS2��CRC
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
