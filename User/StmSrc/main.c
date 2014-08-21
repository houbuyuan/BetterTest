/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Extern variables ----------------------------------------------------------*/

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USARTx, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

bool StartToSendData = FALSE;

int count = 0;
int receive_flag = 1;
int receive_level = 0;
int receive_run = 1;
int send_flag = 0;
int status = 0;
RFBenchmark_TypeDef benchmark;
RFDataLenth_TypeDef dataLenth;
int data_bit[24];
int data_final[12];
int Tim3_flg = 0;

#define SOFT_MODEM_BAUD_RATE   (315)
#define SOFT_MODEM_LOW_FREQ    (1575)
#define SOFT_MODEM_HIGH_FREQ   (3150)

#define TIM_PER_SEC		1000000

#define SOFT_MODEM_BIT_PERIOD      (TIM_PER_SEC/SOFT_MODEM_BAUD_RATE)
#define SOFT_MODEM_HIGH_USEC       (TIM_PER_SEC/SOFT_MODEM_HIGH_FREQ)
#define SOFT_MODEM_LOW_USEC        (TIM_PER_SEC/SOFT_MODEM_LOW_FREQ)

#define SOFT_MODEM_HIGH_CNT        (SOFT_MODEM_BIT_PERIOD/SOFT_MODEM_HIGH_USEC)
#define SOFT_MODEM_LOW_CNT         (SOFT_MODEM_BIT_PERIOD/SOFT_MODEM_LOW_USEC)


void toPhone_modulate(uint8_t b)
{
	volatile int tcnt,cnt;
	if(b){
		cnt = SOFT_MODEM_HIGH_CNT;
		tcnt = SOFT_MODEM_HIGH_USEC / 2;
	}else{
		cnt = SOFT_MODEM_LOW_CNT;
		tcnt = SOFT_MODEM_LOW_USEC / 2;
	}
	do {
		cnt--;
		{
			Tim3_flg = 0;
			TIM_SetCounter(TIM3, tcnt);
			while(!Tim3_flg);
		}
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))));
		{
			Tim3_flg = 0;
			TIM_SetCounter(TIM3, tcnt);
			while(!Tim3_flg);
		}
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))));
	} while (cnt);
	GPIO_WriteBit(GPIOB, GPIO_Pin_5,0);
}

void toPhone_write(uint8_t data)
{
	int mask;
	toPhone_modulate(0);							 // Start Bit
	for(mask = 1; mask; mask <<= 1){ // Data Bits
		if(data & mask){
			toPhone_modulate(1);
		}
		else{
			toPhone_modulate(0);
		}
	}
	toPhone_modulate(1);				// Stop Bit
	toPhone_modulate(1);				// Push Bit
}

void loop_task()
{
	int i;
	for (i = 0 ; i < 20 ; i++)
	{
		toPhone_write(i);
		Tim3_flg = 0;
		TIM_SetCounter(TIM3, 50000);
		while(!Tim3_flg);
	}
}

void calculate_benchmark(int count)
{
	benchmark.narrow_benchmark = count;
	benchmark.narrow_benchmark_min = benchmark.narrow_benchmark - (int)(benchmark.narrow_benchmark/6);
	benchmark.narrow_benchmark_max = benchmark.narrow_benchmark + (int)(benchmark.narrow_benchmark/6);
	benchmark.wide_benchmark = benchmark.narrow_benchmark * 3;	//narrow benchmark : wide benchmark = 1:3
	benchmark.wide_benchmark_min = benchmark.wide_benchmark - (int)(benchmark.wide_benchmark/6);
	benchmark.wide_benchmark_max = benchmark.wide_benchmark + (int)(benchmark.wide_benchmark/6);
}

void isNarrowOrWide(int count, int dataBit)
{
	if(count >= benchmark.narrow_benchmark_min && count <= benchmark.narrow_benchmark_max)
	{
		data_bit[dataBit] = 0;
		status++;
	}
	else if(count >= benchmark.wide_benchmark_min && count <= benchmark.wide_benchmark_max)
	{
		data_bit[dataBit] = 1;
		status++;
	}
	else
	{
		status = STATUS_HEADER_HIGH;
		if(DEBUG) printf("%d Data no match, go to header matching.\r\n", dataBit);
	}
}

