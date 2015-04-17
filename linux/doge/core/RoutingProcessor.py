from socketIO_client import SocketIO
import json
import re

class RoutingProcessor():

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


 def __init__(self):
   #createSocket();
   print "init"

 def createNetworkVis(self,nodes,edges,route_edges,node_id,nte_list,rte_list):
   nodes.append({'id':node_id,'label':node_id,'group':self.radio_group[nte_list[0][2]]})
   for i in nte_list:
     edges.append({'id':self.edge_id, 'from':node_id, 'to': i[self.NTE_ID],'label':i[self.NTE_LQE]})
     self.edge_id += 1
 
   for j in rte_list:
     route_edges.append({'id':self.route_edge_id, 'from': node_id, 'to': j[self.RTE_ID],'label':j[self.RTE_LQE]})
     self.route_edge_id += 1

 def createSocket(self,port):
   #Create client socket 
   socket = SocketIO('localhost', port)

   #Use room name rpsock to join
   #Server can use this to send targetted messages
   socket.emit('join',{'socketid':'rpsock'})

   return socket

 def processMessage(self,*args):
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

 


  

 
  

