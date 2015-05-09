from ctypes import *
from protocol_ctypes import *
import warnings

libprotocol = CDLL("./doge/core/libprotocol.so")

def form_packet(type=RAW_PACKET, srcID=1, dstID=1, shSrcID=1, shDstID=1, cmd=CMD_READ_REG, addr=0, data=0, enc="bytes", memBytes=None):
   validEncodings = ["bytes", "fields"]

   #TODO generalize error check for type and cmd to list valid names instead of the numeric range
   if(type not in range(1, MAX_PACKET_TYPE)): raise Exception("The packet type, {0}, must be in the range [1, {}]".format(type, MAX_PACKET_TYPE))
   if(srcID not in range(0, 2**16)): raise Exception("The source ID, {0}, must be in the range [0, 65535]".format(srcID))
   if(dstID not in range(0, 2**16)): raise Exception("The destination ID, {0}, must be in the range [0, 65535]".format(dstID))
   if(shSrcID not in range(0, 2**16)): raise Exception("The single-hop source ID, {0}, must be in the range [0, 65535]".format(srcID))
   if(shDstID not in range(0, 2**16)): raise Exception("The single-hop destination ID, {0}, must be in the range [0, 65535]".format(dstID))
   if(cmd not in range(1, CMD_ADC_SINGLE)): raise Exception("The command, {0}, must be in the range [1, {}]".format(cmd, CMD_ADC_SINGLE))
   if(addr not in range(0, 2**8)): raise Exception("The address, {0}, must be in the range [0, 255]".format(addr))
   if(data not in range(0, 2**8)): raise Exception("The data, {0}, must be in the range [0, 255]".format(data))
   if(enc not in validEncodings): raise Exception("The encoding must be either 'bytes' or 'int'")

   appPkt = appPacket()
   rawPkt = rawPacket()
   attr = packetAttr()

   libprotocol.application_form_packet(byref(rawPkt.data), byref(attr), cmd, addr, data)
   libprotocol.link_layer_form_packet(byref(rawPkt), byref(attr), type, srcID, dstID, shSrcID, shDstID)
   
#   print("\n\tPacket: \n\t[{0}], size = {1}, data = {2}".format(print_structure(rawPkt.hdr), rawPkt.size, list(i for i in rawPkt.data)))

   if(enc.lower() == "bytes"):
      toSend = packetToBytes(rawPkt)
   elif(enc.lower() == "fields"):
      toSend = packetToList(rawPkt)
   else:
      raise Exception("Unknown value for enc: {0}".format(enc))

   return toSend
 

# Expects a list of ints or strings
def parse_packet(data):
   if(type(data) is not list): raise Exception("Input must be a list. It was instead a {0}".format(type(data)))
   if(len(data) == 0): raise Exception("Input list cannot be empty")
   if( (type(data[0]) is not str) and (type(data[0]) is not int)): raise Exception("The input list must consist of only ints or strings")
   #TODO if input is a list of ints, you'll need to unpack 2-byte fields (e.g. nodeIDs) into two separate bytes

   # convert to a list of strings
   if(type(data[0]) is int):
      data = list(chr(x) for x in data)

   # concat
   data = ''.join(data)

   newPacket = rawPacket()
   length = min(len(data), sizeof(newPacket))
   if(len(data) > sizeof(newPacket)): warnings.warn("Input data larger than raw packet object")

   # move data into packet
   memmove(addressof(newPacket), data, length)
   
   return newPacket

def receive_packet(stream, registers):
   raise Exception("Protocol.receive_packet has been deprecated.")
