import os, sys, inspect
cmd_subfolder = os.path.realpath(os.path.abspath(os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0],"../cloud")))
if cmd_subfolder not in sys.path:
	sys.path.insert(0, cmd_subfolder)

from SparkfunData import SparkfunData
#from IntelAnalytics import IntelAnalytics #XXX: blocked due to python-requests module not in repo
import json

def connect_cloud(service):
    stream = None
    if service.lower() == 'sparkfun':
        stream = SparkfunData()
    elif service.lower() == 'intel':
        stream = IntelAnalytics()
    return stream

def getLatestNodeData(stream, dataFilter):
    if type(dataFilter) != type(dict()):
        return {}
    _rawData = stream.pull('json')
    _jsonData = json.loads(_rawData)
    for row in _jsonData:
        match = 0
        for key in row.viewkeys() & dataFilter.viewkeys():
            if dataFilter[key] == row[key]:
                match += 1
            else:
                break
        if match == len(row.viewkeys() & dataFilter.viewkeys()):
#        if row['network'] == 'wifi' and row['id'] == '0':
#            return row['temperature']
            return row
#    return _rawData
    return {}


"""Connect to a data stream from a specified cloud server
   
Args:
      service (str): Name of the cloud service::
         "sparkfun" -- SparkFun's
         "intel" -- Intel's

      stream (str): Name of the data stream

   Returns:
      socket? Something I can pass around the program and call pull/push methods on

   Raises:
      Should we make a custom set of exceptions for DOGE?

   Example:
      streamMark = connect_cloud("sparkfun", "mark_apartment")
      streamDave = connect_cloud("sparkfun", "dave_apartment")

      newData = streamMark.pull("averageTemperature")

      localNodeData = #something in the form of that json structure
      streamDave.push(localNodeDatanewData)
   """


def connect_webserver(screen, info):
   """Connect to the webserver on Edison

   Args:
      screen (str?): Which page to render::
         "rssi" -- Plot signal strength of nodes over time
         "heatmap?" -- Nodes are represented as a circle whose color changes based on its temperature

      info (dictionary/array?): Depends on screen. Might contain a list of nodes that Edison plans to push data for. That way the page can render w/ the right number of data series.

   Returns:
      socket? Similar to socket in connect_cloud. Will just be used to push data

   Raises:
      See connect_cloud method

   Example:
      #Start a rssi plot for BP nodes 1 & 2, and Leaf nodes 5 & 7
      nodeList = ["BP1", "BP2", "L5", "L7"]
      rssiPage = connect_webserver("rssi", nodeList)
   
      # loop as needed
      localData = get_temp_from_someone(nodeList)
      rssiPage.push(localData)
   """
   pass

def connect_tui(screen, info):
   """Connect to the text user interface on Edison

   Almost the same as connect_webserver.
   """
   pass

def connect_sketch():
   """Connect to the IPC objects from sketch
   """
   pass


