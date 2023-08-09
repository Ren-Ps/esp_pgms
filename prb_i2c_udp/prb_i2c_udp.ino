#include "PCF8574.h"
#include "ESP8266WiFi.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Z70";
const char* password = "0a1i2s3h4";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
//char  replyPacket[] = "Hi there! Got the message :-)";  
unsigned int Dec=0;

PCF8574 PRT1(0x38, D2, D1); // Set i2c address
PCF8574 PRT2(0x39, D2, D1); // Set i2c address

void setup()
{
	Serial.begin(115200); 
	delay(1);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
//  Serial.printf("Connecting to %s ", ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println(" connected");
//
//  Udp.begin(localUdpPort);
//  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
//  delay(1);
//  
//	PRT1.pinMode(P0, INPUT);PRT1.pinMode(P1, INPUT);PRT1.pinMode(P2, INPUT);
//	PRT1.pinMode(P3, INPUT);PRT1.pinMode(P4, INPUT);PRT1.pinMode(P5, INPUT);
//  PRT1.pinMode(P6, INPUT);PRT1.pinMode(P7, INPUT);//PRT1.pinMode(P2, INPUT);
//  
//	PRT2.pinMode(P0, INPUT);PRT2.pinMode(P1, INPUT);PRT2.pinMode(P2, INPUT);
//  PRT2.pinMode(P3, INPUT);PRT2.pinMode(P4, INPUT);PRT2.pinMode(P5, INPUT);
//  PRT2.pinMode(P6, INPUT);PRT2.pinMode(P7, INPUT);//PRT2.pinMode(P2, INPUT);
//  
//	PRT1.begin();
//  PRT2.begin(); delay(1);
//	Serial.println("START");
}

void loop()
{
  Serial.println("Scan starts \t * reffers to Secured Connection");
  int n = WiFi.scanNetworks();//returns the nos. of N/W found
  Serial.println("Scan Done!");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      Serial.print(i+1);
      Serial.print (": "+ WiFi.SSID(i)+ "  (" );
      Serial.print(100+(WiFi.RSSI(i)));
      Serial.print("%)");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");  delay(5000);
  
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    while(1){
      
		PCF8574::DigitalInput H_byt = PRT1.digitalReadAll();
    PCF8574::DigitalInput L_byt = PRT2.digitalReadAll();
   
		/*Serial.print("higherbyt: ");
		Serial.print(H_byt.p0);  Serial.print(H_byt.p1); Serial.print(H_byt.p2);  Serial.print(", "); 
		Serial.print(H_byt.p3);  Serial.print(H_byt.p4); Serial.print(H_byt.p5);  Serial.print(", ");
    Serial.print(H_byt.p6);  Serial.print(H_byt.p7); Serial.print(" - ");

    Serial.print("lowerbyt: ");
    Serial.print(L_byt.p0);  Serial.print(L_byt.p1); Serial.print(L_byt.p2);  Serial.print(", "); 
    Serial.print(L_byt.p3);  Serial.print(L_byt.p4); Serial.print(L_byt.p5);  Serial.print(", ");
    Serial.print(L_byt.p6);  Serial.print(L_byt.p7); Serial.print(" - ");*/

    Dec = L_byt.p5*(pow(2,0))+L_byt.p4*(pow(2,1))+L_byt.p3*(pow(2,2))+L_byt.p2*(pow(2,3))
        +L_byt.p1*(pow(2,4))+L_byt.p0*(pow(2,5))+H_byt.p7*(pow(2,6))+H_byt.p6*(pow(2,7))
        +H_byt.p5*(pow(2,8))+H_byt.p4*(pow(2,9))+H_byt.p3*(pow(2,10))+H_byt.p2*(pow(2,11));

    if (H_byt.p0 == 0) { Serial.print("\n -"); }
    else { Serial.print("\n +");}
    Serial.print(Dec);  delay(3);
    
    /*if (H_byt.p1 == 0) { Serial.println(" ~"); }
    else { Serial.println(" ^");}*/
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(Dec);
    Udp.endPacket();  delay(5);
    }
  }
		//Serial.println("Outofloop");
}


