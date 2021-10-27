
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


enum Algs {
    N3,
    N6,
    LOGN,
    NLOGN,
    LOGN_SQUARED,
    N2LOGN,
    EXP,
    FACTORIAL
};

int test = EXP;
long long unsigned int busyCount;



void doBusyWork(void)
{
    for (int k = 0; k < 100; k += 1);
    busyCount += 1;
}

void busyAlgorithm_N3(long long unsigned int N)
{
    for (long long unsigned k = 0; k < N; k++)
        for (long long unsigned l = 0; l < k; l++)
            for (long long unsigned m = 0; m < k; m++)
                doBusyWork();
}

void busyAlgorithm_NlogN(long long unsigned int N)
{
    if (N > 1)
        busyAlgorithm_NlogN(N / 2);

    for (long long unsigned k = 0; k < N; k++)
        doBusyWork();

}

void busyAlgorithm_logN(long long unsigned int N)
{
    if (N > 1)
        busyAlgorithm_logN(N / 2);
    doBusyWork();
}

void busyAlgorithm_N2logN(long long unsigned int N)
{
    if (N > 1)
        busyAlgorithm_N2logN(N / 2);

    for (long long unsigned k = 0; k < N; k++)
        for (long long unsigned l = 0; l < k; l++)
            doBusyWork();
}

void busyAlgorithm_N6(long long unsigned int N)
{
    /* would be a beautiful pawn structure */
    for (long long unsigned int i = 0; i < N; i++)
        for (long long unsigned int j = 0; j < i; j++)
            for (long long unsigned int k = 0; k < i; k++)
                for (long long unsigned int l = 0; l < i; l++)
                    for (long long unsigned int m = 0; m < i; m++)
                        for (long long unsigned int n = 0; n < i; n++)
                            doBusyWork();

}

void busyAlgorithm_logN_squared(long long unsigned int N)
{
    if (N > 1)
        busyAlgorithm_logN_squared(N / 2);
    doBusyWork();
    doBusyWork();
}

void busyAlgorithm_exp(long long unsigned int N)
{
    if (N > 1) {
        busyAlgorithm_exp(N - 1);
        doBusyWork();
        busyAlgorithm_exp(N - 1);
    }
}

void busyAlgorithm_factorial(long long unsigned int N)
{

}



