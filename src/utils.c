#include "../include/utils.h"

extern char **stopwords, **input_matrix, **output_matrix;
extern int num_stopwords;

void print_matrix(char **data, int rows) {
    for (int i = 0; i < rows; i++) {
        printf("%d - %s\n", i, data[i]);
    }
}

void copy_matrix(char **source, char **target, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            target[i][j] = source[i][j];
            if (source[i][j] == '\0') {
                break;
            }
        }
    }
}

int read_data(FILE *file, char **data, int rows, int columns) {
    int count = 0;
    for (int i = 0; i < rows; i++) {
        if (fgets(data[i], columns, file) == NULL) {
            break;
        }
        int len = strlen(data[i]);
        if (len > 0 && data[i][len - 1] == '\n') {
            data[i][len - 1] = '\0';
        }
        count++;
    }
    return count;
}

void save_data(FILE *file, char **data, int rows) {
    for (int i = 0; i < rows; i++) {
        fprintf(file, "%s\n", data[i]);
    }
}

void *allocate_vector(int size, size_t element_size) {
    void *vec = malloc(size * element_size);
    if (!vec) {
        perror("Failed to allocate vector");
        exit(EXIT_FAILURE);
    }
    return vec;
}

void **allocate_matrix(int rows, int columns, size_t element_size) {
    void **matrix = malloc(sizeof(void *) * rows);
    if (!matrix) {
        perror("Failed to allocate matrix rows");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(element_size * columns);
        if (!matrix[i]) {
            perror("Failed to allocate matrix columns");
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

/*
 * Returns the length of the stopword if the word is a stopword.
 * Checks for space, punctuation, or end of string as boundaries.
 */
int is_stop_word(char *word) {
    int len = 0;
    for (int i = 0; i < num_stopwords; i++) {
        len = strlen(stopwords[i]);
        if (strncmp(word, stopwords[i], len) == 0) {
            char next_char = word[len];
            if (isspace((unsigned char)next_char) || ispunct((unsigned char)next_char) || next_char == '\0') {
                return len;
            }
        }
    }
    return 0;
}

void preprocess_line(int line) {
    int count = 0;
    for (int j = 0; j < COLUMNS; j++) {
        char carct = input_matrix[line][j];
        if (carct == '\0') {
            break;
        }

        // To lowercase
        if (isupper((unsigned char)carct)) {
            carct = tolower((unsigned char)carct);
            input_matrix[line][j] = carct;
        }

        // Ignore digits and punctuation
        if (isdigit((unsigned char)carct) || ispunct((unsigned char)carct)) {
            continue;
        }

        // Remove stopwords
        if (j == 0 || isspace((unsigned char)input_matrix[line][j - 1]) || ispunct((unsigned char)input_matrix[line][j - 1])) {
            int stopword_len = is_stop_word(&input_matrix[line][j]);
            if (stopword_len > 0) {
                j += stopword_len;
                continue;
            }
        }
        output_matrix[line][count++] = carct;
    }
    output_matrix[line][count] = '\0';
}

void preprocess_line_to_lower(int line) {
    for (int j = 0; j < COLUMNS; j++) {
        char carct = input_matrix[line][j];
        if (carct == '\0') {
            output_matrix[line][j] = '\0';
            break;
        }
        if (isupper((unsigned char)carct)) {
            carct = tolower((unsigned char)carct);
            input_matrix[line][j] = carct;
        }
        output_matrix[line][j] = carct;
    }
}

void preprocess_line_digits(int line) {
    int count = 0;
    for (int j = 0; j < COLUMNS; j++) {
        char carct = input_matrix[line][j];
        if (carct == '\0') {
            break;
        }
        if (isdigit((unsigned char)carct)) {
            continue;
        }
        output_matrix[line][count++] = carct;
    }
    output_matrix[line][count] = '\0';
}

void preprocess_line_punctuation(int line) {
    int count = 0;
    for (int j = 0; j < COLUMNS; j++) {
        char carct = input_matrix[line][j];
        if (carct == '\0') {
            break;
        }
        if (ispunct((unsigned char)carct)) {
            continue;
        }
        output_matrix[line][count++] = carct;
    }
    output_matrix[line][count] = '\0';
}

void preprocess_line_stopwords(int line) {
    int count = 0;
    for (int j = 0; j < COLUMNS; j++) {
        char carct = input_matrix[line][j];
        if (carct == '\0') {
            break;
        }
        if (j == 0 || isspace((unsigned char)input_matrix[line][j - 1]) || ispunct((unsigned char)input_matrix[line][j - 1])) {
            int stopword_len = is_stop_word(&input_matrix[line][j]);
            if (stopword_len > 0) {
                j += stopword_len;
                continue;
            }
        }
        output_matrix[line][count++] = carct;
    }
    output_matrix[line][count] = '\0';
}
