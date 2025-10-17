#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <ctime>
#include<future>

using namespace std;
/*
이 경우처럼 병렬화 하는 cost가 큰 경우에는 
sequential하게 하는 것이 성능이 더 잘 나올 수도 있다.
*/
void fibo2(int id, promise<int>&& result){
    int a0 = 0;
    int a1 = 1;

    for(int i = 0; i <= id; i++){
        int tmp = a1;
        a1 += a0;
        a0 = tmp;
    }

    result.set_value(a0); // set을 통해 future에게 전달.
}

void promise_future_fibo(){
    int thread_size = 32;
    vector<thread> threads;
    vector<future<int>> results;

    for(int i = 0; i < thread_size; i++){
        promise<int> promise; // 지역변수.
        results.emplace_back(promise.get_future()); // future를 얻어오고.
        threads.emplace_back(fibo2, i, move(promise));// promise 지역변수 소유권을 해당 함수로 양도.
    }

    for (auto& re: results){
        cout << re.get() << " ";
    }

    for (auto& t : threads)// 
        t.join();
    cout << endl;

}

void fibo(int id, int* result){
    int a0 = 0;
    int a1 = 1;

    for(int i = 0; i <= id; i++){
        int tmp = a1;
        a1 += a0;
        a0 = tmp;
    }

    *result = a0;
}

void paral_fibo(){
    int thread_size = 32;
    vector<int> results(thread_size, 0);
    vector<thread>threads;

    clock_t start = clock();
    for(int i = 0; i < thread_size; i++){
        threads.emplace_back(fibo, i, &results[i]);
    }

    for(auto& thread: threads)
        thread.join();
    clock_t end = clock();

    for(auto element: results)
        cout << element << " ";
    cout << endl << "time: " << end - start << endl;
    
}

void seq_fibo(){

    vector<int> results(32, 0);
    
    clock_t start = clock();

    int a0 = 0;
    int a1 = 1;

    for(int i = 0; i <= 32; i++){
        int tmp = a1;
        a1 += a0;
        a0 = tmp;

        results[i] = a0;
    }

    clock_t end = clock();

    for(auto element: results)
        cout << element << " ";
    cout << endl << "time: " << end - start << endl;
}

int main(){

    promise_future_fibo();
    //paral_fibo();
    //seq_fibo();
}