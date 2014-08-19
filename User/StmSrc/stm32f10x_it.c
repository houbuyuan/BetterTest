/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TxBufferSize   (countof(TxBuffer) - 1)
#define RxBufferSize   0x20

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
u8 TxBuffer[] = "\n\rUSART Example2: USART-Hyperterminal communication using Interrupt\n\r";
u8 RxBuffer[RxBufferSize];
u8 NbrOfDataToTransfer = TxBufferSize;
u8 NbrOfDataToRead = RxBufferSize;
u8 TxCounter = 0;
u16 RxCounter = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
extern u8 Buffer[32];
extern bool StartToSendData;
extern bool DataSendFinish;

extern int count;
extern int receive_flag;
extern int receive_level;
extern int receive_run;
extern int send_flag;
extern int status;
extern int Tim3_flg;

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  This function handles USB Low Priority interrupts request.
  * @param  None
  * @retval None
  */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
//    USB_Istr();
}

/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */ 
void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		if(status > STATUS_GROUP3_MEMBER4_2)
		{
			receive_run = 0;
			send_flag = 1;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */ 
void TIM3_IRQHandler(void)   							//TIM3 interupt
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)		//check if the TIM3 interupt occur: TIM interupt source
	{
		Tim3_flg = 1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);     //clear TIMx interupt bit: TIM interupt source 
	}
  
  /*******************Send by USB*************************/
	#ifdef USB_HID
//    	if (StartToSendData == TRUE)
// 	{
// 		USB_SendData(0);
// 	}
	#endif /* USB_HID */
  /******************************************************/
  //DispLed();		  //*LED display
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USARTx_IRQHandler(void)
{
	if(USART_GetITStatus(USARTx,USART_IT_RXNE) != RESET) 
	{
		USART_ClearITPendingBit(USARTx,USART_IT_RXNE); 
		RxBuffer[0] = USART_ReceiveData(USARTx);
	}
	USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
}

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval : None
  */
void EXTI9_5_IRQHandler(void)
{
	
  if(EXTI_GetITStatus(EXTI_LINE_KEY_BUTTON) != RESET)
  {
		count = TIM_GetCounter(TIM2);
		TIM_SetCounter(TIM2, 0);
		if(count > 100)
		{
			receive_level = !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
			receive_flag = 0;
		}
    /* Clear the Key Button EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_LINE_KEY_BUTTON);
  }
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
