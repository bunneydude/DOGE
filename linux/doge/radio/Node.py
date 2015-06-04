import os.path
import json
import RadioInterface
import doge.core.protocol_ctypes as ProtocolDefs
from __builtin__ import file
import random
from doge.conf.globals import config
import warnings

class Device:
   """The Device class represents a microcontroller and its firmware.

   """

   _validDeviceNames = ["msp430g2553", "lpc812"]

   _mmString = {
      "device":{"size":"mm_device_size", "base":"mm_device_base"},
      "network":{"base":"mm_network_base", "size":"mm_network_size"},
      "gpio":{"base":"mm_gpio_base", "size":"mm_gpio_size"},
      "adc":{"base":"mm_adc_base", "size":"mm_adc_size"},
      "uart":{"base":"mm_uart_base", "size":"mm_uart_size"},
      "dsp":{"base":"mm_dsp_base", "size":"mm_dsp_size"},
      "static_route":{"base":"mm_static_route_base", "size":"mm_static_route_size"}
   }

   def __init__(self, deviceName, memoryMapFile):
      """ Create the device based on the microcontroller name and memory map
      Args:
         deviceName (str) : The name of the microcontroller. Must be one of:
            * msp430g2553
            * lpc812
         memoryMapFile (str) : File name containing a json object that describes the base and sizes of the differet memory sections
      """
      # Validate inputs
      if(deviceName.lower() not in self._validDeviceNames): raise Exception("deviceName {0} is invalid. Must be one of {1}".format(deviceName.lower(), self._validDeviceNames))
      if(not os.path.isfile(memoryMapFile)): raise Exception("File {0} does not exist".format(memoryMapFile))

      # assign inputs
      self._deviceName = deviceName.lower()
      
#      print("Loading memory map: {0}".format(memoryMapFile))
      with open(memoryMapFile, 'r') as file:
         self._memoryMap = json.load(file)

   def to_s(self):
      """ Print a string representation of the object """
      print("Device: config = {0}, version = {1}".format(self._deviceName, self._memoryMap))

   def address(self, space, register):
      """ Return the physical address of the register in the specified memory space
      Args
         space (str) : The memory space to access. Must be one of:
            * "device"
            * "network"
            * "gpio"
            * "adc"
            * "uart"
            * "dsp"

         register (int) : Offset for the register you want to access. A full list is in mmFields.py.
   
      Returns (int) : Returns -1 if the regsister offset was outside the range of the memory space. Otherwise, returns the physical address.
      """
      if(space in self._mmString.keys()):
         if(register < self._memoryMap[self._mmString[space]["size"]]):
            return self._memoryMap[self._mmString[space]["base"]] + register
      return -1
         
   #returns base, size
   # space is one of _mmString.keys()
   def get_region_info(self, space):
      """ Get the base and size value for a given address space

      Args
         space (str) : The memory space to access. Must be one of:
            * "device"
            * "network"
            * "gpio"
            * "adc"
            * "uart"
            * "dsp"
      returns (int, int) : Returns the base and size as a tuple
      """
      if(space not in self._mmString.keys()): raise Exception("The space provided, {0}, must be one of {1}".format(space, self._mmSpace.keys()))
      return self._memoryMap[self._mmString[space]["base"]], self._memoryMap[self._mmString[space]["size"]]      


