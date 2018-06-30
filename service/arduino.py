#!/usr/bin/python

import serial
import time

## ------------------------- ##
#
## ------------------------- ##
class arduinoSerial:

  arduino = None
  status = False

  ## ------------------------- ##
  #
  ## ------------------------- ##
  def __init__(self, device='/dev/ttyUSB0', serial_speed=115200):
    self.arduino = serial.Serial(device, serial_speed)
    self.status = True
    time.sleep(3)

  ## ------------------------- ##
  #
  ## ------------------------- ##
  def isConnected(self):       
    return self.status

  ## ------------------------- ##
  #
  ## ------------------------- ##
  def write(self, message):
    if self.isConnected():
      self.arduino.write(message)
      self.arduino.write('\n')
    else:
      return 0 

  ## ------------------------- ##
  #
  ## ------------------------- ##
  def read(self):
    if self.isConnected():
      return self.arduino.readline()
    else:
      return 0
