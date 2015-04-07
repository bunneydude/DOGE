#import os, sys, inspect
import sys
import time
from collections import defaultdict

#cmd_folder = os.path.realpath(os.path.dirname(inspect.getfile(inspect.currentframe())))
#if cmd_folder not in sys.path:
#	sys.path.insert(0, cmd_folder)


#cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0],"../radio")))
#if cmd_subfolder not in sys.path:
#	sys.path.insert(0, cmd_subfolder)
#
#cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0],"../libs")))
#if cmd_subfolder not in sys.path:
#	sys.path.insert(0, cmd_subfolder)

from doge.radio.RadioInterface import RadioInterface
from doge.libs.to_precision import to_precision

pipe = RadioInterface("edison")

pipe.connect_sketch()
sleepDelay = 30.0 #0.5
frameNumber = 1
try:
    frameNumber = int(sys.argv[1])
except IndexError:
    frameNumber = 1
#frameNumber = 300 
node = 5

data = defaultdict(list)

MIN_TEMP = 39.0 # = 32 Celsius
MAX_TEMP = 68.0
# 39 raw = 32 F
# 68 raw = 100 F
for frame in range(0, frameNumber):
        print str(time.strftime('%H:%M:%S', time.localtime())) + " PDT"
	rawTemp = pipe.pull("boosterpack", node, "temperature")
	rssi = pipe.pull("boosterpack", node, "rssi")
	rssi = round(-((rssi^0xff)+1),1)
        data['rssi'].append(rssi)
	print("Rssi = " + str(rssi))
	if(rawTemp != -1):
                data['temp_r'].append(rawTemp)
		tempC = round(((((rawTemp/255.0)*3.3)-0.5)/0.01)*2)/2
                data['temp_c'].append(tempC)
		tempF = round(32 + (9/5.0)*tempC,1)
                data['temp_f'].append(tempF)

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
        if(frame < frameNumber):
                time.sleep(sleepDelay)

import numpy
avgRssi = to_precision(round(numpy.mean(data['rssi'])),3)
stdRssi = to_precision(round(numpy.std(data['rssi']),3),3)
print "Average Rssi = " + str(avgRssi) + "; Standard Deviation Rssi: " + str(stdRssi)
avgRawTemp = to_precision(round(numpy.mean(data['temp_r'])),3)
stdRawTemp = to_precision(round(numpy.std(data['temp_r']),3),3)
print "Average RawTemp = " + str(avgRawTemp) + "; Standard Deviation RawTemp: " + str(stdRawTemp)
avgTempC = to_precision(round(numpy.mean(data['temp_c'])*2)/2,3)
stdTempC = to_precision(round(numpy.std(data['temp_c']),3),3)
print "Average TempC = " + str(avgTempC) + "; Standard Deviation TempC: " + str(stdTempC)
avgTempF = to_precision(round(numpy.mean(data['temp_f']),1),3)
stdTempF = to_precision(round(numpy.std(data['temp_f']),3),3)
print "Average TempF = " + str(avgTempF) + "; Standard Deviation TempF: " + str(stdTempF)
pktLoss = str(frameNumber - len(data['temp_r'])) + '/' + str(frameNumber)
print "Packet Loss = " + pktLoss

import re
stream = 0
try:
    if(bool(re.match('stream',str(sys.argv[2]), re.I))):
        stream = 1
except IndexError:
    stream = 0

#if(stream == 1):
#    cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0],"../cloud")))
#    if cmd_subfolder not in sys.path:
#        sys.path.insert(0, cmd_subfolder)

    from doge.core.DogeHub import connect_cloud
    cloudStream = connect_cloud("sparkfun")
    jsonData = {'location':'indoors','network':'boosterpack','id':node,'rssi':avgRssi,'rssi_std':stdRssi,'temp_c':avgTempC,'temp_c_std':stdTempC,'temp_f':avgTempF,'temp_f_std':stdTempF,'pkt_loss':pktLoss}
    cloudStream.push(jsonData)

