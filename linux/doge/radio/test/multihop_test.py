from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields

import sys

debug = False

pipe = RadioInterface.RadioInterface("edison", 1, debug, logLevel=2)
pipe.connect_sketch()

mspV1 = Device("msp430g2553", "./doge/radio/mm_msp430_v1.txt")
mspV1.to_s()

kitchenNode = HardwareNode(device=mspV1, nodeID=3, pipe=pipe)
kitchenNode.add_sensor("stoveTemp", "adc", mmFields.ADC_RESULT_3)
kitchenNode.add_sensor("n1_0", "network", 0)
kitchenNode.add_sensor("n1_1", "network", 1)
kitchenNode.add_sensor("n1_2", "network", 2)
kitchenNode.add_sensor("n1_3", "network", 3)

kitchenNode.to_s()

for x in range(0,1):
   kitchenNode.pull("stoveTemp")
