#include<stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <ctime>
#include <emmintrin.h>  
using namespace std;

#define M 2000
#define L 1000
#define N 5000

float a[M][L];
float b[L][N];
float c[M][N];
float b_t [N][L];

void matrix_mul(float a[M][L], float b[L][N], float c[M][N]){
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            float tmp = 0;
            for(int k = 0; k < L; k++){
                tmp += (a[i][k] * b[k][j]);
            }
            c[i][j] = tmp;
        }
    }
}

void t_mul(float a[M][L], float b[L][N], float c[M][N]){
    // transpose matrix b
    for(int i = 0; i < N; i++){
        for(int j = 0; j < L; j++){
            b_t[i][j] = b[j][i];
        }
    }

    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            float tmp = 0;
            for(int k = 0; k < L; k++){
                tmp += (a[i][k] * b_t[j][k]);
            }
            c[i][j] = tmp;
        }
    }
}

void intrinsic_mul(float a[M][L], float b[L][N], float c[M][N]){
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            __m128 tmp = _mm_setzero_ps();
            for(int k = 0; k < L; k += 4){ //  128byte이므로 float 4개씩 가져올 수 있음.
                __m128 x = _mm_load_ps(&a[i][k]);
                __m128 y = _mm_load_ps(&b_t[j][k]);
                tmp = _mm_add_ps(_mm_mul_ps(x, y), tmp);
            }

            c[i][j]  = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        }
    }
}


int main(){
    // setting arr
    int cnt = 0;
    for(int i = 0; i < M; i++){
        for(int j = 0; j < L; j++){
            a[i][j] = cnt++;
        }
    }
    for(int i = 0; i < L; i++){
        for(int j = 0; j < N; j++){
            b[i][j] = cnt--;
        }
    }

    // 1. count basic model execution time
    clock_t s = clock();
    matrix_mul(a,b,c);
    clock_t e = clock();
    cout << "basic: ";
    cout << e - s << endl;


    // 2. count transposed model execution time
    clock_t s2 = clock();
    t_mul(a,b,c);
    clock_t e2 = clock();
    cout << "transposed: ";
    cout << e2 - s2 << endl;

    // 3. count intrinsic model execution time
    clock_t s3 = clock();
    intrinsic_mul(a,b,c);
    clock_t e3 = clock();
    cout << "intrinsic: ";
    cout << e3 - s3 << endl;
}