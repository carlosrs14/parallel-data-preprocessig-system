#include "../lib/utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern char **stopwords, **matriz_entrada, **matriz_salida;
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


void preprocesar_linea(int i) {
    int count = 0;
    for (int j = 0; j < COLUMNS; j++) {
        char carct = matriz_entrada[i][j];
        // to lower case
        if (isupper(carct)) {
            carct = tolower(carct);
            matriz_entrada[i][j] = carct;
        }

        // numeros 
        if (isdigit(carct) || ispunct(carct)) continue;

        // stop words
        if (j == 0 || isspace(matriz_entrada[i][j - 1]) || ispunct(matriz_entrada[i][j - 1])) {
            int tam_stop_word = is_stop_word(&matriz_entrada[i][j]);
            if (tam_stop_word) {
                // con el dato del tamano de la stopword adelantamos la j
                // esto para ignorar las demas letras de la palabra
                j += tam_stop_word;
                continue;
            }
        }
        matriz_salida[i][count++] = carct;
    }
    matriz_salida[i][count] = '\0';
}

/*
retorna el tamano de la stopword, esto por conveniencia
para poder usar ese dato en la funcion que llama esta
*/ 
int is_stop_word(char *palabra) {
    int tam = 0;
    for (int i = 0; i < n_stopwords; i++) {
        tam = strlen(stopwords[i]);
        if (strncmp(palabra, stopwords[i], tam) == 0 && (isspace(palabra[tam]) || palabra[tam] == '\0')) {
            return tam;
        }
    }
    return 0;
}

