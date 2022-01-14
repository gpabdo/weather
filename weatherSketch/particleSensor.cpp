#include "particleSensor.h"


//**** particleSensor() ***********************************//
//
//*********************************************************//
particleSensor::particleSensor()
{
   
  aqi.begin_I2C();
 
  data.particles_03um = 0;
  data.particles_05um = 0;
  data.particles_10um = 0;
  data.particles_25um = 0;
  data.particles_50um = 0;
  data.particles_100um = 0;
}


//**** load() *********************************************//
//
//*********************************************************//
float particleSensor::get03()
{
  return data.particles_03um;
}

//**** load() *********************************************//
//
//*********************************************************//
float particleSensor::get05()
{
  return data.particles_05um;
}


//**** load() *********************************************//
//
//*********************************************************//
float particleSensor::get10()
{
  return data.particles_10um;
}


//**** load() *********************************************//
//
//*********************************************************//
float particleSensor::get25()
{
  return data.particles_25um;
}


//**** load() *********************************************//
//
//*********************************************************//
float particleSensor::get50()
{
  return data.particles_50um;
}


//**** load() *********************************************//
//
//*********************************************************//
float particleSensor::get100()
{
  return data.particles_100um;
}


//**** load() *********************************************//
//
//*********************************************************//
bool particleSensor::load()
{  
  if (! aqi.read(&data)) {
    return false;
  }

  return true;
}
