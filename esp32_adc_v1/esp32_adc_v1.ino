/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "BluetoothSerial.h"
//#include "WiFi.h"
//#include "AsyncUDP.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#define ADC_SAMPLE_COUNT 15
#define ADC1_TEST_CHANNEL (ADC1_CHANNEL_6)      // GPIO 34
uint32_t adc_raw_sample[ADC_SAMPLE_COUNT];
//char ssid[32] = "";
//char password[32] = "";

//AsyncUDP udp;

void setup() //app_main(void)
{
    Serial.begin(115200);
    SerialBT.begin("STS_Ps"); //Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");
    
    /*WiFi.mode(WIFI_STA);  //beging as station mode
  delay(100);
  Serial.print("\nScaning for Wireless Networks...");

  for( int ns=0; ns<3; ns++){
  int ScnNw = WiFi.scanNetworks();  // this will return the number of networks found
  if (ScnNw == 0)
    Serial.print("\nNo Networks Found!");
  else
  {
    Serial.printf("\n   %d Networks Found!\n", ScnNw);
    for (int i = 0; i < ScnNw; ++i)   // Print SSID and RSSI for each network found
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(100+WiFi.RSSI(i));
      Serial.print("dBm)");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  } Serial.print("      Wait...");
  delay(2300); //wait for some time
  }
  Serial.print("\n \t \t      ...Scan Done!\nNOTE: 1. * Stands for the Network is Password Secured.");
  Serial.print("\n      2. In brackets Network Strength is shown.");
  Serial.print("\n      3. Network Strengths are in (100+ actual dBm). ");
  Serial.print("\n      4. For Open type Network, Enter ';' only when password is asked.");
  Serial.print("\n      5. Always Enter ';' at the end of respective input on Serial is asked.");
  Serial.print("\n\nEnter SSID: ");
  Serial.flush();
  String msg="";
  while(1){ 
    msg = Serial.readStringUntil(';');  
    if(msg!="") 
    break; //if new line char recived break the loop
  }
  Serial.println(msg);  delay(10); 
  msg.toCharArray(ssid,32); delay(10);  msg="";
  Serial.print("Enter Password: "); 
  Serial.flush();
  while(1){
    msg = Serial.readStringUntil(';');  //may replace with \n or \r
    if(msg!="") 
    break;
  }
  Serial.println(msg);  delay(10); 
  msg.toCharArray(password,32); delay(100);  msg="";
  Serial.flush();
  Serial.printf("\nConnecting to %s ", ssid);
  WiFi.disconnect(); delay(700);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print(" Connected!");*/
  
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_TEST_CHANNEL, ADC_ATTEN_0db);
        
       /* if(udp.listenMulticast(IPAddress(239,1,2,3), 1234)) {
          int temp = adc_raw_avg;
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            //reply to the client
            //packet.printf("Got %u bytes of data", packet.length());
            packet.printf(" %d ", temp);
        });
        //Send multicast
        udp.print("Hello!");
    }*/
}

void loop() {
        uint32_t adc_raw_avg = 0;
        int32_t adc_raw_dev;
        int32_t adc_raw_dev_above;
        int32_t adc_raw_dev_below;
        for (int i=0; i<ADC_SAMPLE_COUNT; i++) {
            adc_raw_sample[i] = adc1_get_raw(ADC1_TEST_CHANNEL);
            adc_raw_avg += adc_raw_sample[i];
            //printf("%d ", adc_raw_sample[i]);
        }
        adc_raw_avg /= ADC_SAMPLE_COUNT;
        adc_raw_dev       = adc_raw_sample[0] - adc_raw_avg;
        adc_raw_dev_above = adc_raw_dev;
        adc_raw_dev_below = adc_raw_dev;
        for (int i=1; i<ADC_SAMPLE_COUNT; i++) {
            adc_raw_dev = adc_raw_sample[i] - adc_raw_avg;
            if (adc_raw_dev > adc_raw_dev_above) {
                adc_raw_dev_above = adc_raw_dev;
            } else if (adc_raw_dev < adc_raw_dev_below) {
                adc_raw_dev_below = adc_raw_dev;
            }
        }
        if (- adc_raw_dev_below > adc_raw_dev_above) {
            adc_raw_dev = adc_raw_dev_below;
        } else {
            adc_raw_dev = adc_raw_dev_above;
        }
        float adc_dev = 100.0 * adc_raw_dev / adc_raw_avg;

        //printf("Avg: %d Dev: %4.1f%%\n", adc_raw_avg, adc_dev); //vTaskDelay(pdMS_TO_TICKS(200));
        printf(" %d \n",adc_raw_avg);
        delayMicroseconds(200);
        if (SerialBT.available()) {
          char str[16];
          itoa(adc_raw_avg, str, 14);
          //for (int i=0; i<10; i++)
          //  SerialBT.write(str[i]);
          SerialBT.println(adc_raw_avg);
          }
  
  }
