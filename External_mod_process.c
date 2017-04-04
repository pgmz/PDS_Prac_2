/*
 * External_mod_process.c
 *
 *  Created on: 12/03/2017
 *      Author: Patricio Gomez
 */

#include "External_mod_process.h"

uint32_t PORTC_INT_FLAGS;

Mod_State_type Current_state = Amplitude;

/*Maquina de estados*/
static Mod_SM_type Mod_SM[4] = {
		{Amplitude, LP_amplitude, 1, &Modify_amplitude, false, false, true},
		{LP_amplitude, HP_amplitude, 1, &Modify_amplitude, true, false, false},
		{HP_amplitude, Sampling_frec, 1, &Modify_amplitude, false, true, false},
		{Sampling_frec, Amplitude, 25, &Modify_sampling, false, false, false}
};


/*Donde están los valores que modifican el sistema**/
float *Amp_General = &Mod_SM[Amplitude].Modifier;
float *Amp_Low_Filter =  &Mod_SM[LP_amplitude].Modifier;
float *Amp_High_Filter =  &Mod_SM[HP_amplitude].Modifier;
float *Sampling_period =  &Mod_SM[Sampling_frec].Modifier;

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
		if(Mod_SM[Current_state].Modifier >= 1){
			Mod_SM[Current_state].Modifier = 1;
		} else {
			Mod_SM[Current_state].Modifier += 0.1;
		}
	} else if(Increment == false){
		if(Mod_SM[Current_state].Modifier <= 0){
			Mod_SM[Current_state].Modifier = 0;
		} else {
			Mod_SM[Current_state].Modifier -= 0.1;
		}
	}
}

void Modify_sampling(uint8_t Increment){
	if(Increment == false){
		if(Mod_SM[Current_state].Modifier == 100){
			return;
		} else {
			Mod_SM[Current_state].Modifier += 1;
		}
	} else if(Increment){
		if(Mod_SM[Current_state].Modifier == 15){
			return;
		} else {
			Mod_SM[Current_state].Modifier -= 1;
		}
	}

    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(Mod_SM[Current_state].Modifier, CLOCK_GetFreq(kCLOCK_BusClk)));
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
