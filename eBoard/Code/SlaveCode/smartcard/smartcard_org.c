/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : smartcard.c
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : This file provides all the Smartcard firmware functions.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "smartcard.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables definition and initialization ----------------------------*/
SC_ATR SC_A2R;
u8 SC_ATR_Table[40];
static vu8 SCData = 0;
static u32 F_Table[16] = {372, 372, 558, 744, 1116, 1488, 2232, 0,
                          0, 512, 768, 1024, 1536, 2048, 0, 0};
static u32 D_Table[8] = {0, 1, 2, 4, 8, 16, 32, 0,12,20,0,0,0,0,0,0};

SC_State scstate;

SC_ADPU_Commands apdu_commands;
SC_ADPU_Responce apdu_responce;

/* Private function prototypes -----------------------------------------------*/
/* Transport Layer -----------------------------------------------------------*/
/*--------------APDU-----------*/
void SC_SendData(USART_TypeDef* USARTn,SC_ADPU_Commands *SC_ADPU, SC_ADPU_Responce *SC_ResponceStatus);
/*------------ ATR ------------*/
void SC_AnswerReq(USART_TypeDef* USARTn, u8 *card, u8 length);  /* Ask ATR */
static u8 SC_decode_Answer2reset(u8 *card);  /* Decode ATR */

/* Physical Port Layer -------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : SC_DirCmd
* Description    : Send(1) or receive(0)
* Input          : 1-Send to SC, 0-Receive from SC
*                  - USARTn  USART1~USART3
* Output         : None
* Return         : None
*******************************************************************************/
void SC_DirCmd(USART_TypeDef* USARTn,bool direction)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
  if(USARTn==USART1)
  {
    if(direction) //SEND
		{
			if(!GPIO_ReadOutputDataBit(SC_DIR1_PORT,SC_DIR1))
			{
				USART1->CR2&=(~USART_CR2_CLKEN);
				//IO1:GPIOA9,GPIO_Mode_AF_PP
				GPIOA->CRH&=~(1<<6);
				GPIO_SetBits(SC_DIR1_PORT, SC_DIR1);
				USART1->CR2|=USART_CR2_CLKEN;
			}		
		}
    else
		{
				if(GPIO_ReadOutputDataBit(SC_DIR1_PORT,SC_DIR1))
				{
					USART1->CR2&=(~USART_CR2_CLKEN);
          //IO1:GPIOA9,GPIO_Mode_AF_OD
					GPIOA->CRH|=(1<<6);
					GPIO_ResetBits(SC_DIR1_PORT,SC_DIR1);
					USART1->CR2|=USART_CR2_CLKEN;					
				}
		}     
  }

  else if(USARTn==USART2)
  {
    if(direction) //SEND
		{

			GPIO_SetBits(SC_DIR2_PORT, SC_DIR2);
		}
    else
		{
			GPIO_ResetBits(SC_DIR2_PORT,SC_DIR2);
		}
  }
  else
  {
    if(direction) //SEND
		{
			GPIO_SetBits(SC_DIR3_PORT, SC_DIR3);
		}
    else
		{
			GPIO_ResetBits(SC_DIR3_PORT,SC_DIR3);
		}
  }
}



/*******************************************************************************
* Function Name  : SC_PowerCmd
* Description    : Enables or disables the power VCC to the Smartcard.
* Input          : NewState: new state of the Smartcard power supply. 
*                  This parameter can be: ENABLE(VCC=1) or DISABLE(VCC=0).
*                  - USARTn  USART1~USART3
* Output         : None
* Return         : None
* Author 				 : Yanpengfei
*******************************************************************************/
void SC_PowerCmd(USART_TypeDef* USARTn,  NewState)
{
  //USART1
  if(USARTn==USART1)
  {
    if(NewState == ENABLE)
      GPIO_SetBits(SC_VCC1_PORT, SC_VCC1);
    else
      GPIO_ResetBits(SC_VCC1_PORT, SC_VCC1);

  }
  //USART2
  else if(USARTn==USART2)
  {
    if(NewState == ENABLE)
      GPIO_SetBits(SC_VCC2_PORT, SC_VCC2);
    else
      GPIO_ResetBits(SC_VCC2_PORT, SC_VCC2);

  }
  //USART3
  else 
  {
    if(NewState == ENABLE)
      GPIO_SetBits(SC_VCC3_PORT, SC_VCC3);
    else
      GPIO_ResetBits(SC_VCC3_PORT, SC_VCC3);

  }

}

