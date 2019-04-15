// jlwcpsc5260a3
// pxarray.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "pxarray.h"

// printarray()
// print out the elements of an array
void printarray(double* array, int size) {
  #pragma omp for
  for (int i = 0; i < size; i++) {
    printf("%f \n", array[i]);
  }
}

// random_array()
// throws random numbers into the provided array, weighted by scale
void random_array(double* array, int size, double scale) {
  //#pragma omp parallel default(none) shared(array, size, scale)
  {
    //srand(time(0)) ^ omp_get_thread_num();
    //srand(time(omp_get_thread_num()));
    //#pragma omp for
    for (int i = 0; i < size; i++) {
      array[i] = rand() * scale / RAND_MAX;
    }
  }
}

// sum()
// return the sum of all elements in the array
double sum(double* array, int size) {
  double sum = 0.0;

  #pragma omp parallel for reduction(+:sum) schedule(guided)
  for (int i = 0; i < size; i++) {
    sum += array[i];
  }

  return sum;
}

// stdev()
// calculate the standard deviation of the elements in array
double stdev(double* array, int size) {
  double stdv = 0.0;
  double mean = sum(array, size)/size;

  #pragma omp parallel for reduction(+:stdv) schedule(guided)
  for (int i = 0; i < size; i++) {
    stdv += pow(array[i] - mean, 2);
  }

  return sqrt(stdv/size);
}

// smooth()
// replace all internal values (not the first or last) with a weighted sum of
//  itself and average of the neighboring elements
//  newvali = oldvali ∗ w + (oldvali−1 + oldvali+1)(1 − w)/2
void smooth(double* array, int size, double w) {
  double temp[size];

  //init the "inner" array
  temp[0] = array[0];
  temp[size - 1] = array[size - 1];

  //...and then smooth the array as described above
  for (int i = 1; i < size - 1; i++) {
    temp[i] = array[i] * w + (array[i - 1] + array[i + 1]) * (1 - w)/2;
  }

  //finally, replace the elements of array with that of temp
  #pragma omp parallel for
  for (int i = 0; i < size; i++) {
    array[i] = temp[i];
  }
}
