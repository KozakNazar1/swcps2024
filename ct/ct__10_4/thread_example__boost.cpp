//to compile://     g++ -o thread_example__boost thread_example__boost.cpp -lboost_thread -lboost_system -pthread
//    to run://     ./thread_example__boost

#include <boost/thread.hpp>
#include <iostream>

// �������, ��� ��������� ������
void thread_function(int thread_id) {
    std::cout << "�����! � ���� ����� " << thread_id << std::endl;
}

int main() {
    const int NUM_THREADS = 5; // ʳ������ ������
    boost::thread threads[NUM_THREADS];

    // ��������� ������
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads[i] = boost::thread(thread_function, i + 1);
    }

    // ������ ���������� ��� ������
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads[i].join();
    }

    std::cout << "�� ������ ��������." << std::endl;
    return 0;
}