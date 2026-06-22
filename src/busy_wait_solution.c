#include "../include/busy_wait_solution.h"

extern char **input_matrix, **output_matrix;
extern int *indices;
extern int rows, delta, num_threads, laps;
extern pthread_t *ids;
extern int current_lap_rows;

int *turns;

void *preprocess_busy_wait(void *arg) {
    int idx = *((int *) arg);
    
    for (int i = 0; i < laps; i++) {
        while (turns[idx]); // Wait until turn is 0 (set by main thread)
        
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
        
        turns[idx] = 1; // Mark as done for this thread
    }
    
    pthread_exit(0);
}

void run_busy_wait_solution(FILE *file, FILE *file_out) {
    ids = (pthread_t *) allocate_vector(num_threads, sizeof(pthread_t));
    indices = (int *) allocate_vector(num_threads, sizeof(int));

    input_matrix = (char **) allocate_matrix(MAX_COMMENTS_TO_READ, COLUMNS, sizeof(char));
    output_matrix = (char **) allocate_matrix(MAX_COMMENTS_TO_READ, COLUMNS, sizeof(char));
    
    turns = (int *) allocate_vector(num_threads, sizeof(int));
    for (int i = 0; i < num_threads; i++) {
        turns[i] = 1;
    }
    
    for (int i = 0; i < num_threads; i++) {
        indices[i] = i;
        pthread_create(&ids[i], NULL, preprocess_busy_wait, &indices[i]);
    }
    
    int data_to_read = MAX_COMMENTS_TO_READ;

    for (int i = 0; i < laps; i++) {
        if (i == laps - 1) {
            data_to_read = rows - (MAX_COMMENTS_TO_READ * i);
        }
        read_data(file, input_matrix, data_to_read, COLUMNS);

        current_lap_rows = data_to_read;

        // Signal worker threads to start
        for (int j = 0; j < num_threads; j++) {
            turns[j] = 0;
        }
        
        // Busy wait for all worker threads to finish
        int completed;
        do {
            completed = 0;
            for (int j = 0; j < num_threads; j++) {
                completed += turns[j];
            }
        } while (completed != num_threads);
        
        // Save output to file
        save_data(file_out, output_matrix, data_to_read);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(ids[i], NULL);
    }

    free(turns);
}
