import RadioInterface
import time
from DogeHub import connect_cloud

pipe = RadioInterface.RadioInterface("edison")

pipe.connect_sketch()

frameNumber = 1 
node = 5 
tempC = 0
tempF = 0
rawTemp = 0
scaledTemp = 0
rssi = 0
MIN_TEMP = 39.0 # = 32 Celsius
MAX_TEMP = 68.0
# 39 raw = 32 F
# 68 raw = 100 F

cloudStream = connect_cloud("sparkfun")

for frame in range(0, frameNumber):
	rawTemp = pipe.pull("boosterpack", node, "temperature")
	rssi = pipe.pull("boosterpack", node, "rssi")
	rssi = -((rssi^0xff)+1)
	print("Rssi = " + str(rssi))
	if(rawTemp != -1):

		tempC = (((rawTemp/255.0)*3.3)-0.5)/0.01	
		tempF = 32 + (9/5.0)*tempC

		scaledTemp = int(((rawTemp - MIN_TEMP)/(MAX_TEMP-MIN_TEMP))*255.0)
		if(scaledTemp < 0):
			scaledTemp = 0
		elif(scaledTemp > 255):
			scaledTemp = 255

                timestamp = time.time()

		print("   Raw = " + str(rawTemp) + ", scaled = " + str(scaledTemp))
		print("   C: " + str(tempC) + ", F: = " + str(tempF))
                print("   Time = " + str(timestamp))
		print("")

                jsonData = {'time':timestamp,'location':'indoors','network':'boosterpack','id':node,'temperature':tempC,'rssi':rssi}
                cloudStream.push(jsonData)
	else:
		print("Timeout")
	time.sleep(0.5)
