import urllib
import urllib2

class SparkfunData():

    public_key   = 'g6WrZDLm8xHp6Yzzbg21'
    private_key  = 'qzEBGMy8mJSGJpjjqe9D'
    base_url     = 'https://data.sparkfun.com'
    post_url     = base_url + '/input/'  + public_key
    pull_url     = base_url + '/output/' + public_key
    headers = {
        'Content-type': 'application/x-www-form-urlencoded',
        'Phant-Private-Key': private_key
    }

    def __init__(self):
        self = self

    def push(self, _data):
        _data = urllib.urlencode(_data)
        post_request = urllib2.Request(self.post_url,_data,self.headers)
    
        try:
            post_response = urllib2.urlopen(post_request)
            return post_response.read()
    
        except urllib2.URLError as e:
            return "Error: " + e.reason

    def pull(self, _format):
        pull_request = urllib2.Request(self.pull_url+'.'+_format.lower())

        try:
            pull_response = urllib2.urlopen(pull_request)
            return pull_response.read()

        except urllib2.HTTPError as e:
            return e.code
        except urllib2.URLError as e:
            return e.reason.errno #e.args

