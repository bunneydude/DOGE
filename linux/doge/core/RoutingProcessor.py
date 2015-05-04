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
 radio_group  = {0: 'edison',1:'433mhz',2:'915mhz',3:'2.4ghz'}

 #These will be used as id's for the vis.js network
 edge_id = 0
 route_edge_id = 1000

 
 def __init__(self,port):
   self.socket = self.createSocket(port);
   self.network_neighbor_tables = {}
   self.network_routing_tables = {}

 
 def createNetworkVis(self,nodes, edges, route_edges, node_id, nte_list, rte_list):

   #Build lists of entire network neighbor table and routing table entries
   self.network_neighbor_tables[node_id] = nte_list
   self.network_routing_tables[node_id] = rte_list
   
   #Add node to list of network nodes
   nodes.append({'id':node_id,'label':node_id,'group':self.radio_group[nte_list[0][2]]})

   #Go through Neighbor Table Entry list and add edges
   for i in nte_list:
     
     #If empty list, add first edge
     if not edges  :
      edges.append({'id':self.edge_id, 'from':node_id, 'to': i[self.NTE_ID],'label':i[self.NTE_LQE],'radio':i[self.NTE_RADIO]})
      self.edge_id += 1
     
     #If list is populated, check if edge already exists for the same radio
     else:
      add_edge = True
      for edge in edges:
        #Check if edge already exists. If it does, set flag to False so it doesnt get added
        if (edge['to'] == node_id and edge['from'] == i[self.NTE_ID] and edge['radio'] == i[self.NTE_RADIO]):  
           add_edge = False
      if (add_edge) :
          edges.append({'id':self.edge_id, 'from':node_id, 'to': i[self.NTE_ID],'label':i[self.NTE_LQE],'radio':i[self.NTE_RADIO]})
          self.edge_id += 1
          add_edge = True
   
   #Go through Routing Table Entry list and add edges
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

 def getSocket(self):
   return self.socket

 def mask_node(self,command,node_id):
   #Check if this node has any routing table entries
   if self.network_routing_tables[int(node_id)]:
     print "Node {0} has routing table entries".format(node_id)
     data = {'command':'alert','data':'ERROR: Cannot mask node since it has routing tables'}
     self.socket.emit('confirm',json.dumps(data))

   else:
     print "Node {0} has no routes. Allowing masking".format(node_id)
     data = {'command':command,'data':node_id}
     print "Sending confirmation: {0}".format(data)
     self.socket.emit('confirm',json.dumps(data))
  
 def mask_edge(self,command,edge):
     #Edge format is [from,to,id]
     edge_from =json.loads(edge)[0]
     edge_to = json.loads(edge)[1]
     #Loop through NTE list for the source node and change LQE for matching dest node to 999 (mask value)
     for nte_arr in self.network_neighbor_tables[int(edge_from)]:
         if nte_arr[0] == edge_to:
             nte_arr[1] = 999
     #print "NTE = {0}".format(self.network_neighbor_tables)
     data = {'command':command,'data':edge}
     print "Sending confirmation: {0}".format(data)
     self.socket.emit('confirm',json.dumps(data))


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
             print "command:{0} data:{1}".format(command,data)
         if (command == 'mask_node'):  
             self.mask_node(command,data)     
         if (command == 'mask_physical_edge'):  
             self.mask_edge(command,data)   	


  

 
  

