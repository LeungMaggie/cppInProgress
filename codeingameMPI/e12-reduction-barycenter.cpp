#include <iostream>
#include <cmath>
#include <mpi.h>

/**
 * use MPI_Allreduce to compute a reduction on all processes and use the given result
 * * int MPI_Allreduce(void* sendbuf, void* recvbuf,
 * *                   int count, MPI_Datatype datatype,
 * *                   MPI_Op op, MPI_Comm comm);
 * 
 * The following problem: we have a list of N points in three dimensions (3 coordinates)
 * We want to compute the distance of each point to the barycenter of the set
 * We will use M processes in parallel having N/M points each.
 * Algorithm:
 * * each process will compute the sum of all of its own points (sum every coordinate)
 * * the program will then call the reduction to get the sum of all the points on all processes
 * * then the barycenter position is given by dividing this sum by the number of points
 * * finally, every process will compute the distance of each point to the barycenter, and print the result on stdout
*/

void compute(int totalCount, int myCount, float myPoints[][3], int rank)
{
  // totalCount is the total number of points
  // myCount is the number of points for this process
  // myPoints is a float table of shape [myCount][3]

  // 1- Sum over all the points in localSum
  float localSum[3] = {0.0f, 0.0f, 0.0f};
  for (int i = 0; i < myCount; ++i)
  {
    localSum[0] += myPoints[i][0];
    localSum[1] += myPoints[i][1];
    localSum[2] += myPoints[i][2];
  }

  // 2- Reduce the sum of all the points on the variable barycenter
  float barycenter[3];
  MPI_Allreduce(localSum, barycenter, myCount, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

  // 3- Divide every component of the barycenter by the number of points
  for (int i = 0; i < 3; ++i)
    barycenter[i] /= totalCount;

  // For every point
  for (int i = 0; i < myCount; ++i)
  {
    float dist = 0.0f;

    // 4- Compute the distance for every point
    dist = sqrt((myPoints[i][0] - barycenter[0]) * (myPoints[i][0] - barycenter[0]) +
                (myPoints[i][1] - barycenter[1]) * (myPoints[i][1] - barycenter[1]) +
                (myPoints[i][2] - barycenter[2]) * (myPoints[i][2] - barycenter[2]));

    // And printing the result
    std::cout << "Process #" << rank << " dist: " << dist << std::endl;
  }
}

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int totalCount = 20;
  int myCount = int(floor(20 / size));

  // 生成一些点，分配到各个rank
  compute(totalCount, myCount, myPoints, rank);


  MPI_Finalize();
  return 0;
}
