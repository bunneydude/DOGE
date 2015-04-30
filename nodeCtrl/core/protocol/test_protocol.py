# The following commands were used to create the library
# We'll need to roll them into a makefile somewhere so the
# setup script can build all the *.so files needed for 
# the python portion of the project
#
# gcc -c -I. -I.. -fpic protocol.c
# gcc -c -I. -I.. -fpic packet.c
# cd ../memory_map
# gcc -c -fpic memory_map.c
# cd -
# gcc -shared -o libprotocol.so protocol.o ../memory_map/memory_map.o packet.o


from ctypes import *
from protocol_ctypes import *

libprotocol = CDLL('./libprotocol.so')

#mirror the structure in /protocol/testing/main.c

TEST_PACKET_ID   = 0x3
TEST_RTA         = 0xC
TEST_TYPE_ACK    = 0x1
TEST_TYPE_NO_ACK = 0x0
TEST_SRC_NODE_ID = 0x6
TEST_DST_NODE_ID = 0x8
TEST_SH_SRC_NODE_ID = 0x7
TEST_SH_DST_NODE_ID = 0x8
TEST_TTL         = 0x5
TEST_PACKET_SIZE = 16
TEST_PACKET_DATA = 0xDA
TEST_HEADER_TYPE = RAW_PACKET

TEST_CMD_READ_REG_ADDR = 0x2

obj = protocolState()
appPkt1 = appPacket()
rawPkt1 = rawPacket()
attr1 = packetAttr()

rawPkt2 = rawPacket()

libprotocol.Protocol_init(byref(obj))

print("Initial header: \n\t[{0}], size = {1}, data = {2}".format(print_structure(rawPkt1.hdr), rawPkt1.size, list(i for i in rawPkt1.data)))
print("Initial attr: \n\t{0}".format(print_structure(attr1)))

libprotocol.application_form_packet(byref(rawPkt1.data), byref(attr1), CMD_READ_REG, TEST_CMD_READ_REG_ADDR, 0x1);
libprotocol.link_layer_form_packet(byref(rawPkt1), byref(attr1), RAW_PACKET, TEST_SRC_NODE_ID, TEST_DST_NODE_ID, TEST_SH_SRC_NODE_ID, TEST_SH_DST_NODE_ID)

print("\nPost header: \n\t[{0}], size = {1}, data = {2}".format(print_structure(rawPkt1.hdr), rawPkt1.size, list(i for i in rawPkt1.data)))
print("Post attr: \n\t{0}".format(print_structure(attr1)))

toSend = packetToBytes(rawPkt1)
print(toSend)
print(packetToList(rawPkt1))


print("\nmessageRaw.size = {0}, cmd read_reg data size = {1}\n\n".format(rawPkt1.size, CMD_READ_REG_DATA_SIZE))




status = libprotocol.link_layer_parse_packet(byref(obj), byref(rawPkt1), byref(rawPkt2))

print("\nPost header: \n\t[{0}], size = {1}, data = {2}".format(print_structure(rawPkt2.hdr), rawPkt2.size, list(i for i in rawPkt2.data)))
print("Status = {0}, expected = {1}".format(status, TRANSMIT_RESPONSE))
print("Response Src ID = {0}, expected = {1}".format(rawPkt2.hdr.src, TEST_DST_NODE_ID))
print("Response Dst ID = {0}, expected = {1}".format(rawPkt2.hdr.dst, TEST_SRC_NODE_ID))
print("Response shSrc ID = {0}, expected = {1}".format(rawPkt2.hdr.shSrc, TEST_SH_DST_NODE_ID))
print("Response shDst ID = {0}, expected = {1}".format(rawPkt2.hdr.shDst, TEST_SH_SRC_NODE_ID))
print("Response ack = {0}, expected = {1}".format(get_header_type_ack(rawPkt2.hdr.type), 1))
print("Response type = {0}, expected = {1}".format(get_header_type(rawPkt2.hdr.type), RAW_PACKET))
print("Response size = {0}, expected {1}".format(rawPkt2.size, CMD_ACK_DATA_SIZE))

crcResult = libprotocol.check_packet_crc(byref(rawPkt2))
print("CRC result = {0}, expected 0".format(crcResult))
