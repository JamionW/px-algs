// jlwcpsc5260a2
// pxprefix.c
// replace the contents of data with the prefix sum of the contents
#include <iostream>
#include <cstdio>
#include <vector>
#include <omp.h>

void print_vector(std::vector<int>& data) {
  for (int i = 0; i < (int)data.size(); i++) {
    printf("%d\n", data[i]);
  }
  printf("\n");
}

void serial_prefix_sum(std::vector<int>& data) {
  for (int i = 1; i < (int)data.size(); i++)
  {
      data[i] = data[i] + data[i - 1];
  }
}

void px_prefix_sum(std::vector<int>& data) {
  //init the nodetotal array
  std::vector<int> nodetotal(omp_get_max_threads() - 1, 0);

  #pragma omp parallel default(none) shared(data, nodetotal)
    {
      //distribution of the data across all threads
      int rank = omp_get_thread_num();
      int nt = omp_get_num_threads();
      int count = data.size() / nt;
      int start = rank * count;
      int mod = data.size() % nt;

      //Dr. Tanis' trick for uneven distribution
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
      //printf("ON PARTITION %d !!!!\n count is %d\n start is %d\n end is %d\n rank is %d\n nt is %d\n mod is %d\n\n", omp_get_thread_num(), count, start, end, rank, nt, mod);

      //prefix sum within the local nodes
      for (int i = start + 1; i < end; i++)
      {
        data[i] = data[i] + data[i - 1];
        nodetotal[rank] = data[i];
      }

      //synchronize the threads counting locally
      #pragma omp barrier

      //prefix sum the sum of each node
      #pragma omp single
      {
        for (int i = 1; i < nt; i++)
        {
          nodetotal[i] = nodetotal[i] + nodetotal[i - 1];
        }
      }

      //correct the local sums
      if (rank > 0)
      {
        for (int i = start; i < end; i++)
        {
          //printf("NODE %d, prev value of i was %d, now adding %d to that to make %d\n",rank,data[i],nodetotal[rank], data[i] + nodetotal[rank]);
          data[i] = data[i] + nodetotal[rank - 1];
        }
      }
    }
}
