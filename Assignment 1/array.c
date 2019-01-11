// jlwcpsc5260
// array.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "array.h"

// printarray()
// print out the elements of an array
void printarray(double* array, int size) {
  for (int i = 0; i < size; i++) {
    printf("%f", array[i]);
    printf("\n");
  }
}

// random_array()
// throws random numbers into the provided array, weighted by scale
void random_array(double* array, int size, double scale) {
  for (int i = 0; i < size; i++) {
    array[i] = rand() * scale / RAND_MAX;
  }
}

// sum()
// return the sum of all elements in the array
double sum(double* array, int size) {
  double sum;

  for (int i = 0; i < size; i++) {
    sum = sum + array[i];
  }

  return sum;
}

// stdev()
// calculate the standard deviation of the elements in array
double stdev(double* array, int size) {
  double mean, stdv;

  mean = sum(array, size)/size;

  for (int i = 0; i < size; i++) {
    stdv = stdv + pow(array[i] - mean, 2);
  }

  return sqrt(stdv/size);
}

// smooth()
// replace all internal values (not the first or last) with a weighted sum of
//  itself and average of the neighboring elements
//  newvali = oldvali ∗ w + (oldvali−1 + oldvali+1)(1 − w)/2
void smooth(double* array, int size, double w) {
  double temp[size];
  printarray(array,size);
  //init the "inner" array
  //temp[0] = array[0];
  //temp[size] = array[size];
  for (int i = 1; i < size - 1; i++) {
    temp[i] = array[i] * w + (array[i - 1] + array[i + 1]) * (1 - w)/2;
  }

  printarray(temp, size);
//  for (int i = 0; i < size; i++) {
//    array[i] = temp[i];
//  }
}
