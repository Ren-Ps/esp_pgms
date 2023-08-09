/*
  This pgm is used for udp to serial bi-rectional communication.
  The communication begins with any type of input from the remort port,
  so it gets the port to which the data need to be send on.
  
  created 13 June, 2019
  by Pritesh Shirsath
  This code is in the private domain.
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>

String ssid = "JioFi5_51DBFE";
String password = "07eud1c34v";
unsigned int port = 503;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; 
//buffer to hold incoming packet,

WiFiUDP Udp;
SoftwareSerial swSer(D2, D1, false, 128);

void setup() {
  pinMode(2, OUTPUT);
  //Serial.begin(9600); Serial.setTimeout(100); delay(10);
  swSer.begin(9600); swSer.setTimeout(100); delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300); digitalWrite(2, LOW);
    //Serial.print('.');
    swSer.print('.');  delay(300);
    digitalWrite(2, HIGH);
  }

  //Serial.print("\nConnected to: ");
  swSer.print("\nConnected to: ");
  //Serial.print(WiFi.SSID());
  swSer.print(WiFi.SSID());
  //Serial.print("\nIP address: ");
  swSer.print("\nIP address: ");
  //Serial.print(WiFi.localIP());
  swSer.print(WiFi.localIP());
  //Serial.printf("\nUDP server on port: % d \n", port);
  swSer.printf("\nUDP server on port: % d \n", port);

  Udp.begin(port);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0; //Serial.println("Contents: ");
    //Serial.println(packetBuffer);
    swSer.println(packetBuffer);
  }
  if (swSer.available()) {
    byte dataSerl = swSer.read();
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(dataSerl);
    Udp.endPacket();
  }
}
