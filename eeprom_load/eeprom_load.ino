#include <EEPROM.h>

struct master {
  byte PlugNo; 
  unsigned int selectCont, toBeMap;
  unsigned long AbSolVal, UpSLimit, LoSLimit; };

struct master mst_array[30];
int add[30];

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
for( int i=0; i <=27; i++){
    add[i] += (i* sizeof(master));
    EEPROM.put(add[i], i);
} Serial.println("eeprom done");
}

void loop() {
  // put your main code here, to run repeatedly:

}
