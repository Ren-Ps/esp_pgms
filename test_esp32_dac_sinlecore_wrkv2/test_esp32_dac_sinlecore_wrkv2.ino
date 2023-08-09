/* 

   This Code contains Selft Decleared and developed program
   This code is in the Public Domain (or CC0 licensed, at programmer's option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
***********************************************************************************************
   @Ps, 15 Mar, 2019
    ALL COPY AND RESERVED RIGHTS HELD,  ©PS..
***********************************************************************************************
          _____   _____  
    ____ |  __ \ / ____| 
   / __ \| |__) | (___   
  / / _` |  ___/ \___ \  
 | | (_| | |     ____) | 
  \ \__,_|_|    |_____(_)
   \____/                
   
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "soc/rtc_io_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "soc/rtc.h"
#include "driver/dac.h"

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

int clk_8m_div = 0;      // RTC 8M clock divider (division is by clk_8m_div+1, i.e. 0 means 8MHz frequency)
int frequency_step = 39;  // Frequency step for CW generator //39 will give 5058Hz //38 will 4929
int scale = 1;           // 50% of the full scale
int offset;              // leave it default / 0 = no any offset
int invert = 2;          // invert MSB to get sine waveform

boolean startBT=false;
byte eventStat=0; int paramLen=0;

byte crntbtn17=0, crntbtn5=0,crntbtn18=0,crntbtn19=0;
byte lsbtn17=0,lsbtn5=0,lsbtn18=0,lsbtn19=0;
   
// Enable cosine waveform generator on a DAC channel
void dac_cosine_enable(dac_channel_t channel)
{
    // Enable tone generator common to both channels
    SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
    switch(channel) {
        case DAC_CHANNEL_1:
            // Enable / connect tone tone generator on / to this channel
            SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
            // Invert MSB, otherwise part of waveform will have inverted
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, 2, SENS_DAC_INV1_S);
            break;
        case DAC_CHANNEL_2:
            SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN2_M);
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV2, 2, SENS_DAC_INV2_S);
            break;
        default :
           Serial.printf("Channel cosine_enbl CH: %d\n", channel);
           }
}


/* Set frequency of internal CW generator common to both DAC channels
 *
 * clk_8m_div: 0b000 - 0b111
 * frequency_step: range 0x0001 - 0xFFFF  */
void dac_frequency_set(int clk_8m_div, int frequency_step)
{
    REG_SET_FIELD(RTC_CNTL_CLK_CONF_REG, RTC_CNTL_CK8M_DIV_SEL, clk_8m_div);
    SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL1_REG, SENS_SW_FSTEP, frequency_step, SENS_SW_FSTEP_S);
}

/* Scale output of a DAC channel using two bit pattern:
 * - 00: no scale
 * - 01: scale to 1/2
 * - 10: scale to 1/4
 * - 11: scale to 1/8*/
void dac_scale_set(dac_channel_t channel, int scale)
{
    switch(channel) {
        case DAC_CHANNEL_1:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_SCALE1, scale, SENS_DAC_SCALE1_S);
            break;
        case DAC_CHANNEL_2:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_SCALE2, scale, SENS_DAC_SCALE2_S);
            break;
        default :
           Serial.printf("Channel scale_set CH:%d\n", channel);
           }
}

/* Offset output of a DAC channel
 * Range 0x00 - 0xFF */
void dac_offset_set(dac_channel_t channel, int offset)
{
    switch(channel) {
        case DAC_CHANNEL_1:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_DC1, offset, SENS_DAC_DC1_S);
            break;
        case DAC_CHANNEL_2:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_DC2, offset, SENS_DAC_DC2_S);
            break;
        default :
           Serial.printf("Channel offset_set CH:%d\n", channel);
           }
}


/* Invert output pattern of a DAC channel
 * - 00: does not invert any bits,
 * - 01: inverts all bits,
 * - 10: inverts MSB,
 * - 11: inverts all bits except for MSB*/
void dac_invert_set(dac_channel_t channel, int invert)
{
    switch(channel) {
        case DAC_CHANNEL_1:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, invert, SENS_DAC_INV1_S);
            break;
        case DAC_CHANNEL_2:
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV2, invert, SENS_DAC_INV2_S);
            break;
        default :
           Serial.printf("Channel invert_set CH:%d\n", channel); 
           }
}

/* If bluetooth device disconects look for the event and restart esp*/
void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
 eventStat=event; paramLen=param->data_ind.len;
 //Serial.print("\nEvent: ");Serial.print(event);
 //Serial.print("\nPram->int_length: ");Serial.print(param->data_ind.len);
 }

void setup() {
  pinMode(15, OUTPUT); pinMode(2, OUTPUT); pinMode(0, OUTPUT); pinMode(4, OUTPUT);
  pinMode(16, OUTPUT); pinMode(17, INPUT); pinMode(5, INPUT); pinMode(18, INPUT);
  pinMode(19, INPUT);
  Serial.begin(9600); Serial.setTimeout(1000); delay(10);
  
  //Serial.println("SDK:");
  //Serial.println(ESP.getSdkVersion());
  Serial.println("IDF:");
  Serial.println(esp_get_idf_version());
  delay(10);
  SerialBT.register_callback(callback);
  SerialBT.begin("STS_Ps"); //Bluetooth device name
  delay(10);
  Serial.println("©PS.. March, 19. Product under Development Terms with Source Tech System.");
  
/*Generate a sine waveform on both DAC channels:
 * DAC_CHANNEL_1 - GPIO25
 * DAC_CHANNEL_2 - GPIO26  */
    dac_cosine_enable(DAC_CHANNEL_1);
    dac_output_enable(DAC_CHANNEL_1);
    delay(10);
 }

