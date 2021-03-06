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
   READ_REG_ACK_SIZE_COBS = 18
   VALID_PACKET_SIZES = [ READ_REG_ACK_SIZE_COBS, 16, 17 ]

   def __init__(self, name, nodeID, debug=False, logLevel=2):
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
      self.rxPacket = ProtocolDefs.rawPacket()

   # Open the IPC objects to communicate w/ the Arduino sketch
   def connect_sketch(self):
      if(self.debug == False):
         if(self._connected == False):
            self.cmdBuffer.open_sketch()
            self.rxBuffer.open_sketch()
            self._connected = True
         else:
            print("IPC objects already connected")
      else:
         print("In debug mode the sketch is not connected")

   # TODO this method should take a list of bytes instead of forming the packet within. Currently it breaks abstraction between the interface and protocol used
   def proxy_send(self, destination, command, address, payload, singleHopDest=None, cbytes=None):
      if(singleHopDest is None): singleHopDest = self._nodeID
      if(destination not in range(0, 2**16)): raise Exception("The destination, {0}, must be in the range [0,65535]".format(destination))
      if(singleHopDest not in range(0, 2**16)): raise Exception("The single-hop destination, {0}, must be in the range [0,65535]".format(singleHopDest))

      self.txData = Protocol.form_packet(type=ProtocolDefs.RAW_PACKET, srcID=self._nodeID, dstID=destination, shSrcID=self._nodeID, shDstID=singleHopDest, cmd=command, addr=address, data=payload, bytes=cbytes, enc='bytes')
      if self._logLevel >= 2: 
         txPacket = Protocol.parse_packet(self.txData)
         print("About to send: [header: [{0}], size = {1}, data = {2}]".format(ProtocolDefs.print_structure(txPacket.hdr), txPacket.size, list(i for i in txPacket.data)))

      encData = cobs.encode(''.join(self.txData))
      encData = list(ord(x) for x in encData) 

      if self._logLevel >= 3: 
         print("   Raw: {0}".format(["{:02X}".format(int(x.encode("HEX"), 16)) for x in self.txData]))
         print("   Encoded: {0}".format(["{:02X}".format(x) for x in encData]))

      if(self.debug == False):
         if(self._connected == True):
            #write out everything in encData
            for b in encData:
               self.cmdBuffer.write(b)
            self.cmdBuffer.write(0) #needed to mark end of frame
         else:
            print("Error - need to call connect_sketch first")

   # Pull bytes out of the stream, decode w/ COBS, and store the result in self.rxPacket
   def proxy_receive(self):
      self.rxData = []
      encData = []
      timeout = 2000
      duration = 0
      if(self.debug == False):
         while(duration < timeout):
            if self._logLevel >= 3: print("   Available bytes = {0}".format(self.rxBuffer.available()))
            if(self.rxBuffer.available() > 0): 
               if (self.rxBuffer.available() not in RadioInterface.VALID_PACKET_SIZES):
                  print("   Unknown packet size detected: {}. Flushing serial buffer...".format(self.rxBuffer.available()))
                  while(self.rxBuffer.available() != 0):
                     self.rxBuffer.read()
                  continue
               encData.append(ord(self.rxBuffer.read()))
               if(encData[0] == 0): #caught the end of a previous frame
                  return 0
      
               while(encData[-1] != 0): #get bytes until end of frame
                  encData.append(ord(self.rxBuffer.read()))

               encData = encData[0:-1] #remove trailing 0

               tempData = list(cobs.decode(''.join(struct.pack('<B',x) for x in encData)))
               self.rxPacket = Protocol.parse_packet(tempData)
               self.rxData = list(ord(x) for x in tempData)

               if self._logLevel >= 3: 
                  print("   Encoded: {0}".format(["{:02X}".format(x) for x in encData]))
                  print("   Raw: {0}".format(["{:02X}".format(x) for x in self.rxData]))
               
               if(self.rxPacket.data[0] == ProtocolDefs.CMD_NACK):
                  print("   Radio proxy node returned an error: {}".format(["{:02X}".format(x) for x in self.rxPacket.data]))
                  return 0
                  
               return 1
            else:
               duration += 100
               time.sleep(0.1)
         if(duration >= timeout):
            print("RadioInterface.proxy_receive: Timeout")
            self.rxPacket = ProtocolDefs.rawPacket()
            self.rxData = []
            return -1
      else:
         tempData = Protocol.form_packet(type=1, srcID=6, dstID=self._nodeID, shSrcID=6, shDstID=self._nodeID, cmd=ProtocolDefs.CMD_ACK, addr=1, data=2, enc='bytes')
         self.rxPacket = Protocol.parse_packet(tempData)
         self.rxData = list(ord(x) for x in tempData)
         return 1
