/**************************************************************************/
/*!
    @file     main.c

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, K. Townsend (microBuilder.eu)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**************************************************************************/
#include <stdio.h>
#include "LPC8xx.h"
#include "main.h"
#include "./gpio/gpio.h"
#include "mrt.h"
#include "uart.h"
#include "spi.h"
#include "./nrf24/nrf24.h"
#include "./nrf24/nRF24L01.h"
#include "lpc_type.h"
#include "./protocol/protocol.h"
#include "pwm.h"

#if defined(__CODE_RED)
#include <cr_section_macros.h>
#include <NXP/crp.h>
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;
#endif

#define LED_LOCATION    (2)

/* This define should be enabled if you want to      */
/* maintain an SWD/debug connection to the LPC810,   */
/* but it will prevent you from having access to the */
/* LED on the LPC810 Mini Board, which is on the     */
/* SWDIO pin (PIO0_2).                               */
// #define USE_SWD
void SwitchMatrix_Init()
{
	/* Enable SWM clock */
	//LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7); already done in systeminit

	/* Pin Assign 8 bit Configuration */
	/* U0_TXD */
	/* U0_RXD */
	LPC_SWM->PINASSIGN0 = 0xffff0004UL;

	/* SPI0_SCK */
	LPC_SWM->PINASSIGN3 = 0x08ffffffUL;

	/* SPI0_MOSI */
	/* SPI0_MISO */
	/* SPI0_SSEL */
	//LPC_SWM->PINASSIGN4 = 0xff070d03UL;
	LPC_SWM->PINASSIGN4 = 0xffff0d03UL;

	/* Pin Assign 1 bit Configuration */
	/* SWDIO */
	/* RESET */
	LPC_SWM->PINENABLE0 = 0xffffffbfUL;
}

void configurePins()
{
	/* Enable SWM clock */
	//  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 7);  // this is already done in SystemInit()

	/* Pin Assign 8 bit Configuration */
	/* U0_TXD */
	/* U0_RXD */
	LPC_SWM->PINASSIGN0 = 0xffff0004UL;

	/* Pin Assign 1 bit Configuration */
#if !defined(USE_SWD)
	/* Pin setup generated via Switch Matrix Tool
       ------------------------------------------------
       PIO0_5 = RESET
       PIO0_4 = U0_TXD
       PIO0_3 = GPIO            - Disables SWDCLK
       PIO0_2 = GPIO (User LED) - Disables SWDIO
       PIO0_1 = GPIO
       PIO0_0 = U0_RXD
       ------------------------------------------------
       NOTE: SWD is disabled to free GPIO pins!
       ------------------------------------------------ */
	LPC_SWM->PINENABLE0 = 0xffffffbfUL;
#else
	/* Pin setup generated via Switch Matrix Tool
       ------------------------------------------------
       PIO0_5 = RESET
       PIO0_4 = U0_TXD
       PIO0_3 = SWDCLK
       PIO0_2 = SWDIO
       PIO0_1 = GPIO
       PIO0_0 = U0_RXD
       ------------------------------------------------
       NOTE: LED on PIO0_2 unavailable due to SWDIO!
       ------------------------------------------------ */
	LPC_SWM->PINENABLE0 = 0xffffffb3UL;
#endif
}

void nrf24_ce_digitalWrite(uint8_t state){
	gpioSetValue(0, NRF_CE, state);
}

void nrf24_csn_digitalWrite(uint8_t state){
	gpioSetValue(0, NRF_CSN, state);
}

uint8_t spi_transfer(uint8_t tx, uint8_t send_eot)
{
	spiTransfer(LPC_SPI0, tx, send_eot);
	return LPC_SPI0->RXDAT;
}

uint8_t rxData[DOGE_PAYLOAD];
uint8_t txData[DOGE_PAYLOAD] = {95, 96, 97, 98, 99, 100, 101};
uint8_t nrfAddress[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};

struct Protocol spiProtocol;

