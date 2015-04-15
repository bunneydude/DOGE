from socketIO_client import SocketIO
import json
import re

def processMessage(*args):
    argsJson = json.dumps(args)
    #Only process messages with a string "command" in it
    if 'command' in argsJson:
     #remove \\ and " from 
     argsJson= str.replace(argsJson,'\\','')
     argsJson= str.replace(argsJson,'"','')
     #Extract command and data
     m = re.search('{command:(.+),data:(.+)}', argsJson)
     if m:
          command =  m.group(1)
          data  =  m.group(2)
          print "command:"+command
          print "data:"+data

     return (command,data)

#Create client socket 
socket = SocketIO('localhost', 8124)

#Use room name rpsock to join
#Server can use this to send targetted messages
socket.emit('join',{'socketid':'rpsock'})

#Define which bit positions in the array correspond to element in neighbor and routing table entry
NTE_ID = 0
NTE_LQE = 1
NTE_RADIO = 2
NTE_NWID = 3

RTE_ID = 0
RTE_LQE = 1
RTE_INDEX = 2

#Defined radio groups to be used for node grouping
radio_group  = {0: 'edison',1:'433mhz',2:'916mhz',3:'1.2ghz'}

edge_id = 0
route_edge_id = 1000

edges =[]
nodes =[]
route_edges= []

def createNetworkVis(node_id,radio_id,nte_list,rte_list):
  nodes.append({'id':node_id,'label':node_id,'group':radio_group[radio_id]})
  for i in nte_list:
   global edge_id
   edges.append({'id':edge_id, 'from':node_id, 'to': i[NTE_ID],'label':i[NTE_LQE]})
   edge_id += 1
 
  for j in rte_list:
   global route_edge_id
   route_edges.append({'id':route_edge_id, 'from': node_id, 'to': j[RTE_ID],'label':j[RTE_LQE]})
   route_edge_id += 1
 

 #return (nodes,edges)
 
  
nte_list = [ [1,50,1,0],[2,90,1,0],[5,30,1,0]]
rte_list = [ [3,72,1],[4,99,1]]
createNetworkVis (0,0,nte_list,rte_list)


nte_list = [ [0,23,1,1]]
rte_list = []
createNetworkVis (1,1,nte_list,rte_list)

nte_list = [ [4,81,1,1],[3,74,1,1]]
createNetworkVis (2,2,nte_list,rte_list)

nte_list = [ [9,83,1,1],[8,70,1,1]]
createNetworkVis (3,3,nte_list,rte_list)

nte_list = [ [9,83,1,1]]
createNetworkVis (4,1,nte_list,rte_list)

nte_list = [ [6,83,1,1]]
createNetworkVis (5,1,nte_list,rte_list)

nte_list = [ [9,83,1,1]]
createNetworkVis (6,1,nte_list,rte_list)

nodes_json = json.dumps(nodes)
edges_json = json.dumps(edges)
route_edges_json = json.dumps(route_edges)

#print "nodes=",nodes_json
#print "edges=",edges_json
#print "route_edges=",route_edges_json
nw_json = {'nodes':nodes_json,'edges':edges_json,'routing_edges':route_edges_json}

try:
 socket.emit('load_network',json.dumps(nw_json))
 print "Successfuly sent network load message"
except:
 print "Error could not send load network message"

while True:
 #Wait for incoming message targetted to routing processor
 socket.on('message', processMessage)
 socket.wait(seconds=1)
