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


token = "YANPqMm_P2zA0GMMxwH-uj-pSOAlpq5Ylx2jmeqZnOi8X2PppAxp5ir454N3C1k-6j2S6q0z9z5cXTjFMEdxcQ=="
org = "weather"
bucket = "Environment"

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

  with InfluxDBClient(url=api_url, token=token, org=org, ssl=False, verify_ssl=False ) as client:
    write_api = client.write_api(write_options=SYNCHRONOUS)

    print("Starting stupid loop")
    while( True ):

      while( arduinoDevice.isAvalible() ):
        print("Clearing Buffer")
        print( arduinoDevice.read() )

      data = {}
      event = {}

      # Get weather data
      data['command'] = 'getWeather'
      arduinoDevice.write(json.dumps(data))

      try:
        event['weather'] = json.loads(arduinoDevice.read())
        if event['weather']["status"] != "OK":
          print("ERROR: " + str( event['weather']["status"] ))
          continue
      except:
        print("Error reading getWeather json")
        break


      # Get particle data
      data['command'] = 'getParticles'
      arduinoDevice.write(json.dumps(data))

      try:
        event['pm'] = json.loads(arduinoDevice.read())
        if event['pm']["status"] != "OK":
          print("ERROR: " + str( event['pm']["status"] ))
          continue
      except:
        print("Error reading getWeather json")
        break

      # Get VOC data
      data['command'] = 'getVoc'
      arduinoDevice.write(json.dumps(data))

      try:
        event['voc'] = json.loads(arduinoDevice.read())
        if event['voc']["status"] != "OK":
          print("ERROR: " + str( event['voc']["status"] ))
          continue
      except:
        print("Error reading getVoc json")
        break

      point = Point("Environment") \
        .tag("Site", site) \
        .tag("Location", location)

      if 'weather' in event.keys():
        try: 
          point.field("Temp", float(event['weather']['temp'] ))
          point.field("WindSpeed", float(event['weather']['windSpeed'] )) 
          point.field("WindDir", event['weather']['windDir'] )
          point.field("Gust", float(event['weather']['gust'] ))
          point.field("Rain", float(event['weather']['rain'] ))
          point.field("Humidity", float(event['weather']['humidity'] ))
          point.field("Pressure", float(event['weather']['pressure'] ))
        except:
          print("Error adding weather: " + str( event ) )

      if 'voc' in event.keys():
        try:
          point.field("CO2", event['voc']['CO2'] )
          point.field("TOVC", event['voc']['TVOC'] )
        except:
          print("Error adding voc: " + str( event ) )

      if 'pm' in event.keys():
        try:
          point.field("Particles_03um", event['pm']['PM_0.3'] )
          point.field("Particles_05um", event['pm']['PM_0.5'] )
          point.field("Particles_10um", event['pm']['PM_1.0'] )
          point.field("Particles_25um", event['pm']['PM_2.5'] )
          point.field("Particles_50um", event['pm']['PM_5.0'] )
          point.field("Particles_100um", event['pm']['PM_10.0'] )
        except:
          print("Error adding pm: " + str( event ) )        

      point.time(datetime.utcnow(), WritePrecision.NS)

      write_api.write( bucket=bucket, record=point)
      print( str( datetime.now() ) +  " [INFO] : Point written")
      time.sleep (5)

## ----------------------------------------- ##
#
## ----------------------------------------- ##
if __name__ == '__main__':
  main(sys.argv)  
