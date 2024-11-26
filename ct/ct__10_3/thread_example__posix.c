//to compile://     gcc -pthread -o thread_example__posix thread_example__posix.c
//    to run://     ./thread_example__posix

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// �������, ��� ��������� ������
void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    printf("�����! � ���� ����� %d\n", thread_id);
    return NULL;
}

int main() {
    const int NUM_THREADS = 5; // ʳ������ ������
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        // ��������� ����� ����
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
            perror("�� ������� �������� ����");
            exit(EXIT_FAILURE);
        }
    }

    // ������ ���������� ��� ������
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("�� ������� �������� ����");
            exit(EXIT_FAILURE);
        }
    }

    printf("�� ������ ��������.\n");
    return 0;
}