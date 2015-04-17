#import os, sys, inspect
from doge.core.RoutingProcessor import RoutingProcessor
import json

edisonRP = RoutingProcessor()

edges =[]
nodes =[]
route_edges= []

nte_list = [ [1,50,1,0],[2,90,1,0],[5,30,1,0]]
rte_list = [ [3,72,1],[4,99,1]]


edisonRP.createNetworkVis (nodes,edges,route_edges,0,nte_list,rte_list)


nte_list = [ [3,50,1,0],[4,90,1,0],[1,30,1,0]]
rte_list = []


edisonRP.createNetworkVis (nodes,edges,route_edges,1,nte_list,rte_list)


nodes_json = json.dumps(nodes)
edges_json = json.dumps(edges)
route_edges_json = json.dumps(route_edges)

print "nodes=",nodes_json
print "edges=",edges_json
print "route_edges=",route_edges_json
nw_json = {'nodes':nodes_json,'edges':edges_json,'routing_edges':route_edges_json}

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

