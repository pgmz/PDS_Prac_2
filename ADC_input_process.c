/*
 * ADC_input_process.c
 *
 *  Created on: 04/03/2017
 *      Author: Patricio Gomez
 */

#include "ADC_input_process.h"

extern SemaphoreHandle_t ADC_Sampling_Flag;
SemaphoreHandle_t ADC_Convertion_Flag;

double ADC_data;

void ADC_input_process_init()
{

	NVIC_SetPriority(ADC0_IRQn, 7);
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

void ADC_Convertion_task(void *pvParameters)
{
	ADC_Convertion_Flag = xSemaphoreCreateBinary();

	adc16_channel_config_t adc16ChannelConfigStruct;
	adc16ChannelConfigStruct.channelNumber = 0U;
	adc16ChannelConfigStruct.enableDifferentialConversion = false;
	adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true;

	ADC16_SetChannelConfig(ADC0, 0, &adc16ChannelConfigStruct);
	for(;;)
	{
		xSemaphoreTake(ADC_Convertion_Flag, portMAX_DELAY);

		DSP_task ();
		DAC_output_task();

		xSemaphoreTake(ADC_Sampling_Flag, portMAX_DELAY);

		ADC16_SetChannelConfig(ADC0, 0, &adc16ChannelConfigStruct);

	}
}

void ADC0_IRQHandler ()
{
	BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(ADC_Convertion_Flag, &pxHigherPriorityTaskWoken);
	ADC_data = ADC_Convert_Volt(ADC16_GetChannelConversionValue(ADC0, 0U));
}

