#ifndef voc_h
#define voc_h

#include <SparkFun_SGP30_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_SGP30

class voc
{
  
  public:
    voc();
    bool measure();
    bool isConnected();
    void setSensorHumidity( float, float );
    float getCO2();
    float getTVOC();
    
  private:
    SGP30 mySensor;
    bool deviceConnected;
    uint16_t doubleToFixedPoint( double );
    double RHtoAbsolute (float, float ); 
     
};

#endif
