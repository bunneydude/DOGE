READ_REG = 1
WRITE_REG = 2
ACK = 3
NACK = 4
NOP = 5

BAD_CHECKSUM = 1
BAD_COMMAND = 2
FEATURE_WIP = 3
CAT = 4

def form_packet(cmd=0, addr=0, data=0):

   if( (type(cmd) is not int) or (type(addr) is not int) or (type(data) is not int)):
      print("Inputs must be ints\n")
      return []

   to_send = [] #ints
   to_send.append(cmd)

   if(cmd == READ_REG): # cmd 1 src_8
      to_send.append(1)
      to_send.append(addr) 

   elif(cmd == WRITE_REG): # cmd 2 dst_8 data_8
      to_send.append(2)
      to_send.append(addr)
      to_send.append(data)

   elif(cmd == ACK): # cmd 0
      to_send.append(2)
      to_send.append(addr)
      to_send.append(data)

   elif(cmd == NACK):# cmd 0 data 
      to_send.append(2)
      to_send.append(addr)
      to_send.append(data)

   elif(cmd == NOP): # cmd 0
      to_send.append(0)

   else:
      print("Unknown command: " + str(cmd) +"\n")

   checksum = -sum(to_send)%256
   to_send.append(checksum)

   return to_send

def receive_packet(stream, registers):
   # Returns response array
   rxBuffer = 0
   byteNum = 0
   checksum = 0
   execute = 0
   remainder = []
   response = ""


   cmd = 0
   size = 0
   addr = 0
   data = 0
   
   stream.reverse()
   while(len(stream)>0):
      rxBuffer = stream.pop()

      #convert just in case
      if(type(rxBuffer) is str):
         rxBuffer = ord(rxBuffer)

      byteNum = byteNum + 1
      checksum = checksum + rxBuffer

      if(byteNum == 1):
         cmd = rxBuffer
      elif(byteNum == 2):
         size = rxBuffer
      else:
         if( (size + 3) > byteNum): #not at checksum if true
            remainder.append(rxBuffer)
         else: #at checksum if true
            if(checksum%256 == 0):
               execute = 1
            else:
               response = form_packet(NACK, 0, BAD_CHECKSUM)
               execute = 0
               byteNum = 0
               checksum = 0
               remainder = []
               return response

      if(execute == 1):
         if(cmd == READ_REG):
            addr = remainder[0]
            print("Got read_reg. Addr = " + str(addr) + "\n")
            response = form_packet(ACK, addr, registers[addr])
         elif(cmd == WRITE_REG):
            addr = remainder[0]
            data = remainder[1]
            print("Got write_reg. Addr = " + str(addr) + ", Data = " + str(data) + "\n")
            registers[addr] = data
            response = form_packet(ACK, addr, registers[addr])
         elif(cmd == ACK):
            addr = remainder[0]
            data = remainder[1]
            print("Got ack. Addr = " + str(addr) + ", Data = " + str(data) + "\n")
         elif(cmd == NACK):
            addr = remainder[0]
            data = remainder[1]
            print("Got nack. Addr = " + str(addr) + ", Data = " + str(data) + "\n")
         elif(cmd == NOP):
            print("Got nop\n")
         else:
            print("Unknown command: " + str(cmd) + "\n")
            response = form_packet(NACK, 0, BAD_COMMAND)
         execute = 0
         byteNum = 0
         checksum = 0
         remainder = []
         return response

