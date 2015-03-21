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

#Usage:
#Make sure you enter your credentials in the script below before using this script
#The component catalog type has to be registered in the IoT Analtyics Dashboard for the current script to work. 
#example: Register boosterpack or leafnode as a component catalog type

import sys
import requests
import json
import uuid
import time
import random

class IntelAnalytics():

 #####################################
 # Set these values first
 #####################################
 host = "dashboard.us.enableiot.com"

 proxies = {
    # "https": "http://proxy.example.com:8080"
 }

 username = ""
 password = ""
 account_name = ""
 device_id = ""
 device_token = ""


 verify = True # whether to verify certs
 #####################################

 api_root = "/v1/api"
 base_url = "https://{0}{1}".format(host, api_root)
 device_name = "Device-{0}".format(device_id)

 def __init__(self):
   self = self

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
        sys.exit(1)


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
    #print js
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
        return token
    else:
        print js
        sys.exit(1)


# Given an account_id and device_id, and a component type name and name - create a component and return the cid
 def create_component(self, account_id, component_type_name, name):
    url = "{0}/accounts/{1}/devices/{2}/components".format(self.base_url, account_id, self.device_id)
    component = {
        "type": component_type_name,
        "name": name,
        "cid": str(uuid.uuid4())
    }
    data = json.dumps(component)
    resp = requests.post(url, data=data, headers=self.get_device_headers(), proxies=self.proxies, verify=self.verify)
    self.check(resp, 201)
    js = resp.json()
    return js["cid"]


 # Create observations and submit them
 def create_observations(self,account_id, device_id, cid, jsonData):
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
        "value": str(jsonData['temp_f']),
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


# print all of the device names and observation counts, sorted by device name
 def print_observation_counts(js):  # js is result of /accounts/{account}/data/search
    if 'series' in js:
        series = js["series"]
        series = sorted(series, key=lambda v: v["deviceName"])
        for v in series:
            print "Device: {0} Count: {1}".format(v["deviceName"], len(v["points"]))



 

 def push (self,data):

    global g_user_token

    #Get an authentication token for use in the following API calls.
    # It will be put in every header by get_user_headers()
    g_user_token = self.get_token()


    # get my user_id (uid) within the Intel IoT Analytics Platform
    uid =  self.get_user_id()
    print "UserId: {0}".format(uid)

    # for all the accounts I have access to, find the first account
    # with the name {account_name} and return the account_id (aid)
    aid = self.get_account_id(uid)
    print "AccountId: {0}".format(aid)



    # refresh the activation code. It can be used any number of times
    # in the next 60 minutes to activate devices.
    ac = self.generate_activation_code(aid)
    print "Activation code: {0}".format(ac)


    # this registers a time series for this device. The component will have a
    # Component Type of "temperature.v1.0" which defines the data type of the
    # value, the format, the unit of measure, etc. This way, we don't need to
    # specify all of that here.
    # Within the scope of the device, this time series will be named "temp".
    # This call returns the component_id (cid) which is globally unique
    # within the Intel IoT Analytics platform.
    #print "Ctype:{0} Cname:{1}".format(data['ctype'], data['cname'])
    component_type = str( data['network'])+".v1.0"
    component_name =  "node:"+str(data['id'])+"tempF"
    cid = self.create_component(aid,  component_type, component_name)
    #print "ComponentID (cid): {0}".format(cid)

    #Submit observation to the cloud
    self.create_observations(aid, self.device_id, cid, data)


