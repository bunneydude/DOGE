from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields
from doge.conf.globals import config

import sys

config['debug'] = False
config['debug_test_network'] = False

pipe = RadioInterface.RadioInterface("edison", 1, config['debug'], logLevel=2)
pipe.connect_sketch()

mspV1 = Device("msp430g2553", "./doge/radio/mm_msp430_v1.txt")
mspV1.to_s()

kitchenNode = HardwareNode(mspV1, 3, pipe)
#deskNode = HardwareNode(mspV1, 3, pipe)
kitchenNode.add_sensor("stoveTemp", "adc", mmFields.ADC_RESULT_3)
kitchenNode.add_sensor("n0", "network", 0+4)
kitchenNode.add_sensor("n1", "network", 1+4)
kitchenNode.add_sensor("n2", "network", 2+4)
kitchenNode.add_sensor("n3", "network", 3+4)

kitchenNode.to_s()

print("start")
sensors = ["stoveTemp", "n0", "n1", "n2", "n3"]

print("Unfreeze LQE updates")
kitchenNode.push("networkConfig",1)
for x in range(0,3):
   for sensor in sensors:
      size, data = kitchenNode.pull(sensor)
      if data[0] == 3:
         print("Sensor {0} is {1}".format(sensor, data[2]))
      else:
         print("Error for {0}: {1}".format(sensor, data))

print("Freeze LQE updates")
kitchenNode.push("networkConfig",0)
for x in range(0,3):
   for sensor in sensors:
      size, data = kitchenNode.pull(sensor)
      if data[0] == 3:
         print("Sensor {0} is {1}".format(sensor, data[2]))
      else:
         print("Error for {0}: {1}".format(sensor, data))
