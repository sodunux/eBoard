#include "cmdrun.h"

void Pc_Master_Handler()
{
    switch(UART_CMD_INS)
    {
        case 0x00:
            {
                break;
            }
        case 0x01:
            {
                break;
            }
        case 0x02:
            {
                break;
            }
        default:
            {
                uart_tx_buff[0]=0x90;
                uart_tx_cnt=0x01;
            }
    }
}

void Pc_Slave_Handler()
{
    int i;
    //UART_CMD_P1 code iic addr;
    if(UART_CMD_P1>31) //not support IIC addr >31
    {
        uart_tx_buff[0] = 0xE0;
        uart_tx_cnt = 0x01;
    }
    else
    {
        IIC_SendData(UART_CMD_P1,uart_tx_buff,uart_tx_cnt);
        for(i=0;i<8100;i++);
        uart_rx_cnt=0x10;
        IIC_ReadData(UART_CMD_P1,uart_rx_buff,uart_rx_cnt);
    }
}

