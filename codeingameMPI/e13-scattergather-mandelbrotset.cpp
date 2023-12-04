#include <iostream>
#include <mpi.h>
#include <cmath>

// Global variables to store the rank of the process and the size
int rank, size;

// Number of points on one side. The total number of points
// will be pCount * pCount
constexpr int pCount = 512;

// Other global variables. We read them from the command line
// The cutoff variable indicates when we decide the series does not converge
// The other variable are just used to center the view and zoom level
int cutoff;
double minX, maxX, minY, maxY, dx, dy;

// The modulus of a complex number
double modulus(double x, double y)
{
  return sqrt(x * x + y * y);
}

// Multiplying a complex number by itself
void self_mul(double& x, double& y)
{
  double ox = x * x - y * y;
  double oy = x * y + y * x;
  x = ox;
  y = oy;
}

// Computation of the number of iterations on a set of points
// The result is stored in mset
void compute_mandelbrot(double* points, int npts, int mset[])
{
  // For each point
  for (int i = 0; i < npts; ++i)
  {
    double px, py;
    px = points[i * 2];
    py = points[i * 2 + 1];

    int iteration = 0;
    double zx = 0.0;
    double zy = 0.0;

    // We iterate until cutoff or modulus > 2
    while (iteration < cutoff)
    {
      self_mul(zx, zy);
      zx += px;
      zy += py;
      double mod = modulus(zx, zy);

      if (mod > 2.0f)
        break;
      
      iteration++;
    }

    // We store the number of iterations, and we use a special value (-1)
    // if we don't converge
    if (iteration == cutoff)
      mset[i] = -1;
    else
      mset[i] = iteration;
  }
}

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  // Reading the parameters on the command line
  minX = std::stod(argv[1]);
  maxX = std::stod(argv[2]);
  minY = std::stod(argv[3]);
  maxY = std::stod(argv[4]);
  dx = maxX - minX;
  dy = maxY - minY;
  cutoff = std::stoi(argv[5]);

  // Getting rank and size
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Initialization of the points:
  // The process with rank 0 will hold all the points
  // The others will keep the variable points as a null pointer
  MPI_Barrier(MPI_COMM_WORLD);
  double* points = NULL;

  points = new double[pCount * pCount * 2];
  for (int yp = 0; yp < pCount; ++yp)
  {
    double py = minY + dy * yp / pCount;
    for (int xp = 0; xp < pCount; ++xp)
    {
      double px = minX + dx * xp / pCount;

      int lid = yp * pCount * 2 + xp * 2;
      points[lid] = px;
      points[lid + 1] = py;
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

  // The number of points we hold
  int npts = pCount * pCount;

  // Computing the mandelbrot set... This function is already coded
  int mset[npts];
  compute_mandelbrot(points, npts, mset);
  MPI_Barrier(MPI_COMM_WORLD);

  // Printing only one result that will be used to create the image
  if (rank == 0)
  {
    for (int yp = 0; yp < pCount; ++yp)
    {
      for (int xp = 0; xp < pCount; ++xp)
      {
        std::cout << mset[yp * pCount + xp] << " ";
      }
      std::cout << std::endl;
    }
  }

  // Cleaning up the mess and exiting properly
  delete[] points;
  MPI_Finalize();
  return 0;
}
