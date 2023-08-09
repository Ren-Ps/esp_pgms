
#include <SPI.h>

#define SS 10
#define CHIP 0x40
#define CHIP_READ 0x41
#define IO_DIR_REG 0x00
#define GPIO_REG 0x09

void SPIWrite(byte address, byte data){
 digitalWrite(SS,LOW);
 SPI.transfer(CHIP);
 SPI.transfer(address);
 SPI.transfer(data);
 digitalWrite(SS,HIGH);

}

byte SPIRead(byte address){
 digitalWrite(SS,LOW);
 SPI.transfer(CHIP_READ);
 SPI.transfer(address);
 byte retVal=SPI.transfer(0x00);
 digitalWrite(SS,HIGH);
 return retVal;
}

void setup() {
 Serial.begin(9600);
 while (!Serial){}
 pinMode(SS,OUTPUT);
 digitalWrite(SS,HIGH);
 delay(100);
 SPI.begin();
 SPIWrite(IO_DIR_REG,0x01);
 SPIWrite(GPIO_REG,0x01);
}

void loop() {
 // put your main code here, to run repeatedly:
 //SPIWrite(GPIO_REG,0x02);
 //delay(500);
 //SPIWrite(GPIO_REG,0x00);
 delay(500);
 Serial.println(SPIRead(GPIO_REG));
}
