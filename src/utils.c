#include "../lib/utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **stopwords;
extern int n_stopwords;
int leer_datos(FILE *file, char **data, int rows, int columns) {
    int cont = 0;
    for (int i = 0; i < rows; i++) {
        fgets(data[i], columns, file);
        int len = strlen(data[i]);
        if (len) data[i][len - 1] = '\0';
        cont++;
    }
    return cont;
}

void **asignar_espacio_matriz(int rows, int columns, size_t size) {
    void ** matriz = malloc(sizeof(void *) * rows);
    for (int i = 0; i < rows; i++) {
        matriz[i] = (void *) malloc(size * columns);
    }
    return matriz;
}

void imprimir_matriz(char **data, int rows) {
    for (int i = 0; i < rows; i++) {
        printf("%d - %s\n", i, data[i]);
    }
}

void *asignar_espacio_vector(int tam, size_t size) {
    void * vec = malloc(tam * size );
    return vec;
}

/*
retorna el tamano de la stopword, esto por conveniencia
para poder usar ese dato en la funcion que llama esta
*/ 
int is_stop_word(char *palabra) {
    int tam = 0;
    for (int i = 0; i < n_stopwords; i++) {
        tam = strlen(stopwords[i]);
        if (strncmp(palabra, stopwords[i], tam) == 0 && (palabra[tam] == ' ' || palabra[tam] == '\0')) {
            return tam;
        }
    }
    return 0;
}

