/* DAC Cosine Generator Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

   @Ps, 15 Jan, 2019
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

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int clk_8m_div = 0;      // RTC 8M clock divider (division is by clk_8m_div+1, i.e. 0 means 8MHz frequency)
int frequency_step = 39;  // Frequency step for CW generator //39 will give 5058Hz //38 will 4929
int scale = 1;           // 50% of the full scale
int offset;              // leave it default / 0 = no any offset
int invert = 2;          // invert MSB to get sine waveform

TaskHandle_t Task1;
TaskHandle_t Task2;

// Enable cosine waveform generator on a DAC channel
void dac_cosine_enable(dac_channel_t channel)
{
    SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN); // Enable tone generator common to both channels
    switch(channel) {
        case DAC_CHANNEL_1:
            SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);// Enable / connect tone tone generator on / to this channel
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, 2, SENS_DAC_INV1_S);// Invert MSB, otherwise part of waveform will have inverted
            break;
        case DAC_CHANNEL_2:
            SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN2_M);
            SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV2, 2, SENS_DAC_INV2_S);
            break;
        default :
           Serial.printf("_cosine_enable Channel No.%d\n", channel);
    }
}


/* Set frequency of internal CW generator common to both DAC channels
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
           Serial.printf("_scale_set Channel NO.%d\n", channel);
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
           Serial.printf("_offset_set Channel No.%d\n", channel);
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
           Serial.printf("_invert_set Channel No.%d\n", channel);
    }
}

/*Generate a sine waveform on both DAC channels:
 * DAC_CHANNEL_1 - GPIO25
 * DAC_CHANNEL_2 - GPIO26
 * Connect scope to both GPIO25 and GPIO26 to observe the waveform */
void setup()
{
  Serial.begin(115200); delay(10);
    dac_cosine_enable(DAC_CHANNEL_1);
    dac_output_enable(DAC_CHANNEL_1);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  //display.display();
  delay(200);
  display.clearDisplay();
  for(int i=0; i<40; i++){
    for(int j=0; j<40; j++){
      display.drawPixel(i*3, j*3, WHITE);
    }
  }
  display.display();
  delay(3000);
  
  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */
  xTaskCreatePinnedToCore(
      Task2code,
      "Task2", 
      10000, 
      NULL, 
      1,  
      &Task2, 
      1);
   delay(10);
}

void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

    while(1){
        // frequency setting is common to both channels
        dac_frequency_set(clk_8m_div, frequency_step);
        dac_scale_set(DAC_CHANNEL_1, scale);
        dac_offset_set(DAC_CHANNEL_1, offset);
        dac_invert_set(DAC_CHANNEL_1, invert);

        float frequency = RTC_FAST_CLK_FREQ_APPROX / (1 + clk_8m_div) * (float) frequency_step / 65536;
        
        Serial.printf("clk_8m_div: %d, frequency step: %d, frequency: %.0f Hz\n", clk_8m_div, frequency_step, frequency);
        Serial.printf("DAC2 scale: %d, offset %d, invert: %d\n", scale, offset, invert);
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}

void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  while(1){ 
  
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(BLACK, WHITE);        // Draw white text
  display.setCursor(25,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  //display.setTextColor(WHITE); // Draw 'inverse' text
  //display.setCursor(25,0); 
  //display.println(3.141592);
  
  display.setTextSize(3);             // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.print(F("PRITESH")); //display.println(0xDEADBEEF, HEX);

  display.display();
  delay(53000);

  }
}

void loop(){  }

