#include "../lib/utils.h"
#include "../lib/solucion_barrera.h"
#include "../lib/solucion_espera_activa.h"

char **matriz_entrada, **matriz_salida, **stopwords;
pthread_t *ids;
int *indices;
int rows, delta, n_threads, n_stopwords, laps;

void hacer_solucion_barrera(FILE * file, FILE * file_out);

int main(int argc, char *argv[]) {
    n_threads = atoi(argv[1]);
    FILE * file = fopen(argv[2], "r");
    fscanf(file, "%d ", &rows);

    laps = rows / MAX_COMENTS_TO_READ;
    // validar cuando no es divisor
    if (rows % MAX_COMENTS_TO_READ != 0) laps++;

    FILE * file_stopwords = fopen("../stopwords.txt", "r");
    fscanf(file_stopwords, "%d ", &n_stopwords);
    
    FILE * file_out = fopen("../salida.txt", "w");

    delta = MAX_COMENTS_TO_READ / n_threads;

    matriz_entrada = (char **) asignar_espacio_matriz(MAX_COMENTS_TO_READ, COLUMNS, sizeof(char));
    matriz_salida = (char **) asignar_espacio_matriz(MAX_COMENTS_TO_READ, COLUMNS, sizeof(char));
    stopwords = (char **) asignar_espacio_matriz(n_stopwords, MAX_TAM_STOPWORD, sizeof(char));

    leer_datos(file_stopwords, stopwords, n_stopwords, MAX_TAM_STOPWORD);
    
    ids = (pthread_t *) asignar_espacio_vector(n_threads, sizeof(pthread_t));
    indices = (int *) asignar_espacio_vector(n_threads, sizeof(int));
    
    hacer_solucion_espera_activa(file, file_out);

    fclose(file);
    fclose(file_stopwords);

    free(ids);
    free(indices);

    for (int i = 0; i < MAX_COMENTS_TO_READ; i++) {
        free(matriz_entrada[i]);
        free(matriz_salida[i]);
        free(stopwords[i]);
    }

    free(stopwords);
    free(matriz_entrada);
    free(matriz_salida);
    return EXIT_SUCCESS;
}

