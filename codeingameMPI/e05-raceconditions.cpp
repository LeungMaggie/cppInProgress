// ! no waiting, no testing
#include <iostream>
#include <unistd.h>
#include <mpi.h>

/**
 * Result:
 * Process 0 is sending : 0 1 4 9 16 25 36 49 64 81 
 * Process 1 received : 0 0 0 0 0 0 0 0 0 0
 * 因为在没有wait或test的情况下，process1比process0快(1已经在receive，但0还没send，所以1没有收到任何，直接输出了新修改的buffer数据)
 * YOU HAVE TO CALL MPI_Wait at some point, or MPI_Test until the request is processed
*/

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  MPI_Request request;
  MPI_Status status;
  int requestComplete = 0;

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_rank(MPI_COMM_WORLD, &size);

  constexpr int bufferCount = 10;
  int buffer[bufferCount];

  // rank 0 sends, rank 1 receives
  if (rank == 0)
  {
    // filling the buffer
    std::cout << "Process 0 is sending : ";
    for (int i = 0; i < bufferCount; ++i)
    {
      buffer[i] = i * i;
      std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
    // sending the data and waiting for 5 seconds
    MPI_Isend(buffer, bufferCount, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
    // MPI_Wait(&request, &status); // 在本例中有无wait不影响结果
    sleep(5);
  }
  else
  {
    // resetting the buffer
    for (int i = 0; i < bufferCount; ++i)
      buffer[i] = 0;
    
    // receiving and sleeping for 5 seconds
    MPI_Irecv(buffer, bufferCount, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);  // 因为rank0比较慢，所以此处必须wait until 0 is prepared to send，在sleep(5)前后都可以
    sleep(5);

    // printing the buffer received by process 1
    int ite = 0;
    std::cout << "Process 1 received : ";
    for (int i = 0; i < bufferCount; ++i)
      std::cout << buffer[i] << " ";
    std::cout << std::endl;
  }

  MPI_Finalize();
  return 0;
}
