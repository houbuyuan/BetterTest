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

//u16 GetVolt(u16 advalue);

bool StartToSendData = FALSE;
u16 ElapsedMilliseconds=0;	   //system time counter,about every 1ms will add 1 
//u8 Fled = 0;
u8 hour = 0 ;
u8 minute = 0;
u8 second = 0;
u16 millisecond = 0;
//u8 keyValue=0;
//extern u8 keyFlag;
//extern u16 algstatus;

extern uint8_t SensorArray_Data[3][12][64];
uint8_t SendArray[3][12][64]={0};

int flag = 1;
int level = 0;
u16 time1 = 0;
u16 time2 = 0;

extern void Start_TakeMeasurement(void);
extern void SetSlave_Download(void);
extern void Transmit_TM_SCL(void);
extern void Transmit_GetData(void);
/*******************************************************************************
* Function Name  : main
* Description    : the main function
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	uint16_t count = 0;
	RFDataLenth_TypeDef dataLenth;
	int status = 0;
	
	/********Config the Environment***********************/
	SYSTEM_Configuration(); 
	//delay_init(72);

	SYSTEM_Init();
	
	printf("%s  \r\n"," start");
	/****************main loop **********************************/
	//delay_ms(100);
	
	TIM_SetCounter(TIM2, 0);
	status = STATUS_HEADER_HIGH;
	
	while(1)
	{
		printf("%s  \r\n","waiting int");
		
		while(flag)
		{
			//GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_6))));
		}
		
		count = TIM_GetCounter(TIM2);
		TIM_SetCounter(TIM2, 0);
		
		switch(status)
		{
			case STATUS_HEADER_HIGH:
				
				break;
			case STATUS_HEADER_LOW:
				
				break;
		}
		
		dataLenth.HEADER_HIGH = count;
		
		flag = 1;
		

		
		printf("time %d  \r\n",time2);
		printf("level %d  \r\n",level);
		
		
		time1 = time2;
		if(level == 1)  //high level
		{

		}
		else if(level == 0) //low level
		{

		}
		
	}
}

/******************Get the system time***********************/
u16 OS_GetElapsedMilliseconds(void)
{
   return ElapsedMilliseconds;			    //get system run time 
}

/******************System time add 1***********************/
void OS_ElapsedMilliseconds(void)
{
    ElapsedMilliseconds++;	 				//system time ++
}

/*************Set system run time***********************/ 
void SetSysTime()						    //set system run time 
{
	millisecond++;
	if(millisecond == 1000)
	{
		millisecond = 0;
		second++;	
	}
	if(second == 60)
	{
		second = 0;
		minute ++;
	}
	if(minute == 60)
	{
		minute = 0;
		hour ++;
	}
	if(hour == 24)
	{hour = 0;}
}


/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/