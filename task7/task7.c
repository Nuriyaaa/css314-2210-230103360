
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#define STUDENT_ID 230103360
#define N 30000000LL

typedef struct {
    const double *a;
    double *b;
    long long start;
    long long end;
    int kernel;
} Task;

volatile double checksum = 0.0;

double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

void *run_kernel(void *ptr) {
    Task *t = (Task *)ptr;

    if (t->kernel == 0) {
        for (long long i = t->start; i < t->end; i++)
            t->b[i] = t->a[i];
    } else {
        for (long long i = t->start; i < t->end; i++)
            t->b[i] = sin(t->a[i]) * cos(t->a[i]) + tan(t->a[i]);
    }

    return NULL;
}

double execute(const double *a, double *b, int cores, int kernel) {
    pthread_t *threads = malloc((size_t)cores * sizeof(pthread_t));
    Task *tasks = malloc((size_t)cores * sizeof(Task));

    if (!threads || !tasks) {
        fprintf(stderr, "Thread allocation failed\n");
        exit(EXIT_FAILURE);
    }

    double start = now_seconds();

    for (int i = 0; i < cores; i++) {
        tasks[i].a = a;
        tasks[i].b = b;
        tasks[i].start = N * i / cores;
        tasks[i].end = N * (i + 1) / cores;
        tasks[i].kernel = kernel;

        if (pthread_create(&threads[i], NULL, run_kernel, &tasks[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < cores; i++)
        pthread_join(threads[i], NULL);

    double elapsed = now_seconds() - start;

    checksum += b[N / 2] + b[N - 1];

    free(threads);
    free(tasks);
    return elapsed;
}

int main(void) {
    long detected = sysconf(_SC_NPROCESSORS_ONLN);
    int max_cores = detected > 0 ? (int)detected : 1;

    if (max_cores > 256) max_cores = 256;

    double *a = malloc((size_t)N * sizeof(double));
    double *b = malloc((size_t)N * sizeof(double));

    if (!a || !b) {
        fprintf(stderr, "Array allocation failed. Required memory: approximately 480 MB.\n");
        free(a);
        free(b);
        return EXIT_FAILURE;
    }

    for (long long i = 0; i < N; i++)
        a[i] = (double)(i % 1000) / 1000.0;

    int core_counts[3] = {1, 2, max_cores};
    double times[2][3];
    double speedups[2][3];
    double efficiencies[2][3];

    printf("=== TASK 7: Arithmetic Intensity vs. Memory Bottlenecks ===\n");
    printf("Student ID: %d\n", STUDENT_ID);
    printf("Array elements: %lld\n", N);
    printf("Array type: float64\n");
    printf("Logical CPU cores: %d\n", max_cores);
    printf("Kernels: A = vector copy; B = sin(A)*cos(A)+tan(A)\n\n");

    for (int k = 0; k < 2; k++) {
        printf("Kernel %c:\n", k == 0 ? 'A' : 'B');
        printf("%8s | %15s | %12s | %12s\n",
               "Cores", "Time (ms)", "Speedup", "Efficiency");
        printf("---------------------------------------------------------\n");

        for (int c = 0; c < 3; c++) {
            int cores = core_counts[c];
            times[k][c] = execute(a, b, cores, k);

            if (c == 0) {
                speedups[k][c] = 1.0;
                efficiencies[k][c] = 1.0;
            } else {
                speedups[k][c] = times[k][0] / times[k][c];
                efficiencies[k][c] = speedups[k][c] / cores;
            }

            printf("%8d | %15.3f | %12.3f | %11.1f%%\n",
                   cores, times[k][c] * 1000.0,
                   speedups[k][c], efficiencies[k][c] * 100.0);
        }
        printf("\n");
    }

    printf("Memory bandwidth analysis:\n");

    double copy_eff = efficiencies[0][2];
    double compute_eff = efficiencies[1][2];
    double copy_speedup = speedups[0][2];
    double compute_speedup = speedups[1][2];

    if (max_cores > 2 &&
        copy_eff < compute_eff * 0.8 &&
        copy_speedup < compute_speedup) {
        printf("Kernel A (Memory-Bound) shows stronger scaling saturation, "
               "consistent with memory channel bandwidth limitation.\n");
    } else if (max_cores > 2 && copy_speedup < 1.5) {
        printf("Kernel A (Memory-Bound) shows limited speedup, "
               "consistent with memory bandwidth saturation.\n");
    } else {
        printf("Kernel A is the memory-bandwidth-bound candidate. "
               "These measurements do not conclusively establish "
               "memory channel saturation; compare scaling efficiencies.\n");
    }

    printf("Kernel A max-core efficiency: %.1f%%\n", copy_eff * 100.0);
    printf("Kernel B max-core efficiency: %.1f%%\n", compute_eff * 100.0);
    printf("Verification checksum: %.10f\n", checksum);
    printf("Benchmark completed successfully.\n");

    free(a);
    free(b);
    return 0;
}
