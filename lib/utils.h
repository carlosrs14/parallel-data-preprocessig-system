#include <stdio.h>

int leer_datos(FILE *file, char **data, int rows, int columns);
char **asignar_espacio(int rows, int columns);
void imprimir_matriz(char **data, int rows, int columns);
void *asignar_espacio_vector(int tam, size_t size);