from socketIO_client import SocketIO
import json
import re

def process_message(*args):
    json_args = json.dumps(args)
    #Only process messages with a string "command" in it
    if 'command' in json_args:
     #remove \\ and " from 
     json_args= str.replace(json_args,'\\','')
     json_args= str.replace(json_args,'"','')
     #Extract command and data
     m = re.search('{command:(.+),data:(.+)}', json_args)
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
nte_enum = { 'nte_id': 0,'nte_lqe':1, 'nte_radio':2, 'nte_nwid': 3}
rte_enum = { 'rte_id': 0,'rte_lqe':2, 'rte_nindex':3}

#Defined radio groups to be used for node grouping
radio_group  = {0:'433mhz',1:'916mhz',2:'1.2ghz'}

edge_id = 0
route_edge_id = 0

edges =[]
nodes =[]
route_edges= []

def createNetworkVis(node_id,radio_id,nte_list,rte_list):
  nodes.append({'id':node_id,'label':node_id,'group':radio_group[radio_id]})
  for i in nte_list:
   global edge_id
   edges.append({'id':edge_id, 'from':node_id, 'to': i[nte_enum['nte_id']],'label':i[nte_enum['nte_lqe']]})
   edge_id += 1
 
  for j in rte_list:
   global route_edge_id
   route_edges.append({'id':route_edge_id, 'from': node_id, 'to': j[rte_enum['rte_id']],'label':j[rte_enum['rte_lqe']]})
   route_edge_id += 1
 

 #return (nodes,edges)
 
  
nte_list = [ [1,23,1,0],[2,50,1,0]]
rte_list = [ [3,72,1],[4,99,1]]

createNetworkVis (0,1,nte_list,rte_list)


nte_list = [ [2,83,1,1],[3,70,1,1]]

createNetworkVis (1,2,nte_list,rte_list)

nodes_json = json.dumps(nodes)
edges_json = json.dumps(edges)
route_edges_json = json.dumps(route_edges)

#print "nodes=",nodes_json
#print "edges=",edges_json
#print "route_edges=",route_edges_json

try:
 socket.emit('load_network',nodes_json)
 print "Successfuly sent network load message"
except:
 print "Error could not send load network message"

while True:
 #Wait for incoming message targetted to routing processor
 socket.on('message', process_message)
 socket.wait(seconds=1)
