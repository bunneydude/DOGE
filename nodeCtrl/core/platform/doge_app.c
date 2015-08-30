#include "doge_app.h"

lfsrType value = INIT;
// Galois LFSR based on "Maximal Length LFSR Feedback Terms"
// http://users.ece.cmu.edu/~koopman/lfsr/index.html
lfsrType rand_int()
{
   if (value & 0x1){
      value = (value >> 1) ^ feed;
   }else{
      value >>= 1;
   }
   return value;
}

#ifdef __LPC8XX__
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

/* This application repurposes the nodeCtrl LED on GPIO pin TP0 */
void user_application_setup()
{
   /* Configure SCT */
   pwm_set_channel(RED_CH, LED_LOCATION);
   pwm_set_channel(GREEN_CH, LPC_TP2);
   pwm_set_channel(BLUE_CH, LPC_TP1);
   pwm_init();
}

void user_application_loop()
{
}

uint8_t user_application_parse_packet(userAppPacket* message, userAppPacket* response, packetAttr* messageAttr, packetAttr* responseAttr)
{
   uint8_t tempPercent = 0;
   uint8_t redValue = 0;
   uint8_t greenValue = 0;
   uint8_t blueValue = 0;

   responseAttr->ack = FALSE;
   printf("Got %d \n", message->payload[0]);
   tempPercent = (message->payload[0] / 255.0)*100;
   char_to_RGB(tempPercent, &redValue, &greenValue, &blueValue);

   pwm_write(RED_CH, redValue);
   pwm_write(GREEN_CH, greenValue);
   pwm_write(BLUE_CH, blueValue);
   return 0;
}

uint8_t user_application_form_packet(userAppPacket* packet, packetAttr* attr, uint8_t cmd, uint8_t size, uint8_t* bytes)
{
   packet->cmd = cmd;
   switch(packet->cmd){
      case(CMD_USER_APP):
         copy_bytes(packet->payload, bytes, size);
         attr->ack = FALSE;
         attr->size = CMD_USER_APP_DATA_SIZE(size);
         break;
      case(CMD_USER_APP_ACK):
         attr->ack = TRUE;
         attr->size = CMD_USER_APP_ACK_DATA_SIZE;
         break;
      default: //unknown command
         return 0;
   }
   return 1;
}
#elif defined(MSP430)
extern struct dspStatus dspStatus;
void user_application_setup()
{
}

void user_application_loop()
{
   if(dspStatus.counter == 1){ //reset counter and sample temperature sensor
      dspStatus.counter = dspStatus.period;
      dsp_add_sample( analog_read(TEMP_SENSOR) );
   }
}

uint8_t user_application_parse_packet(userAppPacket* message, userAppPacket* response, packetAttr* messageAttr, packetAttr* responseAttr)
{
   return 0;
}

uint8_t user_application_form_packet(userAppPacket* packet, packetAttr* attr, uint8_t cmd, uint8_t size, uint8_t* bytes)
{
   return 0;
}
#elif defined(LINUX)
void user_application_setup()
{
}

void user_application_loop()
{
}

uint8_t user_application_parse_packet(userAppPacket* message, userAppPacket* response, packetAttr* messageAttr, packetAttr* responseAttr)
{
   dogeStatus status = SUCCESS;
   printf("Got %d \n", message->payload[0]);
   if (message->payload[0] == NO_RESPONSE){
      responseAttr->ack = FALSE;
   }else{
      user_application_form_packet(response, responseAttr, CMD_USER_APP_ACK, CMD_USER_APP_ACK_DATA_SIZE, NULL);
   }
   return status;
}

uint8_t user_application_form_packet(userAppPacket* packet, packetAttr* attr, uint8_t cmd, uint8_t size, uint8_t* bytes)
{
   packet->cmd = cmd;
   switch(packet->cmd){
      case(CMD_USER_APP):
         copy_bytes(packet->payload, bytes, size);
         attr->ack = FALSE;
         attr->size = CMD_USER_APP_DATA_SIZE(size);
         break;
      case(CMD_USER_APP_ACK):
         attr->ack = TRUE;
         attr->size = size;
         break;
      default: //unknown command
         return 0;
   }
   return 1;
}
#endif
