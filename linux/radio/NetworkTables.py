class NeighborTableEntry:
   _fields = {'shNodeID':None, 'shLQE':None, 'radioID':None, 'networkID':None}
   _resolutions = {'shNodeID':16, 'shLQE':12, 'radioID':2, 'networkID':2}
   
   def __init__(self, fields):
      for(key in self._fields):
         if(key in fields):
            self._fields = fields[key]

   def to_s(self):
      print("Neighbor fields: shNodeID = {0}, shLQE = {1}, radioID = {2}, networkID = {3}".format(self._fields['shNodeID'], self._fields['shLQE'], self._fields['radioID'], self._fields['networkID']))
      print("Neighbor field resolutions: shNodeID = {0}, shLQE = {1}, radioID = {2}, networkID = {3}".format(self._resolutions['shNodeID'], self._resolutions['shLQE'], self._resolutions['radioID'], self._resolutions['networkID']))


      
class RoutingTableEntry:
   _fields = {'mhNodeID':None, 'mhLQE':None, 'neighborIndex':None}
   _resolutions = {'mhNodeID':16, 'mhLQE':8, 'neighborIndex':8}
   
   def __init__(self, fields):
      for(key in self._fields):
         if(key in fields):
            self._fields = fields[key]

   def to_s(self):
      print("Routing fields: mhNodeID = {0}, mhLQE = {1}, neighborIndex = {2}".format(self._fields['mhNodeID'], self._fields['mhLQE'], self._fields['neighborIndex']))
      print("Routing field resolutions: mhNodeID = {0}, mhLQE = {1}, neighborIndex = {2}".format(self._resolutions['mhNodeID'], self._resolutions['mhLQE'], self._resolutions['neighborIndex']))
