#include "../lib/solucion_espera_activa.h"
// 0 cuando es turno del hilo principal
// 1 cuando es turno de los hilos secundarios

extern char **matriz_entrada, **matriz_salida;
extern int *indices;
extern int rows, delta, n_threads, laps;
extern pthread_t *ids;
int *turnos;

void *preprocesamiento_espera_activa(void *arg) {
    int idx = *((int *) arg);
    int inicio = idx * delta;
    int fin = (idx == n_threads - 1)? MAX_COMENTS_TO_READ: inicio + delta;

    for (int i = 0; i < laps; i++) {
        while (turnos[idx]);
        for (int j = inicio; j < fin; j++) {
            preprocesar_linea(j);
        }
        turnos[idx] = 1;
    }
    
    pthread_exit(0);
}

void hacer_solucion_espera_activa(FILE *file, FILE *file_out) {
    turnos = (int *) asignar_espacio_vector(n_threads, sizeof(int));
    for (int i = 0; i < n_threads; i++) {
        turnos[i] = 1;
    }
    
    for (int i = 0; i < n_threads; i++) {
        indices[i] = i;
        pthread_create(&ids[i], NULL, preprocesamiento_espera_activa, &indices[i]);
    }
    
    int datos_a_leer = MAX_COMENTS_TO_READ;

    for (int i = 0; i < laps; i++) {
        // leer max coments o los que falten
        if (i == laps - 1) datos_a_leer = rows - (MAX_COMENTS_TO_READ * i);
        leer_datos(file, matriz_entrada, datos_a_leer, COLUMNS);

        // avisa que terminó
        for (int j = 0; j < n_threads; j++) {
            turnos[j] = 0;
        }
        
        // espera a los demás
        int terminados;
        do {
            terminados = 0;
            for (int j = 0; j < n_threads; j++) {
                terminados += turnos[j];
            }
        } while (terminados != n_threads);
        
        // guardarlo en el archivo de salida
        guardar_datos(file_out, matriz_salida, datos_a_leer);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(ids[i], NULL);
    }

    free(turnos);
}