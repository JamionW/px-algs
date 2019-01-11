// // // // // // // // //
// jamion williams
//  cpsc5260 assignment 1
//   jlwcpsc5260
// // // // // // // // //

// main.c
#include <stdio.h>
#include "array.h"
int main()
{
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
        printf("The standard deviation of your array is: \n");
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
        }

        printf("\n");

        return 0;
}
