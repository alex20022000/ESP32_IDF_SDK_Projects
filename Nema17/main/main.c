#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

// Definiciones de pines
#define PIN_DIR GPIO_NUM_12
#define PIN_PUL GPIO_NUM_14
#define PIN_SLPRS GPIO_NUM_27
#define STEPS_PER_REV 200
#define PULSE_DELAY_US 1000

// Inicializa los pines
void init_ios(void) {
    gpio_reset_pin(PIN_DIR);
    gpio_reset_pin(PIN_PUL);
    gpio_reset_pin(PIN_SLPRS);

    gpio_set_direction(PIN_DIR, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_PUL, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_SLPRS, GPIO_MODE_OUTPUT);

    gpio_set_level(PIN_SLPRS, 1); // Activa el pin SLPRS
}

void app_main(void) {
    init_ios();

    gpio_set_level(PIN_DIR, 1); // Configurar la dirección del motor en sentido horario

    while (1) {
        // Generar los pulsos para mover el motor una vuelta completa
        for (int i = 0; i < STEPS_PER_REV; i++) {
            gpio_set_level(PIN_PUL, 1);
            int64_t start_time = esp_timer_get_time();
            while (esp_timer_get_time() - start_time < PULSE_DELAY_US) {
                // Espera activa hasta que pasen PULSE_DELAY_US microsegundos
            }
            gpio_set_level(PIN_PUL, 0);
            start_time = esp_timer_get_time();
            while (esp_timer_get_time() - start_time < PULSE_DELAY_US) {
                // Espera activa hasta que pasen PULSE_DELAY_US microsegundos
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo antes de la siguiente vuelta
    }
}
