from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields
from socketIO_client import SocketIO

import json
import sys
import time

port = 3000
socket = SocketIO('localhost', port)

config['debug'] = True
config['debug_test_network'] = True

pipe = RadioInterface.RadioInterface("edison", 1, config['debug'])
pipe.connect_sketch()
root = VirtualNode(1, "Edison")

mspV1 = Device("msp430g2553", "./doge/radio/mm_msp430_v1.txt")
mspV1.to_s()

nodes = [2,3,4,5,6,7,8,9,10]
networkNodes = []

for node_id in nodes:
    #print "Created node ={0}".format(node_id)
    networkNodes.append(HardwareNode(mspV1, node_id, pipe, root, False))
    root.add_neighbor({'shNodeID':networkNodes[-1], 'shLQE':1, 'radioID':0, 'networkID':1})
    networkNodes[-1].load_state()

data = []
while True:
#for i in range (0,5):
    
    timestamp = time.time()
    for node_id in range (0,9):
        rssi = networkNodes[node_id].get_rssi()
        #print "node id={0} rssi={1}".format(nodes[node_id],rssi)
        data.append([int(timestamp),40])#rssi])
    dataJson  = json.dumps(data)
    print "Sending data over socket"
    #print "Sending Socket data = {0}".format(dataJson)
    socket.emit('update',dataJson)
    data = []
    time.sleep(1)    



