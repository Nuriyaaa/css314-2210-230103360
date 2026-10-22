
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define STUDENT_ID 230103360
#define TOTAL_POINTS 40000000LL
#define THREAD_COUNT 4
#define POINTS_PER_THREAD (TOTAL_POINTS / THREAD_COUNT)

#define PI_REFERENCE 3.14159265358979323846

static pthread_mutex_t prng_mutex = PTHREAD_MUTEX_INITIALIZER;
static uint64_t shared_state = STUDENT_ID;

typedef struct {
    int thread_index;
    long long points;
    long long inside;
    int independent;
} ThreadData;

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
    return x * UINT64_C(2685821657736338717);
}

static double random_unit(uint64_t *state) {
    return (double)(next_random(state) >> 11) / 9007199254740992.0;
}

static void *monte_carlo(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    long long inside = 0;

    uint64_t local_state =
        ((uint64_t)STUDENT_ID * UINT64_C(10)) +
        (uint64_t)data->thread_index;

    if (local_state == 0) {
        local_state = 1;
    }

    for (long long i = 0; i < data->points; i++) {
        double x;
        double y;

        if (data->independent) {
            x = random_unit(&local_state);
            y = random_unit(&local_state);
        } else {
            pthread_mutex_lock(&prng_mutex);
            x = random_unit(&shared_state);
            y = random_unit(&shared_state);
            pthread_mutex_unlock(&prng_mutex);
        }

        if (x * x + y * y <= 1.0) {
            inside++;
        }
    }

    data->inside = inside;
    return NULL;
}

static double run_simulation(int independent, double *pi, double *error) {
    pthread_t threads[THREAD_COUNT];
    ThreadData data[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        data[i].thread_index = i;
        data[i].points = POINTS_PER_THREAD;
        data[i].inside = 0;
        data[i].independent = independent;
    }

    double start = now_seconds();

    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&threads[i], NULL, monte_carlo, &data[i]) != 0) {
            fprintf(stderr, "Thread creation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    long long total_inside = 0;

    for (int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Thread join failed.\n");
            exit(EXIT_FAILURE);
        }
        total_inside += data[i].inside;
    }

    double elapsed = now_seconds() - start;

    *pi = 4.0 * (double)total_inside / (double)TOTAL_POINTS;
    *error = fabs(*pi - PI_REFERENCE);

    return elapsed * 1000.0;
}

int main(void) {
    double shared_pi;
    double shared_error;
    double independent_pi;
    double independent_error;

    printf("=== TASK 2: PRNG STATE BOTTLENECK ===\n");
    printf("Student ID: %d\n", STUDENT_ID);
    printf("Total sample points: %lld\n", TOTAL_POINTS);
    printf("Threads: %d\n", THREAD_COUNT);

    double shared_ms = run_simulation(0, &shared_pi, &shared_error);

    shared_state = STUDENT_ID;

    double independent_ms =
        run_simulation(1, &independent_pi, &independent_error);

    printf("\nShared PRNG:\n");
    printf("Computed pi: %.12f\n", shared_pi);
    printf("Absolute error from Math.PI: %.12f\n", shared_error);
    printf("Wall time: %.3f ms\n", shared_ms);

    printf("\nIndependent PRNG:\n");
    printf("Computed pi: %.12f\n", independent_pi);
    printf("Absolute error from Math.PI: %.12f\n", independent_error);
    printf("Wall time: %.3f ms\n", independent_ms);

    if (independent_ms > 0.0) {
        printf("\nSpeedup factor: %.3fx\n", shared_ms / independent_ms);
    } else {
        printf("\nSpeedup factor: undefined\n");
    }

    return EXIT_SUCCESS;
}
