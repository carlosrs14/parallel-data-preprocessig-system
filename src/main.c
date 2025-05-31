#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "../lib/utils.h"
#include "../lib/solucion_barrera.h"

#define MAX_TAM 32

char **matriz_entrada, **matriz_salida;
extern char **stopwords;
pthread_t *ids;
int *indices;
int rows, delta, n_threads, n_stopwords;

int main(int argc, char *argv[]) {
    n_threads = atoi(argv[1]);
    FILE * file = fopen("../in0.txt", "r");
    fscanf(file, "%d ", &rows);

    FILE * file_stopwords = fopen("../stopwords.txt", "r");
    fscanf(file_stopwords, "%d ", &n_stopwords);
    
    delta = rows / n_threads;
    matriz_entrada = (char **) asignar_espacio_matriz(rows, COLUMNS, sizeof(char));
    matriz_salida = (char **) asignar_espacio_matriz(rows, COLUMNS, sizeof(char));
    stopwords = (char **) asignar_espacio_matriz(n_stopwords + 1, MAX_TAM, sizeof(char));
    stopwords[n_stopwords] = NULL;

    leer_datos(file, matriz_entrada, rows, COLUMNS);
    leer_datos(file_stopwords, stopwords, n_stopwords, MAX_TAM);
    //imprimir_matriz(stopwords, n_stopwords);
    // printf("%ld\n", strlen(stopwords[0]));
    // printf("%ld\n", strlen(stopwords[2]));
    // printf("%ld\n", strlen(stopwords[3]));
    // printf("%ld\n", strlen(stopwords[4]));
    
    ids = (pthread_t *) asignar_espacio_vector(n_threads, sizeof(pthread_t));
    indices = (int *) asignar_espacio_vector(n_threads, sizeof(int));
    
    for (int i = 0; i < n_threads; i++) {
        indices[i] = i;
        pthread_create(&ids[i], NULL, preprocesamiento_barrera, &indices[i]);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(ids[i], NULL);
    }
    imprimir_matriz(matriz_salida, rows);

    fclose(file);
    free(ids);
    free(indices);

    for (int i = 0; i < rows; i++) {
        free(matriz_entrada[i]);
        free(matriz_salida[i]);
    }

    free(matriz_entrada);
    free(matriz_salida);
    return EXIT_SUCCESS;
}