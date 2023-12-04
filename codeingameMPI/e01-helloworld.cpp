#include <iostream>
#include <mpi.h>

/**
 * MPI must always be initialized and finalized, both operations
 * must be the first and last calls of your code, always.
 * The corresponding commands are MPI_Init and MPI_Finalize.
 * Their signatures:
 * int MPI_Init(int* argc, char*** argv);
 * int MPI_Finalize();
 * 
 * The way to obtain rank and size:
 * int size, rank;
 * MPI_Comm_size(MPI_COMM_WORLD, &size);
 * MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 * 
 * * Note: 
 * * MPI_COMM_WORLD indicates that the operation is done on the
 * * global communicator that encapsulates every single process
 * * of the current run.
 * 
 * Compile with:
 * mpicxx -o ./bin/helloworld ./helloworld.cpp
 * 
 * *note: "mpicxx --show" shows the actual command line
 * 
 * Run with:
 * mpirun -np 4 ./bin/helloworld
 * (indicates that the program will be run across 4 processes)
*/

int main(int argc, char** argv)
{
  // Initialization
  MPI_Init(&argc, &argv);

  // Reading size and rank
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Printing
  std::cout << "Hello world, from process #" << rank << " with size: " << size << std::endl;

  // Finalization
  MPI_Finalize();

  return 0;
}
