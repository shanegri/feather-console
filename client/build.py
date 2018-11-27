'''
This file converts client/main.js into feather/client.h  
so that it can be served by the feather
'''

import httplib, urllib, sys

file_src = open("main.js", "r")

js_code = ""
for i in iter(file_src): js_code += i

file_src.close()

params = urllib.urlencode([
    ('js_code', js_code), 
    ('compilation_level', 'ADVANCED_OPTIMIZATIONS'),
    ('output_format', 'text'),
    ('output_info', 'compiled_code'),
  ])

headers = { "Content-type": "application/x-www-form-urlencoded" }
conn = httplib.HTTPSConnection('closure-compiler.appspot.com')
conn.request('POST', '/compile', params, headers)
response = conn.getresponse()

file_dest = open("../feather/client.h", "w+")

data = ""
for i in iter(response.read().splitlines()):
  data += " " + i.replace('"', r'\"')

file_dest.write('#define CLIENT_JS "<script>' + data + '</script>"')
file_dest.close()

conn.close()