// nvcc -Xcompiler -Wall -DDOLOG ppm-cuda.cu

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdio>
#include "cuda_helpers.h"

int main(int argc, char *argv[])
{
    int width, height;

    // these are the vectors storing the pixeldata in its three colors
    std::vector<float> r,g,b;
    float *d_r, *d_g, *d_b, *h_r, *h_g, *h_b;

    // read file
    read("input.ppm", width, height, r,g,b);
    LOG("processing " << width << "x" << height);

    // allocate memory for destination and "copy"
    gpuErrchk(cudaMalloc(&d_r, width*height*sizeof(float)));
    gpuErrchk(cudaMalloc(&d_g, width*height*sizeof(float)));
    gpuErrchk(cudaMalloc(&d_b, width*height*sizeof(float)));
    gpuErrchk(cudaMalloc(&h_r, width*height*sizeof(float)));
    gpuErrchk(cudaMalloc(&h_g, width*height*sizeof(float)));
    gpuErrchk(cudaMalloc(&h_b, width*height*sizeof(float)));

    //set i <= 5 or more for blur() testing
    for (int i = 0; i <= 1; i++) {

      // copy to GPU
      gpuErrchk(cudaMemcpy(d_r, &r[0], width*height*sizeof(float), cudaMemcpyHostToDevice));
      gpuErrchk(cudaMemcpy(d_g, &g[0], width*height*sizeof(float), cudaMemcpyHostToDevice));
      gpuErrchk(cudaMemcpy(d_b, &b[0], width*height*sizeof(float), cudaMemcpyHostToDevice));

      // call kernel on GPU
      dim3 tpb(16, 16);
      dim3 bpg((width+tpb.x-1)/tpb.x, (height+tpb.y-1)/tpb.y);

      //uncomment for blur()
      //blur<<<bpg,tpb>>>(width, height, d_r, d_g, d_b, h_r, h_g, h_b, 0.25);

      //uncomment for edgedetect()
      //edgedetect<<<bpg,tpb>>>(width, height, d_r, d_g, d_b, h_r, h_g, h_b, 0.2);

      //uncomment for sinewave()
      //sinewave<<<bpg,tpb>>>(width, height, h_r, h_g, h_b);

      //uncomment for flippy()
      flippy<<<bpg,tpb>>>(width, height, d_r, d_g, d_b, h_r, h_g, h_b);

      // prep arrays for receipt from kernel
      r.resize(width*height);
      g.resize(width*height);
      b.resize(width*height);

      // copy data back from GPU
      gpuErrchk(cudaMemcpy(&r[0], h_r, width*height*sizeof(float), cudaMemcpyDeviceToHost));
      gpuErrchk(cudaMemcpy(&g[0], h_g, width*height*sizeof(float), cudaMemcpyDeviceToHost));
      gpuErrchk(cudaMemcpy(&b[0], h_b, width*height*sizeof(float), cudaMemcpyDeviceToHost));

    }

    // be a good roommate and clean up
    cudaFree(d_r);
    cudaFree(d_g);
    cudaFree(d_b);
    cudaFree(h_r);
    cudaFree(h_g);
    cudaFree(h_b);

    // save image
    write("output.ppm", width, height, r,g,b);

    return 0;
}
