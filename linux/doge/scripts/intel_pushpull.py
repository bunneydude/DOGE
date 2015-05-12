from doge.core.DogeHub import connect_cloud
from pprint import pprint

location = 'testgrid'
network = 'boosterpack'
node = 2
tempC = 23
tempF = 72
pktLoss = 0
avgRssi = 68

cloudStream = connect_cloud("intel")

#push data to cloud
iotkitJson =  {'component_type':'temperature','component_version':"1.0",'component_format':'float','component_measureunit':'Degrees Farenheit'}
jsonData = {'location':location,'network':network,'id':node,'rssi':avgRssi,'temp_c':tempC,'temp_f':tempF,'pkt_loss':pktLoss}

cloudStream.push(jsonData,iotkitJson,'temp_f')

iotkitJson =  {'component_type':'rssi','component_version':"1.0",'component_format':'float','component_measureunit':'Strength'}
cloudStream.push(jsonData,iotkitJson,'rssi')



#pull data from cloud
iotkitJson =  {'component_type':'temperature','component_version':"1.0",'component_format':'float','component_measureunit':'Degrees Farenheit'}
dataFilterJson = {'location':location,'network':network,'id':node}
dataJson = cloudStream.pull(dataFilterJson,iotkitJson)
pprint(dataJson)


iotkitJson =  {'component_type':'rssi','component_version':"1.0",'component_format':'float','component_measureunit':'Strength'}
dataJson = cloudStream.pull(dataFilterJson,iotkitJson)
pprint(dataJson)