/*******************************************************************************
* Function Name  : SC_Reset
* Description    : Sets or clears the Smartcard reset pin.
* Input          : - ResetState: this parameter specifies the state of the Smartcard 
*                    reset pin.
*                    BitVal must be one of the BitAction enum values:
*                       - Bit_RESET: to clear the port pin.
*                       - Bit_SET: to set the port pin.
*                  - USARTn  USART1~USART3
* Output         : None
* Return         : None
*******************************************************************************/
void SC_Reset(USART_TypeDef* USARTn,BitAction ResetState)
{
  if(USARTn==USART1)
    GPIO_WriteBit(SC_RST1_PORT, SC_RST1, ResetState);
  else if(USARTn==USART2)
    GPIO_WriteBit(SC_RST2_PORT, SC_RST2, ResetState); 
  else 
    GPIO_WriteBit(SC_RST3_PORT, SC_RST3, ResetState);   
}

/*******************************************************************************
* Function Name  : SC_CLOSE
* Description    : Close the SC_CLOSE
* Input          :  USARTn,USART1 ~ USART3
*                  
* Output         : None
* Return         : None
*******************************************************************************/
void SC_Close(USART_TypeDef* USARTn)
{
  SC_Reset(USARTn,Bit_RESET);//Reset RST
  USART_Cmd(USARTn, DISABLE);//Disable CLK
  SC_PowerCmd(USARTn,DISABLE);//Disable VCC
}

/*******************************************************************************
* Function Name  : SC_Handler
* Description    : Handles all Smartcard states and serves to send and receive all
*                  communication data between Smartcard and reader.
* Input          : - SCState: pointer to an SC_State enumeration that will contain
*                    the Smartcard state.
*                  - SC_ADPU: pointer to an SC_ADPU_Commands structure that will be
*                    initialized.  
*                  - SC_Response: pointer to a SC_ADPU_Responce structure which will
*                    be initialized.
* Output         : None
* Return         : None
* Modify         : Yanpengfei 
*******************************************************************************/
void SC_Handler(USART_TypeDef* USARTn,SC_State *SCState, SC_ADPU_Commands *SC_ADPU, SC_ADPU_Responce *SC_Response)
{
  u32 i = 0;

  switch(*SCState)
  {
    case SC_POWER_ON://Initial The Global and Periph
      if (SC_ADPU->Header.INS == SC_GET_A2R)
      {
        /* Smartcard intialization ------------------------------------------*/
        SC_Init(USARTn);
        /* Reset Data from SC buffer -----------------------------------------*/
        for (i = 0; i < 40; i++)
        {
          SC_ATR_Table[i] = 0;
        }       
        /* Reset SC_A2R Structure --------------------------------------------*/
        SC_A2R.TS = 0;
        SC_A2R.T0 = 0;
        for (i = 0; i < SETUP_LENGTH; i++)
        {
          SC_A2R.T[i] = 0;
        }
        for (i = 0; i < HIST_LENGTH; i++)
        {
          SC_A2R.H[i] = 0;
        }
        SC_A2R.Tlength = 0;
        SC_A2R.Hlength = 0;
        
        /* Next State --------------------------------------------------------*/
        *SCState = SC_RESET;
      }
      else
      {
        (*SCState) = SC_POWER_OFF;
      }
    break;

    case SC_RESET://Set Reset and Receive data 
      if(SC_ADPU->Header.INS == SC_GET_A2R)
      {
        SC_AnswerReq(USARTn, &SC_ATR_Table[0], 20); /* Check for answer to reset */
			  *SCState = SC_ACTIVE;
      }
      else
      {
        (*SCState) = SC_POWER_OFF;
      } 

    break;

    case SC_ACTIVE:
      if (SC_ADPU->Header.INS == SC_GET_A2R)
      {
        if(SC_decode_Answer2reset(&SC_ATR_Table[0]) == T0_PROTOCOL)
        {
          (*SCState) = SC_ACTIVE_ON_T0;
        }
        else
        {
          (*SCState) = SC_POWER_OFF; 
        }
      }
    break;

    case SC_ACTIVE_ON_T0:
      SC_SendData(USARTn,SC_ADPU, SC_Response);
    break;

    case SC_POWER_OFF://Close Port
      SC_Close(USARTn);   
    break;

    default:
      (*SCState) = SC_POWER_OFF;
    break;
  }
}





