/*
  This sketch receives UDP message strings, prints them to the serial port
  and sends an "acknowledge" string back to the sender

  A Processing sketch is included at the end of file that can be used to send
  and received messages for testing with a computer.

  created 13 June, 2019
  by Pritesh Shirsath

  This code is in the private domain.
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>

String ssid = "SourceTechSys";
String password = "STS#2019#";
unsigned int port = 503;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,

SoftwareSerial SwSl(D2, D1, false, 128);
WiFiUDP Udp;

void setup() {
  pinMode(2, OUTPUT);
  SwSl.begin(9600); SwSl.setTimeout(100);
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300); digitalWrite(2, LOW);
    SwSl.print('.');  delay(300);
    digitalWrite(2, HIGH);
  }
  SwSl.print("\nConnected to: ");
  SwSl.print(WiFi.SSID());
  SwSl.print("\nIP address: ");
  SwSl.print(WiFi.localIP());
  SwSl.printf("\nUDP server on port: % d \n", port);
  Udp.begin(port);
}

String StrgMemSsid = ""; String StrgMemPwd = "";

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0; //Serial.println("Contents: ");
    SwSl.println(packetBuffer);
  }
  if (SwSl.available()) {
    char dataSerl = SwSl.read();  //SwSl.print(dataSerl);
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.print(dataSerl);
    Udp.endPacket();
  }
}
