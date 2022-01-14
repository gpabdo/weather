import serial
import time

class arduinoSerial:

    arduino = None
    status = False

    def __init__(self, device='/dev/ttyUSB0', serial_speed=57600):
        self.arduino = serial.Serial(device, serial_speed)
        self.status = True
        time.sleep(3)

    def isConnected(self):
        return self.status

    def write(self, message):
        if self.isConnected():
            self.arduino.write( str.encode(message))
            self.arduino.write( str.encode('\n'))
        else:
            return 0 

    def isAvalible(self):
        return self.arduino.in_waiting


    def read(self):
        if self.isConnected():
            return self.arduino.readline()
        else:
            return 0
