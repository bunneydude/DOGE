import os.path
import json
import RadioInterface
import doge.core.protocol_ctypes as ProtocolDefs
from __builtin__ import file
import random
from doge.conf.globals import config

#The Device class represents a microcontroller and its firmware.
class Device:
   _deviceName = ""
   _memoryMap = {}
   _validDeviceNames = ["msp430g2553", "lpc812"]

   _mmString = {
      "device":{"size":"mm_device_size", "base":"mm_device_base"},
      "network":{"base":"mm_network_base", "size":"mm_network_size"},
      "gpio":{"base":"mm_gpio_base", "size":"mm_gpio_size"},
      "adc":{"base":"mm_adc_base", "size":"mm_adc_size"},
      "uart":{"base":"mm_uart_base", "size":"mm_uart_size"},
      "dsp":{"base":"mm_dsp_base", "size":"mm_dsp_size"}
   }

   """ Create the device based on the microcontroller name and memory map
   @param deviceName : String : The name of the microcontroller. Must be one of:
      * msp430g2553
      * lpc812
   @param memoryMapFile : String : File name containing a json object that describes the base and sizes of the differet memory sections
   """
   def __init__(self, deviceName, memoryMapFile):
      # Validate inputs
      if(deviceName.lower() not in self._validDeviceNames): raise Exception("deviceName {0} is invalid. Must be one of {1}".format(deviceName.lower(), self._validDeviceNames))
      if(not os.path.isfile(memoryMapFile)): raise Exception("File {0} does not exist".format(memoryMapFile))

      # assign inputs
      self._deviceName = deviceName.lower()
      
      print("Loading memory map")
      with open(memoryMapFile, 'r') as file:
         self._memoryMap = json.load(file)

   """ Print a string representation of the object """
   def to_s(self):
      print("Device: config = {0}, version = {1}".format(self._deviceName, self._memoryMap))

   """ Return the physical address of the register in the specified memory space
   @param space : String : The memory space to access. Must be one of:
      * "device"
      * "network"
      * "gpio"
      * "adc"
      * "uart"
      * "dsp:

   @param register : Integer : Offset for the register you want to access. A full list is in mmFields.py.
   
   @returns Integer : Returns -1 if the regsister offset was outside the range of the memory space. Otherwise, returns the physical address.
   """
   def address(self, space, register):
      if(space in self._mmString.keys()):
         if(register < self._memoryMap[self._mmString[space]["size"]]):
            return self._memoryMap[self._mmString[space]["base"]] + register
      return -1
         



# A node consists of a device (microcontroller + firmware),
# a set of radios, inputs, and outputs
class HardwareNode:
   _pipe = None
   _device = None
   _radios = []
   _inputs = {}
   _outputs = []
   _networkTable = None	   
   _validRadios = ["nrf24", "cc1101l", "rfm69"]
   _maxRadios = 4
   _nodeID = None

   def __init__(self, device, nodeID, pipe):
      print "Device={0}, pipe={1}".format(device,pipe)
      if(not isinstance(device, Device)): raise Exception("The device argument must be an instance of Node.Device.")
      if(nodeID not in range(1, 2**16)): raise Exception("The nodeID, {0}, must be in the range [1, 65535]".format(nodeID))
      if(not isinstance(pipe, RadioInterface.RadioInterface)): raise Exception("The device argument must be an instance of Node.Device.")

      self._device = device
      self._nodeID = nodeID
      self._pipe = pipe
      narray = []
      rarray = []

      self._networkTable = NetworkTable (narray,rarray)

   def to_s(self):
      print("Node: device = {0}, nodeID = {1}, radio = {2}, inputs = {3}, outputs = {4}".format(self._device, self._nodeID, self._radios, self._inputs, self._outputs))

   def get_nodeID(self):
      return self._nodeID

   def add_radio(self, radio):
      if(radio.lower() not in self._validRadios): raise Exception("Invalid radio specified: {0}. Must be one of {1}".format(radio.lower(), self._validRadios))
      if(len(self._radios) == self._maxRadios): raise Exception("This node has exceeded the max number of radios: {0}".format(self._maxRadios))

      self._radios.append(radio.lower())


   def add_sensor(self, name, registerSpace, registerOffset):
      if(name.lower() in self._inputs.keys()): raise Exception("That sensor name, {0}, is already in use. Current sensors are {1}".format(name.lower(), self._inputs.keys()))
      if(registerSpace.lower() not in self._device._mmString.keys()): raise Exception("Invalid registerSpace specified: {0}. Must be one of: {1}".format(registerSpace.lower(), self._device._mmString.keys()))
      
      address = self._device.address(registerSpace, registerOffset)
      if(address == -1): raise Exception("Offset {0} in space {1} is out of range. Please consult the memory map file".format(registerOffset, registerSpace))
      
      self._inputs[name.lower()] = {"space":registerSpace.lower(), "offset":registerOffset}


   def pull(self, sensorName):
       if(sensorName.lower() not in self._inputs.keys()): raise Exception("Unknown sensor: {0}. Current sensor list: {1}".format(sensorName.lower(), self._inputs.keys()))
      
       returnData = {}
       address = self._device.address(self._inputs[sensorName.lower()]["space"], self._inputs[sensorName.lower()]["offset"])

       self._pipe.proxy_send(destination=self._nodeID, command=ProtocolDefs.CMD_READ_REG, address=address, payload=0, singleHopDest=2)
       self._pipe.proxy_receive()
       print("Got: {0}".format(self._pipe.rxData))
       print("Pull complete. Got: [header: [{0}], size = {1}, data = {2}]".format(ProtocolDefs.print_structure(self._pipe.rxPacket.hdr), self._pipe.rxPacket.size, list(i for i in self._pipe.rxPacket.data)))


   def get_neighbor_table(self):
       network_table =  self._networkTable 
       return (network_table.get_neighbors(self._nodeID))
  
   def get_neighbors(self):
       node_array = []
       network_table = self._networkTable 
       neighbor_table_array = network_table.get_neighbors(self._nodeID)
       for x in  neighbor_table_array:
           node_array.append(x[0])
       return (node_array)

   def get_routing_table(self):
       network_table =  self._networkTable 
       return (network_table.get_routes(self._nodeID))
  

   def get_rssi(self):
       rssi = random.randint(20,30)
       return(rssi)


