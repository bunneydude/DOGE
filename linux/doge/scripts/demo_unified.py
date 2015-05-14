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
node4 = routingProcessor.networkNodes[4]
node5 = routingProcessor.networkNodes[5]

print("Node 4 = {0}".format(node4.get_nodeID()))
print("Node 5 = {0}".format(node5.get_nodeID()))

#check routes for edison
print("Root ID {0}: neighbors = {1}, routes = {2}".format(rootNode.get_nodeID(), rootNode.get_neighbor_table(), rootNode.get_routing_table()))

node4.pull("networkConfig")
rootNode.mask_neighbor(nodeID=4)
node4.pull("networkConfig")

plotData = []

while True:
   #handle any network requests (blocking 1 second wait)
   handle_rp_request(networkSocket, routingProcessor)

   plotData = []
   timestamp = time.time()
   node4.get_rssi()
   plotData.append([int(time.time()), node4.get_rssi()])
   print "Send to plot"
   plotSocket.emit('update', json.dumps(plotData))
   time.sleep(1)
