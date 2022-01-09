/**
    Assignment 1: Processes
	Name: Amro Amanuddein
	StudentID: 1572498
	Course: CMPUT 379 Fall 2021
*/

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include "cmdexecuter.h"
#include <sys/resource.h>
#include <iomanip>
#include <fcntl.h>

using namespace std;


struct processTable{
	string job_pid;
	string job_state;
	int job_time;
	string command;	
};

vector<processTable> process_table;

void zombie_clear(){
	for (int i = 0; i < process_table.size(); i++){
		pid_t pid = stoi(process_table[i].job_pid);
		int status;
		// Process exits but does not terminate
		waitpid(pid, &status, WNOHANG);
		// Process terminates and now removed
		waitpid(pid, &status, WNOHANG);
		if (waitpid(pid, &status, WNOHANG) < 0 || waitpid(pid, &status, WNOHANG) == pid){
			process_table.erase(process_table.begin()+ i);
		}
	}
}

void execute(vector<string> cmd_vector, bool ampersand, bool greater, bool less){
	pid_t pid; 
	int status;
	pid = fork();
	// Join vector of strings to form string
	string joinedString = boost::algorithm::join(cmd_vector," ");
	if (ampersand){
		joinedString.append(" &");
	}
	processTable info = {
		to_string(pid),
		"R",
		0,
		joinedString
	};
	process_table.push_back(info);
	
	if (pid < 0){
		perror( "Could not fork!\n" );
		exit(1);
	}
	// Child process executes this
	else if (pid == 0){
		if (greater){
			string output_file;
			for (int i = 0; i < cmd_vector.size(); i++){
				if (cmd_vector[i].find('>') != string::npos){
					output_file = cmd_vector[i];
					output_file.erase(std::remove(output_file.begin(), output_file.end(), '>'),
					output_file.end());
				}
			}
			// Handling file output redirection
			freopen(output_file.c_str(), "a+", stdout);
		}
		if (less){
			string input_file;
			int file_to_read;
			for (int i = 0; i < cmd_vector.size(); i++){
				if (cmd_vector[i].find('<') != string::npos){
					input_file = cmd_vector[i];
					input_file.erase(std::remove(input_file.begin(), input_file.end(), '<'),
					input_file.end());
					// cmd_vector.erase(cmd_vector.begin()+i);
				}
			}
			// Handling file as input
			file_to_read = open(input_file.c_str(), O_RDONLY, 0777);
			dup2(file_to_read, STDIN_FILENO);
			close(file_to_read);
		}
		// Convert vector to array to pass into execvp
		char *argp[cmd_vector.size() + 1];
		int b = 0;
		for (int i = 0; i < cmd_vector.size(); i++){
			if ((cmd_vector[i].find('<') == string::npos) && (cmd_vector[i].find('>') == string::npos)){
				argp[b] = (char*) cmd_vector[i].c_str();
				b++;
			}
		}
		argp[b] = NULL;
		if( execvp(argp[0], argp ) < 0 )
			perror("Could not exec!\n");
			exit(1);
	}
	// Parent executes wait if ampersand is true
	else {
		if (!ampersand)
			waitpid(pid , &status, WUNTRACED);
  	
	}	
}

void exit(vector<string> cmd_vector){
	cout << "Waiting for programs to finish executing (if any)..." << endl;
	for (int i = 0; i < process_table.size(); i++){
		int status;
		pid_t pid = stoi(process_table[i].job_pid);
		waitpid(pid , &status, WUNTRACED);
	}
	cout << "Resources used: " << endl;
	struct rusage usage;
	getrusage(RUSAGE_CHILDREN, &usage);
	cout << "User Time =      " << usage.ru_utime.tv_sec << " seconds" << endl;
	cout << "Sys Time  =      " << usage.ru_stime.tv_sec << " seconds" << endl;
	exit(0);
}

