//  Dr. Tanis did most of the hard work...
//  ...thanks Dr. Tanis!
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdio>
#include "cuda_helpers.h"

// read file courtesy of Dr. Tanis
char* data;
int read(std::string filename,
         int& width,
         int& height,
         std::vector<float>& r,
         std::vector<float>& g,
         std::vector<float>& b)
{
    std::ifstream in(filename.c_str(), std::ios::binary);

    int maxcol;

    if (! in.is_open())
    {
        std::cerr << "could not open " << filename << " for reading" << std::endl;
        return 0;
    }

    {
        std::string magicNum;
        in >> magicNum;
        LOG("got magicNum:" << magicNum);
        //JW what is this?
        // this is broken if magicNum != 'P6'
    }

    {
        long loc = in.tellg();
        std::string comment;
        in >> comment;

        if (comment[0] != '#')
        {
            in.seekg(loc);
        }
        else
        {
            LOG("got comment:" << comment);
        }
    }

    in >> width >> height >> maxcol;
    in.get();                   // eat newline
    LOG("dimensions: " << width << "x" << height << "("<<maxcol<<")");


    data = new char[width*height*3];
    in.read(data, width*height*3);
    in.close();

    r.resize(width*height);
    g.resize(width*height);
    b.resize(width*height);

    for (int i=0; i<width*height; ++i)
    {
        int base = i*3;
        r[i] =  ((unsigned char)data[base+0])/255.0f;
        g[i] =  ((unsigned char)data[base+1])/255.0f;
        b[i] =  ((unsigned char)data[base+2])/255.0f;
    }
    free(data);

    return 1;
}

// write couresy of Dr. Tanis
int write(std::string outfile,
          int width, int height,
          const std::vector<float>& r,
          const std::vector<float>& g,
          const std::vector<float>& b)
{
    std::ofstream ofs(outfile.c_str(), std::ios::out | std::ios::binary);

    if (! ofs.is_open())
    {
        std::cerr << "could not open " << outfile << " for writing" << std::endl;
    }

    ofs << "P6\n#*\n" << width << " " << height << "\n255\n";

    for (int i=0; i < width*height; ++i)
    {
        ofs <<
            (unsigned char)(r[i]*255) <<
            (unsigned char)(g[i]*255) <<
            (unsigned char)(b[i]*255);
    }
    ofs.close();

    return 1;
}

//blur courtesy of Dr. Tanis
__global__
void blur(int width, int height,
          float* r, float* g, float* b,
          float* r_o, float* g_o, float* b_o,
          float weight)
{
    int x_idx = threadIdx.x + blockIdx.x * blockDim.x;
    int y_idx = threadIdx.y + blockIdx.y * blockDim.y;

    if (x_idx < width && y_idx < height)
    {

        // active CUDA thread

        int s_idx = x_idx + y_idx*width;

        int nnbr = 0;
        float sum_r=0, sum_g=0, sum_b=0;

        if (x_idx > 0)
        {
            // left
            nnbr++;
            int left = s_idx-1;

            sum_r += r[left];
            sum_g += g[left];
            sum_b += b[left];
        }

        if (x_idx < width-1)
        {
            //right
            nnbr++;
            int right = s_idx+1;
            sum_r += r[right];
            sum_g += g[right];
            sum_b += b[right];
        }

        if (y_idx > 0)
        {
            //up
            nnbr++;
            int up = s_idx - width;
            sum_r += r[up];
            sum_g += g[up];
            sum_b += b[up];
        }

        if (y_idx < height-1)
        {
            //up
            nnbr++;
            int down = s_idx + width;
            sum_r += r[down];
            sum_g += g[down];
            sum_b += b[down];
        }

        if (nnbr > 0)
        {
            r_o[s_idx] = r[s_idx]*weight + sum_r * (1-weight) / nnbr;
            g_o[s_idx] = g[s_idx]*weight + sum_g * (1-weight) / nnbr;
            b_o[s_idx] = b[s_idx]*weight + sum_b * (1-weight) / nnbr;
        }
        else
        {
            r_o[s_idx] = r[s_idx];
            g_o[s_idx] = g[s_idx];
            b_o[s_idx] = b[s_idx];
        }
    }
}

