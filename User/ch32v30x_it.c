/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch32v30x_it.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main Interrupt Service Routines.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/
#include "ch32v30x_it.h"

extern volatile uint32_t TimingDelay;

//extern void prvvTIMERExpiredISR( void ) __attribute__((interrupt("WCH-Interrupt-fast")));
//extern void prvvUARTTxReadyISR(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//extern void prvvUARTRxISR(void) __attribute__((interrupt("WCH-Interrupt-fast")));


extern void prvvTIMERExpiredISR( void );
extern void prvvUARTTxReadyISR(void);
extern void prvvUARTRxISR(void);

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void TIM4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
	while (1)
	{
	}
}

/*********************************************************************
 * @fn      SysTick_Handler
 *
 * @brief   SysTick_Handler.
 *
 * @return  none
 */
void SysTick_Handler(void)
{
	if(SysTick->SR == 1)
	{
		SysTick->SR = 0;//clear State flag
		if (TimingDelay != 0x00) TimingDelay--;
	}
}


void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		/* Clear TIM4 Capture Compare1 interrupt pending bit*/
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		prvvTIMERExpiredISR( );
	}
}


void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		prvvUARTRxISR();
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}

	if(USART_GetITStatus(USART1, USART_IT_TXE) == SET)
	{
		prvvUARTTxReadyISR();
		USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}

	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);
		USART_ReceiveData(USART1);
	}
}


