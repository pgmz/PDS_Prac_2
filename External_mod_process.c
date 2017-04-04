/*
 * External_mod_process.c
 *
 *  Created on: 12/03/2017
 *      Author: Patricio Gomez
 */

#include "External_mod_process.h"

uint32_t PORTC_INT_FLAGS;

Mod_State_type Current_state = R_PARAM;

/*Maquina de estados*/
static Mod_SM_type Mod_SM[4] = {
		{R_PARAM, A_PARAM, 200, R_PARAM_MIN, R_PARAM_MAX, R_PARAM_MIN,&Modify_amplitude, false, false, true},
		{A_PARAM, R_PARAM, 0.5, A_PARAM_MIN, A_PARAM_MAX, A_PARAM_MIN,&Modify_amplitude, true, false, false}
};


/*Donde están los valores que modifican el sistema**/
float *R_param = &Mod_SM[R_PARAM].Modifier;
float *A_param =  &Mod_SM[A_PARAM].Modifier;


void External_mod_process_init(){

	/*Init botones y LEDS**/

	  CLOCK_EnableClock(kCLOCK_PortC);

		port_pin_config_t config =
		{
				kPORT_PullUp,
				kPORT_FastSlewRate,
				kPORT_PassiveFilterDisable,
				kPORT_OpenDrainDisable,
				kPORT_LowDriveStrength,
				kPORT_MuxAsGpio,
				kPORT_UnlockRegister
		};

		gpio_pin_config_t config_gpio =
		{
				kGPIO_DigitalInput
		};

		PORT_SetPinConfig(PORTC, 5U, &config);
		PORT_SetPinConfig(PORTC, 7U, &config);
		PORT_SetPinConfig(PORTC, 0U, &config);
		PORT_SetPinConfig(PORTC, 9U, &config);
		PORT_SetPinConfig(PORTC, 8U, &config);
		PORT_SetPinConfig(PORTC, 1U, &config);


		NVIC_SetPriority(PORTC_IRQn, 7);
		NVIC_EnableIRQ(PORTC_IRQn);


		PORT_SetPinInterruptConfig(PORTC, 5U, kPORT_InterruptRisingEdge);
		GPIO_PinInit(GPIOC, 5U, &config_gpio);

		PORT_SetPinInterruptConfig(PORTC, 7U, kPORT_InterruptRisingEdge);
		GPIO_PinInit(GPIOC, 7U, &config_gpio);

		PORT_SetPinInterruptConfig(PORTC, 0U, kPORT_InterruptRisingEdge);
		GPIO_PinInit(GPIOC, 0U, &config_gpio);

		PORT_SetPinInterruptConfig(PORTC, 9U, kPORT_InterruptRisingEdge);
		GPIO_PinInit(GPIOC, 9U, &config_gpio);

		PORT_SetPinInterruptConfig(PORTC, 8U, kPORT_InterruptRisingEdge);
		GPIO_PinInit(GPIOC, 8U, &config_gpio);

		PORT_SetPinInterruptConfig(PORTC, 1U, kPORT_InterruptRisingEdge);
		GPIO_PinInit(GPIOC, 1U, &config_gpio);

		gpio_pin_config_t config_led =
		{
				kGPIO_DigitalOutput
		};

		CLOCK_EnableClock(kCLOCK_PortB);
		CLOCK_EnableClock(kCLOCK_PortE);

		PORT_SetPinMux(BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN, kPORT_MuxAsGpio);
		GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, &config_led);
		PORT_SetPinMux(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN, kPORT_MuxAsGpio);
		GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, &config_led);
		PORT_SetPinMux(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN, kPORT_MuxAsGpio);
		GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &config_led);

		LED_GREEN_OFF();
		LED_RED_OFF();
		LED_BLUE_OFF();

		GPIO_WritePinOutput(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, Mod_SM[Current_state].led1);
		GPIO_WritePinOutput(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, Mod_SM[Current_state].led2);
		GPIO_WritePinOutput(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, Mod_SM[Current_state].led3);
}

void Modify_amplitude(uint8_t Increment){
	if(Increment){
		if(Mod_SM[Current_state].Modifier >= Mod_SM[Current_state].Modifier_max){
			Mod_SM[Current_state].Modifier = Mod_SM[Current_state].Modifier_max;
		} else {
			Mod_SM[Current_state].Modifier += Mod_SM[Current_state].Modifier_inc;
		}
	} else if(Increment == false){
		if(Mod_SM[Current_state].Modifier <= Mod_SM[Current_state].Modifier_min){
			Mod_SM[Current_state].Modifier = Mod_SM[Current_state].Modifier_min;
		} else {
			Mod_SM[Current_state].Modifier -= Mod_SM[Current_state].Modifier_inc;
		}
	}
}

void PORTC_IRQHandler(){

	/*De acuerdo al boton oprimido cambiar amplitud, estado o frecuencia de muestreo**/

	PORTC_INT_FLAGS = GPIO_GetPinsInterruptFlags(GPIOC);
	PORT_ClearPinsInterruptFlags(PORTC, ~0U);

	/*Cambiar estado y actualizar LEDS**/
	switch(PORTC_INT_FLAGS){
	case 1<<7U:
			Current_state = Mod_SM[Current_state].Mod_Next;
			GPIO_WritePinOutput(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, Mod_SM[Current_state].led1);
			GPIO_WritePinOutput(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, Mod_SM[Current_state].led2);
			GPIO_WritePinOutput(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, Mod_SM[Current_state].led3);
		    break;

    /*Incrementar de acuerdo al estado**/
	case 1<<0U:
			Mod_SM[Current_state].ftpr(true);
			break;

	/*Decrementar de acuerdo al estado**/
	case 1<<9U:
			Mod_SM[Current_state].ftpr(false);
			break;
	}

	PORTC_INT_FLAGS = 0;
}
