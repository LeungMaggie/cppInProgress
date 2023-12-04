#include <iostream>
#include <mpi.h>

// Collective example 1
int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // initializing the data
  const int bufferSize = 5;
  int buffer[bufferSize];

  // **************** collective example 1 **************** //
  // if (rank == 0)
  // {
  //   //* storing some values in the buffer
  //   for (int i = 0; i < bufferSize; ++i)
  //     buffer[i] = i * i;
    
  //   for (int idDest = 1; idDest < size; ++idDest)
  //     MPI_Send(buffer, bufferSize, MPI_INT, idDest, 0, MPI_COMM_WORLD);
  // }
  // else
  // {
  //   MPI_Recv(buffer, bufferSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  // }
  // ****************************************************** //


  /**
   * Does the exact same thing, but uses only one simple call to an MPI function
  */
  // **************** collective example 2 **************** //
  if (rank == 0)
  {
    // storing some values in the buffer
    for (int i = 0; i < bufferSize; ++i)
      buffer[i] = i * i;
  }
  MPI_Bcast(buffer, bufferSize, MPI_INT, 0, MPI_COMM_WORLD);
  // ****************************************************** //

  std::cout << "Process #" << rank << "; Buffer = (";
  for (int i = 0; i < bufferSize; ++i)
    std::cout << buffer[i] << (i < bufferSize - 1 ? " " : "");
  std::cout << ")" << std::endl;

  MPI_Finalize();
  return 0;
}
