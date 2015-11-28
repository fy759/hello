/*****************************************************************************
Copyright: 
File name: Usart1.c
Description: 
Author: 
Version: 
Date: 
History: 
*****************************************************************************/
#include "Include.h"


USART_InitTypeDef USART_InitStructure;
USART_ClockInitTypeDef USART_ClockInitStructure;

//void USART1_Rx_Function(void);
//void USART1_Tx_Function(void);

void SerialPutChar(unsigned char c);


/*************************************************
Function: Usart1_Init
Description: 
Input: 
Output: 
Return:
*************************************************/
void Uart1Init(void)
{
	USART_DeInit(USART1);

	USART_InitStructure.USART_BaudRate = 115200;
//	USART_InitStructure.USART_BaudRate = ucBaudRate; 
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
    USART_InitStructure.USART_StopBits = USART_StopBits_1; 
    USART_InitStructure.USART_Parity = USART_Parity_No; 
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
 	//USART_InitStructure.USART_Clock = USART_Clock_Disable;
  	USART_ClockInitStructure.USART_Clock=USART_Clock_Disable;
  	//USART_InitStructure.USART_CPOL = USART_CPOL_Low;
  	USART_ClockInitStructure.USART_CPOL=USART_CPOL_Low;                
  	//USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
  	USART_ClockInitStructure.USART_CPHA=USART_CPHA_2Edge;
  	//USART_InitStructure.USART_LastBit = USART_LastBit_Disable;
  	USART_ClockInitStructure.USART_LastBit=USART_LastBit_Disable;
 
    USART_Init(USART1, &USART_InitStructure);
	USART_ClockInit(USART1, &USART_ClockInitStructure);     		 
	/////////////////////////////////////
//	USART_StructInit(&USART_InitStructure);	//initialize a USART_InitTypeDef structure
	USART_Cmd(USART1, ENABLE);								//ʹ�ܴ���1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//TXD�������ã�������PA9�ϣ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //��GPIOAʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 				//configure Pin9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		//�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  		// ����������50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);             		//��ʼ��GPIOA
	//RXD�������ã�������PA10�ϣ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //��GPIOAʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 				//configure Pin10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
    GPIO_Init(GPIOA, &GPIO_InitStructure);            		 //��ʼ��GPIOA

//	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);		//USART1���ù���ӳ��
		
}


void USART1_IRQHandler(void)
{
	    /***********���ڽ����ж�************/
//	if(USART_GetITStatus(USART1, USART_IT_RXNE)==1)
//	{	
//		USART1_Rx_Function();	
//	}
//		/***********���ڷ����ж�************/
//	if(USART_GetITStatus(USART1, USART_IT_TXE)==1)
//	{	
//		USART1_Tx_Function();
//	}
}




/****************************************************************
* ��    ��:USART1_Rx_Function
* ��������:USART1�������ݣ�������жϻص�����
* �������:
* �������:��
* ��    ��:��
* �����ӳ�:
* ʹ�÷���:
----------------------------------------------------------------*/
//void USART1_Rx_Function(void)
//{
//	unsigned char ucByte;
//	
//	ucByte = USART_ReceiveData(USART1);
//	ucByte = ucByte;
//}





//--------------------��������-----------------------
/****************************************************************
* ��    ��:USART1_Tx_Function
* ��������:USART1��������֡��������жϻص�����
* �������:
* �������:��
* ��    ��:��
* �����ӳ�:
* ʹ�÷���:
----------------------------------------------------------------*/
//void USART1_Tx_Function( void )
//{
//
//}
//


/*******************************************************************************
  * @��������	Serial_PutString
  * @����˵��   ���ڷ����ַ���
  * @�������   *s:�跢�͵��ַ���
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void Serial_PutString(unsigned char *s)
{
    while (*s != '\0')
    {
        SerialPutChar(*s);
        s++;
    }
}


/*******************************************************************************
  * @��������	SerialPutChar
  * @����˵��   ���ڷ���һ���ַ�
  * @�������   C:�跢�͵��ַ�
  * @�������   i��
  * @���ز���   ��
*******************************************************************************/
void SerialPutChar(unsigned char c)
{
    while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
    
    USART_SendData(USART1, c);
   // while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
   // {
   // }
}



