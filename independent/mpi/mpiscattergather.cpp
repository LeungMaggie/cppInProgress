#include <iostream>
#include <iomanip>
#include <cmath>
#include <mpi.h>

/**
 * MPI_Scatter(void* sendbuf, int sendcount, MPI_Datatype sendtype,
 *             void* recvbuf, int recvcount, MPI_Datatype recvtype,
 *             int root, MPI_Comm comm);
 * 
 * MPI_Gather(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
 *            void*       recvbuf, int recvcount, MPI_Datatype recvtype,
 *            int root, MPI_Comm comm);
*/

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const int allPtNum = 20;
  double* allPt = NULL;
  double* rankPt = NULL;
  const int sendrecvcount = int(ceil(allPtNum / double(size)));
  if (rank == 0)
  {
    std::cout << "allPtNum / double(size): " << allPtNum / double(size) << ", ceil(): " << ceil(allPtNum / double(size)) << std::endl;
    allPt = new double[allPtNum];
    std::cout << "allPt: ";
    for (int i = 0; i < allPtNum; ++i)
    {
      allPt[i] = i * M_PI;
      std::cout << std::setprecision(4) << allPt[i] << " ";
    }
    std::cout << std::endl;
  }

    rankPt = new double[sendrecvcount];
    std::cout << "PPProcess #" << rank << " sendrecvcount: " << sendrecvcount << std::endl;


  MPI_Scatter(allPt, sendrecvcount, MPI_DOUBLE, rankPt, sendrecvcount, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // if (rank != 0)
  // {
    std::cout << "Process #" << rank << "/" << size << " pts: ";
    for (int i = 0; i < sendrecvcount; ++i)
    {
      std::cout << std::setprecision(4) << rankPt[i] << " ";
      rankPt[i] *= 3.0;
    }
    std::cout << std::endl;
  // }

  MPI_Gather(rankPt, sendrecvcount, MPI_DOUBLE, allPt, sendrecvcount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  if (rank == 0)
  {
    std::cout << "Gathered allPt: ";
    for (int i = 0; i < allPtNum; ++i)
    {
      std::cout << std::setprecision(5) << allPt[i] << " ";
    }
    std::cout << std::endl;
  }

  delete[] rankPt;

  MPI_Finalize();
  return 0;
}
