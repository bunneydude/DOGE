from doge.core.RoutingProcessor import RoutingProcessor
import json


edges =[]
nodes =[]
route_edges= []

edisonRP = RoutingProcessor(8124)

nte_array = [ [1,50,1,0],[9,30,2,0]]
rte_array = []
edisonRP.createNetworkVis (nodes,edges,route_edges,0,nte_array,rte_array)

nte_array = [ [2,50,2,0],[3,90,3,0]]
rte_array = [ [2,50,3],[3,60,3],[4,66,3],[7,88,3]]
edisonRP.createNetworkVis (nodes,edges,route_edges,1,nte_array,rte_array)

nte_array = [ [1,50,2,0],[4,67,3,0]]
rte_array = [ [4,33,3],[7,80,3]]
edisonRP.createNetworkVis (nodes,edges,route_edges,2,nte_array,rte_array)

nte_array = [ [1,90,3,0],[4,67,3,0]]
rte_array = []
edisonRP.createNetworkVis (nodes,edges,route_edges,3,nte_array,rte_array)

nte_array = [ [2,67,3,0],[3,97,3,0],[5,58,3,0],[7,77,3,0]]
rte_array = [[7,77,3]]
edisonRP.createNetworkVis (nodes,edges,route_edges,4,nte_array,rte_array)

nte_array = [ [4,65,3,0],[6,97,3,0],[8,77,1,0]]
rte_array = [[6,77,1],[7,87,1]]
edisonRP.createNetworkVis (nodes,edges,route_edges,5,nte_array,rte_array)

nte_array = [ [7,97,2,0],[5,77,3,0]]
rte_array = [[7,87,1]]
edisonRP.createNetworkVis (nodes,edges,route_edges,6,nte_array,rte_array)

nte_array = [ [4,65,3,0],[6,97,2,0]]
rte_array = []
edisonRP.createNetworkVis (nodes,edges,route_edges,7,nte_array,rte_array)


#print edisonRP.nte
#print edisonRP.rte

nodes_json = json.dumps(nodes)
edges_json = json.dumps(edges)
route_edges_json = json.dumps(route_edges)

nw_json = {'nodes':nodes_json,'edges':edges_json,'routing_edges':route_edges_json}

sock= edisonRP.getSocket()

try:
 sock.emit('load_network',json.dumps(nw_json))
 print "Successfuly sent network load message"
except:
 print "Error could not send load network message"

while True:
 #Wait for incoming message targetted to routing processor
 sock.on('message', edisonRP.processMessage)
 sock.wait(seconds=1)
  
