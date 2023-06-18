#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "RC522.h"
#include <string.h>
extern unsigned char SN[4]; //����

int main(void) {
	Serial_Init();
	delay_init();
	RC522_Init(); //RC522��ʼ��
	while (1) {
		if (read_card_data() == MI_OK) {
			// ��ȡ������
			u8 card_id[20] = { 0 };
			for (int i = 0;i < 4;i++) {
				sprintf(&card_id[i * 2], "%02X", SN[i]);
			}
			printf("ID: %s\r\n", card_id);
			delay_ms(2000);
		}
	}
}
