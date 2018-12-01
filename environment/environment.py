#!/usr/bin/python2.7 

import os
import sys
import argparse 
import json
import time
from arduinoCommunication import arduinoSerial
from kafka import KafkaProducer
from kafka.errors import KafkaError


## ----------------------------------------- ##
#
## ----------------------------------------- ##
def main(argv):
    try:  
           site = os.environ["SITE"]
    except KeyError: 
           print "Please set the environment variable SITE"
           sys.exit(1)

    try:  
           location = os.environ["LOCATION"]
    except KeyError: 
           print "Please set the environment variable LOCATION"
           sys.exit(1)

    try:  
           kafka = os.environ["KAFKA"]
    except KeyError: 
           print "Please set the environment variable KAFKA"
           sys.exit(1)

    try:  
           device = os.environ["DEVICE"]
    except KeyError: 
           print "Please set the environment variable DEVICE"
           sys.exit(1)

    
    collect( site=site, location=location, kafka=kafka, device=device )


## ----------------------------------------- ## 
#
## ----------------------------------------- ## 
def collect( site, location, kafka, device ):

  arduino = arduinoSerial( device=device )
  producer = KafkaProducer(bootstrap_servers=[kafka], value_serializer=lambda v: json.dumps(v).encode('ascii'))

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

     event["site"] = site 
     event["location"] = location 
     
     producer.send('environment', event)

     time.sleep (5)


## ----------------------------------------- ##
#
## ----------------------------------------- ##
if __name__ == '__main__':
  main(sys.argv)  
