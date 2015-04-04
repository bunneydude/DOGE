import os.path
import json

class Device:
   _deviceConfig = ""
   _memoryMap = {}
   _mmString = {
      "device":{"size":"mm_device_size", "base":"mm_device_base"},
      "network":{"base":"mm_network_base", "size":"mm_network_size"},
      "gpio":{"base":"mm_gpio_base", "size":"mm_gpio_size"},
      "adc":{"base":"mm_adc_base", "size":"mm_adc_size"},
      "uart":{"base":"mm_uart_base", "size":"mm_uart_size"},
      "dsp":{"base":"mm_dsp_base", "size":"mm_dsp_size"}
   }

   def __init__(self, deviceConfig, memoryMapFile):
      self._deviceConfig = deviceConfig
      
      if(os.path.isfile(memoryMapFile)):
         print("Loading memory map")
         with open(memoryMapFile, 'r') as file:
                 self._memoryMap = json.load(file)
      else:
         print("Error. File {0} does not exist".format(memoryMapFile))


   def to_s(self):
      print("Device: config = {0}, version = {1}".format(self._deviceConfig, self._memoryMap))

     
   def address(self, space, offset):
      if(space in self._mmString.keys()):
         if(offset < self._memoryMap[self._mmString[space]["size"]]):
            return self._memoryMap[self._mmString[space]["base"]] + offset
      return -1
         

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



# A node consists of a device (microcontroller + firmware),
# a set of radios, inputs, and outputs
class HardwareNode:
   _device = None
   _radios = []
   _inputs = []
   _outputs = []

   def __init__(self, device, radios=[], inputs=[], outputs=[]):
      self._device = device
      self._radios = radios
      self._inputs = inputs
      self._outputs = outputs

   def to_s(self):
      print("Node: device = {0}, radio = {1}, inputs = {2}, outputs = {3}".format(self._device, self._radios, self._inputs, self._outputs))

   
class NetworkTable:
   _neighborArray = []
   _routingArray = []
   _networkArray = []
   _maxNetworkSize = 1
   _maxNeighbors = 1
   _maxRoutes = 0


   def __init__(self, neighborArray = [], routingArray = []):
      print("Network table created")

   def get_neighbors(self):
      return self._neighborArray.copy()

   def get_routes(self):
      return self._routingArray.copy()

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
      
