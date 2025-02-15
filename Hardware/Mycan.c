#include "Mycan.h"
#include "string.h"

void MyCanInit(uint8_t CanOperatingMode)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	CAN_InitTypeDef CAN_InitStructure;
	if(CanOperatingMode == CAN_LOOPBACK)
		CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;	//环回模式
	else if(CanOperatingMode == CAN_NORMAL)
		CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	//正常工作模式
	else if (CanOperatingMode == CAN_SILENT)
		CAN_InitStructure.CAN_Mode = CAN_Mode_Silent;	//静默模式	
	
	CAN_InitStructure.CAN_Prescaler = 6;		//高速CANBUS BaudRate=36M/12/(1+CAN_BS1_2tq+CAN_BS2_3tq)=500Khz	
	CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;	//位时间段1长度 
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;	//位时间段2长度
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq; 	//再同步补偿宽度
	CAN_InitStructure.CAN_ABOM = DISABLE;		//离线自动恢复管理
	CAN_InitStructure.CAN_AWUM = DISABLE;		//自动唤醒管理
	CAN_InitStructure.CAN_NART = DISABLE;		//不自动重传功能
	CAN_InitStructure.CAN_RFLM = DISABLE;		//接收fifo锁定功能
	CAN_InitStructure.CAN_TTCM = DISABLE;		//时间触发通信功能
	CAN_InitStructure.CAN_TXFP = DISABLE;		//发送消息的请求顺序
	CAN_Init(CAN1,&CAN_InitStructure);
	
	CAN_FilterInitTypeDef CFIStructure;
	CFIStructure.CAN_FilterNumber = 0;							//初始化的过滤器序号
	
	CFIStructure.CAN_FilterScale = CAN_FilterScale_32bit;	
	CFIStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	
	CFIStructure.CAN_FilterIdHigh = 0x0000;						//两组32位ID,配置为全通模式
	CFIStructure.CAN_FilterIdLow = 0x0000;
	CFIStructure.CAN_FilterMaskIdHigh = 0x0000;
	CFIStructure.CAN_FilterMaskIdLow = 0x0000;
	
	CFIStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;	//过滤器关联的fifo的序号
	CFIStructure.CAN_FilterActivation = ENABLE;					//过滤器激活		
	CAN_FilterInit(&CFIStructure);
	
}


void myCanTransmit(uint32_t canPeriphTxID,uint8_t dataLenth,uint8_t * data)
{
	CanTxMsg txMsg;
	txMsg.StdId = canPeriphTxID;
	txMsg.ExtId = canPeriphTxID;
	txMsg.IDE = CAN_Id_Standard;
	txMsg.RTR = CAN_RTR_Data;
	txMsg.DLC = dataLenth;	
	memcpy(txMsg.Data, data, dataLenth);  
	uint8_t retVal = CAN_Transmit(CAN1,&txMsg);
	
	uint32_t timeOverFlow = 0;
	while(CAN_TransmitStatus(CAN1,retVal) != CAN_TxStatus_Ok)	//等待报文发送成功
	{
		timeOverFlow++;
		if(timeOverFlow > 10000)
		{
			break;
		}
	}
}

uint8_t myCanReceiveIsEmpty(void)
{
	if(CAN_MessagePending(CAN1,CAN_FIFO0) > 0)
	{
		return 1;
	}
	return 0;
}

void myCanReceive(uint32_t * canPeriphRxID,uint8_t * dataLenth,uint8_t * data)
{
	CanRxMsg rxMsg;
	CAN_Receive(CAN1,CAN_FIFO0,&rxMsg);
	
	if(rxMsg.IDE == CAN_Id_Standard)
	{
		* canPeriphRxID = rxMsg.StdId;
	}
	else
	{
		* canPeriphRxID = rxMsg.ExtId;
	}
	
	if(rxMsg.RTR == CAN_RTR_DATA)
	{
		* dataLenth = rxMsg.DLC;
		memcpy(data,rxMsg.Data,*dataLenth);
	}
	else  
    {  
        //...处理遥控帧 
    }  	
	
}



