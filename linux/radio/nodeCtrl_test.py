import RadioInterface
import Node
import mmFields

pipe = RadioInterface.RadioInterface("edison", True)

mspV1 = Node.Device("msp430g2553", "mm_msp430_v1.txt")
mspV1.to_s()

kitchenNode = Node.HardwareNode(mspV1, 2, pipe)
kitchenNode.add_sensor("stoveTemp", "adc", mmFields.ADC_RESULT_3)

kitchenNode.to_s()

kitchenNode.pull("stoveTemp")
