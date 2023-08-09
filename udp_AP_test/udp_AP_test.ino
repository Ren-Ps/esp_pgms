#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "Ps";
const char *pass = "STS#2019";

unsigned int UDPPort=2390;
//IPAddress ServerIP(192,168,4,1);
//IPAddress ClientIP(192,168,4,2);

WiFiUDP Udp;
char packetBuffer[255];
char ReplyBuffer[]= "acknwldg";

void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFi.softAP("WiFi");
  Udp.begin(UDPPort);
  Serial.println();
  Serial.print("Started AP. Local IP: " + WiFi.localIP().toString());
}

void loop()
{
  int packetSize=Udp.parsePacket();
  if(packetSize){
    Serial.println("Recived Packet of size: ");
    Serial.print(packetSize);
    Serial.println("From: ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(" , port ");
    Serial.println(Udp.remotePort());

    int len=Udp.read(packetBuffer,255);
    if (len>0){
      packetBuffer[len]=0;
    }
  Udp.beginPacket(Udp.remoteIP(),Udp.remotePort());
  Udp.write(ReplyBuffer);
  Udp.endPacket();
  }
}

