#include "Include.h"



typedef  void (*pFunction)(void);



pFunction Jump_To_Application;
unsigned long  JumpAddress;
unsigned char ucUpFlag;

__asm void __set_MSP(unsigned long mainStackPointer)
{
  msr msp, r0
  bx lr
}





void Delay(unsigned long ulCount)
{
	while(ulCount--);
}


/*************************************************************************
** ��������: ϵͳʱ�ӳ�ʼ��.
** �䡡  ��: ��		
** �� �� ��: ��							
**************************************************************************/
void SysClkInit(void)
{
	RCC_DeInit(); 

	RCC_HSEConfig(RCC_HSE_ON);	//Enable the HSE

	while(!RCC_WaitForHSEStartUp());	//�ȴ��ⲿ����ʱ���ȶ�

	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	//Set PLL clock output to 72MHz using HSE (8MHz) as entry clock
	RCC_HCLKConfig(RCC_SYSCLK_Div1);	//Configure HCLK such as HCLK = SYSCLK
    RCC_PCLK1Config(RCC_HCLK_Div4);	    //PCLK1 = HCLK/4 
	RCC_PCLK2Config(RCC_HCLK_Div4);	    //PCLK2 = HCLK/4
	FLASH_SetLatency(FLASH_Latency_2);   //FLASH_Latency��������FLASH�洢����ʱʱ��������
	RCC_PLLCmd(ENABLE);	   //Enable the PLL 

	while(!RCC_GetFlagStatus(RCC_FLAG_PLLRDY));	//�ȴ�PLL�ȶ�

	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	  //Select the PLL as system clock source

	while(RCC_GetSYSCLKSource() != 0x08);
	 
	//*************define for usart1************ //
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|
							RCC_APB2Periph_AFIO|
							RCC_APB2Periph_SPI1|
							RCC_APB2Periph_TIM1,
							ENABLE);		//Enable USART1 clocks

	/*************TIM clock enable**************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|
							RCC_APB1Periph_TIM3|
							RCC_APB1Periph_TIM4|
							RCC_APB1Periph_USART2,
							ENABLE);			
}




/*************************************************************************
** ��������: i/o �˿ڳ�ʼ��.
** �䡡  ��: ��		
** �� �� ��: ��							
**************************************************************************/ 
void GPIOInit(void)
{
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1 
								|GPIO_Pin_8
								|GPIO_Pin_11 
								|GPIO_Pin_12; //configure Pin0,1,8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ����������50Hz
	GPIO_Init(GPIOA, &GPIO_InitStructure);             //��ʼ��GPIOA
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //��GPIOBʱ��
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|
	                            GPIO_Pin_1|
								GPIO_Pin_2|
	                            GPIO_Pin_6|
								GPIO_Pin_7|	   
	                            GPIO_Pin_8|
								GPIO_Pin_9|
								GPIO_Pin_10|
								GPIO_Pin_11|
								GPIO_Pin_12;
								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ����������50Hz
	GPIO_Init(GPIOB, &GPIO_InitStructure);             //��ʼ��GPIOB

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //��GPIOBʱ��
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|
	                            GPIO_Pin_1;								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ����������50Hz
	GPIO_Init(GPIOB, &GPIO_InitStructure);             //��ʼ��GPIOB
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   //��GPIOCʱ��
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4 |
								GPIO_Pin_5 |
								GPIO_Pin_6 |
								GPIO_Pin_7 |
								GPIO_Pin_8 | 
								GPIO_Pin_9 |
								GPIO_Pin_10 |
								GPIO_Pin_11 |
								GPIO_Pin_12; //configure Pinx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ����������50Hz
	GPIO_Init(GPIOC, &GPIO_InitStructure);             //��ʼ��GPIOC
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);   //��GPIODʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;   							
								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ����������50Hz
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
}





/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{		
	Delay(DELAY_100MS);
	SysClkInit();
	GPIOInit();	
	SPIInit();
	Uart1Init();		
	/* Flash unlock */
	FLASH_Unlock();
	
	SerialPutString("\r\nBootload...");
	Delay(DELAY_100MS);	


//	ucUpFlag = UPDATE_FLAG_NO;				 //TEST
//	EXFLASH_WriteCmd(SST25V_UPDATE_FLAG_ADDR, &ucUpFlag, 1);   		 //TEST
	EXFLASH_ReadCmdData(SST25V_UPDATE_FLAG_ADDR, &ucUpFlag, 1);
	
	if(ucUpFlag == UPDATE_FLAG_YES)		//��Ҫ����
	{		
		SerialPutString("\r\nErase internal flash ...");
		FlashEraseProgramArea();		//����Ӧ�ó��������
		SerialPutString("\r\nProgram internal flash ...");
		FlashProgram();					//д�ڲ�Flash
		Delay(DELAY_1MS);
		UpdateTimesCount();
	}
	#if (DEBUG)
	else
	{
		SerialPutString("\r\nSet flag.");
		Delay(DELAY_100US);
		ucUpFlag = UPDATE_FLAG_YES;	
		EXFLASH_WriteCmd(SST25V_UPDATE_FLAG_ADDR, &ucUpFlag, 1);
		Delay(DELAY_100US);	
	}
	#endif
	
	if (((*(unsigned long *)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
	{
		SerialPutString("\r\nRun application program ...");
		/* Test if user code is programmed starting from address "ApplicationAddress" */
		/* Jump to user application */
		JumpAddress = *(unsigned long*) (ApplicationAddress + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(unsigned long*) ApplicationAddress);
		Jump_To_Application();
	}	
	else
	{
		if (((*(unsigned long *)BootAddress) & 0x2FFE0000 ) == 0x20000000)
		{
			SerialPutString("\r\nBootload program running ...");
			/* Test if user code is programmed starting from address "ApplicationAddress" */
			/* Jump to user application */
			JumpAddress = *(unsigned long*) (BootAddress + 4);
			Jump_To_Application = (pFunction) JumpAddress;
			/* Initialize user application's Stack Pointer */
			__set_MSP(*(unsigned long*) BootAddress);
			Jump_To_Application();
		}
	} 

	while (1)
	{}
}
