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



