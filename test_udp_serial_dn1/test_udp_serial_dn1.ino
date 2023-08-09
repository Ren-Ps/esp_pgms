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
#include <EEPROM.h>

String ssid = "SourceTechSys";
String password = "STS#2019#";
unsigned int port = 503;      // local port to listen on
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,

WiFiUDP Udp;

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(9600); Serial.setTimeout(100);
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300); digitalWrite(2, LOW);
    Serial.print('.');  delay(300);
    digitalWrite(2, HIGH);
  }
  Serial.print("\nConnected to: ");
  Serial.print(WiFi.SSID());
  Serial.print("\nIP address: ");
  Serial.print(WiFi.localIP());
  Serial.printf("\nUDP server on port: % d \n", port);
  Udp.begin(port);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED)
    digitalWrite(2, LOW);
  else digitalWrite(2, HIGH);
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0; //Serial.println("Contents: ");
    Serial.println(packetBuffer);
  }
  if (Serial.available()) {
    char dataSerl = Serial.read();
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.print(dataSerl);
    Udp.endPacket();
  }
}
