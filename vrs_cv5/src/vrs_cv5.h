/*
 * vrs_cv5.h
 *
 *  Created on: 18. 10. 2016
 *      Author: jakub a anicka :-*
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_


void startNVIC();
void adc_init(void);
void startUSART();
void blikanie();
void task1();
void sendString(volatile int uart, volatile int hodnota);

#endif /* VRS_CV5_H_ */
