// command to compile this file:
// mpicxx -fopenmp -std=c++17 -o ompMpiTest test.cpp -lgomp

// set number of threads to use for parallel regions:
// export OMP_NUM_THREADS=n

#include <iostream>
#include <mpi.h>
#include <thread>
#include <sstream>
#include <omp.h>

int main(int args, char* argv[])
{
  int rank, nprocs, threadId, nThreads, cxxProcs;
  MPI_Init(&args, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  #pragma omp parallel private(threadId, nThreads, cxxProcs)
  {
    threadId = omp_get_thread_num();
    nThreads = omp_get_num_threads();
    cxxProcs = std::thread::hardware_concurrency();
    std::stringstream ompStream;
    ompStream << "I'm thread " << threadId
              << " out of "    << nThreads
              << " on MPI process nr. " << rank
              << " out of " << nprocs
              << ", while hardware_concurrency reports " << cxxProcs
              << " processors\n";
    std::cout << ompStream.str();
  }

  MPI_Finalize();
  return 0;
}
