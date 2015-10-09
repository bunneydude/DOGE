from doge.core.DogeHub import *
from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields
from doge.core.DogeHub import connect_cloud
import sys
import json
import time

config['debug'] = False
config['debug_test_network'] = False
config['debug_no_sketch'] = False

temp_demo_nte = [
             {'node_id': 21, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 22, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             #{'node_id': 23, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 24, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 25, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 26, 'mcu_name': 'msp430g2553', 'mm_num': 1},
       ]

config['preset_nte_nodes'] = temp_demo_nte
config['preset_node_tables'] = {}
config['preset_routes'] = {}

networkSocket, routingProcessor, rootNode = rp_setup()
plotSocket = plot_setup()

networkNodes = routingProcessor.networkNodes

#Unmask nodes
rootNode.mask_neighbor(action="unmask")

intelStream = connect_cloud("intel")

#rootNode.mask_neighbor(10)

#setup nodes
for node in networkNodes.values():
  if(node is not rootNode):
     node.add_sensor("temp_sensor", "adc", mmFields.ADC_RESULT_3)
     print("Resetting node {}".format(node.get_nodeID()))
     node.reset()

#############################################################################
#Code to calculate temp in C and scaled temperature for LPC812 tri-color LED 
#plotData = []
#MIN_TEMP=39.0
#MAX_TEMP=68.0
#rawTemp = (node.pull("temp_sensor"))[1][2]
#tempC = round(((((rawTemp/255.0)*3.3)-0.5)/0.01)*2)/2
#scaledTemp = int(((rawTemp - MIN_TEMP)/(MAX_TEMP-MIN_TEMP))*255.0)
#############################################################################

while True:
   #handle any network requests (blocking 1 second wait)
   for _ in range(0,1):
      handle_rp_request(networkSocket, routingProcessor)

   plotData = []
   timestamp = time.time()

   for node in networkNodes.itervalues():
       if(node is not rootNode): 
          #plotData.append([int(timestamp), -1*node.get_rssi()])
          rawTemp = (node.pull("temp_sensor"))[1][2]
          tempC = round(((((rawTemp/255.0)*3.3)-0.5)/0.01)*2)/2
          if (node.get_nodeID() == 21):
             tempC -= 5
          print("node: {}, temp: {}".format(node.get_nodeID(), tempC))
          intelStream.push({'id':node.get_nodeID(), 'rssi':-1*node.get_rssi(), 'temp':tempC})
   #print "Send to plot"
   #plotSocket.emit('update', json.dumps(plotData))
   #for point in plotData:
   #intelStream.push(
   time.sleep(1)
#   node10.push(led, rgbValue)
#   rgbValue = (rgbValue + 30) & 0xff
#   switchCounter += 1
#   if switchCounter > 5:
#      switchCounter = 0
#      ledIndex = (ledIndex + 1) % len(leds)
#      led = leds[ledIndex]
#      print("Push to {0}".format(led))
