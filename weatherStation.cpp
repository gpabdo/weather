#include "weatherStation.h"

bool weatherStation::led = 1;
volatile unsigned long weatherStation::windSpeed = 1;
volatile unsigned long weatherStation::rainLevel = 1;
volatile unsigned long weatherStation::lastWindIRQ = 1;
volatile unsigned long weatherStation::lastRainIRQ = 1;

//**** weatherStation() ***********************************//
//
//*********************************************************//
weatherStation::weatherStation()
{

  pinMode(WSPEED, INPUT_PULLUP);      // input from wind meters windspeed sensor
  pinMode(RAIN, INPUT_PULLUP);        // input from wind meters rain gauge sensor

  pinMode(WDIR, INPUT);
  pinMode(LIGHT, INPUT);
  pinMode(BATT, INPUT);
  pinMode(REFERENCE_3V3, INPUT);

  pinMode(STAT1, OUTPUT);

  //Configure the pressure sensor
  myPressure.begin();                 // Get sensor online
  myPressure.setModeBarometer();      // Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(128);  // Set Oversample to the recommended 128
  myPressure.enableEventFlags();      // Enable all three pressure and temp event flags
  myPressure.setModeActive();         // Go to active mode and start measuring!

  //Configure the humidity sensor
  myHumidity.begin();

  // attach external interrupt pins to IRQ functions
  attachInterrupt(RAIN, static_cast<void (*)()>(&rainIRQ), FALLING);
  attachInterrupt(1, static_cast<void (*)()>(&wspeedIRQ), FALLING);

  // turn on interrupts
  interrupts();

}


//**** rainIRQ() ******************************************//
// Count rain gauge bucket tips as they occur
// Activated by the magnet and reed switch in the rain gauge, 
// attached to input D2
//*********************************************************//
void weatherStation::rainIRQ()
{
  float currentRainIRQ = millis();

  // ignore switch-bounce glitches less than 10mS after initial edge
  if (currentRainIRQ - lastRainIRQ > 10)  
  {
    rainLevel += 0.011;                   //Each dump is 0.011" of water
    lastRainIRQ = millis();
  }
}


//**** wspeedIRQ() ****************************************//
// Activated by the magnet in the anemometer (2 ticks per 
// rotation), attached to input D3
//*********************************************************//
void weatherStation::wspeedIRQ()
{
  float currentWindIRQ = millis();

  // Ignore switch-bounce glitches less than 10ms (142MPH max reading) after the reed switch closes
  if (currentWindIRQ - lastWindIRQ > 10)    
  {
     led = !led;
    digitalWrite(STAT1, led);
    // The wind speed is the time since the last click devided by 1.492 mph and 
    //then multiplied by 1000ms. There is 1.492MPH for each click per second.
    windSpeed = ( currentWindIRQ - lastWindIRQ ) / 1.492 * 1000; 
    lastWindIRQ = millis();                                           
  }
}


//**** get_light_level() **********************************//
// Returns the voltage of the light sensor based on the 3.3V 
// rail.
// This allows us to ignore what VCC might be (an Arduino 
// plugged into USB has VCC of 4.5 to 5.2V)
//*********************************************************//
float weatherStation::get_light_level()
{
 /* float operatingVoltage = averageAnalogRead(REFERENCE_3V3);

  float lightSensor = averageAnalogRead(LIGHT);

  operatingVoltage = 3.3 / operatingVoltage; //The reference voltage is 3.3V

  lightSensor *= operatingVoltage;

  return(lightSensor); */
  return( 0 );
}


//**** get_rain_level() ***********************************//
// Returns the amount of rain since last checked.
//*********************************************************//
float weatherStation::get_rain_level()
{
  // If rainLevel is 0, don't do a reset just incase
  //we get interrupted right here for a rain dump.
  if( rainLevel == 0)
    return 0;

  // There has been some rain since last checked, 
  //return the ammount in inches.
  float rainValue = rainLevel;
  rainLevel = 0;
  
  return(rainValue);
}


//**** get_wind_speed() ***********************************//
// Returns the instataneous wind speed
//*********************************************************//
float weatherStation::get_wind_speed()
{

  // If there have been no clicks for 2 at least 2 seconds.
  //then the wind is just zero.
  if( millis() - lastWindIRQ > 2000 )
    return 0; 

  // Return the last calculated wind speed.
  return(windSpeed);
}


//**** get_wind_direction() *******************************//
// Read the wind direction sensor, return heading in degrees 
//*********************************************************//
int weatherStation::get_wind_direction()
{
  unsigned int adc = averageAnalogRead(WDIR);   // get the current reading from the sensor

  // The following table is ADC readings for the wind direction sensor output, sorted from low to high.
  // Each threshold is the midpoint between adjacent headings. The output is degrees for that ADC reading.
  // Note that these are not in compass degree order! See Weather Meters datasheet for more information.

  if (adc < 380) return (113);
  if (adc < 393) return (68);
  if (adc < 414) return (90);
  if (adc < 456) return (158);
  if (adc < 508) return (135);
  if (adc < 551) return (203);
  if (adc < 615) return (180);
  if (adc < 680) return (23);
  if (adc < 746) return (45);
  if (adc < 801) return (248);
  if (adc < 833) return (225);
  if (adc < 878) return (338);
  if (adc < 913) return (0);
  if (adc < 940) return (293);
  if (adc < 967) return (315);
  if (adc < 990) return (270);
  return (-1); // error, disconnected?
}


//**** getHumidity() **************************************//
//
//*********************************************************//
float weatherStation::getHumidity()
{
    return( myHumidity.readHumidity() );
}


//**** getTemp() ******************************************//
//
//*********************************************************//
float weatherStation::getTemp()
{
   return( myPressure.readTempF() );
}


//**** getPressure() **************************************//
//
//*********************************************************//
float weatherStation::getPressure()
{
    return( myPressure.readPressure() );
}


//**** get_battery_level() ********************************//
// Returns the voltage of the raw pin based on the 3.3V rail
// The battery can ranges from 4.2V down to around 3.3V
// This function allows us to ignore what VCC might be 
// (an Arduino plugged into USB has VCC of 4.5 to 5.2V)
// The weather shield has a pin called RAW (VIN) fed through 
// through two 5% resistors and connected to A2 (BATT):
// 3.9K on the high side (R1), and 1K on the low side (R2)
//*********************************************************//
float weatherStation::get_battery_level()
{
  float operatingVoltage = averageAnalogRead(REFERENCE_3V3);

  float rawVoltage = averageAnalogRead(BATT);

  operatingVoltage = 3.30 / operatingVoltage; //The reference voltage is 3.3V

  rawVoltage *= operatingVoltage; //Convert the 0 to 1023 int to actual voltage on BATT pin

  rawVoltage *= 4.90; //(3.9k+1k)/1k - multiply BATT voltage by the voltage divider to get actual system voltage

  return(rawVoltage);
}


int weatherStation::averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);
}

