#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "RC522.h"

extern unsigned char SN[4]; //卡号

int main(void) {
	Serial_Init();
	delay_init();
	RC522_Init(); //RC522初始化
	while (1) {
		if (read_card_data() == MI_OK) {
			// 读取到卡号
			u8 card_id[20] = { 0 };
			for (int i = 0;i < 4;i++) {
				sprintf(&card_id[i * 2], "%02X", SN[i]);
			}
		}
	}
}
