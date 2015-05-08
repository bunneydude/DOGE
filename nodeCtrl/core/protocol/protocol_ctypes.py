from ctypes import *

#mirror the layout of the C header files when possible

### start of packet.h

# max sizes
MAX_RAW_PACKET_PAYLOAD_SIZE = 19
MAX_PACKET_PAYLOAD_SIZE = 20

#packet type enum
UNDEFINED_PACKET_TYPE = 0,
RAW_PACKET = 1
SIGNALING_BROADCAST_BEACON = 2
SIGNALING_UNICAST_BEACON = 3
BULK_DATA_READ_REQ = 4
BULK_DATA_READ_DATA = 5
BULK_DATA_WRITE_REQ = 6
BULK_DATA_WRITE_DATA = 7
BULK_DATA_RW_ABORT = 8
LINK_LAYER_PACKET = 9
MAX_PACKET_TYPE = 10

#response status enum
NO_TRANSMIT = 0
TRANSMIT_RESPONSE = 1

# packet structures
class packetHdr(Structure):
   _fields_ = [("type",   c_ubyte),
               ("txInfo", c_ubyte),
               ("src",    c_ushort),
               ("dst",    c_ushort),
               ("shSrc",  c_ushort),
               ("shDst",  c_ushort),
               ("ttl",    c_ubyte),
               ("crc",    c_ubyte)]

HEADER_SIZE = 12

class rawPacket(Structure):
   _fields_ = [("hdr", packetHdr),
               ("size", c_ubyte),
               ("data", c_ubyte * MAX_RAW_PACKET_PAYLOAD_SIZE)]

class packetAck(Structure):
   _fields_ = [("hdr", packetHdr),
               ("errorCode", c_ubyte)]

class dogePacket(Structure):
   _fields_ = [("hdr", packetHdr),
               ("payload", c_ubyte * MAX_PACKET_PAYLOAD_SIZE)]

# field masks and shifts
HEADER_TYPE_ACK_MASK       = 0x80
HEADER_TYPE_ACK_SHIFT      = 7
HEADER_TYPE_MASK           = 0x7F
HEADER_TYPE_SIZE_MASK      = 0x7F
HEADER_TYPE_SHIFT          = 0
TXINFO_PACKET_ID_MASK      = 0xF0
TXINFO_PACKET_ID_SHIFT     = 4
TXINFO_PACKET_ID_SIZE_MASK = 0xF
TXINFO_RTA_MASK            = 0x0F
TXINFO_RTA_SHIFT           = 0
TXINFO_RTA_SIZE_MASK       = 0xF

# The set macros are used by the C functions and shouldn't be called directly by python:
# - set_header_type_ack
# - set_header_type
# - set_txinfo_packet_id
# - set_txinfo_rta


# get methods
def get_header_type(hdrType):
   return (hdrType & HEADER_TYPE_MASK) >> HEADER_TYPE_SHIFT

def get_header_type_ack(hdrType):
   return (hdrType & HEADER_TYPE_ACK_MASK) >> HEADER_TYPE_ACK_SHIFT

def get_txinfo_packet_id(txinfo):
   return (txinfo & TXINFO_PACKET_ID_MASK) >> TXINFO_PACKET_ID_SHIFT

def get_txinfo_rta(txinfo):
   return (txinfo & TXINFO_RTA_MASK) >> TXINFO_RTA_SHIFT

def is_header_type_ack(hdrType):
   return (hdrType & HEADER_TYPE_ACK_MASK) == HEADER_TYPE_ACK_MASK

#header_type_equals can be replaced by calling get_header_type

#python doesn't currently need the defines for field offsets

### end packet.h


### start protocol.h

MAX_PAYLOAD_SIZE = 14

CMD_READ_REG_DATA_SIZE  = 2
CMD_WRITE_REG_DATA_SIZE = 3
CMD_ACK_DATA_SIZE       = 3
CMD_NACK_DATA_SIZE      = 3
CMD_NOP_DATA_SIZE       = 1


#command enum
CMD_READ_REG = 0x1
CMD_WRITE_REG = 0x2
CMD_ACK = 0x3
CMD_NACK = 0x4
CMD_NOP = 0x5
CMD_READ_MEM = 0x6
CMD_WRITE_MEM = 0x7
CMD_ADC_SINGLE = 0x8
CMD_ADC_RESULT = 0x9
CMD_ADC_LOOP = 0xA
CMD_ADC_END = 0xB
CMD_SPI = 0xC
CMD_READ_MEM_ACK = 0xD
CMD_WRITE_MEM_ACK = 0xE


# error codes
ERROR_PROTOCOL_BASE = 0 #from nodeCtrl_errno.h

ERR_CHECKSUM = ERROR_PROTOCOL_BASE + 1
ERR_COMMAND = ERROR_PROTOCOL_BASE + 2
ERR_FEATURE_WIP = ERROR_PROTOCOL_BASE + 3
ERR_CAT = ERROR_PROTOCOL_BASE + 4
ERR_RANGE = ERROR_PROTOCOL_BASE + 5
ERR_TIMEOUT = ERROR_PROTOCOL_BASE + 6

# protocol structures
class protocolState(Structure): #naming issue if this is just called 'Protocol'
   _fields_ = [("dataRegisters",   c_ubyte)]

class appPacket(Structure):
   _fields_ = [("cmd", c_ubyte),
               ("addr", c_ubyte),
               ("data", c_ubyte),
               ("byteNumber", c_ubyte),
               ("payload", c_ubyte * MAX_PAYLOAD_SIZE)]

class packetAttr(Structure):
   _fields_ = [("ack", c_ubyte),
               ("size", c_ubyte)]

### end protocol.h


# misc
SUCCESS = 0
ERROR = -1


### python-specific helper functions

def print_structure(struct):
   return ", ".join(f_name + " = " + str(getattr(struct, f_name)) for f_name, f_type in struct._fields_)

def packetToList(struct):
   toSend = []
   list((toSend.append(getattr(struct.hdr, f_name))) for f_name, f_type in struct.hdr._fields_) 
   toSend.append(struct.size)
   list((toSend.append(x)) for x in (list(struct.data)[0:struct.size]))
   return toSend


def packetToBytes(struct):
   toSend = buffer(struct)[0:HEADER_SIZE + 1 + struct.size]
   return list(toSend)
