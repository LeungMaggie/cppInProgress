/**
 * Communication modes
*/

/**
 * The 1st send is completed way before the 1st receive,
 * while for the large buffer, both operations seem to be happening at the same time
 * 
 * Time elapsed to complete blocking send 1: 3.02e-05 s
 * (经过8秒...)
 * Just received buffer 1
 * (经过8秒...)
 * Time elapsed to complete blocking send 2 : 8.00028 s
 * Just received buffer 2
 * 
*/

#include <iostream>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // We have 2 buffers, one big and one small
  constexpr int smallCount = 50;
  constexpr int bigCount = 100000;
  int buff1[smallCount];
  int buff2[bigCount];

  // Waiting time, play with this value if you think the difference is due to the quantity of data to send
  constexpr int waitTime = 8; // in seconds

  if (rank == 0)
  {
    // initialize both buffers
    for (int i = 0; i < bigCount; ++i)
    {
      if (i < smallCount)
        buff1[i] = i;
      buff2[i] = i;
    }

    // Synchronisation 1
    MPI_Barrier(MPI_COMM_WORLD);

    // register the current time
    double time = -MPI_Wtime();

    // send the buffer immediately
    MPI_Send(buff1, smallCount, MPI_INT, 1, 0, MPI_COMM_WORLD);

    // print the time it took us to complete this send
    std::cout << "Time elapsed to complete blocking send 1: " << time + MPI_Wtime() << " s" << std::endl;

    // Synchronisation 2
    MPI_Barrier(MPI_COMM_WORLD);
    time = -MPI_Wtime();

    // send the second buffer
    MPI_Send(buff2, bigCount, MPI_INT, 1, 1, MPI_COMM_WORLD);

    // print the time again
    std::cout << "Time elapsed to complete blocking send 2 : " << time + MPI_Wtime() << " s" << std::endl;
  }
  else
  {
    // Synchronisation 1
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(waitTime);

    // Receiving buffer 1
    MPI_Recv(buff1, smallCount, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "Just received buffer 1" << std::endl;

    // Synchronisation 2
    MPI_Barrier(MPI_COMM_WORLD);
    sleep(waitTime);

    // Receiving buffer 2
    MPI_Recv(buff2, bigCount, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "Just received buffer 2" << std::endl;
  }

  MPI_Finalize();

  return 0;
}


