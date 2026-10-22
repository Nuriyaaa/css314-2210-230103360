
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#define STUDENT_ID 230103360
#define REPEATS 5

typedef struct {
    const double *x;
    double *y;
    double a;
    double b;
    size_t start;
    size_t end;
} Task;

double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

void *vector_worker(void *ptr) {
    Task *t = (Task *)ptr;
    for (size_t i = t->start; i < t->end; i++) {
        t->y[i] = t->a * t->x[i] + t->b;
    }
    return NULL;
}

double serial_run(const double *x, double *y, size_t n, double a, double b) {
    double start = now_seconds();
    for (size_t i = 0; i < n; i++) {
        y[i] = a * x[i] + b;
    }
    return now_seconds() - start;
}

double parallel_run(const double *x, double *y, size_t n, double a, double b, int workers) {
    pthread_t *threads = malloc((size_t)workers * sizeof(pthread_t));
    Task *tasks = malloc((size_t)workers * sizeof(Task));

    if (!threads || !tasks) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    double start = now_seconds();

    for (int i = 0; i < workers; i++) {
        tasks[i].x = x;
        tasks[i].y = y;
        tasks[i].a = a;
        tasks[i].b = b;
        tasks[i].start = n * (size_t)i / (size_t)workers;
        tasks[i].end = n * (size_t)(i + 1) / (size_t)workers;

        if (pthread_create(&threads[i], NULL, vector_worker, &tasks[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < workers; i++) {
        pthread_join(threads[i], NULL);
    }

    double elapsed = now_seconds() - start;
    free(threads);
    free(tasks);
    return elapsed;
}

int compare_double(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
}

double median(double values[REPEATS]) {
    qsort(values, REPEATS, sizeof(double), compare_double);
    return values[REPEATS / 2];
}

int main(void) {
    size_t sizes[] = {100, 1000, 10000, 100000, 1000000, 10000000};
    size_t count = sizeof(sizes) / sizeof(sizes[0]);
    long detected = sysconf(_SC_NPROCESSORS_ONLN);
    int workers = detected > 0 ? (int)detected : 1;

    if (workers > 256) workers = 256;

    double a = 2.5;
    double b = 1.75;
    int previous_state = -1;
    size_t previous_n = 0;
    int crossover_found = 0;
    double checksum = 0.0;

    printf("=== TASK 5: The Overhead Crossover Point ===\n");
    printf("Student ID: %d\n", STUDENT_ID);
    printf("Logical CPU cores: %d\n", workers);
    printf("Operation: y[i] = a * x[i] + b\n");
    printf("Timing statistic: median of %d runs\n\n", REPEATS);
    printf("%12s | %15s | %15s | %10s\n",
           "N", "Serial (ms)", "Parallel (ms)", "Faster");
    printf("------------------------------------------------------------\n");

    for (size_t k = 0; k < count; k++) {
        size_t n = sizes[k];
        double *x = malloc(n * sizeof(double));
        double *y = malloc(n * sizeof(double));

        if (!x || !y) {
            fprintf(stderr, "Memory allocation failed for N=%zu\n", n);
            free(x);
            free(y);
            return EXIT_FAILURE;
        }

        for (size_t i = 0; i < n; i++) {
            x[i] = (double)((i % 1000) + 1) / 1000.0;
        }

        double serial_times[REPEATS];
        double parallel_times[REPEATS];

        for (int r = 0; r < REPEATS; r++) {
            serial_times[r] = serial_run(x, y, n, a, b);
            parallel_times[r] = parallel_run(x, y, n, a, b, workers);
        }

        double serial_ms = median(serial_times) * 1000.0;
        double parallel_ms = median(parallel_times) * 1000.0;
        int state = parallel_ms < serial_ms ? 1 : 0;

        printf("%12zu | %15.3f | %15.3f | %10s\n",
               n, serial_ms, parallel_ms,
               state ? "Parallel" : "Serial");

        checksum += y[n - 1];

        if (previous_state == 0 && state == 1 && !crossover_found) {
            printf("CROSSOVER RANGE: N > %zu and N <= %zu\n",
                   previous_n, n);
            crossover_found = 1;
        }

        previous_state = state;
        previous_n = n;

        free(x);
        free(y);
    }

    if (!crossover_found) {
        printf("CROSSOVER RANGE: No slower-to-faster transition observed "
               "among tested sizes.\n");
    }

    printf("Verification checksum: %.6f\n", checksum);
    printf("Benchmark completed successfully.\n");

    return 0;
}