/*******************************************************************************
* Function Name  : SC_PPS
* Description    : Configures the IO speed (BaudRate) communication.
* Input          : PPS1 
*                  USARTn,USART1 ~ USART3
* Output         : 0x00 Failed, 0x01 Succeed
* Return         : None
*******************************************************************************/
u8 SC_PPS(USART_TypeDef* USARTn,u8 PPS1)
{
	RCC_ClocksTypeDef RCC_ClocksStatus;
  u32 workingbaudrate = 0, apbclock = 0;
  u8 locData = 0, PTSConfirmStatus = 1;
  USART_InitTypeDef USART_InitStructure;
  
  /* Reconfigure the USART Baud Rate -------------------------------------------*/
  RCC_GetClocksFreq(&RCC_ClocksStatus);
  apbclock = RCC_ClocksStatus.PCLK1_Frequency;
  apbclock /= ((USARTn->GTPR & (u16)0x00FF) * 2);

  /* Enable the DMA Receive (Set DMAR bit only) to enable interrupt generation
     in case of a framing error FE */  
  USART_DMACmd(USARTn, USART_DMAReq_Rx, ENABLE);
  
  if(PPS1!= 0x11)
    {
      /* Send PTSS */
      SCData = 0xFF;
      USART_SendData(USARTn, SCData);
      while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
      {
      }

      /* Send PTS0 */
      SCData = 0x10;
      USART_SendData(USARTn, SCData);
      while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
      {
      }

      /* Send PTS1 */
      SCData = PPS1; 
      USART_SendData(USARTn, SCData);
      while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
      {
      } 
      
      /* Send PCK */
      SCData = (u8)0xFF^(u8)0x10^(u8)PPS1; 
      USART_SendData(USARTn, SCData);
      while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
      {
      }

      /* Disable the DMA Receive (Reset DMAR bit only) */  
      USART_DMACmd(USARTn, USART_DMAReq_Rx, DISABLE);
   
      if((USART_ByteReceive(USARTn,&locData, SC_Receive_Timeout)) == SUCCESS)
      {
        if(locData != 0xFF)
        {
           PTSConfirmStatus = 0x00;
        }
      }
			if((USART_ByteReceive(USARTn,&locData, SC_Receive_Timeout)) == SUCCESS)
      {
        if(locData != 0xFF)
        {
           PTSConfirmStatus = 0x00;
        }
      }
      if((USART_ByteReceive(USARTn,&locData, SC_Receive_Timeout)) == SUCCESS)
      {
        if(locData != 0x10)
        {
           PTSConfirmStatus = 0x00;
        }
      }
      if((USART_ByteReceive(USARTn,&locData, SC_Receive_Timeout)) == SUCCESS)
      {
        if(locData != PPS1)
        {
           PTSConfirmStatus = 0x00;
        }
      }
      if((USART_ByteReceive(USARTn,&locData, SC_Receive_Timeout)) == SUCCESS)
      {
        if(locData != ((u8)0xFF^(u8)0x10^(u8)PPS1))
        {
           PTSConfirmStatus = 0x00;
        }
      }
      else
      {
        PTSConfirmStatus = 0x00;
      }
      /* PTS Confirm */
      if(PTSConfirmStatus == 0x01)
      {
        workingbaudrate = apbclock * D_Table[(PPS1 & (u8)0x0F)];
        workingbaudrate /= F_Table[((PPS1 >> 4) & (u8)0x0F)];
        USART_StructInit(&USART_InitStructure);
        USART_InitStructure.USART_BaudRate = workingbaudrate;
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        USART_InitStructure.USART_Clock = USART_Clock_Enable;
        USART_Init(USARTn, &USART_InitStructure);

				return 0x01;
      }
			else 
				return 0x00;
    }
	return 0x01;		
}



