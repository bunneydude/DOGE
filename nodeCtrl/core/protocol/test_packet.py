from ctypes import *
from protocol_structs import *

libprotocol = CDLL('./libprotocol.so')

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

rc = libprotocol.check_raw_packet_crc(byref(pkt1))
if(rc == ERR_CHECKSUM):
   print("Bad checksum")
elif(rc == SUCCESS):
   print("Good checksum")
else:
   print("Unknown return code: {0}".format(rc))

libprotocol.add_raw_packet_crc(byref(pkt1))
print("Header = [{0}], size = {1}, data = {2}".format(print_structure(pkt1.hdr), pkt1.size, list(i for i in pkt1.data)))

rc = libprotocol.check_raw_packet_crc(byref(pkt1))
if(rc == ERR_CHECKSUM):
   print("Bad checksum")
elif(rc == SUCCESS):
   print("Good checksum")
else:
   print("Unknown return code: {0}".format(rc))
