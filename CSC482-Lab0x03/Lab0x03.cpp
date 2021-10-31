
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


enum Algs {
    A,
    B,
    C,
    D
};

const int test = B;
long long unsigned int busyCount;



void doBusyWork(void)
{
    for (int k = 0; k < 1; k += 1);
    busyCount += 1;
}

void generateArrayA(long long N, int list[])
{
    srand(NULL);
    for (long long i = 0; i < N; i++) {
        list[i] = rand() % 100;
    }
}

void generateArrayB(long long N, double list[])
{
    srand(NULL);
    for (long long i = 0; i < N; i++) {
        list[i] = (double)(rand() % 100);
    }
}

void algorithm_A(int N, int list[])
{
    int a = 0;
    int b = N;
    while (a < b) {
        if (list[a] < list[b]) {
            a = a + 1;
        }
        else {
            b = b - 1;
        }
        doBusyWork();
    }
}

int algorithm_B(int N, int list[])
{
    for (int q = 1; q < N; q = 2 * q) {
        for (int i = 0; i < N - q; i = i + 2 * q) {
            doBusyWork();
            if (list[i] < list[i + q]) {
                int tmp = list[i];
                list[i] = list[i + q];
                list[i + q] = tmp;

            }
        }
    }
    return list[0];
}

void algorithm_C(double list[], int startIndex, int endIndex) 
{
    if (endIndex > startIndex) {
        //phase 1   
        for (int i = startIndex; i <= (startIndex + endIndex) / 2; i++) {
            int x = list[i];
            list[i] = 2.0 * list[endIndex + startIndex - i];
            list[endIndex + startIndex - i] = x;
        }

        //phase 2: recursion 


        algorithm_C(list, startIndex, startIndex + floor(.5 * (endIndex - startIndex)));
        algorithm_C(list, startIndex + ceil(.25 * (endIndex - startIndex)), startIndex + floor(.75 * (endIndex - startIndex)));      
        algorithm_C(list, startIndex + ceil(.5  * (endIndex - startIndex)), endIndex);

        //phase 3 
        for (int i = startIndex; i <= (startIndex + endIndex) / 2; i++) {
            int x = list[i];
            list[i] = list[endIndex + startIndex - i] / 2.0;
            list[endIndex + startIndex - i] = x;
        }

        doBusyWork();
    }
}

int algorithm_D(int N)
{
    if (N <= 1) return 1;
    else {
        //doBusyWork();
        return (algorithm_D(N - 1) + algorithm_D(N - 2));
    }
}

int main(int argc, char **argv) {

    double trialsTime_max = .250; // in seconds
    long long int trialsCount_max = 1000000,
        N_min = 1,
        trial;
    clock_t splitTimeStamp, trialSetStart;
    const long long  N_max = 1000000000; // adjust as needed, keep small for debugging

    double splitTime, trialSetCount, trialSetTime, dummySetCount, dummySetTime, averageTrialTime, averageDummyTrialTime, estimatedTimePerTrial;

    double times[100] = { 0 };
    int index = 1;
    int *listA = (int*)malloc(sizeof(int) * N_max);
    double* listB = (double*)malloc(sizeof(double) * N_max);
    generateArrayA(N_max, listA);
    generateArrayB(N_max, listB);

    // If you are printing a results table directly, print your header line here.
    printf("+----------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("| %20s | %20s | %20s | %20s | %20s | %20s | %20s |\n", "n^2", "N", "Measured Time", "Measured Dbl Ratio", "Expected Dbl Ratio", "Busy Count", "Time/Busy Count");
    printf("+----------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    // power of 2 | N | Measured Time | Measured Doubling Ratio | Expected Doubling Ratio |Busy Count | Measured Time / Busy Count
    // For each size N of input we want to test -- typically start N at 1 and double each repetition
    //for (long long int n=1; n<N_max; n=2*n ) {
    for (long long int n = 1; n < N_max; n = 2*n) {
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
            case A:
                algorithm_A(n, listA);
                break;
            case B:
                algorithm_B(n, listA);
                break;
            case C:
                algorithm_C(listB, 0, n);
                break;
            case D:
                algorithm_D(n);
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
            case A:
                printf("| %20llu | %20.0f | %20.10f | %20.10f | %20.2f | %20llu | %20.10f |\n", n, log2((double)n), estimatedTimePerTrial, times[index] / times[index - 1], (float)n/(float)(n/2), busyCount, estimatedTimePerTrial / busyCount);
                break;
            case B:
                printf("| %20llu | %20.0f | %20.10f | %20.10f | %20.2f | %20llu | %20.10f |\n", n, log2((double)n), estimatedTimePerTrial, times[index] / times[index - 1], log(n)/log(n/2), busyCount, estimatedTimePerTrial / busyCount);
                break;
            case C:
                printf("| %20llu | %20.0f | %20.10f | %20.10f | %20.2f | %20llu | %20.10f |\n", n, log2((double)n), estimatedTimePerTrial, times[index] / times[index - 1], (float)(3*(n/2)+n) / (float)((3*(n/4)+n)/2), busyCount, estimatedTimePerTrial / busyCount);
                break;
            case D:
                printf("| %20llu | %20.0d | %20.10f | %20.10f | %20.2f | %20s | %20s |\n", 0, n, estimatedTimePerTrial, times[index] / times[index - 1], pow(2, (double)n)/pow(2, (double)n-1), "N/A",  "N/A");
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

    free(listA);
    free(listB);
}

//////////////////////////////////
/////////////////////////////////