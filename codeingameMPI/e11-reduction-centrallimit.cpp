#include <iostream>
#include <cstdlib>
#include <mpi.h>

/**
 * int MPI_Reduce(void* send_data, void* recv_data,
 *                int count, MPI_Datatype type, MPI_Op op,
 *                int root, MPI_Comm comm);
*/

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  constexpr int bufferCount = 5000;
  float buffer[bufferCount];
  memset(buffer, 0, sizeof(buffer));

  // uniform sampling, generating the numbers and doing 1000 repetitions
  for (int rep = 0; rep < 1000; ++rep)
  {
    for (int i = 0; i < bufferCount; ++i)
    {
      float val = (float)rand() / RAND_MAX;
      buffer[i] += val;
    }
  }

  /**
   * *create a buffer called reception and call MPI_Reduce to sum all the variables
   * *overall the processes and store the result on process 0
   * in the end, you should have bufferCount variables
  */
  float reception[bufferCount];
  MPI_Reduce(buffer, reception, bufferCount, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  
  // now we print the results
  if (rank == 0)
  {
    for (int i = 0; i < bufferCount; ++i)
      std::cout << reception[i] << std::endl;
  }
  MPI_Finalize();
  return 0;
}

