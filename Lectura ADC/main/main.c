#include <stdio.h>
#include <esp_log.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#include "driver/adc.h"


int adc_raw = 0;
int interval = 50;
int timerId = 1;

static const char *tag = "Status Timer";
TimerHandle_t xTimers;

esp_err_t set_adc(void);
esp_err_t set_timer(void);

void vTimerCallback(TimerHandle_t pxTimer);

void app_main(void)
{

}

esp_err_t set_adc(void)
{
    esp_err_t adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_12);     /*!< ADC1 channel 4 is GPIO32 */
    esp_err_t adc1_config_width(ADC_WIDTH_BIT_12);

    return ESP_OK;
}

esp_err_t set_timer(void)
{
    ESP_LOGI(tag, "Configuración inicial del temporizador");

    xTimers = xTimerCreate("Timer",                 // Solo un nombre de texto, no utilizado por el kernel.
                           pdMS_TO_TICKS(interval), // El periodo del temporizador en ticks.
                           pdTRUE,                  // Los temporizadores se recargarán automáticamente cuando expiren.
                           (void *)timerId,         // Asignar a cada temporizador una ID única igual a su índice de array.
                           vTimerCallback           // Cada temporizador llama al mismo callback cuando expira.
    );

    if (xTimers == NULL)
    { // El temporizador no fue creado.
        ESP_LOGE(tag, "No se pudo crear el temporizador");
    }
    else
    {
        // Iniciar el temporizador. No se especifica tiempo de bloqueo y, aunque lo hubiera, sería ignorado porque el planificador aún no se ha iniciado.
        if (xTimerStart(xTimers, 0) != pdPASS)
        {
            // El temporizador no pudo activarse.
            ESP_LOGE(tag, "No se pudo activar el temporizador");
        }
    }
    return ESP_OK;
}

void vTimerCallback(TimerHandle_t pxTimer)
{
    adc_raw = adc1_get_raw(ADC1_CHANNEL_4);


}