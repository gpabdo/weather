#include "voc.h"

//**** voc() **********************************************//
//
//*********************************************************//
voc::voc()
{
  Wire.begin();
  
  //Initialize sensor
  if (mySensor.begin() == true) {
    mySensor.initAirQuality();
  }
}


//**** measure() ******************************************//
//
//*********************************************************//
bool voc::measure()
{
  mySensor.measureAirQuality();
  return true;
}


//**** getCO2() *******************************************//
//
//*********************************************************//
float voc::getCO2()
{
  return mySensor.CO2;
}


//**** getTVOC() ******************************************//
//
//*********************************************************//
float voc::getTVOC()
{
  return mySensor.TVOC;
}


//**** setSensorHumidity **********************************//
//
//*********************************************************//
void voc::setSensorHumidity( float relHumidity, float tempF )
{
  uint16_t sensHumidity = doubleToFixedPoint( RHtoAbsolute( relHumidity, ( tempF - 32) * 5 / 9 ) );
  mySensor.setHumidity( sensHumidity );
}

double voc::RHtoAbsolute (float relHumidity, float tempC) {
  double eSat = 6.11 * pow(10.0, (7.5 * tempC / (237.7 + tempC)));
  double vaporPressure = (relHumidity * eSat) / 100; //millibars
  double absHumidity = 1000 * vaporPressure * 100 / ((tempC + 273) * 461.5); //Ideal gas law with unit conversions
  return absHumidity;
}

uint16_t voc::doubleToFixedPoint( double number) {
  int power = 1 << 8;
  double number2 = number * power;
  uint16_t value = floor(number2 + 0.5);
  return value;
}
