#include "../lib/solucion_barrera.h"

extern char **matriz_entrada, **matriz_salida;
extern int *indices;
extern int rows, delta, n_threads, laps;
extern pthread_t *ids;
pthread_barrier_t barrier_start, barrier_end;

void *preprocesamiento_barrera(void * arg) {
    int idx = *((int *) arg);
    int inicio = idx * delta;
    int fin = (idx == n_threads - 1)? MAX_COMENTS_TO_READ: inicio + delta;
    for (int i = 0; i < laps; i++) {
        pthread_barrier_wait(&barrier_start);
        for (int j = inicio; j < fin; j++) {
            preprocesar_linea(j);
        }
        pthread_barrier_wait(&barrier_end);
    }
    
    pthread_exit(0);
}

void hacer_solucion_barrera(FILE * file, FILE * file_out) {
    pthread_barrier_init(&barrier_start, NULL, n_threads + 1);
    pthread_barrier_init(&barrier_end, NULL, n_threads + 1);

    for (int i = 0; i < n_threads; i++) {
        indices[i] = i;
        pthread_create(&ids[i], NULL, preprocesamiento_barrera, &indices[i]);
    }
    
    int datos_a_leer = MAX_COMENTS_TO_READ;

    for (int i = 0; i < laps; i++) {
        // leer max coments o los que falten
        if (i == laps - 1) datos_a_leer = rows - (MAX_COMENTS_TO_READ * i);
        leer_datos(file, matriz_entrada, datos_a_leer, COLUMNS);
        imprimir_matriz(matriz_entrada, datos_a_leer);

        // avisa que terminó
        pthread_barrier_wait(&barrier_start);
        
        // espera a los demás
        pthread_barrier_wait(&barrier_end);

        // guardarlo en el archivo de salida
        guardar_datos(file_out, matriz_salida, datos_a_leer);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(ids[i], NULL);
    }

    pthread_barrier_destroy(&barrier_start);
    pthread_barrier_destroy(&barrier_start);
}