/*******************************************************************************
* Function Name  : SC_SendData
* Description    : Manages the Smartcard transport layer: send APDU commands and
*                  receives the APDU responce.
* Input          : - SC_ADPU: pointer to a SC_ADPU_Commands structure which 
*                    will be initialized.  
*                  - SC_Response: pointer to a SC_ADPU_Responce structure which 
*                    will be initialized.
                   - USARTn 1~3
* Output         : None
* Return         : None
*******************************************************************************/
void SC_SendData(USART_TypeDef* USARTn,SC_ADPU_Commands *SC_ADPU, SC_ADPU_Responce *SC_ResponceStatus)
{
  u32 i = 0;
  u8 locData = 0;

  /* Reset responce buffer ---------------------------------------------------*/
  for(i = 0; i < LCmax; i++)
  {
    SC_ResponceStatus->Data[i] = 0;
  }
  
  SC_ResponceStatus->SW1 = 0;
  SC_ResponceStatus->SW2 = 0;

  /* Enable the DMA Receive (Set DMAR bit only) to enable interrupt generation
     in case of a framing error FE */  
  USART_DMACmd(USARTn, USART_DMAReq_Rx, ENABLE);

  /* Send header -------------------------------------------------------------*/
  SCData = SC_ADPU->Header.CLA;
  USART_SendData(USARTn, SCData);
  while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
  {		
  }
		
  SCData = SC_ADPU->Header.INS;
  USART_SendData(USARTn, SCData);
  while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
  {
  }
	
  SCData = SC_ADPU->Header.P1;
  USART_SendData(USARTn, SCData);
  while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
  {
  }  
  SCData = SC_ADPU->Header.P2;
  USART_SendData(USARTn, SCData);
  while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
  {
  }   
  /* Send body length to/from SC ---------------------------------------------*/
  if(SC_ADPU->Body.LC) //Send P3
  {
    SCData = SC_ADPU->Body.LC;
    USART_SendData(USARTn, SCData);
    while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
    {
    }
  }
  else if(SC_ADPU->Body.LE)
  { 
    SCData = SC_ADPU->Body.LE;
    USART_SendData(USARTn, SCData);
    while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
    {
    }   
  }
	else
	{
		SCData = 0x00;
    USART_SendData(USARTn, SCData);
    while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
    {
    }
	}

  /* Flush the USARTn DR */
  (void)USART_ReceiveData(USARTn);
//	USART_ByteReceive(&locData, SC_Receive_Timeout*2);


  /* --------------------------------------------------------
    Wait Procedure byte from card:
    1 - ACK
    2 - NULL;"60"
    3 - SW1; SW2
   -------------------------------------------------------- */

  if((USART_ByteReceive(USARTn,&locData,SC_Receive_Timeout)) == SUCCESS)
  {
    while(locData==0x60) //NULL 0x60
    {
      locData=0;
      USART_ByteReceive(USARTn,&locData,SC_Receive_Timeout);
    }

    //SW1 SW2
    if(((locData & (u8)0xF0) == 0x60) || ((locData & (u8)0xF0) == 0x90))
    {
      /* SW1 received */
      SC_ResponceStatus->SW1 = locData;
      if((USART_ByteReceive(USARTn,&locData, SC_Receive_Timeout)) == SUCCESS)
      {
      /* SW2 received */
        SC_ResponceStatus->SW2 = locData;
      }
    }
	//ACK 
   else if(((locData & (u8)0xFE) == (((u8)~(SC_ADPU->Header.INS)) & (u8)0xFE))||((locData & (u8)0xFE) == (SC_ADPU->Header.INS & (u8)0xFE)))
    {
      SC_ResponceStatus->Data[0] = locData;/* ACK received */
    }
  }

  /* If no status bytes received ---------------------------------------------*/
  if(SC_ResponceStatus->SW1 == 0x00)
  {
    /* Send body data to SC--------------------------------------------------*/
    if (SC_ADPU->Body.LC)
    {
      for(i = 0; i < SC_ADPU->Body.LC; i++)
      {
        SCData = SC_ADPU->Body.Data[i];        
        USART_SendData(USARTn, SCData);
        while(USART_GetFlagStatus(USARTn, USART_FLAG_TC) == RESET)
        {
        } 
      }
      /* Flush the USARTn DR */
      (void)USART_ReceiveData(USARTn);
      /* Disable the DMA Receive (Reset DMAR bit only) */  
      USART_DMACmd(USARTn, USART_DMAReq_Rx, DISABLE);
    }
    /* Or receive body data from SC ------------------------------------------*/
    else if (SC_ADPU->Body.LE)
    {
      //WTX 0x60
      if((USART_ByteReceive(USARTn,&locData,SC_Receive_Timeout)) == SUCCESS)
      {
        while(locData==0x60) //NULL 0x60
        {
          locData=0;
          USART_ByteReceive(USARTn,&locData,SC_Receive_Timeout);
        }

        SC_ResponceStatus->Data[0]=locData;

        for(i = 1; i < SC_ADPU->Body.LE; i++)
        {
          if(USART_ByteReceive(USARTn,&locData, SC_Receive_Timeout) == SUCCESS)
          {
            SC_ResponceStatus->Data[i] = locData;
          }
        } 
      }
    }

    /* Wait SW1 --------------------------------------------------------------*/
      if(USART_ByteReceive(USARTn,&locData, SC_Receive_Timeout) == SUCCESS)
      {
        while(locData==0x60) //NULL 0x60
        {
          locData=0;
          USART_ByteReceive(USARTn,&locData,SC_Receive_Timeout);
        }

        SC_ResponceStatus->SW1 = locData;
      }

    /* Wait SW2 ------------------------------------------------------------*/   
    i = 0;
    while(i < 10)
    {
      if(USART_ByteReceive(USARTn,&locData, SC_Receive_Timeout) == SUCCESS)
      {
        SC_ResponceStatus->SW2 = locData;
        i = 11;
      }
      else
      {
        i++;
      }
    }
  }
}

