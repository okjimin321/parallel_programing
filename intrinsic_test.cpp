#include<stdio.h>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <ctime>
#include<future>
#include <emmintrin.h>  

#define SIZE 10000000
using namespace std;


float a[SIZE + 1]{};
float b[SIZE + 1]{};
float c[SIZE + 1]{};
float d[SIZE + 1]{};
int main(){

    // setting array
    for(int i = 0; i <= SIZE; i++){
        a[i] = i;
        b[i] = i;
    }

    // sequential
    clock_t s = clock();
    for(int i = 0; i <= SIZE; i++){
        c[i] = a[i] + b[i];
    }
    clock_t e = clock();
    cout << "seq time: " << e - s << endl;
    
    //intrinsic
    clock_t s1 = clock();
    __m128 x, y, z;
    z = _mm_setzero_ps();
    for(int i = 0; i <= SIZE; i+= 4){
        x = _mm_load_ps(a + i);
        y = _mm_load_ps(b + i);
        z = _mm_add_ps(x,y);

        _mm_store_ps(d + i, z);
    }
    clock_t e1 = clock();
    // for (int i = 0; i <= SIZE; i++){
    //     cout << d[i] << " ";
    // }
    cout << endl;
    cout << "intri time: " << e1 - s1 << endl;
}