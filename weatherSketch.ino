//---- WeatherSketch --------------------//
#include "weatherStation.h"

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

 weatherStation *theWeatherStation = new weatherStation();

 while( true )
 {
     if ( theWeatherStation->get_wind_speed() > 1 )
        digitalWrite(LED_BUILTIN, HIGH);
     else
        digitalWrite(LED_BUILTIN, LOW);
        
 }
}
