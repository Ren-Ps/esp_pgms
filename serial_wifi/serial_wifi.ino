#include "ESP8266WiFi.h"

int port = 503;
String ssid = "SourceTechSys";
String password = "STS#2019#";
WiFiServer wifiServer(503);

void setup() {
  Serial.begin(9600); Serial.setTimeout(100); delay(10);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100); Serial.print(".");
  }
  Serial.print("\nConnected to: ");
  Serial.print(ssid);
  Serial.print("\nIP Address:");
  Serial.print(WiFi.localIP()); delay(10);
  wifiServer.begin(); delay(10);
  Serial.print("\nPort: ");
  Serial.println(port);
}

void loop() {
  WiFiClient client = wifiServer.available();
 // if (client) {
    if (client.connected()) {  //while (client.available()) { // Serial.write(client.read());
      Serial.println("Client Connected");
      while (Serial.available()) {
        String dataSerial = Serial.readStringUntil('\r');
        client.print(dataSerial);
        dataSerial = "";
      }
     // client.close();
    }
    //client.stop();  Serial.println("\tClient disconnected");
 // }
}
