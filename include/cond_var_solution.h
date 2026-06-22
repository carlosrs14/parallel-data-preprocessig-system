#ifndef COND_VAR_SOLUTION_H
#define COND_VAR_SOLUTION_H

#include "utils.h"
#include <pthread.h>

void *preprocess_cond_var(void *arg);
void run_cond_var_solution(FILE *file, FILE *file_out);

#endif
