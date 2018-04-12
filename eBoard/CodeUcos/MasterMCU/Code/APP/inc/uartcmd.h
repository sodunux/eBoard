#ifndef __UARTCMD_H__
#define __UARTCMD_H__
#include "includes.h"
#define	UART_CMD_CLA   (uart_rx_buff[0])
#define	UART_CMD_INS	 (uart_rx_buff[1])
#define UART_CMD_P1		 (uart_rx_buff[2])
#define UART_CMD_P2    (uart_rx_buff[3])
#define UART_CMD_LEN	 (uart_rx_buff[4])
#define UART_CMD_DATAP  (uart_rx_buff+5)

extern u8 uart_tx_buff[128];
extern u8 uart_rx_buff[128];
extern u8 uart_tx_cnt;
extern u8 uart_rx_cnt;

u8 Uart_Cmd_Valid();
void Uart_Buff_Clear(void);
void Uart_Cmd_Wait(void);
void Uart_Cmd_Parser(void);
void Uart_Cmd_Resp(void);
u8 Uart_Cmd_Valid();
void Uart_DMAR_Handler(void);
void Uart_DMAT_Handler(void);
#endif