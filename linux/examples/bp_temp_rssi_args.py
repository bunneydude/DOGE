import RadioInterface
import time
import sys

pipe = RadioInterface.RadioInterface("edison")

pipe.connect_sketch()
frameNumber = 1
try:
    frameNumber = int(sys.argv[1])
except IndexError:
    frameNumber = 1
#frameNumber = 300 
node = 5
tempCSamples = 0
avgTempC = 0
rawTempSamples = 0
avgRawTemp = 0
tempC = 0
tempF = 0
rawTemp = 0
scaledTemp = 0
rssi = 0
MIN_TEMP = 39.0 # = 32 Celsius
MAX_TEMP = 68.0
# 39 raw = 32 F
# 68 raw = 100 F
for frame in range(0, frameNumber):
	rawTemp = pipe.pull("boosterpack", node, "temperature")
	rssi = pipe.pull("boosterpack", node, "rssi")
	rssi = -((rssi^0xff)+1)
	print("Rssi = " + str(rssi))
	if(rawTemp != -1):

                rawTempSamples = rawTempSamples + 1
                avgRawTemp = avgRawTemp + rawTemp
                tempCSamples = tempCSamples + 1
		tempC = (((rawTemp/255.0)*3.3)-0.5)/0.01
                avgTempC = avgTempC + tempC
		tempF = 32 + (9/5.0)*tempC

		scaledTemp = int(((rawTemp - MIN_TEMP)/(MAX_TEMP-MIN_TEMP))*255.0)
		if(scaledTemp < 0):
			scaledTemp = 0
		elif(scaledTemp > 255):
			scaledTemp = 255

		print("   Raw = " + str(rawTemp) + ", scaled = " + str(scaledTemp))
		print("   C: " + str(tempC) + ", F: = " + str(tempF))
		print("")
	else:
		print("Timeout")
	time.sleep(0.5)
avgTempC = avgTempC / tempCSamples
print "Average TempC = " + str(avgTempC)
avgTempC = round(avgTempC,2)
print "Average TempC (rounded) = " + str(avgTempC)
avgRawTemp = avgRawTemp / (rawTempSamples*1.0)
print "Average RawTemp = " + str(avgRawTemp)
tempC = (((avgRawTemp/255.0)*3.3)-0.5)/0.01
print "New TempC = " + str(tempC)
tempC = round(tempC,2)
print "New TempC (rounded) = " + str(tempC)
