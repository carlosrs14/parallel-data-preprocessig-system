#include "../lib/solucion_barrera.h"
#include "../lib/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

extern char **matriz_entrada, **matriz_salida;
extern int *indices;
extern int rows, delta, n_threads;

void *preprocesamiento_barrera(void * arg) {
    int idx = *((int *) arg);
    int inicio = idx * delta, fin;
    if (idx == n_threads - 1) {
        fin = rows;
    } else {
        fin = inicio + delta;
    }
    int count;
    for (int i = inicio; i < fin; i++) {
        count = 0;
        for (int j = 0; j < COLUMNS; j++) {
            char carct = matriz_entrada[i][j];
            if (carct == '\n') {
                matriz_salida[i][count++] = '\n';
                break;   
            }
            // to lower case
            if (isupper(carct)) {
                carct = tolower(carct);
                matriz_entrada[i][j] = carct;
            }
            
            // numeros 
            if (isdigit(carct) || ispunct(carct)) continue;

            // stop words
            if (j == 0 || matriz_entrada[i][j - 1] == ' ') {
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
    
    pthread_exit(0);
}
