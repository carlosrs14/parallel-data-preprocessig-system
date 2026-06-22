#include "../include/barrier_solution.h"

extern char **input_matrix, **output_matrix;
extern int *indices;
extern int rows, delta, num_threads, laps;
extern pthread_t *ids;
extern int current_lap_rows;

pthread_barrier_t barrier_start, barrier_end;

void *preprocess_barrier(void *arg) {
    int idx = *((int *) arg);
    
    for (int i = 0; i < laps; i++) {
        pthread_barrier_wait(&barrier_start);
        
        // Dynamically compute thread boundaries based on rows to process in this round
        int lap_delta = current_lap_rows / num_threads;
        if (lap_delta == 0) {
            lap_delta = 1;
        }
        
        int inicio = idx * lap_delta;
        int fin = (idx == num_threads - 1) ? current_lap_rows : inicio + lap_delta;
        
        if (inicio < current_lap_rows) {
            if (fin > current_lap_rows) {
                fin = current_lap_rows;
            }
            for (int j = inicio; j < fin; j++) {
                preprocess_line(j);
            }
        }
        
        pthread_barrier_wait(&barrier_end);
    }
    
    pthread_exit(0);
}

void run_barrier_solution(FILE *file, FILE *file_out) {
    ids = (pthread_t *) allocate_vector(num_threads, sizeof(pthread_t));
    indices = (int *) allocate_vector(num_threads, sizeof(int));
    
    input_matrix = (char **) allocate_matrix(MAX_COMMENTS_TO_READ, COLUMNS, sizeof(char));
    output_matrix = (char **) allocate_matrix(MAX_COMMENTS_TO_READ, COLUMNS, sizeof(char));
    
    pthread_barrier_init(&barrier_start, NULL, num_threads + 1);
    pthread_barrier_init(&barrier_end, NULL, num_threads + 1);

    for (int i = 0; i < num_threads; i++) {
        indices[i] = i;
        pthread_create(&ids[i], NULL, preprocess_barrier, &indices[i]);
    }
    
    int data_to_read = MAX_COMMENTS_TO_READ;

    for (int i = 0; i < laps; i++) {
        if (i == laps - 1) {
            data_to_read = rows - (MAX_COMMENTS_TO_READ * i);
        }
        read_data(file, input_matrix, data_to_read, COLUMNS);
        
        current_lap_rows = data_to_read;

        // Notify worker threads that data is ready
        pthread_barrier_wait(&barrier_start);
        
        // Wait for worker threads to finish processing
        pthread_barrier_wait(&barrier_end);

        // Save output to file
        save_data(file_out, output_matrix, data_to_read);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(ids[i], NULL);
    }

    pthread_barrier_destroy(&barrier_start);
    pthread_barrier_destroy(&barrier_end);
}
