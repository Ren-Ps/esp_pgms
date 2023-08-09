#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>

String ssid = "SourceTechSys";
String password = "STS#2019#";

int currentPacketSize;
char* packet;
char temp[1024];

boolean begun, ended;

WiFiUDP udpListen;
WiFiUDP udpSend;
Ticker ticker;

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.begin(9600); Serial.setTimeout(100);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  udpListen.begin(7777);
  ticker.attach(4.0, [](){
    begun = udpSend.beginPacket(IPAddress(192, 168, 0, 108), 49809); // My computer listening
    udpSend.write("ALIVE\n", 6);
    ended = udpSend.endPacket();
    snprintf(temp, sizeof temp, "ALIVE packet : started == %s : sent == %s", begun?"true":"false", ended?"true":"false");
    Serial.println(temp);
  });
}

void loop() {
  currentPacketSize = udpListen.parsePacket();
  if (currentPacketSize){
    packet = (char*) malloc(currentPacketSize * sizeof(char));
    udpListen.read(packet, currentPacketSize);

    Serial.print("Received packet: ");
    Serial.write(packet, currentPacketSize);
    Serial.print("\n");

    // Echo
    begun = udpListen.beginPacket(udpListen.remoteIP(), 49809); // My computer listening
    udpListen.write(packet, currentPacketSize);
    ended = udpListen.endPacket();

    Serial.print("Echo sent to: ");
    Serial.print(udpListen.remoteIP());
    snprintf(temp, sizeof temp, " : started == %s : sent == %s\n", begun?"true":"false", ended?"true":"false");
    Serial.print(temp);
  }
}
