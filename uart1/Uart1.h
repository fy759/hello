#ifndef		__USART1_H__
#define		__USART1_H__

#define SerialPutString(x) Serial_PutString((unsigned char*)(x))


void Uart1Init(void);
void Serial_PutString(unsigned char *s);



#endif

