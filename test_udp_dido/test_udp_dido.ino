#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//#ifndef APSSID
//#define APSSID "Ps_Demo"
//#define APPSK  "Ps_Demo"
//#endif

const char *ssid = "Ps_Demo";
const char *password = "Ps_Demo";
unsigned int localPort = 351;

/* Go to http://192.168.4.1  */
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];
char  ReplyBuffer[] = "Hello!\r\n";
WiFiUDP Udp;

void setup() {
  pinMode(2, OUTPUT);  pinMode(16, OUTPUT); delay(10);
  Serial.begin(9600); delay(10);
  Serial.print("\nConfiguring access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("\nAP IP address: ");
  Serial.print(myIP);
  Serial.print("\nAP in UDP mode started! ");
  Udp.begin(localPort);
  Serial.print("On Port 351.");
  digitalWrite(2, LOW);
  }

  void loop() {
    digitalWrite(16, HIGH);
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
      packetBuffer[n] = 0; Serial.println("Contents: ");
      Serial.println(packetBuffer);
    } digitalWrite(16, LOW); delay(100);
  }
