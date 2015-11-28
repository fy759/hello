/****************************************Copyright (c)****************************************************
**                                      中帝威科技
**                                     
**                                 http://www.DeviceWell.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               LM3S_Spi.c
** Last modified Date:      2010-12-01
** Last Version:            1.0
** Description:             SPI 接口部分的函数.外部flash 使用的是SST的 16MBit SPI flash.
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              Edison
** Created date:            2010-12-01
** Version:                 1.0
** Descriptions:            The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:				Edison              
** Modified date:           2010-12-24
** Version:                 
** Description:             flash 由SST的sst25vf106  换成了华邦的 w25q16.这2颗flash的操作指令都是相同的,
**							只是华邦的flash在写数据时,仅支持页操作.而SST的flash是可以单字节写的.因此增加了
**                          写flash数据部分的2个函数.其他没有修改.
*********************************************************************************************************/
#include "Include.h"



/*  sst25vf106-16Mbit   flash  的操作命令*/
#define           SST25VF_WREN			    0x06 	//spi write enable
#define           SST25VF_WRDI			    0x04	//spi wtire disable
#define           SST25VF_RD			    0x03	//spi read
#define           SST25VF_HSPEED_RD			0x0B	//spi high speed read
#define           SST25VF_04K_ERASE			0x20	//spi 4kbyte block erase
#define           SST25VF_32K_ERASE			0x52	//spi 32kbyte block erase
#define           SST25VF_64K_ERASE			0xD8	//spi 64kbyte block erase
#define           SST25VF_CHIP_ERASE		0x60	//spi erase full memory array
#define           SST25VF_BYTE_PROG			0x02	//spi program one data byte
#define           SST25VF_RDSR   			0x05	//spi read status register
#define           SST25VF_EWSR  			0x50	//spi enable write status register
#define           SST25VF_WRSR  			0x01	//spi write status register
#define           SST25VF_RDID  			0x90	//spi read id 
#define           SST25VF_EBSY  			0x70	//spi  
#define           SST25VF_DBSY  			0x80	//spi 
#define           SST25VF_JEDEC_ID  		0x9F	//spi JEDEC ID read


#define       CE_HIGH          GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET)		   //flash 选通信号
#define       CE_LOW           GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET)							
			   
GPIO_InitTypeDef GPIO_InitStructure;//声明GPIO_InitStructure

void SPIInit(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	SPI_I2S_DeInit(SPI1); 

	/*********************端口A配置(spi)********************/
	/*  pin5 为SPI1_SCK输入信号, pin6 为SPI1_MISO输入信号  */
	/*  pin7 为SPI1_MOSI输入信号,pin4 为SPI1_NSS输入信号   */
	/*******************************************************/ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | 
								GPIO_Pin_5 | 
								GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);	//将所以管脚先拉高, edison 12.24 . 
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);

	/* SPI1 configuration */ 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(SPI1, &SPI_InitStructure); 

	SPI_Cmd(SPI1, ENABLE);
}
/*******************************************************************************************************************
** 功能描述: 通过SPI接口发送一个字节			Function: send a byte by SPI interface
** 输　  入: INT8U byte: 发送的字节				Input:	  INT8U byte: the byte that will be send
** 输 　 出: 无									Output:	  NULL
********************************************************************************************************************/
void SPI_SendByte(unsigned char byte)
{
	unsigned char DummyByte=0;

	while(!(SPI1->SR & 0x02));	//wait for Tx buffer empty

	SPI1->DR = byte;

	while(!(SPI1->SR & 0x01));
	
	DummyByte = SPI1->DR;
	DummyByte =	DummyByte;
}


