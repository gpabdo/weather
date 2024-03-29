#include "weatherStation.h"

bool weatherStation::led = 1;
volatile float weatherStation::windSpeed = 0;
volatile float weatherStation::rainLevel = 0;
volatile float weatherStation::gust = 0;
volatile unsigned long weatherStation::lastWindIRQ = 0;
volatile unsigned long weatherStation::lastRainIRQ = 0;

//**** weatherStation() ***********************************//
//
//*********************************************************//
weatherStation::weatherStation()
{
  Wire.begin();
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
  myPressure.setOversampleRate(7);    // Set Oversample to the recommended 128
  myPressure.enableEventFlags();      // Enable all three pressure and temp event flags
  myPressure.setModeActive();         // Go to active mode and start measuring!

  //Configure the humidity sensor
  myHumidity.begin();

  // attach external interrupt pins to IRQ functions
  // The uno is f-ed, pin 2 is int 0 and pin 3 is int 1.
  attachInterrupt(0, static_cast<void (*)()>(&rainIRQ), FALLING);
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
  long currentRainIRQ = millis();

  // ignore switch-bounce glitches less than 100mS after initial edge
  if (currentRainIRQ - lastRainIRQ > 100)  
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
  long currentWindIRQ = millis();

  // Ignore switch-bounce glitches less than 10ms (142MPH max reading) after the reed switch closes
  if ( currentWindIRQ - lastWindIRQ > 10 ) 
  {
    led = !led;
    digitalWrite(STAT1, led);
    // The wind speed is the time since the last click devided by 1.492 mph and 
    //then multiplied by 1000ms. There is 1.492MPH for each click per second.
    windSpeed = 1000.0 / (float)( currentWindIRQ - lastWindIRQ ) * 1.492;

    if( windSpeed > gust )
        gust = windSpeed;
           
    lastWindIRQ = millis();                                           
  }
}


//**** getRain() ******************************************//
// Returns the amount of rain since last checked.
//*********************************************************//
float weatherStation::getRain()
{
  // There has been some rain since last checked, 
  //return the ammount in inches.
  float rainValue = rainLevel;
  rainLevel = 0;
  
  return(rainValue);
}


//**** getWindSpeed() *************************************//
// Returns the instataneous wind speed
//*********************************************************//
float weatherStation::getWindSpeed()
{

  // If there have been no clicks for at least 3 seconds,
  // there is no wind.
  if( millis() - lastWindIRQ > 3000 )
     windSpeed = 0;

  // Return the last calculated wind speed.
  return(windSpeed);
}


//**** getWindGust() **************************************//
// Returns the instataneous wind speed
//*********************************************************//
float weatherStation::getWindGust()
{
    float lastGust = gust;
    gust = 0;
    return lastGust;
}


//**** getWindDirection() *********************************//
// Read the wind direction sensor, return heading in degrees 
//*********************************************************//
int weatherStation::getWindDirection()
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
    //return( myHumidity.readHumidity() );
    return( myHumidity.getRH() );
}


//**** getTemp() ******************************************//
//
//*********************************************************//
float weatherStation::getTemp()
{
   return( myPressure.readTempF() );
}


//**** getPressure() **************************************//
// Return pressure in millibars.
//*********************************************************//
float weatherStation::getPressure()
{
    return( myPressure.readPressure() / 100.0 );
}


//**** getLightLevel() ************************************//
// Returns the voltage of the light sensor based on the 3.3V 
// rail.
// This allows us to ignore what VCC might be (an Arduino 
// plugged into USB has VCC of 4.5 to 5.2V)
//*********************************************************//
float weatherStation::getLightLevel()
{
  float operatingVoltage = averageAnalogRead(REFERENCE_3V3);

  float lightSensor = averageAnalogRead(LIGHT);

  operatingVoltage = 3.3 / operatingVoltage; //The reference voltage is 3.3V

  lightSensor *= operatingVoltage;

  return(lightSensor);
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
float weatherStation::getBatteryLevel()
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

