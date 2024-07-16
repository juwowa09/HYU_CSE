#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define n 5

int shared_resource = 0;
int number[n];
int flag[n];

#define NUM_ITERS 20
#define NUM_THREADS 5

void lock();
void unlock();

void lock(int tid)
{
    flag[tid] = 1;
    int max_number = 0;
    for (int i = 0; i < n; i++) {
        if (number[i] > max_number) {
            max_number = number[i];
        }
    }
    number[tid] = max_number + 1;
    flag[tid] = 0;

    for (int i = 0; i < n; i++) {
        while (flag[i]) { /* busy-wait */ }
        while (number[i] != 0 &&
            (number[i] < number[tid] ||
            (number[i] == number[tid] && i < tid))) { /* busy-wait */ }
    }
}

void unlock(int tid)
{
    number[tid] = 0;
}

void* thread_func(void* arg) {
    int tid = *(int*)arg;

    lock(tid);

    for(int i = 0; i < NUM_ITERS; i++)  {

        usleep(100000);
            shared_resource++;
    }
    // critical section
    printf("tid : %d , shared_resource : %d\n",tid, shared_resource);
    unlock(tid);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[n];
    int tids[n];

    // Initialize flag and number arrays
    for (int i = 0; i < n; i++) {
        flag[i] = 0;
        number[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &tids[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("shared: %d\n", shared_resource);

    return 0;
}