/*******************************************************************************************************************
** 功能描述: 从SPI接口接收一个字节				Function: receive a byte from SPI interface
** 输　  入: 无									Input:	  NULL
** 输 　 出: 收到的字节							Output:	  the byte that be received
********************************************************************************************************************/
unsigned char SPI_RecByte(void)
{
	unsigned char RecData=0;
	unsigned char DummyByte=0;

	while(!(SPI1->SR & 0x02));	//wait for Tx buffer empty	

	SPI1->DR = DummyByte;

	while(!(SPI1->SR & 0x01));	//timeout is need.

	RecData = SPI1->DR;

	return RecData;
}
/****************************************************************************************************
** 功能描述: SPI写使能          				Function: 
** 输　  入: 无									Input:	  NULL
** 输 　 出: 无     							Output:	  NULL
*****************************************************************************************************/
void SPI_WriteEn(void)
{
	CE_LOW;

	SPI_SendByte(SST25VF_WREN);

	CE_HIGH;	
}
/****************************************************************************************************
** 功能描述: SPI写禁止          				Function:
** 输　  入: 无									Input:	  NULL
** 输 　 出: 无     							Output:	  NULL
*****************************************************************************************************/
void SPI_WriteDis(void)
{
	CE_LOW;

	SPI_SendByte(SST25VF_WRDI);

	CE_HIGH;	
}
/****************************************************************************************************
** 功能描述: SPI读取状态寄存器          		Function: 
** 输　  入: 无									Input:	  NULL
** 输 　 出: 当前状态寄存器的值     			Output:	  NULL
*****************************************************************************************************/
unsigned char SST25VF_ReadStatusReg(void)
{
	unsigned char ucStatus=0;

	CE_LOW;
	
	SPI_SendByte(SST25VF_RDSR);

	ucStatus = SPI_RecByte();
	
	CE_HIGH;

	return ucStatus; 	
}
/****************************************************************************************************
** 功能描述:                        			Function: 
** 输　  入: 无									Input:	  NULL
** 输 　 出: 无     							Output:	  NULL
*****************************************************************************************************/
#if(0)
void SST25VF_EnableWriteStatusReg(void)
{
	SPI_CS_CLR();

	SPI_SendByte(SST25VF_EWSR);

	SPI_CS_SET();
		
}
#endif
/****************************************************************************************************
** 功能描述: SPI写状态寄存器          			Function: 
** 输　  入: 需写入的状态值						Input:	  NULL
** 输 　 出: 无     							Output:	  NULL
*****************************************************************************************************/
void SST25VF_WriteStatusReg(unsigned char ucStatusValue)
{
	CE_LOW;

	SPI_SendByte(SST25VF_EWSR);

	CE_HIGH;

	CE_LOW;
	
	SPI_SendByte(SST25VF_WRSR);

	SPI_SendByte(ucStatusValue);

	CE_HIGH;	
}
/****************************************************************************************************
** 功能描述: 以字节的方式往flash中写入数据  
** 输　  入: unsigned long ulAddr 源地址
**           unsigned char *pucSendBuf 数据buf的源地址
    	     unsigned long ulLen 写入字节的个数
** 输 　 出: 无     
*****************************************************************************************************/
#define              MAX_ADDR               0x400000
#define              EXFLASH_IDLE           0x03

