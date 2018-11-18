#!/usr/bin/python

import json
import time
import requests
from kafka import KafkaProducer
from kafka.errors import KafkaError
from arduino import arduinoSerial

#
# Oh where is the ardunio?
#
arduino = arduinoSerial()

producer = KafkaProducer(bootstrap_servers=['kafka.pad.the-collective-group.com:9092'], value_serializer=lambda v: json.dumps(v).encode('ascii'))

print "Starting loop"
while( True ):

   data = {}
   event = {}

   data['command'] = 'getParticles'
   arduino.write(json.dumps(data))
  
   event['pm'] = json.loads(arduino.read())
   if event['pm']["status"] != "OK":
     print "ERROR: " + str( event['pm']["status"] )
     continue 

   data['command'] = 'getWeather'
   arduino.write(json.dumps(data))

   event['weather'] = json.loads(arduino.read())
   if event['weather']["status"] != "OK":
     print "ERROR: " + str( event['weather']["status"] )
     continue

   event["site"] = "Pad"
   event["location"] = "Bedroom"
   
   producer.send('environment', event)

   time.sleep (5)
