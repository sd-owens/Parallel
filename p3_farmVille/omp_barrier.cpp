//
// Created by ubuntu on 4/27/20.
//

#include "omp_barrier.h"

omp_lock_t Lock;
int NumInThreadTeam;
int NumAtBarrier;
int NumGone;

void InitBarrier(int n) {
    NumInThreadTeam = n;
    NumAtBarrier = 0;
    omp_init_lock(&Lock);
}

void WaitBarrier(){
    omp_set_lock(&Lock);
    {
        NumAtBarrier++;
        if(NumAtBarrier == NumInThreadTeam) {
            NumGone = 0;
            NumAtBarrier = 0;
            //let all other threads return before this one unlocks:
            while(NumGone != NumInThreadTeam - 1);
            omp_unset_lock(&Lock);
            return;
        }
    }
    omp_unset_lock(&Lock);

    while(NumAtBarrier != 0);  //all threads wait her until the last one arrives.

    #pragma omp atomic         // ... and sets NumAtBarrier to 0.
        NumGone++;
}