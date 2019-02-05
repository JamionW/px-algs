// // // // // // // // //
// jamion williams
//  cpsc5260 assignment 2
//   jlwcpsc5260a2
// // // // // // // // //

// pxmain.cpp
#include <iostream>
#include <cstdio>
#include <vector>
#include <omp.h>
#include "pxprefix.h"

int main(int argc, char *argv[]) {

  // constants
  int ARRAYLENGTH;

  // test with a vector of 1’s
  printf("Enter the length of your array: \n");
  scanf("%i", &ARRAYLENGTH);
  printf("\n...working...\n\n");
  int size=ARRAYLENGTH;
  std::vector<int> serialdata(size, 1);
  //print_vector(serialdata);
  double serialstart = omp_get_wtime();
  serial_prefix_sum(serialdata);
  double serialend = omp_get_wtime();
  //print_vector(serialdata);

  std::vector<int> pxdata(size, 1);
  //print_vector(pxdata);
  double pxstart = omp_get_wtime();
  px_prefix_sum(pxdata);
  double pxend = omp_get_wtime();
  //print_vector(pxdata);

  printf("In serial, it took %lf seconds to prefix sum %d elements.\n", (serialend-serialstart), (int)serialdata.size());
  printf("In parallel, it took %lf seconds to prefix sum %d elements on %d nodes.\n", (pxend-pxstart), (int)pxdata.size(), omp_get_max_threads());
}
