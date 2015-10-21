from doge.core.DogeHub import *
from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields
from doge.core.DogeHub import connect_cloud
import sys
import json
import time
import requests
import logging
import re
import os

os.environ["TZ"] = "America/Los_Angeles"

config['debug'] = False
config['debug_test_network'] = False
config['debug_no_sketch'] = False

temp_demo_nte = [
             {'node_id': 2, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 3, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 4, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 5, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 6, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             #{'node_id': 23, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             #{'node_id': 24, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             #{'node_id': 25, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             #{'node_id': 26, 'mcu_name': 'msp430g2553', 'mm_num': 1},
       ]

config['preset_nte_nodes'] = temp_demo_nte
config['preset_node_tables'] = {}
config['preset_routes'] = {}

networkSocket, routingProcessor, rootNode = rp_setup()
plotSocket = plot_setup()

log = logging.getLogger("demo_temp")
formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
formatter.converter = time.localtime
handler = logging.FileHandler("demo_temp.log")
handler.setFormatter(formatter)
log.addHandler(handler)
log.setLevel(logging.DEBUG)
log.info("START")

networkNodes = routingProcessor.networkNodes

#Unmask nodes
rootNode.mask_neighbor(action="unmask")

intelStream = connect_cloud("intel")

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
           size, data = node.pull("temp_sensor")
           if (size == 0):
               print("Skipping node {}".format(node.get_nodeID()))
               continue
           rawTemp = data[2]
           tempC = round(((((rawTemp/255.0)*3.3)-0.5)/0.01)*2)/2
           print("node: {}, temp: {}".format(node.get_nodeID(), tempC))
           try:
               #intelStream.push({'id':node.get_nodeID(), 'rssi':-1*node.get_rssi()})
               intelStream.push({'id':node.get_nodeID(), 'temperature':tempC})
           except (requests.exceptions.ConnectTimeout, requests.exceptions.SSLError) as ce:
               log.warning("WARNING: {}".format(ce))
           except requests.exceptions.HTTPError as he:
               log.warning("{}".format(he.response))
               if (he.response.status_code == 429):
                   m = re.match(r".*Limit will reset in (-*\d+) seconds.*", he.response.content)
                   if (m is not None):
                       delay = int(m.group(1))
                       log.warning("Server rate limiting detected...")
                       #delay for 30 seconds if negative
                       if (delay <= 0):
                           delay = 30
                       log.warning("Sleeping for {} seconds".format(delay))
                       time.sleep(delay)
           except Exception as e:
               log.warning("{}".format(type(e)))
               log.warning("{}\n".format(e))
   #print "Send to plot"
   #plotSocket.emit('update', json.dumps(plotData))
   #for point in plotData:
   #intelStream.push(
   #time.sleep(1)
#   node10.push(led, rgbValue)
#   rgbValue = (rgbValue + 30) & 0xff
#   switchCounter += 1
#   if switchCounter > 5:
#      switchCounter = 0
#      ledIndex = (ledIndex + 1) % len(leds)
#      led = leds[ledIndex]
#      print("Push to {0}".format(led))
