#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"                  // Device header  

#define EMPTY 		0
#define NOT_EMPTY 	1

#define CAN_LOOPBACK	0
#define CAN_NORMAL		1
#define CAN_SILENT		2	//¾²Ä¬Ä£Ê½

void MyCanInit(uint8_t CanOperatingMode);
void myCanTransmit(uint32_t canPeriphTxID,uint8_t dataLenth,uint8_t * data);
uint8_t myCanReceiveIsEmpty(void);
void myCanReceive(uint32_t * canPeriphRxID,uint8_t * dataLenth,uint8_t * data);




#endif

