#include <EEPROM.h>
#include <Arduino.h>

struct mst {
  byte PlugNo; 
  unsigned int selectCont, toBeMap;
  unsigned long AbSolVal, UpSLimit, LoSLimit; };

struct mst customVar[2];

void setup() {
  pinMode(13, INPUT);
  unsigned int add= 0;
  Serial.begin(9600);
  while (!Serial) {
  }
  Serial.println("Written data type!");
  customVar[0] = {
    12, 1200, 400, 67000, 67100, 66900
  };
  customVar[1] = {
    14, 700, 300, 101000, 101300, 100700};
  
  EEPROM.put(add, customVar[0]); add+= sizeof(mst);
  EEPROM.put(add, customVar[1]);
  Serial.print("Written custom data type! dn!");
  delay(1000);

int addr=0;
  Serial.println("Read from EEPROM: "); 
  EEPROM.get(addr, customVar[0]);
  Serial.println("Read custom object cstm0 from EEPROM: ");
  Serial.println(customVar[0].PlugNo);  Serial.println(customVar[0].selectCont);
  Serial.println(customVar[0].toBeMap);  Serial.println(customVar[0].AbSolVal);
  Serial.println(customVar[0].UpSLimit);  Serial.println(customVar[0].LoSLimit);
  addr  += sizeof(mst);
  EEPROM.get(addr, customVar[1]);
  Serial.println("Read custom object cstm1 from EEPROM: ");
  Serial.println(customVar[1].PlugNo);  Serial.println(customVar[1].selectCont);
  Serial.println(customVar[1].toBeMap);  Serial.println(customVar[1].AbSolVal);
  Serial.println(customVar[1].UpSLimit);  Serial.println(customVar[1].LoSLimit);
}

void loop() {
  
}
