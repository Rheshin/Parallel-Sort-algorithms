#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "sorting.h"

/*
   bubble sort -- sequential, parallel --
*/

void sequential_bubble_sort(uint64_t *T, const uint64_t size) {
    /* TODO: sequential implementation of bubble sort */
    
    for (int  j = size - 1; j > 0; j--)
    {
        /* code */
        for (uint64_t i = 0; i < j ; i++)
        {   uint64_t temp_buffer;
            if( T[i] > T[i+1])
            {   
                temp_buffer = T[i];
                T[i] = T[i+1];
                T[i+1] = temp_buffer;
                // printf(" value is : %ld \n",temp_buffer);
            } 
        }   
    }
    return;
}

void parallel_bubble_sort(uint64_t *T, uint64_t size) {
    int sorted = 0;
    
    while (!sorted) {
        sorted = 1;/* optimistic boolean, we set it to true and change it to false it anything has been swapped */
        
        // Step 1: Sort chunks in parallel
        #pragma omp parallel
        {
            int num_threads = omp_get_num_threads();
            int thread_id = omp_get_thread_num();
            
            // Calculate boundaries
            uint64_t chunk_size = size / num_threads;
            /* for each thread having a "thread_id" we attributes it a unique starting and ending point ( no intersect ) */
            uint64_t start = thread_id * chunk_size;
            uint64_t end = (thread_id == num_threads - 1) ? size : (thread_id + 1) * chunk_size;
            
            // Do a bubble sort on each chunk
            for (uint64_t i = start; i < end - 1; i++) {
                for (uint64_t j = start; j < end - 1; j++) {
                    if (T[j] > T[j + 1]) {
                        // Swap
                        uint64_t temp = T[j];
                        T[j] = T[j + 1];
                        T[j + 1] = temp;
                        sorted = 0; // Mark as not fully sorted
                    }
                }
            }
        }
        
        for (int i = 0; i < size - 1; i++) {
            if (T[i] > T[i + 1]) {
                // Swap
                uint64_t temp = T[i];
                T[i] = T[i + 1];
                T[i + 1] = temp;
                sorted = 0;
            }
        }
    }
    return;
}

int main(int argc, char **argv) {
    // Init cpu_stats to measure CPU cycles and elapsed time
    struct cpu_stats *stats = cpu_stats_init();

    unsigned int exp;

    /* the program takes one parameter N which is the size of the array to
       be sorted. The array will have size 2^N */
    if (argc != 2) {
        fprintf(stderr, "Usage: bubble.run N \n");
        exit(-1);
    }

    uint64_t N = 1 << (atoi(argv[1]));
    /* the array to be sorted */
    uint64_t *X = (uint64_t *)malloc(N * sizeof(uint64_t));

    printf("--> Sorting an array of size %lu\n", N);
#ifdef RINIT
    printf("--> The array is initialized randomly\n");
#endif

    for (exp = 0; exp < NB_EXPERIMENTS; exp++) {
#ifdef RINIT
        init_array_random(X, N);
#else
        init_array_sequence(X, N);
#endif

        cpu_stats_begin(stats);

        sequential_bubble_sort(X, N);

        experiments[exp] = cpu_stats_end(stats);

        /* verifying that X is properly sorted */
#ifdef RINIT
        if (!is_sorted(X, N)) {
            print_array(X, N);
            fprintf(stderr, "ERROR: the sequential sorting of the array failed\n");
            exit(-1);
        }
#else
        if (!is_sorted_sequence(X, N)) {
            print_array(X, N);
            fprintf(stderr, "ERROR: the sequential sorting of the array failed\n");
            exit(-1);
        }
#endif
    }

    println_cpu_stats_report("bubble serial", average_report(experiments, NB_EXPERIMENTS));

    for (exp = 0; exp < NB_EXPERIMENTS; exp++) {
#ifdef RINIT
        init_array_random(X, N);
#else
        init_array_sequence(X, N);
#endif

        cpu_stats_begin(stats);

        parallel_bubble_sort(X, N);

        experiments[exp] = cpu_stats_end(stats);

        /* verifying that X is properly sorted */
#ifdef RINIT
        if (!is_sorted(X, N)) {
            print_array(X, N);
            fprintf(stderr, "ERROR: the parallel sorting of the array failed\n");
            exit(-1);
        }
#else
        if (!is_sorted_sequence(X, N)) {
            print_array(X, N);
            fprintf(stderr, "ERROR: the parallel sorting of the array failed\n");
            exit(-1);
        }
#endif
    }

    println_cpu_stats_report("bubble parallel", average_report(experiments, NB_EXPERIMENTS));

    /* print_array (X, N) ; */

    /* before terminating, we run one extra test of the algorithm */
    uint64_t *Y = (uint64_t *)malloc(N * sizeof(uint64_t));
    uint64_t *Z = (uint64_t *)malloc(N * sizeof(uint64_t));

#ifdef RINIT
    init_array_random(Y, N);
#else
    init_array_sequence(Y, N);
#endif

    memcpy(Z, Y, N * sizeof(uint64_t));

    sequential_bubble_sort(Y, N);
    parallel_bubble_sort(Z, N);

    if (!are_vector_equals(Y, Z, N)) {
        fprintf(stderr, "ERROR: sorting with the sequential and the parallel algorithm does not give the same result\n");
        exit(-1);
    }

    free(X);
    free(Y);
    free(Z);
}
