# simpleftp
a simple file transfer protocol

# what?
simpleftp is a file transfer protocol, implemented here as basic client and server programs.

# why?
These programs are deliverables of an assignment in my Data Communication Networks class. 

I'll be keeping this repository private for a while (i.e. well after the assignment's due date) so I don't get absolutely dunked on by the Honor Council (again).

# how?
Paraphased from the spec:

1. Client and server handshake using TCP over UNIX sockets.

2. Transaction (client -> server) using UDP over UNIX sockets, with data transfer occurring in 4 character chunks.

3. Server writes received data to file.

# usage
The client and server programs are invoked from the command line and accept arguments.

## server
```./server $PORT```

## client
```./client $SERVER_IP $PORT $DATA_FILE```

## example
On the server, hosted at 192.168.0.5:

    ./server 6003

On the client:

    ./client 192.168.0.5 6003 file.txt
