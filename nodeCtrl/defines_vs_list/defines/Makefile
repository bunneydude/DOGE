all:
	gcc -Os main.c nrf24.c -o defs_gcc
	gcc -c -Os -S main.c nrf24.c 

msp:
	msp430-gcc -Os -mmcu=msp430g2553 main.c nrf24.c 
	msp430-gcc -Os -S -mmcu=msp430g2553 main.c nrf24.c 

