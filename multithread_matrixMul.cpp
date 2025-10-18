
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <ctime>

using namespace std;

#define NUM_THREADS 8
#define M 100000
#define N 10000

int main(){
    vector<vector<int>> A(M, vector<int>(N, 0));
    vector<int> B(N, 0);
    vector<int> C(M, 0);

    // setting array
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            A[i][j] = i*j;
        }
    }
    for(int i = 0; i < N; i++){
        B[i] = i;
    }

    // block matrix mult
    auto block = [&] (const int& id) -> void{
        int chnk = M/NUM_THREADS;
        int lower = chnk * id;
        int upper = min(M, lower + chnk);

        for(int i = lower; i < upper; i++){
            int sum = 0;
            for(int j = 0; j < N; j++){
                sum += A[i][j] * B[j];
            }
            C[i] = sum;
        }
    };

    vector<thread> threads;
    clock_t s = clock();
    for(int i = 0; i < NUM_THREADS; i++){
        threads.emplace_back(block, i);
    }
    for(auto& t: threads)
        t.join();
    clock_t e = clock();
    cout << "block: " << e - s << endl;

    // cyclic 
    threads.clear();
    auto cyclic = [&] (const int& id) -> void{
        for(int i = id; i < M; i+=NUM_THREADS){
            int sum = 0;
            for(int j = 0; j < N; j++){
                sum += A[i][j] * B[j];
            }
            C[i] = sum;
        }
    };

    s = clock();
    for(int i = 0; i < NUM_THREADS; i++){
        threads.emplace_back(cyclic, i);
    }
    for(auto& t: threads)
        t.join();
    e = clock();
    cout << "cyclic: " << e - s << endl;
    
    // block cyclic
    threads.clear();
    auto block_cyclic = [&] (const int& id) -> void{
        int chnk =  64/sizeof(int);
        int stride = chnk * NUM_THREADS;
        int lower = stride * id;

        for(int i = lower; i < M; i+= stride){
            int upper = min(M, i + chnk);
            for(int j = i; j < upper; j++){
                int sum = 0;
                for(int k = 0; k < N; k++){
                    sum += A[j][k] * B[k];
                }
                C[j] = sum;
            }
           
        }
    };

    s = clock();
    for(int i = 0; i < NUM_THREADS; i++){
        threads.emplace_back(block_cyclic, i);
    }
    for(auto& t: threads)
        t.join();
    e = clock();
    cout << "block_cyclic: " << e - s << endl;

}   