from ctypes import *
libpacket = CDLL('./libpacket.so')

MAX_RAW_PACKET_PAYLOAD_SIZE = 3
ERR_CHECKSUM = 1
SUCCESS = 0
ERROR = -1

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


def print_structure(struct):
   return ", ".join(f_name + " = " + str(getattr(struct, f_name)) for f_name, f_type in struct._fields_)

print("--- Initial values ---")
hdr1 = packetHdr(1, 2, 3, 4, 5, 0)
print("Header 1")
print(print_structure(hdr1))
print("")

print("Header 2")
hdr2 = packetHdr(11, 12, 13, 14, 15, 0)
print(print_structure(hdr2))
print("")

print("Packet 1")
pkt1 = rawPacket(hdr1, 6, (7,8,9))
print("Header = [{0}], size = {1}, data = {2}".format(print_structure(pkt1.hdr), pkt1.size, list(i for i in pkt1.data)))
print("")

print("Ack 1")
ack1 = packetAck(hdr2, 16)
print("Header = [{0}], error code = {1}".format(print_structure(ack1.hdr), ack1.errorCode))
print("")

print("Test 1")

rc = libpacket.check_raw_packet_crc(byref(pkt1))
if(rc == ERR_CHECKSUM):
   print("Bad checksum")
elif(rc == SUCCESS):
   print("Good checksum")
else:
   print("Unknown return code: {0}".format(rc))

libpacket.add_raw_packet_crc(byref(pkt1))
print("Header = [{0}], size = {1}, data = {2}".format(print_structure(pkt1.hdr), pkt1.size, list(i for i in pkt1.data)))

rc = libpacket.check_raw_packet_crc(byref(pkt1))
if(rc == ERR_CHECKSUM):
   print("Bad checksum")
elif(rc == SUCCESS):
   print("Good checksum")
else:
   print("Unknown return code: {0}".format(rc))