# A node consists of a device (microcontroller + firmware),
# a set of radios, inputs, and outputs
class HardwareNode:
    _validRadios = ["nrf24", "cc1101l", "rfm69"]
    _maxRadios = 4
    _hardwareState = {'neighbors':[], 'routes':[]}
    _stateFields = ['all'] + _hardwareState.keys()
    _validSources = ['state', 'hardware']

    def __init__(self, device, nodeID, pipe, master, load=True):
        if(not isinstance(device, Device)): raise Exception("The device argument must be an instance of Node.Device.")
        if(nodeID not in range(1, 2**16)): raise Exception("The nodeID, {0}, must be in the range [1, 65535]".format(nodeID))
        if(not isinstance(pipe, RadioInterface.RadioInterface)): raise Exception("The device argument must be an instance of Node.Device.")
        if(not isinstance(master, VirtualNode)): raise Exception("The master node must be an instance of VirtualNode.")

        self._device = device
        self._nodeID = nodeID
        self._pipe = pipe
        self._radios = []
        self._inputs = {}
        self._outputs = []
        self._networkTable = None
        self._masterNode = master
        self._loaded = False

        self.add_sensor("neighborCount", "device", 0)
        self.add_sensor("routeCount", "device", 1)
        self.add_sensor("divisionIndex", "device", 2)
        self.add_sensor("networkConfig", "device", 3)

        #TODO determine this from reading a TBD field in memory map
        if(self._device._deviceName.lower() == 'msp430g2553'):
            self._primaryRadio = '915mhz'
        elif(self._device._deviceName.lower() == 'lpc812'):
            self._primaryRadio = '2.4ghz'
        else:
            self._primaryRadio = 'edison'
        
        if(load == True): self.load_state()
         
    # Create the network table for this node
    # If using a debug mode, preload
    # Otherwise, query the node directly for its table with read_network_state()
    def load_state(self):
        self._loaded = True
        narray = []
        rarray = []
        if(True == config['debug'] == config['debug_test_network']):
            #print("Use preloaded network")
            maxNetworkSize = 8
            maxNeighbors = 4
            maxRoutes = 4
            
            narray = config['preset_node_tables'][self._nodeID]["narray"]
            rarray = config['preset_node_tables'][self._nodeID]["rarray"]
            for byte in range(0,4):
                for index in range(0,len(narray)):
                    self.add_sensor("n{0}_{1}".format(index, byte), "network", index*4 + byte)

        else: #read info from node
            narray, rarray, maxNetworkSize, maxNeighbors, maxRoutes = self.read_network_state()
      
        print("Created node {0} with neighbor array {1}, routing array {2}".format(self._nodeID, narray, rarray)) 
        self._networkTable = NetworkTable(narray, rarray, maxNetworkSize, maxNeighbors, maxRoutes)

    def to_s(self):
        print("Node: device = {0}, nodeID = {1}, radio = {2}, inputs = {3}, outputs = {4}".format(self._device, self._nodeID, self._radios, self._inputs, self._outputs))

    def get_nodeID(self):
        return self._nodeID

    def get_radio(self):
        return self._primaryRadio

    # field - which state to commit
    # force - if False, first check to see if the state needs to be updated. Otherwise always update
    def commit(self, field='all', force=False): #FIXME not yet implemented
        pass
    #    if(field.lower() == 'all'):

    #returns True if there is state that needs to be committed to the HW node
    # if returns True, the info list has the keys for which fields have been modified
    def modified(self, info=[]): #FIXME not yet implemented
        pass 

    # Add a radio to this node so the webserver can color it properly
    def add_radio(self, radio):
        if(radio.lower() not in self._validRadios): raise Exception("Invalid radio specified: {0}. Must be one of {1}".format(radio.lower(), self._validRadios))
        if(len(self._radios) == self._maxRadios): raise Exception("This node has exceeded the max number of radios: {0}".format(self._maxRadios))

        self._radios.append(radio.lower())

    # Adds a 'name, offset' pair to this node so the user can call pull/push
    def add_sensor(self, name, registerSpace, registerOffset):
