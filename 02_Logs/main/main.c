/*
Hay varios tipos de logs, entre ellos:
ESP_LOGE -> inidca un error critico [color rojo]
ESP_LOGW -> inidca un warning       [color amarillo]
ESP_LOGI -> inidca informacion      [color verde]
ESP_LOGD -> debug
ESP_LOGV -> verbose
*/

#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>    //libreria de entradas y salidas


#define led1 2

uint8_t led_level = 0;
uint8_t count = 0;
static const char* tag = "Status";

esp_err_t init_led(void);
esp_err_t blink_led(void);

void app_main(void)
{
    init_led();
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        blink_led();
        count++;

        if(count<10)
        {
            ESP_LOGI(tag, "Value: %u.", count);
        }
        if(count>=10 && count<20)
        {
            ESP_LOGW(tag, "Value: %u.", count);
        }
        if(count>20)
        {
            ESP_LOGE(tag, "Value: %u.", count);
        }
        //printf("Led Level: %d\n", led_level);

    }    
}

esp_err_t init_led(void)
{
    gpio_reset_pin(led1);
    gpio_set_direction(led1, GPIO_MODE_OUTPUT);
    return ESP_OK;
}

esp_err_t blink_led(void)
{
    led_level = !led_level;
    gpio_set_level(led1, led_level);
    return ESP_OK;
}
