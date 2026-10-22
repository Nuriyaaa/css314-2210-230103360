
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define STUDENT_ID 230103360
#define ITEMS 1000
#define WORKERS 4

typedef struct {
    int start;
    int end;
    int dynamic;
    int chunk;
    int *next_item;
    pthread_mutex_t *mutex;
    double finish_time;
} ThreadArg;

double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

void simulate_work(int i) {
    long ns = 100000L * (i % 20);
    struct timespec req = {ns / 1000000000L, ns % 1000000000L};
    while (nanosleep(&req, &req) == -1) {}
}

void *worker(void *ptr) {
    ThreadArg *a = ptr;

    if (!a->dynamic) {
        for (int i = a->start; i < a->end; i++)
            simulate_work(i);
    } else {
        while (1) {
            pthread_mutex_lock(a->mutex);
            int start = *a->next_item;
            *a->next_item += a->chunk;
            pthread_mutex_unlock(a->mutex);

            if (start >= ITEMS) break;

            int end = start + a->chunk;
            if (end > ITEMS) end = ITEMS;

            for (int i = start; i < end; i++)
                simulate_work(i);
        }
    }

    a->finish_time = now_seconds();
    return NULL;
}

void run_test(const char *name, int dynamic, int chunk) {
    pthread_t threads[WORKERS];
    ThreadArg args[WORKERS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int next_item = 0;

    double start = now_seconds();

    for (int t = 0; t < WORKERS; t++) {
        args[t].start = t * (ITEMS / WORKERS);
        args[t].end = (t + 1) * (ITEMS / WORKERS);
        args[t].dynamic = dynamic;
        args[t].chunk = chunk;
        args[t].next_item = &next_item;
        args[t].mutex = &mutex;
        args[t].finish_time = 0;

        if (pthread_create(&threads[t], NULL, worker, &args[t]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int t = 0; t < WORKERS; t++)
        pthread_join(threads[t], NULL);

    double total = now_seconds() - start;
    double min_finish = args[0].finish_time;
    double max_finish = args[0].finish_time;

    for (int t = 1; t < WORKERS; t++) {
        if (args[t].finish_time < min_finish)
            min_finish = args[t].finish_time;
        if (args[t].finish_time > max_finish)
            max_finish = args[t].finish_time;
    }

    printf("%-24s | %12.3f ms | %16.3f ms\n",
           name, total * 1000, (max_finish - min_finish) * 1000);

    pthread_mutex_destroy(&mutex);
}

int main(void) {
    printf("=== TASK 4: Work Stealing & Scheduling Under Skew ===\n");
    printf("Student ID: %d\n", STUDENT_ID);
    printf("Items: %d | Workers: %d\n\n", ITEMS, WORKERS);

    printf("%-24s | %15s | %19s\n",
           "Strategy", "Total Time", "Completion Delta");
    printf("------------------------------------------------------------------\n");

    run_test("Static (250 items each)", 0, 1);
    run_test("Dynamic (chunk = 1)", 1, 1);
    run_test("Dynamic (chunk = 5)", 1, 5);

    printf("\nAll benchmark runs completed.\n");
    return 0;
}
