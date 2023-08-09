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

#define ADC_SAMPLE_COUNT 15
#define ADC1_TEST_CHANNEL (ADC1_CHANNEL_6)      // GPIO 34

uint32_t adc_raw_sample[ADC_SAMPLE_COUNT];

void print_chip_info()
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is xxxx with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
}

void setup() //app_main(void)
{
    print_chip_info();
    Serial.begin(115200);
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_TEST_CHANNEL, ADC_ATTEN_0db);

    while(1){
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

        printf("Avg: %d Dev: %4.1f%%\n", adc_raw_avg, adc_dev);
        //vTaskDelay(pdMS_TO_TICKS(200));
        delayMicroseconds(200);
    }
}

void loop() {}