void char_to_RGB(uint8_t input, uint8_t* red, uint8_t* green, uint8_t* blue){

	if(input < 20){
		//R = [255, 0]
		//G = 0
		//B = 255
		*red = ( ( (uint32_t)(20 - input) )/20.0)*100;
		*green = 0;
		*blue = 100;

	}else if( (20 <= input) && (input < 40)){
		//R = 0
		//G = [0,255]
		//B = 255

		*red = 0;
		*green = ( ( (uint32_t)(input - 20) )/20.0)*100;
		*blue = 100;

	}else if( (40 <= input) && (input < 60)){
		//R = 0
		//G = 255
		//B = [255, 0]

		*red = 0;
		*green = 100;
		*blue = ( ( (uint32_t)(60 - input) )/20.0)*100;

	}else if( (60 <= input) && (input < 80)){
		//R = [0, 255]
		//G = 255
		//B = 0

		*red = ( ( (uint32_t)(input - 60) )/20.0)*100;
		*green = 100;
		*blue = 0;

	}else{
		//R = 255
		//G = [255, 0]
		//B = 0

		*red = 100;
		*green = ( ( (uint32_t)(100 - input) )/20.0)*100;
		*blue = 0;

	}
}


int main(void)
{
	/* Initialize the GPIO block */
	gpioInit();

	/* Initialize the UART0 block for printf output */
	//uart0Init(115200);
	uart0Init(9600);
	/* Configure the multi-rate timer for 1ms ticks */
	mrtInit(__SYSTEM_CLOCK/1000);

	/* Configure the switch matrix (setup pins for UART0 and GPIO) */
	//configurePins();
	spiInit(LPC_SPI0, 6, 0);
	SwitchMatrix_Init(); //uart & spi

	LPC_GPIO_PORT->DIR0 |= (1 << NRF_CSN);
	LPC_GPIO_PORT->DIR0 |= (1 << NRF_CE);

	//LPC_GPIO_PORT->DIR0 |= (1 << LPC_SCL);
	//LPC_GPIO_PORT->DIR0 |= (1 << LPC_SDA);
	//LPC_GPIO_PORT->CLR0 = (1 << LPC_SDA);
	//LPC_GPIO_PORT->CLR0 = (1 << LPC_SCL);

	nrf24_init();

	// Set the LED pin to output (1 = output, 0 = input)
#if !defined(USE_SWD)
	LPC_GPIO_PORT->DIR0 |= (1 << LED_LOCATION);
#endif


	mrtDelay(3000);
	printf("I");

	nrf24_config(2,DOGE_PAYLOAD);
	nrf24_tx_address(nrfAddress);
	nrf24_rx_address(nrfAddress);

	uint8_t hbtState = 0;
	//uint8_t nodeID = 0x2;

	Protocol_init(&spiProtocol);


	printf("S");

	//pwm_set_channel(RED_CH, LED_LOCATION);
	//pwm_set_channel(GREEN_CH, LPC_TP2);
	//pwm_set_channel(BLUE_CH, LPC_TP1);
	//pwm_init();

	//uint8_t tempPercent = 0;
	//uint8_t redValue = 0;
	//uint8_t greenValue = 0;
	//uint8_t blueValue = 0;
uint8_t rtCount = 0;

	gpioSetValue(0, LED_LOCATION, hbtState ^= 0x1);
	mrtDelay(500);
	gpioSetValue(0, LED_LOCATION, hbtState ^= 0x1);
	mrtDelay(500);
	gpioSetValue(0, LED_LOCATION, hbtState ^= 0x1);
	mrtDelay(500);
	gpioSetValue(0, LED_LOCATION, hbtState ^= 0x1);
	mrtDelay(500);
	gpioSetValue(0, LED_LOCATION, hbtState ^= 0x1);
	mrtDelay(500);
	gpioSetValue(0, LED_LOCATION, hbtState ^= 0x1);
	mrtDelay(500);

	printf("DOGE node online");
	while(1)
	{
		//test code
		while(!nrf24_dataReady()); //TODO replace w/ IRQ and interrupt handler
		//nrf24_getData(rxData);
		gpioSetValue(0, LED_LOCATION, hbtState ^= 0x1);// Toggle heartbeat LED
		mrtDelay(1);
		//print_nrf_regs();
		nrf24_send(txData); //to root node at address 0x1
		while(nrf24_isSending());
		nrf24_powerUpRx(); //done transmitting, set back to rx mode
		txData[0]++;
		txData[6]++;
		rtCount = nrf24_retransmissionCount();
		if(rtCount > 0){
			printf("RT: %d\n\r", rtCount);
		}
		mrtDelay(1000);


		/*
		while(!nrf24_dataReady()); //TODO replace w/ IRQ and interrupt handler
		nrf24_getData(rxData);

		if(rxData[1] == nodeID){//process packet
			//gpioSetValue(0, LED_LOCATION, hbtState ^= 0x1);// Toggle heartbeat LED

			//update radio info
			spiProtocol.dataRegisters[SREG_RSSI] = 0;

			//parse message
			sendResponse = Protocol_parse_packet(&spiProtocol, &rxData[2], &txData[2]);


			if(sendResponse == 1){	//send reply
				txData[0] = nodeID;
				txData[1] = rxData[0];

				//send reply
				nrf24_send(txData); //to root node at address 0x1
				while(nrf24_isSending());

				nrf24_powerUpRx(); //done transmitting, set back to rx mode

				spiProtocol.dataRegisters[SREG_TEMPERATURE]++;
				printf("tx0: %d\r\n", txData[0]);
				printf("tx1: %d\r\n", txData[1]);
				printf("tx2: %d\r\n", txData[2]);
				printf("tx3: %d\r\n", txData[3]);
				printf("tx4: %d\r\n", txData[4]);
				printf("tx5: %d\r\n", txData[5]);
				printf("tx6: %d\r\n", txData[6]);
			}

			printf("resp: %d\r\n\r\n", sendResponse);
			printf("rx0: %d", rxData[0]);
			printf("rx1: %d", rxData[1]);
			printf("rx2: %d", rxData[2]);
			printf("rx3: %d", rxData[3]);
			printf("rx4: %d", rxData[4]);
			printf("rx5: %d", rxData[5]);
			printf("rx6: %d", rxData[6]);


			tempPercent = ((uint32_t)spiProtocol.dataRegisters[SREG_TEMPERATURE] / 255.0)*100;
			char_to_RGB(tempPercent, &redValue, &greenValue, &blueValue);

			pwm_write(RED_CH, redValue);
			pwm_write(GREEN_CH, greenValue);
			pwm_write(BLUE_CH, blueValue);

			printf("T: %d, P: %d ", spiProtocol.dataRegisters[SREG_TEMPERATURE], tempPercent);

			//Update settings
			//TODO, channel, power, nodeID
		}else{//else ignore packet; TODO possible routing
			printf("ID %d\r\n\r\n", rxData[1]);
		}
		 */

	}//end while(1)
}//end main()

