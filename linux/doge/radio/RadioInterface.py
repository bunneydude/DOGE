#import os, sys, inspect

#cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0],"../core")))
#if cmd_subfolder not in sys.path:
#	sys.path.insert(0, cmd_subfolder)

from doge.core.IPCBuffer import IPCBuffer
import doge.core.Protocol as Protocol

WRITE = 2
SREG_TARGET = 7
SREG_PING = 11
PING = 1
SREG_TEMPERATURE = 4

class RadioInterface():

   def __init__(self, name, debug=False):
      self.name = name
      self.cmdBuffer = IPCBuffer(3)
      self.rxBuffer = IPCBuffer(4)
      self.debug = debug
      
      self.txData = []
      self.rxData = []

   def connect_sketch(self):
      self.cmdBuffer.open_sketch()
      self.rxBuffer.open_sketch()

   def proxy_send(self, command, address, payload):
      self.txData = Protocol.form_packet(cmd=command, addr=address, data=payload)
      if(self.debug == False):
         for b in self.txData:
            self.cmdBuffer.write(b)
      
         #get response
         self.rxData = []
         self.rxData.append(ord(self.rxBuffer.read()))
         self.rxData.append(ord(self.rxBuffer.read()))

         for i in range(self.rxData[-1]):
            self.rxData.append(ord(self.rxBuffer.read()))
         self.rxData.append(ord(self.rxBuffer.read()))

      else:
         print("Debug: send message {0}".format(self.txData))


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
