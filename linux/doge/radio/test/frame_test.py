from cobs import cobs
import struct

stream = [0, 4, 1, 2, 3, 0, 2, 5, 0]
stream.reverse()

buf = []

def proxy_receive():
   if(len(stream) > 0):
      rxData = []
      encData = []
      encData.append(stream.pop())
      if(encData[0] == 0): #caught the end of a previous frame
         print("First byte 0")
         return 0
   
      while(encData[-1] != 0): #get bytes until end of frame
         encData.append(stream.pop())

      encData = encData[0:-1] #remove trailing 0
      print("Encoded data = {0}".format(encData))

      rxData = cobs.decode(''.join(struct.pack('<B',x) for x in encData))
      rxData = list(ord(x) for x in rxData)
      print("Decoded data = {0}".format(rxData))
      return rxData
   else:
      print("Stream empty: {0}".format(len(stream)))
      return 0

buf = proxy_receive()
print(buf)

buf = proxy_receive()
print(buf)

buf = proxy_receive()
print(buf)
