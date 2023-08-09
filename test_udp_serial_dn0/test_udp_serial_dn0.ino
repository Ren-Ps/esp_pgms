/*
  UDPSendReceive.pde:
  This sketch receives UDP message strings, prints them to the serial port
  and sends an "acknowledge" string back to the sender

  A Processing sketch is included at the end of file that can be used to send
  and received messages for testing with a computer.

  created 21 Aug 2010
  by Michael Margolis

  This code is in the public domain.

  adapted from Ethernet library examples
*/


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

String ssid = "SourceTechSys";
String password = "STS#2019#";
unsigned int port = 503;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,

WiFiUDP Udp;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');  delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", port);
  Udp.begin(port);
}
String ssidMem=""; String passwordMem="";
void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    
    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    while (1) {
      while (Serial.available()) {
        String dataSerl = Serial.readStringUntil('\n');
        if(dataSerl.indexOf("NF")>0){
          Udp.stopAll();
          Serial.print("Enter New SSID: ");
          while (Serial.available()){
          ssidMem = Serial.readStringUntil(';'); }
          Serial.print("Enter Passward: ");
          while (Serial.available()){
          passwordMem = Serial.readStringUntil(';'); }
          Serial.print(" New SSID: "); Serial.print(ssidMem);
        }
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.print(dataSerl);
        Udp.endPacket();
        
      }
    }
  }
}
