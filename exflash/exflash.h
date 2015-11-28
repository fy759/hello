#ifdef                          _EXFLASH_

#else

void EXFLASH_WriteCmd(unsigned long ulDstAddr, unsigned char *pucByteBuf, unsigned short usLen);
void EXFLASH_WriteData(unsigned long ulDstAddr, unsigned char *pucByteBuf, unsigned short usLen);
void EXFLASH_DelAllPlay(unsigned short usMaxPageNum);
void EXFLASH_DelOnePlay(unsigned long ulFirstAddr);
void EXFLASH_ReadCmdData(unsigned long ulDstAddr, unsigned char *pucByteBuf, unsigned short usLen);
void EXFLASH_ReadDisplayData(unsigned long ulDstAddr, unsigned char *pucByteBuf, unsigned short usLen);
void EXFLASH_ClearPragramUpdate(unsigned long ulDstAddr, unsigned short usLen);

#endif

#define           MAP_TABLE                         0x000000
#define           ASCII16_OFFSET                    0x004000
#define           ASCII24_OFFSET                    0x005000
#define           ASCII32_OFFSET                    0x007000

#define           FONT16_OFFSET                     0x009000
#define           FONT24_OFFSET                     0x04B000
#define           FONT32_OFFSET                     0x0DB000

#define           SST25V_THAIOFFSET			        0x1DB000		    //泰文字库偏移 

#define           SST25V_AREA_ATTRI_ADDR            0x1e0000			//分区属性
#define           SST25V_MAX_PLAY_NUM_ADDR	        0x1e2000


#define           SST25V_HARD_ATTRI_ADDR			0x1f0000		//屏配置属性
#define           SST25V_BRIGHT_ATTRI_ADDR          0x1f1000	 	//亮度
#define           EQUIPMENT_NUMBER_ADDR				0x1f2000	    //设备号
#define           EQUIPMENT_PARAMETER_ADDR			0x1f3000	    //设备参数
#define           SST25V_UPDATE_PACKAGE_CNT_ADDR    0x1f4000		//升级总包数
#define           SST25V_UPDATE_FLAG_ADDR           0x1f5000		//升级标志
#define           SST25V_UPDATE_SUCCESS_ADDR	   	0x1f6000		//升级成功返回保存
#define			  SST25V_UPDATE_TIMES_CNT_ADDR		0x1f7000		//升级次数计数		升级UPDATE_TIMES_MAX次不成功，取消升级。
#define           SST25V_POWER_ATTRI_ADDR           0x1f9000		//定时开关机
#define           SST25V_TRUST_NUM_ADDR             0x1fc000	    //信任号码
#define           SST25V_PROGRAM_UPDATE_ADDR        0x200000		//升级代码保存地址地址.	128KBytes
#define           SST25V_PLAY_FIRST_ADDR            0x220000		//显示节目的起始地址. 


#define           PLAY_CMD	                        32  			//节目的命令字节所占空间
#define           PLAY_CMD_TIME						256			    //节目命令+时段保存空间 命令[0:31]+日期[32:37]+时段数[39]+时段[40:255]
#define			  TIME_DATA_SIZE					24				//日期和时段
#define           PLAY_DATA_SIZE	                0x4000			//节目的数据字节所占空间  
#define           PLAY_PAGE_SIZE                    0x5000			//一个节目页的所占flash的字节空间. 20K
#define			  VOICE_MEM_SIZE					0x400			//保存汉字编码 1K

#define			  PLAY_EXIST_FLAG        		    0x01 		    //节目存在标志
#define			  MAX_PLAY_NUMBER					100			    //最大100个节目
#define			  UPDATE_PACKAGE_SIZE				256				//升级包数据大小
#define			  EXFLASH_UPDATE_SIZE				128				//升级代码存储大小  128KBytes
#define			  UPDATE_FLAG_YES					1				//需要升级
#define			  UPDATE_FLAG_NO					0				//不需要升级
#define			  UPDATE_TIMES_MAX					10				//升级执行的最大次数