void print_nrf_regs(){
	uint8_t temp=0;
	nrf24_readRegister(RF_CH, &temp,1);
	printf("RF_CH: %d\n\r", temp);

	nrf24_readRegister(RX_PW_P0, &temp,1);
	printf("RX_PW_P0: %d\n\r", temp);

	nrf24_readRegister(RX_PW_P1, &temp,1);
	printf("RX_PW_P1: %d\n\r", temp);

	nrf24_readRegister(RX_PW_P2, &temp, 1);
	printf("RX_PW_P2: %d\n\r", temp);

	nrf24_readRegister(RX_PW_P3, &temp, 1);
	printf("RX_PW_P3: %d\n\r", temp);

	nrf24_readRegister(RX_PW_P4, &temp, 1);
	printf("RX_PW_P4: %d\n\r", temp);

	nrf24_readRegister(RX_PW_P5, &temp, 1);
	printf("RX_PW_P5: %d\n\r", temp);

	nrf24_readRegister(RF_SETUP, &temp, 1);
	printf("RF_SETUP: %d\n\r", temp);

	nrf24_readRegister(CONFIG, &temp, 1);
	printf("CONFIG: %d\n\r", temp);

	nrf24_readRegister(EN_AA, &temp, 1);
	printf("EN_AA: %d\n\r", temp);

	nrf24_readRegister(EN_RXADDR, &temp, 1);
	printf("EN_RXADDR: %d\n\r", temp);

	nrf24_readRegister(SETUP_RETR, &temp, 1);
	printf("SETUP_RETR: %d\n\r", temp);

	nrf24_readRegister(DYNPD, &temp, 1);
	printf("DYNPD: %d\n\r", temp);
}