#        print("DEBUG - Node {0}: add sensor {1} at {2}".format(self._nodeID, name, registerOffset))
        if(name.lower() in self._inputs.keys()): raise Exception("Node {0}: The sensor name, {1}, is already in use. Current sensors are {2}".format(self._nodeID, name.lower(), self._inputs.keys()))
        if(registerSpace.lower() not in self._device._mmString.keys()): raise Exception("Invalid registerSpace specified: {0}. Must be one of: {1}".format(registerSpace.lower(), self._device._mmString.keys()))
       
        address = self._device.address(registerSpace, registerOffset)
        if(address == -1): raise Exception("Offset {0} in space {1} is out of range. Please consult the memory map file".format(registerOffset, registerSpace))       
        self._inputs[name.lower()] = {"space":registerSpace.lower(), "offset":registerOffset}

    # Get data from the declared sensor name
    # Returns a 'int, list' tuple
    def pull(self, sensorName):
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        if(sensorName.lower() not in self._inputs.keys()): raise Exception("Unknown sensor: {0}. Current sensor list: {1}".format(sensorName.lower(), self._inputs.keys()))
        shID = self._masterNode.get_forward_id(self._nodeID)
        if(shID == -1): 
            warnings.warn("Node {0}: No route possible".format(self._nodeID), Warning)
            return 0, []

        returnData = {}
        address = self._device.address(self._inputs[sensorName.lower()]["space"], self._inputs[sensorName.lower()]["offset"])

        self._pipe.proxy_send(destination=self._nodeID, command=ProtocolDefs.CMD_READ_REG, address=address, payload=0, singleHopDest=shID)
        returnCode = self._pipe.proxy_receive()
        #print("Pull complete. Got: [header: [{0}], size = {1}, data = {2}]".format(ProtocolDefs.print_structure(self._pipe.rxPacket.hdr), self._pipe.rxPacket.size, list(i for i in self._pipe.rxPacket.data)))

        if(returnCode == -1):
            return 0, []            
        else:
            return self._pipe.rxPacket.size, list(self._pipe.rxPacket.data)

    # Write a byte of data to the specified sensor name
    def push(self, sensorName, data):
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        if(sensorName.lower() not in self._inputs.keys()): raise Exception("Unknown sensor: {0}. Current sensor list: {1}".format(sensorName.lower(), self._inputs.keys()))       
        
        shID = self._masterNode.get_forward_id(self._nodeID)
        if(shID == -1): 
            warnings.warn("Node {0}: No route possible".format(self._nodeID), Warning)
            return 0, []    

        returnData = {}
        address = self._device.address(self._inputs[sensorName.lower()]["space"], self._inputs[sensorName.lower()]["offset"])

        self._pipe.proxy_send(destination=self._nodeID, command=ProtocolDefs.CMD_WRITE_REG, address=address, payload=data, singleHopDest=shID)
        self._pipe.proxy_receive()

        return self._pipe.rxPacket.size, list(self._pipe.rxPacket.data)

    def get_neighbor_table(self, source='state'):
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        if(source not in self._validSources): raise Exception("The source {0}, must be one of {1}".format(self._validSources))
        return (self._networkTable.get_neighbor_list())
 
    # return an array of node IDs corresponding to single-hop neighbors 
    def get_neighbors(self):
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        nodeIDs = []
        neighborTable = self._networkTable.get_neighbor_list()
        for entry in neighborTable:
            nodeIDs.append(entry[0])
        return(nodeIDs)

    def get_routes(self):
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        nodeIDs = []
        routingTable = self._networkTable.get_routing_list()
        for entry in routingTable:
            nodeIDs.append(entry[0])
        return(nodeIDs)

    def get_routing_table(self, source='state'):
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        if(source not in self._validSources): raise Exception("The source {0}, must be one of {1}".format(self._validSources))
        return (self._networkTable.get_route_list())
  
    def add_neighbor(self, args={}):
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        self._networkTable._neighborArray.append([args['shNodeID'], args['shLQE'], args['radioID'], args['networkID']])

    def add_route(self, destID, shID):
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        print("Error - add_route for HardwareNode not yet implemented")
        return -1

        if(destID not in range(1, 2**16)): raise Exception("The specified destID, {0}, must be in the range [1, 65535]".format(destID))
        if(shID not in range(1, 2**16)): raise Exception("The specified shID, {0}, must be in the range [1, 65535]".format(shID))
      
        entry, index = self._networkTable.get_neighbor_entry(shID)
        if(len(entry) == 0):
            print("Error - Node {0}: Tried to add route to node {1}, but single-hop node {2} isn't a neighbor".format(self._nodeID, destID, shID))
            return -1
        else:
            self._networkTable._routingArray.append([destID, 254, index])
            return 1

    def has_neighbor(self, nodeID):
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        neighbors = self.get_neighbors()
        return(nodeID in neighbors)

    def has_route(self, nodeID=None):
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        if(nodeID is None): #return True if there's any route, False otherwise
            return (len(self._networkTable._routingArray) > 0)
        else: #check for a specific nodeID
            if(nodeID not in range(1, 2**16)): raise Exception("The specified nodeID, {0}, must be in the range [1, 65535]".format(nodeID))            
            return(nodeID in self.get_routes())

    def mask_neighbor(self, nodeID=None, action="mask"): # TODO pick a better name for the method
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        maskValues = {'mask':0, 'unmask':254, 'force':255}
        if(action not in maskValues.keys()): raise Exception("The action, {0}, must be one of {1}".format(action, maskValues.keys()))

        if nodeID is None: # operate on all entries
            for entry in self._networkTable._neighborArray: #FIXME breaking abstraction/protection rules here...
                print("   Set LQE in edge from node {0} to node {1}".format(self._nodeID, entry[0]))
                entry[1] = maskValues[action]
                self.update_neighbor_LQE(entry, self._networkTable._neighborArray.index(entry))
        else:
            if(nodeID not in range(1, 2**16)): raise Exception("The specified nodeID, {0}, must be in the range [1, 65535]".format(nodeID))
            entry, index = self._networkTable.get_neighbor_entry(nodeID)
            if(len(entry) == 0):
                print("Error: could not find node {0} in node {1}'s neighbor table.".format(nodeID, self._nodeID))
                print("   Neighbor table = {0}".format(self._networkTable.get_neighbor_list()))
            else:
                print("   Set LQE in edge from node {0} to node {1}".format(self._nodeID, nodeID))
                entry[1] = maskValues[action]
                self.update_neighbor_LQE(entry, index)

    def update_neighbor_LQE(self, entry, index):
        if(not self._loaded): raise Exception("Node {0} has not been loaded. Call load_state() on it first".format(self._nodeID))
        print("   Node {0}: update neighbor entry at index {1} to {2}".format(self._nodeID, index, entry))
        self.push("n{0}_{1}".format(index, 2), entry[1])

    # returns neighborArray, routingArray, max network size, max neighbors, max routes
    def read_network_state(self):        
        # get max network table size
        base, size = self._device.get_region_info("network")
        maxNetworkSize = size/4
        print("Node {0}: max network size = {1}".format(self._nodeID, maxNetworkSize))

        networkState = {"neighborCount":0, "routeCount":0, "divisionIndex":maxNetworkSize/2, "networkConfig":0}
        for field in networkState.keys():
            size, data = self.pull(field)
            if((data[0] == 3) and (size != 0)):
                networkState[field] = data[2]
                print("   Field {0} = {1}".format(field, networkState[field]))
            else:
                print("   Node {0}: error reading field {1}: size = {2}, data = {3}".format(self._nodeID, field, size, data))
        
        #determine max number of entry types
        maxNeighbors = networkState["divisionIndex"]
        maxRoutes = maxNetworkSize - networkState["divisionIndex"]

        # maybe this should be a warning instead of exception
        if((maxNeighbors + maxRoutes ) != maxNetworkSize): raise Exception("Node {0}: Max neighbors ({1}), max routes ({2}), max table size ({3}) don't add up".format(self._nodeID, maxNeighbors, maxRoutes, maxNetworkSize))

        neighborArray = []
        routingArray = []
        for index in range(0, networkState["neighborCount"]):
            entry = []
            for byte in range(0,4):
                self.add_sensor("n{0}_{1}".format(index, byte), "network", byte + index*4)
                size, data = self.pull("n{0}_{1}".format(index, byte))
                if((data[0] == 3) and (size != 0)):
                    entry.append(data[2])
                else:
                    entry.append(0)
            neighborArray.append(entry)

        for index in range(0, networkState["routeCount"]):
            entry = []
            for byte in range(0,4):
                self.add_sensor("r{0}_{1}".format(index, byte), "network", ((maxNetworkSize-1)*4) - index*4 + byte)
                size, data = self.pull("r{0}_{1}".format(index, byte))
                if((data[0] == 3) and (size != 0)):
                    entry.append(data[2])
                else:
                    entry.append(0)
            routingArray.append(entry)

        print("   Node {0}: neighbors = {1}, routes = {2}".format(self._nodeID, neighborArray, routingArray))
        return neighborArray, routingArray, maxNetworkSize, maxNeighbors, maxRoutes

    def get_rssi(self, nodeID=None):
        if(nodeID is None): nodeID = self._masterNode.get_nodeID()
        if(nodeID not in range(1, 2**16)): raise Exception("The specified nodeID, {0}, must be in the range [1, 65535]".format(nodeID))
	if(config['debug_no_sketch'] == True):
	    return random.randint(-80,-20)
        if(self.has_neighbor(nodeID)):
            entry, index =  self._networkTable.get_neighbor_entry(nodeID)
            size, data = self.pull("n{0}_2".format(index))
            if(len(data) > 3):
                return -((data[2]^0xff)+1)
            else:
                print("Node {0}.get_rssi(): Error reading RSSI for node {1}".format(self._nodeID, nodeID))
                return -1
                
        else:
            print("Node {0}.get_rssi(): Node {1} is not in the neighbor table".format(self._nodeID, nodeID))
            return -1

