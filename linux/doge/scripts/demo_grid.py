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

#add routes for edison
print("Root ID {0}: neighbors = {1}, routes = {2}".format(rootNode.get_nodeID(), rootNode.get_neighbor_table(), rootNode.get_routing_table()))

#rootNode.add_route(destID=4, shID=5)
#print("Root ID {0}: neighbors = {1}, routes = {2}".format(rootNode.get_nodeID(), rootNode.get_neighbor_table(), rootNode.get_routing_table()))

print("Root ID {0}: Send to node {1} through node {2}".format(rootNode.get_nodeID(), 4, rootNode.get_forward_id(4)))
rootNode.mask_neighbor(nodeID=4)
print("Root ID {0}: Send to node {1} through node {2}".format(rootNode.get_nodeID(), 4, rootNode.get_forward_id(4)))


rp_run(socket, routingProcessor)
