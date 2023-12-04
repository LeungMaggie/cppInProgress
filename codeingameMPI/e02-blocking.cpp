/* POINT-TO-POINT COMMUNICATION*/
/** P2P communications are divided into 2 operations: Send & Receive
 * The most basic forms of P2P communication are called blocking communications
 * The process sending a mesage will be waiting until the process receiving
 * has finished receiving all the information.
 * 
 * * Sending / Receiving Messages
 * A send operation, sends a buffer of data of a certain type to another process
 * A P2P message has the following properties:
 * * A reference to a buffer
 *  >> a pointer to a buffer, this array will hold the data that you wish to
 *  >> send from the current process to another
 * * A datatype
 *  >> the datatype must correspond precisely to the data stored in the buffer
 *  >> e.g. char -> MPI_CHAR, int -> MPI_INT, float -> MPI_FLOAT, double -> MPI_DOUBLE
 * * A number of elements
 *  >> the number of elements in the buffer that you want to send to the destination
 * * A tag
 *  >> a simple integer that identifies the "type" of communication
 * * A destination id (Send) / A source id (Receive)
 *  >> the rank of the process you want to send the data to
 * * A communicator
 *  >> the communicator on which to send the data to
 *  >> the rank of precesses might change depending on the communicator you're choosing
 * 
*/

/**
 * ! Send
 * * int MPI_Send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
 * ? Examples:
 * * Sending an integer to process 2:
 * int myVal = 10;
 * MPI_Send(&myVal, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
 * * Sending a table of integer to process 1:
 * int values[5] = {1, 2, 3, 4, 5};
 * MPI_Send(values, 5, MPI_INT, 1, 0, MPI_COMM_WORLD);
 * * Sending a table of doubles to process 10 with a specific tag 6:
 * double values[3] = {1e25, -0.0, M_PI};
 * MPI_Send(values, 3, MPI_DOUBLE, 10, 6, MPI_COMM_WORLD);
 * ! Receive
 * * int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status* status);
 * MPI_STATUS_IGNORE: ignore status
 * MPI_ANY_SOURCE: instead of precise id for the source of the message, indicates that you're ready to receive a message from any source
 * MPI_ANY_TAG: for any tag
 * ? Examples: 
 * * Receiving an integer from process 5 with tag 0:
 * int value;
 * MPI_Recv(&value, 1, MPI_INT, 5, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
 * * Receiving a table of 10 integers from process 3 with no tag:
 * int values[10];
 * MPI_Recv(&values, 10, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
 * * Receiving a table of 3 doubles from any process with any tag:
 * double values[3];
 * MPI_Recv(&values, 3, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
 */

#include <iostream>
#include <mpi.h>
#include <cstdlib>

/** The program will be ran with 2 processes.
 * Your program will be given 2 random integers on the command line
 * and read into a variable localValue
*/

// ! run with mpirun -np 2 ./bin/blocking
int main (int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Read the local value of the process
  // localValue will hold a specific int for process 0, and another for process 1
  int localValue;
  // localValue = atoi(argv[1]);

  int otherValue;

  if (rank == 0)
  {
    /** Here, enter the code for the first process:
     * *1. Send the value to process 1
     * *2. Receive the value from process 1 (in otherValue)
     * *3. Print the sum of the two values on stdout
    */
    localValue = atoi(argv[1]);
    MPI_Send(&localValue, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&otherValue, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "localValue: " << localValue << ", otherValue: " << otherValue << std::endl;
    std::cout << "Sum of the 2 values: " << localValue + otherValue << std::endl;
  }
  else
  {
    /** Here, enter the code for the second process:
     * *1. Receive the value from process 0 (in otherValue)
     * *2. Send the value to process 0
     * *3. Print the product of the two values on stdout
    */
    localValue = atoi(argv[2]);
    MPI_Recv(&otherValue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(&localValue, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    std::cout << "localValue: " << localValue << ", otherValue: " << otherValue << std::endl;
    std::cout << "Product of the 2 values: " << localValue * otherValue << std::endl;
  }

  MPI_Finalize();

  return 0;
}
