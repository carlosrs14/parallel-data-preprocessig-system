#include "../include/cond_var_solution.h"

extern char **input_matrix, **output_matrix;
extern int *indices;
extern int rows, num_threads;
extern pthread_t *ids;

int turn = 0;
pthread_cond_t cond;
pthread_mutex_t mutex, mutex2;

void *preprocess_cond_var(void *args) {
    int idx = *((int *) args);
    
    // Wait for the thread's turn in the pipeline sequence
    pthread_mutex_lock(&mutex);
    while (turn != idx) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    // Apply the specific processing stage for this thread
    for (int i = 0; i < rows; i++) {
        switch (idx) {
            case 0:
                preprocess_line_to_lower(i);
                break;
            case 1:
                preprocess_line_digits(i);
                break;
            case 2:
                preprocess_line_punctuation(i);
                break;
            case 3:
                preprocess_line_stopwords(i);
                break;
            default:
                break;
        }
    }
    
    // Copy the current outputs to input_matrix to feed into the next stage
    pthread_mutex_lock(&mutex2);
    copy_matrix(output_matrix, input_matrix, rows, COLUMNS);  
    turn++;
    pthread_cond_broadcast(&cond); // Signal other threads to check their turn
    pthread_mutex_unlock(&mutex2);
    
    pthread_exit(0);    
}

void run_cond_var_solution(FILE *file, FILE *file_out) {
    input_matrix = (char **) allocate_matrix(rows, COLUMNS, sizeof(char));
    output_matrix = (char **) allocate_matrix(rows, COLUMNS, sizeof(char));
    
    num_threads = 4; // Hardcoded in the cond-var solution to represent the 4 stages
    
    ids = (pthread_t *) allocate_vector(num_threads, sizeof(pthread_t));
    indices = (int *) allocate_vector(num_threads, sizeof(int));
    
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex2, NULL);

    read_data(file, input_matrix, rows, COLUMNS);
    
    for (int i = 0; i < num_threads; i++) {
        indices[i] = i;
        pthread_create(&ids[i], NULL, preprocess_cond_var, &indices[i]);
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(ids[i], NULL);
    }
    
    save_data(file_out, output_matrix, rows);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&cond);
}
