/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : Nedelcu Bogdan Sebastian
* Version            : V1.0.0
* Date               : 2024/08/15
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/****************************** readme.md file ************************************
                           MounRiver Studio_Community
                                Version: v1.50
                                
                           Nedelcu Bogdan Sebastian

                     Modbus RTU stack running on CH32V307VCT6
***********************************************************************************            
  At startup you can open a Termite serial communication tool, configured:
 
     19200 bps, 8N1, no handshake
  
  and see some chip info sent with printf through USART1 configured by debug.c:

     SystemClk: 96000000
     Chip type: 30700518
     ChipID: 00000518
   
  After you see the above info, do not forget to close the port, so the RMMS
  be able to open the port ! :)
***********************************************************************************     
  To test Modbus we need RMMS utility (Radzio! Modbus Master Simulator):
  
  https://en.radzio.dxp.pl/modbus-master-simulator/
  
  Use a USB-TTL adapter connected to pins A9(USART1_Tx) and A10(USART1_Rx)
  
  In MENU > Connections > Settings > Protocol - select - Modbus RTU
  
  In MENU > Connections > Settings > Modbus RTU - select -
      Port = COMx, Bitrate = 19200, Parity = NONE, Stop bits = 1
  
  In MENU > File > New - select - Device ID = 1, Holding registers, Length = 99
  
  In MENU > Connections > Connect

               Below are configurations made in different files  
***********************************************************************************
  Line  59 in mb.c, NULL is redefined. Is first defined in stddef.h (line 34).
***********************************************************************************
  Line 345 in mb.c. Modbus_Request_Flag is set, so we know Modbus was readed.
***********************************************************************************
  Line 395 in mb.c. In a known working example of TCP MODBUS running on STM32F407,
      the lines are commented. Maybe it interfere with TCP.
***********************************************************************************
  Line 30 & 36 in port.c __set_PRIMASK from ARM changed to RISC-V.
***********************************************************************************
  Line 41 in port.c. Holds usefull parameters of Modbus registers configuration.
***********************************************************************************
  Line 140 in portserial.c. Added code to test USART1 interrupt working.
     Send 'A' using Termite over serial (38400,8,N) to blink led.
***********************************************************************************/

/*
#include <ch32v30x_gpio.h>
#include <ch32v30x_misc.h>
#include <ch32v30x_rcc.h>
#include <core_riscv.h>
#include <mb.h>
#include <mbport.h>
#include <sys/_stdint.h>
#include <sys/types.h>
#include <system_ch32v30x.h>
#include "main.h"
*/

#include "debug.h"
#include "ch32v30x.h"
#include "main.h"
#include "mbutils.h"
#include "mb.h"

/* Global typedef */

/* Global define */

#define LED_TOGGLE        GPIOA->OUTDR ^= GPIO_Pin_15
#define BLUE_LED_TOGGLE   GPIOB->OUTDR ^= GPIO_Pin_4

/* Global Variable */

volatile uint32_t TimingDelay;

// We use this flag to know when Modbus server is called
volatile uint8_t Modbus_Request_Flag;

u16 usRegHoldingBuf[100+1]; // 0..99 holding registers
u8  usRegCoilBuf[64/8+1];   // 0..63 coils

void writeCoil(uint8_t coil_index, uint8_t state)
{
	uint8_t coil_offset=coil_index/8;
	if (state == 1)
		usRegCoilBuf[coil_offset] |= (1<<(coil_index%8));
	else usRegCoilBuf[coil_offset] &= ~(1<<(coil_index%8));
}

uint8_t getCoil(uint8_t coil_index)
{
	uint8_t coil_byte=usRegCoilBuf[coil_index/8];
	if (coil_byte && (1<<(coil_index%8))) return 1;
	else return 0;
}

void writeHoldingRegister(uint8_t reg_index, uint16_t reg_val)
{
	usRegHoldingBuf[reg_index] = reg_val;
}

uint16_t readHoldingRegister(uint8_t reg_index)
{
	return usRegHoldingBuf[reg_index];
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SysTick_Config(u_int64_t ticks)
{
	SysTick->SR &= ~(1 << 0); //clear State flag
	SysTick->CMP = ticks;
	SysTick->CNT = 0;
	SysTick->CTLR = 0xF;

	NVIC_SetPriority(SysTicK_IRQn, 15);
	NVIC_EnableIRQ(SysTicK_IRQn);
}

int main(void)
{
	uint8_t state;

	SystemCoreClockUpdate();

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	// SysTick counts miliseconds
	SysTick_Config(SystemCoreClock/1000-1);

	// Config GPIO pins used for blue and red leds
	GPIO_Config();

	// Use USART1 to print chip info
	USART_Printf_Init(19200);
	printf("SystemClk: %d\r\n",SystemCoreClock);
	printf("Chip type: %08x\r\n", DBGMCU_GetCHIPID());
	printf("ChipID: %08x\r\n", DBGMCU_GetDEVID());
    // USART1 is redefined below for Modbus communication

	// Config protocol stack in RTU mode for a slave with address 1
	// MB type = MB_RTU
	// Dev ID = 1
	// Port = USART1 (configured in portserial.h line 58, xMBPortSerialInit)
	// Parity = NONE
	eMBInit(MB_RTU, 1, 1, 19200, MB_PAR_NONE);

	// Enable the Modbus Protocol Stack.
	eMBEnable();

	// Reset Modbus request signall flag
	Modbus_Request_Flag = 0;

	// Set state at zero level
	state = 0;

	/*
	// Use this block to test USART1 interrupt
	// In portserial.c uncomemnt lines from 140
	xMBPortSerialInit( 0, 38400, 8, MB_PAR_NONE );
	// Enable the receiver
	vMBPortSerialEnable( TRUE, FALSE );
	// Now block. Any character received should cause an interrupt
	// If char 'Á' received from serial port then led is toggled
	for( ;; );
    */

	while(1)
	{
        // Modbus related function
		eMBPoll();

		// First holding register in RMMS keeps loop counter
		usRegHoldingBuf[1]++;

		// If there is an Modbus event available toggle led
		// mb.c line 345
		if (Modbus_Request_Flag == 1)
		{
			// Reset Modbus request signall flag
			Modbus_Request_Flag = 0;
			LED_TOGGLE;
		}

		// Toggle led once at timeout (100ms). Non blocking. No delay
		if (state == 1)
		{
			// If we have timeout condition
			// deactivate this block and toggle LED
			if (TimingDelay == 0)
			{
				// Next loop the code below will be active
				state = 0;
				LED_TOGGLE;
			}
		}
		else
		{
			// Next loop the code above will be active
			state = 1;
			// Set timeout in milliseconds
			TimingDelay = 100;
		}
	}
	return 0;
}
