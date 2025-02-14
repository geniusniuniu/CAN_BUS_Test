#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"                  // Device header  

void MyCanInit(void);
void myCanTransmit(uint32_t canPeriphTxID,uint8_t dataLenth,uint8_t * data);
uint8_t myCanReceiveIsEmpty(void);
void myCanReceive(uint32_t * canPeriphRxID,uint8_t * dataLenth,uint8_t * data);




#endif

