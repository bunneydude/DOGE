from ctypes import *

MAX_PAYLOAD_SIZE = 16
CMD_READ_REG_DATA_SIZE  = 2
CMD_WRITE_REG_DATA_SIZE = 3
CMD_ACK_DATA_SIZE       = 3
CMD_NACK_DATA_SIZE      = 3
CMD_NOP_DATA_SIZE       = 1


MAX_RAW_PACKET_PAYLOAD_SIZE = 23
ERR_CHECKSUM = 1
SUCCESS = 0
ERROR = -1

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
MAX_PACKET_TYPE = 9

#response status enum
NO_TRANSMIT = 0
TRANSMIT_RESPONSE = 1

def header_type_ack(hdrType):
   return (hdrType & HEADER_TYPE_ACK_MASK) >> HEADER_TYPE_ACK_SHIFT

def header_type_pkt(hdrType):
   return (hdrType & HEADER_TYPE_MASK) >> HEADER_TYPE_SHIFT

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

class packetHdr(Structure):
   _fields_ = [("type",   c_ubyte),
               ("txInfo", c_ubyte),
               ("src",    c_ushort),
               ("dst",    c_ushort),
               ("ttl",    c_ubyte),
               ("crc",    c_ubyte)]

class rawPacket(Structure):
   _fields_ = [("hdr", packetHdr),
               ("size", c_ubyte),
               ("data", c_ubyte * MAX_RAW_PACKET_PAYLOAD_SIZE)]

class packetAck(Structure):
   _fields_ = [("hdr", packetHdr),
               ("errorCode", c_ubyte)]

class protocolState(Structure):
   _fields_ = [("dataRegisters",   c_ubyte)]

class appPacket(Structure):
   _fields_ = [("cmd", c_ubyte),
               ("addr", c_ubyte),
               ("data", c_ubyte),
               ("byteNumber", c_ubyte),
               ("payload", c_ubyte * MAX_PAYLOAD_SIZE),
               ("reserved", c_ubyte * 2)]

class packetAttr(Structure):
   _fields_ = [("ack", c_ubyte),
               ("size", c_ubyte)]

def print_structure(struct):
   return ", ".join(f_name + " = " + str(getattr(struct, f_name)) for f_name, f_type in struct._fields_)

