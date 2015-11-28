/****************************************Copyright (c)****************************************************
**                                      �е����Ƽ�
**                                     
**                                 http://www.DeviceWell.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               LM3S_Spi.c
** Last modified Date:      2010-12-01
** Last Version:            1.0
** Description:             SPI �ӿڲ��ֵĺ���.�ⲿflash ʹ�õ���SST�� 16MBit SPI flash.
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              Edison
** Created date:            2010-12-01
** Version:                 1.0
** Descriptions:            The original version ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:				Edison              
** Modified date:           2010-12-24
** Version:                 
** Description:             flash ��SST��sst25vf106  �����˻���� w25q16.��2��flash�Ĳ���ָ�����ͬ��,
**							ֻ�ǻ����flash��д����ʱ,��֧��ҳ����.��SST��flash�ǿ��Ե��ֽ�д��.���������
**                          дflash���ݲ��ֵ�2������.����û���޸�.
*********************************************************************************************************/
#include "Include.h"



/*  sst25vf106-16Mbit   flash  �Ĳ�������*/
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


#define       CE_HIGH          GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET)		   //flash ѡͨ�ź�
#define       CE_LOW           GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET)							
			   
GPIO_InitTypeDef GPIO_InitStructure;//����GPIO_InitStructure

void SPIInit(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	SPI_I2S_DeInit(SPI1); 

	/*********************�˿�A����(spi)********************/
	/*  pin5 ΪSPI1_SCK�����ź�, pin6 ΪSPI1_MISO�����ź�  */
	/*  pin7 ΪSPI1_MOSI�����ź�,pin4 ΪSPI1_NSS�����ź�   */
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

	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);	//�����Թܽ�������, edison 12.24 . 
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
** ��������: ͨ��SPI�ӿڷ���һ���ֽ�			Function: send a byte by SPI interface
** �䡡  ��: INT8U byte: ���͵��ֽ�				Input:	  INT8U byte: the byte that will be send
** �� �� ��: ��									Output:	  NULL
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
** ��������: ��SPI�ӿڽ���һ���ֽ�				Function: receive a byte from SPI interface
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: �յ����ֽ�							Output:	  the byte that be received
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
** ��������: SPIдʹ��          				Function: 
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: ��     							Output:	  NULL
*****************************************************************************************************/
void SPI_WriteEn(void)
{
	CE_LOW;

	SPI_SendByte(SST25VF_WREN);

	CE_HIGH;	
}
/****************************************************************************************************
** ��������: SPIд��ֹ          				Function:
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: ��     							Output:	  NULL
*****************************************************************************************************/
void SPI_WriteDis(void)
{
	CE_LOW;

	SPI_SendByte(SST25VF_WRDI);

	CE_HIGH;	
}
/****************************************************************************************************
** ��������: SPI��ȡ״̬�Ĵ���          		Function: 
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: ��ǰ״̬�Ĵ�����ֵ     			Output:	  NULL
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
** ��������:                        			Function: 
** �䡡  ��: ��									Input:	  NULL
** �� �� ��: ��     							Output:	  NULL
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
** ��������: SPIд״̬�Ĵ���          			Function: 
** �䡡  ��: ��д���״ֵ̬						Input:	  NULL
** �� �� ��: ��     							Output:	  NULL
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
** ��������: ���ֽڵķ�ʽ��flash��д������  
** �䡡  ��: unsigned long ulAddr Դ��ַ
**           unsigned char *pucSendBuf ����buf��Դ��ַ
    	     unsigned long ulLen д���ֽڵĸ���
** �� �� ��: ��     
*****************************************************************************************************/
#define              MAX_ADDR               0x400000
#define              EXFLASH_IDLE           0x03

/****************************************************************************************************
** ��������: ���ⲿflash��ָ���ַ����ȡulLen�����ݵ�ָ��buf��.    
** �䡡  ��: 			unsigned long ulAddr Դ��ַ
**                      unsigned char *pucDest  Ŀ�Ļ���
**						unsigned long ulLen ��ȡ���ݵĸ���
**
** �� �� ��: ��     
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
** ��������:    
** �䡡  ��: 			unsigned long ulAddr ����������ʼ��ַ
** �� �� ��: ��     
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
** ��������:    
** �䡡  ��: 			unsigned long ulAddr ����������ʼ��ַ
** �� �� ��: ��     
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
** ��������: WINBOND flash�� д���ݺ���,WINBOND��flash��д����ʱ,��֧��ҳ������. 
** �䡡  ��: ��
** �� �� ��: ��     
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
