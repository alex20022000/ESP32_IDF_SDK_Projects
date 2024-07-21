#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"

#define TAG "STEPPER"

// Definiciones de pines
#define PIN_DIR GPIO_NUM_12
#define PIN_PUL GPIO_NUM_14
#define PIN_SLPRS GPIO_NUM_27
#define STEPS_PER_REV 200

// Inicializa los pines
esp_err_t init_ios(void) {
    gpio_reset_pin(PIN_DIR);
    gpio_reset_pin(PIN_PUL);
    gpio_reset_pin(PIN_SLPRS);

    gpio_set_direction(PIN_DIR, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_PUL, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_SLPRS, GPIO_MODE_OUTPUT);

    gpio_set_level(PIN_SLPRS, 1); // Activa el pin SLPRS
    ESP_LOGI(TAG, "Pin SLPRS (GPIO %d) set to HIGH", PIN_SLPRS);
    return ESP_OK;
}

void app_main(void) {
    ESP_LOGI(TAG, "Initializing GPIOs...");
    if (init_ios() == ESP_OK) {
        ESP_LOGI(TAG, "GPIOs initialized successfully");
    } else {
        ESP_LOGE(TAG, "Failed to initialize GPIOs");
    }

    // Configurar la dirección del motor en sentido horario
    gpio_set_level(PIN_DIR, 1);
    ESP_LOGI(TAG, "Pin DIR (GPIO %d) set to HIGH", PIN_DIR);

    while (1) {
        // Generar los pulsos para mover el motor
        for (int i = 0; i < STEPS_PER_REV; i++) {
            gpio_set_level(PIN_PUL, 1);
            ESP_LOGD(TAG, "Pin PUL (GPIO %d) set to HIGH", PIN_PUL);
            int64_t start_time = esp_timer_get_time();
            while (esp_timer_get_time() - start_time < 2000) {
                // Espera activa hasta que pasen 2000 microsegundos
            }
            gpio_set_level(PIN_PUL, 0);
            ESP_LOGD(TAG, "Pin PUL (GPIO %d) set to LOW", PIN_PUL);
            start_time = esp_timer_get_time();
            while (esp_timer_get_time() - start_time < 2000) {
                // Espera activa hasta que pasen 2000 microsegundos
            }
        }

        ESP_LOGI(TAG, "Completed one full rotation");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo antes de la siguiente rotación
    }
}
