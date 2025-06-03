#ifndef UTILS_H
#define UTILS_H

#define COLUMNS 128
#define MAX_COMENTS_TO_READ 100
#define MAX_TAM_STOPWORD 32

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

void imprimir_matriz(char **data, int rows);
void copiar_matriz(char **source, char **target, int rows, int columns);
int leer_datos(FILE *file, char **data, int rows, int columns);
void guardar_datos(FILE *file, char **data, int rows);
void *asignar_espacio_vector(int tam, size_t size);
void **asignar_espacio_matriz(int rows, int columns, size_t size);
int is_stop_word(char *palabra);
void preprocesar_linea(int linea);
void preprocesar_linea_to_lower(int linea);
void preprocesar_linea_numeros(int linea);
void preprocesar_linea_puntos(int linea);
void preprocesar_linea_stopwords(int linea);
#endif