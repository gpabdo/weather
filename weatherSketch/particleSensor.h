#ifndef ParticleSensor_h
#define ParticleSensor_h

#include "Adafruit_PM25AQI.h"

class particleSensor
{
  public:
    particleSensor();
    bool load();
    float get03();
    float get05();
    float get10();
    float get25();
    float get50();
    float get100();

  private:
    Adafruit_PM25AQI aqi;
    PM25_AQI_Data data;

};

#endif
