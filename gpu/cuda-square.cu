/*
Youtube channel: Tom Nurkkala
Intro to GPU Programming
*/

// GPU code
__global__ void square(float* d_out, float* d_in)
{
  int idx = threadIdx.x;      // threadIdx.x : from 0 to number of threads
  float f = d_in[idx];
  d_out[idx] = f * f;
}

// CPU code
int main(int argc, char** argv)
{
  const int arraySize = 64;
  const int arrayBytes = arraySize * sizeof(float);

  // declare and initialize CPU arrays
  float h_in[arraySize];    // h_ stands for host
  float h_out[arraySize];
  for (int i = 0; i < arraySize; ++i)
  {
    h_in[i] = float(i);
  }

  // declare and allocate GPU memory
  float* d_in;              // d_ stands for device
  float* d_out;
  /**
   * We don't get a pointer back from cudaMalloc that refers to that memory on the GPU.
   * Instead, we pass the address of a variable, that's gonna be filled in by cudaMalloc
   * with information that we can use to refer to the memory on the graphics card.
   * Notice that we don't have direct access to that. We have to access the memory on the GPU 
   * by using these cuda function calls. We can't just reference that address because it's in 
   * a completely separate memory. It's kinda like we can't reference a memory on a different
   * processor when we're doing MPI, while we have to do some communication. In a sense, when 
   * we use these cuda function calls, to move information from a CPU memory to a GPU memory
   * and back, we're doing a similar kind of operation - we don't have direct access to that 
   * memory dans le sense de just putting an address out on the address bus of the motherboard
   * We have access to it by merit of making a function call.
   * So what's coming back from cudaMalloc here, is not a pointer as such.
   * It's sort of a memory reference that is only meaningful on GPU memory
   * cudaMalloc([address of a pointer, (void**) makes the compiler happy], memory bytes)
  */
  cudaMalloc((void**)&d_in, arrayBytes);
  cudaMalloc((void**)&d_out, arrayBytes);


  // Copy over - Compute - Copy back
  // copy array to GPU
  cudaMemcpy(d_in, h_in, arrayBytes, cudaMemcpyHostToDevice);
  /**
   * cudaMemcpy(
   * [reference that we have on the CPU side to memory that was actually allocated on the GPU],
   * [an address on the CPU side],
   * [how many bytes do we want to move from h_in to d_in],
   * [which direction we're going])
  */ 

  // launch the kernel
  /**
   * we want to spin it up on arraySize threads
   * we're gonna have one of those square functions running on each of 64 threads
   * (allocated one thread per element of the array)
  */
  square<<<1, arraySize>>>(d_out, d_in);

  // copy results back from GPU
  cudaMemcpy(h_out, d_out, arrayBytes, cudaMemcpyDeviceToHost);

  // Clean up
  // release GPU memory
  cudaFree(d_in);
  cudaFree(d_out);
}