/*******************************************************************************
* Function Name  : SC_AnswerReq
* Description    : Requests the reset answer from card.
* Input          : - SCState: pointer to an SC_State enumeration that will contain
*                    the Smartcard state.
*                  - card: pointer to a buffer which will contain the card ATR.
*                  - length: maximum ATR length
* Output         : None
* Return         : None
*******************************************************************************/
void SC_AnswerReq(USART_TypeDef* USARTn,u8 *card, u8 length)
{
  u8 Data = 0;
  u32 i = 0;
	u8 volatile *initvalue=card;

	SC_Reset(USARTn,Bit_SET); 
	(void)USART_ReceiveData(USARTn);//Clear Usart DR

	if((USART_ByteReceive(USARTn,&Data,SC_Reset_Timeout)) == SUCCESS)
  {
		card[0] = Data; /* Receive data for timeout = SC_Receive_Timeout */		
  }
	card=card+1;
	while(length--)
	{
		if((USART_ByteReceive(USARTn,&Data,SC_Receive_Timeout)) == SUCCESS)
		{
			*card++ = Data; /* Receive data for timeout = SC_Receive_Timeout */
		}       
	}	
}

/*******************************************************************************
* Function Name  : SC_decode_Answer2reset
* Description    : Decodes the Answer to reset received from card.
* Input          : - Card: pointer to the buffer containing the card ATR.
* Output         : None
* Return         : None
*******************************************************************************/
u8 SC_decode_Answer2reset(u8 *card)
{
  u32 i = 0, flag = 0, buf = 0, protocol = 0;

  SC_A2R.TS = card[0];  /* Initial character */
  SC_A2R.T0 = card[1];  /* Format character */

  SC_A2R.Hlength = SC_A2R.T0 & (u8)0x0F;

  if ((SC_A2R.T0 & (u8)0x80) == 0x80)
  {
    flag = 1;
  }

  for (i = 0; i < 4; i++)
  {
    SC_A2R.Tlength = SC_A2R.Tlength + (((SC_A2R.T0 & (u8)0xF0) >> (4 + i)) & (u8)0x1);
  }

  for (i = 0; i < SC_A2R.Tlength; i++)
  {
    SC_A2R.T[i] = card[i + 2];
  }

  protocol = SC_A2R.T[SC_A2R.Tlength - 1] & (u8)0x0F;

  while (flag)
  {
    if ((SC_A2R.T[SC_A2R.Tlength - 1] & (u8)0x80) == 0x80)
    {
      flag = 1;
    }
    else
    {
      flag = 0;
    }

    buf = SC_A2R.Tlength;
    SC_A2R.Tlength = 0;

    for (i = 0; i < 4; i++)
    {
      SC_A2R.Tlength = SC_A2R.Tlength + (((SC_A2R.T[buf - 1] & (u8)0xF0) >> (4 + i)) & (u8)0x1);
    }
	
    for (i = 0;i < SC_A2R.Tlength; i++)
    {
      SC_A2R.T[buf + i] = card[i + 2 + buf];
    }
    SC_A2R.Tlength += (u8)buf;
  }

  for (i = 0; i < SC_A2R.Hlength; i++)
  {
    SC_A2R.H[i] = card[i + 2 + SC_A2R.Tlength];
  }

  return (u8)protocol;
}

