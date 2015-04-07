from RadioInterface import *
import Node
from mm_registers import *

pipe = RadioInterface("edison", True)

rawTemp = 0
node = Node.Device("msp430", "mm_msp430_v1.txt")
nodeID = 2

rawTemp = pipe.pull("boosterpack", nodeID, "temperature")
print("Got raw temp: {0}".format(rawTemp))

pipe.proxy_send(WRITE, address=node.address("gpio", GPIO_0_DIR), payload=2)
