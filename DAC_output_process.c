/*
 * DAC_output_process.c
 *
 *  Created on: 11/03/2017
 *      Author: Patricio Gomez
 */

#include "DAC_output_process.h"

extern double y_n;


uint16_t DAC_data;

void DAC_output_process_init(){

     dac_config_t dacConfigStruct;

	 DAC_GetDefaultConfig(&dacConfigStruct);
	 DAC_Init(DAC0, &dacConfigStruct);
	 DAC_Enable(DAC0, true);             /* Enable output. */
	 DAC_SetBufferReadPointer(DAC0, 0U);

}

void DAC_output_task(){
		DAC_data = DAC_Convert_Volt(y_n);
		DAC_SetBufferValue(DAC0, 0U, DAC_data);
}
