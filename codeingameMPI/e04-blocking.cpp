#include <iostream>
#include <array>
#include <mpi.h>
#include <chrono>
#include <thread>

// This file is provided as an example of blocking scenario
// Please don't modify it!

void sleep(int64_t second)
{
  std::chrono::seconds duration(second);
  std::this_thread::sleep_for(duration);
}

void play_blocking_scenario(int* buffer, const int& rank, const int& bufferCount)
{
  // Initializing buffer:
  for (int i = 0; i < bufferCount; ++i)
    buffer[i] = (rank == 0 ? i * 2 : 0);
  
  MPI_Barrier(MPI_COMM_WORLD);
  // Starting the chonometer
  double time = -MPI_Wtime(); // This command helps us measuring time
  // Simulate working
  if (rank == 0)
  {
    sleep(3);
    MPI_Send(buffer, bufferCount, MPI_INT, 1, 0, MPI_COMM_WORLD);
    sleep(6);

    // Modifying the buffer for second step
    for (int i = 0; i < bufferCount; ++i)
      buffer[i] = -i;
    
    MPI_Send(buffer, bufferCount, MPI_INT, 1, 1, MPI_COMM_WORLD);
  }
  else
  {
    sleep(5);
    MPI_Recv(buffer, bufferCount, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    sleep(3);
    MPI_Recv(buffer, bufferCount, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  time += MPI_Wtime();

  // This line gives us the maximum time elapsed on each process
  // We will see about reduction later on!
  double finalTime;
  MPI_Reduce(&time, &finalTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0)
    std::cout << "Total time for blocking scenario : " << finalTime << "s" << std::endl;
}

int main(int argc, char** argv)
{
  int rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int bufferCount = 10000;
  std::array<int, bufferCount> buffer;
  play_blocking_scenario(buffer.data(), rank, bufferCount);
  MPI_Finalize();
  return 0;
}
