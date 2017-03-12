/*
 * DAC_output_process.c
 *
 *  Created on: 11/03/2017
 *      Author: Patricio Gomez
 */

#include "DAC_output_process.h"

extern QueueHandle_t DAC_Output_Data;

void DAC_output_process_init(){

     dac_config_t dacConfigStruct;

	 DAC_GetDefaultConfig(&dacConfigStruct);
	 DAC_Init(DAC0, &dacConfigStruct);
	 DAC_Enable(DAC0, true);             /* Enable output. */
	 DAC_SetBufferReadPointer(DAC0, 0U);

}

void DAC_output_task(void *pvParameters){

	uint32_t DAC_Output_Processed;

	for(;;)
	{
		xQueueReceive(DAC_Output_Data, &DAC_Output_Processed, portMAX_DELAY);
		DAC_SetBufferValue(DAC0, 0U, (uint16_t)DAC_Output_Processed);
	}
}
