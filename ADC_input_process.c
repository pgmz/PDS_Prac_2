/*
 * ADC_input_process.c
 *
 *  Created on: 04/03/2017
 *      Author: Patricio Gomez
 */

#include "ADC_input_process.h"

SemaphoreHandle_t ADC_Convertion_Flag;
QueueHandle_t ADC_Convertion_Data;

void ADC_input_process_init()
{

	NVIC_SetPriority(ADC0_IRQn, 5);
	NVIC_EnableIRQ(ADC0_IRQn);

	adc16_config_t adc16ConfigStruct;

	ADC16_GetDefaultConfig(&adc16ConfigStruct);

	adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
	adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;

	ADC16_Init(ADC0, &adc16ConfigStruct);
	ADC16_EnableHardwareTrigger(ADC0, false);

	ADC16_DoAutoCalibration(ADC0);
}

void ADC_Convertion_task(void *pvParameters)
{
	ADC_Convertion_Flag = xSemaphoreCreateBinary();
	ADC_Convertion_Data = xQueueCreate(ADC_QUEUE_LENGTH, ADC_QUEUE_ITEM_SIZE);

	adc16_channel_config_t adc16ChannelConfigStruct;
	adc16ChannelConfigStruct.channelNumber = 0U;
	adc16ChannelConfigStruct.enableDifferentialConversion = false;
	adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true;

	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	ADC16_SetChannelConfig(ADC0, 0, &adc16ChannelConfigStruct);
	for(;;)
	{
		xSemaphoreTake(ADC_Convertion_Flag, portMAX_DELAY);

		vTaskDelayUntil(&xLastWakeTime, 100);

		ADC16_SetChannelConfig(ADC0, 0, &adc16ChannelConfigStruct);

	}
}

void ADC0_IRQHandler ()
{
	BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(ADC_Convertion_Flag, &pxHigherPriorityTaskWoken);
	double data = ADC_Convert_Volt(ADC16_GetChannelConversionValue(ADC0, 0U));
	xQueueSendFromISR(ADC_Convertion_Data, &data, &pxHigherPriorityTaskWoken);
}

