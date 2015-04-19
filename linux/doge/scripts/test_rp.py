from doge.core.RoutingProcessor import RoutingProcessor
import json

edisonRP = RoutingProcessor()

edges =[]
nodes =[]
route_edges= []
nte = {}
rte= {}

nte_array = [ [1,50,1,0],[9,30,1,0]]
rte_array = []
nte[0] = nte_array
edisonRP.createNetworkVis (nodes,edges,route_edges,0,nte_array,rte_array)

nte_array = [ [2,50,1,0],[3,90,1,0]]
rte_array = [ [2,50,3],[3,60,3]]
nte[1] = nte_array
edisonRP.createNetworkVis (nodes,edges,route_edges,1,nte_array,rte_array)

nte_array = [ [1,45,1,0],[4,67,1,0]]
rte_array = [ [4,33,3],[7,80,3]]
nte[2] = nte_array
edisonRP.createNetworkVis (nodes,edges,route_edges,2,nte_array,rte_array)

nte_array = [ [1,45,1,0],[4,67,1,0]]
rte_array = []
nte[3] = nte_array
edisonRP.createNetworkVis (nodes,edges,route_edges,3,nte_array,rte_array)

nte_array = [ [2,65,1,0],[3,97,1,0],[5,58,1,0],[7,77,1,0]]
rte_array = [[7,77,3]]
nte[4] = nte_array
edisonRP.createNetworkVis (nodes,edges,route_edges,4,nte_array,rte_array)

nte_array = [ [4,65,1,0],[6,97,1,0],[8,77,1,0]]
rte_array = [[6,77,1],[7,87,1]]
nte[5] = nte_array
edisonRP.createNetworkVis (nodes,edges,route_edges,5,nte_array,rte_array)

nte_array = [ [7,97,1,0],[5,77,1,0]]
rte_array = [[7,87,1]]
nte[6] = nte_array
edisonRP.createNetworkVis (nodes,edges,route_edges,6,nte_array,rte_array)

nte_array = [ [4,65,1,0],[6,97,1,0]]
rte_array = []
nte[7] = nte_array
edisonRP.createNetworkVis (nodes,edges,route_edges,7,nte_array,rte_array)


nodes_json = json.dumps(nodes)
edges_json = json.dumps(edges)
route_edges_json = json.dumps(route_edges)

#print "nodes=",nodes_json
#print "edges=",edges_json
#print "route_edges=",route_edges_json
nw_json = {'nodes':nodes_json,'edges':edges_json,'routing_edges':route_edges_json}

#print nte
#n=1

#for x in nte:
 # for y in nte[x]:
  #  if y[0]==n:
   #   y[1]=999

#print nte

port=8124
sock = edisonRP.createSocket(port)

try:
 sock.emit('load_network',json.dumps(nw_json))
 print "Successfuly sent network load message"
except:
 print "Error could not send load network message"

while True:
 #Wait for incoming message targetted to routing processor
 sock.on('message', edisonRP.processMessage)
 sock.wait(seconds=1)

