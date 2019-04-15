// // // // // // // // //
// jamion williams
//  cpsc5260 assignment 3
//   jlwcpsc5260a3
// // // // // // // // //

// pxmain.c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "pxarray.h"
int main()
{
        // constants
        long ARRAYLENGTH;
        int NUMSMOOTH;
        double SMOOTHWEIGHT, ARRAYSCALE;

        //read input for the above
        printf("Enter the length of your array: \n");
        scanf("%li", &ARRAYLENGTH);
        printf("Enter the max possible value of an element in your array:\n");
        scanf("%lf", &ARRAYSCALE);
        printf("Enter the weight of your smoothing algorithm [e.g. 0.25]:\n");
        scanf("%lf", &SMOOTHWEIGHT);
        printf("Enter the number of times you would like to smooth your array:\n");
        scanf("%i", &NUMSMOOTH);
        printf("\n");

        // create an array d[] and initialize it
        // demonstration of random_array()
        double *d;
        d = (double *)malloc(sizeof(double)*ARRAYLENGTH);
        random_array(d,ARRAYLENGTH,ARRAYSCALE);

        // // // //
        // uncomment the below to print the array
        //  ...leaving it out for large sizes
        // // // //

        //printf("Hello! Here's your array: \n");
        //printarray(d,ARRAYLENGTH);

        // demonstration of sum()
        printf("The sum of your array is: ");
        double start_a = omp_get_wtime();
        printf("%f", sum(d,ARRAYLENGTH));
        double end_a = omp_get_wtime();
        printf("\n");

        // demonstration of stdev()
        printf("The standard deviation of your array is:");
        double start_b = omp_get_wtime();
        printf("%f", stdev(d,ARRAYLENGTH));
        double end_b = omp_get_wtime();
        printf("\n");

        // demonstration of smooth()
        printf("If we smooth out your array, weighted at ");
        printf("%f", SMOOTHWEIGHT);
        printf(", we get: \n");

        double start_c = omp_get_wtime();
        for (int i = 0; i < NUMSMOOTH; i++) {
          printf("*** Iteration %d *** \n", i + 1);
          smooth(d,ARRAYLENGTH,SMOOTHWEIGHT);
          //printarray(d,ARRAYLENGTH);
          printf("The stdev of your array after smoothing is: ");
          printf("%f \n", stdev(d,ARRAYLENGTH));
        }
        double end_c = omp_get_wtime();

        printf("The runtime of this algorithm was %lf seconds in total.\n",(end_a - start_a) + (end_b - start_b) + (end_c - start_c));
        printf("The summation took %lf seconds. \n", end_a - start_a);
        printf("The standard deviation took %lf seconds. \n", end_b - start_b);
        printf("The smoothing took %lf seconds (totaled over %d iterations). \n", end_c - start_c, NUMSMOOTH);

        printf("\n");

        return 0;
}
