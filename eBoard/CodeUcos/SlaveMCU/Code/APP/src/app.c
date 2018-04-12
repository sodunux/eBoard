/*
*********************************************************************************************************
*                                              Main code
*
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : BAN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK      App_TaskStartStk[APP_TASK_STK_SIZE];
static  OS_STK      App_Task_A_Stk[APP_TASK_STK_SIZE];
static  OS_STK			App_Task_B_Stk[APP_TASK_STK_SIZE]; 
static  OS_STK			App_Task_C_Stk[APP_TASK_STK_SIZE];
static  OS_STK      App_Task_D_Stk[APP_TASK_STK_SIZE];
static  OS_STK			App_Task_E_Stk[APP_TASK_STK_SIZE]; 
static  OS_STK			App_Task_F_Stk[APP_TASK_STK_SIZE]; 					
INT8U err;
OS_EVENT *uart_receive_1;
OS_EVENT *uart_receive_2;
OS_EVENT *uart_receive_3;



/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static  void  	App_TaskStart (void *p_arg);
static  void  	App_Task_A    (void       *p_arg);
static  void  	App_Task_B    (void       *p_arg);
static 	void 		App_Task_C		(void 		*p_arg);
static  void  	App_Task_D    (void       *p_arg);
static  void  	App_Task_E    (void       *p_arg);
static 	void 		App_Task_F		(void 		*p_arg);

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
  OSInit();/* Initialize "uC/OS-II, The Real-Time Kernel"*/
  OSTaskCreate(
								App_TaskStart,
								(void*)0,
								&App_TaskStartStk[APP_TASK_STK_SIZE-1],
								APP_TASK_START_PRIO
								);
  OSStart();/* Start multitasking (i.e. give control to uC/OS-II)*/
  return (0);
}


/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskStart (void *p_arg)
{
  (void)p_arg;
	BSP_Init();
	uart_receive_1= OSSemCreate(0);
	uart_receive_2= OSSemCreate(0);
	uart_receive_3= OSSemCreate(0);
	OSStatInit(); 

	OSTaskCreate(
				(void (*)(void *)) App_Task_A, 
				NULL, 
				&App_Task_A_Stk[APP_TASK_LED_STK_SIZE-1],
				APP_TASK_A_PRIO
				);	
	
	OSTaskCreate(
				(void (*)(void *)) App_Task_B,  //USART1
				NULL, 
				&App_Task_B_Stk[APP_TASK_CCID_STK_SIZE-1],
				APP_TASK_B_PRIO
				);

	OSTaskCreate(
				(void (*)(void *)) App_Task_C, //USART2
				NULL, 
				&App_Task_C_Stk[APP_TASK_CCID_STK_SIZE-1],
				APP_TASK_C_PRIO
				);
	OSTaskCreate(
				(void (*)(void *)) App_Task_D, //USART3
				NULL, 
				&App_Task_D_Stk[APP_TASK_CCID_STK_SIZE-1],
				APP_TASK_D_PRIO
				);
	OSTaskCreate(
				(void (*)(void *)) App_Task_E, //IIC Cmd
				NULL, 
				&App_Task_E_Stk[APP_TASK_CCID_STK_SIZE-1],
				APP_TASK_E_PRIO
				);
	OSTaskCreate(
				(void (*)(void *)) App_Task_F, //RFU for test
				NULL, 
				&App_Task_F_Stk[APP_TASK_CCID_STK_SIZE-1],
				APP_TASK_F_PRIO
				);
	while(1)
	{
		OSTaskSuspend(OS_PRIO_SELF);
	}
}


