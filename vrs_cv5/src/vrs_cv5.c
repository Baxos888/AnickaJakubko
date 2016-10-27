/*
 * vrs_cv5.c
 *
 *  Created on: 18. 10. 2016
 *      Author: jakub a anicka :-*
 */

#include "stm32l1xx.h"
#include "vrs_cv5.h"

void startNVIC() {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn; // nam preru�en� n�jdete v s�bore stm32l1xx.h
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//uloha 2
void startUSART() {
	GPIO_InitTypeDef gpioInitStruc; //RX
	GPIO_InitTypeDef gpioInitStruc2; //TX
	USART_InitTypeDef usartInitStruc;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	gpioInitStruc.GPIO_Mode = GPIO_Mode_AF;
	gpioInitStruc.GPIO_OType = GPIO_OType_PP;
	gpioInitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInitStruc.GPIO_Pin = GPIO_Pin_2;
	gpioInitStruc.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &gpioInitStruc);

	gpioInitStruc2.GPIO_Mode = GPIO_Mode_AF;
	gpioInitStruc2.GPIO_OType = GPIO_OType_PP;
	gpioInitStruc2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInitStruc2.GPIO_Pin = GPIO_Pin_3;
	gpioInitStruc2.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &gpioInitStruc2);

	usartInitStruc.USART_BaudRate = 2 * 9600;
	usartInitStruc.USART_WordLength = USART_WordLength_8b;
	usartInitStruc.USART_StopBits = USART_StopBits_1;
	usartInitStruc.USART_Parity = USART_Parity_No;
	usartInitStruc.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartInitStruc.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &usartInitStruc);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitTypeDef NVIC_InitStructure2;
	NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn; // nam preru�en� n�jdete v s�bore stm32l1xx.h
	NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure2);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);
}

void adc_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* Configure ADCx Channel 2 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	/* Check that HSI oscillator is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
		;
	/* Enable ADC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADCx regular channel8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_96Cycles);
	/* Enable the ADC */
	ADC_Cmd(ADC1, ENABLE);

	/* Wait until the ADC1 is ready */
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET) {
	}
	/* Start ADC Software Conversion */
	ADC_SoftwareStartConv(ADC1);
}

void blikanie(volatile int AD_value) {
	int i = 0;
	int j = 0;
	int y = 0;

	switch (AD_value) {
	case 3900 ... 4000:
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		y = 0;
		break;
	case 2000 ... 2100:
		y = 25000;
		break;
	case 2800 ... 3000:
		y = 50000;
		break;
	case 3400 ... 3600:
		y = 150000;
		break;
	case 3601 ... 3800:
		y = 500000;
		break;
	}
	for (i = 0; i < y; i++) {
		if (i == y - 1)
			GPIO_ResetBits(GPIOA, GPIO_Pin_5); //vypni LED
	}
	for (j = 0; j < y; j++) {
		if (j == y - 1)
			GPIO_SetBits(GPIOA, GPIO_Pin_5); // zapni LED
	}
}

// uloha 1
void task1() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef gpioInitStruc;
	gpioInitStruc.GPIO_Mode = GPIO_Mode_OUT;
	gpioInitStruc.GPIO_OType = GPIO_OType_PP;
	gpioInitStruc.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInitStruc.GPIO_Pin = GPIO_Pin_5;
	gpioInitStruc.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIOA, &gpioInitStruc);

	ADC_SoftwareStartConv(ADC1);
}

void sendString(volatile int uart, volatile int hodnota) {
	int c1, c2, c3, c4, d1, d2, d3;
	double AD_hodnota;
	int dlzka;

	if (uart == 1) {
		char text[6];
		//prevod cisla na char
		AD_hodnota = hodnota * 3.3 / 4096.0;
		d1 = AD_hodnota;
		d2 = (AD_hodnota * 10) - (d1 * 10);
		d3 = (AD_hodnota * 100) - (d1 * 100) - (d2 * 10);
		text[0] = d1 + '0';
		text[1] = '.';
		text[2] = d2 + '0';
		text[3] = d3 + '0';
		text[4] = 'V';
		text[5] = '\n';
		text[6] = '\r';
		dlzka = 7;

		for (int i = 0; i < dlzka; i++) {
			USART_SendData(USART2, (uint8_t) text[i]);
			while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
				;
		}
	} else {
		char text2[7];
		//prevod cisla na char
		c1 = hodnota / 1000;
		c2 = (hodnota - (1000 * c1)) / 100;
		c3 = (hodnota - (1000 * c1 + c2 * 100)) / 10;
		c4 = (hodnota - (1000 * c1 + c2 * 100 + c3 * 10));
		text2[0] = c1 + '0';
		text2[1] = c2 + '0';
		text2[2] = c3 + '0';
		text2[3] = c4 + '0';
		text2[4] = '\n';
		text2[5] = '\r';
		dlzka = 6;

		for (int i = 0; i < dlzka; i++) {
			USART_SendData(USART2, (uint8_t) text2[i]);
			while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
				;
		}
	}
}
