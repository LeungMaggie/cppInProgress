
# Non-blocking communications

## Non-blocking send & receive
### Non-blocking send
The command when you want to send data in a non-blocking way is called `MPI_Isend`, defined as:
```cpp
int MPI_Isend(void* buffer, int count, MPI_Datatype datatype, int dest, int tag, MPI_Communicator comm, MPI_Request* request);
```
* Rappel blocking send:
  ```cpp
  int MPI_Send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
  ```

* `MPI_Request`: 
`MPI_Isend` is preparing a request. This request is going to be executed when both processes are ready to synchronize.
This command only sets up the send, but actually does not transfer anything to the destination process, only prepares it.
Once this request has been prepared, it is necessary to complete it: there are 2 ways of completing a request: **wait** & **test**

### Non-blocking receive
The non-blocking call to receive:
```cpp
int MPI_Irecv(void* buffer, int count, MPI_Datatype datatype, int source, int tag, MPI_Communicator comm, MPI_Request* request)
```

### Waiting
Waiting forces the process to go in "blocking mode". The sending process will simply wait for the request to finish
If your process waits right after `MPI_Isend`, the send is the same as calling `MPI_Send`
2 ways to wait:
```cpp
int MPI_Wait(MPI_Request* request, MPI_Status* status);
```
waits for the completion of the given request. As soon as the request is complete, an instance of `MPI_Status` is returned in `status`
```cpp
int MPI_Waitany(int count, MPI_Request array_of_requests[], int* index, MPI_Status* status);
```
waits for the first completed request in an array of requests to continue. As soon as a request completes, the value of index is set 
to store the completed request of array_of_requests. the call also stores the status of the completed request.

### Testing
Waiting blocks the process until the requests (or a request) is fulfilled, while Testing checks if the request can be completed.
If it can, the request is automatically completed and the data transferred. As for wait, there are two waits of testing:
```cpp
int MPI_Test(MPI_Request* request, int* flag, MPI_Status* status);
```
As for `MPI_Wait`, the parameters `request` and `status` hold no mystery. Now remember that testing is non-blocking, so in any case
the process continues execution after the call. The variable `flag` is there to tell if the request was completed during the test or not.
if `flag != 0` that means the request has been completed.
```cpp
int MPI_Testany(int count, MPI_Request array_of_requests[], int* index, int* flag, MPI_Status* status);
```
Also, `MPI_Testany` sets `flag` to a non-zero value if any request is fulfillable. If so, `status` and `index` are also given a value.

#### **BE CAREFUL WHEN USING `MPI_Test` TO ALWAYS ADD A `MPI_Wait` IN CASE THE REQUEST HAS NOT BEEN COMPLETING EVEN THOUGH YOUR WORK IS FINISHED**

### Example
* Simple example to using `MPI_Isend`, `MPI_Irecv`, `MPI_Wait`, `MPI_Test`
* The data is stored in a variable called `buffer`, defined as an array of `int` of size `bufferCount`
* 2 processes, waiting and testing only one request

```cpp
MPI_Request request;
MPI_Status status;
int requestComplete = 0;

// Rank 0 sends, rank 1 receives
if (rank == 0)
{
  MPI_Isend(buffer, bufferCount, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
  // Here we do some work whild waiting for process 1 to be ready
  while (hasWork)
  {
    do_work();

    // We only test if the request is not already fulfilled
    if (!requestComplete)
      MPI_Test(&request, &requestComplete, &status);
  }

  // No more work, we wait for the request to be complete if it's not the case
  if (!requestComplete)
    MPI_Wait(&request, &status);
}
else
{
  MPI_Irecv(buffer, bufferCount, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

  // Here we just wait for the message to come
  MPI_Wait(&request, &status);
}
```

### `MPI_Barrier`
This command forces all the processes in a certain communicator (`MPI_COMM_WORLD` in this case) to wait for each other. So the processes in the communicator are going to pause until every single one has reached a barrier (not necessarily the same one). Then, execution resumes. This is a way of **hard synchronising the different processes**.

## TWO POINTS YOU SHOULD ALWAYS OBEY WHEN IT COMES TO MPI COMMUNICATIONS: (Section 10/30)
### Try to group as many data as possible in one communication. Sending N communications of M bytes will always be more costly than one communication of NxM bytes
### Try to send the exact amount of data you are storing in your buffer and no more
