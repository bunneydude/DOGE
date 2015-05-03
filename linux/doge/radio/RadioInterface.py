from doge.core.IPCBuffer import IPCBuffer
import doge.core.Protocol as Protocol
import doge.core.protocol_ctypes as ProtocolDefs
import time
from cobs import cobs
import struct

class RadioInterface():
   _connected = False
   _name = None
   _nodeID = 1
   _logLevel = 1

   def __init__(self, name, nodeID, debug=False, logLevel=3):
      if(not isinstance(name, str)): raise Exception("The name must be a string")
      if(nodeID < 0 or nodeID > 255): raise Exception("The nodeID, {0}, must be in the range [0,255]".format(nodeID))
      if(logLevel not in range(1,4)): raise Exception("The log level must be in the range [1,3]")

      self._name = name
      self._nodeID = nodeID
      self._logLevel = logLevel

      self.cmdBuffer = IPCBuffer(3)
      self.rxBuffer = IPCBuffer(4)
      self.debug = debug
      
      self.txData = []
      self.rxData = []
      self.rxPacket = None

   def connect_sketch(self):
      if(self.debug == False):
         self.cmdBuffer.open_sketch()
         self.rxBuffer.open_sketch()
         self._connected = True #TODO error handling
      else:
         print("In debug mode the sketch is not connected")

   def proxy_send(self, destination, command, address, payload):
      if(destination not in range(0, 2**16)): raise Exception("The destination, {0}, must be in the range [0,65535]".format(destination))

      self.txData = Protocol.form_packet(type=1, srcID=self._nodeID, dstID=destination, shSrcID=self._nodeID, shDstID=self._nodeID, cmd=command, addr=address, data=payload, enc='bytes')
      #TODO need to import a constants file of sorts so we can use 'RAW_PACKET' instead of '1' for type, etc
      if self._logLevel >= 2: print("   About to send: {0}".format(list(ord(x) for x in self.txData)))
      encData = cobs.encode(''.join(self.txData))
#      print("   Encoded: {0}".format(list(encData)))
      encData = list(ord(x) for x in encData) 
      if self._logLevel >= 2: print("   Encoded: {0}".format(encData))

      if(self.debug == False):
         if(self._connected == True):
            #write out everything in encData
            for b in encData:
               self.cmdBuffer.write(b)
            self.cmdBuffer.write(0) #needed to mark end of frame
         else:
            print("Error - need to call connect_sketch first")

   def proxy_receive(self):
      self.rxData = []
      encData = []
      timeout = 2000
      duration = 0
      if(self.debug == False):
         while(duration < timeout):
            print("   Available bytes = {0}".format(self.rxBuffer.available())) 
            if(self.rxBuffer.available() > 0): 
               encData.append(ord(self.rxBuffer.read()))
               if(encData[0] == 0): #caught the end of a previous frame
                  return 0
      
               while(encData[-1] != 0): #get bytes until end of frame
                  encData.append(ord(self.rxBuffer.read()))

               encData = encData[0:-1] #remove trailing 0
               print(" Got encData: {0}".format(list(encData)))
               self.rxData = cobs.decode(''.join(struct.pack('<B',x) for x in encData))
               self.rxData = list(ord(x) for x in self.rxData)
               break
            else:
               duration += 100
               time.sleep(0.1)
         if(duration >= timeout):      
            print("Timeout")      
      else:
         self.rxData = Protocol.form_packet(type=1, srcID=6, dstID=self._nodeID, shSrcID=6, shDstID=self._nodeID, cmd=ProtocolDefs.CMD_ACK, addr=1, data=2, enc='fields')
         self.rxPacket = Protocol.parse_packet(self.rxData)
         return 1