class VirtualNode:
    _pipe = None
    _deviceName = ""
    _nodeID = None

    def __init__(self, nodeID, name="Edison"):
        if(nodeID not in range(0, 2**16)): raise Exception("The specified nodeID, {0}, must be in the range [0, 65535]".format(nodeID))
 
        self._name = name + '-' + str(nodeID)
        self._nodeID = nodeID
        narray = []
        rarray = [] 
        self._networkTable = NetworkTable(narray,rarray)
   
    def get_nodeID(self):
        return self._nodeID
 
    def load_preset_nte_config(self, pipe):
        nte_nodes = []
        for nte_entry in config['preset_nte_nodes']:
            device = Device(deviceName=nte_entry['mcu_name'],memoryMapFile=config['config_file_paths']['mm_map_default_profile'])
            nte_nodes.append(
		HardwareNode(
                    device,
                    nodeID=nte_entry['node_id'], 
                    pipe=pipe
                )
            )
        return nte_nodes

    def get_neighbors(self):
        node_array = self.load_preset_nte_config()
        network_table = self._networkTable = NetworkTable(neighborArray=node_array,routingArray=[])
        return node_array
        
    def get_neighbor_table(self):
        network_table =  self._networkTable 
        return (network_table.get_neighbors(self._nodeID))

    def get_routing_table(self):
        network_table =  self._networkTable 
        return (network_table.get_routes(self._nodeID))


 
class NetworkTable:
   _neighborArray = []
   _routingArray = []
   _networkArray = []
   _maxNetworkSize = 1
   _maxNeighbors = 1
   _maxRoutes = 0


   def __init__(self, neighborArray = [], routingArray = []):
      print("Network table created")

   def get_neighbors(self,nodeID):
      if(config['debug'] is True):
         if(nodeID not in range(0, 2**16)): raise Exception("The node ID, {0}, must be in the range [0,65535]".format(nodeID))
         if (nodeID == 2 ):
            narray =  [[1,74,2,1]]
         elif (nodeID == 3 ):
            narray =  [[1,44,2,1]]
         elif (nodeID == 4 ):
            narray =  [[1,66,2,1]]
         elif (nodeID == 5 ):
            narray =  [[1,96,2,1]]
         else:
            raise exception("Unexpected node ID: {0}".format(nodeID))
            narray=[]
         return (narray)
      else:
         raise exception("Not implemented")
         #return self._neighborArray.copy()

   def get_routes(self, nodeID):
      if(config['debug'] is True):
         if(nodeID not in range(0, 2**16)): raise Exception("The node ID, {0}, must be in the range [0,65535]".format(nodeID))
         elif (nodeID == 2 ):
            rarray =  []
         elif (nodeID == 3 ):
            rarray =  [[2, 1, 9]]
         elif (nodeID == 4 ):
            rarray =  []
         elif (nodeID == 5 ):
            rarray =  []
         else:
            raise exception("Unexpected node ID: {0}".format(nodeID))
            rarray = []
         return (rarray)
      else:   
         raise exception("not implemented")
         #return self._routingArray.copy()

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

