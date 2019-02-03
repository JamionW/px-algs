// jlwcpsc5260a2
// pxprefix.c
// replace the contents of data with the prefix sum of the contents
#include <iostream>
#include <cstdio>
#include <vector>
#include <omp.h>

void print_vector(std::vector<int>& data) {
  for (int i = 0; i < data.size(); i++) {
    printf("%d\n", data[i]);
  }
  printf("\n");
}

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
      int count = data.size() / nt;
      int start = rank * count;
      int mod = data.size() % nt;

      if (rank < mod)
      {
        count ++;
        start += rank;
      }
        else
      {
        start += mod;
      }

      //have to accommodate uneven partitions before calculating end
      int end = start + count;

      //debugging
      printf("ON PARTITION %d !!!!\n count is %d\n start is %d\n end is %d\n rank is %d\n nt is %d\n mod is %d\n\n", omp_get_thread_num(), count, start, end, rank, nt, mod);
      //prefix sum
      if (rank == 0)
      {
        for (int i = start + 1; i < end; i++)
          {
            data[i] = data[i] + data[i - 1];
          }
      }
      else
      {
          for (int i = start; i < end; i++)
          {
            data[i] = data[i] + data[(i - 1)];
          }
      }

      #pragma omp barrier

      if (rank > 0)
      {
        for (int i = 1; i < nt; i++)
        {
          data[i * count] = data[i * count] + data[(i - 1) * count];
        }
      }

    }
}
