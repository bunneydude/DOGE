import IPCBuffer
import os
import Protocol
import time
import json
import sys

if sys.version_info[:3] != (2,7,3):
	print("Needs Python version 2.7.3 for default Galileo and Edison compatibility.")
	sys.exit()

frameNumber = 0

if (len(sys.argv) < 2):
	frameNumber = 5
else:
	# max is 15 so no crazy inputs for you
	frameNumber = int(sys.argv[1]) & 0xF



# setup

proxyCmdBuffer = IPCBuffer.IPCBuffer(3)
proxyRxBuffer = IPCBuffer.IPCBuffer(4)


proxyCmdBuffer.open_sketch()
proxyRxBuffer.open_sketch()

toSend = []
rxBytes = []

target = 1
temperature = 80
rssi = -60
timestamp = 0

WRITE = 2
SREG_TARGET = 7
SREG_PING = 11
PING = 1

for frame in range(0,frameNumber):
	for target in range(2,4):
		# Set target node
		toSend = Protocol.form_packet(cmd=WRITE, addr=SREG_TARGET, data=target)
		for b in toSend:
			proxyCmdBuffer.write(b)

		#get response
		rxBytes = []
		rxBytes.append(ord(proxyRxBuffer.read()))
		rxBytes.append(ord(proxyRxBuffer.read()))

		for i in range(rxBytes[-1]):
			rxBytes.append(ord(proxyRxBuffer.read()))

		rxBytes.append(ord(proxyRxBuffer.read()))

		

#####################
		# Get data
		toSend = Protocol.form_packet(cmd=WRITE, addr=SREG_PING, data=PING)
		for b in toSend:
			proxyCmdBuffer.write(b)

		#get response
		rxBytes = []
		rxBytes.append(ord(proxyRxBuffer.read()))
		rxBytes.append(ord(proxyRxBuffer.read()))

		for i in range(rxBytes[-1]):
			rxBytes.append(ord(proxyRxBuffer.read()))

		rxBytes.append(ord(proxyRxBuffer.read()))

		rssi = -((rxBytes[3]^0xff)+1)
		temperature = rxBytes[4]
		timestamp = time.time()
		if(rxBytes[0] == 4):
			print("\nTimeout for node " + str(target) + "\n")
		else:
			jsonData = [{'timestamp':timestamp,'name':'kitchen','network':'boosterpack','id':target,'data':[{'temperature':temperature,'rssi':rssi}]}]
			print(json.dumps(jsonData, indent=2))
		time.sleep(0.5)
