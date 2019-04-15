// jlwcpsc5260a3
// pxarray.h
#ifndef PXARRAY_H
#define PXARRAY_H
void printarray(double* array, int size);
void random_array(double* array, int size, double scale);
double sum(double* array, int size);
double stdev(double* array, int size);
void smooth(double* array, int size, double w);
#endif
