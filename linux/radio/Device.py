import json
import sys

class Device:
   _deviceConfig = ""
   _memoryMapVersion = 0

   def __init__(self, deviceConfig, memoryMapVersion):
      self._deviceConfig = deviceConfig
      self._memoryMapVersion = memoryMapVersion

   def to_s(self):
      print("Device: config = {0}, version = {1}".format(self._deviceConfig, self._memoryMapVersion))
     

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
class Node:
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
