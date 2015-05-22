from curses import wrapper
import signal
import time
import Queue
import threading
import curses
import random

import os
import sys
import json

menu = ["ID", "Temperature", "RSSI"]
MAX_COLUMN_WIDTH = max([len(x) for x in menu])
HCH = ord('-')
VCH = ord('|')
c = ""
uiRun = True
sketchRun = True
screenPos = 0

uiQueue = Queue.Queue()
sketchQueue = Queue.Queue()

def main(stdscr):
   global uiRun
   global sketchRun
   global screenPos

   keepRunning = True

   rowDict = {}

   stdscr.nodelay(True)
   stdscr.clear()
   screen = curses.newpad(stdscr.getmaxyx()[0]-1, stdscr.getmaxyx()[1]-1)
   screen.scrollok(True)   

   draw_frame(stdscr, screen)
   screen.refresh(0,0, 0,0, stdscr.getmaxyx()[0], stdscr.getmaxyx()[1])


   inputThread = threading.Thread(target=user_input, args=(stdscr,))
   inputThread.setDaemon(True)
   inputThread.start()

   #sketchThread = threading.Thread(target=sketch_link, args=())
   sketchThread = threading.Thread(target=sketch_loop, args=())
   sketchThread.setDaemon(True)
   sketchThread.start()

   while keepRunning:
      if(not uiQueue.empty()):    
         c = uiQueue.get()
         keepRunning = handle_ui(stdscr, screen, c)

      if(not sketchQueue.empty()):
         data = sketchQueue.get()
         handle_data(rowDict, data)       
         draw_rows(stdscr, screen, rowDict)  
      time.sleep(0.02)

   # Signal for the threads to end
   uiRun = False
   sketchRun = False
   inputThread.join()
   sketchThread.join()


# @param data - dictionary
#           'id' - node ID
#           'timestamp' - time of the measurement
#           'network' - which node this came from
#           'temp' - temperature
#           'rssi' - signal strength

def handle_data(rowDict, data):
   timestamp = data['timestamp']
   network = str(data['network'])
   temp = str(data['data']['temp'])
   rssi = str(data['data']['rssi'])

   nodeID = str(data['id']).zfill(2) + "-" + network

   if nodeID not in rowDict.keys():
      rowDict[nodeID] = []
   rowDict[nodeID] = [nodeID, temp, rssi]


def handle_ui(stdscr, screen, c):
   global screenPos
   if(c in range(0,256)):
      stdscr.addstr(3,2,str(chr(c)))

      if(c == ord('q')):
         return False
   elif(c == curses.KEY_DOWN and screenPos != screen.getmaxyx()[0]-1):      
      screenPos += 1
      screen.refresh(screenPos,0, 0,0, stdscr.getmaxyx()[0]-1, stdscr.getmaxyx()[1]-1)
   elif(c == curses.KEY_UP and screenPos != 0):
      screenPos -= 1
      screen.refresh(screenPos,0, 0,0, stdscr.getmaxyx()[0]-1, stdscr.getmaxyx()[1]-1)

   return True


def user_input(stdscr):
   global uiQueue
   global uiRun
   char = ""

   while uiRun:
      char = stdscr.getch()
      if(char != -1):
         uiQueue.put(char)
      time.sleep(0.02)

def sketch_loop():
   global sketchQueue
   global sketchRun

   toSend = []
   rxBytes = []

   target = 1
   temperature = 80
   rssi = -60
   timestamp = 0

   while sketchRun:
      nodeDataList = []
      for target in range(2,6):
         # set target

         rssi = random.randint(-80, -20)
         temperature = random.randint(60,100)
         timestamp = time.time()
         nodeDataList.append([timestamp,-1*rssi])
         sketchQueue.put({'timestamp':timestamp, 'id':target, 'network':'BP', 'data':{'temp':temperature, 'rssi':rssi} })
         time.sleep(0.1)


def sketch_link():
   global sketchQueue
   global sketchRun

   for i in range (0,10):
      sketchQueue.put({'timestamp':i, 'id':i+1, 'network':'L', 'data':{'temp':i+2, 'rssi':i+3} })
      time.sleep(2)
      if sketchRun == False:
         break

def draw_frame(stdscr, screen):
   menuString = ""
   for column in menu[0:-1]:
      menuString += column.center(MAX_COLUMN_WIDTH) + "|"
   menuString += menu[-1].center(MAX_COLUMN_WIDTH)

   screen.addstr(1,1,menuString)

   #Draw outer border
   maxY, maxX = stdscr.getmaxyx()
   screen.hline(0, 1, HCH, len(menuString))
   screen.vline(1, 0, VCH, maxY)
   screen.vline(1, len(menuString)+1, VCH, maxY)
   screen.hline(2, 1, HCH, len(menuString))

   #draw column lines
   columnOffsets = [i for i in range(len(menuString)) if menuString.startswith('|',i)]
   for x in columnOffsets:
      screen.vline(2, x+1, 0, maxY)

def draw_rows(stdscr, screen, rowDict):
   rowNumber = 0
   for key in sorted(rowDict):
      rowInfo = rowDict[key]
      rowString = ""
      for item in rowInfo[0:-1]:
         rowString += str(item).center(MAX_COLUMN_WIDTH) + "|"
      rowString += str(rowInfo[-1]).center(MAX_COLUMN_WIDTH) # last entry doesn't need border
      
      if(3+rowNumber >= screen.getmaxyx()[0]):
         screen.resize(3+rowNumber+1, screen.getmaxyx()[1])
      screen.addstr(3 + rowNumber, 1, rowString)
      rowNumber = rowNumber + 1
      screen.refresh(0,0, 0,0, stdscr.getmaxyx()[0]-1, stdscr.getmaxyx()[1]-1)

def resize_handler(n, frame):
   curses.endwin()
   curses.initscr()

#signal.signal(signal.SIGWINCH, resize_handler)
wrapper(main)