void jobs(bool print_flag){
	zombie_clear();
	vector<vector<string>> pipe_output;
	pid_t pid = getpid();
	// 65536 is the line size
	char buf[65536];
	FILE *fp;
	string pipe_cmd = "ps -o pid:1,time:1,state:1| grep -v " + to_string(pid) + " | grep -v bash | grep -v ps";
	fp = popen(pipe_cmd.c_str(), "r");
	int j = 0;
	while (fgets(buf, 65536, fp) != NULL){
		vector<string> tmp;
		boost::split(tmp , buf, boost::is_any_of(" "));
		pipe_output.push_back(tmp);
		j++;
	}
	pclose(fp);
	for (int i = 1; i < pipe_output.size(); i++){
		for (int j = 0; j < process_table.size(); j++){
			if (pipe_output[i][0] == process_table[j].job_pid){
				vector<string> tmp;
				boost::split(tmp , pipe_output[i][1], boost::is_any_of(":"));
				// Get time in seconds from HH:MM:SS
				process_table[j].job_time = stoi(tmp[0]) * 3600 + stoi(tmp[1]) * 60 + stoi(tmp[2]);
				// Change state
				string state = pipe_output[i][2].substr(0,1);
				if (state.compare("T") == 0){
					process_table[j].job_state = "S";
				}
				else {
					process_table[j].job_state = "R";
				}
			}
		}

	}	

	if (print_flag){
		int counts = 0;
		cout << "Running Processes: " << endl;
		if (process_table.size() > 0){
			cout << "#       PID S SEC   COMMAND" << endl;
			for (int i = 0; i < process_table.size(); i++){
				cout 
					<< right << setw(0) << to_string(i)+":"
					<< right << setw(9) << process_table[i].job_pid
					<< right << setw(2) << process_table[i].job_state
					<< right << setw(3) << process_table[i].job_time
					<< right << setw(process_table[i].command.length()+4) << process_table[i].command
				<< endl;
				// We don't count suspended processes as active
				if (process_table[i].job_state != "S"){
					counts++;
				}
			}
		}	
		cout << "Processes =      " + to_string(counts) + " active" << endl;
		cout << "Completed processes: " << endl;
		struct rusage usage;
		getrusage(RUSAGE_CHILDREN, &usage);
		cout << "User Time =      " << usage.ru_utime.tv_sec << " seconds" << endl;
		cout << "Sys Time  =      " << usage.ru_stime.tv_sec << " seconds" << endl;
	}
}

void kill(vector<string> cmd_vector, bool ampersand){
	execute(cmd_vector, ampersand, false, false);

}

void resume(vector<string> cmd_vector, bool ampersand){
    execute(parse("kill -CONT "+ cmd_vector[1]).cmd_vector, ampersand, false, false);
}

void sleep(vector<string> cmd_vector, bool ampersand){
	execute(cmd_vector, ampersand, false, false);
}

void suspend(vector<string> cmd_vector, bool ampersand){
    execute(parse("kill -TSTP "+ cmd_vector[1]).cmd_vector, ampersand, false, false);
}

void wait(vector<string> cmd_vector){
    int status;
	try{
    	pid_t pid = stoi(cmd_vector[1]);
    	waitpid(pid , &status, WUNTRACED);
	}
	catch(...){
		cerr << "Please provide an integer for wait command" << endl;
		return;
	}
}

void execute_cmd(cmds cmd_input){
    if (cmd_input.cmd_vector[0] == "exit"){
        exit(cmd_input.cmd_vector);
    }
    else if (cmd_input.cmd_vector[0] == "jobs"){
		int save_cin, save_cout;
		if (cmd_input.greater){
			string output_file;
			for (int i = 0; i < cmd_input.cmd_vector.size(); i++){
				if (cmd_input.cmd_vector[i].find('>') != string::npos){
					output_file = cmd_input.cmd_vector[i];
					output_file.erase(std::remove(output_file.begin(), output_file.end(), '>'),
					output_file.end());
				}
			}
			// Handling file output redirection
			save_cout = dup(STDOUT_FILENO);
			freopen(output_file.c_str(), "a+", stdout);
		}
		if (cmd_input.less){
			string input_file;
			int file_to_read;
			for (int i = 0; i < cmd_input.cmd_vector.size(); i++){
				if (cmd_input.cmd_vector[i].find('<') != string::npos){
					input_file = cmd_input.cmd_vector[i];
					input_file.erase(std::remove(input_file.begin(), input_file.end(), '<'),
					input_file.end());
					// cmd_vector.erase(cmd_vector.begin()+i);
				}
			}
			// Handling file as input
			file_to_read = open(input_file.c_str(), O_RDONLY, 0777);
			save_cin = dup(STDIN_FILENO);
			dup2(file_to_read, STDIN_FILENO);
			close(file_to_read);
		}
        jobs(true);
		//int terminal = open(cin, O_RDWR, 0777);
		dup2(save_cin, STDIN_FILENO);
		dup2(save_cout, STDOUT_FILENO);
    }
    else if (cmd_input.cmd_vector[0] == "kill"){
		jobs(false);
        kill(cmd_input.cmd_vector, cmd_input.ampersand);
		jobs(false);

    }
    else if (cmd_input.cmd_vector[0] == "resume"){
        resume(cmd_input.cmd_vector, cmd_input.ampersand);
		jobs(false);
    }
    else if (cmd_input.cmd_vector[0] == "sleep"){
        sleep(cmd_input.cmd_vector, cmd_input.ampersand);
		jobs(false);
    }
    else if (cmd_input.cmd_vector[0] == "suspend"){
        suspend(cmd_input.cmd_vector, cmd_input.ampersand);
		jobs(false);
    }
    else if (cmd_input.cmd_vector[0] == "wait"){
        wait(cmd_input.cmd_vector);
		jobs(false);
    }
    else{
		jobs(false);
        execute(cmd_input.cmd_vector, cmd_input.ampersand, cmd_input.greater, cmd_input.less);
		//jobs(false);
    }
}