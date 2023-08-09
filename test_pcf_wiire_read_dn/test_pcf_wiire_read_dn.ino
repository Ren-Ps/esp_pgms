#include <Wire.h>
#define PCF8574_ADDR (0x20)

byte x = 0;

void setup()
{
  Wire.begin();
  Wire.beginTransmission(PCF8574_ADDR);
  Wire.write(0xFF);
  Wire.endTransmission();
  Serial.begin(9600);
}

void loop()
{
  Wire.requestFrom(PCF8574_ADDR, 1);
  while (Wire.available())
  {
    x = Wire.read();
    Serial.println(x);
  }
}
