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
#jsonData = {'location':location,'network':network,'id':node,'rssi':avgRssi,'temp_c':tempC,'temp_f':tempF,'pkt_loss':pktLoss}

cloudStream.push({'id':node, 'temperature':tempF})

cloudStream.push({'id':node, 'rssi':avgRssi})



#pull data from cloud
#dataFilterJson = {'location':location,'network':network,'id':node}
dataJson = cloudStream.pull({'id':node, 'sensor':'temperature'})
print('temperature='+dataJson['temperature'])

dataJson = cloudStream.pull({'id':node, 'sensor':'rssi'})
print('rssi='+dataJson['rssi'])

