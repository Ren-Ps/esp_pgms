#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup(){
  pinMode(18, OUTPUT); pinMode(19, OUTPUT); pinMode(21, OUTPUT); pinMode(22, OUTPUT); pinMode(23, OUTPUT);
  pinMode(25, OUTPUT); pinMode(26, OUTPUT); pinMode(27, OUTPUT); pinMode(32, OUTPUT); pinMode(2, OUTPUT);
  pinMode(16, OUTPUT); pinMode(17, OUTPUT);delay(10);

   pinMode(4, OUTPUT); 
  SerialBT.begin("STS_Ps"); delay(10);
}

void loop(){
  String SeRecv = SerialBT.readStringUntil('\n');
    if (SeRecv.indexOf('P') >= 1) {
      int indx1 = SeRecv.indexOf(":"); 
      int indx2 = SeRecv.indexOf("-");
      String sPin = SeRecv.substring(indx1 + 1, indx2);
      int pin = sPin.toInt();
      int logic = SeRecv.substring(indx2 + 1).toInt(); 
      switch (pin) {
        case 1: digitalWrite(22, logic); SerialBT.println("\nUP");break; 
        case 2: digitalWrite(21, logic); SerialBT.println("\nSTOP");break; 
        case 3: digitalWrite(19, logic); SerialBT.println("\nCLOSE");break; 
        case 4: digitalWrite(18, logic); SerialBT.println("\nOPEN");break; 
        case 5: digitalWrite(27, logic); SerialBT.println("\nJET");break; 
        case 6: digitalWrite(26, logic); SerialBT.println("\nRIGHT");break; 
        case 7: digitalWrite(25, logic); SerialBT.println("\nLEFT");break; 
        case 8: digitalWrite(23, logic); SerialBT.println("\nDOWN");break; 
        case 9: digitalWrite(17, logic); SerialBT.println("\nSPR-1");break; 
        case 10: digitalWrite(16, logic); SerialBT.println("\nSPR-2");break; 
        case 11: digitalWrite(2, logic); SerialBT.println("\nSYS-STS");break; 
        case 12: digitalWrite(32, logic); SerialBT.println("\nFOG");break; 
        case 13: digitalWrite(4, logic); SerialBT.println("\nIP:OPN");break; 
        default: SeRecv = ""; break;
      }  SeRecv = "";
    }
    else {
       SeRecv = "";
    }
}
