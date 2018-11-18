#!/usr/bin/python2.7 

import sys
import argparse 
import json
import time
import requests
from arduinoCommunication import arduinoSerial
from kafka import KafkaProducer
from kafka.errors import KafkaError


## ----------------------------------------- ##
#
## ----------------------------------------- ##
def main(argv):
  parser = argparse.ArgumentParser()
  parser.add_argument('-s', '--site',
                    required=True,
                    dest="site",
                    help="Geographic site of the weatherstation" )

  parser.add_argument('-l', '--location',
                    required=True,
                    dest="location",
                    help="Room the weatherstatoin is in" )
  
  parser.add_argument('-k', '--kafka',
                    required=False,
                    dest="kafka",
                    help="Kafka bootstrap servers" )

  args = parser.parse_args()
  collect( site=args.site, location=args.location, kafka=args.kafka )


## ----------------------------------------- ## 
#
## ----------------------------------------- ## 
def collect( site="default", location="default", kafka='kafka.pad.the-collective-group.com:9092' ):

  arduino = arduinoSerial()
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
