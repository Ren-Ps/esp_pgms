#include "PCF8574.h"
#include "ESP8266WiFi.h"
#include <WiFiUdp.h>

char ssid[32] = "";
char password[32] = "";

WiFiUDP Udp;

unsigned short int localUdpPort = 4210; // local port to listen on

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
  } Serial.print("      Wait...");
  delay(2300); //wait for some time
  }
  Serial.print("\n \t \t      ...Scan Done!\nNOTE: 1. * Stands for the Network is Password Secured.");
  Serial.print("\n      2. In brackets Network Strength is shown.");
  Serial.print("\n      3. Network Strengths are in (100+ actual dBm). ");
  Serial.print("\n      4. For Open type Network, Enter ';' only when password is asked.");
  Serial.print("\n      5. Always Enter ';' at the end of respective input on Serial is asked.");
  Serial.print("\n\nEnter SSID: ");
  Serial.flush();
  String msg="";
  while(1){ 
    msg = Serial.readStringUntil(';');  
    if(msg!="") 
    break; //if new line char recived break the loop
  }
  Serial.println(msg);  delay(10); 
  msg.toCharArray(ssid,32); delay(10);  msg="";
  Serial.print("Enter Password: "); 
  Serial.flush();
  while(1){
    msg = Serial.readStringUntil(';');  //may replace with \n or \r
    if(msg!="") 
    break;
  }
  Serial.println(msg);  delay(10); 
  msg.toCharArray(password,32); delay(100);  msg="";
  Serial.flush();
  Serial.printf("\nConnecting to %s ", ssid);
  WiFi.disconnect(); delay(700);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print(" Connected!");
  Udp.begin(localUdpPort);
  Serial.printf("\n Now listening at IP %s, UDP port %d!\n", WiFi.localIP().toString().c_str(), localUdpPort);
  delay(1);

  PRT1.pinMode(P0, INPUT);PRT1.pinMode(P1, INPUT);PRT1.pinMode(P2, INPUT);
  PRT1.pinMode(P3, INPUT);PRT1.pinMode(P4, INPUT);PRT1.pinMode(P5, INPUT);
  PRT1.pinMode(P6, INPUT);PRT1.pinMode(P7, INPUT);//PRT1.pinMode(P2, INPUT);
  
  PRT2.pinMode(P0, INPUT);PRT2.pinMode(P1, INPUT);PRT2.pinMode(P2, INPUT);
  PRT2.pinMode(P3, INPUT);PRT2.pinMode(P4, INPUT);PRT2.pinMode(P5, INPUT);
  PRT2.pinMode(P6, INPUT);PRT2.pinMode(P7, INPUT);//PRT2.pinMode(P2, INPUT);
  
  PRT1.begin();
  PRT2.begin(); delay(1);
  pinMode(D0, INPUT);
  Serial.println("Waiting for incoming packet once..");
}

void loop() {
  char inPacket[255];  // buffer for incoming packets
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("\n Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(inPacket, 255);
    if (len > 0)
    {
      inPacket[len] = 0;
    }
    Serial.printf("\n UDP packet contents: %s\n", inPacket); delay(10);
    while(1){
    PCF8574::DigitalInput H_byt = PRT1.digitalReadAll();
    PCF8574::DigitalInput L_byt = PRT2.digitalReadAll();
    unsigned short int Count = L_byt.p5*(pow(2,0))+L_byt.p4*(pow(2,1))+L_byt.p3*(pow(2,2))+L_byt.p2*(pow(2,3))
        +L_byt.p1*(pow(2,4))+L_byt.p0*(pow(2,5))+H_byt.p7*(pow(2,6))+H_byt.p6*(pow(2,7))
        +H_byt.p5*(pow(2,8))+H_byt.p4*(pow(2,9))+H_byt.p3*(pow(2,10))+H_byt.p2*(pow(2,11));

    if (H_byt.p0 == 0) { Count = 5000 - Count; }
    else { Count = 5000 + Count;}
    Serial.print(digitalRead(D0)); Serial.println(Count);  delay(3);
    
    /*if (H_byt.p1 == 0) { Serial.println(" ~"); }
    else { Serial.println(" ^");}*/
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    char txBuff[12];
    itoa(Count, txBuff, 10);
    Udp.write(txBuff);
    Udp.endPacket();  delay(5);
    }
  }
}


