from DogeHub import connect_cloud

node = 2
tempC = 23
tempF = 72
pktLoss = 0
avgRssi = 2.4

cloudStream = connect_cloud("intel")
jsonData = {'location':'indoors','network':'boosterpack','id':node,'rssi':avgRssi,'temp_c':tempC,'temp_f':tempF,'pkt_loss':pktLoss}
cloudStream.push(jsonData)