class VirtualNode:
    _pipe = None
    _deviceName = ""
    _nodeID = None

    def __init__(self, nodeID, name="Edison"):
        if(nodeID not in range(1, 2**16)): raise Exception("The specified nodeID, {0}, must be in the range [1, 65535]".format(nodeID))
 
        self._name = name + '-' + str(nodeID)
        self._nodeID = nodeID
        self._primaryRadio = 'edison'
        
        narray = []
        rarray = [] 
        self._networkTable = NetworkTable(narray, rarray, maxNetworkSize=255, maxNeighbors=128, maxRoutes=128) #virtual nodes have 'infinite' memory for now
   
    def get_nodeID(self):
        return self._nodeID
 
    def get_radio(self):
        return self._primaryRadio

    def get_neighbors(self):
        nodeIDs = []
        neighborTable = self._networkTable.get_neighbor_list()
        for entry in neighborTable:
            nodeIDs.append(entry[0])
        return(nodeIDs)

    def get_routes(self):
        nodeIDs = []
        routingTable = self._networkTable.get_routing_list()
        for entry in routingTable:
            nodeIDs.append(entry[0])
        return(nodeIDs)
        
    def get_neighbor_table(self):
        return (self._networkTable.get_neighbor_list())

    def get_routing_table(self):
        return (self._networkTable.get_route_list())

    def add_neighbor(self, args={}):
        self._networkTable._neighborArray.append([args['shNodeID'], args['shLQE'], args['radioID'], args['networkID']])

    def add_route(self, destID, shID):
        if(destID not in range(1, 2**16)): raise Exception("The specified destID, {0}, must be in the range [1, 65535]".format(destID))
        if(shID not in range(1, 2**16)): raise Exception("The specified shID, {0}, must be in the range [1, 65535]".format(shID))
      
        entry, index = self._networkTable.get_neighbor_entry(shID)
        if(len(entry) == 0):
            print("Error - Node {0}: Tried to add route to node {1}, but single-hop node {2} isn't a neighbor".format(self._nodeID, destID, shID))
            return -1
        else:
            self._networkTable._routingArray.append([destID, 254, index])
            return 1
   
    def has_neighbor(self, nodeID):
        if(nodeID not in range(1, 2**16)): raise Exception("The specified nodeID, {0}, must be in the range [1, 65535]".format(nodeID))
        return(nodeID in self.get_neighbors())

    def has_route(self, nodeID=None):
        if(nodeID is None): #return True if there's any route, False otherwise
            return (len(self._networkTable._routingArray) > 0)
        else: #check for a specific nodeID
            if(nodeID not in range(1, 2**16)): raise Exception("The specified nodeID, {0}, must be in the range [1, 65535]".format(nodeID))            
            return(nodeID in self.get_routes())
    
    #returns the single-hop node ID to forward a packet
    # if there is no route possible, returns -1
    # assuming no reserved LQEs, a neighbor entry beats a routing entry
    def get_forward_id(self, destID):
        if(destID not in range(1, 2**16)): raise Exception("The specified destID, {0}, must be in the range [1, 65535]".format(destID))

        forwardOptions = [] #entries are arrays w/ (nodeID, LQE)
        routingEntries = []

        neighborEntry, index = self._networkTable.get_neighbor_entry(destID)
        if(len(neighborEntry) != 0):
            if(neighborEntry[1] != 0): return neighborEntry[0] #ignore masked edges
        for entry in self.get_routing_table():
            if(entry[0] == destID):
                nEntry = self.get_neighbor_table()[entry[2]]