/****************************************************************************************************
** 功能描述: 从外部flash中指令地址处读取ulLen个数据到指令buf中.    
** 输　  入: 			unsigned long ulAddr 源地址
**                      unsigned char *pucDest  目的缓存
**						unsigned long ulLen 读取数据的个数
**
** 输 　 出: 无     
*****************************************************************************************************/
void SST25VF_Read(unsigned long ulAddr, unsigned char *pucDest, unsigned short usLen)
{
	unsigned short i;

	if(((ulAddr+usLen) >= MAX_ADDR) || (usLen == 0))
	{
		return;
	}

	CE_LOW;

	SPI_SendByte(SST25VF_RD);
	SPI_SendByte((ulAddr & 0xFFFFFF) >> 16);
	SPI_SendByte((ulAddr & 0xFFFF) >> 8);
	SPI_SendByte(ulAddr & 0xFF);

	for(i = 0; i < usLen; i++)
	{
		*pucDest++ = SPI_RecByte();	
	}

	CE_HIGH;	
}
/****************************************************************************************************
** 功能描述:    
** 输　  入: 			unsigned long ulAddr 需擦除块的起始地址
** 输 　 出: 无     
*****************************************************************************************************/
void SST25VF_4KErase(unsigned long ulAddr)
{
	unsigned char ucStatusRegVal=0;

	ulAddr = ulAddr&0xFFF000;

	if((ulAddr + 4096) > MAX_ADDR)
	{
		return;
	}
		
	SPI_WriteEn();
	
	CE_LOW;
	
	SPI_SendByte(SST25VF_04K_ERASE);
	SPI_SendByte((ulAddr & 0xFFFFFF) >> 16);
	SPI_SendByte((ulAddr & 0xFFFF) >> 8);
	SPI_SendByte(ulAddr & 0xFF);

	CE_HIGH;

	do
	{
		ucStatusRegVal = SST25VF_ReadStatusReg();	
	}while(ucStatusRegVal == EXFLASH_IDLE);
}
/****************************************************************************************************
** 功能描述:    
** 输　  入: 			unsigned long ulAddr 需擦除块的起始地址
** 输 　 出: 无     
*****************************************************************************************************/
void SST25VF_64KErase(unsigned long ulAddr)
{
	unsigned char ucStatusRegVal=0;

	ulAddr = ulAddr&0xFF0000;

	if((ulAddr + 65536) > MAX_ADDR)
	{
		return;
	}
	
	SPI_WriteEn();
	
	CE_LOW;
	
	SPI_SendByte(SST25VF_64K_ERASE);
	SPI_SendByte((ulAddr & 0xFFFFFF) >> 16);
	SPI_SendByte((ulAddr & 0xFFFF) >> 8);
	SPI_SendByte(ulAddr & 0xFF);

	CE_HIGH;

	do
	{
		ucStatusRegVal = SST25VF_ReadStatusReg();	
	}while(ucStatusRegVal == EXFLASH_IDLE);
}
/****************************************************************************************************
** 功能描述: WINBOND flash的 写数据函数,WINBOND的flash在写数据时,仅支持页操作的. 
** 输　  入: 无
** 输 　 出: 无     
*****************************************************************************************************/
void WinBond_PageWrite(unsigned char *pucBuf,unsigned long ulAddr,unsigned short usNumByteToWrite)
{
	unsigned char ucStatusRegVal=0;

	if(((ulAddr + usNumByteToWrite) >= MAX_ADDR) || (usNumByteToWrite == 0))
	{	//out of range.
		return;
	}

	SPI_WriteEn();
	
	CE_LOW;

	SPI_SendByte(SST25VF_BYTE_PROG);

	SPI_SendByte((ulAddr & 0xFFFFFF) >> 16);
	SPI_SendByte((ulAddr& 0xFFFF) >> 8);
	SPI_SendByte(ulAddr & 0xFF);
	
	while(usNumByteToWrite--)
	{
		SPI_SendByte(*pucBuf++);
	}
	
	CE_HIGH;

	do
	{
		ucStatusRegVal = SST25VF_ReadStatusReg();	
	}while(ucStatusRegVal == EXFLASH_IDLE);
}
//--------------------------------------------------------------------
#define             WINBOND_W25Q16_PAGESIZE                256

void WinBond_BufferWrite(unsigned char *pBuffer, unsigned long WriteAddr, unsigned short NumByteToWrite)
{
  unsigned char NumOfPage = 0;
  unsigned char NumOfSingle = 0;
  unsigned char Addr = 0;
  unsigned char count = 0;
  unsigned char temp = 0;

  Addr = WriteAddr % WINBOND_W25Q16_PAGESIZE;
  count = WINBOND_W25Q16_PAGESIZE - Addr;

  NumOfPage =  NumByteToWrite / WINBOND_W25Q16_PAGESIZE;
  NumOfSingle = NumByteToWrite % WINBOND_W25Q16_PAGESIZE;
  
  if(Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
  {
    if(NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      WinBond_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */ 
    {
      while(NumOfPage--)
      {
        WinBond_PageWrite(pBuffer, WriteAddr, WINBOND_W25Q16_PAGESIZE);
        WriteAddr +=  WINBOND_W25Q16_PAGESIZE;
        pBuffer += WINBOND_W25Q16_PAGESIZE;  
      }    
     
      WinBond_PageWrite(pBuffer, WriteAddr, NumOfSingle);
   }
  }
  else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
  {
    if(NumOfPage== 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if(NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;
      
        WinBond_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count; 
        
        WinBond_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        WinBond_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / WINBOND_W25Q16_PAGESIZE;
      NumOfSingle = NumByteToWrite % WINBOND_W25Q16_PAGESIZE;
      
      WinBond_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;  
     
      while(NumOfPage--)
      {
        WinBond_PageWrite(pBuffer, WriteAddr, WINBOND_W25Q16_PAGESIZE);
        WriteAddr +=  WINBOND_W25Q16_PAGESIZE;
        pBuffer += WINBOND_W25Q16_PAGESIZE;
      }
      
      if(NumOfSingle != 0)
      {
        WinBond_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}
