# Probing incoming communications
## TWO POINTS YOU SHOULD ALWAYS OBEY WHEN IT COMES TO MPI COMMUNICATIONS
### 1. Try to group as many data as possible in one communication. Sending N communications of M bytes will always be more costly than one communication of NxM bytes
### 2. Try to send the exact amount of data you are storing in your buffer and no more

但很多时候，接收的process并不知道数据的数量
It can happen sometimes that before a transfer, the receiving process has actually no idea of the quantity of data that is being transferred to it. In such a case, there are 3 solutions:
* Sending everything in two communications: first sending the quantity of data you are going to send (1 int), then another communication with the actual buffer of N elements. **This tends to go against rule #1**.
* Sending a nominal amount of data: always sending the maximum amount in the buffer. **That goes against rule #2**.
* **Probing** the message. Basically asking MPI to give you the size of the message

Probing serves many purposes, such as getting
* the count of elements we are about to receive
* the id and tags of the processes we are receiving from
* to know if we are actually receiving anything or not

Two functions used to probe:
* `MPI_Probe`: blocking call
  * `MPI_Probe` is only going to give you the `MPI_Status` associated with the next message received corresponding to a certain tag and id
  * It is possible to use `MPI_ANY_SOURCE` and `MPI_ANY_TAG` if you want to probe the reception of any kind of message or from any source.
* `MPI_IProbe`: non-blocking call
Then it's possible to use the resulting `MPI_Status` object in conjunction with other functions to get more information.




