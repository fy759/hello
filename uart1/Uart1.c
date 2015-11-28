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
	USART_Cmd(USART1, ENABLE);								//使能串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//TXD引脚设置（连接在PA9上）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //开GPIOA时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 				//configure Pin9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  		// 最高输出速率50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);             		//初始化GPIOA
	//RXD引脚设置（连接在PA10上）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //开GPIOA时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 				//configure Pin10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  	
    GPIO_Init(GPIOA, &GPIO_InitStructure);            		 //初始化GPIOA

//	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);		//USART1复用功能映射
		
}


void USART1_IRQHandler(void)
{
	    /***********串口接收中断************/
//	if(USART_GetITStatus(USART1, USART_IT_RXNE)==1)
//	{	
//		USART1_Rx_Function();	
//	}
//		/***********串口发送中断************/
//	if(USART_GetITStatus(USART1, USART_IT_TXE)==1)
//	{	
//		USART1_Tx_Function();
//	}
}




/****************************************************************
* 名    称:USART1_Rx_Function
* 功能描述:USART1接收数据，这个是中断回调函数
* 输入参量:
* 输出参量:无
* 返    回:无
* 调用子程:
* 使用方法:
----------------------------------------------------------------*/
//void USART1_Rx_Function(void)
//{
//	unsigned char ucByte;
//	
//	ucByte = USART_ReceiveData(USART1);
//	ucByte = ucByte;
//}





//--------------------发送数据-----------------------
/****************************************************************
* 名    称:USART1_Tx_Function
* 功能描述:USART1发送数据帧，这个是中断回调函数
* 输入参量:
* 输出参量:无
* 返    回:无
* 调用子程:
* 使用方法:
----------------------------------------------------------------*/
//void USART1_Tx_Function( void )
//{
//
//}
//


/*******************************************************************************
  * @函数名称	Serial_PutString
  * @函数说明   串口发送字符串
  * @输入参数   *s:需发送的字符串
  * @输出参数   无
  * @返回参数   无
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
  * @函数名称	SerialPutChar
  * @函数说明   串口发送一个字符
  * @输入参数   C:需发送的字符
  * @输出参数   i无
  * @返回参数   无
*******************************************************************************/
void SerialPutChar(unsigned char c)
{
    while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
    
    USART_SendData(USART1, c);
   // while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
   // {
   // }
}



