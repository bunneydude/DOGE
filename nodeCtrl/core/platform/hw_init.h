#ifndef HW_INIT_H
#define HW_INIT_H
#ifdef __cplusplus
extern "C" {
#endif
static inline void gpio_init() __attribute__ ((always_inline));
static inline void uart_init(int baudRate) __attribute__ ((always_inline));
static inline void radio_init() __attribute__ ((always_inline));
static inline void comparator_init() __attribute__ ((always_inline));
static inline void timer_hw_init() __attribute__ ((always_inline));
#ifdef __cplusplus
}
#endif

#ifdef __LPC8XX__
uint8_t nrfAddress[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
#endif

static void gpio_init()
{
#ifdef __LPC8XX__
   /* Initialize the GPIO block */
   gpioInit();
   /* Configure the switch matrix (setup pins for UART0, SPI, GPIO) */
   SwitchMatrix_Init();
   /* Configure NRF CSN/CE output pins */
   LPC_GPIO_PORT->DIR0 |= (1 << RADIO_NRF_CSN);
   LPC_GPIO_PORT->DIR0 |= (1 << RADIO_NRF_CE);
   /* Set LED pin to output (1 = output, 0 = input) */
   LPC_GPIO_PORT->DIR0 |= (1 << LED_LOCATION);
#elif defined(MSP430)
   /* GPIO pins for UART, SPI are configured by Energia serial/radio libraries */
   pinMode(RED_LED, OUTPUT);
#endif
}

static void uart_init(int baudRate)
{
#ifdef __LPC8XX__
   uart0Init(baudRate);
#elif defined(MSP430)
   Serial.begin(baudRate);
#endif
   print_string("START", NEWLINE);
}

static void radio_init()
{
#ifdef __LPC8XX__
   spiInit(LPC_SPI0, 6, 0);
   nrf24_init();
   nrf24_config(2, MAX_DATA_LENGTH);
   nrf24_tx_address(nrfAddress);
   nrf24_rx_address(nrfAddress);
#elif defined(MSP430)
   // The radio library uses the SPI library internally, this call initializes
   // SPI/CSn and GDO0 lines. Also setup initial address, channel, and TX power.
   Radio.begin(ADDRESS_BROADCAST, CHANNEL_1, POWER_MAX); 
#endif
}

static void comparator_init(){}

static void timer_hw_init()
{
#ifdef __LPC8XX__
   /* Configure the multi-rate timer for 1ms ticks */
   mrtInit(SystemCoreClock/1000);
#elif defined(MSP430)
   /*
   * This setup code is similar to wiring_analog.c analogWrite
   * Differences:
   * 1. Changed TACCR0 to count to MAX TIMER VAL
   * 2. Changed clock source to ACLK instead of SMCLK
   */
   TA0CCR0 = MAX_TIMER_VAL;       // PWM Period
   TA0CCTL1 = OUTMOD_7;           // reset/set
   TA0CCR1 = PWM_DUTY(100);       // PWM duty cycle
   TA0CTL = TASSEL_1 + MC_1 + ANALOG_DIV;       // ACLK, up mode
#endif
}
#endif