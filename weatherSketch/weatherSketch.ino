//---- WeatherSketch --------------------//
#include "weatherStation.h"

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

 weatherStation *theWeatherStation = new weatherStation();

 while( true )
 {
     
 }
}
