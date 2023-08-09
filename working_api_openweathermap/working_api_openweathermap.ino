#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> // http web access library
#include <ArduinoJson.h> // JSON decoding library

// set Wi-Fi SSID and password
const char *ssid = "Aish_LOL";
const char *password = "PeCash_LOL";

// set location and API key
String Location = "London,uk";
String API_Key = "2496c96fd2cea53a7aaac69697aaf597";

void setup(void)
{
  pinMode(2, OUTPUT); pinMode(16, OUTPUT); //define pintype OUTPUT/INPUT, these inbuilt LED pins used for indiaction purpose only
  digitalWrite(2, HIGH); digitalWrite(16, HIGH); delay(10);
  Serial.begin(9600); //begin Serial Communication with 9600 baud rate
  delay(100);
  WiFi.begin(ssid, password); //begin wifi connection ussing ssid and password
  Serial.print("\nConnecting.");
  while ( WiFi.status() != WL_CONNECTED ) //attempting connection
  {
    digitalWrite(2, LOW); delay(200);
    digitalWrite(2, HIGH); Serial.print("."); delay(200);
  }
  Serial.println("Connected!");
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
  Serial.print("Signal Strength (RSSI): "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
  digitalWrite(2, LOW); delay(100);
}

void loop()
{
  digitalWrite(16,LOW); //ping/quarry status indication
  if (WiFi.status() == WL_CONNECTED) //Check WiFi connection status
  {
    HTTPClient http; //Declare an object of class HTTPClient
    http.begin("http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&APPID=" + API_Key); // specify request destination
    int httpCode = http.GET(); // send the request
    if (httpCode > 0) // check the returning code
    {
      String payload = http.getString(); //Get the request response payload
      DynamicJsonBuffer jsonBuffer(512);
      JsonObject& root = jsonBuffer.parseObject(payload);// Parse JSON object
      if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        return;
      }

      float temp = (float)(root["main"]["temp"]) - 273.15;          // get temperature in °C
      int humidity = root["main"]["humidity"];                      // get humidity in %
      float pressure = (float)(root["main"]["pressure"]) / 1000;    // get pressure in bar
      float wind_speed = root["wind"]["speed"];                     // get wind speed in m/s
      int wind_degree = root["wind"]["deg"];                        // get wind degree in °

      // print all phrased data
      Serial.printf("Temperature = %.2f°C\r\n", temp);
      Serial.printf("Humidity = %d %%\r\n", humidity);
      Serial.printf("Pressure = %.3f bar\r\n", pressure);
      Serial.printf("Wind speed = %.1f m/s\r\n", wind_speed);
      Serial.printf("Wind degree = %d°\r\n\r\n", wind_degree);
    }
    http.end(); //Close connection
  }   
  digitalWrite(16,HIGH);
  delay(60000); // wait for 1 minute approx
}
