#include <Wire.h>
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp0;
Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;
Adafruit_MCP23017 mcp3;

byte bt[16]; double dec=0;
  
void setup() {  
  
  mcp0.begin(0); // use default address 0
  mcp1.begin(1);
  mcp2.begin(2);
  mcp3.begin(3);
  
  Serial.begin(9600);

 for (int i=0; i<16; i++){
  mcp0.pinMode(i, INPUT);  mcp0.pullUp(i, HIGH);
  mcp1.pinMode(i, INPUT);  mcp1.pullUp(i, HIGH);
  mcp2.pinMode(i, INPUT);  mcp2.pullUp(i, HIGH);
  mcp3.pinMode(i, INPUT);  mcp3.pullUp(i, HIGH);
 } 
}

void loop() {
  Serial.print("Dev: 1 - Binary:");
  for (int i=0; i<16; i++){
    bt[i]= mcp0.digitalRead(i);
    Serial.print(bt[i]);    } 
    
  Serial.print("  Decimal:");
  dec= bt[0]*(pow(2,0))+bt[1]*(pow(2,1))+bt[2]*(pow(2,2))+bt[3]*(pow(2,3))+bt[4]*(pow(2,4))+bt[5]*(pow(2,5))+
       bt[6]*(pow(2,6))+bt[7]*(pow(2,7))+bt[8]*(pow(2,8))+bt[9]*(pow(2,9))+bt[10]*(pow(2,10))+bt[11]*(pow(2,11))
        +bt[12]*(pow(2,12))+bt[13]*(pow(2,13))+bt[14]*(pow(2,14))+bt[15]*(pow(2,15));
  Serial.println(dec);  delay(1);

  Serial.print("Dev: 2 - Binary:");
  for (int i=0; i<16; i++){
    bt[i]= mcp1.digitalRead(i);
    Serial.print(bt[i]);    }
  
  Serial.print("  Decimal:");
  dec= bt[0]*(pow(2,0))+bt[1]*(pow(2,1))+bt[2]*(pow(2,2))+bt[3]*(pow(2,3))+bt[4]*(pow(2,4))+bt[5]*(pow(2,5))+
       bt[6]*(pow(2,6))+bt[7]*(pow(2,7))+bt[8]*(pow(2,8))+bt[9]*(pow(2,9))+bt[10]*(pow(2,10))+bt[11]*(pow(2,11))
        +bt[12]*(pow(2,12))+bt[13]*(pow(2,13))+bt[14]*(pow(2,14))+bt[15]*(pow(2,15));
  Serial.println(dec);  delay(1);
  
  Serial.print("Dev: 3 - Binary:");
  for (int i=0; i<16; i++){
    bt[i]= mcp2.digitalRead(i);
    Serial.print(bt[i]);  }
    
    Serial.print("  Decimal:");
  dec= bt[0]*(pow(2,0))+bt[1]*(pow(2,1))+bt[2]*(pow(2,2))+bt[3]*(pow(2,3))+bt[4]*(pow(2,4))+bt[5]*(pow(2,5))+
       bt[6]*(pow(2,6))+bt[7]*(pow(2,7))+bt[8]*(pow(2,8))+bt[9]*(pow(2,9))+bt[10]*(pow(2,10))+bt[11]*(pow(2,11))
        +bt[12]*(pow(2,12))+bt[13]*(pow(2,13))+bt[14]*(pow(2,14))+bt[15]*(pow(2,15));
  Serial.println(dec);  delay(1);

  Serial.print("Dev: 4 - Binary:");
  for (int i=0; i<16; i++){
    bt[i]= mcp3.digitalRead(i);
  Serial.print(bt[i]);  }
  
  Serial.print("  Decimal:");
  dec= bt[0]*(pow(2,0))+bt[1]*(pow(2,1))+bt[2]*(pow(2,2))+bt[3]*(pow(2,3))+bt[4]*(pow(2,4))+bt[5]*(pow(2,5))+
       bt[6]*(pow(2,6))+bt[7]*(pow(2,7))+bt[8]*(pow(2,8))+bt[9]*(pow(2,9))+bt[10]*(pow(2,10))+bt[11]*(pow(2,11))
        +bt[12]*(pow(2,12))+bt[13]*(pow(2,13))+bt[14]*(pow(2,14))+bt[15]*(pow(2,15));
  Serial.println(dec);  delay(1);
}
