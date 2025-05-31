#include "../lib/solucion_barrera.h"
#include "../lib/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

extern char **matriz_entrada, **matriz_salida;
extern int *indices;
extern int rows, delta, n_threads;

void *preprocesamiento_barrera(void * arg) {
    int idx = *((int *) arg);
    int inicio = idx * delta, count;
    int fin = (idx == n_threads - 1)? rows: inicio + delta;
    
    for (int j = inicio; j < fin; j++) {
        preprocesar_linea(j);
    }
    
    pthread_exit(0);
}
