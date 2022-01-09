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
Project #3: message-passing
-----

