#include <iostream>
#include <iomanip>
#include <mpi.h>

/**
 * Broadcasting function:
 * * int MPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
 * 
 * non-blocking version:
 * * int MPI_Ibcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm, MPI_Request request);
 * root is the id of the process sending the data
 * should also be followed by MPI_Test and MPI_Wait
 * 
*/

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // input reading for process 0
  int nElements;
  double* buffer;
  if (rank == 0)
  {
    std::cin >> nElements;
    buffer = new double[nElements];
  
    for (int i = 0; i < nElements; ++i)
      std::cin >> buffer[i];
  }

  // 1- Broadcast the value nElements to all processes
  MPI_Bcast(&nElements, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // create buffers for the non-root processes now that we have nElements
  if (rank != 0)
    buffer = new double[nElements];
  

  // 2- Broadcast the buffer to every process
  //    NOTE: the type here should be MPI_DOUBLE not MPI_FLOAT!
  MPI_Bcast(buffer, nElements, MPI_DOUBLE, 0, MPI_COMM_WORLD);


  // 3- Compute the sum depending on the rank
  double sum = 0.0;
  if (rank == 0)
  {
    std::cout << "Process #0 buffer: ";
    for (int i = 0; i < nElements; ++i)
    {
      sum += buffer[i];
      std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
  }
  else if (rank == 1)
  {
    std::cout << "Process #1 positive elements: ";
    for (int i = 0; i < nElements; ++i)
    {
      if (buffer[i] > 0.0)
      {
        sum += buffer[i];
        std::cout << buffer[i] << " ";
      }
    }
    std::cout << std::endl;
  }
  else if (rank == 2)
  {
    std::cout << "Process #2 negative elements: ";
    for (int i = 0; i < nElements; ++i)
    {
      if (buffer[i] < 0.0)
      {
        sum += buffer[i];
        std::cout << buffer[i] << " ";
      }
    }
    std::cout << std::endl;
  }

  // Printing the result and terminating the program
  // Precision is set high for the validation process, please do not modify this
  std::cout << std::setprecision(16) << sum << std::endl;

  MPI_Finalize();
  delete[] buffer;
  return 0;
}
