from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields

import sys

debug = True

pipe = RadioInterface("edison", 1, debug)
pipe.connect_sketch()

mspV1 = Device("msp430g2553", "./doge/radio/mm_msp430_v1.txt")
mspV1.to_s()

edison = HardwareNode(mspV1, 0, pipe)

neighbors = edison.get_neighbors(0)
neighbor_table_array = edison.get_neighbor_table(0)
routing_table_array = edison.get_routing_table(0)

print "Neighbors={0}".format(neighbors)
print "Neighbor Table Array={0}".format(neighbor_table_array)
print "Routing Table Array={0}".format(routing_table_array)


