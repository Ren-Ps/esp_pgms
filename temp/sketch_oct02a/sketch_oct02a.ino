#include <Wire.h>


#define DEV1 B0100000
#define DEV2 B0100001

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    //IOexpanderWrite(DEVICE_2, 0x0F);
}

void loop()
{
   byte k=0;
   Wire.requestFrom(DEV1, 1);
 if(Wire.available())
   k = Wire.read();
   Serial.println(k);
   k=(k>>4)|0xF0;
   //Serial.println(k);

//   Wire.beginTransmission(DEV1);
//   Wire.write(k);
//   Wire.endTransmission();

}
