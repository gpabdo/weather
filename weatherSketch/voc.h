#ifndef voc_h
#define voc_h

#include <SparkFun_SGP30_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_SGP30
#include <Wire.h>

class voc
{
  
  public:
    voc();
    bool measure();
    void setSensorHumidity( float, float );
    float getCO2();
    float getTVOC();
    
  private:
    SGP30 mySensor;
    uint16_t doubleToFixedPoint( double );
    double RHtoAbsolute (float, float ); 
     
};

#endif
