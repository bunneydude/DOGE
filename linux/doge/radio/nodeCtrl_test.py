from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields

pipe = RadioInterface.RadioInterface("edison", 1, True)
pipe.connect_sketch()

mspV1 = Device("msp430g2553", "mm_msp430_v1.txt")
mspV1.to_s()

kitchenNode = HardwareNode(mspV1, 6, pipe)
kitchenNode.add_sensor("stoveTemp", "adc", mmFields.ADC_RESULT_3)
kitchenNode.add_sensor("n1_0", "device", 0)
kitchenNode.add_sensor("n1_1", "device", 1)
kitchenNode.add_sensor("n1_2", "device", 2)
kitchenNode.add_sensor("n1_3", "device", 3)

kitchenNode.to_s()

kitchenNode.pull("stoveTemp")

print("Try to read N0")
kitchenNode.pull("n1_0")
kitchenNode.pull("n1_1")
kitchenNode.pull("n1_2")
kitchenNode.pull("n1_3")
