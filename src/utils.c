#include "../lib/utils.h"
#include <stdlib.h>

int leer_datos(FILE *file, char **data, int rows, int columns) {
    int cont = 0;
    for (int i = 0; i < rows; i++) {
        fgets(data[i], columns, file);
        cont++;
    }
    return cont;
}
char **asignar_espacio(int rows, int columns) {
    char ** matriz = (char **) malloc(sizeof(char *) * rows);
    for (int i = 0; i < rows; i++) {
        matriz[i] = (char *) malloc(sizeof(char) * columns);
    }
    return matriz;
}

void imprimir_matriz(char **data, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        printf("%d - %s", i, data[i]);
    }
}

void *asignar_espacio_vector(int tam, size_t size) {
    void * vec = malloc(tam * size );
    return vec;
}
