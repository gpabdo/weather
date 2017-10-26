#!/usr/bin/python

import json
import time
import socket
from arduino import arduinoSerial

#
# Setup UDP socket.
#
UDP_IP = "127.0.0.1"
UDP_PORT = 8092
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

#
#
#
arduino = arduinoSerial()

data = {}
data['command'] = 'getStatus'

while( True ):
   arduino.write(json.dumps(data))

   object = json.loads(arduino.read())
   MESSAGE = "environment,site=Ballard,location=Outside temperature=%s,humidity=%s,pressure=%s,light=%s,windDir=%s,windSpeed=%s,rain=%s,gust=%s" % (object['temp'] , object['humidity'], object['pressure'], object['light'], object['windDir'], object['windSpeed'], object['rain'], object['gust'] )
   sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
#   print( MESSAGE )
   time.sleep (1)
