#!/usr/bin/python2.7 

import os
import sys
import argparse 
import json
import time
from datetime import datetime
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
from arduinoCommunication import arduinoSerial


token = "mXsXRVbnQUbrJ6rJmIFlY3NaI8JZneJTmXN_sZxVW5qkDuyKgwzTyn-L-TDU0tDOW0C0M9kJyUXazT6F3c8law=="
org = "weather"
bucket = "weather"

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

  arduinoDevice = arduinoSerial.arduinoSerial( device=device )

  print("Starting loop")

  with InfluxDBClient(url=api_url, token=token, org=org, ssl=False, verify_ssl=False ) as client:
    write_api = client.write_api(write_options=SYNCHRONOUS)

    while( True ):

      data = {}
      event = {}

      # Get particle data
      data['command'] = 'getParticles'
      arduinoDevice.write(json.dumps(data))
    
      event['pm'] = json.loads(arduinoDevice.read())
      if event['pm']["status"] != "OK":
        print("ERROR: " + str( event['pm']["status"] ))
        continue

      # Get weather data
      data['command'] = 'getWeather'
      arduinoDevice.write(json.dumps(data))

      event['weather'] = json.loads(arduinoDevice.read())
      if event['weather']["status"] != "OK":
        print("ERROR: " + str( event['weather']["status"] ))
        continue


      # Get VOC data
      data['command'] = 'getVoc'
      arduinoDevice.write(json.dumps(data))

      event['voc'] = json.loads(arduinoDevice.read())
      if event['voc']["status"] != "OK":
        print("ERROR: " + str( event['voc']["status"] ))
        continue

      point = Point("environment") \
        .tag("Site", site) \
        .tag("Location", location) \
        .field("Temp", event['weather']['temp'] ) \
        .field("WindSpeed", event['weather']['windSpeed'] ) \
        .field("WindDir", event['weather']['windDir'] ) \
        .field("Rain", event['weather']['rain'] ) \
        .field("Humidity", event['weather']['humidity'] ) \
        .field("Pressure", event['weather']['pressure'] ) \
        .field("Particles_03um", event['pm']['PM_0.3'] ) \
        .field("Particles_05um", event['pm']['PM_0.5'] ) \
        .field("Particles_10um", event['pm']['PM_1.0'] ) \
        .field("Particles_25um", event['pm']['PM_2.5'] ) \
        .field("Particles_50um", event['pm']['PM_5.0'] ) \
        .field("Particles_100um", event['pm']['PM_10.0'] ) \
        .field("CO2", event['voc']['CO2'] ) \
        .field("TOVC", event['voc']['TVOC'] ) \
        .time(datetime.utcnow(), WritePrecision.NS)


      write_api.write( bucket=bucket, record=point)
      time.sleep (5)


## ----------------------------------------- ##
#
## ----------------------------------------- ##
if __name__ == '__main__':
  main(sys.argv)  
