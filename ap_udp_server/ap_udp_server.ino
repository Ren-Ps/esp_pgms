
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "circuits4you";
const char *pass = "password";
unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192, 168, 4, 1);
IPAddress ClientIP(192, 168, 4, 2);

WiFiUDP udp;
char packetBuffer[9];
char data[1]; int lastst = 0; //char temp='z';
void setup()
{
  pinMode(D5, INPUT_PULLUP); pinMode(D6, INPUT_PULLUP); pinMode(D7, INPUT_PULLUP); pinMode(D8, INPUT);
  pinMode(D1, OUTPUT); pinMode(D2, OUTPUT); pinMode(D3, OUTPUT); pinMode(D4, OUTPUT); pinMode(16, OUTPUT);
  Serial.begin(9600);
  Serial.println();
  WiFi.softAP(ssid, pass);  
  Serial.print("\nStartingUDP.");
  udp.begin(localPort);
  Serial.print("Localport:");
  Serial.println(udp.localPort());
}
int dn=0;
void loop()
{
  int cb = udp.parsePacket();
  if (!cb)
  {
    if (digitalRead(D5) == 0 && lastst != 5) {
      data[0] = 'A';
      digitalWrite(D1, HIGH);
      Serial.println("D5");
      lastst = 5;
    }
    if (digitalRead(D6) == 0 && lastst != 6) {
      data[0] = 'B';
      digitalWrite(D2, HIGH);
      Serial.println("D6");
      lastst = 6;
    }
    if (digitalRead(D7) == 0 && lastst != 7) {
      data[0] = 'C';
      digitalWrite(D3, HIGH);
      Serial.println("D7");
      lastst = 7;
    }
    if (digitalRead(D8) == 1 && lastst != 8) {
      data[0] = 'D';
      digitalWrite(D4, HIGH);
      Serial.println("D8");
      lastst = 8;
    }
   //else  data[0] = ' ';

    if (lastst != 0 && lastst != dn) {
      udp.beginPacket(ClientIP, 2000);//Send UDP requests are to port 2000
      Serial.println(data[0]);
      udp.write(data, 1); //Send one byte to ESP8266
      udp.endPacket();
      dn=lastst; lastst=0;
    } delay(20);
  }
  else {
    // We've received a UDP packet, send it to serial
    udp.read(packetBuffer, 1); // read the packet into the buffer, we are reading only one byte
    Serial.print(packetBuffer);
    delay(20);
  }
}
