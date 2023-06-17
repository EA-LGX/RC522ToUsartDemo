#include "sys.h"
#include "usart.h"
#include "stdbool.h"
//#include "Myself_define.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 

char USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

u16 USART1_RX_STA = 0;       //����״̬���	 

bool  Flag_Usart1_Receive;
bool  Flag_Usart1_Remap_Receive;
bool  Flag_Usart1_Remap;
bool  Flag_usart1_receive_OK = 0;
bool 	Flag_usart1_receive_T = 0;


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE
{
	int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) {
	x = x;
}
//�ض���fputc���� 
int fputc(int ch, FILE* f) {
	while ((USART1->SR & 0X40) == 0);//ѭ������,ֱ���������   
	USART1->DR = (u8)ch;
	return ch;
}
#endif 

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA = 0;       //����״̬���	  

void Usart1_Init(u32 bound) {
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART1��GPIOAʱ��
	GPIO_PinRemapConfig(GPIO_Remap_USART1, DISABLE);//��ӳ��ر�	
	//USART1_TX   PA.9��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;	//һ������Ϊ9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;	//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure);	//��ʼ������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//�����ж�
	USART_Cmd(USART1, ENABLE);	//ʹ�ܴ���

}

void Usart1_Remap_Init(u32 bound) {
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART1��GPIOAʱ��
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

	//USART1_TX   PB.6��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PB.6
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.6

	//USART1_RX	  GPIOB.7��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOAB.7  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;	//һ������Ϊ9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;	//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure);	//��ʼ������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//�����ж�
	USART_Cmd(USART1, ENABLE);	//ʹ�ܴ���

}

/* USART1����һ���ַ� */
void USART1TxChar(char ch) {
	USART_SendData(USART1, (u8)ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
	}

}

/* USART1����һ���������ݣ�ʮ�����ƣ� */
void USART1TxData(unsigned char* pt) {
	while (*pt != '\0') {
		USART1TxChar(*pt);
		pt++;
	}
}

void USART1TxData_hex(unsigned char* pt, unsigned char len) {
	while (len) {
		USART1TxChar(*pt);
		pt++;
		len--;
	}
}

/* USART1����һ���ַ��� */
void USART1TxStr(char* pt) {
	while (*pt != '\0') {
		USART1TxChar(*pt);
		pt++;
	}
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if (USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET)	//�����ж�
	{
		Res = USART_ReceiveData(USART1);	//��ȡ���յ�������
		USART1_RX_BUF[USART1_RX_STA] = Res;
		USART1_RX_STA++;
		if (USART1_RX_STA > (USART1_REC_LEN - 1))
			USART1_RX_STA = 0;
		Flag_Usart1_Receive = 1;
		Count_Timer3_value_USART1_receive_timeout = 2;
	}
}

void CLR_Buf1(void)     //�������1���ջ���
{
	unsigned int y = 0;
	for (y = 0;y < USART1_REC_LEN;y++) {
		USART1_RX_BUF[y] = '\0';
	}
	USART1_RX_STA = 0;
}

#endif
