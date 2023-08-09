// inslude the SPI library:
#include <SPI.h>
const int chipSelect = 10;

void sendCommand(int address,int data) {
  digitalWrite(chipSelect, LOW);   delay(1);
  //  send in the address and value via SPI:
  // SPI addressing 0x40 with A0 and A1 set to ground and R/W bit set to output
  SPI.transfer(0x40); 
  SPI.transfer(address);
  SPI.transfer(data); delay(1);
  // take the chipSelect pin high to de-select the chip:
  digitalWrite(chipSelect, HIGH);
}

void setup() {
  // initialize SPI:
  SPI.begin();
  pinMode(chipSelect, OUTPUT);
  digitalWrite(chipSelect, HIGH);
  sendCommand(0x00, 0x00);
}

void loop() {
  sendCommand(0x09,0xff);
  delay(1000);
  sendCommand(0x09,0x00);
  delay(1000);
}
