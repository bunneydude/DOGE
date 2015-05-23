/*
 * main.h
 *
 *  Created on: Oct 19, 2014
 *      Author: mabunney
 */

#ifndef MAIN_H_
#define MAIN_H_

#define DOGE_PAYLOAD 32
#define MY_NODE_ID 11


#define NRF_CE 14
#define NRF_CSN 7
#define NRF_IRQ 17

#define LPC_ISP 12
#define LPC_SCL 11
#define LPC_SDA 10

#define LPC_ADC 6
#define LPC_TEMP1 9
#define LPC_TEMP2 1

#define LPC_TP1 16
#define LPC_TP2 15

#define RED_CH 0
#define GREEN_CH 1
#define BLUE_CH 2

void print_nrf_regs();


#endif /* MAIN_H_ */
