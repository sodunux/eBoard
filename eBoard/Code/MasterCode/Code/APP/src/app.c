#include "includes.h"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "bsp.h"
#include "program.h"



int  main (void)
{
	
	BSP_Init();
	
	while(1)
	{
		switch(program_sel)
		{
			case 0x80:
				Program1();break;//PC download Cos to Master
			case 0x40:
				Program2();break;//Download COS to Slave, Enter Auto,DownLoad Cos to Card
			case 0x20:
				Program3();break;//Nop
			case 0x10:
				Program4();break;//Nop
			case 0x08:
				Program5();break;//Nop
			case 0x04:
				Program6();break;//Nop
			case 0x02:
				Program7();break;//Nop
			case 0x01:
				Program8();break;//Nop
			default:
				ProgramDebug();break;	//PC Send data to Slave Via IIC 
		}
		
	}
    return (0);
}



