#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h> // Librería de entradas y salidas
#include <freertos/timers.h>

#define led1 2
#define PIN0 21

uint8_t led_level = 0;
static const char *tag = "Status Timer";
static const char *tag2 = "Callback";
TimerHandle_t xTimers;
int interval = 50;
int timerId = 1;

//vector para almacenar las diferencias de tiempo
#define VECTOR_SIZE  100
uint32_t time_diffs[VECTOR_SIZE];
int index = 0;

TickType_t prev_ticks = 0;

esp_err_t init_led(void);
esp_err_t init_pins(void);
esp_err_t blink_led(void);
esp_err_t set_timer(void);

void print_timestamp(TickType_t current_ticks)
{
    // obtengo el tiempo desde inicio de esp32 en ticks
    TickType_t ticks = xTaskGetTickCount();

    // convierto los ticks en milisegundos
    uint32_t time_in_ms = ticks * portTICK_PERIOD_MS;

    ESP_LOGI(tag2, "Timestamp: %lu ms", time_in_ms);
}

void vTimerCallback(TimerHandle_t pxTimer)
{
    ESP_LOGI(tag2, "Función callback iniciada");
    // Obtener el nivel actual del pin y cambiar su estado
    int level = gpio_get_level(PIN0);
    gpio_set_level(PIN0, !level);

    TickType_t current_ticks = xTaskGetTickCount();

    // Calcular la diferencia entre el timestamp actual y el anterior
    if (prev_ticks != 0) // No calcular la primera vez
    {
        uint32_t diff = (current_ticks - prev_ticks) * portTICK_PERIOD_MS;
        ESP_LOGI(tag2, "Diferencia de tiempo: %lu ms", (unsigned long)diff);

        // Almacenar la diferencia en el vector
        if (index < VECTOR_SIZE)
        {
            time_diffs[index++] = diff;
        }
    }

    print_timestamp(current_ticks);
    prev_ticks = current_ticks;
}

void app_main(void)
{
    init_led();
    init_pins();
    set_timer();
}

esp_err_t init_led(void)
{
    gpio_reset_pin(led1);
    gpio_set_direction(led1, GPIO_MODE_OUTPUT);
    return ESP_OK;
}

esp_err_t init_pins(void)
{
    gpio_reset_pin(PIN0);
    gpio_set_direction(PIN0, GPIO_MODE_OUTPUT);
    return ESP_OK;
}

esp_err_t blink_led(void)
{
    led_level = !led_level;
    gpio_set_level(led1, led_level);
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