void loop(){ 
  dac_frequency_set(clk_8m_div, frequency_step);//F is common for both channels
  dac_scale_set(DAC_CHANNEL_1, scale);
  dac_offset_set(DAC_CHANNEL_1, offset);
  dac_invert_set(DAC_CHANNEL_1, invert);
  delay(10);
  //float frequency = RTC_FAST_CLK_FREQ_APPROX / (1 + clk_8m_div) * (float) frequency_step / 65536;
  //Serial.printf("clk_8m_div: %d, frequency step: %d, frequency: %.0f Hz\n", clk_8m_div, frequency_step, frequency);
  //Serial.printf("DAC1 scale: %d, offset %d, invert: %d\n", scale, offset, invert);
  //vTaskDelay(2000/portTICK_PERIOD_MS);
//  byte I6=0, I7=0, I8=0;
int ipflg=0; int ipSt6=0,ipSt7=0,ipSt8=0;

 if (Serial.available()) {

  crntbtn17=digitalRead(17);crntbtn5=digitalRead(5);crntbtn18=digitalRead(18);crntbtn19=digitalRead(19);
   
  if(crntbtn17 != lsbtn17){
    ipflg=6;
    if(crntbtn17==HIGH){
      Serial.print("\nIP:6-1"); ipSt6=1;
    } else {
      Serial.print("\nIP:6-0"); ipSt6=0;
    }
     delay(20);
   }
   crntbtn17=lsbtn17;
   
   if(crntbtn5 != lsbtn5){
    ipflg=7;
    if(crntbtn5==HIGH){
      Serial.print("\nIP:7-1");ipSt7=1;
    } else {
      Serial.print("\nIP:7-0");ipSt7=0;
    }
     delay(20);
   }
   crntbtn5=lsbtn5;
   
   if(crntbtn18 != lsbtn18){
    ipflg=8;
    if(crntbtn18==HIGH){
      Serial.print("\nIP:8-1");ipSt8=1;
    } else {
      Serial.print("\nIP:8-0");ipSt8=0;
    }
     delay(20);
   }
   crntbtn18=lsbtn18;
   
  String SeRecv=Serial.readStringUntil('\n');
  if (SeRecv.indexOf("OP")>0){
    
    int indx1=SeRecv.indexOf(":");
    int indx2=SeRecv.indexOf("-");
    String sPin= SeRecv.substring(indx1+1,indx2); 
    int pin= sPin.toInt(); 
    int logic= SeRecv.substring(indx2+1).toInt();
    switch (pin){
      case 1: digitalWrite(15,logic);  break;
      case 2: digitalWrite(2,logic);  break;
      case 3: digitalWrite(0,logic);  break;
      case 4: digitalWrite(4,logic);  break;
      case 5: digitalWrite(16,logic);  break;
      default: break;
      }  SeRecv="";
     }

     switch (eventStat){
      case 27:startBT=false; ESP.restart();  break;
      case 34:startBT=true;  break;
      case 30: 
        if (paramLen == 6){
          String BTrecv=SerialBT.readStringUntil('\n'); //Serial.println(BTrecv); 
          
          int indx1=BTrecv.indexOf(":");
          int indx2=BTrecv.indexOf("-");
          String sPin= BTrecv.substring(indx1+1,indx2); //Serial.println(sPin);
          int pin= sPin.toInt();  //Serial.print("\nPIN "); Serial.print(pin);
          int logic= BTrecv.substring(indx2+1).toInt(); //Serial.print("\nLOGIC "); Serial.println(logic);
          switch (pin){
            case 1: digitalWrite(15,logic);  break;
            case 2: digitalWrite(2,logic); break;
            case 3: digitalWrite(0,logic); break;
            case 4: digitalWrite(4,logic); break;
            case 5: digitalWrite(16,logic); break;
            default: break;
               } BTrecv=""; //}
             }
            default: break;
          }
     
     if(startBT==true){
      SerialBT.print(SeRecv); SerialBT.flush(); SeRecv="";
      switch(ipflg){
        case 6: if (ipSt6==1) {SerialBT.print("\nIP:6-1"); ipflg=0;}
                  else if (ipSt6==0){SerialBT.print("\nIP:6-0"); ipflg=0;} break;
        case 7: if (ipSt7==1) {SerialBT.print("\nIP:7-1"); ipflg=0;}
                  else if (ipSt7==0){SerialBT.print("\nIP:7-0"); ipflg=0;} break;
        case 8: if (ipSt8==1) {SerialBT.print("\nIP:8-1"); ipflg=0;}
                  else if (ipSt8==0){SerialBT.print("\nIP:8-0"); ipflg=0;} break;
        default: break;
      }
     }
   }
 }
