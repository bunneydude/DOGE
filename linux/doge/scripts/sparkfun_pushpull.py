from doge.core.DogeHub import connect_cloud,getLatestNodeData
from pprint import pprint

#action = 'push'
action = 'pull'
cloudStream = connect_cloud("sparkfun")

#push data to sparkfun service
jsonData = {'location':'testgrid','network':'boosterpack','id':'0','rssi':-30.0,'rssi_std':'0.00','temp_c':'22.5','temp_c_std':'0.00','temp_f':'72.5','temp_f_std':'0.00','pkt_loss':'0/1'}
jsonData = {'location':'testgrid','network':'boosterpack','id':'0','rssi':'-25.0','temp_c':'22.0','temp_f':'71.6'}
if action == 'push':
	cloudStream.push(jsonData)

action = 'pull'

#pull data from sparkfun service
dataFilter = {'location':'testgrid','network':'boosterpack','id':'0'}
if action == 'pull':
	data = getLatestNodeData(cloudStream,dataFilter)
	pprint(data)

