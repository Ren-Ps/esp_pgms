#include <Wire.h>
// address of PCF8574 IC
#define PCF8574_ADDR (0x20)
 
void setup()
{
  Wire.begin();
}
 
void loop()
{
  //send the data //0x00 to 0xFF
  Wire.beginTransmission(PCF8574_ADDR);
  Wire.write(0xFF);
  Wire.endTransmission();
  delay(1000);
  Wire.beginTransmission(PCF8574_ADDR);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(1000);
}
