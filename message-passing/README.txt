/**
    Assignment 3: Client/Server
	Name: Amro Amanuddein
	StudentID: 1572498
	Course: CMPUT 379 Fall 2021
*/


This submission includes:
- server.cpp -> this file contains all server-related functionality to communicate with the many clients
- client.cpp -> this file contains all client-related functionality to send transactions to be completed by the server and recieve done IDs.
- tands.cpp  -> contains the two functions executed in this assignment.
- helper.h   -> header file
- server.man -> MAN pages for server
- client.man -> MAN pages for client
- Makefile 
- README.txt

Assumptions:
- The only assumption in this program is that when entering S<int> and T<int> commands via the terminal from the client-side, 
you are expected to use CTRL-D to terminate the program. Moreover, there will be no more than 10 clients connected to the server simultaenously.

Approach:
The way this assignment was approached was to intitially read the assignment and brainstorm a little on how to approach it.
Most importantly going through the example log file line by line. After brain storming, 
I realised a strong understanding client-server archietecture is needed.
This was done by reading the lecture slides, going over the professor's code and reading the linux man pages for the communication functions. 

Instructions:

Begin by running: 
                            make all

To run the client-side program, there are two methods:

Method 1:                   ./client <port_number> <ip_address> < <file-name>

Method 1 will take in a file name as input and the file's content consists of T<int> and S<int> commands that will execute the Trans()
and Sleep() functions.

Method 2:
                            ./client <port_number> <ip_address>

Method 2 will take in T<int>, S<int> from the command line and you must use CTRL+D to terminate the program after entering all the commands.

Regardless of the method used here:
- port_number -> is the port number that the server is using and you wish to connect to.
- ip_address  -> is the IP address that the server is running on that you wish to connect to.

To run the server-side program:

                            ./server <port_number>
- port_number -> the port number which you want to establish the server on, this will be the port number clients use to establish a connection with the server.