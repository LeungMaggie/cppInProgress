# Communication modes

## Four communication modes
* There are actually 4 different communication modes you can trigger while communicating: **standard**, **buffered**, **synchronous** and **ready**
* Each of these mode has a corresponding `Send` function (the `Recv` function will always be the same)

### Standard mode (`MPI_Send`)
* It's actually a "non-mode" that lets the MPI implementation choose which communication mode is preferable. This might be heavily dependent on the implementation
* In OpenMPI, the observed behaviour is that for short messages, the send is automatically buffered, while for long messages, the message will be sent using a mode somewhat close to the synchronous mode
* In this case: 发送1->等8秒->接收1->等8秒->发送2&接收2(几乎同时)，因为`发送1`係short message, -->`MPI_Bsend`, `发送2`係long message-->`MPI_Ssend`
```
Time elapsed to complete blocking send 1: 3.02e-05 s
(经过8秒...)
Just received buffer 1
(经过8秒...)
Time elapsed to complete blocking send 2 : 8.00028 s
Just received buffer 2
```

### Buffered mode (`MPI_Bsend`)
* Stores all the data to be sent in a temporary buffer and returns to the execution, just as a non-blocking send would do.
* The advantage is that execution continues immediately even if the corresponding blocking `Recv` has not been called yet
* On the other hand, buffered mode copies all the data of your buffer into another region of memory, duplicating the data. This might be dangerous memory wise if you're transferring large amounts of data
```
Time elapsed to complete blocking send 1: 3.351e-05 s
Just received buffer 1
[TF-Lattice:2383749] *** An error occurred in MPI_Bsend
[TF-Lattice:2383749] *** reported by process [3592617985,0]
[TF-Lattice:2383749] *** on communicator MPI_COMM_WORLD
[TF-Lattice:2383749] *** MPI_ERR_BUFFER: invalid buffer pointer
[TF-Lattice:2383749] *** MPI_ERRORS_ARE_FATAL (processes in this communicator will now abort,
[TF-Lattice:2383749] ***    and potentially your MPI job)
```

### Ready mode (`MPI_Rsend`)
* Can start only if the corresponding receive has already been called.
* This allows your program to gain time from some additional overhead in the initialization of messages.
* 发送1->等8秒->接收1->等8秒->发送2&接收2(几乎同时)
```
Time elapsed to complete blocking send 1: 5.396e-05 s
(经过8秒...)
Just received buffer 1
(经过8秒...)
Time elapsed to complete blocking send 2 : 8.00026 s
Just received buffer 2
```

### Synchronous mode (`MPI_Ssend`)
* Will wait for the corresponding `Recv` to complete
* The data transfer will occur at that exact moment, ensuring that both processes are ready for transfer
```
Time elapsed to complete blocking send 1: 8.00026 s
Just received buffer 1
Time elapsed to complete blocking send 2 : 8.00039 s
Just received buffer 2
```
