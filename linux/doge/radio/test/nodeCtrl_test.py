from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields

import sys

debug = False

pipe = RadioInterface.RadioInterface("edison", 1, debug)
pipe.connect_sketch()

mspV1 = Device("msp430g2553", "./doge/radio/mm_msp430_v1.txt")
mspV1.to_s()

kitchenNode = HardwareNode(mspV1, 6, pipe)
kitchenNode.add_sensor("stoveTemp", "adc", mmFields.ADC_RESULT_3)
kitchenNode.add_sensor("n1_0", "network", 0)
kitchenNode.add_sensor("n1_1", "network", 1)
kitchenNode.add_sensor("n1_2", "network", 2)
kitchenNode.add_sensor("n1_3", "network", 3)

kitchenNode.to_s()

kitchenNode.pull("stoveTemp")

print("Try to read N0")
print("")

kitchenNode.pull("n1_0")
kitchenNode.pull("n1_1")
kitchenNode.pull("n1_2")
kitchenNode.pull("n1_3")
