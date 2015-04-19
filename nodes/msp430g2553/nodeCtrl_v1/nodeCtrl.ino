#include <stdint.h>
#include <memory_map.h>
#include <network.h>
#include <protocol.h>
#include <neighbor-config.h>
#include <neighbor.h>
#include <gpio.h>
#include <msp430_gpio.h>
#include <adc.h>
#include <SPI.h>
#include <AIR430BoostFCC.h>
#include <dsp.h>
// -----------------------------------------------------------------------------
/**
 *  Global data
 */
#define DATA_LENGTH 7
// Data to write to radio TX FIFO (60 bytes MAX.)
unsigned char txData[DATA_LENGTH];    

// Data to read from radio RX FIFO (60 bytes MAX.)
unsigned char rxData[DATA_LENGTH] = {10,11,12,13,14,15,16};

#define MY_NODE_ID 0x6
#define NODECTRL_VERSION 1

struct Protocol spiProtocol;
uint8_t sendResponse = 0;
uint8_t hbt_output = 0x1;
char printBuffer[10];
uint8_t returnData = 0;
uint8_t i =0;
uint8_t rawADC = 0;
uint8_t tempIndex;
union networkEntry tempEntry;
// -----------------------------------------------------------------------------
// Main example

void print_packet(uint8_t* buf){
  uint8_t i = 0;
  Serial.println("Packet");
    Serial.print("   ["); 
  for(i=0; i<DATA_LENGTH-1; i++){ 
//    sprintf(printBuffer, "%p", &buf[i]);
//    Serial.print(printBuffer); Serial.print(", ");
    Serial.print(buf[i]); Serial.print(", ");
  }
    Serial.print(buf[DATA_LENGTH-1]); Serial.println("]");
}

