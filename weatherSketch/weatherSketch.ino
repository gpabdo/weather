//---- WeatherSketch --------------------//
#include "weatherStation.h"
#include "particleSensor.h"
#include <ArduinoJson.h>


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(57600);
}

void loop() {
  weatherStation *theWeatherStation = new weatherStation();
  particleSensor *theParticleSensor = new particleSensor();
  String data = "";
  String status = "off";
  digitalWrite(LED_BUILTIN, LOW);

  while( true )
  {
    theParticleSensor->load();
    
    // If there is datai n the buffer.
    if( Serial.available() )
    {
       data = Serial.readStringUntil('\n');

       StaticJsonBuffer<200> jsonBuffer;
       JsonObject& json = jsonBuffer.parseObject(data);

       String command = json["command"];
       if ( command == "getWeather" )
       {
          digitalWrite(LED_BUILTIN, HIGH);
          JsonObject& reply = jsonBuffer.createObject();
          reply["windSpeed"] = double_with_n_digits( theWeatherStation->getWindSpeed(), 4);
          reply["windDir"] = double_with_n_digits( theWeatherStation->getWindDirection(), 4);
          reply["rain"] = double_with_n_digits( theWeatherStation->getRain(), 4);
          reply["temp"] = double_with_n_digits( theWeatherStation->getTemp(), 4);
          reply["humidity"] = double_with_n_digits( theWeatherStation->getHumidity(), 4);
          reply["pressure"] = double_with_n_digits( theWeatherStation->getPressure(), 6);
          reply["batt"] = double_with_n_digits( theWeatherStation->getBatteryLevel(), 4);
          reply["light"] = double_with_n_digits( theWeatherStation->getLightLevel(), 4);
          reply["gust"] = double_with_n_digits( theWeatherStation->getWindGust(), 4);
          reply["status"] = "OK";
          
          digitalWrite(LED_BUILTIN, LOW);
          
          reply.printTo(Serial);
          Serial.println();
       }

       else if ( command == "getParticles" )
       {
         digitalWrite(LED_BUILTIN, HIGH);
         JsonObject& reply = jsonBuffer.createObject(); 

         reply["PM_0.3"] = double_with_n_digits( theParticleSensor->get03(), 4);
         reply["PM_0.5"] = double_with_n_digits( theParticleSensor->get05(), 4);
         reply["PM_1.0"] = double_with_n_digits( theParticleSensor->get10(), 4);
         reply["PM_2.5"] = double_with_n_digits( theParticleSensor->get25(), 4);
         reply["PM_5.0"] = double_with_n_digits( theParticleSensor->get50(), 4);
         reply["PM_10.0"] = double_with_n_digits( theParticleSensor->get100(), 4);
         reply["status"] = "OK";

         reply.printTo(Serial);
         Serial.println();
         }
       
       else
       {
         JsonObject& reply = jsonBuffer.createObject();
         reply["status"] = "UNKOWN_COMMAND";
         reply.printTo(Serial);
         Serial.println(); 
       }
    }
  }
}


