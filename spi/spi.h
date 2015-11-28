#ifndef                      __SPI__
#define						 __SPI__
#include "stm32f10x_lib.h"

extern GPIO_InitTypeDef GPIO_InitStructure;//ÉùÃ÷GPIO_InitStructure


void SPIInit(void);
void SST25VF_Read(unsigned long ulAddr, unsigned char *pucDest, unsigned short usLen);
void SST25VF_4KErase(unsigned long ulAddr);
void SST25VF_64KErase(unsigned long ulAddr);

unsigned char SST25VF_ReadStatusReg(void);
void SST25VF_WriteStatusReg(unsigned char ucStatusValue);

void WinBond_BufferWrite(unsigned char *pBuffer, unsigned long WriteAddr, unsigned short NumByteToWrite);

#endif

