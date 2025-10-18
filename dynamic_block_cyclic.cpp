#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <ctime>
#include <iostream>
using namespace std;

#define N 1000
#define M 100

int main(){

    int global_lower = 0;
    int chnk = 64 / sizeof(int);
    mutex mutex;
    int arr[N][M]{};
    int results[N][N];

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            arr[i][j] = i * j;
        }
    }

    auto dynamic_block_cycle = [&]()-> void{
        while(true){
            // get lower from global
            int local_lower;
            {
                lock_guard<std::mutex> lock(mutex);
                
                mutex.lock();
                local_lower = global_lower;
                if (local_lower > N)
                    break;
                global_lower += chnk;
                mutex.unlock();
            }
            int upper = local_lower + chnk;
            for(int i = local_lower; i < upper; i++){
               for(int j = 0; j <= i; j++){
                int sum = 0;
                for(int k = 0; k < M; k++){
                    sum += arr[i][k] - arr[j][k];
                }   
                results[j][i] = results[i][j] = sum * sum;
                
               }
            }
        }
    };
    vector<thread> threads;
    clock_t start = clock();
    for (int i = 0; i < 10; i++) {
        threads.emplace_back(dynamic_block_cycle);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    clock_t finish = clock();
    cout << "time: " << finish - start << endl;
    return 0;
}