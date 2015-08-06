#include "nodeCtrl.h"

#ifdef MSP430
void setup() __attribute__ ((alias("nodeCtrl_init")));
void loop() __attribute__ ((alias("nodeCtrl_entry")));
#endif

// Data to write to radio TX FIFO (60 bytes MAX.)
dogePacket txPacket;
extern appPacket* const txAppPacket;
appPacket* const txAppPacket = (appPacket*)(&((rawPacket*)(&txPacket))->data);
packetAttr txAttr;

// Data to read from radio RX FIFO (60 bytes MAX.)
dogePacket rxPacket;
extern appPacket* const rxAppPacket;
appPacket* const rxAppPacket = (appPacket*)(&((rawPacket*)(&rxPacket))->data);
packetAttr rxAttr;

struct Protocol spiProtocol;
uint8_t sendResponse = 0;
uint8_t hbt_output = 0x1;
uint8_t tempIndex;

void nodeCtrl_init()
{
   /* Initialize HW */
   gpio_init();
   timer_hw_init();
   uart_init(9600);
   radio_init();
   /* Initialize FW */
   Protocol_init(&spiProtocol);
   network_init(NETWORK_DIVISION_DEFAULT);
   dsp_init(5,0);
   memoryMapRegionMethods.gpio_handler = gpio_mm_handler;
   memoryMapRegionMethods.adc_handler = adc_mm_handler;
   memoryMapRegionMethods.static_route_handler = static_route_mm_handler;
   /* Creates static routes for IOTG competition demo grid */
   setup_iotg_demo_grid();
   digital_write(RED_LED, hbt_output); // set the LED on
}

void nodeCtrl_entry()
{
   while (1) {
      uint8_t neighborIndex = 0;
      toggle_led(FALSE);

      //Make sure radio is ready to receive
      while (sending());

      // Turn on the receiver and listen for incoming data. Timeout after 500ms.
      if (reliable_receive(TIMEOUT_500_MS)){
         if(MY_NODE_ID == rxPacket.hdr.dst && MY_NODE_ID == rxPacket.hdr.shDst){ //parse message
            toggle_led(TRUE);
            sendResponse = link_layer_parse_packet(&spiProtocol, &rxPacket, &txPacket);
            //update neighbor table
            network_update(rxPacket.hdr.shSrc, get_rssi(), RADIO_ID_915, NETWORK_ID_0, NEIGHBOR_ENTRY);
            if (sendResponse == TRANSMIT_RESPONSE){
               if (HEADER_TYPE_EQUALS(txPacket.hdr.type, RAW_PACKET)){
                  reliable_transmit();
               }
            }
         }
         else if (MY_NODE_ID == rxPacket.hdr.shDst && MY_NODE_ID != rxPacket.hdr.dst){
            if(network_has_neighbor(rxPacket.hdr.dst, &tempIndex, RADIO_ID_915, FALSE)){
               if (HEADER_TYPE_EQUALS(rxPacket.hdr.type, RAW_PACKET)){
                  toggle_led(TRUE);
                  txAttr.ack = GET_HEADER_TYPE_ACK(rxPacket.hdr.type);
                  txAttr.size = RAW_PACKET_DATA_SIZE(&rxPacket);
                  copy_raw_packet_data((rawPacket*)&txPacket, (rawPacket*)&rxPacket);
                  link_layer_form_packet(&txPacket, &txAttr, GET_HEADER_TYPE(rxPacket.hdr.type),
                        rxPacket.hdr.src, rxPacket.hdr.dst, MY_NODE_ID,
                        network[tempIndex].neighbor.shNodeID);
                  reliable_transmit();
               }
            }
            else if (network_has_route(rxPacket.hdr.dst, &tempIndex, RADIO_ID_915, FALSE)){
               if (HEADER_TYPE_EQUALS(rxPacket.hdr.type, RAW_PACKET)){
                  toggle_led(TRUE);
                  txAttr.ack = GET_HEADER_TYPE_ACK(rxPacket.hdr.type);
                  txAttr.size = RAW_PACKET_DATA_SIZE(&rxPacket);
                  copy_raw_packet_data((rawPacket*)&txPacket, (rawPacket*)&rxPacket);
                  neighborIndex = network[tempIndex].routing.neighborIndex;
                  link_layer_form_packet(&txPacket, &txAttr, GET_HEADER_TYPE(rxPacket.hdr.type), 
                        rxPacket.hdr.src, rxPacket.hdr.dst, MY_NODE_ID, 
                        network[neighborIndex].neighbor.shNodeID);
                  reliable_transmit();
               }
            }
            else
            {
               print_string("ERROR: Attempt to forward packet that does not exist in neighbor or routing tables", NEWLINE);
            }
         }
      }
      else{//end if got packet
         //no packet
      }

      if(dspStatus.counter == 1){ //reset counter and sample temperature sensor
         dspStatus.counter = dspStatus.period;
         dsp_add_sample( analog_read(TEMP_SENSOR) );
      }
   }
}

/*
 * pin named by 0-255 
 * To set multiple pins, specify starting pin, 8-bit mask, then the values.
 * e.g. set pins 20, 22, 23, 24, 26: high, low, low, high, low
 *   CMD = CMD_PIN_OUTPUT
 *      SUBCMD = multiple
 *   start pin = d20
 *   mask = 0b01011101
 *   values = 0b01001 (upper 3 bits are ignored)


   Set just pin 20 high
   CMD = CMD_PIN_OUTPUT
   SUBCMD = single
   pin = d20
   value = 1

   Alt to above: adds a bye, but using a subcmd adds a byte as well
   CMD = CMD_PIN_OUTPUT
   SUBCMD = multiple
   start pin = d20
   mask = 0x1
   values = 0x1

*/
