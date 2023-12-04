#include <iostream>
#include <array>
#include <mpi.h>

/* MPI_Status
   struct ompi_status_public_t {
    * These fields are publicly defined in the MPI specification.
    * User applications may freely read from these fields.
    int MPI_SOURCE;
    int MPI_TAG;
    int MPI_ERROR;
    * The following two fields are internal to the Open MPI
    * implementation and should not be accessed by MPI applications.
    * They are subject to change at any time.  These are not the
    * droids you're looking for. 
    int _cancelled;
    size_t _ucount;
  };
*/

// ! run with mpirun -np 2 ./bin/mpistatus

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  std::array<double, 3> values;
  std::array<double, 3> wowowo;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Status status;
  if (rank == 0)
  {
    values = {2, 4, 6};
    MPI_Send(values.data(), 3, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(wowowo.data(), 3, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    std::cout << "Process #" << rank << " values: " << values[0] << " " << values[1] << " " << values[2] << std::endl;
    std::cout << "Process #" << rank << " wowowo: " << wowowo[0] << " " << wowowo[1] << " " << wowowo[2] << std::endl;
    std::cout << "Received from process #" << status.MPI_SOURCE << " with tag " << status.MPI_TAG << std::endl;
  }
  else
  {
    values = {1, 3, 5};
    MPI_Recv(wowowo.data(), 3, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Send(values.data(), 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    std::cout << "Process #" << rank << " values: " << values[0] << " " << values[1] << " " << values[2] << std::endl;
    std::cout << "Process #" << rank << " wowowo: " << wowowo[0] << " " << wowowo[1] << " " << wowowo[2] << std::endl;
    std::cout << "Received from process #" << status.MPI_SOURCE << " with tag " << status.MPI_TAG << std::endl;
  }

  MPI_Finalize();
  return 0;
}
