from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields
from socketIO_client import SocketIO

import json
import sys
import time

port = 3000
socket = SocketIO('localhost', port)


debug = True

pipe = RadioInterface.RadioInterface("edison", 1, debug)
pipe.connect_sketch()

mspV1 = Device("msp430g2553", "./doge/radio/mm_msp430_v1.txt")
mspV1.to_s()

nodes = [2,3,4,5,6,7,8,9,10]
node = []

for node_id in nodes:
    #print "Created node ={0}".format(node_id)
    node.append(HardwareNode(mspV1, node_id, pipe))

data = []
while True:
#for i in range (0,5):
    
    timestamp = time.time()
    for node_id in range (0,9):
        rssi = node[node_id].get_rssi(node_id)
        #print "node id={0} rssi={1}".format(nodes[node_id],rssi)
        data.append([int(timestamp),rssi])
    dataJson  = json.dumps(data)
    print "Sending data over socket"
    #print "Sending Socket data = {0}".format(dataJson)
    socket.emit('update',dataJson)
    data = []
    time.sleep(1)    



