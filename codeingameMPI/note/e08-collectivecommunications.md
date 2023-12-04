# Collective communications
Collective communications allow us to exchange more easily information across all processes (of a communicator)

## Types of collective communications
### Broadcast
* One process sends a message to every other process
* ```cpp
  int MPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);
  // non-blocking version
  int MPI_Ibcast(void* buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm, MPI_Request request);
  ```
* Broadcast is a non-blocking operation, so processes continue running as soon as the information has been sent/received
* After the operation, all process will have a copy of the data from the **root** process in their buffers
* There are various algorithms for broadcasting. Implementations such as OpenMPI have **decision algorithms** running on top of your calls, that choose which algorithm to apply depending on your bandwidth, quantity of data and process
* Advantages compared to a sequence of send:
  * elegance of not having an `if`-statement or having to pick between Send & Recv operations
  * a Broadcast is a lot more effective than a list of send/receives

### Reduction
* One process gets data from all the other processes and applies an operation on it (sum, minimum, maximum, etc.)
* ```cpp
  int MPI_Reduce(void* send_data, void* recv_data, int count, MPI_Datatype type, MPI_Op op, int root, MPI_Comm communicator);
  ```
* Operations:

| operator  |  operation |
| :-------: | :--------: |
| MPI_MAX   | maximum value |
| MPI_MIN   | minimum value |
| MPI_SUM   | sum           |
| MPI_PROD  | product       |
| MPI_LAND  | logical and   |
| MPI_BAND  | bit-wise and  |
| MPI_LOR   | logical or    |
| MPI_BOR   | bit-wise or   |
| MPI_LXOR  | logical xor   |
| MPI_BXOR  | bit-wise xor  |
| MPI_MAXLOC| max value and location |
| MPI_MINLOC| min value and location |
* Operations on arrays are **element-wise**
* Reducing on all processes
  * The example (*e10-reductionEx.cpp*) shows us `MPI_Reduce` in which the reduction operation takes place on only one process (in this case process 0). The reception buffer (`result`) is only valid for process 0. If you want to have the result of the reduction stored on all processes, you can use `MPI_Allreduce`.
* DO NOT use the same buffer for sending and receiving in a reduction

### Scatter
* A single process partitions the data to send pieces to every other process
* ```cpp
  int MPI_Scatter(void* sendbuf, int sendcount, MPI_Datatype sendtype,
                  void* recvbuf, int recvcount, MPI_Datatype recvtype,
                  int root, MPI_Comm comm);
  // non-blocking version
  int MPI_Iscatter(void* sendbuf, int sendcount, MPI_Datatype sendtype,
                  void* recvbuf, int recvcount, MPI_Datatype recvtype,
                  int root, MPI_Comm comm, MPI_Request* request);
  ```
* The scattering 
* The two different parameters for sending count and receiving count are to account for different data types ??


### Gather
* A single process assembles the data from different process in a buffer
* ```cpp
  int MPI_Gather(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
                 void* recvbuf,       int recvcount, MPI_Datatype recvtype,
                 int root, MPI_Comm comm);
  // non-blocking version
  int MPI_Igather(const void* sendbuf, int sendcount, MPI_Datatype sendtype,
                  void* recvbuf,       int recvcount, MPI_Datatype recvtype,
                  int root, MPI_Comm comm, MPI_Request* request);
  ```

## Blocking or non-blocking
* Just need to add the `I` to switch to non-blocking mode (e.g. `MPI_Bcast` -> `MPI_Ibcast`)
* Non-blocking global requires the use of `MPI_Wait` and `MPI_Test` to be completed correctly