#                print("Try to forward through {0}, LQE = {1}".format(nEntry, nEntry[1]))
                if(nEntry[1] != 0): forwardOptions.append([nEntry[0], nEntry[1]]) #ignore masked edges

        if(len(forwardOptions) == 0):
            return -1 #no route found
        else:
            bestOption = list(forwardOptions[0])
            for pair in forwardOptions[1:]:
                if(pair[1] > bestOption[1]):
                    bestOption = list(pair)
#            print("Best option = {0}".format(bestOption[0]))
            return bestOption[0]
        raise Exception("Unhandled execution in get_forward_id for ID {0}".format(destID))

        
   
    def mask_neighbor(self, nodeID=None, action="mask"): # TODO pick a better name for the method
        maskValues = {'mask':0, 'unmask':254, 'force':255}
        if(action not in maskValues.keys()): raise Exception("The action, {0}, must be one of {1}".format(action, maskValues.keys()))

        if nodeID is None: #mask all entries
            for entry in self._networkTable._neighborArray: #FIXME breaking abstraction/protection rules here...
                print("   {0} edge from node {1} to node {2}".format(action, self._nodeID, entry[0]))
                entry[1] = maskValues[action]
        else:
            if(nodeID not in range(1, 2**16)): raise Exception("The specified nodeID, {0}, must be in the range [1, 65535]".format(nodeID))
            entry, index = self._networkTable.get_neighbor_entry(nodeID)
            if(len(entry) == 0):
                print("Error: could not find node {0} in node {1}'s neighbor table.".format(nodeID, self._nodeID))
                print("   Neighbor table = {0}".format(self._networkTable.get_neighbor_list()))
            else:
                print("   {0} the edge from node {1} to node {2}".format(action, self._nodeID, nodeID))
                entry[1] = maskValues[action]
                self.update_neighbor_LQE(entry, index)

    def update_neighbor_LQE(self, entry, index): #nothing to do for a virtual node
        print("   Node {0}: update neighbor entry at index {1} to {2}".format(self._nodeID, index, entry))
