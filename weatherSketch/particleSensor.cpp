#include "particleSensor.h"


//**** particleSensor() ***********************************//
//
//*********************************************************//
particleSensor::particleSensor()
{
  s = new SoftwareSerial(SENSOR_RX_PIN, SENSOR_TX_PIN);
  s->begin(9600);
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
  return s->available();
  return data.particles_03um;
}


String particleSensor::getError()
{
  return error;
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
  if (! s->available()) {
    error = "NO_DATA";
    return false;
  }

  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }

  // Now read all 32 bytes
  if (s->available() < 32) {
    error = "NOT_32_BYTES";
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);

  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }

  /* debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  */

  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }

  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);

  if (sum != data.checksum) {
    error = "INVALID_CHECKSUM";
    return false;
  }
  // success!
  return true;
}

