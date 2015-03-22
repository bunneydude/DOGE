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
# This program will:
#   * Authenticate a user using existing credentials
#   * Create a device
#   * Activate the device (but currently does not persist the token)
#   * Register 2 time series for the device - one for temperature and
#     one for humidity (The component types for these are already
#      defined in the account catalog)
#   * Send observations for both time series. As configured it will 
#     send one per hour for the last 25 hours for each time series.
#   * Read the observations back and display the count.
#
#  To use:
#   * On the web:
#       * Go to https://dashboard.us.enableiot.com
#       * Register - be sure to click the "Sign Up Here" link. Do not use
#       * any of the OAuth options, or you will not be able to use the API.#
#       * Verify your email address
#       * Enter a name for your account
#   * Below line 39 in this file:
#       * Update your username, password and account_name
#       * Update the proxy address if required
#       * Update the device id below. The device id MUST be unique or
#         the step to create the device will fail
#   * Install the python "requests" library. You can use Python 
#     virtual environments, or install it globally:
#       $ pip install requests
#   * Run the program
#       $ python iotkit_client.py
#

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
   global g_user_token,g_uid,g_aid
   g_user_token = self.get_token()
   g_uid = self.get_user_id()
   g_aid =  self.get_account_id(g_uid)
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
    print js
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

 #Returns component id if component name is present. If not present, creates one. 
 def get_cid (self,ctype,cname):
    url =  "{0}/accounts/{1}/devices/{2}".format(self.base_url, g_aid,self.device_id)
    resp = requests.get(url, headers=self.get_user_headers(), proxies=self.proxies, verify=self.verify)
    self.check(resp, 200)
    js = resp.json()
    series = js['components']
    series = sorted(series, key=lambda u: u["type"])
    for u in series:
        #print "Type: {0} Name: {1} Cid: {2}".format(u["type"], u["name"], u["cid"])
        if cname == str(u["name"]):
          #print "Match Ctype {0} with CID {1}".format(u["type"], u["cid"])
          myCid = u["cid"]
          cname_found = 1
        if ctype == str(u["type"]):
          ctype_found = 1

    #FIXME register component type if not found 
    #if ctype == 0:

    if cname_found == 0:
        myCid = self.create_component(g_aid,  ctype, cname) 
    return myCid
 

# print all of the device names and observation counts, sorted by device name
 def print_observation_counts(js):  # js is result of /accounts/{account}/data/search
    if 'series' in js:
        series = js["series"]
        series = sorted(series, key=lambda v: v["deviceName"])
        for v in series:
            print "Device: {0} Count: {1}".format(v["deviceName"], len(v["points"]))



 def push (self,data):

    # refresh the activation code. It can be used any number of times
    # in the next 60 minutes to activate devices.
    ac = self.generate_activation_code(g_aid)
    #print "Activation code: {0}".format(ac)

    component_type = str( data['network'])+".v1.0"
    component_name =  "node:"+str(data['id'])+"tempF"

    cid = self.get_cid(component_type,component_name)
 
    #Submit observation to the cloud
    self.create_observations(g_aid, self.device_id, cid, data)


