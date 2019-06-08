#include "Timer.hpp"
#include <iostream>

//..."borrowed" from the reduction examples.
#include "../include/reduction.h"
#include <cooperative_groups.h>

namespace cg = cooperative_groups;

// Utility class used to avoid linker errors with extern
// unsized shared memory arrays with templated type
template<class T>
struct SharedMemory
{
    __device__ inline operator       T *()
    {
        extern __shared__ int __smem[];
        return (T *)__smem;
    }

    __device__ inline operator const T *() const
    {
        extern __shared__ int __smem[];
        return (T *)__smem;
    }
};

/*
    Parallel sum reduction using shared memory
    - takes log(n) steps for n input elements
    - uses n threads
    - only works for power-of-2 arrays
*/

/* This reduction interleaves which threads are active by using the modulo
   operator.  This operator is very expensive on GPUs, and the interleaved
   inactivity means that no whole warps are active, which is also very
   inefficient */
template <class T>
__global__ void
reduce0(T *g_idata, T *g_odata, unsigned int n)
{
    // Handle to thread block group
    cg::thread_block cta = cg::this_thread_block();
    T *sdata = SharedMemory<T>();

    // load shared mem
    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x*blockDim.x + threadIdx.x;

    sdata[tid] = (i < n) ? g_idata[i] : 0;

    cg::sync(cta);

    // do reduction in shared mem
    for (unsigned int s=1; s < blockDim.x; s *= 2)
    {
        // modulo arithmetic is slow!
        if ((tid % (2*s)) == 0)
        {
            sdata[tid] += sdata[tid + s];
        }

        cg::sync(cta);
    }

    // write result for this block to global mem
    if (tid == 0) g_odata[blockIdx.x] = sdata[0];
}

/*
    This version adds multiple elements per thread sequentially.  This reduces the overall
    cost of the algorithm while keeping the work complexity O(n) and the step complexity O(log n).
    (Brent's Theorem optimization)

    Note, this kernel needs a minimum of 64*sizeof(T) bytes of shared memory.
    In other words if blockSize <= 32, allocate 64*sizeof(T) bytes.
    If blockSize > 32, allocate blockSize*sizeof(T) bytes.
*/
template <class T, unsigned int blockSize, bool nIsPow2>
__global__ void
reduce6(T *g_idata, T *g_odata, size_t n)
{
    // Handle to thread block group
    cg::thread_block cta = cg::this_thread_block();
    T *sdata = SharedMemory<T>();

    // perform first level of reduction,
    // reading from global memory, writing to shared memory
    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x*blockSize*2 + threadIdx.x;
    unsigned int gridSize = blockSize*2*gridDim.x;

    T mySum = 0;

    // we reduce multiple elements per thread.  The number is determined by the
    // number of active thread blocks (via gridDim).  More blocks will result
    // in a larger gridSize and therefore fewer elements per thread
    while (i < n)
    {
        mySum += g_idata[i];

        // ensure we don't read out of bounds -- this is optimized away for powerOf2 sized arrays
        if (nIsPow2 || i + blockSize < n)
            mySum += g_idata[i+blockSize];

        i += gridSize;
    }

    // each thread puts its local sum into shared memory
    sdata[tid] = mySum;
    cg::sync(cta);


    // do reduction in shared mem
    if ((blockSize >= 512) && (tid < 256))
    {
        sdata[tid] = mySum = mySum + sdata[tid + 256];
    }

    cg::sync(cta);

    if ((blockSize >= 256) &&(tid < 128))
    {
            sdata[tid] = mySum = mySum + sdata[tid + 128];
    }

    cg::sync(cta);

    if ((blockSize >= 128) && (tid <  64))
    {
       sdata[tid] = mySum = mySum + sdata[tid +  64];
    }

    cg::sync(cta);

    cg::thread_block_tile<32> tile32 = cg::tiled_partition<32>(cta);

    if (cta.thread_rank() < 32)
    {
        // Fetch final intermediate sum from 2nd warp
        if (blockSize >=  64) mySum += sdata[tid + 32];
        // Reduce final warp using shuffle
        for (int offset = tile32.size()/2; offset > 0; offset /= 2) 
        {
             mySum += tile32.shfl_down(mySum, offset);
        }
    }

    // write result for this block to global mem
    if (cta.thread_rank() == 0) g_odata[blockIdx.x] = mySum;
}


extern "C"
bool isPow2(unsigned int x);

__global__
void sq(int n, float *x, float *y){
  /* Write me -- kernel */
  for (size_t i = 0; i < n; ++i)
  {
    y[i] = x[i] * x[i];
  }
}

int main(int argc, char* argv[]) {

  size_t exponent           = 27;
  size_t num_trips          = 1;

  if (argc >= 2) exponent   = std::stol(argv[1]);
  if (argc >= 3) num_trips  = std::stol(argv[2]);

  size_t N = 1 << exponent;
  int const block_size = 256;
  int num_blocks = (N + block_size - 1) / block_size;

  float *x = nullptr, *y = nullptr, *result = nullptr;

  cudaMallocManaged(&x, N*sizeof(float));
  cudaMallocManaged(&y, num_blocks*sizeof(float));
  cudaMallocManaged(&result, 1*sizeof(float));

  for (int i = 0; i < N; i++) {
    x[i] = i;
  }

  DEF_TIMER(cuda_norm);
  START_TIMER(cuda_norm);
  cudaDeviceSynchronize();
  for (size_t i = 0; i < num_trips; ++i) {
    sq<<<num_blocks, block_size>>>(N, x, y);
    //reduce6<float, block_size, true><<<num_blocks, block_size>>>(y, result, N);
    reduce0<float><<<num_blocks, block_size>>>(y, result, N);
    cudaDeviceSynchronize();
  }


  double cuda_time = STOP_TIMER_QUIETLY(cuda_norm);
  std::cout << exponent << "\t" << num_trips << "\t" << cuda_time << "\t" << *result << std::endl;

  cudaFree(x); cudaFree(y);
  
  return 0;
}
