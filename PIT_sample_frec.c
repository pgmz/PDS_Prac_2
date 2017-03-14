/*
 * PIT_sample_frec.c
 *
 *  Created on: 13/03/2017
 *      Author: Patricio Gomez
 */

#include "PIT_sample_frec.h"

uint8_t ADC_Sampling_Flag = false;

void PIT0_IRQHandler(void)
{
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
    ADC_Sampling_Flag = true;
}


void PIT_sample_frec_init(){

    pit_config_t pitConfig;
    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);

    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(25U, CLOCK_GetFreq(kCLOCK_BusClk)));

    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

	NVIC_SetPriority(PIT0_IRQn, 5);
	NVIC_EnableIRQ(PIT0_IRQn);

}

void PIT_sample_frec_start(){
    PIT_StartTimer(PIT, kPIT_Chnl_0);

}
