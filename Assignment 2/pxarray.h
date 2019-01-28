// jlwcpsc5260
// array.h
#ifndef ARRAY_H
#define ARRAY_H
void printarray(double* array, int size);
void random_array(double* array, int size, double scale);
double sum(double* array, int size);
double stdev(double* array, int size);
void smooth(double* array, int size, double w);
#endif
