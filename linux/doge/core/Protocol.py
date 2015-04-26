from ctypes import *
from protocol_ctypes import *

libprotocol = CDLL("./doge/core/libprotocol.so")

def form_packet(type=RAW_PACKET, srcID=1, dstID=1, cmd=CMD_READ_REG, addr=0, data=0, enc="bytes"):
   validEncodings = ["bytes", "fields"]

   #TODO generalize error check for type and cmd to list valid names instead of the numeric range
   if(type not in range(1, MAX_PACKET_TYPE)): raise Exception("The packet type, {0}, must be in the range [1, 8]".format(type))
   if(srcID not in range(0, 2**16)): raise Exception("The source ID, {0}, must be in the range [0, 65535]".format(srcID))
   if(dstID not in range(0, 2**16)): raise Exception("The destination ID, {0}, must be in the range [0, 65535]".format(dstID))
   if(cmd not in range(1, 6)): raise Exception("The command, {0}, must be in the range [1, 5]".format(cmd))
   if(addr not in range(0, 2**8)): raise Exception("The address, {0}, must be in the range [0, 255]".format(addr))
   if(data not in range(0, 2**8)): raise Exception("The data, {0}, must be in the range [0, 255]".format(data))
   if(enc not in validEncodings): raise Exception("The encoding must be either 'bytes' or 'int'")

   appPkt = appPacket()
   rawPkt = rawPacket()
   attr = packetAttr()

   libprotocol.application_form_packet(byref(rawPkt.data), byref(attr), cmd, addr, data)
   libprotocol.link_layer_form_packet(byref(rawPkt), byref(attr), type, srcID, dstID)
   
#   print("\n\tPacket: \n\t[{0}], size = {1}, data = {2}".format(print_structure(rawPkt.hdr), rawPkt.size, list(i for i in rawPkt.data)))

   if(enc.lower() == "bytes"):
      toSend = packetToBytes(rawPkt)
   elif(enc.lower() == "fields"):
      toSend = packetToList(rawPkt)
   else:
      raise Exception("Unknown value for enc: {0}".format(enc))

   return toSend
 

def parse_packet(data)



def receive_packet(stream, registers):
   raise Exception("Protocol.receive_packet has been deprecated.")
