#include <iostream>
#include <array>
#include <mpi.h>
#include <chrono>
#include <thread>

/**
 * Algorithm:
 * if rank == 0 then
 *  work for 3 seconds
 *  initialize the send to process 1
 *  work for 6 seconds
 *    every mili-second, test if process 1 is ready to communicate
 *  send the second batch data to process 1
 *  wait for process 1 to receive the data
 * else if rank == 1 then
 *  work for 5 seconds
 *  initialize receive from process 0
 *  wait for a communication from process 0
 *  work for 3 seconds
 *  initialize receive from process 0
 *  wait for a communication from process 0
 * 
*/

// ! run with mpirun -np 2 ./bin/e04nonblocking
void print_buffer(const int* buffer, const int& bufferCount)
{
  std::cout << "buffer: ";
  for (int i = 0; i < bufferCount; ++i)
    std::cout << buffer[i] << "  ";
  std::cout << std::endl;
}

void play_non_blocking_scenario(int* buffer, const int& rank, const int& bufferCount)
{
  MPI_Request request;
  MPI_Status status;
  int requestFinished = 0;

  // Initializing buffer:
  for (int i = 0; i < bufferCount; ++i)
  {
    buffer[i] = (rank == 0 ? i * 2 : 0);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  // Starting the chronometer
  double time = -MPI_Wtime(); // This command helps us measure time

  // should not modify anything before this point //
  if (rank == 0)
  {
    std::chrono::seconds duration1(3);
    std::this_thread::sleep_for(duration1);
    // sleep(5);
    // 1- Initialize the non-blocking send to process 1
    MPI_Isend(buffer, bufferCount, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

    double timeLeft = 6000.0;
    while (timeLeft > 0.0)
    {
      std::chrono::microseconds duration2(1);
      std::this_thread::sleep_for(duration2);
      // usleep(1000); // We work for 1ms

      // 2- Test if the request is finished (only if not already finished)
      MPI_Test(&request, &requestFinished, &status);

      // 1ms left to work
      timeLeft -= 1000.0;
    }

    // 3- If the request is not yet complete, wait here
    MPI_Wait(&request, &status);

    // Modifying the buffer for second step
    for (int i = 0; i < bufferCount; ++i)
      buffer[i] = -i;
    
    // 4- Prepare another request for process 1 with a different tag
    MPI_Isend(buffer, bufferCount, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);

    timeLeft = 3000.0;
    while (timeLeft > 0.0)
    {
      std::chrono::milliseconds duration3(1);
      std::this_thread::sleep_for(duration3);
      // usleep(1000); // We work for 1ms

      // 5- Test if the request is finished (only if not already finished)
      MPI_Test(&request, &requestFinished, &status);

      // 1ms left to work
      timeLeft -= 1000.0;
    }

    // 6- Wait for it to finish
    MPI_Wait(&request, &status);
  }
  else
  {
    // Work for 5 seconds
    // sleep(5);
    std::chrono::seconds duration1(5);
    std::this_thread::sleep_for(duration1);

    // 7- Initialize the non-blocking receive from process 0
    MPI_Irecv(buffer, bufferCount, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

    // 8- Wait here for the request to be completed
    MPI_Wait(&request, &status);

    print_buffer(buffer, bufferCount);

    // Work for 3 seconds
    // sleep(3);
    std::chrono::seconds duration2(3);
    std::this_thread::sleep_for(duration2);

    // 9- Initialize another non-blocking receive
    MPI_Irecv(buffer, bufferCount, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);

    // 10- Wait for it to be completed
    MPI_Wait(&request, &status);

    print_buffer(buffer, bufferCount);
  }
  //********************* should not modify anything after this point *********************//

  // Stopping the chronometer
  time += MPI_Wtime();

  // This line gives us the maximum time elapsed on each process
  // We will see about reduction later on!

  double finalTime;
  MPI_Reduce(&time, &finalTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0)
    std::cout << "Total time for non-blocking scenario : " << finalTime << "s" << std::endl;

}

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int bufferCount = 10;
  std::array<int, bufferCount> buffer;
  play_non_blocking_scenario(buffer.data(), rank, bufferCount);

  MPI_Finalize();
  return 0;
}
