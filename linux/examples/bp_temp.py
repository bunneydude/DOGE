import RadioInterface
import time

pipe = RadioInterface.RadioInterface("edison")

pipe.connect_sketch()

frameNumber = 120
tempC = 0
tempF = 0
rawTemp = 0
scaledTemp = 0
MIN_TEMP = 39.0 # = 32 Celsius
MAX_TEMP = 68.0
# 39 raw = 32 F
# 68 raw = 100 F
for frame in range(0, frameNumber):
	rawTemp = pipe.pull("boosterpack", 5, "temperature")
	if(rawTemp != -1):

		tempC = (((rawTemp/255.0)*3.3)-0.5)/0.01	
		tempF = 32 + (9/5.0)*tempC

		scaledTemp = int(((rawTemp - MIN_TEMP)/(MAX_TEMP-MIN_TEMP))*255.0)
		if(scaledTemp < 0):
			scaledTemp = 0
		elif(scaledTemp > 255):
			scaledTemp = 255

		pipe.push("leaf", 2, scaledTemp)
		print("Raw = " + str(rawTemp) + ", scaled = " + str(scaledTemp))
		print("C: " + str(tempC) + ", F: = " + str(tempF))
		print("")
	else:
		print("Timeout for node 5")
	time.sleep(0.5)
