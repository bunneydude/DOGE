# Copyright (c) 2015, Intel Corporation
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright notice,
#      this list of conditions and the following disclaimer in the documentation
#      and/or other materials provided with the distribution.
#    * Neither the name of Intel Corporation nor the names of its contributors
#      may be used to endorse or promote products derived from this software
#      without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

###################################################################
#Usage: 
#Make sure you enter user credentials in intel_credentials.txt
#You can leave the device_id and device_token fields empty and the script
#will populate this with device_id = "Edison_DOGE" and a unique device_token


import sys
import requests
import json
import uuid
import time
import random
import re 
import os

def find(name, path):
    for root, dirs, files in os.walk(path):
        if name in files:
            return os.path.join(root, name)

class IntelAnalytics():

 iotkitJsonRssi =  {'component_type':'rssi','component_version':"1.0",'component_format':'float','component_measureunit':'Degrees Farenheit'}
 iotkitJsonTemp =  {'component_type':'temperature','component_version':"1.0",'component_format':'float','component_measureunit':'Strength'}
 sensor = {'rssi':iotkitJsonRssi, 'temperature':iotkitJsonTemp}

 #####################################
 # Set these values first
 #####################################
 host = "dashboard.us.enableiot.com"

 proxies = {
    # "https": "http://proxy.example.com:8080"
 }
 
 verify = True # whether to verify certs
 #####################################

 api_root = "/v1/api"
 base_url = "https://{0}{1}".format(host, api_root)

 g_user_token = ""
 g_uid = ""
 g_aid = ""
 username = ""
 password = ""
 account_name  = ""
 device_id = ""
 device_token = ""
 
 filename = "intel_credentials.txt"
 filename = find(filename, os.getcwd())
   #Extract all user credentials from the file intel_credentials.txt
 with open(filename, 'r') as f:
    for line in f:
      searchObj = re.search( r'(.*)\s*=\s*\"(.*)\"', line, re.M|re.I)
      if searchObj:
        if (searchObj.group(1) == "username" ):
          username =  searchObj.group(2)
        elif (searchObj.group(1) == "password" ):
          password =  searchObj.group(2)
        elif (searchObj.group(1) == "account_name" ):
          account_name =  searchObj.group(2)
        elif (searchObj.group(1) == "device_id" ):
          device_id =  searchObj.group(2)
        elif (searchObj.group(1) == "device_token" ):
          device_token =  searchObj.group(2)
 f.closed

 def __init__(self):
   global g_user_token,g_uid,g_aid
   g_user_token = self.get_token()
   g_uid = self.get_user_id()
   g_aid =  self.get_account_id(g_uid)
   if self.device_id == "":
     self.device_id = "Edison_DOGE1"
     device_name = "Device-{0}".format(self.device_id)
     print "Creating device {0}".format(self.device_id)
     self.create_device (g_aid,self.device_id,device_name)
     #Get device token
     ac = self.generate_activation_code(g_aid)
     self.device_token = self.activate(g_aid,self.device_id,ac)
     self.save_user_cred()
   self = self
 
 def save_user_cred (self):
   fr = open(self.filename, 'r')
   fw = open(self.filename+"_tmp", 'w')
   for line in fr:
      searchObj = re.search( r'(.*)\s*=\s*\"(.*)\"', line, re.M|re.I)
      if searchObj:
        if (searchObj.group(1) == "device_id" ):
          fw.write(re.sub( r'device_id\s*=\s*\"(.*)\"', "device_id=\"{0}\"".format(self.device_id),line))
        elif (searchObj.group(1) == "device_token" ):
          fw.write(re.sub( r'device_token\s*=\s*\"(.*)\"', "device_token=\"{0}\"".format(self.device_token),line))
        else :
          fw.write(line)
   fr.close()
   fw.close()
   os.rename (self.filename+"_tmp",self.filename)
 
 def get_user_headers(self):
    headers = {
        'Authorization': 'Bearer ' + g_user_token,
        'content-type': 'application/json'
    }
    #print "Headers = " + str(headers)
    return headers


 def get_device_headers(self):
    headers = {
        'Authorization': 'Bearer ' + self.device_token,
        'content-type': 'application/json'
    }
    #print "Headers = " + str(headers)
    return headers


 def check(self,resp, code):
    if resp.status_code != code:
        print "Expected {0}. Got {1} {2}".format(code, resp.status_code, resp.text)
        resp.raise_for_status()


# Given a username and password, get the user token
 def get_token(self):
    url = "{0}/auth/token".format(self.base_url)
    headers = {'content-type': 'application/json'}
    payload = {"username": self.username, "password": self.password}
    data = json.dumps(payload)
    resp = requests.post(url, data=data, headers=headers, proxies=self.proxies, verify=self.verify)
    self.check(resp, 200)
    js = resp.json()
    token = js['token']
    return token