/*******************************************************************************
* Function Name  : SC_Init
* Description    : Initializes all peripheral used for Smartcard interface.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SC_Init(USART_TypeDef* USARTn)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  
   if(USARTn==USART1)
   {
    /* Enable GPIOB, GPIOD and GPIOE clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
                         
    /* Enable USARTx clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3, ENABLE);
    
    /* Configure USART1 CK as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = SC_CLK1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SC_CLK1_PORT, &GPIO_InitStructure);
  
    /* Configure USART1 Tx  as alternate function open-drain */
    GPIO_InitStructure.GPIO_Pin = SC_IO1;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SC_IO1_PORT, &GPIO_InitStructure);

    /* Configuure smartcard Dir*/ 
    GPIO_InitStructure.GPIO_Pin = SC_DIR1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SC_DIR1_PORT, &GPIO_InitStructure);

    /* Configure Smartcard Reset  */
    GPIO_InitStructure.GPIO_Pin = SC_RST1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SC_RST1_PORT, &GPIO_InitStructure);

    /* Configure Smartcard VCC  */
    GPIO_InitStructure.GPIO_Pin = SC_VCC1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SC_VCC1_PORT, &GPIO_InitStructure);

    SC_PowerCmd(USART1,DISABLE);//VCC=0

    /* USART Clock set to 3.6 MHz (PCLK1 (36 MHZ) / 10) */
    USART_SetPrescaler(USART1, 0x05);

    /* USART Guard Time set to 16 Bit */
    USART_SetGuardTime(USART1, 0);

    USART_StructInit(&USART_InitStructure);
		//USART_InitStructure.USART_Mode=USART_Mode_Tx;
		USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
    USART_InitStructure.USART_BaudRate = 9677;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_Clock = USART_Clock_Enable;  
  
		USART_Init(USART1, &USART_InitStructure);

    /* Enable the USARTn Parity Error Interrupt */
    USART_ITConfig(USART1, USART_IT_PE, ENABLE);
    /* Enable the USARTn Framing Error Interrupt */
    USART_ITConfig(USART1, USART_IT_ERR, ENABLE);  
    /* Enable USARTn */
    USART_Cmd(USART1, ENABLE);
    /* Enable the NACK Transmission */
    USART_SmartCardNACKCmd(USART1, ENABLE);
    /* Enable the Smartcard Interface */
    USART_SmartCardCmd(USART1, ENABLE);
    /* Set RSTIN LOW */  
    SC_Reset(USART1,Bit_RESET);
    SC_PowerCmd(USART1,ENABLE);//VCC=1
		SC_DirCmd(USART1,DIR_RECEIVE);
		//SC_DirCmd(USART1,DIR_SEND);
		
   }
   else if(USARTn==USART2)
   {
    /* Enable GPIOB, GPIOD and GPIOE clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
                         
    /* Enable USARTx clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* Configure USART1 CK as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = SC_CLK2;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SC_CLK2_PORT, &GPIO_InitStructure);
  
    /* Configure USART1 Tx  as alternate function open-drain */
    GPIO_InitStructure.GPIO_Pin = SC_IO2;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SC_IO2_PORT, &GPIO_InitStructure);

    /* Configuure smartcard Dir*/ 
    GPIO_InitStructure.GPIO_Pin = SC_DIR2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SC_DIR2_PORT, &GPIO_InitStructure);

    /* Configure Smartcard Reset  */
    GPIO_InitStructure.GPIO_Pin = SC_RST2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SC_RST2_PORT, &GPIO_InitStructure);

    /* Configure Smartcard VCC  */
    GPIO_InitStructure.GPIO_Pin = SC_VCC2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SC_VCC2_PORT, &GPIO_InitStructure);

    SC_PowerCmd(USART2,DISABLE);//VCC=0

    /* USART Clock set to 3.6 MHz (PCLK1 (36 MHZ) / 10) */
    USART_SetPrescaler(USART2, 0x05);

    /* USART Guard Time set to 16 Bit */
    USART_SetGuardTime(USART2, 0);

    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = 9677;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_Clock = USART_Clock_Enable;  
  
    USART_Init(USART2, &USART_InitStructure);

    /* Enable the USARTn Parity Error Interrupt */
    USART_ITConfig(USART2, USART_IT_PE, ENABLE);
    /* Enable the USARTn Framing Error Interrupt */
    USART_ITConfig(USART2, USART_IT_ERR, ENABLE);  
    /* Enable USARTn */
    USART_Cmd(USART2, ENABLE);
    /* Enable the NACK Transmission */
    USART_SmartCardNACKCmd(USART2, ENABLE);
    /* Enable the Smartcard Interface */
    USART_SmartCardCmd(USART2, ENABLE);
    /* Set RSTIN LOW */  
    SC_Reset(USART2,Bit_RESET);
    SC_PowerCmd(USART2,ENABLE);//VCC=1
		SC_DirCmd(USART2,DIR_RECEIVE);
   }
   else
   {
    /* Enable GPIOB, GPIOD and GPIOE clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
                         
    /* Enable USARTx clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    
    /* Configure USART1 CK as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = SC_CLK3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SC_CLK3_PORT, &GPIO_InitStructure);
  
    /* Configure USART1 Tx  as alternate function open-drain */
    GPIO_InitStructure.GPIO_Pin = SC_IO3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(SC_IO3_PORT, &GPIO_InitStructure);

    /* Configuure smartcard Dir*/ 
    GPIO_InitStructure.GPIO_Pin = SC_DIR3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SC_DIR3_PORT, &GPIO_InitStructure);

    /* Configure Smartcard Reset  */
    GPIO_InitStructure.GPIO_Pin = SC_RST3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SC_RST3_PORT, &GPIO_InitStructure);

    /* Configure Smartcard VCC  */
    GPIO_InitStructure.GPIO_Pin = SC_VCC3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SC_VCC3_PORT, &GPIO_InitStructure);

    SC_PowerCmd(USART3,DISABLE);//VCC=0

    /* USART Clock set to 3.6 MHz (PCLK1 (36 MHZ) / 10) */
    USART_SetPrescaler(USART3, 0x05);

    /* USART Guard Time set to 16 Bit */
    USART_SetGuardTime(USART3, 0);

    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = 9677;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_Clock = USART_Clock_Enable;  
  
    USART_Init(USART3, &USART_InitStructure);

    /* Enable the USARTn Parity Error Interrupt */
    USART_ITConfig(USART3, USART_IT_PE, ENABLE);
    /* Enable the USARTn Framing Error Interrupt */
    USART_ITConfig(USART3, USART_IT_ERR, ENABLE);  
    /* Enable USARTn */
    USART_Cmd(USART3, ENABLE);
    /* Enable the NACK Transmission */
    USART_SmartCardNACKCmd(USART3, ENABLE);
    /* Enable the Smartcard Interface */
    USART_SmartCardCmd(USART3, ENABLE);
    /* Set RSTIN LOW */  
    SC_Reset(USART3,Bit_RESET);
    SC_PowerCmd(USART3,ENABLE);//VCC=1
		SC_DirCmd(USART3,DIR_RECEIVE);
   } 
}


/*******************************************************************************
* Function Name  : USART_ByteReceive
* Description    : Receives a new data while the time out not elapsed.
* Input          : None
* Output         : None
* Return         : An ErrorStatus enumuration value:
*                         - SUCCESS: New data has been received
*                         - ERROR: time out was elapsed and no further data is 
*                                  received
*******************************************************************************/
ErrorStatus USART_ByteReceive(USART_TypeDef* USARTn,u8 *Data, u32 TimeOut)
{
  u32 Counter = 0;

  //set IO transfer direction

  SC_DirCmd(USARTn,DIR_RECEIVE);
  
  while((USART_GetFlagStatus(USARTn, USART_FLAG_RXNE) == RESET) && (Counter != TimeOut))
  {
    Counter++;
  }

  if(Counter != TimeOut)
  {
    *Data = (u8)USART_ReceiveData(USARTn);
    return SUCCESS;    
  }
  else 
  {
    return ERROR;
  }

//  SC_DirCmd(USARTn,DIR_SEND);
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
