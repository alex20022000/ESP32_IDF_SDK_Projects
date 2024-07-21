/*===================[inclusions]===================*/

#include <stdio.h>
#include <FreeRTOSConfig.h>
#include <freertos/task.h>

/*===================[macros and definitions]===================*/

#define TAM_PILA 150    // bytes
#define PRIO_TAREA1 1
#define PRIO_TAREA2 1
#define mainDELAY_LOOP_COUNT (0xffffff)


static void vTarea1(void *pvParameters)
{
    const char *pcTaskName = "Tarea 1 is running\r\n";
    uint32_t ul;

    for(;;)
    {
        printf("%p/n",pcTaskName);
        for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++)
        {

        }
    }
}

static void vTarea2(void *pvParameters)
{
    const char *pcTaskName = "Tarea 2 is running\r\n";
    uint32_t ul;

    for(;;)
    {
        printf("%p/n",pcTaskName);
        for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++)
        {

        }
    }
}

void app_main(void)
{
    xTaskCreate(vTarea1, (const char *)"Tarea1", TAM_PILA/4, NULL, PRIO_TAREA1, NULL);
    xTaskCreate(vTarea2, (const char *)"Tarea2", TAM_PILA/4, NULL, PRIO_TAREA2, NULL);

    vTaskStartScheduler();  // invocamos al planificador para que ejecute las tareas

    // nunca deberia llegar aqui
    for(;;);

}
