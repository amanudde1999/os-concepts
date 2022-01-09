# os-concepts

**Description:** Projects that I completed in the CMPUT 379: Operating Systems Concepts course at the University of Alberta.

Project #1: shell
-----
**Description:** This project involved building a command-line interface shell interface (CLI). It accepts simple commands that the program executes such as:
 - *exit* - ends the execution of the shell and waits until all the processes initiated by the shell are complete, then it prints out the total user and system time for all the processes that were run by the shell
 - *jobs* - displays the status of all running processes that were spawned by the shell
 - *kill &lt;int&gt;* - kills the process with pid &lt;int&gt;
 - *resume &lt;int&gt;* - resumes the execution of the process with pid &lt;int&gt; (undoes suspend)
 - *suspend &lt;int&gt;* - suspends the execution of process with pid &lt;int&gt;. (resume awakens this process)
 - *wait &lt;int&gt;* - waits until the process with pid &lt;int&gt; has finished executing
 - *sleep &lt;int&gt;* - sleeps for &lt;int&gt; seconds
 
 If none of the above special commands were entered, then the input to be executed by the shell is of the form: 
 - *&lt;cmd&gt; &lt;arg&gt;* *: here, a process is spawned to execute command &lt;cmd&gt; with 0 or more arguements &lt;arg&gt;.
 
 Special Arguements that any command can have:
 
 - *&*: if used, this must be the last arguement and indicates that a command is to be executed in the background
 - *<fname*: file name to be used for program input
 - *>fname*: file name to be used for program output
 
 
Project #2: producer-consumer
-----
**Description**: This project involves solving the multi-process synchronization producer/consumer problem using thread programming, shared memory, and synchronization. The producer (one thread) generates data and puts it into a shared buffer while at the same time multiple consumers (multiple threads) are consuming the data. To synchronize access to shared data, semaphores (both binary and counting) were used.

Project #3: message-passing
-----
**Description**: This project involves implementing a client-server architecture. Here, the client recieves transactions that they are to pass to the server to process, the server then responds to the client with an indication on how the sent transaction. The server in this project had a specified backlog of 10 clients.



