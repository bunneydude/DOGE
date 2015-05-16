from doge.core.DogeHub import *
from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields
from doge.core.DogeHub import connect_cloud
import sys
import json
import time

config['debug'] = True
config['debug_test_network'] = True
config['debug_no_sketch'] = False

networkSocket, routingProcessor, rootNode = rp_setup()
plotSocket = plot_setup()

networkNodes = routingProcessor.networkNodes

#Unmask nodes
rootNode.mask_neighbor(action="unmask")

intelStream = connect_cloud("intel")

#rootNode.mask_neighbor(10)

node10 = networkNodes[10]

node10.add_sensor("rgLED1", "static_route", mmFields.NRF24_NODE_11)
node10.add_sensor("rgbLED", "static_route", mmFields.NRF24_NODE_12)
node10.add_sensor("rgLED2", "static_route", mmFields.NRF24_NODE_13)

leds = ["rgLED1", "rgbLED", "rgLED2"]

plotData = []

rgbValue = 0
switchCounter = 0
ledIndex = 0
led = leds[0]

while True:
   #handle any network requests (blocking 1 second wait)
   for _ in range(0,1):
      handle_rp_request(networkSocket, routingProcessor)

   plotData = []
   timestamp = time.time()

   for node in networkNodes.itervalues():
       if(node is not rootNode): 
	       #plotData.append([int(timestamp), -1*node.get_rssi()])
	       intelStream.push({'id':node.get_nodeID(), 'rssi':-1*node.get_rssi()})
   #print "Send to plot"
   #plotSocket.emit('update', json.dumps(plotData))
   #for point in plotData:
#	   intelStream.push(
   time.sleep(1)
#   node10.push(led, rgbValue)
#   rgbValue = (rgbValue + 30) & 0xff
#   switchCounter += 1
#   if switchCounter > 5:
#      switchCounter = 0
#      ledIndex = (ledIndex + 1) % len(leds)
#      led = leds[ledIndex]
#      print("Push to {0}".format(led))
