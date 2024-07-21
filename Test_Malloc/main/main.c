#include <stdio.h>
#include <stdlib.h>

void app_main(void)
{
    int *array;
    int *arrayCalloc;

    array = malloc(10 * sizeof(int));  // malloc reserva los espacios de memoria sin importar que haya dentro
    arrayCalloc = calloc(10, sizeof(int));  // calloc reserva los espacios de memoria Y LOS INICIALIZA EN 0
    //array = realloc(array, 15 * sizeof(int)); redimensiona el tamaño de bytes reservados en memoria, CONSERVANDO los datos que hay dentro del array

    if (array == NULL)
    {
        printf("Ha ocurrido un error al momento de reservar la memoria");
        exit(1);
    }

    for (int i = 0; i < 10; i++)
{
    array[i] = i; // Inicializando el array con i
}

    // imprimimos array malloc
    for(int i = 0; i <10; i++)
        printf("%i ", *(array + i));    // = array[i]

    free(array);

    // imprimimos array calloc
    printf("\n");
    for(int i = 0; i <10; i++)
        printf("%i ", *(arrayCalloc + i));    // = array[i]

    free(arrayCalloc);

}
