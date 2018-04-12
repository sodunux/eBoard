/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "stm32f10x.h"
#include "smartcard.h"
#include "bsp.h"
#include "Instr.h"
#include "stm32f10x_it.h"

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

int  main (void)
{
	BSP_Init();
	Auto_Init();
	while(1)
	{
		AutoHandler();
		InstrHandler();
	}	
}