int main(int argc, int argv) {

    double trialsTime_max = .250; // in seconds
    long long int trialsCount_max = 1000000,
        N_min = 1,
        N_max = 100000000000, // adjust as needed, keep small for debugging
        trial;
    clock_t splitTimeStamp, trialSetStart;
    double splitTime, trialSetCount, trialSetTime, dummySetCount, dummySetTime, averageTrialTime, averageDummyTrialTime, estimatedTimePerTrial;

    double times[100] = { 0 };
    int index = 1;


    // If you are printing a results table directly, print your header line here.
    printf("+----------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("| %20s | %20s | %20s | %20s | %20s | %20s | %20s |\n", "n^2", "N", "Measured Time", "Measured Dbl Ratio", "Expected Dbl Ratio", "Busy Count", "Time/Busy Count");
    printf("+----------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    // power of 2 | N | Measured Time | Measured Doubling Ratio | Expected Doubling Ratio |Busy Count | Measured Time / Busy Count
    // For each size N of input we want to test -- typically start N at 1 and double each repetition
    //for (long long int n=1; n<N_max; n=2*n ) {
    for (long long int n = 1; n < N_max; n++) {
        /********* any preparations, test input generation, to be used for entire trial set ********/

        splitTime = 0.0;
        // get timestamp before set of trials are run:
        trialSetStart = clock();
        // For each trial trialNumber=1 to Number of Trials per input size:
        for (trial = 0; trial < trialsCount_max && splitTime < trialsTime_max; trial++) {
            /******** any preparations, test input generation, for a single trial run *********/
            busyCount = 0;
            /**** >>>> Call the algorithm function we are testing, using the generated test input <<<<< ******/

            switch (test) {
            case N3:
                busyAlgorithm_N3(n);
                break;
            case N6:
                busyAlgorithm_N6(n);
                break;
            case LOGN:
                busyAlgorithm_logN(n);
                break;
            case NLOGN:
                busyAlgorithm_NlogN(n);
                break;
            case LOGN_SQUARED:
                busyAlgorithm_logN_squared(n);
                break;
            case N2LOGN:
                busyAlgorithm_N2logN(n);
                break;
            case EXP:
                busyAlgorithm_exp(n);
                break;
            case FACTORIAL:
                busyAlgorithm_factorial(n);
                break;
            default:
                break;
            }


            /******* do any "clean up" after running your algorithm ********/

            // get split time -- "split time" just means the total time so far for this tiral set
            splitTimeStamp = clock(); // 
            // split time is the difference between current split timestamp and the starting time stamp for trial set
            splitTime = (splitTimeStamp - trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
        }
        trialSetCount = trial; // value of trial when loop ends is how many we did
        trialSetTime = splitTime; // total time for trial set is the last split time
        averageTrialTime = trialSetTime / trialSetCount; // this is the average tiem per trial, including any prep/overhead



        /********* NOW DO A "DUMMY" TRIAL SET TO ESTIMATE THE OVERHEAD TIME ********/
        /* We can just repeat this trialSetCount times, which we know should be faster than above */

        splitTime = 0.0;
        // get timestamp before set of dummy trials are run:
        trialSetStart = clock();
        for (trial = 0; trial < trialSetCount && splitTime < trialsTime_max; trial++) {

            /******** any preparations, test input generation, for a single trial run *********/

            /**** DO NOT Call the algorithm function!!! ******/

            /******* do any "clean up" after running your algorithm ********/

            // get split time -- "split time" just means the total time so far for this tiral set
            splitTimeStamp = clock(); // 
            // split time is the difference between current split timestamp and the starting time stamp for trial set
            splitTime = (splitTimeStamp - trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
        }
        dummySetCount = trial; // value of trial when loop ends is how many we did, should be same as trialsSetCount
        dummySetTime = splitTime; // total time for dummy trial set is the last split time
        averageDummyTrialTime = dummySetTime / dummySetCount; // this is the average tiem per dummy trial, including any prep/overhead


        estimatedTimePerTrial = averageTrialTime - averageDummyTrialTime; // should be a decent measurement of time taken to run your algorithm

        times[index] = estimatedTimePerTrial;

        if (n == 1)
            printf("| %20llu | %20.0f | %20.10f | %20.10f | %20.2f | %20llu | %20.10f |\n", n, log2((double)n), 0, 0, 0, busyCount, 0);
        else {
            switch (test) {
            case N3:
                printf("| %20llu | %20.0f | %20.10f | %20.10f | %20.2f | %20llu | %20.10f |\n", n, log2((double)n), estimatedTimePerTrial, times[index] / times[index - 1], pow((double)n, 3) / pow((double)n / 2, 3), busyCount, estimatedTimePerTrial / busyCount);
                break;
            case N6:
                printf("| %20llu | %20.0f | %20.10f | %20.10f | %20.2f | %20llu | %20.10f |\n", n, log2((double)n), estimatedTimePerTrial, times[index] / times[index - 1], pow((double)n, 6) / pow((double)n / 2, 6), busyCount, estimatedTimePerTrial / busyCount);
                break;
            case LOGN:
                printf("| %20llu | %20.0f | %20.10f | %20.10f | %20.2f | %20llu | %20.10f |\n", n, log2((double)n), estimatedTimePerTrial, times[index] / times[index - 1], log((double)n) / log((double)n / 2), busyCount, estimatedTimePerTrial / busyCount);
                break;
            case NLOGN:
                printf("| %20llu | %20.0f | %20.10f | %20.10f | %20.2f | %20llu | %20.10f |\n", n, log2((double)n), estimatedTimePerTrial, times[index] / times[index - 1], (n * log((double)n)) / ((n / 2) * log((double)n / 2)), busyCount, estimatedTimePerTrial / busyCount);
                break;
            case LOGN_SQUARED:
                printf("| %20llu | %20.0f | %20.10f | %20.10f | %20.2f | %20llu | %20.10f |\n", n, log2((double)n), estimatedTimePerTrial, times[index] / times[index - 1], log((double)n) / log((double)n / 2), busyCount, estimatedTimePerTrial / busyCount);
                break;
            case N2LOGN:
                printf("| %20llu | %20.0f | %20.10f | %20.10f | %20.2f | %20llu | %20.10f |\n", n, log2((double)n), estimatedTimePerTrial, times[index] / times[index - 1], pow((double)n, 2) / pow((double)n / 2, 2), busyCount, estimatedTimePerTrial / busyCount);
                break;
            case EXP:
                if (n % 2 == 0) {
                    printf("| %20llu | %20s | %20.10f | %20.10f | %20.2f | %20llu | %20.10f |\n", n, "N/A", estimatedTimePerTrial, times[n] / times[n / 2], pow(2, (double)n) / pow(2, ((double)n / 2)), busyCount, estimatedTimePerTrial / busyCount);
                }
                else {
                    printf("| %20llu | %20s | %20.10f | %20s | %20s | %20llu | %20.10f |\n", n, "N/A", estimatedTimePerTrial, "N/A", "N/A", busyCount, estimatedTimePerTrial / busyCount);
                }
                break;
            case FACTORIAL:
                break;
            default:
                break;
            }
        }
        index++;

        busyCount = 0;

        /************ save and/or print your results here to generate your table of results **************/
        // You should be able to print one row of your results table here.
        // Calculate doubling ratios and any other results for desired columns in table.
        // Consider additional columns you include in a more detailed table that may help with debugging or just making sense of results.
        // May be a good idea to print a row of results to output terminal window or a file, so if your program crashes you still have results to look at
        // use the fflush(stdout) or fflush(filePointer) or equivalent in C++, to force output so nothing is lost if a crash occurs
        // can also save data to array(s) for later printing/processing

    }
}

//////////////////////////////////
/////////////////////////////////