void setup()
{ 
  Protocol_init(&spiProtocol);
  network_init(NETWORK_DIVISION_DEFAULT);
  dsp_init(5,0);
  memoryMapRegionMethods.gpio_handler = gpio_mm_handler;
  memoryMapRegionMethods.adc_handler = adc_mm_handler; 
 
  
//  spiProtocol.dataRegisters = &(memoryMap[0].u8[0]);
  
   Serial.begin(9600);
  
  // The radio library uses the SPI library internally, this call initializes
  // SPI/CSn and GDO0 lines. Also setup initial address, channel, and TX power.
  Radio.begin(0x02, CHANNEL_1, POWER_MAX); 

  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, hbt_output);   // set the LED on
  Serial.print("DOGE node online. ID: "); Serial.print(MY_NODE_ID); Serial.print(", Version: "); Serial.print(NODECTRL_VERSION); Serial.println(".");  
  Serial.print("Device byte base = "); Serial.print(MM_DEVICE_BASE); Serial.print(", size = "); Serial.println(MM_DEVICE_SIZE);
  Serial.print("Network byte base = "); Serial.print(MM_NETWORK_BASE); Serial.print(", size = "); Serial.println(MM_NETWORK_SIZE);
  Serial.print("GPIO byte base = "); Serial.print(MM_GPIO_BASE); Serial.print(", size = "); Serial.println(MM_GPIO_SIZE);
  Serial.print("ADC byte base = "); Serial.print(MM_ADC_BASE); Serial.print(", size = "); Serial.println(MM_ADC_SIZE);
  Serial.print("UART byte base = "); Serial.print(MM_UART_BASE); Serial.print(", size = "); Serial.println(MM_UART_SIZE);
  Serial.print("DSP byte base = "); Serial.print(MM_DSP_BASE); Serial.print(", size = "); Serial.println(MM_DSP_SIZE);
  
  /*
  Serial.println("");

  
  Serial.print("sizeof(memoryMap) = "); Serial.println(sizeof(memoryMap));
  Serial.print("sizeof(memoryMapEntry) = "); Serial.println(sizeof(memoryMapEntry));  
  Serial.print("memoryMap[X] = "); Serial.println(MM_PHYSICAL_MAX/4);
  
  
  Serial.println("\n Main memory regions");
  sprintf(printBuffer, "%p", &(memoryMap[MM_PHYSICAL_BAR/4].u8[MM_PHYSICAL_BAR%4]));
  Serial.print("Physical bar byte base = "); Serial.println(MM_PHYSICAL_BAR);
  Serial.print("   &memmap["); Serial.print((MM_PHYSICAL_BAR/4)); Serial.print("]["); Serial.print((MM_PHYSICAL_BAR % 4)); Serial.print("]: "); Serial.println(printBuffer);

  sprintf(printBuffer, "%p", &(memoryMap[MM_PHYSICAL_MAX/4].u8[MM_PHYSICAL_MAX%4]));
  Serial.print("Physical max byte base = "); Serial.println(MM_PHYSICAL_MAX);
  Serial.print("   &memmap["); Serial.print((MM_PHYSICAL_MAX/4)); Serial.print("]["); Serial.print((MM_PHYSICAL_MAX % 4)); Serial.print("]: "); Serial.println(printBuffer); 
 
  sprintf(printBuffer, "%p", &(memoryMap[MM_FUNCTION_BAR/4].u8[MM_FUNCTION_BAR%4]));
  Serial.print("Function bar byte base = "); Serial.println(MM_FUNCTION_BAR);
  Serial.print("   &memmap["); Serial.print((MM_FUNCTION_BAR/4)); Serial.print("]["); Serial.print((MM_FUNCTION_BAR % 4)); Serial.print("]: "); Serial.println(printBuffer);

  sprintf(printBuffer, "%p", &(memoryMap[MM_FUNCTION_MAX/4].u8[MM_FUNCTION_MAX%4]));
  Serial.print("Function max byte base = "); Serial.println(MM_FUNCTION_MAX);
  Serial.print("   &memmap["); Serial.print((MM_FUNCTION_MAX/4)); Serial.print("]["); Serial.print((MM_FUNCTION_MAX % 4)); Serial.print("]: "); Serial.println(printBuffer); 


  Serial.println("\n Peripheral memory spaces");
  sprintf(printBuffer, "%p", &(memoryMap[MM_DEVICE_BASE/4].u8[MM_DEVICE_BASE%4]));
  Serial.print("Device byte base = "); Serial.print(MM_DEVICE_BASE); Serial.print(", size = "); Serial.println(MM_DEVICE_SIZE);
  Serial.print("   &memmap["); Serial.print((MM_DEVICE_BASE/4)); Serial.print("]["); Serial.print((MM_DEVICE_BASE % 4)); Serial.print("]: "); Serial.println(printBuffer);
  
  sprintf(printBuffer, "%p", &(memoryMap[MM_NETWORK_BASE/4].u8[MM_NETWORK_BASE%4]));
  Serial.print("Network byte base = "); Serial.print(MM_NETWORK_BASE); Serial.print(", size = "); Serial.println(MM_NETWORK_SIZE);
  Serial.print("   &memmap["); Serial.print((MM_NETWORK_BASE/4)); Serial.print("]["); Serial.print((MM_NETWORK_BASE % 4)); Serial.print("]: "); Serial.println(printBuffer);

  sprintf(printBuffer, "%p", &(memoryMap[MM_GPIO_BASE/4].u8[MM_GPIO_BASE%4]));
  Serial.print("GPIO byte base = "); Serial.print(MM_GPIO_BASE); Serial.print(", size = "); Serial.println(MM_GPIO_SIZE);
  Serial.print("   &memmap["); Serial.print((MM_GPIO_BASE/4)); Serial.print("]["); Serial.print((MM_GPIO_BASE % 4)); Serial.print("]: "); Serial.println(printBuffer);

  sprintf(printBuffer, "%p", &(memoryMap[MM_ADC_BASE/4].u8[MM_ADC_BASE%4]));
  Serial.print("ADC byte base = "); Serial.print(MM_ADC_BASE); Serial.print(", size = "); Serial.println(MM_ADC_SIZE);
  Serial.print("   &memmap["); Serial.print((MM_ADC_BASE/4)); Serial.print("]["); Serial.print((MM_ADC_BASE % 4)); Serial.print("]: "); Serial.println(printBuffer);

  sprintf(printBuffer, "%p", &(memoryMap[MM_UART_BASE/4].u8[MM_UART_BASE%4]));
  Serial.print("UART byte base = "); Serial.print(MM_UART_BASE); Serial.print(", size = "); Serial.println(MM_UART_SIZE);
  Serial.print("   &memmap["); Serial.print((MM_UART_BASE/4)); Serial.print("]["); Serial.print((MM_UART_BASE % 4)); Serial.print("]: "); Serial.println(printBuffer);

  sprintf(printBuffer, "%p", &(memoryMap[MM_DSP_BASE/4].u8[MM_DSP_BASE%4]));
  Serial.print("DSP byte base = "); Serial.print(MM_DSP_BASE); Serial.print(", size = "); Serial.println(MM_DSP_SIZE);
  Serial.print("   &memmap["); Serial.print((MM_DSP_BASE/4)); Serial.print("]["); Serial.print((MM_DSP_BASE % 4)); Serial.print("]: "); Serial.println(printBuffer);
digitalWrite(RED_LED, 0x1);
Serial.println("\n Check physical addresses");

 sprintf(printBuffer, "%p", &(network[0]) );
  Serial.print("&network[0] = "); Serial.println(printBuffer);

 sprintf(printBuffer, "%p", &(network[1]) );
  Serial.print("&network[1] = "); Serial.println(printBuffer);
  
 sprintf(printBuffer, "%p", &(network[MAX_NETWORK_ENTRIES-1]) );
  Serial.print("&network[MAX_NETWORK_ENTRIES-1] = "); Serial.println(printBuffer);  
  
  union networkEntry nEntry;
  union networkEntry nEntry2;
  union networkEntry rEntry;
  union networkEntry rEntry2;
  
  nEntry.neighbor.shNodeID = 0x8823;
  nEntry.neighbor.shLQE = 0x9ab;
  nEntry.neighbor.radioID = 0x2;
  nEntry.neighbor.networkID = 0x3;

  nEntry2.neighbor.shNodeID = 0x9943;
  nEntry2.neighbor.shLQE = 0xab8;
  nEntry2.neighbor.radioID = 0x1;
  nEntry2.neighbor.networkID = 0x2;  
  
  rEntry.routing.mhNodeID = 0x1234;
  rEntry.routing.mhLQE = 0x56;
  rEntry.routing.neighborIndex = 0x78;

  rEntry2.routing.mhNodeID = 0x9abc;
  rEntry2.routing.mhLQE = 0xde;
  rEntry2.routing.neighborIndex = 0xf1;
  
  network_insert(&nEntry, NEIGHBOR_ENTRY);
  network_insert(&nEntry2, NEIGHBOR_ENTRY); 
 nEntry.neighbor.shNodeID++; 

network_insert(&rEntry, ROUTING_ENTRY);    
network_insert(&rEntry2, ROUTING_ENTRY);    
network_insert(&rEntry, ROUTING_ENTRY);    
network_insert(&rEntry2, ROUTING_ENTRY);    
network_insert(&rEntry, ROUTING_ENTRY);    
network_insert(&rEntry2, ROUTING_ENTRY);    

  network_insert(&nEntry, NEIGHBOR_ENTRY);
  network_insert(&nEntry2, NEIGHBOR_ENTRY); 
    network_insert(&nEntry, NEIGHBOR_ENTRY);
  network_insert(&nEntry2, NEIGHBOR_ENTRY); 

for(i=0; i<MM_NETWORK_SIZE; i++){
  sprintf(printBuffer, "%p", &(spiProtocol.dataRegisters[i]));
  Serial.print("&spiProtocol.dataRegisters["); Serial.print(i); Serial.print("] = ("); Serial.print(printBuffer); Serial.print(", 0x"); Serial.print(spiProtocol.dataRegisters[i],HEX); Serial.println(")");
}

Serial.print("N1.shNodeID = "); Serial.println(nEntry.neighbor.shNodeID);
Serial.print("N1.shLQE = "); Serial.println(nEntry.neighbor.shLQE);
Serial.print("N1.radioID = "); Serial.println(nEntry.neighbor.radioID);
Serial.print("N1.networkID = "); Serial.println(nEntry.neighbor.networkID);
Serial.print("Number neighbors = "); Serial.print(networkTable.numberEntries[NEIGHBOR_ENTRY]); Serial.print(", number routes = "); Serial.println(networkTable.numberEntries[ROUTING_ENTRY]);

digitalWrite(RED_LED, 0x0);
delay(250);
digitalWrite(RED_LED, 0x1);
delay(250);
digitalWrite(RED_LED, 0x0);

*/

 
 
 
 /*
i = 0xff;
 returnData = (*memoryMapRegionMethods.gpio_handler)(0, 0x2, &i, ~(1<<6));
  Serial.print("Set p1.6 high: ("); Serial.print(returnData); Serial.print(", "); Serial.print(i); Serial.println(")");  

i = 0xff;
 returnData = (*memoryMapRegionMethods.gpio_handler)(0, 0x3, &i, ~(1<<6));
  Serial.print("Set p1.6 output: ("); Serial.print(returnData); Serial.print(", "); Serial.print(i); Serial.println(")");    
*/

