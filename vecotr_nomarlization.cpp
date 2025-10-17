#include<iostream>
#include <emmintrin.h>  
#include<cmath>
#include <ctime>
using namespace std;

void AoS_normal(float* xyz, int size){
    for (int i = 0; i < size; i += 3){
        int x = xyz[i + 0];
        int y = xyz[i + 1];
        int z = xyz[i + 2];
        float irho = 1.0/(sqrt(x*x + y*y + z*z));

        xyz[i + 0] *= irho;
        xyz[i + 1] *= irho;
        xyz[i + 2] *= irho;
    }
}

void SoA_normal(float* x, float* y, float* z, int size){
    for(int i = 0; i < size; i += 4){
        __m128 a = _mm_load_ps(x + i);
        __m128 b = _mm_load_ps(y + i);
        __m128 c = _mm_load_ps(z + i);
        
        __m128 R = (_mm_add_ps(_mm_mul_ps(a,a), _mm_mul_ps(b, b)), _mm_add_ps(_mm_mul_ps(c,c), R));
        
        R = _mm_rsqrt_ps(R);

    }   
}

int main(){
    const int size = 1200000000;

    
    // setting arr for AoS
    float* xyz = new float[size]{};
    for(int i = 0; i < size; i++){
        xyz[i] = i;
    }

    // setting arr for SoA
    float* x = new float[size]{};
    float* y = new float[size]{};
    float* z = new float[size]{};
    for(int i = 0; i < size/3; i++){
        x[i] = i * 1.0;
        y[i] = i * 2.0;
        z[i] = i * 3.0;
    }

    // count AoS execution time
    clock_t s = clock();
    AoS_normal(xyz, size);
    clock_t e = clock();
    cout << "AoS time: " << e - s << endl;

    // count SoA execution time
    s = clock();
    SoA_normal(x,y,z, size/3);
    e = clock();
    cout << "SoA time: " << e - s << endl;
}
