// // // // // // // // //
// jamion williams
//  cpsc5260 assignment 2
//   jlwcpsc5260a2
// // // // // // // // //

// main.c
#include <stdio.h>
#include <omp.h>
#include "pxarray.h"

int main()
{
        #pragma omp parallel
          {
            printf("hello %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
          }

        // constants
        int ARRAYLENGTH, NUMSMOOTH;
        double SMOOTHWEIGHT, ARRAYSCALE;

        //read input for the above
        printf("Enter the length of your array: \n");
        scanf("%i", &ARRAYLENGTH);
        printf("Enter the max possible value of an element in your array:\n");
        scanf("%lf", &ARRAYSCALE);
        printf("Enter the weight of your smoothing algorithm [e.g. 0.25]:\n");
        scanf("%lf", &SMOOTHWEIGHT);
        printf("Enter the number of times you would like to smooth your array:\n");
        scanf("%i", &NUMSMOOTH);
        printf("\n");

        // create an array d[] and initialize it
        // demonstration of random_array()
        double d[ARRAYLENGTH];
        random_array(d,ARRAYLENGTH,ARRAYSCALE);

        printf("Hello! Here's your array: \n");
        printarray(d,ARRAYLENGTH);

        // demonstration of sum()
        printf("The sum of your array is: \n");
        printf("%f", sum(d,ARRAYLENGTH));
        printf("\n");

        // demonstration of stdev()
        printf("The standard deviation of your array is:");
        printf("%f", stdev(d,ARRAYLENGTH));
        printf("\n");

        // demonstration of smooth()
        printf("If we smooth out your array, weighted at ");
        printf("%f", SMOOTHWEIGHT);
        printf(", we get: \n");

        for (int i = 0; i < NUMSMOOTH; i++) {
          printf("Iteration %d", i + 1);
          printf("\n");
          smooth(d,ARRAYLENGTH,SMOOTHWEIGHT);
          printarray(d,ARRAYLENGTH);
          printf("Hey, cool! The standard deviation of your array after smoothing is: \n");
          printf("%f", stdev(d,ARRAYLENGTH));
          printf("\n");
        }

        printf("\n");

        return 0;
}
