#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

int adc_raw = 0;

esp_err_t set_adc(void);


void app_main(void)
{

}

esp_err_t set_adc(void)
{
    esp_err_t adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_12);     /*!< ADC1 channel 4 is GPIO32 */
    esp_err_t adc1_config_width(ADC_WIDTH_BIT_12);

    return ESP_OK;
}