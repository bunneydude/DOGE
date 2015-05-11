from doge.core.DogeHub import *
from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields
import sys


star_nte = [
#             {'node_id': 2, 'mcu_name': 'msp430g2553', 'mm_num': 1},
#             {'node_id': 3, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 4, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 5, 'mcu_name': 'msp430g2553', 'mm_num': 1},
            ]

config['preset_nte_nodes'] = star_nte

config['debug'] = True
config['debug_test_network'] = True

print("In debug mode? {0}".format(config['debug']))

socket, routingProcessor, rootNode = rp_setup()

node4 = routingProcessor.networkNodes[4]
node5 = routingProcessor.networkNodes[5]

print("Node 4 = {0}".format(node4.get_nodeID()))
print("Node 5 = {0}".format(node5.get_nodeID()))

#add routes for edison
print("Root ID {0}: neighbors = {1}, routes = {2}".format(rootNode.get_nodeID(), rootNode.get_neighbor_table(), rootNode.get_routing_table()))

node4.pull("networkConfig")

rootNode.mask_neighbor(nodeID=4)
node4.pull("networkConfig")


rp_run(socket, routingProcessor)
