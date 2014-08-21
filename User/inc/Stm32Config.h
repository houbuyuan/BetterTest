#ifndef __STM32CONFIG_H
#define __STM32CONFIG_H

#define FWLIB_3_5			// or FWLIB_1_0 : File library version
#define EXTERNAL_CLK		// or INTERNAL_CLK

#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_exti.h"
#include "stdio.h"

/*************************DMA/ADC Define**********************/
#define ADC1_DR_Address			((u32)0x4001244C)
#define DMA1_AHB_CLK		    RCC_AHBPeriph_DMA1
#define	ADC1_APB2_CLK				RCC_APB2Periph_ADC1

/**************************USART1*************************/
#define USARTx										USART1
#define USARTx_GPIO								GPIOA
#define USARTx_APB2_CLK						RCC_APB2Periph_USART1
#define USARTx_GPIO_CLK						RCC_APB2Periph_GPIOA
#define USARTx_RxPin							GPIO_Pin_10
#define USARTx_TxPin							GPIO_Pin_9
#define USARTx_IRQn								USART1_IRQn
#define USARTx_IRQHandler					USART1_IRQHandler

/**************************SPI*************************/
#define SPIa                   SPI1
#define SPIa_CLK               RCC_APB2Periph_SPI1
#define SPIa_GPIO              GPIOA
#define SPIa_GPIO_CLK          RCC_APB2Periph_GPIOA
#define SPIa_PIN_SCK           GPIO_Pin_5
#define SPIa_PIN_MISO          GPIO_Pin_6
#define SPIa_PIN_MOSI          GPIO_Pin_7

/****************************KEY**************************/
#define KEY_GPIO              GPIOC
#define KEY_GPIO_CLK					RCC_APB2Periph_GPIOC
#define KEY1_PIN	            GPIO_Pin_5
#define KEY2_PIN	            GPIO_Pin_4
#define KEY1_PRESS()					(!GPIO_ReadInputDataBit(KEY_GPIO, KEY1_PIN))
#define KEY2_PRESS()					(!GPIO_ReadInputDataBit(KEY_GPIO, KEY2_PIN))

#define GPIO_LED                          GPIOC
#define RCC_APB2Periph_GPIO_LED           RCC_APB2Periph_GPIOC

#define GPIO_KEY_BUTTON                   GPIOB
#define RCC_APB2Periph_GPIO_KEY_BUTTON    RCC_APB2Periph_GPIOB
#define GPIO_PIN_KEY_BUTTON               GPIO_Pin_7
#define EXTI_LINE_KEY_BUTTON              EXTI_Line7
#define GPIO_PORT_SOURCE_KEY_BUTTON       GPIO_PortSourceGPIOB
#define GPIO_PIN_SOURCE_KEY_BUTTON        GPIO_PinSource7

/**************************BEEP***************************/
#define BUZZER_GPIO             GPIOC
#define BUZZER_GPIO_CLK					RCC_APB2Periph_GPIOC
#define BUZZER_PIN	            GPIO_Pin_10

/************************Test Piont***********************/
#define	TP_GPIO					GPIOA
#define	TP_GPIO_CLK			RCC_APB2Periph_GPIOA
#define TP12_PIN				GPIO_Pin_5
#define TP11_PIN				GPIO_Pin_4
#define TP10_PIN				GPIO_Pin_3
#define TP9_PIN 				GPIO_Pin_2
#define TP8_PIN 				GPIO_Pin_1
#define TP7_PIN 				GPIO_Pin_0
#define TP6_PIN 				GPIO_Pin_6
#define TP5_PIN 				GPIO_Pin_7

#define	TP_VBAT_GPIO				GPIOB
#define	TP_VBAT_GPIO_CLK		RCC_APB2Periph_GPIOB
#define	TP_VBAT_PIN					GPIO_Pin_0


/***********************Power Control*********************/
#define POWER_GPIO				GPIOC
#define POWER_GPIO_CLK		RCC_APB2Periph_GPIOC
#define POWER_VBAT_PIN		GPIO_Pin_8
#define POWER_VDD_PIN			GPIO_Pin_9
#define	POWER_VBAT_ON()		GPIO_SetBits(POWER_GPIO, POWER_VBAT_PIN)
#define	POWER_VBAT_OFF()	GPIO_ResetBits(POWER_GPIO, POWER_VBAT_PIN)
#define	POWER_VDD_ON()		GPIO_SetBits(POWER_GPIO, POWER_VDD_PIN)
#define	POWER_VDD_OFF()		GPIO_ResetBits(POWER_GPIO, POWER_VDD_PIN)

/*********************Start TP Control********************/
#define START_TP_GPIO				GPIOB
#define START_TP_GPIO_CLK		RCC_APB2Periph_GPIOB
#define START_TP_PIN				GPIO_Pin_15
#define	START_TP_TEST()			GPIO_ResetBits(START_TP_GPIO, START_TP_PIN)
#define	STOP_TP_TEST()			GPIO_SetBits(START_TP_GPIO, START_TP_PIN)

void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void EXTI_Configuration(void);
void UART_Configuration(void);
void TIM2_Configuration(void);
void TIM3_Configuration(void);
void SYSTEM_Configuration(void);
void SYSTEM_Init(void);
void ADC1_Configuration(void);
void DMA_Configuration(void);
void I2C_Configuration(void);
void UART_Task(u16 x,u16 y,u16 z,u8 n);

#endif


