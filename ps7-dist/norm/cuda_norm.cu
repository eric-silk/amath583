#include "Timer.hpp"
#include <assert.h>
#include <iostream>
#include <vector>

#undef NDEBUG

// Sequential norm, for validation
float norm(const std::vector<float>& v) {
  float sum = 0.0;
  for (size_t i = 0; i < v.size(); ++i){
    sum += v[i] * v[i];
  }
  return std::sqrt(sum);
}

template <class T>
__global__ void
noblock_sq(unsigned int array_size, T *x, T *y){
  size_t i = blockDim.x * blockIdx.x + threadIdx.x;
  if (i<array_size) y[i] = x[i] * x[i];
}

__global__ void
sq(float *g_idata, float *g_odata){
  extern __shared__ float sdata[];

  size_t tid = threadIdx.x;
  size_t i = blockIdx.x * blockDim.x + threadIdx.x;

  sdata[tid] = (g_idata[i] * g_idata[i]);
  __syncthreads();

  for (size_t s = 1; s<blockDim.x; s*=2){
    size_t index = 2*s*tid;
    if (index < blockDim.x) {
      sdata[index] += sdata[index + s];
    }
    __syncthreads();
  }

  if (tid == 0){
    g_odata[blockIdx.x] = sdata[0];
  }
}


int main(int argc, char* argv[]) {

  size_t exponent           = 27;
  size_t num_trips          = 1;

  if (argc >= 2) exponent   = std::stol(argv[1]);
  if (argc >= 3) num_trips  = std::stol(argv[2]);

  const size_t N = 1 << exponent;
  int block_size = 256;
  int num_blocks = (N + block_size - 1) / block_size;

  float *x = nullptr, *y = nullptr;

  cudaMallocManaged(&x, N*sizeof(float));
  cudaMallocManaged(&y, num_blocks*sizeof(float));

  for (int i = 0; i < N; i++) {
    x[i] = i;
  }

  std::vector<float> sequential_vector(x, x+N);
  for (int i = 0; i < N; i++)
  {
    assert(x[i] == sequential_vector[i]);
  }

  DEF_TIMER(cuda_norm);
  START_TIMER(cuda_norm);
  cudaDeviceSynchronize();
  for (size_t i = 0; i < num_trips; ++i) {
    sq<<<num_blocks, block_size, block_size*sizeof(float)>>>(x, y);
    cudaDeviceSynchronize();
  }
  /* write me: final step, copy out values from y and add on cpu */
  float result = 0.0;
  for (size_t i = 0; i < num_blocks; ++i)
  {
    result += y[i];
  }

  result = std::sqrt(result);

  double cuda_time = STOP_TIMER_QUIETLY(cuda_norm);
  std::cout << exponent << "\t" << num_trips << "\t" << cuda_time << "\t" << result << std::endl;

  // this WILL fail for exponents above 8 or so. Floating point error is a bitch.
  assert(norm(sequential_vector) == result);

  cudaFree(x); cudaFree(y);
  
  return 0;
}
