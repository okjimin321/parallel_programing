#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include<cmath>
#include <mutex>
#include <condition_variable>
using namespace std;

const int size = 8;
int completed = 0;
condition_variable cv;

mutex m;

void add_arr(int id, int* arr){
    
    for(int i = 0; i < log2(size); i++){
        if(id >= pow(2, i))
            arr[id] += arr[id - pow(2, i)];
    
        
        unique_lock<mutex> lock(m);
        completed++;
        while(completed != 8){
            cv.wait(lock, [&](){return completed == 0});
        }
        cv.notify_all();
        completed = 0;
        
    }

    
}


int main(){
    const int size = 8;
    int arr[size] = {4,3,5,8,2,6,3,1};
    vector<thread> threads;

    // setting thread
    for(int i = 0; i < size; i++){
        threads.emplace_back(add_arr, &arr);
    }

    for(auto& t: threads){
        t.join();
    }

    // results
    for(const auto& r: result)
        cout << r << endl;
    
}