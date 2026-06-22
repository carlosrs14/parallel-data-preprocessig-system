#ifndef BUSY_WAIT_SOLUTION_H
#define BUSY_WAIT_SOLUTION_H

#include "utils.h"
#include <pthread.h>

void *preprocess_busy_wait(void *arg);
void run_busy_wait_solution(FILE *file, FILE *file_out);

#endif
