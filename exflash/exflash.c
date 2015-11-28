/****************************************Copyright (c)****************************************************
**                                      �е����Ƽ�
**                                     
**                                 http://www.DeviceWell.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               exflash.c
** Last modified Date:      2011-07-25
** Last Version:            1.0
** Description:             �ⲿflash�Ĳ�������.
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              �»�
** Created date:            2011-07-25
** Version:                 1.0
** Descriptions:            The original version ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Description:             
**
*********************************************************************************************************/
#define                         _EXFLASH_

#include "Include.h"

#define               SECTOR_NUM_PAGE               5
#define               SECTOR_SIZE                   4096

/****************************************************************************
** ��������: д�����ݵ�flash��,��д֮ǰ��Ҫ��ִ�в����Ķ���.
** �䡡  ��:    unsigned long ulDstAddr  д�����ݵ���ʼ��ַ,
				unsigned char *pucByteBuf ��д�������,
				unsigned short usLen д�����ݵĸ���,
** �� �� ��: ��     
*****************************************************************************/
void EXFLASH_WriteCmd(unsigned long ulDstAddr, unsigned char *pucByteBuf, unsigned short usLen)
{
	unsigned char temp=0;

	temp = SST25VF_ReadStatusReg();

	SST25VF_WriteStatusReg(0x00);

	SST25VF_4KErase(ulDstAddr);
//	SST25VF_4KErase(ulDstAddr + SECTOR_SIZE);
//	SST25VF_4KErase(ulDstAddr + SECTOR_SIZE);

	WinBond_BufferWrite(pucByteBuf, ulDstAddr, usLen);

	SST25VF_WriteStatusReg(temp);
}
/****************************************************************************
** ��������: д�����ݵ�flash��.
** �䡡  ��:    unsigned long ulDstAddr  д�����ݵ���ʼ��ַ,
				unsigned char *pucByteBuf ��д�������,
				unsigned short usLen д�����ݵĸ���,
** �� �� ��: ��     
*****************************************************************************/
void EXFLASH_WriteData(unsigned long ulDstAddr, unsigned char *pucByteBuf, unsigned short usLen)
{
	unsigned char temp=0;

	temp = SST25VF_ReadStatusReg();

	SST25VF_WriteStatusReg(0x00);

	WinBond_BufferWrite(pucByteBuf, ulDstAddr, usLen);

	SST25VF_WriteStatusReg(temp);
}
/****************************************************************************
** ��������: ɾ����Ŀ�����������ʾ��Ŀ.
** �䡡  ��: ��   
** �� �� ��: ��     
*****************************************************************************/
void EXFLASH_DelAllPlay(unsigned short usMaxPageNum)
{
	unsigned char temp=0;
	unsigned char i;
	unsigned long ulDstAddr = 0; 
	unsigned char uctemp = 0;

	temp = SST25VF_ReadStatusReg();

	SST25VF_WriteStatusReg(0x00);

	for(i = 0; i < usMaxPageNum; i++)		//
	{
		ulDstAddr = PLAY_PAGE_SIZE * i + SST25V_PLAY_FIRST_ADDR + 30;

		EXFLASH_WriteData(ulDstAddr, &uctemp, 1);	   //��Flash��ʾ������Ϣ����

//		SST25VF_64KErase(ulDstAddr); 	
	}

	SST25VF_WriteStatusReg(temp);	
}
/****************************************************************************
** ��������: ɾ����Ŀ����ĵ�����ʾ��Ŀ.
** �䡡  ��: ��   
** �� �� ��: ��     
*****************************************************************************/

void EXFLASH_DelOnePlay(unsigned long ulFirstAddr)
{
	unsigned char temp=0;
	unsigned char i;
	unsigned long ulDstAddr = 0; 

	temp = SST25VF_ReadStatusReg();

	SST25VF_WriteStatusReg(0x00);
	
	for(i = 0; i < SECTOR_NUM_PAGE; i++)
	{
		ulDstAddr = SECTOR_SIZE * i + ulFirstAddr;
		SST25VF_4KErase(ulDstAddr);	
	}

	SST25VF_WriteStatusReg(temp);	
}
/****************************************************************************
** ��������:   ��flash��ָ���ַ����ȡ����,����Ҫ�ж����ݼ���.
** �䡡  ��:    unsigned long ulDstAddr  д�����ݵ���ʼ��ַ,
				unsigned char *pucByteBuf ��д�������,
				unsigned short usLen д�����ݵĸ���,
** �� �� ��: ��     
*****************************************************************************/
void EXFLASH_ReadCmdData(unsigned long ulDstAddr, unsigned char *pucByteBuf, unsigned short usLen)
{
	SST25VF_Read(ulDstAddr, pucByteBuf, usLen);
}
/****************************************************************************
** ��������:   ��flash��ָ���ַ����ȡ���ݣ���Ҫ�������ݼ����ж϶�ȡ�������Ƿ���Ҫȡ��.
** �䡡  ��:    unsigned long ulDstAddr  д�����ݵ���ʼ��ַ,
				unsigned char *pucByteBuf ��д�������,
				unsigned short usLen д�����ݵĸ���,
** �� �� ��: ��     
*****************************************************************************/
#if 0
void EXFLASH_ReadDisplayData(unsigned long ulDstAddr, unsigned char *pucByteBuf, unsigned short usLen)
{
	unsigned char temp;
	unsigned short i;

	SST25VF_Read(ulDstAddr, pucByteBuf, usLen);

	if(!(DISPanelAttri.ucDataPolar))
	{
		for(i = 0; i < usLen; i++)
		{
			temp = *pucByteBuf;
			*pucByteBuf++ = ~temp;	
		}		
	}		
}
#endif


