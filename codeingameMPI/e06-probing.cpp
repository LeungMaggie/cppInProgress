#include <iostream>
#include <mpi.h>

/**
 * * int MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status* status)
 * source can be MPI_ANY_SOURCE
 * tag can be MPI_ANY_TAG
 * RAPPEL:
 * struct MPI_Status
 * {
 *    int MPI_SOURCE;
 *    int MPI_TAG;
 *    int MPI_ERROR;
 *    int _cancelled;
 *    size_t _ucount;
 * }
*/

// * What is written in the website & able to run **************
void probing_process_web(int& intSum, float& floatSum)
{
  MPI_Status status;

  // 1- Probe the incoming message
  MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

  // 2- Get the tag and the source
  int tag = status.MPI_TAG;
  int source = status.MPI_SOURCE;

  // Printing the message
  std::cout << "Received a message from process " << source << " with tag " << tag << std::endl;

  // 3- Add to intSum or floatSum depending on the tag of the message
  //    If tag == 0, receive as a single integer and add it to intSum;
  //    If tag == 1, receive as a single float and add it to floatSum;
  int in = 0;
  float fp = 0.0;
  if (tag == 0)
    MPI_Recv(&in, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
  else if (tag == 1)
    MPI_Recv(&fp, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
  
  intSum += in;
  floatSum += fp;
}
// *************************************************************

void probing_process(int& intSum, float& floatSum)
{
  MPI_Status status;

  // 1- Probe the incoming message
  MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

  // 2- Get the tag and the source
  int tag = status.MPI_TAG;
  int source = status.MPI_SOURCE;

  // Printing the message
  std::cout << "RRRRRRRRRRRRReceived a message from process " << source << " with tag " << tag << std::endl;

  int tmpInt;
  float tmpFloat;
  // 3- Add to intSum or floatSum depending on the tag of the message
  //    If tag == 0, receive as a single integer and add it to intSum;
  //    If tag == 1, receive as a single float and add it to floatSum;
  if (tag == 0)
  {
    MPI_Recv(&tmpInt, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
    std::cout << "rrrrrrrrrrrrreceived int: " << tmpInt << std::endl;
  }
  else if (tag == 1)
  {
    MPI_Recv(&tmpFloat, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
    std::cout << "rrrrrrrrrrrrreceived float: " << tmpFloat << std::endl;
  }
  else
    std::cout << "hey what's wrong, tag: " << tag << std::endl;
  intSum += tmpInt;
  floatSum += tmpFloat;
  std::cout << "iiiiiiiiiiiiiiintSum: " << intSum << ", floatSum: " << floatSum << std::endl;
}

// ! problem in adding intSum & floatSum, donno why, maybe should use MPI_Reduce?
void send_recv(int argc, char** argv, int& intSum, float& floatSum, int& intSumFinal, float& floatSumFinal)
{
  MPI_Init(&argc, &argv);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
  {
    probing_process(intSum, floatSum);
  }
  if (rank % 2 != 0)
  {
    int numInt = rank + size * 2;
    std::cout << "TTTThis is process " << rank << " out of " << size << " with numInt: " << numInt << std::endl;
    MPI_Send(&numInt, 1, MPI_INT, 0, rank % 2, MPI_COMM_WORLD);
  }
  else if (rank % 2 == 0)
  {
    float numFloat = rank + size * 2.333;
    std::cout << "This is processsss " << rank << " out of " << size << " with numFloat: " << numFloat << std::endl;
    MPI_Send(&numFloat, 1, MPI_FLOAT, 0, rank % 2, MPI_COMM_WORLD);
  }

  MPI_Reduce(&intSum, &intSumFinal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce(&floatSum, &floatSumFinal, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Finalize();
}

int main(int argc, char** argv)
{
  int intSum = 0;
  float floatSum = 0.0;
  int intSumFinal = 0;
  float floatSumFinal = 0.0;
  send_recv(argc, argv, intSum, floatSum, intSumFinal, floatSumFinal);
  std::cout << "wowowow intSumFinal: " << intSumFinal << ", floatSumFinal: " << floatSumFinal << std::endl;
  return 0;
}
