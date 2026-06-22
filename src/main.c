#include "../include/utils.h"
#include "../include/barrier_solution.h"
#include "../include/busy_wait_solution.h"
#include "../include/cond_var_solution.h"

char **input_matrix, **output_matrix, **stopwords;
pthread_t *ids;
int *indices;
int rows, delta, num_threads, num_stopwords, laps;
int current_lap_rows;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <num_threads> <input_file> <method: barrier|variable|espera>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    num_threads = atoi(argv[1]);
    char *method = argv[3];

    FILE *file = fopen(argv[2], "r");
    if (!file) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    if (fscanf(file, "%d ", &rows) != 1) {
        fprintf(stderr, "Error: Could not read rows count from input file\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    laps = rows / MAX_COMMENTS_TO_READ;
    if (rows % MAX_COMMENTS_TO_READ != 0) {
        laps++;
    }

    // Stopwords file was moved to the data/ directory
    FILE *file_stopwords = fopen("data/stopwords.txt", "r");
    if (!file_stopwords) {
        perror("Error opening stopwords file (expected data/stopwords.txt)");
        fclose(file);
        return EXIT_FAILURE;
    }

    if (fscanf(file_stopwords, "%d ", &num_stopwords) != 1) {
        fprintf(stderr, "Error: Could not read stopwords count from stopwords file\n");
        fclose(file_stopwords);
        fclose(file);
        return EXIT_FAILURE;
    }
    
    FILE *file_out = fopen("output.txt", "w");
    if (!file_out) {
        perror("Error opening output file (output.txt)");
        fclose(file_stopwords);
        fclose(file);
        return EXIT_FAILURE;
    }

    if (num_threads > 0) {
        delta = MAX_COMMENTS_TO_READ / num_threads;
        if (delta == 0) {
            delta = 1;
        }
    } else {
        delta = 1;
    }

    stopwords = (char **) allocate_matrix(num_stopwords, MAX_STOPWORD_LEN, sizeof(char));
    read_data(file_stopwords, stopwords, num_stopwords, MAX_STOPWORD_LEN);
    
    int free_rows_count = MAX_COMMENTS_TO_READ;
    
    if (strcmp("barrier", method) == 0) {
        printf("Running barrier solution...\n");
        run_barrier_solution(file, file_out);
    } else if (strcmp("cond_var", method) == 0) {
        printf("Running condition variable solution...\n");
        run_cond_var_solution(file, file_out);
        free_rows_count = rows;
    } else if (strcmp("busy_wait", method) == 0) {
        printf("Running busy-wait solution...\n");
        run_busy_wait_solution(file, file_out);
    } else {
        fprintf(stderr, "Unknown method: %s (choose 'barrier', 'cond_var', or 'busy_wait')\n", method);
        fclose(file_out);
        fclose(file_stopwords);
        fclose(file);
        for (int i = 0; i < num_stopwords; i++) {
            free(stopwords[i]);
        }
        free(stopwords);
        return EXIT_FAILURE;
    }
    
    fclose(file);
    fclose(file_stopwords);
    fclose(file_out);

    // Free resources safely
    if (input_matrix) {
        for (int i = 0; i < free_rows_count; i++) {
            free(input_matrix[i]);
        }
        free(input_matrix);
    }
    
    if (output_matrix) {
        for (int i = 0; i < free_rows_count; i++) {
            free(output_matrix[i]);
        }
        free(output_matrix);
    }
    
    for (int i = 0; i < num_stopwords; i++) {
        free(stopwords[i]);
    }
    free(stopwords);

    free(ids);
    free(indices);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Execution time: %.6f seconds\n", duration);
    return EXIT_SUCCESS;
}
