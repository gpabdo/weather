#ifndef ParticleSensor_h
#define ParticleSensor_h

#define SENSOR_RX_PIN 2
#define SENSOR_TX_PIN 3

#include <SoftwareSerial.h>

struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

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
    String getError();

  private:
    SoftwareSerial * s;
    struct pms5003data data;
    String error;

};

#endif
