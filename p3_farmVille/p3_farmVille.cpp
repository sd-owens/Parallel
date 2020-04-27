/* Program Name:  Project 3 FarmVille
 * Author:  Steven Owens
 * Date:  4/27/2020.
 * Summary:
 */
#include <iostream>
#include <cmath>
#include <omp.h>

// global constants
const float GRAIN_GROWS_PER_MONTH =		9.0;
const float ONE_DEER_EATS_PER_MONTH =   1.0;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			    2.0;	// plus or minus noise

const float AVG_TEMP =				    60.0;	// average
const float AMP_TEMP =				    20.0;	// plus or minus
const float RANDOM_TEMP =			    10.0;	// plus or minus noise

const float MIDTEMP =				    40.0;
const float MIDPRECIP =				    10.0;

// global variables
int	    NowYear;		// 2020 - 2025
int	    NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	    NowNumDeer;		// number of deer in the current population

bool     WrathOfGod = false; // set to false (0) by default
bool     FavorOfGod = false; // set to false (0) by default

unsigned int seed = 0;

float SQR( float x ) {

    return x * x;
}

float Ranf( unsigned int *seedp,  float low, float high ){

    float r = (float) rand_r( seedp );              // 0 - RAND_MAX

    return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int Ranf( unsigned int *seedp, int ilow, int ihigh ){

    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;

    return (int)(  Ranf(seedp, low,high) );
}
/* Summary:  Helper function to advance state of simulation (+1 month)
 * Params:   none
 * Returns:  none
 * */
void advanceTime() {

    NowMonth++;

    if(NowMonth % 12 == 0) {
        NowYear++;
    }

}
/* Summary:  Function to update environment variables after each month
 * Params:   none
 * Returns:  none
 * */
void updateEnvironment(){

    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

    float temp = AVG_TEMP - AMP_TEMP * cos( ang );

    NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. )
        NowPrecip = 0.;

}
/* Summary:  Function to handle initialization of environment variables and
 *           setup of simulation before parallization.
 * Params:   none
 * Returns:  none
 * */
void setup(){

    NowMonth = 0;
    NowYear = 2020;
    NowNumDeer = 1;
    NowHeight = 1.;

    float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

    float temp = AVG_TEMP - AMP_TEMP * cos( ang );

    NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. )
        NowPrecip = 0.;

    //fprintf(stdout, "Setup Complete\n");
}
/* Summary:  Handles thread logic for GrainDeer based on Grain Height
 * Params:   none
 * Returns:  none
 * */
void GrainDeer(){

    int NextNumDeer {};

    while(NowYear < 2026) {

        if(NowNumDeer > NowHeight){
            NextNumDeer = NowNumDeer - 1;
            if(NextNumDeer < 0)
                NextNumDeer = 0;
        } else
            NextNumDeer = NowNumDeer + 1;

        #pragma omp barrier // DoneComputing

        NowNumDeer = NextNumDeer;

        #pragma omp barrier // DoneAssigning

        #pragma omp barrier // DonePrinting
    }
}
/* Summary:  Handles thread logic for Grain based on environment factors and
 *           influence of the GrainGod.  Handles growth of grain based on rain
 *           and temperature as well as consumption by deer.
 * Params:   none
 * Returns:  none
 * */
void Grain(){

    float NextHeight {};

    while(NowYear < 2026) {

        float tempFactor = exp(-SQR((NowTemp - MIDTEMP) / 10.));
        float precipFactor = exp(-SQR((NowPrecip - MIDPRECIP) / 10.));

        if(FavorOfGod) {
            NextHeight = NowHeight + GRAIN_GROWS_PER_MONTH;

        } else if (WrathOfGod) {

            NextHeight = NextHeight;  // 0 growth;

        } else {
                NextHeight = NowHeight + (tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH);

        }

        NextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

        if(NextHeight < 0.) {
            NextHeight = 0.;
        }

        #pragma omp barrier // DoneComputing

        NowHeight = NextHeight;
        #pragma omp barrier // DoneAssigning

        //do nothing
        #pragma omp barrier // DonePrinting
    }
}
/* Summary:  Handles thread logic for printing status of simulation after each month and
 *           updates environment variables for next iteration of while loop.
 * Params:   none
 * Returns:  none
 * */
void Watcher(){

    fprintf(stdout, "Month\tTemp(c)\t\tRain(cm)\tDeer\tHeight(cm)\tGrain God\n");

    while(NowYear < 2026) {
        //do nothing
        #pragma omp barrier // DoneComputing

        //do nothing
        #pragma omp barrier // DoneAssigning

        std::string status = "";

        if(WrathOfGod)
            status = "severe drought";
        if(FavorOfGod)
            status = "bountiful growth";

        double NowPrecipCm = NowPrecip * 2.54;
        double NowHeightCm = NowHeight * 2.54;
        double NowTempC = (5./9.) * (NowTemp - 32.);

        //print results
        fprintf(stdout, "%d\t%10.2lf\t%10.2lf\t\t%d\t%10.2lf\t\t%s\n",
                NowMonth + 1, NowTempC, NowPrecipCm, NowNumDeer, NowHeightCm, status.c_str());

        //update environment variables
        advanceTime();  // advance time by one month;
        updateEnvironment();  // update monthly precipitation and temp data

        #pragma omp barrier // DonePrinting

    }
}
/* Summary:  Handles thread logic for GrainGod influence on Grain Growth
 *           Uses a random number to set a negative effect (1)(WrathOfGod)
 *           or a positive effect (0)(FavorOfGod) boolean flag which affects
 *           the growth rate of the grain in the following month.  Appeasing
 *           the gods this month makes for a bountiful harvest next month
 *           regardless of temp or rain condition.  Additionally, a vengeful
 *           god results in an effect resulting in zero grain growth in the
 *           following month.  Effects are reset each round.
 * Params:   none
 * Returns:  none
 * */
void GrainGod(){

    while(NowYear < 2026) {

        // Calculate a random 1 between 1 and 10.

        int randNum = Ranf(&seed, 1, 10);
        #pragma omp barrier // DoneComputing

        // clear previous month God Status
        WrathOfGod = false;
        FavorOfGod = false;
        // set new month God Status
        if(randNum == 1)
            WrathOfGod = true;

        if(randNum == 10)
            FavorOfGod = true;

        #pragma omp barrier // DoneAssigning

        //do nothing
        #pragma omp barrier // DonePrinting
    }

}
/* Summary:  Function to handle parallization of threads that handle the
 *           Grain, GrainDeer, Watcher, and GrainGod functions.
 * Params:   none
 * Returns:  none
 * */
void run(){

    omp_set_num_threads( 4 );	// same as # of sections
    //InitBarrier(4);
    #pragma omp parallel sections default(none)
    {
        #pragma omp section
        {
            GrainDeer( );
        }

        #pragma omp section
        {
            Grain( );
        }

        #pragma omp section
        {
            Watcher( );
        }

        #pragma omp section
        {
            GrainGod( );
        }
    }       // implied barrier -- all functions must return in order
            // to allow any of them to get past here
}
/* Summary:  Main function which handles setup and running of the simulation.
 *           Provides a check that openMP is setup and available in g++.
 * Params:   none
 * Returns:  none
 * */
int main() {

#ifndef _OPENMP
    fprintf( stderr, "No OpenMP support!\n" );
    return 1;
#endif

    setup();
    run();

    return 0;
}
