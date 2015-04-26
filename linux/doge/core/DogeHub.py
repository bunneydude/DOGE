from doge.cloud.SparkfunData import SparkfunData
from doge.cloud.IntelAnalytics import IntelAnalytics
import json
from collections import defaultdict
from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import HardwareNode, VirtualNode
from doge.core.RoutingProcessor import RoutingProcessor
from doge.conf.globals import config

# TODO: Create inner class definition for operation on RP calls via private methods post-sketch connection

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

'''
Connect to a data stream from a specified cloud server
   
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
'''


def connect_webserver(screen, info):
    '''
    Connect to the webserver on Edison

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
    '''
    pass

def connect_tui(screen, info):
    '''
    Connect to the text user interface on Edison
    Almost the same as connect_webserver.
    '''
    pass

def connect_sketch():
    '''
    Connect to the IPC objects from sketch
    '''
    node = 0 #TODO: where to read this in from?
    pipe = RadioInterface("edison", node, config['debug'])
    pipe.connect_sketch()
    
    root = VirtualNode(0)
    return root

def find_neighbors(nids):
    pass

def rp_run():
    root = connect_sketch() #if not already not connected 
    
    #List of network edges,nodes,routing edges. Sent to webserver/browser for vis.js n/w visiualization
    edges = []
    nodes = []
    route_edges = []
    
    #Dicts for neighbor table entry and routing table entry. Node id is key and value is list of all nte/rte for that node
    nte = {}
    rte = {}
    
    root.get_neighbors()
    root.get_neighbor_table() #get edison neighbors
    
    edisonRP = RoutingProcessor()
    
    
    
