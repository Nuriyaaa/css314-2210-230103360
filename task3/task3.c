#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define STUDENT_ID 230103360
#define ITERATIONS 50000000LL

typedef struct {
    volatile long long count;
} SharedCounter;

typedef struct {
    volatile long long count;
    char padding[64 - sizeof(long long)];
} PaddedCounter;

typedef struct {
    int id;
    int padded;
    SharedCounter *shared;
    PaddedCounter *padded_counts;
} ThreadArg;

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static void *worker(void *arg) {
    ThreadArg *a = (ThreadArg *)arg;

    if (a->padded) {
        long long local_count = 0;

        for (long long i = 0; i < ITERATIONS; i++) {
            local_count++;
        }

        a->padded_counts[a->id].count = local_count;
    } else {
        for (long long i = 0; i < ITERATIONS; i++) {
            a->shared[a->id].count++;
        }
    }

    return NULL;
}

static double run_test(int n, int padded) {
    pthread_t *threads = calloc(n, sizeof(pthread_t));
    ThreadArg *args = calloc(n, sizeof(ThreadArg));
    SharedCounter *shared = calloc(n, sizeof(SharedCounter));
    PaddedCounter *padded_counts = NULL;

    if (posix_memalign((void **)&padded_counts, 64,
                       n * sizeof(PaddedCounter)) != 0) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        shared[i].count = 0;
        padded_counts[i].count = 0;
    }

    double start = now_seconds();

    for (int i = 0; i < n; i++) {
        args[i].id = i;
        args[i].padded = padded;
        args[i].shared = shared;
        args[i].padded_counts = padded_counts;

        if (pthread_create(&threads[i], NULL, worker, &args[i]) != 0) {
            fprintf(stderr, "Thread creation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    long long total = 0;

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
        total += padded ? padded_counts[i].count : shared[i].count;
    }

    double elapsed = now_seconds() - start;

    if (total != (long long)n * ITERATIONS) {
        fprintf(stderr, "Count verification failed: %lld\n", total);
        exit(EXIT_FAILURE);
    }

    free(threads);
    free(args);
    free(shared);
    free(padded_counts);

    return elapsed;
}

int main(void) {
    long n = 0;

#ifdef __APPLE__
    FILE *fp = popen("sysctl -n hw.physicalcpu", "r");
    if (fp != NULL) {
        if (fscanf(fp, "%ld", &n) != 1) n = 0;
        pclose(fp);
    }
#endif

    if (n <= 0) {
        long online = sysconf(_SC_NPROCESSORS_ONLN);
        n = online > 0 ? online : 1;
    }

    printf("=== TASK 3: Cache Line Contention ===\n");
    printf("Student ID: %d\n", STUDENT_ID);
    printf("Physical CPU cores / worker threads: %ld\n", n);
    printf("Increments per thread: %lld\n\n", ITERATIONS);

    double unpadded = run_test((int)n, 0);
    double local = run_test((int)n, 1);

    printf("Unpadded Shared: %.3f ms\n", unpadded * 1000);
    printf("Local Variable:  %.3f ms\n", local * 1000);
    printf("Execution Ratio (Unpadded / Local): %.3fx\n",
           local > 0 ? unpadded / local : 0);

    printf("\nCount verification: PASSED\n");
    return 0;
}
