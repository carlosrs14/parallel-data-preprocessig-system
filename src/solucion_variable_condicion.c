#include "../lib/solucion_variable_condicion.h"
extern char **matriz_entrada, **matriz_salida;
extern int *indices;
extern int rows, n_threads;
extern pthread_t *ids;

int turno = 0;
pthread_cond_t cond;
pthread_mutex_t mutex, mutex2;

void *preprocesamiento_variable_condicion(void *args) {
    int idx = *((int *) args);
    pthread_mutex_lock(&mutex);
    while (turno != idx) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < rows; i++) {
        switch (idx) {
            case 0:
                preprocesar_linea_to_lower(i); 
                break;
            case 1:
                preprocesar_linea_numeros(i);
                break;
            case 2:
                preprocesar_linea_puntos(i);
                break;
            case 3:
                preprocesar_linea_stopwords(i);
                break;
            default:
                break;
        }
    }
    
    pthread_mutex_lock(&mutex2);
    copiar_matriz(matriz_salida, matriz_entrada, rows, COLUMNS);  
    turno++;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex2);
    
    pthread_exit(0);    
}

void hacer_solucion_variable_condicion(FILE *file, FILE *file_out) {

    matriz_entrada = (char **) asignar_espacio_matriz(rows, COLUMNS, sizeof(char));
    matriz_salida = (char **) asignar_espacio_matriz(rows, COLUMNS, sizeof(char));
    
    n_threads = 4;
    
    ids = (pthread_t *) asignar_espacio_vector(n_threads, sizeof(pthread_t));
    indices = (int *) asignar_espacio_vector(n_threads, sizeof(int));
    
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex2, NULL);

    leer_datos(file, matriz_entrada, rows, COLUMNS);
    
    for (int i = 0; i < n_threads; i++) {
        indices[i] = i;
        pthread_create(&ids[i], NULL, preprocesamiento_variable_condicion, &indices[i]);
    }
    
    for (int i = 0; i < n_threads; i++) {
        pthread_join(ids[i], NULL);
    }
    
    guardar_datos(file_out, matriz_salida, rows);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&cond);
}