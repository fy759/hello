#include "Include.h"


#define FLASH_DESTION	0x08003000
#define PAGE_SIZE		0x400
#define FLASH_PAGES		116		//128-12
#define PACKAGE_SIZE	256

#define PROGRAM_ALL_PACKAGES				464		//FLASH_PAGES*4

FLASH_Status FLASHStatus = FLASH_COMPLETE;







/***********************************************************
**** 擦除内部应用程序Flash区
***********************************************************/
void FlashEraseProgramArea(void)
{
	unsigned EraseCounter;
	
	for(EraseCounter = 0; EraseCounter < FLASH_PAGES; EraseCounter ++)
	{
		FLASHStatus = FLASH_ErasePage(FLASH_DESTION + PAGE_SIZE * EraseCounter);
		Delay(DELAY_100US);		
	}
	
}


/***********************************************************
**** 读取外部Flash代码，并且写内部应用程序
***********************************************************/
void FlashProgram(void)
{
	unsigned char ucProgramBuf[PACKAGE_SIZE];
	unsigned int i,j,k;
	unsigned long Address;
	unsigned long FlashDestination;
	unsigned int HalfPackageSize;
	unsigned long RamSource;
	
	HalfPackageSize = PACKAGE_SIZE >> 2;		//1/4
	
	for(i=0; i<PROGRAM_ALL_PACKAGES; i++)
	{
		Address = SST25V_PROGRAM_UPDATE_ADDR + PACKAGE_SIZE * i;
		EXFLASH_ReadCmdData(Address, ucProgramBuf, PACKAGE_SIZE);
		Delay(DELAY_1MS);
		
		for(j=0; j<HalfPackageSize; j++)
		{
			k = j*4;
			
			RamSource = ucProgramBuf[k+3] << 24;
			RamSource |= ucProgramBuf[k+2] << 16;
			RamSource |= ucProgramBuf[k+1] << 8;
			RamSource |= ucProgramBuf[k];
			FlashDestination = ApplicationAddress + PACKAGE_SIZE * i + k;
			FLASH_ProgramWord(FlashDestination, RamSource);
			
		}		
	}	
}



/***********************************************************
**** 计数升级次数，UPDATE_TIMES_MAX次不成功，放弃升级
***********************************************************/
void UpdateTimesCount(void)
{
	unsigned char ucUpFlag;
	unsigned char ucUpdateTimes = 0;
	
	EXFLASH_ReadCmdData(SST25V_UPDATE_TIMES_CNT_ADDR, &ucUpdateTimes, 1);
	Delay(DELAY_100US);
	if(ucUpdateTimes >= UPDATE_TIMES_MAX)				//超过次数
	{
		ucUpdateTimes = 0;
		EXFLASH_WriteCmd(SST25V_UPDATE_TIMES_CNT_ADDR, &ucUpdateTimes, 1);		//清升级次数
		Delay(DELAY_100US);
		ucUpFlag = UPDATE_FLAG_NO;
		EXFLASH_WriteCmd(SST25V_UPDATE_FLAG_ADDR, &ucUpFlag, 1);		//清标志
		Delay(DELAY_100US);
		SerialPutString("\r\nUpdate fail.");
	}
	else
	{
		ucUpdateTimes ++;
		EXFLASH_WriteCmd(SST25V_UPDATE_TIMES_CNT_ADDR, &ucUpdateTimes, 1);		//升级次数加1
		Delay(DELAY_100US);
	}
	
}






