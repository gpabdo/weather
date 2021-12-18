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
           print("Please set the environment variable SITE")
           sys.exit(1)

    try:  
           location = os.environ["LOCATION"]
    except KeyError: 
           print("Please set the environment variable LOCATION")
           sys.exit(1)

    try:  
           api_url = os.environ["API_URL"]
    except KeyError: 
           print("Please set the environment variable API_URL")
           sys.exit(1)

    try:  
           device = os.environ["DEVICE"]
    except KeyError: 
           print("Please set the environment variable DEVICE")
           sys.exit(1)

    
    collect( site=site, location=location, api_url=api_url, device=device )


## ----------------------------------------- ## 
#
## ----------------------------------------- ## 
def collect( site, location, api_url, device ):
 
  print("Init:")
  print("SITE: " + site)
  print("LOCATION: " + location)
  print("DEVICE: " + device)
  print("API_URL: " + api_url)


  arduino = arduinoSerial( device=device )

  print("Starting loop")
  while( True ):

     data = {}
     event = {}

     # Get particle data
     data['command'] = "getParticles"
     arduino.write(json.dumps(data))
    
     event['pm'] = json.loads(arduino.read())
     if event['pm']["status"] != "OK":
       print("ERROR: " + str( event['pm']["status"] ))
       continue


     # Get weather data
     data['command'] = 'getWeather'
     arduino.write(json.dumps(data))

     event['weather'] = json.loads(arduino.read())
     if event['weather']["status"] != "OK":
       print("ERROR: " + str( event['weather']["status"] ))
       continue


     # Get VOC data
     data['command'] = 'getVoc'
     arduino.write(json.dumps(data))

     event['voc'] = json.loads(arduino.read())
     if event['voc']["status"] != "OK":
       print("ERROR: " + str( event['voc']["status"] ))
       continue


     # Add location data
     event["site"] = site 
     event["location"] = location 

     # Send data
     headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}
     print( event )

#     r = requests.post(url = api_url, json = event, headers=headers ) 
 
#     print(r.text)

     time.sleep (5)


## ----------------------------------------- ##
#
## ----------------------------------------- ##
if __name__ == '__main__':
  main(sys.argv)  
