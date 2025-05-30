#include "../lib/solucion_barrera.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *preprocesamiento_barrera(void * arg) {
    int i = *((int *) arg);
    printf("mi indice es %d\n", i);
    pthread_exit(0);
}
