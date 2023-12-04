#include <iostream>
#include <vector>

__global__ void vec_add(float* a, float* b, float* c)
{
  int i = threadIdx.x;
  c[i] = a[i] + b[i];
}

// int main()
// {
//   std::vector<float> a, b, c;
//   int n = 100;
//   vec_add<<<1, n>>>(a.data(), b.data(), c.data());    // <<<nb blocks per grid, nb threads per block>>>
// }

/////////////////////////////////
constexpr int N = 128;

__global__ void mat_add(float a[N][N], float b[N][N], float c[N][N])
{
  int i = threadIdx.x;
  int j = threadIdx.y;
  c[i][j] = a[i][j] + b[i][j];
}

int main()
{
  // one block of N * N * 1 threads
  int numBlocks = 1;
  dim3 threadsPerBlock(N, N);
  // mat_add<<<numBlocks, threadsPerBlock>>>(a, b, c);
}
