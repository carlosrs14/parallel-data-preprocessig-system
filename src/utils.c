#include "../lib/utils.h"

extern char **stopwords, **matriz_entrada, **matriz_salida;
extern int n_stopwords;

void imprimir_matriz(char **data, int rows) {
    for (int i = 0; i < rows; i++) {
        printf("%d - %s\n", i, data[i]);
    }
}

void copiar_matriz(char **source, char **target, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            target[i][j] = source[i][j];
        }
    }
}

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

void guardar_datos(FILE *file, char **data, int rows) {
    for (int i = 0; i < rows; i++) {
        fprintf(file, "%s\n", data[i]);
    }
}

void *asignar_espacio_vector(int tam, size_t size) {
    void * vec = malloc(tam * size );
    return vec;
}

void **asignar_espacio_matriz(int rows, int columns, size_t size) {
    void ** matriz = malloc(sizeof(void *) * rows);
    for (int i = 0; i < rows; i++) {
        matriz[i] = (void *) malloc(size * columns);
    }
    return matriz;
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

void preprocesar_linea(int linea) {
    int count = 0;
    for (int j = 0; j < COLUMNS; j++) {
        char carct = matriz_entrada[linea][j];
        // to lower case
        if (isupper(carct)) {
            carct = tolower(carct);
            matriz_entrada[linea][j] = carct;
        }

        // numeros y puntos 
        if (isdigit(carct) || ispunct(carct)) continue;

        // stop words
        if (j == 0 || isspace(matriz_entrada[linea][j - 1]) || ispunct(matriz_entrada[linea][j - 1])) {
            int tam_stop_word = is_stop_word(&matriz_entrada[linea][j]);
            if (tam_stop_word) {
                // con el dato del tamano de la stopword adelantamos la j
                // esto para ignorar las demas letras de la palabra
                j += tam_stop_word;
                continue;
            }
        }
        matriz_salida[linea][count++] = carct;
    }
    matriz_salida[linea][count] = '\0';
}

void preprocesar_linea_to_lower(int linea) {
    for (int j = 0; j < COLUMNS; j++) {
        char carct = matriz_entrada[linea][j];
        // to lower case
        if (isupper(carct)) {
            carct = tolower(carct);
            matriz_entrada[linea][j] = carct;
        }
        matriz_salida[linea][j] = carct;
    }
}

void preproecesar_linea_numeros(int linea) {
    int count = 0;
    for (int j = 0; j < COLUMNS; j++) {
        char carct = matriz_entrada[linea][j];
        // numeros 
        if (isdigit(carct)) continue;

        matriz_salida[linea][count++] = carct;
    }
    matriz_salida[linea][count] = '\0';
}

void preproecesar_linea_puntos(int linea) {
    int count = 0;
    for (int j = 0; j < COLUMNS; j++) {
        char carct = matriz_entrada[linea][j];
        // puntos 
        if (ispunct(carct)) continue;
        matriz_salida[linea][count++] = carct;
    }
    matriz_salida[linea][count] = '\0';
}

void preproecesar_linea_stopwords(int linea) {
    int count = 0;
    for (int j = 0; j < COLUMNS; j++) {
        char carct = matriz_entrada[linea][j];
        // stop words
        if (j == 0 || isspace(matriz_entrada[linea][j - 1]) || ispunct(matriz_entrada[linea][j - 1])) {
            int tam_stop_word = is_stop_word(&matriz_entrada[linea][j]);
            if (tam_stop_word) {
                // con el dato del tamano de la stopword adelantamos la j
                // esto para ignorar las demas letras de la palabra
                j += tam_stop_word;
                continue;
            }
        }
        matriz_salida[linea][count++] = carct;
    }
    matriz_salida[linea][count] = '\0';
}
