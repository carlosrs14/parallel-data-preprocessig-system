#ifndef UTILS_H
#define UTILS_H

#define COLUMNS 128
#define MAX_COMMENTS_TO_READ 100
#define MAX_STOPWORD_LEN 32

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

void print_matrix(char **data, int rows);
void copy_matrix(char **source, char **target, int rows, int columns);
int read_data(FILE *file, char **data, int rows, int columns);
void save_data(FILE *file, char **data, int rows);
void *allocate_vector(int size, size_t element_size);
void **allocate_matrix(int rows, int columns, size_t element_size);
int is_stop_word(char *word);
void preprocess_line(int line);
void preprocess_line_to_lower(int line);
void preprocess_line_digits(int line);
void preprocess_line_punctuation(int line);
void preprocess_line_stopwords(int line);

#endif