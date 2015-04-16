import sysv_ipc
import struct

class IPCBuffer():

      def __init__(self, id):
         print("In init method with " +str(id) +"\n")
         self.id = id
         self.k_sem_lock = sysv_ipc.ftok("/sketch/k_lock.txt", self.id, True)
         self.k_shm = sysv_ipc.ftok("/sketch/k_shm.txt", self.id, True)
         self.k_sem_data = sysv_ipc.ftok("/sketch/k_data.txt", self.id, True)
         self.k_sem_space = sysv_ipc.ftok("/sketch/k_space.txt", self.id, True)

      def open_sketch(self):
         #get ipc objects created by sketch
         self.sem_lock = sysv_ipc.Semaphore(self.k_sem_lock)
         self.mem = sysv_ipc.SharedMemory(self.k_shm)
         self.sem_data = sysv_ipc.Semaphore(self.k_sem_data) #initially 0
         self.sem_space = sysv_ipc.Semaphore(self.k_sem_space) #initially 1

         self.head=0
         self.tail=0
         self.width=0
         self.length=0

         self.sem_lock.acquire()
         self.width = self.mem.read(4,8)
         self.length = self.mem.read(4,12)
         self.sem_lock.release()

         self.width = int(self.width[::-1].encode('hex'),16)
         self.length = int(self.length[::-1].encode('hex'),16)

      def read(self):
         self.sem_data.acquire() #wait here until new data is in buffer
         self.sem_lock.acquire() #block anyone else from touching buffer

         self.tail = int(self.mem.read(4,4)[::-1].encode('hex'),16) 
         val = self.mem.read(self.width, 16 + self.tail*self.width)  
         self.mem.write(struct.pack('<I',(self.tail+1)%self.length), 4)

         self.sem_lock.release()
         self.sem_space.release() #indicate head == tail..(TODO do this)

         return val

      def write(self,data):
         self.sem_space.acquire() #make sure there's space
         self.sem_lock.acquire() #block anyone else from touching buffer

         self.head = int(self.mem.read(4,0)[::-1].encode('hex'),16) 
         self.mem.write(struct.pack('<B',data), 16 + self.head*self.width)
         self.mem.write(struct.pack('<I',(self.head+1)%self.length), 0)

         self.sem_lock.release()
         self.sem_data.release()

      def available(self):
         self.sem_lock.acquire() #block anyone else from touching buffer
         self.head = int(self.mem.read(4,0)[::-1].encode('hex'),16) 
         self.tail = int(self.mem.read(4,4)[::-1].encode('hex'),16) 
         self.sem_lock.release()

         if(self.head >= self.tail):
            return self.head - self.tail
         else:
            return self.head + self.length - self.tail


      def printKeys(self):
         print("k_sem_lock = " + str(self.k_sem_lock) + "\n")
         print("k_shm = " + str(self.k_shm) + "\n")
         print("k_sem_data = " + str(self.k_sem_data) + "\n")
         print("k_sem_space = " + str(self.k_sem_space) + "\n")
