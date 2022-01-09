In the file, we would like to see the following:

This submission includes:
- prodcon.cpp -> this file contains the producer and consumer functions and thread-handling functions (creation/joining)
- helper.cpp -> this file contains all the helper functions for prodcon.cpp.
- tands.cpp -> contains the two functions executed in this assignment.
- helper.h -> header file for helper.cpp
- Makefile 
- README.txt

Assumptions:
- The only assumption in this program is that when entering S<int> and T<int> commands via the terminal, you are expected to use
CTRL-D to terminate the program.

Approach:
The way this assignment was approached was to intitially read the assignment and brainstorm a little on how to approach it.
Most importantly going through the example log file line by line. After brain storming, 
I realised a strong understanding of semaphores and threads and how they're utilized.
This was done by reading the lecture slides and going over the professor's code. 

Instructions:

Begin by running: 
                            make all

To run the program, there are two methods:

Method 1:                   ./prodcon nthreads <id> < <file-name>

Method 1 will take in a file name as input and the file's content consists of T<int> and S<int> commands that will execute the Trans()
and Sleep() functions.

Method 2:
                            ./prodcon nthreads <id>

Method 2 will take in T<int>, S<int> from the command line and you must use CTRL+D to terminate the program after entering all the commands.

Regardless of the method used here:
- nthreads -> the number of consumer threads you wish to create to execute the command(s)
- id -> an optional variable that is used for the naming of the output log file, if an id is provided (that is no 0), the log file will be called 
prodcon.<id>.log otherwise it will be called prodcon.log .   