int receiveData()
{
	int run = 1;
	int i,j, dataTmp;
	
	TIM_SetCounter(TIM2, 0);
	status = STATUS_HEADER_HIGH;
	receive_run = 1;
	receive_flag = 1;

	while(receive_run)
	{
		while(receive_flag);
		receive_flag = 1;
		
		switch(status)
		{
			case STATUS_HEADER_HIGH:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.HEADER_HIGH = count;
				status++;
				break;
			case STATUS_HEADER_LOW:
				if(receive_level == LEVEL_HIGHT)
					break;
				dataLenth.HEADER_LOW = count;
				if((dataLenth.HEADER_LOW >= dataLenth.HEADER_HIGH * 30) 
					&& (dataLenth.HEADER_LOW <= dataLenth.HEADER_HIGH * 32))	//HEADER_LOW : HEADER_HIGHT = 31:1
				{
					calculate_benchmark(dataLenth.HEADER_HIGH);
					status++;
				}
				else
				{
					status = STATUS_HEADER_HIGH;
				}
				break;
			case STATUS_GROUP1_MEMBER1_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP1_MEMBER1_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP1_MEMBER1_1, 0);
				break;
			case STATUS_GROUP1_MEMBER1_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP1_MEMBER1_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP1_MEMBER1_2, 1);
				break;
			case STATUS_GROUP1_MEMBER2_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP1_MEMBER2_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP1_MEMBER2_1, 2);
				break;
			case STATUS_GROUP1_MEMBER2_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP1_MEMBER2_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP1_MEMBER2_2, 3);
				break;
			case STATUS_GROUP1_MEMBER3_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP1_MEMBER3_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP1_MEMBER3_1, 4);
				break;
			case STATUS_GROUP1_MEMBER3_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP1_MEMBER3_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP1_MEMBER3_2, 5);
				break;
			case STATUS_GROUP1_MEMBER4_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP1_MEMBER4_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP1_MEMBER4_1, 6);
				break;
			case STATUS_GROUP1_MEMBER4_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP1_MEMBER4_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP1_MEMBER4_2, 7);
				break;
			case STATUS_GROUP2_MEMBER1_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP2_MEMBER1_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP2_MEMBER1_1, 8);
				break;
			case STATUS_GROUP2_MEMBER1_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP2_MEMBER1_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP2_MEMBER1_2, 9);
				break;
			case STATUS_GROUP2_MEMBER2_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP2_MEMBER2_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP2_MEMBER2_1, 10);
				break;
			case STATUS_GROUP2_MEMBER2_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP2_MEMBER2_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP2_MEMBER2_2, 11);
				break;
			case STATUS_GROUP2_MEMBER3_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP2_MEMBER3_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP2_MEMBER3_1, 12);
				break;
			case STATUS_GROUP2_MEMBER3_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP2_MEMBER3_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP2_MEMBER3_2, 13);
				break;
			case STATUS_GROUP2_MEMBER4_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP2_MEMBER4_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP2_MEMBER4_1, 14);
				break;
			case STATUS_GROUP2_MEMBER4_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP2_MEMBER4_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP2_MEMBER4_2, 15);
				break;
			case STATUS_GROUP3_MEMBER1_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP3_MEMBER1_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP3_MEMBER1_1, 16);
				break;
			case STATUS_GROUP3_MEMBER1_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP3_MEMBER1_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP3_MEMBER1_2, 17);
				break;
			case STATUS_GROUP3_MEMBER2_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP3_MEMBER2_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP3_MEMBER2_1, 18);
				break;
			case STATUS_GROUP3_MEMBER2_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP3_MEMBER2_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP3_MEMBER2_2, 19);
				break;
			case STATUS_GROUP3_MEMBER3_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP3_MEMBER3_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP3_MEMBER3_1, 20);
				break;
			case STATUS_GROUP3_MEMBER3_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP3_MEMBER3_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP3_MEMBER3_2, 21);
				break;
			case STATUS_GROUP3_MEMBER4_1:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP3_MEMBER4_1 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP3_MEMBER4_1, 22);
				break;
			case STATUS_GROUP3_MEMBER4_2:
				if(receive_level == LEVEL_LOW)
					break;
				dataLenth.DATA_GROUP3_MEMBER4_2 = count;
				isNarrowOrWide(dataLenth.DATA_GROUP3_MEMBER4_2, 23);
				if(status > STATUS_GROUP3_MEMBER4_2)
				{
					for(i = 0; i< 12; i++)
					{
						dataTmp = 0;
						for(j = 0; j < 2; j++)
						{
							dataTmp = ((dataTmp << 1) + data_bit[i * 2 + j]);
						}
						data_final[i] = dataTmp;
					}
					receive_run = 0;
				}
				break;
		}
	}
	if(DEBUG) printf("[Reciver]Done.\r\n");
}

/*******************************************************************************
* Function Name  : main
* Description    : the main function
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	
	/********Config the Environment***********************/
	SYSTEM_Configuration(); 
	//delay_init(72);
	SYSTEM_Init();
	if(DEBUG) printf("Better Call boot successful.\r\n");
	/****************main loop **********************************/
	//delay_ms(100);
	
	while(1)
	{
		send_flag = 0;
		
		//loop_task();
		
		receiveData();
		if(DEBUG) printf("Better Call receive data done.\r\n");
		if(DEBUG)
		{
			printf("Better Call final data:%d%d%d%d %d%d%d%d %d%d%d%d\r\n",
			data_final[0],data_final[1],data_final[2],data_final[3],
			data_final[4],data_final[5],data_final[6],data_final[7],
			data_final[8],data_final[9],data_final[10],data_final[11]);
		}
		
		if(DEBUG) printf("Better Call waiting for sending data.\r\n");
		while(!send_flag);
		if(DEBUG) printf("Better Call sent data.\r\n");
		
		//send data
		//data_final[12]
		
	}
}

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
