from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields
from doge.radio import mmPython

import sys

debug = False

pipe = RadioInterface.RadioInterface("edison", 1, debug, logLevel=3)
pipe.connect_sketch()


NETWORK_TABLE_CHUNK_1_ADDRESS = mmPython.MM_NETWORK_BASE
NETWORK_TABLE_CHUNK_2_ADDRESS = NETWORK_TABLE_CHUNK_1_ADDRESS + ProtocolDefs.MAX_CMD_READ_MEM_DATA_SIZE
NETWORK_TABLE_CHUNK_3_ADDRESS = NETWORK_TABLE_CHUNK_2_ADDRESS + ProtocolDefs.MAX_CMD_READ_MEM_DATA_SIZE
NETWORK_TABLE_CHUNK_1_SIZE = ProtocolDefs.MAX_CMD_READ_MEM_DATA_SIZE
NETWORK_TABLE_CHUNK_2_SIZE = ProtocolDefs.MAX_CMD_READ_MEM_DATA_SIZE
NETWORK_TABLE_CHUNK_3_SIZE = 8

pipe.proxy_send(destination=2, command=ProtocolDefs.CMD_READ_MEM, address=NETWORK_TABLE_CHUNK_1_ADDRESS, payload=NETWORK_TABLE_CHUNK_1_SIZE, singleHopDest=2)
pipe.proxy_receive()
bytes = ProtocolDefs.packetToBytes(pipe.rxPacket)
print([byte.encode("hex") for byte in bytes])
pipe.proxy_send(destination=2, command=ProtocolDefs.CMD_READ_MEM, address=NETWORK_TABLE_CHUNK_2_ADDRESS, payload=NETWORK_TABLE_CHUNK_2_SIZE, singleHopDest=2)
pipe.proxy_receive()
bytes = ProtocolDefs.packetToBytes(pipe.rxPacket)
print([byte.encode("hex") for byte in bytes])
pipe.proxy_send(destination=2, command=ProtocolDefs.CMD_READ_MEM, address=NETWORK_TABLE_CHUNK_3_ADDRESS, payload=NETWORK_TABLE_CHUNK_3_SIZE, singleHopDest=2)
pipe.proxy_receive()
bytes = ProtocolDefs.packetToBytes(pipe.rxPacket)
print([byte.encode("hex") for byte in bytes])
