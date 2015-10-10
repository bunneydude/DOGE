#include "doge_gpio.h"

void toggle_led(dogeBool init)
{
   static dogeTimer LEDTimer = {0, 0};
   static dogeLEDState state = LED_RESET;
   static uint8_t counter = 0;

   switch(state){
      case(LED_RESET):
         counter = 0;
         digitalWrite(RED_LED, LED_ON_VALUE);
         timer_reset(&LEDTimer);
         if (init){
            state = LED_START_TOGGLE;
         }
         else{
            state = LED_RESET;
            break;
         }
      case(LED_START_TOGGLE):
         digitalWrite(RED_LED, LED_OFF_VALUE);
         timer_init(&LEDTimer, TIMEOUT_100_MS);
				 state = LED_OFF;
         break;
      case(LED_OFF):
         if (timer_expired(&LEDTimer)){
            state = LED_INC_COUNTER;
         }
         else{
            state = LED_OFF;
         }
         break;
      case(LED_INC_COUNTER):
         timer_init(&LEDTimer, TIMEOUT_100_MS);
         digitalWrite(RED_LED, LED_ON_VALUE);
         counter++;
         state = LED_ON;
         break;
      case(LED_ON):
         if (timer_expired(&LEDTimer)){
            if (counter == NUM_BLINKS){
               state = LED_RESET;
            }
            else{
               state = LED_START_TOGGLE;
            }
         }
         else{
            state = LED_ON;
         }
         break;
      default:
         state = LED_RESET;
   }
}
