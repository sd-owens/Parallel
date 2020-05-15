#include <iostream>
#include <cmath>
#include <omp.h>
#include <xmmintrin.h>
#define SSE_WIDTH		        4
#define NUMTRIES                10
#define NUM_ELEMENTS_PER_CORE   WIDTH / NUMT

#ifndef NUMT
#define NUMT                    4
#endif
#ifndef WIDTH
#define WIDTH                   1024 * 1024 * 8
#endif

using namespace std;

void printArray(float *arr, int len) {

    for(int i = 0; i < len; i++) {

        cout << arr[i] << " ";
    }
    cout << endl;
}

float MyMultSum(float *a, float *b, int len) {

    float sum = 0.;

    for(int i = 0; i < len; i++){
        sum += a[i] * b[i];
    }
    return sum;
}

float SimdMulSum( float *a, float *b, int len ){

    float sum[4] = { 0., 0., 0., 0. };
    int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
    register float *pa = a;
    register float *pb = b;

    __m128 ss = _mm_loadu_ps( &sum[0] );
    for( int i = 0; i < limit; i += SSE_WIDTH ){
        ss = _mm_add_ps( ss, _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
        pa += SSE_WIDTH;
        pb += SSE_WIDTH;
    }
    _mm_storeu_ps( &sum[0], ss );

    for( int i = limit; i < len; i++ ){
        sum[0] += a[i] * b[i];
    }

    return sum[0] + sum[1] + sum[2] + sum[3];
}



int main() {

#ifndef _OPENMP
    fprintf( stderr, "No OpenMP support!\n" );
    return 1;
#endif

    double maxPerformanceSIMD = 0., maxPerformanceNOSIMD = 0., maxPerformanceSIMDOPENMP;

    for (int t = 0; t < NUMTRIES; t++) {

        auto *A = new float[WIDTH];
        auto *B = new float[WIDTH];

        for(int i = 0; i < WIDTH; i++){

            A[i] = B[i] = sqrtf( (float) i );
        }

        double time0 = omp_get_wtime();

        MyMultSum(A, B, WIDTH);

        double time1 = omp_get_wtime();

        double megaMultsPerSecond = (double) (WIDTH) / (time1 - time0) / 1000000.;

        if (megaMultsPerSecond > maxPerformanceNOSIMD)
            maxPerformanceNOSIMD = megaMultsPerSecond;

        delete [] A;
        delete [] B;
    }

    for (int t = 0; t < NUMTRIES; t++) {

        auto *A = new float[WIDTH];
        auto *B = new float[WIDTH];

        for(int i = 0; i < WIDTH; i++){

            A[i] = B[i] = sqrtf( (float) i );
        }

        double time0 = omp_get_wtime();

        SimdMulSum(A, B, WIDTH);

        double time1 = omp_get_wtime();

        double megaMultsPerSecond = (double) (WIDTH) / (time1 - time0) / 1000000.;

        if (megaMultsPerSecond > maxPerformanceSIMD)
            maxPerformanceSIMD = megaMultsPerSecond;

        delete [] A;
        delete [] B;
    }

    for (int t = 0; t < NUMTRIES; t++) {

        auto *A = new float[WIDTH];
        auto *B = new float[WIDTH];

        omp_set_num_threads(NUMT);

        for(int i = 0; i < WIDTH; i++){

            A[i] = B[i] = sqrtf( (float) i );
        }

        double time0 = omp_get_wtime();

        float sum = 0;

        #pragma omp parallel default(none) shared(A, B) reduction(+:sum)
        {
            int first = omp_get_thread_num() * NUM_ELEMENTS_PER_CORE;
            sum = SimdMulSum(&A[first], &B[first], NUM_ELEMENTS_PER_CORE);
        }

        double time1 = omp_get_wtime();

        double megaMultsPerSecond = (double) (WIDTH) / (time1 - time0) / 1000000.;

        if (megaMultsPerSecond > maxPerformanceSIMDOPENMP)
            maxPerformanceSIMDOPENMP = megaMultsPerSecond;

        delete [] A;
        delete [] B;
    }


    fprintf(stdout,"%d\t%10.2lf\t%10.2lf\t%10.2lf\n", WIDTH, maxPerformanceNOSIMD, maxPerformanceSIMD, maxPerformanceSIMDOPENMP);


    return 0;
}
