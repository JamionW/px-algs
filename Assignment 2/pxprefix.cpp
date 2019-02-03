// jlwcpsc5260a2
// pxprefix.c
// replace the contents of data with the prefix sum of the contents
#include <iostream>
#include <cstdio>
#include <vector>
#include <omp.h>

void serial_prefix_sum(std::vector<int>& data) {
  for (int i = 1; i < data.size(); i++)
  {
      data[i] = data[i] + data[i - 1];
  }
}

void px_prefix_sum(std::vector<int>& data) {
  #pragma omp parallel default(none) shared(data)
    {
    int rank = omp_get_thread_num();
    int nt = omp_get_num_threads();
    int count = (int)data.size() / nt;
    int start = rank * count;
    int mod = (int)data.size() % nt;

      if (rank < mod)
      {
        count ++;
        start += rank;
      }
        else
      {
        start += mod;
      }

    // Your code goes here.
    }
}
