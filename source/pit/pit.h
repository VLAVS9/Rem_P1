/*
 * pit.h
 *
 *  Created on: Feb 23, 2024
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */

#ifndef PIT_H_
#define PIT_H_

#include <stdint.h>
#include <fsl_pit.h>

#define PIT_BASEADDR		PIT
#define PIT0_CHANNEL		kPIT_Chnl_0
#define PIT0_IRQ_ID			PIT0_IRQn
#define PIT0_TIME			USEC_TO_COUNT(10000U, PIT_SOURCE_CLOCK)

#define PIT1_CHANNEL		kPIT_Chnl_1
#define PIT1_IRQ_ID			PIT1_IRQn
#define PIT1_TIME			USEC_TO_COUNT(100000U, PIT_SOURCE_CLOCK)

#define PIT2_CHANNEL		kPIT_Chnl_2
#define PIT2_IRQ_ID			PIT2_IRQn
#define PIT2_TIME			USEC_TO_COUNT(1000000U, PIT_SOURCE_CLOCK)

#define PIT3_CHANNEL		kPIT_Chnl_3
#define PIT3_IRQ_ID			PIT3_IRQn
#define PIT3_TIME			USEC_TO_COUNT(20000U, PIT_SOURCE_CLOCK)

#define PIT_SOURCE_CLOCK	CLOCK_GetFreq(kCLOCK_BusClk)

#define DISABLE_PIT0_INTERRUPT() PIT_DisableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable)
#define ENABLE_PIT0_INTERRUPT()	 PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable)

#define DISABLE_PIT3_INTERRUPT() PIT_DisableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable)
#define ENABLE_PIT3_INTERRUPT()	 PIT_EnableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable)

/*! These constants are used to select an specific port in the different API functions. */
typedef enum {

	PIT_0,	/*!< Definition to select PIT0. */
	PIT_1,	/*!< Definition to select PIT1. */
	PIT_2,	/*!< Definition to select PIT2. */
	PIT_3,	/*!< Definition to select PIT3. */

} pit_t;

void PIT_callback_init(pit_t pit, void (*handler)(void));

void PIT_enable(void);

void PIT0_init(void);
void PIT0_config(uint32_t time);
void PIT0_stop(void);
void PIT0_notification_set(void);
void PIT0_notification_turnoff(void);
uint8_t PIT0_is_interrupted(void);

void PIT1_init(void);
void PIT1_config(uint32_t time);
void PIT1_stop(void);

void PIT2_init(void);
void PIT2_config(uint32_t time);
void PIT2_stop(void);

void PIT3_init(void);
void PIT3_config(uint32_t time);
void PIT3_stop(void);

#endif /* PIT_H_ */
