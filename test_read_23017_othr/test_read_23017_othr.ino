#include <Wire.h> // Wire.h
byte inputa=0; byte inputb=0;
void setup()
{
  Serial.begin(9600);
  Wire.begin(); // wake up I2C bus
  Wire.beginTransmission(0x20);
  Wire.write(0x00); // IODIRA register
  Wire.write(0x00); // set entire PORT A as output
  Wire.endTransmission();
}
 
void loop()
{
  // read the inputs of bank B
  Wire.beginTransmission(0x20);
  Wire.write(0x13);
  Wire.endTransmission();
  Wire.requestFrom(0x20, 1);
  inputa=Wire.read();
  Serial.println(inputa); Serial.print("  ");
  // now send the input data to bank A
  Wire.beginTransmission(0x20);
  Wire.write(0x12); // address PORT A
  Wire.endTransmission();
  Wire.requestFrom(0x20, 1);
  inputb=Wire.read();
  Serial.print(inputb);
  delay(100); // for debounce
}
