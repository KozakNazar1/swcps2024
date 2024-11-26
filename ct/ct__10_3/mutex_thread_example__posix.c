//to compile://     gcc -pthread -o mutex_thread_example__posix mutex_thread_example__posix.c
//    to run://     ./mutex_thread_example__posix

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// �������� ����
int counter = 0;                  // ������� ������
pthread_mutex_t mutex;            // �'����� ��� ������������

// ������� ��� ������
void* increment_counter(void* arg) {
    int id = *(int*)arg;

    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&mutex); // ���������� �'������

        // �������� ������
        printf("Thread %d: Incrementing counter (before: %d)\n", id, counter);
        counter++;
        printf("Thread %d: Counter after increment: %d\n", id, counter);

        pthread_mutex_unlock(&mutex); // ��������� �'������

        sleep(1); // ������� ������
    }

    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int thread_id1 = 1, thread_id2 = 2;

    // ����������� �'������
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Mutex initialization failed\n");
        return 1;
    }

    // ��������� ������
    pthread_create(&thread1, NULL, increment_counter, &thread_id1);
    pthread_create(&thread2, NULL, increment_counter, &thread_id2);

    // ���������� ���������� ������
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // ��������� ������� �'������
    pthread_mutex_destroy(&mutex);

    printf("Final counter value: %d\n", counter);
    return 0;
}