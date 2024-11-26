//to compile://     gcc -pthread -o thread_example__posix thread_example__posix.c
//    to run://     ./thread_example__posix

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Функція, яку виконують потоки
void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    printf("Привіт! Я потік номер %d\n", thread_id);
    return NULL;
}

int main() {
    const int NUM_THREADS = 5; // Кількість потоків
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        // Створюємо новий потік
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
            perror("Не вдалося створити потік");
            exit(EXIT_FAILURE);
        }
    }

    // Чекаємо завершення всіх потоків
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Не вдалося приєднати потік");
            exit(EXIT_FAILURE);
        }
    }

    printf("Усі потоки завершені.\n");
    return 0;
}