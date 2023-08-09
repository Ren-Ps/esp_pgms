#include "PCF8574.h"
#include "ESP8266WiFi.h"
#include <WiFiUdp.h>

char ssid[32] = "";
char password[32] = "";

WiFiUDP Udp;

unsigned short int localUdpPort = 4210, Count=5000;  // local port to listen on
char inPacket[255];  // buffer for incoming packets
char  rplyPacket[] = "Hi there! Got the message :-)";  
String msg="";

PCF8574 PRT1(0x38, D2, D1); // Set i2c address & pin
PCF8574 PRT2(0x39, D2, D1); // Set i2c address

void setup() {
  Serial.begin(115200); delay(10);
  Serial.println();
  
  WiFi.disconnect();  //disconnect all
  WiFi.mode(WIFI_STA);  //beging as station mode
  delay(100);
  Serial.print("\nScaning for Wireless Networks...");

  for( int ns=0; ns<3; ns++){
  int ScnNw = WiFi.scanNetworks();  // this will return the number of networks found
  if (ScnNw == 0)
    Serial.print("\nNo Networks Found!");
  else
  {
    Serial.printf("\n   %d Networks Found!\n", ScnNw);
    for (int i = 0; i < ScnNw; ++i)   // Print SSID and RSSI for each network found
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(100+WiFi.RSSI(i));
      Serial.print("dBm)");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  delay(2300); //wait for some time
  }
  Serial.print("\n\tScan Done.\nNOTE: 1. * Stands for the Network is Password Secured.");
  Serial.print("\n      2. In brackets Network Strength is shown.");
  Serial.print("\n      3. Network Strengths are in (100+ actual dBm). ");
  Serial.print("\n      4. For Open type Network, Enter ';' only when password is asked.");
  Serial.print("\n      5. Always Enter ';' at the end of respective input on Serial is asked.");
  Serial.print("\n\nEnter SSID: ");
  Serial.flush();
  while(1){
    msg = Serial.readStringUntil(';');  
    if(msg!="") 
    break; //if new line char recived break the loop
  }
  Serial.println(msg);  delay(10); 
  msg.toCharArray(ssid,32); delay(10);  msg="";
 // for (int q=0; q<33; q++) { Serial.print("\narray ssid: "); Serial.print(" "+ssid[q]);}
  Serial.print("\nEnter Password: "); 
  Serial.flush();
  while(1){
    msg = Serial.readStringUntil(';');  //may replace with \n or \r
    if(msg!="") 
    break;
  }
  Serial.println(msg);  delay(10); 
  msg.toCharArray(password,32); delay(100);  msg="";
 // for (int q=0; q<33; q++) { Serial.print("array pass: "); Serial.print(" "+password[q]);}
  Serial.flush();
  Serial.printf("Connecting to %s ", ssid);
  WiFi.disconnect(); delay(700);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print(" Connected!");
  Udp.begin(localUdpPort);
  Serial.printf("\nNow listening at IP %s, UDP port %d!\n", WiFi.localIP().toString().c_str(), localUdpPort);
  delay(1);
}

void loop() {
  
}