# given a user token, get the user_id
 def get_user_id(self):
    url = "{0}/auth/tokenInfo".format(self.base_url)
    resp = requests.get(url, headers=self.get_user_headers(), proxies=self.proxies, verify=self.verify)
    self.check(resp, 200)
    js = resp.json()
    user_id = js["payload"]["sub"]
    return user_id


# given a user_id, get the account_id of the associated account with account_name
# if there are multiple accounts with the same name, return one of them
 def get_account_id(self,user_id):
    url = "{0}/users/{1}".format(self.base_url, user_id)
    resp = requests.get(url, headers=self.get_user_headers(), proxies=self.proxies, verify=self.verify)
    self.check(resp, 200)
    js = resp.json()
    if 'accounts' in js:
        accounts = js["accounts"]
        for k, v in accounts.iteritems():
            if 'name' in v and v["name"] == self.account_name:
                return k
    print "Account name {0} not found.".format(self.account_name)
    print "Available accounts are: {0}".format([v["name"] for k, v in accounts.iteritems()])
    return None


 # create a device
 def create_device(self,account, device_id, device_name):
    url = "{0}/accounts/{1}/devices".format(self.base_url, account)
    device = {
        "deviceId": str(device_id),
        "gatewayId": str(device_id),
        "name": device_name,
        "tags": ["US", "California", "San Francisco"],
        # if the device will be static, use this
        # to remember where you put it
        #"loc": [37.783944, -122.401289, 17],
        "attributes": {
            "vendor": "intel",
            "platform": "x86",
            "os": "linux"
        }
    }
    data = json.dumps(device)
    resp = requests.post(url, data=data, headers=self.get_user_headers(), proxies=self.proxies, verify=self.verify)
    print data
    self.check(resp, 201)
    return resp


 # Generate an activation code and return it
 # This activation code will be good for 60 minutes
 def generate_activation_code(self,account_id):
    url = "{0}/accounts/{1}/activationcode/refresh".format(self.base_url, account_id)
    resp = requests.put(url, headers=self.get_user_headers(), proxies=self.proxies, verify=self.verify)
    self.check(resp, 200)
    js = resp.json()
    activation_code = js["activationCode"]
    return activation_code


# Activate a device using a valid activation code
 def activate(self,account_id, device_id, activation_code):
    url = "{0}/accounts/{1}/devices/{2}/activation".format(self.base_url, account_id, device_id)
    activation = {
        "activationCode": activation_code
    }
    data = json.dumps(activation)
    resp = requests.put(url, data=data, headers=self.get_user_headers(), proxies=self.proxies, verify=self.verify)
    self.check(resp, 200)
    js = resp.json()
    if "deviceToken" in js:
        token = js["deviceToken"]
        print "Device token:{0}".format(token)
        return token
    else:
        print js
        sys.exit(1)


