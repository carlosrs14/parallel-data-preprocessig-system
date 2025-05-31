#ifndef UTILS_H

#define UTILS_H
#define COLUMNS 128

#include <stdio.h>

int leer_datos(FILE *file, char **data, int rows, int columns);
void preprocesar_linea(int linea);
void **asignar_espacio_matriz(int rows, int columns, size_t size);
void imprimir_matriz(char **data, int rows);
void *asignar_espacio_vector(int tam, size_t size);
int is_stop_word(char *palabra);

#endif