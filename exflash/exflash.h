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

#define           SST25V_THAIOFFSET			        0x1DB000		    //̩���ֿ�ƫ�� 

#define           SST25V_AREA_ATTRI_ADDR            0x1e0000			//��������
#define           SST25V_MAX_PLAY_NUM_ADDR	        0x1e2000


#define           SST25V_HARD_ATTRI_ADDR			0x1f0000		//����������
#define           SST25V_BRIGHT_ATTRI_ADDR          0x1f1000	 	//����
#define           EQUIPMENT_NUMBER_ADDR				0x1f2000	    //�豸��
#define           EQUIPMENT_PARAMETER_ADDR			0x1f3000	    //�豸����
#define           SST25V_UPDATE_PACKAGE_CNT_ADDR    0x1f4000		//�����ܰ���
#define           SST25V_UPDATE_FLAG_ADDR           0x1f5000		//������־
#define           SST25V_UPDATE_SUCCESS_ADDR	   	0x1f6000		//�����ɹ����ر���
#define			  SST25V_UPDATE_TIMES_CNT_ADDR		0x1f7000		//������������		����UPDATE_TIMES_MAX�β��ɹ���ȡ��������
#define           SST25V_POWER_ATTRI_ADDR           0x1f9000		//��ʱ���ػ�
#define           SST25V_TRUST_NUM_ADDR             0x1fc000	    //���κ���
#define           SST25V_PROGRAM_UPDATE_ADDR        0x200000		//�������뱣���ַ��ַ.	128KBytes
#define           SST25V_PLAY_FIRST_ADDR            0x220000		//��ʾ��Ŀ����ʼ��ַ. 


#define           PLAY_CMD	                        32  			//��Ŀ�������ֽ���ռ�ռ�
#define           PLAY_CMD_TIME						256			    //��Ŀ����+ʱ�α���ռ� ����[0:31]+����[32:37]+ʱ����[39]+ʱ��[40:255]
#define			  TIME_DATA_SIZE					24				//���ں�ʱ��
#define           PLAY_DATA_SIZE	                0x4000			//��Ŀ�������ֽ���ռ�ռ�  
#define           PLAY_PAGE_SIZE                    0x5000			//һ����Ŀҳ����ռflash���ֽڿռ�. 20K
#define			  VOICE_MEM_SIZE					0x400			//���溺�ֱ��� 1K

#define			  PLAY_EXIST_FLAG        		    0x01 		    //��Ŀ���ڱ�־
#define			  MAX_PLAY_NUMBER					100			    //���100����Ŀ
#define			  UPDATE_PACKAGE_SIZE				256				//���������ݴ�С
#define			  EXFLASH_UPDATE_SIZE				128				//��������洢��С  128KBytes
#define			  UPDATE_FLAG_YES					1				//��Ҫ����
#define			  UPDATE_FLAG_NO					0				//����Ҫ����
#define			  UPDATE_TIMES_MAX					10				//����ִ�е�������




