#ifndef CUDA_TEST_H
#define CUDA_TEST_H
#include <stdio.h>

__global__
void blur(int width, int height, float* r, float* g, float* b, float* r_o, float* g_o, float* b_o, float weight);
__global__
void edgedetect(int width, int height, float* r, float* g, float* b, float* r_o, float* g_o, float* b_o, float threshold);
__global__
void sinewave(int width, int height, float* r_o, float* g_o, float* b_o);
__global__
void flippy(int width, int height, float* r, float* g, float* b, float* r_o, float* g_o, float* b_o);
int write(std::string outfile, int width, int height, const std::vector<float>& r, const std::vector<float>& g, const std::vector<float>& b);
int read(std::string filename, int& width, int& height, std::vector<float>& r, std::vector<float>& g, std::vector<float>& b);

// host code for validating last cuda operation (not kernel launch)
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess)
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

#endif /* CUDA_TEST_H */

#ifdef DOLOG
#define LOG(msg) std::cerr<<msg<<std::endl
//#define LOG(msg) fprintf(stderr, msg "\n");
#else
#define LOG(msg)
#endif

#define imin(a,b) (a<b?a:b)