#End of VirtualNode
 
class NetworkTable:
   _neighborArray = []
   _routingArray = []
   _networkArray = []
   _maxNetworkSize = None
   _maxNeighbors = 1
   _maxRoutes = 0
   

   def __init__(self, neighborArray, routingArray, maxNetworkSize, maxNeighbors, maxRoutes):
      if(maxNetworkSize not in range(1, (2**8)+1)): raise Exception("The specified max network size, {0}, must be in the range [1, 256]".format(maxNetworkSize))
      if(maxNeighbors not in range(1, maxNetworkSize+1)): raise Exception("The specified max neighbors, {0}, must be in the range [1, {1}]".format(maxNeighbors, maxNetworkSize))
      if(maxRoutes not in range(1, maxNetworkSize+1)): raise Exception("The specified max routes {0}, must be in the range [1, {1}]".format(maxNeighbors, maxNetworkSize))

      self._neighborArray = list(neighborArray)
      self._routingArray = list(routingArray)
      self._maxNetworkSize = maxNetworkSize        
      self._maxNeighbors = maxNeighbors
      self._maxRoutes = maxRoutes


   # return a list of neighbor table entries
   def get_neighbor_list(self):
      return list(self._neighborArray)

   # returns an entry and the table index if foud
   # return empty array and -1 if node isn't found
   def get_neighbor_entry(self, nodeID):
      for entry in self._neighborArray: #entry is an array
         if(nodeID == entry[0]):
            return entry, self._neighborArray.index(entry)
      return [], -1

   def get_route_list(self):
      return list(self._routingArray)

   def set_max_neighbors(self, num):
      if(num < len(self._neighborArray)):
         print("Error: Trying to set max number of neighbors ({0}) smaller than current array ({1})".format(num, len(self._neighborArray)))
      else:
         self._maxNeighbors = num

   def set_max_routes(self, num):
      if(num < len(self._routingArray)):
         print("Error: Trying to set max number of routes ({0}) smaller than current array ({1})".format(num, len(self._routingArray)))
      else:
         self._maxRoutes = num

