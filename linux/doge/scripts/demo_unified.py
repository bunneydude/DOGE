from doge.core.DogeHub import *
from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields
import sys
import json
import time

config['debug'] =True
config['debug_test_network'] = True

networkSocket, routingProcessor, rootNode = rp_setup()
plotSocket = plot_setup()

networkNodes = routingProcessor.networkNodes

#get node objects
for node in networkNodes.itervalues():
#   print("Have node {0}".format(node.get_nodeID()))
   node.mask_neighbor(action="unmask")

node10 = networkNodes[10]
node9 = networkNodes[9]

plotData = []

while True:
   #handle any network requests (blocking 1 second wait)
   for _ in range(0,4):
      handle_rp_request(networkSocket, routingProcessor)

   plotData = []
   plotData.append([int(time.time()), node10.get_rssi()])
   print "Send to plot"
   plotSocket.emit('update', json.dumps(plotData))
   time.sleep(1)
   node9.pull("networkConfig")
