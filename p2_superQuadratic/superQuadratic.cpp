#include <iostream>
#include <cmath>
#include <omp.h>

// setting the number of threads:
#ifndef NUMT
#define NUMT    1
#endif

// how many tries to discover the maximum performance:
#ifndef NUMTRIES
#define NUMTRIES	10
#endif

// how many nodes per axis:
#ifndef NUMNODES
#define NUMNODES    100
#endif

#define N         4
#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

float Height( int iu, int iv ) {	// iu,iv = 0 .. NUMNODES-1
    float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
    float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

    float xn = pow( fabs(x), (double)N );
    float yn = pow( fabs(y), (double)N );
    float r = 1. - xn - yn;
    if( r < 0. )
        return 0.;
    float height = pow( 1. - xn - yn, 1./(float)N );
    return height;
}

int main( int argc, char *argv[ ] ) {

#ifndef _OPENMP
    fprintf( stderr, "No OpenMP support!\n" );
    return 1;
#endif

    double maxPerformance = 0, peakVolume = 0;

    omp_set_num_threads(NUMT);  // set the number of threads to use in the for-loop

    // looking for the maximum performance:
    for (int t = 0; t < NUMTRIES; t++) {

        double volume = 0;

        // the area of a single full-sized tile:
        float fullTileArea = (((XMAX - XMIN) / (float) (NUMNODES - 1)) *
                              ((YMAX - YMIN) / (float) (NUMNODES - 1)));

        float halfTileArea = fullTileArea / 2;
        float quarterTileArea = fullTileArea / 4;

        // sum up the weighted heights into the variable "volume"
        // using an OpenMP for loop and a reduction:
        double time0 = omp_get_wtime();

#pragma omp parallel for default(none) shared(fullTileArea) reduction(+:volume)
        for (int i = 0; i < NUMNODES * NUMNODES; i++) {
            int iu = i % NUMNODES;   // e.g. 2 % 4 = 0  15 % 4 = 3
            int iv = i / NUMNODES;   // e.g. 2 / 4 = 0  15 / 4 = 3
            float z = Height(iu, iv) * 2;

//            // logic to check if it is a corner node
//            if((iu == 0 && iv == 0) ||
//               (iu == NUMNODES - 1 && iv == NUMNODES - 1) ||
//               (iu == 0 && iv == NUMNODES - 1) ||
//               (iv == 0 && iu == NUMNODES - 1)) {
//
//                volume += z * fullTileArea / 4;
//
//            // logic to check if it is a side node
//            } else if (iu == 0 || iv == 0 || iu == NUMNODES - 1 || iv == NUMNODES - 1) {
//                volume += z * fullTileArea / 2;
//
//            // must be a full size node
//            } else {
//
//                volume += z * fullTileArea;
//            }

            volume += z * fullTileArea;
        }


        double time1 = omp_get_wtime();
        double megaNodesPerSecond = (double) (NUMNODES * NUMNODES) / (time1 - time0) / 1000000.;

        if (megaNodesPerSecond > maxPerformance)
            maxPerformance = megaNodesPerSecond;

        if(volume > peakVolume)
            peakVolume = volume;

    }

    fprintf(stdout,"%d\t%10.2lf\t%d\t%10.2lf", NUMT, peakVolume, NUMNODES, maxPerformance);
    std::cout << std::endl;

    return 0;
}
