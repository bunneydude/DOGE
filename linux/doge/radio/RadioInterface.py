#import os, sys, inspect

#cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0],"../core")))
#if cmd_subfolder not in sys.path:
#	sys.path.insert(0, cmd_subfolder)

from doge.core.IPCBuffer import IPCBuffer
import doge.core.Protocol as Protocol

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
      if(destination not in range(0, 2**16)): raise Exception("The destination, {0}, must be in the range [0,65535]".format(destination))

      self.txData = Protocol.form_packet(type=1, srcID=self._nodeID, dstID=destination, cmd=command, addr=address, data=payload)
      #TODO need to import a constants file of sorts so we can use 'RAW_PACKET' instead of '1' for type, etc
      print("About to send: {0}".format(self.txData))
      if(self.debug == False):
         if(self._connected == True):
            #write out everything in txData
            for b in self.txData:
               self.cmdBuffer.write(b)
            
            print("Waiting for response")
            #get response
            self.rxData = []
            for _ in range(4): #read first 4 bytes (srcID, dstID, cmd, size)
               self.rxData.append(ord(self.rxBuffer.read()))
               #print("Got: {0}".format(self.rxData))

            for _ in range(self.rxData[-1]): #get rest of payload
               self.rxData.append(ord(self.rxBuffer.read()))
               #print("Got: {0}".format(self.rxData))
            
            # get checksun
            self.rxData.append(ord(self.rxBuffer.read()))
            #print("Got: {0}".format(self.rxData))
         else:
            print("Error - need to call connect_sketch first")
      else:
         print("Debug: send message {0}".format(self.txData))
         self.rxData = [destination, self._nodeID, 3, 2, 5, 6]
         self.rxData.append(-sum(self.rxData)%256)


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
