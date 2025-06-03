/*
 * gpio.c
 *
 *  Created on: Feb 9, 2025
 *      Authors:
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */

#include <gpio/gpio.h>
#include <bits.h>
#include <fsl_gpio.h>
#include <fsl_port.h>
#include <pin_mux.h>
#include <MK66F18.h>

volatile static gpio_interrupt_flags_t g_intr_status_flag = {bit_0};

static void (*gpio_A_callback)(uint32_t flags) = bit_0;
static void (*gpio_B_callback)(uint32_t flags) = bit_0;
static void (*gpio_C_callback)(uint32_t flags) = bit_0;
static void (*gpio_D_callback)(uint32_t flags) = bit_0;

void GPIO_callback_init(gpio_name_t gpio, void (*handler)(uint32_t flags)) {

	if(GPIO_A == gpio) {

		gpio_A_callback = handler;

	}
	else if(GPIO_B == gpio) {

		gpio_B_callback = handler;

	}
	else if(GPIO_C == gpio) {

		gpio_C_callback = handler;

	}
	else if(GPIO_D == gpio) {

		gpio_D_callback = handler;

	}

}

void GPIO_clear_irq_status(gpio_name_t gpio) {

	if(GPIO_A == gpio) {

		g_intr_status_flag.flag_port_a = false;

	}
	else if(GPIO_B == gpio) {

		g_intr_status_flag.flag_port_b = false;

	}
	else if(GPIO_C == gpio) {

		g_intr_status_flag.flag_port_c = false;

	}
	else if(GPIO_D == gpio) {

		g_intr_status_flag.flag_port_d = false;

	}

}

uint8_t GPIO_get_irq_status(gpio_name_t gpio) {

	uint8_t status = bit_0;

	if(GPIO_A == gpio) {

		status = g_intr_status_flag.flag_port_a;

	}
	else if(GPIO_B == gpio) {

		status = g_intr_status_flag.flag_port_b;

	}
	else if(GPIO_C == gpio) {

		status = g_intr_status_flag.flag_port_c;

	}
	else if(GPIO_D == gpio) {

		status = g_intr_status_flag.flag_port_d;

	}

	return(status);

}

void PORTA_IRQHandler(void) {

	uint32_t irq_status = bit_0;

	irq_status = GPIO_PortGetInterruptFlags(GPIOA);

	if(gpio_A_callback) {

		gpio_A_callback(irq_status);

	}

	GPIO_PortClearInterruptFlags(GPIOA, irq_status);

}

void PORTB_IRQHandler(void) {

	uint32_t irq_status = bit_0;

	irq_status = GPIO_PortGetInterruptFlags(GPIOB);

	if(gpio_B_callback) {

		gpio_B_callback(irq_status);

	}

	GPIO_PortClearInterruptFlags(GPIOB, irq_status);

}

void PORTC_IRQHandler(void) {

	uint32_t irq_status = bit_0;

	irq_status = GPIO_PortGetInterruptFlags(GPIOC);

	if(gpio_C_callback) {

		gpio_C_callback(irq_status);

	}

	GPIO_PortClearInterruptFlags(GPIOC, irq_status);

}

void PORTD_IRQHandler(void) {

	uint32_t irq_status = bit_0;

	irq_status = GPIO_PortGetInterruptFlags(GPIOD);

	if(gpio_D_callback) {

		gpio_D_callback(irq_status);

	}

	GPIO_PortClearInterruptFlags(GPIOD, irq_status);

}

// Input.
const gpio_pin_config_t gpio_input_config = {

		kGPIO_DigitalInput,
		bit_0,

};

// Output.
const gpio_pin_config_t gpio_output_config = {

		kGPIO_DigitalOutput,
		bit_1,

};

// Button.
const port_pin_config_t button_config = {

		kPORT_PullUp,					/* Internal pull-up resistor is enabled. */
		kPORT_FastSlewRate,				/* Fast slew rate is configured. */
		kPORT_PassiveFilterEnable,		/* Passive filter is enable. */
		kPORT_OpenDrainDisable,			/* Open drain is disabled. */
		kPORT_HighDriveStrength,		/* High drive strength is configured. */
		kPORT_MuxAsGpio,				/* Pin is configured as PT. */
		kPORT_UnlockRegister			/* Pin Control Register fields [15:0] are not locked. */

};

void GPIO_init(void) {

	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);

}

void LED_init(void) {

	// Red LED.
	PORT_SetPinMux(PORTC, bit_9, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOC, bit_9, &gpio_output_config);

	// Blue LED.
	PORT_SetPinMux(PORTA, bit_11, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOA, bit_11, &gpio_output_config);

	// Green LED.
	PORT_SetPinMux(PORTE, bit_6, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOE, bit_6, &gpio_output_config);

}

void SW_init(void) {

	// SW2.
	GPIO_PinInit(GPIOD, bit_11, &gpio_input_config);
	PORT_SetPinConfig(PORTD, bit_11, &button_config);
	PORT_SetPinInterruptConfig(PORTD, bit_11, kPORT_InterruptFallingEdge);

	// SW3.
	GPIO_PinInit(GPIOA, bit_10, &gpio_input_config);
	PORT_SetPinConfig(PORTA, bit_10, &button_config);
	PORT_SetPinInterruptConfig(PORTA, bit_10, kPORT_InterruptFallingEdge);

}

void UART0_port_init(void) {

	// UART0_RX.
	PORT_SetPinMux(PORTB, bit_16, kPORT_MuxAlt3);

	// UART0_TX.
	PORT_SetPinMux(PORTB, bit_17, kPORT_MuxAlt3);

	SIM->SOPT5 = ((SIM->SOPT5 & (~(SIM_SOPT5_UART0TXSRC_MASK))) | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX));

}