### FIXME the following classes aren't used ### 

class NeighborTableEntry:
   _fields = {'shNodeID':None, 'shLQE':None, 'radioID':None, 'networkID':None}
   _resolutions = {'shNodeID':16, 'shLQE':12, 'radioID':2, 'networkID':2}
   
   def __init__(self, fields, resolutions={}):
      for key in self._fields:
         if(key in fields):
            self._fields = fields[key]

         if(key in resolutions):
            self._resolutions = resolutions[key]

   def to_s(self):
      print("Neighbor fields: shNodeID = {0}, shLQE = {1}, radioID = {2}, networkID = {3}".format(self._fields['shNodeID'], self._fields['shLQE'], self._fields['radioID'], self._fields['networkID']))
      print("Neighbor field resolutions: shNodeID = {0}, shLQE = {1}, radioID = {2}, networkID = {3}".format(self._resolutions['shNodeID'], self._resolutions['shLQE'], self._resolutions['radioID'], self._resolutions['networkID']))


class RoutingTableEntry:
   _fields = {'mhNodeID':None, 'mhLQE':None, 'neighborIndex':None}
   _resolutions = {'mhNodeID':16, 'mhLQE':8, 'neighborIndex':8}
   
   def __init__(self, fields, resolutions={}):
      for key in self._fields:
         if(key in fields):
            self._fields = fields[key]
         
         if(key in resolutions):
            self._resolutions = resolutions[key]

   def to_s(self):
      print("Routing fields: mhNodeID = {0}, mhLQE = {1}, neighborIndex = {2}".format(self._fields['mhNodeID'], self._fields['mhLQE'], self._fields['neighborIndex']))
      print("Routing field resolutions: mhNodeID = {0}, mhLQE = {1}, neighborIndex = {2}".format(self._resolutions['mhNodeID'], self._resolutions['mhLQE'], self._resolutions['neighborIndex']))
     
 
class InputOutput:
   _interface = None
   _protocol = None

   def __init__(self, interface, protocol=None):
      self._interface = interface
      self._protocol = protocol

   def to_s(self):
      print("Input: interface = {0}, protocol = {1}".format(self._interface, self._protocol))


class Sensor(InputOutput):

   _name = None
   _units = None #the units of 1 LSB
   _resolution = None

   def __init__(self, name, interface, protocol):
      super(Sensor, self).__init__(interface, protocol)
      self._name = name      

   def to_s(self):
      print("Sensor: name = {0}, interface = {1}, protocol = {2}".format(self._name, super(Sensor, self)._interface, super(Sensor, self)._protocol))

