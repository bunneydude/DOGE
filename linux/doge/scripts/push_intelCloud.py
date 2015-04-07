#import os, sys, inspect
#cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0],"../core")))
#if cmd_subfolder not in sys.path:
#	sys.path.insert(0, cmd_subfolder)

from doge.core.DogeHub import connect_cloud

node = 2
tempC = 23
tempF = 72
pktLoss = 0
avgRssi = 68

cloudStream = connect_cloud("intel")

iotkitJson =  {'component_type':'temperature','component_version':"1.0",'component_format':'float','component_measureunit':'Degrees Farenheit'}
jsonData = {'location':'indoors','network':'boosterpack','id':node,'rssi':avgRssi,'temp_c':tempC,'temp_f':tempF,'pkt_loss':pktLoss}

cloudStream.push(jsonData,iotkitJson,'temp_f')

iotkitJson =  {'component_type':'rssi','component_version':"1.0",'component_format':'float','component_measureunit':'Strength'}
cloudStream.push(jsonData,iotkitJson,'rssi')



