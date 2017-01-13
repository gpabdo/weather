#ifndef WeatherStation_h
#define WeatherStation_h

#include <avr/wdt.h>            //We need watch dog for this program
#include <Wire.h>               //I2C needed for sensors
#include <SparkFunMPL3115A2.h>  //Pressure sensor
#include <SparkFunHTU21D.h>     //Humidity sensor
//#include "AS3935.h"             //Lighting dtector
//#include <SPI.h>                //Needed for lighting sensor

#define WSPEED 3
#define RAIN 2
#define STAT1 7
//const byte LIGHTNING_IRQ = 4;     //Not really an interrupt pin, we will catch it in software
//const byte slaveSelectPin = 10;   //SS for AS3935

// analog I/O pins
#define WDIR A0
#define LIGHT A1
#define BATT A2
#define REFERENCE_3V3 A3


class weatherStation
{

  public:
  	weatherStation();
 	  float get_light_level();
 	  float get_rain_level();
 	  float get_wind_speed();
 	  int get_wind_direction();
 	  float get_battery_level();
 	  float getHumidity();
 	  float getTemp();
 	  float getPressure();


  private:
  	static void rainIRQ();
  	static void wspeedIRQ();
	  int averageAnalogRead(int);

	  static bool led;	
	  static volatile unsigned long rainLevel, lastWindIRQ, lastRainIRQ;
    static volatile float windSpeed;

	  MPL3115A2 myPressure;   //Create an instance of the pressure sensor
	  HTU21D myHumidity;      //Create an instance of the humidity sensor

};

#endif

