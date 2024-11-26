//to compile://     g++ -o thread_example__boost thread_example__boost.cpp -lboost_thread -lboost_system -pthread
//    to run://     ./thread_example__boost

#include <boost/thread.hpp>
#include <iostream>

// Функція, яку виконують потоки
void thread_function(int thread_id) {
    std::cout << "Привіт! Я потік номер " << thread_id << std::endl;
}

int main() {
    const int NUM_THREADS = 5; // Кількість потоків
    boost::thread threads[NUM_THREADS];

    // Створюємо потоки
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads[i] = boost::thread(thread_function, i + 1);
    }

    // Чекаємо завершення всіх потоків
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads[i].join();
    }

    std::cout << "Усі потоки завершені." << std::endl;
    return 0;
}