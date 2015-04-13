#import os, sys, inspect

#cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0],"../core")))
#if cmd_subfolder not in sys.path:
#	sys.path.insert(0, cmd_subfolder)

from doge.core.IPCBuffer import IPCBuffer
import doge.core.Protocol as Protocol
from cobs import cobs
import struct


WRITE = 2
READ = 1
SREG_TARGET = 7
SREG_PING = 11
PING = 1
SREG_TEMPERATURE = 4

class RadioInterface():
   _connected = False
   _name = None
   _nodeID = 1

   def __init__(self, name, nodeID, debug=False):
      if(not isinstance(name, str)): raise Exception("The name must be a string")
      if(nodeID < 0 or nodeID > 255): raise Exception("The nodeID, {0}, must be in the range [0,255]".format(nodeID))

      self._name = name
      self._nodeID = nodeID

      self.cmdBuffer = IPCBuffer(3)
      self.rxBuffer = IPCBuffer(4)
      self.debug = debug
      
      self.txData = []
      self.rxData = []

   def connect_sketch(self):
      if(self.debug == False):
         self.cmdBuffer.open_sketch()
         self.rxBuffer.open_sketch()
         self._connected = True #TODO error handling
      else:
         print("In debug mode the sketch is not connected")

   def proxy_send(self, destination, command, address, payload):
      if(destination < 0 or destination > 255): raise Exception("The destination, {0}, must be in the range [0,255]".format(destination))

      self.txData = [self._nodeID, destination] + Protocol.form_packet(cmd=command, addr=address, data=payload)
      print("About to send: {0}".format(self.txData))
      encData = cobs.encode(''.join(struct.pack('<B',x) for x in self.txData))
      print("   Encoded: {0}".format(list(encData)))
      encData = list(ord(x) for x in encData) 
      print("   Encoded: {0}".format(encData))

      if(self.debug == False):
         if(self._connected == True):
            #write out everything in encData
            for b in encData:
               self.cmdBuffer.write(b)
         else:
            print("Error - need to call connect_sketch first")
      else:
         print("Debug: send message {0}".format(self.txData))
         self.rxData = [destination, self._nodeID, 3, 2, 5, 6]
         self.rxData.append(-sum(self.rxData)%256)

   def proxy_receive(self):
      self.rxData = []
      encData = []
      encData.append(ord(self.rxBuffer.read()))
      if(encData[0] == 0): #caught the end of a previous frame
         return 0
      
      while(encData[-1] != 0):
         encData.append(ord(self.rxBuffer.read()))

      self.rxData = cobs.decode(''.join(struct.pack('<B',x) for x in encData))
      self.rxData = list(ord(x) for x in self.rxData)

   def push(self, network, nodeID, data):      
      # set target - write nodeID to SREG_TARGET
      self.proxy_send(WRITE, SREG_TARGET, nodeID)

      # set data - write data to address
      self.proxy_send(WRITE, SREG_TEMPERATURE, data)
      
      # transmit - write to SREG_PING
      self.proxy_send(WRITE, SREG_PING, 0x2)

      if(self.debug == False):
         if(self.rxData[0] == 4):
            print("\nError for node " + str(network) + " " + str(nodeID) + ": " + str(self.rxData[3])  + "\n")


   def pull(self, network, nodeID, field):
      fieldDecode = {"rssi":0x0, "temperature":0x4}
      
      # set target
      self.proxy_send(WRITE, SREG_TARGET, nodeID)

      #transmit
      self.proxy_send(WRITE, SREG_PING, fieldDecode[field.lower()] | 0x1)

      if(self.debug == False):
         if(self.rxData[0] == 4):
            print("\nError for node " + str(network) + " " + str(nodeID) + ": " + str(self.rxData[3])  + "\n")
            return -1
         else:
            return self.rxData[3]
      else:
         return 1
