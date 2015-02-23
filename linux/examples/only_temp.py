import RadioInterface
import time

pipe = RadioInterface.RadioInterface("edison")

pipe.connect_sketch()

frameNumber = 30
cloudTemperature = 0

for frame in range(0, frameNumber):
	rssi = pipe.pull("boosterpack", 2, "rssi")
	rssi = -((rssi^0xff)+1)
	print("Rssi = " + str(rssi))
	
	pipe.push("leaf", 2, cloudTemperature)
	
	cloudTemperature = (cloudTemperature + 8) & 0xFF
	time.sleep(0.5)
