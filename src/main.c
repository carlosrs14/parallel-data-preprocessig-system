#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../lib/utils.h"
#include "../lib/solucion_barrera.h"

#define COLUMNS 64

char **matriz;
int n_threads;
pthread_t *ids;
int *indices;

int main(int argc, char *argv[]) {
    n_threads = atoi(argv[1]);
    int rows;
    FILE * file = fopen("../in1.txt", "r");
    fscanf(file, "%d ", &rows);
    matriz = asignar_espacio(rows, COLUMNS);
    leer_datos(file, matriz, rows, COLUMNS);
    imprimir_matriz(matriz, rows, COLUMNS);

    ids = (pthread_t *) asignar_espacio_vector(n_threads, sizeof(pthread_t));
    indices = (int *) asignar_espacio_vector(n_threads, sizeof(int));
    
    for (int i = 0; i < n_threads; i++) {
        indices[i] = i;
        pthread_create(&ids[i], NULL, preprocesamiento_barrera, &indices[i]);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(ids[i], NULL);
    }
    return EXIT_SUCCESS;
}