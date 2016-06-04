from doge.core.DogeHub import *
from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields
import sys
import json
import time

config['debug'] =True
config['debug_test_network'] = True
config['debug_no_sketch'] = False

networkSocket, routingProcessor, rootNode = rp_setup()
plotSocket = plot_setup()

networkNodes = routingProcessor.networkNodes

#Unmask nodes
rootNode.mask_neighbor(action="unmask")
#for node in networkNodes.itervalues():
#    node.mask_neighbor(action="unmask")

node10 = networkNodes[10]

#NRF24_NODE_11 address is repurposed as SW_RESET
#node10.add_sensor("rgLED1", "static_route", mmFields.NRF24_NODE_11)
node10.add_sensor("rgbLED", "static_route", mmFields.NRF24_NODE_12)
node10.add_sensor("rgLED2", "static_route", mmFields.NRF24_NODE_13)

leds = ["rgbLED", "rgLED2"]

plotData = []

rgbValue = 0
switchCounter = 0
ledIndex = 0
led = leds[0]


for i in networkNodes:
   if "reset" in dir(networkNodes[i]):
      print("Resetting node {}".format(i))
      networkNodes[i].reset()

while True:
   #handle any network requests (blocking 1 second wait)
   for _ in range(0,2):
      handle_rp_request(networkSocket, routingProcessor)

   plotData = []
   timestamp = time.time()
   #rssi = node10.get_rssi()

   #plotData.append([int(timestamp),-1*rssi])
   #print "Send to plot: {0}, {1}".format(int(timestamp), rssi)
   #plotSocket.emit('update', json.dumps(plotData))
   #time.sleep(2)
   print("rgbValue="+str(rgbValue))
   node10.push(led, rgbValue)
   rgbValue = (rgbValue + 30) & 0xff
   switchCounter += 1
   if switchCounter > 5:
      switchCounter = 0
      ledIndex = (ledIndex + 1) % len(leds)
      led = leds[ledIndex]
      print("Push to {0}".format(led))
