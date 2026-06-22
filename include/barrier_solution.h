#ifndef BARRIER_SOLUTION_H
#define BARRIER_SOLUTION_H

#include "utils.h"
#include <pthread.h>

void *preprocess_barrier(void *arg);
void run_barrier_solution(FILE *file, FILE *file_out);

#endif
