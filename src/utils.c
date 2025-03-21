#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sorting.h"

struct cpu_stats_report experiments[NB_EXPERIMENTS];

void init_array_sequence(uint64_t *T, uint64_t size) {
    for (uint64_t i = 0; i < size; i++) {
        T[i] = size - i;
    }
}

void init_array_random(uint64_t *T, uint64_t size) {
    uint64_t i;

    srand(time(NULL));

    for (i = 0; i < size; i++) {
        T[i] = rand() % size;
    }
}

void print_array(uint64_t *T, uint64_t size) {
    uint64_t i;

    for (i = 0; i < size; i++) {
        printf("%ld ", T[i]);
    }
    printf("\n");
}

/* test if the array is sorted assuming that the elements to be sorted
 * is a sequence of consecutive values */
int is_sorted_sequence(uint64_t *T, uint64_t size) {
    uint64_t i;

    for (i = 1; i < size; i++) {
        /* test designed specifically for our usecase */
        if (T[i - 1] + 1 != T[i])
            return 0;
    }
    return 1;
}

/* test if the array is sorted */
int is_sorted(uint64_t *T, uint64_t size) {
    uint64_t i;

    for (i = 1; i < size; i++) {
        if (T[i - 1] > T[i]) {
            return 0;
        }
    }
    return 1;
}

/* test if T1 and T2 are equal */
int are_vector_equals(uint64_t *T1, uint64_t *T2, uint64_t size) {
    uint64_t i;

    for (i = 0; i < size; i++) {
        if (T1[i] != T2[i]) {
            return 0;
        }
    }
    return 1;
}