__global__
void edgedetect(int width, int height,
          float* r, float* g, float* b,
          float* r_o, float* g_o, float* b_o,
          float threshold)
{
    int x_idx = threadIdx.x + blockIdx.x * blockDim.x;
    int y_idx = threadIdx.y + blockIdx.y * blockDim.y;

    if (x_idx < width && y_idx < height)
    {
        int s_idx = x_idx + y_idx*width;
        int nnbr = 0, d_x = 0, d_y = 0;
        float diff = 0, sum_left = 0, sum_right = 0, sum_up = 0, sum_down = 0;

        if (x_idx > 0)
        {
            // left
            nnbr++;
            int left = s_idx-1;
            sum_left += r[left] + g[left] + b[left];
        }

        if (x_idx < width-1)
        {
            //right
            nnbr++;
            int right = s_idx+1;
            sum_right += r[right] + g[right] + b[right];
        }

        if (y_idx > 0)
        {
            //up
            nnbr++;
            int up = s_idx - width;
            sum_up += r[up] + g[up] + b[up];
        }

        if (y_idx < height-1)
        {
            //down
            nnbr++;
            int down = s_idx + width;
            sum_down += r[down] + g[down] + b[down];
        }


        if (nnbr > 0)
        {
            d_x = (-0.5 * sum_left) + (-0.5 * sum_right);
            d_y = (-0.5 * sum_down) + (-0.5 * sum_up);
            diff = sqrtf((powf(d_x, 2)) + (powf(d_y, 2)));

            if (diff >= threshold) {
              r_o[s_idx] = 1;
              g_o[s_idx] = 1;
              b_o[s_idx] = 1;
            }
            else
            {
              r_o[s_idx] = 0;
              g_o[s_idx] = 0;
              b_o[s_idx] = 0;
            }
        }
        else
        {
            r_o[s_idx] = r[s_idx];
            g_o[s_idx] = g[s_idx];
            b_o[s_idx] = b[s_idx];
        }
    }
}

__global__
void sinewave(int width, int height,
          float* r_o, float* g_o, float* b_o)
{
  int x_idx = threadIdx.x + blockIdx.x * blockDim.x;
  int y_idx = threadIdx.y + blockIdx.y * blockDim.y;
  float radian = (x_idx) * (3.14/180);
  float sineVal = sin(radian);

  if (x_idx < width && y_idx < height)
  {
      int s_idx = x_idx + y_idx*width;
      float sineAlter = ((sineVal * x_idx) + (height / 4));
      int numSin = (int)sineAlter;

        if (y_idx >= sineAlter) {
          r_o[s_idx] = 0;
          g_o[s_idx] = sineAlter - numSin;
          b_o[s_idx] = sineAlter - numSin / 2;
        }
        else
        {
          r_o[s_idx] = 0.75;
          g_o[s_idx] = 0.75;
          b_o[s_idx] = 0.75;
        }
  }
}


__global__
void flippy(int width, int height,
          float* r, float* g, float* b,
          float* r_o, float* g_o, float* b_o)
{
    int x_idx = threadIdx.x + blockIdx.x * blockDim.x;
    int y_idx = threadIdx.y + blockIdx.y * blockDim.y;
    int s_idx = x_idx + y_idx * width;
    int xdi_s = -x_idx + (-1 * (y_idx)) * width;
    //int half = (width * height) / 2;

    if (x_idx < width && y_idx < height)
    //if (s_idx >= half)
    {
        r_o[xdi_s] = r[s_idx];
        g_o[xdi_s] = g[s_idx];
        b_o[xdi_s] = b[s_idx];
    }
}
