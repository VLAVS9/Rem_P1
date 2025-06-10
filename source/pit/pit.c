/*
 * pit.c
 *
 *  Created on: Feb 23, 2024
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */

#include <pit/pit.h>
#include <bits.h>
#include <fsl_pit.h>

static void (*pit_0_callback)(void) = bit_0;
static void (*pit_1_callback)(void) = bit_0;
static void (*pit_2_callback)(void) = bit_0;
static void (*pit_3_callback)(void) = bit_0;

volatile uint8_t PIT0_Isr_Flag = FALSE;
volatile uint8_t PIT1_Isr_Flag = FALSE;
volatile uint8_t PIT2_Isr_Flag = FALSE;
volatile uint8_t PIT3_Isr_Flag = FALSE;

uint8_t PIT0_notification = FALSE;

void PIT_callback_init(pit_t pit, void (*handler)(void)) {

	if(PIT_0 == pit) {

		pit_0_callback = handler;

	}
	else if(PIT_1 == pit) {

		pit_1_callback = handler;

	}
	else if(PIT_2 == pit) {

		pit_2_callback = handler;

	}
	else if(PIT_3 == pit) {

		pit_3_callback = handler;

	}

}

void PIT0_IRQHandler(void) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT0_CHANNEL, kPIT_TimerFlag);

	if(pit_0_callback) {

		pit_0_callback();

	}

	PIT0_Isr_Flag = true;

	__DSB();

}
void PIT1_IRQHandler(void) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT1_CHANNEL, kPIT_TimerFlag);

	if(pit_1_callback) {

		pit_1_callback();

	}

	PIT1_Isr_Flag = true;

	__DSB();

}

void PIT2_IRQHandler(void) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT2_CHANNEL, kPIT_TimerFlag);

	if(pit_2_callback) {

		pit_2_callback();

	}

	PIT2_Isr_Flag = true;

	__DSB();

}

void PIT3_IRQHandler(void) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT3_CHANNEL, kPIT_TimerFlag);

	if(pit_3_callback) {

		pit_3_callback();

	}

	PIT3_Isr_Flag = true;

	__DSB();

}

void PIT_enable(void) {

	CLOCK_SetSimSafeDivs();
	pit_config_t pitConfig;
	PIT_GetDefaultConfig(&pitConfig);
	PIT_Init(PIT_BASEADDR, &pitConfig);

}

// PIT0.
void PIT0_init(void) {

	PIT_SetTimerPeriod(PIT_BASEADDR, PIT0_CHANNEL, PIT0_TIME);
	PIT_EnableInterrupts(PIT_BASEADDR, PIT0_CHANNEL, kPIT_TimerInterruptEnable);
	EnableIRQ(PIT0_IRQ_ID);
	PIT_StartTimer(PIT_BASEADDR, PIT0_CHANNEL);

}

void PIT0_config(uint32_t time) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT0_CHANNEL, kPIT_TimerFlag);
	PIT_StopTimer(PIT_BASEADDR, PIT0_CHANNEL);
	PIT_SetTimerPeriod(PIT_BASEADDR, PIT0_CHANNEL, USEC_TO_COUNT(time, PIT_SOURCE_CLOCK));
	PIT_StartTimer(PIT_BASEADDR, PIT0_CHANNEL);

}

void PIT0_stop(void) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT0_CHANNEL, kPIT_TimerFlag);
	PIT_StopTimer(PIT_BASEADDR, PIT0_CHANNEL);

}

void PIT0_notification_set(void) {

	PIT0_notification =  TRUE;

}
void PIT0_notification_turnoff(void) {

	PIT0_notification =  FALSE;

}
uint8_t PIT0_is_interrupted(void){
	return PIT0_notification;
}

// PIT1.
void PIT1_init(void) {

	PIT_SetTimerPeriod(PIT_BASEADDR, PIT1_CHANNEL, PIT1_TIME);
	PIT_EnableInterrupts(PIT_BASEADDR, PIT1_CHANNEL, kPIT_TimerInterruptEnable);
	EnableIRQ(PIT1_IRQ_ID);
	PIT_StartTimer(PIT_BASEADDR, PIT1_CHANNEL);

}

void PIT1_config(uint32_t time) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT1_CHANNEL, kPIT_TimerFlag);
	PIT_StopTimer(PIT_BASEADDR, PIT1_CHANNEL);
	PIT_SetTimerPeriod(PIT_BASEADDR, PIT1_CHANNEL, USEC_TO_COUNT(time, PIT_SOURCE_CLOCK));
	PIT_StartTimer(PIT_BASEADDR, PIT1_CHANNEL);

}

void PIT1_stop(void) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT1_CHANNEL, kPIT_TimerFlag);
	PIT_StopTimer(PIT_BASEADDR, PIT1_CHANNEL);

}

// PIT2.
void PIT2_init(void) {

	PIT_SetTimerPeriod(PIT_BASEADDR, PIT2_CHANNEL, PIT2_TIME);
	PIT_EnableInterrupts(PIT_BASEADDR, PIT2_CHANNEL, kPIT_TimerInterruptEnable);
	EnableIRQ(PIT2_IRQ_ID);
	PIT_StartTimer(PIT_BASEADDR, PIT2_CHANNEL);

}

void PIT2_config(uint32_t time) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT2_CHANNEL, kPIT_TimerFlag);
	PIT_StopTimer(PIT_BASEADDR, PIT2_CHANNEL);
	PIT_SetTimerPeriod(PIT_BASEADDR, PIT2_CHANNEL, USEC_TO_COUNT(1000000U * time, PIT_SOURCE_CLOCK));
	PIT_StartTimer(PIT_BASEADDR, PIT2_CHANNEL);

}

void PIT2_stop(void) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT2_CHANNEL, kPIT_TimerFlag);
	PIT_StopTimer(PIT_BASEADDR, PIT2_CHANNEL);

}

// PIT3.
void PIT3_init(void) {

	PIT_SetTimerPeriod(PIT_BASEADDR, PIT3_CHANNEL, PIT3_TIME);
	PIT_EnableInterrupts(PIT_BASEADDR, PIT3_CHANNEL, kPIT_TimerInterruptEnable);
	EnableIRQ(PIT3_IRQ_ID);
	PIT_StartTimer(PIT_BASEADDR, PIT3_CHANNEL);

}

void PIT3_config(uint32_t time) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT3_CHANNEL, kPIT_TimerFlag);
	PIT_StopTimer(PIT_BASEADDR, PIT3_CHANNEL);
	PIT_SetTimerPeriod(PIT_BASEADDR, PIT3_CHANNEL, USEC_TO_COUNT(time, PIT_SOURCE_CLOCK));
	PIT_StartTimer(PIT_BASEADDR, PIT3_CHANNEL);

}

void PIT3_stop(void) {

	PIT_ClearStatusFlags(PIT_BASEADDR, PIT3_CHANNEL, kPIT_TimerFlag);
	PIT_StopTimer(PIT_BASEADDR, PIT3_CHANNEL);

}
