#!/usr/bin/python2.7 

import os
import sys
import argparse 
import json
import time
from arduinoCommunication import arduinoSerial
import requests 

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
           api_url = os.environ["API_URL"]
    except KeyError: 
           print "Please set the environment variable API_URL"
           sys.exit(1)

    try:  
           device = os.environ["DEVICE"]
    except KeyError: 
           print "Please set the environment variable DEVICE"
           sys.exit(1)

    
    collect( site=site, location=location, api_url=api_url, device=device )


## ----------------------------------------- ## 
#
## ----------------------------------------- ## 
def collect( site, location, api_url, device ):

  arduino = arduinoSerial( device=device )

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

     data['command'] = 'getVoc'
     arduino.write(json.dumps(data))

     event['voc'] = json.loads(arduino.read())
     if event['voc']["status"] != "OK":
       print "ERROR: " + str( event['voc']["status"] )
       continue

     event["site"] = site 
     event["location"] = location 
     
     r = requests.post(url = api_url, data = event) 
 
     print(r.text)

     time.sleep (5)


## ----------------------------------------- ##
#
## ----------------------------------------- ##
if __name__ == '__main__':
  main(sys.argv)  
