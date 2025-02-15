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
		CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;	//����ģʽ
	else if(CanOperatingMode == CAN_NORMAL)
		CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	//��������ģʽ
	else if (CanOperatingMode == CAN_SILENT)
		CAN_InitStructure.CAN_Mode = CAN_Mode_Silent;	//��Ĭģʽ	
	
	CAN_InitStructure.CAN_Prescaler = 6;		//����CANBUS BaudRate=36M/12/(1+CAN_BS1_2tq+CAN_BS2_3tq)=500Khz	
	CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;	//λʱ���1���� 
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;	//λʱ���2����
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq; 	//��ͬ���������
	CAN_InitStructure.CAN_ABOM = DISABLE;		//�����Զ��ָ�����
	CAN_InitStructure.CAN_AWUM = DISABLE;		//�Զ����ѹ���
	CAN_InitStructure.CAN_NART = DISABLE;		//���Զ��ش�����
	CAN_InitStructure.CAN_RFLM = DISABLE;		//����fifo��������
	CAN_InitStructure.CAN_TTCM = DISABLE;		//ʱ�䴥��ͨ�Ź���
	CAN_InitStructure.CAN_TXFP = DISABLE;		//������Ϣ������˳��
	CAN_Init(CAN1,&CAN_InitStructure);
	
	CAN_FilterInitTypeDef CFIStructure;
	CFIStructure.CAN_FilterNumber = 0;							//��ʼ���Ĺ��������
	
	CFIStructure.CAN_FilterScale = CAN_FilterScale_32bit;	
	CFIStructure.CAN_FilterMode = CAN_FilterMode_IdMask;	
	CFIStructure.CAN_FilterIdHigh = 0x0000;						//����32λID,����Ϊȫͨģʽ
	CFIStructure.CAN_FilterIdLow = 0x0000;
	CFIStructure.CAN_FilterMaskIdHigh = 0x0000;
	CFIStructure.CAN_FilterMaskIdLow = 0x0000;
	
	CFIStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;	//������������fifo�����
	CFIStructure.CAN_FilterActivation = ENABLE;					//����������		
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
	while(CAN_TransmitStatus(CAN1,retVal) != CAN_TxStatus_Ok)	//�ȴ����ķ��ͳɹ�
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
        //...����ң��֡ 
    }  	
	
}



