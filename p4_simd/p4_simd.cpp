#include <iostream>
#include <cmath>
#include <omp.h>
#include <xmmintrin.h>
#define SSE_WIDTH		  4
#define WIDTH           100
#define NUMTRIES         10

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
        sum += a[i] * a[i];
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

    double maxPerformance = 0;
    float simdSum = 0., mySum = 0.;

    for (int t = 0; t < NUMTRIES; t++) {

        float A[WIDTH];
        float B[WIDTH];

        for(int i = 0; i < WIDTH; i++){

            A[i] = B[i] = sqrtf( (float) i );
        }

        //printArray(A, WIDTH);
        //printArray(B, WIDTH);

        double time0 = omp_get_wtime();

        simdSum = SimdMulSum(A, B, WIDTH);
        mySum = MyMultSum(A, B, WIDTH);

        double time1 = omp_get_wtime();

        double megaMultsPerSecond = (double) (WIDTH * WIDTH) / (time1 - time0) / 1000000.;

        if (megaMultsPerSecond > maxPerformance)
            maxPerformance = megaMultsPerSecond;

    }

    fprintf(stdout,"%d\t%10.2lf\t%10.2lf\t%10.2lf\n", WIDTH, simdSum, mySum, maxPerformance);


    return 0;
}
