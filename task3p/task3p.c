
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>

#define STUDENT_ID 230103360
#define N 2000000
#define THREAD_COUNT 4

static int *input;
static long long *serial_output;
static long long *parallel_output;
static long long chunk_sums[THREAD_COUNT];
static long long offsets[THREAD_COUNT];

typedef struct {
    int id;
    int start;
    int end;
    int phase;
} Task;

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}

static uint64_t next_random(uint64_t *state) {
    uint64_t x = *state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    *state = x;
    return x;
}

static void *scan_worker(void *arg) {
    Task *task = (Task *)arg;
    int start = task->start;
    int end = task->end;

    if (task->phase == 0) {
        long long sum = 0;

        for (int i = start; i < end; i++) {
            sum += input[i];
            parallel_output[i] = sum;
        }

        chunk_sums[task->id] = sum;
    } else {
        long long offset = offsets[task->id];

        for (int i = start; i < end; i++) {
            parallel_output[i] += offset;
        }
    }

    return NULL;
}

static int run_phase(int phase) {
    pthread_t threads[THREAD_COUNT];
    Task tasks[THREAD_COUNT];
    int created = 0;

    for (int t = 0; t < THREAD_COUNT; t++) {
        tasks[t].id = t;
        tasks[t].start = (int)((long long)N * t / THREAD_COUNT);
        tasks[t].end = (int)((long long)N * (t + 1) / THREAD_COUNT);
        tasks[t].phase = phase;

        if (pthread_create(&threads[t], NULL, scan_worker, &tasks[t]) != 0) {
            fprintf(stderr, "Thread creation failed.\n");

            for (int j = 0; j < created; j++) {
                pthread_join(threads[j], NULL);
            }

            return 1;
        }

        created++;
    }

    for (int t = 0; t < THREAD_COUNT; t++) {
        if (pthread_join(threads[t], NULL) != 0) {
            fprintf(stderr, "Thread join failed.\n");
            return 1;
        }
    }

    return 0;
}

int main(void) {
    input = malloc((size_t)N * sizeof(int));
    serial_output = malloc((size_t)N * sizeof(long long));
    parallel_output = malloc((size_t)N * sizeof(long long));

    if (!input || !serial_output || !parallel_output) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(input);
        free(serial_output);
        free(parallel_output);
        return 1;
    }

    uint64_t state = STUDENT_ID;

    if (state == 0) {
        state = 1;
    }

    for (int i = 0; i < N; i++) {
        input[i] = (int)(next_random(&state) % 10) + 1;
    }

    double serial_start = now_seconds();

    serial_output[0] = input[0];

    for (int i = 1; i < N; i++) {
        serial_output[i] = serial_output[i - 1] + input[i];
    }

    double serial_ms = (now_seconds() - serial_start) * 1000.0;

    double parallel_start = now_seconds();

    if (run_phase(0) != 0) {
        free(input);
        free(serial_output);
        free(parallel_output);
        return 1;
    }

    offsets[0] = 0;

    for (int t = 1; t < THREAD_COUNT; t++) {
        offsets[t] = offsets[t - 1] + chunk_sums[t - 1];
    }

    if (run_phase(1) != 0) {
        free(input);
        free(serial_output);
        free(parallel_output);
        return 1;
    }

    double parallel_ms = (now_seconds() - parallel_start) * 1000.0;

    printf("=== TASK 3: PREFIX SCAN VERIFICATION ===\n");
    printf("Student ID: %d\n", STUDENT_ID);
    printf("Array size: %d\n", N);
    printf("Threads: %d\n", THREAD_COUNT);
    printf("Serial Time: %.3f ms\n", serial_ms);
    printf("Parallel Time: %.3f ms\n", parallel_ms);

    if (parallel_ms > 0.0) {
        printf("Speedup: %.3fx\n", serial_ms / parallel_ms);
    } else {
        printf("Speedup: undefined\n");
    }

    if (parallel_output[N - 1] != serial_output[N - 1]) {
        printf("Verification Status: FAIL\n");
        free(input);
        free(serial_output);
        free(parallel_output);
        return 1;
    }

    assert(parallel_output[N - 1] == serial_output[N - 1]);

    printf("Verification Status: PASS\n");
    printf("Serial final sum: %lld\n", serial_output[N - 1]);
    printf("Parallel final sum: %lld\n", parallel_output[N - 1]);

    free(input);
    free(serial_output);
    free(parallel_output);

    return 0;
}
