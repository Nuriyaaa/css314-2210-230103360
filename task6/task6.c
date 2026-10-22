
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdatomic.h>

#define STUDENT_ID 230103360
#define THREADS 4
#define INCREMENTS 2000000LL
#define EXPECTED (THREADS * INCREMENTS)

volatile long long shared_counter = 0;
long long locked_counter = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

void *unsafe_increment(void *arg) {
    (void)arg;
    for (long long i = 0; i < INCREMENTS; i++) {
        long long value = shared_counter;
        shared_counter = value + 1;
    }
    return NULL;
}

void *safe_increment(void *arg) {
    (void)arg;
    for (long long i = 0; i < INCREMENTS; i++) {
        pthread_mutex_lock(&counter_mutex);
        locked_counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

void run_threads(void *(*function)(void *)) {
    pthread_t threads[THREADS];

    for (int i = 0; i < THREADS; i++) {
        if (pthread_create(&threads[i], NULL, function, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main(void) {
    printf("=== TASK 6: Synchronization Cost & Data Loss ===\n");
    printf("Student ID: %d\n", STUDENT_ID);
    printf("Threads: %d\n", THREADS);
    printf("Increments per thread: %lld\n", INCREMENTS);
    printf("Expected final value: %lld\n\n", (long long)EXPECTED);

    double start = now_seconds();
    run_threads(unsafe_increment);
    double unsafe_time = now_seconds() - start;

    long long actual = shared_counter;
    long long lost = (long long)EXPECTED - actual;
    double lost_percent = 100.0 * (double)lost / (double)EXPECTED;

    printf("Without mutex:\n");
    printf("Actual final value: %lld\n", actual);
    printf("Lost updates (estimated): %lld\n", lost);
    printf("Percentage of updates lost: %.2f%%\n", lost_percent);
    printf("Wall-clock time: %.3f ms\n\n", unsafe_time * 1000.0);

    start = now_seconds();
    run_threads(safe_increment);
    double safe_time = now_seconds() - start;

    printf("With mutex:\n");
    printf("Actual final value: %lld\n", locked_counter);
    printf("Wall-clock time: %.3f ms\n", safe_time * 1000.0);
    printf("Synchronization time penalty: %.3f ms\n",
           (safe_time - unsafe_time) * 1000.0);

    if (locked_counter != EXPECTED) {
        fprintf(stderr, "ERROR: synchronized counter verification failed.\n");
        pthread_mutex_destroy(&counter_mutex);
        return EXIT_FAILURE;
    }

    printf("Count verification: PASSED\n");
    printf("Benchmark completed successfully.\n");

    pthread_mutex_destroy(&counter_mutex);
    return 0;
}
