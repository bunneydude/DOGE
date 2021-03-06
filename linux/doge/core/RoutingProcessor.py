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

 networkNodes = {}

 
 def __init__(self, port, initialNetwork={}):
   self.socket = self.createSocket(port);
   self.network_neighbor_tables = {}
   self.network_routing_tables = {}
   self.networkNodes = initialNetwork

 # Appends a dictionary to 'nodes', 'edges' and 'route_edges' based on the passed in node object
 # These structures are later sent to the webserver to create the initial view of the network 
 def createNetworkVis(self,nodes, edges, route_edges, node):
   nodeID = node.get_nodeID()
   neighborTable = node.get_neighbor_table()
   routingTable = node.get_routing_table()
   radioName = node.get_radio()

   #Build lists of entire network neighbor table and routing table entries
   self.network_neighbor_tables[nodeID] = neighborTable
   self.network_routing_tables[nodeID] = routingTable
  
   #Add node to list of network nodes for webserver
   nodes.append({'id':nodeID,'label':nodeID,'group':radioName})

   #Go through Neighbor Table Entry list and add edges
   for entry in neighborTable:
      radioNumber = entry[3]
      edges.append({'id':self.edge_id, 'from':nodeID, 'to': entry[self.NTE_ID],'label':entry[self.NTE_LQE],'radio':radioNumber})
      self.edge_id += 1
     
   #Go through Routing Table Entry list and add edges
   for entry in routingTable:
     route_edges.append({'id':self.route_edge_id, 'from': nodeID, 'to': entry[self.RTE_ID],'label':entry[self.RTE_LQE]})
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

 # nodeID is a string
 def mask_node(self, command, nodeID, action):
     data = {'command':'alert','data':'Error: Unhandled execution in mask_node'}
     if int(nodeID) in self.networkNodes:
         node = self.networkNodes[int(nodeID)]
         #assert int(nodeID) == node.get_nodeID()
         
         if node.has_route(): #Check if this node has any routing table entries
             errorMessage = "Error: Node {0} cannot be masked since it has routing table entries".format(int(nodeID))
             print(errorMessage)
             data = {'command':'alert','data':errorMessage}
         else:
             print "Node {0} has no routes. Allowing masking".format(int(nodeID))
             node.mask_neighbor(action=action) #no id means to mask all neighbor edges
             data = {'command':command,'data':nodeID}
             print "Sending confirmation: {0}".format(data)

     else:
         print("networkNodes = {0}".format(self.networkNodes))
         errorMessage = "Error: Node {0} not found in networkNodes".format(int(nodeID))
         print(errorMessage)
         data = {'command':'alert','data':errorMessage}
     
     return data
 
 
 def mask_edge(self, command, edge, action="mask"):
     foundNode = False
     responseData = {'command':'alert', 'data':'ERROR: Unhandled execution in mask_edge'}

     #Edge format is [from_nodeID, to_nodeID, edgeID]
     edgeFrom =json.loads(edge)[0]
     edgeTo = json.loads(edge)[1]

     #Find node then mask specified edge
     if edgeFrom in self.networkNodes:                      
         node = self.networkNodes[edgeFrom]
         #assert edgeFrom == node.get_nodeID()
         if(node.has_neighbor(edgeTo)):
             foundNode = True
             node.mask_neighbor(edgeTo, action)
             responseData = {'command':command,'data':edge}
             print "Sending confirmation: {0}".format(responseData)
         else:
             errorMessage = "Error: Node {0} does not have a neighbor entry for node {1}".format(edgeFrom, edgeTo)
             print(errorMessage)
             responseData = {'command':'alert','data':errorMessage}
     else:
         errorMessage =  "Error: Could not find nodeID {0} in list of network nodes".format(edgeFrom)
         print(errorMessage)
         responseData = {'command':'alert','data':errorMessage}
              
     return responseData


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
             response = self.mask_node(command,data,"mask")     
             self.socket.emit('confirm', json.dumps(response))

         elif (command == 'unmask_node'):  
             response = self.mask_node(command,data,"unmask") 
             self.socket.emit('confirm', json.dumps(response))

         elif (command == 'mask_physical_edge'):  
             response = self.mask_edge(command,data,"mask")   	
             self.socket.emit('confirm', json.dumps(response))

         elif (command == 'unmask_physical_edge'):  
             response = self.mask_edge(command,data,"unmask")   	
             self.socket.emit('confirm', json.dumps(response))

  

 
  

