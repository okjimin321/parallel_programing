#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <ctime>

using namespace std;

int main() {
    mutex m;
    vector<thread> threads;
    int thread_size = 10;
    int iters = 10000000; // 너무 크면 오래 걸리므로 줄임

    // 단순 락 테스트 ----------------------------------------------;
    auto lock_count = [&](volatile int* counter, int id) {
        for (int i = 0; i < iters; i++) {
            lock_guard<mutex> lock(m);
            (*counter)++;
        }
    };

    clock_t start, finish;
    start = clock();

    int counter = 0;
    for (int i = 0; i < thread_size; i++) {
        threads.emplace_back(lock_count, &counter, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }
    finish = clock();
    cout << "Final counter = " << counter << endl;
    cout << "time: " << finish - start << endl;


    // 아토믹 사용 -----------------------------------------------;
    auto atomic_count = [&](volatile atomic<int>* counter, int id) {
        for (int i = 0; i < iters; i++) {

            (*counter)++;
        }
    };

    threads.clear();
    start = clock();
    atomic<int> counter_atomic(0);
    for (int i = 0; i < thread_size; i++) {
        threads.emplace_back(atomic_count, &counter_atomic, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }
    finish = clock();
    cout << "Atomic test = " << counter_atomic << endl;
    cout << "time: " << finish - start << endl;


    // 아토믹 CAS 사용 -----------------------------------------------;
    auto CAS_count = [&](atomic<int>* counter, int id) {
        for (int i = 0; i < iters; i++) {
            auto previous = counter->load();
            int desired;
            do{
                desired = previous + 1;
            }while(!counter->compare_exchange_weak(previous, desired));
        }
    };

    threads.clear();
    start = clock();
    atomic<int> CAS_atomic(0);
    for (int i = 0; i < thread_size; i++) {
        threads.emplace_back(CAS_count, &CAS_atomic, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }
    finish = clock();
    cout << "Atomic CAS test = " << CAS_atomic << endl;
    cout << "time: " << finish - start << endl;
    return 0;
}
