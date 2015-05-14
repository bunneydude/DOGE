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

#get node objects
for node in routingProcessor.networkNodes:
   print("Have node {0}".format(routingProcessor.networkNodes[node].get_nodeID()))

node10 = routingProcessor.networkNodes[10]
node10.to_s()
node10.pull("networkConfig")
rootNode.mask_neighbor()
rootNode.mask_neighbor(nodeID=2, action="unmask")

plotData = []

while True:
   #handle any network requests (blocking 1 second wait)
   handle_rp_request(networkSocket, routingProcessor)

   plotData = []
   plotData.append([int(time.time()), 1])#node10.get_rssi()])
   print "Send to plot"
   plotSocket.emit('update', json.dumps(plotData))
   time.sleep(1)
   node10.pull("networkConfig")
