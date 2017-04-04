/*
 * ADC_input_process.c
 *
 *  Created on: 04/03/2017
 *      Author: Patricio Gomez
 */

#include "ADC_input_process.h"

extern uint8_t ADC_Sampling_Flag;
uint8_t ADC_Convertion_Flag = false;

float ADC_data;

void ADC_input_process_init()
{

	/*Init ADC**/
	NVIC_SetPriority(ADC0_IRQn, 5);
	NVIC_EnableIRQ(ADC0_IRQn);

	adc16_config_t adc16ConfigStruct;

	ADC16_GetDefaultConfig(&adc16ConfigStruct);

	adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
	adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
	adc16ConfigStruct.clockDivider = kADC16_ClockDivider1;

	ADC16_Init(ADC0, &adc16ConfigStruct);
	ADC16_EnableHardwareTrigger(ADC0, false);

	ADC16_DoAutoCalibration(ADC0);
}

void ADC_Convertion_task()
{

	/* Init canal de ADC**/
	adc16_channel_config_t adc16ChannelConfigStruct;
	adc16ChannelConfigStruct.channelNumber = 0U;
	adc16ChannelConfigStruct.enableDifferentialConversion = false;
	adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true;

	for(;;)
	{

		/*Mientras no haya pasado el periodo de muestreo, detenerse**/
		//while(ADC_Sampling_Flag == false);
		//ADC_Sampling_Flag = false;

		/*Empezar conversión y esperar a que termine**/
		ADC16_SetChannelConfig(ADC0, 0, &adc16ChannelConfigStruct);
		while(ADC_Convertion_Flag == false);
		ADC_Convertion_Flag = false;

		/*Procesar y sacar por DAC**/
		DSP_task();
		DAC_output_task();

	}
}

void ADC0_IRQHandler ()
{
	ADC_Convertion_Flag = true;
	ADC_data = (ADC16_GetChannelConversionValue(ADC0, 0U));
}