# Given an account_id and device_id, and a component type name and name - create a component and return the cid
 def create_component(self, account_id, component_type, component_name):
    print "In create_component: Component type={0} Component name={1}".format (component_type,component_name) 
    url = "{0}/accounts/{1}/devices/{2}/components".format(self.base_url, account_id, self.device_id)
    component = {
        "type": component_type,
        "name": component_name,
        "cid": str(uuid.uuid4())
    }
    data = json.dumps(component)
    resp = requests.post(url, data=data, headers=self.get_device_headers(), proxies=self.proxies, verify=self.verify)
    self.check(resp, 201)
    js = resp.json()
    return js["cid"]


 # Create observations and submit them
 def create_observations(self,account_id, device_id, cid, jsonData,value):
    url = "{0}/data/{1}".format(self.base_url, device_id)
    timestamp = int(time.time()) * 1000;
    body = {
        "on": timestamp,
        "accountId": account_id,
        "data": []
    }
    
    o = {
        "componentId": cid,
        "on": timestamp,
        # if the device is mobile, you can record where it was when
        # this observation was captured
        #"loc": [ 45.5434085, -122.654422, 124.3 ],
        "value": str(jsonData[value]),
        #"attributes": {
        #    "pkt_loss": pkt_loss
        #  }
        }
    body["data"].append(o)
    
    data = json.dumps(body)
    resp = requests.post(url, data=data, headers=self.get_device_headers(), proxies=self.proxies, verify=self.verify)
    self.check(resp, 201)


 #get_observations
 def get_observations(self,account_id, device_id, component_id):
    url = "{0}/accounts/{1}/data/search".format(self.base_url, account_id)
    search = {
        "from": 0,
        "targetFilter": {
            "deviceList": [device_id]
        },
        "metrics": [
            {
                "id": component_id
            }
        ]
        # This will include lat, lon and alt keys
        #,"queryMeasureLocation": True
    }
    data = json.dumps(search)
    resp = requests.post(url, data=data, headers=self.get_user_headers(), proxies=self.proxies, verify=self.verify)
    self.check(resp, 200)
    js = resp.json()
    return js

 #Returns component id if component name is present. If not present, creates one. 
 def get_cid (self,iotkitJson,cname):
    ctype = iotkitJson['component_type']
    cversion = iotkitJson['component_version']
    component_type = ctype+".v"+cversion
    ctype_found = 0
    cname_found = 0
    url =  "{0}/accounts/{1}/devices/{2}".format(self.base_url, g_aid,self.device_id)
    resp = requests.get(url, headers=self.get_user_headers(), proxies=self.proxies, verify=self.verify)
    self.check(resp, 200)
    js = resp.json()
    #If json has a component attribute check for component name and cid
    if 'components' in js:
     series = js['components']
     series = sorted(series, key=lambda u: u["type"])
     for u in series:
        #print "Type: {0} Name: {1} Cid: {2}".format(u["type"], u["name"], u["cid"])
        if ( cname == str(u["name"]) and component_type == str(u["type"]) ) :
          #print "Match Ctype {0} with CID {1}".format(u["type"], u["cid"])
          myCid = u["cid"]
          cname_found = 1
          ctype_found = 1
    if ctype_found == 0:
        print "No components found for device,registering:{0}".format(ctype)
        self.register_component_type (iotkitJson)
    if cname_found == 0:
        myCid = self.create_component(g_aid,  component_type, cname) 
    return myCid
 
 #Register a new component type
 def register_component_type (self,iotkitJson):
    url =  "{0}/accounts/{1}/cmpcatalog".format(self.base_url, g_aid)
    payload = {   
      "dimension": iotkitJson['component_type'],
      "version": iotkitJson['component_version'],
      "type": "sensor",
      "dataType":"Number",
      "format": iotkitJson['component_format'],
      "min": -150,
      "max": 150,
      "measureunit": iotkitJson['component_measureunit'],
      "display": "timeSeries"
    }
    data = json.dumps(payload)
    resp = requests.post(url, data=data,headers=self.get_user_headers(), proxies=self.proxies, verify=self.verify)
    #Check if component was created successfully (201) or if already exists (409)
    if (resp.status_code != 201 and resp.status_code != 409):
        print "Expected 201 or 409. Got {0} {1}".format( resp.status_code, resp.text)
        sys.exit(1)


 # print all of the device names and observation counts, sorted by device name
 def print_observation_counts(js):  # js is result of /accounts/{account}/data/search
    if 'series' in js:
        series = js["series"]
        series = sorted(series, key=lambda v: v["deviceName"])
        for v in series:
            print "Device: {0} Count: {1}".format(v["deviceName"], len(v["points"]))



 def push (self,data):
    iotkitJson = {}
    val = ''
    data['network'] = 'IoT-Demo'
    if 'rssi' in data:
        iotkitJson = self.sensor['rssi']
        val = 'rssi'
    elif 'temperature' in data:
        iotkitJson = self.sensor['temperature']
        val = 'temperature'
    else:
        return

    # refresh the activation code. It can be used any number of times
    # in the next 60 minutes to activate devices.
    ac = self.generate_activation_code(g_aid)
    #print "Activation code: {0}".format(ac)

    component_type = iotkitJson['component_type']
    component_name =  str(data['network'])+"-node:"+str(data['id'])+"-"+component_type

    cid = self.get_cid(iotkitJson,component_name)
 
    #Submit observation to the cloud
    self.create_observations(g_aid, self.device_id, cid, data,val)

 def pull (self,dataFilter):
    iotkitJson = self.sensor[dataFilter['sensor']]

    if 'network' not in dataFilter:
        dataFilter['network'] = 'IoT-Demo'
    # refresh the activation code. It can be used any number of times
    # in the next 60 minutes to activate devices.
    ac = self.generate_activation_code(g_aid)
    component_type = iotkitJson['component_type']
    component_name =  str(dataFilter['network'])+"-node:"+str(dataFilter['id'])+"-"+component_type
    cid = self.get_cid(iotkitJson,component_name)
    
    #Submit request for [filtered] observations from the cloud
    dataset = self.get_observations(g_aid, self.device_id, cid)
    dataJson = {dataFilter['sensor']: dataset['series'][0]['points'][-1]['value']}
    return dataJson


