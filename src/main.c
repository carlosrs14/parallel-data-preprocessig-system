#include "../lib/utils.h"
#include "../lib/solucion_barrera.h"
#include "../lib/solucion_espera_activa.h"
#include "../lib/solucion_variable_condicion.h"

char **matriz_entrada, **matriz_salida, **stopwords;
pthread_t *ids;
int *indices;
int rows, delta, n_threads, n_stopwords, laps;

void hacer_solucion_barrera(FILE * file, FILE * file_out);

int main(int argc, char *argv[]) {
    n_threads = atoi(argv[1]);
    FILE * file = fopen(argv[2], "r");
    char *solucion = argv[3];
    fscanf(file, "%d ", &rows);

    laps = rows / MAX_COMENTS_TO_READ;
    // validar cuando no es divisor
    if (rows % MAX_COMENTS_TO_READ != 0) laps++;

    FILE * file_stopwords = fopen("../stopwords.txt", "r");
    fscanf(file_stopwords, "%d ", &n_stopwords);
    
    FILE * file_out = fopen("../salida.txt", "w");

    delta = MAX_COMENTS_TO_READ / n_threads;

    stopwords = (char **) asignar_espacio_matriz(n_stopwords, MAX_TAM_STOPWORD, sizeof(char));

    leer_datos(file_stopwords, stopwords, n_stopwords, MAX_TAM_STOPWORD);
    
    int a_liberar = MAX_COMENTS_TO_READ;
    
    if (strcmp("barrera", solucion) == 0) {
        printf("haciendo solucion barrera\n");
        hacer_solucion_barrera(file, file_out);
    }
    
    if (strcmp("variable", solucion) == 0) {
        printf("haciendo solucion variable de condicion\n");
        hacer_solucion_variable_condicion(file, file_out);
        a_liberar = rows;
    }

    if (strcmp("espera", solucion) == 0) {
        printf("haciendo solucion espera activa\n");
        hacer_solucion_espera_activa(file, file_out);
    }
    
    fclose(file);
    fclose(file_stopwords);

    for (int i = 0; i < a_liberar; i++) {
        free(matriz_entrada[i]);
        free(matriz_salida[i]);
    }
    
    for (int i = 0; i < n_stopwords; i++) {
        free(stopwords[i]);
    }

    free(ids);
    free(indices);

    free(stopwords);
    free(matriz_entrada);
    free(matriz_salida);
    return EXIT_SUCCESS;
}

