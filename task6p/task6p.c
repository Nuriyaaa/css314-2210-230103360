
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define STUDENT_ID 230103360
#define THREAD_COUNT 8
#define INCREMENTS_PER_THREAD 1000000LL
#define TOTAL_OPERATIONS (THREAD_COUNT * INCREMENTS_PER_THREAD)

static long long mutex_counter = 0;
static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
static _Atomic long long atomic_counter = 0;

typedef struct {
    int mode;
} ThreadData;

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}

static void *mutex_worker(void *arg) {
    (void)arg;

    for (long long i = 0; i < INCREMENTS_PER_THREAD; i++) {
        pthread_mutex_lock(&counter_mutex);
        mutex_counter++;
        pthread_mutex_unlock(&counter_mutex);
    }

    return NULL;
}

static void *cas_worker(void *arg) {
    (void)arg;

    for (long long i = 0; i < INCREMENTS_PER_THREAD; i++) {
        long long expected = atomic_load_explicit(&atomic_counter, memory_order_relaxed);

        while (!atomic_compare_exchange_weak_explicit(
            &atomic_counter,
            &expected,
            expected + 1,
            memory_order_relaxed,
            memory_order_relaxed
        )) {
        }
    }

    return NULL;
}

static double run_benchmark(void *(*worker)(void *)) {
    pthread_t threads[THREAD_COUNT];
    double start = now_seconds();

    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&threads[i], NULL, worker, NULL) != 0) {
            fprintf(stderr, "Thread creation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Thread join failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    return (now_seconds() - start) * 1000.0;
}

int main(void) {
    printf("=== TASK 6: MUTEX VS ATOMIC CAS ===\n");
    printf("Student ID: %d\n", STUDENT_ID);
    printf("Threads: %d\n", THREAD_COUNT);
    printf("Increments per thread: %lld\n", INCREMENTS_PER_THREAD);
    printf("Total operations: %lld\n\n", TOTAL_OPERATIONS);

    double mutex_ms = run_benchmark(mutex_worker);
    long long mutex_result = mutex_counter;

    double cas_ms = run_benchmark(cas_worker);
    long long cas_result = atomic_load_explicit(&atomic_counter, memory_order_relaxed);

    double mutex_throughput = (double)TOTAL_OPERATIONS / (mutex_ms / 1000.0);
    double cas_throughput = (double)TOTAL_OPERATIONS / (cas_ms / 1000.0);

    printf("Mutex Execution Time: %.3f ms\n", mutex_ms);
    printf("Mutex Throughput: %.2f ops/sec\n", mutex_throughput);
    printf("Mutex Final Counter: %lld\n\n", mutex_result);

    printf("Atomic CAS Execution Time: %.3f ms\n", cas_ms);
    printf("Atomic CAS Throughput: %.2f ops/sec\n", cas_throughput);
    printf("Atomic CAS Final Counter: %lld\n\n", cas_result);

    if (mutex_result != TOTAL_OPERATIONS || cas_result != TOTAL_OPERATIONS) {
        fprintf(stderr, "Verification failed: counter mismatch.\n");
        return 1;
    }

    printf("Throughput Ratio (CAS / Mutex): %.3fx\n", cas_throughput / mutex_throughput);
    printf("Verification Status: PASS\n");

    pthread_mutex_destroy(&counter_mutex);

    return 0;
}
