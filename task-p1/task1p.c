
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define N 8000
#define STUDENT_ID 230103360

static double *matrix;
static int workers;
static double *partial_sums;

typedef struct {
    int id;
    int start;
    int end;
    int mode;
} Task;

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

static uint64_t next_random(uint64_t *state) {
    uint64_t x = *state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    *state = x;
    return x;
}

static void *sum_matrix(void *arg) {
    Task *task = (Task *)arg;
    double sum = 0.0;

    if (task->mode == 0) {
        for (int i = task->start; i < task->end; i++) {
            double *row = matrix + (size_t)i * N;
            for (int j = 0; j < N; j++) {
                sum += row[j];
            }
        }
    } else {
        for (int j = task->start; j < task->end; j++) {
            for (int i = 0; i < N; i++) {
                sum += matrix[(size_t)i * N + j];
            }
        }
    }

    partial_sums[task->id] = sum;
    return NULL;
}

static double run_mode(int mode) {
    pthread_t *threads = malloc((size_t)workers * sizeof(pthread_t));
    Task *tasks = malloc((size_t)workers * sizeof(Task));

    if (!threads || !tasks) {
        fprintf(stderr, "Thread allocation failed.\n");
        free(threads);
        free(tasks);
        exit(EXIT_FAILURE);
    }

    for (int t = 0; t < workers; t++) {
        int limit = N;
        tasks[t].id = t;
        tasks[t].start = (int)((long long)limit * t / workers);
        tasks[t].end = (int)((long long)limit * (t + 1) / workers);
        tasks[t].mode = mode;
    }

    double start = now_seconds();

    for (int t = 0; t < workers; t++) {
        if (pthread_create(&threads[t], NULL, sum_matrix, &tasks[t]) != 0) {
            fprintf(stderr, "Thread creation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int t = 0; t < workers; t++) {
        pthread_join(threads[t], NULL);
    }

    double total = 0.0;
    for (int t = 0; t < workers; t++) {
        total += partial_sums[t];
    }

    double elapsed = now_seconds() - start;

    printf("Mode %c sum: %.10f\n", mode == 0 ? 'A' : 'B', total);

    free(threads);
    free(tasks);

    return elapsed * 1000.0;
}

int main(void) {
    long logical_cores = sysconf(_SC_NPROCESSORS_ONLN);
    long physical_cores = 0;

    FILE *cpuinfo = popen("sysctl -n hw.physicalcpu", "r");
    if (cpuinfo) {
        if (fscanf(cpuinfo, "%ld", &physical_cores) != 1) {
            physical_cores = 0;
        }
        pclose(cpuinfo);
    }

    if (physical_cores < 1) {
        physical_cores = logical_cores > 0 ? logical_cores : 1;
    }

    if (logical_cores < 1) {
        logical_cores = physical_cores;
    }

    workers = (int)physical_cores;

    matrix = malloc((size_t)N * N * sizeof(double));
    partial_sums = calloc((size_t)workers, sizeof(double));

    if (!matrix || !partial_sums) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(matrix);
        free(partial_sums);
        return EXIT_FAILURE;
    }

    uint64_t state = STUDENT_ID;
    if (state == 0) {
        state = 1;
    }

    for (size_t i = 0; i < (size_t)N * N; i++) {
        matrix[i] = (double)(next_random(&state) >> 11) /
                    9007199254740992.0;
    }

    printf("=== PRACTICE TASK 1: ROW-MAJOR VS COLUMN-MAJOR ===\n");
    printf("Student ID: %d\n", STUDENT_ID);
    printf("Matrix size: %d x %d\n", N, N);
    printf("Logical cores: %ld\n", logical_cores);
    printf("Physical cores / workers: %d\n", workers);

    double mode_a_ms = run_mode(0);
    double mode_b_ms = run_mode(1);

    printf("Mode A (Row-Major): %.3f ms\n", mode_a_ms);
    printf("Mode B (Column-Major / Strided): %.3f ms\n", mode_b_ms);

    if (mode_a_ms > 0.0) {
        printf("Cache penalty factor: %.3fx\n", mode_b_ms / mode_a_ms);
    } else {
        printf("Cache penalty factor: undefined\n");
    }

    free(matrix);
    free(partial_sums);

    return EXIT_SUCCESS;
}
