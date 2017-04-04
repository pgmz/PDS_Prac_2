/*
 * Digital_Signal_Processing.c
 *
 *  Created on: 11/03/2017
 *      Author: Patricio Gomez
 */

#include "Digital_Signal_Processing.h"

float h1_n_low_filter[7] = {
		0.07840464525404556,
		0.17707825519483075,
		0.22014353249171387,
		0.2759015644497544,
		0.22014353249171387,
		0.17707825519483075,
		0.17707825519483075
};

float h2_n_high_filter[7] = {
		-0.08857280384687653,
		-0.20001387909943527,
		-0.13289448474069163,
		0.7755518089951376,
		-0.13289448474069163,
		-0.20001387909943527,
		-0.08857280384687653
};

int n = 0;

float y_n;
float x_n;
float y_n_1[7] = {
		0,0,0,0,0,0,0
};

float y_n_2[7] = {
		0,0,0,0,0,0,0
};
extern float ADC_data;
extern float *Amp_General;
extern float *Amp_Low_Filter;
extern float *Amp_High_Filter;

void DSP_task (){
	/*Valor leído por ADC, es x_n **/
	x_n = ADC_data - 1489;

	/*Modificar amplitud de muestra, de acuerdo a Amp_General**/
	DSP_Amplitude(&x_n, (*Amp_General));
	/*Modificar amplitud de pasa bajas y pasa altas**/
	DSP_Filter(&x_n, &y_n_1[0], &h1_n_low_filter[0], (*Amp_Low_Filter));
	DSP_Filter(&x_n, &y_n_2[0], &h2_n_high_filter[0], (*Amp_High_Filter));
	/*Sumar valores filtro pasa bajas y pasa altas**/
	DSP_Add(&y_n, &y_n_1[0], &y_n_2[0]);
}

void DSP_Amplitude (float * x_n, float Amplitude_factor){
	/*Modificar amplitud*/
	*x_n = Amplitude_factor*(*x_n);
}

void DSP_Filter (float * x_n, float *y_n, const float *h_n, float Amplitude_factor){

	/*Convolución**/
	/* y(n) = sum_from_k=0_to_k=7 x(n)*h(n-k) */
	for(n=0; n<7; n++)
	{
		/*El valor de y(0) es igual a y(1) + y(0)*h(0)
		 *
		 * De modo que siempre se va recorriendo los valores, ya que
		 *
		 * iteración 1:
		 * y(0) = x(0)*h(0)
		 * iteración 2:
		 * y(1) = (x(0)*h(0) de iteración 2) + (x(1)*h(1) de iteración 1)
		 * iteración 3:
		 * y(2) = (x(0)*h(0) de iteración 3) + (x(1)*h(1) de iteración 2 + (x(2)*h(2) de iteración 1)
		 *
		 * etc...
		 *
		 * Entonces, podemos decir que si se va a recorrer el arreglo (en este caso a la izquierda) el primer
		 * valor del arreglo siempre es la próxima salida, y depende de la suma de las últimas 7 salidas.
		 *
		 */
		*(y_n + n) = Amplitude_factor*(*(y_n + (n+1)) + (*x_n)*(h_n[n]));
	}

}

void DSP_Add (float * y_n, float * y_n_1, float *y_n_2){
	/*Valor de salida es suma de filtros en posición 0**/
	*y_n = (*(y_n_1) + *(y_n_2)) + 2048;

	/*Proximo valor de salida es 0**/
	*(y_n_1) = 0;
	*(y_n_2) = 0;
}