/*
*********************************************************************************************************
*                                            App_Task_A()
*
* Description : *******
*
* Argument(s) : p_arg       Argument passed to 'App_TaskLedCount()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void App_Task_A(void *p_arg)
{
	u8 ledstate;
	(void)p_arg;
	while(1)
	{
		if(ledstate)
		{
			LEDShow(GREEN1_PORT,GREEN1,LED_ON);
			LEDShow(GREEN2_PORT,GREEN2,LED_ON);
			LEDShow(GREEN3_PORT,GREEN3,LED_ON);
			LEDShow(RED1_PORT,RED1,LED_ON);
			LEDShow(RED2_PORT,RED2,LED_ON);
			LEDShow(RED3_PORT,RED3,LED_ON);		
			ledstate=0;
		}
		else
		{
			LEDShow(GREEN1_PORT,GREEN1,LED_OFF);
			LEDShow(GREEN2_PORT,GREEN2,LED_OFF);
			LEDShow(GREEN3_PORT,GREEN3,LED_OFF);
			LEDShow(RED1_PORT,RED1,LED_OFF);
			LEDShow(RED2_PORT,RED2,LED_OFF);
			LEDShow(RED3_PORT,RED3,LED_OFF);
			ledstate=1;
		}
		OSTimeDlyHMSM(0, 0, 0, 400);
	}
}

/*
*********************************************************************************************************
*                                            App_Task_B()
*
* Description : *******
*
* Argument(s) : p_arg       Argument passed to 'App_TaskLedCount()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void App_Task_B(void *p_arg)
{
	(void)p_arg;
	while(1)
	{
		OSTaskSuspend(OS_PRIO_SELF);
		//OSTimeDlyHMSM(0, 0, 0, 500);
	}
}


/*
*********************************************************************************************************
*                                            App_Task_C()
*
* Description : *******
*
* Argument(s) : p_arg       Argument passed to 'App_TaskLedCount()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void App_Task_C(void *p_arg)
{
	(void)p_arg;
	while(1)
	{
		OSTaskSuspend(OS_PRIO_SELF);
	}
}

/*
*********************************************************************************************************
*                                            App_Task_D()
*
* Description : *******
*
* Argument(s) : p_arg       Argument passed to 'App_TaskLedCount()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void App_Task_D(void *p_arg)
{
	(void)p_arg;
	while(1)
	{
		OSTaskSuspend(OS_PRIO_SELF);
	}
}
/*
*********************************************************************************************************
*                                            App_Task_E()
*
* Description : *******
*
* Argument(s) : p_arg       Argument passed to 'App_TaskLedCount()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void App_Task_E(void *p_arg)
{
	(void)p_arg;
	while(1)
	{
		OSTaskSuspend(OS_PRIO_SELF);
	}
}
/*
*********************************************************************************************************
*                                            App_Task_F()
*
* Description : *******
*
* Argument(s) : p_arg       Argument passed to 'App_TaskLedCount()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void App_Task_F(void *p_arg)
{
	(void)p_arg;
	SC_Init(USART1);
	SC_Init(USART2);
	SC_Init(USART3);
	//SC_DirCmd(USART1,DIR_SEND);
	//SC_DirCmd(USART2,DIR_SEND);
	//SC_DirCmd(USART3,DIR_SEND);	
	while(1)
	{
//		uartbuff_1[0] = 0x55;
//		uartbuff_1[1] = 0xAA;
//		uartbuff_1[2] = 0xFF;
//		uartbuff_1[3] = 0x5A;
//		uartbuff_1[4] = 0x00;

//		uartbuff_2[0] = 0xAA;
//		uartbuff_2[1] = 0xAA;
//		uartbuff_2[2] = 0x55;
//		uartbuff_2[3] = 0xAB;
//		uartbuff_2[4] = 0xAC;
//		
//		uartbuff_3[0] = 0xA4;
//		uartbuff_3[1] = 0xAA;
//		uartbuff_3[2] = 0xFF;
//		uartbuff_3[3] = 0xED;
//		uartbuff_3[4] = 0xF1;
//		
//		SC_Transmit(USART1,uartbuff_1,0x01,0x00);
//		SC_Transmit(USART2,uartbuff_2,0x01,0x00);
//		SC_Transmit(USART3,uartbuff_3,0x01,0x00);

		
		
		SC_ColdReset(USART2);
		SC_ColdReset(USART3);
		SC_ColdReset(USART1);
		OSTimeDlyHMSM(0, 0, 0, 400);
		//OSTaskSuspend(OS_PRIO_SELF);
	}
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                          uC/OS-II APP HOOKS
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_APP_HOOKS_EN > 0)
/*
*********************************************************************************************************
*                                      TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskCreateHook (OS_TCB *ptcb)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TaskCreateHook(ptcb);
#endif
}

/*
*********************************************************************************************************
*                                    TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskDelHook (OS_TCB *ptcb)
{
    (void)ptcb;
}

/*
*********************************************************************************************************
*                                      IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void  App_TaskIdleHook (void)
{
}
#endif

/*
*********************************************************************************************************
*                                        STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument(s) : none.
*********************************************************************************************************
*/

void  App_TaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                        TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*
*               (2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void  App_TaskSwHook (void)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TaskSwHook();
#endif
}
#endif

/*
*********************************************************************************************************
*                                     OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument(s) : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void  App_TCBInitHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/*
*********************************************************************************************************
*                                        TICK HOOK (APPLICATION)
*
* Description : This function is called every tick.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_TIME_TICK_HOOK_EN > 0
void  App_TimeTickHook (void)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TickHook();
#endif
}
#endif
#endif
