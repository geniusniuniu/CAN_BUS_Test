#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Mycan.h"
#include "Key.h"

uint8_t keyNum;
uint8_t txData[2] = {0x66,0x88};
uint32_t canPeriphRxID;
uint8_t rxDataLenth;
uint8_t rxData[8];
int main(void)
{
	OLED_Init();
	MyCanInit();
	Key_Init();
	OLED_ShowString(2,1,"RxID:");
	OLED_ShowString(3,1,"Leng:");
	OLED_ShowString(4,1,"Data:");
	while (1)
	{
		keyNum = Key_GetNum();
		if(keyNum == 1)
		{
			myCanTransmit(0x123,2,txData);
		}
		
		if(myCanReceiveIsEmpty() == 1)
		{
			myCanReceive(&canPeriphRxID,&rxDataLenth,rxData);
			
			OLED_ShowHexNum(2,6,canPeriphRxID,4);
			OLED_ShowHexNum(3,6,rxDataLenth,1);
			OLED_ShowHexNum(4,6,rxData[0],2);
			OLED_ShowHexNum(4,10,rxData[1],2);
		}
	}
}
