# gcc -shared -I. -I.. -shared -o libprotocol.so -fpic protocol.c

from ctypes import *
libprotocol = CDLL('./libprotocol.so')

class Protocol(Structure):
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


