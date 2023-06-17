#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stdio.h"	
#include "sys.h" 
#include "stdbool.h"

//////////////////////////////////////////////////////////////////////////////////	 

#define USART1_REC_LEN			500
#define USART_REC_LEN  		500  	//�����������ֽ��� 500
#define EN_USART1_RX 				1		//ʹ�ܣ�1��/��ֹ��0������1����

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;         		//����״̬���
extern u16 USART2_RX_STA;         		//����״̬���
extern u16 USART3_RX_STA;         		//����״̬���
extern char USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.
extern unsigned char USART2_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern unsigned char USART3_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.
extern unsigned char UART5_RX_BUF[USART_REC_LEN]; 					//���ջ���,���USART_REC_LEN���ֽ�.

extern bool Flag_Usart1_Receive;
extern bool Flag_Usart2_Receive;
extern bool Flag_Usart3_Receive;
extern bool Flag_usart1_receive_OK;
extern bool Flag_usart1_receive_T;
extern bool Flag_usart2_receive_T;

extern unsigned char Count_Timer3_value_USART1_receive_timeout;
extern unsigned char Count_Timer3_value_USART2_receive_timeout;
extern unsigned char Count_Timer3_value_USART3_receive_timeout;
extern unsigned char Count_time_wait_ok;

extern unsigned char Flag_Uart5_Receive;
extern unsigned char Flag_uart5_receive_OK;


void Usart1_Init(u32 bound);
void Usart1_Remap_Init(u32 bound);
void USART1TxChar(char ch);
void USART1TxData(unsigned char* pt);
void USART1TxStr(char* pt);
void USART1_IRQHandler(void);
void CLR_Buf1(void);

void Usart2_Init(u32 bound);
void Usart2_Remap_Init(u32 bound);
static void USART2_RCC_Configuration(void);
static void USART2_GPIO_Configuration(void);
static void USART2_Configuration(u32 bound);
static void USART2_NVIC_Configuration(void);
void USART2TxChar(int ch);
void USART2TxData(unsigned char* pt);
void USART2TxData_hex(unsigned char* pt, unsigned char len);
void USART2TxStr(char* pt);
void CLR_Buf2(void);     //�������2���ջ���

void Usart3_Init(u32 bound);
void Usart3_Remap_Init(u32 bound);
void USART3TxChar(int ch);
void USART3TxData(unsigned char* pt);
void USART3TxData_hex(unsigned char* pt, unsigned char len);
void USART3TxStr(char* pt);
void USART3_IRQHandler(void);
void CLR_Buf3(void);

void Uart5_Init(u32 bound);//����5��׼�ӿڳ�ʼ��
static void UART5_GPIO_Configuration(void);
static void UART5_Configuration(u32 bound);
static void UART5_NVIC_Configuration(void);
void UART5TxChar(int ch);
void UART5TxData(unsigned char* pt);
void UART5TxData_hex(unsigned char* pt, unsigned char len);
void UART5TxStr(char* pt);
void CLR_Buf5(void);     //�������5���ջ���
void dataReceive(u8* data, u8 len); // ���ݽ��ջص�����

void addToBuff(u8 ch);

#endif

