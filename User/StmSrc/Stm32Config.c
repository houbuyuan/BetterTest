#include "Stm32Config.h"


/* Private variables ---------------------------------------------------------*/
vu16 AD_Value[9];

/* Private function prototypes -----------------------------------------------*/

ErrorStatus HSEStartUpStatus;

/*******************************************************************************
* Function Name  : SYSTEM_Configuration
* Description    : Configures the SYSTEM .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SYSTEM_Configuration(void)
{
	/* System Clocks Configuration */
	RCC_Configuration();

	/* NVIC configuration */
	NVIC_Configuration();

	/* Configure the GPIO ports */
	GPIO_Configuration();

	/* Configure the GPIO ports */
	UART_Configuration();
	
	EXTI_Configuration();

 	TIM2_Configuration();

  TIM3_Configuration();
}

void SYSTEM_Init(void)
{

}

/********************************************************************************/
void UART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	/* USART1 configuration ------------------------------------------------------*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
#ifdef FWLIB_1_0
	USART_InitStructure.USART_Clock = USART_Clock_Disable;
	USART_InitStructure.USART_CPOL = USART_CPOL_Low;
	USART_InitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_InitStructure.USART_LastBit = USART_LastBit_Disable;
#endif /* FWLib 1.0 */
	
	/* Configure the USARTx */
	USART_Init(USARTx, &USART_InitStructure);

	/* Enable the USART Receive interrupt: this interrupt is generated when the
	USART1 receive data register is not empty */
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

	/* Enable USARTx */
	USART_Cmd(USARTx, ENABLE);
}
/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
	
#if defined(FWLIB_1_0)	
	/* Enable the USART3 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/////////////Config TIM2///////////////////////////////////////
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel; 	//通道TIM3,作为系统计时
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 		//副优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//	///////////////Config TIM3///////////////////////////////////////
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel; 	//通道TIM3,
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 		//副优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
#elif defined(FWLIB_3_5)

  /* Enable the EXTI9_5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/////////////Config TIM2///////////////////////////////////////
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 	//通道TIM3,作为系统计时
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 		//副优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	///////////////Config TIM3///////////////////////////////////////
//	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//只能调用一次
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 	//通道TIM3,
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 		//副优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
#endif /* FWLib 1.0 */
}

void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
  /* Connect Key Button EXTI Line to Key Button GPIO Pin */
  GPIO_EXTILineConfig(GPIO_PORT_SOURCE_KEY_BUTTON, GPIO_PIN_SOURCE_KEY_BUTTON);

  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_KEY_BUTTON;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  //EXTI_GenerateSWInterrupt(EXTI_LINE_KEY_BUTTON);
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
#if defined(INTERNAL_CLK)
	// Use internal RC clock
	RCC_DeInit();//将外设 RCC寄存器重设为缺省值
	//  使用内部RC晶振
	RCC_HSICmd(ENABLE);//使能内部时钟;
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)//等待HSI使能成功
	{
	}
	RCC_HCLKConfig(RCC_SYSCLK_Div1);//选择HCLK时钟源为系统时钟SYYSCLOCK
	RCC_PCLK1Config(RCC_HCLK_Div2);//APB1时钟为2M 
	RCC_PCLK2Config(RCC_HCLK_Div1);//APB2时钟为2M
	
	/* Flash 2 wait state */
	FLASH_SetLatency(FLASH_Latency_2);
	/* Enable Prefetch Buffer */
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	
	//设置 PLL 时钟源及倍频系数
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);                
	//使能或者失能 PLL,这个参数可以取：ENABLE或者DISABLE
	RCC_PLLCmd(ENABLE);//如果PLL被用于系统时钟,那么它不能被失能
	//等待指定的 RCC 标志位设置成功 等待PLL初始化成功
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}

	//设置系统时钟（SYSCLK） 设置PLL为系统时钟源
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  
	//等待PLL成功用作于系统时钟的时钟源
	//  0x00：HSI 作为系统时钟 
	//  0x04：HSE作为系统时钟 
	//  0x08：PLL作为系统时钟  
	while(RCC_GetSYSCLKSource() != 0x08)
	{
	}
#elif defined(EXTERNAL_CLK)
	#if defined(FWLIB_1_0)
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS)
	{
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
	#elif defined(FWLIB_3_5)						
	SystemInit();
	#endif /* Internal Clock */
#endif /* FWLib 1.0 */
	
	/* Enable GPIOA、GPIOB /TIM3 /I2C2/USART1 clocks */
	RCC_APB2PeriphClockCmd(USARTx_GPIO_CLK | RCC_APB2Periph_GPIO_KEY_BUTTON
		 | RCC_APB2Periph_GPIOC | USARTx_APB2_CLK | RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(DMA1_AHB_CLK, ENABLE);
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE); //使能TIM2&3和USART3模块时钟
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_I2C1,ENABLE); //使能TIM2&3和USART3模块时钟
}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
 	/////////////////////////USART1/////////////////////////	
	/* Configure USARTx Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USARTx_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USARTx_GPIO, &GPIO_InitStructure);
	
 	/* Configure USART1 Rx (PA10) as input floating */
 	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	//GPIO_Init(GPIOA, &GPIO_InitStructure);	

	/*********************TIMER_OUT***********************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*********************toPhone_Tx***********************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*********************KEY****************************/
	GPIO_InitStructure.GPIO_Pin  = KEY1_PIN | KEY2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_Init(KEY_GPIO, &GPIO_InitStructure);

  /* Configure Key Button GPIO Pin as input floating (Key Button EXTI Line) */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_KEY_BUTTON;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIO_KEY_BUTTON, &GPIO_InitStructure);
//	/////////////////////////Key/////////////////////////
//	/*Configure PA0 as input for key*/				   //上拉输入
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure ;

	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period=(5000 - 1);//65535;   		//自动重装载寄存器的值	50ms定时
	TIM_TimeBaseStructure.TIM_Prescaler=(720 - 1);// 0;   		//时钟预分频数	 100kHZ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//采样分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	//计数方式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update); 						//清除溢出中断标志
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2, ENABLE);										//开启时钟
}
void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure ;

	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period=(1000 - 1);//65535;    	//自动重装载寄存器的值	1ms定时
	TIM_TimeBaseStructure.TIM_Prescaler=(72 - 1);// 0;   	//时钟预分频数	1000kHZ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//采样分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down;	//计数方式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update); 						//清除溢出中断标志
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE); 									//开启时钟
}

