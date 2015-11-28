/****************************************Copyright (c)****************************************************
**                                      中帝威科技
**                                     
**                                 http://www.DeviceWell.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               exflash.c
** Last modified Date:      2011-07-25
** Last Version:            1.0
** Description:             外部flash的操作函数.
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              陈华
** Created date:            2011-07-25
** Version:                 1.0
** Descriptions:            The original version 初始版本
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
** 功能描述: 写入数据到flash中,在写之前需要先执行擦除的动作.
** 输　  入:    unsigned long ulDstAddr  写入数据的起始地址,
				unsigned char *pucByteBuf 需写入的数据,
				unsigned short usLen 写入数据的个数,
** 输 　 出: 无     
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
** 功能描述: 写入数据到flash中.
** 输　  入:    unsigned long ulDstAddr  写入数据的起始地址,
				unsigned char *pucByteBuf 需写入的数据,
				unsigned short usLen 写入数据的个数,
** 输 　 出: 无     
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
** 功能描述: 删除节目区域的所有显示节目.
** 输　  入: 无   
** 输 　 出: 无     
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

		EXFLASH_WriteData(ulDstAddr, &uctemp, 1);	   //将Flash表示分区信息置零

//		SST25VF_64KErase(ulDstAddr); 	
	}

	SST25VF_WriteStatusReg(temp);	
}
/****************************************************************************
** 功能描述: 删除节目区域的单个显示节目.
** 输　  入: 无   
** 输 　 出: 无     
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
** 功能描述:   从flash的指令地址处读取数据,不需要判断数据极性.
** 输　  入:    unsigned long ulDstAddr  写入数据的起始地址,
				unsigned char *pucByteBuf 需写入的数据,
				unsigned short usLen 写入数据的个数,
** 输 　 出: 无     
*****************************************************************************/
void EXFLASH_ReadCmdData(unsigned long ulDstAddr, unsigned char *pucByteBuf, unsigned short usLen)
{
	SST25VF_Read(ulDstAddr, pucByteBuf, usLen);
}
/****************************************************************************
** 功能描述:   从flash的指令地址处读取数据，需要根据数据极性判断读取的数据是否需要取反.
** 输　  入:    unsigned long ulDstAddr  写入数据的起始地址,
				unsigned char *pucByteBuf 需写入的数据,
				unsigned short usLen 写入数据的个数,
** 输 　 出: 无     
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