i=0;
sendResponse = 1;
}
void loop()
{  
/*
  rawADC = (uint8_t)((analogRead(A5) >> 2)&0xFF);
  Serial.print("A5 native: "); Serial.println(rawADC);
  rawADC = 0;
  (*memoryMapRegionMethods.adc_handler)(1, ADC_RESULT_5, &rawADC, 0);
  Serial.print("A5 mm: "); Serial.println(rawADC);
  
    rawADC = 0;
    check_mm_space(1, 0x39, &rawADC, 0);
  Serial.print("A5 full: "); Serial.println(rawADC);
  */    
  //i=0xff;
//  returnData = (*memoryMapRegionMethods.gpio_handler)(0, GPIO_0_TOGGLE, &i, ~(1<<0)); //toggles P1.0
  //Make sure radio is ready
  while (Radio.busy());
  
  // Turn on the receiver and listen for incoming data. Timeout after 1 seconds.
  if (Radio.receiverOn(rxData, sizeof(rxData), 1000) > 0){
    if(rxData[1] == MY_NODE_ID){
      digitalWrite(RED_LED, hbt_output ^= 0x1);

      //parse message
      sendResponse = Protocol_parse_packet(&spiProtocol, &rxData[2], &txData[2]);
      //Protocol_form_packet(&txData[2], CMD_ACK, 4, i++); //as debug, always send ACK w/ increasing data value
      txData[0] = MY_NODE_ID;
      txData[1] = rxData[0];      

      if(sendResponse == 1){
        Radio.transmit(0x1, txData, DATA_LENGTH); //to root node at address 0x1
      }
      
    }
    
    //check if this is a new neighbor
    if(network_has_neighbor(rxData[0], &tempIndex)){
      network[tempIndex].neighbor.shLQE = 0xFF & Radio.getRssi();
      Serial.print("Neighbor "); Serial.print(rxData[0]); Serial.print(", RSSI 0x"); Serial.println(network[tempIndex].neighbor.shLQE, HEX);
    }else{ //make a new entry
      tempEntry.neighbor.shNodeID = rxData[0];
      tempEntry.neighbor.shLQE = 0xFF & Radio.getRssi();
      tempEntry.neighbor.radioID = 0x1;
      tempEntry.neighbor.networkID = 0x0;
      
      returnData = network_insert(&tempEntry, NEIGHBOR_ENTRY); 
      if(returnData == 1){
        Serial.print("Added neighbor: "); Serial.println(rxData[0]);
      }else{
        Serial.print("No room for neighbor: "); Serial.println(rxData[0]);
      }
    }
  }else{//end if got packet
  //we timed out, decrement counter...this is lame I know
    if(dspStatus.counter > 1){dspStatus.counter--;}
  }
  
  if(dspStatus.counter == 1){ //reset counter and sample temperature sensor
    dspStatus.counter = dspStatus.period;
    dsp_add_sample( analogRead(A3) );    
  }
  
}//end main
