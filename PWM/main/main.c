#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h> // Librería de entradas y salidas
#include <freertos/timers.h>
#include <driver/ledc.h>

#define led1 2
#define PIN0 21

uint8_t led_level = 0;
static const char *tag = "Status Timer";
static const char *tag2 = "Callback";
TimerHandle_t xTimers;
int interval = 50;
int timerId = 1;
int dutyR = 0;
int dutyG = 300;
int dutyB = 600;

//vector para almacenar las diferencias de tiempo
#define VECTOR_SIZE  100
uint32_t time_diffs[VECTOR_SIZE];
int index = 0;

TickType_t prev_ticks = 0;

esp_err_t init_led(void);
esp_err_t init_pins(void);
esp_err_t blink_led(void);
esp_err_t set_timer(void);
esp_err_t set_pwm(void);
esp_err_t set_pwm_duty(void);

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
    dutyR += 10;
    if(dutyR > 1023)
        dutyR = 0;

    dutyG += 10;
    if(dutyG > 1023)
        dutyG = 0;

    dutyB += 10;
    if(dutyB > 1023)
        dutyB = 0;


    blink_led();
    set_pwm_duty();
}

void app_main(void)
{
    init_led();
    //init_pins();
    set_pwm();
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

esp_err_t set_pwm(void)
{
    ledc_channel_config_t channelConfigR = {0};
    channelConfigR.gpio_num = 33;
    channelConfigR.speed_mode = LEDC_HIGH_SPEED_MODE;
    channelConfigR.channel = LEDC_CHANNEL_0;
    channelConfigR.intr_type = LEDC_INTR_DISABLE;
    channelConfigR.timer_sel = LEDC_TIMER_0;
    channelConfigR.duty = 0;

    ledc_channel_config_t channelConfigG = {0};
    channelConfigR.gpio_num = 25;
    channelConfigR.speed_mode = LEDC_HIGH_SPEED_MODE;
    channelConfigR.channel = LEDC_CHANNEL_1;
    channelConfigR.intr_type = LEDC_INTR_DISABLE;
    channelConfigR.timer_sel = LEDC_TIMER_0;
    channelConfigR.duty = 0;

    ledc_channel_config_t channelConfigB = {0};
    channelConfigR.gpio_num = 26;
    channelConfigR.speed_mode = LEDC_HIGH_SPEED_MODE;
    channelConfigR.channel = LEDC_CHANNEL_2;
    channelConfigR.intr_type = LEDC_INTR_DISABLE;
    channelConfigR.timer_sel = LEDC_TIMER_0;
    channelConfigR.duty = 0;

    ledc_channel_config(&channelConfigR);
    ledc_channel_config(&channelConfigG);
    ledc_channel_config(&channelConfigB);


    ledc_timer_config_t timerconfig = {0};
    timerconfig.speed_mode = LEDC_HIGH_SPEED_MODE;
    timerconfig.duty_resolution = LEDC_TIMER_10_BIT;
    timerconfig.timer_num = LEDC_TIMER_0;
    timerconfig.freq_hz = 20000; //20kHz

    ledc_timer_config(&timerconfig);

    return ESP_OK;
}

esp_err_t set_pwm_duty(void)
{
    ledc_set_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_0,dutyR);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_1,dutyG);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_2,dutyB);

    ledc_update_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_1);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_2);

    return ESP_OK;
}