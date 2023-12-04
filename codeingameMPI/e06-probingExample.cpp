#include <iostream>
#include <cstdlib>
#include <mpi.h>

// ONE GOOD RULE TO AVOID SUPERFLUOUS OVERHEAD (REALLOCATING THE COMMUNICATION BUFFER FOR EACH CALL)
// IS TO ALLOCATE ONCE ONE OR TWO BUFFERS FOR COMMUNICATION (TYPICALLY, EITHER ONE UNIVERSAL BUFFER, OR ONE FOR SENDING AND ONE FOR RECEIVING)

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
  {
    // process 0 is sending a random number (between 10 and 25) of integers to process 1
    int nItems = rand() % 16 + 10; // BAD way of doing random
    std::cout << "process 0, random count gives us " << nItems << " ints to send." << std::endl;

    // allocation and initialization of the buffer
    int* sendBuf = new int[nItems];
    for (int i = 0; i < nItems; ++i)
      sendBuf[i] = i * i;
    
    std::cout << "process 0, sending : ";
    for (int i = 0; i < nItems; ++i)
      std::cout << sendBuf[i] << " ";
    std::cout << std::endl;

    // blocking send
    MPI_Send(sendBuf, nItems, MPI_INT, 1, 0, MPI_COMM_WORLD);

    // deallocation
    delete[] sendBuf;
  }
  else
  {
    // probing the reception of messages
    MPI_Status status;
    // blocking, so process 1 is going to wait for a message from process 0
    // the probing only informs that the process is ready to receive a communication
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);

    // from the probed status we get the number of elements to receive
    int nItems;
    MPI_Get_count(&status, MPI_INT, &nItems);

    std::cout << "process 1, probing tells us message will have " << nItems << " ints." << std::endl;

    // allocating and receiving
    int* recvBuf = new int[nItems];
    MPI_Recv(recvBuf, nItems, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    std::cout << "process 1, buffer received : ";
    for (int i = 0; i < nItems; ++i)
      std::cout << recvBuf[i] << " ";
    std::cout << std::endl;

    delete[] recvBuf;
  }

  MPI_Finalize();
  return 0;
}
